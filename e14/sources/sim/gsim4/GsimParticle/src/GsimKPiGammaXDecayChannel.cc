
/**
 *  @file
 *  @brief  GsimKPiGammaXDecayChannel
 *  $Id: GsimKPiGammaXDecayChannel.cc,v 1.3 2006/12/06 19:25:52 nanjo Exp $
 *  $Log: GsimKPiGammaXDecayChannel.cc,v $
 *  Revision 1.3  2006/12/06 19:25:52  nanjo
 *  CVS variables.
 *
 */

#include "GsimParticle/GsimKPiGammaXDecayChannel.h"

#include "CLHEP/Random/Randomize.h"
#include "CLHEP/Vector/Rotation.h"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4DecayProducts.hh"
#include "G4SystemOfUnits.hh"

GsimKPiGammaXDecayChannel::GsimKPiGammaXDecayChannel(const G4String& theParentName,
						       G4double        theBR,
						       const G4String& thePionName1,
						       const G4String& thePionName2,
						       const G4String& thePionName3)
  :G4VDecayChannel("KPiGammaX Decay",theParentName,
		   theBR,  3,
		   thePionName1,  thePionName2,  thePionName3)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = 0;
  particle = particleTable->FindParticle("pi0");
  m_mp=particle->GetPDGMass()/GeV;
  particle = particleTable->FindParticle("kaon0L");
  m_mk=particle->GetPDGMass()/GeV;
  m_mmsqmax=std::pow(m_mk-m_mp,2.);///GeV
  m_wmax=getM2Weight( m_mmsqmax );
  
}
  
GsimKPiGammaXDecayChannel::~GsimKPiGammaXDecayChannel()
{
  ;
}


G4DecayProducts* GsimKPiGammaXDecayChannel::DecayIt(G4double)
{
  G4ParticleDefinition* my_parent = GetParent();
  G4ParticleDefinition* my_daughters[3];
  for(int k=0;k<3;k++) {
    my_daughters[k] = GetDaughter(k);
  }
  
  double mmsq=0;
  double rw=1;
  double wm2=0.;
  while ( rw > wm2 ) {
    mmsq = CLHEP::RandFlat::shoot(0.,m_mmsqmax);//GeV
    wm2=getM2Weight(mmsq);
    rw = CLHEP::RandFlat::shoot(0.,m_wmax);
  }

  //   0<mmsq<(mk-mp)^2
  //A)   : mk = sqrt( pp^2 + mp^2) + sqrt(pp^2+mmsq)
  //           (A)*(sqrt-sqrt)
  //B)   : mk * (sqrt -sqrt) = (pp^2+mp^2) -(pp^2-mmsq)=mp^2-mmsq
  //C)   : (mp^2-mmsq)/mk=sqrt-sqrt
  //A)+C): mk+(mp^2-mmsq)/mk=2sqrt(pp^2+mp^2)
  //       [(mk+(mp^2-mmsq)/mk)*0.5]^2-mp^2=pp^2
  //pp=sqrt( [ (mk+(mp^2-mmsq)/mk)*0.5]^2-mp^2 )
  
  
  double mom=std::sqrt(
		       std::pow( (m_mk+ (m_mp*m_mp - mmsq)/m_mk)*0.5,2.)
		       -m_mp*m_mp
		       );
  //                                   / gamma
  //    pi0       m2                  / theta
  // <------ -------->     mmsq rest /-----
  //                                /  A
  //                               /

  double Eg_CM   =std::sqrt(mmsq)/2.*GeV;
  double cosTh_CM=CLHEP::RandFlat::shoot(-1,1);
  double sinTh_CM=std::sqrt( 1 - cosTh_CM*cosTh_CM);
  
  CLHEP::HepLorentzVector pGamma_CM(Eg_CM*sinTh_CM,0,Eg_CM*cosTh_CM,Eg_CM);
  CLHEP::HepLorentzVector pA_CM(-Eg_CM*sinTh_CM,0,-Eg_CM*cosTh_CM,Eg_CM);

  double beta=mom/std::sqrt( mom*mom+mmsq );
  
  CLHEP::HepLorentzVector pGamma=pGamma_CM.boostZ(beta);
  CLHEP::HepLorentzVector pA=pA_CM.boostZ(beta);
  
  G4ThreeVector direction(1.0,0.0,0.0);
  G4DynamicParticle parentparticle( my_parent, direction, 0.0);
  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(parentparticle);


  G4ThreeVector momentum[3];
  momentum[0] =  G4ThreeVector(0,0,-mom*GeV);
  momentum[1] =  G4ThreeVector( pGamma.x(),pGamma.y(),pGamma.z());
  momentum[2] =  G4ThreeVector( pA.x(),pA.y(),pA.z());

  double psi  =  CLHEP::RandFlat::shoot(0.,2*M_PI)*rad;
  double phi   = CLHEP::RandFlat::shoot(0.,2*M_PI)*rad;
  double theta = (std::acos(2.*CLHEP::RandFlat::shoot(0.,1.)-1.));


  CLHEP::HepRotation rot(phi,theta,psi);

  for(int k=0;k<3;k++) {
    momentum[k]=rot*momentum[k];
    G4DynamicParticle * daughterparticle 
      = new G4DynamicParticle( my_daughters[k], momentum[k]);
    products->PushProducts(daughterparticle);
  }
  
  return products;
}


/**********************************************************************
 m^2(gamma,A) function in Kl->pi0gammaA decay
 in Eur. Phys. J. C (2020) 80 :824
**********************************************************************/
double GsimKPiGammaXDecayChannel::getM2Weight( double mmsq )
{
  //mmsq in GeV m(gamma,A)^2

  double MK=m_mk;
  double MP=m_mp;

  if(mmsq>m_mmsqmax) return 0;
  
  double weight=
    std::pow(mmsq,2.)*
    (std::pow(MK*MK-MP*MP-mmsq*mmsq,2.)-4*MP*MP*mmsq*mmsq);
  
  return weight;
}



