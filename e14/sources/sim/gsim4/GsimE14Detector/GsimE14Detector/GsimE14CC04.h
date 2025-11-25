/**
 *  @file
 *  @brief  GsimE14CC04
 *  $Id$
 *  $Log$
 */
#ifndef GsimE14CC04_h
#define GsimE14CC04_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14CC04
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14CC04 : public GsimDetector
{
 public:
  GsimE14CC04(std::string name,
	      GsimDetector* motherDetector,
	      G4ThreeVector transV,
	      G4ThreeVector rotV,
	      int userFlag=0);
  
  virtual ~GsimE14CC04();

  static double getCC04Length();

  virtual void setFastSimulationLevel(int level);
  virtual void comment();

 protected:
  GsimE14CC04();
  int m_model;
  std::list<GsimDetector*> m_detList;
};




#endif // GsimE14CC04_h
