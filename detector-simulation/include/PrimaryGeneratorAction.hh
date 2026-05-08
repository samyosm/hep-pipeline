#pragma once

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event *anEvent) override;

private:
  G4ParticleGun *fParticleGun;
};
