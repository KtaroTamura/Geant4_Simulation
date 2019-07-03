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
  G4double E_total;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;                                                    
    ran_f = G4UniformRand()*max_f;
    E_e = ran_x;
	E_total=E_e+m_e;
    p_e = sqrt(pow(E_total,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_total),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_total),2)*sqrt(pow(E_total,2)-pow(m_e,2))*E_total;

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
  G4double E_total;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;
    ran_f = G4UniformRand()*max_f;
    E_Sr = ran_x;
	E_total=E_Sr+m_e;
    p_e = sqrt(pow(E_total,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_total),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_total),2)*sqrt(pow(E_total,2)-pow(m_e,2))*E_total;

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
  G4double E_total;

  G4int j=0;
  while(j==0){
    // ******** Electron Generation using Statistical Shape
    ran_x = G4UniformRand()*(max_x-min_x)+min_x;
    ran_f = G4UniformRand()*max_f;
    E_Y = ran_x;
	E_total=E_Y+m_e;
    p_e = sqrt(pow(E_total,2)-pow(m_e,2));
    beta = sqrt(1-pow((m_e/E_total),2));
    E_0 = m_e+Q_b;
    G4double eta = Z_i/beta/137;
    G4double fermi = eta/(1-exp(-2*pi*eta));
    G4double efunc = fermi*pow((E_0-E_total),2)*sqrt(pow(E_total,2)-pow(m_e,2))*E_total;

    if (ran_f<efunc){
      j=1;
    }
  }
  return E_Y;
}
