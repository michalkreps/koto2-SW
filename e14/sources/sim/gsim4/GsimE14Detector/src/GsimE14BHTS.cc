/**
 *  @file
 *  @brief  GsimE14BHTS
 *  $Id: $ 
 *  $Log: $
 */
#include "GsimE14Detector/GsimE14BHTS.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4SystemOfUnits.hh"

GsimE14BHTS::GsimE14BHTS(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14BHTS";
  //Downstream In-Beam Trigger


  double width=200*mm;
  double height=200*mm;
  double thick =5*mm;

  if(userFlag==201305 ||
     userFlag==201503 ||
     userFlag==201504 ||
     userFlag==201605
     ) {
    //200x200x5mm^3
    //zCetner = 168mm from downstream end of the module
    std::vector<double> paramVec;
    paramVec.push_back(width);
    paramVec.push_back(height);
    paramVec.push_back(thick);
    
    GsimBox* bhts = new GsimBox( "BHTS", this,
				 G4ThreeVector(0,0,0),
				 G4ThreeVector(0,0,0) );
    bhts->setParameters( paramVec );
    bhts->setOuterMaterial( "G4_PLASTIC_SC_VINYLTOLUENE" );
    bhts->setSensitiveDetector( "BHTS", 0);
    addDaughter( bhts );
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BHTS is not constructed.");
  }
}

GsimE14BHTS::~GsimE14BHTS()
{
  ;
}
void GsimE14BHTS::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::cout << "Place it at the center of CBAR." << std::endl;
  std::cout << "z=(614.8+479.2+100.+52)*cm+573.5*cm+(62.6+4.28)*cm+2*cm" << std::endl;
  std::cout << "When userFlag is 201605, place it donstream by 2 cm." << std::endl;
  std::cout << "z=(614.8+479.2+100.+52)*cm+573.5*cm+(62.6+4.28)*cm" << std::endl;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
void GsimE14BHTS::setFastSimulationLevel(int level)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
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

