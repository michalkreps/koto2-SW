/**
 *  @file
 *  @brief  GsimE14LCV
 *  $Id$
 *  $Log$
 */
#include "GsimE14Detector/GsimE14LCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimBox.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14LCV::GsimE14LCV(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14LCV";

  if(
     ! (userFlag == 201301 ||
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
	       "Invalid user flag is assigned. LCV is not constructed.");
    return;
  }
  
  
  m_model=1;
  if(m_model==1) {
    
    
    //CsI CFRP Beam pipe
    double cfrpLength=500*mm;
    double cfrpOuterD=164*mm;
    double cfrpInnerD=155*mm;
    
    std::vector<double> paramVec;
    paramVec.push_back(45*deg);
    paramVec.push_back(360*deg);
    paramVec.push_back(4);
    paramVec.push_back(cfrpLength);
    paramVec.push_back(cfrpInnerD/2.);
    paramVec.push_back(cfrpOuterD/2.);
    
    GsimPolyhedra2* hedra=0;
    hedra = new GsimPolyhedra2("BeamPipe",this,
			       G4ThreeVector(0,0,0),
			       G4ThreeVector(0,0,0));
    hedra->setOuterMaterial("GsimCFRP");
    hedra->setParameters(paramVec);
    hedra->setOuterColor("gray70");
    addDaughter(hedra);
    m_detMoveList.push_back(hedra);
    
    
    
    
    //LCV scintillator
    
    double thick   = 3.*mm;
    double length  = 575*mm;
    double wTop    = 153.4*mm;
    double wSide   = 150.9*mm;
    double wBottom = 147.4*mm;
    double zPos    =
      +(length-cfrpLength)/2.
      -Detpos_cv_z_shift*cm;



    //Position for CV
    //
    //     CV CVRP
    // RCV
    // |   | |
    // |   | |      ---------
    // |   | |     | CVRP beam pipe
    // |    -----------------
    // |   |__________________LCV
    // |   |
    //  --- <------>
    //        Detpos_cv_z_shift*cm
    // Place it
    //  -cfrpLegnth/2.-Detpos_cv_z_shif*cm+length/2.
    
    
    
    double wFiber  = 1.1*mm;
    double sFiber  = 10*mm;
    double xFiber=0;
    double yFiber=0;
    
    GsimBox* box=0;
    GsimBox* fiber=0;

    bool withFiber=true;
    ////////////////////////////////////////////////////
    //Top
    paramVec.clear();
    paramVec.push_back(wTop);
    paramVec.push_back(thick);
    paramVec.push_back(length);
    box = new GsimBox("LCVSciTop",this,
		      G4ThreeVector(0,cfrpInnerD/2.-thick/2.,zPos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("yellow");
    box->setSensitiveDetector("LCV",1);
    addDaughter(box);
    m_detMoveList.push_back(box);

    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length);
      fiber=0;
      yFiber=thick/2.-wFiber/2.;
      for(int iF=0;iF<15;iF++) {
	xFiber=(iF-7)*sFiber;
	if(!fiber) {
	  fiber = new GsimBox("LCVFiberTop",box,
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
    ////////////////////////////////////////////////////
    //South
    paramVec.clear();
    paramVec.push_back(thick);
    paramVec.push_back(wSide);
    paramVec.push_back(length);
    double ySide=
      -cfrpInnerD/2.
      +(cfrpInnerD-(wSide+thick))/2.
      +wSide/2.;
    
    box = new GsimBox("LCVSciSouth",this,
		      G4ThreeVector(-cfrpInnerD/2.+thick/2.,ySide,zPos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("yellow");
    box->setSensitiveDetector("LCV",2);
    addDaughter(box);
    m_detMoveList.push_back(box);

    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length);
      fiber=0;
      xFiber=-thick/2.+wFiber/2.;
      for(int iF=0;iF<15;iF++) {
	yFiber=(iF-7)*sFiber;
	if(!fiber) {
	  fiber = new GsimBox("LCVFiberSouth",box,
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
    ////////////////////////////////////////////////////
    //Bottom
    paramVec.clear();
    paramVec.push_back(wBottom);
    paramVec.push_back(thick);
    paramVec.push_back(length);
    box = new GsimBox("LCVSciBottom",this,
		      G4ThreeVector(0,-cfrpInnerD/2.+thick/2.,zPos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("yellow");
    box->setSensitiveDetector("LCV",3);
    addDaughter(box);
    m_detMoveList.push_back(box);

    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length);
      fiber=0;
      yFiber=-thick/2.+wFiber/2.;
      for(int iF=0;iF<15;iF++) {
	xFiber=(iF-7)*sFiber;
	if(!fiber) {
	  fiber = new GsimBox("LCVFiberBottom",box,
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
    ////////////////////////////////////////////////////
    //North
    paramVec.clear();
    paramVec.push_back(thick);
    paramVec.push_back(wSide);
    paramVec.push_back(length);
    box = new GsimBox("LCVSciNorth",this,
		      G4ThreeVector(+(cfrpInnerD/2.-thick/2.),ySide,zPos),
		      G4ThreeVector(0,0,0)
		      );
    box->setParameters(paramVec);
    box->setOuterColor("yellow");
    box->setSensitiveDetector("LCV",0);
    addDaughter(box);
    m_detMoveList.push_back(box);

    if(withFiber) {
      paramVec.clear();
      paramVec.push_back(wFiber);
      paramVec.push_back(wFiber);
      paramVec.push_back(length);
      fiber=0;
      xFiber=+thick/2.-wFiber/2.;
      for(int iF=0;iF<15;iF++) {
	yFiber=(iF-7)*sFiber;
	if(!fiber) {
	  fiber = new GsimBox("LCVFiberNorth",box,
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
    ////////////////////////////////////////////////////
    setThisAndDaughterBriefName("LCV");
    hedra->setThisAndDaughterBriefName("BPIP");
  } else if(m_model==0) {
    //double bh=13.8;//cm
    //double th=0.4;//cm
    double bh=14.0;//cm
    double th=0.3;//cm
    
    
    std::vector<double> paramVec;
    paramVec.push_back((bh+th)*cm);
    paramVec.push_back(th*cm);
    paramVec.push_back(CC03_Len_NEW*cm);
    
    GsimBox* box=0;
    box = new GsimBox("LCVbox",this,
		      G4ThreeVector(th/2.*cm,
				    (bh/2.+th/2.)*cm,
				    0.*cm),
		      G4ThreeVector(0,0,0)
		      );
    
    box->setParameters(paramVec);
    box->setOuterColor("yellow");
    box->setSensitiveDetector("LCV",0);
    addDaughter(box);
    m_detMoveList.push_back(box);

    box->cloneDetector(G4ThreeVector((bh/2.+th/2.)*cm,
				     -th/2.*cm,
				     0.*cm),
		       G4ThreeVector(0,0,90*deg),1);

    box->cloneDetector(G4ThreeVector(-th/2.*cm,
				     -(bh/2.+th/2.)*cm,
				     0.*cm),
		       G4ThreeVector(0,0,0),2);

    box->cloneDetector(G4ThreeVector(-(bh/2.+th/2.)*cm,
				     th/2.*cm,
				     0.*cm),
		       G4ThreeVector(0,0,90*deg),3);
    
    setThisAndDaughterBriefName("LCV");
  } else {
    ;
  }
}

GsimE14LCV::~GsimE14LCV()
{
  ;
}

void GsimE14LCV::comment()
{
  std::cout << "GsimE14LCV Z position:" << std::endl;
  std::cout << "    CFRP beam pipe center." << std::endl;
  std::cout << "    Place it 250*mm downstream of CsI upstream surface." << std::endl;
  std::cout << "When userFlag is 201605, place it downstream by 2 cm." << std::endl;
  std::cout << "When userFlag is 201605, shift it further upstream by 8.5 mm," << std::endl;
  std::cout << "when userFlag is 201902 and later, shift it further upstream by 9.3 mm" << std::endl;
  std::cout << "in addition to 2 cm downstream." << std::endl;
}


void GsimE14LCV::setFastSimulationLevel(int level)
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
