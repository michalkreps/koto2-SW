/**
 *  @file
 *  @brief  GsimE14IBAR
 *  $Log: GsimE14IBAR.h,v $
 *
 */
#ifndef GsimE14IBAR_h
#define GsimE14IBAR_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14IBAR
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14IBAR : public GsimDetector
{
 public:
  GsimE14IBAR(std::string name,
	  GsimDetector* motherDetector,
	  G4ThreeVector transV,
	  G4ThreeVector rotV,
          int userFlag=0);
  
  virtual void   setFastSimulationLevel(int level);
  virtual void comment();
  
  virtual ~GsimE14IBAR();

 protected:
  std::list<GsimDetector*> m_detList;
  GsimE14IBAR();

};


#endif // GsimE14IBAR_h
