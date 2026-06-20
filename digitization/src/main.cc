#include "TFile.h"
#include "TTree.h"
#include "config.hh"
#include "utils.hpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDFHelpers.hxx>
#include <TRandom3.h>
#include <iostream>

struct CellKey {
  int eventID;
  int layerID;
  int cellX;
  int cellY;
  int cellZ;

  bool operator==(const CellKey &other) const {
    return eventID == other.eventID && layerID == other.layerID &&
           cellX == other.cellX && cellY == other.cellY && cellZ == other.cellZ;
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
    combine(k.eventID);
    return h;
  }
};
} // namespace std

struct HitData {
  int eventID;
  int layerID;
  int cellX;
  int cellY;
  int cellZ;
  double edep;
  double time;
};

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

  std::cout << "Building discrete adressses" << std::endl;

  auto mapped_df =
      df.Define("Cell_X",
                [=](int layerID, double x) -> int {
                  double pitch = (layerID < ecal_layer_start) ? pixel_pitch_x_mm
                                 : (layerID < hcal_layer_start)
                                     ? ecal_cell_size_mm
                                     : hcal_cell_size_mm;
                  return static_cast<int>(std::floor(x / pitch));
                },
                {"LayerID", "X"})
          .Define("Cell_Y",
                  [=](int layerID, double y) -> int {
                    double pitch =
                        (layerID < ecal_layer_start)   ? pixel_pitch_y_mm
                        : (layerID < hcal_layer_start) ? ecal_cell_size_mm
                                                       : hcal_cell_size_mm;
                    return static_cast<int>(std::floor(y / pitch));
                  },
                  {"LayerID", "Y"})
          .Define("Cell_Z",
                  [=](int layerID, double z) -> int {
                    double pitch =
                        (layerID < ecal_layer_start)   ? pixel_pitch_z_mm
                        : (layerID < hcal_layer_start) ? ecal_cell_size_mm
                                                       : hcal_cell_size_mm;
                    return static_cast<int>(std::floor(z / pitch));
                  },
                  {"LayerID", "Z"})
          .Define("Hit",
                  [=](int eventID, int layerID, int x, int y, int z, double e,
                      double t) {
                    return HitData{eventID, layerID, x, y, z, e, t};
                  },
                  {"EventID", "LayerID", "Cell_X", "Cell_Y", "Cell_Z", "Edep",
                   "Time"});

  // Summing energies per cell
  std::cout << "Aggregating energy deposits per cell" << std::endl;

  struct CellAggregate {
    double total_edep = 0.0;
    double min_time = 1e9;
  };
  using CellMap = std::unordered_map<CellKey, CellAggregate>;

  auto aggregator = [](CellMap &acc, const HitData &hit) {
    CellKey key{hit.eventID, hit.layerID, hit.cellX, hit.cellY, hit.cellZ};

    acc[key].total_edep += hit.edep;

    if (hit.time < acc[key].min_time) {
      acc[key].min_time = hit.time;
    }
  };

  auto merger = [](CellMap &target, const CellMap &source) {
    for (const auto &[key, src_agg] : source) {
      auto &tgt_agg = target[key];
      tgt_agg.total_edep += src_agg.total_edep;
      if (src_agg.min_time < tgt_agg.min_time) {
        tgt_agg.min_time = src_agg.min_time;
      }
    }

    return target;
  };

  auto cell_energy_map_ptr =
      mapped_df.Aggregate(aggregator, merger, "Hit", CellMap{});

  const auto &cell_energy_map = *cell_energy_map_ptr;

  TFile out_file(output_file.c_str(), "RECREATE");
  TTree out_tree("Digits", "Digitized Detector Readout");

  int out_layerID, out_cellX, out_cellY, out_cellZ, out_adc;
  double out_time;

  out_tree.Branch("EventID", &out_layerID, "EventID/I");
  out_tree.Branch("LayerID", &out_layerID, "LayerID/I");
  out_tree.Branch("Cell_X", &out_cellX, "Cell_X/I");
  out_tree.Branch("Cell_Y", &out_cellY, "Cell_Y/I");
  out_tree.Branch("Cell_Z", &out_cellZ, "Cell_Z/I");
  out_tree.Branch("ADC", &out_adc, "ADC/I");
  out_tree.Branch("Time", &out_time, "Time/D");

  TRandom3 gRand(4357);

  std::cout << "Adding noise" << std::endl;

  // For progress
  size_t total_cells = cell_energy_map.size();
  size_t current_cell = 0;

  for (const auto &[cell, agg] : cell_energy_map) {
    double e_visible = agg.total_edep;

    hepp::showProgressBar(current_cell++, total_cells);

    // Gaussian noise too all layers
    double e_smeared = e_visible;
    if (cell.layerID < ecal_layer_start) {
      e_smeared += gRand.Gaus(0.0, tracker_noise_MeV);
    } else if (cell.layerID < hcal_layer_start) {
      e_smeared += gRand.Gaus(0.0, ecal_noise_MeV);
    } else {
      e_smeared += gRand.Gaus(0.0, hcal_noise_MeV);
    }

    // Discretization on all layers
    int adc_val = 0;
    if (cell.layerID < ecal_layer_start) {
      adc_val = static_cast<int>(e_smeared * tracker_gain_per_MeV);
    } else if (cell.layerID < hcal_layer_start) {
      adc_val = static_cast<int>(e_smeared * ecal_gain_per_MeV);
    } else {
      adc_val = static_cast<int>(e_smeared * hcal_gain_per_MeV);
    }

    if (adc_val < 0)
      adc_val = 0;
    if (adc_val > 4095)
      adc_val = 4095;

    // 3σ
    int tracker_cut =
        static_cast<int>(3.0 * tracker_noise_MeV * tracker_gain_per_MeV);
    int ecal_cut = static_cast<int>(3.0 * ecal_noise_MeV * ecal_gain_per_MeV);
    int hcal_cut = static_cast<int>(3.0 * hcal_noise_MeV * hcal_gain_per_MeV);

    // Why those values?
    if (cell.layerID < ecal_layer_start && adc_val <= tracker_cut)
      continue;
    if (cell.layerID >= ecal_layer_start && cell.layerID < hcal_layer_start &&
        adc_val <= ecal_cut)
      continue;
    if (cell.layerID >= hcal_layer_start && adc_val <= hcal_cut)
      continue;

    // Output
    out_layerID = cell.layerID;
    out_cellX = cell.cellX;
    out_cellY = cell.cellY;
    out_cellZ = cell.cellZ;
    out_adc = adc_val;
    out_time = agg.min_time;

    out_tree.Fill();
  }

  std::cout << "Writing output file at " << output_file.c_str() << std::endl;

  out_tree.Write();
  out_file.Purge();
  out_file.Close();

  return 0;
}
