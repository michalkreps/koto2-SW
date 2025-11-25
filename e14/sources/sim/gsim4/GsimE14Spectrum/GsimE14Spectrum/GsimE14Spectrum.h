/**
 *  @file
 *  @brief  GsimE14Spectrum
 *  $Id: GsimE14Spectrum.h,v 1.2 2006/12/06 19:16:29 nanjo Exp $
 *  $Log: GsimE14Spectrum.h,v $
 *  Revision 1.2  2006/12/06 19:16:29  nanjo
 *  CVS variables.
 *
 */
#ifndef GsimE14Spectrum_h
#define GsimE14Spectrum_h

#include "GsimKernel/GsimSpectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimE14Spectrum : public GsimSpectrum
{
public:
  GsimE14Spectrum(std::string name);
  virtual ~GsimE14Spectrum();

  void   setDetectorShiftZ(double dZ);
  
  virtual void   setBeamCondition(int yearMonth);

 protected:
  GsimE14Spectrum();
  double getKaonMomentum(double z);

  void getCircularProfile(double zStart,
			  double& xStart,double& yStart,
			  double& cost,double& sint,double& phi);
    
  void getSquareProfile(double zStart,
			double& xStart,double& yStart,
			double& cost,double& sint,double& phi);

  void getSquareProfile201504(double zStart,
			      double& xStart,double& yStart,
			      double& cost,double& sint,double& phi);
  
  virtual void   generateSpecial(const G4Event* anEvent);

  static double pKLDistExit( double Pk );
  static double pKLWeight( double Pk, double dZ );
  static double pKLDist(double* x,double* par);
  static double gaus(double x,double mean,double sigma);
  static double xTargetImage(double* x,double* par);
  double getMaximumWeight(double z);

  void getCoreLikeProfile(double& zEnd,
			  double& xEnd,double& yEnd,
			  double& cost,
			  double& sint,
			  double& phi);
    
  double m_pKLDistExitZ;
  double m_pMin;
  double m_wMax;
  double m_z;
  int    m_mode;
  
  TF1* m_pKLFunc;

  TF1* m_xTargetImageFunc;
  TF1* m_yTargetImageFunc;

  /// z detector shift used in generateSpecail
  /**
   *  Move detector in z direction by m_detectorShiftZ.
   *  KL spectrum is defined in 20m position from the target,
   *  which is -1m in the original detector coordinate.
   *  If detector is moved by dZ in z direction,
   *  We should use KL spectrum at z=-1m + dZ
   *  in the original detector corrdinate; ie 20m+dZ from the target.
   */
  double m_detectorShiftZ;

  double m_xTarget;
  double m_yTarget;
  bool   m_isFirst;

  int    m_beamCondition;

  ///Lead absorber and collimator edges(mm)
  double m_beamLineEdgeX[14];
  double m_beamLineEdgeY[14];
  double m_beamLineEdgeZ[14];
};

inline void GsimE14Spectrum::setDetectorShiftZ(double dZ)
{
  m_detectorShiftZ=dZ;
}

#endif // GsimE14Spectrum_h





