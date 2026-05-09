#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4UserRunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetVerboseLevel(1);
  analysisManager->SetDefaultFileType("root");

  // 0
  analysisManager->CreateNtuple("Events", "Detectors");
  analysisManager->CreateNtupleDColumn("TotalEdep");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->FinishNtuple();

  // 1
  analysisManager->CreateNtuple("Steps", "Detectors");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("LayerID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("PDG");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  if (isMaster) {
    analysisManager->SetNtupleMerging(true, 8); // FIX: Not working
  }

  analysisManager->OpenFile("data/detector_simulation.root");
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
