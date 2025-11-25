/**
 *  @file
 *  @brief  GsimKOTOIIOuterVeto
 *  $Id:
 *  $Log:
 */
#ifndef GsimKOTOIIOuterVeto_h
#define GsimKOTOIIOuterVeto_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimKOTOIIOuterVeto
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimKOTOIIOuterVeto : public GsimDetector
{
 public:
  GsimKOTOIIOuterVeto(std::string name,
		      GsimDetector* motherDetector,
		      G4ThreeVector transV,
		      G4ThreeVector rotV,
		      int userFlag=0);
  virtual ~GsimKOTOIIOuterVeto();
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
 protected:
  std::list<GsimDetector*> m_detList;
  std::list<GsimDetector*> m_detMoveList;
  GsimKOTOIIOuterVeto();
};


#endif // GsimKOTOIIOuterVeto_h
