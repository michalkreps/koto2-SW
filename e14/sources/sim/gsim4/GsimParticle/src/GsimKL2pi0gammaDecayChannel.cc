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

#include "G4Version.hh"
#include "GsimParticle/GsimKL2pi0gammaDecayChannel.h"
#include "GsimPersistency/GsimMessage.h"

Gsim2Pi0GDecayChannel::Gsim2Pi0GDecayChannel(
					     const G4String& theParentName, 
					     G4double        theBR,
					     const G4String& thePionName1,
					     const G4String& thePionName2,
					     const G4String& theGammaName)
  :G4VDecayChannel("2Pi0G Decay",theParentName,
		   theBR,  3,
		   thePionName1,  thePionName2,  theGammaName)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_mKL=0.;
  m_mp0=0.;
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

Gsim2Pi0GDecayChannel::~Gsim2Pi0GDecayChannel()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* Gsim2Pi0GDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  

  //GetParent and GetDaughters call FillParent/Daughters
  G4ParticleDefinition* my_parent = GetParent();
  G4ParticleDefinition* my_daughters[3];
  my_daughters[0] = GetDaughter(0);
  my_daughters[1] = GetDaughter(1);
  my_daughters[2] = GetDaughter(2);


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

  if(m_mKL==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("kaon0L");
    m_mKL=particle->GetPDGMass()/MeV;
  }
  double mKL=m_mKL;
  if(m_mp0==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("pi0");
    m_mp0=particle->GetPDGMass()/MeV;
  }
  double mp0=m_mp0;
  
  
  
  
  //-----------------------------------------------------------------------------------------
  // Generate omega and costh
  // omega = energy of photon in the KL rest frame
  // th    = angle of photon in the pipi rest frame
  //         (or equivalently angle between pion and photon in the KL rest frame 
  // pipigamma spectrum:
  // dGamma/domega dcosth propto omega^5 beta^3 (1-2omega/Mk + 4mpi^2/Mk^2) sinth^2 costh^2 
  // The formula (3) in Phys. Lett. B307 182 (P. Heiliger and L. M. Sehgal) is incorrect!
  // The author misidentifies (1-2omega/Mk + 4mpi^2/mK^2) as (1-2omega/Mk - 4mpi^2/MK^2).
  //-----------------------------------------------------------------------------------------         
  
  double omega, costh;
  
  double beta;
  double prob;
  while(1){
    
    // The majorinzing constant (which is arbitrary as long as prob>maj is satisfied)
    const double maj = 2e9;
    
    double omega_max = (mKL*mKL-4*mp0*mp0)/2/mKL;
    
    omega = omega_max*G4UniformRand();
    costh = 2.0*G4UniformRand()-1;
    
    beta = sqrt((mKL*mKL-2*mKL*omega-4*mp0*mp0)/(mKL*mKL-2*mKL*omega));
    
    prob = omega*omega*omega*omega*omega*beta*beta*beta
      *(1-2*omega/mKL+4*mp0*mp0/mKL/mKL)*(1-costh*costh)*costh*costh;
    
    // in this algorithm, the efficiency of this loop is 17% (sufficiently fast).
    if(prob>G4UniformRand()*maj)break;
    
  }
  
  double pi = CLHEP::pi;
  double ph = (2*G4UniformRand()-1)*pi;
  
  double mpipisq = mKL*mKL-2*mKL*omega;
  double epi = sqrt(mpipisq)/2;
  double ppi = sqrt(mpipisq/4-mp0*mp0);

  
  double Ppi1[4];
  double Ppi2[4];
  double Pg[4];
  
  // Now the direction of pipi is z-axis
  Ppi1[0] = ppi*sqrt(1-costh*costh)*cos(ph);
  Ppi1[1] = ppi*sqrt(1-costh*costh)*sin(ph);
  Ppi1[2] = ppi*costh;
  Ppi1[3] = epi;
  
  Ppi2[0] = -Ppi1[0]; 
  Ppi2[1] = -Ppi1[1]; 
  Ppi2[2] = -Ppi1[2]; 
  Ppi2[3] = epi;
  
  
  // boost two pis into z direction
  double bt = (mKL*mKL-mpipisq)/(mKL*mKL+mpipisq);
  double gm = (mKL*mKL+mpipisq)/epi/mKL/4;
  
  double pz = Ppi1[2];
  double e  = Ppi1[3];
  
  Ppi1[2] = gm*(pz+bt*e);
  Ppi1[3] = gm*(bt*pz+e);
  
  pz = Ppi2[2];
  e  = Ppi2[3];
  
  Ppi2[2] = gm*(pz    + bt*e);
  Ppi2[3] = gm*(bt*pz + e);
  
  Pg[0] = 0;
  Pg[1] = 0;
  Pg[2] = -omega;
  Pg[3] =  omega;
  
  // rotate all particles uniformly
  double th = acos(2*G4UniformRand()-1);
  ph = pi*(2*G4UniformRand()-1);
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
  
  
  // pion1
  G4ThreeVector momentum0 =  G4ThreeVector(Ppi1[0]*MeV,Ppi1[1]*MeV,Ppi1[2]*MeV);
  G4DynamicParticle * daughterparticle 
    = new G4DynamicParticle(my_daughters[0], momentum0);
  products->PushProducts(daughterparticle);
  
  // pion2
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

