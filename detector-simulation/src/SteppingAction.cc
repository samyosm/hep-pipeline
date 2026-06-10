#include "SteppingAction.hh"
#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4RunManager.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4VPhysicalVolume.hh>
#include <G4ios.hh>

SteppingAction::SteppingAction(EventAction *eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {

  G4Track *track = step->GetTrack();
  G4VPhysicalVolume *physicalVolume =
      step->GetPreStepPoint()->GetPhysicalVolume();

  auto edep = step->GetTotalEnergyDeposit();

  auto layerName = physicalVolume->GetName();

  if (track->GetCurrentStepNumber() > 20000) {
    // auto eventID =
    //     G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // auto trackID = track->GetTrackID();
    // auto PDG = track->GetParticleDefinition()->GetPDGEncoding();
    // auto pos = step->GetPreStepPoint()->GetPosition();
    //
    // auto analysisManager = G4AnalysisManager::Instance();
    // analysisManager->FillNtupleIColumn(2, 0, eventID);
    // analysisManager->FillNtupleSColumn(2, 1, layerName);
    // analysisManager->FillNtupleIColumn(2, 2, trackID);
    // analysisManager->FillNtupleIColumn(2, 3, PDG);
    // analysisManager->FillNtupleDColumn(2, 4, pos.x());
    // analysisManager->FillNtupleDColumn(2, 5, pos.y());
    // analysisManager->FillNtupleDColumn(2, 6, pos.z());
    // analysisManager->FillNtupleDColumn(2, 7, edep);
    // analysisManager->AddNtupleRow(2);
    //
    // G4cerr << "Track reached maximum step count. Killing ID "
    //        << track->GetTrackID() << " at " << track->GetPosition() <<
    //        G4endl;
    track->SetTrackStatus(fStopAndKill);
    return;
  }

  if (G4StrUtil::contains(layerName, "Active")) {
    auto eventID =
        G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    auto layerID = physicalVolume->GetCopyNo();
    auto trackID = track->GetTrackID();
    auto PDG = track->GetParticleDefinition()->GetPDGEncoding();
    auto pos = step->GetPreStepPoint()->GetPosition();

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(1, 0, eventID);
    analysisManager->FillNtupleIColumn(1, 1, layerID);
    analysisManager->FillNtupleIColumn(1, 2, trackID);
    analysisManager->FillNtupleIColumn(1, 3, PDG);
    analysisManager->FillNtupleDColumn(1, 4, pos.x());
    analysisManager->FillNtupleDColumn(1, 5, pos.y());
    analysisManager->FillNtupleDColumn(1, 6, pos.z());
    analysisManager->FillNtupleDColumn(1, 7, edep);
    analysisManager->AddNtupleRow(1);

  } else {
    fEventAction->AddEdep(edep);
  }
}
