#include "TFile.h"
#include "TTree.h"
#include "config.hh"
#include "utils.hpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDFHelpers.hxx>
#include <TRandom3.h>
#include <iostream>

struct CellKey {
  int layerID;
  int cellX;
  int cellY;
  int cellZ;

  bool operator==(const CellKey &other) const {
    return layerID == other.layerID && cellX == other.cellX &&
           cellY == other.cellY && cellZ == other.cellZ;
  }
};

namespace std {
template <> struct hash<CellKey> {
  size_t operator()(const CellKey &k) const {
    size_t h = 0;
    auto combine = [&h](int val) {
      h ^= std::hash<int>{}(val) + 0x9e3779b9 + (h << 6) + (h >> 2); // cool
    };
    combine(k.layerID);
    combine(k.cellX);
    combine(k.cellY);
    combine(k.cellZ);
    return h;
  }
};
} // namespace std

// struct HitData {
//   int eventID;
//   int layerID;
//   int cellX;
//   int cellY;
//   int cellZ;
//   double edep;
//   double time;
// };

int main(int argc, char **argv) {
  auto &config = *hepp::Config::GetConfig();
  int threadCount = config["threads"].value_or(1);

  const double pixel_pitch_x_mm = 0.05; // 50 micrometers silicon pitch
  const double pixel_pitch_y_mm = 0.40; // 400 micrometers silicon pitch
  const double pixel_pitch_z_mm = 0.40; // 400 micrometers silicon pitch

  const double ecal_cell_size_mm = 20.0; // 20.0 mm (2.0 cm readout matrix grid)
  const double hcal_cell_size_mm =
      100.0; // 100.0 mm (10.0 cm readout matrix grid)

  const double tracker_gain_per_MeV = 100'000.0;
  const double ecal_gain_per_MeV =
      4095.0 / 1'500'000.0; // Expected large values of 1.5 TeV in ECAL
  const double hcal_gain_per_MeV =
      4095.0 / 3'000'000.0; // Expected large values of 3 TeV

  const double tracker_noise_MeV =
      0.005; // 5 keV intrinsic silicon electronic noise
  const double ecal_noise_MeV =
      30.0; // 30 MeV front-end electronics noise floor
  const double hcal_noise_MeV =
      50.0; // 50 MeV front-end electronics noise floor

  // NOTE: Below is without taking into account the fact layer id are 2i for
  // active layers and 2i+1 for passive layer const int tracker_layer_count =
  // 10; const int ecal_layer_count = 60; const int hcal_layer_start =
  // tracker_layer_count + ecal_layer_count;
  const int tracker_start = 1;
  const int ecal_layer_start = 21;
  const int hcal_layer_start = 141;

  ROOT::EnableImplicitMT();

  std::string input_file = "data/detector_simulation.root";
  std::string output_file = "data/digits.root";

  std::cout << "Reading from: " << input_file << std::endl;

  ROOT::RDataFrame df("Steps", input_file);
  ROOT::RDF::Experimental::AddProgressBar(df);

  using DigitReturnTuple =
      std::tuple<ROOT::RVec<int>, ROOT::RVec<int>, ROOT::RVec<int>,
                 ROOT::RVec<int>, ROOT::RVec<int>, ROOT::RVec<double>>;

  auto digitized_df =
      df.Define(
            "DigitizedOutputs",
            [=](int eventID, const ROOT::RVec<int> &layers,
                const ROOT::RVec<double> &xs, const ROOT::RVec<double> &ys,
                const ROOT::RVec<double> &zs, const ROOT::RVec<double> &edeps,
                const ROOT::RVec<double> &times) {
              // Key: {layerID, cellX, cellY, cellZ} -> Value: {total_edep,
              // min_time}
              std::map<std::tuple<int, int, int, int>,
                       std::pair<double, double>>
                  cells;

              for (size_t i = 0; i < layers.size(); ++i) {
                int layerID = layers[i];

                double pitch_x = (layerID < ecal_layer_start) ? pixel_pitch_x_mm
                                 : (layerID < hcal_layer_start)
                                     ? ecal_cell_size_mm
                                     : hcal_cell_size_mm;
                double pitch_y = (layerID < ecal_layer_start) ? pixel_pitch_y_mm
                                 : (layerID < hcal_layer_start)
                                     ? ecal_cell_size_mm
                                     : hcal_cell_size_mm;
                double pitch_z = (layerID < ecal_layer_start) ? pixel_pitch_z_mm
                                 : (layerID < hcal_layer_start)
                                     ? ecal_cell_size_mm
                                     : hcal_cell_size_mm;

                int cx = static_cast<int>(std::floor(xs[i] / pitch_x));
                int cy = static_cast<int>(std::floor(ys[i] / pitch_y));
                int cz = static_cast<int>(std::floor(zs[i] / pitch_z));

                auto cell = std::make_tuple(layerID, cx, cy, cz);
                auto &[total_edep, min_time] = cells[cell];

                // Initialize tracking for the earliest hit time
                if (total_edep == 0.0) {
                  min_time = 1e9;
                }

                total_edep += edeps[i];
                if (times[i] < min_time) {
                  min_time = times[i];
                }
              }

              ROOT::RVec<int> out_layerID, out_cellX, out_cellY, out_cellZ,
                  out_adc;
              ROOT::RVec<double> out_time;

              std::cout << "Adding noise" << std::endl;

              // what??
              static thread_local TRandom3 gRand(4357 +
                                                 ROOT::IsImplicitMTEnabled());

              size_t total_cells = cells.size();
              size_t current_cell = 0;

              for (auto &[key, value] : cells) {
                auto [layerID, cellX, cellY, cellZ] = key;
                auto [total_edep, min_time] = value;
                if (current_cell++ % 1000 == 0) {
                  hepp::showProgressBar(current_cell, total_cells);
                }

                // Gaussian noise to all layers
                double e_smeared = total_edep;
                double noise_floor = 0.0;

                if (layerID < ecal_layer_start) {
                  e_smeared += gRand.Gaus(0.0, tracker_noise_MeV);
                  noise_floor = 3.0 * tracker_noise_MeV;
                } else if (layerID < hcal_layer_start) {
                  e_smeared += gRand.Gaus(0.0, ecal_noise_MeV);
                  noise_floor = 3.0 * ecal_noise_MeV;
                } else {
                  e_smeared += gRand.Gaus(0.0, hcal_noise_MeV);
                  noise_floor = 3.0 * hcal_noise_MeV;
                }

                // TODO: Remove hardcoded 3
                // 3σ noise cut
                if (e_smeared <= noise_floor) {
                  continue;
                }

                // Discretization on all layers
                int adc_val = 0;
                if (layerID < ecal_layer_start)
                  adc_val = static_cast<int>(e_smeared * tracker_gain_per_MeV);
                else if (layerID < hcal_layer_start)
                  adc_val = static_cast<int>(e_smeared * ecal_gain_per_MeV);
                else
                  adc_val = static_cast<int>(e_smeared * hcal_gain_per_MeV);

                if (adc_val < 0)
                  adc_val = 0;
                if (adc_val > 4095)
                  adc_val = 4095;

                out_layerID.push_back(layerID);
                out_cellX.push_back(cellX);
                out_cellY.push_back(cellY);
                out_cellZ.push_back(cellZ);
                out_adc.push_back(adc_val);
                out_time.push_back(min_time);
              }
              return std::make_tuple(out_layerID, out_cellX, out_cellY,
                                     out_cellZ, out_adc, out_time);
            },
            {"EventID", "LayerID", "X", "Y", "Z", "Edep", "Time"})
          .Redefine("LayerID",
                    [](const DigitReturnTuple &t) { return std::get<0>(t); },
                    {"DigitizedOutputs"})
          .Define("Cell_X",
                  [](const DigitReturnTuple &t) { return std::get<1>(t); },
                  {"DigitizedOutputs"})
          .Define("Cell_Y",
                  [](const DigitReturnTuple &t) { return std::get<2>(t); },
                  {"DigitizedOutputs"})
          .Define("Cell_Z",
                  [](const DigitReturnTuple &t) { return std::get<3>(t); },
                  {"DigitizedOutputs"})
          .Define("ADC",
                  [](const DigitReturnTuple &t) { return std::get<4>(t); },
                  {"DigitizedOutputs"})
          .Redefine("Time",
                    [](const DigitReturnTuple &t) { return std::get<5>(t); },
                    {"DigitizedOutputs"});

  std::cout << "Writing output file at " << output_file.c_str() << std::endl;

  digitized_df.Snapshot(
      "Digits", output_file,
      {"EventID", "LayerID", "Cell_X", "Cell_Y", "Cell_Z", "ADC", "Time"});
  return 0;
}
