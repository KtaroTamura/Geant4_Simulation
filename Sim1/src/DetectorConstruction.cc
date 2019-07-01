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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

//Visual Setting//
#include "DetectorConstruction.hh"
//#include "DetectorMessenger.hh"
#include "Messenger.hh"
#include "TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

//Add by myself
#include "G4Trd.hh"
#include "ScintSD.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
fWorldVolume(0),
fSCVolume(0),fCDCVolume(0),fTCVolume(0),fPipeVolume(0),fVacuumVolume(0),
fStepLimit(NULL),fMessenger(0),fCheckOverlaps(true)
{
  fMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
// Material definition
	
	G4NistManager* nist = G4NistManager::Instance();
	
	air = nist->FindOrBuildMaterial("G4_AIR");
	Pb = nist->FindOrBuildMaterial("G4_Pb");
	Al = nist->FindOrBuildMaterial("G4_Al");
	W = nist->FindOrBuildMaterial("G4_W");
	Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

	G4double cf4_n=0.00378*g/cm3;
	G4Element* elC=nist->FindOrBuildElement("C");
	G4Element* elF=nist->FindOrBuildElement("F");
	CF4=new G4Material("CF4",cf4_n,2);
	CF4->AddElement(elC, 1);
	CF4->AddElement(elF, 4);

	Vacuum=new G4Material("interGalactic",1.,1.008*g/mole,1.e-25*g/cm3,kStateGas,2.73*kelvin,3.e-18*pascal);

#ifdef Detector_skin 
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
#endif
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
	// World
	G4double World_size=0.8*m;
	G4Box* WorldSolid    = new G4Box("World",World_size,World_size,World_size); //size
			fWorldVolume = new G4LogicalVolume(WorldSolid,air,"World");

	G4VPhysicalVolume* physWorld
	  = new G4PVPlacement(0,                     //no rotation
	                      G4ThreeVector(),       //at (0,0,0)
	                      fWorldVolume,          //logical volume
	                      "World",               //name
	                      0,                     //mother volume
	                      false,                 //no boolean operation
	                      0);                    //copy number

	//SC//
	G4double SC_upper = 11.15*cm/2;
	G4double SC_lower = 14.902*cm/2;
	G4double SC_thick =  6.999*cm/2;
	G4double SC_length= 70.*cm/2;
	G4double SC_placeR= 24.4*cm;

	G4Trd* SCsolid	 =new G4Trd("SC",SC_lower,SC_upper,SC_length, SC_length, SC_thick);
	 	 fSCVolume   =new G4LogicalVolume(SCsolid,Plastic,"SC");

#ifdef Detector_skin 
	G4Trd* SCskin_solid	 =new G4Trd("SC_skin",SC_lower+0.05*mm,SC_upper+0.05*mm,SC_length+0.05*mm, SC_length+0.05*mm, SC_thick+0.05*mm);
	          fSC_skin   =new G4LogicalVolume(SCskin_solid,Al,"SC_skin");
#endif

	G4int Nscint=12;
	G4double scint_dphi=360./Nscint*deg;

	G4RotationMatrix SC_Rot=G4RotationMatrix();
	G4ThreeVector SC_Vec=G4ThreeVector(0,SC_placeR,0);
	SC_Rot.rotateX(90.*deg);
	char SCname[10];

#ifndef Detector_skin
	for(G4int ii=0;ii<12;ii++){
		if(ii!=0){
			SC_Vec.rotateZ(-scint_dphi);
			SC_Rot.rotateZ(-scint_dphi);
		}
		sprintf(SCname,"SC%d",ii+1);
		SC_Pos[ii]=new G4PVPlacement(G4Transform3D(SC_Rot,SC_Vec),fSCVolume,SCname,fWorldVolume,false,ii,fCheckOverlaps);
	}
#else
	char SCskin_name[10];
	for(G4int ii=0;ii<12;ii++){
		if(ii!=0){
			SC_Vec.rotateZ(-scint_dphi);
			SC_Rot.rotateZ(-scint_dphi);
		}
		sprintf(SCskin_name,"SC_skin%d",ii+1);
		sprintf(SCname,"SC%d",ii+1);
		SCskin_Pos[ii]=new G4PVPlacement(G4Transform3D(SC_Rot,SC_Vec),fSC_skin,SCskin_name,fWorldVolume,false,ii,fCheckOverlaps);
		SC_Pos[ii]=new G4PVPlacement(G4Transform3D(G4RotationMatrix(),G4ThreeVector()),fSCVolume,SCname,fSC_skin,false,ii,fCheckOverlaps);
	}
#endif

	// CDC volume
	G4double rmin = 10.*cm;
	G4double rmax = 20.*cm;
	G4double phiMin = 0.;
	G4double deltaPhi = 360*deg;

	G4Tubs* CDCsolid =new G4Tubs("CDC",rmin,rmax,SC_length,phiMin,deltaPhi);
		  fCDCVolume =new G4LogicalVolume(CDCsolid,CF4,"CDC");

#ifndef Detector_skin 
		     CDC_Pos =new G4PVPlacement(0,G4ThreeVector(),fCDCVolume,"CDC",fWorldVolume,false,0);
#else
	G4Tubs* CDCskinsolid =new G4Tubs("CDC_Mylar",rmin-50.*um,rmax+50.*um,SC_length,phiMin,deltaPhi);
	           fCDC_skin =new G4LogicalVolume(CDCskinsolid,Mylar,"CDC_skin");
	         CDCskin_Pos =new G4PVPlacement(0,G4ThreeVector(),fCDC_skin,"CDC_skin",fWorldVolume,false,0);
		         CDC_Pos =new G4PVPlacement(0,G4ThreeVector(),fCDCVolume,"CDC",fCDC_skin,false,0);
#endif

	// TC volume
	G4double TC_thick = 1.*mm/2;
	G4double TC_width = 4.5*cm/2;
	G4double TC_length= 31.66*cm/2;
	G4double TC_pR=8.5*cm;
	G4Box* TCsolid =new G4Box("TC",TC_width,TC_thick,TC_length);
		 fTCVolume =new G4LogicalVolume(TCsolid,Plastic,"TC");//(solid,material,name)

#ifdef Detector_skin
	G4Box* TC_skin_solid =new G4Box("TC_skin",TC_width+0.05*mm,TC_thick+0.05*mm,TC_length+0.05*mm);
	          fTC_skin   =new G4LogicalVolume(TC_skin_solid,Al,"TC_skin");
#endif
	G4RotationMatrix TC_Rot=G4RotationMatrix();
	G4ThreeVector TC_Vec=G4ThreeVector(0,TC_pR,0);
	char TCname[10];

#ifndef Detector_skin
	for(G4int ii=0;ii<Nscint;ii++){
		if(ii!=0){
			TC_Vec.rotateZ(scint_dphi);
			TC_Rot.rotateZ(scint_dphi);
		}
		sprintf(TCname,"TC%d",ii+1);
		TC_Pos[ii]=new G4PVPlacement(G4Transform3D(TC_Rot,TC_Vec),fTCVolume,TCname,fWorldVolume,false,ii,fCheckOverlaps);
	}
#else
		char TCskin_name[10];
		for(G4int ii=0;ii<12;ii++){
			if(ii!=0){
				TC_Vec.rotateZ(-scint_dphi);
				TC_Rot.rotateZ(-scint_dphi);
			}
			sprintf(TCskin_name,"TC_skin%d",ii+1);
			sprintf(TCname,"TC%d",ii+1);
			TCskin_Pos[ii]=new G4PVPlacement(G4Transform3D(TC_Rot,TC_Vec),fTC_skin,TCskin_name,fWorldVolume,false,ii,fCheckOverlaps);
			TC_Pos[ii]=new G4PVPlacement(G4Transform3D(G4RotationMatrix(),G4ThreeVector()),fTCVolume,TCname,fTC_skin,false,ii,fCheckOverlaps);
		}
#endif


	//Wires
	G4double wire_r_W    =10.*um;
	G4double wire_r_Al   =50.*um;
	G4double wire_length =61.8*cm/2;


	G4Tubs* WireSolid_W  =new G4Tubs("Al_wire",0,wire_r_W,wire_length,0,deltaPhi);
	 	  fAnode =new G4LogicalVolume(WireSolid_W,W,"Anode");

	G4Tubs* WireSolid_Al =new G4Tubs("Al_wire",0,wire_r_Al,wire_length,0,deltaPhi);
	 	fCathode =new G4LogicalVolume(WireSolid_Al,Al,"Cathod");
		  fField =new G4LogicalVolume(WireSolid_Al,Al,"Field");
		 fShield =new G4LogicalVolume(WireSolid_Al,Al,"Shield");

	G4int 		nWire=100;
	G4double    Wire_dphi=360./nWire*deg;
	G4double	Anode_r[4] ={118.0*mm,125.53*mm,133.3*mm,141.31*mm};
	G4double  Cathode_r[5] ={114.29*mm,121.71*mm,129.35*mm,137.24*mm,145.37*mm};
	G4double   Shield_r[2] ={110.0*mm,149.37*mm};
	G4double	Field_r[4] ={118.0*mm,125.53*mm,133.3*mm,141.31*mm};
	G4RotationMatrix Wire_Rot=G4RotationMatrix();

	char Wire_name[10];

	for(G4int layer=0;layer<4;layer++){
		G4ThreeVector Anode_Vec    =G4ThreeVector(0,Anode_r[layer],0);
		G4ThreeVector Field_Vec    =G4ThreeVector(0,Field_r[layer],0);
		G4ThreeVector Shield_Vec   =G4ThreeVector(0,Shield_r[0],0);

		if(layer<2){
			Shield_Vec.setY(Shield_r[0]);
		}
		else if(layer>=2){
			Shield_Vec.setY(Shield_r[1]);
		}

		if(layer==1 || layer==3)Shield_Vec.rotateZ(Wire_dphi/2);
		Field_Vec.rotateZ(Wire_dphi/2);
		for(G4int ii=0;ii<nWire;ii++){
			if(ii!=0){
				Anode_Vec.rotateZ(Wire_dphi);
				Field_Vec.rotateZ(Wire_dphi);
				Shield_Vec.rotateZ(Wire_dphi);
			}
			sprintf(Wire_name,"Anode%d",layer*100+ii);
			Anode_Pos[layer*100+ii]=new G4PVPlacement(G4Transform3D(Wire_Rot,Anode_Vec),fAnode,Wire_name,fCDCVolume,false,layer*100+ii);
			new G4PVPlacement(G4Transform3D(Wire_Rot,Field_Vec),fField,"Field",fCDCVolume,false,layer*100+ii);
			new G4PVPlacement(G4Transform3D(Wire_Rot,Shield_Vec),fShield,"Shield",fCDCVolume,false,layer*100+ii);
		}
	}
	for(G4int layer=0;layer<5;layer++){
		G4ThreeVector Cathode_Vec =G4ThreeVector(0,Cathode_r[layer],0);
		for(G4int ii=0;ii<2*nWire;ii++){
			Cathode_Vec.rotateZ(Wire_dphi/2.);
			new G4PVPlacement(G4Transform3D(Wire_Rot,Cathode_Vec),fCathode,"Cathode",fCDCVolume,false,layer*100+ii);
		}
	}

	//Pb foil//
	G4double Foil_r=17.0*cm;
	G4Tubs* Foilsolid =new G4Tubs("Foil",Foil_r,Foil_r+0.1*mm,wire_length,phiMin,deltaPhi);
		  fFoilVolume =new G4LogicalVolume(Foilsolid,Pb,"Foil");
		     Foil_Pos =new G4PVPlacement(0,G4ThreeVector(),fFoilVolume,"Foil",fCDCVolume,false,0);

	//BeamPipe
	G4double Pipe_r=45.*mm/2;

	G4Tubs* PipeSolid =new G4Tubs("Pipe",0,Pipe_r,TC_length,0,deltaPhi);
		  fPipeVolume =new G4LogicalVolume(PipeSolid,Plastic,"Pipe");
		     Pipe_Pos =new G4PVPlacement(0,G4ThreeVector(),fPipeVolume,"Pipe",fWorldVolume,false,0);

	G4Tubs* VacuumSolid =new G4Tubs("Vacuum",0,Pipe_r-1.5*mm,TC_length,0,deltaPhi);
	  	  fVacuumVolume =new G4LogicalVolume(VacuumSolid,Vacuum,"Vacuum");
		     Vacuum_Pos =new G4PVPlacement(0,G4ThreeVector(),fVacuumVolume,"Vacuum",fPipeVolume,false,0);

	G4double maxStep = 0.4*cm;
	fStepLimit = new G4UserLimits(maxStep);
	fCDCVolume->SetUserLimits(fStepLimit);

  SetColor();  //Original Method 

  // Always return the physical world

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "/CDC";
  TrackerSD* aTrackerSD = new TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  SetSensitiveDetector( fCDCVolume,  aTrackerSD );

  G4String SC_SDname = "/SC_SD";
  ScintSD* SC_SD=new ScintSD(SC_SDname,"SCHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(SC_SD);
  SetSensitiveDetector( fSCVolume,  SC_SD );
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Visual Setting//
void DetectorConstruction::SetColor(){

//	const G4Colour* Al_col=new G4Colour(128,140,138,0.5);
	const G4Colour* Al_col=new G4Colour(0.75,0.85,1.0,0.5);

#ifdef TC_VIS
//    G4VisAttributes* TC_atb=new G4VisAttributes(G4Color(0.9,0.9,0,0.5));
    G4VisAttributes* TC_atb=new G4VisAttributes(*Al_col);
    TC_atb->SetForceSolid(true);
    fTCVolume->SetVisAttributes(TC_atb);
#endif

#ifdef SC_VIS
//    G4VisAttributes* SC_atb=new G4VisAttributes(G4Color(0.9,0.9,0,0.5));
    G4VisAttributes* SC_atb=new G4VisAttributes(*Al_col);
    SC_atb->SetForceSolid(true);
    fSCVolume->SetVisAttributes(SC_atb);
#endif

#ifdef CDC_VIS
    G4VisAttributes* CDC_atb=new G4VisAttributes(G4Color(0.97,0.97,0.97,0.2));
    CDC_atb->SetForceSolid(true);
    fCDCVolume->SetVisAttributes(CDC_atb);
#endif

#ifdef FOIL_VIS
    G4VisAttributes* Foil_atb=new G4VisAttributes(G4Color(1.,0,0,0.3));
    Foil_atb->SetForceSolid(true);
    fFoilVolume->SetVisAttributes(Foil_atb);
#endif

#ifdef WIRE_VIS
    G4VisAttributes* Wire_atb=new G4VisAttributes(G4Color(0,0,1.));
    Wire_atb->SetForceSolid(true);
    fAnode->SetVisAttributes(Wire_atb);
    fField->SetVisAttributes(Wire_atb);
    fShield->SetVisAttributes(Wire_atb);
#endif

#ifdef PIPE_VIS
    G4VisAttributes* Pipe_atb=new G4VisAttributes(G4Color(0.1,0.1,0.1,0.2));
    fPipeVolume->SetVisAttributes(Pipe_atb);
#endif
} 

