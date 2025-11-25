/**
 *  @file
 *  @brief  GsimKOTOIIBarrel
 *  $Id: 
 *  $Log:
 */
#ifndef GsimKOTOIIBarrel_h
#define GsimKOTOIIBarrel_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimKOTOIIBarrel
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimKOTOIIBarrel : public GsimDetector
{
 public:
  GsimKOTOIIBarrel(std::string name,
		   GsimDetector* motherDetector,
		   G4ThreeVector transV,
		   G4ThreeVector rotV,
		   int userFlag=0);
  
  virtual ~GsimKOTOIIBarrel();
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
  
 protected:
  GsimKOTOIIBarrel();
  std::list<GsimDetector*> m_detMoveList;

};


#endif // GsimKOTOIIBarrel_h
