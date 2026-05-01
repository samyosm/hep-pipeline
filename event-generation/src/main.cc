#include "Pythia8/Pythia.h"
#include "Pythia8/PythiaParallel.h"
#include "Pythia8Plugins/HepMC3.h"
#include "utils.hpp"
#include <HepMC3/WriterAscii.h>
#include <atomic>
#include <iostream>
#include <mutex>

using namespace Pythia8;

int main() {
  hepp::ensurePythiaData();
  PythiaParallel pythia;
  pythia.readString("Parallelism:numThreads = 8");

  if (!pythia.readFile("resources/cards/main.cmnd")) {
    std::cerr << "Error: couldn't read command file!" << std::endl;
    return 1;
  }

  if (!pythia.init()) {
    std::cerr << "Pythia failed to initialize!" << std::endl;
    return 1;
  }

  int nEvents = pythia.settings.mode("Main:numberOfEvents");

  HepMC3::Pythia8ToHepMC3 toHepMC;
  HepMC3::WriterAscii ascii_io("data/events.hepmc");
  std::mutex fileMutex;
  std::atomic<int> completedEvents{0};

  pythia.run([&](Pythia *pythiaPtr) {
    HepMC3::Pythia8ToHepMC3 toHepMC;
    HepMC3::GenEvent hepmcevt(HepMC3::Units::GEV, HepMC3::Units::MM);

    toHepMC.fill_next_event(*pythiaPtr, &hepmcevt);

    {
      std::lock_guard<std::mutex> lock(fileMutex);
      ascii_io.write_event(hepmcevt);
    }

    int current = ++completedEvents;

    if (current % 100 == 0 || current == nEvents) {
      std::lock_guard<std::mutex> lock(fileMutex);
      hepp::showProgressBar(current, nEvents, "Parallel Generation");
    }
  });

  ascii_io.close();
  pythia.stat();

  return 0;
}
