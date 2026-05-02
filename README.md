# High Energy Physics Pipeline

## Overview

This repository contains a complete, end-to-end implementation of a High Energy Physics (HEP) analysis pipeline, inspired by workflows used at CERN.

The goal of this project is to gain practical, hands-on experience with every stage of the pipeline while reproducing known physics results.

## Pipeline Structure

- [x] 1. Event generation
- [ ] 2. Detector simulation
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
./build/event-generation/gen_events
```


## LICENSE

[MIT](LICENSE) © Samy Rahmani
