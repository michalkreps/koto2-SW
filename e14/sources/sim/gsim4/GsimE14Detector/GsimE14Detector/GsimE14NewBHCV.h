/**
 *  @file
 *  @brief  GsimE14NewBHCV
 *  $Id:$
 *  $Log:$
 */
#ifndef GsimE14NewBHCV_h
#define GsimE14NewBHCV_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14NewBHCV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14NewBHCV : public GsimDetector
{
 public:
  GsimE14NewBHCV(std::string name,
		 GsimDetector* motherDetector,
		 G4ThreeVector transV,
		 G4ThreeVector rotV,
		 int userFlag=0);

  virtual ~GsimE14NewBHCV();
  virtual void setFastSimulationLevel(int level);

  virtual void comment();
  
 protected:
  GsimE14NewBHCV();
 protected:
  std::list<GsimDetector*> m_detMoveList;
};

#endif // GsimE14NewBHCV_h
