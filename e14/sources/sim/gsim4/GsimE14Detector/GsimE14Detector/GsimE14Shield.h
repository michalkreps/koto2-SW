/**
 *  @file
 *  @brief  GsimE14Shield
 *  $Id$
 *  $Log$
 */

#ifndef GsimE14Shield_h
#define GsimE14Shield_h

//includes
#include "GsimKernel/GsimDetector.h"
#include <string>
#include <list>


//forward declaration

/**
 *  @class GsimE14Shield
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14Shield : public GsimDetector
{
 public:
  GsimE14Shield(std::string name,
		GsimDetector* motherDetector,
		G4ThreeVector transV,
		G4ThreeVector rotV,
		int userFlag=0);
  
  virtual ~GsimE14Shield();
  virtual void   setFastSimulationLevel(int level);

  static double getShieldLength();
  
 protected:
  GsimE14Shield();
  int m_model;
  std::list<GsimDetector*> m_detMoveList;
};


#endif // GsimE14Shield_h
