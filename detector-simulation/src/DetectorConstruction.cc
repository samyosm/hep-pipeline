#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include <G4Tubs.hh>
#include <G4Types.hh>
#include <G4VPhysicalVolume.hh>

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *pb_mat = nist->FindOrBuildMaterial("G4_Pb");

  /* WORLD */
  G4double world_size = 1 * m; // 2m x 2m x 2m world
  G4Box *solidWorld = new G4Box("World", world_size, world_size, world_size);

  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, air_mat, "World");

  /* DETECTOR */
  G4double inner_radius = 0. * cm;
  G4double outer_radius = 90. * cm;
  G4double hz = 30. * cm;
  G4double start_angle = 0. * deg;
  G4double spinning_angle = 360. * deg;

  G4Tubs *trackerTube = new G4Tubs("Tracker", inner_radius, outer_radius, hz,
                                   start_angle, spinning_angle);

  G4LogicalVolume *logicalTracker =
      new G4LogicalVolume(trackerTube, pb_mat, "Tracker");

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        logicWorld,      // logical volume
                        "World",         // name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        true             // check for 0overlaps
      );

  G4VPhysicalVolume *physTracker =
      new G4PVPlacement(0, G4ThreeVector(0, 0, 50. * cm), logicalTracker,
                        "Tracker", logicWorld, false, 0);

  return physWorld;
}
