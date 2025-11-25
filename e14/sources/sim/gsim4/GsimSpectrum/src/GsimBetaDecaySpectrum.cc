/**
 *  $Id$
 *  $Log$
 */

#include "GsimSpectrum/GsimBetaDecaySpectrum.h"
#include "GsimSpectrum/GsimBetaDecaySpectrumMessenger.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimTools/GsimBetaDecay.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"

#include <string>
#include <cmath>

GsimBetaDecaySpectrum::GsimBetaDecaySpectrum(std::string name)
  : GsimSpectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif


  m_betaDecay[0]=0;
  m_betaDecay[1]=0;

  //90_38 Sr -> 90_39Y -> 90_40Zr
  m_nCascade=2;
  
  m_A[0]=90;
  m_Z[0]=-39;
  m_Q[0]=0.546;

  m_A[1]=90;
  m_Z[1]=-40;
  m_Q[1]=2.282;
  
  update();

  m_sourceR=3*mm;
  m_sourceTheta=M_PI/2.;

  delete messenger;
  messenger = new GsimBetaDecaySpectrumMessenger(this);

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hBetaKineticEnergy","Ekin (MeV)",1000,0,10);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimBetaDecaySpectrum::~GsimBetaDecaySpectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  for(int i=0;i<2;i++) {
    if(m_betaDecay[i]) delete m_betaDecay[i]; 
  }
  delete messenger;
  messenger=0;
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimBetaDecaySpectrum::update()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::string nam[2]={"first","second"};
  for(int i=0;i<m_nCascade;i++) {
    if(!m_betaDecay[i]) m_betaDecay[i] = new GsimBetaDecay(nam[i]);
    m_betaDecay[i]->fillBetaSpectrum(m_A[i],m_Z[i],m_Q[i]);//<--here
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

  
void GsimBetaDecaySpectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  /////////////////////////////////////////////////////////
  //Get original primvary vertex
  
  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  
  /////////////////////////////////////////////////////////
  //Set momentum and direction

  double pMag=0;
  if(m_nCascade==1) {
    pMag = m_betaDecay[0]->generateBetaMomentum();
  } else {
    if(CLHEP::RandFlat::shoot()<0.5) {
      pMag = m_betaDecay[0]->generateBetaMomentum();
    } else {
      pMag = m_betaDecay[1]->generateBetaMomentum();
    }
  }
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hBetaKineticEnergy",pMag);
  
  pMag= pMag*MeV;
  
  // 1 - 0   -   1  -  cos(m_sourceTheta);
  double dcos= 1. - cos(m_sourceTheta);
  double cost = 1. - dcos* CLHEP::RandFlat::shoot();
  
  double sint = 1-cost*cost;
  if(sint<0) sint=0;
  else       sint=sqrt(sint);
  double phi =2* M_PI * CLHEP::RandFlat::shoot();
  G4ThreeVector  dir = G4ThreeVector(sint*sin(phi),sint*cos(phi),cost);
  G4ThreeVector pVec = pMag * dir;
  
  G4PrimaryParticle* particle = pv->GetPrimary();
  particle->SetMomentum(pVec.x(),pVec.y(),pVec.z());

  double xIni = pv->GetPosition().x();
  double yIni = pv->GetPosition().y();
  double zIni = pv->GetPosition().z();
  
  double r   =std::sqrt(CLHEP::RandFlat::shoot());
  phi =2* M_PI * CLHEP::RandFlat::shoot();
  //Set origin points
  double x=xIni+r*std::cos(phi);
  double y=yIni+r*std::sin(phi);
  double z=zIni;
  
  pv->SetPosition(x,y,z);
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

