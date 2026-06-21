// TODO:

#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4Event.hh>
#include <G4Types.hh>
#include <G4UserEventAction.hh>

void EventAction::BeginOfEventAction(const G4Event *event) {}
void EventAction::EndOfEventAction(const G4Event *event) {
  if (fLayerIDs.empty()) {
    G4cout << "Warning: empty LayerID vector" << G4endl;
  }

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, 0, event->GetEventID());

  analysisManager->AddNtupleRow(0);

  fLayerIDs.clear();
  fPDGs.clear();
  fXs.clear();
  fYs.clear();
  fZs.clear();
  fEdeps.clear();
  fTimes.clear();
}
