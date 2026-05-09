#include "PrimaryGeneratorAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include <G4Event.hh>
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <G4ios.hh>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <Randomize.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction() {
  // fParticleGun = new G4ParticleGun(1);
  // fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  fReader = new HepMC3::ReaderAscii("data/events.hepmc");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  fReader->close();
  delete fReader;

  // delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

  HepMC3::GenEvent evt;

  if (fReader->read_event(evt)) {
    G4PrimaryVertex *vertex = new G4PrimaryVertex(0, 0, 0, 0);

    for (auto p : evt.particles()) {
      if (p->status() == 1) {
        G4int pdgID = p->pdg_id();
        G4double px = p->momentum().px() * GeV;
        G4double py = p->momentum().py() * GeV;
        G4double pz = p->momentum().pz() * GeV;

        G4PrimaryParticle *g4part = new G4PrimaryParticle(pdgID, px, py, pz);
        vertex->SetPrimary(g4part);
      } else {
        // G4cout << "Non final particle" << G4endl;
      }
    }
    anEvent->AddPrimaryVertex(vertex);
  } else {
    G4cout << "Event final complete" << G4endl;
  }
}

// void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
//
//   G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
//   G4ParticleDefinition *proton = particleTable->FindParticle("proton");
//   G4ParticleDefinition *electron = particleTable->FindParticle("e-");
//
//   G4ThreeVector dir = G4ThreeVector(0, 0, 1 * m);
//   fParticleGun->SetParticleMomentumDirection(dir);
//
//   if (G4UniformRand() > 0.5) {
//     fParticleGun->SetParticleDefinition(proton);
//   } else {
//     fParticleGun->SetParticleDefinition(electron);
//   }
//
//   G4double pEnergy = 500;
//
//   fParticleGun->SetParticleEnergy(pEnergy * MeV);
//
//   fParticleGun->GeneratePrimaryVertex(anEvent);
// }
