/**
 *  @file
 *  @brief  GsimE14Membrane
 *  $Id: GsimE14Membrane.cc,v 1.4 2007/02/04 12:56:55 nanjo Exp $ 
 *  $Log: GsimE14Membrane.cc,v $
 *  Revision 1.4  2007/02/04 12:56:55  nanjo
 *  GsimBoxWithAHole is used.
 *
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14Membrane.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimE14Detector/GsimE14BHCV.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimDetector/GsimTubeWithAHole.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimTrapWithHoles.h"
#include "GsimDetector/GsimBoxWithAHole.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14Membrane::GsimE14Membrane(std::string name,
				 GsimDetector* motherDetector,
				 G4ThreeVector transV,G4ThreeVector rotV,
				 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14Membrane";

  if(
     ! (	
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
	       "Invalid user flag is assigned. Membrane is not constructed.");
    return;
  }
  
  // Membrane
  int model=1;

  if(m_userFlag==2011) {
    model=0;
  }

  /////////////////////////////////////////
  //model==1
  /////////////////////////////////////////
  //m_userFlag<201504
  //   upstream beam window : 125um
  //else
  //   upstream beam window : 12.5um
  //
  /////////////////////////////////////////
  //m_userFlag>=201605
  //   Vertical Membrane in front of the CV
  //   Outer radius : 1490*mm/2.;
  //   Downstream items should be moved to +2 cm
  // else
  //   Outer radius : 1986*mm/2.;
  //
  /////////////////////////////////////////
  
  
  if(model==1) {
    
    // Upstream
    //     Flange1
    //     Vpipe
    //     Flange2
    //     in-beam window Kapton 0.125t
    //     Square pipe to NCC upstream 139-139.4mm
    //     Square G10 pipex2 at NCC 130-131mm
    //     Square pip inside NCC
    // Downstream
    //     Vertical membrane
    //     Horizontal membrane at CV
    //     CsI  G10
    //     CC04 G10
    //     Pipe SUS
    //     in-beam membrane SUS 0.2t
    //     Flange 20t thick x2
    //     membrane pipe from CC04
    //     
    //

    
    //Information from Watanabe
    //kds.kek.jp/getFile.py/access?resId=0&materialId=5&confId=7028
    //Upstream geometry
    //202002 with prototypeUCV
    //202102 with fiberUCV (geometry is not correct with UCV chamber)
    //202306 with filmUCV (geometry is corrected)
    
    G4double usmFlange1_R=300*mm/2.;
    G4double usmFlange1_L=16*mm;
    G4double usmFlange2_R=400*mm/2.;
    G4double usmFlange2_L=40*mm;
    G4double usmFlange_Rin=0;
    G4double usmDuct_L=0;
    G4double usmDuct_R=0;
    G4double usmMbnShift=0;
    G4double bellows_L = 122*mm;

    G4double usmFlange0_R=0;
    G4double usmFlange0_L=0;
    if(m_userFlag>=202306) {
      //usmFlange1 with gateValve
      usmFlange1_R=300*mm/2.;
      usmFlange1_L=112*mm;
      usmFlange2_R=400*mm/2.;
      usmFlange2_L=80*mm;
      usmFlange_Rin=206.3*mm/2;

      //duct between the two flange
      usmDuct_L=80*mm;
      usmDuct_R=216.2*mm/2.;

      //upstream shift in usmFlagne2
      usmMbnShift=20*mm;

      bellows_L = 76*mm;

      usmFlange0_R=335*mm/2.;
      usmFlange0_L=10*mm;
    }

    G4double usmVPipe_XY=190*mm;
    G4double usmVPipe_L=295*mm-usmFlange1_L-usmFlange2_L;
    
    G4double usm_thick=0.0125*mm;
    if(m_userFlag>0 && m_userFlag<201504) {
      usm_thick=0.125*mm;
    }
    
    G4double usm_XY=152.*mm;
    G4double usm_Z=-500*mm;

    G4double dsPipe_L=1515*mm;
    if(m_userFlag>0 && m_userFlag<201504) {
      dsPipe_L=1555*mm;
    }

    G4double dsm_thick=0.2*mm;
    G4double dsm_R=280*mm/2.;
    G4double dsm_Z=Csic_Zmin*cm+(500+980+210+735)*mm+dsPipe_L;
    //DS Pipe start from
    // 500+980+210+735=2425 from CSI UPS by Watanabe's pdf
    // 1800 + 625=2425 from CSI UPS also by CAD and Watanabe's pdf
    //G4double dsm_Z=Csic_Zmin*cm+500*mm+3480.*mm;
    if(m_userFlag>=201605) {
      dsm_Z+=2*cm;
    }

    G4double dsmFlange_R=400*mm/2.;
    G4double dsmFlange_thick=20*mm;
    
    //=3980
    //=2425+1555=3980

    G4double dsmuFlange_R=575*mm/2.;
    G4double dsmuFlange_thick=20*mm;
    G4double dsmu_Z=Csic_Zmin*cm+2445*mm;
    if(m_userFlag>=201605) {
      dsmu_Z+=2*cm;
    }

    
    std::vector<double> paramVec;
    if(m_userFlag<202102) {
	
      //Upstram
      //Flange
      //   phi = 300*mm  16mm thick with 152x152 square hole
      //Pipe
      //   190x190 (295-40-16) long with 152x152 square hole
      //Flange
      //   phi = 400*mm  40mm thick with 152x152 square hole
    

      //Upstream
      // Flange2
      paramVec.clear();
      paramVec.push_back(usmFlange2_R);
      paramVec.push_back(usmFlange2_L);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(usm_XY);
      paramVec.push_back(usm_XY);

      GsimTubeWithAHole* usmFlange2 = new GsimTubeWithAHole("usmFlange2",this,
							    G4ThreeVector(0,0,usm_Z+2*mm),
							    G4ThreeVector(0,0,0));
      usmFlange2->setParameters(paramVec);
      usmFlange2->setOuterMaterial("GsimSUS");
      usmFlange2->setOuterColor("gray70");
      addDaughter(usmFlange2);


	
	
      //Upstream
      // Flange1
      paramVec.clear();
      paramVec.push_back(usmFlange1_R);
      paramVec.push_back(usmFlange1_L);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(usm_XY);
      paramVec.push_back(usm_XY);

      GsimTubeWithAHole* usmFlange1 = new GsimTubeWithAHole("usmFlange1",this,
							    G4ThreeVector(0,0,usm_Z+2*mm
									  -usmFlange2_L/2.
									  -usmVPipe_L
									  -usmFlange1_L/2.),
							    G4ThreeVector(0,0,0));
      usmFlange1->setParameters(paramVec);
      usmFlange1->setOuterMaterial("GsimSUS");
      usmFlange1->setOuterColor("gray70");
      addDaughter(usmFlange1);
    } else if (m_userFlag<202306) {
      //userFlag==202102
      //fiberUCV but upstream membrane geometry is not correct
      //Upstream
      // Flange1 (Flange1 is moved to downstream direction)
      paramVec.clear();
      paramVec.push_back(200.0*mm/2);
      paramVec.push_back(usmFlange1_R);
      paramVec.push_back(usmFlange1_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmFlange1 = new GsimTube("usmFlange1",this,
					  G4ThreeVector(0,0,usm_Z+2*mm + usmFlange2_L/2 -136*mm + usmFlange1_L/2.),
					  G4ThreeVector(0,0,0));
      usmFlange1->setParameters(paramVec);
      usmFlange1->setOuterMaterial("GsimSUS");
      usmFlange1->setOuterColor("green");
      addDaughter(usmFlange1);


      //Upstream
      // Flange2 
      paramVec.clear();
      paramVec.push_back(usmFlange2_R);
      paramVec.push_back(usmFlange2_L);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(usm_XY);
      paramVec.push_back(usm_XY);
	
      GsimTubeWithAHole* usmFlange2 = new GsimTubeWithAHole("usmFlange2",this,
							    G4ThreeVector(0,0,usm_Z+2*mm),
							    G4ThreeVector(0,0,0));
      usmFlange2->setParameters(paramVec);
      usmFlange2->setOuterMaterial("GsimSUS");
      usmFlange2->setOuterColor("green");
      addDaughter(usmFlange2);

      // Conversion duct body
      paramVec.clear();
      paramVec.push_back(203.3*mm/2);
      paramVec.push_back(216.3*mm/2);
      paramVec.push_back((136.0-16-40)*mm);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmduct = new GsimTube("usmduct",this,
				       G4ThreeVector(0,0,usm_Z+2*mm + usmFlange2_L/2 -(136-16+40)*mm/2),
				       G4ThreeVector(0,0,0));
      usmduct->setParameters(paramVec);
      usmduct->setOuterMaterial("GsimSUS");
      usmduct->setOuterColor("green");
      addDaughter(usmduct);


      // Bellows
      // Bellows downstream flange
      paramVec.clear();
      paramVec.push_back(180.0*mm/2);
      paramVec.push_back(300.0*mm/2);
      paramVec.push_back(16.0*mm);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmBellows_down = new GsimTube("usmBellows_down",this,
					       G4ThreeVector(0,0,usm_Z+2*mm + usmFlange2_L/2 -136*mm - 16.0*mm/2),
					       G4ThreeVector(0,0,0));
      usmBellows_down->setParameters(paramVec);
      usmBellows_down->setOuterMaterial("GsimSUS");
      usmBellows_down->setOuterColor("gray70");
      addDaughter(usmBellows_down);


      // Bellows upstream flange
      paramVec.clear();
      paramVec.push_back(180.0*mm/2);
      paramVec.push_back(384.0*mm/2);
      paramVec.push_back(12*mm);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmBellows_up = new GsimTube("usmBellows_up",this,
					     G4ThreeVector(0,0,usm_Z+2*mm + usmFlange2_L/2 -136*mm - bellows_L + 12.0*mm/2),
					     G4ThreeVector(0,0,0));
      usmBellows_up->setParameters(paramVec);
      usmBellows_up->setOuterMaterial("GsimSUS");
      usmBellows_up->setOuterColor("gray70");
      addDaughter(usmBellows_up);


      // Bellows  
      paramVec.clear();
      paramVec.push_back(180.0*mm/2);
      paramVec.push_back(210.0*mm/2);
      paramVec.push_back(bellows_L-16*mm - 12*mm);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmBellows = new GsimTube("usmBellows",this,
					  G4ThreeVector(0,0,usm_Z+2*mm + usmFlange2_L/2 -136*mm - (bellows_L-12*mm+16*mm)/2 ),
					  G4ThreeVector(0,0,0));
      usmBellows->setParameters(paramVec);
      usmBellows->setOuterMaterial("GsimSUS");
      usmBellows->setOuterColor("gray70");
      addDaughter(usmBellows);

    } else {
      //m_userFlag>=202306
      //filmUCV
      // Flange2 
      paramVec.clear();
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usmFlange2_R);
      paramVec.push_back(usmFlange2_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);
      GsimTube* usmFlange2 = new GsimTube("usmFlange2",this,
					  G4ThreeVector(0,0,usm_Z+usmMbnShift),
					  G4ThreeVector(0,0,0));
      usmFlange2->setParameters(paramVec);
      usmFlange2->setOuterMaterial("GsimSUS");
      usmFlange2->setOuterColor("green");
      addDaughter(usmFlange2);

      //Upstream Flange1
      paramVec.clear();
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usmFlange1_R);
      paramVec.push_back(usmFlange1_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmFlange1 = new GsimTube("usmFlange1",this,
					  G4ThreeVector(0,0,
							usm_Z+usmMbnShift
							- usmFlange2_L/2
							- usmDuct_L
							- usmFlange1_L/2.),
					  G4ThreeVector(0,0,0));
      usmFlange1->setParameters(paramVec);
      usmFlange1->setOuterMaterial("GsimSUS");
      usmFlange1->setOuterColor("green");
      addDaughter(usmFlange1);


      // Conversion duct body
      paramVec.clear();
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usmDuct_R);
      paramVec.push_back(usmDuct_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmduct = new GsimTube("usmduct",this,
				       G4ThreeVector(0,0,
						     usm_Z+usmMbnShift
						     -usmFlange2_L/2
						     -usmDuct_L/2.),
				       G4ThreeVector(0,0,0));
      usmduct->setParameters(paramVec);
      usmduct->setOuterMaterial("GsimSUS");
      usmduct->setOuterColor("green");
      addDaughter(usmduct);


      // Bellows 
      paramVec.clear();
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usmDuct_R);
      paramVec.push_back(bellows_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmBellows = new GsimTube("usmBellows",this,
					  G4ThreeVector(0,0,
							usm_Z+usmMbnShift
							-usmFlange2_L/2
							-usmDuct_L
							-usmFlange1_L
							- bellows_L/2 ),
					  G4ThreeVector(0,0,0));
      usmBellows->setParameters(paramVec);
      usmBellows->setOuterMaterial("GsimSUS");
      usmBellows->setOuterColor("gray70");
      addDaughter(usmBellows);


      //Upstream Flange0
      paramVec.clear();
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usmFlange0_R);
      paramVec.push_back(usmFlange0_L);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);

      GsimTube* usmFlange0 = new GsimTube("usmFlange0",this,
					  G4ThreeVector(0,0,
							usm_Z+usmMbnShift
							- usmFlange2_L/2
							- usmDuct_L
							- usmFlange1_L
							- bellows_L
							- usmFlange0_L/2.
							),
					  G4ThreeVector(0,0,0));
      usmFlange0->setParameters(paramVec);
      usmFlange0->setOuterMaterial("GsimSUS");
      usmFlange0->setOuterColor("green");
      addDaughter(usmFlange0);
    }


    // Z0 chamber
  
    if(m_userFlag<202002) {
    	
      paramVec.clear();
      paramVec.push_back(45*deg);
      paramVec.push_back(360*deg);
      paramVec.push_back(4);
      paramVec.push_back(usmVPipe_L);
      paramVec.push_back(usm_XY/2.);
      paramVec.push_back(usmVPipe_XY/2.);

      GsimPolyhedra2* usmVPipe = new GsimPolyhedra2("usmVPipe",this,
						    G4ThreeVector(0,0,usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
						    G4ThreeVector(0,0,0));
      usmVPipe->setParameters(paramVec);
      usmVPipe->setOuterMaterial("GsimSUS");
      usmVPipe->setOuterColor("gray70");
      addDaughter(usmVPipe);

    }else if(m_userFlag<202102){

      double thick_pipe = (usmVPipe_XY-usm_XY)/2;
      // side
      paramVec.clear();
      paramVec.push_back(thick_pipe);
      paramVec.push_back(usm_XY);
      paramVec.push_back(usmVPipe_L);
    
      GsimBox* usmVPipe_sidesn = new GsimBox("usmVPipe_siden",this,
					     G4ThreeVector((usm_XY/2 + thick_pipe/2),0,usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
					     G4ThreeVector(0,0,0));

      usmVPipe_sidesn->setParameters(paramVec);
      usmVPipe_sidesn->setOuterMaterial("GsimSUS");
      usmVPipe_sidesn->setOuterColor("gray70");
      addDaughter(usmVPipe_sidesn);

      GsimBox* usmVPipe_sidess = new GsimBox("usmVPipe_sides",this,
					     G4ThreeVector(-(usm_XY/2 + thick_pipe/2),0,usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
					     G4ThreeVector(0,0,0));

      usmVPipe_sidess->setParameters(paramVec);
      usmVPipe_sidess->setOuterMaterial("GsimSUS");
      usmVPipe_sidess->setOuterColor("gray70");
      addDaughter(usmVPipe_sidess);

      paramVec.clear();
      paramVec.push_back(usm_XY);
      paramVec.push_back(thick_pipe);
      paramVec.push_back(usmVPipe_L);
    
      // bottom
      GsimBox* usmVPipe_sidesb = new GsimBox("usmVPipe_sideb",this,
					     G4ThreeVector(0, -(usm_XY/2 + thick_pipe/2),usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
					     G4ThreeVector(0,0,0));

      usmVPipe_sidesb->setParameters(paramVec);
      usmVPipe_sidesb->setOuterMaterial("GsimSUS");
      usmVPipe_sidesb->setOuterColor("gray70");
      addDaughter(usmVPipe_sidesb);


      // top
      paramVec.clear();
      paramVec.push_back(usm_XY);
      paramVec.push_back(usmVPipe_L);
      paramVec.push_back(thick_pipe);
      paramVec.push_back(110*mm);
      paramVec.push_back(110*mm);
      paramVec.push_back(0*mm);
      paramVec.push_back(0.5*mm);
      paramVec.push_back(0*mm);
    
      GsimBoxWithoutABox* usmVPipe_sidest = new GsimBoxWithoutABox("usmVPipe_sidet",this,
								   G4ThreeVector(0, (usm_XY/2 + thick_pipe/2), usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
								   G4ThreeVector(90*deg,0,0));

      usmVPipe_sidest->setParameters(paramVec);
      usmVPipe_sidest->setOuterMaterial("GsimSUS");
      usmVPipe_sidest->setOuterColor("gray70");
      addDaughter(usmVPipe_sidest);

      // box for evacuation area for Z0 and UCV

      paramVec.clear();
      paramVec.push_back(124*mm);
      paramVec.push_back(124*mm);
      paramVec.push_back(90*mm);
      paramVec.push_back(110*mm);
      paramVec.push_back(110*mm);
      paramVec.push_back(0*mm);
      paramVec.push_back(0.0*mm);
      paramVec.push_back(0*mm);
    
      GsimBoxWithoutABox* usmVPipe_box = new GsimBoxWithoutABox("usmVPipe_box",this,
								G4ThreeVector(0, usm_XY/2 + thick_pipe+90*mm/2, -0.5*mm+usm_Z+2*mm-usmFlange2_L/2.-usmVPipe_L/2),
								G4ThreeVector(90*deg,0,0));

      usmVPipe_box->setParameters(paramVec);
      usmVPipe_box->setOuterMaterial("GsimSUS");
      usmVPipe_box->setOuterColor("gray70");
      addDaughter(usmVPipe_box);

      
    } else {
      // 2021- with new UCV and new UCV box
      // 202102 geometry is wrong
      // 202306 geometry is corrected
      double z_width = 232*mm;
      double z_thick = 20*mm;
      double usm_z_2021 = usm_Z+2*mm + usmFlange2_L/2 -136*mm - bellows_L; // downstream surface of Bellows
      if (m_userFlag>=202306) {
	usm_z_2021 = usm_Z+usmMbnShift
	  - usmFlange2_L/2
	  - usmDuct_L
	  - usmFlange1_L
	  - bellows_L
	  - usmFlange0_L;
      }
      
      // downstream and upstream sides	
      paramVec.clear();
      paramVec.push_back(520*mm);
      paramVec.push_back(360*mm);
      paramVec.push_back(z_thick*mm);
      paramVec.push_back(0*mm);
      paramVec.push_back(0*mm);
      paramVec.push_back(125*mm);
   	
      GsimBoxWithAHole* usmChamber_upstream = new GsimBoxWithAHole("usmChamber_upstream",this,
								   G4ThreeVector(0, 0, usm_z_2021 -z_thick/2),
								   G4ThreeVector(0,0,0));

      usmChamber_upstream->setParameters(paramVec);
      usmChamber_upstream->setOuterMaterial("GsimSUS");
      usmChamber_upstream->setOuterColor("gray70");
      addDaughter(usmChamber_upstream);

      usmChamber_upstream->cloneDetector(G4ThreeVector(0,0, usm_z_2021 -z_width+z_thick/2), G4ThreeVector(0,0,0));


      // top and bottom sides
      paramVec.clear();
      paramVec.push_back(520*mm);
      paramVec.push_back(12*mm);
      paramVec.push_back(z_width);
    
      GsimBox* usmChamber_top = new GsimBox("usmChamber_top",this,
					    G4ThreeVector(0, 180*mm+12*mm/2, usm_z_2021 -z_width/2),
					    G4ThreeVector(0,0,0));

      usmChamber_top->setParameters(paramVec);
      usmChamber_top->setOuterMaterial("GsimSUS");
      usmChamber_top->setOuterColor("gray70");
      addDaughter(usmChamber_top);
      usmChamber_top->cloneDetector(G4ThreeVector(0, -180*mm-12*mm/2, usm_z_2021 -z_width/2), G4ThreeVector(0,0,0));

      // south side
      paramVec.clear();
      paramVec.push_back(12*mm);
      paramVec.push_back(384*mm);
      paramVec.push_back(z_width);
    
      GsimBox* usmChamber_south = new GsimBox("usmChamber_south",this,
					      G4ThreeVector(-260*mm-12.0*mm/2, 0, usm_z_2021 -z_width/2),
					      G4ThreeVector(0,0,0));

      usmChamber_south->setParameters(paramVec);
      usmChamber_south->setOuterMaterial("GsimSUS");
      usmChamber_south->setOuterColor("gray70");
      addDaughter(usmChamber_south);

      paramVec.clear();
      paramVec.push_back(20*mm);
      paramVec.push_back(384*mm);
      paramVec.push_back(z_width);
      GsimBox* usmChamber_north = new GsimBox("usmChamber_north",this,
					      G4ThreeVector(260*mm+20.0*mm/2, 0, usm_z_2021 -z_width/2),
					      G4ThreeVector(0,0,0));

      usmChamber_north->setParameters(paramVec);
      usmChamber_north->setOuterMaterial("G4_Al");
      usmChamber_north->setOuterColor("red");
      addDaughter(usmChamber_north);
    }


    
    //Upstram
    // in-beam window
    if(m_userFlag<202306) {
      paramVec.clear();
      paramVec.push_back(usm_XY);
      paramVec.push_back(usm_XY);
      paramVec.push_back(usm_thick);
      
      GsimBox* usmbn = new GsimBox("usmbn",this,
				   G4ThreeVector(0,0,usm_Z-usm_thick/2.),
				   G4ThreeVector(0,0,0));
      
      usmbn->setParameters(paramVec);
      usmbn->setOuterMaterial("G4_KAPTON");
      usmbn->setOuterColor("red");
      addDaughter(usmbn);
    } else {
      paramVec.clear();
      paramVec.push_back(0);
      paramVec.push_back(usmFlange_Rin);
      paramVec.push_back(usm_thick);
      paramVec.push_back(0);
      paramVec.push_back(2*M_PI*rad);
      
      GsimTube* usmbn = new GsimTube("usmbn",this,
				     G4ThreeVector(0,0,usm_Z-usm_thick/2.),
				     G4ThreeVector(0,0,0));
      
      usmbn->setParameters(paramVec);
      usmbn->setOuterMaterial("G4_KAPTON");
      usmbn->setOuterColor("red");
      addDaughter(usmbn);
    }

    //Upstream membrane pipe
    //usm_z <-> NCC LCV upstream end
    //139 sqaure membrane pipe
    //150mm-139mm prismoid in real geometry
    // (LCV innder d=137.2mm)
    G4double lNCCCFRP=446*mm;
    double usmpipeZ2=(Fbar_Len-Detpos_ncc_z_shift-NCC_Len/2.)*cm-lNCCCFRP/2.-3*mm;
      
    paramVec.clear();
    paramVec.push_back(139*mm+2.*0.2*mm);
    paramVec.push_back(139*mm+2.*0.2*mm);
    paramVec.push_back(usmpipeZ2-usm_Z);
    paramVec.push_back(139*mm);
    paramVec.push_back(139*mm);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(0);
    GsimBoxWithoutABox* usmPipe
      = new GsimBoxWithoutABox("usmPipe",this,
			       G4ThreeVector(0,0,(usm_Z+usmpipeZ2)/2.),
			       G4ThreeVector(0,0,0));
    usmPipe->setParameters(paramVec);
    usmPipe->setOuterMaterial("G4_POLYETHYLENE");
    usmPipe->setOuterColor("yellow");
    addDaughter(usmPipe);


    //               |usmpipeZ2                     
    //              \|/
    //   ============= 3mm for additional sciniti  
    //     ^        ||<->
    //     |        || 
    //    139       ||===================
    //                         ^
    //                         | 
    //                        131
    //--------------------------------
    //Vertical membrane parts
    //        139-4  4
    //    <--------><-
    //    ==========||
    //    ||  131   ||
    //    ||        ||
    //    ||        ||
    //    ||=========
    //
    //
    

    double usmVZ1=usmpipeZ2-0.2*mm;
    double usmVZ2=usmpipeZ2;
    double usmVX=(139.-4.)*mm;
    double usmVY=4.*mm;

    double usmVposX=usmVY/2.;
    double usmVposY=((usmVX+usmVY)/2.-usmVY/2.);
    double usmVposZ=(usmVZ2+usmVZ1)/2.;
    paramVec.clear();
    paramVec.push_back(usmVX);
    paramVec.push_back(usmVY);
    paramVec.push_back(usmVZ2-usmVZ1);
    GsimBox* usmV = new GsimBox("usmV",this,
				G4ThreeVector(-usmVposX,+usmVposY,usmVposZ),
				G4ThreeVector(0,0,0));

    usmV->setParameters(paramVec);
    usmV->setOuterMaterial("G4_POLYETHYLENE");
    usmV->setOuterColor("yellow");
    usmV->cloneDetector(G4ThreeVector(+usmVposY,+usmVposX,usmVposZ),
			G4ThreeVector(0,0,90*deg));
    usmV->cloneDetector(G4ThreeVector(+usmVposX,-usmVposY,usmVposZ),
			G4ThreeVector(0,0,0));
    usmV->cloneDetector(G4ThreeVector(-usmVposY,-usmVposX,usmVposZ),
			G4ThreeVector(0,0,90*deg));
    addDaughter(usmV);

    

    //inside-NCC membrane pipe
    //LCV innder d = 137.2
    //G10 outer  d = 131.0
    // -> 131-132

    //double inNCCMBN_Z1=(Fbar_Len-Detpos_ncc_z_shift-NCC_Len)*cm;
    double inNCCMBN_Z1=usmpipeZ2;
    double inNCCMBN_Z2=(Fbar_Len-Detpos_ncc_z_shift)*cm;
    
    paramVec.clear();
    paramVec.push_back(131*mm+2.*0.2*mm);
    paramVec.push_back(131*mm+2.*0.2*mm);
    paramVec.push_back(inNCCMBN_Z2-inNCCMBN_Z1);
    paramVec.push_back(131*mm);
    paramVec.push_back(131*mm);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(0);

    GsimBoxWithoutABox* inNCCPipe
      = new GsimBoxWithoutABox("inNCCPipe",this,
			       G4ThreeVector(0,0,(inNCCMBN_Z1+inNCCMBN_Z2)/2.),
			       G4ThreeVector(0,0,0));
    inNCCPipe->setParameters(paramVec);
    inNCCPipe->setOuterMaterial("G4_POLYETHYLENE");
    inNCCPipe->setOuterColor("yellow");
    addDaughter(inNCCPipe);
    


    //G10 at NCC
    //inner 130mmx130mm 0.5t
    //12.5cm@NCC upstream
    //12.5cm@10cm upstream from NCC downstream end
    

    double usNCCG10_Z=(Fbar_Len-Detpos_ncc_z_shift-NCC_Len)*cm+12.5*cm/2.;
    double dsNCCG10_Z=(Fbar_Len-Detpos_ncc_z_shift)*cm-10*cm-12.5*cm/2.;
      
    paramVec.clear();
    paramVec.push_back(130*mm+2*0.5*mm);
    paramVec.push_back(130*mm+2*0.5*mm);
    paramVec.push_back(12.5*cm);
    paramVec.push_back(130*mm);
    paramVec.push_back(130*mm);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(0);
    GsimBoxWithoutABox* usNCCG10
      = new GsimBoxWithoutABox("usNCCG10",this,
			       G4ThreeVector(0,0,usNCCG10_Z),
			       G4ThreeVector(0,0,0));
    usNCCG10->setParameters(paramVec);
    usNCCG10->setOuterMaterial("GsimG10");
    usNCCG10->setOuterColor("red");
    addDaughter(usNCCG10);

    GsimBoxWithoutABox* dsNCCG10
      = new GsimBoxWithoutABox("dsNCCG10",this,
			       G4ThreeVector(0,0,dsNCCG10_Z),
			       G4ThreeVector(0,0,0));
    dsNCCG10->setParameters(paramVec);
    dsNCCG10->setOuterMaterial("GsimG10");
    dsNCCG10->setOuterColor("red");
    addDaughter(dsNCCG10);


    //Vertical Membrane in front of the CV
    //     PE phi1986 with 275 square hole (t=0.2)
    //         z=310mm from FCV upstream suraface
    //     square 275 -> square 148 L=522 trapezoid
    G4double rCVMem=1986*mm/2.;
    if(m_userFlag>=201605) {
      rCVMem=1490*mm/2.;
    }
    G4double dCVMem1=275*mm;
    G4double dCVMem2=148*mm;
    G4double tCVMem=0.2*mm;
    G4double lCVMem=522*mm;
    G4double zCVMem=Csic_Zmin*cm-52.3*mm-250.8*mm-3*mm-310*mm;
    if(m_userFlag>=201605) {
      zCVMem+=12*mm;
    }

    paramVec.clear();
    paramVec.push_back(rCVMem);
    paramVec.push_back(tCVMem);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(dCVMem1);
    paramVec.push_back(dCVMem1);
    GsimTubeWithAHole* CVMem1 = new GsimTubeWithAHole("CVMem1",this,
						      G4ThreeVector(0,0,zCVMem),
						      G4ThreeVector(0,0,0));
    CVMem1->setParameters(paramVec);
    CVMem1->setOuterMaterial("G4_POLYETHYLENE");
    CVMem1->setOuterColor("yellow");
    addDaughter(CVMem1);


    double hCVMem2=sqrt( pow(lCVMem,2.)+pow( (dCVMem1-dCVMem2)/2.,2.) );
    paramVec.clear();
    paramVec.push_back(hCVMem2 );
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(tCVMem);
    paramVec.push_back(dCVMem1);
    paramVec.push_back(dCVMem1);
    paramVec.push_back(0);
    paramVec.push_back(tCVMem);
    paramVec.push_back(dCVMem2);
    paramVec.push_back(dCVMem2);
    paramVec.push_back(0);


    double thetaCVMem2=atan( (dCVMem1-dCVMem2)/2./lCVMem );
    double zCVMem2=zCVMem+tCVMem/2.+lCVMem/2.+tCVMem/2.*sin(thetaCVMem2);
    double sCVMem2=(dCVMem1/2.+dCVMem2/2.)/2.+tCVMem/2.*cos(thetaCVMem2);
    //zCVMem2 is linked to zCVMem. 
    //Don't move this.
    
    GsimTrap* CVMem2 = new GsimTrap("CVMem2",this,
				    G4ThreeVector(0,sCVMem2,zCVMem2),
				    G4ThreeVector( thetaCVMem2*rad,0,0));
    CVMem2->setParameters(paramVec);
    CVMem2->setOuterMaterial("G4_POLYETHYLENE");
    CVMem2->setOuterColor("blue");
    
    CVMem2->cloneDetector(G4ThreeVector(sCVMem2,0,zCVMem2),
			  G4ThreeVector( thetaCVMem2*rad,0,-90*deg));
    
    CVMem2->cloneDetector(G4ThreeVector(0,-sCVMem2,zCVMem2),
			  G4ThreeVector( -thetaCVMem2*rad,0,0));

    CVMem2->cloneDetector(G4ThreeVector(-sCVMem2,0,zCVMem2),
			  G4ThreeVector( thetaCVMem2*rad,0,90*deg));
			  
    addDaughter(CVMem2);


    
    
    
    if(userFlag<201902){
      
      
      //Downstream G10 at CSI
      //   G10 216 upstream of CsI
      //     146x146x900 0.5t
      
      G4double csiG10_L=900*mm;
      G4double csiG10_Z=Csic_Zmin*cm-216*mm+csiG10_L/2.;
      if(m_userFlag>=201605) {
	//insert it to RCV
	csiG10_Z=
	  Csic_Zmin*cm+2*cm-52.3*mm  // rearCV downstream
	  -250*mm-0.13*mm*6-3*mm     // frontCV upstram
	  +180*mm                    // from log of Watanabe(2016.04.29)
	  +csiG10_L/2.;
	csiG10_Z-=8.5*mm;            // Report 2018.9.5 by HN, CV position was shifted.
      }
      
      paramVec.clear();
      paramVec.push_back(146*mm);
      paramVec.push_back(146*mm);
      paramVec.push_back(900*mm);
      paramVec.push_back(146*mm-2*0.5*mm);
      paramVec.push_back(146*mm-2*0.5*mm);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(0);
      
      GsimBoxWithoutABox* csiG10
	= new GsimBoxWithoutABox("csiG10",this,
				 G4ThreeVector(0,0,csiG10_Z),
				 G4ThreeVector(0,0,0));
      csiG10->setParameters(paramVec);
      csiG10->setOuterMaterial("GsimG10");
      csiG10->setOuterColor("red");
      addDaughter(csiG10);
      
      // Not used in 201605 since csiG10 was upstream of RCV
      // Keep it in case of future use. 
      // if(m_userFlag>=xxxxx) {
      //   //Gap between CVMem2 and csiG10
      //   double zCVMem3=zCVMem+tCVMem/2.+lCVMem+tCVMem*sin(thetaCVMem2);
      //   double zCVMem4=csiG10_Z-csiG10_L/2.;
      //   paramVec.clear();
      //   paramVec.push_back(dCVMem2);
      //   paramVec.push_back(dCVMem2);
      //   paramVec.push_back(zCVMem4-zCVMem3);
      //   paramVec.push_back(dCVMem2-2*tCVMem);
      //   paramVec.push_back(dCVMem2-2*tCVMem);
      //   paramVec.push_back(0);
      //   paramVec.push_back(0);
      //   paramVec.push_back(0);
      //   GsimBoxWithoutABox* CVMem3
      // 	= new GsimBoxWithoutABox("CVMem3",this,
      // 				 G4ThreeVector(0,0,(zCVMem3+zCVMem4)/2.),
      // 				 G4ThreeVector(0,0,0));
      //   CVMem3->setParameters(paramVec);
      //   CVMem3->setOuterMaterial("G4_POLYETHYLENE");
      //   CVMem3->setOuterColor("yellow");
      //   addDaughter(CVMem3);
      // }
      
      
      
      //Downstream G10 at CC04
      //G10 160x160x900 0.5t
      //200mm ds of CC04
      G4double cc04G10_L=900*mm;
      G4double cc04G10_Z=Detpos_cc04_z*cm+CC04_Len*cm+200*mm-cc04G10_L/2.;
      if(m_userFlag>=201605) {
	cc04G10_Z+=2*cm;
      }
      paramVec.clear();
      paramVec.push_back(160*mm);
      paramVec.push_back(160*mm);
      paramVec.push_back(900*mm);
      paramVec.push_back(160*mm-2*0.5*mm);
      paramVec.push_back(160*mm-2*0.5*mm);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(0);
      GsimBoxWithoutABox* cc04G10
	= new GsimBoxWithoutABox("cc04G10",this,
				 G4ThreeVector(0,0,cc04G10_Z),
				 G4ThreeVector(0,0,0));
      cc04G10->setParameters(paramVec);
      cc04G10->setOuterMaterial("GsimG10");
      cc04G10->setOuterColor("red");
      addDaughter(cc04G10);
      
      
      //DS square pipe
      //SUS 200x200x1555 5t before 201504
      //Al  200x200x1515 5t from 201504
      //Start from
      // 500+980+210+735=2425 from CSI UPS by Watanabe's pdf
      // 1800 + 625=2425 from CSI UPS also by CAD and Watanabe's pdf
      G4double dsPipe_Z=dsm_Z-dsPipe_L/2.;
      
      paramVec.clear();
      paramVec.push_back(200*mm);
      paramVec.push_back(200*mm);
      paramVec.push_back(dsPipe_L);
      paramVec.push_back(200*mm-2*5*mm);
      paramVec.push_back(200*mm-2*5*mm);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(0);
      GsimBoxWithoutABox* dsPipe
	= new GsimBoxWithoutABox("dsPipe",this,
				 G4ThreeVector(0,0,dsPipe_Z),
				 G4ThreeVector(0,0,0));
      dsPipe->setParameters(paramVec);
      if(m_userFlag>0 && m_userFlag<201504) {
	dsPipe->setOuterMaterial("GsimSUS");
      } else {
	dsPipe->setOuterMaterial("G4_Al");
      }
      dsPipe->setOuterColor("gray70");
      addDaughter(dsPipe);
      
      //Downstram in-beam window 0.2t SUS
      paramVec.clear();
      paramVec.push_back(0.);
      paramVec.push_back(dsm_R);
      paramVec.push_back(dsm_thick);
      paramVec.push_back(0.);
      paramVec.push_back(360.*deg);
      GsimTube* dsmbn = new GsimTube("dsmbn",this,
				     G4ThreeVector(0,0,dsm_Z+dsm_thick/2.),
				     G4ThreeVector(0,0,0));
      dsmbn->setParameters(paramVec);
      dsmbn->setOuterMaterial("GsimSUS");
      dsmbn->setOuterColor("red");
      addDaughter(dsmbn);
      
      
      //Downstram flange 20t downstream of pipe
      paramVec.clear();
      paramVec.push_back(dsmFlange_R);
      paramVec.push_back(dsmFlange_thick);
      paramVec.push_back(0.);
      paramVec.push_back(0.);
      paramVec.push_back(200.1*mm);
      paramVec.push_back(200.1*mm);
      GsimTubeWithAHole* dsFlange
	= new GsimTubeWithAHole("dsFlange",this,
				G4ThreeVector(0,0,dsm_Z-dsmFlange_thick/2.),
				G4ThreeVector(0,0,0));
      dsFlange->setParameters(paramVec);
      if(m_userFlag>=201605) {
	dsFlange->setOuterMaterial("G4_Al");
      } else {
	dsFlange->setOuterMaterial("GsimSUS");
      }
      dsFlange->setOuterColor("gray70");
      addDaughter(dsFlange);

      paramVec.clear();
      paramVec.push_back(280*mm/2);
      paramVec.push_back(dsmFlange_R);
      paramVec.push_back(dsmFlange_thick);
      paramVec.push_back(0);
      paramVec.push_back(360*deg);

      //Downstram flange 20t to hold window
      GsimTube* dsFlange2 = new GsimTube( "dsFlange2", this, 
					  G4ThreeVector(0,
							0,
							dsm_Z
							+dsm_thick
							+dsmFlange_thick/2.),
					  G4ThreeVector(0,0,0) );
      dsFlange2->setOuterMaterial( "GsimSUS" );
      dsFlange2->setOuterColor( "yellow" );
      dsFlange2->setOuterVisibility( true );
      dsFlange2->setParameters( paramVec );
      addDaughter(dsFlange2);
      
      //Downstram window flange 20t upstream of DS pipe
      paramVec.clear();
      paramVec.push_back(dsmuFlange_R);
      paramVec.push_back(dsmuFlange_thick);
      paramVec.push_back(0.);
      paramVec.push_back(0.);
      paramVec.push_back(200.1*mm);
      paramVec.push_back(200.1*mm);
      GsimTubeWithAHole* dsuFlange
	= new GsimTubeWithAHole("dsuFlange",this,
				G4ThreeVector(0,0,dsmu_Z-dsmuFlange_thick/2.),
				G4ThreeVector(0,0,0));
      dsuFlange->setParameters(paramVec);
      if(m_userFlag>=201605) {
	dsuFlange->setOuterMaterial("G4_Al");
      } else {
	dsuFlange->setOuterMaterial("GsimSUS");
      }
      dsuFlange->setOuterColor("gray70");
      addDaughter(dsuFlange);
      
      
      //DS membrane pipe
      //CC04 end + 1mm(margin): 161x161
      //DS Flange z=2395 : 240x240 trapezoid
      double dDSMP1=161*mm;
      double dDSMP2=240*mm;
      double zDSMP1=Detpos_cc04_z*cm+CC04_Len*cm+1*mm;
      double zDSMP2=Csic_Zmin*cm+2395*mm;
      if(m_userFlag>=201605) {
	zDSMP1+=20*mm;
	zDSMP2+=20*mm;
      }
      
      
      double hDSMP=sqrt( pow(zDSMP2-zDSMP1,2.) + pow( (dDSMP2-dDSMP1)/2.,2.) );
      paramVec.clear();
      paramVec.push_back(hDSMP );
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(tCVMem);
      paramVec.push_back(dDSMP1);
      paramVec.push_back(dDSMP1);
      paramVec.push_back(0);
      paramVec.push_back(tCVMem);
      paramVec.push_back(dDSMP2);
      paramVec.push_back(dDSMP2);
      paramVec.push_back(0);
      
      
      double thetaDSMP=atan( (dDSMP2-dDSMP1)/2./(zDSMP2-zDSMP1) );
      double zDSMP=(zDSMP1+zDSMP2)/2.-tCVMem/2.*sin(thetaDSMP);
      double sDSMP=(dDSMP1+dDSMP2)/4.+tCVMem/2.*cos(thetaDSMP);
      if(m_userFlag>=201605) {
	zDSMP+=2*cm;
      }
      
      GsimTrap* DSMP = new GsimTrap("DSMP",this,
				    G4ThreeVector(0,sDSMP,zDSMP),
				    G4ThreeVector( -thetaDSMP*rad,0,0));
      DSMP->setParameters(paramVec);
      DSMP->setOuterMaterial("G4_POLYETHYLENE");
      DSMP->setOuterColor("blue");
      
      DSMP->cloneDetector(G4ThreeVector(sDSMP,0,zDSMP),
			  G4ThreeVector( -thetaDSMP*rad,0,-90*deg));
      
      DSMP->cloneDetector(G4ThreeVector(0,-sDSMP,zDSMP),
			  G4ThreeVector( thetaDSMP*rad,0,0));
      
      DSMP->cloneDetector(G4ThreeVector(-sDSMP,0,zDSMP),
			  G4ThreeVector( -thetaDSMP*rad,0,90*deg));
      
      addDaughter(DSMP);
      
      
    } else {
      
      
      //Downstream G10 at CSI
      //   G10 216 upstream of CsI
      //     146x146x900 0.5t
      
      G4double csiG10_L=550*mm;
      G4double csiG10_Z=Csic_Zmin*cm+2*cm-70*mm+csiG10_L/2.;
      
      paramVec.clear();
      paramVec.push_back(146*mm);
      paramVec.push_back(146*mm);
      paramVec.push_back(csiG10_L);
      paramVec.push_back(146*mm-2*0.5*mm);
      paramVec.push_back(146*mm-2*0.5*mm);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(0);
      
      GsimBoxWithoutABox* csiG10
	= new GsimBoxWithoutABox("csiG10",this,
				 G4ThreeVector(0,0,csiG10_Z),
				 G4ThreeVector(0,0,0));
      csiG10->setParameters(paramVec);
      csiG10->setOuterMaterial("GsimG10");
      csiG10->setOuterColor("red");
      addDaughter(csiG10);
      
      
      double t_pipe = 5*mm;
      double BeamPipe_lenz = dsPipe_L;
      double BeamPipe_z = Csic_Zmin*cm+2*cm+(500+980+210+735)*mm+dsPipe_L/2;
      double BeamPipeWidth = 200*mm;
      
      const double upstream_frange_thick = 20*mm;
      const double upstream_frange_z = BeamPipe_z - BeamPipe_lenz/2+ upstream_frange_thick/2;
      
      const double downstream_frange_thick = 20*mm;
      const double downstream_frange_z = BeamPipe_z + BeamPipe_lenz/2 - downstream_frange_thick/2;
      
      paramVec.clear();
      paramVec.push_back(575*mm/2);
      paramVec.push_back(upstream_frange_thick);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(200*mm);
      paramVec.push_back(200*mm);
      
      GsimTubeWithAHole* frange_upstream = new GsimTubeWithAHole( "frangeupstream", this, 
								  G4ThreeVector(0,0,upstream_frange_z), G4ThreeVector(0,0,0) );
      frange_upstream->setOuterMaterial( "G4_Al" );
      frange_upstream->setOuterColor( "green" );
      frange_upstream->setOuterVisibility( true );
      frange_upstream->setParameters( paramVec );
      addDaughter(frange_upstream);
      
      
      paramVec.clear();
      paramVec.push_back(400*mm/2);
      paramVec.push_back(downstream_frange_thick);
      paramVec.push_back(0);
      paramVec.push_back(0);
      paramVec.push_back(200*mm);
      paramVec.push_back(200*mm);
      
      GsimTubeWithAHole* frange_downstream = new GsimTubeWithAHole( "frangedownstream", this, 
								    G4ThreeVector(0,0,downstream_frange_z),
								    G4ThreeVector(0,0,0) );
      frange_downstream->setOuterMaterial( "G4_Al" );
      frange_downstream->setOuterColor( "green" );
      frange_downstream->setOuterVisibility( true );
      frange_downstream->setParameters( paramVec );
      addDaughter(frange_downstream);
      
      double tol=0.01*mm;
      paramVec.clear();
      paramVec.push_back(50/2*mm);
      paramVec.push_back(60/2*mm);
      paramVec.push_back(45*mm-tol);
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      G4ThreeVector postube = G4ThreeVector(0, BeamPipeWidth/2 + 45*mm/2, BeamPipe_z+980*mm-BeamPipe_lenz/2);
      postube.rotateZ(90*deg);
      GsimTube* tube_side1 = new GsimTube( "tube_side1", this, 
					   postube, G4ThreeVector(0,90*deg,0) );
      tube_side1->setOuterMaterial( "G4_Al" );
      tube_side1->setOuterColor( "green" );
      tube_side1->setOuterVisibility( true );
      tube_side1->setParameters( paramVec );
      addDaughter(tube_side1);
      
      
      postube.rotateZ(-180*deg);
      GsimTube* tube_side2 = new GsimTube( "tube_side2", this, 
					   postube, G4ThreeVector(0,90*deg,0) );
      tube_side2->setOuterMaterial( "G4_Al" );
      tube_side2->setOuterColor( "green" );
      tube_side2->setOuterVisibility( true );
      tube_side2->setParameters( paramVec );
      addDaughter(tube_side2);
      
      paramVec.clear();
      paramVec.push_back(50/2*mm);
      paramVec.push_back(120/2*mm);
      paramVec.push_back(25*mm); // including the attachment side of 10 mm
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      postube = G4ThreeVector(0, BeamPipeWidth/2 + 45*mm + 25*mm/2, BeamPipe_z+980*mm-BeamPipe_lenz/2);
      postube.rotateZ(90*deg);
      GsimTube* tube_side1_1 = new GsimTube( "tube_side1_1", this, 
					     postube, G4ThreeVector(0,90*deg,0) );
      tube_side1_1->setOuterMaterial( "G4_Al" );
      tube_side1_1->setOuterColor( "green" );
      tube_side1_1->setOuterVisibility( true );
      tube_side1_1->setParameters( paramVec );
      addDaughter(tube_side1_1);
      
      postube.rotateZ(-180*deg);
      GsimTube* tube_side2_1 = new GsimTube( "tube_side2_1", this, 
					     postube, G4ThreeVector(0,90*deg,0) );
      tube_side2_1->setOuterMaterial( "G4_Al" );
      tube_side2_1->setOuterColor( "green" );
      tube_side2_1->setOuterVisibility( true );
      tube_side2_1->setParameters( paramVec );
      addDaughter(tube_side2_1);
      
      
      paramVec.clear();
      paramVec.push_back(150/2*mm);
      paramVec.push_back(160/2*mm);
      paramVec.push_back(98*mm);
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      postube = G4ThreeVector(0, BeamPipeWidth/2 + 98*mm/2 , BeamPipe_z+980*mm-BeamPipe_lenz/2);
      GsimTube* tube_side3 = new GsimTube( "tube_top", this, 
					   postube, G4ThreeVector(90*deg,0,0) );
      tube_side3->setOuterMaterial( "G4_Al" );
      tube_side3->setOuterColor( "green" );
      tube_side3->setOuterVisibility( true );
      tube_side3->setParameters( paramVec );
      addDaughter(tube_side3);
      
      paramVec.clear();
      paramVec.push_back(150/2*mm);
      paramVec.push_back(203/2*mm);
      paramVec.push_back(44*mm);  // including the pump side
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      postube = G4ThreeVector(0, BeamPipeWidth/2 + 98*mm + 44*mm/2, BeamPipe_z+980*mm-BeamPipe_lenz/2);
      GsimTube* tube_side3_1 = new GsimTube( "tube_top_1", this, 
					     postube, G4ThreeVector(90*deg,0,0) );
      tube_side3_1->setOuterMaterial( "G4_Al" );
      tube_side3_1->setOuterColor( "green" );
      tube_side3_1->setOuterVisibility( true );
      tube_side3_1->setParameters( paramVec );
      addDaughter(tube_side3_1);
      
      // place molecular turbo pump
      paramVec.clear();
      paramVec.push_back(0*mm);
      paramVec.push_back(150*mm/2);
      paramVec.push_back(250*mm);
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      postube = G4ThreeVector(0, BeamPipeWidth/2 + 98*mm + 44*mm + 250*mm/2, BeamPipe_z+980*mm-BeamPipe_lenz/2);
      GsimTube* TBMP = new GsimTube( "pump", this, 
				     postube, G4ThreeVector(90*deg,0,0) );
      TBMP->setOuterMaterial( "G4_Fe" );
      TBMP->setOuterColor( "yellow" );
      TBMP->setOuterVisibility( true );
      TBMP->setParameters( paramVec );
      addDaughter(TBMP);
      
      
      // place window
      paramVec.clear();
      paramVec.push_back(0*mm);
      paramVec.push_back(280*mm/2);
      paramVec.push_back(0.2*mm);
      paramVec.push_back(0*deg);
      paramVec.push_back(360*deg);
      
      G4ThreeVector pos_window = G4ThreeVector(
					       0,
					       0,
					       BeamPipe_z
					       +BeamPipe_lenz/2
					       +0.2*mm/2);
      GsimTube* window = new GsimTube( "window", this, 
				       pos_window, G4ThreeVector(0,0,0) );
      window->setOuterMaterial( "GsimSUS" );
      window->setOuterColor( "red" );
      window->setOuterVisibility( true );
      window->setParameters( paramVec );
      addDaughter(window);

      // place frange to hold membrane
      paramVec.clear();
      paramVec.push_back(280*mm/2);
      paramVec.push_back(400*mm/2);
      paramVec.push_back(downstream_frange_thick);
      paramVec.push_back(0);
      paramVec.push_back(360*deg);
      
      GsimTube* frange2_downstream = new GsimTube( "frange2downstream", this, 
						   G4ThreeVector(0,
								 0,
								 BeamPipe_z+BeamPipe_lenz/2
								 +0.2*mm
								 +downstream_frange_thick/2 ),
						   G4ThreeVector(0,0,0) );
      frange2_downstream->setOuterMaterial( "GsimSUS" );
      frange2_downstream->setOuterColor( "yellow" );
      frange2_downstream->setOuterVisibility( true );
      frange2_downstream->setParameters( paramVec );
      addDaughter(frange2_downstream);
      
      
      
      for(int i=0;i<4;i++){
	
	// for left and right sides, we cut the edges
	double cut = 0;
	if(i==1 || i==3) cut = 1;
	
	// Beam pipe
	paramVec.resize(23);
	paramVec[0] = BeamPipe_lenz/2;
	paramVec[1] = 0; // theta
	paramVec[2] = 0; // phi
	paramVec[3] = t_pipe/2; // y
	paramVec[4] = (BeamPipeWidth/2)-t_pipe*cut; // x
	paramVec[5] = (BeamPipeWidth/2)-t_pipe*cut; // x
	paramVec[6] = 0;
	paramVec[7] = t_pipe/2;
	paramVec[8] = (BeamPipeWidth/2)-t_pipe*cut;
	paramVec[9] = (BeamPipeWidth/2)-t_pipe*cut;
	paramVec[10] = 0;
	
	paramVec[11] = -1*mm;
	paramVec[12] = -1*mm;
	paramVec[13] = -1*mm;
	paramVec[14] = 0*mm;
	paramVec[15] = 0*mm;
	paramVec[16] = 0*mm;
	
	if(i==0){
	  paramVec[17] = 0*mm; 
	  paramVec[18] = 75*mm;
	  paramVec[19] = 10*mm;
	  paramVec[20] = 0*mm;
	  paramVec[21] = -t_pipe/2;
	  paramVec[22] = 980*mm-BeamPipe_z/2;
	}else if(i==1 || i==3){
	  paramVec[17] = 0*mm; 
	  paramVec[18] = 25*mm;
	  paramVec[19] = 10*mm;
	  paramVec[20] = 0*mm;
	  paramVec[21] = -t_pipe/2;
	  paramVec[22] = 980*mm-BeamPipe_z/2;
	}else{
	  paramVec[17] = -1*mm; 
	  paramVec[18] = -1*mm;
	  paramVec[19] = -1*mm;
	  paramVec[20] = 0*mm;
	  paramVec[21] = 0*mm;
	  paramVec[22] = 0*mm;
	}
	const double pos_pipe_frombeam = BeamPipeWidth/2 - t_pipe/2;

			
	G4ThreeVector pos_pipe(0*mm, pos_pipe_frombeam, BeamPipe_z );
	pos_pipe.rotateZ(i*90*deg);
			
	G4ThreeVector rot(0,0, 90*i*deg);

	std::stringstream sspipe;
	sspipe << "BeamPipe2_" << i;
	GsimTrapWithHoles* pipe = new GsimTrapWithHoles(sspipe.str().c_str(),this,
							pos_pipe, rot
							);
	
	pipe->setParameters(paramVec);
	pipe->setOuterColor("green");
	pipe->setOuterMaterial( "G4_Al" );
	addDaughter(pipe);
      }
    }
    
  } else if(model ==0) {
    std::vector<double> paramVec;
    paramVec.push_back(0.);
    paramVec.push_back(BeamLine_R*cm);
    paramVec.push_back(MBN_Thick*cm);
    paramVec.push_back(0.);
    paramVec.push_back(360.*deg);
    GsimTube* usmbn = new GsimTube("usmbn",this,
				   G4ThreeVector(0,0,MBN_Thick/2.*cm),
				   G4ThreeVector(0,0,0));
    usmbn->setParameters(paramVec);
    usmbn->setOuterMaterial("GsimMembrane");
    addDaughter(usmbn);

    // paramVec.clear();
    //   paramVec.push_back(BeamLine_R*cm);
    //   paramVec.push_back(99*cm);
    //   paramVec.push_back(MBN_Thick*cm);
    //   paramVec.push_back(0.);
    //   paramVec.push_back(360.*deg);
    //   GsimTube* cvmbn = new GsimTube("cvmbn",this,
    // 				 G4ThreeVector(0,0,(Csic_Zmin-CV_Len-MBN_Thick/2.)*cm),
    // 				 G4ThreeVector(0,0,0));
    //   cvmbn->setParameters(paramVec);
    //   cvmbn->setOuterMaterial("GsimMembrane");
    //   cvmbn->setOuterVisibility(false);
    //   addDaughter(cvmbn);


    double c_mbn1_z = //front face of CC04 
      Detpos_cc04_z-MBN_Thick/2.;

    paramVec.clear();
    paramVec.push_back(0.);
    paramVec.push_back(BeamLine_R*cm);
    paramVec.push_back(MBN_Thick*cm);
    paramVec.push_back(0.);
    paramVec.push_back(360.*deg);
    GsimTube* dsmbn = new GsimTube("dsmbn",this,
				   G4ThreeVector(0,0,c_mbn1_z*cm),
				   G4ThreeVector(0,0,0));
    dsmbn->setParameters(paramVec);
    dsmbn->setOuterMaterial("GsimMembrane");    
    addDaughter(dsmbn);
  }
  setThisAndDaughterBriefName("MBN");
  m_detMoveList.push_back(this);

}

GsimE14Membrane::~GsimE14Membrane()
{
  ;
}

void GsimE14Membrane::comment()
{
  std::cout << "Place it at z=0" << std::endl;;
  std::cout << "When userFlag is >=201605, downstream items are automatically shifted downstream by 2 cm." << std::endl;
}

void GsimE14Membrane::setFastSimulationLevel(int level)
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
