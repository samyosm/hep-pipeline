#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4UserRunAction.hh"

RunAction::RunAction() : G4UserRunAction() {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  if (G4Threading::IsMasterThread()) {
    analysisManager->SetNtupleMerging(true);
  }

  analysisManager->CreateNtuple("Steps", "Detectors");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("LayerID");
  analysisManager->CreateNtupleIColumn("PDG");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("Time");
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
