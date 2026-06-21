#include "SteppingAction.hh"
#include "EventAction.hh"
#include "config.hh"
#include <G4AnalysisManager.hh>
#include <G4RunManager.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4SystemOfUnits.hh>
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

    auto &config = *hepp::Config::GetConfig();
    auto quenching = config["detector"]["quenching"].value_or(true);

    G4double e_quenched = edep;
    // TODO: Remove hardcoding of 21 to ecal start layer ID
    if (quenching && layerID >= 21 && edep > 0.0 && stepLength > 0.0) {
      G4double dEdx = edep / stepLength;

      // TODO: Add to config file
      const G4double kB_mm_per_MeV =
          0.126; // https://geant4-forum.web.cern.ch/t/birks-constant-for-different-materials/1014

      e_quenched = edep / (1.0 + kB_mm_per_MeV * dEdx);
    }

    fEventAction->fLayerIDs.push_back(layerID);
    fEventAction->fPDGs.push_back(PDG);
    fEventAction->fXs.push_back(pos.x());
    fEventAction->fYs.push_back(pos.y());
    fEventAction->fZs.push_back(pos.z());
    fEventAction->fEdeps.push_back(e_quenched);
    fEventAction->fTimes.push_back(globalTime);
  }
}
