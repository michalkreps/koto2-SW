/**
 *  @file
 *  @brief  GsimKOTOIIBarrel
 *  $Id: 
 *  $Log:
 */
#include "GsimKOTOIIDetector/GsimKOTOIIBarrel.h"
#include "GsimKOTOIIDetector/GsimKOTOIIUserGeom.h"
#include "GsimDetector/GsimTetraExtruded2.h"
#include "GsimDetector/GsimTube.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

using namespace KOTOII;

GsimKOTOIIBarrel::GsimKOTOIIBarrel(std::string name,
				   GsimDetector* motherDetector,
				   G4ThreeVector transV,G4ThreeVector rotV,
				   int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimKOTOIIBarrel";

  const int nSection=7;
  GsimTetraExtruded2* module[nSection]={0};
  double Din[nSection]={
     550,
     800,
    1250,
    1700,
    2150,
    2600,
    3050
  };//mm
  double nDiv[nSection]={
    20,
    20,
    24,
    32,
    32,
    32,
    32
  };
  double nZ[nSection]={
    2,
    2,
    4,
    4,
    4,
    4,
    4
  };

  double zStart=0;
  double z1=zStart;
  double z2=zStart;
  double dzModule=850;//mm
  double dzPb=1;//mm
  double dzSci=5.05;//mm
  double zOffset=1.5;
  int    nLayer=140;
  
  double tGap=3;//mm
  double dR=450;//mm
  double Rin[nSection];
  double Rout[nSection];
  for(int i=0;i<nSection;i++) {
    Rin[i]=Din[i]/2.;//mm
    Rout[i]=Rin[i]+dR;//mm
  }
  char nam[100];
  std::vector<double> paramVec;

  int modId=0;
  for(int i=0;i<nSection;i++) {
    z2=z1;
    double dzSection=dzModule*nZ[i];
    double ang=2*TMath::Pi()/nDiv[i];//rad
    double hout=Rout[i]*TMath::Tan(ang/2.)*2-tGap;
    double x3=+hout/2.-dR*TMath::Sin(ang);
    double y3=Rout[i]-dR*TMath::Cos(ang);

    double xm[4]={
      -hout/2.,-hout/2.,x3,+hout/2.
    };
    
    double ym[4]={
      Rout[i],Rin[i],y3,Rout[i]
    };

    paramVec.clear();
    for(int k=0;k<4;k++) {
      paramVec.push_back(xm[k]*mm);
      paramVec.push_back(ym[k]*mm);
    }
    paramVec.push_back(dzModule*mm);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(1);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(1);

    for(int j=0;j<nZ[i];j++) {
      for(int k=0;k<nDiv[i];k++) {
	if(j==0 && k==0) {
	  std::sprintf(nam,"BAR%d",i);
	  module[i] = new GsimTetraExtruded2(std::string(nam),this,
					     G4ThreeVector(0,0,(z1+dzModule/2.)*mm),
					     G4ThreeVector(0.,0, (-M_PI/2.+2*M_PI/nDiv[i]*k)*rad)
					     );
	  module[i]->setParameters(paramVec);
	  module[i]->setOuterVisibility(true);
	  addDaughter(module[i]);
	  module[i]->setSensitiveDetector("BAR",modId);
	  
	} else {
	  module[i]->cloneDetector(G4ThreeVector(0,0,(z1+dzModule/2.)*mm),
				   G4ThreeVector(0.,0, (-M_PI/2.+2*M_PI/nDiv[i]*k)*rad),
				   modId
				   );
	}
	modId++;
      }
      z1+=dzModule;
    }

    
    paramVec.clear();
    for(int k=0;k<4;k++) {
      paramVec.push_back(xm[k]*mm);
      paramVec.push_back(ym[k]*mm);
    }
    paramVec.push_back(dzPb*mm);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(1);
    paramVec.push_back(0);
    paramVec.push_back(0);
    paramVec.push_back(1);

    GsimTetraExtruded2* pb=0;
    for(int j=0;j<nLayer;j++) {
      if(j==0) {
	std::sprintf(nam,"Pb%d",i);
	pb = new GsimTetraExtruded2(std::string(nam),module[i],
				    G4ThreeVector(0,0,(-dzModule/2.+zOffset+j*(dzPb+dzSci)+dzPb/2.)*mm),
				    G4ThreeVector(0,0,0)
				    );
	pb->setOuterMaterial("G4_Pb");
	pb->setParameters(paramVec);
	pb->setOuterVisibility(true);
	pb->setOuterColor("gray50");
	module[i]->addDaughter(pb);
      } else {
	pb->cloneDetector(G4ThreeVector(0,0,(-dzModule/2.+zOffset+j*(dzPb+dzSci)+dzPb/2.)*mm),
			  G4ThreeVector(0,0,0));
      }
    }

    double chamberRin=Rout[i]+200;//mm
    double chamberRout=chamberRin+20;//mm
    double dzFlange=80;//mm
    paramVec.clear();
    paramVec.push_back(chamberRin*mm);
    paramVec.push_back(chamberRout*mm);
    paramVec.push_back((dzSection-dzFlange*2)*mm);
    paramVec.push_back(0*deg);
    paramVec.push_back(360*deg);

    std::sprintf(nam,"Tank%d",i);
    GsimTube* tank = new GsimTube(std::string(nam),this,
				  G4ThreeVector(0,0,(z2+dzSection/2.)*mm),
				  G4ThreeVector(0,0,0)
				  );
    tank->setOuterMaterial("GsimSUS");
    tank->setOuterColor("gray50");
    tank->setParameters(paramVec);
    tank->setOuterVisibility(true);
    addDaughter(tank);


    paramVec.clear();
    paramVec.push_back(chamberRin*mm);
    paramVec.push_back((chamberRout+150)*mm);
    paramVec.push_back(dzFlange*mm);
    paramVec.push_back(0*deg);
    paramVec.push_back(360*deg);

    GsimTube* flange = 0;
    std::sprintf(nam,"USFlange%d",i);
    flange = new GsimTube(std::string(nam),this,
			  G4ThreeVector(0,0,(z2+dzFlange/2.)*mm),
			  G4ThreeVector(0,0,0)
			  );
    flange->setOuterMaterial("GsimSUS");
    flange->setOuterColor("gray50");
    flange->setParameters(paramVec);
    flange->setOuterVisibility(true);
    addDaughter(flange);


    paramVec.clear();
    paramVec.push_back(chamberRin*mm);
    if(i==nSection-1) {
      paramVec.push_back((Rout[i]+200+20+150)*mm);
    } else {
      paramVec.push_back((Rout[i+1]+200+20+150)*mm);
    }
    paramVec.push_back(dzFlange*mm);
    paramVec.push_back(0*deg);
    paramVec.push_back(360*deg);
      
    std::sprintf(nam,"DSFlange%d",i);
    flange = new GsimTube(std::string(nam),this,
			  G4ThreeVector(0,0,(z2+dzSection-dzFlange/2.)*mm),
			  G4ThreeVector(0,0,0)
			  );
    flange->setOuterMaterial("GsimSUS");
    flange->setOuterColor("gray50");
    flange->setParameters(paramVec);
    flange->setOuterVisibility(true);
    addDaughter(flange);
  }


  
  setThisAndDaughterBriefName("BAR");
}

GsimKOTOIIBarrel::~GsimKOTOIIBarrel()
{
  ;
}

void GsimKOTOIIBarrel::comment()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::cout << "Place it at (0,0,0)" << std::endl;


#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


void GsimKOTOIIBarrel::setFastSimulationLevel(int level)
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
      pos.setZ(z+detMoveZ*cm);
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
      pos.setZ(z-detMoveZ*cm);
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

