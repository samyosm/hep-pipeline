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
  G4double stepLength = step->GetStepLength();

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

    G4double e_quenched = edep;
    if (layerID >= 10 && edep > 0.0 && stepLength > 0.0) {
      // dE/dx in MeV/mm
      G4double dEdx = edep / stepLength;

      // TODO: Add to config file
      const G4double kB_mm_per_MeV =
          0.126; // https://geant4-forum.web.cern.ch/t/birks-constant-for-different-materials/1014

      e_quenched = edep / (1.0 + kB_mm_per_MeV * dEdx);
    }

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0, eventID);    // EventID
    analysisManager->FillNtupleIColumn(0, 1, layerID);    // LayerID
    analysisManager->FillNtupleIColumn(0, 2, PDG);        // PDG
    analysisManager->FillNtupleDColumn(0, 3, pos.x());    // X
    analysisManager->FillNtupleDColumn(0, 4, pos.y());    // Y
    analysisManager->FillNtupleDColumn(0, 5, pos.z());    // Z
    analysisManager->FillNtupleDColumn(0, 6, e_quenched); // Edep
    analysisManager->FillNtupleDColumn(0, 7, globalTime); // Time
    analysisManager->AddNtupleRow(0);
  }
}
