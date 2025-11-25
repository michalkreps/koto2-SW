/**
 *  @file
 *  @brief  GsimE14DCV
 *
 */
#ifndef GsimE14DCV_h
#define GsimE14DCV_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14BPCV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14DCV : public GsimDetector
{
 public:
  GsimE14DCV(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14DCV();
  virtual void comment();
  virtual void setFastSimulationLevel(int level);
  
 protected:
  GsimE14DCV();
  std::list<GsimDetector*> m_detMoveList;

};


#endif // GsimE14BPCV_h
