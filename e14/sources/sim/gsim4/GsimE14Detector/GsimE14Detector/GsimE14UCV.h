/**
 *  @file
 *  @brief  GsimE14UCV
 *  $Id$
 *  $Log$
 */

#ifndef GsimE14UCV_h
#define GsimE14UCV_h

//includes
#include "GsimKernel/GsimDetector.h"
#include <string>
#include <list>


//forward declaration

/**
 *  @class GsimE14UCV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14UCV : public GsimDetector
{
 public:
  GsimE14UCV(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14UCV();
  virtual void   setFastSimulationLevel(int level);
  virtual void comment();
  
 protected:
  GsimE14UCV();
  int m_model;
  std::list<GsimDetector*> m_detMoveList;
};


#endif // GsimE14CV_h
