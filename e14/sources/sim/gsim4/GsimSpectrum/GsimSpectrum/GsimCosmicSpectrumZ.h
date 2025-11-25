/**
 *  @file
 *  @brief  GsimCosmicSpectrumZ
 *  $Id$
 *  $Log$
 */
#ifndef GsimCosmicSpectrumZ_h
#define GsimCosmicSpectrumZ_h

#include "GsimKernel/GsimSpectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF2;

class GsimCosmicSpectrumZ : public GsimSpectrum
{
public:
  GsimCosmicSpectrumZ(std::string name);
  virtual ~GsimCosmicSpectrumZ();

  void   setCosmicRayDX(double dX);
  void   setCosmicRayDZ(double dZ);

 protected:
  GsimCosmicSpectrumZ();
  virtual void   generateSpecial(const G4Event* anEvent);

  TF2* m_pMuonFunc;

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


inline void GsimCosmicSpectrumZ::setCosmicRayDX(double dX)
{
  m_cosmicRayDX=dX;
}

inline void GsimCosmicSpectrumZ::setCosmicRayDZ(double dZ)
{
  m_cosmicRayDZ=dZ;
}

#endif // GsimCosmicSpectrumZ_h





