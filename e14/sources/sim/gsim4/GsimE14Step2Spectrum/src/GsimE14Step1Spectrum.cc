/**
 *  $Id: 
 *  $Log:
 */

#include "GsimE14Step2Spectrum/GsimE14Step1Spectrum.h"
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




GsimE14Step1Spectrum::GsimE14Step1Spectrum(std::string name)
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


GsimE14Step1Spectrum::~GsimE14Step1Spectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pKLFunc;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14Step1Spectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

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

  double beamWidth=m_parameter;
  
  double PKaon = getKaonMomentum(z+m_detectorShiftZ);
  G4ThreeVector  Kdir(0,0,1);
  if(m_beamCondition>=201504) {
    getSquareProfile201504(zStart+m_detectorShiftZ,
			   xStart,yStart,cost,sint,phi);
    //direction will be modified to fit the beam size
    //Assume caloriemter z from target is (21+6.148)m
    double xEnd=xStart+CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
    double yEnd=yStart+CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
    double dx=xEnd-xStart;
    double dy=yEnd-yStart;
    double dz=(21+6.148)*m;
    double mag=std::sqrt(dz*dz+dx*dx+dy*dy);
    Kdir=G4ThreeVector(dx/mag,dy/mag,dz/mag);
  } else {
    xStart = xStart+CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
    yStart = yStart+CLHEP::RandFlat::shoot(-beamWidth/2.,beamWidth/2.);
  }

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


double GsimE14Step1Spectrum::pKLDistExit( double Pk )
{
  Pk=Pk/GeV;
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


double GsimE14Step1Spectrum::pKLDist(double* x,double* par)
{
  //x[0]: Pk with unit
  //par[0]: dZ with unit
  //50m beam line is default
  return pKLDistExit(x[0])*pKLWeight((x[0]),par[0]);
}



