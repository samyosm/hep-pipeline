#include "Pythia8/Pythia.h"
#include "utils.hpp"
#include <fstream>
#include <iostream>

using namespace Pythia8;

int main() {
  Pythia pythia;

  if (!pythia.readFile("resources/cards/basic.cmnd")) {
    std::cerr << "Error: couldn't read command file!" << std::endl;
    return 1;
  }

  if (!pythia.init()) {
    std::cerr << "Pythia failed to initialize!" << std::endl;
    return 1;
  }

  std::ofstream file("data/output.csv");
  file << "ID,count\n";

  int nEvents = pythia.mode("Next:numberCount");
  double totalEnergy = 0.0;
  Vec4 momentum = 0.0;
  int finalParticleCount = 0;

  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next())
      continue;

    hepp::showProgressBar(iEvent, nEvents, "Particle Generaton");

    int size = pythia.event.size();
    file << iEvent << "," << size << "\n";

    for (auto e : pythia.event) {
      if (e.isFinal()) {
        totalEnergy += e.e();
        momentum += e.p();
        finalParticleCount++;
      }
    }
  }

  std::cout << "Final partical count: " << finalParticleCount << std::endl;
  std::cout << "Total energy: " << totalEnergy << std::endl;
  std::cout << "Total momentum: " << momentum << std::endl;

  file.close();
  pythia.stat();

  return 0;
}
