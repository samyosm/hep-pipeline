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
    track->SetTrackStatus(fStopAndKill);
    return;
  }

  if (G4StrUtil::contains(layerName, "Active") && edep > 0.0) {
    auto eventID =
        G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    auto layerID = physicalVolume->GetCopyNo();
    auto trackID = track->GetTrackID();
    auto PDG = track->GetParticleDefinition()->GetPDGEncoding();
    auto pos = step->GetPreStepPoint()->GetPosition();
    auto globalTime = step->GetPreStepPoint()->GetGlobalTime();

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0, layerID);    // LayerID
    analysisManager->FillNtupleIColumn(0, 1, PDG);        // PDG
    analysisManager->FillNtupleDColumn(0, 2, pos.x());    // X
    analysisManager->FillNtupleDColumn(0, 3, pos.y());    // Y
    analysisManager->FillNtupleDColumn(0, 4, pos.z());    // Z
    analysisManager->FillNtupleDColumn(0, 5, edep);       // Edep
    analysisManager->FillNtupleDColumn(0, 6, globalTime); // Time
    analysisManager->AddNtupleRow(0);
  }
}
