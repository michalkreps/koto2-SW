/**
 *  @file
 *  @brief  GsimCosmicSpectrum
 *  $Id$
 *  $Log$
 */
#ifndef GsimCosmicSpectrum_h
#define GsimCosmicSpectrum_h

#include "GsimKernel/GsimSpectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimCosmicSpectrum : public GsimSpectrum
{
public:
  GsimCosmicSpectrum(std::string name);
  virtual ~GsimCosmicSpectrum();

  void   setCosmicRayDX(double dX);
  void   setCosmicRayDZ(double dZ);

 protected:
  GsimCosmicSpectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  TF1* m_thetaFunc;
  TF1* m_pMuonFunc;

  /// dx for cosmic ray generation
  /**
   * cosmic ray is generated from -dx+x to +dx+x at y
   */
  double m_cosmicRayDX;

  /// dz for cosmic ray generation
  /**
   * cosmic ray is generated from -dz+z to +dz+z at y
   */
  double m_cosmicRayDZ;

  

  double m_boundaryX;
  double m_boundaryY;
  double m_boundaryZ;
  bool   m_isBoundaryChecked;
};


inline void GsimCosmicSpectrum::setCosmicRayDX(double dX)
{
  m_cosmicRayDX=dX;
}

inline void GsimCosmicSpectrum::setCosmicRayDZ(double dZ)
{
  m_cosmicRayDZ=dZ;
}

#endif // GsimCosmicSpectrum_h





