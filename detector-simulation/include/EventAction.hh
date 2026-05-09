#pragma once
#include "G4UserEventAction.hh"
#include <G4Types.hh>

class EventAction : public G4UserEventAction {
public:
  EventAction();
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event *event) override;

  virtual void EndOfEventAction(const G4Event *event) override;

  void AddEdep(G4double edep) { fTotalEdep += edep; }

private:
  G4double fTotalEdep;
};
