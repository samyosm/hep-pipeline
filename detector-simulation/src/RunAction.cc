#include "RunAction.hh"
#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4UserRunAction.hh"

RunAction::RunAction(EventAction *eventAction)
    : G4UserRunAction(), fEventAction(eventAction) {

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");

  analysisManager->SetNtupleMerging(true);
  analysisManager->SetNtupleRowWise(false, false);
  analysisManager->SetBasketSize(32 * 1024);

  analysisManager->CreateNtuple("Steps", "Detector Simulation Steps");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("LayerID", fEventAction->fLayerIDs);
  analysisManager->CreateNtupleIColumn("PDG", fEventAction->fPDGs);
  analysisManager->CreateNtupleDColumn("X", fEventAction->fXs);
  analysisManager->CreateNtupleDColumn("Y", fEventAction->fYs);
  analysisManager->CreateNtupleDColumn("Z", fEventAction->fZs);
  analysisManager->CreateNtupleDColumn("Edep", fEventAction->fEdeps);
  analysisManager->CreateNtupleDColumn("Time", fEventAction->fTimes);
  analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("data/detector_simulation.root");
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
