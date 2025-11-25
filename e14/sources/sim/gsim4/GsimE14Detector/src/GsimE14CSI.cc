/**
 *  @file
 *  @brief  GsimE14CSI
 *  $Id:  GsimE14CSI.cc,v  2012/05/14 14:36:58 sato Exp
 *  $Log: GsimE14CSI.cc,v $
 *
 *  Revision 2.0  2012/05/14 14:34  sato
 *  make CsI ID assignment left-right revarsal  
 *
 *  Revision 1.9  2007/02/04 12:56:55  nanjo
 *  GsimBoxWithAHole is used.
 *
 *  Revision 1.8  2006/12/06 18:59:15  nanjo
 *  Etot trigger is added.
 *
 *
 */
#include "GsimE14Detector/GsimE14CSI.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimKernel/GsimTrackInformation.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimData/GsimEventData.h"

#include "G4Track.hh"
#include "G4Color.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <list>

using namespace E14; 


GsimE14CSI::GsimE14CSI(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{

  m_className = "GsimE14CSI";
  if(
     ! (userFlag == 201212 ||
	userFlag == 201301 ||
	userFlag == 201303 ||
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
	       "Invalid user flag is assigned. CSI is not constructed.");
    return;
  }

  m_model=1;
  //0:20cm beam hole
  //1:15cm beam hole
  m_totEThreshold=0;
  
  // CsI crystal
  GsimTube* tube=0;
  
  // CsI region
  //double  csir_rmin = 0;
  //double  csir_rmax = 97.3;
  //double  csir_rmax = 98;
  double  csir_rmax = 96.5; // corrected by Matsumura

  char nam[100];

  // CsI Cylinder
  //   CsIRegion
  //      static double  Csir_Zmin    = 609.8;
  //      static double  Csir_Len     = 180; // CsI region
  //   CsICrystal
  //      static double  Csic_Zmin    = 614.8;
  //   Detector placemnet : Z=Csir_Zmin+Csir_Len/2.)*cm
  //   Offset:              Csir_Zmin-Csic_Zmin = 50*mm;
  //
  //   Cylinder
  //     CSCL:  12*mm thick : 510*mm
  //     CSCLD: 19*mm thick : 1800*mm-510*mm
  //    EndCap
  //      innerR    = 1894*mm
  //        Rcenter=333.4*mm from CsISurface
  //      thickness = 12*mm
  //      
  //    VacuumPort
  //      innerD=208.3*mm
  //      outerD=216.3*mm
  //        downstreamZ=2300*mm from CsI surface
  //        length = 90*mm

  double crystalZOffset = Csic_Zmin-Csir_Zmin;
  
  double  cscl_thick = 1.2;   // 12 mm thick SUS corrected by Matsumura
  double  cscl_len  = 51;     //510*mm
  double  cscl_rmax = csir_rmax;
  double  cscl_rmin = cscl_rmax - cscl_thick;
  double  cscl_zpos = -Csir_Len/2.+crystalZOffset+cscl_len/2.;
  
  std::vector<double> paramVec;
  paramVec.push_back(cscl_rmin*cm);
  paramVec.push_back(cscl_rmax*cm);
  paramVec.push_back(cscl_len*cm);
  paramVec.push_back(0.*deg);
  paramVec.push_back(360.*deg);

  sprintf(nam,"CSCL");
  tube = new GsimTube(std::string(nam),this,
		      G4ThreeVector(0*cm,0*cm,cscl_zpos*cm),
		      G4ThreeVector(0,0,0));
  tube->setOuterMaterial("G4_Fe");
  tube->setParameters(paramVec);
  tube->setOuterColor("gray70");
  addDaughter(tube);
  m_detMoveList.push_back(tube);


  double  cscld_thick = 1.9;   // 19 mm thick SUS
  double  cscld_len  = 180-51; // (1800-510)*mm
  double  cscld_rmax = csir_rmax;
  double  cscld_rmin = cscld_rmax - cscld_thick;
  double  cscld_zpos = -Csir_Len/2.+crystalZOffset+cscl_len+cscld_len/2.;
  
  paramVec.clear();
  paramVec.push_back(cscld_rmin*cm);
  paramVec.push_back(cscld_rmax*cm);
  paramVec.push_back(cscld_len*cm);
  paramVec.push_back(0.*deg);
  paramVec.push_back(360.*deg);

  sprintf(nam,"CSCLD");
  tube = new GsimTube(std::string(nam),this,
		      G4ThreeVector(0*cm,0*cm,cscld_zpos*cm),
		      G4ThreeVector(0,0,0));
  tube->setOuterMaterial("G4_Fe");
  tube->setParameters(paramVec);
  tube->setOuterColor("gray70");
  addDaughter(tube);
  m_detMoveList.push_back(tube);



  ////////////////////////////////////////////
  // CsI Cylinder Endcap (old version)
  // double  csec_thick = 2.0; // 20 mm thick SUS
  // double  csec_rmax = csir_rmax - cscl_thick;
  // double  csec_rmin = 20;
  // paramVec.clear();
  // paramVec.push_back(csec_rmin*cm);
  // paramVec.push_back(csec_rmax*cm);
  // paramVec.push_back(csec_thick*cm);
  // paramVec.push_back(0.*deg);
  // paramVec.push_back(360.*deg);
  
  // sprintf(nam,"CSEC");
  // GsimTube* ecap = new GsimTube(std::string(nam),this,
  // 				G4ThreeVector(0*cm,0*cm,(Csir_Len/2-cscl_thick/2)*cm),
  // 				G4ThreeVector(0,0,0));
  // ecap->setOuterMaterial("G4_Fe");
  // ecap->setParameters(paramVec);
  // ecap->setOuterColor("gray70");
  // addDaughter(ecap);
  // m_detMoveList.push_back(ecap);
  ////////////////////////////////////////////


  // CsI Cylinder Endcap 
  // endcap vacuum port
  std::list<GsimTube*> ecapList;
  
  double  cecp_rmin = 33.96/2.0;
  double  cecp_rmax = 35.56/2.0;
  double  cecp_len  = 21.1-6;
  double cecp_pos = -Csir_Len/2.+crystalZOffset+236.5-cecp_len/2.;
  paramVec.clear();
  paramVec.push_back(cecp_rmin*cm);
  paramVec.push_back(cecp_rmax*cm);
  paramVec.push_back(cecp_len*cm);
  paramVec.push_back(0.*deg);
  paramVec.push_back(360.*deg);

  sprintf(nam,"CSEC");
  GsimTube* cecp = new GsimTube(std::string(nam),this,
				G4ThreeVector(0*cm,0*cm,cecp_pos*cm),
				G4ThreeVector(0,0,0));
  cecp->setOuterMaterial("G4_Fe");
  cecp->setParameters(paramVec);
  cecp->setOuterColor("gray70");
  addDaughter(cecp);
  m_detMoveList.push_back(cecp);
  ecapList.push_back(cecp);

  // Flange at encap vacuum port
  double  cef1_rmin = 33.96/2.0;
  double  cef1_rmax = 57.5/2.0;
  double  cef1_len  = 6;
  double  cef1_pos = -Csir_Len/2.+crystalZOffset+242.5-cef1_len/2.;
  paramVec.clear();
  paramVec.push_back(cef1_rmin*cm);
  paramVec.push_back(cef1_rmax*cm);
  paramVec.push_back(cef1_len*cm);
  paramVec.push_back(0.*deg);
  paramVec.push_back(360.*deg);

  sprintf(nam,"CSF1");
  GsimTube* cef1 = new GsimTube(std::string(nam),this,
				G4ThreeVector(0*cm,0*cm,cef1_pos*cm),
				G4ThreeVector(0,0,0));
  cef1->setOuterMaterial("G4_Fe");
  cef1->setParameters(paramVec);
  cef1->setOuterColor("gray70");
  addDaughter(cef1);
  m_detMoveList.push_back(cef1);
  ecapList.push_back(cef1);

  

  // endcap
  double cec_thick = 1.2;
  double cecR0 = 189.4;
  double cecR1 = cecR0 + cec_thick;
  double cec_centerZ=33.34;//from CsI Surface
  double cec_endZ=223.0;//from CsI surface  2230
  
  
  double ce_rmax[1000];
  double ce_rmin[1000];
  double ce_pos[1000];

  double ldiv = 0.12; // 1.2mm division
  int nlayer = 0;
  do {
    double z=(cec_endZ-ldiv*nlayer);
    double thetaMax=std::acos( (z-cec_centerZ)/cecR1 );
    ce_rmax[nlayer]=cecR1*std::sin(thetaMax);
    if(z-cec_centerZ<cecR0) {
      double thetaMin=std::acos( (z-cec_centerZ)/cecR0 );
      ce_rmin[nlayer]=cecR0*std::sin(thetaMin);
      if(ce_rmin[nlayer]<cecp_rmax) ce_rmin[nlayer]=cecp_rmax;
    }  else {
      ce_rmin[nlayer]=cecp_rmax;
    }
    ce_pos[nlayer]= -Csir_Len/2.+crystalZOffset+z;
      
    paramVec.clear();
    paramVec.push_back(ce_rmin[nlayer]*cm);
    paramVec.push_back(ce_rmax[nlayer]*cm);
    paramVec.push_back(ldiv*cm);
    paramVec.push_back(0.*deg);
    paramVec.push_back(360.*deg);
    sprintf(nam,"ECC%03d",nlayer);
    GsimTube* ecc = new GsimTube(std::string(nam),this,
				 G4ThreeVector(0*cm,0*cm,ce_pos[nlayer]*cm),
				 G4ThreeVector(0,0,0));
    ecc->setOuterMaterial("G4_Fe");
    ecc->setParameters(paramVec);
    ecc->setOuterColor("gray70");
    addDaughter(ecc);
    m_detMoveList.push_back(ecc);
    ecapList.push_back(ecc);
    nlayer++;
  }while(ce_rmax[nlayer-1]<csir_rmax);
  
  // endcap extention with cylinder
  double  cece_thick  = 1.2;
  double  cece_rmax = csir_rmax;
  double  cece_rmin = cece_rmax-cece_thick;
  double  cece_len  =
    cec_endZ+ldiv/2. //Curve end
    -nlayer*ldiv     // curve region     
    -(cscl_len+ cscld_len);    // Cylinder length
  double  cece_pos = -Csir_Len/2+crystalZOffset+(cscl_len+ cscld_len)+cece_len/2.;
  paramVec.clear();
  paramVec.push_back(cece_rmin*cm);
  paramVec.push_back(cece_rmax*cm);
  paramVec.push_back(cece_len*cm);
  paramVec.push_back(0.*deg);
  paramVec.push_back(360.*deg);

  sprintf(nam,"CECE");
  GsimTube* cece = new GsimTube(std::string(nam),this,
				G4ThreeVector(0*cm,0*cm,cece_pos*cm),
				G4ThreeVector(0,0,0));
  cece->setOuterMaterial("G4_Fe");
  cece->setParameters(paramVec);
  cece->setOuterColor("gray70");
  addDaughter(cece);
  m_detMoveList.push_back(cece);
  ecapList.push_back(cece);


  // CsI Dummy Inner Compartment 1,2
  
  double csh1_len = KtevCsicFine_Len;
  double c_csh1_z = -Csir_Len/2 + 5 + csh1_len/2;

  sprintf(nam,"CSH1");
  GsimDetector* csh1
    = new GsimDetector(std::string(nam),this,
		       G4ThreeVector(0,0,c_csh1_z*cm),
		       G4ThreeVector(0,0,0));
  addDaughter(csh1);

  std::vector<double> paramKTEVFine;
  paramKTEVFine.push_back(KtevCsicFine_XY*cm);
  paramKTEVFine.push_back(KtevCsicFine_XY*cm);
  paramKTEVFine.push_back(KtevCsicFine_Len*cm);

  //double tol=0.1e-3;//1um
  double tol=0.01e-3;//cm = 0.1um
  std::vector<double> paramWrapFine;
  paramWrapFine.push_back((KtevCsicFine_XY+KtevCsicFine_Gap-tol)*cm);
  paramWrapFine.push_back((KtevCsicFine_XY+KtevCsicFine_Gap-tol)*cm);
  paramWrapFine.push_back(KtevCsicFine_Len*cm);
  paramWrapFine.push_back((KtevCsicFine_XY+tol)*cm);
  paramWrapFine.push_back((KtevCsicFine_XY+tol)*cm);
  paramWrapFine.push_back(0);
  paramWrapFine.push_back(0);
  paramWrapFine.push_back(0);

  std::vector<double> paramAcrylicRoofFine;
  paramAcrylicRoofFine.push_back(FineCsiAcrylicCapWidth*cm);
  paramAcrylicRoofFine.push_back(FineCsiAcrylicCapWidth*cm);
  paramAcrylicRoofFine.push_back(FineCsiAcrylicCapThick*cm);

  std::vector<double> paramAcrylicSideFine;
  paramAcrylicSideFine.push_back((FineCsiAcrylicCapWidth - FineCsiAcrylicCapThick)*cm);
  paramAcrylicSideFine.push_back(FineCsiAcrylicCapThick*cm);
  paramAcrylicSideFine.push_back((FineCsiAcrylicCapLen - FineCsiAcrylicCapThick)*cm);

  std::vector<double> paramAcrylicRoof;
  paramAcrylicRoof.push_back(CsiAcrylicCapWidth*cm);
  paramAcrylicRoof.push_back(CsiAcrylicCapWidth*cm);
  paramAcrylicRoof.push_back(CsiAcrylicCapThick*cm);

  std::vector<double> paramAcrylicSide;
  paramAcrylicSide.push_back((CsiAcrylicCapWidth - CsiAcrylicCapThick)*cm);
  paramAcrylicSide.push_back(CsiAcrylicCapThick*cm);
  paramAcrylicSide.push_back((CsiAcrylicCapLen - CsiAcrylicCapThick)*cm);

  std::vector<double> paramMPPC;
  paramMPPC.push_back(2 * CsiMPPCWidth*cm);
  paramMPPC.push_back(2 * CsiMPPCWidth*cm);
  paramMPPC.push_back(CsiMPPCThick*cm);

  std::vector<double> paramMPPCFine;
  paramMPPCFine.push_back(CsiMPPCWidth*cm);
  paramMPPCFine.push_back(CsiMPPCWidth*cm);
  paramMPPCFine.push_back(CsiMPPCThick*cm);

  std::vector<double> paramMPPCBoardFine;
  paramMPPCBoardFine.push_back(13*mm);
  paramMPPCBoardFine.push_back(13*mm);
  paramMPPCBoardFine.push_back(1.6*mm);
	
  std::vector<double> paramMPPCBoardHieFine;
  paramMPPCBoardHieFine.push_back(18*mm);
  paramMPPCBoardHieFine.push_back(18*mm);
  paramMPPCBoardHieFine.push_back(1.6*mm);
	

  std::vector<double> paramMPPCBoard;
  paramMPPCBoard.push_back(23*mm);
  paramMPPCBoard.push_back(23*mm);
  paramMPPCBoard.push_back(1.6*mm);

  
  double const regionOuterRadius = 965;
  double minRUFL = 100*sqrt(2);

  const double totlengthUFL = 518500; // total for 1020 cables (mm)
  const double radiusUFL = 1.3/2;
  double vol_UFL = totlengthUFL*radiusUFL*radiusUFL*M_PI;
  double effthickUFL = vol_UFL/(regionOuterRadius*regionOuterRadius - minRUFL*minRUFL)/M_PI;

  std::vector<double> paramVecUFL;
  paramVecUFL.push_back(minRUFL*mm);
  paramVecUFL.push_back(regionOuterRadius*mm);
  paramVecUFL.push_back(effthickUFL*mm);
  paramVecUFL.push_back(0);
  paramVecUFL.push_back(360*deg);

  GsimBox* boxKTEVFine=0;
  GsimBoxWithoutABox* wrapFine=0;
  int nBlockFine=48;
  int nBeamHole=8+2;
  if(m_model!=0) {
    nBeamHole=8;
  }

  GsimBox* FineboxAcrylicCoverRoof = 0;
  GsimBox* FineboxAcrylicCoverSide = 0;
  GsimBox* boxAcrylicCoverRoof = 0;
  GsimBox* boxAcrylicCoverSide = 0;
  GsimBox* boxMPPCFine = 0;
  GsimBox* boxMPPC = 0;
  GsimBox* boxMPPCBoardFine = 0;
  GsimBox* boxMPPCBoardHieFine = 0;
  GsimBox* boxMPPCBoard = 0;

 double crystalZ[2716] = {
    -4.5,
    -5.5,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -4,
    -4,
    -5.5,
    -4,
    -4.5,
    -5,
    -4,
    -5,
    -4,
    -5,
    -5,
    -5,
    -4.5,
    -6,
    -5,
    -5,
    -5,
    -5.5,
    -5.5,
    -4,
    -5.5,
    -5,
    -4,
    -4.5,
    -4,
    -5,
    -5,
    -5,
    -5,
    -5.5,
    -5,
    -4,
    -4.5,
    -5,
    -5.5,
    -5,
    -4.5,
    -5.5,
    -4.5,
    -4.5,
    -5,
    -4,
    -5,
    -6,
    -5,
    -5.5,
    -5.5,
    -4.5,
    -5,
    -5.5,
    -5,
    -5,
    -5.5,
    -5,
    -5,
    -4.5,
    -4.5,
    -5.5,
    -4.5,
    -5,
    -5,
    -3.5,
    -6,
    -5.5,
    -5.5,
    -5,
    -5,
    -4.5,
    -6,
    -4,
    -5,
    -5,
    -5,
    -5.5,
    -6,
    -4,
    -5,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -6,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -5,
    -5,
    -5,
    -4.5,
    -4.5,
    -4.5,
    -5,
    -6,
    -4,
    -5,
    -6,
    -5,
    -4,
    -5,
    -5,
    -4.5,
    -5.5,
    -5,
    -5,
    -5,
    -4,
    -5,
    -5,
    -5,
    -4.5,
    -5,
    -5,
    -5.5,
    -4.5,
    -4.5,
    -6,
    -6.5,
    -4.5,
    -4.5,
    -4.5,
    -5,
    -5,
    -4.5,
    -5,
    -5.5,
    -5,
    -4.5,
    -5.5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -6,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -3,
    -5,
    -6.5,
    -6,
    -5,
    -7,
    -5,
    -5.5,
    -5.5,
    -5.5,
    -5,
    -5.5,
    -7,
    -5,
    -5,
    -5.5,
    -5,
    -6,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -4.5,
    -5,
    -5,
    -5,
    -5.5,
    -5,
    -4.5,
    -5,
    -5.5,
    -5,
    -6,
    -5.5,
    -4.5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5.5,
    -5.5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -4.5,
    -5,
    -5,
    -5,
    -5.5,
    -4.5,
    -6,
    -6,
    -5,
    -6.5,
    -5,
    -5.5,
    -5.5,
    -6.5,
    -5,
    -6,
    -6,
    -5.5,
    -5,
    -5.5,
    -6,
    -5,
    -5,
    -5.5,
    -4.5,
    -5,
    -5.5,
    -4.5,
    -5,
    -5,
    -4.5,
    -4,
    -5.5,
    -5,
    -5,
    -5,
    -4.5,
    -5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -6.5,
    -5,
    -6,
    -6.5,
    -6,
    -6,
    -4.5,
    -5.5,
    -6,
    -6,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -6,
    -5.5,
    -5,
    -4.5,
    -6.5,
    -5,
    -4.5,
    -5.5,
    -6.5,
    -5,
    -6,
    -5.5,
    -6.5,
    -5,
    -5,
    -5.5,
    -5.5,
    -5,
    -6.5,
    -5,
    -4.5,
    -4.5,
    -5.5,
    -5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -5,
    -5.5,
    -5,
    -5,
    -5.5,
    -5.5,
    -7,
    -6.5,
    -5,
    -5,
    -6.5,
    -5.5,
    -6.5,
    -5,
    -5,
    -6,
    -5.5,
    -6,
    -6,
    -5,
    -5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -5,
    -5,
    -6.5,
    -4.5,
    -5.5,
    -5.5,
    -5,
    -5,
    -5,
    -6,
    -5.5,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -5,
    -6.5,
    -5,
    -6,
    -5.5,
    -5.5,
    -5,
    -5.5,
    -5,
    -5.5,
    -5,
    -5.5,
    -6.5,
    -6,
    -5.5,
    -5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -5,
    -5.5,
    -5.5,
    -5,
    -5,
    -5,
    -5.5,
    -5.5,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -4.5,
    -6,
    -5.5,
    -6,
    -5.5,
    -5,
    -6.5,
    -5.5,
    -6,
    -6.5,
    -6,
    -6.5,
    -5.5,
    -7,
    -5,
    -5.5,
    -7,
    -5.5,
    -6,
    -5.5,
    -6,
    -6,
    -5,
    -6.5,
    -5.5,
    -4.5,
    -6,
    -6,
    -5.5,
    -6.5,
    -4.5,
    -6,
    -5,
    -6.5,
    -6.5,
    -5.5,
    -5.5,
    -6.5,
    -5.5,
    -6,
    -5,
    -5,
    -5.5,
    -4.5,
    -5.5,
    -6.5,
    -5,
    -5,
    -5.5,
    -6,
    -7,
    -7.5,
    -6,
    -6.5,
    -6,
    -5.5,
    -6,
    -6,
    -6,
    -7,
    -6.5,
    -5,
    -6,
    -6.5,
    -5,
    -6,
    -5,
    -6,
    -6.5,
    -7,
    -5.5,
    -6.5,
    -6,
    -6,
    -7,
    -6,
    -4,
    -6.5,
    -6,
    -7,
    -5.5,
    -9,
    -5.5,
    -6,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -5,
    -5.5,
    -6,
    -6.5,
    -6.5,
    -5.5,
    -5.5,
    -5.5,
    -5,
    -6.5,
    -6,
    -5,
    -6,
    -5.5,
    -6,
    -6,
    -6,
    -6.5,
    -6,
    -6,
    -6.5,
    -7,
    -6.5,
    -5.5,
    -5.5,
    -6.5,
    -6.5,
    -6.5,
    -5.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -7,
    -7,
    -7,
    -7,
    -5,
    -6,
    -6,
    -7,
    -7,
    -6,
    -6.5,
    -6,
    -6,
    -5.5,
    -7.5,
    -7,
    -6,
    -6.5,
    -6,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -6,
    -6,
    -7,
    -7,
    -5.5,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -7,
    -5.5,
    -6,
    -7,
    -6.5,
    -5,
    -6,
    -6.5,
    -6,
    -6.5,
    -6,
    -6.5,
    -7,
    -7,
    -6.5,
    -5.5,
    -6.5,
    -7,
    -6,
    -5.5,
    -6,
    -6.5,
    -7,
    -6,
    -6,
    -6.5,
    -6,
    -5.5,
    -6.5,
    -6.5,
    -6,
    -6,
    -7,
    -5,
    -6.5,
    -6,
    -6.5,
    -6,
    -6.5,
    -7,
    -5.5,
    -6.5,
    -6.5,
    -5.5,
    -6.5,
    -6,
    -6.5,
    -6,
    -7,
    -7,
    -7,
    -6.5,
    -8,
    -7,
    -7.5,
    -7,
    -7.5,
    -6,
    -7,
    -7,
    -6,
    -6,
    -7,
    -7,
    -5.5,
    -6,
    -5.5,
    -6,
    -6,
    -7,
    -6.5,
    -5,
    -6,
    -7,
    -6,
    -7,
    -7,
    -7,
    -7,
    -5.5,
    -6,
    -6.5,
    -8,
    -6.5,
    -6,
    -6,
    -6,
    -5.5,
    -6,
    -7,
    -6,
    -6,
    -6,
    -6,
    -8,
    -7,
    -6,
    -7,
    -7.5,
    -6.5,
    -6.5,
    -6,
    -7.5,
    -7,
    -8,
    -7.5,
    -7,
    -7,
    -6.5,
    -6,
    -5.5,
    -6,
    -7.5,
    -7,
    -4.5,
    -6,
    -7.5,
    -7,
    -6.5,
    -5,
    -6,
    -7,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -7,
    -6.5,
    -6,
    -7,
    -7,
    -6.5,
    -7,
    -6,
    -6,
    -6,
    -5,
    -7,
    -6,
    -6.5,
    -7,
    -6.5,
    -7,
    -7,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -7.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -6,
    -6,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -6.5,
    -6,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -7.5,
    -6.5,
    -6,
    -6.5,
    -6.5,
    -6,
    -7,
    -7,
    -5.5,
    -6.5,
    -6.5,
    -6,
    -6,
    -7,
    -7.5,
    -8,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -6.5,
    -6.5,
    -7,
    -7,
    -7.5,
    -7,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -7,
    -5.5,
    -6,
    -7,
    -6.5,
    -6.5,
    -7,
    -6.5,
    -6.5,
    -7.5,
    -7,
    -6.5,
    -7,
    -8,
    -7,
    -7.5,
    -6.5,
    -6.5,
    -7,
    -6.5,
    -7,
    -6.5,
    -7.5,
    -5.5,
    -6.5,
    -5.5,
    -6,
    -6.5,
    -5.5,
    -7.5,
    -8.5,
    -7.5,
    -7.5,
    -8.5,
    -6.5,
    -7,
    -7,
    -6.5,
    -6.5,
    -7,
    -6.5,
    -7,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -8.5,
    -7,
    -8,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -8,
    -7,
    -6.5,
    -8.5,
    -6.5,
    -8,
    -8.5,
    -7,
    -7,
    -6.5,
    -6.5,
    -5.5,
    -6,
    -6.5,
    -7,
    -8.5,
    -7.5,
    -7,
    -8,
    -7,
    -7,
    -7,
    -6.5,
    -7.5,
    -7.5,
    -6.5,
    -6.5,
    -7.5,
    -7,
    -7.5,
    -6.5,
    -7,
    -7,
    -7,
    -7,
    -6.5,
    -7,
    -7.5,
    -7.5,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7.5,
    -7.5,
    -7,
    -6,
    -7,
    -7,
    -7,
    -7,
    -7,
    -6.5,
    -6,
    -6,
    -6.5,
    -8,
    -7,
    -6.5,
    -6.5,
    -7,
    -7,
    -7.5,
    -7.5,
    -7,
    -7.5,
    -7.5,
    -7,
    -7,
    -8,
    -7,
    -8,
    -7,
    -7,
    -7.5,
    -5.5,
    -7,
    -6.5,
    -6.5,
    -7,
    -7.5,
    -7,
    -6.5,
    -7,
    -6.5,
    -7,
    -7,
    -7,
    -8,
    -6,
    -7,
    -7.5,
    -7,
    -7,
    -7,
    -7.5,
    -7.5,
    -6.5,
    -7,
    -7.5,
    -6,
    -7.5,
    -7,
    -7,
    -7.5,
    -7,
    -6.5,
    -8.5,
    -6.5,
    -8,
    -7.5,
    -8,
    -7.5,
    -7,
    -7.5,
    -7,
    -7.5,
    -8,
    -8,
    -7.5,
    -7.5,
    -7.5,
    -7,
    -7.5,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7,
    -6,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7,
    -7,
    -8.5,
    -7.5,
    -6,
    -6,
    -6.5,
    -7.5,
    -6,
    -8,
    -8.5,
    -7,
    -7.5,
    -7,
    -7.5,
    -8,
    -7.5,
    -7,
    -8,
    -7.5,
    -7.5,
    -7.5,
    -7,
    -7.5,
    -7.5,
    -8,
    -7.5,
    -8,
    -7,
    -7,
    -8.5,
    -7.5,
    -7,
    -7,
    -7,
    -7.5,
    -7,
    -8,
    -8,
    -7,
    -7.5,
    -8.5,
    -7,
    -7.5,
    -7,
    -7,
    -7.5,
    -7,
    -8,
    -7,
    -7.5,
    -7,
    -7,
    -7,
    -7.5,
    -7,
    -8,
    -8,
    -7.5,
    -9,
    -7.5,
    -8.5,
    -7.5,
    -8,
    -7.5,
    -6.5,
    -8,
    -7.5,
    -8,
    -8,
    -8,
    -9.5,
    -7.5,
    -7.5,
    -8,
    -9,
    -9,
    -8,
    -9,
    -7,
    -8,
    -8,
    -8,
    -7.5,
    -7,
    -6,
    -7,
    -7,
    -7.5,
    -6,
    -7,
    -7,
    -7.5,
    -6.5,
    -8.5,
    -9,
    -7.5,
    -8,
    -8,
    -8,
    -8.5,
    -9,
    -8,
    -9,
    -9,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -8.5,
    -7.5,
    -7.5,
    -7.5,
    -8,
    -7.5,
    -7.5,
    -8,
    -8,
    -8.5,
    -6.5,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -9,
    -7.5,
    -7.5,
    -7.5,
    -8,
    -7,
    -7,
    -7.5,
    -8,
    -8,
    -7.5,
    -7.5,
    -6.5,
    -7,
    -8.5,
    -9.5,
    -8,
    -8,
    -8,
    -7.5,
    -10,
    -7,
    -9,
    -8,
    -7.5,
    -7,
    -7.5,
    -7,
    -7.5,
    -8,
    -8,
    -7.5,
    -8,
    -8,
    -7.5,
    -7.5,
    -8,
    -8,
    -8,
    -8,
    -8,
    -9,
    -8,
    -8,
    -8,
    -7,
    -7,
    -8,
    -7.5,
    -8,
    -8,
    -7,
    -7,
    -7,
    -8,
    -8,
    -8,
    -7,
    -7,
    -7,
    -8,
    -7.5,
    -8,
    -8,
    -8,
    -9.5,
    -8,
    -7.5,
    -8,
    -8.5,
    -8,
    -8,
    -8,
    -6.5,
    -8.5,
    -8,
    -8,
    -8.5,
    -8,
    -7.5,
    -8,
    -8,
    -7.5,
    -8,
    -7.5,
    -8,
    -6.5,
    -7.5,
    -8.5,
    -8,
    -8,
    -8,
    -7.5,
    -7.5,
    -7.5,
    -7.5,
    -8.5,
    -7.5,
    -8.5,
    -7.5,
    -7,
    -8.5,
    -8,
    -7,
    -8,
    -8,
    -8,
    -8,
    -6.5,
    -8,
    -8,
    -8,
    -7.5,
    -8,
    -8,
    -7.5,
    -8,
    -7,
    -8,
    -8,
    -8,
    -8,
    -8,
    -6.5,
    -7,
    -7,
    -8.5,
    -8,
    -7,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -7,
    -8,
    -8,
    -8.5,
    -7,
    -8,
    -8,
    -8,
    -7.5,
    -8.5,
    -8,
    -7.5,
    -9,
    -8,
    -8,
    -7,
    -8,
    -8,
    -8,
    -8,
    -8,
    -9,
    -8,
    -8,
    -9,
    -7,
    -8,
    -8,
    -8,
    -7,
    -8,
    -7.5,
    -8.5,
    -7.5,
    -7.5,
    -8.5,
    -8,
    -7.5,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8.5,
    -8,
    -9,
    -8.5,
    -9,
    -8,
    -9,
    -8,
    -9,
    -8,
    -8.5,
    -6.5,
    -7.5,
    -8,
    -7.5,
    -8,
    -7.5,
    -7,
    -8.5,
    -8,
    -9,
    -8,
    -7,
    -8.5,
    -8,
    -8,
    -7.5,
    -8,
    -9.5,
    -6.5,
    -6.5,
    -8,
    -8,
    -8.5,
    -7,
    -9,
    -7.5,
    -8,
    -8,
    -8.5,
    -8,
    -8,
    -9.5,
    -9,
    -10,
    -10,
    -8,
    -8,
    -8,
    -9,
    -8.5,
    -8.5,
    -8,
    -8,
    -8,
    -7,
    -8,
    -8,
    -9,
    -9,
    -8,
    -8,
    -8,
    -9,
    -8,
    -7.5,
    -8,
    -8,
    -8.5,
    -8,
    -9,
    -7.5,
    -8,
    -7.5,
    -7,
    -8.5,
    -9.5,
    -9.5,
    -8,
    -7.5,
    -9.5,
    -8,
    -8.5,
    -8.5,
    -9,
    -9.5,
    -9,
    -9.5,
    -8.5,
    -8.5,
    -8.5,
    -7.5,
    -9,
    -8.5,
    -9,
    -8.5,
    -8.5,
    -9,
    -9,
    -9,
    -9,
    -8,
    -9,
    -9,
    -8.5,
    -9.5,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8,
    -9.5,
    -9,
    -6.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -8.5,
    -7.5,
    -7.5,
    -8,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -7,
    -8,
    -8,
    -8.5,
    -9.5,
    -8.5,
    -9.5,
    -9,
    -8.5,
    -7.5,
    -9,
    -9,
    -9,
    -9,
    -10,
    -9,
    -8.5,
    -9.5,
    -8.5,
    -8.5,
    -8,
    -9.5,
    -8,
    -10,
    -8,
    -8.5,
    -9,
    -6.5,
    -9,
    -7,
    -8,
    -9,
    -7,
    -8,
    -8,
    -8.5,
    -7.5,
    -8,
    -9,
    -9.5,
    -7,
    -6.5,
    -8,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -7.5,
    -7,
    -8,
    -8,
    -8.5,
    -8.5,
    -7,
    -8.5,
    -8.5,
    -9,
    -8.5,
    -9.5,
    -9,
    -9,
    -9.5,
    -10.5,
    -10,
    -8.5,
    -8.5,
    -10,
    -10.5,
    -10,
    -9,
    -9.5,
    -9,
    -8,
    -9,
    -9,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -10,
    -8,
    -8,
    -8.5,
    -7.5,
    -8,
    -8,
    -8,
    -9,
    -7.5,
    -8,
    -7,
    -7.5,
    -6.5,
    -8,
    -7.5,
    -8.5,
    -9,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8,
    -9.5,
    -8,
    -8,
    -9,
    -10,
    -8,
    -8,
    -8.5,
    -9,
    -8.5,
    -8.5,
    -7.5,
    -9,
    -8,
    -9,
    -8,
    -7.5,
    -8,
    -8.5,
    -8,
    -7.5,
    -7,
    -7,
    -7,
    -9,
    -8.5,
    -8,
    -8,
    -8.5,
    -7,
    -8,
    -8.5,
    -7,
    -8,
    -9,
    -8.5,
    -9,
    -8.5,
    -9.5,
    -8,
    -8,
    -10,
    -9,
    -9.5,
    -9,
    -10,
    -10,
    -8.5,
    -7.5,
    -9.5,
    -9.5,
    -8.5,
    -9,
    -10.5,
    -9.5,
    -9,
    -7.5,
    -9,
    -8,
    -8,
    -7,
    -9,
    -8.5,
    -9.5,
    -8.5,
    -9,
    -7,
    -7.5,
    -8,
    -8,
    -10,
    -9,
    -7.5,
    -8,
    -9,
    -9.5,
    -9,
    -8.5,
    -9,
    -9,
    -8,
    -8,
    -9,
    -7.5,
    -9,
    -8.5,
    -9,
    -8,
    -10,
    -9,
    -8,
    -9.5,
    -9,
    -8,
    -9,
    -8,
    -9.5,
    -9,
    -8,
    -7,
    -8,
    -6.5,
    -8.5,
    -9,
    -8,
    -9.5,
    -9,
    -8,
    -8,
    -9,
    -9,
    -7.5,
    -8,
    -9,
    -8,
    -8.5,
    -9,
    -8,
    -9,
    -8,
    -8.5,
    -8,
    -9,
    -9,
    -9.5,
    -7.5,
    -9,
    -7.5,
    -8.5,
    -9,
    -8.5,
    -8.5,
    -8,
    -8.5,
    -9,
    -8.5,
    -9.5,
    -9,
    -9,
    -10,
    -8,
    -9,
    -8.5,
    -9.5,
    -9,
    -9,
    -8,
    -8,
    -8.5,
    -8,
    -10,
    -10,
    -9.5,
    -9,
    -9,
    -8,
    -8,
    -8,
    -9,
    -8,
    -8,
    -8,
    -8,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -9,
    -8.5,
    -9,
    -8,
    -9,
    -9,
    -8,
    -8,
    -8.5,
    -9,
    -9,
    -9,
    -10,
    -9,
    -9.5,
    -9,
    -8.5,
    -9.5,
    -9,
    -8.5,
    -9,
    -9.5,
    -9,
    -8,
    -10,
    -8.5,
    -9,
    -9,
    -8,
    -9,
    -8.5,
    -9,
    -9.5,
    -9,
    -8.5,
    -6.5,
    -8,
    -7,
    -9.5,
    -8.5,
    -9,
    -9,
    -9,
    -9,
    -9,
    -9,
    -8,
    -8,
    -7.5,
    -7,
    -8.5,
    -8,
    -9,
    -7.5,
    -8,
    -9,
    -9,
    -9.5,
    -9.5,
    -8.5,
    -9.5,
    -9,
    -8.5,
    -10,
    -9,
    -8.5,
    -9.5,
    -10,
    -9.5,
    -10,
    -10,
    -8,
    -8.5,
    -8.5,
    -9,
    -10,
    -9,
    -10,
    -10,
    -10,
    -11,
    -9.5,
    -8,
    -9.5,
    -9,
    -10,
    -8.5,
    -9.5,
    -9.5,
    -10,
    -9,
    -8.5,
    -8,
    -9,
    -8.5,
    -8.5,
    -8,
    -8.5,
    -7.5,
    -8.5,
    -8.5,
    -8,
    -8.5,
    -9,
    -9,
    -8,
    -9.5,
    -8,
    -9,
    -9.5,
    -9.5,
    -7,
    -8.5,
    -9,
    -8.5,
    -9.5,
    -8.5,
    -9,
    -8.5,
    -8.5,
    -9.5,
    -8,
    -8.5,
    -8,
    -10,
    -10,
    -9,
    -8.5,
    -9,
    -10,
    -9,
    -9,
    -9,
    -8.5,
    -8.5,
    -8.5,
    -7.5,
    -8,
    -8.5,
    -9,
    -7.5,
    -8,
    -9,
    -6,
    -8,
    -8,
    -7.5,
    -7.5,
    -9,
    -7,
    -8,
    -8,
    -8.5,
    -8,
    -9,
    -9,
    -8.5,
    -9,
    -8.5,
    -9,
    -9,
    -9,
    -9,
    -8.5,
    -9,
    -9,
    -8,
    -9.5,
    -9,
    -8,
    -10,
    -8.5,
    -9,
    -9,
    -8,
    -10,
    -8,
    -9,
    -9,
    -7.5,
    -9,
    -9,
    -8.5,
    -8.5,
    -8,
    -7.5,
    -7.5,
    -8,
    -8,
    -7.5,
    -9,
    -8,
    -8.5,
    -8,
    -7,
    -9,
    -7.5,
    -8,
    -7.5,
    -8,
    -8.5,
    -7.5,
    -8.5,
    -9,
    -8.5,
    -9,
    -8.5,
    -8.5,
    -8.5,
    -9.5,
    -9,
    -8,
    -8.5,
    -8,
    -9.5,
    -8,
    -8.5,
    -9,
    -8,
    -9.5,
    -8,
    -8.5,
    -8.5,
    -9,
    -9,
    -8,
    -8,
    -10,
    -8,
    -8,
    -8,
    -9,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -8,
    -8.5,
    -8,
    -7.5,
    -7,
    -9,
    -8.5,
    -7.5,
    -7,
    -8,
    -7.5,
    -7.5,
    -7.5,
    -8,
    -7,
    -8,
    -9,
    -8,
    -9,
    -10,
    -8,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8,
    -9.5,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -9.5,
    -8.5,
    -8.5,
    -8,
    -9.5,
    -9.5,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8,
    -8.5,
    -9.5,
    -8,
    -8,
    -8.5,
    -7.5,
    -8,
    -9,
    -8.5,
    -8.5,
    -9.5,
    -7.5,
    -7,
    -7.5,
    -7.5,
    -8,
    -9.5,
    -9,
    -8.5,
    -10,
    -9,
    -10,
    -9.5,
    -9,
    -9,
    -9,
    -10,
    -9,
    -9,
    -7.5,
    -9,
    -8.5,
    -9,
    -9,
    -9,
    -10.5,
    -8.5,
    -9,
    -10,
    -9.5,
    -8.5,
    -7.5,
    -8.5,
    -9,
    -9.5,
    -9,
    -9,
    -9.5,
    -9,
    -10,
    -10,
    -9,
    -8,
    -9,
    -8,
    -9,
    -8,
    -8.5,
    -8.5,
    -8,
    -7.5,
    -8.5,
    -8.5,
    -8,
    -7,
    -9,
    -9,
    -8.5,
    -9,
    -10,
    -9,
    -8,
    -9.5,
    -8,
    -10,
    -10,
    -8,
    -8.5,
    -8,
    -10,
    -9.5,
    -9,
    -9,
    -9.5,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -7.5,
    -9,
    -9.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -9,
    -8.5,
    -9,
    -8.5,
    -9,
    -8.5,
    -9,
    -8.5,
    -6.5,
    -7.5,
    -8.5,
    -8,
    -7.5,
    -7.5,
    -7,
    -8,
    -8.5,
    -8.5,
    -11,
    -9,
    -10,
    -9,
    -9.5,
    -8.5,
    -9,
    -9,
    -8.5,
    -10,
    -9,
    -7.5,
    -9,
    -9,
    -8.5,
    -7.5,
    -7,
    -9,
    -9,
    -10,
    -9.5,
    -9,
    -9,
    -9.5,
    -9.5,
    -8,
    -7,
    -7,
    -9.5,
    -8,
    -9,
    -7.5,
    -7,
    -10,
    -9,
    -8.5,
    -7,
    -7,
    -9,
    -9,
    -9,
    -9,
    -8,
    -8,
    -8.5,
    -9,
    -9,
    -9,
    -10,
    -9.5,
    -9.5,
    -9.5,
    -9,
    -10,
    -8.5,
    -10,
    -10.5,
    -7.5,
    -9.5,
    -9,
    -10,
    -9.5,
    -10,
    -8.5,
    -9,
    -9.5,
    -10,
    -8.5,
    -9.5,
    -7.5,
    -9.5,
    -10,
    -10,
    -8.5,
    -10,
    -10,
    -8,
    -7,
    -7,
    -9,
    -8.5,
    -9.5,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -6.5,
    -8,
    -5,
    -8.5,
    -9,
    -9,
    -8.5,
    -9,
    -10,
    -9.5,
    -9.5,
    -8.5,
    -10.5,
    -8.5,
    -10,
    -10,
    -9.5,
    -9,
    -10,
    -9,
    -10.5,
    -8,
    -8.5,
    -9,
    -9.5,
    -8.5,
    -8.5,
    -8,
    -9.5,
    -8,
    -7.5,
    -7,
    -10,
    -9,
    -9.5,
    -7.5,
    -9.5,
    -9,
    -8,
    -9,
    -9,
    -10,
    -6,
    -8.5,
    -8.5,
    -7.5,
    -8.5,
    -8.5,
    -8.5,
    -7,
    -9.5,
    -9,
    -9,
    -10,
    -9.5,
    -10,
    -9.5,
    -8,
    -7.5,
    -9.5,
    -10,
    -10,
    -9.5,
    -8.5,
    -9.5,
    -9.5,
    -10,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -10,
    -9.5,
    -10,
    -8.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -8.5,
    -6.5,
    -7.5,
    -7.5,
    -8,
    -8.5,
    -9.5,
    -8,
    -8,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8,
    -8.5,
    -8.5,
    -3.1667,
    -3.1667,
    -3.8333,
    -3.6667,
    -4,
    -4,
    -4,
    -4,
    -3.8333,
    -3.5,
    -3.1667,
    -3.1667,
    -4,
    -2.5,
    -4,
    -3.5,
    -4,
    -4,
    -3.5,
    -3,
    -4,
    -3.5,
    -4,
    -4,
    -4,
    -4,
    -3.5,
    -3.5,
    -4,
    -4,
    -4,
    -4,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4,
    -3.5,
    -4,
    -4,
    -3.5,
    -3.5,
    -4,
    -4,
    -4,
    -3.5,
    -3.5,
    -3.5,
    -4,
    -4,
    -4,
    -4,
    -4.5,
    -3.5,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -3.5,
    -4,
    -4.5,
    -3.5,
    -4,
    -4,
    -4,
    -4,
    -4.5,
    -4,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -3.5,
    -4,
    -4.5,
    -4,
    -5,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4,
    -4.5,
    -4,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -5,
    -4.5,
    -5,
    -4,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -5,
    -4.5,
    -5,
    -4.5,
    -4,
    -5,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -4.5,
    -5,
    -5,
    -5,
    -5.5,
    -5.5,
    -5,
    -6,
    -5,
    -5.5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5,
    -5.5,
    -6,
    -5.5,
    -5,
    -5.5,
    -5,
    -5,
    -5,
    -6,
    -5.5,
    -6,
    -5.5,
    -6,
    -6,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -6,
    -6,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -6,
    -5,
    -5,
    -6,
    -5.5,
    -5.5,
    -6,
    -6,
    -6,
    -6,
    -5,
    -6.5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -5.5,
    -6,
    -5.5,
    -5.5,
    -5.5,
    -7,
    -6,
    -5,
    -5.5,
    -6,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -6,
    -6.5,
    -6,
    -6,
    -5.5,
    -5.5,
    -6,
    -6,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -6,
    -6,
    -6,
    -6,
    -6,
    -6,
    -6.5,
    -6.5,
    -6,
    -7,
    -7,
    -6,
    -6.5,
    -6.5,
    -6,
    -6,
    -7,
    -6.5,
    -7,
    -6.5,
    -7,
    -7.5,
    -7,
    -6,
    -6,
    -7,
    -6.5,
    -7.5,
    -6.5,
    -6,
    -7,
    -7.5,
    -7.5,
    -6,
    -7.5,
    -7,
    -7,
    -7,
    -7,
    -7,
    -6.5,
    -7,
    -6.5,
    -6,
    -7.5,
    -7.5,
    -7,
    -7,
    -7.5,
    -8,
    -6.5,
    -7.5,
    -6.5,
    -6.5,
    -7,
    -6.5,
    -7.5,
    -7,
    -7.5,
    -7,
    -7,
    -7,
    -5.5,
    -5.5,
    -5.5,
    -6.5,
    -6,
    -5.5,
    -8,
    -7,
    -7,
    -6.5,
    -6,
    -6,
    -6,
    -6,
    -6,
    -6.5,
    -5.5,
    -5.5,
    -7,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -5.5,
    -7,
    -6.5,
    -7,
    -6.5,
    -7,
    -8,
    -6.5,
    -6,
    -6.5,
    -6.5,
    -7,
    -6,
    -6,
    -7,
    -6,
    -6.5,
    -7,
    -7,
    -6.5,
    -6.5,
    -6.5,
    -6,
    -6.5,
    -7,
    -7,
    -7.5,
    -7.5,
    -6,
    -6.5,
    -6.5,
    -6.5,
    -6.5,
    -7,
    -7,
    -6,
    -6,
    -8,
    -7,
    -7,
    -6.5,
    -7,
    -7.5,
    -7,
    -7,
    -7,
    -6.5,
    -7.5,
    -7,
    -7,
    -7.5,
    -8.5,
    -7,
    -7,
    -7,
    -8,
    -7.5,
    -7.5,
    -7,
    -6.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9,
    -9,
    -8.5,
    -8,
    -8.5,
    -9,
    -8.5,
    -9.5,
    -8,
    -9,
    -7.5,
    -9,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -8.5,
    -7,
    -8,
    -7.5,
    -7.5,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -9,
    -8,
    -9.5,
    -9.5,
    -9.5,
    -8,
    -9,
    -9.5,
    -9,
    -9.5,
    -8.5,
    -9,
    -8.5,
    -8,
    -8.5,
    -8,
    -7,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -10,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -9.5,
    -8.5,
    -9.5,
    -8,
    -9,
    -8.5,
    -8.5,
    -10,
    -10.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -10,
    -9,
    -9.5,
    -10,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -8.5,
    -9.5,
    -8,
    -8,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -9,
    -9.5,
    -10,
    -9,
    -10,
    -9.5,
    -9,
    -9,
    -9.5,
    -9,
    -9,
    -9.5,
    -9.5,
    -10,
    -9.5,
    -9.5,
    -9.5,
    -9.5,
    -8.5,
    -9.5,
    -10
  };

 double ZminAcrylicCover = 1e10;

  int id=0;
  for(int i=0;i<nBlockFine;i++) {
    double YBlock=
      (KtevCsicFine_XY+KtevCsicFine_Gap)*(i-nBlockFine/2.)
      +(KtevCsicFine_XY+KtevCsicFine_Gap)/2.;
    
    for(int j=0;j<nBlockFine;j++) {
      {
	double XBlock=
	  (KtevCsicFine_XY+KtevCsicFine_Gap)*(j-nBlockFine/2.)
	  +(KtevCsicFine_XY+KtevCsicFine_Gap)/2.;
	
	//beam hole
	if( i>=(nBlockFine-nBeamHole)/2. &&
	    i<(nBlockFine-nBeamHole)/2.+nBeamHole &&
	    j>=(nBlockFine-nBeamHole)/2. &&
	    j<(nBlockFine-nBeamHole)/2.+nBeamHole )
	  continue;
	
	// added by Sato 20120514
	XBlock*=-1;

	double ZBlock=(-csh1_len/2.+KtevCsicFine_Len/2.);
	if(userFlag >= 201605) {
	  ZBlock+=crystalZ[id]*0.1;
	}
	if(boxKTEVFine==0) {
	  std::sprintf(nam,"KTEVCSIFine");
	  boxKTEVFine = new GsimBox(std::string(nam),csh1,
				    G4ThreeVector(XBlock*cm,
						  YBlock*cm,
						  ZBlock*cm),
				    G4ThreeVector(0,0,0)
				    );
	  boxKTEVFine->setOuterMaterial("G4_CESIUM_IODIDE");
	  boxKTEVFine->setParameters(paramKTEVFine);
	  boxKTEVFine->setOuterColor("lightskyblue");
	  boxKTEVFine->setSensitiveDetector("CSI",id);
	  csh1->addDaughter(boxKTEVFine);
	} else {
	  boxKTEVFine->cloneDetector(G4ThreeVector(XBlock*cm,
						   YBlock*cm,
						   ZBlock*cm),
				     G4ThreeVector(0,0,0),id);
	}
	
	if (userFlag >= 201605) {
	  if(wrapFine==0) {
	    std::sprintf(nam,"WrapFine");
	    wrapFine = new GsimBoxWithoutABox(std::string(nam),csh1,
					      G4ThreeVector(XBlock*cm,
							    YBlock*cm,
							    ZBlock*cm),
					      G4ThreeVector(0,0,0)
					      );
	    wrapFine->setOuterMaterial("G4_MYLAR");
	    wrapFine->setParameters(paramWrapFine);
	    wrapFine->setOuterColor("gray70");
	    m_detList.push_back(wrapFine);
	    csh1->addDaughter(wrapFine);
	  } else {
	    wrapFine->cloneDetector(G4ThreeVector(XBlock*cm,
						  YBlock*cm,
						  ZBlock*cm),
				    G4ThreeVector(0,0,0));
	  }
	}

	if (userFlag >= 201902) {
	  //Add MPPC, acrylic box by Shimizu
	  double ZacrylicCoverRoof = ZBlock - KtevCsicFine_Len / 2.0 - FineCsiAcrylicCapLen + FineCsiAcrylicCapThick / 2.0;
	  double ZacrylicCoverSide = ZBlock - KtevCsicFine_Len / 2.0 - (FineCsiAcrylicCapLen - FineCsiAcrylicCapThick) / 2.0;
	  double ZMPPC  = ZBlock - KtevCsicFine_Len / 2.0 - CsiMPPCThick / 2.0;
	  double ZBoard = ZMPPC - CsiMPPCThick/2.0 - 0.16/2;
	  
	  if(ZminAcrylicCover>ZacrylicCoverRoof) ZminAcrylicCover = ZacrylicCoverRoof;
	  
	  if (FineboxAcrylicCoverRoof == 0) {
	    std::cout << "MPPC installed\n";
	    FineboxAcrylicCoverRoof = new GsimBox("FineAcrylicCoverRoof", csh1,
						  G4ThreeVector(XBlock*cm, YBlock*cm, ZacrylicCoverRoof*cm),
						  G4ThreeVector(0, 0, 0));
	    FineboxAcrylicCoverRoof->setOuterMaterial("G4_LUCITE");
	    FineboxAcrylicCoverRoof->setParameters(paramAcrylicRoofFine);
	    FineboxAcrylicCoverRoof->setOuterColor("gray75");
	    csh1->addDaughter(FineboxAcrylicCoverRoof);
	    m_detMoveList.push_back(FineboxAcrylicCoverRoof);
	  }else{
	    FineboxAcrylicCoverRoof->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZacrylicCoverRoof*cm),
						   G4ThreeVector(0, 0, 0), id);
	  }

	  if (FineboxAcrylicCoverSide == 0) {
	    
	    FineboxAcrylicCoverSide = new GsimBox("FineAcrylicCoverSide", csh1,
						  G4ThreeVector((XBlock - FineCsiAcrylicCapThick / 2)*cm,
								(YBlock + FineCsiAcrylicCapWidth / 2 - FineCsiAcrylicCapThick / 2)*cm,
								ZacrylicCoverSide*cm),
						  G4ThreeVector(0, 0, 0 * deg));
	    FineboxAcrylicCoverSide->setOuterMaterial("G4_LUCITE");
	    FineboxAcrylicCoverSide->setParameters(paramAcrylicSideFine);
	    FineboxAcrylicCoverSide->setOuterColor("gray75");
	    csh1->addDaughter(FineboxAcrylicCoverSide);
	    m_detMoveList.push_back(FineboxAcrylicCoverSide);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock + FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock - FineCsiAcrylicCapWidth / 2 + FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 0 * deg), 1);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock - FineCsiAcrylicCapWidth / 2 + FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock - FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 90 * deg), 2);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock + FineCsiAcrylicCapWidth / 2 - FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock + FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 90 * deg), 3);

	  } else {
	    
	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock - FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock + FineCsiAcrylicCapWidth / 2 - FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 0 * deg), 4 * id + 0);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock + FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock - FineCsiAcrylicCapWidth / 2 + FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 0 * deg), 4 * id + 1);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock - FineCsiAcrylicCapWidth / 2 + FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock - FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 90 * deg), 4 * id + 2);

	    FineboxAcrylicCoverSide->cloneDetector(G4ThreeVector(
								 (XBlock + FineCsiAcrylicCapWidth / 2 - FineCsiAcrylicCapThick / 2)*cm,
								 (YBlock + FineCsiAcrylicCapThick / 2)*cm,
								 ZacrylicCoverSide*cm),
						   G4ThreeVector(0, 0, 90 * deg), 4 * id + 3);

	  }
	  
	  if (boxMPPCFine == 0) {
	    boxMPPCFine = new GsimBox("FineMPPC", csh1,
				      G4ThreeVector(XBlock*cm, YBlock*cm, ZMPPC*cm),
				      G4ThreeVector(0, 0, 0 * deg));
	    boxMPPCFine->setOuterMaterial("G4_ALUMINUM_OXIDE");
	    boxMPPCFine->setParameters(paramMPPCFine);
	    boxMPPCFine->setOuterColor("black");
	    csh1->addDaughter(boxMPPCFine);
	    m_detMoveList.push_back(boxMPPCFine);
	  } else {
	    boxMPPCFine->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZMPPC*cm), G4ThreeVector(0, 0, 0), id);
	  }


	  if (boxMPPCBoardFine == 0) {
	    boxMPPCBoardFine = new GsimBox("MPPCBoardFine", csh1,
					   G4ThreeVector(XBlock*cm, YBlock*cm, ZBoard*cm),
					   G4ThreeVector(0, 0, 0 * deg));
	    boxMPPCBoardFine->setOuterMaterial("GsimG10");
	    boxMPPCBoardFine->setParameters(paramMPPCBoardFine);
	    boxMPPCBoardFine->setOuterColor("green");
	    csh1->addDaughter(boxMPPCBoardFine);
	    m_detMoveList.push_back(boxMPPCBoardFine);
	  } else {
	    boxMPPCBoardFine->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZBoard*cm), G4ThreeVector(0, 0, 0), id);
	  }

	  if(id%4==1){

	    if (boxMPPCBoardHieFine == 0) {
	      boxMPPCBoardHieFine = new GsimBox("MPPCBoardHieFine", csh1,
						G4ThreeVector(XBlock*cm, YBlock*cm, (ZBoard-0.46)*cm),
						G4ThreeVector(0, 0, 0 * deg));
	      boxMPPCBoardHieFine->setOuterMaterial("GsimG10");
	      boxMPPCBoardHieFine->setParameters(paramMPPCBoardHieFine);
	      boxMPPCBoardHieFine->setOuterColor("green");
	      csh1->addDaughter(boxMPPCBoardHieFine);
	      m_detMoveList.push_back(boxMPPCBoardHieFine);
	    }else{
	      boxMPPCBoardHieFine->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, (ZBoard-0.46)*cm), G4ThreeVector(0, 0, 0), id/4);
	    }
	  }
	  //end of if (userFlag >= 201902) 
	}
	
	id++;
      }
    }
  }

  int id_MPPC = 0;
  double csh2_len = KtevCsic_Len;
  double c_csh2_z = -Csir_Len/2 + 5 + csh2_len/2;
  
  sprintf(nam,"CSH2");
  GsimDetector* csh2
    = new GsimDetector(std::string(nam),this,
		       G4ThreeVector(0,0,c_csh2_z*cm),
		       G4ThreeVector(0,0,0));
  addDaughter(csh2);
  

  std::vector<double> paramKTEV;
  paramKTEV.push_back(KtevCsic_XY*cm);
  paramKTEV.push_back(KtevCsic_XY*cm);
  paramKTEV.push_back(KtevCsic_Len*cm);

  std::vector<double> paramWrap;
  paramWrap.push_back((KtevCsic_XY+KtevCsic_Gap-tol)*cm);
  paramWrap.push_back((KtevCsic_XY+KtevCsic_Gap-tol)*cm);
  paramWrap.push_back(KtevCsic_Len*cm);
  paramWrap.push_back((KtevCsic_XY+tol)*cm);
  paramWrap.push_back((KtevCsic_XY+tol)*cm);
  paramWrap.push_back(0);
  paramWrap.push_back(0);
  paramWrap.push_back(0);

  GsimBox* boxKTEV=0;
  GsimBoxWithoutABox* wrap=0;

  int csiStack[38]
    = { 0,12,16,20,22,24,
        26,28,30,32,32,34,
        34,36,36,36,36,36,36,
        36,36,36,36,36,36,34,
        34,32,32,30,28,26,
        24,22,20,16,12,0 };
  
  int nLayer=38;
  for(int i=0;i<nLayer;i++) {
    double YBlock=
      (KtevCsic_XY+KtevCsic_Gap)*(i-nLayer/2.)
      +(KtevCsic_XY+KtevCsic_Gap)/2.;
    
    int nBlock = csiStack[i];
    
    for(int j=0;j<nBlock;j++) {
      //KtevFine
      if( i>=(nLayer-nBlockFine/2.)/2. &&
	  i<(nLayer-nBlockFine/2.)/2.+nBlockFine/2. &&
	  j>=(nBlock-nBlockFine/2.)/2. &&
	  j<(nBlock-nBlockFine/2.)/2.+nBlockFine/2. )
	continue;
      
      double XBlock=
	(KtevCsic_XY+KtevCsic_Gap)*(j-nBlock/2.)
	+(KtevCsic_XY+KtevCsic_Gap)/2.;
      // added by Sato 20120514
      XBlock*=-1;

      double ZBlock=(-csh1_len/2.+KtevCsicFine_Len/2.);
      if(userFlag >= 201605) {
	ZBlock+=crystalZ[id]*0.1;
      }
      if(boxKTEV==0) {
	std::sprintf(nam,"KTEVCSI");
	boxKTEV = new GsimBox(std::string(nam),csh2,
			      G4ThreeVector(XBlock*cm,
					    YBlock*cm,
					    ZBlock*cm),
			      G4ThreeVector(0,0,0)
			      );
	boxKTEV->setOuterMaterial("G4_CESIUM_IODIDE");
	boxKTEV->setParameters(paramKTEV);
	boxKTEV->setOuterColor("lightskyblue");
	boxKTEV->setSensitiveDetector("CSI",id);
	csh2->addDaughter(boxKTEV);
      } else {
	boxKTEV->cloneDetector(G4ThreeVector(XBlock*cm,
					     YBlock*cm,
					     ZBlock*cm),
			       G4ThreeVector(0,0,0),id);
      }

      if (userFlag >= 201605) {
	if(wrap==0) {
	  std::sprintf(nam,"Wrap");
	  wrap = new GsimBoxWithoutABox(std::string(nam),csh2,
					G4ThreeVector(XBlock*cm,
						      YBlock*cm,
						      ZBlock*cm),
					G4ThreeVector(0,0,0)
					);
	  wrap->setOuterMaterial("G4_MYLAR");
	  wrap->setParameters(paramWrap);
	  wrap->setOuterColor("gray70");
	  m_detList.push_back(wrap);
	  csh2->addDaughter(wrap);
	} else {
	  wrap->cloneDetector(G4ThreeVector(XBlock*cm,
					    YBlock*cm,
					    ZBlock*cm),
			      G4ThreeVector(0,0,0));
	}
      }
      if (userFlag >= 201902) {
	//Add MPPC, acrylic box by Shimizu
	double ZacrylicCoverRoof = ZBlock - KtevCsic_Len / 2.0 - CsiAcrylicCapLen + CsiAcrylicCapThick / 2.0;
	double ZacrylicCoverSide = ZBlock - KtevCsic_Len / 2.0 - (CsiAcrylicCapLen - CsiAcrylicCapThick) / 2.0;
	double ZMPPC = ZBlock - KtevCsic_Len / 2.0 - CsiMPPCThick / 2.0;
	double ZBoard = ZMPPC - CsiMPPCThick / 2.0 - 0.16/2;
	
	if(ZminAcrylicCover>ZacrylicCoverRoof) ZminAcrylicCover = ZacrylicCoverRoof;
				
	if (boxAcrylicCoverRoof == 0) {
	  boxAcrylicCoverRoof = new GsimBox("AcrylicCoverRoof", csh1,
					    G4ThreeVector(XBlock*cm, YBlock*cm, ZacrylicCoverRoof*cm),
					    G4ThreeVector(0, 0, 0));
	  boxAcrylicCoverRoof->setOuterMaterial("G4_LUCITE");
	  boxAcrylicCoverRoof->setParameters(paramAcrylicRoof);
	  boxAcrylicCoverRoof->setOuterColor("gray75");
	  csh1->addDaughter(boxAcrylicCoverRoof);
	  m_detMoveList.push_back(boxAcrylicCoverRoof);
	} else {
	  boxAcrylicCoverRoof->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZacrylicCoverRoof*cm),
					     G4ThreeVector(0, 0, 0), id_MPPC);
	}
	
	if (boxAcrylicCoverSide == 0) {
	  
	  boxAcrylicCoverSide = new GsimBox("AcrylicCoverSide", csh1,
					    G4ThreeVector((XBlock - CsiAcrylicCapThick / 2)*cm,
							  (YBlock + CsiAcrylicCapWidth / 2 - CsiAcrylicCapThick / 2)*cm,
							  ZacrylicCoverSide*cm),
					    G4ThreeVector(0, 0, 0 * deg));
	  boxAcrylicCoverSide->setOuterMaterial("G4_LUCITE");
	  boxAcrylicCoverSide->setParameters(paramAcrylicSide);
	  boxAcrylicCoverSide->setOuterColor("gray75");
	  csh1->addDaughter(boxAcrylicCoverSide);
	  m_detMoveList.push_back(boxAcrylicCoverSide);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock + CsiAcrylicCapThick / 2)*cm,
							   (YBlock - CsiAcrylicCapWidth / 2 + CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 0 * deg), 1);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock - CsiAcrylicCapWidth / 2 + CsiAcrylicCapThick / 2)*cm,
							   (YBlock - CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 90 * deg), 2);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock + CsiAcrylicCapWidth / 2 - CsiAcrylicCapThick / 2)*cm,
							   (YBlock + CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 90 * deg), 3);
	  
	} else {
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock - CsiAcrylicCapThick / 2)*cm,
							   (YBlock + CsiAcrylicCapWidth / 2 - CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 0 * deg), 4 * id_MPPC + 0);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock + CsiAcrylicCapThick / 2)*cm,
							   (YBlock - CsiAcrylicCapWidth / 2 + CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 0 * deg), 4 * id_MPPC + 1);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock - CsiAcrylicCapWidth / 2 + CsiAcrylicCapThick / 2)*cm,
							   (YBlock - CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 90 * deg), 4 * id_MPPC + 2);
	  
	  boxAcrylicCoverSide->cloneDetector(G4ThreeVector(
							   (XBlock + CsiAcrylicCapWidth / 2 - CsiAcrylicCapThick / 2)*cm,
							   (YBlock + CsiAcrylicCapThick / 2)*cm,
							   ZacrylicCoverSide*cm),
					     G4ThreeVector(0, 0, 90 * deg), 4 * id_MPPC + 3);
	  
	}
	
	if (boxMPPC == 0) {
	  boxMPPC = new GsimBox("MPPC", csh1,
				G4ThreeVector(XBlock*cm, YBlock*cm, ZMPPC*cm),
				G4ThreeVector(0, 0, 0 * deg));
	  boxMPPC->setOuterMaterial("G4_ALUMINUM_OXIDE");
	  boxMPPC->setParameters(paramMPPC);
	  boxMPPC->setOuterColor("black");
	  csh1->addDaughter(boxMPPC);
	  m_detMoveList.push_back(boxMPPC);
	}else{
	  boxMPPC->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZMPPC*cm), G4ThreeVector(0, 0, 0), id);
	}
	
	
	if (boxMPPCBoard == 0){
	  boxMPPCBoard = new GsimBox("MPPCBoard", csh1,
				     G4ThreeVector(XBlock*cm, YBlock*cm, ZBoard*cm),
				     G4ThreeVector(0, 0, 0 * deg));
	  boxMPPCBoard->setOuterMaterial("GsimG10");
	  boxMPPCBoard->setParameters(paramMPPCBoard);
	  boxMPPCBoard->setOuterColor("green");
	  csh1->addDaughter(boxMPPCBoard);
	  m_detMoveList.push_back(boxMPPCBoard);
	}else{
	  boxMPPCBoard->cloneDetector(G4ThreeVector(XBlock*cm, YBlock*cm, ZBoard*cm), G4ThreeVector(0, 0, 0), id);
	}
	//end of if (userFlag >= 201902) 
      }
      
      id++;
      id_MPPC++;
    }
  }

  if (userFlag >= 201902) {
    //U.FL cable wad added by Shimizu
    GsimTube* UFL = new GsimTube( "simplifiedUFL", csh1 ,
				  G4ThreeVector(0,0, (ZminAcrylicCover-CsiAcrylicCapThick/2.)*cm-effthickUFL/2*mm),
				  G4ThreeVector(0,0,0) );
    UFL->setOuterMaterial( "G4_Cu" );
    UFL->setOuterColor( "yellow" );
    UFL->setOuterVisibility( true );
    UFL->setParameters( paramVecUFL );
    csh1->addDaughter(UFL);
    m_detMoveList.push_back(UFL);
  }

  
  setThisAndDaughterBriefName("CSI");
  for(std::list<GsimTube*>::iterator it=ecapList.begin();
      it!=ecapList.end();it++) {
    (*it)->setThisAndDaughterBriefName("ECAP");
  }
  
}

GsimE14CSI::~GsimE14CSI()
{
  ;
}




bool GsimE14CSI::endOfEventAction(const G4Event* )
{
  std::list<GsimSensitiveDetector*> sdList
    =getThisAndDaughterSensitiveDetectorList();
  for(std::map<G4LogicalVolume*,GsimSensitiveDetector*>::iterator
	it=m_sensitiveDetectorMap.begin();
      it!=m_sensitiveDetectorMap.end();it++) {
    GsimSensitiveDetector* sd=(*it).second;
    sdList.push_back(sd);
  }
  sdList.sort();
  sdList.unique();

  double totE=0;
  for(std::list<GsimSensitiveDetector*>::iterator
	it=sdList.begin();it!=sdList.end();it++) {
    totE+=(*it)->getTotalEnergy();
  }
  GsimPersistencyManager* pm=GsimPersistencyManager::getPersistencyManager();

  if(totE>m_totEThreshold) {
    pm->addTriggerBit(TRIG_NCLUS);
  }
  
  return true;
}

void GsimE14CSI::comment()
{
  std::cout << "GsimE14CSI Z position:" << std::endl;
  std::cout << "    Center of CSI region is 180 cm " << std::endl;
  std::cout << "      from 5 cm upstream of the CSI surface." << std::endl;
  std::cout << "    Place it at (609.8+180./2.) cm" << std::endl;
  std::cout << "             if CSI surface is 614.8 cm." << std::endl;
  std::cout << "When userFlag is >=201605, place it downstream by 2 cm."  << std::endl;
  std::cout << "       z=(609.8+180./2.+2)*cm" << std::endl;
}

void GsimE14CSI::setFastSimulationLevel(int level)
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
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("CSI",0);
    }
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
