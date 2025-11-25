/**
 *  $Id: GsimDetectorHitConverter.cc,v 1.29 2006/12/09 21:34:09 nanjo Exp $
 *  $Log: GsimDetectorHitConverter.cc,v $
 *  Revision 1.29  2006/12/09 21:34:09  nanjo
 *  briefHistory is corrected and dictionary write.
 *
 *  Revision 1.28  2006/12/06 18:44:00  nanjo
 *  Midas I/O is added and Root I/O is separated. etc.
 *
 *  Revision 1.27  2006/12/04 17:27:49  nanjo
 *  Data are kept in each class, which are registered to persistencyManger.
 *  GsimPersistencyManger::setBranchOf is added then UserEventData is removed.
 *
 *  Revision 1.26  2006/12/02 07:06:11  nanjo
 *  sensitiveDetectorID, StopFlag, detID are updated.
 *
 *  Revision 1.25  2006/11/30 20:07:11  nanjo
 *  Data are re-structured.
 *
 *  Revision 1.24  2006/11/16 13:13:00  nanjo
 *  Module id with detector depth..
 *
 *  Revision 1.23  2006/10/26 21:21:41  nanjo
 *  inf and nan are removed.
 *
 *  Revision 1.22  2006/10/24 15:23:08  nanjo
 *  SensitiveDetector is re-structred.
 *
 *  Revision 1.21  2006/10/11 01:01:35  nanjo
 *  SensitiveDetector and HitConverter are restructured.
 *
 *  Revision 1.20  2006/08/24 22:19:01  nanjo
 *  Almost all data type is changed from double to Float_t, from int to UShort_t.
 *
 *  Revision 1.19  2006/08/02 00:39:19  nanjo
 *  A bug of multiple call of digitize is fixed.
 *
 *  Revision 1.18  2006/08/01 15:29:46  nanjo
 *  DetectorIDHistory is modified.
 *
 *  Revision 1.17  2006/08/01 05:27:55  nanjo
 *  GsimSpectrum is added. Debug print is updated. With/without Digi is implemented.
 *
 *  Revision 1.16  2006/06/26 05:27:28  nanjo
 *  Update around storedTrackID and storedMotherTrackID.
 *
 *  Revision 1.15  2006/06/22 12:09:50  nanjo
 *  Mother track ID means stored track ID.
 *
 *  Revision 1.14  2006/06/14 14:59:40  nanjo
 *  More smart protection against too many hits.
 *
 *  Revision 1.13  2006/06/14 05:54:22  nanjo
 *  GsimDigitizer is added.
 *
 *  Revision 1.12  2006/06/12 22:47:45  nanjo
 *  Protection against too many hits is added.
 *
 *  Revision 1.11  2006/06/04 10:14:31  nanjo
 *  mass is removed.
 *
 *  Revision 1.10  2006/06/04 10:08:45  nanjo
 *  mass is added.
 *
 *  Revision 1.9  2006/06/04 09:38:36  nanjo
 *  Correct from using G4Track information ( = PostStepPoint information)
 *  to using PreStepPoint information.
 *
 *  Revision 1.8  2006/05/25 14:36:35  nanjo
 *  The processName, "Primary", is added for the track without iniCreatorProcess.
 *
 *  Revision 1.7  2006/04/12 12:07:17  nanjo
 *  Fix typo.
 *
 *  Revision 1.6  2006/03/04 10:48:33  nanjo
 *  If a negative number, -n, is set for the detector channel, the copy number of n-th up detector is used.
 *
 *  Revision 1.5  2006/03/01 12:45:32  nanjo
 *  A member function of GsimDetector, cloneDetector, is added and used in E391Detector.
 *
 *  Revision 1.4  2006/02/21 09:09:59  nanjo
 *  Sensitive detector name and channel id can be assigned. If the same sensitive detector name is used for some detectors, these detectors are recognized as channels in one detector.
 *
 *  Revision 1.3  2006/02/01 15:42:26  nanjo
 *  BriefTrajectory is stored in ROOT file.
 *
 *  Revision 1.2  2006/01/31 03:00:38  nanjo
 *  Detector ID and process ID are introduced and stored in TFile.
 *
 *  Revision 1.1  2006/01/26 04:53:30  nanjo
 *  Detector and persistency classes are added.
 *
 */
#include "GsimKernel/GsimDetectorHitConverter.h"
#include "GsimData/GsimDetectorEventData.h"
#include "GsimData/GsimDetectorHitData.h"
#include "GsimKernel/GsimTrackInformation.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "float.h"

GsimDetectorHitConverter::GsimDetectorHitConverter(std::string name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_name = name;
  if(std::string(m_name,0,6)=="/world") {
    m_name=std::string(m_name,6,m_name.size()-6);
  }
  if(std::string(m_name,0,1)=="/") {
    m_name=std::string(m_name,1,m_name.size()-1);
  }

  if(std::string(m_name,m_name.size()-1,1)=="/") {
    m_name=std::string(m_name,0,m_name.size()-1);
  }
  
  while(1) {
    size_t n = m_name.find("/");
    if(n==std::string::npos) break;
    m_name.replace(n,1,"_",1);
  }
  
  m_converterName="GsimDetectorHitConverter";
  m_thisCopyNoFactor=-1;
  m_deepCopyNoFactor=1;
  //m_overflowTime=300e3;//300usec
  m_overflowTime=DBL_MAX;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimDetectorHitConverter::~GsimDetectorHitConverter()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimDetectorHitConverter::fillHitData(GsimDetectorHitData* data,int thisID,
					   const G4Step* aStep,int channelID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif  
  G4Track* track = aStep->GetTrack();
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  
  
  GsimTrackInformation* trackInfo
    = dynamic_cast<GsimTrackInformation*>(track->GetUserInformation());
  trackInfo->addDetectorHit(data);
  
  data->thisID = static_cast<UInt_t>(thisID);
  ////////////////////////////////////////////
  //These are filleld at postTrackingAction.
  //data->trackID = trackInfo->getStoredTrackID();
  ////////////////////////////////////////////
  data->stop=static_cast<UShort_t>(trackInfo->getStopStatus());
  data->hitChannel = static_cast<UShort_t>(channelID);
  data->time = static_cast<Float_t>(preStepPoint->GetGlobalTime()/ns);
  if(!std::isfinite(data->time)) {
    data->time = static_cast<Float_t>(m_overflowTime);
  }
  if(data->time > m_overflowTime) {
    data->time= static_cast<Float_t>(m_overflowTime);
  }
  data->edep = static_cast<Float_t>(aStep->GetTotalEnergyDeposit()/MeV);
  if(track->GetDefinition()==G4Geantino::Definition()) {
    //RadLen for Geantino
    data->edep =
      static_cast<Float_t>(
			   aStep->GetStepLength()/
			   (preStepPoint->GetMaterial()->GetRadlen()));
  }
  data->pid = static_cast<Int_t>(track->GetDefinition()->GetPDGEncoding());
  G4ThreeVector vec;
  vec = preStepPoint->GetPosition();
  data->r = TVector3(vec.x()/mm,vec.y()/mm,vec.z()/mm);
  
  data->ek = static_cast<Float_t>(preStepPoint->GetKineticEnergy()/MeV);
  vec = preStepPoint->GetMomentum();
  data->p = TVector3(vec.x()/MeV,vec.y()/MeV,vec.z()/MeV);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimDetectorHitConverter::convertHit(GsimDetectorEventData* eventData,
					  const G4Step* aStep,G4TouchableHistory* ROhist,
					  std::list<int>& hitChannelList,
					  bool processHit)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4Track* track = aStep->GetTrack();
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

  bool isFirstHit=false;
  if(eventData->nHit==0) isFirstHit=true;
  
  if(processHit) {
    if(eventData->nHit> static_cast<UInt_t>( GsimDetectorEventData::getArraySize() )) {
      std::ostringstream ostr;
      ostr << "Number of hits, " << eventData->nHit
	   << ", is too many. This hit is not stored.";
      GsimMessage::getInstance()
	->report("warning",ostr.str());
    } else {
      TClonesArray &ar = *(eventData->hits);
      if(hitChannelList.size()>0) {
	for(std::list<int>::iterator it=hitChannelList.begin();
	    it!=hitChannelList.end();it++) {
	  GsimDetectorHitData* data = new (ar[eventData->nHit]) GsimDetectorHitData();
	  int thisID = eventData->nHit;
	  int channelID = (*it);
	  fillHitData(data,thisID,aStep,channelID);
	  eventData->nHit+=1;
	}
      } else {
	GsimDetectorHitData* data = new (ar[eventData->nHit]) GsimDetectorHitData();
	int thisID = eventData->nHit;
	G4StepPoint* preStepPoint2 = aStep->GetPreStepPoint();
	int channelID = getChannelID(preStepPoint2);
	if(ROhist) {
	  channelID = ROhist->GetReplicaNumber();
	}
	fillHitData(data,thisID,aStep,channelID);
	eventData->nHit+=1;
      }
    }
  } else {
    if(hitChannelList.size()>0) {
      eventData->nHit+=hitChannelList.size();
    } else {
      eventData->nHit+=1;
    }
  }
  
  
  if(track->GetDefinition()==G4Geantino::Definition()) {
    //RadLen for Geantino
    eventData->totalEnergy+= static_cast<Float_t>(aStep->GetStepLength()/
						  (preStepPoint->GetMaterial()->GetRadlen()));
  } else {
    eventData->totalEnergy+=static_cast<Float_t>(aStep->GetTotalEnergyDeposit()/MeV);
  }

  Float_t hTime=static_cast<Float_t>(preStepPoint->GetGlobalTime()/ns);
  if(isFirstHit) {
    eventData->firstHitTime=hTime;
    eventData->lastHitTime=hTime;
  } else {
    if(hTime < eventData->firstHitTime) {
      eventData->firstHitTime = hTime;
    }
    if(hTime > eventData->lastHitTime ) {
      eventData->lastHitTime = hTime;
    }
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

int GsimDetectorHitConverter::getChannelID(G4StepPoint* sp)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4VPhysicalVolume* pv =sp->GetPhysicalVolume();
  int ch=0;
  int cn = pv->GetCopyNo();
  int chadd=0;
  
  if(cn>=0&&m_thisCopyNoFactor>=0) {
    chadd=m_thisCopyNoFactor*cn;
    cn=-1;
  }
  if(cn>=0) {
    ch = cn;
  } else {
    int depth=-cn;
    G4TouchableHistory* theTouchable = 
      (G4TouchableHistory*)( sp->GetTouchable() );
    int nDepth = theTouchable->GetHistoryDepth();
    if(depth>=nDepth) {
	std::ostringstream ostr;
	ostr << m_name << " The assigned depth, " << depth
	     << ", is larger than this depth, " << nDepth;
	GsimMessage::getInstance()
	  ->report("warning",ostr.str());
	ch = 0;
    } else {
      int chDep=theTouchable->GetReplicaNumber(depth);
      ch = chDep*m_deepCopyNoFactor+chadd;
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return ch;
}




std::list<int> GsimDetectorHitConverter::getChannelID(G4VPhysicalVolume* pv,
						      std::map<G4VPhysicalVolume*,G4VPhysicalVolume*>& pvr)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::list<int> chList;
  int cn = pv->GetCopyNo();
  int chadd=0;
  if(cn>=0&&m_thisCopyNoFactor>=0) {
    chadd=m_thisCopyNoFactor*cn;
    cn=-1;
  }
  if(cn>=0) {
    chList.push_back(cn);
  } else {
    int depth=-cn;

    std::map<G4VPhysicalVolume*,G4VPhysicalVolume*>::iterator itNow;
    G4VPhysicalVolume* pvUp=pv;
    for(int k=0;k<depth;k++) {
      itNow=pvr.find( pvUp );
      if(itNow==pvr.end()) {
	std::ostringstream ostr;
	ostr << m_name << " The assigned depth, " << depth
	     << ", is larger than this depth";
	GsimMessage::getInstance()
	  ->report("warning",ostr.str());
	pvUp=0;
      } else {
	pvUp=(*itNow).second;
      }
    }
    if(pvUp) {
      G4String pvName=pvUp->GetName();
      itNow=pvr.find( pvUp );
      
      if(itNow==pvr.end()) {
	std::ostringstream ostr;
	ostr << m_name << " The assigned depth, " << depth
	     << ", is larger than this depth";
	GsimMessage::getInstance()
	  ->report("warning",ostr.str());
	pvUp=0;
      } else {
	pvUp=(*itNow).second;
      }
      if(pvUp) {
	for(std::map<G4VPhysicalVolume*,G4VPhysicalVolume*>::iterator itp=pvr.begin();
	    itp!=pvr.end();itp++) {
	  if( (*itp).second == pvUp  && ((*itp).first)->GetName()==pvName ) {
	    int chDep=((*itp).first)->GetCopyNo();
	    int ch = chDep*m_deepCopyNoFactor+chadd;
	    chList.push_back(ch);
	  }
	}
      }
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return chList;
}
