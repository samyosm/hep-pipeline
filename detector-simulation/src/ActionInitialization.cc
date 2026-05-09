#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  RunAction *runAction = new RunAction();
  SetUserAction(runAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);

  SteppingAction *steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
