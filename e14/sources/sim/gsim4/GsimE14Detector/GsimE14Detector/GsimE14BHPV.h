/**
 *  @file
 *  @brief  GsimE14BHPV
 *  $Id: GsimE14BHPV.h,v 1.2 2006/12/06 19:04:45 nanjo Exp $
 *  $Log: GsimE14BHPV.h,v $
 *  Revision 1.2  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 */
#ifndef GsimE14BHPV_h
#define GsimE14BHPV_h

//includes
#include <string>
#include <list>
#include "GsimKernel/GsimDetector.h"

//forward declaration
class G4Step;
class G4TouchableHistory;
class GsimWinstonCone;

/**
 *  @class GsimE14BHPV
 *  @brief Detector.
 *
 *  This class provides ...
 */

class GsimE14BHPV : public GsimDetector
{
 public:
  GsimE14BHPV(std::string name,
	      GsimDetector* motherDetector,
	      G4ThreeVector transV,
	      G4ThreeVector rotV,
	      int userFlag=0);
  
  virtual ~GsimE14BHPV();
  
  virtual void setFastSimulationLevel(int level);


  virtual void beginOfRunAction();
  virtual void endOfRunAction();
  virtual bool beginOfEventAction(const G4Event* evt);
  virtual bool endOfEventAction(const G4Event* evt);
  virtual bool processHits(const G4Step* aStep,G4TouchableHistory* ROhist, std::list<int>& channelList);
  
  virtual void comment();
  virtual void setUserParameters(std::string tag,std::vector<double> vec);
  
  double getBHPVLength();
  

 protected:
  bool traceFromInsideAerogel(const G4Step* aStep,G4TouchableHistory* ROhist,
			      std::list<int>& channelList,bool isAirCerenkov);
  
  bool traceWCone(double waveL,double* pPos, double* pVec);
  bool traceAerogel(double waveL,double* pPos, double* pVec,int ModID);
  bool traceOpticalBox(double waveL,double* pPos, double* pVec);
  bool traceOpticalBox_2(double waveL,double* pPos, double* pVec, int ModID,
			 bool IsAirCerenkov);//"_2" means 2-PMT readout
  void scatterInAerogel(double* pVec);
  double getPMTQE(double waveL);

  void translateFromGlobalToAG(const G4Step* aStep,double* pPos, double* pVec);
  void translateFromOBoxToWCone(double* pPos, double* pVec);

  double dNextAbsorption(double waveL,int modID);
  double dNextScattering(double waveL,int modID);

  int chooseWCone(double* pPos, double* pVec);

  
  
 


  



  
  bool   m_opticalMode;
  bool   m_opticalAir;
  
  //optical box 
  double m_angleThick;
  double m_angleSize;
  double m_plateThick;
  double m_OBoxX;
  double m_OBoxY;
  double m_OBoxZ;
  double m_bottomplateThick;
  double m_topplateThick;
  
  //aerogel
  double m_AGBoxX;
  double m_AGBoxZ;
  double m_AGBoxBottomPlateThick;
  double m_AGBoxXFrameThick;
  double m_AGBoxYFrameThick;
  double m_AGBoxCrowLength;
  double m_AGX;
  double m_AGY;
  double m_refractiveIndexAG;
  double m_lenTile;

  //flat mirror
  double m_mirrorThick;
  double m_mirrorY;
  double m_mirrorX;
  double m_mirrorCrossZ;
  double m_mirrorAngle;
  double m_mirrorAngleThick;
  double m_mirrorAngleSize;
  double m_mirrorAngleLength;

  //side mirror
  double m_SideMirrorX;
  double m_SideMirrorZ;
  double m_spacerThick;
  double m_spacerWide;
  double m_spacerNarrow;
  double m_spacer1Z;
  double m_spacer2Z;
  double m_spacer2X;

  //Winston Cone and PMT
  double m_gapOBoxWCone;
  double m_dInWCone;
  double m_dOutWCone;
  double m_rPMT;
  double m_gapWConePMT;
  double m_heightWCone;
  GsimWinstonCone* m_winstonCone;

  //reflectance and transmittance
  double m_reflectanceMirror;
  double m_reflectanceWall;
  double m_reflectanceWCone;
  double m_transmittanceWrap;
  
  double m_reflectanceAGWall[3];
  int m_nMaxReflectionAGWall[3];
  int m_nReflectionAGWall[3];

  int m_nReflectionOBoxWall;
  int m_nReflectionMirror;
  int m_nReflectionWConeWall;

  //pathlength
  double m_pathLengthAG;
  double m_pathLengthOBox;
  double m_pathLengthWCone;
  double m_pathLengthAir;
  
  std::list<GsimDetector*> m_detList;
  GsimE14BHPV();

  //variable to save simulation output
  int* m_nPE;//from both aerogel and air Cerenkov
  int* m_nPE_airCerenkov;
  double* m_HitTime;




  //Parameter for initial setup
  int    m_nMaxPE;
  int     m_nMod; 
  double *m_PbThick;
  double *m_PbAGGap;//distance between upstream edge of Pb and downstream edge of AG box bottom
  double *m_AGThick;
  double *m_ModGap;
  double *m_AGParamAbsorption;
  double *m_AGParamScattering;
  double *m_OverallFactor;
};

      


#endif // GsimE14BHPV_h
