/**
 *  @file
 *  @brief  GsimKPiPiNuNuDecayChannel
 *  $Id: 
 *  $Log: 
 */

#include "GsimParticle/GsimKPiPiNuNuDecayChannel.h"

#include "CLHEP/Random/Randomize.h"
#include "CLHEP/Vector/Rotation.h"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4DecayProducts.hh"
#include "G4SystemOfUnits.hh"

GsimKPiPiNuNuDecayChannel::GsimKPiPiNuNuDecayChannel(const G4String& theParentName,
						     G4double        theBR,
						     const G4String& thePiname0,
						     const G4String& thePiname1,
						     const G4String& theNuname0,
						     const G4String& theNuname1)
  :G4VDecayChannel("KPiPiNuNu Decay",theParentName,
		   theBR,  4,
		   thePiname0,thePiname1,theNuname0,theNuname1)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = 0;

  particle = particleTable->FindParticle("kaon0L");
  double mKL=particle->GetPDGMass();
  
  particle = particleTable->FindParticle("pi0");
  double mPi0=particle->GetPDGMass();
  
  m_event = new TGenPhaseSpace();
  
  Double_t masses[4];
  masses[0]= mPi0/GeV;
  masses[1]= mPi0/GeV;
  masses[2]= 0; //for neutrino
  masses[3]= 0; //for neutrino
  
  TLorentzVector W(0,0,0,mKL/GeV);
  m_event->SetDecay(W, 4, masses);


  m_F1=107.2;
  m_F2=-112.7;
  m_F3=-23.1;
  m_F4=34.7;
  m_F6=27.8;
  m_F7=24.7;
  m_wmax=0.24;
}
  
GsimKPiPiNuNuDecayChannel::~GsimKPiPiNuNuDecayChannel()
{
  delete m_event;
}


G4DecayProducts* GsimKPiPiNuNuDecayChannel::DecayIt(G4double)
{
  
  // if (parent == 0) {
  //   FillParent();
  // }
  G4ParticleDefinition* my_parent = GetParent();

  // if (daughters == 0) {
  //   FillDaughters();
  // }
  G4ParticleDefinition* my_daughters[4];
  for(int k=0;k<4;k++) {
    my_daughters[k] = GetDaughter(k);
  }
  
  generate();

  G4ThreeVector* direction = new G4ThreeVector(1.0,0.0,0.0);
  G4DynamicParticle * parentparticle = new G4DynamicParticle( my_parent, *direction, 0.0);
  delete direction;

  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;


  for(int k=0;k<4;k++) {
    TLorentzVector *lv = m_event->GetDecay(k);
    G4ThreeVector momentum =  G4ThreeVector(
					    lv->Px()*GeV,
					    lv->Py()*GeV,
					    lv->Pz()*GeV
					    );
    G4DynamicParticle * daughterparticle 
      = new G4DynamicParticle( my_daughters[k], momentum);
    products->PushProducts(daughterparticle);
  }
  
  return products;
}




void GsimKPiPiNuNuDecayChannel::generate()
{
  Double_t weight=0;
  Double_t rW=1;
  
  while(rW>weight) {
    weight = m_event->Generate();
    TLorentzVector *pPi0 = m_event->GetDecay(0);
    TLorentzVector *pPi1 = m_event->GetDecay(1);
    TLorentzVector *pNu0 = m_event->GetDecay(2);
    TLorentzVector *pNu1 = m_event->GetDecay(3);

    
    double wME=kx4gen(pPi0,pPi1,pNu0,pNu1);
    
    weight=weight*wME;

    if(weight>m_wmax) {
      //Warning
    }
    
    rW=CLHEP::RandFlat::shoot(0.,m_wmax);
  }
}

double GsimKPiPiNuNuDecayChannel::kx4gen(
					 const TLorentzVector *pPi0,
					 const TLorentzVector *pPi1,
					 const TLorentzVector *pNu0,
					 const TLorentzVector *pNu1)
{
  //     From E391a library
  //     Modified for e14 librarary by H.Nanjo, March 23, 2014
  //
  //     From KTEV library
  //     Modified for seperate use & K->pi0pi0nunu use by Jon Nix, May 17, 2005 
  //     
  //     THIS ROUTINE GENERATE NEUTRAL KE4 DECAY.
  //     REFERENCES: PHYS. REV. 168,1858; PAIS AND TREIMAN
  //     PHYS. REV. 137,B438; CABIBBO AND MAKSYMOWICZ
  //     BUNCH OF EXPERIMENTAL PAPER, SOME OF THEM ARE OK
  
  // 4-vector for Nu-Nu system
  TLorentzVector pNuNu = (*pNu0)+(*pNu1);

  // Boost pNu0 to the rest frame for the Nu-Nu system
  TVector3 vBoost = pNuNu.BoostVector();
  TLorentzVector pNu0AtNuNuRestFrame=(*pNu0);
  pNu0AtNuNuRestFrame.Boost(-vBoost);

  //Angle between pNuNu and pNu0 at the Nu-Nu rest frame.
  TVector3 pNu0VecAtNuNuRestFrame=pNu0AtNuNuRestFrame.Vect();
  TVector3 pNuNuVec=pNuNu.Vect();
  double TL=pNuNuVec.Angle(pNu0VecAtNuNuRestFrame);

  //Direction
  TVector3 pPi0Vec = (*pPi0).Vect();
  TVector3 pPi1Vec = (*pPi1).Vect();
  TVector3 pNu0Vec = (*pNu0).Vect();
  TVector3 pNu1Vec = (*pNu1).Vect();

  //normal vector w.r.t the each scattering plane
  TVector3 normVecPiPiPlane = (pPi0Vec.Cross(pPi1Vec)).Unit();
  TVector3 normVecNuNuPlane = (pNu0Vec.Cross(pNu1Vec)).Unit();

  //Angle between the two scattering plane
  double APHIF=normVecPiPiPlane.Angle(normVecNuNuPlane);

  //Calcualte the matrix element with form factor
  //     HERE COMES THE FORM-FACTOR:
  //     WE START WITH THE RESULTS OF K+E4 DECAY FROM PHYS. LETT.
  //     VOL.36B,P619.(GOOD ENOUGH FOR BACKGROUND STUDY AND FIRST
  //     ITERATION OF KE4 FORM-FACTOR STUDY)
  
  double AF1=m_F1;
  double AF2=m_F2*cos(2*TL);
  double AF3=m_F3*sin(TL)*sin(TL)*cos(2*APHIF);
  double AF4=m_F4*sin(2*TL)*cos(APHIF);
  double AF6=m_F6*cos(TL);
  double AF7=m_F7*sin(TL)*sin(APHIF);
  double FTOTAL=(AF1+AF2+AF3+AF4+AF6+AF7)/100.;

  return FTOTAL;
}
