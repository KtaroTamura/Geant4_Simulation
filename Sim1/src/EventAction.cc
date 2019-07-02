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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "TrackerHit.hh"
#include "ScintHit.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"
#include "PrimaryGeneratorAction.hh"

class PrimaryGeneratorAction;
extern std::ofstream ofsA;
extern std::ofstream ofsB;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace{
	// Utility function which finds a hit collection with the given Id
	// and print warnings if not found 
	G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
	  auto hce = event->GetHCofThisEvent();
	  if (!hce) {
	      G4ExceptionDescription msg;
	      msg << "No hits collection of this event found." << G4endl; 
	      G4Exception("EventAction::EndOfEventAction()",
	                  "Code001", JustWarning, msg);
	      return nullptr;
	  }
	
	  auto hc = hce->GetHC(collId);
	  if ( ! hc) {
	    G4ExceptionDescription msg;
	    msg << "Hits collection " << collId << " of this event not found." << G4endl; 
	    G4Exception("EventAction::EndOfEventAction()",
	                "Code001", JustWarning, msg);
	  }
	  return hc;  
	}
}


EventAction::EventAction()
: G4UserEventAction(),fPGAction(NULL)
{
	G4cout<<"Constrcuta"<<G4endl;
	fPGAction=new PrimaryGeneratorAction();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
	delete fPGAction;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* )
{
//	G4cout <<"Begin This particle Energy is" <<G4BestUnit(init_E,"Energy") <<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	Calorimeter(event);
/*
	// get number of stored trajectories	
	G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

	// periodic printing
	G4int eventID = event->GetEventID();
	if ( eventID < 100 || eventID % 100 == 0) {
	  G4cout << ">>> Event: " << eventID  << G4endl;
	  if ( trajectoryContainer ) {
	    G4cout << "    " << n_trajectories
	           << " trajectories stored in this event." << G4endl;
	  }
	}
*/

//	G4bool VIEW=false;
//	if(G4EventManager::GetEventManager()->GetVerboseLevel()>0)(VIEW=true);
/*
	auto CDC_hc=GetHC(event,0);
	auto SC_hc=GetHC(event,1);
	G4int hitnum_CDC =CDC_hc->GetSize();
	G4int hitnum_SC  =SC_hc->GetSize();
	if(hitnum_CDC==0)return;
*/
}

void EventAction::Calorimeter(const G4Event* event)
{
	// get number of stored trajectories	
	G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

	G4int eventID=event->GetEventID();
	init_E=fPGAction->GetInitEnergy();

	auto CDC_hc=GetHC(event,0);
	auto SC_hc=GetHC(event,1);
	G4int hitnum_CDC =CDC_hc->GetSize();
	if(hitnum_CDC==0 ||hit_num_SC==0 )return;

	G4double end_E;
	auto SC_hit=static_cast<TrackerHit*>(SC_hc->GetHit(0));
	end_E=SC_hit->GetKineticE();

#ifdef save_fileA
	ofsA<<eventID<<","<<init_E<<","<<end_E<<std::endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
