/**
 *  @file
 *  @brief  GsimE14NCC
 *  $Id:$
 *  $Log:$
 */
#include "GsimE14Detector/GsimE14NCC.h"
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

GsimE14NCC::GsimE14NCC(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14NCC";


  bool isFiber=false;
  if(m_userFlag==2011 ||
     m_userFlag==201212 ||
     m_userFlag==201301 ||
     m_userFlag==201303 ||
     m_userFlag==201304 ||
     m_userFlag==201305 ||
     m_userFlag==201503 ||
     m_userFlag==201504 ||
     m_userFlag==201605 ||
     m_userFlag==201902 ||
     m_userFlag==202002 ||
     m_userFlag==202102 ||
     m_userFlag==202306
     ) {
    isFiber=true;
  } else {
    isFiber=false;

    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. NCC is not constructed.");
    return;
  }
  

  //NCC parameters

  G4double wCsI=66.0*mm;
  //G4double tTeflon=0.2*mm;
  G4double tTeflon=0.3*mm;
  //G4double tPMMA=2.1*mm;
  G4double tPMMA=2.3*mm;
  G4double tSUS=0.1*mm;
  G4double tCrystalMirror=0.5*mm;
  G4double modgap=0.0*mm;

  G4double beamholeExtension=7*mm;
  
  G4double wCFRP=147.2*mm;
  G4double tCFRP=2*mm;
  G4double lCFRP=446*mm;
  G4double cutOutCFRP=7*mm;
  G4double cutInCFRP=6.37*mm;

  G4double lFrame=580.*mm;
  G4double hFrame=628.*mm;

  G4double tJoint=20*mm;
  G4double tJoint2=10*mm;
  G4double lPMTSupport=700*mm;


  
  
  
  G4double CsIZlength[3]={148.0*mm,200.0*mm,96.0*mm};

  G4double moduleWidth =wCsI      +tTeflon*2+tSUS*2+modgap*2;
  G4double moduleHeight=wCsI+tPMMA+tTeflon*2+tSUS*2+modgap*2;

  G4double moduleZlength[3];
  moduleZlength[0]=tTeflon        + CsIZlength[0] + tCrystalMirror;
  moduleZlength[1]=tCrystalMirror + CsIZlength[1] + tCrystalMirror;
  moduleZlength[2]=tCrystalMirror + CsIZlength[2] + tTeflon;

  G4double nccZlength=moduleZlength[0]+moduleZlength[1]+moduleZlength[2];

  G4double zShift=0;//Origin is the center of the crystal
  
  G4double moduleZpos[3]={
    -nccZlength/2.+tTeflon+CsIZlength[0]/2.,
    -nccZlength/2.+moduleZlength[0]+tCrystalMirror+CsIZlength[1]/2.,
    -nccZlength/2.+moduleZlength[0]+moduleZlength[1]+tCrystalMirror+CsIZlength[2]/2.,
  };

  G4double outerCsIZlength=446.*mm;
  G4double wOuterCsI=50*mm;
  G4double outerModuleWidth =wOuterCsI+tTeflon*2+tSUS*2+modgap*2;
  G4double outerL=29*mm;
  G4double outerS=25*mm;
  

  G4double CsIXpos[58];
  G4double CsIYpos[58];
  int      CsIRotN[58] = {
    //   =====
    //  |     | rotate 0 times
    //  |     |
    //   -----
    //inner
    0,1,1,2,2,3,3,0,
    //corner
    1,1,3,3,
    //middle
    0,1,1,1,1,1,1,2,
    2,3,3,3,3,3,3,0,
    //outer
    0,0,1,1,1,1,1,1,2,2,
    2,2,3,3,3,3,3,3,0,0,
    //no use
    -1,-1,
    //outter (no acryrlic)
    0,0,0,0,0,0,0,0
  };



  //////////////////////////
  // inner layer 2moduels x 4trun = 8 modules
  //      7  0
  //   6        1
  //   5        2
  //      4  3 
  {
    CsIXpos[0]=+moduleWidth/2.;
    CsIYpos[0]=+(moduleWidth/2.+moduleWidth+beamholeExtension);
    
    CsIXpos[1]=+(moduleWidth/2.+moduleWidth+beamholeExtension);
    CsIYpos[1]=+moduleWidth/2.;

    CsIXpos[2]=+CsIXpos[1];
    CsIYpos[2]=-CsIYpos[1];

    CsIXpos[3]=+CsIXpos[0];
    CsIYpos[3]=-CsIYpos[0];
    
    CsIXpos[4]=-CsIXpos[3];
    CsIYpos[4]=+CsIYpos[3];
    
    CsIXpos[5]=-CsIXpos[2];
    CsIYpos[5]=+CsIYpos[2];

    CsIXpos[6]=-CsIXpos[1];
    CsIYpos[6]=+CsIYpos[1];

    CsIXpos[7]=-CsIXpos[0];
    CsIYpos[7]=+CsIYpos[0];
  }
  //////////////////////////
  // corner
  //   11    8
  //
  //   10    9
  //
  {
    CsIXpos[8]=+moduleWidth/2.+moduleWidth;
    CsIYpos[8]=+moduleWidth/2.+moduleWidth;

    CsIXpos[9]=+CsIXpos[8];
    CsIYpos[9]=-CsIYpos[8];

    CsIXpos[10]=-CsIXpos[9];
    CsIYpos[10]=+CsIYpos[9];

    CsIXpos[11]=-CsIXpos[8];
    CsIYpos[11]=+CsIYpos[8];
  }
  
  //////////////////////////
  // middle layer  4modules x 4trun = 16 modules
  //          
  //          26  27  12  13 
  //      25                  14
  //      24                  15
  //      23                  16
  //      22                  17
  //          21  20  19  18
  {

    CsIXpos[12]=+moduleWidth/2.;
    CsIYpos[12]=+moduleWidth/2.+(moduleWidth+beamholeExtension+moduleHeight);

    CsIXpos[13]=+moduleWidth/2.+moduleWidth;
    CsIYpos[13]=+moduleWidth/2.+(moduleWidth+moduleWidth);

    //------------------------------------------------------------------

    CsIXpos[14]=+moduleWidth/2.+(moduleWidth+moduleHeight);
    CsIYpos[14]=+moduleWidth/2.+moduleWidth;

    CsIXpos[15]=+moduleWidth/2.+(moduleWidth+beamholeExtension+moduleHeight);
    CsIYpos[15]=+moduleWidth/2.;

    CsIXpos[16]=+CsIXpos[15];
    CsIYpos[16]=-CsIYpos[15];

    CsIXpos[17]=+CsIXpos[14];
    CsIYpos[17]=-CsIYpos[14];

    //------------------------------------------------------------------

    CsIXpos[18]=+CsIXpos[13];
    CsIYpos[18]=-CsIYpos[13];

    CsIXpos[19]=+CsIXpos[12];
    CsIYpos[19]=-CsIYpos[12];

    //------------

    CsIXpos[20]=-CsIXpos[19];
    CsIYpos[20]=+CsIYpos[19];

    CsIXpos[21]=-CsIXpos[18];
    CsIYpos[21]=+CsIYpos[18];

    //------------------------------------------------------------------

    CsIXpos[22]=-CsIXpos[17];
    CsIYpos[22]=+CsIYpos[17];

    CsIXpos[23]=-CsIXpos[16];
    CsIYpos[23]=+CsIYpos[16];

    CsIXpos[24]=-CsIXpos[15];
    CsIYpos[24]=+CsIYpos[15];

    CsIXpos[25]=-CsIXpos[14];
    CsIYpos[25]=+CsIYpos[14];

    //------------------------------------------------------------------

    CsIXpos[26]=-CsIXpos[13];
    CsIYpos[26]=+CsIYpos[13];

    CsIXpos[27]=-CsIXpos[12];
    CsIYpos[27]=+CsIYpos[12];
  }
  //////////////////////////

  //////////////////////////
  // outer layer  5modules x 4trun = 20 modules
  //
  //          46  47  28  29
  //      45                  30  
  //  44                          31
  //  43                          32
  //  42                          33
  //  41                          34
  //      40                  35
  //          39  38  37  36
  //
  //
  {
    CsIXpos[28]=+moduleWidth/2.;
    CsIYpos[28]=+moduleWidth/2.+(moduleWidth+beamholeExtension+moduleHeight*2);

    CsIXpos[29]=+moduleWidth/2.+moduleWidth;
    CsIYpos[29]=+moduleWidth/2.+(moduleWidth*3);

    CsIXpos[30]=+moduleWidth/2.+(moduleWidth+moduleHeight);
    CsIYpos[30]=+moduleWidth/2.+(moduleWidth*2);

    CsIXpos[31]=+moduleWidth/2.+(moduleWidth+moduleHeight*2);
    CsIYpos[31]=+moduleWidth/2.+(moduleWidth*1);

    CsIXpos[32]=+moduleWidth/2.+(moduleWidth+beamholeExtension+moduleHeight*2);
    CsIYpos[32]=+moduleWidth/2.;

    //-----------------

    CsIXpos[33]=+CsIXpos[32];
    CsIYpos[33]=-CsIYpos[32];
    
    CsIXpos[34]=+CsIXpos[31];
    CsIYpos[34]=-CsIYpos[31];

    CsIXpos[35]=+CsIXpos[30];
    CsIYpos[35]=-CsIYpos[30];

    CsIXpos[36]=+CsIXpos[29];
    CsIYpos[36]=-CsIYpos[29];

    CsIXpos[37]=+CsIXpos[28];
    CsIYpos[37]=-CsIYpos[28];
    
    //-----------------

    for(int k=38;k<48;k++) {
      CsIXpos[k]=-CsIXpos[37-(k-38)];
      CsIYpos[k]=+CsIYpos[37-(k-38)];
    }

    //-----------------
  }

  //no use
  for(int k=48;k<50;k++) {
    CsIXpos[k]=0;
    CsIYpos[k]=0;
  }

  //outer
  //    57         50
  // 56               51
  //
  //
  // 55              52
  //    54        53
  {
    CsIXpos[50]=+outerModuleWidth/2.+(moduleWidth*2);
    CsIYpos[50]=+outerModuleWidth/2.+(moduleWidth*3);

    CsIXpos[51]=+outerModuleWidth/2.+(moduleWidth+moduleHeight*2);
    CsIYpos[51]=+outerModuleWidth/2.+(moduleWidth*2);

    
    CsIXpos[52]=+CsIXpos[51];
    CsIYpos[52]=-CsIYpos[51];

    CsIXpos[53]=+CsIXpos[50];
    CsIYpos[53]=-CsIYpos[50];

    //----

    CsIXpos[54]=-CsIXpos[53];
    CsIYpos[54]=+CsIYpos[53];

    CsIXpos[55]=-CsIXpos[52];
    CsIYpos[55]=+CsIYpos[52];

    CsIXpos[56]=-CsIXpos[51];
    CsIYpos[56]=+CsIYpos[51];

    CsIXpos[57]=-CsIXpos[50];
    CsIYpos[57]=+CsIYpos[50];
  }
  //////////////////////////

  //Bug fix for opposite x sign
  for(int k=0;k<58;k++) {
    CsIXpos[k]=-CsIXpos[k];
  }
  
  
  
  

  GsimBox* nccCsI[3]={0,0,0};
  const char* nccName[3]={"nccCsIF","nccCsIM","nccCsIR"};
  //char* nccColor[3]={"red","blue","green"};
  const char* nccColor[3]={"cyan","cyan","cyan"};
  
  
  for(int i=0;i<3;i++){
    for(int j=0; j<48; j++){
      if(j>=8 && j<12) {
	//Corner blocks
	continue;
      }
      
      if(nccCsI[i]==0) {
	std::vector<double> param;
	param.clear();
	param.push_back(wCsI);
	param.push_back(wCsI);
	param.push_back(CsIZlength[i]);

	G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],moduleZpos[i]+zShift);
	G4ThreeVector rot = G4ThreeVector(0,0,0);
	nccCsI[i] = new GsimBox(nccName[i],this ,pos,rot);
	nccCsI[i]->setParameters(param);
	nccCsI[i]->setOuterMaterial("G4_CESIUM_IODIDE");
	nccCsI[i]->setOuterColor(nccColor[i]);
	nccCsI[i]->setSensitiveDetector("NCC",j*10+i+1);
	addDaughter(nccCsI[i]);
      } else {
	G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],moduleZpos[i]+zShift);
	G4ThreeVector rot = G4ThreeVector(0,0,0);
	nccCsI[i]->cloneDetector(pos,rot,j*10+i+1);
      }
      //F:001 011 ... 471
      //M:002 012 ... 472
      //R:003 013 ... 473
    }
  }


  /////////////////////////////////////////////
  //  Corner Block
  /////////////////////////////////////////////

  //          7
  //    -----   
  //   |      \ 7
  //66  -----  |
  //   |______ |
  //      66
  //
  G4ThreeVector boxV  = G4ThreeVector(0,
				      -wCsI/2.+(wCsI-beamholeExtension)/2.,
				      0);
  G4ThreeVector trapV = G4ThreeVector(-wCsI/2.+ ((wCsI-beamholeExtension)+wCsI)/4.,
				      +wCsI/2.-beamholeExtension/2.,
				      0);				      

  GsimBox* nccCornerBox[3]={0,0,0};
  GsimTrap* nccCornerTrap[3]={0,0,0};
  
  const char* nccCornerBoxName[3]={"nccCornerBoxCsIF",
				   "nccCornerBoxCsIM",
				   "nccCornerBoxCsIR"};
  
  const char* nccCornerTrapName[3]={"nccCornerTrapCsIF",
				    "nccCornerTrapCsIM",
				    "nccCornerTrapCsIR"};
  
  for(int i=0;i<3;i++) {
    for(int j=8;j<12;j++) {


      {
	G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],moduleZpos[i]+zShift);
	G4ThreeVector rot = G4ThreeVector(0,0,(j-9)*M_PI/2.);
	G4ThreeVector mov = boxV;
	mov.rotateZ(rot[2]);
	pos=mov+pos;
	
	if(nccCornerBox[i]==0) {
	  std::vector<double> paramCornerBox;
	  paramCornerBox.push_back(wCsI);
	  paramCornerBox.push_back(wCsI-beamholeExtension);
	  paramCornerBox.push_back(CsIZlength[i]);
	  
	  nccCornerBox[i]= new GsimBox(nccCornerBoxName[i],this ,pos,rot);
	  nccCornerBox[i]->setParameters(paramCornerBox);
	  nccCornerBox[i]->setOuterMaterial("G4_CESIUM_IODIDE");
	  nccCornerBox[i]->setOuterColor(nccColor[i]);
	  nccCornerBox[i]->setSensitiveDetector("NCC",j*10+i+1);
	  addDaughter(nccCornerBox[i]);
	} else {
	  nccCornerBox[i]->cloneDetector(pos,rot,j*10+i+1);
	}

      }

      {

	G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],moduleZpos[i]+zShift);
	G4ThreeVector rot = G4ThreeVector(0,0,(j-9)*M_PI/2.);
	G4ThreeVector mov = trapV;
	mov.rotateZ(rot[2]);
	pos=mov+pos;
	
	if(nccCornerTrap[i]==0) {

	  double alpha=-std::atan( (wCsI/2.- (wCsI-beamholeExtension)/2.)
				   /beamholeExtension );
	  std::vector<double> paramCornerTrap;
	  paramCornerTrap.push_back(CsIZlength[i]);
	  paramCornerTrap.push_back(0.);
	  paramCornerTrap.push_back(0.);
	  paramCornerTrap.push_back(beamholeExtension);
	  paramCornerTrap.push_back(wCsI);
	  paramCornerTrap.push_back(wCsI-beamholeExtension);
	  paramCornerTrap.push_back(alpha);
	  paramCornerTrap.push_back(beamholeExtension);
	  paramCornerTrap.push_back(wCsI);
	  paramCornerTrap.push_back(wCsI-beamholeExtension);
	  paramCornerTrap.push_back(alpha);
	  
	  nccCornerTrap[i]= new GsimTrap(nccCornerTrapName[i],this ,pos,rot);
	  nccCornerTrap[i]->setParameters(paramCornerTrap);
	  nccCornerTrap[i]->setOuterMaterial("G4_CESIUM_IODIDE");
	  nccCornerTrap[i]->setOuterColor(nccColor[i]);
	  nccCornerTrap[i]->setSensitiveDetector("NCC",j*10+i+1);
	  addDaughter(nccCornerTrap[i]);
	} else {
	  nccCornerTrap[i]->cloneDetector(pos,rot,j*10+i+1);
	}
      }
    }
  }

  /////////////////////////////////////////////
  //  Outer L
  /////////////////////////////////////////////
  //      29
  //    -----   
  //   |      ' 
  //50  -----  |
  //   |______ |29
  //      50
  //

  //double outerRot[4]={0,3,2,1};
  //Bug fix for opposite x sign
  double outerRot[4]={1,2,3,0};
  
  G4ThreeVector boxLV  = G4ThreeVector(0,
				       -wOuterCsI/2.+outerL/2.,
				       0);
  G4ThreeVector trapLV = G4ThreeVector(-wOuterCsI/2.+ (outerL+wOuterCsI)/4.,
				       +wOuterCsI/2.-(wOuterCsI-outerL)/2.,
				       0);
  GsimBox*  nccOuterLBox=0;
  GsimTrap* nccOuterLTrap=0;
  const char* nccOuterLBoxName="nccOuterLBox";
  const char* nccOuterLTrapName="nccOuterLTrap";

  int idL[4]={50,53,54,57};
  for(int j=0;j<4;j++) {
    
    int id=idL[j];

    {
      G4ThreeVector pos = G4ThreeVector(CsIXpos[id],CsIYpos[id],+zShift);
      G4ThreeVector rot = G4ThreeVector(0,0,outerRot[j]*M_PI/2.);
      G4ThreeVector mov = boxLV;
      mov.rotateZ(rot[2]);
      pos=mov+pos;
      
      if(nccOuterLBox==0) {
	std::vector<double> paramOuterLBox;
	paramOuterLBox.push_back(wOuterCsI);
	paramOuterLBox.push_back(outerL);
	paramOuterLBox.push_back(outerCsIZlength);
	
	nccOuterLBox= new GsimBox(nccOuterLBoxName,this ,pos,rot);
	nccOuterLBox->setParameters(paramOuterLBox);
	nccOuterLBox->setOuterMaterial("G4_CESIUM_IODIDE");
	nccOuterLBox->setOuterColor("cyan");
	nccOuterLBox->setSensitiveDetector("NCC",id*10);
	addDaughter(nccOuterLBox);
      } else {
	nccOuterLBox->cloneDetector(pos,rot,id*10);
      }
    }

    {
      G4ThreeVector pos = G4ThreeVector(CsIXpos[id],CsIYpos[id],+zShift);
      G4ThreeVector rot = G4ThreeVector(0,0,outerRot[j]*M_PI/2.);
      G4ThreeVector mov = trapLV;
      mov.rotateZ(rot[2]);
      pos=mov+pos;
      
      if(nccOuterLTrap==0) {

	double alpha=-std::atan( (wOuterCsI/2.- outerL/2.)
				 /(wOuterCsI-outerL) );
	std::vector<double> paramOuterLTrap;
	paramOuterLTrap.push_back(outerCsIZlength);
	paramOuterLTrap.push_back(0.);
	paramOuterLTrap.push_back(0.);
	paramOuterLTrap.push_back(wOuterCsI-outerL);
	paramOuterLTrap.push_back(wOuterCsI);
	paramOuterLTrap.push_back(outerL);
	paramOuterLTrap.push_back(alpha);
	paramOuterLTrap.push_back(wOuterCsI-outerL);
	paramOuterLTrap.push_back(wOuterCsI);
	paramOuterLTrap.push_back(outerL);
	paramOuterLTrap.push_back(alpha);

	
	nccOuterLTrap= new GsimTrap(nccOuterLTrapName,this ,pos,rot);
	nccOuterLTrap->setParameters(paramOuterLTrap);
	nccOuterLTrap->setOuterMaterial("G4_CESIUM_IODIDE");
	nccOuterLTrap->setOuterColor("cyan");
	nccOuterLTrap->setSensitiveDetector("NCC",id*10);
	addDaughter(nccOuterLTrap);
      } else {
	nccOuterLTrap->cloneDetector(pos,rot,id*10);
      }
    }


    
  }

  /////////////////////////////////////////////
  //  Outer S
  /////////////////////////////////////////////
  //      25
  //    -----   
  //   |      ' 
  //50  -----  |
  //   |______ |25
  //      50
  //



  G4ThreeVector boxSV  = G4ThreeVector(0,
				       -wOuterCsI/2.+outerS/2.,
				       0);
  G4ThreeVector trapSV = G4ThreeVector(-wOuterCsI/2.+ (outerS+wOuterCsI)/4.,
				       +wOuterCsI/2.-(wOuterCsI-outerS)/2.,
				       0);
  GsimBox*  nccOuterSBox=0;
  GsimTrap* nccOuterSTrap=0;
  const char* nccOuterSBoxName="nccOuterSBox";
  const char* nccOuterSTrapName="nccOuterSTrap";
  int idS[4]={51,52,55,56};

  for(int j=0;j<4;j++) {
    
    int id=idS[j];
    {
      G4ThreeVector pos = G4ThreeVector(CsIXpos[id],CsIYpos[id],+zShift);
      G4ThreeVector rot = G4ThreeVector(0,0,outerRot[j]*M_PI/2.);
      G4ThreeVector mov = boxSV;
      mov.rotateZ(rot[2]);
      pos=mov+pos;
      
      if(nccOuterSBox==0) {
	std::vector<double> paramOuterSBox;
	paramOuterSBox.push_back(wOuterCsI);
	paramOuterSBox.push_back(outerS);
	paramOuterSBox.push_back(outerCsIZlength);
	
	nccOuterSBox= new GsimBox(nccOuterSBoxName,this ,pos,rot);
	nccOuterSBox->setParameters(paramOuterSBox);
	nccOuterSBox->setOuterMaterial("G4_CESIUM_IODIDE");
	nccOuterSBox->setOuterColor("cyan");
	nccOuterSBox->setSensitiveDetector("NCC",id*10);
	addDaughter(nccOuterSBox);
      } else {
	nccOuterSBox->cloneDetector(pos,rot,id*10);
      }
    }


    {
      G4ThreeVector pos = G4ThreeVector(CsIXpos[id],CsIYpos[id],+zShift);
      G4ThreeVector rot = G4ThreeVector(0,0,outerRot[j]*M_PI/2.);
      G4ThreeVector mov = trapSV;
      mov.rotateZ(rot[2]);
      pos=mov+pos;
      
      if(nccOuterSTrap==0) {

	double alpha=-std::atan( (wOuterCsI/2.- outerS/2.)
				 /(wOuterCsI-outerS) );
	std::vector<double> paramOuterSTrap;
	paramOuterSTrap.push_back(outerCsIZlength);
	paramOuterSTrap.push_back(0.);
	paramOuterSTrap.push_back(0.);
	paramOuterSTrap.push_back(wOuterCsI-outerS);
	paramOuterSTrap.push_back(wOuterCsI);
	paramOuterSTrap.push_back(outerS);
	paramOuterSTrap.push_back(alpha);
	paramOuterSTrap.push_back(wOuterCsI-outerS);
	paramOuterSTrap.push_back(wOuterCsI);
	paramOuterSTrap.push_back(outerS);
	paramOuterSTrap.push_back(alpha);

	
	nccOuterSTrap= new GsimTrap(nccOuterSTrapName,this ,pos,rot);
	nccOuterSTrap->setParameters(paramOuterSTrap);
	nccOuterSTrap->setOuterMaterial("G4_CESIUM_IODIDE");
	nccOuterSTrap->setOuterColor("cyan");
	nccOuterSTrap->setSensitiveDetector("NCC",id*10);
	addDaughter(nccOuterSTrap);
      } else {
	nccOuterSTrap->cloneDetector(pos,rot,id*10);
      }
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
    
  for(int j=0;j<48;j++) {
    int iRot=CsIRotN[j];
    G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],+zShift);
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
      m_detList.push_back(acrylic);

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
	  
	  //common
	  // 00 01 ... 47
	  //individaul
	  
	  
	}
      }
    } else {
      acrylic->cloneDetector(pos,rot,j);
    }
  }
  

  /*
  //2t Arcyrlic
  GsimBox* acrylic[3] ={0,0,0};
  const char* acrylicName[3]={"acrylicF","acrylicM","acrylicR"};

  GsimTube* fiber[3]={0,0,0};
  const char* fiberName[3]={"fiberF","fiberM","fiberR"};
  

  for(int i=0;i<3;i++) {
    for(int j=0;j<48;j++) {

      int iRot=CsIRotN[j];
      G4ThreeVector pos = G4ThreeVector(CsIXpos[j],CsIYpos[j],moduleZpos[i]);
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
    
    
      if(acrylic[i]==0) {
	std::vector<double> paramAcrylic;
	paramAcrylic.push_back(wCsI);
	paramAcrylic.push_back(tPMMA);
	paramAcrylic.push_back(CsIZlength[i]);
      
      
	acrylic[i] =  new GsimBox(acrylicName[i],this ,pos,rot);
	acrylic[i]->setParameters(paramAcrylic);
	acrylic[i]->setOuterMaterial("G4_PLEXIGLASS");
	acrylic[i]->setOuterColor("lightyellow");
	addDaughter(acrylic[i]);


	for(int k=0;k<40;k++) {
	  double xPos=1.65*mm*(k-19.5);
	  G4ThreeVector posAcrylic = G4ThreeVector(xPos,-0.5*mm,0);
	  if(fiber[i]==0) {
	    std::vector<double> paramFiber;
	    paramFiber.push_back(0);
	    paramFiber.push_back(0.5*mm);
	    paramFiber.push_back(CsIZlength[i]);
	    paramFiber.push_back(0.*deg);
	    paramFiber.push_back(360.*deg);
	    
	    fiber[i]= new GsimTube(fiberName[i],acrylic[i],
				   posAcrylic,
				   G4ThreeVector(0,0,0));
	    fiber[i]->setOuterMaterial("G4_Fe");
	    fiber[i]->setParameters(paramFiber);
	    fiber[i]->setOuterColor("blue");
	    acrylic[i]->addDaughter(fiber[i]);
	  } else {
	    fiber[i]->cloneDetector(posAcrylic,
				    G4ThreeVector(0,0,0));
	  }
	}
      } else {
	acrylic[i]->cloneDetector(pos,rot);
      }
    }
  }
  */

  //CFRP beam pipe

  double cfrpL1=wCFRP-cutOutCFRP*2;
  double cfrpL2=wCFRP-cutInCFRP*2-tCFRP*2;

  std::vector<double> paramCFRPL;
  paramCFRPL.push_back(lCFRP);
  paramCFRPL.push_back(0.);
  paramCFRPL.push_back(0.);
  paramCFRPL.push_back(tCFRP);
  paramCFRPL.push_back(cfrpL1);
  paramCFRPL.push_back(cfrpL2);
  paramCFRPL.push_back(0);
  paramCFRPL.push_back(tCFRP);
  paramCFRPL.push_back(cfrpL1);
  paramCFRPL.push_back(cfrpL2);
  paramCFRPL.push_back(0);

  GsimTrap* cfrpL = new GsimTrap("cfrpL",this ,
				 G4ThreeVector(0,-(wCFRP/2.-tCFRP/2.),+zShift),
				 G4ThreeVector(0,0,0));
  cfrpL->setParameters(paramCFRPL);
  cfrpL->setOuterMaterial("GsimCFRP");
  cfrpL->setOuterColor("gray50");
  addDaughter(cfrpL);
  m_detList.push_back(cfrpL);
  cfrpL->cloneDetector(G4ThreeVector(+(wCFRP/2.-tCFRP/2.),0,+zShift),
		       G4ThreeVector(0,0,M_PI/2.));
  cfrpL->cloneDetector(G4ThreeVector(0,+(wCFRP/2.-tCFRP/2.),+zShift),
		       G4ThreeVector(0,0,M_PI/2.*2.));
  cfrpL->cloneDetector(G4ThreeVector(-(wCFRP/2.-tCFRP/2.),0,+zShift),
		       G4ThreeVector(0,0,M_PI/2.*3.));


  double cfrpS1=cutInCFRP*std::sqrt(2.);
  double cfrpS2=cutOutCFRP*std::sqrt(2.);
  double cfrpSH=(cutInCFRP+tCFRP)*std::sqrt(2.)
    -cutOutCFRP*std::sqrt(2.)/2.
    -cutInCFRP*std::sqrt(2.)/2.;
  double cfrpSD=wCFRP/2.*std::sqrt(2.)-cutOutCFRP/std::sqrt(2.)-cfrpSH/2.;

  std::vector<double> paramCFRPS;
  paramCFRPS.push_back(lCFRP);
  paramCFRPS.push_back(0.);
  paramCFRPS.push_back(0.);
  paramCFRPS.push_back(cfrpSH);
  paramCFRPS.push_back(cfrpS2);
  paramCFRPS.push_back(cfrpS1);
  paramCFRPS.push_back(0.);
  paramCFRPS.push_back(cfrpSH);
  paramCFRPS.push_back(cfrpS2);
  paramCFRPS.push_back(cfrpS1);
  paramCFRPS.push_back(0.);

  GsimTrap* cfrpS = new GsimTrap("cfrpS",this ,
				 G4ThreeVector(+cfrpSD/std::sqrt(2.),-cfrpSD/std::sqrt(2.),+zShift),
				 G4ThreeVector(0,0,M_PI/4.));

  cfrpS->setParameters(paramCFRPS);
  cfrpS->setOuterMaterial("GsimCFRP");
  cfrpS->setOuterColor("gray50");
  addDaughter(cfrpS);
  m_detList.push_back(cfrpS);
  cfrpS->cloneDetector(G4ThreeVector(+cfrpSD/std::sqrt(2.),+cfrpSD/std::sqrt(2.),+zShift),
		       G4ThreeVector(0,0,M_PI/4.+M_PI/2.*1));
  cfrpS->cloneDetector(G4ThreeVector(-cfrpSD/std::sqrt(2.),+cfrpSD/std::sqrt(2.),+zShift),
		       G4ThreeVector(0,0,M_PI/4.+M_PI/2.*2));
  cfrpS->cloneDetector(G4ThreeVector(-cfrpSD/std::sqrt(2.),-cfrpSD/std::sqrt(2.),+zShift),
		       G4ThreeVector(0,0,M_PI/4.+M_PI/2.*3));

  ///////////////////////////////////////////////////////
  //SUS outer base plate
  ///////////////////////////////////////////////////////
  double rotZ=0;
  std::vector<double> param;
  param.clear();
  param.push_back(lFrame);
  param.push_back(0.);
  param.push_back(0.);
  param.push_back(5.*mm);
  param.push_back(123.*mm);
  param.push_back(133.*mm);
  param.push_back(0.);
  param.push_back(5.*mm);
  param.push_back(123.*mm);
  param.push_back(133.*mm);
  param.push_back(0.);
  
  G4ThreeVector pos = G4ThreeVector(0,-hFrame/2+5*mm/2.,+zShift);
  GsimTrap* basePlateL = new GsimTrap("basePlateL",this ,
				      pos,
				      G4ThreeVector(0,0,0.));
  basePlateL->setParameters(param);
  basePlateL->setOuterMaterial("GsimSUS");
  basePlateL->setOuterColor("gray75");
  addDaughter(basePlateL);
  m_detList.push_back(basePlateL);
  for(int i=1;i<8;i++) {
    rotZ=2.*M_PI/16.*2.*i;
    pos.rotateZ(2.*M_PI/16.*2.);
    basePlateL->cloneDetector(pos,
			      G4ThreeVector(0,0,rotZ));
  }

  param.clear();
  param.push_back(133.*mm);
  param.push_back(15.*mm);
  param.push_back(lFrame);

  pos = G4ThreeVector(0,-hFrame/2+5*mm+15*mm/2.,+zShift);
  GsimBox* basePlateL2 = new GsimBox("basePlateL2",this ,
				     pos,
				     G4ThreeVector(0,0,0.));
  basePlateL2->setParameters(param);
  basePlateL2->setOuterMaterial("GsimSUS");
  basePlateL2->setOuterColor("gray75");
  addDaughter(basePlateL2);
  m_detList.push_back(basePlateL2);
  for(int i=1;i<8;i++) {
    rotZ=2.*M_PI/16.*2.*i;
    pos.rotateZ(2.*M_PI/16.*2.);
    basePlateL2->cloneDetector(pos,
			       G4ThreeVector(0,0,rotZ));
  }

  

  param.clear();
  param.push_back(100.*mm);
  param.push_back(12.*mm);
  param.push_back(lFrame);

  
  pos = G4ThreeVector(0,-hFrame/2+12*mm/2.,+zShift);
  rotZ=2.*M_PI/16.;
  pos.rotateZ(rotZ);
  
  GsimBox* basePlateS = new GsimBox("basePlateS",this ,
				    pos,
				    G4ThreeVector(0,0,rotZ));
  basePlateS->setParameters(param);
  basePlateS->setOuterMaterial("GsimSUS");
  basePlateS->setOuterColor("gray75");
  addDaughter(basePlateS);
  m_detList.push_back(basePlateS);
  for(int i=1;i<8;i++) {
    pos.rotateZ(2.*M_PI/16.*2.);
    if(i==1 || i==2 || i==5 || i==6 ) continue;
    rotZ=2.*M_PI/16.+2.*M_PI/16.*2.*i;
    basePlateS->cloneDetector(pos,
			      G4ThreeVector(0,0,rotZ));
  }


  param.clear();
  param.push_back(100.*mm);
  param.push_back(8.5*mm);
  param.push_back(lFrame);
  
  
  pos = G4ThreeVector(0,-hFrame/2+8.5*mm/2.,+zShift);
  rotZ=2.*M_PI/16.*3;
  pos.rotateZ(rotZ);
  
  GsimBox* basePlateSprim = new GsimBox("basePlateSprim",this ,
					pos,
					G4ThreeVector(0,0,rotZ));
  basePlateSprim->setParameters(param);
  basePlateSprim->setOuterMaterial("GsimSUS");
  basePlateSprim->setOuterColor("gray75");
  addDaughter(basePlateSprim);
  m_detList.push_back(basePlateSprim);
  for(int i=2;i<8;i++) {
    pos.rotateZ(2.*M_PI/16.*2.);
    if(i==2 || i==5 || i==6 ) {
      rotZ=2.*M_PI/16.+2.*M_PI/16.*2.*i;
      basePlateSprim->cloneDetector(pos,
				    G4ThreeVector(0,0,rotZ));
    }
  }



  //-----------------------------------------------
  //NCC Crystal Base ( milling 2mm more )

  //------------------
  {
    double h=13.6*mm-2*mm;
    param.clear();
    param.push_back(133.*mm);
    param.push_back(h);
    param.push_back(nccZlength);
    
    pos = G4ThreeVector(0,-hFrame/2+5*mm+15*mm+h/2.,0);
    GsimBox* cBase0 = new GsimBox("cBase0",this ,
				  pos+G4ThreeVector(0,0,+zShift),
				  G4ThreeVector(0,0,0));
    cBase0->setParameters(param);
    cBase0->setOuterMaterial("GsimSUS");
    cBase0->setOuterColor("gray75");
    addDaughter(cBase0);
    m_detList.push_back(cBase0);
    for(int i=1;i<4;i++) {
      rotZ=2*M_PI/4.*i;
      pos.rotateZ(2.*M_PI/4.);
      cBase0->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,0,rotZ));
    }
  }
  //------------------
  {
    param.clear();
    double h=65.6*mm;
    double l1=27.3*mm;
    double l2=l1 -  h*std::tan(2*M_PI/16.);
    double alpha=- std::atan((l1/2.-l2/2.)/h);

    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);

    pos = G4ThreeVector(-100*mm/2.-0.85*mm,
			-hFrame/2+12*mm,0);
    rotZ=2.*M_PI/16;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(+h/2.,+l1-(l1+l2)/4.,0);
    
    GsimTrap* cBase1 = new GsimTrap("cBase1",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,+M_PI/2.));
    cBase1->setParameters(param);
    cBase1->setOuterMaterial("GsimSUS");
    cBase1->setOuterColor("gray75");
    addDaughter(cBase1);
    m_detList.push_back(cBase1);
    {
      pos.rotateX(M_PI);
      cBase1->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,-M_PI/2.));
      
      pos.rotateY(M_PI);
      cBase1->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,0,-M_PI/2.));
      
      pos.rotateX(M_PI);
      cBase1->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,M_PI/2.));
      
    }
    
  }
  //------------------
  {
    param.clear();
    double h=25*mm; 
    double l1=17.9*mm;
    double l2=l1 -  h*std::tan(2*M_PI/16.);
    double alpha=- std::atan((l1/2.-l2/2.)/h);

    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);

    pos = G4ThreeVector(+100*mm/2.-1.7*mm,
			-hFrame/2+12*mm,0);
    rotZ=2.*M_PI/16;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(-h/2.,+l2-(l1+l2)/4.,0);
    
    GsimTrap* cBase2 = new GsimTrap("cBase2",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,+M_PI/2.));
    cBase2->setParameters(param);
    cBase2->setOuterMaterial("GsimSUS");
    cBase2->setOuterColor("gray75");
    addDaughter(cBase2);
    m_detList.push_back(cBase2);
    {
      pos.rotateX(M_PI);
      cBase2->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,-M_PI/2.));
      
      pos.rotateY(M_PI);
      cBase2->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,0,-M_PI/2.));
      
      pos.rotateX(M_PI);
      cBase2->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,M_PI/2.));
      
    }
    
  }
  //------------------
  {
    param.clear();
    double h=20*mm; 
    double l1=26.2*mm;
    double l2=l1 -  h*std::tan(2*M_PI/16.*2.);
    double alpha=- std::atan((l1/2.-l2/2.)/h);

    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    
    pos = G4ThreeVector(-133*mm/2.+36.6*mm,
			-hFrame/2+20*mm,0);
    rotZ=2.*M_PI/16.*2.;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(+h/2.,+l1-(l1+l2)/4.,0);
    GsimTrap* cBase3 = new GsimTrap("cBase3",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,M_PI/2.));
    cBase3->setParameters(param);
    cBase3->setOuterMaterial("GsimSUS");
    cBase3->setOuterColor("gray75");
    addDaughter(cBase3);
    m_detList.push_back(cBase3);

    {
      pos.rotateX(M_PI);
      cBase3->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,-M_PI/2.));
      
      pos.rotateY(M_PI);
      cBase3->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,0,-M_PI/2.));
      
      pos.rotateX(M_PI);
      cBase3->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,M_PI/2.));
      
    }
    
  }




  {
    param.clear();
    double h=22.2*mm;
    double l2=0.1*mm;
    double l1=l2 +  h*std::tan(2*M_PI/16.*2.);
    double alpha=- std::atan((l1/2.-l2/2.)/h);

    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    
    pos = G4ThreeVector(+133*mm/2.-36.6*mm,
			-hFrame/2+20*mm,0);
    rotZ=2.*M_PI/16.*2.;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(-h/2.,+l2-(l1+l2)/4.,0);
    GsimTrap* cBase4 = new GsimTrap("cBase4",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,M_PI/2.));
    cBase4->setParameters(param);
    cBase4->setOuterMaterial("GsimSUS");
    cBase4->setOuterColor("gray75");
    addDaughter(cBase4);
    m_detList.push_back(cBase4);

    {
      pos.rotateX(M_PI);
      cBase4->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,-M_PI/2.));
      
      pos.rotateY(M_PI);
      cBase4->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,0,-M_PI/2.));
      
      pos.rotateX(M_PI);
      cBase4->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,M_PI/2.));
      
    }
    
  }

  
  {
    param.clear();
    double h=25*mm;
    double l1=18*mm;
    double l2=l1 -  h*std::tan(2*M_PI/16.);
    double alpha=  std::atan((l1/2.-l2/2.)/h);
    
    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    
    pos = G4ThreeVector(-100*mm/2.+3.14*mm,
			-hFrame/2+8.5*mm,0);
    rotZ=2.*M_PI/16.*3;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(-l2+(l1+l2)/4.,h/2.,0);
    GsimTrap* cBase5 = new GsimTrap("cBase5",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,0));
    cBase5->setParameters(param);
    cBase5->setOuterMaterial("GsimSUS");
    cBase5->setOuterColor("gray75");
    addDaughter(cBase5);
    m_detList.push_back(cBase5);
    {
      pos.rotateX(M_PI);
      cBase5->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,0));
      
      pos.rotateY(M_PI);
      cBase5->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,M_PI,0));
      
      pos.rotateX(M_PI);
      cBase5->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,M_PI,0));
      
    }
  }
  //------------------
  {
    
    double h=66.6*mm;
    double l1=27.8*mm;
    double l2=0.1*mm;
    double alpha=  std::atan((l1/2.-l2/2.)/h);
    
    param.clear();
    param.push_back(nccZlength);
    param.push_back(0.);
    param.push_back(0.);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    param.push_back(h);
    param.push_back(l2);
    param.push_back(l1);
    param.push_back(alpha);
    
    pos = G4ThreeVector(+100*mm/2.+2.4*mm,
			-hFrame/2+8.5*mm,0);
    rotZ=2.*M_PI/16.*3;
    pos.rotateZ(rotZ);
    pos=pos+G4ThreeVector(-l1+(l1+l2)/4.,-h/2.,0);
    GsimTrap* cBase6 = new GsimTrap("cBase6",this ,
				    pos+G4ThreeVector(0,0,+zShift),
				    G4ThreeVector(0,0,0));
    cBase6->setParameters(param);
    cBase6->setOuterMaterial("GsimSUS");
    cBase6->setOuterColor("gray75");
    addDaughter(cBase6);
    m_detList.push_back(cBase6);
    {
      pos.rotateX(M_PI);
      cBase6->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,0,0));
      
      pos.rotateY(M_PI);
      cBase6->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(M_PI,M_PI,0));
      
      pos.rotateX(M_PI);
      cBase6->cloneDetector(pos+G4ThreeVector(0,0,+zShift),
			    G4ThreeVector(0,M_PI,0));
      
    }
  }
  //------------------

  //
  // Joint 
  //
  {
    double startPhi=2*M_PI/16/2.;
    double Hmax=hFrame/2.;
    double Hmin=284*mm;
    double zPos = lFrame/2.+tJoint/2.;


    param.clear();
    param.push_back(startPhi);
    param.push_back(2.*M_PI);
    param.push_back(16);
    param.push_back(tJoint);
    param.push_back(Hmin);
    param.push_back(Hmax);
    GsimPolyhedra2* joint = new GsimPolyhedra2("joint",this ,
					       G4ThreeVector(0,0,zPos+zShift),
					       G4ThreeVector(0,0,0));
    joint->setParameters(param);
    joint->setOuterMaterial("GsimSUS");
    joint->setOuterColor("gray50");
    addDaughter(joint);
    m_detMoveList.push_back(joint);
    
    GsimPolyhedra2* joint2 = new GsimPolyhedra2("joint2",this ,
						G4ThreeVector(0,0,-zPos+zShift),
						G4ThreeVector(0,0,0));
    joint2->setParameters(param);
    joint2->setOuterMaterial("GsimSUS");
    joint2->setOuterColor("gray50");
    addDaughter(joint2);
    m_detMoveList.push_back(joint2);
  }

  // PMT support

  {
    double startPhi=2*M_PI/16/2.;
    double Hmax=hFrame/2.;
    double Hmin=284*mm;
    double zPos = -lFrame/2.-tJoint-lPMTSupport-tJoint2/2.;
    

    param.clear();
    param.push_back(startPhi);
    param.push_back(2.*M_PI);
    param.push_back(16);
    param.push_back(tJoint2);
    param.push_back(Hmin);
    param.push_back(Hmax);
    GsimPolyhedra2* joint3 = new GsimPolyhedra2("joint3",this ,
						G4ThreeVector(0,0,zPos+zShift),
						G4ThreeVector(0,0,0));
    joint3->setParameters(param);
    joint3->setOuterMaterial("G4_Al");
    joint3->setOuterColor("gray50");
    addDaughter(joint3);
    m_detMoveList.push_back(joint3);
  }


  GsimDetector* pmtSupport = new GsimDetector("pmtSupport",this ,
					      G4ThreeVector(0,0,0),
					      G4ThreeVector(0,0,0));
  addDaughter(pmtSupport);
  m_detMoveList.push_back(pmtSupport);
  
  
  {
    //PMT support
    double h=4*mm;
    double l1=80*mm-h*2;
    param.clear();
    param.push_back(l1);
    param.push_back(h);
    param.push_back(lPMTSupport);
    double zPos = -lFrame/2.-tJoint-lPMTSupport/2.;
    pos = G4ThreeVector(0,
			-hFrame/2+40*mm-h/2.,zPos+zShift);
    GsimBox* pmtSupport0 = 0;
    for(int k=0;k<16;k++) {
      if(pmtSupport0==0) {
	pmtSupport0 = new GsimBox("pmtSupport0",pmtSupport ,
				  pos,
				  G4ThreeVector(0,0,2.*M_PI/16.*k));
	pmtSupport0->setParameters(param);
	pmtSupport0->setOuterMaterial("G4_Al");
	pmtSupport0->setOuterColor("gray50");
	pmtSupport->addDaughter(pmtSupport0);
      } else {
	pmtSupport0->cloneDetector(pos,
				   G4ThreeVector(0,0,2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }
  }
    

  {
    double h=4*mm;
    double l1=40*mm;
    
    param.clear();
    param.push_back(h);
    param.push_back(l1);
    param.push_back(lPMTSupport);

    double zPos = -lFrame/2.-tJoint-lPMTSupport/2.;
    
    pos = G4ThreeVector(+80*mm/2.-h/2.,
			-hFrame/2+l1/2.,zPos+zShift);

    GsimBox* pmtSupport1 = 0;
    for(int k=0;k<16;k++) {
      if(pmtSupport1 == 0) {
	pmtSupport1 = new GsimBox("pmtSupport1",pmtSupport ,
				  pos,
				  G4ThreeVector(0,0,2.*M_PI/16.*k));
	pmtSupport1->setParameters(param);
	pmtSupport1->setOuterMaterial("G4_Al");
	pmtSupport1->setOuterColor("gray50");
	pmtSupport->addDaughter(pmtSupport1);
      } else {
	pmtSupport1->cloneDetector(pos,
				   G4ThreeVector(0,0,2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }
    pos = G4ThreeVector(-80*mm/2.+h/2.,
			-hFrame/2+l1/2.,zPos+zShift);
    
    for(int k=0;k<16;k++) {
      pmtSupport1->cloneDetector(pos,
				 G4ThreeVector(0,0,2.*M_PI/16.*k));
      pos.rotateZ(2.*M_PI/16.);
    }
  }


  {
    //IPMT BOARD
    //3t (x=0 - 3mm)
    //h=140mm,start from y=-274*mm=-hFrame/2.+40*mm;
    //l=127*mm start from 98 mm upstream of PMT frame downstream end
    param.clear();
    param.push_back(3*mm);
    param.push_back(140*mm);
    param.push_back(127*mm);

    pos=G4ThreeVector(+3/2.*mm,
		      -hFrame/2.+40*mm+140/2.*mm,
		      -lFrame/2.-tJoint-98*mm-127/2.*mm+zShift);
    
    GsimBox* iBoard = 0;
    for(int k=0;k<16;k++) {

      if(iBoard == 0) {
	iBoard = new GsimBox("iBoard",pmtSupport ,
			     pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
	iBoard->setParameters(param);
	iBoard->setOuterMaterial("G4_Al");
	iBoard->setOuterColor("gray50");
	pmtSupport->addDaughter(iBoard);
      } else {
	iBoard->cloneDetector(pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }
  }
  

  {
    //CPMT BOARD
    //3t (x=0 - 3mm)
    //h=142mm,start from y=-274*mm=-hFrame/2.+40*mm;
    //l=230*mm start from 338 mm upstream of PMT frame downstream end
    
    param.clear();
    param.push_back(3*mm);
    param.push_back(142*mm);
    param.push_back(230*mm);

    pos=G4ThreeVector(+3/2.*mm,
		      -hFrame/2.+40*mm+142/2.*mm,
		      -lFrame/2.-tJoint-338*mm-230/2.*mm+zShift);
    
    GsimBox* cBoard = 0;
    for(int k=0;k<16;k++) {
      if(cBoard == 0) {
	cBoard = new GsimBox("cBoard",pmtSupport ,
			     pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
	cBoard->setParameters(param);
	cBoard->setOuterMaterial("G4_Al");
	cBoard->setOuterColor("gray50");
	pmtSupport->addDaughter(cBoard);
      } else {
	cBoard->cloneDetector(pos,
			      G4ThreeVector(0,0,2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }
  }

  {
    //CPMT BOX
    //x=37 +3 - +40
    //y=3  start from y=-hFrame/2.+40*mm+5*mm;
    //l=(176+30)*mm start from 348 mm upstream of PMT frame downstream end

    param.clear();
    param.push_back(37*mm);
    param.push_back(3*mm);
    param.push_back(206*mm);

    
    GsimBox* cBox = 0;

    //Bottom
    pos=G4ThreeVector((+3+37/2.)*mm,
		      -hFrame/2.+40*mm+(5+3/2.)*mm,
		      -lFrame/2.-tJoint-348*mm-206/2.*mm+zShift);
    for(int iy=0;iy<3;iy++) {

      if(iy==1) {
	pos=pos+G4ThreeVector(-43*mm,45*mm,0);
      }
      if(iy==2) {
	pos=pos+G4ThreeVector(+43*mm,52*mm,0);
      }
      for(int k=0;k<16;k++) {
	if(cBox == 0) {
	  cBox = new GsimBox("cBox",pmtSupport ,
			     pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
	  cBox->setParameters(param);
	  cBox->setOuterMaterial("G4_Al");
	  cBox->setOuterColor("gray50");
	  pmtSupport->addDaughter(cBox);
	} else {
	  cBox->cloneDetector(pos,
			      G4ThreeVector(0,0,2.*M_PI/16.*k));
	}
	pos.rotateZ(2.*M_PI/16.);
      }
    }


    //Top
    pos=G4ThreeVector((+6+37/2.)*mm,
		      -hFrame/2.+40*mm+(5+37+3/2.)*mm,
		      -lFrame/2.-tJoint-348*mm-206/2.*mm+zShift);
    for(int iy=0;iy<3;iy++) {
      
      if(iy==1) {
	pos=pos+G4ThreeVector(-43*mm,45*mm,0);
      }
      if(iy==2) {
	pos=pos+G4ThreeVector(+43*mm,52*mm,0);
      }
      for(int k=0;k<16;k++) {
	cBox->cloneDetector(pos,
			    G4ThreeVector(0,0,2.*M_PI/16.*k));
	pos.rotateZ(2.*M_PI/16.);
      }
    }


    //Left
    pos=G4ThreeVector((+3+3/2.)*mm,
		      -hFrame/2.+40*mm+(5+3+37/2.)*mm,
		      -lFrame/2.-tJoint-348*mm-206/2.*mm+zShift);
    for(int iy=0;iy<3;iy++) {
      
      if(iy==1) {
	pos=pos+G4ThreeVector(-43*mm,45*mm,0);
      }
      if(iy==2) {
	pos=pos+G4ThreeVector(+43*mm,52*mm,0);
      }
      for(int k=0;k<16;k++) {
	cBox->cloneDetector(pos,
			    G4ThreeVector(0,0,M_PI/2.+2.*M_PI/16.*k));
	pos.rotateZ(2.*M_PI/16.);
      }
    }

    //Right
    pos=G4ThreeVector((+3+37+3/2.)*mm,
		      -hFrame/2.+40*mm+(5+37/2.)*mm,
		      -lFrame/2.-tJoint-348*mm-206/2.*mm+zShift);
    for(int iy=0;iy<3;iy++) {
      
      if(iy==1) {
	pos=pos+G4ThreeVector(-43*mm,45*mm,0);
      }
      if(iy==2) {
	pos=pos+G4ThreeVector(+43*mm,52*mm,0);
      }
      for(int k=0;k<16;k++) {
	cBox->cloneDetector(pos,
			    G4ThreeVector(0,0,M_PI/2.+2.*M_PI/16.*k));
	pos.rotateZ(2.*M_PI/16.);
      }
    }


    //FOR HINEMOS
    G4double eps=-1e-3*mm;//to avoid geometry overlap test 
    //Bottom
    pos=G4ThreeVector((-40.+37./2.)*mm+eps,
		      -hFrame/2.+40*mm+(97.5+3/2.)*mm,
		      -lFrame/2.-tJoint-71*mm-206/2.*mm+zShift);
    for(int k=0;k<4;k++) {
      cBox->cloneDetector(pos,
			  G4ThreeVector(0,0,2.*M_PI/4.*k));
      pos.rotateZ(2.*M_PI/4.);
    }

    //Top 
    pos=G4ThreeVector((-40.+3.+37./2.)*mm+eps,
    		      -hFrame/2.+40*mm+(97.5+37+3/2.)*mm,
    		      -lFrame/2.-tJoint-71*mm-206/2.*mm+zShift);
    for(int k=0;k<4;k++) {
      cBox->cloneDetector(pos,
    			  G4ThreeVector(0,0,2.*M_PI/4.*k));
      pos.rotateZ(2.*M_PI/4.);
    }

    //Left
    pos=G4ThreeVector((-40.+3./2.)*mm+eps,
    		      -hFrame/2.+40*mm+(97.5+3+37/2.)*mm,
    		      -lFrame/2.-tJoint-71*mm-206/2.*mm+zShift);
    for(int k=0;k<4;k++) {
      cBox->cloneDetector(pos,
    			  G4ThreeVector(0,0,M_PI/2.+2.*M_PI/4.*k));
      pos.rotateZ(2.*M_PI/4.);
    }

    //Right
    pos=G4ThreeVector((-40.+37.+3./2.)*mm+eps,
    		      -hFrame/2.+40*mm+(97.5+37/2.)*mm,
    		      -lFrame/2.-tJoint-71*mm-206/2.*mm+zShift);
    for(int k=0;k<4;k++) {
      cBox->cloneDetector(pos,
    			  G4ThreeVector(0,0,M_PI/2.+2.*M_PI/4.*k));
      pos.rotateZ(2.*M_PI/4.);
    }
    
  }
  
  {
    //IPMT BOX
    //x=37 -40 - -3
    //y=3  start from y=-hFrame/2.+40*mm+57*mm;
    //l=(96+17)*mm start from 98 mm upstream of PMT frame downstream end

    param.clear();
    param.push_back(37*mm);
    param.push_back(3*mm);
    param.push_back(113*mm);

    GsimBox* iBox = 0;

    //Bottom
    pos=G4ThreeVector((-40.+37./2.)*mm,
		      -hFrame/2.+40.*mm+(57.+3./2.)*mm,
		      -lFrame/2.-tJoint-98*mm-113/2.*mm+zShift);
    
    for(int k=0;k<16;k++) {
      if(k==0 || k==4 || k==8 || k==12) {
      } else {
	if(iBox == 0) {
	  iBox = new GsimBox("iBox",pmtSupport ,
			     pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
	  iBox->setParameters(param);
	  iBox->setOuterMaterial("G4_Al");
	  iBox->setOuterColor("gray50");
	  pmtSupport->addDaughter(iBox);
	} else {
	  iBox->cloneDetector(pos,
			      G4ThreeVector(0,0,2.*M_PI/16.*k));
	}
      }
      pos.rotateZ(2.*M_PI/16.);
    }

    //Top
    pos=G4ThreeVector((-40.+3+37./2.)*mm,
		      -hFrame/2.+40*mm+(57.+37.+3./2.)*mm,
		      -lFrame/2.-tJoint-98*mm-113/2.*mm+zShift);
    for(int k=0;k<16;k++) {
      if(k==0 || k==4 || k==8 || k==12) {
	// do nothing
      } else {
	iBox->cloneDetector(pos,
			    G4ThreeVector(0,0,2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }

    //Left
    pos=G4ThreeVector((-40.+3./2.)*mm,
		      -hFrame/2.+40*mm+(57.+3.+37./2.)*mm,
		      -lFrame/2.-tJoint-98*mm-113/2.*mm+zShift);
    for(int k=0;k<16;k++) {
      if(k==0 || k==4 || k==8 || k==12) {
	//do nothing
      } else {
	iBox->cloneDetector(pos,
			    G4ThreeVector(0,0,M_PI/2.+2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }

    //Right
    pos=G4ThreeVector((-40.+37.+3./2.)*mm,
		      -hFrame/2.+40*mm+(57.+37./2.)*mm,
		      -lFrame/2.-tJoint-98*mm-113/2.*mm+zShift);
    for(int k=0;k<16;k++) {
      if(k==0 || k==4 || k==8 || k==12) {
	//do nothing
      } else {
	iBox->cloneDetector(pos,
			    G4ThreeVector(0,0,M_PI/2.+2.*M_PI/16.*k));
      }
      pos.rotateZ(2.*M_PI/16.);
    }
  }
  
  {
    //HINEMOS BP
    //x=50 -50 - 0
    //y=5  start from y=-hFrame/2.+40*mm+92.5*mm;
    //l=230*mm start from 61 mm upstream of PMT frame downstream end

    param.clear();
    param.push_back(50*mm);
    param.push_back(5*mm);
    param.push_back(230*mm);

    GsimBox* hBP = 0;
    pos=G4ThreeVector((-50.+50./2.)*mm,
		      -hFrame/2.+40.*mm+(92.5+5./2.)*mm,
		      -lFrame/2.-tJoint-61*mm-230/2.*mm+zShift);
    for(int k=0;k<4;k++) {
      if(hBP == 0) {
	hBP = new GsimBox("hBP",pmtSupport ,
			  pos,
			  G4ThreeVector(0,0,2.*M_PI/4.*k));
	hBP->setParameters(param);
	hBP->setOuterMaterial("G4_Al");
	hBP->setOuterColor("gray50");
	pmtSupport->addDaughter(hBP);
      } else {
	hBP->cloneDetector(pos,
			   G4ThreeVector(0,0,2.*M_PI/4.*k));
      }
      pos.rotateZ(2.*M_PI/4.);
    }
  }

  {
    //IBP
    //x=50 -50 - 0
    //y=5  start from y=-hFrame/2.+40*mm+52*mm;
    //l=127*mm start from 98 mm upstream of PMT frame downstream end

    param.clear();
    param.push_back(50*mm);
    param.push_back(5*mm);
    param.push_back(127*mm);

    GsimBox* iBP = 0;
    pos=G4ThreeVector((-50.+50./2.)*mm,
		      -hFrame/2.+40.*mm+(52+5./2.)*mm,
		      -lFrame/2.-tJoint-98*mm-127/2.*mm+zShift);
    for(int k=0;k<16;k++) {
      if(k==0 || k==4 || k==8 || k==12) {
	//do nothing
      } else {
	if(iBP == 0) {
	  iBP = new GsimBox("iBP",pmtSupport ,
			    pos,
			    G4ThreeVector(0,0,2.*M_PI/16.*k));
	  iBP->setParameters(param);
	  iBP->setOuterMaterial("G4_Al");
	  iBP->setOuterColor("gray50");
	  pmtSupport->addDaughter(iBP);
	} else {
	  iBP->cloneDetector(pos,
			     G4ThreeVector(0,0,2.*M_PI/16.*k));
	}
      }
      pos.rotateZ(2.*M_PI/16.);
    }
  }

  //HINEMOS(NCC Liner)
  //
  //            133.3
  //     , ----------------------------------
  //   ,   `    H=1.9mm (L=3mm)            ,  ' 
  //  ,    , ----------------------------  2.6 ' 4mm(L=3mm)
  // ,    ,         129.5                 '     '
  //  `  ,       H=3mm(L=449mm)            '  ,   
  //   `------------------------------------'
  //             135.5
  //
  //
  //
  //


  double linerT=3.*mm;
  double linerAddL=3*mm;
  double linerL=lCFRP+linerAddL;
  double linerW2=129.5*mm;
  double linerW1=linerW2+linerT*2;
  
  

  std::vector<double> paramLiner;
  paramLiner.push_back(linerL);
  paramLiner.push_back(0.);
  paramLiner.push_back(0.);
  paramLiner.push_back(linerT);
  paramLiner.push_back(linerW1);
  paramLiner.push_back(linerW2);
  paramLiner.push_back(0);
  paramLiner.push_back(linerT);
  paramLiner.push_back(linerW1);
  paramLiner.push_back(linerW2);
  paramLiner.push_back(0);
  
  GsimTrap* liner = new GsimTrap("liner",this ,
				 G4ThreeVector(0,(wCFRP/2.-tCFRP-linerT/2.),-linerAddL/2.+zShift),
				 G4ThreeVector(0,0,0));
  liner->setParameters(paramLiner);
  liner->setOuterColor("yellow");
  liner->setSensitiveDetector("NCC",600);
  addDaughter(liner);
  liner->cloneDetector(G4ThreeVector((wCFRP/2.-tCFRP-linerT/2.),0,-linerAddL/2.+zShift),
		       G4ThreeVector(0,0,-90*deg),630);
  liner->cloneDetector(G4ThreeVector(0,-(wCFRP/2.-tCFRP-linerT/2.),-linerAddL/2.+zShift),
		       G4ThreeVector(0,0,-90*2*deg),620);
  liner->cloneDetector(G4ThreeVector(-(wCFRP/2.-tCFRP-linerT/2.),0,-linerAddL/2.+zShift),
 		       G4ThreeVector(0,0,-90*3*deg),610);


  double linerAddT=1.9*mm;
  double linerAddW1=linerW2;
  double linerAddW2=133.3*mm;
  
    
  std::vector<double> paramLinerAdd;
  paramLinerAdd.push_back(linerAddL);
  paramLinerAdd.push_back(0.);
  paramLinerAdd.push_back(0.);
  paramLinerAdd.push_back(linerAddT);
  paramLinerAdd.push_back(linerAddW1);
  paramLinerAdd.push_back(linerAddW2);
  paramLinerAdd.push_back(0);
  paramLinerAdd.push_back(linerAddT);
  paramLinerAdd.push_back(linerAddW1);
  paramLinerAdd.push_back(linerAddW2);
  paramLinerAdd.push_back(0);


  double zLinerAdd=-lCFRP/2.-linerAddL/2.+zShift;
  GsimTrap* linerAdd = new GsimTrap("linerAdd",this ,
				    G4ThreeVector(0,(wCFRP/2.-tCFRP+linerAddT/2.),zLinerAdd),
				    G4ThreeVector(0,0,0));
  linerAdd->setParameters(paramLinerAdd);
  linerAdd->setOuterColor("yellow");
  linerAdd->setSensitiveDetector("NCC",600);
  addDaughter(linerAdd);
  
  linerAdd->cloneDetector(G4ThreeVector((wCFRP/2.-tCFRP+linerAddT/2.),0,zLinerAdd),
			  G4ThreeVector(0,0,-90*deg),630);
  linerAdd->cloneDetector(G4ThreeVector(0,-(wCFRP/2.-tCFRP+linerAddT/2.),zLinerAdd),
			  G4ThreeVector(0,0,-90*2*deg),620);
  linerAdd->cloneDetector(G4ThreeVector(-(wCFRP/2.-tCFRP+linerAddT/2.),0,zLinerAdd),
 			  G4ThreeVector(0,0,-90*3*deg),610);

  double linerSideL=linerAddL;
  double linerSideT=2.6*mm;
  double linerSideW=4*mm;

  std::vector<double> paramLinerSide;
  paramLinerSide.push_back(linerSideW);
  paramLinerSide.push_back(linerSideT);
  paramLinerSide.push_back(linerSideL);

  double xLinerSide=linerW2/2.     +linerSideT/2./std::sqrt(2)+linerSideW/2./std::sqrt(2);
  double yLinerSide=wCFRP/2.-tCFRP +linerSideT/2./std::sqrt(2)-linerSideW/2./std::sqrt(2);
  GsimBox* linerSide = new GsimBox("linerSide",this ,
				   G4ThreeVector(xLinerSide,yLinerSide,zLinerAdd),
				   G4ThreeVector(0,0,-45*deg));
  linerSide->setParameters(paramLinerSide);
  linerSide->setOuterColor("yellow");
  linerSide->setSensitiveDetector("NCC",600);
  addDaughter(linerSide);
  linerSide->cloneDetector(G4ThreeVector(-xLinerSide,yLinerSide,zLinerAdd),
			   G4ThreeVector(0,0,45*deg),600);


  linerSide->cloneDetector(G4ThreeVector(yLinerSide,-xLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,-45*deg-90*deg),630);


  linerSide->cloneDetector(G4ThreeVector(yLinerSide,+xLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,45*deg-90*deg),630);



  linerSide->cloneDetector(G4ThreeVector(-xLinerSide,-yLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,-45*deg-90*2*deg),620);


  linerSide->cloneDetector(G4ThreeVector(xLinerSide,-yLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,45*deg-90*2*deg),620);


  
  linerSide->cloneDetector(G4ThreeVector(-yLinerSide,+xLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,-45*deg-90*3*deg),610);


  linerSide->cloneDetector(G4ThreeVector(-yLinerSide,-xLinerSide,zLinerAdd),
 			   G4ThreeVector(0,0,45*deg-90*3*deg),610);
  
  
  if(isFiber) {
    GsimSensitiveDetector* sd =
      dynamic_cast<GsimSensitiveDetector*>(GsimUtil::getUtil()->getSensitiveDetector("NCCfiber"));
    sd->setDeepCopyNoFactor(10);
    sd->setThisCopyNoFactor(1);
  }
  

  setThisAndDaughterBriefName("NCC");
}

void GsimE14NCC::comment()
{
  double nccZlength=
    0.3+ 148 + 0.5+
    0.5+ 200 + 0.5+
    0.5+  96 + 0.3;
  
  std::cout << "GsimE14NCC Z position" << std::endl;
  std::cout << "    The center of NCC module." << std::endl;
  std::cout << "    Module length  : " << nccZlength/mm << " mm long."  << std::endl;
  std::cout << "    Tefron         : 0.3 mm thick."  << std::endl;
  std::cout << "    Acrylic        : 2.3 mm thick."  << std::endl;
  
  std::cout << "GsimE14NCC userFlag:" << std::endl;
  std::cout << "    0: with WLSFibers" << std::endl;
  std::cout << "       Edep in WLSfibers -> 1xxx for scintillation of PMP." << std::endl;
  std::cout << "           rear   :  outer 4 fibers" << std::endl;
  std::cout << "           middel :  next inner 4 fibers" << std::endl;
  std::cout << "           front  :  next inner 4 fibers" << std::endl;
  std::cout << "           common :  central 28 fibers" << std::endl;
  std::cout << "    1: without WLSFibers" << std::endl;
}

GsimE14NCC::~GsimE14NCC()
{
  ;
}


void GsimE14NCC::setFastSimulationLevel(int level)
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
      pos.setZ(z+29*m);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(true);
    }
  }
  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("NCC",1000);
    }
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

bool GsimE14NCC::endOfEventAction(const G4Event* )
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
