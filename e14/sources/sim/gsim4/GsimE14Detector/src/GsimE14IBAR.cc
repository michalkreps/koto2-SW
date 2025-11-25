/**
 *  @file
 *  @brief  GsimE14IBAR
 *
 *
 */
#include "GsimE14Detector/GsimE14IBAR.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14IBAR::GsimE14IBAR(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14IBAR";

  if(
     !(userFlag == 201605 ||
       userFlag == 201902 ||
       userFlag == 202002 ||
       userFlag == 202102 ||
       userFlag == 202306
       )
     ) {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. OEV is not constructed.");
    return;
  }

  // Inner Barrel
  double  ibbp_height = 1.0; // Ibar Back Plate Thickness 
  double  ibfp_height = 0.3; // Ibar Front Plate Thickness 
  double  ibrg_height = 5.5; // Ibar Ring Height
  double  ibdt_height = 15.84; // IBAR detector thickness
  double  ibar_height   = ibdt_height + (ibbp_height + ibrg_height) + ibfp_height; // total thickness from inside Front Plate to outside Ring
  double  ibrl_space = 2.8; // IBAR space for rail
  double  ibdt_len_bot  = 277.7 - 2.*(17.95-12.9); // IBAR detector length
  double  ibdt_len_top = 277.7; // IBAR detector length 
  double  ibar_len_bot  = ibdt_len_bot;
  double  ibar_len_top  = ibdt_len_top + 2.*(17.95-12.9)*(ibbp_height+ibrg_height)/ibdt_height;
  double  ibar_len_rail  = 300.0;
  double  ibar_sidegap = 0.32; // Clearance b/w Module
  double  ibdt_bottom = 14.64; // IBAR detector inner width
  double  ibdt_top = ibdt_bottom + ibdt_height*tan( 2*M_PI/32 );
  double  ibdt_bottom_wgap   = ibdt_bottom + ibar_sidegap; // = width of first scinti. + sidegap
  double  ibdt_top_wgap = ibdt_bottom_wgap + ibdt_height*tan( 2*M_PI/32 ); // = width of last scinti. + sidegap
  double  ibar_bottom = ibdt_bottom_wgap - ibfp_height*tan( 2*M_PI/32 ); // bottom of normal module
  double  ibar_top = ibdt_bottom_wgap + (ibar_height - ibfp_height)*tan( 2*M_PI/32 );
  double  ibar_bot_bottom = ibdt_top - 2*(ibdt_height + ibfp_height )*tan( 2*M_PI/32 ); // bottom of bottom module
  double  ibar_bot_top = ibdt_top + 2*(ibbp_height + ibrg_height)*tan( 2*M_PI/32 ); // top of bottom module
  double  ibar_top_wrail = ibdt_top_wgap + (ibbp_height + ibrg_height + ibrl_space)*tan( 2*M_PI/32 ); // top of modules with rail
  double  ibar_bot_top_wrail = ibar_bot_top + 2*ibrl_space*tan( 2*M_PI/32 ); // top of rail of bottom module
  double  ibar_bot_height = ibar_height + ibrl_space; // height of bottom module
  double  ibar_rbkpl     = 92.0;
  double  ibar_rmin     = ibar_rbkpl - ibdt_height - ibfp_height;
  double  ibar_trap_th  = -atan( (ibar_top - ibar_bottom)/2/ibar_height );
  double  mod_rotate = 0.5; // Ibar phi rotation (2*PI=32*mod_rotate)
  double  mod_slidey = 0.55; // fine tuning of position due to rotation (see top and neighbor modules)


  std::vector<double> paramVec;
  paramVec.push_back(ibar_height*cm);
  paramVec.push_back(ibar_trap_th*rad);
  paramVec.push_back(0*rad);
  paramVec.push_back(ibar_len_top*cm);
  paramVec.push_back(ibar_top*cm);
  paramVec.push_back(ibar_top*cm);
  paramVec.push_back(0*deg);
  paramVec.push_back(ibar_len_bot*cm);
  paramVec.push_back(ibar_bottom*cm);
  paramVec.push_back(ibar_bottom*cm);
  paramVec.push_back(0*deg);

  GsimTrap* trapIBAR[7]={0,0,0,0,0,0,0};
  char nam[100];

  for( int i=0;i<32;i++ ){
    double  x = ibar_rmin + ibar_height/2;
    double  y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4;
    double  z = 280.0 + ibar_len_bot/2;// 280.0...position of ibar inside z  (cf. Detpos_cbar_z=134.8)

    if(i==7){ //top module
      y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4 + mod_slidey;
    }else if(i>=8 && i<19) { // south modules
      y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4 + 2*mod_slidey;
    }else if(i>=19 && i<23) { // south modules with rail
      x = ibar_rmin + ibar_height/2 + ibrl_space/2;
      y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4
          + 2*mod_slidey*ibar_height/ibar_bot_height;
    }else if(i==23) { //bottom module
      x = ibar_rmin + ibar_height/2 + ibrl_space/2;
      y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4 + mod_slidey;
    }else if(i>=24 && i<29) { // north modules with rail
      x = ibar_rmin + ibar_height/2 + ibrl_space/2;
      y = ibdt_bottom_wgap/2 - (ibdt_top_wgap+ibdt_bottom_wgap)/4
          + 2*mod_slidey*(1-ibar_height/ibar_bot_height);
    }
    
    CLHEP::Hep3Vector c( x,y,z );
    c.rotateZ( 2*M_PI*(i+mod_rotate)/32 );

    if(i==0) { //normal (south and north) modules
      std::sprintf(nam,"IBAR0");
      trapIBAR[0] = new GsimTrap(std::string(nam),this,
			      G4ThreeVector(c.x()*cm,c.y()*cm,0),
			      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i
			      );
      trapIBAR[0]->setOuterMaterial("G4_Galactic");
      trapIBAR[0]->setParameters(paramVec);
      trapIBAR[0]->setOuterVisibility(false); //comment out for test
//      trapIBAR[0]->setOuterColor("blue"); // test only
      addDaughter(trapIBAR[0]);
      m_detList.push_back(trapIBAR[0]);

    } else if( i<7 ) {
      trapIBAR[0]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else if( i==7 ) {   //top module
      paramVec.clear();
      paramVec.push_back(ibar_height*cm);
      paramVec.push_back(0*rad);
      paramVec.push_back(0*rad);
      paramVec.push_back(ibar_len_top*cm);
      paramVec.push_back(ibdt_top_wgap*cm);
      paramVec.push_back(ibdt_top_wgap*cm);
      paramVec.push_back(0*deg);
      paramVec.push_back(ibar_len_bot*cm);
      paramVec.push_back(ibdt_top_wgap*cm);
      paramVec.push_back(ibdt_top_wgap*cm);
      paramVec.push_back(0*deg);

      std::sprintf(nam,"IBAR1");
      trapIBAR[1] = new GsimTrap(std::string(nam),this,
			      G4ThreeVector(c.x()*cm,c.y()*cm,0),
			      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i
			      );
      trapIBAR[1]->setOuterMaterial("G4_Galactic");
      trapIBAR[1]->setParameters(paramVec);
      trapIBAR[1]->setOuterVisibility(false); //comment out for test
//      trapIBAR[1]->setOuterColor("blue"); // test only
      trapIBAR[1]->setOuterCopyNo(i);
      addDaughter(trapIBAR[1]);
      m_detList.push_back(trapIBAR[1]);

    } else if( i<19 ) { // south normal modules
      trapIBAR[0]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else if( i==19 || i==20 ) { //south modules with rail
      paramVec.clear();
      paramVec.push_back(ibar_bot_height*cm); //Thicker flame for Rail
      paramVec.push_back(ibar_trap_th*rad);
      paramVec.push_back(0*rad);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_top_wrail*cm);
      paramVec.push_back(ibar_top_wrail*cm);
      paramVec.push_back(0*deg);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_bottom*cm);
      paramVec.push_back(ibar_bottom*cm);
      paramVec.push_back(0*deg);

      std::sprintf(nam,"IBAR%01d",i-16);
      trapIBAR[i-16] = new GsimTrap(std::string(nam),this,
			      G4ThreeVector(c.x()*cm,c.y()*cm,0),
			      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);
      trapIBAR[i-16]->setOuterMaterial("G4_Galactic");
      trapIBAR[i-16]->setParameters(paramVec);
      trapIBAR[i-16]->setOuterVisibility(false); //comment out for test
//      trapIBAR[i-16]->setOuterColor("blue"); // test only
      trapIBAR[i-16]->setOuterCopyNo(i);
      addDaughter(trapIBAR[i-16]);
      m_detList.push_back(trapIBAR[i-16]);

    } else if( i==21 ) { // south module with rail on right angle side
      trapIBAR[3]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else if( i==22 ) { // south module with rail on opposite side
      trapIBAR[4]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else if( i==23 ) { // bottom module
      paramVec.clear();
      paramVec.push_back(ibar_bot_height*cm); //Thicker flame for Rail
      paramVec.push_back(0*rad);
      paramVec.push_back(0*rad);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_bot_top_wrail*cm+1*mm); //HNDEBUG 1*mm added to remove geom.overlap
      paramVec.push_back(ibar_bot_top_wrail*cm+1*mm);//HNDEBUG 1*mm added to remove geom.overlap
      paramVec.push_back(0*deg);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_bot_bottom*cm+1*mm);//HNDEBUG 1*mm added to remove geom.overlap
      paramVec.push_back(ibar_bot_bottom*cm+1*mm);//HNDEBUG 1*mm added to remove geom.overlap
      paramVec.push_back(0*deg);

      std::sprintf(nam,"IBAR2");
      trapIBAR[2] = new GsimTrap(std::string(nam),this,
			      G4ThreeVector(c.x()*cm,c.y()*cm,0),
			      G4ThreeVector(M_PI/2.,0,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);
      trapIBAR[2]->setOuterMaterial("G4_Galactic");
      trapIBAR[2]->setParameters(paramVec);
      trapIBAR[2]->setOuterVisibility(false); //comment out for test
//      trapIBAR[2]->setOuterColor("blue"); // test only
      trapIBAR[2]->setOuterCopyNo(i);
      addDaughter(trapIBAR[2]);
      m_detList.push_back(trapIBAR[2]);

    } else if( i==24 || i==25 ) { // north module with rail
      paramVec.clear();
      paramVec.push_back(ibar_bot_height*cm); //Thicker flame for Rail
      paramVec.push_back(ibar_trap_th*rad);
      paramVec.push_back(0*rad);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_top_wrail*cm);
      paramVec.push_back(ibar_top_wrail*cm);
      paramVec.push_back(0*deg);
      paramVec.push_back(ibar_len_rail*cm); // Longer flame for Rail
      paramVec.push_back(ibar_bottom*cm);
      paramVec.push_back(ibar_bottom*cm);
      paramVec.push_back(0*deg);

      std::sprintf(nam,"IBAR%01d",i-19);
      trapIBAR[i-19] = new GsimTrap(std::string(nam),this,
			      G4ThreeVector(c.x()*cm,c.y()*cm,0),
			      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);
      trapIBAR[i-19]->setOuterMaterial("G4_Galactic");
      trapIBAR[i-19]->setParameters(paramVec);
      trapIBAR[i-19]->setOuterVisibility(false); //comment out for test
//      trapIBAR[i-19]->setOuterColor("blue"); // test only
      trapIBAR[i-19]->setOuterCopyNo(i);
      addDaughter(trapIBAR[i-19]);
      m_detList.push_back(trapIBAR[i-19]);

    } else if( i==26 || i==28) {
      trapIBAR[5]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else if( i==27 ) {
      trapIBAR[6]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);

    } else {
      trapIBAR[0]->cloneDetector(G4ThreeVector(c.x()*cm,c.y()*cm,0),
      G4ThreeVector(M_PI/2.,M_PI,1.*((24+i+mod_rotate))/32.*2.*M_PI),i);
    }
  }



  GsimTrap* trap[7]={0,0,0,0,0,0,0};
  
  // Back Plate 
  double  ibbp_len_bot  = ibdt_len_top;
  double  ibbp_len_top  = ibbp_len_bot;
  double  ibbp_bottom   = ibdt_top;
  double  ibbp_top      = ibbp_bottom;
  for( int l=0;l<7;l++ ){ //0...normal, 1...top, 2...bottom module,
                          //3(,5)...south (north) right angle,
                          //4,(6)...south (north) acute angle
    double  ibbp_trap_th  = -atan( (ibbp_top-ibbp_bottom)/2/ibbp_height );
    if( l==1 || l==2 ){ // top and bottom modules
      ibbp_trap_th  = 0;
    }
    paramVec.clear();
    paramVec.push_back(ibbp_height*cm);
    paramVec.push_back(ibbp_trap_th*rad);
    paramVec.push_back(0*rad);
    paramVec.push_back(ibbp_len_top*cm);
    paramVec.push_back(ibbp_top*cm);
    paramVec.push_back(ibbp_top*cm);
    paramVec.push_back(0*deg);
    paramVec.push_back(ibbp_len_bot*cm);
    paramVec.push_back(ibbp_bottom*cm);
    paramVec.push_back(ibbp_bottom*cm);
    paramVec.push_back(0*deg);

  
    double  ibbp_x = (ibbp_bottom+ibbp_top)/4 - (ibar_top + ibar_bottom)/4 + ibar_sidegap;
    double  ibbp_y = 0;
    double  ibbp_z = -ibar_height/2 + ibbp_height/2 + ibrg_height;
    if( l==1 ){
      ibbp_x = 0;
    }else if( l==2 ){
      ibbp_x = 0;
      ibbp_z = -ibar_bot_height/2 + ibbp_height/2 + ibrg_height + ibrl_space;
    }else if( l>2 ){
      ibbp_x = (ibbp_bottom+ibbp_top)/4 - (ibar_top_wrail + ibar_bottom)/4 + ibar_sidegap;
      ibbp_z = -ibar_bot_height/2 + ibbp_height/2 + ibrg_height + ibrl_space;
    }

    std::sprintf(nam,"IBBP%01d",l);
    trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
		      G4ThreeVector(ibbp_x*cm,ibbp_y*cm,ibbp_z*cm),
		      G4ThreeVector(0,0,0)
		      );
    trap[l]->setOuterMaterial("G4_Al");
    trap[l]->setParameters(paramVec);
    trap[l]->setOuterColor("gray70");
    trapIBAR[l]->addDaughter(trap[l]);
     m_detList.push_back(trap[l]);
  }
  

  // Rib1~3 for 8-Rings (Rib1= Ring bottom, Rib2=Web, Rib3 = Ring top),
  // Earthquake bracket, Teflon Pad, Rail and Stopper
  {
    // vartical direction from beam center
    double  ibr1_height  = 0.8; // plate thickness for Rib1
    double  ibr3_height = 0.8; // plate thickness for Rib3
    double  ibr2_height = ibrg_height - ibr1_height - ibr3_height;  // height of Rib2
    double  ibbr_height  = 0.8; // plate thickness for Earthquake Bracket
    double  ibpd_height  = 1.55; // plate thickness for Teflon Pad
    double  ibrl_center_height  = 1.2; // plate thickness for Rail
    double  ibrl_joint_height  = 0.6; // plate thickness for Rail Joint Part
    double  ibst_height  = 1.0; // plate thickness for Stopper

    // beam direction
    double  ibr1_xthick  = 10.0; // length of Rib1
    double  ibr2_xthick = 2.0; // length of Rib2
    double  ibr3_xthick = 10.0; // length of Rib3
    double  ibebl_xthick  = 10.0; //length of Earthquake Bracket (6 Long brackets)
    double  ibebs_xthick  = 5.0; //length of Earthquake Bracket (Center 2 Short brackets)
    double  ibpd_xthick  = 12.0; //length of Teflon Pad

    // circumference
    double  ibeb_ywidth  = 3.5; //half width of Earthquake Bracket
    double  ibpd_ywidth  = 11.0/2.; //half width of Teflon Pad
    double  ibrl_ywidth  = 18.0/2.; //half width of Rail
    double  ibr1_bottom  = ibdt_top + ibbp_height*tan( 2*M_PI/32) 
                           + ibar_sidegap; // Rib1 bottom width
    double  ibr1_top     = ibr1_bottom + ibr1_height*tan( 2*M_PI/32 ); // Rib1 top width
    double  ibr2_bottom  = ibr1_top; // Rib2 bottom width
    double  ibr2_top     = ibr2_bottom + ibr2_height*tan( 2*M_PI/32 ); // Rib2 top width
    double  ibr3_bottom  = ibr2_top; // Rib3 bottom width
    double  ibr3_top     = ibr3_bottom + ibr3_height*tan( 2*M_PI/32 ); // Rib3 top width
    double  ibst_ywidth  = ibar_top_wrail - (ibrl_center_height+ibst_height)*tan( 2*M_PI/32); //width of Stopper

    // others
    int     ibrg_nrib    = 8; // number of Rings
    double  ibrg_hpitch  = 33.0;  // pitch of Rings
    double  ibrg_hpos  = ibrg_hpitch/2.; // nearest Ring position from IBAR center
    double  ibrg_ibeb_dist  = 11.2; // distance from Ring to Earthquake bracket


    // Rinb Rib[1~3] for 8-Rings
    double  ibrr_len_bot[3]  = { ibr1_xthick, ibr2_xthick, ibr3_xthick};
    double  ibrr_len_top[3]  = { ibr1_xthick, ibr2_xthick, ibr3_xthick};
    double  ibrr_height[3]   = { ibr1_height, ibr2_height, ibr3_height};
    double  ibrr_bottom[7][3]   = {}; //[l][k]
    double  ibrr_top[7][3]      = {}; //]l][k]
    for( int l=0;l<7;l++ ){ //0...normal, 1...top module, 2...bottom module
                            //3(,5)...south (north) right angle,
                            //4,(6)...south (north) acute angle
      for( int k=0;k<3;k++ ){ //0...Rib1 (Ring bottom), 1...Rib2 (Ring Web), 2...Rib3 (Ring top)
        if( l==1 ){
          ibrr_top[l][k]  = ibdt_top_wgap;
          ibrr_bottom[l][k]  = ibdt_top_wgap;
        }else if( l==2 ){
          ibrr_top[l][2]  = ibar_bot_top;
          ibrr_bottom[l][2]  = ibrr_top[l][2] - 2*ibrr_height[2]*tan( 2*M_PI/32 );
          ibrr_top[l][1]  = ibrr_bottom[l][2];
          ibrr_bottom[l][1]  = ibrr_top[l][1] - 2*ibrr_height[1]*tan( 2*M_PI/32 );
          ibrr_top[l][0]  = ibrr_bottom[l][1];
          ibrr_bottom[l][0]  = ibrr_top[l][0] - 2*ibrr_height[0]*tan( 2*M_PI/32 );
        }else{
          ibrr_top[l][0]  = ibr1_top;
          ibrr_bottom[l][0]  = ibr1_bottom;
          ibrr_top[l][1]  = ibr2_top;
          ibrr_bottom[l][1]  = ibr2_bottom;
          ibrr_top[l][2]  = ibr3_top;
          ibrr_bottom[l][2]  = ibr3_bottom;
        }

        double  ibrr_trap_th = -atan((ibrr_top[l][k] - ibrr_bottom[l][k])/2/ibrr_height[k] );
        if( l==1 || l==2 ){
          ibrr_trap_th = 0;
        }


        for( int i=0;i<ibrg_nrib;i++ ){ // loop for number of Rings

          paramVec.clear();
          paramVec.push_back(ibrr_height[k]*cm);
          paramVec.push_back(ibrr_trap_th*rad);
          paramVec.push_back(0*rad);
          paramVec.push_back(ibrr_len_top[k]*cm);
          paramVec.push_back(ibrr_top[l][k]*cm);
          paramVec.push_back(ibrr_top[l][k]*cm);
          paramVec.push_back(0*deg);
          paramVec.push_back(ibrr_len_bot[k]*cm);
          paramVec.push_back(ibrr_bottom[l][k]*cm);
          paramVec.push_back(ibrr_bottom[l][k]*cm);
          paramVec.push_back(0*deg);
   

          double  ibrr_x = - (ibar_top + ibar_bottom)/4 + (ibrr_bottom[l][k]+ibrr_top[l][k])/4;
          double  ibrr_y = ibrg_hpos + ibrg_hpitch * (i - ibrg_nrib/2); 
          double  ibrr_z = 0;
          if( l==1 || l==2 ){
            ibrr_x = 0;
          }else if( l>2 ){
            ibrr_x = - (ibar_top_wrail + ibar_bottom)/4 + (ibrr_bottom[l][k]+ibrr_top[l][k])/4;
          }
          if( k==0 ){
            ibrr_z = -ibar_height/2 + ibrr_height[0]/2 + ibrr_height[1] + ibrr_height[2];
          }else if( k==1 ){
            ibrr_z = -ibar_height/2 + ibrr_height[1]/2 + ibrr_height[0];
          }else if( k==2 ){
            ibrr_z = -ibar_height/2 + ibrr_height[2]/2;
          }
          if( l>=2 ) ibrr_z += ibrl_space/2;

          std::sprintf(nam,"IBR%01d%01d%02d",l,k,i);
          trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
                    G4ThreeVector(ibrr_x*cm,ibrr_y*cm,ibrr_z*cm),
                    G4ThreeVector(0,0,0)
                    );
          trap[l]->setOuterMaterial("G4_Al");
          trap[l]->setParameters(paramVec);
          trap[l]->setOuterColor("gray70");
          trapIBAR[l]->addDaughter(trap[l]);
          m_detList.push_back(trap[l]);
        }
      }
    }

    // Placed on or side of 8-Rings
    // IBEBL = Brackets connecting modules provide against earthquake (left side)
    // IBEBR = Brackets connecting modules provide against earthquake (right side)
    // IBPD = Teflon Pad on bottom 5 modules of 8-Rings
    for( int l=0;l<7;l++ ){ //0...normal, 1...top module, 2...bottom module
                            //3(,5)...south (north) right angle,
                            //4,(6)...south (north) acute angle
      for( int im=0;im<3;im++ ){ //0...IBEBL, 1...IBEBR, 2...IBPD
        if( l<2 && im==2 ) continue; // no Teflon Pad for normal and top module

        double  ibeb_height      = ibbr_height;
        if( im==2 ) ibeb_height=ibpd_height;


        for( int i=0;i<ibrg_nrib;i++ ){ // loop for number of Rings
          double  ibeb_len_bot  = 0;
          if( im<2 && (i==3 || i==4) ){
            ibeb_len_bot = ibebs_xthick;
          }else if( im<2 ){
            ibeb_len_bot  = ibebl_xthick;
          }else if( im==2){
            ibeb_len_bot  = ibpd_xthick;
          }

          double  ibeb_len_top  = ibeb_len_bot;
          double  ibeb_bottom   = ibeb_ywidth;
          if( (l==2 || l==3 || l==5 ) && im==2 ){
            ibeb_bottom   = ibpd_ywidth;
          }else if( (l==4 || l==6 ) && im==2 ){
            ibeb_bottom   = ibpd_ywidth - ibpd_height*tan( 2*M_PI/32);
          }

          double  ibeb_top      = ibeb_bottom;
          if( (l==0 || l>2) && im==1 ){
            ibeb_top   = ibeb_bottom + ibeb_height*tan( 2*M_PI/32);
          }else if( l==2 ){
            ibeb_top   = ibeb_bottom + ibeb_height*tan( 2*M_PI/32);
          }else if( l>=2 && im==2 ){
            ibeb_top   = ibpd_ywidth;
          }

          double  ibeb_trap_th  = -atan((ibeb_top - ibeb_bottom)/2/ibeb_height );
          if( l==1 && im==1 ){
            ibeb_trap_th = 0;
          }else if( l==2 && (im==0 || im==2) ){
            ibeb_trap_th  = atan((ibeb_top - ibeb_bottom)/2/ibeb_height );
          }else if( (l==3 || l==5 ) && im==2 ){
            ibeb_trap_th = 0;
          }
      
          double  ibeb_hpos    = ibrg_hpitch*i
              + ibrg_ibeb_dist * ( (i<ibrg_nrib/2) ? 1 : 0 )
              + ibrg_ibeb_dist * ( (i>=ibrg_nrib/2) ? -1 : 0 )
              - (ibebl_xthick - ibebs_xthick)/2 * ( (i==3) ? 1 : 0 )
              - (ibebl_xthick - ibebs_xthick)/2 * ( (i==4) ? -1 : 0 );
          if( im==2 ){
            ibeb_hpos    = ibrg_hpitch*i;
          }

          paramVec.clear();
          paramVec.push_back(ibeb_height*cm);
          paramVec.push_back(ibeb_trap_th*rad);
          paramVec.push_back(0*rad);
          paramVec.push_back(ibeb_len_top*cm);
          paramVec.push_back(ibeb_top*cm);
          paramVec.push_back(ibeb_top*cm);
          paramVec.push_back(0*deg);
          paramVec.push_back(ibeb_len_bot*cm);
          paramVec.push_back(ibeb_bottom*cm);
          paramVec.push_back(ibeb_bottom*cm);
          paramVec.push_back(0*deg);

          double  ibeb_x = - (ibar_top + ibar_bottom)/4 
                           + (ibeb_bottom+ibeb_top)/4;
          double  ibeb_y = ibrg_hpos - ibrg_hpitch * ibrg_nrib/2 + ibeb_hpos; 
          double  ibeb_z = -ibar_height/2 + ibrg_height - ibeb_height/2;

          if( (l==1 || l==2) && im==0 ){
            ibeb_x = - (ibrr_bottom[l][0] + ibrr_top[l][0])/4
                     + (ibeb_bottom+ibeb_top)/4;
          }else if( l>2 && im==0 ){
            ibeb_x = - (ibar_top_wrail + ibar_bottom)/4 
                     + (ibeb_bottom+ibeb_top)/4;

          }else if( l==0 && im==1 ){
            ibeb_x = - (ibar_top + ibar_bottom)/4
                     + (ibeb_bottom+ibeb_top)/4 + ibrr_bottom[l][0] - ibeb_ywidth;
          }else if( (l==1 || l==2) && im==1 ){
            ibeb_x = + (ibrr_bottom[l][0] + ibrr_top[l][0])/4
                     - (ibeb_bottom+ibeb_top)/4;
          }else if( l>2 && im==1 ){
            ibeb_x = - (ibar_top_wrail + ibar_bottom)/4
                     + (ibeb_bottom+ibeb_top)/4 + ibrr_bottom[l][0] - ibeb_ywidth;

          }else if( l==2 && im==2 ){
            ibeb_x = - (2*ibrr_top[l][2]+2*(ibeb_top-ibeb_bottom))/4  // =(top width of ring + top width of teflon pad)/4
                     + (ibeb_bottom+ibeb_top)/4;
          }else if( (l==3 || l==5) && im==2 ){
            ibeb_x = - (ibar_top_wrail + ibar_bottom)/4 
                     + (ibeb_bottom+ibeb_top)/4;
          }else if( (l==4 || l==6) && im==2 ){
            ibeb_x = - (ibar_top_wrail + ibar_bottom)/4 + ibr3_top - ibpd_ywidth
                     + (ibeb_bottom+ibeb_top)/4 + ibar_sidegap-0.02;
	    //HNDEBUG 0.02*cm = 0.2mm to fix geometry overlap
          }

          if( l>=2 && im<2 ){
            ibeb_z = - ibar_bot_height/2 + ibrg_height - ibeb_height/2
                     + ibrl_space;
          }else if( l>=2 && im==2 ){
            ibeb_z = - ibar_bot_height/2 + ibrg_height - ibeb_height/2
                     + ibrl_space - ibrg_height;
          }

          std::sprintf(nam,"IBEB%01d%01d%02d",l,im,i);
          trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
                    G4ThreeVector(ibeb_x*cm,ibeb_y*cm,ibeb_z*cm),
                    G4ThreeVector(0,0,0)
                    );
          trap[l]->setOuterMaterial("G4_Al");
          trap[l]->setParameters(paramVec);
          trap[l]->setOuterColor("red");
          trapIBAR[l]->addDaughter(trap[l]);
          m_detList.push_back(trap[l]);
        }
      }
    }


    // IBRLC = Rail to install IBAR inside CBAR w/o Joint Parts
    // IBRLU = Upstream Joint Part of the Rail
    // IBRLD = Downstream Joint Part of the Rail
    // IBSTU = Upstream Stopper on the Rail
    // IBSTD = Downstream Stopper on the Rail
    double  ibrl_len_center  = 250; //Rail w/o joint length (Total length=300=250+25+25)
    double  ibrl_len_joint  = 25; // joint length for upstream (downstream) side
    double  ibrl_len_stop  = 3; // stopper length for upstream (downstream) side
    for( int l=2;l<7;l++ ){ //2...bottom module, 3(,5)...south (north) right angle,
                          //4,(6)...south (north) acute angle
      for( int im=0;im<5;im++ ){ //0...IBRLC, 1...IBRLU, 2...IBRLD, 3...IBSTU, 4...IBSTD

        double  ibrl_hpos    = 0.;
        double margin = 0.2; // distance from Joint Part by guess
        if( im==1 ){
          ibrl_hpos = -1.*(ibrl_len_center/2.+ibrl_len_joint/2.);
        }else if( im==2 ){
          ibrl_hpos = ibrl_len_center/2.+ibrl_len_joint/2.;
        }else if( im==3 ){
          ibrl_hpos = -1.*(ibrl_len_center/2.-ibrl_len_stop/2.-margin);
        }else if( im==4 ){
          ibrl_hpos = ibrl_len_center/2.-ibrl_len_stop/2.-margin;
        }

        double  ibrl_len_bot  = ibrl_len_center;
        if( im==1 || im==2 ){
          ibrl_len_bot  = ibrl_len_joint;
        }else if( im==3 || im==4 ){
          ibrl_len_bot  = ibrl_len_stop;
        }
        double  ibrl_len_top  = ibrl_len_bot;
  
        double  ibrl_bottom   = ibrl_ywidth;
        if( (im==1 || im==2) && (l==2 || l==4 || l==6) ){
          ibrl_bottom   = ibrl_ywidth + (ibrl_center_height-ibrl_joint_height)*tan( 2*M_PI/32 );
        }else if( (im==3 || im==4) && l==2 ){
          ibrl_bottom  = ibar_bot_top_wrail - 2*(ibrl_center_height+ibst_height)*tan( 2*M_PI/32 );
        }else if( im==3 || im==4 ){
          ibrl_bottom  = ibst_ywidth;
        }

        double  ibrl_height   = ibrl_center_height;
        if( im==1 || im==2 ){
          ibrl_height  = ibrl_joint_height;
        }else if( im==3 || im==4 ){
          ibrl_height  = ibst_height;
        }

        double  ibrl_top      = ibrl_bottom;
        if( im<3 && ( l==2 || l==4 || l==6 ) ){
          ibrl_top   = ibrl_bottom + ibrl_height*tan( 2*M_PI/32);
        }else if( im>=3 && l==2 ){
          ibrl_top  = ibar_bot_top_wrail - 2*ibrl_center_height*tan( 2*M_PI/32 );
        }else if( im>=3 || l==2 || l==4 || l==6 ){
          ibrl_top   = ibrl_bottom + ibrl_height*tan( 2*M_PI/32);
        }

        double  ibrl_trap_th  = -atan((ibrl_top - ibrl_bottom)/2/ibrl_height );
        if( im<3 && (l==3 || l==5) ){
          ibrl_trap_th = 0;
        }else if( im<3 && l==2 ){
          ibrl_trap_th  = atan((ibrl_top - ibrl_bottom)/2/ibrl_height );
        }else if( im>=3 && l==2 ){
          ibrl_trap_th  = 0;
        }else if( im>=3 ){
          ibrl_trap_th  = - atan((ibrl_top - ibrl_bottom)/2/ibrl_height );
        }
      
        paramVec.clear();
        paramVec.push_back(ibrl_height*cm);
        paramVec.push_back(ibrl_trap_th*rad);
        paramVec.push_back(0*rad);
        paramVec.push_back(ibrl_len_top*cm);
        paramVec.push_back(ibrl_top*cm);
        paramVec.push_back(ibrl_top*cm);
        paramVec.push_back(0*deg);
        paramVec.push_back(ibrl_len_bot*cm);
        paramVec.push_back(ibrl_bottom*cm);
        paramVec.push_back(ibrl_bottom*cm);
        paramVec.push_back(0*deg);


        double  ibrl_x = - (ibar_top + ibar_bottom)/4 
                         + (ibrl_bottom+ibrl_top)/4;
        double  ibrl_y = ibrl_hpos; 
        double  ibrl_z = -ibar_bot_height/2 + ibrl_height/2;
        if( im<3 && l==2 ){
          ibrl_x = - ( ibar_bot_top_wrail - ibrl_height*tan( 2*M_PI/32) )/2
                   + ( ibrl_bottom + ibrl_top )/4;
        }else if( im<3 && (l==3 || l==5) ){
          ibrl_x = - (ibar_top_wrail + ibar_bottom)/4 
                   + (ibrl_bottom+ibrl_top)/4;
        }else if( im<3 && (l==4 || l==6) ){
          ibrl_x = - (ibar_top_wrail + ibar_bottom)/4
	    + (ibrl_bottom+ibrl_top)/4 + ibr3_top - ibrl_ywidth + ibar_sidegap-0.02;
	  //HNDEBUG 0.02*cm = 0.2mm to fix geometry overlap

        }else if( im>=3 && l==2 ){
          ibrl_x = 0;
        }else if( im>=3 ){
          ibrl_x = - (ibar_top_wrail + ibar_bottom)/4 + (ibrl_bottom+ibrl_top)/4;
        }

        if( im>=3 ){
          ibrl_z = -ibar_bot_height/2 + ibrl_center_height + ibrl_height/2;
        }


        std::sprintf(nam,"IBRL%01d%01d",l,im);
        trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
                  G4ThreeVector(ibrl_x*cm,ibrl_y*cm,ibrl_z*cm),
                  G4ThreeVector(0,0,0)
                  );
        trap[l]->setOuterMaterial("G4_Al");
        trap[l]->setParameters(paramVec);
        if( im<3 ){
          trap[l]->setOuterColor("gray70");
        }else{
          trap[l]->setOuterColor("green");
        }
        trapIBAR[l]->addDaughter(trap[l]);
        m_detList.push_back(trap[l]);
      }
    }

  // End loop for
  // Rib1~3 for 8-Rings (Rib1= Ring bottom, Rib2=Web, Rib3 = Ring top),
  // Earthquake bracket, Teflon Pad, Rail and Stopper
  }
  

  // Front Plate
  double  ibfp_len_top  = ibar_len_bot;
  double  ibfp_len_bot  = ibfp_len_top;
  for( int l=0;l<7;l++ ){ //0...normal, 1...top module, 2...bottom module
                          //3(,5)...south (north) right angle,
                          //4,(6)...south (north) acute angle
    double  ibfp_top      = ibdt_bottom;
    double  ibfp_bottom   = ibfp_top - ibfp_height*tan( 2*M_PI/32 );
    if( l==1 ){
      ibfp_top   = ibdt_top;
      ibfp_bottom   = ibfp_top;
    }else if( l==2 ){
      ibfp_bottom = ibar_bot_bottom;
      ibfp_top = ibfp_bottom + 2*ibfp_height*tan( 2*M_PI/32 );
    }
    double  ibfp_trap_th  = -atan( (ibfp_top-ibfp_bottom)/2/ibfp_height );

    paramVec.clear();
    paramVec.push_back(ibfp_height*cm);
    paramVec.push_back(ibfp_trap_th*rad);
    paramVec.push_back(0*rad);
    paramVec.push_back(ibfp_len_top*cm);
    paramVec.push_back(ibfp_top*cm);
    paramVec.push_back(ibfp_top*cm);
    paramVec.push_back(0*deg);
    paramVec.push_back(ibfp_len_bot*cm);
    paramVec.push_back(ibfp_bottom*cm);
    paramVec.push_back(ibfp_bottom*cm);
    paramVec.push_back(0*deg);

  
    double  ibfp_x = (ibfp_bottom+ibfp_top)/4 - (ibar_top + ibar_bottom)/4 + ibar_sidegap;
    double  ibfp_y = 0;
    double  ibfp_z = ibar_height/2 - ibfp_height/2;
    if( l==1 ){
      ibfp_x = 0;
    }else if( l==2 ){
      ibfp_x = 0;
      ibfp_z = ibar_bot_height/2 - ibfp_height/2;
    }else if( l>2 ){
      ibfp_x = (ibfp_bottom+ibfp_top)/4 - (ibar_top_wrail + ibar_bottom)/4 + ibar_sidegap;
      ibfp_z = ibar_bot_height/2 - ibfp_height/2;
    }

    std::sprintf(nam,"IBFP%01d",l);
    trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
	      G4ThreeVector(ibfp_x*cm,ibfp_y*cm,ibfp_z*cm),
	      G4ThreeVector(0,0,0)
	      );
    trap[l]->setOuterMaterial("G4_Fe");
    trap[l]->setParameters(paramVec);
    trap[l]->setOuterColor("gray70");
    trapIBAR[l]->addDaughter(trap[l]);
    m_detList.push_back(trap[l]);

  }
  
  
  // Layer structure
  {
    double  ibar_lead_thick = 0.1;
    int     ibar_nlead      = 24;
    double  ibar_sci_thick   = 0.5;
    double  ibar_sci_sheet   = 0.0188;
    
    for( int l=0;l<7;l++ ){ //0...normal, 1...top module, 2...bottom module
                            //3(,5)...south (north) right angle,
                            //4,(6)...south (north) acute angle
      // Scintillator
      for( int i=0;i<ibar_nlead+1;i++ ){
        double  ibsc_height   = ibar_sci_thick + ibar_sci_sheet*2;
        double  ibsc_hpos     = ibsc_height*i + ibar_sci_sheet + ibar_lead_thick*i;
        double  ibsc_len_bot  = ibar_len_bot 
                                + (ibar_len_top - ibar_len_bot) * (ibsc_hpos / ibar_height);
        double  ibsc_len_top  = ibsc_len_bot 
                                + (ibar_len_top - ibar_len_bot) * (ibar_sci_thick / ibar_height);
        double  ibsc_bottom   = ibdt_bottom + ibsc_hpos  *tan( 2*M_PI/32 );
        double  ibsc_top      = ibsc_bottom + ibar_sci_thick*tan( 2*M_PI/32 );
        if( l==1 ){
              ibsc_bottom   = ibdt_top;
              ibsc_top      = ibdt_top;
        }else if( l==2 ){
              ibsc_bottom   = ibdt_bottom - (ibdt_top - ibdt_bottom)
                              + 2*ibsc_hpos*tan( 2*M_PI/32 );
              ibsc_top      = ibsc_bottom
                              + 2*ibar_sci_thick*tan( 2*M_PI/32 );
        }
        double  ibsc_trap_th  = -atan( (ibsc_top-ibsc_bottom)/2/ibar_sci_thick );
      
        paramVec.clear();
        paramVec.push_back(ibar_sci_thick*cm);
        paramVec.push_back(ibsc_trap_th*rad);
        paramVec.push_back(0*rad);
        paramVec.push_back(ibsc_len_top*cm);
        paramVec.push_back(ibsc_top*cm);
        paramVec.push_back(ibsc_top*cm);
        paramVec.push_back(0*deg);
        paramVec.push_back(ibsc_len_bot*cm);
        paramVec.push_back(ibsc_bottom*cm);
        paramVec.push_back(ibsc_bottom*cm);
        paramVec.push_back(0*deg);

      
        double  ibsc_x = (ibsc_bottom+ibsc_top)/4 - (ibar_top + ibar_bottom)/4 + ibar_sidegap;
        double  ibsc_y = 0;
        double  ibsc_z = ibar_height/2 - ibsc_hpos - ibar_sci_thick/2 - ibfp_height;
        if( l==1 ){
          ibsc_x = 0;
        }else if( l==2 ){
          ibsc_x = 0;
          ibsc_z = ibar_bot_height/2 - ibsc_hpos - ibar_sci_thick/2 - ibfp_height;
        }else if( l>2 ){
          ibsc_x = (ibsc_bottom+ibsc_top)/4 - (ibar_top_wrail + ibar_bottom)/4 + ibar_sidegap;
          ibsc_z = ibar_bot_height/2 - ibsc_hpos - ibar_sci_thick/2 - ibfp_height;
        }

        std::sprintf(nam,"IBSC%01d%02d",l,i);
        trap[l] = new GsimTrap(std::string(nam),trapIBAR[l],
			  G4ThreeVector(ibsc_x*cm,ibsc_y*cm,ibsc_z*cm),
			  G4ThreeVector(0,0,0)
			  );
        trap[l]->setParameters(paramVec);
//        trap[l]->setOuterVisibility(false); //comment out for test
        trap[l]->setOuterColor("yellow");
        trap[l]->setSensitiveDetector("IBAR");
        trapIBAR[l]->addDaughter(trap[l]);
      }

      // LEAD plate
      for( int i=0;i<ibar_nlead;i++ ){
        double  ibld_hschinti = (ibar_sci_thick + ibar_sci_sheet*2);
        double  ibld_height   = ibar_lead_thick;
        double  ibld_hpos     = 0. + ibld_hschinti*(i+1)
                                + ibar_lead_thick * i;
        double  ibld_len_bot  = ibar_len_bot 
          + (ibar_len_top - ibar_len_bot) * (ibld_hpos / ibar_height);
        double  ibld_bottom   = ibdt_bottom + ibld_hpos*tan( 2*M_PI/32 );
        if( l==1 ){
              ibld_bottom   = ibdt_top;
        }else if( l==2 ){
              ibld_bottom   = ibdt_bottom - ( ibdt_top - ibdt_bottom )
                              + 2*ibld_hpos  *tan( 2*M_PI/32 );
        }
      
        paramVec.clear();
        paramVec.push_back(ibld_bottom*cm);
        paramVec.push_back(ibld_len_bot*cm);
        paramVec.push_back(ibld_height*cm);

        double  ibld_x = ibld_bottom/2 - (ibar_top + ibar_bottom)/4 + ibar_sidegap;
        double  ibld_y = 0;
        double  ibld_z = ibar_height/2 - ibld_hpos - ibld_height/2 - ibfp_height;
        if( l==1 ){
          ibld_x = 0;
        }else if( l==2 ){
          ibld_x = 0;
          ibld_z = ibar_bot_height/2 - ibld_hpos - ibld_height/2 - ibfp_height;
        }else if( l>2 ){
          ibld_x = ibld_bottom/2 - (ibar_top_wrail + ibar_bottom)/4 + ibar_sidegap;
          ibld_z = ibar_bot_height/2 - ibld_hpos - ibld_height/2 - ibfp_height;
        }

        std::sprintf(nam,"IBLD%01d%02d",l,i);
        GsimBox *box = new GsimBox(std::string(nam),trapIBAR[l],
                       G4ThreeVector(ibld_x*cm,ibld_y*cm,ibld_z*cm),
                       G4ThreeVector(0,0,0)
                       );
        box->setOuterMaterial("G4_Pb");
        box->setParameters(paramVec);
        box->setOuterColor("gray50");
        trapIBAR[l]->addDaughter(box);
        m_detList.push_back(box);
      }
    }

  }

  GsimSensitiveDetector* sd =
    dynamic_cast<GsimSensitiveDetector*>(GsimUtil::getUtil()->getSensitiveDetector("IBAR"));
  sd->setThisCopyNoFactor(32);
  setThisAndDaughterBriefName("IBAR");

}


void GsimE14IBAR::comment()
{
  std::cout << "GsimE14IBAR Z position:" << std::endl;
  std::cout << "    Place IBAR center 227.5*mm downstream of CBAR center." << std::endl;
}


void GsimE14IBAR::setFastSimulationLevel(int level)
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
    GsimSensitiveDetector* sd =
      dynamic_cast<GsimSensitiveDetector*>(GsimUtil::getUtil()->getSensitiveDetector("IBAR"));
    sd->setThisCopyNoFactor(32);
  }

  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
        it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("IBAR");
    }
    GsimSensitiveDetector* sd =
      dynamic_cast<GsimSensitiveDetector*>(GsimUtil::getUtil()->getSensitiveDetector("IBAR"));
    sd->setThisCopyNoFactor(-1);
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




GsimE14IBAR::~GsimE14IBAR()
{
  ;
}

