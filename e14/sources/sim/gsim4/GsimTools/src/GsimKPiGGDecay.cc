#include "GsimTools/GsimKPiGGDecay.h"
#include "TH1.h"
#include "TFile.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include <cmath>
#include <iostream>
#include <cstdio>

// 2014.9.30 H.Nanjo
// KL->pi0 gamma gamma decay ported from KTeV
// Br : 1.273 x 10^-6 (PDG)
///AV : -0.43 (PDG)
//
//-----------------------------------------------------------------------
//
// V5.18  28-May-1989 Taku
//        * Add switch PI0GGP, PI0GGL and PI0GGS for pi0 gam gam decay wit
//          phase space, theoretical KL, and Ks decay, respectively.
//07-May-98 ECC
//    o Installed new generator for KL-> pi0 gamma gamma events.
//31-Jul-98 ECC
//    o New pi0gg decay model from D'Ambrosia and Portoles.
//      - Uses MBDCAY = 'KPI0GGDP6'. See PGG6INIT for details.
//	(KPI0GGDG6 is Donoghue and Gabbiani model).
//
//-----------------------------------------------------------------------
TH1D GsimKPiGGDecay::s_hMGG = TH1D("hMGG","",10,0,10);


GsimKPiGGDecay::GsimKPiGGDecay()
{
  //2012 PDG
  m_mkl=497.614;
  m_mp0=134.9630;
  m_gammakl=1.291e-14;
  m_alpha=1./137.036;
  
  m_mkl2 = pow(m_mkl,2.);
  m_mkl4 = pow(m_mkl,4.);
  m_mp2  = pow(m_mp0,2.);
  m_rp   = m_mp0/m_mkl;
  m_rp2  = pow((m_mp0/m_mkl),2.);
  m_rp4  = pow(m_rp,4.);

  m_fp=93.;

  m_alfa1 =  91.7e-8;
  m_beta1 = -25.68e-8;
  m_zita1 = -0.47e-8;
  m_csi1  = -1.51e-8;
  m_alfa3 = -7.36e-8;
  m_beta3 = -2.43e-8;
  m_gamma3= 2.26e-8;
  m_zita3 = 0.;
  m_csi3  = 0.;
  m_csi3primo = 0.;

  m_h8 = 3.2e-7;
  m_h27= -1.0e-8;

  m_PGG_AV = -0.43;//PDG average between KTeV and NA48
  m_lambdacut = 770.;
}


GsimKPiGGDecay::~GsimKPiGGDecay()
{
  ;
}

double GsimKPiGGDecay::pgg6babs(double z)
{
  double r2 = m_rp2;
  double ret_val = 0.;


  if (z < 4.*r2) return ret_val;

  double b = sqrt(1.-4.*r2/z);

  double f3 = b-5./3.*pow(b,3.) -.5*pow((1.-b*b),2) * log((1.+b)/(1.-b));
  double j3B = -f3/2./(r2*r2);

  double c = 16./3.*(m_zita1-2.*m_zita3-m_csi1+2.*m_csi3);

  //double lg=log((1.+b)/(1.-b));
  //double imf5=-M_PI*(.5*pow((1.-b*b),2.)*lg+(1.-3.*pow(b,2.))*b)/16.;
  //double imf6=-M_PI*pow(b,3.)/6.;

  ret_val = -m_alpha/8. *c*j3B;

  return ret_val;
}

double GsimKPiGGDecay::pgg6aabs(double y, double z)
{
  double r2 = m_rp2;
  double ret_val = 0.;
  

  if (z<4.*r2) return ret_val;
    
  double b = sqrt(1.-4.*r2/z);
  double lambda = 1 + r2*r2 + z*z - 2.*(r2 + z + r2*z);
    
  double f1 = (1.-b*b)*(  2.*b - (1.-b*b)*log((1.+b)/(1.-b)) );
  double f2 = (1.-b*b)*( -6.*b + (3.-b*b)*log((1.+b)/(1.-b)) );
  double f3 = b-5./3.*b*b -.5*pow((1.-b*b),2.) *log((1.+b)/(1.-b));
    
  double Ymaiusc = z/r2-1./3./r2-1.;
    
  double a = (m_alfa1 + m_alfa3) - (m_beta1 + m_beta3)*Ymaiusc +
    ( m_zita1-2.*m_zita3+m_csi1-2.*m_csi3 )*Ymaiusc*Ymaiusc;
    
  double c = 16./3.*(m_zita1-2.*m_zita3-m_csi1+2.*m_csi3);
    
  double j1A = 4./z*(1.-b*b)*log((1.+b)/(1.-b));

  double j3A = 0.5/(r2*r2) *(lambda*f1/4./z + y*y *f2/z + f3);

  ret_val = -m_alpha/8.*(a*j1A + c*j3A);


  double lg=log((1.+b)/(1.-b));

  //double imfk=0.;

  double imf=-.5*M_PI*(1.-b*b)*lg;

  double imf4=M_PI*(.25*(1.-b*b)*lg+b*(b*b-3.)/6.);

  double imf5=-M_PI*(.5*pow((1.-b*b),2.)*lg-(1.-3.*b*b)*b)/16.;

  double imf6=-M_PI*pow(b,3.)/6.;
    
  double imf1=M_PI*(lg*pow((1.-b*b),2.)/32.+b*(b*b-3.)/48.);
    
  double imf8=
    (M_PI/(192.*m_mp2))*(
		     -6.*(m_mp2+m_mkl2)*pow((1.-b*b),2.)*lg
		     -(
		       6.*(m_mp2-m_mkl2)*pow(b,4.)
		       +12.*(3.*m_mp2+m_mkl2)*b*b
		       -6.*(7.*m_mp2+m_mkl2))
		     *b);
		       
  
  double ime2=-4.*(m_mp2+m_mkl2-z*m_mkl2)*imf1
    -2.*(m_mkl2-m_mp2+z*m_mkl2)*(2.*(imf5-imf8)-imf6);
      
  //c      Absorptive contribution

  // double Aabsp2 = m_alpha*m_h8*m_mkl2/(4.*M_PI*m_fp*m_fp) *
  //   ( imf *
  //     ( (1.-m_rp2 /z)*(1.-m_h27/(5.*m_h8)) +
  // 	m_h27/2./m_h8 *
  // 	( (3.-m_rp2-14.*m_rp4-(5.-14.*m_rp2)*z ) / (z*(1.-m_rp2)) ))
  //     + imfk * (1./z + m_rp2/z - 1.) );
	
      
  //double A1abs = -m_alpha*m_mkl2/(2.*M_PI) * (1.-1./z-m_rp2/z)*(m_rp2-.5*z)*imf;

  double A2abs = m_alpha*m_mkl2/(4.*M_PI) * z*(1.-pow((1./z-m_rp2/z),2.))*imf;

  double A3abs = m_alpha*m_mkl2/(4.*M_PI)*z*pow((1.+1./z-m_rp2/z),2.) *(imf4+imf6-4.*imf5);

  double A4abs = (m_alpha/(4.*M_PI))*ime2;
    
  double A6abs = -(m_alpha*m_mkl2/M_PI)*(y*y/z)*(imf4+imf6-4.*imf5);

  double aux = (m_alfa1 + m_alfa3) - (m_beta1 + m_beta3)*Ymaiusc
    + ( m_zita1-2.*m_zita3+m_csi1-2.*m_csi3 )*Ymaiusc*Ymaiusc;

  double cux = 8.*pow(m_mkl,2.)/(3.*pow(m_mp0,4.))
    *(m_zita1-2.*m_zita3-m_csi1+2.*m_csi3);
    
  double Aabs1 = ((m_alpha/(M_PI*z))*aux*imf) + cux*(A3abs+A4abs+A6abs+.5*A2abs);
      

  //c       inclusion of the pi0-pi0 contribution
  //c
  double poly = -199.41+1750.9*z-5777.2*z*z+8620.2*pow(z,3.)-4925.8*pow(z,4.);
      

  double Aabs2 = -2.*m_alpha*poly*1e-8/z;

  ret_val  = Aabs1+Aabs2;

  return ret_val;
}

double GsimKPiGGDecay::d2brabs(double y, double z)
{
  double aabsorpt = pgg6aabs(y, z);
  double babsorpt = pgg6babs(z);
  double lambda = m_rp4 + 1. + z * z - (m_rp2 + z + m_rp2 * z) * 2.;
  double d2 = aabsorpt + babsorpt;
  double d3 = babsorpt;
  double d4 = lambda / 4. - y * y;
  double ret_val = m_mkl / (M_PI * M_PI * M_PI * 512. * m_gammakl) *
    (z * z * d2 * d2 + d3 * d3 * d4 * d4);
  return ret_val;
}


double GsimKPiGGDecay::pgg6pbdisp(double z)
{
  double c = 16./3.*(m_zita1-2.*m_zita3-m_csi1+2.*m_csi3);
  double r2 = m_rp2;
  double m2= pow(m_mp0,2.);
  double ha8 = 12.*pow(m_fp,2.)/m_mkl2 *(m_alfa1 + 0.1*m_alfa3);
  
  double Bdispa=0;
  if (z <= r2 * 4.) {
    double bt = sqrt(r2*4./z-1.);
    double arcseno = asin(sqrt(z/r2)/2.);

    double ref5 = log(m2/(pow(m_lambdacut,2.)))/8.
      -  pow(arcseno,2.)*pow((pow(bt,2.)+1.),2.)/16.
      - arcseno*bt*(3.*pow(bt,2.)+1.)/8. + 7.*pow(bt,2.)/16.
      + 1./12.;

    double ref6 = log(m2/(pow(m_lambdacut,2.)))/6.
      - pow(bt,3.)*arcseno/3. + pow(bt,2.)/3.
      -1./9.;

    Bdispa = -(m_alpha/(M_PI*2.*pow(r2,2.)))*c*(ref6-2.*ref5);
  } else {
    double b = sqrt(1.-r2*4./z);
    double real2 = pow(log((1. + b)/(1. - b)),2.) - pow(M_PI,2.);

    double ref5 = log(m2/(pow(m_lambdacut,2.)))/8.
      + real2*pow((pow(b,2.)-1.),2.)/64.
      + log((1.+b)/(1.-b))*b*(3.*pow(b,2.)-1.)/16.
      -7.*pow(b,2.)/16.+1./12.;

    double ref6 = log(m2/(pow(m_lambdacut,2.)))/6.
      + pow(b,3.)*log((1.+b)/(1.-b))/6.
      - pow(b,2.)/3. - 1./9.;

    Bdispa = -(m_alpha/(M_PI*2.*pow(r2,2.)))*c*(ref6-2.*ref5);
  }

  double Bdispop = Bdispa
    -2.*m_mkl2*m_alpha*m_PGG_AV*ha8/(4.*pow(m_fp,2.))/M_PI;

  double ret_val=0;
  ret_val = Bdispop;
  
  return ret_val;
}

double GsimKPiGGDecay::pgg6adisp(double y, double z)
{
  double ha8 = 12.*pow(m_fp,2.)/m_mkl2 *(m_alfa1 + 0.1*m_alfa3);
  //double lambda = 1.+ m_rp4 + pow(z,2.) - 2.*(m_rp2 + z + m_rp2*z);

  double r2=m_rp2;
  double m2=pow(m_mp0,2.);


  double ref=0;
  //double refk=0;//rekf is not defined in else-> to be checked
  double ref4=0;
  double ref6=0;
  double ref5=0;
  double ree2=0;

  if (z <= r2 * 4.) {
    double bt=sqrt(4.*r2/z-1.);
    double arco = asin(sqrt(z/r2)/2.) * 2.;
    ref=(1.-(r2/z)*pow(arco,2.));
    //double arcok = asin(sqrt(z)/2.)*2.;
    //refk=(1.-pow(arcok,2.)/z);

    ref4=(1./3.)*log(m2/pow(m_lambdacut,2.))
      +(1.+pow(bt,2.))*pow(arco,2.)/8.
      +(pow(bt,2.)+3.)*bt*arco/6.-pow(bt,2.)/3.-25./18.;

    ref5=(1./8.)*log(m2/pow(m_lambdacut,2.))
      -pow((1.+pow(bt,2.)),2.)*pow(arco,2.)/64.
      -(1.+3.*pow(bt,2.))*bt*arco/16.
      +7.*pow(bt,2.)/16.+1./12.;

    ref6=(1./6.)*log(m2/pow(m_lambdacut,2.))-pow(bt,3.)*arco/6.
      +pow(bt,2.)/3.-1./9.;

    double redive2=pow(m_mkl,4.)/(144.*m2*(1.+pow(bt,2.)))
      *9.*(bt*arco-2.)*(-pow(bt,6.))*(2.*m_rp4-4.*m_rp2+2.);

    double reconve2=pow(m_mkl,4.)/(144.*m2*(1.+pow(bt,2.)))
      *(48.*log(m2/pow(m_lambdacut,2.))*m_rp2*(-pow(bt,2.)-1)
	+9.*(-pow(arco,2.))*(pow(bt,6.)
			     *(1.+m_rp4)+pow(bt,4.)
			     *(-m_rp4+4.*m_rp2+3.)
			     -pow(bt,2.)*(5.*m_rp4-8.*m_rp2-3.)
			     +(-3.*m_rp4+4.*m_rp2+1.)
			     )
	+9.*arco*bt*(pow(bt,4.)*(10.*m_rp4-20.*m_rp2/3.-6.)
		     -pow(bt,2.)*(6.*m_rp4+92.*m_rp2/3.+6.)
		     +(-18.*m_rp4-20.*m_rp2-2.))
	+3.*pow(bt,4.)*(-52.*m_rp4+48.*m_rp2+44.)
	+pow(bt,2.)*(108.*m_rp4+656.*m_rp2+156.)
	+108.*m_rp4+440.*m_rp2+60.);
    ree2 = redive2 + reconve2;
  } else {
    double b = sqrt(1.-r2 * 4./z);
    double relo = log((b + 1.) / (1. - b));
    double relo2=(pow(relo,2.)-pow(M_PI,2.));
    ref=1.+(1.-pow(b,2.))*relo2/4.;

    ref4=(1./3.)*log(m2/pow(m_lambdacut,2.))
      -(1.-pow(b,2.))*relo2/8.-(pow(b,2.)-3.)*b*relo/6.
      +pow(b,2.)/3.-25./18.;

    ref5=(1./8.)*log(m2/pow(m_lambdacut,2.))
      +pow((1.-pow(b,2.)),2.)*relo2/64.
      -(1.-3.*pow(b,2.))*b*relo/16.
      -7.*pow(b,2.)/16.+1./12.;

    ref6=(1./6.)*log(m2/pow(m_lambdacut,2.))+pow(b,3.)*relo/6.
      -pow(b,2.)/3.-1./9.;

    double redive2=pow(m_mkl,4.)/(144.*m2*(1.-pow(b,2.)))
      *9.*(b*relo-2.)*(pow(b,6.))*(2.*m_rp4-4.*m_rp2+2.);


    double reconve2=pow(m_mkl,4.)/(144.*m2*(1.-pow(b,2.)))
      *(
	48.*log(m2/pow(m_lambdacut,2.))*m_rp2*(pow(b,2.)-1.)
	+9.*(relo2)*(-pow(b,6.)*(1.+m_rp4)
		     +pow(b,4.)*(-m_rp4+4.*m_rp2+3.)
		     +pow(b,2.)*(5.*m_rp4-8.*m_rp2-3.)
		     -3.*m_rp4+4.*m_rp2+1.)
	+9.*relo*b*(pow(b,4.)*(10.*m_rp4-20.*m_rp2/3.-6.)
		    +pow(b,2.)*(6.*m_rp4+92.*m_rp2/3.+6.)
		    +(-18.*m_rp4-20.*m_rp2-2.)
		    )
	+3.*pow(b,4.)*(-52.*m_rp4+48.*m_rp2+44.)
	+(-pow(b,2.)*(108.*m_rp4+656.*m_rp2+156.))
	+108.*m_rp4+440.*m_rp2+60.
	);

    ree2 = redive2 + reconve2;
  }

  /*     Amplitudes */



  // double Adispp2 = m_alpha*m_h8*m_mkl2/(4.*M_PI*pow(m_fp,2.))
  //   * ( ref * ( (1.-m_rp2 /z)*(1.-m_h27/(5.*m_h8))
  // 		+m_h27/2./m_h8
  // 		* (
  // 		   (3.-m_rp2-14.*m_rp4-(5.-14.*m_rp2)*z )/(z*(1.-m_rp2))
  // 		   )
  // 		)
  // 	+ refk * (1./z + m_rp2/z - 1.)
  // 	);

  //double A1disp = -m_alpha*m_mkl2/(2.*M_PI) * (1.-1./z-m_rp2/z)*(m_rp2-.5*z)*ref;

  double A2disp = m_alpha*m_mkl2/(4.*M_PI) * z*(1.-pow((1./z-m_rp2/z),2.))*ref;

  double A3disp = m_alpha*m_mkl2/(4.*M_PI) * z*pow((1.+1./z-m_rp2/z),2.)*(ref4+ref6-4.*ref5);

  double A4disp = (m_alpha/(4.*M_PI))*ree2;

  double A6disp = -(m_alpha*m_mkl2/M_PI)*(pow(y,2.)/z)*(ref4+ref6-4.*ref5);

  double Ymaiusc = z/r2-1./(3.*r2)-1.;


  double aux = (m_alfa1 + m_alfa3) - (m_beta1 + m_beta3)*Ymaiusc
    +( m_zita1-2.*m_zita3+m_csi1-2.*m_csi3 )*pow(Ymaiusc,2.);

  double cux = 8.*pow(m_mkl,2.)/(3.*pow(m_mp0,4.))
    *(m_zita1-2.*m_zita3-m_csi1+2.*m_csi3);

  double Adispa = ((m_alpha/(M_PI*z))*aux*ref)
    + cux*(A3disp +A4disp+A6disp+.5*A2disp);

  double ret_val=0;
  ret_val = Adispa
    +m_mkl2*m_alpha*m_PGG_AV*(3.-z+m_rp2)*ha8/(4.*pow(m_fp,2.))/M_PI;
 
  return ret_val;
}


double GsimKPiGGDecay::d2brdisp(double y, double z)
{

  double Adispe = pgg6adisp(y, z);
  double Bdispe = pgg6pbdisp(z);

  double lambda = 1. + m_rp4 + z*z - 2.*(m_rp2 + z + m_rp2*z);
  
  double ret_val
    = m_mkl/(pow(2.,9.) *pow(M_PI,3.) * m_gammakl) 
    * ( z*z * pow( (Adispe + Bdispe),2.)
	+ pow(Bdispe,2.) *pow((lambda/4. -y*y),2.) );
  return ret_val;
}


double GsimKPiGGDecay::pgg6zmax(double y)
{
  double r2 = m_rp2;
  double ret_val = 1.+ r2 - 2.*sqrt( y*y + r2 );
  return ret_val;
}

double GsimKPiGGDecay::pgg6ymax(double z)
{
  double r2 = m_rp2;
  double ret_val  = .5*sqrt(1+r2*r2+z*z-2.*(r2+z+r2*z) );
  return ret_val;
}

double GsimKPiGGDecay::pgg6zmin(double )
{
  return m_zminimo;
}

double GsimKPiGGDecay::pgg6ymin(double )
{
  return m_yminimo;
}


double GsimKPiGGDecay::pgg6int2(double (GsimKPiGGDecay::*f)(double,double),
			     double xa,
			     double xb,
			     double (GsimKPiGGDecay::*ymin)(double),
			     double (GsimKPiGGDecay::*ymax)(double),
			     int numrett)
{


  //integration of a function of a variable
  //  with the method of Gauss with 12 points


  // Initialized data

  double coeff[12] = {
    .1252334085,
    .3678314989,
    .5873179542,
    .7699026741,
    .9041172563,
    .9815606342,
    -.1252334085,
    -.3678314989,
    -.5873179542,
    -.7699026741,
    -.9041172563,
    -.9815606342 };
  
  double peso[12] = {
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363,
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363 };
  

  //epsx = small shift to avoid the extremes of the domain integration in x
  double epsx = (xb - xa) * 1e-12;
  
  int numintervalli = (int) sqrt(numrett) + 1;
  double xsemilargh_interv = (xb - xa - epsx * 2.) / numintervalli / 2.;
  double xcentro_interv = xa + epsx + xsemilargh_interv;
  double xsomma = 0.;
  int i1 = numintervalli;

  
  for (int xconta_interv = 0; xconta_interv < i1; xconta_interv++) {
    for (int ii = 0; ii < 12; ii++) {
      double xpunto = xsemilargh_interv * coeff[ii] + xcentro_interv;

      // Calculation of x-integration in xpunto
      double ya = (this->*ymin)(xpunto);
      double yb = (this->*ymax)(xpunto);
      double epsy = (yb - ya) * 1e-12;
      double ysemilargh_interv = (yb - ya - epsy * 2.) / numintervalli / 2.;
      double ycentro_interv = ya + epsy + ysemilargh_interv;
      double ysomma = 0.;
      int i2 = numintervalli;
      for (int yconta_interv = 0; yconta_interv < i2; yconta_interv++) {
	for (int j = 0; j < 12; j++) {
	  double ypunto = ysemilargh_interv * coeff[j] + ycentro_interv;
	  ysomma += peso[j] * (this->*f)(xpunto, ypunto);
	}
	ycentro_interv += ysemilargh_interv * 2.;
      }
      double xintegrando = ysemilargh_interv * ysomma;
      xsomma += peso[ii] * xintegrando;
    }
    xcentro_interv += xsemilargh_interv * 2.;
  }
  double ret_val = xsemilargh_interv * xsomma;
  return ret_val;
}

double GsimKPiGGDecay::pgg6intx(double (GsimKPiGGDecay::*f)(double,double),
			    double y, double a, double b,
			    int numintervalli)
{

  // integration of a function of a variable
  //    with the method of Gauss with 12 points
  
  // Initialized data 
  static double coeff[12] = {
    .1252334085,
    .3678314989,
    .5873179542,
    .7699026741,
    .9041172563,
    .9815606342,
    -.1252334085,
    -.3678314989,
    -.5873179542,
    -.7699026741,
    -.9041172563,
    -.9815606342
  };
  
  static double peso[12] = {
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363,
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363
  };


  //eps = small shift to avoid the extremes of the domain integration
  double eps = (b - a) * 1e-12;
  double semilargh_interv = (b - a - eps * 2.) / (double) (numintervalli)/ 2.;
  double centro_interv = a + eps + semilargh_interv;
  double punto=0;
  double somma = 0.;
  int i1 = numintervalli;
  for (int conta_interv=0; conta_interv< i1; conta_interv++) {
    for (int j = 0; j < 12; j++) {
      punto = semilargh_interv * coeff[j] + centro_interv;
      somma += peso[j] * (this->*f)(punto, y);
    }
    centro_interv += semilargh_interv * 2.;
  }
  double ret_val = semilargh_interv* somma;
  return ret_val;
}


double GsimKPiGGDecay::pgg6intx2(double (GsimKPiGGDecay::*f)(double,double),
			     double y, double a, double b,
			     int numintervalli)
{
  /* Initialized data */
  static double coeff[12] = {
    .1252334085,
    .3678314989,
    .5873179542,
    .7699026741,
    .9041172563,
    .9815606342,
    -.1252334085,
    -.3678314989,
    -.5873179542,
    -.7699026741,
    -.9041172563,
    -.9815606342
  };
  
  static double peso[12] = {
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363,
    .2491470458,
    .2334925365,
    .2031674267,
    .1600783285,
    .1069393259,
    .0471753363
  };
  
  double eps = (b - a) * 1e-12;
  double semilargh_interv = (b - a - eps * 2.) / (double) (numintervalli)/ 2.;
  double centro_interv = a + eps + semilargh_interv;
  double punto=0;
  double somma = 0.;
  int i1 = numintervalli;
  for (int conta_interv=0; conta_interv< i1; conta_interv++) {
    for (int j = 0; j < 12; j++) {
      punto = semilargh_interv * coeff[j] + centro_interv;
      somma += peso[j] * (this->*f)(punto, y);
    }
    centro_interv += semilargh_interv * 2.;
  }
  
  double ret_val = semilargh_interv* somma;
  return ret_val;
}

void GsimKPiGGDecay::pgg6init()
{

  //------------------------------------------------------------------------
  //
  //      a) Purpose .-
  //
  //                 To evaluate the spectra and branching ratios
  //               for the process
  //                             K_L -> pi0 gamma gamma
  //               in ChPT
  //               Includes:
  //                 1/ O(p^4)
  //                         G. Ecker et al., Phys. Lett. B189 (1987) 363,
  //                    L. Cappiello et al., Nuovo Cimento A99 (1988) 155.
  //                 2/ O(p^6)
  //                   -> Unitarity corrections from K_L -> pi0 pi+ pi-.
  //                         L. Cappiello et al, Phys. Lett. B298 (1993) 423,
  //                         A.G. Cohen et al, Phys. Lett. B304 (1993) 347.
  //                      -> Vector meson contributions (a_V parameter).
  //                         G. Ecker et al., Phys. Lett. B237 (1990) 481.
  //                 3/ Correction from pi0 pi0 -> gamma gamma.
  //                         J. Kambor et al, Phys. Rev. D49 (1994) 2346.
  //
  //
  //      b) Subroutines to be linked with .-
  //                 None. Every needed subroutine is included.
  //
  //
  //      c) Particulars .-
  //
  //                 1/ Unit of energy and mass : MeV.
  //                 2/ Uses double precision.
  //------------------------------------------------------------------------


  //cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
  //   DATA SECTION
  //cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
  m_yminimo = 0.;
  m_zminimo = 0.;
  
  
  //cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
  //   Introduction and Main Menu
  //cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

  double ymassimo = pgg6ymax(m_zminimo);
  double zmassimo = pgg6zmax(m_yminimo);

  //Number of points in the histogram and number of points in Y.
  int numpunti  = 200;
  int numinterv = 100;
  
  double dispi = pgg6int2(&GsimKPiGGDecay::d2brdisp,
			  m_yminimo,
			  ymassimo,
			  &GsimKPiGGDecay::pgg6zmin,
			  &GsimKPiGGDecay::pgg6zmax,
			  numinterv);
  
  double absi = pgg6int2(&GsimKPiGGDecay::d2brabs,
			 m_yminimo,
			 ymassimo,
			 &GsimKPiGGDecay::pgg6zmin,
			 &GsimKPiGGDecay::pgg6zmax,
			 numinterv);
  
  double toti = dispi + absi;
  m_branch=toti;
  //double eps = (zmassimo - m_zminimo) * 1e-12;
  //double passo = (zmassimo - m_zminimo - eps * 2.) / double(numpunti-1);

  // Calculate the minimum and maximum mgg values. 
  
  double mggmin = m_mkl*sqrt(m_zminimo) *1e-3;
  double mggmax = m_mkl*sqrt(zmassimo)*1e-3;
  double epsmgg = (mggmax - mggmin) * 1e-6;
  double passmgg= (mggmax - mggmin - 2.*epsmgg)/double(numpunti-1);

  // Book the histogram 
  char tit[1024];
  std::sprintf(tit,"PGG_AV:%3.2f,Br:%3.2e,",m_PGG_AV,toti);
  s_hMGG.SetBins(numpunti,mggmin,mggmax);
  s_hMGG.SetTitle(tit);
  s_hMGG.SetXTitle("m_{#gamma#gamma} (GeV)");
  s_hMGG.SetYTitle("d#Gamma(K_{L}#rightarrow#pi_{0}#gamma#gamma)/dm_{#gamma#gamma}/#Gamma");
  
  // Loop over the gg mass and fill histogram.
  double mgg=0;
  double z=0;
  double d1=0;
  double d2=0;
  double disp=0;
  double abs0=0;
  double tot1=0;
  double tot3=0;
  for (int k = 0; k <= numpunti-1; k++) {
    mgg = mggmin + epsmgg + passmgg * (double) k;
    z    = pow((mgg/m_mkl * 1.e3),2.);
    d1   = pgg6ymin(z);
    d2   = pgg6ymax(z);
    disp = pgg6intx(&GsimKPiGGDecay::d2brdisp, z, d1, d2, numinterv);
    abs0 = pgg6intx2(&GsimKPiGGDecay::d2brabs, z, d1, d2, numinterv);
    tot1 = disp + abs0;

    // Spectrum of the diphoton-invariant mass 
    tot3 = mgg * 2e6 * tot1 / m_mkl2 / toti;

    // Fill histogram for gg mass distribution.
    s_hMGG.Fill(mgg,tot3);
  }
}



double GsimKPiGGDecay::ranuni() {
  return gRandom->Uniform();
}
  
void GsimKPiGGDecay::gensph(double* vec)
{

  //--------------------------------------------------------
  // GENERATES AN UNIT-LENGTH VECTOR UNIFORMLY IN 4PI.      
  // USES 1 SQRT AND NO SIN,COS'S. HITOSHI OCT-86           
  // Aug 12, 1999 R.Kessler : replace RANF with RANUNI      
  //--------------------------------------------------------
  
  double s=2;
  double u1=0;
  double u2=0;
  while(s>=1) {
    u1=2.*ranuni()-1.;
    u2=2.*ranuni()-1.;
    s=u1*u1+u2*u2;
  }

  double co2=2.*sqrt(1.-s);
  vec[0]=u1*co2;
  vec[1]=u2*co2;
  vec[2]=1.-2.*s;
}


void GsimKPiGGDecay::dboost(double* P0,double U0,double* PA,double* PB)
{
  //---------------------------------------------------------C
  // 4-MOMENTUM PA IN THE REST FRAME OF PARTICLE 0 IS        C
  // EVALUATED IN THE FRAME WHERE PARTILE 0 HAS A 4-MOMENTUM C
  // P0 AND RETURNED AS PB. U0 IS THE MASS OF THE PARTICLE 0.C
  // PA AND PB CAN SHARE THE SAME MEMORY. OCT-86 HITOSHI     C
  // THIS IS THE DOUBLE PRECISION VERSION OF LBOOST.         C
  //---------------------------------------------------------C

  double EP=(PA[3]*P0[3]+PA[0]*P0[0]+PA[1]*P0[1]+PA[2]*P0[2])/U0;
  double CONS=(PA[3]+EP)/(U0+P0[3]);
  PB[0]=PA[0]+CONS*P0[0];
  PB[1]=PA[1]+CONS*P0[1];
  PB[2]=PA[2]+CONS*P0[2];
  PB[3]=EP;
}

void GsimKPiGGDecay::pi0ggdk(double* Ppi,double* Pg2,double* Pg3)
{

  //------------------------------------------------------
  // Generates KL->pi0,g,g->4gamma decay.
  // Uses theory of D'Ambrosia and Portoles               
  //   - See PGG6INIT for details                         
  //------------------------------------------------------

  bool isPhysical=false;

  double sqmk=m_mkl*m_mkl;
  double sqmp0=m_mp0*m_mp0;

  double m34=-1;
  double e1=0;
  double e1min=m_mp0;
  double e1max=(m_mp0 * m_mp0 + m_mkl * m_mkl) / (2.*m_mkl);

  
  while(!isPhysical) {
    // Get mass of the two photons.
    m34=s_hMGG.GetRandom()*1e3;//GeV to MeV
    if(m34<=0) {
      isPhysical=false;
      continue;
    }

    // Calculate the energy of the pi0.
    e1 = (sqmk + sqmp0 - m34 * m34) / (2.*m_mkl);
    if (e1 < e1min || e1 > e1max) {
      isPhysical=false;
      continue;
    }

    // protect against divide by zero.
    if (e1*e1-sqmp0<=0) {
      isPhysical=false;
      continue;
    }
    isPhysical=true;
  }


  // Get energy of the M34 system.
  double eming=0.;
  double emaxg=0.5*(m_mkl-m_mp0*m_mp0/m_mkl);
  double deg=emaxg-eming;
  double sqkp2=0.5*(sqmk+sqmp0);
  double e2=0;
  isPhysical=false;
  while(!isPhysical) {
    e2=eming+deg*ranuni();
    if(pow( (sqkp2+e1*e2-m_mkl*(e1+e2)),2.)/((e1*e1-sqmp0)*(e2*e2)) >1.) {
      isPhysical=false;
      continue;
    }
    isPhysical=true;
  }


  // 4-Momentum of pi1, and 34-system
  double  vec[3];
  gensph(vec);

  double ps=sqrt(e1*e1-sqmp0);

  double pi0[4]={
    ps*vec[0],
    ps*vec[1],
    ps*vec[2],
    e1
  };

  double p34[4]={
    -pi0[0],
    -pi0[1],
    -pi0[2],
    m_mkl-e1
  };

  
  gensph(vec);
  double es=.5*m34;
  
  double pg3[4]={
    es*vec[0],
    es*vec[1],
    es*vec[2],
    es
  };

  double pg4[4]={
    -pg3[0],
    -pg3[1],
    -pg3[2],
    es
  };

  //boost 2 gammas to cm frame of kaon
  dboost(p34,m34,pg3,pg3);
  dboost(p34,m34,pg4,pg4);


  for(int i=0;i<4;i++) {
    Ppi[i]=pi0[i];
    Pg2[i]=pg3[i];
    Pg3[i]=pg4[i];
  }
}

void GsimKPiGGDecay::crateRootFile(const char* ofn)
{
  TFile* tfo = new TFile(ofn,"RECREATE");
  TH1D* hout = (TH1D*)s_hMGG.Clone("hMGGout");
  hout->Write();
  tfo->Close();
}
