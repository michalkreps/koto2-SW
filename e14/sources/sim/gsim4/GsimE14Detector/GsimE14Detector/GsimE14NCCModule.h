/**
 *  @file
 *  @brief  GsimE14NCCModule
 *  $Id: $
 *  $Log: $
 */
#ifndef GsimE14NCCModule_h
#define GsimE14NCCModule_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14NCCModule
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14NCCModule : public GsimDetector
{
 public:
  GsimE14NCCModule(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14NCCModule();
  virtual bool endOfEventAction(const G4Event* );
 protected:
  GsimE14NCCModule();
};

#endif // GsimE14NCCModule_h
