/**
 *  @file
 *  @brief  GsimE14CsICover
 *  $Id:$
 *  $Log:$
 */
#ifndef GsimE14CsICover_h
#define GsimE14CsICover_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14CsICover
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14CsICover : public GsimDetector
{
 public:
  GsimE14CsICover(std::string name,
			GsimDetector* motherDetector,
			G4ThreeVector transV,
			G4ThreeVector rotV,
			int nParameterized=-1);
  
  virtual ~GsimE14CsICover();
  virtual void comment();
  virtual void setFastSimulationLevel(int level);
 protected:
  GsimE14CsICover();
  std::list<GsimDetector*> m_detMoveList;
  
};


#endif // GsimE14CsICover_h
