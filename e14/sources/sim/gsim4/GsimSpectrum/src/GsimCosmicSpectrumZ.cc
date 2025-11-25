/**
 *  $Id$
 *  $Log$
 */
#include "TF2.h"
#include "TMath.h"

#include "GsimSpectrum/GsimCosmicSpectrumZ.h"
#include "GsimSpectrum/GsimCosmicSpectrumZMessenger.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimKernel/GsimUtil.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"

#include <string>
#include <vector>
#include <cmath>

GsimCosmicSpectrumZ::GsimCosmicSpectrumZ(std::string name)
  : GsimSpectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  delete messenger;
  messenger = new GsimCosmicSpectrumZMessenger(this);

  //Default parameter for /gps/position 0 0 5.25 m
  // to be checked.
  m_cosmicRayDX=2.5*m;
  m_cosmicRayDZ=7.5*m;

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hCosmicAngularDist","angle [degree]",100,0,90);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hCosmicMomentum","momentum [GeV]",100,0,20);
  
  // This function was obtained from an empirical formula give by D.Reyna
  // http://arxiv.org/abs/hep-ph/0604145
  // Recall that this improved parameterization applies to all zenith angles. Given
  // the available data, it can be considered valid for muon momenta p_mu > 1 GeV
  // and p_mu < 2000 GeV/cos(theta).
  
  m_pMuonFunc = new TF2("m_pMuonFunc",
			"sin(y)*cos(y)^3*[0]*"
			"pow(x,-([1]+[2]*log10(x*cos(y))+"
			"[3]*log10(x*cos(y))^2+"
			"[4]*log10(x*cos(y))^3))",
			1.,100,0.,TMath::Pi()/2.-0.00001);
  
  m_pMuonFunc->SetParameters(0.00253, 0.2455, 1.288, -0.2555, 0.0209);
  m_pMuonFunc->SetNpx(400);
  m_pMuonFunc->SetNpy(100);
  
  m_boundaryX=0;
  m_boundaryY=0;
  m_boundaryZ=0;
  
  m_isBoundaryChecked=false;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimCosmicSpectrumZ::~GsimCosmicSpectrumZ()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pMuonFunc;
  delete messenger;
  messenger=0;
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimCosmicSpectrumZ::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif


  //Get world boundary
  //  This should not be called at the constructor.
  //  The dimension of the world can be changed in the macro.
  //  Not supported the case the dimesion is changed again.
  
  if(!m_isBoundaryChecked) {
    GsimUtil* util = GsimUtil::getUtil();
    util->getWorldHalfXYZ(
			  m_boundaryX,
			  m_boundaryY,
			  m_boundaryZ
			  );
  }

  

  /////////////////////////////////////////////////////////
  //Get original primvary vertex
  
  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  
  /////////////////////////////////////////////////////////
  //Set momentum and direction












  double p_muon=0;
  double theta=0;
  m_pMuonFunc->GetRandom2(p_muon, theta);
  p_muon *= GeV;
  
  double cost = cos(theta);
  double sint = std::sqrt(1 -cost*cost);
  double phi =2* M_PI * CLHEP::RandFlat::shoot();
  G4ThreeVector  muon_dir = G4ThreeVector(sint*sin(phi),sint*cos(phi),cost);

  muon_dir.rotateX( 90.*deg);
  
  G4ThreeVector p = p_muon * muon_dir;
  
  G4PrimaryParticle* particle = pv->GetPrimary();
  particle->SetMomentum(p.x(),p.y(),p.z());

  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hCosmicMomentum",p.mag()/GeV);
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hCosmicAngularDist",p.theta()/deg);

  /////////////////////////////////////////////////////////
  //Set position
  //  1st set the passing through point with random number.
  //  2nd set the origin extrapolating to the world boundary
  
  double xIni = pv->GetPosition().x();
  double yIni = pv->GetPosition().y();
  double zIni = pv->GetPosition().z();

  //------------------------------------------------------
  // generate passing through points with random number
  double x=xIni;
  double y=yIni;
  double z=zIni;
  x=CLHEP::RandFlat::shoot(-1.*m_cosmicRayDX,m_cosmicRayDX)+xIni;
  z=CLHEP::RandFlat::shoot(-1.*m_cosmicRayDZ,m_cosmicRayDZ)+zIni;
  double pVec[3]= {x,y,z}; // passing point vector

  //------------------------------------------------------
  //Set anti direction of cosmic-ray -> aVec
  double aVec[3]= {
    (-1.)*muon_dir.x(),
    (-1.)*muon_dir.y(),
    (-1.)*muon_dir.z()
  };

  //------------------------------------------------------
  //Select boundary wall w.r.t. the aVec -> bVec
  double bVec[3] = {
    ((aVec[0]>0)?1:-1) *  m_boundaryX,
    ((aVec[1]>0)?1:-1) *  m_boundaryY,
    ((aVec[2]>0)?1:-1) *  m_boundaryZ
  };

  double absB[3]; // absolute value of the boundary
  for(int i=0;i<3;i++) {
    absB[i]=std::fabs(bVec[i]);
  }

  //------------------------------------------------------
  //Get factor of vector a to reach the boundary -> tVec
  
  bool isSolution=false;
  double tVec[3]={DBL_MAX,DBL_MAX,DBL_MAX};//solution canditate
  double tValid = DBL_MAX;//final solution

  //get x,y,z boundary points candidste -> check it is solution
  for(int i=0;i<3;i++) {
    
    if(aVec[i]!=0) { // avoid zero division
      isSolution=true;
      
      //get boundary points candidste
      tVec[i]=(bVec[i]-pVec[i])/aVec[i];
      
      //check it is the soultion
      for(int k=0;k<3;k++) {
	double boundaryReachPos=tVec[i]*aVec[k]+pVec[k];
	if(k==i) continue; //skip boundary coordinate
	
	if( boundaryReachPos >= -absB[k] &&
	    boundaryReachPos <= absB[k]) {
	  // inside the boundary plane -> condition of the solution
	} else  {
	  // outside the boundary plane -> not a solution
	  isSolution=false;
	}
      }
      
      if(isSolution) {
	//if it is a solution, break;
	tValid=tVec[i];
	break;
      }
    }
  }
    
  if(!isSolution) {
    std::string msg = "CR totally outside the world but shoot anyway. ";
    msg+="Please re-consider the follwoing parameters for economy. ";
    msg+="setCosmicRayDX and setCosmicRayDY and /gps/position";
    GsimMessage::getInstance()
      ->report("warning",msg);
    tValid=tVec[1];//set y plane as a solution
  }

  //Set origin points
  x=aVec[0]*tValid+x;
  y=aVec[1]*tValid+y;
  z=aVec[2]*tValid+z;
  
  pv->SetPosition(x,y,z);
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

