/**
 *  @file
 *  @brief  GsimKOTOIIUpstreamCollar
 *  $Id:
 *  $Log:
 */
#ifndef GsimKOTOIIUpstreamCollar_h
#define GsimKOTOIIUpstreamCollar_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimKOTOIIUpstreamCollar
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimKOTOIIUpstreamCollar : public GsimDetector
{
 public:
  GsimKOTOIIUpstreamCollar(std::string name,
			   GsimDetector* motherDetector,
			   G4ThreeVector transV,
			   G4ThreeVector rotV,
			   int userFlag=0);
  virtual ~GsimKOTOIIUpstreamCollar();
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
 protected:
  std::list<GsimDetector*> m_detList;
  std::list<GsimDetector*> m_detMoveList;
  GsimKOTOIIUpstreamCollar();
};


#endif // GsimKOTOIIUpstreamCollar_h
