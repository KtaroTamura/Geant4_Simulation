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
/// \file ScintSD.cc
/// \brief Implementation of the ScintSD class

#include "ScintSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintSD::ScintSD(const G4String& name,
                         const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintSD::~ScintSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection
	
	fHitsCollection
	  = new ScintHitsCollection(SensitiveDetectorName, collectionName[0]);
	
	// Add this collection in hce
	G4int hcID
	  = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection( hcID, fHitsCollection );
	
	//clear energy deposit buffer (Add by myself)
	for(G4int i=0; i<12;i++){HitSC[i]={0};TotalE[i]=0;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ProcessHits is like a trigger
G4bool ScintSD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
	// energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();
	
	G4bool fenter=aStep->IsFirstStepInVolume();
	G4int  Prime=aStep->GetTrack()->GetTrackID();
	G4int  SC_number=aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

	if (edep==0. || Prime!=1) return false;
//	if (edep==0.) return false;
	  for(G4int ii=0;ii<12;ii++){
	  	if(SC_number==ii){HitSC[ii]+=1;TotalE[ii]+=edep;}
	  }
//	if(Prime==1 && fenter==true && SC_number==0){
/*
	if(fenter==true && SC_number==0){
		ScintHit* newHit = new ScintHit();
		newHit->SetTrackID  (Prime);
		newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
		newHit->SetKineticE(aStep->GetTrack()->GetKineticEnergy());
		fHitsCollection->insert( newHit );
	}
*/
	//newHit->Print();
	
	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) {
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl
       << "-------->Hits Collection: in this event they are " << nofHits
       << " hits in the SCs: " << G4endl;
	 G4cout <<G4endl
			<<"Eloss in SC= " << TotalE <<" MeV"<<G4endl;
	 G4cout <<"Hit SC:";
	   for(G4int i=0; i<12;i++){if(HitSC[i]!=0)G4cout<< i+1 << ",";}
	G4cout << G4endl;
  }

	for(G4int kk=0;kk<12;kk++){
		if(TotalE[kk]>0){
		ScintHit* newHit=new ScintHit(kk+1,TotalE[kk]);	
		fHitsCollection->insert(newHit);
		}
	}
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
