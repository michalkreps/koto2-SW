/**
 *  @file
 *  @brief  GsimKOTOIICalorimeter
 *  $Id:
 *  $Log:
 */
#ifndef GsimKOTOIICalorimeter_h
#define GsimKOTOIICalorimeter_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimKOTOIICalorimeter
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimKOTOIICalorimeter : public GsimDetector
{
 public:
  GsimKOTOIICalorimeter(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimKOTOIICalorimeter();
  virtual void   setFastSimulationLevel(int level);
  virtual void comment();
 protected:
  std::list<GsimDetector*> m_detList;
  std::list<GsimDetector*> m_detMoveList;
  GsimKOTOIICalorimeter();
};


#endif // GsimKOTOIICalorimeter_h
