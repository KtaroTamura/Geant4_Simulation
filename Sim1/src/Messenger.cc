#include "Messenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "DetectorConstruction.hh"
DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fB2Directory = new G4UIdirectory("/B2/");
  fB2Directory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/B2/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/B2/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorMessenger::~DetectorMessenger()
{
  delete fStepMaxCmd;
  delete fB2Directory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction.hh"
PGAMessenger::PGAMessenger(PrimaryGeneratorAction* Det)
 : G4UImessenger(),
   fPrimaryGeneratorAction(Det)
{
  
//  fGunDist = new G4UIdirectory("/gun/distribute/");
//  fGunDist->SetGuidance("Beam distribution control");

  fGunDistCmd = new G4UIcmdWithAnInteger("/gun/distributeLevel",this);
  fGunDistCmd->SetGuidance("Eneter dist level\n \n0:mono E & direction\n1:mono E \n2:8Li_dist\n3:8Li_dist & random");
  fGunDistCmd->SetParameterName("Level",false);
  fGunDistCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PGAMessenger::~PGAMessenger()
{
 // delete fGunDist;                                                                                                 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PGAMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fGunDistCmd ) {
    fPrimaryGeneratorAction
      ->SetDistLevel(fGunDistCmd->GetNewIntValue(newValue));
  }
}

