/**
 *  @file
 *  @brief  GsimE14BHPV
 *  $Id: $ 
 *  $Log: $
 */
#include "GsimE14Detector/GsimE14BHPV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimTools/GsimWinstonCone.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "CLHEP/Random/Randomize.h"

#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4VTouchable.hh"
#include "G4NavigationHistory.hh"
#include "G4SystemOfUnits.hh"

#include <list>

using namespace E14;

GsimE14BHPV::GsimE14BHPV(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{


  m_nMaxPE=300;
  m_userFlag = userFlag;
  //userFlag check
  ////////////////////////////
  if(userFlag==2502) {
    m_opticalMode=true;
    m_opticalAir =true;
    m_nMod=25; 
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];
    double tmp_PbThick[25] = { 0.15, 0.15, 0.15, 0.15, 0.15,
			       0.15, 0.15, 0.15, 0.15, 0.15,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0.3, 0.3, 0.3, 0.0, 0.0 };//[cm]
    double tmp_PbAGGap[25] = { 1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5 };//[cm]
    double tmp_AGThick[25] = { 2.9, 2.9, 2.9, 2.9, 2.9,
			       2.9, 2.9, 2.9, 2.9, 2.9,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8 };//[cm]
    double tmp_ModGap[25] = { 0.5, 0.5, 0.5, 0.5, 0.5,
			      0.5, 0.5, 0.5, 0.5, 0.5,
			      0.5, 0.5, 0.5, 0.5, 0.5,
			      0.5, 0.5, 0.5, 0.5, 0.5,
			      0.5, 0.5, 0.5, 0.5, 0.5 };//last entry is ignored
    
    double tmp_AGParamAbsorption[25] = { 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972 };
    double tmp_AGParamScattering[25] = { 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692 };
    double tmp_OverallFactor[25] = { 0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8 };//[cm]

    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      m_OverallFactor[i]=tmp_OverallFactor[i];
    }
      
  } else if(userFlag==2011 || userFlag==25) {
    if(userFlag==2011) {
      m_opticalMode=false;
      m_opticalAir =false;
    } else {
      m_opticalMode=true;
      m_opticalAir =true;
    }
    m_nMod=25; 
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];

    double tmp_PbThick[25] = { 0.15, 0.0, 0.0, 0.0, 0.0,
			       0.15, 0.15, 0.15, 0.15, 0.15,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0.3, 0.3, 0.45, 0.0, 0.0 };//[cm]
    double tmp_PbAGGap[25] = { 5.8, 5.8, 5.8, 5.8, 5.8, 
			       4.4, 4.4, 4.4, 4.4, 4.4, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5 };//[cm]
    double tmp_AGThick[25] = { 1.5, 1.5, 1.5, 1.5, 1.5,
			       2.9, 2.9, 2.9, 2.9, 2.9,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8 };//[cm]
    double tmp_ModGap[25] = { 0.6, 4.7, 0.4, 4.0, 0.3,
			      2.8, 0.6, 5.7, 0.5, 4.8,
			      0.6, 4.7, 0.4, 4.0, 0.3,
			      2.8, 0.6, 5.7, 0.5, 4.8,
			      0.6, 4.7, 0.4, 4.0, 16.8 };//last entry is ignored
    double tmp_AGParamAbsorption[25] = { 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972,
					 0.972, 0.972, 0.972, 0.972, 0.972 };
    double tmp_AGParamScattering[25] = { 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692 };
    double tmp_OverallFactor[25] = { 0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8,
				     0.8, 0.8, 0.8, 0.8, 0.8 };//[cm]

    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      m_OverallFactor[i]=tmp_OverallFactor[i];
    }
  } else if(userFlag==1) {
    //single optical
    m_opticalMode=true;
    m_opticalAir =true;
    m_nMod = 1;
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];
    
    double tmp_PbThick[1] = { 0.15 };//[cm]
    double tmp_PbAGGap[1] = { 1.5 };//[cm]
    double tmp_AGThick[1] = { 5.8 };//[cm]
    double tmp_ModGap[1] = { 16.8 };//last entry is ignored
    double tmp_AGParamAbsorption[1] = { 0.972 };
    double tmp_AGParamScattering[1] = { 0.00692 };
    double tmp_OverallFactor[1] = { 0.58 };//[cm]

    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      m_OverallFactor[i]=tmp_OverallFactor[i];
    }
    
  } else if(userFlag==201303 ||
	    userFlag==201304 ||
	    userFlag==201305) {
    //2013Mar optical
    m_opticalMode=true;
    m_opticalAir =true;
    m_nMod = 12;
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];
    
    double tmp_PbThick[12] = { 0.15, 0.15, 0.15, 0.15, 0.15,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0, 0 };//[cm]
    
    double tmp_PbAGGap[12] = { 1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5 };//[cm]
    double tmp_AGThick[12] = { 5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8 };//[cm]
    double tmp_ModGap[12] = { 0.6, 4.7, 0.4, 4.0, 0.3,
			      2.8, 0.6, 5.7, 0.5, 4.8,
			      0.5, 16.8 };
    
    double  tmp_AGParamAbsorption[12] = { 0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972 };
    
    double tmp_AGParamScattering[12] = { 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692 };
    double tmp_OverallFactor[12] =  { 0.658, 0.890, 0.828, 0.788, 0.911,
				      0.770, 0.866, 0.782, 0.860, 0.788, 
				      0.869, 0.847 };//liner interpolation of results after 1st and 2nd tune
    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      m_OverallFactor[i]=tmp_OverallFactor[i];
    }

  } else if(userFlag==201411) {
    //add 4 modules toward 2014 Feb
    m_opticalMode=true;
    m_opticalAir =true;
    m_nMod = 12+4;
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];
    
    double tmp_PbThick[16] = { 0.15, 0.15, 0.15, 0.15, 0.15,
			       0.3, 0.3, 0.3, 0.3, 0.3,
			       0.3, 0.3, 0.3, 0.3,
			       0, 0 };//[cm]
    
    double tmp_PbAGGap[16] = { 1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5,
			       1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5 };//[cm]
    double tmp_AGThick[16] = { 5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8 };//[cm]
    double tmp_ModGap[16] = { 0.6, 4.7, 0.4, 4.0, 0.3,
			      2.8, 0.6, 5.7, 0.5, 4.8,
			      0.4,4.0,0.4,4.0,
			      0.5, 16.8 };
    
    double  tmp_AGParamAbsorption[16] = { 0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972 };
    
    double tmp_AGParamScattering[16] = { 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 
					 0.00692, 0.00692 };
    double tmp_OverallFactor[16] =  { 0.658, 0.890, 0.828, 0.788, 0.911,
				      0.770, 0.866, 0.782, 0.860, 0.788,
				      0.85,0.85,0.85,0.85,
				      0.869, 0.847 };//liner interpolation of results after 1st and 2nd tune
    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      m_OverallFactor[i]=tmp_OverallFactor[i];
    }
  } else if(userFlag==201503 ||
	    userFlag==201504 ||
	    userFlag==201605 ||
	    userFlag==201902 ||
	    userFlag==202002 ||
	    userFlag==202102 ||
	    userFlag==202306) {
    //add 4 modules 2015 Apr.
    m_opticalMode=true;
    m_opticalAir =true;
    m_nMod = 12+4;
    m_PbThick = new double[m_nMod];
    m_PbAGGap = new double[m_nMod];
    m_AGThick = new double[m_nMod];
    m_ModGap = new double[m_nMod];
    m_AGParamAbsorption = new double[m_nMod];
    m_AGParamScattering = new double[m_nMod];
    m_OverallFactor = new double[m_nMod];
    
    double tmp_PbThick[16] = { 0.15, 0.15, 0.15, 0.15,
			       0.3, 0.3, 0.3, 0.3,
			       0.15,0.3, 
			       0.3, 0.3, 0.3, 0.3,
			       0, 0 };//[cm]

    if(userFlag>=201605) {
      //Original good order in lead thickness.
      tmp_PbThick[4]=0.15;
      tmp_PbThick[8]=0.3;
    }
    
    
    double tmp_PbAGGap[16] = { 1.5, 1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5, 1.5, 1.5, 1.5,
			       1.5, 1.5, 1.5, 1.5, 
			       1.5, 1.5 };//[cm]
    double tmp_AGThick[16] = { 5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8, 5.8, 5.8,
			       5.8, 5.8 };//[cm]
    double tmp_ModGap[16] = { 0.6, 6.9,
			      0.4, 6.4,
			      0.6,8.3,
			      0.5, 3.4,
			      0.3, 5.1,
			      0.6,2.6,
			      0.5,5.6,
			      0.5, 4.0 };

    // after 201605, gap values are overwritten
    // added by Shimizu based on measurments
    if(userFlag==201605){
      
      tmp_ModGap[0]  = 0.7;
      tmp_ModGap[1]  = 0.5;
      tmp_ModGap[2]  = 0.7;
      tmp_ModGap[3]  = 0.1;
      tmp_ModGap[4]  = 0.3;
      tmp_ModGap[5]  = 0.1;
      tmp_ModGap[6]  = 0.3;
      tmp_ModGap[7]  = 0.1;
      tmp_ModGap[8]  = 0.2;
      tmp_ModGap[9]  = 0.1;
      tmp_ModGap[10] = 0.4;
      tmp_ModGap[11] = 0.5;
      tmp_ModGap[12] = 0.2;
      tmp_ModGap[13] = 0.1;
      tmp_ModGap[14] = 0.1;
      tmp_ModGap[15] = 4.2;
      
    } else if(userFlag==201902 ||
	      userFlag==202002 ||
	      userFlag==202102 
	      ) {
      
      tmp_ModGap[1]  = 0.8;
      tmp_ModGap[3]  = 1.1;
      tmp_ModGap[5]  = 1.5;
      tmp_ModGap[7]  = 0.6;
      tmp_ModGap[9]  = 0.5;
      tmp_ModGap[11] = 0.1;
      tmp_ModGap[13] = 0.4;
      
    }
    
    double  tmp_AGParamAbsorption[16] = { 0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972, 0.972, 0.972,
					  0.972, 0.972 };
    
    double tmp_AGParamScattering[16] = { 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 0.00692,
					 0.00692, 0.00692, 0.00692, 0.00692, 
					 0.00692, 0.00692 };

    double tmp_OverallFactor[16] =  { 0.705474992,
				      0.676551381,
				      0.788052328,
				      0.78167357,
				      0.700775199,
				      0.570564365,
				      0.686107653,
				      0.748243557,
				      0.818147861,
				      0.652563868,
				      0.679968235,
				      0.514393654,
				      0.69652337,
				      0.4455011,
				      0.689277554,
				      0.784875819 };//First try by Shinohara. No iteration.


    double tmp_OverallFactor201605[16] = { 0.661,
					   0.655,
					   0.718,
					   0.699,
					   0.701,
					   0.601,
					   0.602,
					   0.634,
					   0.758,
					   0.673,
					   0.658,
					   0.580,
					   0.711,
					   0.502,
					   0.671,
					   0.809 };//Obtained afte twice iterations by Shinohara.
    
    
    for(int i=0;i<m_nMod;i++) {
      m_PbThick[i]=tmp_PbThick[i];
      m_PbAGGap[i]=tmp_PbAGGap[i];
      m_AGThick[i]=tmp_AGThick[i];
      m_ModGap[i]=tmp_ModGap[i];
      m_AGParamAbsorption[i]=tmp_AGParamAbsorption[i];
      m_AGParamScattering[i]=tmp_AGParamScattering[i];
      if(userFlag==201605) {
	m_OverallFactor[i]=tmp_OverallFactor201605[i];
      } else {
	m_OverallFactor[i]=tmp_OverallFactor[i];
      }
    }
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BHCV is not constructed.");
  }

  ////////////////////////////

  m_className = "GsimE14BHPV";
  // BHPV

  if(m_opticalMode) {
    GsimUtil::getUtil()->addCerenkov();
  }



  ////////////////////////////////////////////
  //optical box 
  m_angleThick = 0.5;//[cm]
  m_angleSize = 5.0;//[cm]
  m_plateThick = 0.1;//[cm]
  m_OBoxX = 40.0;//[cm]
  m_OBoxY = 32.0;//[cm]
  m_OBoxZ = 32.0;//[cm]
  m_bottomplateThick = 0.4;//[cm]
  m_topplateThick = 0.2;//[cm]

  //aerogel
  m_AGBoxX = 33.4;//[cm], outer size of aerogel box ; Y size is equal to m_OBoxY 
  //m_AGBoxZ = 5.5;//[cm], exlucde bottom Al plate
  m_AGBoxZ = 0.3;//[cm] length of aerogel protrusion
  m_AGBoxBottomPlateThick = 0.1;//[cm]
  m_AGBoxXFrameThick = 0.8;//[cm], right and left part
  m_AGBoxYFrameThick = 0.1;//[cm], upper and lower part
  m_AGBoxCrowLength = 3.0;//[cm]
  m_AGX = 31.8;//[cm]
  m_AGY = 31.8;//[cm]
  m_refractiveIndexAG =1.03;
  //m_lenTile=10.0;//[cm], not used
  
  //flat mirror
  m_mirrorThick = 0.075;//[cm]
  m_mirrorX = 28.18;//[cm]
  m_mirrorY = 29.35;//[cm]
  m_mirrorCrossZ = 10.5+std::sqrt(2)*m_mirrorThick;//[cm], distance between downstream of Al plate and inner cross point of mirrors
  m_mirrorAngle = 45.;//[degree]
  m_mirrorAngleThick = 0.1;//[cm]
  m_mirrorAngleSize = 2.0;//[cm]
  m_mirrorAngleLength = m_mirrorY-0.05*2;//[cm]

  //side mirror
  m_SideMirrorX = 39.9;//[cm]
  m_SideMirrorZ = 24.85-0.6;//[cm]
  m_spacerThick = 1.2;//[cm]
  m_spacerWide = 5.0;//[cm]
  m_spacerNarrow = 2.0;//[cm]
  m_spacer1Z = 12.6;//[cm], from downstream edge of module
  m_spacer2Z = 2.6;//[cm], from downstream edge of module
  m_spacer2X = 10.0;//[cm], from downstream edge of module
  
  //Winston Cone and PMT
  m_gapOBoxWCone = 0.2;//[cm]
  m_dInWCone = 30.0;//[cm]
  m_dOutWCone = 12.0;//[cm]
  m_rPMT = 5.5;//[cm] modified at 08/8/26 from 6.0 why not 6.0cm?
  m_gapWConePMT = 1.0;//[cm]
  m_winstonCone = new GsimWinstonCone(m_dInWCone,m_dOutWCone);
  m_heightWCone=m_winstonCone->getHight();

  //reflectance and transmittance
  m_reflectanceMirror=0.85;//from data of Yokohama Kiko
  m_reflectanceWall=0.85;
  m_reflectanceWCone=0.85;//from data of Yokohama Kiko
  m_transmittanceWrap=0.9;
  
  for(int k=0;k<3;k++) { 
    m_reflectanceAGWall[k]=0.0;
  }
  for(int k=0;k<3;k++) { 
    m_nMaxReflectionAGWall[k]=0;
  }
  for(int k=0;k<3;k++) {
    m_nReflectionAGWall[k]=0;
  }
  m_nReflectionOBoxWall=0;
  m_nReflectionMirror=0;
  m_nReflectionWConeWall=0;

  //pathlength
  m_pathLengthAG = 0.;
  m_pathLengthOBox = 0.;
  m_pathLengthWCone = 0.;
  m_pathLengthAir = 0.;
  
  //variables to save simulation data
  int  nCh = m_nMod*2;
  m_nPE = new int[nCh];
  m_nPE_airCerenkov = new int[nCh];
  m_HitTime = new double[nCh*m_nMaxPE];
  for( int iCh=0;iCh<nCh;iCh++ ){
    m_nPE[iCh] = 0;
    m_nPE_airCerenkov[iCh] = 0;
    for( int iPE=0;iPE<m_nMaxPE;iPE++ ) m_HitTime[iCh*m_nMaxPE+iPE] = 0; 
  }

  //--------------------------------------------------------------------------------------------
  //create materials

  double x_mod = m_OBoxX;
  double y_mod = m_OBoxY+m_bottomplateThick+m_topplateThick+4*m_angleThick;
  double z_mod =//length of one module
    (m_angleThick + m_plateThick)*2//upstream and downstream
    + m_OBoxZ;//33.2cm
  double zLengthTotal = m_nMod*z_mod;
  for( int iMod=0 ; iMod<(m_nMod-1) ; iMod++ ) zLengthTotal += m_ModGap[iMod];

  {
    std::ostringstream ostr;
    ostr << "BHPV total length : " << zLengthTotal  
	 << "cm (# of modules :" << m_nMod << ")" << std::endl;
    GsimMessage::getInstance()
      ->report("info",ostr.str());
  }
      
  double current_z = (-zLengthTotal/2.+z_mod/2.);
  
  for( int iMod=0 ; iMod<m_nMod ; iMod++ ){
    
    GsimBox* boxMod = 0;
    if(boxMod==0) {
      std::ostringstream ModName;
      ModName << "BHPVMod_" << iMod;
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( y_mod*cm );
      paramVec.push_back( z_mod*cm );
      double y = (m_topplateThick-m_bottomplateThick)/2.;
      boxMod = new GsimBox( ModName.str(), this,
			    G4ThreeVector(0,y*cm,current_z*cm),
			    G4ThreeVector(0,0,0) );
      boxMod->setParameters( paramVec );
      boxMod->setOuterMaterial( "G4_AIR" );
      boxMod->setOuterCopyNo( iMod );
      boxMod->setOuterVisibility( false );
      addDaughter( boxMod );
      m_detList.push_back( boxMod );
    } else {
      boxMod->cloneDetector( G4ThreeVector(0,0,current_z*cm),
			     G4ThreeVector(0,0,0),
			     iMod );
    }
    
    current_z += (z_mod+m_ModGap[iMod]);
    GsimBox* OAbox = 0;
    double OAz=0;
    if( m_opticalAir ){//opticalAir
      
      double AGDownstreamFromOBoxUpstream = 
	m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick
	+ m_AGThick[iMod];
      OAz=AGDownstreamFromOBoxUpstream/2.; 
      
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( m_mirrorY*cm );
      paramVec.push_back( (m_OBoxZ-AGDownstreamFromOBoxUpstream)*cm );
      std::ostringstream DetName;
      DetName << "BHPVOpticalAir_" << iMod;
      double z = OAz;
      OAbox = new GsimBox( DetName.str(), boxMod,
			   G4ThreeVector(0,0,z*cm),
			   G4ThreeVector(0,0,0) );
      OAbox->setParameters( paramVec );
      OAbox->setOuterMaterial( "GsimOpticalAir" );
      OAbox->setOuterColor( "blue" );
      if(m_opticalMode) {
	OAbox->setSensitiveDetector( "BHPV", -1, false, true, true, this );
      } else {
	OAbox->setSensitiveDetector( "BHPV", -1, false, true, true);
      }
      boxMod->addDaughter(OAbox);
    }

    if( std::fabs(m_angleThick)>0.001 ){//X-angleUp

      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( (m_angleSize-m_angleThick)*cm );
      paramVec.push_back( m_angleThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_XangleUp_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick/2.;
      double y = m_OBoxY/2.+m_topplateThick
	- (m_angleSize-m_angleThick)/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //downstream
      z *= (-1);
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_angleThick)>0.001 ){//X-angleDown

      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( (m_angleSize-m_angleThick)*cm );
      paramVec.push_back( m_angleThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_XangleDown_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick/2.;
      double y = -m_OBoxY/2.-m_bottomplateThick
	+ (m_angleSize-m_angleThick)/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //downstream
      z *= (-1);
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_plateThick)>0.001 ){//upstream and downstream plate
      
      std::vector<double> paramVec;
      paramVec.push_back( m_OBoxX*cm );
      paramVec.push_back( m_OBoxY*cm );
      paramVec.push_back( m_plateThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_plate_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,0,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //downstream
      z *= (-1);
      box->cloneDetector( G4ThreeVector(0,0,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_PbThick[iMod])>0.001 ){//Pb plate

      std::vector<double> paramVec;
      paramVec.push_back( m_OBoxX*cm );
      paramVec.push_back( m_OBoxY*cm );
      paramVec.push_back( m_PbThick[iMod]*cm );
      std::ostringstream DetName;
      DetName << "BHPV_Pb_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbThick[iMod]/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,0,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "GsimPbSb4" );
      box->setOuterColor( "blue" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );
    }
    
    if( std::fabs(m_AGBoxBottomPlateThick)>0.001 ){//bottom(upstream) plate of AG Box
      
      std::vector<double> paramVec;
      paramVec.push_back( m_AGBoxX*cm );
      paramVec.push_back( m_OBoxY*cm );
      paramVec.push_back( m_AGBoxBottomPlateThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_AGBoxBottom_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,0,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );
    }
    
    if( std::fabs(m_AGBoxXFrameThick)>0.001 ){//AG Box vertical(right and left) frame
      
      std::vector<double> paramVec;
      paramVec.push_back( m_AGBoxXFrameThick*cm );
      paramVec.push_back( (m_OBoxY-2*m_AGBoxYFrameThick)*cm );
      paramVec.push_back( (m_AGThick[iMod]-m_AGBoxZ)*cm );
      std::ostringstream DetName;
      DetName << "BHPV_AGBoxX_" << iMod;
      double x = m_AGBoxX/2.-m_AGBoxXFrameThick/2.;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick
	+ (m_AGThick[iMod]-m_AGBoxZ)/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(x*cm,0,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_POLYACRYLONITRILE" );
      box->setOuterColor( "green" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //opposite side(x<0, right)
      x *= (-1);
      box->cloneDetector( G4ThreeVector(x*cm,0,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_AGBoxYFrameThick)>0.001 ){//AG Box horizontal(upper and lower) frame

      std::vector<double> paramVec;
      paramVec.push_back( m_AGBoxX*cm );
      paramVec.push_back( m_AGBoxYFrameThick*cm );
      paramVec.push_back( (m_AGThick[iMod]-m_AGBoxZ)*cm );
      std::ostringstream DetName;
      DetName << "BHPV_AGBoxY_" << iMod;
      double y = m_OBoxY/2.-m_AGBoxYFrameThick/2.;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick
	+ (m_AGThick[iMod]-m_AGBoxZ)/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "green" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //opposite side(x<0, right)
      y *= (-1);
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_AGBoxYFrameThick)>0.001 ){//AG Box horizontal frame crow
      
      std::vector<double> paramVec;
      paramVec.push_back( m_AGBoxYFrameThick*cm );
      paramVec.push_back( m_AGBoxCrowLength*cm );
      paramVec.push_back( (m_AGThick[iMod]-m_AGBoxZ)*cm );
      std::ostringstream DetName;
      DetName << "BHPV_AGBoxCrow_" << iMod;
      double x = m_AGBoxX/2.+m_AGBoxYFrameThick/2.;
      double y = m_OBoxY/2.-m_AGBoxCrowLength/2.;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick
	+ (m_AGThick[iMod]-m_AGBoxZ)/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(x*cm,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "green" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );
      
      //another 3 sides 
      box->cloneDetector( G4ThreeVector((-1)*x*cm,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
      box->cloneDetector( G4ThreeVector(x*cm,(-1)*y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
      box->cloneDetector( G4ThreeVector((-1)*x*cm,(-1)*y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    
    if( std::fabs(m_AGThick[iMod])>0.001 ){//aerogel
      
      std::vector<double> paramVec;
      paramVec.push_back( m_AGX*cm );
      paramVec.push_back( m_AGY*cm );
      paramVec.push_back( m_AGThick[iMod]*cm );
      std::ostringstream DetName;
      DetName << "BHPV_aerogel_" << iMod;
      double z = -z_mod/2.
	+ m_angleThick
	+ m_plateThick
	+ m_PbAGGap[iMod]
	+ m_AGBoxBottomPlateThick
	+ m_AGThick[iMod]/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,0,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      if( m_opticalMode ) {
	box->setOuterMaterial( "GsimOpticalAerogel103" );
      } else {
	box->setOuterMaterial( "GsimAerogel" );
      }
      box->setOuterColor( "yellow" );
      box->setSensitiveDetector( "BHPV", -1, false, true, true, this );
      boxMod->addDaughter(box);
    }

    if( std::fabs(m_mirrorThick)>0.001 ){//flat mirrors

      std::vector<double> paramVec;
      paramVec.push_back( m_mirrorX*cm );
      paramVec.push_back( m_mirrorY*cm );
      paramVec.push_back( m_mirrorThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_mirror_" << iMod;
      double x = (m_mirrorX+m_mirrorThick)/(2*std::sqrt(2));
      double z = -m_OBoxZ/2.//-z_mod/2. <-- modified at 2010/5/20
	+ m_mirrorCrossZ
	+ (m_mirrorX-m_mirrorThick)/(2*std::sqrt(2));
      double angle = m_mirrorAngle+90;
      GsimBox* box = 0;
      if(OAbox) {
	box = new GsimBox( DetName.str(), OAbox,
			   G4ThreeVector(x*cm,0,(z-OAz)*cm),
			   G4ThreeVector(0,angle*deg,0) );
      } else {
	box = new GsimBox( DetName.str(), boxMod,
			   G4ThreeVector(x*cm,0,z*cm),
			   G4ThreeVector(0,angle*deg,0) );
      }
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      if(OAbox) {
	OAbox->addDaughter( box );
      } else {
	boxMod->addDaughter( box );
      }
      m_detList.push_back( box );

      //opposite side 
      x *= (-1);
      angle *= (-1);
      box->cloneDetector( G4ThreeVector(x*cm,0,(z-OAz)*cm),
			  G4ThreeVector(0,angle*deg,0), 0 );
    }

    if( std::fabs(m_mirrorAngleThick)>0.001 ){//angle to fix flat mirrors

      std::vector<double> paramVec;
      paramVec.push_back( (m_mirrorAngleSize-m_mirrorAngleThick*2)*cm );
      paramVec.push_back( m_mirrorAngleLength*cm );
      paramVec.push_back( m_mirrorAngleThick*cm );
      std::ostringstream DetName;
      DetName << "BHPV_mirrorAngle_" << iMod;
      double x = (m_mirrorAngleSize-m_mirrorAngleThick)/(2*std::sqrt(2));
      double z = -m_OBoxZ/2.//-z_mod/2. <-- modified at 2010/5/20
	+ m_mirrorCrossZ
	+ (m_mirrorAngleSize+m_mirrorAngleThick)/(2*std::sqrt(2));
      double angle = m_mirrorAngle+90;
      GsimBox* box = 0;
      if(OAbox) {
	box = new GsimBox( DetName.str(), OAbox,
			   G4ThreeVector(x*cm,0,(z-OAz)*cm),
			   G4ThreeVector(0,angle*deg,0) );
      } else {
	box = new GsimBox( DetName.str(), boxMod,
			   G4ThreeVector(x*cm,0,z*cm),
			   G4ThreeVector(0,angle*deg,0) );
      }
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      if(OAbox) {
	OAbox->addDaughter( box );
      } else {
	boxMod->addDaughter( box );
      }
      m_detList.push_back( box );

      //opposite side 
      x *= (-1);
      angle *= (-1);
      box->cloneDetector( G4ThreeVector(x*cm,0,(z-OAz)*cm),
			  G4ThreeVector(0,angle*deg,0), 0 );
    }

    if( std::fabs(m_mirrorThick)>0.001 ){//side mirrors

      std::vector<double> paramVec;
      paramVec.push_back( m_SideMirrorX*cm );
      paramVec.push_back( m_mirrorThick*cm );
      paramVec.push_back( m_SideMirrorZ*cm );
      std::ostringstream DetName;
      DetName << "BHPV_sidemirror_" << iMod;
      double y = (m_mirrorY+m_mirrorThick)/2.;
      double z = z_mod/2.
	- 0.65 
	- m_SideMirrorZ/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //opposite side 
      y *= (-1);
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_spacerThick)>0.001 ){//mirror spacer1 (upstream)

      std::vector<double> paramVec;
      paramVec.push_back( m_spacerNarrow*cm );
      paramVec.push_back( m_spacerThick*cm );
      paramVec.push_back( m_spacerWide*cm );
      std::ostringstream DetName;
      DetName << "BHPV_spacer1_" << iMod;
      double y = m_mirrorY/2.+m_mirrorThick+m_spacerThick/2.;
      double z = z_mod/2. - m_spacer1Z;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //opposite side 
      y *= (-1);
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_spacerThick)>0.001 ){//mirror spacer2 (downstream)

      std::vector<double> paramVec;
      paramVec.push_back( m_spacerWide*cm );
      paramVec.push_back( m_spacerThick*cm );
      paramVec.push_back( m_spacerNarrow*cm );
      std::ostringstream DetName;
      DetName << "BHPV_spacer2_" << iMod;
      double x = m_spacer2X;
      double y = m_mirrorY/2.+m_mirrorThick+m_spacerThick/2.;
      double z = z_mod/2. - m_spacer2Z;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(x*cm,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      //other 3 ones
      x *= (-1);//opposite side(right) of higher (x<0,y>0) 
      box->cloneDetector( G4ThreeVector(x*cm,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
      y *= (-1);//right side of lower (x<0,y<0) 
      box->cloneDetector( G4ThreeVector(x*cm,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
      x *= (-1);//left side of lower (x<0,y<0) 
      box->cloneDetector( G4ThreeVector(x*cm,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }
    
    if( std::fabs(m_topplateThick)>0.001 ){//top plate
      
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( m_topplateThick*cm );
      paramVec.push_back( (z_mod-2*m_angleThick)*cm );
      std::ostringstream DetName;
      DetName << "BHPV_topplate_" << iMod;
      double y = m_OBoxY/2.
	+ m_topplateThick/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,0),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );
    }

    if( std::fabs(m_bottomplateThick)>0.001 ){//bottom plate
      
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( m_bottomplateThick*cm );
      paramVec.push_back( (z_mod-2*m_angleThick)*cm );
      std::ostringstream DetName;
      DetName << "BHPV_bottomplate_" << iMod;
      double y = -m_OBoxY/2.
	- m_bottomplateThick/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,0),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );
    }

    if( std::fabs(m_angleThick)>0.001 ){//X-angle top crow
      
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( m_angleThick*cm );
      paramVec.push_back( m_angleSize*cm );
      std::ostringstream DetName;
      DetName << "BHPV_angleCrowTop_" << iMod;
      double y = m_OBoxY/2.
	+ m_topplateThick
	+ m_angleThick/2.;
      double z = -z_mod/2.
	+ m_angleSize/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      z *= (-1);//left side of lower (x<0,y<0) 
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    if( std::fabs(m_angleThick)>0.001 ){//X-angle bottom crow
      
      std::vector<double> paramVec;
      paramVec.push_back( x_mod*cm );
      paramVec.push_back( m_angleThick*cm );
      paramVec.push_back( m_angleSize*cm );
      std::ostringstream DetName;
      DetName << "BHPV_angleCrowBottom_" << iMod;
      double y = -m_OBoxY/2.
	- m_bottomplateThick
	- m_angleThick/2.;
      double z = -z_mod/2.
	+ m_angleSize/2.;
      GsimBox* box = new GsimBox( DetName.str(), boxMod,
				  G4ThreeVector(0,y*cm,z*cm),
				  G4ThreeVector(0,0,0) );
      box->setParameters( paramVec );
      box->setOuterMaterial( "G4_Al" );
      box->setOuterColor( "red" );
      box->setOuterCopyNo( -1 );
      boxMod->addDaughter( box );
      m_detList.push_back( box );

      z *= (-1);//left side of lower (x<0,y<0) 
      box->cloneDetector( G4ThreeVector(0,y*cm,z*cm),
			  G4ThreeVector(0,0,0), 0 );
    }

    setThisAndDaughterBriefName( "BHPV" );
    if( m_opticalMode ) {
      std::list<GsimSensitiveDetector*> sdList=getThisAndDaughterSensitiveDetectorList();  
      for(std::list<GsimSensitiveDetector*>::iterator it=sdList.begin();
	  it!=sdList.end();it++) {
	std::string sdName=(*it)->getName();
	if(sdName == "BHPV") {
	  (*it)->setTdcThreshold(0.5);
	  (*it)->setSensitiveMode(opticalPhotonSensitiveFastMode);
	  (*it)->setHitProcessDetector(this);
	}
      }
    }
  }
}

GsimE14BHPV::~GsimE14BHPV()
{

  delete[] m_nPE;
  delete[] m_nPE_airCerenkov;
  delete[] m_HitTime;
  
  delete [] m_PbThick;
  delete [] m_PbAGGap;
  delete [] m_AGThick;
  delete [] m_ModGap;
  delete [] m_AGParamAbsorption;
  delete [] m_AGParamScattering;
  delete [] m_OverallFactor;
      
  delete m_winstonCone;
}

void GsimE14BHPV::setFastSimulationLevel(int level)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //reset
  if(m_fastSimulationLevel==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->unsetSensitiveDetector();
    }
  }

  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("BHPV",0,true);
    }
  }

  
  {
    m_fastSimulationLevel=level;
    for(GsimDetectorContainer::iterator it=m_daughterDetectorContainer.begin();
	it!=m_daughterDetectorContainer.end();it++) {
      GsimDetector* daughter =  (*it).second;
      daughter->setFastSimulationLevel(level);
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif 
}

double GsimE14BHPV::getBHPVLength() {

  double z_mod =//length of one module
    (m_angleThick + m_plateThick)*2//upstream and downstream
    + m_OBoxZ;//33.2cm
  
  double zLengthTotal = m_nMod*z_mod;
  for( int iMod=0 ; iMod<(m_nMod-1) ; iMod++ ) zLengthTotal += m_ModGap[iMod];
  
  return zLengthTotal;
}



bool GsimE14BHPV::processHits(const G4Step* aStep,G4TouchableHistory* ROhist, std::list<int>& channelList)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  if(!m_opticalMode) return true;
  
  G4Track*              track    = aStep->GetTrack();
  G4ParticleDefinition* particle = track->GetDefinition();

  

  //Treat only optical photon
  if(particle!=G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  bool IsAirCerenkov = false;
  if( std::string(track->GetMaterial()->GetName())
      ==std::string("GsimOpticalAir") )
    IsAirCerenkov = true;

  bool isHit=traceFromInsideAerogel(aStep,ROhist,channelList,IsAirCerenkov);
  return isHit;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


bool GsimE14BHPV::traceFromInsideAerogel(const G4Step* aStep,G4TouchableHistory* ,
					 std::list<int>& channelList,
					 bool IsAirCerenkov=false)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //Set undetected as default
  const_cast<G4Step*>(aStep)->SetTotalEnergyDeposit(0);
  m_pathLengthAir=0.;

  int ModID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
  
  double hbarc=197.327053;//ev nm
  double energy=aStep->GetPreStepPoint()->GetKineticEnergy()/eV;//eV
  double waveL=2.*M_PI*hbarc/energy;//nm

  //Local pos and mom in Aerogel
  double pPos[3]={0,0,0};
  double pVec[3]={0,0,0};
  translateFromGlobalToAG(aStep,pPos,pVec);

  //normalization
  double mag=std::sqrt(
		       std::pow(pVec[0],2.)+
		       std::pow(pVec[1],2.)+
		       std::pow(pVec[2],2.));
  if(mag==0) return false;//error
  
  for(int k=0;k<3;k++) {
    pVec[k]/=mag;
  }
  bool isTraced = false;
  
  //RayTrace within Aerogel with AG coordinate
  if( !IsAirCerenkov ){
    isTraced = traceAerogel(waveL,pPos, pVec,ModID);
    if(!isTraced) return false;
  }

  //RayTrace in Optical Box with its coordinate
  isTraced = traceOpticalBox_2(waveL,pPos,pVec,ModID,IsAirCerenkov);
  if(!isTraced) return false;

  if( std::sqrt(std::pow(pPos[1],2.)+std::pow(pPos[2],2.))>m_dInWCone/2. ) return false;


  //gap between OBox and WCone
  if( pVec[0]!=0 ){
    double dGapPath = m_gapOBoxWCone/pVec[0];
    for( int i(0);i<3;i++ ) pPos[i] += pVec[i]*dGapPath;
    m_pathLengthAir += dGapPath;
  }
  else return false;

  if( std::sqrt(std::pow(pPos[1],2.)+std::pow(pPos[2],2.))>m_dInWCone/2. ) return false;

  int hitWCone = chooseWCone(pPos, pVec);
  //left(x>0) : 0, right(x<0) : 1

  //RayTrace in WinstonCone
  // trans to WCone coordinate from OBOX coordinate
  translateFromOBoxToWCone(pPos,pVec);
  // RayTrace within WinstonCone with WinstonCone coordinate
  isTraced=traceWCone(waveL, pPos, pVec);
  if(!isTraced) return false;

  //Hit decision at PMT
  double dGapPath = -1.*m_gapWConePMT/pVec[2];//PMT phtocathod is 1cm far from PMT face
  for(int k=0;k<3;k++) {
    pPos[k]=pPos[k]+pVec[k]*dGapPath;
  }

  m_pathLengthAir +=
    (m_pathLengthOBox+
     m_pathLengthWCone+dGapPath);
  
  //double angleinPMT=std::atan(std::sqrt(std::pow(pVec[0],2.)+std::pow(pVec[2],2.))/pVec[1]);
  //     === on Detection Plane at Y=yPMT
  //     --- circular gate for PMT
  if (std::sqrt(std::pow(pPos[0],2.)+std::pow(pPos[2],2.))>m_rPMT) {
    isTraced=false;
  }
  if(!isTraced) return false;

  {
    //hit inside PMT cathode
    //Q.E. check
    double rnd=CLHEP::RandFlat::shoot();
    if( rnd > getPMTQE(waveL)*m_OverallFactor[ModID] ) {
      isTraced=false;
    }
  }
  if(!isTraced) return false;


  {
    //Detected(PhotoElectron is generated)
    const_cast<G4Step*>(aStep)->SetTotalEnergyDeposit(1.*MeV);
    int ChID = 2*ModID+hitWCone;

    channelList.push_back(ChID);
    
    if( IsAirCerenkov ) m_nPE_airCerenkov[ChID]++;
    int PENo = m_nPE[ChID]++;
    
    //timing calculation with TOF
    double cLight=29.9792458;// [cm/nsec]
    double dt=
      m_pathLengthAG/(cLight/m_refractiveIndexAG)
      + m_pathLengthAir/cLight;
    
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    double PhotonStartTime = preStepPoint->GetGlobalTime()/ns;
    double PhotonHitTime = (PhotonStartTime+dt)*ns;
    preStepPoint->SetGlobalTime( PhotonHitTime );

    if(PENo<m_nMaxPE) {
      m_HitTime[ChID*m_nMaxPE+PENo] = PhotonHitTime;
    } else {
      //Number of photoelectrons / channel exceed the limit.
      std::ostringstream ostr;
      ostr << "BHPV number of photoelectrons : " << PENo
	   << " / channel : " <<  ChID
	   << " exceeds the limit : " << m_nMaxPE << std::endl;
      GsimMessage::getInstance()
	->report("warning",ostr.str());
    }
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif

  return true;
}

double GsimE14BHPV::dNextAbsorption(double waveL,int ModID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  //waveL[nm]
  //  according to NIM A440 (2000) 338  //
  //double A=0.96;
  //double A=0.972;
  double A=m_AGParamAbsorption[ModID];//make changable module by module (16th Apr, 2013)
  //  cutoff wavelength (by hand)  //
  if (waveL<300.) {
    return 0;
  }
  
  double lambda=-1./log(A);
  double rnd=CLHEP::RandFlat::shoot();
  double d=-log(rnd)*lambda;

  return d;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

double GsimE14BHPV::dNextScattering(double waveL,int ModID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double CT=m_AGParamScattering[ModID];//[um^4/cm], make changable module by module (16th Apr, 2013)
  double lambda=1./(CT/std::pow(waveL*1e-3,4.));
  double rnd=CLHEP::RandFlat::shoot();
  double d=-log(rnd)*lambda;
  return d;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimE14BHPV::scatterInAerogel(double* pVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  double cth=0.;
  while(1) {
    double rnd0=CLHEP::RandFlat::shoot();
    double rnd1=CLHEP::RandFlat::shoot();
    cth=1.-2.*rnd0;
    if(rnd1<=(1.+std::pow(cth,2.))/2.) break;
  }
  double sth=sqrt(1.-std::pow(cth,2.));
  double rnd=CLHEP::RandFlat::shoot();
  double cphi=cos(2.*M_PI*rnd);
  double sphi=sin(2.*M_PI*rnd);

  
  
  G4ThreeVector vec(pVec[0],pVec[1],pVec[2]);
  double COSTH=vec.cosTheta();
  double SINTH=std::sqrt(1.-std::pow(COSTH,2.));
  double fac=1./std::sqrt(std::pow(pVec[0],2.)+std::pow(pVec[1],2.));
  double COSPH=fac*pVec[0];
  double SINPH=fac*pVec[1];

  double P1=sth*cphi;
  double P2=sth*sphi;
  double P3=cth;
  
  pVec[0]=P1*COSTH*COSPH - P2*SINPH + P3*SINTH*COSPH;
  pVec[1]=P1*COSTH*SINPH + P2*COSPH + P3*SINTH*SINPH;
  pVec[2]=P1*SINTH                 + P3*COSTH;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
  return;
}


bool GsimE14BHPV::traceAerogel(double waveL,double* pPos, double* pVec,int ModID)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  bool isReach=false;
  bool isRefU=false;
  bool isScat=false;
  
  double dNextAbs=dNextAbsorption(waveL,ModID);
  double dNextSc =dNextScattering(waveL,ModID);
  double pPosS[3]={0,0,0};
  double dAGTile[3]={m_AGX/2.,m_AGY/2.,m_AGThick[ModID]/2.};
  
  for(int k=0;k<3;k++) {
    m_nReflectionAGWall[k]=0;
  }
  m_pathLengthAG=0.;

  //bool ScatteredAtTileJoint = false;
  while(!isReach) {
    for(int k=0;k<2;k++) {

      pPosS[k]=pPos[k]+dNextSc*pVec[k];
      if (pPos[k]*pPosS[k]<0.) {
	double frac = std::abs(pPos[k]) / std::abs(pPosS[k]-pPos[k]);
	dNextSc = frac * dNextSc;
	//ScatteredAtTileJoint = true;
      }
    }
    ////////////////////////////////////////////////

    ////////////////////////////////////////////////
    //1 Step
    double dStep=0;
    double zNext=pPos[2]+std::min(dNextAbs,dNextSc)*pVec[2];
    if (zNext > dAGTile[2]) {
      //--- reach to downstream end
      isReach=true;
      dStep=( dAGTile[2]-pPos[2])/pVec[2];
    } else if(zNext<-dAGTile[2]) {
      //--- go back to upstream end
      isRefU=true;
      m_nReflectionAGWall[2]+=1;
      if (m_nReflectionAGWall[2]>m_nMaxReflectionAGWall[2]) {
	return false;
      }
      dStep=(-dAGTile[2]-pPos[2])/pVec[2];
      dNextAbs=dNextAbs-dStep;
      dNextSc =dNextSc -dStep;
    } else {
      //--- absorbed or scatttered at somewhere in tile
      if (dNextAbs<dNextSc) {
	return false;	//discard if absorbed
      }
      isScat=true;
      dStep=dNextSc;
      dNextAbs=dNextAbs-dStep;
      dNextSc =dNextScattering(waveL,ModID);
    }

    //--- proceed to next point
    pPos[2]=dStep*pVec[2]+pPos[2];
    for(int k=0;k<2;k++) {
      pPos[k]=dStep*pVec[k]+pPos[k];
      
      if (std::abs(pPos[k])>dAGTile[k]) {
	int nRthis=int((std::abs(pPos[k])+dAGTile[k])/(dAGTile[k]*2.));
	m_nReflectionAGWall[k]+=nRthis;
	if (m_nReflectionAGWall[k]>m_nMaxReflectionAGWall[k]) return false;
	if (pPos[k]<0.) {
	  pPos[k]=pPos[k]+dAGTile[k]*2.*nRthis;
	} else {
	  pPos[k]=pPos[k]-dAGTile[k]*2.*nRthis;
	}
	pPos[k]=pPos[k]*std::pow((-1.),nRthis);
	pVec[k]=pVec[k]*std::pow((-1.),nRthis);
      }
    }

    if (isRefU) pVec[2]=-pVec[2];
    if (isScat) scatterInAerogel(pVec);
    m_pathLengthAG+=dStep;
  }

  //Dead in reflection in AG wall
  for(int k=0;k<3;k++) {
    double rnd=CLHEP::RandFlat::shoot();
    if (rnd>std::pow(m_reflectanceAGWall[k],m_nReflectionAGWall[k])) 
      return false;
  }


  //Dead in wrap transmittance
  {
    double rnd=CLHEP::RandFlat::shoot();
    if ( rnd>m_transmittanceWrap ) {
      return false;
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return true;
}


bool GsimE14BHPV::traceOpticalBox_2( double , double* pPos, double* pVec,
				     int ModID, bool IsAirCerenkov = false )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  //2-segmented model******************************************************
  m_pathLengthOBox=0.;
  m_nReflectionOBoxWall=0;
  m_nReflectionMirror=0;

  double dSafe = 0.0001;//1um
  double lenAGBox =
    m_PbAGGap[ModID]
    + m_AGBoxBottomPlateThick
    + m_AGThick[ModID];
  double OBoxHalfSize[3] =
    { m_OBoxX/2.,
      m_OBoxY/2.-1.2-m_mirrorThick,
      (m_mirrorCrossZ-lenAGBox+m_OBoxX/2.-m_mirrorThick*std::sqrt(2.))/2.
    };
  double lenBehindMirror = m_OBoxZ - lenAGBox - 2*OBoxHalfSize[2];
  
  double z_mirrorEdge =//z-position of the edge of mirror (in OBox cordinate)
    m_mirrorCrossZ-lenAGBox-OBoxHalfSize[2]-m_mirrorThick*std::sqrt(2.);
  double invalid = dSafe/2.;//when denominator is zero, this value is set
  double nVecmir[2][3] =//normal vector of mirror plain
      //for left(x>0) mirror
    { { std::cos(m_mirrorAngle*M_PI/180.),
	0,
	-std::sin(m_mirrorAngle*M_PI/180.) },
      //for right(x<0) mirror
      { -std::cos(m_mirrorAngle*M_PI/180.),
	0,
	-std::sin(m_mirrorAngle*M_PI/180.) } };

  if( IsAirCerenkov ){
    pPos[2] += lenBehindMirror/2.;
    if( ( !(std::fabs(pPos[0])>pPos[2]-OBoxHalfSize[2]+OBoxHalfSize[0])
	  || pPos[2]>OBoxHalfSize[2] ) )
      return false;//invalid starting point (behind flat mirrors) 
  }
  else
    pPos[2] = -OBoxHalfSize[2];//move to optical box coordinate

  int ctr = 0;
  bool isReach = false;

  while( !isReach ){
    
    ctr++;
    if( ctr>10 ){
      return false;//too many bounce
    }
    
    double lenStep[5] = { 0., 0., 0., 0., 0. };//length to where ray and mirror plain or OBox wall cross
    // 0:left(x>0) mirror
    // 1:right(x<0) mirror
    // 2:exits of OBox (x-direction)
    // 3:top or bottom wall of OBox (wall for y-direction)
    // 4:upstream(aerogel)
    double ndotv[2] = { 0., 0. };//inner product of ray vector and normal vector of mirror plain
    // 0:left(x>0) mirror 1:right(x<0) mirror
    double tempVertex[5][3] = { { 0., 0., 0. },
				  { 0., 0., 0. },
				  { 0., 0., 0. },
				  { 0., 0., 0. },
				  { 0., 0., 0. } };
    int realStepNumber = -1;//moved from outside of while-loop ********************
    
    //caluculate length in each case----------
    //lenStep[0],[1] : when reach the mirror surface (i=0:left(x>0) i=1:right(x<0))
    for( int i(0);i<2;i++ ){
      
      for( int j(0);j<3;j++ ) ndotv[i] += (pVec[j]*nVecmir[i][j]);
      if( ndotv[i]!=0 ) {//remove the case when parallel with mirror plane
	int sign = ((i==0) ? 1 : -1);
	lenStep[i] = (sign*pPos[0]-pPos[2]+z_mirrorEdge)/(pVec[2]-sign*pVec[0]);
	//eqation of mirror : Z = (+/-) X + z_mirrorEdge
	/*	
	lenStep[i] = ( (pPos[2]-OBoxHalfSize[2])*std::sin(m_mirrorAngle*M_PI/180.)
		       - sign*(pPos[0]-sign*OBoxHalfSize[0])*std::cos(m_mirrorAngle*M_PI/180.) ) / ndotv[i];
	*/
	for( int j(0);j<3;j++ ) tempVertex[i][j] = pPos[j]+lenStep[i]*pVec[j];
	//check incorrect vertex (w/ virtual region)
	if( (tempVertex[i][0]*sign<0. || tempVertex[i][0]*sign>OBoxHalfSize[0]) //for x-direction
	    || ( std::fabs(tempVertex[i][1])>OBoxHalfSize[1] ) //for y-direction
	    || ( tempVertex[i][2]<z_mirrorEdge || tempVertex[i][2]>OBoxHalfSize[2] ) ) //for z-direction
	  lenStep[i] = invalid;
      }
      else lenStep[i] = invalid;//do not move
    }

    //reach the wall or the exit of OBox
    //(i=0,1,2 for x(exit), y(top and bottom), z(back to aerogel) respectively)
    for(int i(0);i<3;i++){
      if( pVec[i]!=0 ){
	int sign = ((pVec[i]>0.) ? 1 : -1);
	lenStep[2+i] = (sign*OBoxHalfSize[i]-pPos[i])/pVec[i];
	for( int j(0);j<3;j++ ) tempVertex[2+i][j] = pPos[j]+lenStep[2+i]*pVec[j];
	for( int j(0);j<3;j++ ){
	  //vertex is outside of OBox 
	  if( std::fabs(tempVertex[2+i][j]) > OBoxHalfSize[j]+((i==j)? dSafe : 0) ){
	    if( i!=0 || j!=2 || (tempVertex[2][2]>OBoxHalfSize[2]) ){
	      //exclude the special case where the photon can go to the side of AG box (will be processed later)
	      lenStep[2+i] = invalid;
	      break;
	    }
	  }

	  if( i==1 ){//for y-direction (vertex in mirror region)
	    if( tempVertex[3][2]>std::fabs(tempVertex[3][0])+z_mirrorEdge )
	      lenStep[3] = invalid;
	  }
	  if( i==2 ){//for z-direction (exclude the case of flying forward)
	    if( pVec[2]>0. ) lenStep[4] = invalid;
	  }
	}
      }
      else lenStep[2+i] = invalid;
    }
    //the special case where the photon can go to the side of AG box
    if( tempVertex[2][2]<-OBoxHalfSize[2] ){
      if( tempVertex[2][2]<-OBoxHalfSize[2]-lenAGBox || std::fabs(tempVertex[4][0])<m_AGBoxX/2. )
	lenStep[2] = invalid;
      else{
	//optical photon can go to the side of AG box
	lenStep[4] = invalid;
      }
    }
    
    //decide which course the ray will take----------
    for( int i(0);i<5;i++ ){
      if( (lenStep[i]>dSafe)//select proper length
	  && ( (realStepNumber<0) || (lenStep[i]<lenStep[realStepNumber]) ) )
	realStepNumber=i;
    }
    if( realStepNumber<0 ){
      return false;//never happen
    }
    m_pathLengthOBox += lenStep[realStepNumber];

    //proceed next step----------
    for( int i(0);i<3;i++ ) pPos[i] = tempVertex[realStepNumber][i];

    if( realStepNumber<=1 ){//reflected by mirror
      for( int i(0);i<3;i++ ){
	pVec[i] += 2.*std::fabs(ndotv[realStepNumber])*nVecmir[realStepNumber][i];
      }
      m_nReflectionMirror++;
      if( CLHEP::RandFlat::shoot()>m_reflectanceMirror ){
	return false;
      }
      //joint of mirrors, edge... 
      if( //std::fabs(pPos[0])<m_mirrorThick/std::sqrt(2) || 
	 std::fabs(pPos[0])<m_mirrorThick*std::sqrt(2)*2 || 
	 std::fabs(pPos[0])>(m_mirrorX+m_mirrorThick)/std::sqrt(2) ||
	 std::fabs(pPos[1])>m_mirrorY/2. ){
	return false;
      }
    }else if( realStepNumber==2 ){//reach the exits (x-direction)
      isReach = true;
    }else if( realStepNumber==3 ){//reflected by OBox top or bottom wall(y-direction)
      pVec[1] *= (-1);
      m_nReflectionOBoxWall++;
      if( CLHEP::RandFlat::shoot()>m_reflectanceWall ){
	return false;
      }
    }
    else if( realStepNumber==4 ){//back upstream(z-direction)
      return false;
    }
  }

  //pPos[2] += m_OBoxZ/2 - OBoxHalfSize[2];
  pPos[2] += (lenAGBox + OBoxHalfSize[2] - m_OBoxZ/2);

  return true;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


bool GsimE14BHPV::traceWCone(double ,double* pPos, double* pVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_pathLengthWCone=0;
  m_nReflectionWConeWall=0;
  bool isTraced=m_winstonCone->trace(pPos,pVec);
  m_nReflectionWConeWall=
    m_winstonCone->getNReflections();
  m_pathLengthWCone=
    m_winstonCone->getPathLength();

  //Dead in reflection at WinstonCone surface.
  if(isTraced) {
    double rnd=CLHEP::RandFlat::shoot();
    if ( rnd>std::pow(m_reflectanceWCone,m_nReflectionWConeWall) ) {
      isTraced=false;
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return isTraced;
}


double GsimE14BHPV::getPMTQE(double waveL)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //  Blue sensitivity ratio of real PMTs to typical value (9.0)  //
  //       Average of 6 PMTs delivered in 2001/2002 = 9.7  -> R=1.08
  //double R2TYP=1.08;
  double R2TYP=1.;

  //  Typical spectral response from Hamamatsu's spec sheet
  int np=11;
  double wlx[11]=
    {
      280.0,
      300.0,
      320.0,
      340.0,
      360.0,
      380.0,
      400.0,
      450.0,
      500.0,
      550.0,
      600.0
    };
  
  double QEwlx[11]=
    {
      0.00,
      0.050,
      0.13,
      0.18,
      0.21,
      0.22,
      0.22,
      0.17,
      0.11,
      0.060,
      0.022
    };

  double QEwl=0.0;
  if (waveL<wlx[0] || waveL>wlx[np-1]) return QEwl;
  
  for(int k=0;k<np-1;k++) {
    if (waveL<wlx[k+1]) {
      QEwl = QEwlx[k]+
	(QEwlx[k+1]-QEwlx[k])/(wlx[k+1]-wlx[k])* (waveL-wlx[k]);
      QEwl /= R2TYP;
      return QEwl;
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return QEwl;
}

void GsimE14BHPV::translateFromGlobalToAG(const G4Step* aStep,double* pPos, double* pVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4ThreeVector globalPos=aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector globalMom=aStep->GetPreStepPoint()->GetMomentum();
  const G4VTouchable*   touch= aStep->GetPreStepPoint()->GetTouchable();
  
  G4ThreeVector agLocalPos
    =touch->GetHistory()->GetTopTransform().TransformPoint(globalPos);
  G4ThreeVector agLocalMom
    =touch->GetHistory()->GetTopTransform().TransformAxis(globalMom);
  
  pPos[0]=agLocalPos.x()/cm;
  pPos[1]=agLocalPos.y()/cm;
  pPos[2]=agLocalPos.z()/cm;
  pVec[0]=agLocalMom.x();
  pVec[1]=agLocalMom.y();
  pVec[2]=agLocalMom.z();
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14BHPV::translateFromOBoxToWCone(double* pPos, double* pVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  if( pVec[0]<0. ){
    pPos[0] *= (-1);
    pVec[0] *= (-1);
  }

  // trans to WCone coordinate from OBOX coordinate
  double new_x = pPos[2];
  double new_y = pPos[1];
  double new_z = m_heightWCone;//-pPos[0]+m_winstonCone->getHight()+(m_AGXYLength+2*m_AGBoxThick)/2.+OBOX_EXTEND*std::tan(m_mirrorAngle*M_PI/180.);
  double new_vx = pVec[2];
  double new_vy = pVec[1];
  double new_vz = -pVec[0];

  pPos[0] = new_x;
  pPos[1] = new_y;
  pPos[2] = new_z;
  pVec[0] = new_vx;
  pVec[1] = new_vy;
  pVec[2] = new_vz;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

int GsimE14BHPV::chooseWCone( double* , double* pVec )
{
  if( pVec[0]>0. ) return 0;//reach left(x>0) exit
  else return 1;//reach right(x<0) exit    
}

void GsimE14BHPV::beginOfRunAction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  GsimPersistencyManager* pm = GsimPersistencyManager::getPersistencyManager();
  
  std::ostringstream mkBranch;
  mkBranch << "BHPV_nPE[" << (2*m_nMod) << "]/I";
  pm->setBranchOf( "eventTree", "nPE", m_nPE, mkBranch.str().c_str() );
  std::ostringstream mkBranch_air;
  mkBranch_air << "BHPV_nPE_air[" << (2*m_nMod) << "]/I";
  pm->setBranchOf( "eventTree", "airCerenkov", m_nPE_airCerenkov, mkBranch_air.str().c_str() );
  std::ostringstream mkBranch_time;
  mkBranch_time << "BHPV_time[" << (2*m_nMod) << "][" << m_nMaxPE << "]/D";
  pm->setBranchOf( "eventTree", "HitTime", m_HitTime, mkBranch_time.str().c_str() );
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14BHPV::endOfRunAction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
  
bool GsimE14BHPV::beginOfEventAction(const G4Event* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  int nCh = m_nMod*2;
  for( int iCh=0;iCh<nCh;iCh++ ){
    m_nPE[iCh] = 0;
    m_nPE_airCerenkov[iCh] = 0;
    for( int iPE=0;iPE<m_nMaxPE;iPE++ ) m_HitTime[iCh*m_nMaxPE+iPE] = 0; 
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  
  return true;
}

bool GsimE14BHPV::endOfEventAction(const G4Event* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return true;
}

void GsimE14BHPV::comment()
{
  std::cout << "GsimE14BHPV Z position:" << std::endl;
  std::cout << "    Center of total z length of BHPV." << std::endl;
  std::cout << "GsimE14BHPV userFlag:" << std::endl;
  std::cout << "  2011: 25 layers, non-optical." << std::endl;
  std::cout << "     0: 25 layers, non-optical." << std::endl;
  std::cout << "    25: 25 layers, optical." << std::endl;
  std::cout << "  2502: 25 layers, optical." << std::endl;
  std::cout << "     1: single layer, optical." << std::endl;
  std::cout << "201303: 12 layer, optical. " << std::endl;
  std::cout << "201304: 12 layer, optical. " << std::endl;
  std::cout << "201305: 12 layer, optical. " << std::endl;
  std::cout << "201411: 16 layer, optical. " << std::endl;
  std::cout << "201504: 16 layer, optical with odd lead-thickness order." << std::endl;
  std::cout << "201605: 16 layer, optical with good lead-thickness order." << std::endl;
  std::cout << "      : Place it downstream by 2 cm." << std::endl;
  std::cout << "      : z=(614.8+479.2+100.+52)*cm+535.6/2.*cm+2*cm" << std::endl;
  std::cout << "201902: 16 layer, optical with good lead-thickness order." << std::endl;
  std::cout << "      : Place it downstream by 2 cm." << std::endl;
  std::cout << "      : z=(614.8+479.2+100.+52)*cm+535.6/2.*cm+2*cm" << std::endl;
  std::cout << "202002: the same as 201902" << std::endl;
  std::cout << "202102: the same as 201902" << std::endl;
}

void GsimE14BHPV::setUserParameters(std::string tag,std::vector<double> vec)
{
  if(vec.size()!=0&&vec.size()!=static_cast<size_t>(m_nMod)) {
    std::cout << "Number of parameters is not euqal to nMod, " << m_nMod << std::endl;
    return;
  }


  
  double* par=0;
  if(tag == "Absorption") {
    par=m_AGParamAbsorption;
  } else if(tag == "Scattering") {
    par=m_AGParamScattering;
  } else if(tag == "OverallFactor") {
    par=m_OverallFactor;
  } else {
    std::cout << "Tag, " << tag << ", does not match." << std::endl;
    std::cout << "Candidates are " << std::endl;
    std::cout << "Absorption : A for A*exp(-Ct/lambda^4)"  << std::endl;
    std::cout << "Scattering : C (um^4/cm) for A*exp(-Ct/lambda^4)" << std::endl;
    std::cout << "OverallFactor" << std::endl;
    return;
  }

  if(vec.size()==0) {
    std::cout << "Print for " << tag << std::endl;
    for(int i=0;i<m_nMod;i++) {
      std::cout << i << "\t" << par[i] << std::endl;
    }
  } else {
    for(int i=0;i<m_nMod;i++) {
      par[i] = vec[i];
    }
  }
}
