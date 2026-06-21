#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction(nullptr));
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);

  RunAction *runAction = new RunAction(eventAction);
  SetUserAction(runAction);

  SteppingAction *steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
