#include "PrimaryGeneratorAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction() {
  fParticleGun = new G4ParticleGun(1);

  fParticleGun->SetParticleEnergy(50.0 * MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -40.0 * cm));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *proton = particleTable->FindParticle("proton");
  G4ParticleDefinition *electron = particleTable->FindParticle("e-");

  G4ThreeVector randomDir = G4RandomDirection();
  fParticleGun->SetParticleMomentumDirection(randomDir);

  if (G4UniformRand() > 0.5) {
    fParticleGun->SetParticleDefinition(proton);
  } else {
    fParticleGun->SetParticleDefinition(electron);
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
