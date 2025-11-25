/**
 *  @file
 *  @brief  GsimKPiGGDecayChannel
 *  $Id$
 *  $Log$
 */

#include "GsimParticle/GsimKPiGGDecayChannel.h"
#include "GsimTools/GsimKPiGGDecay.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4DecayProducts.hh"
#include "G4VDecayChannel.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "G4SystemOfUnits.hh"

GsimKPiGGDecayChannel::GsimKPiGGDecayChannel(
					     const G4String& theParentName, 
					     G4double        theBR,
					     const G4String& thePionName1,
					     const G4String& theGammaName2,
					     const G4String& theGammaName3)
  :G4VDecayChannel("KPiGG Decay",theParentName,
		   theBR,  3,
		   thePionName1,  theGammaName2,  theGammaName3)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_kpggd = new GsimKPiGGDecay();
  m_kpggd->pgg6init();

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimKPiGGDecayChannel::~GsimKPiGGDecayChannel()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_kpggd;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* GsimKPiGGDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  // if (parent == 0) {
  //   FillParent();
  // }
  G4ParticleDefinition* my_parent = GetParent();

  // fill daughter particles and their mass
  // if (daughters == 0) {
  //   FillDaughters();
  // }
  G4ParticleDefinition* my_daughters[3];
  for(int k=0;k<3;k++) {
    my_daughters[k] = GetDaughter(k);
  }
  
  //create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  G4DynamicParticle * parentparticle = new G4DynamicParticle( my_parent,
							      dummy, 0.0);
  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;

  //create daughter G4DynamicParticle

  double Ppi[4];
  double Pg2[4];
  double Pg3[4];
  m_kpggd->pi0ggdk(Ppi,Pg2,Pg3);
  
  // pion1
  G4ThreeVector momentum0 =  G4ThreeVector(Ppi[0]*MeV,Ppi[1]*MeV,Ppi[2]*MeV);
  G4DynamicParticle * daughterparticle 
    = new G4DynamicParticle( my_daughters[0], momentum0);
  products->PushProducts(daughterparticle);

  // gamma2
  G4ThreeVector momentum1 =  G4ThreeVector(Pg2[0]*MeV,Pg2[1]*MeV,Pg2[2]*MeV);
  daughterparticle = new G4DynamicParticle( my_daughters[1], momentum1);
  products->PushProducts(daughterparticle);

  // gamma3
  G4ThreeVector momentum2 =  G4ThreeVector(Pg3[0]*MeV,Pg3[1]*MeV,Pg3[2]*MeV);
  daughterparticle = new G4DynamicParticle( my_daughters[2], momentum2);
  products->PushProducts(daughterparticle);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return products;
}





