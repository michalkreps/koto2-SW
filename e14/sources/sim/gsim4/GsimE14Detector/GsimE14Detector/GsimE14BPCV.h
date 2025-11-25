/**
 *  @file
 *  @brief  GsimE14BPCV
 *
 */
#ifndef GsimE14BPCV_h
#define GsimE14BPCV_h

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

class GsimE14BPCV : public GsimDetector
{
 public:
  GsimE14BPCV(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14BPCV();
  virtual void comment();
  virtual void setFastSimulationLevel(int level);
  
 protected:
  GsimE14BPCV();
  std::list<GsimDetector*> m_detMoveList;

};


#endif // GsimE14BPCV_h
