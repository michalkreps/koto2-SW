/**
 *  @file
 *  @brief  GsimE14Shield
 *  $Id$
 *  $Log$
 */
#include "GsimE14Detector/GsimE14Shield.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimBox.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14Shield::GsimE14Shield(std::string name,
			     GsimDetector* motherDetector,
			     G4ThreeVector transV,G4ThreeVector rotV,
			     int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14Shield";

  if(
     ! (userFlag == 201212 ||
	userFlag == 201301 ||
	userFlag == 201303 ||
	userFlag == 201304 ||
	userFlag == 201305 ||
	userFlag == 201503 ||
	userFlag == 201504 ||
	userFlag == 201605 ||
	userFlag == 201902 ||
	userFlag == 202002 ||
	userFlag == 202102 ||
	userFlag == 202306 
	)
     ) {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. Shield is not constructed.");
    return;
  }
  
  
  double shieldL=1000*mm;
  double shieldHoleW=256*mm;

  double ironOffset=-40*mm;
  double ironSideW=1000*mm;
  double ironSideH=500*mm;
  
  double ironTopW=2000*mm;
  double ironTopH=100*mm;

  double ironBottomW=shieldHoleW;
  double ironBottomH=160*mm;
  
  
  double concreteW=2000*mm;
  double concreteTopH=1000*mm;
  double concreteBottomH=1500*mm;

  GsimBox* box=0;

  //Iron Side
  std::vector<double> paramVec;
  paramVec.push_back(ironSideW);
  paramVec.push_back(ironSideH);
  paramVec.push_back(shieldL);

  double xPos=shieldHoleW/2.+ironSideW/2.;
  double yPos=ironOffset;
  
  box = new GsimBox("ironSide",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_Fe");
  box->setOuterColor("red");
  addDaughter(box);
  
  box->cloneDetector(G4ThreeVector(-xPos,yPos,0),
		     G4ThreeVector(0,0,0));


  //Iron Bottom
  paramVec.clear();
  paramVec.push_back(ironBottomW);
  paramVec.push_back(ironBottomH);
  paramVec.push_back(shieldL);
  
  xPos=0;
  yPos=ironOffset-ironSideH/2.+ironBottomH/2.;

  box = new GsimBox("ironBottom",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_Fe");
  box->setOuterColor("red");
  addDaughter(box);

  //Iron Top
  paramVec.clear();
  paramVec.push_back(ironTopW);
  paramVec.push_back(ironTopH);
  paramVec.push_back(shieldL);
  
  xPos=0;
  yPos=ironOffset+ironSideH/2.+ironTopH/2.;

  box = new GsimBox("ironTop",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_Fe");
  box->setOuterColor("red");
  addDaughter(box);




  //Concrete Bottom
  paramVec.clear();
  paramVec.push_back(concreteW);
  paramVec.push_back(concreteBottomH);
  paramVec.push_back(shieldL);
  
  xPos=0;
  yPos=ironOffset-ironSideH/2.-concreteBottomH/2.;

  box = new GsimBox("concreteBottom",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_CONCRETE");
  box->setOuterColor("lightyellow");
  addDaughter(box);


  //Concrete Top
  paramVec.clear();
  paramVec.push_back(concreteW);
  paramVec.push_back(concreteTopH);
  paramVec.push_back(shieldL);
  
  xPos=0;
  yPos=ironOffset+ironSideH/2.+ironTopH+concreteTopH/2.;

  box = new GsimBox("concreteTop",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_CONCRETE");
  box->setOuterColor("lightyellow");
  addDaughter(box);



  //Air box
  paramVec.clear();
  paramVec.push_back(shieldHoleW);
  paramVec.push_back(ironSideH-ironBottomH);
  paramVec.push_back(shieldL);
  
  xPos=0;
  yPos=ironOffset+ironSideH/2.-(ironSideH-ironBottomH)/2.;

  box = new GsimBox("airBox",this,
		    G4ThreeVector(xPos,yPos,0),
		    G4ThreeVector(0,0,0)
		    );
  box->setParameters(paramVec);
  box->setOuterMaterial("G4_AIR");
  box->setOuterColor("white");
  addDaughter(box);
  
  setThisAndDaughterBriefName("SHIELD");
  m_detMoveList.push_back(this);

}

GsimE14Shield::~GsimE14Shield()
{
  ;
}

double GsimE14Shield::getShieldLength() {
  return 100;
}

void GsimE14Shield::setFastSimulationLevel(int level)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //reset
  if(m_fastSimulationLevel==6) {
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z+29*m);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(true);
    }
  }

  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z-29*m);
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
