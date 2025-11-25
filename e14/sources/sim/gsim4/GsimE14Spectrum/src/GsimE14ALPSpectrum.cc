/**
 *  @file
 *  @brief  GsimE14ALPSpectrum
 *  $Id: $
 *  $Log: $
 */
#include "GsimE14Spectrum/GsimE14ALPSpectrum.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"


#include "TLeaf.h"

#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>



GsimE14ALPSpectrum::GsimE14ALPSpectrum(std::string name)
  : GsimSpectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hE14ALPMome","hE14ALPMome [GeV/c]",
		      300,0,30);

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hE14ALPR","hE14ALPR(mm)",
		      100,0,1000);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14ALPSpectrum::~GsimE14ALPSpectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14ALPSpectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  G4ThreeVector vPos = pv->GetPosition();
  G4PrimaryParticle*    particle    = pv->GetPrimary();
  double mom=particle->GetTotalMomentum();
  

  

  double zBL=21.507*m;//m
  double zFB=2.75*m;//m
  double rCal=1.9/2.*m;//m
  
  double maxT= std::atan( rCal/(zBL+zFB) );
  double ct = CLHEP::RandFlat::shoot(std::cos(maxT),1);
  double phi = CLHEP::RandFlat::shoot(0.,2.*M_PI);

  double R=(zBL+zFB)*std::tan(std::acos(ct));
  CLHEP::Hep3Vector vtx=CLHEP::Hep3Vector(R*std::cos(phi),
					  R*std::sin(phi),
					  zFB);

  pv->SetPosition(vtx.x(), vtx.y(),vtx.z());

  double st=1-ct*ct;
  if(st<0) st=0;
  else st=std::sqrt(st);

  CLHEP::Hep3Vector p(mom*st*std::cos(phi),
			mom*st*std::sin(phi),
			mom*ct);
  
  particle->SetMomentum(p.x(),p.y(),p.z());
  
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hE14ALPMome",p.mag()*1e-3);

  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hE14ALPR",R/mm);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



