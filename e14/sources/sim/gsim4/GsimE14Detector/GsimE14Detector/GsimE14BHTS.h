/**
 *  @file
 *  @brief  GsimE14BHTS
 *  $Id:$
 *  $Log:$
 */
#ifndef GsimE14BHTS_h
#define GsimE14BHTS_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14BHTS
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14BHTS : public GsimDetector
{
 public:
  GsimE14BHTS(std::string name,
	      GsimDetector* motherDetector,
	      G4ThreeVector transV,
	      G4ThreeVector rotV,
	      int userFlag=0);

  virtual ~GsimE14BHTS();
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
  
 protected:
  GsimE14BHTS();
};

     


#endif // GsimE14BHTS_h
