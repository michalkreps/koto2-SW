/**
 *  $Id: 
 *  $Log:
 */
#include "TF1.h"

#include "GsimE14Spectrum/GsimE14SatoSpectrum.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"

#include <string>
#include <vector>
#include <cmath>




GsimE14SatoSpectrum::GsimE14SatoSpectrum(std::string name)
  : GsimE14Spectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  //This was already made at GsimE14Spectrum()
  //GsimPersistencyManager::getPersistencyManager()
  //->createHistogram("hE14Mome","hE14Mome [GeV/c]",100,0,10);

  m_parameter=1.;

  m_pMin=5e-2*GeV;
  m_z=1000*m;
  m_wMax=1.;
  m_pKLDistExitZ=-1*m;

  if(m_pKLFunc) delete m_pKLFunc;
  m_pKLFunc = new TF1("pKLFunc",
		      pKLDist,0,10*GeV,1);
  m_pKLFunc->SetNpx(1000);
  m_pKLFunc->SetParameter(0,m_z);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14SatoSpectrum::~GsimE14SatoSpectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pKLFunc;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14SatoSpectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  G4ThreeVector vPos = pv->GetPosition();
  double x=vPos.x();
  double y=vPos.y();
  double z=vPos.z();

  double PKaon = getKaonMomentum(z+m_detectorShiftZ);

  double xStart = x;
  double yStart = y;
  double zStart = z;
  double cost=0;
  double sint=0;
  double phi=0;

  if(m_beamCondition>=201504) {
    getSquareProfile201504(zStart+m_detectorShiftZ,
			   xStart,yStart,cost,sint,phi);
  } else {
    getSquareProfile(zStart+m_detectorShiftZ,
		     xStart,yStart,cost,sint,phi);
  }

  G4ThreeVector vtx(xStart,yStart,zStart);
  
  G4ThreeVector  Kdir(sint*cos(phi),sint*sin(phi),cost);
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


double GsimE14SatoSpectrum::pKLDistExit( double Pk )
{
  Pk=Pk/GeV;
  //Pk[GeV/c]
  //30-GeV proton on Pt target
  //After 7-cm Pb absorber 
  //Fitted by Shiomi by using measured data
  //Yield
  //Ni Target: (1.94 +-0.05 +0.16-0.15)*1e7
  //Pt Target: (4.19 +00.09 +0.32-0.26)*1e7

  // Shiomi's value
  // double pKLDistExitPar[5] =
  //     { 1.74e8 ,
  //       1.37e+00 ,
  //       7.48e-01 ,
  //       -2.95e-01 ,
  //       1.68e-04 } ;  // MC(geant3) pt target result 2011/06/28


  // Sato's KL spectrum derived from KL->pi+pi- 
  // with spectrometer and chambers
  // http://kds.kek.jp/getFile.py/access?contribId=16&resId=0&materialId=slides&confId=8802
  double pKLDistExitPar[5] =
    { 1,
      1.41991,
      0.810237,
      -0.301413,
      0.017092};

  double sigma = pKLDistExitPar[2]*( 1 - (pKLDistExitPar[3] + Pk*pKLDistExitPar[4])*(Pk-pKLDistExitPar[1]) );
  double yval = pKLDistExitPar[0]*
    std::exp(-0.5*(Pk-pKLDistExitPar[1])*(Pk-pKLDistExitPar[1])/(sigma*sigma));
  
  double val =  yval;
  val=val/pKLDistExitPar[0];
  return val;
}

double GsimE14SatoSpectrum::pKLDist(double* x,double* par)
{
  //x[0]: Pk with unit
  //par[0]: dZ with unit
  return pKLDistExit(x[0])*pKLWeight(x[0],par[0]);
}



