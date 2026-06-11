# Event Generation

This component handles the simulation of final-state particles produced in collision events under LHC Run 3 conditions.

Pythia8 is used to generate events in a multithreaded process (currently hardcoded to use 8 threads).

## Default Configuration

The configuration file is located at `/resources/cards/main.cmnd`.

- Process: HardQCD
- Beams: proton–proton collisions
- Center-of-mass energy: 13.6 TeV
- Filter: transverse momentum > 20 GeV
- Number of events: 1000

## Output

The output is an HEPMC3-formatted file located at `data/event.hepmc`, using `GeV` as the unit of energy/mass and `mm` as the unit of length.

## Result

The simulation results of this module are published on Zenodo as sets of 100k, 50k, 10k and 1k collision events.

Hard QCD with pT greater than 20 GeV in pp Collisions at 13.6 TeV: https://doi.org/10.5281/zenodo.19960730


