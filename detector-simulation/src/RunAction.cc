#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4UserRunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
  auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetVerboseLevel(1);
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetNtupleMerging(true); // FIX: Not working

  // 0
  analysisManager->CreateNtuple("Events", "Tracker Data");
  analysisManager->CreateNtupleDColumn("TotalEdep");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->FinishNtuple();

  // 1
  analysisManager->CreateNtuple("Steps", "Tracker Data");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleIColumn("TrackID");
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
