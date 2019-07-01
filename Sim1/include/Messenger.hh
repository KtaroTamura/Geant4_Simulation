#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

class DetectorConstruction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for DetectorConstruction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    virtual ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fB2Directory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

#ifndef PGAMessenger_h
class G4UIcmdWithAnInteger;
class PrimaryGeneratorAction; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for PrimaryGeneratorAction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class PGAMessenger: public G4UImessenger
{
  public:
    PGAMessenger(PrimaryGeneratorAction* );
    virtual ~PGAMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    PrimaryGeneratorAction*  fPrimaryGeneratorAction;

//    G4UIdirectory*           fGunDist;                                                                             
    G4UIcmdWithAnInteger* fGunDistCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
