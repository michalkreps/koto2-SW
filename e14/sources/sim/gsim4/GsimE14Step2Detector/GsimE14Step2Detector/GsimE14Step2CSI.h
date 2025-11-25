/**
 *  @file
 *  @brief  GsimE14Step2CSI
 */
#ifndef GsimE14Step2CSI_h
#define GsimE14Step2CSI_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14Step2CSI
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14Step2CSI : public GsimDetector
{
 public:
  GsimE14Step2CSI(std::string name,
		  GsimDetector* motherDetector,
		  G4ThreeVector transV,
		  G4ThreeVector rotV,
		  int userFlag=0);
  
  virtual ~GsimE14Step2CSI();
  virtual void comment();
 protected:
  GsimE14Step2CSI();
  
};


#endif // GsimE14Step2CSI
