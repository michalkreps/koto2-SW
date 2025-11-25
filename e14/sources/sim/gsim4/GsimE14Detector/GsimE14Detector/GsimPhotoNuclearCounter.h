/**
 *  @file
 *  @brief  GsimPhotoNuclearCounter
 *  $Id: $
 *  $Log: $
 */
#ifndef GsimPhotoNuclearCounter_h
#define GsimPhotoNuclearCounter_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration
class G4Event;
class G4Track;
class G4Step;
class G4TouchableHistory;

/**
 *  @class GsimPhotoNuclearCounter
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimPhotoNuclearCounter : public GsimDetector
{
 public:
  GsimPhotoNuclearCounter(std::string name,
	      GsimDetector* motherDetector,
	      G4ThreeVector transV,
	      G4ThreeVector rotV,
	      int userFlag=0);
  
  virtual ~GsimPhotoNuclearCounter();

  virtual void beginOfRunAction();
  virtual void endOfRunAction();
  virtual bool beginOfEventAction(const G4Event* evt);
  virtual bool endOfEventAction(const G4Event* evt);
  virtual bool preTrackingAction(const G4Track* aTrack);
  virtual bool postTrackingAction(const G4Track* aTrack);
  virtual bool steppingAction(const G4Step* theStep);
  virtual bool processHits(const G4Step* aStep,G4TouchableHistory* ROhist,std::list<int>& hitChannelList);
  
 protected:
  virtual void constructPhysiDetector();
  virtual void updateParametersEtc();
  GsimPhotoNuclearCounter();


  int    m_photoNuclearIniNum;
  int    m_photoNuclearIniPid[1000];
  double m_photoNuclearIniPosX[1000];
  double m_photoNuclearIniPosY[1000];
  double m_photoNuclearIniPosZ[1000];
  int    m_photoNuclearIniThisID[1000];
  int    m_photoNuclearIniParentID[1000];
  int    m_photoNuclearIniProcessID[1000];
  
  int    m_photoNuclearFinNum;
  int    m_photoNuclearFinPid[1000];
  double m_photoNuclearFinPosX[1000];
  double m_photoNuclearFinPosY[1000];
  double m_photoNuclearFinPosZ[1000];


  int    m_neutronIniNum;
  double m_neutronIniPosX[1000];
  double m_neutronIniPosY[1000];
  double m_neutronIniPosZ[1000];
  int    m_neutronIniCreProcessID[1000];
  double m_neutronIniE[1000];
  
  int    m_neutronFinNum;
  double m_neutronFinPosX[1000];
  double m_neutronFinPosY[1000];
  double m_neutronFinPosZ[1000];
  int    m_neutronFinCreProcessID[1000];
  double m_neutronFinE[1000];

};


#endif // GsimPhotoNuclearCounter_h
