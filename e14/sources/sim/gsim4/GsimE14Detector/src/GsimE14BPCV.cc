/**
 *  @file
 *  @brief  GsimE14BPCV
 *
 */
#include "GsimE14Detector/GsimE14BPCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14BPCV::GsimE14BPCV(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14BPCV";

  if(m_userFlag==201503 ||
     m_userFlag==201504 ||
     m_userFlag==201605
     ) {
    
    //////////////////
    //BPCV Position 
    /*

      BPCV0:Top
      _ ___________ _
      | |___________| |                  
      | |           | |                       
      BPCV3: | |           | | BPCV1:                          
      South  | |   Beam    | | North
      | |   Pipe    | |
      | |           | |
      | |___________| |
      |_|___________|_| 
               
      BPCV2:Bottom




      South
      |\__________________                 
      | \                 \
      |  \       Top       \            /
      CC06 |	|__________________\  CC05    /_____ 
      |	|                  |         / (KL Beam)
      |  |       North      |         \ _____          
      \	|__________________|          \
      \|                              \
      Bottom


    */
    //////////BPCV parameters////

    
    
    std::vector<double> paramVec;

    double  bpcv_t = 5.0*mm;
    double  bpcv_w1 = 200.0*mm;//width for Top and Bottom module
    double  bpcv_w2 = 210.0*mm;//width for North and South module
    double  bpcv_l = 1000.0*mm;

    double  x1_Pos = 0*mm;//x_Position for Top and Bottom module
    double  y1_Pos = (100.0 + bpcv_t/2)*mm;//y_Position for Top and Bottom module

    double  x2_Pos = (100.0 + bpcv_t/2)*mm;//x_Position for North and South module
    double  y2_Pos = 0*mm;//y_Position for North and South module
    double  z_Pos = 0*mm;//z_Position 
    
    GsimBox* box=0;


    //////////fiber parameters////
    double wFiber  = 1.5*mm; // diameter of fiber
    double sFiber  = 10.0*mm; // interval of fibers
    double xFiber=0*mm;
    //  double x2Fiber=0;
    double yFiber=0*mm;
    //  double y2Fiber=0;
    double tFiber = 1.55*mm; // deepth of groove
    double length_fiber=1000.0*mm;

    GsimBox* fiber=0;
    bool withFiber=true;
    

    //////////////////////
    //Modify z_pos, bpcv_l, length_fiber  due to z division
    int    nDivZ=20;//Number of z division
    z_Pos=
      z_Pos-bpcv_l/2.//front position
      +(bpcv_l/nDivZ)/2.;//cernter of z division
    bpcv_l = bpcv_l/nDivZ;
    length_fiber=length_fiber/nDivZ;
    //////////////////////


    //////////////////////
    //Top : BPCV0
    paramVec.clear();
    paramVec.push_back(bpcv_w1);
    paramVec.push_back(bpcv_t);
    paramVec.push_back(bpcv_l);
    box = new GsimBox("BPCVTop",this,
		      G4ThreeVector(x1_Pos, y1_Pos, z_Pos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("blue");
    box->setSensitiveDetector("BPCV",0);
    addDaughter(box);
    m_detMoveList.push_back(box);
  
    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length_fiber);
      fiber=0;
      yFiber=bpcv_t/2 - tFiber;
      for(int iF=0;iF<20;iF++) {
	xFiber=(iF-10)*sFiber + sFiber/2;
	if(!fiber) {
	  fiber = new GsimBox("BPCVFiberTop",box,
			      G4ThreeVector(xFiber,yFiber,0),
			      G4ThreeVector(0,0,0)
			      );
	  fiber->setParameters(paramVec);
	  fiber->setOuterColor("green");
	  fiber->setOuterMaterial("G4_POLYSTYRENE");
	  box->addDaughter(fiber);
	} else {
	  fiber->cloneDetector(G4ThreeVector(xFiber,yFiber,0),
			       G4ThreeVector(0,0,0));
	}
      }
    }

    //Z division
    for(int iZ=1;iZ<nDivZ;iZ++) {
      double zDiv=z_Pos+bpcv_l*iZ;
      box->cloneDetector(
			 G4ThreeVector(x1_Pos, y1_Pos, zDiv),
			 G4ThreeVector(0,0,0),
			 0
			 );
    }

  
  
    //////////////////////
    //North : BPCV1
    paramVec.clear();
    paramVec.push_back(bpcv_t);
    paramVec.push_back(bpcv_w2);
    paramVec.push_back(bpcv_l);
    box = new GsimBox("BPCVNorth",this,
		      G4ThreeVector(x2_Pos, y2_Pos, z_Pos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("blue");
    box->setSensitiveDetector("BPCV",1);
    addDaughter(box);
    m_detMoveList.push_back(box);

    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length_fiber);
      fiber=0;
      xFiber=bpcv_t/2 - tFiber;
      for(int iF=0;iF<21;iF++) {
	yFiber=(iF-10)*sFiber ;
	if(!fiber) {
	  fiber = new GsimBox("BPCVFiberNorth",box,
			      G4ThreeVector(xFiber,yFiber,0),
			      G4ThreeVector(0,0,0)
			      );
	  fiber->setParameters(paramVec);
	  fiber->setOuterColor("green");
	  fiber->setOuterMaterial("G4_POLYSTYRENE");
	  box->addDaughter(fiber);
	} else {
	  fiber->cloneDetector(G4ThreeVector(xFiber,yFiber,0),
			       G4ThreeVector(0,0,0));
	}
      }
    }
    
    //Z division
    for(int iZ=1;iZ<nDivZ;iZ++) {
      double zDiv=z_Pos+bpcv_l*iZ;
      box->cloneDetector(
			 G4ThreeVector(x2_Pos, y2_Pos, zDiv),
			 G4ThreeVector(0,0,0),
			 1
			 );
    }
  
    //////////////////////
    //Bottom : BPCV2
    paramVec.clear();
    paramVec.push_back(bpcv_w1);
    paramVec.push_back(bpcv_t);
    paramVec.push_back(bpcv_l);
    box = new GsimBox("BPCVBottom",this,
		      G4ThreeVector(x1_Pos, -y1_Pos, z_Pos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("blue");
    box->setSensitiveDetector("BPCV",2);
    addDaughter(box);
    m_detMoveList.push_back(box);
  
    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length_fiber);
      fiber=0;
      yFiber=bpcv_t/2 - tFiber;
      for(int iF=0;iF<20;iF++) {
	xFiber=(iF-10)*sFiber + sFiber/2;
	if(!fiber) {
	  fiber = new GsimBox("BPCVFiberBottom",box,
			      G4ThreeVector(xFiber,-yFiber,0),
			      G4ThreeVector(0,0,0)
			      );
	  fiber->setParameters(paramVec);
	  fiber->setOuterColor("green");
	  fiber->setOuterMaterial("G4_POLYSTYRENE");
	  box->addDaughter(fiber);
	} else {
	  fiber->cloneDetector(G4ThreeVector(xFiber,-yFiber,0),
			       G4ThreeVector(0,0,0));
	}
      }
    }

    //Z division
    for(int iZ=1;iZ<nDivZ;iZ++) {
      double zDiv=z_Pos+bpcv_l*iZ;
      box->cloneDetector(
			 G4ThreeVector(x1_Pos, -y1_Pos, zDiv),
			 G4ThreeVector(0,0,0),
			 2
			 );
    }
  
  
  
    //////////////////////
    //South : BPCV3
    paramVec.clear();
    paramVec.push_back(bpcv_t);
    paramVec.push_back(bpcv_w2);
    paramVec.push_back(bpcv_l);
    box = new GsimBox("BPCVSouth",this,
		      G4ThreeVector(-x2_Pos, y2_Pos, z_Pos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("blue");
    box->setSensitiveDetector("BPCV",3);
    addDaughter(box);
    m_detMoveList.push_back(box);
    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length_fiber);
      fiber=0;
      xFiber=bpcv_t/2 - tFiber;
      for(int iF=0;iF<21;iF++) {
	yFiber=(iF-10)*sFiber ;
	if(!fiber) {
	  fiber = new GsimBox("BPCVFiberSouth",box,
			      G4ThreeVector(-xFiber,yFiber,0),
			      G4ThreeVector(0,0,0)
			      );
	  fiber->setParameters(paramVec);
	  fiber->setOuterColor("green");
	  fiber->setOuterMaterial("G4_POLYSTYRENE");
	  box->addDaughter(fiber);
	} else {
	  fiber->cloneDetector(G4ThreeVector(-xFiber,yFiber,0),
			       G4ThreeVector(0,0,0));
	}
      }
    }

    //Z division
    for(int iZ=1;iZ<nDivZ;iZ++) {
      double zDiv=z_Pos+bpcv_l*iZ;
      box->cloneDetector(
			 G4ThreeVector(-x2_Pos, y2_Pos, zDiv),
			 G4ThreeVector(0,0,0),
			 3
			 );
    }
    

    setThisAndDaughterBriefName("BPCV");
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BPCV is not constructed.");
  }
}



 
GsimE14BPCV::~GsimE14BPCV()
{
  ;
}

void GsimE14BPCV::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  std::cout << "GsimE14BPCV Z position:" << std::endl;
  std::cout << "The center of scintillator along z." << std::endl;
  std::cout << "z=(614.8+242.5+2.+20.-2+(7.006+0.029)*3+8+100/2.)*cm" << std::endl;
  std::cout << "When user flag = 201605, place it downstream by 2 cm." << std::endl;
  std::cout << "z=(614.8+242.5+2.+20.-2+(7.006+0.029)*3+8+100/2.+2)*cm" << std::endl;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14BPCV::setFastSimulationLevel(int level)
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
