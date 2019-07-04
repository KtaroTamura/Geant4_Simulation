//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"
#include "config.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;

class DetectorMessenger;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // Set methods
    void SetTargetMaterial (G4String );
//    void SetChamberMaterial(G4String );
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );

  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
	void SetColor();	

    // data members
	//Material
	G4Material* air;
  	G4Material* Pb;
  	G4Material* Al;
  	G4Material* W;
    G4Material* Plastic;
    G4Material* CF4;
    G4Material* Vacuum; 

    //Logical Volume
    G4LogicalVolume* fWorldVolume;
    G4LogicalVolume* fSCVolume;
    G4LogicalVolume* fCDCVolume;
    G4LogicalVolume* fTCVolume;
    G4LogicalVolume* fFoilVolume;
    G4LogicalVolume* fPipeVolume;
    G4LogicalVolume* fVacuumVolume;

    //Logical Wires
    G4LogicalVolume* fAnode;
    G4LogicalVolume* fCathode;
    G4LogicalVolume* fField;
    G4LogicalVolume* fShield;

    //Physical Volume
    G4VPhysicalVolume* CDC_Pos;
    G4VPhysicalVolume* Foil_Pos;
    G4VPhysicalVolume* Anode_Pos[400];
    G4VPhysicalVolume* Pipe_Pos;
    G4VPhysicalVolume* Vacuum_Pos;

#ifndef Detector_skin
    G4VPhysicalVolume* TC_Pos[12];
    G4VPhysicalVolume* SC_Pos[12];

	//Skin Detail//
#else
	G4Material* Mylar;
	
    G4LogicalVolume* fSC_skin;
    G4LogicalVolume* fTC_skin;
    G4LogicalVolume* fCDC_skin;

    G4VPhysicalVolume* CDCskin_Pos;
    G4VPhysicalVolume* SCskin_Pos[12];
    G4VPhysicalVolume* TCskin_Pos[12];
	G4VPhysicalVolume* TC_Pos;
	G4VPhysicalVolume* SC_Pos;
#endif
       
	G4UserLimits*      fStepLimit;       // pointer to user step limits
    DetectorMessenger*  fMessenger;   // detector messenger

    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
                                         // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
