/**
 *  @file
 *  @brief  GsimE14IBCV
 *  $Id: GsimE14IBCV.h,v 1.2 2006/12/06 19:04:45 nanjo Exp $
 *  $Log: GsimE14IBCV.h,v $
 *  Revision 1.2  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 */
#ifndef GsimE14IBCV_h
#define GsimE14IBCV_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"

//forward declaration

/**
 *  @class GsimE14IBCV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14IBCV : public GsimDetector
{
 public:
  GsimE14IBCV(std::string name,
	     GsimDetector* motherDetector,
	     G4ThreeVector transV,
	     G4ThreeVector rotV,
	     int userFlag=0);
  
  virtual ~GsimE14IBCV();
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
  
 protected:
  GsimE14IBCV();
  std::list<GsimDetector*> m_detMoveList;

};


#endif // GsimE14IBCV_h
