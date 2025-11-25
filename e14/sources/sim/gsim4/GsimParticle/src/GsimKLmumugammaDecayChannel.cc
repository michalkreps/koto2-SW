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
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "GsimParticle/GsimKLmumugammaDecayChannel.h"

double GsimMMGDecayChannel::calc_F1(double t){
	double bt  = sqrt(1-4*m_mp1*m_mp1/m_mkl/m_mkl);
	double btp = sqrt(fabs(1-4*m_mp1*m_mp1/t));
	double A = 1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(m_mp1*m_mp1*log((1+bt)/(1-bt))-0.5*bt*t);
	double Ap = 0;
	if(t>4*m_mp1*m_mp1) Ap = 1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(m_mp1*m_mp1*log((1+btp)/(1-btp))-0.5*btp*t);

	double Im = A-Ap;
		
	double Re;
	if(t<4*m_mp1*m_mp1){
		double I1 = -2/(m_mkl*m_mkl-t)/btp/t*atan(1/btp) + 1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(m_pi*m_pi/2 - pow(log((1+bt)/(1-bt)),2)/2-2*pow(atan(1/btp),2));
		double I2 = 1/(m_mkl*m_mkl-t)*(1/t - 4*m_mp1*m_mp1/t/t/btp*atan(1/btp)) + 1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(bt*log((1-bt)/(1+bt))+2*btp*atan(1/btp));
		Re = (m_mp1*m_mp1*I1-0.5*t*I2)/m_pi;
	}else{
				
		double J1 = 1/(m_mkl*m_mkl-t)*(1/t/btp*log((1+btp)/(1-btp))-1/t/btp/btp*log((1+btp)/(1-btp)) - 1/t/btp*log((t-4*m_mp1*m_mp1)/(m_mkl*m_mkl-4*m_mp1*m_mp1))) 
			+1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(0.5*pow(log((1+btp)/(1-btp)),2) -0.5*pow(log((1+bt)/(1-bt)),2) - log((1+btp)/(1-btp))*log((t-4*m_mp1*m_mp1)/(m_mkl*m_mkl-4*m_mp1*m_mp1)));
		double J2 = 1/(m_mkl*m_mkl-t)*(1/t+2*m_mp1*m_mp1/t/t/btp*log((1+btp)/(1-btp)) -2*m_mp1*m_mp1/t/t/btp*log((t-4*m_mp1*m_mp1)/(m_mkl*m_mkl-4*m_mp1*m_mp1))-1/t/btp)
			+1/(m_mkl*m_mkl-t)/(m_mkl*m_mkl-t)*(bt*log((1-bt)/(1+bt)) - btp*log((1-btp)/(1+btp)*(t-4*m_mp1*m_mp1)/(m_mkl*m_mkl-4*m_mp1*m_mp1)));
		Re = (m_mp1*m_mp1*J1-0.5*t*J2)/m_pi;
	}

	return sqrt(Im*Im + Re*Re);
}

double GsimMMGDecayChannel::calc_F2(double t){
	return (1-pow(asin(sqrt(t)/2/m_mn),2)/pow(asin(m_mkl/2/m_mn),2))/(1-t/m_mkl/m_mkl);
}

GsimMMGDecayChannel::GsimMMGDecayChannel(
					     const G4String& theParentName, 
					     G4double        theBR,
					     const G4String& theMumName1,
					     const G4String& theMupName2,
					     const G4String& theGammaName)
  :G4VDecayChannel("MMG Decay",theParentName,
		   theBR,  3,
		   theMumName1,  theMupName2,  theGammaName)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_mkl=0.;
  m_mp1=0.;
  m_ml=0.;
  m_mn=0.;
  m_eps=0.00228;
  
  m_pi = pi;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimMMGDecayChannel::~GsimMMGDecayChannel()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* GsimMMGDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  //if (parent == 0) {
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
  if(m_mn==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("proton");
    m_mn=particle->GetPDGMass()/MeV;
	std::cout << "mn=" << m_mn << "\n";
  }

  if(m_ml==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("mu-");
    m_ml=particle->GetPDGMass()/MeV;
  }



  /* ---------------------------------------------------------------------------
  
     The matrix element is taken from Phys. Rev. D7 3303 (1973).	 

	 t = mll^2 dependence on the differential decay rate is taken into
	 acount.

	 If two CP eigenstates do not interfere each other, each component
	 independently contributes to K->llgamma transition, thus we get
	 
	 dGamma/dt = (eps^2*dGamma(K1)/dt + dGamma(K2)/dt)/(1+eps^2)  (*), 
	 
	 where eps is the mixing parameter: KL = (K2+K1*eps)/sqrt(1+eps^2).
	 Since eps is very small ~2e-3, basically we can forget the first 
	 term of (*). However, for the completeness both contributions are 
	 implemented. Each term can be given by:

	 dGamam(Ki) propt (1-t/Mkl)^3 sqrt(1-4ml^2/t)*(1+2ml^2/t)*|Fi|^2/t 
	 (for i = 1,2),
	 
	 where F1 and F2 are complex form factors and given as a function of t.
	 They are analytic continuation of F1(t=0) and F2(t=0), hence this paper
	 gives F1(t) and F2(t) as ratios to F1(0) and F2(0). However,
	 this overall constant is not important and they are ignored.
	 
	 ImF1(t) and ReF1(t) are given by Eq.(8) and Eq.(12), respectively.
	 Note that the original expression of I1 in Eq.(13) is incorrect
	 if we respect the continuity of ReF1(t) in the vicinity of t=4mu^2 (mu=mpi).
	 The correct formula seems to be obtained if we swap beta -> beta'
	 in the last factor (atan(1/beta)). 
	 
	 For the F2(t) contribution, we use Eq.(16). 
  
  -----------------------------------------------------------------------------*/





  double t;
  double maj;
  double tmin = 4*m_ml*m_ml;
  double tmax = m_mkl*m_mkl;
  double prob;
  double F;

  if(G4UniformRand()<m_eps*m_eps){
  	  // K1 = (K0+K0Bar)/sqrt(2) component
	  // which is almost same as KS
	  while(1){
		  // t is distributed by the PDF(t) ~ 1/t
		  t = tmin*pow(tmax/tmin, G4UniformRand());
		  F = calc_F1(t);
		  maj = 3e-13; // l=e

		  prob = pow(1-t/m_mkl/m_mkl,3)*sqrt(1-4*m_ml*m_ml/t)*(1+2*m_ml*m_ml/t)*F*F;
		  if(maj<prob){
			  std::cout << "warning! (K1) prob/maj=" << prob/maj << "\n";
		  }
		  if(G4UniformRand()*maj<prob)break;
	  }
		   
  }else{
	  // K2 = (K0-K0Bar)/sqrt(2) component
	  // which is almost same as KL
	  while(1){
		  
		  // t is distributed by the PDF(t) ~ 1/t

		  t = tmin*pow(tmax/tmin, G4UniformRand());
		  F = calc_F2(t);
		  maj = 0.32; // l=mu
						
		  prob = pow(1-t/m_mkl/m_mkl,3)*sqrt(1-4*m_ml*m_ml/t)*(1+2*m_ml*m_ml/t)*F*F;
		  if(maj<prob){
		  	std::cout << "warning! (K2) prob/maj=" << prob/maj << "\n";
		  }
		  if(G4UniformRand()*maj<prob)break;
	  }

  } 

  double ph = (2*G4UniformRand()-1)*m_pi;
  double costh = 2*G4UniformRand()-1;
  
  double Ek = (m_mkl*m_mkl-t)/2/m_mkl;
  double Ell = (m_mkl*m_mkl+t)/2/m_mkl;
  double mll = sqrt(t);
  double plst  = sqrt(mll*mll/4-m_ml*m_ml);

  double Pl1[4];
  double Pl2[4];
  double Pg[4];
  
  // Now in the ll rest frame
  Pl1[0] = plst*sqrt(1-costh*costh)*cos(ph);
  Pl1[1] = plst*sqrt(1-costh*costh)*sin(ph);
  Pl1[2] = plst*costh;
  Pl1[3] = mll/2;
  
  Pl2[0] = -Pl1[0]; 
  Pl2[1] = -Pl1[1]; 
  Pl2[2] = -Pl1[2]; 
  Pl2[3] = mll/2;
  
  
  // boost leptons into z direction (ll rest --> kl rest)
  double bt = Ek/Ell; // Ek = Pll
  double gm = Ell/mll;
  
  double pz = Pl1[2];
  double e  = Pl1[3];
  
  Pl1[2] = gm*(pz+bt*e);
  Pl1[3] = gm*(bt*pz+e);
  
  pz = Pl2[2];
  e  = Pl2[3];
  
  Pl2[2] = gm*(pz+bt*e);
  Pl2[3] = gm*(bt*pz+e);
  
  Pg[0] = 0;
  Pg[1] = 0;
  Pg[2] = -Ek;
  Pg[3] =  Ek;
  
  // rotate all particles isotropically
  double th = acos(2*G4UniformRand()-1);
  ph = m_pi*(2*G4UniformRand()-1);
  double* ptr[3] = {Pl1, Pl2, Pg};
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
  
  
  // electron
  G4ThreeVector momentum0 =  G4ThreeVector(Pl1[0]*MeV,Pl1[1]*MeV,Pl1[2]*MeV);
  G4DynamicParticle * daughterparticle = new G4DynamicParticle(my_daughters[0], momentum0);
  products->PushProducts(daughterparticle);
  
  // positron
  G4ThreeVector momentum1 =  G4ThreeVector(Pl2[0]*MeV,Pl2[1]*MeV,Pl2[2]*MeV);
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

