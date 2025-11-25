#include "GsimTools/GsimBetaDecay.h"
#include <cmath>
#include <iostream>
#include "TH1.h"

const double GsimBetaDecay::s_lanczos_7_c[9] = {
  0.99999999999980993227684700473478,
  676.520368121885098567009190444019,
  -1259.13921672240287047156078755283,
  771.3234287776530788486528258894,
  -176.61502916214059906584551354,
  12.507343278686904814458936853,
  -0.13857109526572011689554707,
  9.984369578019570859563e-6,
  1.50563273514931155834e-7
};



GsimBetaDecay::GsimBetaDecay(std::string name)
{
  m_name = name;
  
  m_alpha=1./137.035999074;
  m_me=0.510998928;//[MeV]
  m_hbarc=197.3269718;//[MeV fm]
  
  m_nu=0.1e-6; //[MeV]
  m_withCoulombDistortion=true;
  m_hBetaSpectrum=0;
}

GsimBetaDecay::~GsimBetaDecay()
{
  if(m_hBetaSpectrum) delete m_hBetaSpectrum;
}


void GsimBetaDecay::fillBetaSpectrum(int A, int Zdaughter, double Q,int N)
{
  if(m_hBetaSpectrum) delete m_hBetaSpectrum;

  std::string htit = "hBetaSpectrum_";
  htit=htit+m_name;

  m_hBetaSpectrum = new TH1D(htit.c_str(),"beta spectrum",N,0,Q);

  double Ek=0;
  double w=0;
  for(int i=0;i<N;i++) {
    Ek=Q/(N*1.)*(i+0.5);
    w=betaSpectrum(Ek,A,Zdaughter,Q);
    m_hBetaSpectrum->SetBinContent(i+1,w);
  }
}

double GsimBetaDecay::generateBetaMomentum()
{
  if(!m_hBetaSpectrum) {
    std::cerr << "Error : generateBetaMomentum is called without filling the spectrum histogram." << std::endl;
    return -1;
  }

  double Ek=m_hBetaSpectrum->GetRandom();
  double P= sqrt( pow(Ek+m_me,2.)-pow(m_me,2.) );
  return P;
}

void GsimBetaDecay::cloneHistogram(std::string name,TH1D** hist)
{
  if( (*hist) ) delete (*hist);

  if(m_hBetaSpectrum) {
    (*hist) = (TH1D*)m_hBetaSpectrum->Clone(name.c_str());
  }
}

double GsimBetaDecay::coulombDistortion(int A,int Zdaughter,double E)
{
  double k=std::sqrt( 1. - std::pow(m_alpha*Zdaughter,2.) );
  double p=std::sqrt( E*E - m_me*m_me );//[MeV]
  double y= m_alpha*Zdaughter*E/p;
  double rn=1.15*std::pow(A,1./3.);//[fm]

  double p0=
    2.*(1+k) *
    std::pow( 2.*p*rn/m_hbarc, -2.*(1-k) ) *
    std::exp(M_PI*y);

  double lnr=0;
  double arg=0;
  gsim_gsl_sf_lngamma_complex_e(k, y, &lnr,&arg);
  double p1=lnr*2.;
  
  gsim_gsl_sf_lngamma_complex_e(2.*k+1, 0., &lnr,&arg);
  double p2=lnr*2.;

  double out=p0 * std::exp(p1-p2);

  return out;
}


double GsimBetaDecay::fermiIntegral(int A, int Zdaughter, double Q)
{
  double sum=0;
  double N=1000;
  double dE=Q/N;
  for(int i=0;i<N;i++) {
    double Ek=Q/N*(i+1);
    double E=m_me+Ek;
    double p=std::sqrt(E*E-m_me*m_me);
    double cd=coulombDistortion(A,Zdaughter,E);
    double add=cd*p*E
      *sqrt( pow(Q+m_nu-Ek,2.) - pow(m_nu,2.) )
      *(Q+m_nu-Ek)
      *dE;
    sum+=add;
  }
  return sum;
}

double GsimBetaDecay::betaSpectrum(double Ek,int A, int Zdaughter, double Q)
{
  double E=Ek+m_me;
  double out=0;
  if(Ek>0 && Ek<Q) { 
    double p=std::sqrt( std::pow(E,2)-std::pow(m_me,2) );
    out=1.;
    if(m_withCoulombDistortion) {
      out*=coulombDistortion(A,Zdaughter,E);
    }
    out*=p*E
      *sqrt( pow(Q+m_nu-Ek,2.) - pow(m_nu,2.) )
      *(Q+m_nu-Ek);
  }
  return out;
}

double GsimBetaDecay::nuSpectrum(double Ek,int A, int Zdaughter, double Q)
{
  double ke=Q-Ek;
  return betaSpectrum(ke,A,Zdaughter,Q);
}


int
GsimBetaDecay::gsim_gsl_sf_complex_log_e(const double zr, const double zi,
				    double* lnr, double* theta)
{
  /* CHECK_POINTER(lnr) */
  /* CHECK_POINTER(theta) */

  if(zr != 0.0 || zi != 0.0) {
    const double ax = fabs(zr);
    const double ay = fabs(zi);
    //const double minv = std::min(ax, ay);
    const double minv = ((ax) < (ay) ? (ax) : (ay));
    //const double maxv = std::max(ax, ay);
    const double maxv = ((ax) > (ay) ? (ax) : (ay));
    (*lnr) = log(maxv) + 0.5 * log(1.0 + (minv/maxv)*(minv/maxv));
    (*theta) = atan2(zi, zr);
    return 0;
  }
  else {
    return 1;
  }
}


int
GsimBetaDecay::gsim_gsl_sf_angle_restrict_symm_err_e(const double theta, double* result)
{
  /* synthetic extended precision constants */
  const double P1 = 4 * 7.8539812564849853515625e-01;
  const double P2 = 4 * 3.7748947079307981766760e-08;
  const double P3 = 4 * 2.6951514290790594840552e-15;
  const double TwoPi = 2*(P1 + P2 + P3);
  
  const double y =
    ((theta) >= 0.0 ? 1 : -1) //GSL_SIGN(theta)
    * 2 * floor(fabs(theta)/TwoPi);
  double r = ((theta - y*P1) - y*P2) - y*P3;

  if(r >  M_PI) { r = (((r-2*P1)-2*P2)-2*P3); }  /* r-TwoPi */
  else if (r < -M_PI) r = (((r+2*P1)+2*P2)+2*P3); /* r+TwoPi */

  (*result) = r;

  if(fabs(theta) > 0.0625
     /2.2204460492503131e-16//GSL_DBL_EPSILON
     ) {
    (*result) = NAN;
    return 1;
  } else if(fabs(theta) > 0.0625
	    /1.4901161193847656e-08//GSL_SQRT_DBL_EPSILON
	    ) {
    return 0;
  } else {
    return 1;
  }
}


int
GsimBetaDecay::gsim_lngamma_lanczos_complex(double zr, double zi,
					    double* yr, double* yi)
{
  int k;
  double log1_r,    log1_i;
  double logAg_r,   logAg_i;
  double Ag_r, Ag_i;
  double yi_tmp_val;

  zr -= 1.0; /* Lanczos writes z! instead of Gamma(z) */

  Ag_r = s_lanczos_7_c[0];
  Ag_i = 0.0;
  for(k=1; k<=8; k++) {
    double R = zr + k;
    double I = zi;
    double a = s_lanczos_7_c[k] / (R*R + I*I);
    Ag_r +=  a * R;
    Ag_i -=  a * I;
  }

  gsim_gsl_sf_complex_log_e(zr + 7.5, zi, &log1_r,  &log1_i);
  gsim_gsl_sf_complex_log_e(Ag_r, Ag_i,   &logAg_r, &logAg_i);

  /* (z+0.5)*log(z+7.5) - (z+7.5) + LogRootTwoPi_ + log(Ag(z)) */
  (*yr) = (zr+0.5)*log1_r - zi*log1_i - (zr+7.5)
    + 0.9189385332046727418//+ LogRootTwoPi_
    + logAg_r;
  (*yi) = zi*log1_r + (zr+0.5)*log1_i - zi + logAg_i;
  yi_tmp_val = (*yi);
  gsim_gsl_sf_angle_restrict_symm_err_e(yi_tmp_val, yi);
  return 0;
}



/* sinh(x) series
 * double-precision for |x| < 1.0
 */
int
GsimBetaDecay::gsim_sinh_series(const double x, double * result)
{
  const double y = x*x;
  const double c0 = 1.0/6.0;
  const double c1 = 1.0/120.0;
  const double c2 = 1.0/5040.0;
  const double c3 = 1.0/362880.0;
  const double c4 = 1.0/39916800.0;
  const double c5 = 1.0/6227020800.0;
  const double c6 = 1.0/1307674368000.0;
  const double c7 = 1.0/355687428096000.0;
  *result = x*(1.0 + y*(c0+y*(c1+y*(c2+y*(c3+y*(c4+y*(c5+y*(c6+y*c7))))))));
  return 0;
}

/* cosh(x)-1 series
 * double-precision for |x| < 1.0
 */
int
GsimBetaDecay::gsim_cosh_m1_series(const double x, double * result)
{
  const double y = x*x;
  const double c0 = 0.5;
  const double c1 = 1.0/24.0;
  const double c2 = 1.0/720.0;
  const double c3 = 1.0/40320.0;
  const double c4 = 1.0/3628800.0;
  const double c5 = 1.0/479001600.0;
  const double c6 = 1.0/87178291200.0;
  const double c7 = 1.0/20922789888000.0;
  const double c8 = 1.0/6402373705728000.0;
  *result = y*(c0+y*(c1+y*(c2+y*(c3+y*(c4+y*(c5+y*(c6+y*(c7+y*c8))))))));
  return 0;
}


int
GsimBetaDecay::gsim_gsl_sf_complex_sin_e(const double zr, const double zi,
					 double* szr, double* szi)
{
  /* CHECK_POINTER(szr) */
  /* CHECK_POINTER(szi) */

  if(fabs(zi) < 1.0) {
    double ch_m1, sh;
    gsim_sinh_series(zi, &sh);
    gsim_cosh_m1_series(zi, &ch_m1);
    (*szr) = sin(zr)*(ch_m1 + 1.0);
    (*szi) = cos(zr)*sh;
    return 0;
  } else if(fabs(zi) <
	    7.0978271289338397e+02//GSL_LOG_DBL_MAX
	    ) {
    double ex = exp(zi);
    double ch = 0.5*(ex+1.0/ex);
    double sh = 0.5*(ex-1.0/ex);
    (*szr) = sin(zr)*ch;
    (*szi) = cos(zr)*sh;
    return 0;
  } else {
    return 1;
  }
}


int
GsimBetaDecay::gsim_gsl_sf_angle_restrict_symm_e(double * theta)
{
  double r;
  int stat = gsim_gsl_sf_angle_restrict_symm_err_e(*theta, &r);
  *theta = r;
  return stat;
}


int
GsimBetaDecay::gsim_gsl_sf_complex_logsin_e(const double zr, const double zi,
					    double* lszr, double* lszi)
{
  /* CHECK_POINTER(lszr) */
  /* CHECK_POINTER(lszi) */

  if(zi > 60.0) {
    (*lszr) = -M_LN2 + zi;
    (*lszi) =  0.5*M_PI - zr;
  } else if(zi < -60.0) {
    (*lszr) = -M_LN2 - zi;
    (*lszi) = -0.5*M_PI + zr;
  } else {
    double sin_r, sin_i;
    int status;
    gsim_gsl_sf_complex_sin_e(zr, zi, &sin_r, &sin_i); /* ok by construction */
    status = gsim_gsl_sf_complex_log_e(sin_r, sin_i, lszr, lszi);
    if(status == 1) {
      return 1;
    }
  }
  return gsim_gsl_sf_angle_restrict_symm_e(lszi);
}


int
GsimBetaDecay::gsim_gsl_sf_lngamma_complex_e(double zr, double zi, double* lnr, double* arg)
{
  if(zr <= 0.5) {
    /* Transform to right half plane using reflection;
     * in fact we do a little better by stopping at 1/2.
     */
    double x = 1.0-zr;
    double y = -zi;
    double a, b;
    double lnsin_r, lnsin_i;

    int stat_l = gsim_lngamma_lanczos_complex(x, y, &a, &b);
    int stat_s = gsim_gsl_sf_complex_logsin_e(M_PI*zr, M_PI*zi, &lnsin_r, &lnsin_i);

    if(stat_s == 0) {
      int stat_r;
      (*lnr) = 1.14472988584940017414342735135      /* ln(pi) */
	- lnsin_r - a;
      (*arg) = -lnsin_i - b;
      stat_r = gsim_gsl_sf_angle_restrict_symm_e(arg);

      if(stat_r==1 || stat_l==1) return 1;
      else return 0;
    }
    else {
      return 1;
    }
  }
  else {
    /* otherwise plain vanilla Lanczos */
    return gsim_lngamma_lanczos_complex(zr, zi, lnr, arg);
  }
}

