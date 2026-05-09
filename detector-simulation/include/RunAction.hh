#pragma once

#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction {
public:
  RunAction();
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run *) override;
  virtual void EndOfRunAction(const G4Run *) override;
};
