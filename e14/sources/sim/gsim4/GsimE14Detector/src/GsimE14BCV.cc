/**
 *  @file
 *  @brief  GsimE14BCV
 *  $Id: GsimE14BCV.cc,v 1.3 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14BCV.cc,v $
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14BCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14BCV::GsimE14BCV(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14BCV";

  if(m_userFlag==201301 ||
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

    if(m_userFlag>=201605 ) {
      m_modID[0]=15;
      for(int i=1;i<32;i++) m_modID[i]=(i-1)/2; //MBCV: 15(31,0),0(1,2),1(2,3),...
    }else{
      for(int i=0;i<32;i++) m_modID[i]=i; //BCV: 0,1,2,3,...
    }

    double  bcv_x = 0.5;
    double  bcv_y = 19.72;
    double  bcv_z = Cbar_Len;
    if(m_userFlag>=201605 ) {
      bcv_z = 103.;
    }

    double  x = 100.3 + bcv_x/2;
    double  y = 0;
    //double  z = 135 + Cbar_Len/2;
    
    std::vector<double> paramVec;
    paramVec.push_back(bcv_x*cm);
    paramVec.push_back(bcv_y*cm);
    paramVec.push_back(bcv_z*cm);
    
    char nam[100];
    G4ThreeVector c1ini( x*cm,         y*cm,     0 );
    G4ThreeVector c2ini( (x+0.5)*cm, (y-0.5)*cm, 0 );
    if(m_userFlag>=201605 ) {
      sprintf(nam,"MBCV");
    } else {
      sprintf(nam,"BCV");
    }
    GsimBox* box = new GsimBox(std::string(nam),this,
			       c1ini,
			       G4ThreeVector(0,0,0));
    box->setParameters(paramVec);
    if(m_userFlag>=201605 ) {
      box->setSensitiveDetector("MBCV",m_modID[0]);
    } else {
      box->setSensitiveDetector("BCV",m_modID[0]);
    }
    addDaughter(box);
    
    
    for( int i=0;i<32;i++ ){
      G4ThreeVector c1( x*cm,         y*cm,     0 );
      G4ThreeVector c2( (x+0.5)*cm, (y-0.5)*cm, 0 );
      c1.rotateZ( 2.*M_PI*i/32 );
      c2.rotateZ( 2.*M_PI*i/32 );

      if(i!=0) {
	box->cloneDetector(c1,
			   G4ThreeVector(0,0,2.*M_PI*i/32),m_modID[i]);
      }
      
      box->cloneDetector(c2,
			 G4ThreeVector(0,0,2.*M_PI*i/32),m_modID[i]);
    }
    if(m_userFlag>=201605 ) {
      setThisAndDaughterBriefName("MBCV");
    } else {
      setThisAndDaughterBriefName("BCV");
    }
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BCV is not constructed.");
  }
  
  m_detMoveList.push_back(this);
}

GsimE14BCV::~GsimE14BCV()
{
  ;
}

void GsimE14BCV::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::cout << "Place it at the center of CBAR." << std::endl;
  std::cout << "z=(134.8+550./2.)*cm" << std::endl;
  std::cout << "For userFlag >= 201605, it is MBCV." << std::endl;
  std::cout << "Place MBCV at 103/2 mm upstream from the end of CBAR." << std::endl;
  std::cout << "z=(134.8+550.-103/2.+1)*cm" << std::endl;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


void GsimE14BCV::setFastSimulationLevel(int level)
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

