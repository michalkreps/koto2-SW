/**
 *  @file
 *  @brief  GsimSensitiveDetector
 *  @author Hajime Nanjo <nanjo@scphys.kyoto-u.ac.jp>
 *  @date   2005.10.5
 *  $Id$
 *  $Log$
 */
#ifndef GsimSensitiveDetector_h
#define GsimSensitiveDetector_h

//includes
#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include <string>
#include <vector>
#include <list>
#include <map>

//forward declaration
class GsimSensitiveDetectorMessenger;
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class G4LogicalVolume;
class GsimDetectorHitConverter;
class GsimDigitizer;
class GsimDetectorEventData;
class G4VPhysicalVolume;
class GsimDetector;

/**
 *  @class GsimSensitieiveDetector
 *  @brief SensitiveDetector.
 *
 *  This class provides ...
 */

enum SensitiveDetectorMode {
  chargedParticleSensitiveMode,
  energyDepositSensitiveMode,
  opticalPhotonSensitiveMode,
  opticalPhotonSensitiveFastMode,
  anyParticleSensitiveMode
};

class GsimSensitiveDetector : public G4VSensitiveDetector
{
 public:
  GsimSensitiveDetector(G4String name);
  virtual ~GsimSensitiveDetector();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);
  virtual void clear();

  
  
  
  inline void  setSensitiveMode(SensitiveDetectorMode mode);
  inline void  setOpticalPhotonEfficiency(std::vector<double> waveLen,
					 std::vector<double> effi);
  virtual void process(const G4Step* aStep,G4TouchableHistory* ROhist);
  virtual void process(const G4Step* aStep,G4TouchableHistory* ROhist, std::list<int>& hitChannelList);
  void storeHit(bool withHit);
  void storeDigi(bool withDigi);
  void storeMtime(bool withMtime);
  bool isStoreHit();
  bool isStoreDigi();
  bool isStoreMtime();

  bool withDataToStore();

  /**
   *  @function addDigitizer
   *  @brief Add digitizer.
   *  @param digi GsimDigitizer to be added.
   */
  void addDigitizer(GsimDigitizer* digi);
  
  std::string getName();
  int  getSensitiveDetectorID();
  void setSensitiveDetectorID(int id);
  void setThisCopyNoFactor(int thisCopyNoFactor);
  void setDeepCopyNoFactor(int deepCopyNoFactor);
  void setClusterID(int channelID,int clusterID);
  int  getClusterID(int channelID);
  std::list<int>  getChannelID(G4VPhysicalVolume* pv,
  			       std::map<G4VPhysicalVolume*,G4VPhysicalVolume*>& pvr);

  void setFastSimulationLevel(int level);
  double getTotalEnergy();

  /**
   *  @function setTdcThreshold
   *  @brief set TDC threshold
   *  @param eth threshold energy in the unit of MeV
   */
  void setTdcThreshold(double eth);
  
  /**
   *  @function setOnlineVetoThreshold
   *  @brief set online veto  threshold
   *  @param eth threshold energy in the unit of MeV
   */
  void setOnlineVetoThreshold(double eth);

  /**
   *  @function setOnlineTriggerThreshold
   *  @brief set online trigger  threshold
   *  @param eth threshold energy in the unit of MeV
   */
  void setOnlineTriggerThreshold(double eth);
  
  void setHitProcessDetector(GsimDetector* detector);
  void dump(int imode);
  bool processHitsDetector(G4Step* aStep, G4TouchableHistory* ROhist);
  bool processHitsDetector(G4Step* aStep, G4TouchableHistory* ROhist, std::list<int>& hitChannelList);

  /**
   *  @function addDigiMergeByN
   *  @brief add digi merging existing digi
   *  @param nDiv new ch = int(modID/nDiv)*nDiv
   *  @param nMax  maximum number of digi to be added
   */
  void addDigiMergeByN(int nDiv,int nMax);
  
 protected:
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

  
  bool   isOpticalPhotonHit(double opticalPhotonEnergy);
  double getOpticalPhotonEfficiency(double opticalPhotonEnergy);

  /**
   *  @function makeBranch
   *  @brief Make a ROOT Branch for the hit. 
   */
  virtual void makeBranch();

  /**
   *  @function initializeDataValues
   *  @brief Initialize the data.
   */
  virtual void initializeDataValues();

  /**
   *  @function digitize
   *  @brief Digitize.
   */
  void digitize();

  /**
   *  @function sortHitData
   *  @brief Sort hit data with hitChannel, time, and, energy.
   */
  void sortHitData();

  /**
   *  @function clearHitData
   *  @brief Clear hit data.
   */
  void clearHitData();

  /**
   *  @function clearDigiData
   *  @brief Clear digi data.
   */
  void clearDigiData();
	
  /**
   *  @function clearMtimeData
   *  @brief Clear hit data.
   */
  void clearMtimeData();

  
		    
  GsimSensitiveDetectorMessenger* m_messenger;
  
  std::string m_name;

  //automatically assigned in GsimRunAction.
  int m_sensitiveDetectorID;
  
  SensitiveDetectorMode m_sensitiveMode;
  std::vector<double> m_opticalPhotonEfficiency;
  std::vector<double> m_opticalPhotonWaveLength;// in nm;
  
  
  
  bool m_storeHit;
  bool m_storeDigi;
  bool m_storeMtime;
  std::map<int,int> m_channelIDClusterID;

  GsimDetectorEventData* m_detectorEventData;
  GsimDetectorHitConverter* m_hitConverter;
  std::list<GsimDigitizer*> m_digitizerList;
  GsimDetector* m_hitProcessDetector;


  int m_fastSimulationLevel;
  double m_onlineVetoThreshold;//[MeV]
  double m_onlineTriggerThreshold;//[MeV]
};

inline void GsimSensitiveDetector::setSensitiveMode(SensitiveDetectorMode mode) {
  m_sensitiveMode=mode;
}

inline void GsimSensitiveDetector::
setOpticalPhotonEfficiency(std::vector<double> waveLen,
			   std::vector<double> effi) {
  if(waveLen.size()!=effi.size()) return;
  
  m_opticalPhotonWaveLength=waveLen;
  m_opticalPhotonEfficiency=effi;
}

inline void GsimSensitiveDetector::storeHit(bool withHit) {
  m_storeHit=withHit;
}

inline void GsimSensitiveDetector::storeDigi(bool withDigi) {
  m_storeDigi=withDigi;
}

inline void GsimSensitiveDetector::storeMtime(bool withMtime) {
	m_storeMtime=withMtime;
}

inline bool GsimSensitiveDetector::isStoreHit() {
  return m_storeHit;
}
inline bool GsimSensitiveDetector::isStoreDigi() {
  return m_storeDigi;
}
inline bool GsimSensitiveDetector::isStoreMtime() {
  return m_storeMtime;
}

inline std::string GsimSensitiveDetector::getName()
{
  return m_name;
}

inline int  GsimSensitiveDetector::getSensitiveDetectorID()
{
  return m_sensitiveDetectorID;
}
inline void GsimSensitiveDetector::setFastSimulationLevel(int level)
{
  m_fastSimulationLevel=level;
}

inline void GsimSensitiveDetector::setOnlineVetoThreshold(double eth) {
  m_onlineVetoThreshold=eth;
}

inline void GsimSensitiveDetector::setOnlineTriggerThreshold(double eth) {
  m_onlineTriggerThreshold=eth;
}

inline void GsimSensitiveDetector::setHitProcessDetector(GsimDetector* detector) {
  m_hitProcessDetector=detector;
}

#endif // GsimSensitiveDetector_h

