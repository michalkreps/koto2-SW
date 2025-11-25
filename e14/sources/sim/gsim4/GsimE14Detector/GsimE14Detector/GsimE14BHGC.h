/**
 *  @file
 *  @brief  GsimE14BHGC
 *  $Id:$
 *  $Log:$
 */
#ifndef GsimE14BHGC_h
#define GsimE14BHGC_h

//includes
#include "GsimKernel/GsimDetector.h"
#include <string>
#include <list>



//forward declaration

/**
 *  @class GsimE14BHGC
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14BHGC : public GsimDetector
{
 public:
  GsimE14BHGC(std::string name,
	      GsimDetector* motherDetector,
	      G4ThreeVector transV,
	      G4ThreeVector rotV,
	      int userFlag=0);

  virtual ~GsimE14BHGC();
  
  virtual bool processHits(const G4Step* aStep,G4TouchableHistory* ROhist,
			   std::list<int>& channelList);
  
  virtual void setFastSimulationLevel(int level);
  virtual void comment();
  virtual void setUserParameters(std::string tag,std::vector<double> vec);
  
 protected:
  GsimE14BHGC();

  bool trace(const G4Step* aStep,G4TouchableHistory* ROhist,
	     std::list<int>& channelList);

  void getPath(G4ThreeVector& pos,
	       G4ThreeVector& vec,
	       double& pathLength,
	       int&    nReflectionY,
	       int&    nReflectionZ);

  double getPMTQE(double waveL);
  double getTransmittance(double lambda);
  double getRefractiveIndex(double lambda);

  double m_width;
  double m_height;
  double m_thick;

  int     m_nModule;
  double* m_factor;
  double* m_reflectance;
  
  bool    m_isInBeamPosition;
  
  bool m_opticalMode;

  std::list<GsimDetector*> m_detMoveList;

};

     


#endif // GsimE14BHGC_h
