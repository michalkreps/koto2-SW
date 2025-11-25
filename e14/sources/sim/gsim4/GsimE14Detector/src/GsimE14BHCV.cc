/**
 *  @file
 *  @brief  GsimE14BHCV
 *  $Id: GsimE14BHCV.cc,v 1.3 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14BHCV.cc,v $
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14BHCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14BHCV::GsimE14BHCV(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14BHCV";
  // BHCV



  if(
     m_userFlag==201504 ||
     m_userFlag==201503 ||
     m_userFlag==201305 ||
     m_userFlag==201304 ||
     m_userFlag==201303 ||
     m_userFlag==202306
     ) {
    double  s_width  =  115*mm;
    double  s_height =   60*mm;
    double  s_thick  =    3*mm;

    std::vector<double> paramVec;
    paramVec.push_back(s_width);
    paramVec.push_back(s_height);
    paramVec.push_back(s_thick);

    double overlap_v = 5*mm;
    double overlap_h = 5*mm;

    double y_offset=-s_height*1.5+overlap_v*1.5;

    char nam[100];
    sprintf(nam,"BHCV");
    GsimBox* box=0;

    int modIDArray[8]=
      {
	0,2,1,3,   // North from bottom to top
	4,6,5,7    // South from bottom to top
    };
    double  zArray[8]=
      {
	+1.5*mm,+4.5*mm,+1.5*mm,+4.5*mm,// North from bottom to top
	-4.5*mm,-1.5*mm,-4.5*mm,-1.5*mm // South from bottom to top
      };


    int cnt=0;
    for(int ix=0;ix<2;ix++) {
      double x=-overlap_h/2.+s_width/2.;//North
      if(ix==1) {
	x=+overlap_h/2.-s_width/2.;//South
      }

      for (int iy=0; iy<4; iy++ ){
	double y=y_offset+(s_height-overlap_v)*iy;

	double z=zArray[cnt];
	if(box==0) {
	  box = new GsimBox(std::string(nam),this,
			    G4ThreeVector(x,y,z),
			    G4ThreeVector(0,0,0));
	  box->setParameters(paramVec);
	  box->setSensitiveDetector("BHCV",modIDArray[cnt]);
	  addDaughter(box);
	} else {
	  box->cloneDetector(G4ThreeVector(x,y,z),
			     G4ThreeVector(0,0,0),
			     modIDArray[cnt]);
	}
	cnt++;
      }
    }
    setThisAndDaughterBriefName("BHCV");
    
  } else if (m_userFlag==2011) {
    // Charged Veto
    //double c_bhcv_z = Detpos_bhcv_z + 7.0 + 5.0;

    double  s_width  = 10.5;
    double  s_height =  3.0;
    double  s_thick  =  0.3; // 3 mmt  plastic scintillator

    std::vector<double> paramVec;
    paramVec.push_back(s_width*cm);
    paramVec.push_back(s_height*cm);
    paramVec.push_back(s_thick*cm);

    double overlap_v = 0.5;
    double overlap_h = 0.5;

    double y_offset=-s_height*3.5+overlap_v*3;
  
    char nam[100];
    sprintf(nam,"BHCV");
    GsimBox* box=0;
    int cnt=0;
    for (int iy=0; iy<8; iy++ ){
      double y=y_offset+(s_height-overlap_v)*iy;
      for(int ix=0;ix<2;ix++) {
	double x=-(s_width/2. - overlap_h/2.);
	if(ix==1) {
	  x=(s_width/2. - overlap_h/2.);
	  y+=overlap_v/2.;
	}
      
	double z=0;
	int iz=cnt%4;
	if(iz==0) {
	  z=-s_thick*1.5;
	} else if(iz==1) {
	  z=s_thick*1.5;
	} else if(iz==2) {
	  z=s_thick*0.5;
	} else {
	  z=-s_thick*0.5;
	}
      
	if(box==0) {
	  box = new GsimBox(std::string(nam),this,
			    G4ThreeVector(x*cm,y*cm,z*cm),
			    G4ThreeVector(0,0,0));
	  box->setParameters(paramVec);
	  box->setSensitiveDetector("BHCV",cnt);
	  addDaughter(box);
	} else {
	  box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
			     G4ThreeVector(0,0,0),
			     cnt);
	}
	cnt++;
      }
    }
    setThisAndDaughterBriefName("BHCV");
  } else if(m_userFlag>=201606) {
    //uninstalled
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BHCV is not constructed.");
  }
  m_detMoveList.push_back(this);
}

GsimE14BHCV::~GsimE14BHCV()
{
  ;
}

double GsimE14BHCV::getBHCVLength() {
  double  s_thick  =  0.3; // 3 mmt  plastic scintillator
  return s_thick*4;
}


void GsimE14BHCV::setFastSimulationLevel(int level)
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


void GsimE14BHCV::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::cout << "GsimE14BHCV Z position:" << std::endl;
  std::cout << "    Center of total z length of BHCV." << std::endl;
  std::cout << "Uninstalled from 201605." << std::endl;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}
