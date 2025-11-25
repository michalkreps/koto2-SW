/**
 *  @file
 *  @brief  GsimBetaDecay
 *  $Id$
 *  $Log$
 */
#ifndef GsimBetaDecay_h
#define GsimBetaDecay_h

#include <string>

class TH1D;

class GsimBetaDecay
{
 public:
  GsimBetaDecay(std::string name);
  virtual ~GsimBetaDecay();


  void   fillBetaSpectrum(int A, int Zdaughter, double Q,//[MeV]
			  int N=10000);
  double generateBetaMomentum();
  void   cloneHistogram(std::string name,TH1D** hist);
  
  /// betaSpectrum
  /**
   *  @function betaSpectrum
   *  @brief generate weight of beta spectrum
   *  @param Ek:kinetic energy
   *  @param A:Mass number
   *  @param Zdaugher:Daughter Z,+ for beta plus / - for beta minus decay
   *  @param Q: Q-value (available energy) of beta decay in MeV
   **/
  double betaSpectrum(double Ek,int A, int Zdaughter, double Q);//[MeV]

  double nuSpectrum(double Ek,int A, int Zdaughter, double Q);//[MeV]

  double coulombDistortion(int A, int Zdaughter, double E);//[MeV]
  double fermiIntegral(int A, int Zdaughter, double Q);//[MeV]

  void setNuMass(double m);
  void setCoulombDistortion(bool withCoulombDistortion);

  
  
 protected:
  std::string m_name;
  double m_me;
  double m_nu;
  double m_alpha;
  double m_hbarc;
  bool   m_withCoulombDistortion;
  TH1D*  m_hBetaSpectrum;

  /// coefficients for gamma=7, kmax=8  Lanczos method 
  static const double s_lanczos_7_c[9];

  int gsim_gsl_sf_complex_log_e(const double zr, const double zi,double* lnr, double* theta);
  int gsim_gsl_sf_angle_restrict_symm_err_e(const double theta, double* result);
  
  int gsim_lngamma_lanczos_complex(double zr, double zi,double* yr, double* yi);

  /// sinh(x) series
  static int gsim_sinh_series(const double x, double * result);

  /// cosh(x)-1 series
  static int gsim_cosh_m1_series(const double x, double * result);

  int gsim_gsl_sf_complex_sin_e(const double zr, const double zi,double* szr, double* szi);
  int gsim_gsl_sf_angle_restrict_symm_e(double* theta);
  int gsim_gsl_sf_complex_logsin_e(const double zr, const double zi,double* lszr, double* lszi);
  int gsim_gsl_sf_lngamma_complex_e(double zr, double zi, double* lnr, double* arg);
  
  GsimBetaDecay();
};

inline void GsimBetaDecay::setNuMass(double m)
{
  m_nu=m;
}

inline void GsimBetaDecay::setCoulombDistortion(bool withCoulombDistortion)
{
  m_withCoulombDistortion=withCoulombDistortion;
}
#endif //GsimBetaDecay_h
