/**
 *  @file
 *  @brief  GsimKL2pi0gammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */


#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayProducts.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "G4SystemOfUnits.hh"

#include "GsimParticle/GsimKLpipigammaDecayChannel.h"


GsimPiPiGDecayChannel::GsimPiPiGDecayChannel(
					     const G4String& theParentName, 
					     G4double        theBR,
					     const G4String& theElectronName1,
					     const G4String& thePositronName2,
					     const G4String& theGammaName)
  :G4VDecayChannel("PiPiG Decay",theParentName,
		   theBR,  3,
		   theElectronName1,  thePositronName2,  theGammaName)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_mkl=0.;
  m_mp1=0.;
  m_pi = CLHEP::pi;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimPiPiGDecayChannel::~GsimPiPiGDecayChannel()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* GsimPiPiGDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  //  if (parent == 0) {
  //    FillParent();
  //  }
  G4ParticleDefinition* my_parent = GetParent();

  // fill daughter particles and their mass
  //  if (daughters == 0) {
  //    FillDaughters();
  //  }
  G4ParticleDefinition* my_daughters[3];
  for(int k=0;k<3;k++) {
    my_daughters[k] = GetDaughter(k);
  }



  //create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  G4DynamicParticle * parentparticle 
    = new G4DynamicParticle(my_parent, dummy, 0.0);
  
  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;
  
  //KL and pi0 mass from Geant4
  //double mKL = 497.614;//MeV
  //double mp0 = 134.9766;//MeV

  if(m_mkl==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("kaon0L");
    m_mkl=particle->GetPDGMass()/MeV;
  }
  if(m_mp1==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("pi-");
    m_mp1=particle->GetPDGMass()/MeV;
  }


  /* ---------------------------------------------------------------------------
  
     The matrix element is taken from Phys. Rev. Lett. 83 4933 (1999).	 

	 photon energy dependence on the differential decay rate is taken into
	 acount.

	 dGamma(KL->pipigamma)/dEg/dcosth propt (Eg/Mkl)^3*(1-4ml^2/mpipi^2)^3*(1-2Eg/Mkl)cos^2th

	 where, th is an angle of pion inside pipi rest frame with respect to gamma.
	 
   -----------------------------------------------------------------------------*/


  double Eg;
  double Mpipisq;
  double maj;
  double Egmax = (m_mkl*m_mkl-4*m_mp1*m_mp1)/2/m_mkl;
  double prob;

  while(1){
	  // t is distributed by the PDF(t) ~ t^3
	  Eg = Egmax*sqrt(sqrt(G4UniformRand()));
	  Mpipisq = m_mkl*(m_mkl-2*Eg);
	  maj = 0.6; 

	  prob = (1-4*m_mp1*m_mp1/Mpipisq)*sqrt(1-4*m_mp1*m_mp1/Mpipisq)*(1-2*Eg/Mpipisq);
	  if(maj<prob){
		  std::cout << "warning! prob/maj=" << prob/maj << "\n";
	  }
	  if(G4UniformRand()*maj<prob)break;
  }
	   
 
  // th is distributed according to sinth^3 
  double costh = cos(asin(sqrt(sqrt(G4UniformRand()))));
  double ph = (2*G4UniformRand()-1)*m_pi;
  
  double Epipi = (m_mkl*m_mkl+Mpipisq)/2/m_mkl;
  double mpipi = sqrt(Mpipisq);
  double ppist  = sqrt(Mpipisq/4-m_mp1*m_mp1);

  double Ppi1[4];
  double Ppi2[4];
  double Pg[4];
  
  // Now in the pipi rest frame
  Ppi1[0] = ppist*sqrt(1-costh*costh)*cos(ph);
  Ppi1[1] = ppist*sqrt(1-costh*costh)*sin(ph);
  Ppi1[2] = ppist*costh;
  Ppi1[3] = mpipi/2;
  
  Ppi2[0] = -Ppi1[0]; 
  Ppi2[1] = -Ppi1[1]; 
  Ppi2[2] = -Ppi1[2]; 
  Ppi2[3] = mpipi/2;
  
  
  // boost leptons into z direction (pipi rest --> kl rest)
  double bt = Eg/Epipi; // Eg = Ppipi
  double gm = Epipi/mpipi;
  
  double pz = Ppi1[2];
  double e  = Ppi1[3];
  
  Ppi1[2] = gm*(pz+bt*e);
  Ppi1[3] = gm*(bt*pz+e);
  
  pz = Ppi2[2];
  e  = Ppi2[3];
  
  Ppi2[2] = gm*(pz+bt*e);
  Ppi2[3] = gm*(bt*pz+e);
  
  Pg[0] = 0;
  Pg[1] = 0;
  Pg[2] = -Eg;
  Pg[3] =  Eg;
  
  // rotate all particles isotropically
  double th = acos(2*G4UniformRand()-1);
  ph = m_pi*(2*G4UniformRand()-1);
  double* ptr[3] = {Ppi1, Ppi2, Pg};
  for(int i=0;i<3;i++){
    
    double* v = ptr[i];
    
    double x,y,z;
    x = v[0];
    y = v[1];
    z = v[2];
    
    // rotate by th around y
    v[0] =  cos(th)*x + sin(th)*z;
    v[2] = -sin(th)*x + cos(th)*z;
    
    x = v[0];
    z = v[2];
    
    // rotate by ph around z
    v[0] = cos(ph)*x - sin(ph)*y;
    v[1] = sin(ph)*x + cos(ph)*y;
  }
  
  
  // pim
  G4ThreeVector momentum0 =  G4ThreeVector(Ppi1[0]*MeV,Ppi1[1]*MeV,Ppi1[2]*MeV);
  G4DynamicParticle * daughterparticle = new G4DynamicParticle(my_daughters[0], momentum0);
  products->PushProducts(daughterparticle);
  
  // pip
  G4ThreeVector momentum1 =  G4ThreeVector(Ppi2[0]*MeV,Ppi2[1]*MeV,Ppi2[2]*MeV);
  daughterparticle = new G4DynamicParticle(my_daughters[1], momentum1);
  products->PushProducts(daughterparticle);
  
  // gamma
  G4ThreeVector momentum2 =  G4ThreeVector(Pg[0]*MeV,Pg[1]*MeV,Pg[2]*MeV);
  daughterparticle = new G4DynamicParticle(my_daughters[2], momentum2);
  products->PushProducts(daughterparticle);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return products;
}

