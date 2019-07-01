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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

//#define DIST_MODE

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
//#include "PGAMessenger.hh"
#include "Messenger.hh"
#include "G4SystemOfUnits.hh"
#include "config.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
  fMessenger(0)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
  fMessenger= new PGAMessenger(this);
  // default particle kinematic

  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("e-");

  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0));
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleEnergy(init_gunE*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double PrimaryGeneratorAction::E_decay;

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.

//  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0.,0.));
	switch(DistLevel){
		case 0:	
			break;
		case 1:
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(G4UniformRand()*2.-1.,G4UniformRand()*2.-1, G4UniformRand()*2.-1));
			break;
		case 2:
			Dist_E=Distribute_Li();
			fParticleGun->SetParticleEnergy(Dist_E);
			break;
		case 12:	
			Dist_E=Distribute_Li();
			fParticleGun->SetParticleEnergy(Dist_E);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(G4UniformRand()*2.-1.,G4UniformRand()*2.-1, G4UniformRand()*2.-1));
			break;
		case 3:
			Dist_E=Distribute_Sr();
//			Dist_E=Y_E();  ignore Sr
			fParticleGun->SetParticleEnergy(Dist_E);
			break;
		case 13:	
			Dist_E=Distribute_Sr();
			fParticleGun->SetParticleEnergy(Dist_E);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(G4UniformRand()*2.-1.,G4UniformRand()*2.-1, G4UniformRand()*2.-1));
			break;

		}
	
	E_decay=fParticleGun->GetParticleEnergy()/MeV; 
//	G4cout <<"now Level is " <<DistLevel <<G4endl;
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::SetDistLevel(G4int Level){
	DistLevel=Level;
}

G4double PrimaryGeneratorAction::GetInitEnergy(){
	return E_decay;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
