#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include <G4String.hh>
#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>
#include <G4VisManager.hh>

int main(int argc, char **argv) {
  auto runManager = G4RunManagerFactory::CreateRunManager();

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new FTFP_BERT);
  runManager->SetUserInitialization(new ActionInitialization);

  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (argc != 1) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];

    UI->ApplyCommand(command + fileName);
  } else {
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
    UI->ApplyCommand("/control/execute resources/macros/init.mac");
    ui->SessionStart();

    delete ui;
  }

  delete runManager;
  return 0;
}
