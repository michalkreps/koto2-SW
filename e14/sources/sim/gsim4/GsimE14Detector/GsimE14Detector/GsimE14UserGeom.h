/**
 *  @file
 *  @brief  GsimUserGeom.h
 *  $Id: GsimE14UserGeom.h,v 1.3 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14UserGeom.h,v $
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */

#ifndef GsimE14UserGeom_h
#define GsimE14UserGeom_h

namespace E14 {
  // Constants
  static const double  Cbar_Len     = 550; // Length of MainBarrel (bottom)
  static const double  Fiber_Len    = 640; // Length of fibers for barrel counters
  
  static const double  Fbar_Len     = 275;
  //static const double  Fbar_Rmin    = 30.83;
  static const double  Fbar_Rmin    = 31.5;
  static const double  Csir_Len     = 180; // CsI region
  static const double  Csir_Zmin    = 609.8; // CsI region (updated : Mon Jun 21 22:45:34 JST 2004)
  static const double  Csic_Zmin    = 614.8; // CsI face   (updated : Mon Jun 21 22:45:34 JST 2004)
  static const double  Csic_XY      = 7.0;
  static const double  Csic_Len     = 30.0;
  static const double  Csic_Gap     = 0.03;
  static const double  KtevCsic_XY  = 5.0;
  static const double  KtevCsic_Len = 50.0;
  static const double  KtevCsic_Gap = 0.01; //corrected by Matsumura
  
  static const double  KtevCsicFine_XY  = 2.5;
  static const double  KtevCsicFine_Len = 50.0;
  static const double  KtevCsicFine_Gap = KtevCsic_Gap*2.5/5.;

  static const double  FineCsiAcrylicCapLen   = 1.3;
  static const double  FineCsiAcrylicCapWidth = 2.4;
  static const double  FineCsiAcrylicCapThick = 0.1;
  
  static const double  CsiAcrylicCapLen   = 1.2;
  static const double  CsiAcrylicCapWidth = 4.9;
  static const double  CsiAcrylicCapThick = 0.1;
  
  static const double  CsiMPPCThick = 0.2;
  static const double  CsiMPPCWidth = 1.0;
  
  static const double  CC03_X  = 4.54;
  static const double  CC03_Y  = 1.79;
  static const double  CC03_Len = 50.0;
  static const double  CC03_YGap=0.01;

  //////unused
  static const double  CC03_XY  = 2.5;
  static const double  CC03_Gap = KtevCsic_Gap*2.5/5.;
  static const double  CC03_XY_NEW  = 2.2;
  static const double  CC03_Len_NEW = 50.0;
  static const double  CC03_Gap_NEW = 0.02;
  //////unused
  

  static const double  CV_Len   =  50.0;
  
  static const double  BeamLine_R = 10.6/2.;
  static const double  MBN_Thick = 0.02;
  
  static const double  Detpos_fbar_z  = 0;
  static const double  Detpos_cbar_z  = 134.8;
  // Measured by Misha (27/Mar/2004)
  static const double  Detpos_csi_z   = Csic_Zmin;

  static const double  Detpos_bcv_z = (134.8+550./2.);
  static const double  Detpos_bcv_z_201605 = (134.8+550-103/2.+1);

  static const double  Detpos_ibar_z =(134.8+550./2.+22.75);
  static const double  Detpos_ibcv_z =(134.8+550./2.+22.75);
  
  
  static const double  Detpos_cv_z_shift = 5.23;//cm 2012.12 by Maeda
  //RCV downstream surface is 5.23 cm upsstream of Csi
  static const double  Detpos_cv_z = Csic_Zmin-Detpos_cv_z_shift;
  static const double  Detpos_cv_z_201605 = Csic_Zmin-Detpos_cv_z_shift+2;
  static const double  Detpos_cv_z_201902 = Csic_Zmin-Detpos_cv_z_shift+2-0.93;

  static const double  Detpos_cover_z = Csic_Zmin;
  static const double  Detpos_cover_z_201605 = Csic_Zmin+2-0.58;
  static const double  Detpos_cover_z_201902 = Csic_Zmin+2;

  static const double  Detpos_plate_z = (Csic_Zmin+KtevCsicFine_Len+6./2.);
  static const double  Detpos_plate_z_201605 = (Csic_Zmin+KtevCsicFine_Len+6./2.+2);

  static const double  Detpos_oev_z = Csic_Zmin;
  static const double  Detpos_oev_z_201605 = Csic_Zmin+2;
  
  static const double  Detpos_cc00_z = -59.0;
  static const double  Detpos_cc01_z =  0;
  static const double  Detpos_cc02_z_shift = 30;
  static const double  NCC_Len=44.66;//cm Lneght of NCC module
  static const double  Detpos_ncc_z_shift = 31.5;//cm NCC module end from FBAR downstream end 2012/12

  static const double  Detpos_lcv_z = Detpos_csi_z;
  static const double  Detpos_lcv_z_201605  = Detpos_csi_z+2-0.85;
  static const double  Detpos_lcv_z_201902  = Detpos_csi_z+2-0.93;

  static const double  Detpos_cc03_z  = Detpos_csi_z;
  //static const double  Detpos_cc04_z  = Detpos_csi_z + 95.5;
  static const double  Detpos_cc03_z_201605  = Detpos_csi_z+2;
  
  static const double  CC04_Len   =  (7.+0.03)*3.;
  
  static const double  Detpos_cc04_z  = Detpos_csi_z + 95.5+31.2;
  static const double  Detpos_cc04_z_201605  = Detpos_csi_z + 95.5+31.2+2;
  //Crystal upstream surfrace : Banno information 2012/12
  
  //static const double  Detpos_cc05_z  = Detpos_csi_z + 250;
  static const double  Detpos_cc05_z  = Detpos_csi_z + 242.5+ 2.0 + 20.0;// CsI+264.5cm
  //crystal upstream end : 200 mm from endcap ds flange by takashima
  static const double  Detpos_cc05_z_201504  = Detpos_csi_z + 242.5+ 2.0 + 20.0 -2;//
  //move 2 cm upstream
  static const double  Detpos_cc05_z_201605  = Detpos_csi_z + 242.5+ 2.0 + 20.0 -2+2;//
  //move 2 cm downstream further
  static const double  Detpos_cc05_z_201902  = Detpos_csi_z + 242.5+ 22.+2;
  

  

  static const double  Detpos_bpcv_z  = Detpos_cc05_z_201504 + (7.006+0.029)*3 + 8;//cm
  static const double  Detpos_bpcv_z_201605  = Detpos_cc05_z_201504 + (7.006+0.029)*3 + 8 +2;//cm
   
  
  static const double  Detpos_dcv_z   = Detpos_csi_z+2+94.72+70.5;
  
  //static const double  Detpos_cc06_z  = Detpos_csi_z + 350;
  static const double  Detpos_cc06_z  = Detpos_csi_z + 242.5+155.5+2.0+19.0;// CsI+419cm
  //crystal upstream end : 190 mm from beam pipe ds end  by takashima
  static const double  Detpos_cc06_z_201605  = Detpos_csi_z + 242.5+155.5+ 2.0+19.0+2;
  static const double  Detpos_cc06_z_201902  = Detpos_csi_z + 242.5+155.5+18.8+19.0+2;

  static const double  Detpos_newbhcv_z  = Detpos_cc06_z+(7.006+0.029)*3+1.125+(1+0.14);
  static const double  Detpos_newbhcv_z_201605  = Detpos_cc06_z+(7.006+0.029)*3+1.125+(1+0.14)+2;
  // wire plane of the 1st module
  //   = 1.125 cm from CC06 downstream surface -> 1cm frame + 0.14cm gap
    
  
  //static const double  Detpos_bhcv_z  = Detpos_csi_z + 400;
  static const double  Detpos_bhcv_z  = Detpos_csi_z + 242.5+155.5+2.0+61.0;
  //zCenter of BHCV: 461*cm from GYLim 2013.4
  // = 110mm from CC06 frame
  // = 610mm from Beam frange

  static const double Detpos_shield_z = Detpos_csi_z + 479.2;
  static const double Detpos_shield_z_201605 = Detpos_csi_z + 479.2+2;
  
  //static const double  Detpos_bhpv_z  = Detpos_csi_z + 430 + 200;
  static const double  Detpos_bhpv_z  = (31500+723+1361-(21000+507))*0.1; //cm
  //BHPV 1st module upstream surface : Mark(31500 from T1)+723mm+1361mm from Maeda 2013.3 
  //z=0 : 21m+507mm from the T1 target
  // then  12077mm = CSI+592.9cm

  static const double  Detpos_bhpv_z_201504  = Detpos_shield_z+100+52;//cm

  static const double Detpos_bhgc_z_shift = 0.;//cm
  
  static const double Detpos_bhts_z_shift = 16.8;

  static const double Detpos_bhts_z_shift_201504
    = Detpos_bhgc_z_shift+62.6 + 4.28;


  static const double Detpos_ucv_z_202002 = -60.7;//cm
  static const double Detpos_ucv_z_202102 = -85.75;//cm
}

#endif // GsimE14UserGeom_h

