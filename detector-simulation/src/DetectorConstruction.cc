#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4UniformMagField.hh"
#include "config.hh"
#include <G4Material.hh>
#include <G4String.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
  current_radius =
      (*hepp::Config::GetConfig())["detector"]["inner_chamber_radius"].value_or(
          0.0);
  layerCount = 0;
  magnetic_requiring_volumes = std::vector<G4LogicalVolume *>();
}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  auto &config = *hepp::Config::GetConfig();

  auto world_material =
      config["detector"]["world_material"].value_or("G4_Galactic");
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *air_mat = nist->FindOrBuildMaterial(world_material);

  /* WORLD */
  auto world_half_size = config["detector"]["world_half_size"].value_or(2000);
  G4Box *solidWorld =
      new G4Box("World", world_half_size, world_half_size, world_half_size);

  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, air_mat, "World");

  G4VPhysicalVolume *physWorld = new G4PVPlacement(
      0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

  /* Tracker */
  auto trackerEnabled = config["detector"]["tracker"]["enabled"].value_or(true);

  if (trackerEnabled) {
    auto layerCountLocal =
        config["detector"]["tracker"]["layerCount"].value_or(10);

    G4double tracker_inner =
        config["detector"]["inner_chamber_radius"].value_or(500) * mm;
    G4double active_width =
        config["detector"]["tracker"]["active_width"].value_or(14.8) * mm;
    G4double passive_width =
        config["detector"]["tracker"]["passive_width"].value_or(0.2) * mm;

    auto tracker_outer =
        tracker_inner + layerCountLocal * (active_width + passive_width);

    G4double hz = config["detector"]["hz"].value_or(750.0) * mm;

    G4Tubs *solidTrackerContainer = new G4Tubs(
        "TrackerContainer", 0, tracker_outer, hz, 0 * deg, 360 * deg);

    auto logicTrackerContainer =
        new G4LogicalVolume(solidTrackerContainer, air_mat, "TrackerContainer");
    G4VPhysicalVolume *physicalTrackerContainer = new G4PVPlacement(
        0, G4ThreeVector(), logicTrackerContainer, "TrackerContainerPhysical",
        logicWorld, false, layerCount++);

    magnetic_requiring_volumes.push_back(logicTrackerContainer);

    for (size_t i = 0; i < (size_t)layerCountLocal; i++) {
      AddTrackerLayer(logicTrackerContainer);
    }
  }

  /* 1nd Gap */
  auto firstGapEnabled = config["detector"]["gap1"]["enabled"].value_or(true);
  if (firstGapEnabled) {
    auto width = config["detector"]["gap1"]["width"].value_or(0.0);
    AddGap(width);
  }

  /* ECal */
  auto ecalEnabled = config["detector"]["ecal"]["enabled"].value_or(true);

  if (ecalEnabled) {
    auto layerCountLocal =
        config["detector"]["ecal"]["layerCount"].value_or(50);
    for (size_t i = 0; i < (size_t)layerCountLocal; i++) {
      AddECalLayer(logicWorld);
    }
  }

  /* 2nd Gap */
  auto secondGapEnabled = config["detector"]["gap2"]["enabled"].value_or(true);
  if (secondGapEnabled) {
    auto width = config["detector"]["gap2"]["width"].value_or(0.0);
    AddGap(width);
  }

  /* HCal */
  auto hcalEnabled = config["detector"]["hcal"]["enabled"].value_or(true);

  if (hcalEnabled) {
    auto layerCountLocal =
        config["detector"]["hcal"]["layerCount"].value_or(96);
    for (size_t i = 0; i < (size_t)layerCountLocal; i++) {
      AddHCalLayer(logicWorld);
    }
  }

  return physWorld;
}

void DetectorConstruction::AddGap(G4double width) { current_radius += width; }

void DetectorConstruction::AddTrackerLayer(G4LogicalVolume *logicWorld) {
  auto &config = *hepp::Config::GetConfig();

  G4double active_thickness =
      config["detector"]["tracker"]["active_width"].value_or(0.2) * mm;
  G4double passive_thickness =
      config["detector"]["tracker"]["passive_width"].value_or(14.8) * mm;

  G4String passive_material =
      config["detector"]["tracker"]["passive_material"].value_or("G4_Si");
  G4String active_material =
      config["detector"]["tracker"]["active_material"].value_or("G4_Galactic");

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *active_mat = nist->FindOrBuildMaterial(active_material);
  G4Material *passive_mat = nist->FindOrBuildMaterial(passive_material);

  AddTubeLayer("ActiveTracker", active_thickness, active_mat, logicWorld);
  AddTubeLayer("PassiveTracker", passive_thickness, passive_mat, logicWorld);
}

void DetectorConstruction::AddHCalLayer(G4LogicalVolume *logicWorld) {
  auto &config = *hepp::Config::GetConfig();

  G4double active_thickness =
      config["detector"]["hcal"]["active_width"].value_or(2.0) * mm;
  G4double passive_thickness =
      config["detector"]["hcal"]["passive_width"].value_or(14.0) * mm;

  G4String passive_material =
      config["detector"]["hcal"]["passive_material"].value_or("G4_Pb");
  G4String active_material =
      config["detector"]["hcal"]["active_material"].value_or("G4_POLYSTYRENE");

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *active_mat = nist->FindOrBuildMaterial(active_material);
  G4Material *passive_mat = nist->FindOrBuildMaterial(passive_material);

  AddTubeLayer("ActiveHCal", active_thickness, active_mat, logicWorld);
  AddTubeLayer("PassiveHCal", passive_thickness, passive_mat, logicWorld);
}

void DetectorConstruction::AddECalLayer(G4LogicalVolume *logicWorld) {
  auto &config = *hepp::Config::GetConfig();

  G4double active_thickness =
      config["detector"]["ecal"]["active_width"].value_or(2.0) * mm;
  G4double passive_thickness =
      config["detector"]["ecal"]["passive_width"].value_or(2.5) * mm;

  G4String passive_material =
      config["detector"]["ecal"]["passive_material"].value_or("G4_Fe");
  G4String active_material =
      config["detector"]["ecal"]["active_material"].value_or("G4_POLYSTYRENE");

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *active_mat = nist->FindOrBuildMaterial(active_material);
  G4Material *passive_mat = nist->FindOrBuildMaterial(passive_material);

  AddTubeLayer("ActiveECal", active_thickness, active_mat, logicWorld);
  AddTubeLayer("PassiveECal", passive_thickness, passive_mat, logicWorld);
}

void DetectorConstruction::AddTubeLayer(G4String name, G4double width,
                                        G4Material *mat,
                                        G4LogicalVolume *logicWorld,
                                        G4bool with_magfield) {
  auto &config = *hepp::Config::GetConfig();
  G4double hz = config["detector"]["hz"].value_or(750.0) * mm;

  G4Tubs *tube = new G4Tubs(name + "-tube", current_radius,
                            current_radius + width, hz, 0 * deg, 360 * deg);

  current_radius += width;

  G4LogicalVolume *logical = new G4LogicalVolume(tube, mat, name + "-logical");
  // logical->SetVisAttributes(activeVis); // TODO: Add visual

  if (with_magfield) {
    magnetic_requiring_volumes.push_back(logical);
  }

  G4VPhysicalVolume *physical =
      new G4PVPlacement(0, G4ThreeVector(), logical, name + "-physical",
                        logicWorld, false, layerCount++);
}

void DetectorConstruction::ConstructSDandField() {
  auto &config = *hepp::Config::GetConfig();
  G4double strength =
      config["detector"]["tracker"]["magnetic_field_strength"].value_or(2) *
      tesla;

  G4MagneticField *magField =
      new G4UniformMagField(G4ThreeVector(0.0, 0.0, strength));

  G4FieldManager *fieldManager = new G4FieldManager();

  fieldManager->SetDetectorField(magField);
  fieldManager->CreateChordFinder(magField);

  for (G4LogicalVolume *volume : magnetic_requiring_volumes) {
    G4cout << "Adding magnetic field of " << strength << " teslas to "
           << volume->GetName() << G4endl;
    volume->SetFieldManager(fieldManager, true);
  }
}
