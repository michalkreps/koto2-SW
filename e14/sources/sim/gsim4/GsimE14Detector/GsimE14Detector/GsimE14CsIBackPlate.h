/**
 * Coded by Toru Matsumura
 * 2017
 */
#ifndef GsimE14CsIBackPlate_h
#define GsimE14CsIBackPlate_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14CsIBackPlate
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14CsIBackPlate : public GsimDetector
{
 public:
  GsimE14CsIBackPlate(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  virtual ~GsimE14CsIBackPlate();
  virtual void comment();
  virtual void setFastSimulationLevel(int level);
    
 protected:
  GsimE14CsIBackPlate();
  std::list<GsimDetector*> m_detMoveList;
 private:

};


#endif // GsimE14CsIBackPlate_h
