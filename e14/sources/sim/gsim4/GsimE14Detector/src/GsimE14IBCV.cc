/**
 *  @file
 *  @brief  GsimE14IBCV
 *  $Id: GsimE14IBCV.cc,v 1.3 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14IBCV.cc,v $
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14IBCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14IBCV::GsimE14IBCV(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14IBCV";
  
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
  

  double  bcv_x = 0.5;
  double  bcv_y = 14.5;
  double  bcv_top_y = 17.7;
  double  bcv_bottom_y = 11.4;
  //double  bcv_z = Cbar_Len;
  double  bcv_z = 267.6;

  double mod_rotate = 0.5; // phi rotation (2*PI=32*mod_rotate)
  double mod_slidey = 1.5; // fine tuneing of position due to rotation (see top and neighbor modules)

  std::vector<double> paramVec;
  paramVec.push_back(bcv_x*cm);
  paramVec.push_back(bcv_y*cm);
  paramVec.push_back(bcv_z*cm);

  char nam[100];
  sprintf(nam,"IBCV");
  GsimBox* box[3] = {}; // 0...normal, 1...top, 2...bottom

  double  x = 74.5 + bcv_x/2;
  double  y = -1*mod_slidey; // north modules
//  double  z = 135 + Cbar_Len/2;

  for( int i=0;i<32;i++ ){

    if( i==7 || i==23 ){ // top and bottom module
      y = 0;
    }else if( i>7 && i<23 ){ // south modules (Not top nor bottom module)
      y =  mod_slidey;
    }else if( i>23 ){ // south modules (Not top nor bottom module)
      y =  -1*mod_slidey; // north modules
    }

    G4ThreeVector c1ini( x*cm,         y*cm,     0 );
    c1ini.rotateZ( 2.*M_PI*(i+mod_rotate)/32 );

    if( i==0 ){ // module ID=0 and (for normal (north and south) modules)
      box[0] = new GsimBox(std::string(nam),this,
			     c1ini,
			     G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32.));
      box[0]->setParameters(paramVec);
      box[0]->setSensitiveDetector("IBCV");
      addDaughter(box[0]);
    }else if( i<7 ){ // north modules
      box[0]->cloneDetector(c1ini,
			 G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32),i);
    }else if( i==7 ){ // top module
      paramVec.clear();
      paramVec.push_back(bcv_x*cm);
      paramVec.push_back(bcv_top_y*cm);
      paramVec.push_back(bcv_z*cm);
      sprintf(nam,"IBCV1");
      box[1] = new GsimBox(std::string(nam),this,
			     c1ini,
			     G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32.));
      box[1]->setParameters(paramVec);
      box[1]->setOuterCopyNo(i);
      box[1]->setSensitiveDetector("IBCV");
      addDaughter(box[1]);
    }else if( i<23 ){ // south modules
      box[0]->cloneDetector(c1ini,
			 G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32),i);
    }else if( i==23 ){ // bottom module
      paramVec.clear();
      paramVec.push_back(bcv_x*cm);
      paramVec.push_back(bcv_bottom_y*cm);
      paramVec.push_back(bcv_z*cm);
      sprintf(nam,"IBCV2");
      box[2] = new GsimBox(std::string(nam),this,
			     c1ini,
			     G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32.));
      box[2]->setParameters(paramVec);
      box[2]->setOuterCopyNo(i);
      box[2]->setSensitiveDetector("IBCV");
      addDaughter(box[2]);
    }else{ // ID>23 north modules
      box[0]->cloneDetector(c1ini,
			 G4ThreeVector(0,0,2.*M_PI*(i+mod_rotate)/32),i);
    }
  }

  setThisAndDaughterBriefName("IBCV");
  m_detMoveList.push_back(this);
}


void GsimE14IBCV::comment()
{
  std::cout << "GsimE14IBCV Z position:" << std::endl;
  std::cout << "    Place IBCV center 227.5*mm downstream of CBAR center." << std::endl;
}


GsimE14IBCV::~GsimE14IBCV()
{
  ;
}


void GsimE14IBCV::setFastSimulationLevel(int level)
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
