/**
 *  @file
 *  @brief  GsimE14NeutronMonitor
 *  $Id:$
 *  $Log:$
 */
#ifndef GsimE14NeutronMonitor_h
#define GsimE14NeutronMonitor_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

#include "TTree.h"
#include "TString.h"
#include "TVector3.h"

//forward declaration
class G4Step;
class G4TouchableHistory;
class GsimWinstonCone;

/**
 *  @class GsimE14NeutronMonitor
 *  @brief Detector.
 *
 *  This class provides ...
 *    neutron
 *      point for initial step
 *      point for momentum change during the tracking
 *      point for final step
 */

class GsimE14NeutronMonitor : public GsimDetector
{
 public:
  GsimE14NeutronMonitor(std::string name,
		 GsimDetector* motherDetector,
		 G4ThreeVector transV,
		 G4ThreeVector rotV,
		 int userFlag=0);
  
  virtual ~GsimE14NeutronMonitor();

  virtual void beginOfRunAction();
  virtual void endOfRunAction();
  virtual bool beginOfEventAction(const G4Event* evt);
  virtual bool preTrackingAction(const G4Track* aTrack);
  virtual bool steppingAction(const G4Step* theStep);

  void setUserParameters(std::string tag, std::vector<double> vec);
  
 protected:
  bool isHitMBN(TVector3& p,TVector3& r);
  int isCore(TVector3& p,TVector3& r);
  void getBeamAndDetectorShift();
  
  GsimE14NeutronMonitor();
  
  bool m_isBeamShiftObtained;
  bool m_isKillNoHitInVacuum;
  bool m_isKillCore;

  std::list<std::string> split(const std::string &str, char delim);


  double m_detectorShiftZ;
  double m_beamShiftX;
  double m_beamShiftY;

  std::string m_nTagVName;
  double      m_targetZ[6];
  double      m_targetD[6];
    
  TTree* m_tree;
  int m_eventID;
  int m_trackID;
  int m_parentID;
  int m_nChange;
  char m_vName[1024];
  TVector3 m_vPosPre;
  TVector3 m_vPosPost;
  TVector3 m_pPre;
  TVector3 m_pPost;
  double   m_dP;
  double   m_eDep;
  int      m_flag;
  int      m_isCore;
};

#endif // GsimE14NeutronMonitor_h
