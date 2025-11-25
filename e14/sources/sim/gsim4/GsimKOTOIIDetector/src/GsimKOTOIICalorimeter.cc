/**
 *  @file
 *  @brief  GsimKOTOIICalorimeter
 *  $Id:  
 *  $Log: 
 *
 */
#include "GsimKOTOIIDetector/GsimKOTOIICalorimeter.h"
#include "GsimKOTOIIDetector/GsimKOTOIIUserGeom.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimKernel/GsimTrackInformation.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTube.h"
#include "GsimData/GsimEventData.h"

#include "G4Track.hh"
#include "G4Color.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <list>

using namespace KOTOII;



GsimKOTOIICalorimeter::GsimKOTOIICalorimeter(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimKOTOIICalorimeter";
  char nam[100];

  m_userFlag=userFlag;
  //0 : defaut Pb;0.275mm Sci : 1.5mm,300 Layer
  //1 : Pb:0.6mm,Sci:1.4mm 250 Layer
   
  
  GsimBox* boxKTEVFine=0;
  int nBlockFine=48;
  int nBeamHole=8+2;
  nBeamHole=8;

  std::vector<double> paramKTEVFine;
  paramKTEVFine.push_back(KtevCsicFine_XY*cm);
  paramKTEVFine.push_back(KtevCsicFine_XY*cm);
  paramKTEVFine.push_back(KtevCsicFine_Len*cm);
  
  int id=0;
  for(int i=0;i<nBlockFine;i++) {
    double YBlock=
      (KtevCsicFine_XY+KtevCsicFine_Gap)*(i-nBlockFine/2.)
      +(KtevCsicFine_XY+KtevCsicFine_Gap)/2.;
    
    for(int j=0;j<nBlockFine;j++) {
      {
	double XBlock=
	  (KtevCsicFine_XY+KtevCsicFine_Gap)*(j-nBlockFine/2.)
	  +(KtevCsicFine_XY+KtevCsicFine_Gap)/2.;
	
	//beam hole
	if( i>=(nBlockFine-nBeamHole)/2. &&
	    i<(nBlockFine-nBeamHole)/2.+nBeamHole &&
	    j>=(nBlockFine-nBeamHole)/2. &&
	    j<(nBlockFine-nBeamHole)/2.+nBeamHole )
	  continue;
	
	// added by Sato 20120514
	XBlock*=-1;

	double ZBlock=(KtevCsicFine_Len/2.);
	if(boxKTEVFine==0) {
	  std::sprintf(nam,"KTEVCSIFine");
	  boxKTEVFine = new GsimBox(std::string(nam),this,
				    G4ThreeVector(XBlock*cm,
						  YBlock*cm,
						  ZBlock*cm),
				    G4ThreeVector(0,0,0)
				    );
	  boxKTEVFine->setOuterMaterial("G4_CESIUM_IODIDE");
	  boxKTEVFine->setParameters(paramKTEVFine);
	  boxKTEVFine->setOuterColor("lightskyblue");
	  boxKTEVFine->setSensitiveDetector("CAL",id);
	  addDaughter(boxKTEVFine);
	} else {
	  boxKTEVFine->cloneDetector(G4ThreeVector(XBlock*cm,
						   YBlock*cm,
						   ZBlock*cm),
				     G4ThreeVector(0,0,0),id);
	}
	id++;
      }
    }
  }


  std::vector<double> paramKTEV;
  paramKTEV.push_back(KtevCsic_XY*cm);
  paramKTEV.push_back(KtevCsic_XY*cm);
  paramKTEV.push_back(KtevCsic_Len*cm);


  GsimBox* boxKTEV=0;
  int csiStack[36];
  for(int i=0;i<36;i++) {
    csiStack[i]=36;
  }
  
  int nLayer=36;
  for(int i=0;i<nLayer;i++) {
    double YBlock=
      (KtevCsic_XY+KtevCsic_Gap)*(i-nLayer/2.)
      +(KtevCsic_XY+KtevCsic_Gap)/2.;
    
    int nBlock = csiStack[i];
    
    for(int j=0;j<nBlock;j++) {
      //KtevFine
      if( i>=(nLayer-nBlockFine/2.)/2. &&
	  i<(nLayer-nBlockFine/2.)/2.+nBlockFine/2. &&
	  j>=(nBlock-nBlockFine/2.)/2. &&
	  j<(nBlock-nBlockFine/2.)/2.+nBlockFine/2. )
	continue;
      
      double XBlock=
	(KtevCsic_XY+KtevCsic_Gap)*(j-nBlock/2.)
	+(KtevCsic_XY+KtevCsic_Gap)/2.;
      // added by Sato 20120514
      XBlock*=-1;

      double ZBlock=(KtevCsicFine_Len/2.);
      if(boxKTEV==0) {
	std::sprintf(nam,"KTEVCSI");
	boxKTEV = new GsimBox(std::string(nam),this,
			      G4ThreeVector(XBlock*cm,
					    YBlock*cm,
					    ZBlock*cm),
			      G4ThreeVector(0,0,0)
			      );
	boxKTEV->setOuterMaterial("G4_CESIUM_IODIDE");
	boxKTEV->setParameters(paramKTEV);
	boxKTEV->setOuterColor("lightskyblue");
	boxKTEV->setSensitiveDetector("CAL",id);
	addDaughter(boxKTEV);
      } else {
	boxKTEV->cloneDetector(G4ThreeVector(XBlock*cm,
					     YBlock*cm,
					     ZBlock*cm),
			       G4ThreeVector(0,0,0),id);
      }
      id++;
    }
  }

  double L_XY=5;//cm
  double L_Gap=0.01;
  double L_Len=55.5;//cm
  std::vector<double> paramL;
  paramL.push_back(L_XY*cm);
  paramL.push_back(L_XY*cm);
  paramL.push_back(L_Len*cm);
  const int nLSLayer=62;
  int lsStack[nLSLayer]
    = {
    0,
    0,
    0,//14,
    32,//20,
    32,//26,
    32,//28,
    32,
    40,//36,
    40,//38,
    40,
    40,//42,
    48,//44,
    48,//46,
    48,
    48,
    56,//50,
    56,//50,
    56,//52,
    56,//54,
    56,//54,
    56,//54,
    56,
    56,
    56,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,//58,
    56,
    56,
    56,
    56,//54,
    56,//54,
    56,//54,
    56,//52,
    56,//50,
    56,//50,
    48,
    48,
    48,//46,
    48,//44,
    40,//42,
    40,
    40,//38,
    40,//36,
    32,
    32,//28,
    32,//26,
    32,//20,
    0,//14,
    0,
    0
  };
  GsimBox* boxL=0;
  for(int i=0;i<nLSLayer;i++) {
    double YBlock=
      (L_XY+L_Gap)*(i-nLSLayer/2.+0.5);
    int nBlock = lsStack[i];
    for(int j=0;j<nBlock;j++) {
      double XBlock=
	(L_XY+L_Gap)*(j-nBlock/2.+0.5);
      // added by Sato 20120514
      //beam hole
      if( i>=(nLSLayer-nLayer)/2. &&
	  i<(nLSLayer-nLayer)/2.+nLayer &&
	  j>=(nBlock-nLayer)/2. &&
	  j<(nBlock-nLayer)/2.+nLayer )
	continue;
      
      
      
      XBlock*=-1;

      double ZBlock=L_Len/2.;
      if(boxL==0) {
	std::sprintf(nam,"LeadScinti");
	boxL = new GsimBox(std::string(nam),this,
			   G4ThreeVector(XBlock*cm,
					 YBlock*cm,
					 ZBlock*cm),
			   G4ThreeVector(0,0,0)
			   );
	boxL->setParameters(paramL);
	boxL->setOuterColor("yellow");
	boxL->setSensitiveDetector("CAL",id);
	addDaughter(boxL);

	double Pb_Len=0.275*0.1;//cm
	double Sc_Len=1.5*0.1+(0.35-0.275)*0.1;//cm
	int nPbScLayer=300;
	if(m_userFlag==1) {
	  Pb_Len=0.6*0.1;//cm
	  Sc_Len=1.4*0.1;//cm
	  nPbScLayer=250;
	}

	std::vector<double> paramPb;
	paramPb.push_back(L_XY*cm);
	paramPb.push_back(L_XY*cm);
	paramPb.push_back(Pb_Len*cm);
	
	std::sprintf(nam,"Pb");
	double PbZ=-L_Len/2.+(Pb_Len+Sc_Len)*0+Pb_Len/2.;
	GsimBox* pb = new GsimBox(std::string(nam),boxL,
				  G4ThreeVector(0,0,PbZ*cm),
				  G4ThreeVector(0,0,0)
				  );
	pb->setOuterMaterial("G4_Pb");
	pb->setParameters(paramPb);
	pb->setOuterColor("gray50");
	boxL->addDaughter(pb);
	for(int k=1;k<nPbScLayer;k++) {
	  PbZ=-L_Len/2.+(Pb_Len+Sc_Len)*k+Pb_Len/2.;
	  pb->cloneDetector(G4ThreeVector(0,0,PbZ*cm),
			    G4ThreeVector(0,0,0));
	}
	
	
      } else {
	boxL->cloneDetector(G4ThreeVector(XBlock*cm,
					  YBlock*cm,
					  ZBlock*cm),
			    G4ThreeVector(0,0,0),id);
      }
      id++;
    }
  }
  
  double chamberRin=(3050/2.+450+200);
  double chamberRout=chamberRin+20;//mm
  double dzTank=2000;//mm
  double dzFlange=80;//mm
  std::vector<double> paramVec;
  paramVec.clear();
  paramVec.push_back( chamberRin*mm);
  paramVec.push_back( chamberRout*mm);
  paramVec.push_back((dzTank-dzFlange*2)*mm);
  paramVec.push_back(0*deg);
  paramVec.push_back(360*deg);
  
  std::sprintf(nam,"Tank");
  GsimTube* tank = new GsimTube(std::string(nam),this,
				G4ThreeVector(0,0,(dzTank/2.)*mm),
				G4ThreeVector(0,0,0)
				);
  tank->setOuterMaterial("GsimSUS");
  tank->setOuterColor("gray50");
  tank->setParameters(paramVec);
  tank->setOuterVisibility(true);
  addDaughter(tank);

  paramVec.clear();
  paramVec.push_back(chamberRin*mm);
  paramVec.push_back((chamberRout+150)*mm);
  paramVec.push_back(dzFlange*mm);
  paramVec.push_back(0*deg);
  paramVec.push_back(360*deg);

  GsimTube* flange = 0;
  std::sprintf(nam,"USFlange");
  flange = new GsimTube(std::string(nam),this,
			G4ThreeVector(0,0,(dzFlange/2.)*mm),
			G4ThreeVector(0,0,0)
			);
  flange->setOuterMaterial("GsimSUS");
  flange->setOuterColor("gray50");
  flange->setParameters(paramVec);
  flange->setOuterVisibility(true);
  addDaughter(flange);

  paramVec.clear();
  paramVec.push_back(chamberRin*mm);
  paramVec.push_back((chamberRout+150)*mm);
  paramVec.push_back(dzFlange*mm);
  paramVec.push_back(0*deg);
  paramVec.push_back(360*deg);
      
  std::sprintf(nam,"DSFlange");
  flange = new GsimTube(std::string(nam),this,
			G4ThreeVector(0,0,(dzTank-dzFlange/2.)*mm),
			G4ThreeVector(0,0,0)
			);
  flange->setOuterMaterial("GsimSUS");
  flange->setOuterColor("gray50");
  flange->setParameters(paramVec);
  flange->setOuterVisibility(true);
  addDaughter(flange);
}

GsimKOTOIICalorimeter::~GsimKOTOIICalorimeter()
{
  ;
}


void GsimKOTOIICalorimeter::comment()
{
  std::cout << "GsimKOTOIICalorimeter Z position:" << std::endl;
  std::cout << "    Place it at 20.4 m" << std::endl;
}

void GsimKOTOIICalorimeter::setFastSimulationLevel(int level)
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
    
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z+detMoveZ*cm);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(true);
    }
  }

  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("CSI",0);
    }
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z-detMoveZ*cm);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(false);
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
