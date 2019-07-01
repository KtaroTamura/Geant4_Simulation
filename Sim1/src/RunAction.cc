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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
//#include "G4Exception.hh"

extern std::ofstream ofsA;
extern std::ofstream ofsB;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
 : G4UserRunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	//Add by myself
#ifdef save_fileA
	char filenameA[40];
	G4int SetE=100;
	sprintf(filenameA,"./Analysis_room/Eloss.csv");
	ofsA.open(filenameA,std::ios::out);
	if(! ofsA.good()){
	  G4ExceptionDescription msg;
	      msg << "FILE OPEN ERROR! " << G4endl;
	      G4Exception("FILEA OPEN",
	                  "Code002", JustWarning, msg);
	}else{G4cout <<"FileA open succeeded!"<<G4endl<<G4endl;}
#endif

#ifdef save_fileB
	char filenameB[40];
	ofsB.open(filenameB,std::ios::out);
	if(! ofsB.good()){
	  G4ExceptionDescription msg;
	      msg << "FILE OPEN ERROR! " << G4endl;
	      G4Exception("FILEB OPEN",
	                  "Code002", JustWarning, msg);
	}else{G4cout <<"FileB open succeeded!"<<G4endl<<G4endl;}
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
#ifdef save_fileA
	ofsA.close();
#endif

#ifdef save_fileB
	ofsB.close();
#endif
	G4cout<<">> #events generated="
		  << aRun->GetNumberOfEvent()<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
