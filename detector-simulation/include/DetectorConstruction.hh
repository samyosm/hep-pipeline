#pragma once

#include "G4VUserDetectorConstruction.hh"
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4Types.hh>
#include <vector>

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

private:
  void AddTrackerLayer(G4LogicalVolume *logicWorld);
  void AddECalLayer(G4LogicalVolume *logicWorld);
  void AddHCalLayer(G4LogicalVolume *logicWorld);
  void AddGap(G4double width);

  void AddTubeLayer(G4String name, G4double width, G4Material *mat,
                    G4LogicalVolume *logicWorld, G4bool with_magfield = false);

  G4double current_radius;
  G4int layerCount;
  std::vector<G4LogicalVolume *> magnetic_requiring_volumes;
};
