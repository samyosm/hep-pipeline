#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4double world_size = 0.5 * m;
  G4Box *solidWorld = new G4Box("World", world_size, world_size, world_size);

  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        logicWorld, "World",
                        0,     // its mother  volume
                        false, // no boolean operation
                        0,     // copy number
                        true   // check for overlaps
      );

  return physWorld;
}
