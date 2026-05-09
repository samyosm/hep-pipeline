#include "SteppingAction.hh"
#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4Step.hh>

SteppingAction::SteppingAction(EventAction *eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  const G4double edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edep);

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(1, 0, edep);
  analysisManager->FillNtupleIColumn(1, 1, step->GetTrack()->GetTrackID());
  analysisManager->AddNtupleRow(1);
}
