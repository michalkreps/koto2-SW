/**
 *  @file
 *  @brief  GsimUserGeom.h
 *  $Id: 
 *  $Log:
 */

#ifndef GsimKOTOIIUserGeom_h
#define GsimKOTOIIUserGeom_h

namespace KOTOII {
  // Constants
  static const double  detMoveZ  = 4000;//cm
  
  static const double  KtevCsic_XY  = 5.0;//cm
  static const double  KtevCsic_Len = 50.0;//cm
  static const double  KtevCsic_Gap = 0.01;//cm //corrected by Matsumura
  
  static const double  KtevCsicFine_XY  = 2.5;//cm
  static const double  KtevCsicFine_Len = 50.0;//cm
  static const double  KtevCsicFine_Gap = KtevCsic_Gap*2.5/5.;//cm

  static const int barrelNumberOfSections=6;
  static const double barrelFirstInnerDimater=80;//cm
  static const double barrelDR=45;//cm
  static const double barrelDZsection=340;//cm
  static const double barrelZstart=0;//cm
  static const double barrelNumberOfPhiDivisions[barrelNumberOfSections]=
    {
      20,
      24,
      32,
      32,
      32,
      32
    };
  
}

#endif // GsimKOTOIIUserGeom_h

