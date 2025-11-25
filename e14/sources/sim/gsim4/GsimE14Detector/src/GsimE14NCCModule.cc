/**
 *  @file
 *  @brief  GsimE14NCCModule
 *  $Id:$
 *  $Log:$
 */
#include "GsimE14Detector/GsimE14NCCModule.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimBox.h"

#include "G4SystemOfUnits.hh"

#include <list>
#include <sstream>

GsimE14NCCModule::GsimE14NCCModule(std::string name,
				   GsimDetector* motherDetector,
				   G4ThreeVector transV,G4ThreeVector rotV,
				   int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14NCCModule";


  bool isFiber=true;

  //NCC parameters

  G4double wCsI=66.0*mm;
  //G4double tTeflon=0.2*mm;
  G4double tTeflon=0.3*mm;
  //G4double tPMMA=2.1*mm;
  G4double tPMMA=2.3*mm;
  G4double tCrystalMirror=0.5*mm;
  G4double CsIZlength[3]={148.0*mm,200.0*mm,96.0*mm};


  G4double moduleZlength[3];
  moduleZlength[0]=tTeflon        + CsIZlength[0] + tCrystalMirror;
  moduleZlength[1]=tCrystalMirror + CsIZlength[1] + tCrystalMirror;
  moduleZlength[2]=tCrystalMirror + CsIZlength[2] + tTeflon;

  G4double nccZlength=moduleZlength[0]+moduleZlength[1]+moduleZlength[2];

  G4double moduleZpos[3]={
    -nccZlength/2.+tTeflon+CsIZlength[0]/2.,
    -nccZlength/2.+moduleZlength[0]+tCrystalMirror+CsIZlength[1]/2.,
    -nccZlength/2.+moduleZlength[0]+moduleZlength[1]+tCrystalMirror+CsIZlength[2]/2.,
  };


  GsimBox* nccCsI[3]={0,0,0};
  const char* nccName[3]={"nccCsIF","nccCsIM","nccCsIR"};
  //char* nccColor[3]={"red","blue","green"};
  const char* nccColor[3]={"cyan","cyan","cyan"};
  
  
  for(int i=0;i<3;i++){
    if(nccCsI[i]==0) {
      std::vector<double> param;
      param.clear();
      param.push_back(wCsI);
      param.push_back(wCsI);
      param.push_back(CsIZlength[i]);
      
      G4ThreeVector pos = G4ThreeVector(0,0,moduleZpos[i]);
      G4ThreeVector rot = G4ThreeVector(0,0,0);
      nccCsI[i] = new GsimBox(nccName[i],this ,pos,rot);
      nccCsI[i]->setParameters(param);
      nccCsI[i]->setOuterMaterial("G4_CESIUM_IODIDE");
      nccCsI[i]->setOuterColor(nccColor[i]);
      nccCsI[i]->setSensitiveDetector("NCC",i+1);
      addDaughter(nccCsI[i]);
    }
  }

  //2t Arcyrlic
  GsimBox* acrylic=0;
  GsimTube* fiber[3]={0,0,0};//rear and common, middle, front
  const char* fiberName[3]={"fiberRC","fiberM","fiberF"};
  double fiberL[3]={
    moduleZlength[0]+moduleZlength[1]+moduleZlength[2],
    moduleZlength[0]+moduleZlength[1],
    moduleZlength[0]
  };
  double fiberZ[3]={
    -fiberL[0]/2.+fiberL[0]/2.,
    -fiberL[0]/2.+fiberL[1]/2.,
    -fiberL[0]/2.+fiberL[2]/2.
  };
    
  {
    int iRot=0;
    G4ThreeVector pos = G4ThreeVector(0,0,0);
    double dPos=(wCsI/2.+tPMMA/2.);
    if(iRot==0) {
      pos=pos+G4ThreeVector(0,dPos,0);
    } else if(iRot==1) {
      pos=pos+G4ThreeVector(-dPos,0,0);
    } else if(iRot==2) {
      pos=pos+G4ThreeVector(0,-dPos,0);
    } else if(iRot==3) {
      pos=pos+G4ThreeVector(+dPos,0,0);
    }
    
    G4ThreeVector rot = G4ThreeVector(0,0,iRot*M_PI/2.);
    
    if(acrylic==0) {
      std::vector<double> paramAcrylic;
      paramAcrylic.push_back(wCsI);
      paramAcrylic.push_back(tPMMA);
      paramAcrylic.push_back(nccZlength);
      
      
      acrylic =  new GsimBox("acrylic",this ,pos,rot);
      acrylic->setParameters(paramAcrylic);
      acrylic->setOuterMaterial("G4_PLEXIGLASS");
      acrylic->setOuterColor("lightyellow");
      addDaughter(acrylic);

      double rFiber=0.5*mm*0.88;
      //core is 1-(3%+3%)*2=0.88, 2013.12.24 from Kawasaki

      std::vector<double> paramFiber;
      paramFiber.push_back(0);
      //paramFiber.push_back(0.5*mm);2013.12.24 from Kawasaki
      paramFiber.push_back(rFiber);
      paramFiber.push_back(0.);
      paramFiber.push_back(0.*deg);
      paramFiber.push_back(360.*deg);

      if(isFiber) {
	for(int k=0;k<40;k++) {
	  
	  // fiber --> rear and common ,middle,front fibers
	  //   3 types of fiber length
	  
	  
	  int itype=0;
	  int id=0;
	  // fiber scintillation -> id>=1000
	  if(k==0 || k==1 || k==38 || k==39) {
	    // 4 rear fibers
	    itype=0;
	    id=3;
	  } else if(k==2 || k==3 || k==36 || k==37) {
	    // 4 middle fibers
	    itype=1;
	    id=2;
	  } else if(k==4 || k==5 || k==34 || k==35) {
	    // 4 front fibers
	    itype=2;
	    id=1;
	  } else {
	    //28 common fibers
	    itype=0;
	    id=0;
	  }
	  
	  double xPos=1.65*mm*(k-19.5);
	  G4ThreeVector posAcrylic = G4ThreeVector(xPos,-0.5*mm,fiberZ[itype]);
	  
	  if(fiber[itype]==0) {
	    
	    fiber[itype]= new GsimTube(fiberName[itype],acrylic,
				       posAcrylic,
				       G4ThreeVector(0,0,0));
	    
	    fiber[itype]->setOuterMaterial("G4_POLYSTYRENE");
	    paramFiber[2]=fiberL[itype];
	    fiber[itype]->setParameters(paramFiber);
	    fiber[itype]->setOuterColor("blue");
	    fiber[itype]->setSensitiveDetector("NCCfiber",id);
	    acrylic->addDaughter(fiber[itype]);
	  } else {
	    fiber[itype]->cloneDetector(posAcrylic,
					G4ThreeVector(0,0,0),id);
	  }
	}
      }
    }
  }
  
  if(isFiber) {
    GsimSensitiveDetector* sd =
      dynamic_cast<GsimSensitiveDetector*>(GsimUtil::getUtil()->getSensitiveDetector("NCCfiber"));
    sd->setDeepCopyNoFactor(10);
    sd->setThisCopyNoFactor(1);
  }
  

  setThisAndDaughterBriefName("NCC");
}

GsimE14NCCModule::~GsimE14NCCModule()
{
  ;
}


bool GsimE14NCCModule::endOfEventAction(const G4Event* )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::list<GsimSensitiveDetector*> sdList
    =getThisAndDaughterSensitiveDetectorList();
  for(std::list<GsimSensitiveDetector*>::iterator it=sdList.begin();
      it!=sdList.end();it++) {
    if( (*it)->getName()=="NCC") 
      (*it)->addDigiMergeByN(10,48);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif 
  return true;
}
