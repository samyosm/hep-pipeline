// TODO:

#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4Event.hh>
#include <G4Types.hh>
#include <G4UserEventAction.hh>

EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event *event) { fTotalEdep = 0.; }
void EventAction::EndOfEventAction(const G4Event *event) {
  // auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->FillNtupleDColumn(0, 0, fTotalEdep);
  // analysisManager->FillNtupleIColumn(0, 1, event->GetEventID());
  // analysisManager->AddNtupleRow(0);
}
