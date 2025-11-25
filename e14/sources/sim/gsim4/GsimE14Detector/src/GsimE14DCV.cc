/**
 *  @file
 *  @brief  GsimE14DCV
 *
 */
#include "GsimE14Detector/GsimE14DCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Box.hh"
#include "GsimDetector/GsimTrap.h"
#include "G4SystemOfUnits.hh"
#include "GsimDetector/GsimTubeWithAHole.h"
#include "GsimDetector/GsimTube.h"

using namespace E14;

GsimE14DCV::GsimE14DCV(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14DCV";

  if(m_userFlag < 201902
     ) {
    
    std::cout << "Error! DCV did not exist in this time\n";
    exit(0);
  }
  if(m_userFlag==201902 ||
     m_userFlag==202002 ||
     m_userFlag==202102 ||
     m_userFlag==202306
     ) {
    
    	
    std::cout << "DCV will be constructed.\n"; 

    std::vector<double> paramVec;

    {
		
      const double t  = 5*mm;
      const double len_z = 1410*mm;
      const double width_upstream   = 160.0*mm;
      const double width_downstream = 174.0*mm;
      const double DCV_box_w = 11.8*mm;
      const double DCV_mylar_t=12e-6*m;
      
      
      double delta_width = (width_downstream-width_upstream)/2;
      double leng_trap = sqrt(len_z*len_z+delta_width*delta_width);
      double ang_tilt = atan(delta_width/len_z);
      double costilt = cos(ang_tilt);

      
      double tol=200*nm;//tolerance
      double dBoreUp=width_upstream-t/costilt*2;
      double dBoreDown=width_downstream-t/costilt*2;
      // place DCV1 + beam pipe
      for(int i=0;i<4;i++){
	//0 : top(+y)    side
	//1 : south(-x)  side
	//2 : bottom(-y) side
	//3 : north(+x)  side
	
	// for left and right sides, we cut the edges
	double cut = 0;
	if(i==1 || i==3) cut = 1;
	double dUp   = width_upstream  -cut*t/costilt*2;
	double dDown = width_downstream-cut*t/costilt*2;

	for(int j=0;j<2;j++){
				
	  std::stringstream ssDCV1;
	  ssDCV1 << "DCV1_" << i << "_" << j;
	  int sgn = 2*j-1;
	  
	  G4ThreeVector pos(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,0);//center on inner surface
	  pos=pos+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  pos=pos+G4ThreeVector(0,+(t/2.)*cos(ang_tilt),-(t/2.)*sin(ang_tilt));//thickness direction
	  pos.rotateZ(i*90*deg);
	  
	  //shape of the half trap
	  paramVec.resize(11);
	  paramVec[0] = t; //z
	  paramVec[1] = 0;  //theta
	  paramVec[2] = 0;  // phi
	  paramVec[3] = leng_trap; // y
	  paramVec[4] = dDown/2.;   // x at (-y)
	  paramVec[5] = dUp/2.; // x at (+y)
	  paramVec[6] = sgn*atan((dDown/4.-dUp/4.)/leng_trap)*rad;
	  paramVec[7] = leng_trap; // y
	  paramVec[8] = dDown/2.;   // x at (-y)
	  paramVec[9] = dUp/2.; // x at (+y)
	  paramVec[10] =sgn*atan((dDown/4.-dUp/4.)/leng_trap)*rad;
	  //Top:     X:+90*deg + ang_tilt
	  //Then Zrotation for other sides
	  G4ThreeVector rot(-90*deg - ang_tilt*rad, 0, 90*i*deg);


	  GsimTrap* plate = new GsimTrap(ssDCV1.str().c_str(),this,
					 pos, rot
					 );
	  
	  plate->setParameters(paramVec);
	  plate->setOuterColor("blue");
	  plate->setSensitiveDetector("DCV",(i+1)%4);
	  addDaughter(plate);
	  m_detMoveList.push_back(plate);
	
	  // light box in the upstream side inside scinti.
	  paramVec.clear();
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(3*mm);
	  std::stringstream ss_lightbox;
	  ss_lightbox << "DCV1_lightbox_" << i << "_" << j << "_1";
				
	  G4ThreeVector pos_lb1(-sgn*(-dUp/4.+37.62475*mm), +(leng_trap/2-11*mm), t/2-3*mm/2.);
	  GsimBox* box = new GsimBox(ss_lightbox.str().c_str(),plate,
	   			     pos_lb1,
				     G4ThreeVector(0,0,0)
				     );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  plate->addDaughter(box);
	  

	  // light box in the downstream side inside scinti.
	  std::stringstream ss_lightbox2;
	  ss_lightbox2 << "DCV1_lightbox_" << i << "_" << j << "_2";
				
	  G4ThreeVector pos_lb2(-sgn*(-dDown/4.+41.12473*mm) ,-(leng_trap/2-9*mm),t/2-3*mm/2.);
	  box = new GsimBox(ss_lightbox2.str().c_str(),plate,
	   		    pos_lb2,
			    G4ThreeVector(0,0,0)
			    );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  plate->addDaughter(box);


	  // light box in the upstream side outside scinti.
	  paramVec.clear();
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(1*mm);
	  std::stringstream ss_lightbox3;
	  ss_lightbox3 << "DCV1_lightbox_" << i << "_" << j << "_3";
				
	  G4ThreeVector pos_lb3(-sgn*(-dUp/4.+37.62475*mm), +(leng_trap/2-11*mm), t/2+1*mm/2.);
	  pos_lb3.rotateX(-90*deg - ang_tilt*rad);
	  pos_lb3=pos_lb3+G4ThreeVector(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,0);//center on inner surface
	  pos_lb3=pos_lb3+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  pos_lb3=pos_lb3+G4ThreeVector(0,+(t/2.)*cos(ang_tilt),-(t/2.)*sin(ang_tilt));//thickness direction
	  pos_lb3.rotateZ(i*90*deg);
	  box = new GsimBox(ss_lightbox3.str().c_str(),this,
			    pos_lb3,
			    rot
			    );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  addDaughter(box);
	  m_detMoveList.push_back(box);


	  // light box in the downstream side outside scinti.
	  // not to be implemented due to the geometorical conflict to the holder
	  // paramVec.clear();
	  // paramVec.push_back(DCV_box_w);
	  // paramVec.push_back(DCV_box_w);
	  // paramVec.push_back(1*mm);
	  // std::stringstream ss_lightbox4;
	  // ss_lightbox4 << "DCV1_lightbox_" << i << "_" << j << "_4";
		

	  // G4ThreeVector pos_lb4(-sgn*(-dDown/4.+41.12473*mm) ,-(leng_trap/2-9*mm),t/2+1*mm/2.);
	  // pos_lb4.rotateX(-90*deg - ang_tilt*rad);
	  // pos_lb4=pos_lb4+G4ThreeVector(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,0);//center on inner surface
	  // pos_lb4=pos_lb4+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  // pos_lb4=pos_lb4+G4ThreeVector(0,+(t/2.)*cos(ang_tilt),-(t/2.)*sin(ang_tilt));//thickness direction
	  // pos_lb4.rotateZ(i*90*deg);
	  // box = new GsimBox(ss_lightbox4.str().c_str(),this,
	  // 		    pos_lb4,
	  // 		    rot
	  // 		    );
	  // box->setParameters(paramVec);
	  // box->setOuterColor("red");
	  // addDaughter(box);
	  // m_detMoveList.push_back(box);

	  // groove in the scintillator
	  double grooveL=leng_trap-5*mm-5*mm-12*mm*2;
	  double grooveW=1.2*mm;
	  double grooveD=1.5*mm;
	  double grooveInterval=8*mm;
	  paramVec.clear();
	  paramVec.push_back(grooveW);
	  paramVec.push_back(grooveL);
	  paramVec.push_back(grooveD);
	  std::stringstream ss_groove;
	  ss_groove << "DCV1_groove_" << i << "_" << j;
	  for(int k=0;k<9;k++) {
	    G4ThreeVector pos_groove(-grooveInterval*(k-4),0,+t/2-grooveD/2.);
	    if(k==0) {
	      box = new GsimBox(ss_groove.str(),plate,
				pos_groove,
				G4ThreeVector(0,0,0)
				);
	      box->setParameters(paramVec);
	      box->setOuterColor("green");
	      plate->addDaughter(box);
	    } else {
	      box->cloneDetector(pos_groove,
				 G4ThreeVector(0,0,0));
	    }
	  }
	}

	double width_holder_upstream = (15*mm*width_upstream + (leng_trap-15*mm)*width_downstream)/leng_trap;
	const double gap = 0.5*mm;
	const double t_holder = 5*mm;
	// Al support structure
	paramVec.resize(11);
	paramVec[0] = 15*mm;
	paramVec[1] = 0; // theta
	paramVec[2] = 0; // phi
	paramVec[3] = t_holder; // y
	paramVec[4] = ((gap+t_holder)+(width_holder_upstream/2)-t_holder*cut)*2; // x
	paramVec[5] = ((gap+t_holder)+(width_holder_upstream/2)-t_holder*cut)*2; // x
	paramVec[6] = 0;
	paramVec[7] = t_holder;
	paramVec[8] = ((gap+t_holder)+(width_downstream/2)-t_holder*cut)*2;
	paramVec[9] = ((gap+t_holder)+(width_downstream/2)-t_holder*cut)*2;
	paramVec[10] = 0;
				
	const double pos_holder_frombeam = gap + t_holder/2+(width_holder_upstream/2+width_downstream/2)/2;

			
	G4ThreeVector pos_holder(0*mm, pos_holder_frombeam+tol, len_z/2-t*sin(ang_tilt));
	pos_holder.rotateZ(i*90*deg);
			
	G4ThreeVector rot_holder(-ang_tilt*radian, 0, 90*i*deg);
	
	std::stringstream ssholder;
	ssholder << "AlDCV1holder_" << i;
	GsimTrap* holder = new GsimTrap(ssholder.str().c_str(),this,
					pos_holder, rot_holder
					);
	
	holder->setParameters(paramVec);
	holder->setOuterColor("green");
	holder->setOuterMaterial( "G4_Al" );
	addDaughter(holder);
	m_detMoveList.push_back(holder);
      }

      //Mylar
      paramVec.resize(11);
      paramVec[0] = DCV_mylar_t; //z
      paramVec[1] = 0;  //theta
      paramVec[2] = 0;  // phi
      paramVec[3] = leng_trap; // y
      paramVec[4] = dBoreDown-DCV_mylar_t*2;// x at (-y)
      paramVec[5] = dBoreUp-DCV_mylar_t*2;  // x at (+y)
      paramVec[6] = 0;
      paramVec[7] = leng_trap; // y
      paramVec[8] = dBoreDown-DCV_mylar_t*2;// x at (-y)
      paramVec[9] = dBoreUp-DCV_mylar_t*2;  // x at (+y)
      paramVec[10] =0;
      
      GsimTrap* mylar=0;
      for(int i=0;i<4;i++) {
	G4ThreeVector pos_mylar(0, (dBoreUp/2.+dBoreDown/2.)/2.-tol, 0);
	pos_mylar=pos_mylar+G4ThreeVector(0,
					  -DCV_mylar_t/2.*cos(ang_tilt),
					  +DCV_mylar_t/2.*sin(ang_tilt));
	pos_mylar.rotateZ(i*90*deg);
	G4ThreeVector rot_mylar(-90*deg - ang_tilt*rad, 0, 90*i*deg);
	if(i==0) {
	  mylar = new GsimTrap("DCV1_mylar",this,
			       pos_mylar, rot_mylar
			       );
	  mylar->setParameters(paramVec);
	  mylar->setOuterColor("gray50");
	  mylar->setOuterMaterial("G4_MYLAR");
	  addDaughter(mylar);
	  m_detMoveList.push_back(mylar);
	} else {
	  mylar->cloneDetector(pos_mylar, rot_mylar);
	}
      }
    }
    
    {


      const double DCV2_z_downstream = 1525*mm;
      const double t  = 5*mm;
      const double len_z = 1460*mm;
      const double width   = 176.0*mm;
      const double DCV_box_pos = 42.15*mm;
      const double DCV_box_w = 11.8*mm;
      const double DCV_mylar_t=12e-6*m;

      double tol=200*nm;//tolerance
      double dBoreUp=width-t*2;
      double dBoreDown=width-t*2;
      for(int i=0;i<4;i++){
	
	//0 : top(+y)    side
	//1 : south(-x)  side
	//2 : bottom(-y) side
	//3 : north(+x)  side
	
	// for left and right sides, we cut the edges
	double cut = 0;
	if(i==1 || i==3) cut = 1;
	double dUp   = width-cut*t*2;
	double dDown = width-cut*t*2;
	
	for(int j=0;j<2;j++){
	  
	  std::stringstream ssDCV2;
	  ssDCV2 << "DCV2_" << i << "_" << j;
	  int sgn = 2*j-1;

	  G4ThreeVector pos(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,DCV2_z_downstream);//center on inner surface
	  pos=pos+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  pos=pos+G4ThreeVector(0,+(t/2.),0);//thickness direction
	  pos.rotateZ(i*90*deg);
	  
	  paramVec.resize(11);
	  paramVec[0] = t;
	  paramVec[1] = 0;
	  paramVec[2] = 0; // phi
	  paramVec[3] = len_z; // y
	  paramVec[4] = dDown/2.; // x
	  paramVec[5] = dUp/2.; // x
	  paramVec[6] = 0;
	  paramVec[7] = len_z;
	  paramVec[8] = dDown/2.;
	  paramVec[9] = dUp/2.;
	  paramVec[10] = 0;
	  
	  G4ThreeVector rot(-90*deg, 0, 90*i*deg);
	  GsimTrap* plate = new GsimTrap(ssDCV2.str().c_str(),this,
					 pos, rot
					 );
	  
	  plate->setParameters(paramVec);
	  plate->setOuterColor("blue");
	  plate->setSensitiveDetector("DCV",(i+1)%4+4);
	  addDaughter(plate);
	  m_detMoveList.push_back(plate);

	  // light box in the upstream side inside scinti.
	  paramVec.clear();
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(3*mm);
	  std::stringstream ss_lightbox;
	  ss_lightbox << "DCV2_lightbox_" << i << "_" << j << "_1";
				
	  G4ThreeVector pos_lb1(-sgn*(-dUp/4.+DCV_box_pos), +(len_z/2-9*mm), t/2-3*mm/2.);
	  GsimBox* box = new GsimBox(ss_lightbox.str().c_str(),plate,
	   			     pos_lb1,
				     G4ThreeVector(0,0,0)
				     );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  plate->addDaughter(box);
	  
	  // light box in the downstream side inside scinti.
	  std::stringstream ss_lightbox2;
	  ss_lightbox2 << "DCV2_lightbox_" << i << "_" << j << "_2";
	  
	  G4ThreeVector pos_lb2(-sgn*(-dDown/4.+DCV_box_pos) ,-(len_z/2-9*mm),t/2-3*mm/2.);
	  box = new GsimBox(ss_lightbox2.str().c_str(),plate,
	   		    pos_lb2,
			    G4ThreeVector(0,0,0)
			    );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  plate->addDaughter(box);
	  
	  // light box in the upstream side outside scinti.
	  paramVec.clear();
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(1*mm);
	  std::stringstream ss_lightbox3;
	  ss_lightbox3 << "DCV2_lightbox_" << i << "_" << j << "_3";
				
	  G4ThreeVector pos_lb3(-sgn*(-dUp/4.+DCV_box_pos), +(len_z/2-9*mm), t/2+1*mm/2.);
	  pos_lb3.rotateX(-90*deg);
	  pos_lb3=pos_lb3+G4ThreeVector(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,DCV2_z_downstream);//center on inner surface
	  pos_lb3=pos_lb3+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  pos_lb3=pos_lb3+G4ThreeVector(0,+t/2.,0);//thickness direction
	  pos_lb3.rotateZ(i*90*deg);
	  box = new GsimBox(ss_lightbox3.str().c_str(),this,
			    pos_lb3,
			    rot
			    );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  addDaughter(box);
	  m_detMoveList.push_back(box);


	  // light box in the downstream side outside scinti.
	  paramVec.clear();
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(DCV_box_w);
	  paramVec.push_back(1*mm);
	  std::stringstream ss_lightbox4;
	  ss_lightbox4 << "DCV2_lightbox_" << i << "_" << j << "_4";

	  G4ThreeVector pos_lb4(-sgn*(-dDown/4.+DCV_box_pos) ,-(len_z/2-9*mm),t/2+1*mm/2.);
	  pos_lb4.rotateX(-90*deg);
	  pos_lb4=pos_lb4+G4ThreeVector(0,(dBoreUp/2.+dBoreDown/2.)/2.+tol,DCV2_z_downstream);//center on inner surface
	  pos_lb4=pos_lb4+G4ThreeVector(-sgn*(dUp/4.+dDown/4.)/2.,0,0);//half
	  pos_lb4=pos_lb4+G4ThreeVector(0,+t/2.,0);//thickness direction
	  pos_lb4.rotateZ(i*90*deg);
	  box = new GsimBox(ss_lightbox4.str().c_str(),this,
	   		    pos_lb4,
			    rot
			    );
	  box->setParameters(paramVec);
	  box->setOuterColor("red");
	  addDaughter(box);
	  m_detMoveList.push_back(box);

	  // groove in the scintillator
	  double grooveL=len_z-3*mm-3*mm-12*mm*2;
	  double grooveW=1.2*mm;
	  double grooveD=1.5*mm;
	  double grooveInterval=8*mm;
	  paramVec.clear();
	  paramVec.push_back(grooveW);
	  paramVec.push_back(grooveL);
	  paramVec.push_back(grooveD);
	  std::stringstream ss_groove;
	  ss_groove << "DCV2_groove_" << i << "_" << j;
	  for(int k=0;k<9;k++) {
	    G4ThreeVector pos_groove(-grooveInterval*(k-4),0,+t/2-grooveD/2.);
	    if(k==0) {
	      box = new GsimBox(ss_groove.str(),plate,
				pos_groove,
				G4ThreeVector(0,0,0)
				);
	      box->setParameters(paramVec);
	      box->setOuterColor("green");
	      plate->addDaughter(box);
	    } else {
	      box->cloneDetector(pos_groove,
				 G4ThreeVector(0,0,0));
	    }
	  }
	}	
      }
      //Mylar
      paramVec.resize(11);
      paramVec[0] = DCV_mylar_t; //z
      paramVec[1] = 0;  //theta
      paramVec[2] = 0;  // phi
      paramVec[3] = len_z; // y
      paramVec[4] = dBoreDown-DCV_mylar_t*2;// x at (-y)
      paramVec[5] = dBoreUp-DCV_mylar_t*2;  // x at (+y)
      paramVec[6] = 0;
      paramVec[7] = len_z; // y
      paramVec[8] = dBoreDown-DCV_mylar_t*2;// x at (-y)
      paramVec[9] = dBoreUp-DCV_mylar_t*2;  // x at (+y)
      paramVec[10] =0;
      GsimTrap* mylar=0;
      for(int i=0;i<4;i++) {
	G4ThreeVector pos_mylar(0, (dBoreUp/2.+dBoreDown/2.)/2.-tol, DCV2_z_downstream);
	pos_mylar=pos_mylar+G4ThreeVector(0,
					  -DCV_mylar_t/2.,
					  0);
	pos_mylar.rotateZ(i*90*deg);
	G4ThreeVector rot_mylar(-90*deg, 0, 90*i*deg);
	if(i==0) {
	  mylar = new GsimTrap("DCV2_mylar",this,
			       pos_mylar, rot_mylar
			       );
	  mylar->setParameters(paramVec);
	  mylar->setOuterColor("gray50");
	  mylar->setOuterMaterial("G4_MYLAR");
	  addDaughter(mylar);
	  m_detMoveList.push_back(mylar);
	} else {
	  mylar->cloneDetector(pos_mylar, rot_mylar);
	}
      }
    }
    
    
    setThisAndDaughterBriefName("DCV");
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. DCV is not constructed.");
  }

}



 
GsimE14DCV::~GsimE14DCV()
{
  ;
}

void GsimE14DCV::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  std::cout << "GsimE14DCV Z position is the center of DCV1." << std::endl;
  std::cout << "The center of design is (947.2 + L/2) mm from CsI front surface," << std::endl;
  std::cout << "where L=1410 is the length of DCV1." << std::endl;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14DCV::setFastSimulationLevel(int level)
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
