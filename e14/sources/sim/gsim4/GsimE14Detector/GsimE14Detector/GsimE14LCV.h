/**
 *  @file
 *  @brief  GsimE14LCV
 *  $Id$
 *  $Log$
 */

#ifndef GsimE14LCV_h
#define GsimE14LCV_h

//includes
#include "GsimKernel/GsimDetector.h"
#include <string>
#include <list>


//forward declaration

/**
 *  @class GsimE14LCV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14LCV : public GsimDetector
{
 public:
  GsimE14LCV(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14LCV();
  virtual void   setFastSimulationLevel(int level);
  virtual void comment();
  
 protected:
  GsimE14LCV();
  int m_model;
  std::list<GsimDetector*> m_detMoveList;
};


#endif // GsimE14CV_h
