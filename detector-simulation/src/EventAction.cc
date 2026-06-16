// TODO:

#include "EventAction.hh"
#include <G4AnalysisManager.hh>
#include <G4Event.hh>
#include <G4Types.hh>
#include <G4UserEventAction.hh>

EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event *event) {}
void EventAction::EndOfEventAction(const G4Event *event) {}
