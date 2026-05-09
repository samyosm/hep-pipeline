#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();
};
