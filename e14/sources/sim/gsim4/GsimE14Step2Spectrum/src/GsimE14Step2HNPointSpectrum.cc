/**
 *  $Id: 
 *  $Log:
 */

#include "GsimE14Step2Spectrum/GsimE14Step2HNPointSpectrum.h"
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




GsimE14Step2HNPointSpectrum::GsimE14Step2HNPointSpectrum(std::string name)
  : GsimE14Spectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  //43m beamline
  //space 1m
  //detector z=0 start from 44m

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hE14MomeMy","hE14MomeMy [GeV/c]",100,0,30);

  m_parameter=15.*cm;//beam full size
  m_detectorShiftZ=0;
  m_pMin=5e-2*GeV;
  m_z=-1*m;//For default we will shoot KL from -1 m
  m_wMax=1.;
  m_pKLDistExitZ=0*m;//Nomura seed at Z=-5mm ~0 = 44m from target
  
  m_pKLFunc = new TF1("pKLFunc",
		      pKLDist,0,30*GeV,1);
  m_pKLFunc->SetNpx(1000);
  m_pKLFunc->SetParameter(0,m_z);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14Step2HNPointSpectrum::~GsimE14Step2HNPointSpectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pKLFunc;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14Step2HNPointSpectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  double zCSI=20e3;//mm = 20m
  double zSpace=1e3;//1m
  double zBeamExit=43e3;//mm = 43m
  

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  G4ThreeVector vPos = pv->GetPosition();
  double x=vPos.x();
  double y=vPos.y();
  double z=vPos.z();
  
  double xStart = x;
  double yStart = y;
  double zStart = z;

  double beamWidth=m_parameter; // at the CsI
  double xCSI  = CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
  double yCSI  = CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
  double zCSI_target  = zCSI+zSpace+zBeamExit;

  xStart = xStart+(z+zSpace+zBeamExit)/zCSI_target*xCSI;
  yStart = yStart+(z+zSpace+zBeamExit)/zCSI_target*yCSI;
  
  double PKaon = getKaonMomentum(z+m_detectorShiftZ);
  G4ThreeVector  Kdir(xCSI,yCSI,zCSI_target);
  Kdir=Kdir.unit();

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


double GsimE14Step2HNPointSpectrum::pKLDistExit( double Pk )
{
  Pk=Pk/GeV;
  double pKLDistExitPar[5] =
    {
      1.19274e+03,
      2.90893e+00,
      1.72179e+00,
      -2.01089e-01,
      5.61485e-03
    };
  
  double sigma = pKLDistExitPar[2]*( 1 - (pKLDistExitPar[3] + Pk*pKLDistExitPar[4])*(Pk-pKLDistExitPar[1]) );
  double yval = //pKLDistExitPar[0]*
    std::exp(-0.5*(Pk-pKLDistExitPar[1])*(Pk-pKLDistExitPar[1])/(sigma*sigma));
  
  double val =  yval;
  return val;
}


double GsimE14Step2HNPointSpectrum::pKLDist(double* x,double* par)
{
  //x[0]: Pk with unit
  //par[0]: dZ with unit
  return pKLDistExit(x[0])*pKLWeight(x[0],par[0]);
}



