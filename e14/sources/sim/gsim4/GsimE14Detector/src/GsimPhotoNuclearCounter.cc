/**
 *  @file
 *  @brief  GsimPhotoNuclearCounter
 *  $Id: $ 
 *  $Log: $
 */
#include "GsimE14Detector/GsimPhotoNuclearCounter.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Neutron.hh"
#include "G4SystemOfUnits.hh"

GsimPhotoNuclearCounter::GsimPhotoNuclearCounter(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_className = "GsimPhotoNuclearCounter";
  update();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimPhotoNuclearCounter::~GsimPhotoNuclearCounter()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimPhotoNuclearCounter::constructPhysiDetector()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimPhotoNuclearCounter::updateParametersEtc()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimPhotoNuclearCounter::beginOfRunAction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  GsimPersistencyManager* pm= GsimPersistencyManager::getPersistencyManager();
  
  pm->setBranchOf("eventTree",
		  "photoNuclearIniNum",&m_photoNuclearIniNum,
		  "photoNuclearIniNum/I");
  pm->setBranchOf("eventTree",
		  "photoNuclearFinNum",&m_photoNuclearFinNum,
		  "photoNuclearFinNum/I");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniPosX",m_photoNuclearIniPosX,
		  "photoNuclearIniPosX[photoNuclearIniNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniPosY",m_photoNuclearIniPosY,
		  "photoNuclearIniPosY[photoNuclearIniNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniPosZ",m_photoNuclearIniPosZ,
		  "photoNuclearIniPosZ[photoNuclearIniNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearFinPosX",m_photoNuclearFinPosX,
		  "photoNuclearFinPosX[photoNuclearFinNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearFinPosY",m_photoNuclearFinPosY,
		  "photoNuclearFinPosY[photoNuclearFinNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearFinPosZ",m_photoNuclearFinPosZ,
		  "photoNuclearFinPosZ[photoNuclearFinNum]/D");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniThisID",m_photoNuclearIniThisID,
		  "photoNuclearIniThisID[photoNuclearIniNum]/I");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniParentID",m_photoNuclearIniParentID,
		  "photoNuclearIniParentID[photoNuclearIniNum]/I");
  pm->setBranchOf("eventTree",
		  "photoNuclearIniProcessID",m_photoNuclearIniProcessID,
		  "photoNuclearIniProcessID[photoNuclearIniNum]/I");
  
  
  pm->setBranchOf("eventTree",
		  "neutronIniNum",&m_neutronIniNum,
		  "neutronIniNum/I");
  pm->setBranchOf("eventTree",
		  "neutronFinNum",&m_neutronFinNum,
		  "neutronFinNum/I");
  pm->setBranchOf("eventTree",
		  "neutronIniPosX",m_neutronIniPosX,
		  "neutronIniPosX[neutronIniNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronIniPosY",m_neutronIniPosY,
		  "neutronIniPosY[neutronIniNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronIniPosZ",m_neutronIniPosZ,
		  "neutronIniPosZ[neutronIniNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronIniCreProcessID",m_neutronIniCreProcessID,
		  "neutronIniCreProcessID[neutronIniNum]/I");
  pm->setBranchOf("eventTree",
		  "neutronIniE",m_neutronIniE,
		  "neutronIniE[neutronIniNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronFinPosX",m_neutronFinPosX,
		  "neutronFinPosX[neutronFinNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronFinPosY",m_neutronFinPosY,
		  "neutronFinPosY[neutronFinNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronFinPosZ",m_neutronFinPosZ,
		  "neutronFinPosZ[neutronFinNum]/D");
  pm->setBranchOf("eventTree",
		  "neutronFinE",m_neutronFinE,
		  "neutronFinE[neutronFinNum]/D");
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimPhotoNuclearCounter::endOfRunAction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
  
bool GsimPhotoNuclearCounter::beginOfEventAction(const G4Event* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_photoNuclearIniNum=0;
  m_photoNuclearFinNum=0;
  m_neutronIniNum=0;
  m_neutronFinNum=0;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return true;
}

bool GsimPhotoNuclearCounter::endOfEventAction(const G4Event* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return false;
}

bool GsimPhotoNuclearCounter::preTrackingAction(const G4Track* aTrack)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //PN origin track
  GsimPersistencyManager* pm= GsimPersistencyManager::getPersistencyManager();
  if(aTrack->GetCreatorProcess()) {
    int pnID=-1;
    if(
       aTrack->GetCreatorProcess()->GetProcessName()=="PhotonInelastic") {
      pnID=22;
    } else if(
	      aTrack->GetCreatorProcess()->GetProcessName()=="ElectroNuclear") {
      pnID=11;
    } else if(
	      aTrack->GetCreatorProcess()->GetProcessName()=="photonNuclear") {
      pnID=22;
    } else if(
	      aTrack->GetCreatorProcess()->GetProcessName()=="electronNuclear") {
      pnID=11;
    } else if(
	      aTrack->GetCreatorProcess()->GetProcessName()=="positronNuclear") {
      pnID=11;
    }
    
    if(pnID>0) {
      m_photoNuclearIniPid[m_photoNuclearIniNum]=aTrack->GetDefinition()->GetPDGEncoding();
      m_photoNuclearIniPosX[m_photoNuclearIniNum]=(aTrack->GetPosition()).x();
      m_photoNuclearIniPosY[m_photoNuclearIniNum]=(aTrack->GetPosition()).y();
      m_photoNuclearIniPosZ[m_photoNuclearIniNum]=(aTrack->GetPosition()).z();
      m_photoNuclearIniThisID[m_photoNuclearIniNum]=aTrack->GetTrackID();
      m_photoNuclearIniParentID[m_photoNuclearIniNum]=aTrack->GetParentID();
      m_photoNuclearIniProcessID[m_photoNuclearIniNum]=pnID;
      m_photoNuclearIniNum++;
    }
  }
  //neutron track
  if(aTrack->GetDefinition()==G4Neutron::Neutron()) {
    m_neutronIniPosX[m_neutronIniNum]=(aTrack->GetPosition()).x();
    m_neutronIniPosY[m_neutronIniNum]=(aTrack->GetPosition()).y();
    m_neutronIniPosZ[m_neutronIniNum]=(aTrack->GetPosition()).z();
    std::string prcName = std::string ( (aTrack->GetCreatorProcess()->GetProcessName()).data() );
    m_neutronIniCreProcessID[m_neutronIniNum]=pm->getProcessIDByName(prcName);
    m_neutronIniE[m_neutronIniNum]=aTrack->GetKineticEnergy()/MeV;
    m_neutronIniNum++;
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return true;
}

bool GsimPhotoNuclearCounter::postTrackingAction(const G4Track* aTrack)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //PN origin track
  GsimPersistencyManager* pm= GsimPersistencyManager::getPersistencyManager();
  if(aTrack->GetCreatorProcess()) {
    if(
       //9.5
       aTrack->GetCreatorProcess()->GetProcessName()=="PhotonInelastic" ||
       aTrack->GetCreatorProcess()->GetProcessName()=="ElectroNuclear" ||
       //10.xx
       aTrack->GetCreatorProcess()->GetProcessName()=="photonNuclear" ||
       aTrack->GetCreatorProcess()->GetProcessName()=="electronNuclear" ||
       aTrack->GetCreatorProcess()->GetProcessName()=="positronNuclear"
       ) {
      m_photoNuclearFinPid[m_photoNuclearFinNum]=aTrack->GetDefinition()->GetPDGEncoding();
      m_photoNuclearFinPosX[m_photoNuclearFinNum]=(aTrack->GetPosition()).x();
      m_photoNuclearFinPosY[m_photoNuclearFinNum]=(aTrack->GetPosition()).y();
      m_photoNuclearFinPosZ[m_photoNuclearFinNum]=(aTrack->GetPosition()).z();
      aTrack->GetParentID();
      m_photoNuclearFinNum++;
    }
  }
  //neutron track
  if(aTrack->GetDefinition()==G4Neutron::Neutron()) {
    m_neutronFinPosX[m_neutronFinNum]=(aTrack->GetPosition()).x();
    m_neutronFinPosY[m_neutronFinNum]=(aTrack->GetPosition()).y();
    m_neutronFinPosZ[m_neutronFinNum]=(aTrack->GetPosition()).z();
    std::string prcName = std::string ( (aTrack->GetCreatorProcess()->GetProcessName()).data() );
    m_neutronFinCreProcessID[m_neutronFinNum]=pm->getProcessIDByName(prcName);
    m_neutronFinE[m_neutronFinNum]=aTrack->GetKineticEnergy()/MeV;
    m_neutronFinNum++;
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return true;
}

bool GsimPhotoNuclearCounter::steppingAction(const G4Step* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return false;
}


bool GsimPhotoNuclearCounter::processHits(const G4Step* ,G4TouchableHistory*,std::list<int>&) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return false;
}
