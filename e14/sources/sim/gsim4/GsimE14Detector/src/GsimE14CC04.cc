/**
 *  @file
 *  @brief  GsimE14CC04
 *  $Id$ 
 *  $Log$
 */
#include "GsimE14Detector/GsimE14CC04.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14CC04::GsimE14CC04(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14CC04";
  // CC04
  char nam[250];

  if(m_userFlag==2011) {
    m_model=1;
  } else if(m_userFlag==201212 ||
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
    m_model=3;
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. CC04 is not constructed.");
    return;
  }

  if(m_model==3) {
    //Shiomi
    double csi_size = 70.0; // Mean of CsI crystal size y,z (mm)
    double csi_step = 1.5;   // Step of CsI crystal (mm)

    double cc04_e391_1xy=     csi_size;
    double cc04_e391_1length= 200; // mm
    double cc04_e391_2xy=     csi_size-2*csi_step;
    double cc04_e391_2length= 100; // mm


    double perm_thickness = 1.0;     // Permalloy
    double perm_innerXY   = csi_size-2*csi_step;
    double perm_outerXY   = perm_innerXY+perm_thickness*2;
    double perm_length    = 100; // mm

    double acryl_xy        = 69.5; // mm
    double acryl_thickness = 2.0; // mm

    double carbon_xy        = 69.5; // mm
    double carbon_thickness = 1.0; // mm
    

    double cc04_len  = Csic_Len+acryl_thickness/10.;
    double cc04_len2  = Csic_Len+carbon_thickness/10.;
    //  double cc04_len2[2][2][4]  = {{{24.0, 24.3, 25.0, 25.6},{24.0, 24.0, 25.7, 26.0}},{{24.0, 24.4, 26.0, 26.0},{24.0, 25.0, 26.0, 26.0}}}; //cm
    double cc04_width = Csic_XY;
    double cc04_width2 = 5.0; //cm
    double cc04_sci_thk = 1.0;//cm
    double cc04_sci_len = 25.0;//cm
    double cc04_sci_len2 = 60.0;//cm
    double cc04_sci_width = 18.2;//cm
    double cc04_sci_width2 = 21.0;//cm
    //double cc04_len2 = 25.0;

  
    double c_cc04_x_offset[4] = {1.5, -1.5, 0, 0};
    double c_cc04_y_offset[4] = {0, 0, 0, 0};

  
    double c_cc04_gap = Csic_Gap;
    double c_cc04_y0 = -(cc04_width2+cc04_width*3.0+c_cc04_gap*4);
    double c_cc04_z0 = -(cc04_width+c_cc04_gap)*1;
    double c_cc04_z02 = -(cc04_width*1.5+c_cc04_gap-cc04_width2*0.5)*1;

    double c_cc04_x[2] = { -(cc04_len+c_cc04_gap)*0.5,
			   (cc04_len+c_cc04_gap)*0.5};
    double c_cc04_x2[2] = { -(cc04_len2+c_cc04_gap)*0.5,
			    (cc04_len2+c_cc04_gap)*0.5};

    double c_cc04_sci_z0 =
      -(cc04_width+c_cc04_gap)*1.5-2.5-1.5;
    if(m_userFlag>=201902) {
      // Distance between upstream surface of cc04 Crystal and downstream surface of
      // cc04 Scintillator is 4.85cm.
      // 0.5 cm is thickness of cc04 scintillator
      c_cc04_sci_z0 =
	-(cc04_width+c_cc04_gap)*1.5-4.85-0.5; 
    }
    
    if(m_userFlag>=201902) {
      //Shomi added
      double spacer_xlength=5.0; //cm
      double spacer_ylength=0.05; //cm
      double spacer_zlength=20; //cm
      
      double spacer_z0=0;
      double spacer_y0 =-(cc04_width2+cc04_width*2.5+c_cc04_gap*3.5);
      double spacer_x0[2] ={ -(c_cc04_gap*0.5+cc04_len-spacer_xlength/2.),
			     (c_cc04_gap*0.5+cc04_len-spacer_xlength/2.)};
      
      
      // Spacer between CC04 crystals //
      
      GsimBox* boxSpacer     =0;
      
      
      int spacernum=0;
      for(int iy=0; iy<4; iy++){
	for(int ix=0; ix<2; ix++){
	  
	  if(ix==0){
	    if(0<iy&&iy<2) spacer_y0=spacer_y0+cc04_width+c_cc04_gap;
	    else if(iy==2) spacer_y0=spacer_y0+(cc04_width2*2+cc04_width*3+c_cc04_gap*5);
	    else if(iy>2) spacer_y0=spacer_y0+cc04_width+c_cc04_gap;
	  }
	  
	  
	  if(boxSpacer==0){
	    std::vector<double> paramVec;
	    paramVec.push_back(spacer_xlength*cm);
	    paramVec.push_back(spacer_ylength*cm);
	    paramVec.push_back(spacer_zlength*cm);
	    
	    
	    boxSpacer = new GsimBox("CC04Spacer",this,
				    G4ThreeVector(spacer_x0[ix]*cm,spacer_y0*cm,spacer_z0*cm),
				    G4ThreeVector(0,0,0));
	    boxSpacer->setOuterMaterial("G4_PLEXIGLASS");
	    boxSpacer->setParameters(paramVec);
	    boxSpacer->setOuterColor("yellow");
	    addDaughter(boxSpacer);
	  }
	  else {
	    boxSpacer->cloneDetector(G4ThreeVector(spacer_x0[ix]*cm,spacer_y0*cm,spacer_z0*cm),
				     G4ThreeVector(0,0,0),spacernum);
	  }
	  spacernum+=1;            
	}
      }
      
      ///////////////////////////////////
      
      // Spacer in CC04 beam hole
      
      
      GsimBox* BHSpacer=0;
      {
	
	std::vector<double> paramVec;
	paramVec.push_back(0.1*cm);
	paramVec.push_back(16*cm);
	paramVec.push_back(20*cm);
	
	BHSpacer = new GsimBox("CC04BHSpacer",this,
			       G4ThreeVector((8.495*cm-0.05*cm),0,0),
			       G4ThreeVector(0,0,0));      
	BHSpacer->setOuterMaterial("G4_PLEXIGLASS");
	BHSpacer->setParameters(paramVec);
	BHSpacer->setOuterColor("yellow");
	addDaughter(BHSpacer);
	
	BHSpacer->cloneDetector(G4ThreeVector(-(8.495*cm-0.05*cm),0,0),
				G4ThreeVector(0,0,0),1);      
	
      }
      
      ////////////////////////////////////
      
      
      // CFRP in front of and behind CC04 crystals
      
      double CFRP_thickness=0.1; //cm
      double c_cc04_Front_CFRP_z0=-(cc04_width*1.5+c_cc04_gap)-CFRP_thickness/2.;
      double c_cc04_Back_CFRP_z0=(cc04_width*1.5+c_cc04_gap)+CFRP_thickness/2.;
      
      GsimBoxWithoutABox* FrontCFRP=0;
      GsimBoxWithoutABox* BackCFRP=0;
      
      
      double CFRPInnerHole=22;  //cm
      double CFRPXLength=77;    //
      double CFRPYLength=59;    //
      {
	std::vector<double> paramVec;    
	paramVec.push_back(CFRPXLength*cm);
	paramVec.push_back(CFRPYLength*cm);
	paramVec.push_back(CFRP_thickness*cm);
	paramVec.push_back(CFRPInnerHole*cm);
	paramVec.push_back(CFRPInnerHole*cm);
	paramVec.push_back(0);
	paramVec.push_back(0);
	paramVec.push_back(0);
	
	FrontCFRP = new GsimBoxWithoutABox("CC04_FrontCFRP",this,
					   G4ThreeVector(0*cm,0*cm,c_cc04_Front_CFRP_z0*cm),
					   G4ThreeVector(0,0,0));
	FrontCFRP->setOuterMaterial("GsimCFRP");
	FrontCFRP->setOuterColor("gray70");
	FrontCFRP->setParameters(paramVec);
	addDaughter(FrontCFRP);
	m_detList.push_back(FrontCFRP);
	
	BackCFRP = new GsimBoxWithoutABox("CC04_BackCFRP",this,
					  G4ThreeVector(0*cm,0*cm,c_cc04_Back_CFRP_z0*cm),
					  G4ThreeVector(0,0,0));
	BackCFRP->setOuterMaterial("GsimCFRP");
	BackCFRP->setOuterColor("gray70");
	BackCFRP->setParameters(paramVec);
	addDaughter(BackCFRP);
	m_detList.push_back(BackCFRP);
	
      }
      //end of if m_userFlag==201902
    }
    
    int cnt=0;
    GsimBox* boxE391_1 =0;
    GsimBox* boxE391_2 =0;
    GsimBoxWithoutABox* boxPermalloy =0;
    GsimBox* boxPlate     =0;
    GsimBox* boxCarbon     =0;
    
    //char nam[250] = "";
    char nam2[250] = "";
    char nam3[250] = "";
    char nam4[999] = "";
    char nam5[999] = "";
    
    for(int iz=0;iz<3;iz++) {
      for(int iy=0;iy<7;iy++) {
	for(int ix=0;ix<2;ix++) {
	  //for(int iz=0;iz<1;iz++) {
	  //for(int iy=0;iy<1;iy++) {
	  //for(int ix=0;ix<1;ix++) {

	  double z = c_cc04_z0+(cc04_width+c_cc04_gap)*iz;
	  double y;
	  if(iy<3){
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*iy+c_cc04_y_offset[iz];
	  } else if(iy==3 || iy==4){
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*2+
	      (cc04_width+cc04_width2+c_cc04_gap*2)*(iy-2)+c_cc04_y_offset[iz];
	  } else {
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*(iy-2)+
	      (cc04_width+cc04_width2+c_cc04_gap*2)*2+c_cc04_y_offset[iz];
	  }
	  //double x1=c_cc04_x[ix]+c_cc04_x_offset[iz]+(2*ix-1)*0.35+5*pow(-1,ix);
	  //double x2=c_cc04_x[ix]+c_cc04_x_offset[iz]+(2*ix-1)*0.35-10*pow(-1,ix);
	  //double xPlate=c_cc04_x[ix]+c_cc04_x_offset[iz]+(2*ix-1)*0.35+(15+0.1)*pow(-1,ix);;

	  double x1=c_cc04_x[ix]+c_cc04_x_offset[iz]+4.9*pow(-1,ix);
	  double x2=c_cc04_x[ix]+c_cc04_x_offset[iz]-10.1*pow(-1,ix);
	  double xPlate=c_cc04_x[ix]+c_cc04_x_offset[iz]+15*pow(-1,ix);;
	  if(iy==3) {
	    if(ix==0){
	      x1=c_cc04_x[ix]-8.5+4.95*pow(-1,ix);
	      x2=c_cc04_x[ix]-8.5-10.05*pow(-1,ix);
	      xPlate=c_cc04_x2[ix]-8.5+15*pow(-1,ix);;
	    }
	    else{
	      x1=c_cc04_x[ix]+8.5+4.9*pow(-1,ix);
	      x2=c_cc04_x[ix]+8.5-10.1*pow(-1,ix);
	      xPlate=c_cc04_x2[ix]+8.5+15*pow(-1,ix);;
	    }	      
	  }

	  //std::cout<<x1<<" "<<y<<" "<<z<<std::endl;
	  //std::cout<<x2<<" "<<y<<" "<<z<<std::endl;
	  //std::cout<<cnt<<" "<<xPlate<<" "<<y<<" "<<z<<std::endl;
	  
	  sprintf(nam,"CC04E391CSI1");
	  sprintf(nam2,"CC04E391CSI2");
	  sprintf(nam3,"CC04_Permalloy");
	  sprintf(nam4,"CC04_AcrylPlate");
	  sprintf(nam5,"CC04_CFRP");

	  if(boxE391_1==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_e391_1length*mm);
	    paramVec.push_back(cc04_e391_1xy*mm);
	    paramVec.push_back(cc04_e391_1xy*mm);
	    
	  
	    boxE391_1 = new GsimBox(std::string(nam),this,
				    G4ThreeVector(x1*cm,y*cm,z*cm),
				    G4ThreeVector(0,0,0));
	    boxE391_1->setOuterMaterial("G4_CESIUM_IODIDE");
	    boxE391_1->setParameters(paramVec);
	    boxE391_1->setSensitiveDetector("CC04",cnt);
	    addDaughter(boxE391_1);
	  } else {
	    boxE391_1->cloneDetector(G4ThreeVector(x1*cm,y*cm,z*cm),
				     G4ThreeVector(0,0,0),cnt);
	  }
	  if(boxE391_2==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_e391_2length*mm);
	    paramVec.push_back(cc04_e391_2xy*mm);
	    paramVec.push_back(cc04_e391_2xy*mm);
	    
	  
	    boxE391_2 = new GsimBox(std::string(nam2),this,
				    G4ThreeVector(x2*cm,y*cm,z*cm),
				    G4ThreeVector(0,0,0));
	    boxE391_2->setOuterMaterial("G4_CESIUM_IODIDE");
	    boxE391_2->setParameters(paramVec);
	    boxE391_2->setSensitiveDetector("CC04",cnt);
	    boxE391_2->setOuterColor("green");
	    addDaughter(boxE391_2);
	  } else {
	    boxE391_2->cloneDetector(G4ThreeVector(x2*cm,y*cm,z*cm),
				     G4ThreeVector(0,0,0),cnt);
	  }
	  if(boxPermalloy==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(perm_outerXY*mm);
	    paramVec.push_back(perm_outerXY*mm);
	    paramVec.push_back(perm_length*mm);
	    paramVec.push_back(perm_innerXY*mm);
	    paramVec.push_back(perm_innerXY*mm);
	    paramVec.push_back(0);
	    paramVec.push_back(0);
	    paramVec.push_back(0);
	  
	    boxPermalloy = new GsimBoxWithoutABox(std::string(nam3),this,
						  G4ThreeVector(x2*cm,y*cm,z*cm),
						  G4ThreeVector(0,90*deg,0));
	    boxPermalloy->setOuterMaterial("G4_Fe");
	    boxPermalloy->setOuterColor("red");
	    boxPermalloy->setParameters(paramVec);
	    addDaughter(boxPermalloy);
	    m_detList.push_back(boxPermalloy);
	  } else {
	    boxPermalloy->cloneDetector(G4ThreeVector(x2*cm,y*cm,z*cm),
					G4ThreeVector(0,90*deg,0),cnt);
	  }
	  if(iy!=3){
	    if(boxPlate==0) {	  
	      std::vector<double> paramVec;
	      paramVec.push_back(acryl_thickness*mm);
	      paramVec.push_back(acryl_xy*mm);
	      paramVec.push_back(acryl_xy*mm);
	      
	      
	      boxPlate = new GsimBox(std::string(nam4),this,
				     G4ThreeVector(xPlate*cm,y*cm,z*cm),
				     G4ThreeVector(0,0,0));
	      boxPlate->setOuterMaterial("G4_PLEXIGLASS");
	      boxPlate->setOuterColor("yellow");
	      boxPlate->setParameters(paramVec);
	      addDaughter(boxPlate);
	      m_detList.push_back(boxPlate);
	    } else {
	      boxPlate->cloneDetector(G4ThreeVector(xPlate*cm,y*cm,z*cm),
				      G4ThreeVector(0,0,0),cnt);
	    }	  	  
	    cnt++;
	  }
	  else {
	    if(boxCarbon==0) {	  
	      std::vector<double> paramVec;
	      paramVec.push_back(carbon_thickness*mm);
	      paramVec.push_back(carbon_xy*mm);
	      paramVec.push_back(carbon_xy*mm);
	      
	      
	      boxCarbon = new GsimBox(std::string(nam5),this,
				      G4ThreeVector(xPlate*cm,y*cm,z*cm),
				      G4ThreeVector(0,0,0));
	      boxCarbon->setOuterMaterial("GsimCFRP");
	      boxCarbon->setOuterColor("gray70");
	      boxCarbon->setParameters(paramVec);
	      addDaughter(boxCarbon);
	      m_detList.push_back(boxCarbon);
	    } else {
	      boxCarbon->cloneDetector(G4ThreeVector(xPlate*cm,y*cm,z*cm),
				       G4ThreeVector(0,0,0),cnt);
	    }	  	  
	    cnt++;
	  }
	}
      }
    }

    GsimBox* box1=0;
    GsimBox* box2=0;


    for(int iz=0;iz<4;iz++) {
      for(int iy=0;iy<2;iy++) {
	for(int ix=0;ix<2;ix++) {
	  double z = c_cc04_z02+(cc04_width2+c_cc04_gap)*iz;
	  double y = c_cc04_y0+(cc04_width+cc04_width2+c_cc04_gap*2)*iy+
	    (cc04_width*2.5+cc04_width2*0.5+c_cc04_gap*3)+c_cc04_y_offset[iz];	
	  double x = (2*ix-1)*(cc04_len2/2.+8.5);
	  //	cc04_length = cc04_len2[ix][iy][iz];
	
	  sprintf(nam,"CC04KTeVCSI");
	  if(box2==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_len2*cm);
	    paramVec.push_back(cc04_width2*cm);
	    paramVec.push_back(cc04_width2*cm);
	  
	    box2 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box2->setOuterMaterial("G4_CESIUM_IODIDE");
	    box2->setParameters(paramVec);
	    box2->setSensitiveDetector("CC04",cnt);
	    addDaughter(box2);
	  } else {
	    box2->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }
  
    box1=0;
    box2=0;

    for(int iz=0;iz<2;iz++) {
      double z = c_cc04_sci_z0+1.5*iz;
      if(iz==0){
	double y = 0;
	for(int ix=0;ix<2;ix++){
	  double x = (8.5+cc04_sci_len/2.)*(2*ix-1);

	  if(ix==0) {
	    cnt = 63;
	  } else {
	    cnt = 62;
	  }

	  sprintf(nam,"CC04SCI1");
	  if(box1==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_sci_len*cm);
	    paramVec.push_back(cc04_sci_width*cm);
	    paramVec.push_back(cc04_sci_thk*cm);
	  
	    box1 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box1->setParameters(paramVec);
	    box1->setOuterColor("yellow");
	    box1->setSensitiveDetector("CC04",cnt);
	    addDaughter(box1);
	  } else {
	    box1->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	}
      } else {
	double x = 0;
	for(int iy=0;iy<2;iy++){
	  //	 if(iy==0)
	  //{
	  //double y = c_cc04_y0+(cc04_width+c_cc04_gap);
	  //}
	  //else if(iy==1)
	  //{
	  //double y = c_cc04_y0+(cc04_width+c_cc04_gap)*(5-2)+
	  //   (cc04_width+cc04_width2+c_cc04_gap*2)*2;
	  //}
	  double y = (8.5+cc04_sci_width2/2.)*(2*iy-1);
	  if(iy==0) {
	    cnt=60;
	  } else {
	    cnt=64;
	  }

	  sprintf(nam,"CC04SCI2");
	  if(box2==0) {
	    
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_sci_len2*cm);
	    paramVec.push_back(cc04_sci_width2*cm);
	    paramVec.push_back(cc04_sci_thk*cm);
	    
	    box2 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box2->setParameters(paramVec);
	    box2->setOuterColor("yellow");
	    box2->setSensitiveDetector("CC04",cnt);
	    addDaughter(box2);
	  } else {
	    box2->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	}
      }
    }
  } else if(m_model==2) {
    //Banno
    double cc04_len  = Csic_Len;
    //  double cc04_len2[2][2][4]  = {{{24.0, 24.3, 25.0, 25.6},{24.0, 24.0, 25.7, 26.0}},{{24.0, 24.4, 26.0, 26.0},{24.0, 25.0, 26.0, 26.0}}}; //cm
    double cc04_width = Csic_XY;
    double cc04_width2 = 5.0; //cm
    double cc04_sci_thk = 1.0;//cm
    double cc04_sci_len = 25.0;//cm
    double cc04_sci_len2 = 60.0;//cm
    double cc04_sci_width = 18.2;//cm
    double cc04_sci_width2 = 21.0;//cm

    double cc04_len2 = 25.0;

  
    double c_cc04_x_offset[4] = {1.5, -1.5, 0, 0};
    double c_cc04_y_offset[4] = {0, 0, 0, 0};

  
    double c_cc04_gap = Csic_Gap;
    double c_cc04_y0 = -(cc04_width2+cc04_width*3.0+c_cc04_gap*4);
    double c_cc04_z0 = -(cc04_width+c_cc04_gap)*1;
    double c_cc04_z02 = -(cc04_width*1.5+c_cc04_gap-cc04_width2*0.5)*1;
    double c_cc04_x[2] = { -(cc04_len+c_cc04_gap)*0.5,
			   (cc04_len+c_cc04_gap)*0.5 };
  
    double c_cc04_sci_z0 =
      -(cc04_width+c_cc04_gap)*1.5-2.5-1.5;
  
    int cnt=0;
    GsimBox* box1 =0;
    GsimBox* box2 =0;
  
    for(int iz=0;iz<3;iz++) {
      for(int iy=0;iy<7;iy++) {
	for(int ix=0;ix<2;ix++) {
	  double z = c_cc04_z0+(cc04_width+c_cc04_gap)*iz;
	  double y;
	  if(iy<3){
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*iy+c_cc04_y_offset[iz];
	  } else if(iy==3 || iy==4){
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*2+
	      (cc04_width+cc04_width2+c_cc04_gap*2)*(iy-2)+c_cc04_y_offset[iz];
	  } else {
	    y = c_cc04_y0+(cc04_width+c_cc04_gap)*(iy-2)+
	      (cc04_width+cc04_width2+c_cc04_gap*2)*2+c_cc04_y_offset[iz];
	  }
	  double x=c_cc04_x[ix]+c_cc04_x_offset[iz]+(2*ix-1)*0.35;
	  if(iy==3) {
	    if(ix==0) x=c_cc04_x[ix]-8.5;
	    else      x=c_cc04_x[ix]+8.5;
	  }

	  sprintf(nam,"CC04CSI1");
	  if(box1==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_len*cm);
	    paramVec.push_back(cc04_width*cm);
	    paramVec.push_back(cc04_width*cm);
	  
	    box1 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box1->setOuterMaterial("G4_CESIUM_IODIDE");
	    box1->setParameters(paramVec);
	    box1->setSensitiveDetector("CC04",cnt);
	    addDaughter(box1);
	  } else {
	    box1->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }
    for(int iz=0;iz<4;iz++) {
      for(int iy=0;iy<2;iy++) {
	for(int ix=0;ix<2;ix++) {
	  double z = c_cc04_z02+(cc04_width2+c_cc04_gap)*iz;
	  double y = c_cc04_y0+(cc04_width+cc04_width2+c_cc04_gap*2)*iy+
	    (cc04_width*2.5+cc04_width2*0.5+c_cc04_gap*3)+c_cc04_y_offset[iz];	
	  double x = (2*ix-1)*(cc04_len2/2.+8.5);
	  //	cc04_length = cc04_len2[ix][iy][iz];
	
	  sprintf(nam,"CC04CSI2");
	  if(box2==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_len2*cm);
	    paramVec.push_back(cc04_width2*cm);
	    paramVec.push_back(cc04_width2*cm);
	  
	    box2 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box2->setOuterMaterial("G4_CESIUM_IODIDE");
	    box2->setParameters(paramVec);
	    box2->setSensitiveDetector("CC04",cnt);
	    addDaughter(box2);
	  } else {
	    box2->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }
  
    box1=0;
    box2=0;

    for(int iz=0;iz<2;iz++) {
      double z = c_cc04_sci_z0+1.5*iz;
      if(iz==0){
	double y = 0;
	for(int ix=0;ix<2;ix++){
	  double x = (8.5+cc04_sci_len/2.)*(2*ix-1);

	  if(ix==0) {
	    cnt = 63;
	  } else {
	    cnt = 62;
	  }

	  sprintf(nam,"CC04SCI1");
	  if(box1==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_sci_len*cm);
	    paramVec.push_back(cc04_sci_width*cm);
	    paramVec.push_back(cc04_sci_thk*cm);
	  
	    box1 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box1->setParameters(paramVec);
	    box1->setOuterColor("yellow");
	    box1->setSensitiveDetector("CC04",cnt);
	    addDaughter(box1);
	  } else {
	    box1->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	}
      } else {
	double x = 0;
	for(int iy=0;iy<2;iy++){
	  //	 if(iy==0)
	  //{
	  //double y = c_cc04_y0+(cc04_width+c_cc04_gap);
	  //}
	  //else if(iy==1)
	  //{
	  //double y = c_cc04_y0+(cc04_width+c_cc04_gap)*(5-2)+
	  //   (cc04_width+cc04_width2+c_cc04_gap*2)*2;
	  //}
	  double y = (8.5+cc04_sci_width2/2.)*(2*iy-1);
	  if(iy==0) {
	    cnt=60;
	  } else {
	    cnt=64;
	  }

	  sprintf(nam,"CC04SCI2");
	  if(box2==0) {
	    
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_sci_len2*cm);
	    paramVec.push_back(cc04_sci_width2*cm);
	    paramVec.push_back(cc04_sci_thk*cm);
	    
	    box2 = new GsimBox(std::string(nam),this,
			       G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0));
	    box2->setParameters(paramVec);
	    box2->setOuterColor("yellow");
	    box2->setSensitiveDetector("CC04",cnt);
	    addDaughter(box2);
	  } else {
	    box2->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0),cnt);
	  }
	}
      }
    }
  } else if(m_model==0) {
    double cc04_len  = Csic_Len;
    double cc04_width = Csic_XY;
    double c_cc04_gap = Csic_Gap;
    double cc04_sci_thk = 0.5;//cm

    double c_cc04_sci_z0 =
      -(cc04_len+c_cc04_gap)*0.5
      -(cc04_sci_thk+c_cc04_gap)*1.5;
    double c_cc04_sci_x_offset[2] = {-0.3, 0.3};
    double c_cc04_sci_y_offset[2] = {-0.3, 0.3};


    int cnt=0;
    GsimBox* box =0;
    GsimBox* box2 =0;

    for(int iz=0;iz<2;iz++) {
      for(int iy=0;iy<13;iy++) {
	for(int ix=0;ix<2;ix++) {
	  double z = c_cc04_sci_z0+(cc04_sci_thk+c_cc04_gap)*iz;
	  double y = (cc04_width+c_cc04_gap)*(iy-6)
	    +c_cc04_sci_y_offset[iz];
	  if(iy>=5 && iy<=7) {
	    sprintf(nam,"CC04SCII");
	    double len=(cc04_width+c_cc04_gap)*(10.*0.5);
	    double off=(cc04_width+c_cc04_gap)*(3.*0.5);
	    double x = (len+off*2.)*(ix-0.5)
	      +c_cc04_sci_x_offset[iz];
	    if(box2==0) {
	      std::vector<double> paramVec;
	      paramVec.push_back(len*cm);
	      paramVec.push_back(cc04_width*cm);
	      paramVec.push_back(cc04_sci_thk*cm);
	      box2 = new GsimBox(std::string(nam),this,
				 G4ThreeVector(x*cm,y*cm,z*cm),
				 G4ThreeVector(0,0,0));
	      box2->setParameters(paramVec);
	      box2->setOuterColor("yellow");
	      box2->setSensitiveDetector("CC04",cnt);
	      addDaughter(box2);
	    } else {
	      box2->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				  G4ThreeVector(0,0,0),cnt);
	    }
	  } else {
	    sprintf(nam,"CC04SCIO");
	    double len=(cc04_width+c_cc04_gap)*(13.*0.5);
	    double off=0;
	    double x = (len+off*2.)*(ix-0.5)
	      +c_cc04_sci_x_offset[iz];
	    if(box==0) {
	      std::vector<double> paramVec;
	      paramVec.push_back(len*cm);
	      paramVec.push_back(cc04_width*cm);
	      paramVec.push_back(cc04_sci_thk*cm);
	      box = new GsimBox(std::string(nam),this,
				G4ThreeVector(x*cm,y*cm,z*cm),
				G4ThreeVector(0,0,0));
	      box->setParameters(paramVec);
	      box->setOuterColor("yellow");
	      box->setSensitiveDetector("CC04",cnt);
	      addDaughter(box);
	    } else {
	      box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
				 G4ThreeVector(0,0,0),cnt);
	    }
	  }
	  cnt++;
	}
      }
    }
      
    
    
    cnt=0;
    box =0;
    for(int ix=0;ix<13;ix++) {
      for(int iy=0;iy<13;iy++) {

	if(ix>=5 && ix<=7 &&
	   iy>=5 && iy<=7 ) continue;

	
	double z = 0;
	double x = (cc04_width+c_cc04_gap)*(ix-6);
	double y = (cc04_width+c_cc04_gap)*(iy-6);
	sprintf(nam,"CC04CSI");
	if(box==0) {
	  std::vector<double> paramVec;
	  paramVec.push_back(cc04_width*cm);
	  paramVec.push_back(cc04_width*cm);
	  paramVec.push_back(cc04_len*cm);
	  
	  box = new GsimBox(std::string(nam),this,
			    G4ThreeVector(x*cm,y*cm,z*cm),
			    G4ThreeVector(0,0,0));
	  box->setOuterMaterial("G4_CESIUM_IODIDE");
	  box->setParameters(paramVec);
	  box->setSensitiveDetector("CC04",cnt);
	  addDaughter(box);
	} else {
	  box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
			     G4ThreeVector(0,0,0),cnt);
	}
	cnt++;
      }
    }
    
  } else if(m_model==1) {

  
    double cc04_len  = Csic_Len;
    double cc04_width = Csic_XY;
    double cc04_sci_thk = 0.5;//cm

    double c_cc04_x_offset[3] = {-0.3, 0,0.3};
    double c_cc04_y_offset[3] = {-0.3, 0,0.3};
    double c_cc04_sci_x_offset[2] = {-0.3, 0.3};
    double c_cc04_sci_y_offset[2] = {-0.3, 0.3};

    double c_cc04_gap = Csic_Gap;
    double c_cc04_y0 = -(cc04_width+c_cc04_gap)*3.5;
    double c_cc04_z0 = -(cc04_width+c_cc04_gap)*1;
    double c_cc04_x[2] = { -(cc04_len+c_cc04_gap)*0.5,
			   (cc04_len+c_cc04_gap)*0.5 };

    double c_cc04_sci_z0 =
      -(cc04_width+c_cc04_gap)*1.5
      -(cc04_sci_thk+c_cc04_gap)*1.5;
  
    int cnt=0;
    GsimBox* box =0;
    for(int iz=0;iz<2;iz++) {
      cnt=0;
      for(int iy=0;iy<8;iy++) {
	for(int ix=0;ix<2;ix++) {
	  double z = c_cc04_sci_z0+(cc04_sci_thk+c_cc04_gap)*iz;
	  double y = c_cc04_y0+(cc04_width+c_cc04_gap)*iy+c_cc04_sci_y_offset[iz];
	  double x=c_cc04_x[ix]+c_cc04_sci_x_offset[iz];
	  if(iy==3 || iy==4) {
	    if(ix==0) x-=7;
	    else      x+=7;
	  }
	  sprintf(nam,"CC04SCI");
	  if(box==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_len*cm);
	    paramVec.push_back(cc04_width*cm);
	    paramVec.push_back(cc04_sci_thk*cm);
	  
	    box = new GsimBox(std::string(nam),this,
			      G4ThreeVector(x*cm,y*cm,z*cm),
			      G4ThreeVector(0,0,0));
	    box->setParameters(paramVec);
	    box->setOuterColor("yellow");
	    box->setSensitiveDetector("CC04",cnt);
	    addDaughter(box);
	  } else {
	    box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }

    box=0;
    for(int iz=0;iz<3;iz++) {
      for(int iy=0;iy<8;iy++) {
	for(int ix=0;ix<2;ix++) {
	
	  double z = c_cc04_z0+(cc04_width+c_cc04_gap)*iz;
	  double y = c_cc04_y0+(cc04_width+c_cc04_gap)*iy+c_cc04_y_offset[iz];
	  double x=c_cc04_x[ix]+c_cc04_x_offset[iz];
	  if(iy==3 || iy==4) {
	    if(ix==0) x-=7;
	    else      x+=7;
	  }
	  sprintf(nam,"CC04CSI");
	  if(box==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc04_len*cm);
	    paramVec.push_back(cc04_width*cm);
	    paramVec.push_back(cc04_width*cm);
	  
	    box = new GsimBox(std::string(nam),this,
			      G4ThreeVector(x*cm,y*cm,z*cm),
			      G4ThreeVector(0,0,0));
	    box->setOuterMaterial("G4_CESIUM_IODIDE");
	    box->setParameters(paramVec);
	    box->setSensitiveDetector("CC04",cnt);
	    addDaughter(box);
	  } else {
	    box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }
  }
  setThisAndDaughterBriefName("CC04");
}

GsimE14CC04::~GsimE14CC04()
{
  ;
}

double GsimE14CC04::getCC04Length() {
  double cc04_width = Csic_XY;
  double c_cc04_gap = Csic_Gap;
  return (cc04_width+c_cc04_gap)*3.;
}

void GsimE14CC04::comment()
{
  std::cout << "GsimE14CC04 Z position:" << std::endl;
  std::cout << "    The center of central crystal in three 7 cm crystals along z." << std::endl;
  std::cout << "Place it at z=(614.8+95.5+31.2+(7.+0.03)*3./2.)*cm" << std::endl;
  std::cout << "When userFlag ==201605, place it downstream by 2 cm" << std::endl;
  std::cout << "      z=(614.8+95.5+31.2+(7.+0.03)*3./2.+2)*cm" << std::endl;
  std::cout << "For userFlag >=201902, place it at" << std::endl;
  std::cout << "      z=(614.8+95.5+31.2+6.5+(7.+0.03)*3./2.+2)*cm" << std::endl;
}

void GsimE14CC04::setFastSimulationLevel(int level)
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
  }

  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("CC04",0);
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
