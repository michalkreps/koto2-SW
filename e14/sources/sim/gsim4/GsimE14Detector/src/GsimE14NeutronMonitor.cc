/**
 *  @file
 *  @brief  GsimE14NeutronMonitor
 *  $Id: $ 
 *  $Log: $
 */
#include "GsimE14Detector/GsimE14NeutronMonitor.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimTools/GsimWinstonCone.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "GsimGun/GsimHEPEvtInterface.h"

#include "CLHEP/Random/Randomize.h"

#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4VTouchable.hh"
#include "G4NavigationHistory.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4SystemOfUnits.hh"

#include "TVector3.h"
#include "TMath.h"



#include <sstream>

using namespace E14;

GsimE14NeutronMonitor::GsimE14NeutronMonitor(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14NeutronMonitor";

  double tV=1*mm;
  double wV=139*mm-0.1*mm;
  double zV=-500*mm+0.1*mm+tV/2.;

  std::vector<double> paramVec;
  paramVec.push_back( wV );
  paramVec.push_back( wV );
  paramVec.push_back( tV );

  m_nTagVName="nTagV";

  GsimBox* nTagV = new GsimBox( m_nTagVName, this,
				G4ThreeVector(0,0,zV),
				G4ThreeVector(0,0,0) );
  nTagV->setParameters( paramVec );
  nTagV->setOuterMaterial( "G4_Galactic" );
  addDaughter( nTagV );

  {
    //double mm=1.;
    //double cm=10.;
    //Beam window
    m_targetZ[0]=-500*mm;
    m_targetD[0]=139*mm/2.;
    
    //NCC beam hole
    m_targetZ[1]=(275-31.5-44.66/2.)*cm-446/2.*mm-3*mm;
    m_targetD[1]=130*mm/2.;
    
    //Vertical membrane and CsI G10
    m_targetZ[2]=614.8*cm-52.3*mm-250.8*mm-3*mm-310*mm;
    m_targetD[2]=145*mm/2.;
    
    //CC04 G10
    m_targetZ[3]=(614.8 + 95.5+31.2)*cm+(7.+0.03)*3.*cm+200*mm-900*mm;
    m_targetD[3]=159*mm/2.;
    
    //Beam pipe
    m_targetZ[4]=614.8*cm+500*mm+3480.*mm-1555*mm;
    m_targetD[4]=190*mm/2.;
    
    //Beam window
    m_targetZ[5]=614.8*cm+500*mm+3480.*mm;
    m_targetD[5]=190*mm/2.;
  }

  m_isBeamShiftObtained=false;
  m_isKillNoHitInVacuum=false;
  m_isKillCore=false;

  m_detectorShiftZ=0;
  m_beamShiftX=0;
  m_beamShiftY=0;
}  


GsimE14NeutronMonitor::~GsimE14NeutronMonitor()
{
  
}

void GsimE14NeutronMonitor::beginOfRunAction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_tree = new TTree("nTree","neutron track");
  m_tree->Branch("eventID",&m_eventID,"eventID/I");
  m_tree->Branch("trackID",&m_trackID,"trackID/I");
  m_tree->Branch("parentID",&m_parentID,"parentID/I");
  m_tree->Branch("nChange",&m_nChange,"nChange/I");
  m_tree->Branch("vName",&m_vName,"vName/C");
  m_tree->Branch("vPosPre","TVector3",&m_vPosPre);
  m_tree->Branch("vPosPost","TVector3",&m_vPosPost);
  m_tree->Branch("pPre","Vector3",&m_pPre);
  m_tree->Branch("pPost","Vector3",&m_pPost);
  m_tree->Branch("dP",&m_dP,"dP/D");
  m_tree->Branch("eDep",&m_eDep,"eDep/D");
  m_tree->Branch("flag",&m_flag,"flag/I");
  m_tree->Branch("isCore",&m_isCore,"isCore/I");
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14NeutronMonitor::endOfRunAction()
{
  GsimPersistencyManager* pm = GsimPersistencyManager::getPersistencyManager();
  pm->createHistogram("h0","h0",1,0,1);//dummy for TFile::cd();
  
  m_tree->Write();
  m_isBeamShiftObtained=false;
}

bool GsimE14NeutronMonitor::beginOfEventAction(const G4Event* evt)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_nChange=0;
  m_eventID=evt->GetEventID();
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  
  return true;
}

bool GsimE14NeutronMonitor::preTrackingAction(const G4Track* aTrack)
{
  int pid = aTrack->GetParticleDefinition()->GetPDGEncoding();
  if(pid!=2112) return true;

  m_trackID=aTrack->GetTrackID();
  m_parentID=aTrack->GetParentID();
  G4ThreeVector p=aTrack->GetMomentum();
  m_pPost=TVector3(p.x(),p.y(),p.z());
  m_pPre=TVector3(p.x(),p.y(),p.z());
  m_dP=0;
  m_eDep=0;

  const G4ThreeVector pp=aTrack->GetVertexPosition();
  m_vPosPre=TVector3(pp.x(),pp.y(),pp.z());
  m_vPosPost=m_vPosPre;

  G4VPhysicalVolume* pv=aTrack->GetVolume();
  
  std::string sName = std::string( pv->GetName().data() );
  std::list<std::string> ar=split(sName,'/');
  std::list<std::string>::iterator it=ar.end();
  it--;
  
  std::sprintf(m_vName,"%s",(*it).c_str());

  m_isCore=isCore(m_pPost,m_vPosPost);

    
  m_flag=0;
  m_tree->Fill();
  m_nChange++;
  
  return true;
}


bool GsimE14NeutronMonitor::steppingAction(const G4Step* theStep)
{
  G4Track* aTrack = theStep->GetTrack();
  int pid = aTrack->GetParticleDefinition()->GetPDGEncoding();
  G4VPhysicalVolume* pv=0;
  std::string sName ="";
  
  if(pid==2112 || pid==22) {
    //stable particle 
    pv=theStep->GetPreStepPoint()->GetPhysicalVolume();
    sName=std::string( pv->GetName().data() );
    std::list<std::string> ar=split(sName,'/');
    std::list<std::string>::iterator it=ar.end();
    it--;
    sName=(*it);

    if(m_isKillCore) {
      if( sName == m_nTagVName ) {
	//inside tag volume
	G4ThreeVector p=theStep->GetPreStepPoint()->GetMomentum();
	TVector3 pV(p.x(),p.y(),p.z());
	
	p=theStep->GetPreStepPoint()->GetPosition();
	TVector3 rV(p.x(),p.y(),p.z());
	if( isCore(pV,rV)==1 ) {
	  //core
	  aTrack->SetTrackStatus(fStopAndKill);
	} else {
	  //non-core
	}
      }
    }
      
    if(m_isKillNoHitInVacuum) {
      if( sName == m_nTagVName ) {
	//inside tag volume
	G4ThreeVector p=theStep->GetPreStepPoint()->GetMomentum();
	TVector3 pV(p.x(),p.y(),p.z());
	
	p=theStep->GetPreStepPoint()->GetPosition();
	TVector3 rV(p.x(),p.y(),p.z());
	if( isHitMBN(pV,rV) ) {
	  //make hit
	} else {
	  //goto ds beam window
	  //kill it
	  aTrack->SetTrackStatus(fStopAndKill);
	}
      }
    }
  }
  if(pid!=2112) return true;

  ////////////////////////////////////////
  m_trackID=aTrack->GetTrackID();
  m_parentID=aTrack->GetParentID();
  G4ThreeVector p=theStep->GetPreStepPoint()->GetMomentum();
  m_pPre=TVector3(p.x(),p.y(),p.z());
  p=theStep->GetPostStepPoint()->GetMomentum();
  m_pPost=TVector3(p.x(),p.y(),p.z());
  TVector3 dpv=m_pPost-m_pPre;
  m_dP=dpv.Mag();
  if(m_dP>0) {
    p=theStep->GetPreStepPoint()->GetPosition();
    m_vPosPre=TVector3(p.x(),p.y(),p.z());
    p=theStep->GetPostStepPoint()->GetPosition();
    m_vPosPost=TVector3(p.x(),p.y(),p.z());
    m_eDep=theStep->GetTotalEnergyDeposit();

    {
      //G4VPhysicalVolume* pv=theStep->GetPreStepPoint()->GetPhysicalVolume();
      //std::string sName = std::string( pv->GetName().data() );
      //std::list<std::string> ar=split(sName,'/');
      //std::list<std::string>::iterator it=ar.end();
      //it--;
      std::sprintf(m_vName,"%s",sName.c_str());
    }

    m_isCore=isCore(m_pPost,m_vPosPost);
    
    m_flag=1;
    m_tree->Fill();
    m_nChange++;
  }
    

  return true;
}


std::list<std::string> GsimE14NeutronMonitor::split(const std::string &str, char delim)
{
  std::istringstream iss(str);
  std::string tmp;
  std::list<std::string> res;
  
  while(getline(iss, tmp, delim)) {
    res.push_back(tmp);
  }
  return res;
}

void GsimE14NeutronMonitor::getBeamAndDetectorShift()
{
  m_detectorShiftZ=0;
  m_beamShiftX=0;
  m_beamShiftY=0;
  
  G4RunManager* fRunManager = G4RunManager::GetRunManager();
  if(!fRunManager) {
    GsimMessage::getInstance()
      ->report("warning","No runManager. Cerenkovs is not added.");
    return;
  }

  G4VUserPrimaryGeneratorAction* upga
    = const_cast<G4VUserPrimaryGeneratorAction*>(fRunManager->GetUserPrimaryGeneratorAction());
  if(!upga) {
    GsimMessage::getInstance()
      ->report("warning","No primary generator action.");
    return;
  }


  
  GsimHEPEvtInterface* hepEVT= dynamic_cast<GsimHEPEvtInterface*>(upga);
  if(hepEVT) {
    m_detectorShiftZ=hepEVT->getDetectorShiftZ();
    m_beamShiftX=hepEVT->getBeamShiftX();
    m_beamShiftY=hepEVT->getBeamShiftY();
    return;
  }

  GsimMessage::getInstance()
    ->report("warning","No GsimHEPEvtInterface.");

  return;
}

int GsimE14NeutronMonitor::isCore(TVector3& p,TVector3& r)
{
  //1: core
  //0: non-core
  //-1: unsolved (p=0 etc.)

  
  //Collimation line in T1 target coordinate
  double zCol[2]={7000.0,10500.0};
  double dColX[2]={7.6,15.9};
  double dColY[2]={12.5,19.3};
  
  //For conversion to the target coordinate
  double zExit=21000.;
  if(!m_isBeamShiftObtained) {
    //obtained run by run 
    // because flag is reset at the end of run action.
    getBeamAndDetectorShift();
    m_isBeamShiftObtained=true;
  }
  
  double z=r.Z()+zExit+m_detectorShiftZ;
  double x=r.X()-m_beamShiftX;
  double y=r.Y()-m_beamShiftY;

  //Check collimation line
  double xSlope=(dColX[1]-dColX[0])/(zCol[1]-zCol[0]);
  double ySlope=(dColY[1]-dColY[0])/(zCol[1]-zCol[0]);
  double xLine=xSlope*(z-zCol[0])+dColX[0];
  double yLine=ySlope*(z-zCol[0])+dColY[0];

  if(! (
        TMath::Abs(x)<xLine && TMath::Abs(y)<yLine
        )
     ) return 0;


  if(p.Z()==0) {
    if(p.Mag()==0) return -1;
    else           return 0;
  }
    
  if(! (
        TMath::Abs(p.X()/p.Z())<xSlope && TMath::Abs(p.Y()/p.Z())<ySlope
        )
     ) return 0;
  
  
  //Now position is inside the collimation line
  // and direction is also inside the collimation line
  return 1;
}

bool GsimE14NeutronMonitor::isHitMBN(TVector3& p,TVector3& r)
{
  if(p.Z()<0) {
    //if it go upstream adopt.
    return true;
  }
  
  bool isInside=false;
  int  iOrigi=-1;
  double zOrig=r.Z();
  for(int i=0;i<5;i++) {
    if(zOrig>m_targetZ[i] && zOrig <=m_targetZ[i+1]) {

      if( TMath::Abs( r.X() ) < m_targetD[i] &&
	  TMath::Abs( r.y() ) < m_targetD[i]) {

	//inside
	isInside=true;
	iOrigi=i;
      }
      
    }
  }


  if(!isInside) {
    return true;
  }
  //adpopt for simulation

  bool isHit=false;
  {
    //here it should go downstream
    
    for(int i=iOrigi+1;i<6;i++) {
      
      double zTarget=m_targetZ[i];
      double t=   (zTarget-r.Z())/p.Z();
      double xTarget=p.X()*t+r.X();
      double yTarget=p.Y()*t+r.Y();

      if(TMath::Abs(xTarget)<m_targetD[i] &&
	 TMath::Abs(yTarget)<m_targetD[i]) {
	//is inside
      } else {
	//is outside
	isHit=true;
      }
    }
  }

  if(isHit) {
    return true;
    //adpopt for simulation
  } else {
    return false;
    //discard for simulation
  }
}


void GsimE14NeutronMonitor::setUserParameters(std::string tag, std::vector<double> vec)
{
  if(tag=="killNoHitInVacuum" && vec.size()>0) {
    if(vec[0]==0) {
      m_isKillNoHitInVacuum=false;
    } else {
      m_isKillNoHitInVacuum=true;
    }
  } else if(tag=="killCore" && vec.size()>0) {
    if(vec[0]==0) {
      m_isKillCore=false;
    } else {
      m_isKillCore=true;
    }
  } else {
    std::cout << "Command : setUserParameters killNoHitInVacuum 1/0" << std::endl;
  }
}
