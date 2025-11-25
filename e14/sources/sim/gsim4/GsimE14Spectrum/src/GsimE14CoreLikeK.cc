/**
 *  $Id: 
 *  $Log:
 */

#include "GsimE14Spectrum/GsimE14CoreLikeK.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"
#include <cmath>




GsimE14CoreLikeK::GsimE14CoreLikeK(std::string name)
  : GsimE14SatoSpectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14CoreLikeK::~GsimE14CoreLikeK()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  

  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14CoreLikeK::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  

  double xEnd = 0;
  double yEnd = 0;
  double zEnd = 0;
  double cost=0;
  double sint=0;
  double phi=0;

  getCoreLikeProfile(zEnd,
		     xEnd,yEnd,cost,sint,phi);

  //Change to detector corrdinate
  zEnd = zEnd - (21e3+m_detectorShiftZ);//mm 
  
  G4ThreeVector vtx(xEnd*mm,yEnd*mm,zEnd*mm);
  G4ThreeVector Kdir(sint*cos(phi),sint*sin(phi),cost);

  double PKaon = getKaonMomentum(zEnd+m_detectorShiftZ);
  G4ThreeVector p = Kdir * PKaon;
  
  pv->SetPosition(vtx.x(), vtx.y(),vtx.z());
  G4PrimaryParticle*    particle    = pv->GetPrimary();
  particle->SetMomentum(p.x(),p.y(),p.z());

  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hE14Mome",p.mag()/GeV);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}




