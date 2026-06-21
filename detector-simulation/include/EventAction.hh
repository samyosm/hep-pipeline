#pragma once
#include "G4UserEventAction.hh"
#include <G4Types.hh>
#include <vector>

class EventAction : public G4UserEventAction {
public:
  EventAction() = default;
  virtual ~EventAction() = default;

  virtual void BeginOfEventAction(const G4Event *event) override;
  virtual void EndOfEventAction(const G4Event *event) override;

  void AddStepData(int layer, int pdg, double x, double y, double z,
                   double edep, double time) {
    fLayerIDs.push_back(layer);
    fPDGs.push_back(pdg);
    fXs.push_back(x);
    fYs.push_back(y);
    fZs.push_back(z);
    fEdeps.push_back(edep);
    fTimes.push_back(time);
  }

  std::vector<int> fLayerIDs;
  std::vector<int> fPDGs;
  std::vector<double> fXs;
  std::vector<double> fYs;
  std::vector<double> fZs;
  std::vector<double> fEdeps;
  std::vector<double> fTimes;
};
