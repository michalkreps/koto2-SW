/**
 *  $Id$
 *  $Log$
 */
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimSensitiveDetectorMessenger.h"
#include "GsimKernel/GsimDetectorHitConverter.h"
#include "GsimKernel/GsimDigitizer.h"
#include "GsimKernel/GsimDetector.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimKernel/GsimTrackInformation.h"
#include "GsimKernel/GsimEventAction.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimData/GsimDetectorEventData.h"
#include "GsimData/GsimDigiData.h"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4Geantino.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"
#include "TClonesArray.h"
#include <iostream>

GsimSensitiveDetector::GsimSensitiveDetector(G4String name)
  :G4VSensitiveDetector(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_name=name;
  m_sensitiveDetectorID=-1;
  m_storeHit=false;
  m_storeDigi=true;
  m_storeMtime=false;
  m_sensitiveMode=energyDepositSensitiveMode;
  
  m_detectorEventData = new GsimDetectorEventData();
  m_hitConverter = new GsimDetectorHitConverter(name);

  m_messenger = new GsimSensitiveDetectorMessenger(this);

  m_fastSimulationLevel=0;
  m_onlineVetoThreshold=1e6;//[MeV]=1TeV
  m_onlineTriggerThreshold=0;//[MeV]
  m_hitProcessDetector=0;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimSensitiveDetector::~GsimSensitiveDetector(){
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  delete m_detectorEventData;
  for(std::list<GsimDigitizer*>::iterator it=m_digitizerList.begin();
      it!=m_digitizerList.end();it++) {
    delete (*it);
  }
  m_digitizerList.clear();
  delete m_hitConverter;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimSensitiveDetector::Initialize(G4HCofThisEvent*)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  int eventID=G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if(eventID==0) {
    makeBranch();
  }
  initializeDataValues();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  double eTot=m_detectorEventData->totalEnergy;
  if(eTot<m_onlineTriggerThreshold) {
    //Abort event
    GsimEventAction::abortCurrentEvent();
  }
  digitize();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::clear()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4bool GsimSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  G4Track*              track    = aStep->GetTrack();
  G4ParticleDefinition* particle = track->GetDefinition();
  double                charge   = particle->GetPDGCharge();
  double                edep     = aStep->GetTotalEnergyDeposit();

  bool isHit=false;
  if(particle==G4Geantino::Definition()) {
    isHit=true;
  } else {
    if(m_sensitiveMode==chargedParticleSensitiveMode) {
      if(charge!=0) {
	isHit=true;
      } else return false;
    } else if(m_sensitiveMode==energyDepositSensitiveMode) {
      if(edep>0){
	isHit=true;
      } else return false;
    } else if(m_sensitiveMode==opticalPhotonSensitiveMode) {
      if(particle==G4OpticalPhoton::OpticalPhotonDefinition()) {
	if(isOpticalPhotonHit(track->GetKineticEnergy())) {
	  aStep->SetTotalEnergyDeposit(1.*MeV);
	  isHit=true;
	}
      }
    } else if(m_sensitiveMode==opticalPhotonSensitiveFastMode) {
      if(particle==G4OpticalPhoton::OpticalPhotonDefinition()) {
	isHit=true;
      }
    } else if(m_sensitiveMode==anyParticleSensitiveMode) {
      isHit=true;
    } else {
      GsimMessage::getInstance()->report("warning","No valid sensitive mode.");
    }
  }
  
  if(isHit)  {
    std::list<int> hitChannelList;
    isHit = processHitsDetector(aStep, ROhist, hitChannelList);
    if(isHit) {
      process(aStep,ROhist, hitChannelList);
      if(m_sensitiveMode==opticalPhotonSensitiveMode &&
	 particle==G4OpticalPhoton::OpticalPhotonDefinition()) {
	//Optical photon is vanished if detected.
	track->SetTrackStatus(fStopAndKill);
      }
    }
  }

  if(m_sensitiveMode==opticalPhotonSensitiveFastMode &&
     particle==G4OpticalPhoton::OpticalPhotonDefinition()) {
    //Optical photon is vanished at the first step in any case.
    track->SetTrackStatus(fStopAndKill);
  }
#ifdef GSIDMEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif

  return isHit;
}

bool GsimSensitiveDetector::isOpticalPhotonHit(double opticalPhotonEnergy)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double effi=getOpticalPhotonEfficiency(opticalPhotonEnergy);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  if(CLHEP::RandFlat::shoot()<effi) return true;
  else                       return false; 
}

double GsimSensitiveDetector::getOpticalPhotonEfficiency(double opticalPhotonEnergy)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  if(m_opticalPhotonWaveLength.size()<2) return 0;
  
  double hbarc=197.326968;//[eV nm]
  double waveLength = 2*M_PI*hbarc/(opticalPhotonEnergy/eV);

  std::vector<double>::iterator itFast = m_opticalPhotonWaveLength.begin();
  std::vector<double>::iterator itLast = m_opticalPhotonWaveLength.end();
  itLast--;

  double minWL=(*itFast);
  double maxWL=(*itLast);
  if((*itLast)<(*itFast)) {
    maxWL=(*itFast);
    minWL=(*itLast);
  }

  if(waveLength<minWL || waveLength>maxWL) return 0;


  std::vector<double>::iterator itW1=m_opticalPhotonWaveLength.begin();
  std::vector<double>::iterator itW2=m_opticalPhotonWaveLength.begin();
  itW2++;
  std::vector<double>::iterator itE1=m_opticalPhotonEfficiency.begin();
  std::vector<double>::iterator itE2=m_opticalPhotonEfficiency.begin();
  itE2++;

  
  for(;itW2!=m_opticalPhotonWaveLength.end();itW1++,itW2++,itE1++,itE2++) {
    if( ( waveLength-(*itW1) ) * ( waveLength-(*itW2) ) < 0) {
      
      double l1=std::abs( waveLength-(*itW1) );
      double l2=std::abs( waveLength-(*itW2) );
      double e1=(*itE1);
      double e2=(*itE2);

      return (l2*e1+l1*e2)/(l1+l2);
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return 0.;
}

void GsimSensitiveDetector::makeBranch()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::string className = m_detectorEventData->getClassName();
  std::string tit;
  tit+=m_name;
  tit+=".";
  GsimPersistencyManager::getPersistencyManager()
    ->setEventBranch(tit,className,&m_detectorEventData);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
void GsimSensitiveDetector::initializeDataValues()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_detectorEventData->initializeDataValues();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::addDigitizer(GsimDigitizer* digi)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_digitizerList.push_back(digi);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::process(const G4Step* aStep,G4TouchableHistory* ROhist, std::list<int>& hitChannelList)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  bool doProcess=true;

  G4Track* track=aStep->GetTrack();
  int status=track->GetTrackStatus();
  GsimTrackInformation* trackInfo
    = dynamic_cast<GsimTrackInformation*>(track->GetUserInformation());
  trackInfo->setStopStatus(status+10*m_fastSimulationLevel);
  
  if( (!m_storeDigi) && (!m_storeHit) && (!m_storeMtime) ) {
    doProcess=false;
  }
  m_hitConverter->convertHit(m_detectorEventData,aStep,ROhist,hitChannelList,doProcess);
  
  double eTot=m_detectorEventData->totalEnergy;
  if(
     eTot>m_onlineVetoThreshold 
     ) {
    GsimEventAction::abortCurrentEvent();
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::process(const G4Step* aStep,G4TouchableHistory* ROhist)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::list<int> hitChannelList;
  process(aStep,ROhist, hitChannelList);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::digitize()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  if( (!m_storeDigi) && (!m_storeHit) && (!m_storeMtime) ) return;

  sortHitData();
  
  if(m_storeDigi || m_storeMtime) {
    for(std::list<GsimDigitizer*>::iterator it=m_digitizerList.begin();
	it!=m_digitizerList.end();it++) {
      (*it)->digitize(m_detectorEventData);
    }
  }
  
  if(!m_storeHit) {
    clearHitData();
  }
	
  if(!m_storeDigi) {
	clearDigiData();
  }
	
  if(!m_storeMtime) {
	clearMtimeData();
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::sortHitData()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  TClonesArray &arHits = *(m_detectorEventData->hits);
  arHits.Sort();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::clearHitData()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_detectorEventData->clearHitData();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimSensitiveDetector::clearDigiData()
{
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
	m_detectorEventData->clearDigiData();
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimSensitiveDetector::clearMtimeData()
{
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
	m_detectorEventData->clearMtimeData();
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimSensitiveDetector::setThisCopyNoFactor(int thisCopyNoFactor)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_hitConverter->setThisCopyNoFactor(thisCopyNoFactor);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimSensitiveDetector::setDeepCopyNoFactor(int deepCopyNoFactor)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_hitConverter->setDeepCopyNoFactor(deepCopyNoFactor);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimSensitiveDetector::setClusterID(int channelID,int clusterID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  for(std::list<GsimDigitizer*>::iterator it=m_digitizerList.begin();
      it!=m_digitizerList.end();it++) {
    (*it)->setClusterID(channelID,clusterID);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

int GsimSensitiveDetector::getClusterID(int channelID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  int clid=-1;
  if(m_digitizerList.size()!=0) {
    std::list<GsimDigitizer*>::iterator it=m_digitizerList.begin();
    clid=(*it)->getClusterID(channelID);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return clid;
}



std::list<int> GsimSensitiveDetector::GsimSensitiveDetector::getChannelID(G4VPhysicalVolume* pv,
							       std::map<G4VPhysicalVolume*,G4VPhysicalVolume*>& pvr) {
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return m_hitConverter->getChannelID(pv,pvr);
}

bool GsimSensitiveDetector::withDataToStore()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  
  if(m_detectorEventData) {
    if(m_storeDigi &&
       m_detectorEventData->nDigi>0) return true;
    if(m_storeHit &&
       m_detectorEventData->nHit>0)  return true;
    if(m_storeMtime &&
       m_detectorEventData->nMtime>0) return true;
  }
  return false;
}


void GsimSensitiveDetector::setSensitiveDetectorID(int id)
{
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_sensitiveDetectorID=id;
  m_hitConverter->setSensitiveDetectorID(id);
  for(std::list<GsimDigitizer*>::iterator it=m_digitizerList.begin();
      it!=m_digitizerList.end();it++) {
    (*it)->setSensitiveDetectorID(id);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

double GsimSensitiveDetector::getTotalEnergy()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double totE=0;
  if(m_detectorEventData) {
    totE=m_detectorEventData->totalEnergy;
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return totE;
}

void GsimSensitiveDetector::dump(int imode=0)
{
  std::cout << m_name  << "\t"
	    << m_sensitiveDetectorID << "\t"
	    << getTotalEnergy()
	    << std::endl;
  if(imode!=0) {
    m_detectorEventData->dumpDigi();
  }
}


bool GsimSensitiveDetector::
processHitsDetector(G4Step* aStep, G4TouchableHistory* ROhist,std::list<int>& hitChannelList)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  bool isPass=true;
  if(m_hitProcessDetector) {
    isPass=m_hitProcessDetector->processHits(aStep, ROhist, hitChannelList);
  }
  return isPass;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}

bool GsimSensitiveDetector::
processHitsDetector(G4Step* aStep, G4TouchableHistory* ROhist)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  bool isPass=true;
  std::list<int> hitChannelList;
  isPass = processHitsDetector(aStep, ROhist,hitChannelList);
  return isPass;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


void GsimSensitiveDetector::setTdcThreshold(double eth) {
  if(m_digitizerList.size()>0) {
    std::list<GsimDigitizer*>::iterator it
      =m_digitizerList.begin();
    (*it)->setTdcThresholdEnergy(eth);
  }
}

void GsimSensitiveDetector::addDigiMergeByN(int nDiv,int nMax) {

  double *eneMerge  = new double[nMax];
  double *timeMerge = new double[nMax];
  for(int k=0;k<nMax;k++) {
    eneMerge[k]=0;
    timeMerge[k]=9999;
  }
  
  TClonesArray &arDigi = *(m_detectorEventData->digi);

  GsimDigiData* digiData=0;
  int detID=0;
  for(int i=0;i<arDigi.GetEntriesFast();i++) {
    digiData = (GsimDigiData*)arDigi[i];
    detID=digiData->detID;
    int ch = int(digiData->modID/nDiv);
    if(ch<nMax) {
      eneMerge[ch]+=digiData->energy;
      if(timeMerge[ch]>digiData->time) {
	timeMerge[ch]=digiData->time;
      }
    }
  }
  
  int digiCount=m_detectorEventData->nDigi;
  for(int k=0;k<nMax;k++) {
    if(eneMerge[k]>0) {
      digiData = new (arDigi[digiCount]) GsimDigiData();
      digiData->detID=detID;
      digiData->modID=k*nDiv;
      digiData->energy=eneMerge[k];
      digiData->time=timeMerge[k];
      digiData->thisID=digiCount;
      digiData->status=999;
      digiData->mtimeEntry=0;
      digiData->mtimeSize=0;
      
      
      digiCount++;
    }
  }
  m_detectorEventData->nDigi=digiCount;

  delete [] eneMerge;
  delete [] timeMerge;
}

