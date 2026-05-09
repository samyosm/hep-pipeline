#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"

#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

int main(int argc, char **argv) {
  G4UIExecutive *ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  auto runManager = G4RunManagerFactory::CreateRunManager();
  runManager->SetNumberOfThreads(8);

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new FTFP_BERT);
  runManager->SetUserInitialization(new ActionInitialization);

  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();
  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command + fileName);
  } else {
    UI->ApplyCommand("/control/execute resources/macros/init.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
  return 0;
}
