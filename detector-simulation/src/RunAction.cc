#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4UserRunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetVerboseLevel(1);
  analysisManager->SetDefaultFileType("root");
  if (G4Threading::IsMasterThread()) {
    analysisManager->SetNtupleMerging(true);
  }

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

  // 2: Dead tracks
  analysisManager->CreateNtuple("DeadSteps", "Detectors");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleSColumn("LayerName");
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
  analysisManager->OpenFile("data/detector_simulation.root");
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
