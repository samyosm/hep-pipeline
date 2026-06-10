# High Energy Physics Pipeline

![Build Status](https://github.com/samyosm/hep-pipeline/actions/workflows/build.yml/badge.svg)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.19960730.svg)](https://doi.org/10.5281/zenodo.19960730)

## Overview

This repository contains a complete, end-to-end implementation of a High Energy Physics (HEP) analysis pipeline, inspired by workflows used at CERN.

The goal of this project is to gain practical, hands-on experience with every stage of the pipeline while reproducing known physics results.

## Datasets

The simulation results for this project are published on Zenodo. This dataset provides set of simulated proton-proton events at 13.6 TeV (LHC Run 3 conditions).

**Citation:**
> Rahmani, S. (2026). HepMC3 Events: Hard QCD with pT greater than 20 GeV in pp Collisions at 13.6 TeV. Zenodo. [https://doi.org/10.5281/zenodo.19960730](https://doi.org/10.5281/zenodo.19960730)

## Pipeline Structure

- [x] 1. Event generation
- [x] 2. Detector simulation
- [ ] 3. Digitization
- [ ] 4. Reconstruction
- [ ] 5. Analysis
- [ ] 6. Statistical interpretation

## Usage

Conda (Miniforge) is used to ensure easy replicability. It is the only requisite for this project after cmake.

Begin by cloning this repository:

```sh
git clone https://github.com/samyosm/hep-pipeline
```

Create a conda environment and activate:
```sh
mamba env create -f environment.yml
mamba activate hep-pipeline
```

Build using cmake:

```sh
cmake -B build -S .
cmake --build build
```

You are now ready to use the different components.

### Event generation

Run the following command to generate `data/events.hepmc`:

```sh
mkdir data
./build/event-generation/gen_events
```

Note: It generates 100,000 events so may produce files up to 23GB in size and take some time. The relevent data can also be downloaded from the published dataset.

### Detector simulation

Run the following command to generate various root files (1 for each core) in `data/`:
```sh
./build/detector-simulation/simulate_detection resources/macros/prod.mac
```
## Results

The following energy deposition maps show the detector response in the transverse plane (XY) for different particle species. The maps use logarithmic energy scaling. The simulation was only run with the tracker and ecal.

### Leptons

<table align="center">
  <tr>
    <td align="center" width="25%">
      <img src="docs/assets/detector_energy_map_pdg_11_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Electron (PDG = 11)</sub>
    </td>
    <td align="center" width="25%">
      <img src="docs/assets/detector_energy_map_pdg_-11_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Positron (PDG = -11)</sub>
    </td>
    <td align="center" width="25%">
      <img src="docs/assets/detector_energy_map_pdg_13_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Muon (PDG = 13)</sub>
    </td>
    <td align="center" width="25%">
      <img src="docs/assets/detector_energy_map_pdg_-13_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Anti-muon (PDG = -13)</sub>
    </td>
  </tr>
</table>

### Electromagnetic Particles

<table align="center">
  <tr>
    <td align="center" width="50%">
      <img src="docs/assets/detector_energy_map_pdg_22_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Photon (PDG = 22)</sub>
    </td>
  </tr>
</table>

### Hadrons

<table align="center">
  <tr>
    <td align="center" width="50%">
      <img src="docs/assets/detector_energy_map_pdg_211_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Charged pion π⁺ (PDG = 211)</sub>
    </td>
    <td align="center" width="50%">
      <img src="docs/assets/detector_energy_map_pdg_-211_tracker_ecal_hcal.png" width="100%" /><br/>
      <sub>Charged pion π⁻ (PDG = -211)</sub>
    </td>
  </tr>
</table>

### Detectors

<table align="center">
  <tr>
    <td align="center" width="33%">
      <img src="docs/assets/tracker.png" width="100%" /><br/>
      <sub>Tracker only</sub>
    </td>
    <td align="center" width="33%">
      <img src="docs/assets/tracker+ecal.png" width="100%" /><br/>
      <sub>Tracker + ECAL</sub>
    </td>
    <td align="center" width="33%">
      <img src="docs/assets/tracker+ecal+hcal.png" width="100%" /><br/>
      <sub>Tracker + ECAL + HCAL</sub>
    </td>
  </tr>
</table>

## LICENSE

[MIT](LICENSE) © Samy Rahmani

