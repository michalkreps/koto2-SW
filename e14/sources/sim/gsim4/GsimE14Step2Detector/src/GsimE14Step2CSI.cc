/**
 *  @file
 *  @brief  GsimE14Step2CSI
 *  $Id:  GsimE14Step2CSI.cc,v  2012/05/14 14:36:58 sato Exp
 *  $Log: GsimE14Step2CSI.cc,v $
 */
#include "GsimE14Step2Detector/GsimE14Step2CSI.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimBox.h"
#include "GsimData/GsimEventData.h"
#include "GsimE14Detector/GsimE14UserGeom.h"

#include "G4SystemOfUnits.hh"

#include <cmath>
#include <list>

using namespace E14; 


GsimE14Step2CSI::GsimE14Step2CSI(std::string name,
				 GsimDetector* motherDetector,
				 G4ThreeVector transV,G4ThreeVector rotV,
				 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14Step2CSI";

  double L_XY=5;//cm
  double L_Len=50;//cm
  const int nLayer=60;
  std::vector<double> paramL;
  paramL.push_back(L_XY*cm);
  paramL.push_back(L_XY*cm);
  paramL.push_back(L_Len*cm);

  int nBeamHole=4;
  GsimBox* boxL=0;
  int csiStack[nLayer]
    = {
    0,
    14,
    20,
    26,
    28,
    32,
    36,
    38,
    40,
    42,
    44,
    46,
    48,
    48,
    50,
    50,
    52,
    54,
    54,
    54,
    56,
    56,
    56,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    58,
    56,
    56,
    56,
    54,
    54,
    54,
    52,
    50,
    50,
    48,
    48,
    46,
    44,
    42,
    40,
    38,
    36,
    32,
    28,
    26,
    20,
    14,
    0
  };


  int id=0;
  for(int i=0;i<nLayer;i++) {
    double YBlock=
      L_XY*(i-nLayer/2.+0.5);
    int nBlock = csiStack[i];
    for(int j=0;j<nBlock;j++) {
      double XBlock=
	L_XY*(j-nBlock/2.+0.5);
      // added by Sato 20120514
      //beam hole
      if( i>=(nLayer-nBeamHole)/2. &&
	  i<(nLayer-nBeamHole)/2.+nBeamHole &&
	  j>=(nBlock-nBeamHole)/2. &&
	  j<(nBlock-nBeamHole)/2.+nBeamHole )
	continue;
      
      
      
      XBlock*=-1;

      double ZBlock=L_Len/2.;


      char nam[256];
      if(boxL==0) {
	std::sprintf(nam,"boxL");
	boxL = new GsimBox(std::string(nam),this,
			   G4ThreeVector(XBlock*cm,
					 YBlock*cm,
					 ZBlock*cm),
			   G4ThreeVector(0,0,0)
			   );
	boxL->setOuterMaterial("G4_CESIUM_IODIDE");
	boxL->setParameters(paramL);
	boxL->setOuterColor("lightskyblue");
	boxL->setSensitiveDetector("CSI",id);
	addDaughter(boxL);
      } else {
	boxL->cloneDetector(G4ThreeVector(XBlock*cm,
					  YBlock*cm,
					  ZBlock*cm),
			    G4ThreeVector(0,0,0),id);
      }
      id++;
    }
  }

  
  setThisAndDaughterBriefName("CSI");
}

GsimE14Step2CSI::~GsimE14Step2CSI()
{
  ;
}





void GsimE14Step2CSI::comment()
{
  ;
}

