#pragma once

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "HepMC3/ReaderAscii.h"

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event *anEvent) override;

private:
  // G4ParticleGun *fParticleGun;
  HepMC3::ReaderAscii *fReader;
};
