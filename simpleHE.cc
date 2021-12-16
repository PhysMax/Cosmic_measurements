#include "SimpleGeometry.hh"
#include "SimplePrimaryGeneratorAction.hh"
#include "SimpleSteppingAction.hh"
#include "SimpleEventAction.hh"
#include "SimpleRootWriter.hh"

#include "G4RunManager.hh"
#include "G4OpticalPhysics.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
#include "SimpleRootWriter.hh"

#include <stdlib.h>

int main(int argc,char** argv)
{
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Detector construction
  runManager->SetUserInitialization(new SimpleGeometry(atof(argv[argc-1])));

  // Physics list
  G4VModularPhysicsList* physicsList = new QGSP_BERT;
  physicsList->SetVerboseLevel(1);
  physicsList->RegisterPhysics(new G4OpticalPhysics);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserAction(new SimplePrimaryGeneratorAction);
  runManager->SetUserAction(new SimpleEventAction());  
  runManager->SetUserAction(new SimpleSteppingAction());

  // Initialize ROOT
  SimpleRootWriter::GetPointer()->Initialize(atof(argv[argc-1]));
  SimpleRootWriter::GetPointer()->h1[0] = new TH1F("edep1","Energy deposition in the first scintillator; Edep_{1} [MeV]; Entries", 100, 0, 20);
  SimpleRootWriter::GetPointer()->h1[1] = new TH1F("scint_flag","0 = 1st scint, 1 = 2nd scint, 2 = both scint", 3, -0.5, 2.5);
  SimpleRootWriter::GetPointer()->h1[2] = new TH1F("angle_nhits","Nhits in both scintillators vs angle; angle [deg]; Nhits in both detectors",90, 0, 90);
  SimpleRootWriter::GetPointer()->h1[3] = new TH1F("delta_e_energy","Kinetic energy of delta electrons produced by the initial muon; E[keV]; Entries", 200, 0, 100);
  SimpleRootWriter::GetPointer()->h1[4] = new TH1F("theta","Theta", 200, 0, 3.14);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (*(argv[argc - 2]) == '1' ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  SimpleRootWriter::GetPointer()->Finalize();
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
