#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

#define m_e electron_mass_c2

G4double PrimaryGeneratorAction::Distribute_Li(){
  G4double Z_i=3.0,  Q_b=13.1;
  G4double min_x, max_x, max_f;
  min_x = 0;  max_x = Q_b;
  max_f=1000;
  G4double E_0,E_e,p_e,beta;
  G4double ran_x,ran_f;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;                                                    
    ran_f = G4UniformRand()*max_f;
    E_e = ran_x;
    p_e = sqrt(pow(E_e,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_e),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_e),2)*sqrt(pow(E_e,2)-pow(m_e,2))*E_e;

    if (ran_f<efunc){
      j=1;
    }
  }
  return E_e;
}

G4double PrimaryGeneratorAction::Distribute_Sr(){
	G4double E_e;
	G4double rand;
	rand=G4UniformRand();
	if(rand>0.5){
		E_e=Sr_E();
	}else{
		E_e=Y_E();
	}
	return E_e;
}

G4double PrimaryGeneratorAction::Sr_E(){
  G4double Z_i=40.0,  Q_b=0.5459;
  G4double min_x, max_x, max_f;
  min_x = 0;  max_x = Q_b;
  max_f=1000;
  G4double E_0,E_Sr,p_e,beta;
  G4double ran_x,ran_f;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;
    ran_f = G4UniformRand()*max_f;
    E_Sr = ran_x;
    p_e = sqrt(pow(E_Sr,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_Sr),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_Sr),2)*sqrt(pow(E_Sr,2)-pow(m_e,2))*E_Sr;

    if (ran_f<efunc){
      j=1;
    }
  }
  return E_Sr;
}

G4double PrimaryGeneratorAction::Y_E(){
  G4double Z_i=39.0,  Q_b=2.280;
  G4double min_x, max_x, max_f;
  min_x = 0;  max_x = Q_b;
  max_f=1000;
  G4double E_0,E_Y,p_e,beta;
  G4double ran_x,ran_f;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;
    ran_f = G4UniformRand()*max_f;
    E_Y = ran_x;
    p_e = sqrt(pow(E_Y,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_Y),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_Y),2)*sqrt(pow(E_Y,2)-pow(m_e,2))*E_Y;

    if (ran_f<efunc){
      j=1;
    }
  }
  return E_Y;
}
