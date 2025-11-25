/**
 *  $Id: 
 *  $Log:
 */

#include "GsimE14Step2Spectrum/GsimE14Step2IsoePointSpectrum.h"
#include "TF1.h"

#include "G4SystemOfUnits.hh"

#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"


#include "CLHEP/Random/Randomize.h"



#include <string>
#include <vector>
#include <cmath>




GsimE14Step2IsoePointSpectrum::GsimE14Step2IsoePointSpectrum(std::string name)
  : GsimE14Spectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hE14MomeMy","hE14MomeMy [GeV/c]",100,0,30);

  m_parameter=10.*cm;
  m_detectorShiftZ=0;

  m_pMin=5e-2*GeV;
  m_z=1000*m;
  m_wMax=1.;
  m_pKLDistExitZ=-1*m;
  
  m_pKLFunc = new TF1("pKLFunc",
		      pKLDist,0,30*GeV,1);
  m_pKLFunc->SetNpx(1000);
  m_pKLFunc->SetParameter(0,m_z);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14Step2IsoePointSpectrum::~GsimE14Step2IsoePointSpectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pKLFunc;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14Step2IsoePointSpectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  double zCSI=20e3;//mm = 20m
  double zBeamExit=43e3;//mm = 43m
  

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  G4ThreeVector vPos = pv->GetPosition();
  double x=vPos.x();
  double y=vPos.y();
  double z=vPos.z();
  
  double xStart = x;
  double yStart = y;
  double zStart = z;
  double cost=0;
  double sint=0;
  double phi=0;

  double beamWidth=m_parameter; // at the CsI
  double minCT = std::cos(std::atan(beamWidth*0.5/(zBeamExit+zCSI)));
  
  phi  = CLHEP::RandFlat::shoot(0.,2.*M_PI);
  cost = CLHEP::RandFlat::shoot(minCT,1.);
  sint = 1-cost*cost;
  if(sint<0) sint=0;
  else       sint=std::sqrt(sint);
  

  double PKaon = getKaonMomentum(z+m_detectorShiftZ);
  G4ThreeVector  Kdir(sint*std::cos(phi),sint*std::sin(phi),cost);

  xStart = xStart+zBeamExit/cost*sint*std::cos(phi);
  yStart = yStart+zBeamExit/cost*sint*std::sin(phi);
  
  G4ThreeVector vtx(xStart,yStart,zStart);
  G4ThreeVector p = Kdir * PKaon;
  pv->SetPosition(vtx.x(), vtx.y(),vtx.z());
  G4PrimaryParticle*    particle    = pv->GetPrimary();
  particle->SetMomentum(p.x(),p.y(),p.z());

  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hE14MomeMy",p.mag()/GeV);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


double GsimE14Step2IsoePointSpectrum::pKLDistExit( double Pk )
{
  //30m from Isoe's  MT

  Pk=Pk/GeV;
  
  double pKLDistExitPar[5] =
    { 1.57923e+02,
      2.94235e+00,
      1.67860e+00,
      -2.15910e-01,
      6.57169e-03};
  
  double sigma = pKLDistExitPar[2]*( 1 - (pKLDistExitPar[3] + Pk*pKLDistExitPar[4])*(Pk-pKLDistExitPar[1]) );
  double yval = pKLDistExitPar[0]*
    std::exp(-0.5*(Pk-pKLDistExitPar[1])*(Pk-pKLDistExitPar[1])/(sigma*sigma));
  
  double val =  yval;
  val=val/pKLDistExitPar[0];
  return val;
}


double GsimE14Step2IsoePointSpectrum::pKLDist(double* x,double* par)
{
  //x[0]: Pk with unit
  //par[0]: dZ with unit
  //50m beam line is default
  return pKLDistExit(x[0])*pKLWeight((x[0]+20*m),par[0]);
}



