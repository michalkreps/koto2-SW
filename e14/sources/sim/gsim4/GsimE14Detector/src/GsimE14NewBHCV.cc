/**
 *  @file
 *  @brief  GsimE14NewBHCV
 *
 */

#include "GsimE14Detector/GsimE14NewBHCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTube.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14NewBHCV::GsimE14NewBHCV(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14NewBHCV";
  
  if(m_userFlag==201503 ||
     m_userFlag==201504 ||
     m_userFlag==201605 ||
     m_userFlag==201902 ||
     m_userFlag==202002 ||
     m_userFlag==202102 ||
     m_userFlag==202306
     ) {
    /////////////////////////////////////////////////
    // Geometry 
    /////////////////////////////////////////////////
    //
    //  Mylar (12um)===================================================
    //  -----------------------------------------------------------------
    //  Module0  FrontFrame(10mm) FrontBufferGas
    //           Kapton(50um)===========================================
    //           G10(2.8mm)       Carbon(30um)
    //                            Sense/GurardGas(2.8mm-2*30um)
    //                                  SenseWire/GuardWire
    //                            Carbon(30um)
    //           Kapton(50um)============================================
    //           RearFrame(15mm)  RearBufferGas
    //  -----------------------------------------------------------------
    //  Module1
    //  Module2
    //  Mylar (12um)===================================================
    //
    /////////////////////////////////////////////////
    //        360mm
    //    _______________
    //   |               |
    //   |   __300mm__   |
    //   |  |         |  |
    //   |  |         |  |
    //   |  |         |  |
    //   |  |         |  |
    //   |  |_________|  |
    //   |               |
    //   |_______________|
    //
    /////////////////////////////////////////////////
    // Mount
    //  5t 219x520
    /////////////////////////////////////////////////

    
    /////////////////////////////////////////////////
    //Geometrical parameters
    /////////////////////////////////////////////////
    //Size
    G4double lOuter = 360.*mm;
    G4double lInner = 300.*mm;
    G4double dTorrance=1e-6*mm;
    
    //Thickness
    G4double tMylar      = 0.012*mm;
    G4double tFrontFrame = 10*mm;
    G4double tKapton     = 0.05*mm;
    G4double tG10        = 2.8*mm;
    G4double tRearFrame  = 15*mm;

    G4double tCarbon     = 0.03*mm;
    G4double tGas        = tG10-2*tCarbon;
    G4double tModule     = tFrontFrame+tKapton+tG10+tKapton+tRearFrame;
    
    //Radius
    G4double rSenseWire  = 0.025*mm;
    G4double rGuardWire  = 0.05*mm;

    //Module Shift
    G4double xShift[3]   = { 0.*mm ,  0.5*mm,  0.5*mm+0.45*mm};
    G4double yShift[3]   = { 0.*mm , -0.7*mm, -0.7*mm+0.5*mm };

    //Module, Readout,Wire
    G4int    nModule=3;
    G4int    nReadout=16;
    G4int    nWirePerChannel=10;
    G4double wWireSpacing=1.8*mm;
    /////////////////////////////////////////////////

    
    std::vector<double> paramVec;

    /////////////////////////////////////////////////
    // GsimDetector list
    GsimBox* mylar=0;
    GsimBox* frontFrame=0;
    GsimBox* kapton=0;
    GsimBox* g10=0;
    GsimBox* rearFrame=0;
    
    GsimBox* frontBufferGas=0;
    GsimBox* carbon=0;
    GsimBox* senseGas=0;
    GsimBox* guardGas=0;
    GsimBox* rearBufferGas=0;
    
    GsimTube* senseWire=0;
    GsimTube* guardWire=0;

    //Mount
    GsimBox* mount=0;
    GsimBox* bottomBar=0;

    //HeatPath
    GsimBox* heatPathOnAmp=0;
    GsimBox* heatPath=0;
    
    
    /////////////////////////////////////////////////

    
    /////////////////////////////////////////////////
    // Mylar
    /////////////////////////////////////////////////
    {
      //Front mylar
      paramVec.clear();
      paramVec.push_back(lOuter);
      paramVec.push_back(lOuter);
      paramVec.push_back(tMylar);

      G4double zFrontMylar=
	-tG10/2.
	-tKapton
	-tFrontFrame
	-tMylar/2.;
      mylar = new GsimBox("mylar",this,
			  G4ThreeVector(xShift[0],yShift[0],zFrontMylar),
			  G4ThreeVector(0,0,0));
      mylar->setParameters(paramVec);
      mylar->setOuterMaterial("G4_MYLAR");
      mylar->setOuterColor("yellow");
      addDaughter(mylar);

      //Rear mylar
      G4double zRearMylar=
	+tG10/2.
	+tKapton
	+tRearFrame
	+tModule*(nModule-1)
	+tMylar/2.;
      mylar->cloneDetector(G4ThreeVector(xShift[2],yShift[2],zRearMylar),
			   G4ThreeVector(0,0,0));
    }
    /////////////////////////////////////////////////
    int iSenseCount=0;
    for(int iModule = 0 ; iModule < nModule ; iModule++){
      /////////////////////////////////////////////////
      // FrontFrame (one side x 4 rotation)
      /////////////////////////////////////////////////
      {
	G4double width=(lOuter-lInner)/2.;
	paramVec.clear();
	paramVec.push_back(lOuter-width);
	paramVec.push_back(width);
	paramVec.push_back(tFrontFrame);
	
	G4double xPos=-width/2;
	G4double yPos=lOuter/2.-width/2.;
	G4double zPos=
	  -tG10/2.
	  -tKapton
	  -tFrontFrame/2.
	  +tModule*iModule;
	G4ThreeVector vPos   = G4ThreeVector(xPos,yPos,zPos);
	
	G4ThreeVector vShift = G4ThreeVector(xShift[iModule],yShift[iModule],0);
	
	for(int iRot=0;iRot<4;iRot++) {
	  G4ThreeVector vPosR = vPos;
	  vPosR.rotateZ(iRot*M_PI/2.);
	  vPosR=vPosR+vShift;
	  G4ThreeVector vRot = G4ThreeVector(0,0,iRot*M_PI/2.);
	  
	  if(frontFrame==0) {
	    frontFrame = new GsimBox("frontFrame",this,vPosR,vRot);
	    frontFrame->setParameters(paramVec);
	    frontFrame->setOuterMaterial("G4_Al");
	    frontFrame->setOuterColor("gray70");
	    addDaughter(frontFrame);
	  } else {
	    frontFrame->cloneDetector(vPosR,vRot);
	  }
	}
      }
      /////////////////////////////////////////////////
      // Kapton (Front/Rear)
      /////////////////////////////////////////////////
      {
	paramVec.clear();
	paramVec.push_back(lOuter);
	paramVec.push_back(lOuter);
	paramVec.push_back(tKapton);

	G4double xPos=xShift[iModule];
	G4double yPos=yShift[iModule];
	G4double zFrontPos=-tG10/2.-tKapton/2.+tModule*iModule;
	G4double zRearPos =+tG10/2.+tKapton/2.+tModule*iModule;

	//Front
	if(kapton==0) {
	  kapton = new GsimBox("kapton",this,
			       G4ThreeVector(xPos,yPos,zFrontPos),
			       G4ThreeVector(0,0,0));
	  kapton->setParameters(paramVec);
	  kapton->setOuterMaterial("G4_KAPTON");
	  kapton->setOuterColor("yellow");
	  addDaughter(kapton);
	} else {
	  kapton->cloneDetector(
				G4ThreeVector(xPos,yPos,zFrontPos),
				G4ThreeVector(0,0,0));
	}
	//Rear
	kapton->cloneDetector(
			      G4ThreeVector(xPos,yPos,zRearPos),
			      G4ThreeVector(0,0,0));
      }

      /////////////////////////////////////////////////
      // G10 (one side x 4 rotation)
      /////////////////////////////////////////////////
      {
	G4double width=(lOuter-lInner)/2.;
	paramVec.clear();
	paramVec.push_back(lOuter-width);
	paramVec.push_back(width);
	paramVec.push_back(tG10);
	
	G4double xPos=-width/2;
	G4double yPos=lOuter/2.-width/2.;
	G4double zPos=0+tModule*iModule;
	G4ThreeVector vPos   = G4ThreeVector(xPos,yPos,zPos);
	
	G4ThreeVector vShift = G4ThreeVector(xShift[iModule],yShift[iModule],0);
	
	for(int iRot=0;iRot<4;iRot++) {
	  G4ThreeVector vPosR = vPos;
	  vPosR.rotateZ(iRot*M_PI/2.);
	  vPosR=vPosR+vShift;
	  G4ThreeVector vRot = G4ThreeVector(0,0,iRot*M_PI/2.);
	  
	  if(g10==0) {
	    g10 = new GsimBox("g10",this,vPosR,vRot);
	    g10->setParameters(paramVec);
	    g10->setOuterMaterial("GsimG10");
	    g10->setOuterColor("green");
	    addDaughter(g10);
	  } else {
	    g10->cloneDetector(vPosR,vRot);
	  }
	}
      }

      /////////////////////////////////////////////////
      // RearFrame (one side x 4 rotation)
      /////////////////////////////////////////////////
      {
	G4double width=(lOuter-lInner)/2.;
	paramVec.clear();
	paramVec.push_back(lOuter-width);
	paramVec.push_back(width);
	paramVec.push_back(tRearFrame);
	
	G4double xPos=-width/2;
	G4double yPos=lOuter/2.-width/2.;
	G4double zPos=
	  +tG10/2.
	  +tKapton
	  +tRearFrame/2.
	  +tModule*iModule;
	G4ThreeVector vPos   = G4ThreeVector(xPos,yPos,zPos);
	G4ThreeVector vShift = G4ThreeVector(xShift[iModule],yShift[iModule],0);
	
	for(int iRot=0;iRot<4;iRot++) {
	  G4ThreeVector vPosR = vPos;
	  vPosR.rotateZ(iRot*M_PI/2.);
	  vPosR=vPosR+vShift;
	  G4ThreeVector vRot = G4ThreeVector(0,0,iRot*M_PI/2.);
	  
	  if(rearFrame==0) {
	    rearFrame = new GsimBox("rearFrame",this,vPosR,vRot);
	    rearFrame->setParameters(paramVec);
	    rearFrame->setOuterMaterial("G4_Al");
	    rearFrame->setOuterColor("gray70");
	    addDaughter(rearFrame);
	  } else {
	    rearFrame->cloneDetector(vPosR,vRot);
	  }
	}
      }

      /////////////////////////////////////////////////
      // FrontBufferGas
      /////////////////////////////////////////////////
      {
	paramVec.clear();
	paramVec.push_back(lInner);
	paramVec.push_back(lInner);
	paramVec.push_back(tFrontFrame);
	
	G4double xPos=xShift[iModule];
	G4double yPos=yShift[iModule];
	G4double zPos=
	  -tG10/2.
	  -tKapton
	  -tFrontFrame/2.
	  +tModule*iModule;
	if(frontBufferGas==0) {
	  frontBufferGas = new GsimBox("frontBufferGas",this,
				       G4ThreeVector(xPos,yPos,zPos),
				       G4ThreeVector(0,0,0));
	  frontBufferGas->setParameters(paramVec);
	  frontBufferGas->setOuterMaterial("GsimCF4nPentane");
	  frontBufferGas->setOuterColor("yellow");
	  addDaughter(frontBufferGas);
	} else {
	  frontBufferGas->cloneDetector(
					G4ThreeVector(xPos,yPos,zPos),
					G4ThreeVector(0,0,0));
	}
      }

      /////////////////////////////////////////////////
      // Carbon
      /////////////////////////////////////////////////
      {
	paramVec.clear();
	paramVec.push_back(lInner);
	paramVec.push_back(lInner);
	paramVec.push_back(tCarbon);
	
	G4double xPos=xShift[iModule];
	G4double yPos=yShift[iModule];
	G4double zFrontPos=
	  -tGas/2.
	  -tCarbon/2
	  +tModule*iModule;
	G4double zRearPos=
	  +tGas/2.
	  +tCarbon/2
	  +tModule*iModule;

	//Front
	if(carbon==0) {
	  carbon = new GsimBox("carbon",this,
			       G4ThreeVector(xPos,yPos,zFrontPos),
			       G4ThreeVector(0,0,0));
	  carbon->setParameters(paramVec);
	  carbon->setOuterMaterial("G4_GRAPHITE");
	  carbon->setOuterColor("gray70");
	  addDaughter(carbon);
	} else {
	  carbon->cloneDetector(
				G4ThreeVector(xPos,yPos,zFrontPos),
				G4ThreeVector(0,0,0));
	}

	//Rear
	carbon->cloneDetector(
			      G4ThreeVector(xPos,yPos,zRearPos),
			      G4ThreeVector(0,0,0));
      }
      
      /////////////////////////////////////////////////
      // RearBufferGas
      /////////////////////////////////////////////////
      {
	paramVec.clear();
	paramVec.push_back(lInner);
	paramVec.push_back(lInner);
	paramVec.push_back(tRearFrame);
	
	G4double xPos=xShift[iModule];
	G4double yPos=yShift[iModule];
	G4double zPos=
	  +tG10/2.
	  +tKapton
	  +tRearFrame/2.
	  +tModule*iModule;
	if(rearBufferGas==0) {
	  rearBufferGas = new GsimBox("rearBufferGas",this,
				      G4ThreeVector(xPos,yPos,zPos),
				      G4ThreeVector(0,0,0));
	  rearBufferGas->setParameters(paramVec);
	  rearBufferGas->setOuterMaterial("GsimCF4nPentane");
	  rearBufferGas->setOuterColor("yellow");
	  addDaughter(rearBufferGas);
	} else {
	  rearBufferGas->cloneDetector(
				       G4ThreeVector(xPos,yPos,zPos),
				       G4ThreeVector(0,0,0));
	}
      }
      
      /////////////////////////////////////////////////
      // GuardGas (Bottom,Top)
      /////////////////////////////////////////////////
      {
	double wGuardGas=
	  (lInner - nReadout*nWirePerChannel*wWireSpacing)/2.;
	
	paramVec.clear();
	paramVec.push_back(lInner);
	paramVec.push_back(wGuardGas-dTorrance);
	paramVec.push_back(tGas);
	
	G4double xPos       = xShift[iModule];
	G4double yPosBottom = yShift[iModule]-lInner/2.+wGuardGas/2.;
	G4double yPosTop    = yShift[iModule]+lInner/2.-wGuardGas/2.;
	
	G4double zPos=0+tModule*iModule;

	//Bottom
	if(guardGas==0) {
	  guardGas = new GsimBox("guardGas",this,
				 G4ThreeVector(xPos,yPosBottom,zPos),
				 G4ThreeVector(0,0,0));
	  guardGas->setParameters(paramVec);
	  guardGas->setOuterMaterial("GsimCF4nPentane");
	  guardGas->setOuterColor("yellow");
	  addDaughter(guardGas);

	  //GuardWire
	  {
	    paramVec.clear();
	    paramVec.push_back(0);
	    paramVec.push_back(rGuardWire);
	    paramVec.push_back(lInner);
	    paramVec.push_back(0.*deg);
	    paramVec.push_back(360.*deg);
	    
	    guardWire = new GsimTube("guardWire",guardGas,
				     G4ThreeVector(0,+wGuardGas/2.-wWireSpacing/2.,0),
				     G4ThreeVector(0,90*deg,0));
	    guardWire->setParameters(paramVec);
	    guardWire->setOuterMaterial("G4_W");
	    guardWire->setOuterColor("red");
	    guardGas->addDaughter(guardWire);
	  }
	} else {
	  guardGas->cloneDetector(
				  G4ThreeVector(xPos,yPosBottom,zPos),
				  G4ThreeVector(0,0,0));
	}
	// Top
	guardGas->cloneDetector(
				G4ThreeVector(xPos,yPosTop,zPos),
				G4ThreeVector(M_PI,0,0));
      }

      /////////////////////////////////////////////////
      // SenseGas x nReadout
      /////////////////////////////////////////////////
      {
	double wSenseGas=
	  nWirePerChannel*wWireSpacing-dTorrance;
	
	paramVec.clear();
	paramVec.push_back(lInner);
	paramVec.push_back(wSenseGas-dTorrance);
	paramVec.push_back(tGas);
	
	G4double xPos=xShift[iModule];
	G4double yPos0=yShift[iModule]
	  +wSenseGas*nReadout/2.
	  -wSenseGas/2.;
	G4double zPos=0+tModule*iModule;
	
	
	for(int iCh=0;iCh<nReadout;iCh++) {
	  G4double yPos=yPos0-wSenseGas*iCh;
	  
	  if(senseGas==0) {
	    senseGas = new GsimBox("senseGas",this,
				   G4ThreeVector(xPos,yPos,zPos),
				   G4ThreeVector(0,0,0));
	    senseGas->setParameters(paramVec);
	    senseGas->setOuterMaterial("GsimCF4nPentane");
	    senseGas->setOuterColor("yellow");
	    senseGas->setSensitiveDetector("NewBHCV",iSenseCount);
	    addDaughter(senseGas);

	    //SenseWire;
	    {
	      paramVec.clear();
	      paramVec.push_back(0);
	      paramVec.push_back(rSenseWire);
	      paramVec.push_back(lInner);
	      paramVec.push_back(0.*deg);
	      paramVec.push_back(360.*deg);
	      
	      for(int iWire=0;iWire<nWirePerChannel;iWire++) {
		G4double yWire=
		  -(nWirePerChannel*wWireSpacing)/2.
		  +wWireSpacing/2.
		  +wWireSpacing*iWire;
		if(senseWire==0) {
		  senseWire = new GsimTube("senseWire",senseGas,
					   G4ThreeVector(0,yWire,0),
					   G4ThreeVector(0,90*deg,0));
		  senseWire->setParameters(paramVec);
		  senseWire->setOuterMaterial("G4_W");
		  senseWire->setOuterColor("red");
		  senseGas->addDaughter(senseWire);
		} else {
		  senseWire->cloneDetector(G4ThreeVector(0,yWire,0),
					   G4ThreeVector(0,90*deg,0));
		}
	      }
	    }
	  } else {
	    senseGas->cloneDetector(
				    G4ThreeVector(xPos,yPos,zPos),
				    G4ThreeVector(0,0,0),
				    iSenseCount);
	  }
	  iSenseCount++;
	}
      }
      //End of Module loop
    }

    /////////////////////////////////////////////////
    // Mount (Left,Right) x (Front,Rear)
    /////////////////////////////////////////////////
    {
      G4double dMount=151*mm;// half distance between Left and Right inner side
      G4double wMount=219*mm;
      G4double lMount=520*mm;
      G4double tMount=5*mm;
      
      paramVec.clear();
      paramVec.push_back(wMount);
      paramVec.push_back(lMount);
      paramVec.push_back(tMount);
      
      G4double zPosFront=
	-tG10/2.
	-tKapton
	-tFrontFrame
	-tMylar
	-tMount/2.;
      G4double zPosRear=
	+tG10/2.
	+tKapton
	+tRearFrame
	+tModule*(nModule-1)
	+tMylar
	+tMount/2.;

      G4double xPosLeft =-dMount-wMount/2.;
      G4double xPosRight=+dMount+wMount/2.;
      G4double yPos=-15*mm;

      //Front Right
      mount = new GsimBox("mount",this,
			  G4ThreeVector(xPosRight+xShift[0],yPos+yShift[0],zPosFront),
			  G4ThreeVector(0,0,0));
      mount->setParameters(paramVec);
      mount->setOuterMaterial("G4_Al");
      mount->setOuterColor("gray70");
      addDaughter(mount);
      //Front Left
      mount->cloneDetector(G4ThreeVector(xPosLeft+xShift[0],yPos+yShift[0],zPosFront),
			   G4ThreeVector(0,0,0));
      //Rear Right
      mount->cloneDetector(G4ThreeVector(xPosRight+xShift[2],yPos+yShift[2],zPosRear),
			   G4ThreeVector(0,0,0));
      //Rear Left
      mount->cloneDetector(G4ThreeVector(xPosLeft+xShift[2],yPos+yShift[2],zPosRear),
			   G4ThreeVector(0,0,0));
      
    }

    /////////////////////////////////////////////////
    // BottomBar (Left,Right) x (Front,Rear)
    /////////////////////////////////////////////////
    {
      G4double xBottomBar=282.5*mm;// xPosition
      G4double wBottomBar=175*mm;
      G4double lBottomBar=70*mm;
      G4double tBottomBar=15*mm;
      
      paramVec.clear();
      paramVec.push_back(wBottomBar);
      paramVec.push_back(lBottomBar);
      paramVec.push_back(tBottomBar);

      G4double xPosLeft=-xBottomBar;
      G4double xPosRight=+xBottomBar;
      G4double zPosFront=
	-tG10/2.
	-tKapton
	-tFrontFrame
	-tMylar
	+tBottomBar/2.;
      G4double zPosRear=
	+tG10/2.
	+tKapton
	+tRearFrame
	+tModule*(nModule-1)
	+tMylar
	-tBottomBar/2.;

      G4double yPos=-214.5*mm;

      //Front Right
      bottomBar = new GsimBox("bottomBar",this,
			      G4ThreeVector(xPosRight+xShift[0],yPos+yShift[0],zPosFront),
			      G4ThreeVector(0,0,0));
      bottomBar->setParameters(paramVec);
      bottomBar->setOuterMaterial("G4_Al");
      bottomBar->setOuterColor("gray70");
      addDaughter(bottomBar);
      //Front Left
      bottomBar->cloneDetector(G4ThreeVector(xPosLeft+xShift[0],yPos+yShift[0],zPosFront),
			       G4ThreeVector(0,0,0));
      //Rear Right
      bottomBar->cloneDetector(G4ThreeVector(xPosRight+xShift[2],yPos+yShift[2],zPosRear),
			       G4ThreeVector(0,0,0));
      //Rear Left
      bottomBar->cloneDetector(G4ThreeVector(xPosLeft+xShift[2],yPos+yShift[2],zPosRear),
			       G4ThreeVector(0,0,0)); 
    }

    /////////////////////////////////////////////////
    // HeatPathOnAmp North(Top,Bottom) South (Top,Bottom)x(Front,Rear)
    /////////////////////////////////////////////////
    {
      G4double xHeatPathOnAmp=(180+106.5)*mm;// xPosition
      G4double yHeatPathOnAmp= 88.5*mm;// yPosition
      G4double wHeatPathOnAmp= 65*mm;
      G4double lHeatPathOnAmp=155*mm;
      G4double tHeatPathOnAmp= 10*mm;

      paramVec.clear();
      paramVec.push_back(wHeatPathOnAmp);
      paramVec.push_back(lHeatPathOnAmp);
      paramVec.push_back(tHeatPathOnAmp);
      
      G4double zPos[3][2] // module [0,1,2] x [Top,Bottom]
	= 
	{
	  {+tG10/2.+(tModule)*0+tHeatPathOnAmp/2.,
	   +tG10/2.+(tModule)*0+tHeatPathOnAmp/2.},
	  {+tG10/2.+(tModule)*1+tHeatPathOnAmp/2.,
	   +tG10/2.+(tModule)*1+tHeatPathOnAmp/2.},
	  {+tG10/2.+(tModule)*2+tHeatPathOnAmp/2.,
	   +tG10/2.+(tModule)*2+tHeatPathOnAmp/2.}
	};
      
      G4double xPos[3][2] // module [0,1,2] x [Top,Bottom]
	=
	{
	  {-xHeatPathOnAmp,-xHeatPathOnAmp},
	  {+xHeatPathOnAmp,+xHeatPathOnAmp},
	  {-xHeatPathOnAmp,-xHeatPathOnAmp}
	};

      G4double yPos[3][2] // module [0,1,2] x [Top,Bottom]
	=
	{
	  {+yHeatPathOnAmp,-yHeatPathOnAmp},
	  {+yHeatPathOnAmp,-yHeatPathOnAmp},
	  {+yHeatPathOnAmp,-yHeatPathOnAmp}
	};
      

      for(int iModule=0;iModule<nModule;iModule++) {
	for(int iTop=0;iTop<2;iTop++) {
	  if(heatPathOnAmp==0) {
	    heatPathOnAmp = new GsimBox("heatPathOnAmp",this,
					G4ThreeVector(xPos[iModule][iTop]+xShift[iModule],
						      yPos[iModule][iTop]+yShift[iModule],
						      zPos[iModule][iTop]),
					G4ThreeVector(0,0,0));
	    heatPathOnAmp->setParameters(paramVec);
	    heatPathOnAmp->setOuterMaterial("G4_Al");
	    heatPathOnAmp->setOuterColor("gray70");
	    addDaughter(heatPathOnAmp);	    
	  } else {
	    heatPathOnAmp->cloneDetector(
					 G4ThreeVector(xPos[iModule][iTop]+xShift[iModule],
						       yPos[iModule][iTop]+yShift[iModule],
						       zPos[iModule][iTop]),
					 G4ThreeVector(0,0,0));
	  }
	}
      }
    }

    /////////////////////////////////////////////////
    // HeatPath (Left,Right) x (Top,Bottom)
    /////////////////////////////////////////////////
    {
      G4double xHeatPath=(180+106.5)*mm;// xPosition
      G4double wHeatPath=65*mm;
      G4double lHeatPath=10*mm;
      G4double tHeatPath=50*mm;
      
      paramVec.clear();
      paramVec.push_back(wHeatPath);
      paramVec.push_back(lHeatPath);
      paramVec.push_back(tHeatPath);

      G4double zHeatPath=
	+tG10/2.
	+tKapton
	+tRearFrame
	+tModule*0.5;
      G4double yHeatPath=(171+1)*mm; // 1*mm for yShift

      //South Bottom
      heatPath = new GsimBox("heatPath",this,
			     G4ThreeVector(-xHeatPath,-yHeatPath,zHeatPath),
			     G4ThreeVector(0,0,0));
      heatPath->setParameters(paramVec);
      heatPath->setOuterMaterial("G4_Al");
      heatPath->setOuterColor("gray70");
      addDaughter(heatPath);
      //North Bottom
      heatPath->cloneDetector(
			      G4ThreeVector(+xHeatPath,-yHeatPath,zHeatPath),
			      G4ThreeVector(0,0,0));
      //South Top
      heatPath->cloneDetector(
			      G4ThreeVector(-xHeatPath,+yHeatPath,zHeatPath),
			      G4ThreeVector(0,0,0));
      //North Top
      heatPath->cloneDetector(
			      G4ThreeVector(+xHeatPath,+yHeatPath,zHeatPath),
			      G4ThreeVector(0,0,0));
    }

    setThisAndDaughterBriefName("NewBHCV");
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. NewBHCV is not constructed.");
    return;
  }
  m_detMoveList.push_back(this);    
}

GsimE14NewBHCV::~GsimE14NewBHCV()
{
  ;
}

void GsimE14NewBHCV::setFastSimulationLevel(int level)
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


void GsimE14NewBHCV::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::cout << "GsimE14NewBHCV Z position:" << std::endl;
  std::cout << "    wire plane of the 1st module" << std::endl;
  std::cout << "When userFlag is 201605, place it downstream by 2 cm."
	    << std::endl;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}
