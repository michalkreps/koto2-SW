/**
 *  @file
 *  @brief  GsimKOTOIIUpstreamCollar
 *  $Id:  
 *  $Log: 
 *
 */
#include "GsimKOTOIIDetector/GsimKOTOIIUpstreamCollar.h"
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



GsimKOTOIIUpstreamCollar::GsimKOTOIIUpstreamCollar(std::string name,
						   GsimDetector* motherDetector,
						   G4ThreeVector transV,G4ThreeVector rotV,
						   int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimKOTOIIUpstreamCollar";
  char nam[100];
  
  double UC_XY=5;//cm
  double UC_Gap=0.01;
  double UC_Len=50;//cm
  std::vector<double> paramOV;
  paramOV.push_back(UC_XY*cm);
  paramOV.push_back(UC_XY*cm);
  paramOV.push_back(UC_Len*cm);
  const int nUCLayer=10;
  int UCStack[nUCLayer]=
    {
      4,
      6,
      8,
      10,
      10,
      10,
      10,
      8,
      6,
      4
    };
  int id=0;
  GsimBox* boxUC=0;
  for(int i=0;i<nUCLayer;i++) {
    double YBlock=
      (UC_XY+UC_Gap)*(i-nUCLayer/2.+0.5);
    int nBlock = UCStack[i];
    for(int j=0;j<nBlock;j++) {
      if(i>=3 && i<=6) {
	if(j>=3 && j<=6) continue;
      }
      double XBlock=
	(UC_XY+UC_Gap)*(j-nBlock/2.+0.5);
      XBlock*=-1;
      
      double ZBlock=UC_Len/2.;
      if(boxUC==0) {
	std::sprintf(nam,"UC");
	boxUC = new GsimBox(std::string(nam),this,
			    G4ThreeVector(XBlock*cm,
					  YBlock*cm,
					  ZBlock*cm),
			    G4ThreeVector(0,0,0)
			    );
	boxUC->setParameters(paramOV);
	boxUC->setOuterColor("lightskyblue");
	boxUC->setSensitiveDetector("UC",id);
	addDaughter(boxUC);
      } else {
	boxUC->cloneDetector(G4ThreeVector(XBlock*cm,
					   YBlock*cm,
					   ZBlock*cm),
			     G4ThreeVector(0,0,0),id);
      }
      id++;
    }
  }
}

GsimKOTOIIUpstreamCollar::~GsimKOTOIIUpstreamCollar()
{
  ;
}


void GsimKOTOIIUpstreamCollar::comment()
{
  std::cout << "GsimKOTOIIUpstreamCollar Z position:" << std::endl;
  std::cout << "    Upstream surface" << std::endl;
  std::cout << "    Place it at 850 mm" << std::endl;
}

void GsimKOTOIIUpstreamCollar::setFastSimulationLevel(int level)
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
