#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4UniformMagField.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4Types.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *active_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material *passive_mat = nist->FindOrBuildMaterial("G4_Pb");

  /* WORLD */
  G4double world_size = 2 * m; // 4m x 4m x 4m world
  G4Box *solidWorld = new G4Box("World", world_size, world_size, world_size);

  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, air_mat, "World");

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

  /* DETECTORS */
  std::size_t layer_count = 50;

  G4double inner_radius = 50. * cm;

  G4double active_thickness = 2. * mm;
  G4double passive_thickness = 4. * mm;

  G4double hz = 0.75 * m; // 1.5 m long detectors
  G4double start_angle = 0. * deg;
  G4double spinning_angle = 360. * deg;

  G4VisAttributes *passiveVis =
      new G4VisAttributes(G4Color(1.0, 0., 1.0, 0.8)); // Gray, 30% opaque
  passiveVis->SetForceSolid(true);

  G4VisAttributes *activeVis =
      new G4VisAttributes(G4Color(0., 1., 1., 0.3)); // Pink, 80% opaque
  activeVis->SetForceSolid(true);

  for (int i = 0; i < layer_count; i++) {

    /* Passive layer*/
    G4Tubs *passiveTube = new G4Tubs("PassiveTube", inner_radius,
                                     inner_radius + passive_thickness, hz,
                                     start_angle, spinning_angle);

    inner_radius += passive_thickness;

    G4LogicalVolume *passiveLogical =
        new G4LogicalVolume(passiveTube, passive_mat, "PassiveLogical");
    passiveLogical->SetVisAttributes(passiveVis);

    G4VPhysicalVolume *passivePhysical =
        new G4PVPlacement(0, G4ThreeVector(), passiveLogical, "PassivePhysical",
                          logicWorld, false, 2 * i); // NOTE: Even = Passive

    /* Active layer*/
    G4Tubs *activeTube =
        new G4Tubs("ActiveTube", inner_radius, inner_radius + active_thickness,
                   hz, start_angle, spinning_angle);

    inner_radius += active_thickness;

    G4LogicalVolume *activeLogical =
        new G4LogicalVolume(activeTube, active_mat, "ActiveLogical");
    activeLogical->SetVisAttributes(activeVis);

    G4VPhysicalVolume *activePhysical =
        new G4PVPlacement(0, G4ThreeVector(), activeLogical, "ActivePhysical",
                          logicWorld, false, 2 * i + 1); // NOTE: Odd = Active
  }

  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4MagneticField *magField =
      new G4UniformMagField(G4ThreeVector(0.0, 0.0, 2.0 * tesla));

  G4FieldManager *fieldManager =
      G4TransportationManager::GetTransportationManager()->GetFieldManager();

  fieldManager->SetDetectorField(magField);
  fieldManager->CreateChordFinder(magField);
}
