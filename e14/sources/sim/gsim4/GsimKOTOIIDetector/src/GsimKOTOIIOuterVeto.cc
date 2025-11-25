/**
 *  @file
 *  @brief  GsimKOTOIIOuterVeto
 *  $Id:  
 *  $Log: 
 *
 */
#include "GsimKOTOIIDetector/GsimKOTOIIOuterVeto.h"
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



GsimKOTOIIOuterVeto::GsimKOTOIIOuterVeto(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimKOTOIIOuterVeto";
  char nam[100];
  
  double OV_XY=5*4;//cm
  double OV_Gap=0.01*4;
  double OV_Len=6*83*0.1;//cm
  std::vector<double> paramOV;
  paramOV.push_back(OV_XY*cm);
  paramOV.push_back(OV_XY*cm);
  paramOV.push_back(OV_Len*cm);
  const int nOVLayer=18;
  int OVStack[nOVLayer]=
    {
      8,
      10,
      12,
      14,
      16,
      18,
      18,
      18,
      18,
      18,
      18,
      18,
      18,
      16,
      14,
      12,
      10,
      8
    };
  int id=0;
  GsimBox* boxOV=0;
  for(int i=0;i<nOVLayer;i++) {
    double YBlock=
      (OV_XY+OV_Gap)*(i-nOVLayer/2.+0.5);
    int nBlock = OVStack[i];
    for(int j=0;j<nBlock;j++) {
      if(i>=2 && i<16) {
	if(j>1 && j<nBlock-2) continue;
      }
      double XBlock=
	(OV_XY+OV_Gap)*(j-nBlock/2.+0.5);
      XBlock*=-1;
      
      double ZBlock=OV_Len/2.;
      if(boxOV==0) {
	std::sprintf(nam,"OV");
	boxOV = new GsimBox(std::string(nam),this,
			    G4ThreeVector(XBlock*cm,
					  YBlock*cm,
					  ZBlock*cm),
			    G4ThreeVector(0,0,0)
			    );
	boxOV->setParameters(paramOV);
	boxOV->setOuterColor("green");
	boxOV->setSensitiveDetector("OV",id);
	addDaughter(boxOV);


	double Pb_Len=1*0.1;//cm
	double Sc_Len=5*0.1;//cm

	std::vector<double> paramPb;
	paramPb.push_back(OV_XY*cm);
	paramPb.push_back(OV_XY*cm);
	paramPb.push_back(Pb_Len*cm);
	
	std::sprintf(nam,"Pb");
	double PbZ=-OV_Len/2.+(Pb_Len+Sc_Len)*0+Pb_Len/2.;
	GsimBox* pb = new GsimBox(std::string(nam),boxOV,
				  G4ThreeVector(0,0,PbZ*cm),
				  G4ThreeVector(0,0,0)
				  );
	pb->setOuterMaterial("G4_Pb");
	pb->setParameters(paramPb);
	pb->setOuterColor("gray50");
	boxOV->addDaughter(pb);
	for(int k=1;k<83;k++) {
	  PbZ=-OV_Len/2.+(Pb_Len+Sc_Len)*k+Pb_Len/2.;
	  pb->cloneDetector(G4ThreeVector(0,0,PbZ*cm),
			    G4ThreeVector(0,0,0));
	}




	
      } else {
	boxOV->cloneDetector(G4ThreeVector(XBlock*cm,
					   YBlock*cm,
					   ZBlock*cm),
			     G4ThreeVector(0,0,0),id);
      }
      id++;
    }
  }

}

GsimKOTOIIOuterVeto::~GsimKOTOIIOuterVeto()
{
  ;
}


void GsimKOTOIIOuterVeto::comment()
{
  std::cout << "GsimKOTOIIOuterVeto Z position:" << std::endl;
  std::cout << "    Place it at 20.4 m" << std::endl;
}

void GsimKOTOIIOuterVeto::setFastSimulationLevel(int level)
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
