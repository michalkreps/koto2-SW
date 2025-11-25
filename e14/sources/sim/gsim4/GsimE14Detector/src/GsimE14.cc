/**
 *  @file
 *  @brief  GsimE14
 *  $Id: GsimE14.cc,v 1.4 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14.cc,v $
 *  Revision 1.4  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14.h"
#include "GsimE14Detector/GsimE14UserGeom.h"

#include "GsimE14Detector/GsimE14Membrane.h"

#include "GsimE14Detector/GsimE14FBAR.h"
#include "GsimE14Detector/GsimE14CBAR.h"
#include "GsimE14Detector/GsimE14IBAR.h"
#include "GsimE14Detector/GsimE14IBCV.h"
#include "GsimE14Detector/GsimE14BCV.h"
#include "GsimE14Detector/GsimE14CV.h"
#include "GsimE14Detector/GsimE14LCV.h"
#include "GsimE14Detector/GsimE14CsICover.h"
#include "GsimE14Detector/GsimE14CSI.h"
#include "GsimE14Detector/GsimE14CsIBackPlate.h"
#include "GsimE14Detector/GsimE14OEV.h"
//#include "GsimE14Detector/GsimE14CC00.h"
//#include "GsimE14Detector/GsimE14CC01.h"
#include "GsimE14Detector/GsimE14NCC.h"
#include "GsimE14Detector/GsimE14CC02.h"
#include "GsimE14Detector/GsimE14CC03.h"
#include "GsimE14Detector/GsimE14CC04.h"
#include "GsimE14Detector/GsimE14CC05.h"
#include "GsimE14Detector/GsimE14BPCV.h"
#include "GsimE14Detector/GsimE14DCV.h"
#include "GsimE14Detector/GsimE14CC06.h"
#include "GsimE14Detector/GsimE14BHCV.h"
#include "GsimE14Detector/GsimE14NewBHCV.h"
#include "GsimE14Detector/GsimE14BHPV.h"
#include "GsimE14Detector/GsimE14Shield.h"
#include "GsimE14Detector/GsimE14BHGC.h"
#include "GsimE14Detector/GsimE14BHTS.h"
#include "GsimE14Detector/GsimE14UCV.h"

#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimTube.h"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>

using namespace E14;


GsimE14::GsimE14(std::string name,
		 GsimDetector* motherDetector,
		 G4ThreeVector transV,G4ThreeVector rotV,
		 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14";
  m_photoNuclearDetID = new int[1000];

  
  //userFlag
  //201212 : Air
  //201301 : Vacuum
  //201303 : CC06, BHCV, BHPV, Shield(air is filled inside the beam hole) 
  //         CC06 in March : 3mm thick acryrlic was attached for real situation
  //                           instead of 1mm thick in MC around the beam pipe.
  //201304 : CC05, CC06, BHCV, BHPV, Shield
  //201305 : CC05, CC06, BHCV, BHPV, Shield

  if(m_userFlag==201212) {
    GsimDetector::getWorldDetector()->setOuterMaterial("G4_AIR");
  }

  GsimE14Membrane* mbn = new GsimE14Membrane("mbn",this,
					     G4ThreeVector(0,0,0),
					     G4ThreeVector(0,0,0),m_userFlag);
  addDaughter(mbn);
  

  GsimE14FBAR* fbar = new GsimE14FBAR("fbar",this,
				      G4ThreeVector(0,0,(Fbar_Len/2.)*cm),
				      G4ThreeVector(0,0,0),m_userFlag);
  addDaughter(fbar);
  
  
  GsimE14CBAR* cbar = new GsimE14CBAR("cbar",this,
				      G4ThreeVector(0,0,(Detpos_cbar_z+Cbar_Len/2.)*cm),
				      G4ThreeVector(0,0,0),m_userFlag);
  addDaughter(cbar);

  GsimE14BCV* bcv =0;
  if(m_userFlag>=201605) {
    bcv = new GsimE14BCV("bcv",this,
			 G4ThreeVector(0,0,Detpos_bcv_z_201605*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  } else {
    bcv = new GsimE14BCV("bcv",this,
			 G4ThreeVector(0,0,Detpos_bcv_z*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  }
  if(bcv) addDaughter(bcv);


  if(m_userFlag>=201605) {
    GsimE14IBAR* ibar = new GsimE14IBAR("ibar",this,
					G4ThreeVector(0,0,Detpos_ibar_z*cm),
					G4ThreeVector(0,0,0),m_userFlag);
    addDaughter(ibar);
  }
  if(m_userFlag>=201605) {
    GsimE14IBCV* ibcv = new GsimE14IBCV("ibcv",this,
					G4ThreeVector(0,0,Detpos_ibcv_z*cm),
					G4ThreeVector(0,0,0),m_userFlag);
    addDaughter(ibcv);
  }




  
  
  GsimE14CV* cv = 0;
  if(m_userFlag>=201902) {
    cv = new GsimE14CV("cv",this,
		       G4ThreeVector(0,0,Detpos_cv_z_201902*cm),
		       G4ThreeVector(0,0,0),m_userFlag);
  } else if(m_userFlag>=201605) {
    cv = new GsimE14CV("cv",this,
		       G4ThreeVector(0,0,Detpos_cv_z_201605*cm),
		       G4ThreeVector(0,0,0),m_userFlag);
  } else {
    cv = new GsimE14CV("cv",this,
		       G4ThreeVector(0,0,Detpos_cv_z*cm),
		       G4ThreeVector(0,0,0),m_userFlag);
  }
  if(cv) addDaughter(cv);



  GsimE14CsICover* cover =0;
  if(m_userFlag>=201902) {
    cover = new GsimE14CsICover("cover",this,
				G4ThreeVector(0,0,Detpos_cover_z_201902*cm),
				G4ThreeVector(0,0,0),m_userFlag);
  } else if(m_userFlag>=201605) {
    cover = new GsimE14CsICover("cover",this,
				G4ThreeVector(0,0,Detpos_cover_z_201605*cm),
				G4ThreeVector(0,0,0),m_userFlag);
  } else {
    cover = new GsimE14CsICover("cover",this,
				G4ThreeVector(0,0,Detpos_cover_z*cm),
				G4ThreeVector(0,0,0),m_userFlag);
  }
  if(cover) addDaughter(cover);
  

  GsimE14CSI* csi =0;
  if(m_userFlag>=201605) {
    csi = new GsimE14CSI("csi",this,
			 G4ThreeVector(0,0,(Csir_Zmin+2+Csir_Len/2.)*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  } else {
    csi = new GsimE14CSI("csi",this,
			 G4ThreeVector(0,0,(Csir_Zmin+Csir_Len/2.)*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  }
  addDaughter(csi);

  GsimE14CsIBackPlate* plate=0;
  if(m_userFlag>=201605) {
    plate = new GsimE14CsIBackPlate("plate",this,
				    G4ThreeVector(0,0,Detpos_plate_z_201605*cm),
				    G4ThreeVector(0,0,0),m_userFlag);
  } else {
    plate = new GsimE14CsIBackPlate("plate",this,
				    G4ThreeVector(0,0,Detpos_plate_z*cm),
				    G4ThreeVector(0,0,0),m_userFlag);
  }
  addDaughter(plate);


  
  

  GsimE14OEV* oev =0;
  if(m_userFlag>=201605) {
    oev = new GsimE14OEV("oev",this,
			 G4ThreeVector(0,0,Detpos_oev_z_201605*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  } else {
    oev = new GsimE14OEV("oev",this,
			 G4ThreeVector(0,0,Detpos_oev_z*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  }
  addDaughter(oev);
  

//   double cc00Len= GsimE14CC00::getCC00Length();
//   GsimE14CC00* cc00 = new GsimE14CC00("cc00",this,
// 					  G4ThreeVector(0,0,(Detpos_cc00_z+cc00Len/2.)*cm),
// 					  G4ThreeVector(0,0,0));
//   addDaughter(cc00);

//   double cc01Len= GsimE14CC01::getCC01Length();
//   GsimE14CC01* cc01 = new GsimE14CC01("cc01",this,
// 					  G4ThreeVector(0,0,(Detpos_cc01_z+cc01Len/2.)*cm),
// 					  G4ThreeVector(0,0,0));
//   addDaughter(cc01);

  // double cc02Len= GsimE14CC02::getCC02Length();
  // GsimE14CC02* cc02 = new GsimE14CC02("cc02",this,
  // 					  G4ThreeVector(0,0,(Fbar_Len-Detpos_cc02_z_shift-cc02Len/2.)*cm),
  // 					  G4ThreeVector(0,0,0));
  // addDaughter(cc02);

  
  GsimE14NCC* ncc = new GsimE14NCC("ncc",this,
				   G4ThreeVector(0,0,(Fbar_Len-Detpos_ncc_z_shift-NCC_Len/2.)*cm),
				   G4ThreeVector(0,0,0),m_userFlag);

  addDaughter(ncc);
  
  
  GsimE14CC03* cc03 =0;
  if(m_userFlag>=201605) {
    cc03 = new GsimE14CC03("cc03",this,
			   G4ThreeVector(0,0,(Detpos_cc03_z_201605+CC03_Len/2.)*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
  } else {
    cc03 = new GsimE14CC03("cc03",this,
			   G4ThreeVector(0,0,(Detpos_cc03_z+CC03_Len/2.)*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
  }
  if(cc03) addDaughter(cc03);


  GsimE14LCV* lcv =0;
  if(m_userFlag>=201902) {
    lcv = new GsimE14LCV("lcv",this,
			 G4ThreeVector(0,0,(Detpos_lcv_z_201902+CC03_Len/2.)*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  } else if(m_userFlag>=201605) {
    lcv = new GsimE14LCV("lcv",this,
			 G4ThreeVector(0,0,(Detpos_lcv_z_201605+CC03_Len/2.)*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  } else {
    lcv = new GsimE14LCV("lcv",this,
			 G4ThreeVector(0,0,(Detpos_lcv_z+CC03_Len/2.)*cm),
			 G4ThreeVector(0,0,0),m_userFlag);
  }
  if(lcv) addDaughter(lcv);


  double cc04Len= GsimE14CC04::getCC04Length();
  GsimE14CC04* cc04 = 0;
  if(m_userFlag>=201605) {
    cc04 = new GsimE14CC04("cc04",this,
			   G4ThreeVector(0,0,(Detpos_cc04_z_201605+cc04Len/2.)*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
  } else {
    cc04 = new GsimE14CC04("cc04",this,
			   G4ThreeVector(0,0,(Detpos_cc04_z+cc04Len/2.)*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
    
  }
  if(cc04) addDaughter(cc04);


  if(m_userFlag==201212 || m_userFlag==201301) {
    //No downstream counters
  } else {
    GsimE14CC05* cc05 = new GsimE14CC05("cc05",this,
					G4ThreeVector(0,0,0),
					G4ThreeVector(0,0,0),m_userFlag);
    double cc05Len=cc05->getCC05Length();
    if(m_userFlag>=201902) {
      G4ThreeVector CC05v(0,0,(Detpos_cc05_z_201902+cc05Len/2.)*cm);
      cc05->setTranslationVector(CC05v);
    } else if(m_userFlag>=201605) {
      G4ThreeVector CC05v(0,0,(Detpos_cc05_z_201605+cc05Len/2.)*cm);
      cc05->setTranslationVector(CC05v);
    } else if(m_userFlag>=201504) {
      G4ThreeVector CC05v(0,0,(Detpos_cc05_z_201504+cc05Len/2.)*cm);
      cc05->setTranslationVector(CC05v);
    } else {
      G4ThreeVector CC05v(0,0,(Detpos_cc05_z+cc05Len/2.)*cm);
      cc05->setTranslationVector(CC05v);
    }
    addDaughter(cc05);
    
    if(m_userFlag==201503 || m_userFlag==201504 ) {
      double bpcvLen=100;//cm
      GsimE14BPCV* bpcv = new GsimE14BPCV("bpcv",this,
					  G4ThreeVector(0,0,(Detpos_bpcv_z+bpcvLen/2.)*cm),
					  G4ThreeVector(0,0,0),m_userFlag);
      addDaughter(bpcv);
    } else if(m_userFlag==201605) {
      double bpcvLen=100;//cm
      GsimE14BPCV* bpcv = new GsimE14BPCV("bpcv",this,
					  G4ThreeVector(0,0,(Detpos_bpcv_z_201605+bpcvLen/2.)*cm),
					  G4ThreeVector(0,0,0),m_userFlag);
      addDaughter(bpcv);
    }

    if(m_userFlag>=201902) {
      GsimE14DCV* dcv = new GsimE14DCV("bpcv",this,
				       G4ThreeVector(0,0,Detpos_dcv_z*cm),
				       G4ThreeVector(0,0,0),m_userFlag);
      addDaughter(dcv);
    }
    
    
    
    
    GsimE14CC06* cc06 = new GsimE14CC06("cc06",this,
					G4ThreeVector(0,0,0),
					G4ThreeVector(0,0,0),m_userFlag);
    double cc06Len= cc06->getCC06Length();
    if(m_userFlag>=201902) {
      G4ThreeVector CC06v(0,0,(Detpos_cc06_z_201902+cc06Len/2.)*cm);
      cc06->setTranslationVector(CC06v);
    } else if(m_userFlag>=201605) {
      G4ThreeVector CC06v(0,0,(Detpos_cc06_z_201605+cc06Len/2.)*cm);
      cc06->setTranslationVector(CC06v);
    } else {
      G4ThreeVector CC06v(0,0,(Detpos_cc06_z+cc06Len/2.)*cm);
      cc06->setTranslationVector(CC06v);
    }
    if(cc06) addDaughter(cc06);
    

    GsimE14NewBHCV* newbhcv = 0;
    if(m_userFlag>=201605) {
      newbhcv = new GsimE14NewBHCV("newbhcv",this,
				   G4ThreeVector(-0.08*cm,-0.13*cm,(Detpos_newbhcv_z_201605)*cm),
				   G4ThreeVector(0,0,0),m_userFlag);
    } else if(m_userFlag==201504) {
      newbhcv = new GsimE14NewBHCV("newbhcv",this,
				   G4ThreeVector(-0.08*cm,-0.13*cm,(Detpos_newbhcv_z)*cm),
				   G4ThreeVector(0,0,0),m_userFlag);
      
    }
    if(newbhcv) addDaughter(newbhcv);
    
    if(m_userFlag<=201504) {
      GsimE14BHCV* bhcv = new GsimE14BHCV("bhcv",this,
					  G4ThreeVector(0,0,(Detpos_bhcv_z)*cm),
					  G4ThreeVector(0,0,0),m_userFlag);
      addDaughter(bhcv);
    }

    double ShieldLen=GsimE14Shield::getShieldLength();
    GsimE14Shield* shield = 0;
    if(m_userFlag>201605) {
      shield = new GsimE14Shield("shield",this,
				 G4ThreeVector(0,0,(Detpos_shield_z_201605+ShieldLen/2)*cm),
				 G4ThreeVector(0,0,0),m_userFlag);
    } else {
      shield = new GsimE14Shield("shield",this,
				 G4ThreeVector(0,0,(Detpos_shield_z+ShieldLen/2)*cm),
				 G4ThreeVector(0,0,0),m_userFlag);
    }
    addDaughter(shield);


    GsimE14BHPV* bhpv = new GsimE14BHPV("bhpv",this,
					G4ThreeVector(0,0,0),
					G4ThreeVector(0,0,0),m_userFlag);
    
    double BHPVLen=bhpv->getBHPVLength();
    if(m_userFlag>=201504) {
      G4ThreeVector BHPVv(0,0,(Detpos_bhpv_z_201504+BHPVLen/2)*cm);
      bhpv->setTranslationVector(BHPVv);
    } else {
      G4ThreeVector BHPVv(0,0,(Detpos_bhpv_z+BHPVLen/2)*cm);
      bhpv->setTranslationVector(BHPVv);
    }
    addDaughter(bhpv);

    if(m_userFlag>=201503) {
      double bhgc_z=Detpos_bhpv_z_201504+BHPVLen+Detpos_bhgc_z_shift;
      GsimE14BHCV* bhgc = new GsimE14BHCV("bhgc",this,
					  G4ThreeVector(0,0,(bhgc_z)*cm),
					  G4ThreeVector(0,0,0),m_userFlag);
      addDaughter(bhgc);
    }
    
    GsimE14BHTS* bhts = 0;
    if(m_userFlag==201504) {
      bhts = new GsimE14BHTS("bhts",this,
			     G4ThreeVector(0,0,(Detpos_bhpv_z_201504+BHPVLen
						+Detpos_bhts_z_shift_201504)*cm),
			     G4ThreeVector(0,0,0),m_userFlag);
    } else if(m_userFlag<=201605) {
      bhts = new GsimE14BHTS("bhts",this,
			     G4ThreeVector(0,0,(Detpos_bhpv_z+BHPVLen+Detpos_bhts_z_shift)*cm),
			     G4ThreeVector(0,0,0),m_userFlag);
    }
    if(bhts) addDaughter(bhts);
    
    GsimE14UCV* ucv = 0;
    if(m_userFlag==202002) {
      ucv = new GsimE14UCV("ucv",this,
			   G4ThreeVector(0,0,Detpos_ucv_z_202002*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
    } else if(m_userFlag>=202102) {
      ucv = new GsimE14UCV("ucv",this,
			   G4ThreeVector(0,0,Detpos_ucv_z_202102*cm),
			   G4ThreeVector(0,0,0),m_userFlag);
    }
    if(ucv) addDaughter(ucv);
  }
}


GsimE14::~GsimE14()
{
  delete [] m_photoNuclearDetID;
}



void GsimE14::beginOfRunAction()
{
  GsimPersistencyManager* pm= GsimPersistencyManager::getPersistencyManager();
  pm->setBranchOf("eventTree",
		  "photoNuclearN",&m_photoNuclearN,"photoNuclearN/I");
  pm->setBranchOf("eventTree",
		  "photoNuclearDetID",m_photoNuclearDetID,"photoNuclearDetID[photoNuclearN]/I");
}

void GsimE14::endOfRunAction()
{
  ;
}

bool GsimE14::beginOfEventAction(const G4Event*)
{
  m_photoNuclearN=0;
  return true;
}

bool GsimE14::endOfEventAction(const G4Event*)
{
  return false;
}

bool GsimE14::steppingAction(const G4Step* theStep)
{
  G4Track* track = theStep->GetTrack();
  G4String theProName =""; 
  if(theStep->GetPostStepPoint()->GetProcessDefinedStep() != 0){
    theProName = 
      theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  }
  if(theProName == "PhotonInelastic") {
    GsimPersistencyManager* persistencyManager
      = GsimPersistencyManager::getPersistencyManager();
    std::string volumeName = std::string( (track->GetVolume()->GetName()).data() );
    int detID=persistencyManager->getDetectorIDByPhysicalVolumeName(volumeName);
    int briefID = persistencyManager->getDetectorBriefIDByID(detID);
    if(briefID!=-1) {
      if(m_photoNuclearN<1000) {
	m_photoNuclearDetID[m_photoNuclearN]=briefID;
	m_photoNuclearN++;
      }
    }
  }
  return true;
}



void GsimE14::setFastSimulationLevel(int level)
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
