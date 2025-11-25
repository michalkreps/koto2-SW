/**
 *  $Id: GsimE14Spectrum.cc,v 1.2 2006/12/06 19:16:29 nanjo Exp $
 *  $Log: GsimE14Spectrum.cc,v $
 *  Revision 1.2  2006/12/06 19:16:29  nanjo
 *  CVS variables.
 *
 */
#include "TF1.h"

#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include "GsimE14Spectrum/GsimE14SpectrumMessenger.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"

#include "CLHEP/Random/Randomize.h"

#include <string>
#include <vector>
#include <cmath>




GsimE14Spectrum::GsimE14Spectrum(std::string name)
  : GsimSpectrum(name)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  delete messenger;
  messenger = new GsimE14SpectrumMessenger(this);

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("hE14Mome","hE14Mome [GeV/c]",100,0,10);
  
  m_parameter=1.;
  
  m_pMin=5e-2*GeV;
  m_z=1000*m;
  m_wMax=1.;
  m_pKLDistExitZ=-1*m;
  
  
  
  m_pKLFunc = new TF1("pKLFunc",
		      pKLDist,0,10*GeV,1);
  m_pKLFunc->SetNpx(1000);
  m_pKLFunc->SetParameter(0,m_z);
  
  m_xTargetImageFunc=0;
  m_yTargetImageFunc=0;
  
  m_mode=1;

  m_detectorShiftZ=0.;
  m_beamCondition=0;

  m_isFirst=true;

  {
    double X[14]=
      {
	3.6/2.,
	3.6/2.,
	0.97,
	0.76,
	0.76,
	1.59,
	2.66,
	2.81,
	2.81,
	3.56,
	3.56,
	4.15,
	4.15,
	4.35
      };
    
    double Y[14]=
      {
	3.4/2.,
	3.4/2.,
	1.51,
	1.25,
	1.25,
	1.93,
	2.80,
	2.95,
	2.95,
	3.67,
	3.67,
	4.34,
	4.34,
	4.56
      };
    
    double Z[14]=
      {
	376.5-7/2.,
	376.5+7/2.,
	850-200+25-50/2.,
	850-200+25+50/2.,
	850-200+50+175-350/2.,
	850-200+50+175+350/2.,
	1750-250+25-50/2.,
	1750-250+25+50/2.,
	1750-250+50+125-250/2.,
	1750-250+50+125+250/2.,
	1750-250+50+250+75-150/2.,
	1750-250+50+250+75+150/2.,
	1750+250-25-50/2.,
	1750+250-25+50/2.
      };

    for(int i=0;i<14;i++) {
      m_beamLineEdgeX[i]=X[i]*10;
      m_beamLineEdgeY[i]=Y[i]*10;
      m_beamLineEdgeZ[i]=Z[i]*10;
    }
  }
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimE14Spectrum::~GsimE14Spectrum()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_pKLFunc;
  delete m_xTargetImageFunc;
  delete m_yTargetImageFunc;
  delete messenger;
  messenger=0;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimE14Spectrum::getCircularProfile(double zStart,
					 double& xStart,double& yStart,
					 double& cost,double& sint,double& phi)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double zTarget   = -21 * m;

  //Originally 9.0 usr -> 7.8 usr
  //double dcost = 9.0e-6 / (2 * M_PI ) * m_parameter;
  
  double dcost = 7.8e-6 / (2 * M_PI ) * m_parameter;

  cost = 1 - dcost * CLHEP::RandFlat::shoot();
  sint = std::sqrt(1 -cost*cost);
  double tant = sint/cost;
  phi =2* M_PI * CLHEP::RandFlat::shoot();
  
  xStart = xStart+tant*cos(phi) * (zStart - zTarget); 
  yStart = yStart+tant*sin(phi) * (zStart - zTarget);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14Spectrum::getSquareProfile(double zStart,
				       double& xStart,double& yStart,
				       double& cost,double& sint,double& phi)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  double xColSize = 15.2/2.* mm;
  double yColSize = 25.0/2.* mm;
  double zCol     = -21*m + 7*m;

  m_xTarget=0;
  m_yTarget=0;
  double zTarget = -21 * m;

  double xCol=1e3 *m;
  double yCol=1e3 *m;

  if(m_isFirst) {
    GsimPersistencyManager* pm = GsimPersistencyManager::getPersistencyManager();
  
    pm->setBranchOf( "eventTree", "xTarget", &m_xTarget, "xTarget/D");
    pm->setBranchOf( "eventTree", "yTarget", &m_yTarget, "yTarget/D");
    
    
    m_isFirst=false;
  }
  
  

  while(
	!
	(xCol>  -xColSize &&
	 xCol < +xColSize &&
	 yCol > -yColSize &&
	 yCol < +yColSize
	 )
	) {
    
    m_xTarget = CLHEP::RandFlat::shoot(-9.1, 9.1) * mm;
    m_yTarget = CLHEP::RandFlat::shoot(-1., 1.) * mm;
    
    cost = CLHEP::RandFlat::shoot(cos(3.1e-3),1.);
    sint = std::sqrt(1 -cost*cost);
    phi =2* M_PI * CLHEP::RandFlat::shoot();
    
    double tCol=(zCol-zTarget)/cost;
    //Zcol = cost*t + zTarget
    xCol = sint*cos(phi)*tCol+m_xTarget;
    yCol = sint*sin(phi)*tCol+m_yTarget;
  }

  double tStart = (zStart-zTarget)/cost;
  xStart = xStart+sint*cos(phi)*tStart+m_xTarget;
  yStart = yStart+sint*sin(phi)*tStart+m_yTarget;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14Spectrum::generateSpecial(const G4Event* anEvent)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  G4ThreeVector vPos = pv->GetPosition();
  double x=vPos.x();
  double y=vPos.y();
  double z=vPos.z();

  double PKaon = getKaonMomentum(z+m_detectorShiftZ);


  double xStart = x;
  double yStart = y;
  double zStart = z;
  double cost = 0.;
  double sint = 0.;
  double phi  = 0.;
  

  getCircularProfile(zStart+m_detectorShiftZ,
		     xStart,yStart,cost,sint,phi);
  
  G4ThreeVector vtx(xStart,yStart,zStart);

  G4ThreeVector  Kdir = G4ThreeVector(sint*cos(phi),sint*sin(phi),cost);
  G4ThreeVector p = Kdir * PKaon;

  
  pv->SetPosition(vtx.x(), vtx.y(),vtx.z());
  G4PrimaryParticle*    particle    = pv->GetPrimary();
  particle->SetMomentum(p.x(),p.y(),p.z());

  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("hE14Mome",p.mag()/GeV);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}




double GsimE14Spectrum::pKLDistExit( double Pk )
{
  Pk=Pk/GeV;
  //Pk[GeV/c]
  //30-GeV proton on Ni target
  //After 7-cm Pb absorber 
  //Fitted by Nakajima


  double pKLDistExitPar[8]=
    {
      1.78303e+03,
      1.24432e+00,
      5.39864e-01,
      -5.11454e-01,
      4.43691e-02,
      6.18994e+02,
      1.00234e+00,
      1.31606e-01
    };

  
  double sigma = pKLDistExitPar[2]*( 1 - (pKLDistExitPar[3] + Pk*pKLDistExitPar[4])*(Pk-pKLDistExitPar[1]) );
  double yval = pKLDistExitPar[0]*
    std::exp(-0.5*(Pk-pKLDistExitPar[1])*(Pk-pKLDistExitPar[1])/(sigma*sigma));
					     
  
  
  //gausian part
  double A = Pk-pKLDistExitPar[6];
  double fit_gaus =  pKLDistExitPar[5]*std::exp(- A*A/(2*pKLDistExitPar[7]*pKLDistExitPar[7]));
  
  double val =  yval-fit_gaus;
  val=val/pKLDistExitPar[0];
  return val;
}


double GsimE14Spectrum::pKLWeight( double Pk, double dZ )
{

  // Limit the wieght increase
  // KL spectrum near PK~0.15GeV should be zero.
  // But it remains due to function approximation.
  // In order to avoid spectrum distortion at low momentum,
  // weight is limitted by hand.
  if(dZ<0 && Pk<0.15*GeV) Pk=0.15*GeV;
  if(dZ<8*m && Pk<0.2*GeV) Pk=0.2*GeV;
  if(dZ<12*m && Pk<0.25*GeV) Pk=0.25*GeV;
  
  //maximum wieght = exp(20.)
  double MK=0.497614*GeV;
  double ctau=15.34*m;
  double weight=1.;
  double lnweight=-MK/Pk*dZ/ctau;
  if(lnweight<20.) {
    weight=std::exp(lnweight);
  } else {
    weight=std::exp(20.);
  }
  return weight;
}

double GsimE14Spectrum::getKaonMomentum(double z)
{
  //z should be larger than just after the Pb abosrber
  if(m_z!=z) {
    m_z=z;
    m_wMax=getMaximumWeight(z);
    double dZ=z-m_pKLDistExitZ;
    m_pKLFunc->SetParameter(0,dZ);
  }

  if(m_mode==1) {
    double Pk=m_pKLFunc->GetRandom();
    return Pk;
  } else {
    double dZ=z-m_pKLDistExitZ;
    double Pk=0;
    while (1) {
      Pk = 8.0* GeV *  CLHEP::RandFlat::shoot(); // GeV/c  maximam 8GeV/c
      
      double weight=pKLWeight(Pk, dZ);
      double val =  pKLDistExit( Pk )*weight;
      if (val > m_wMax*CLHEP::RandFlat::shoot() ) break;
    }
    return Pk;
  }
}


double GsimE14Spectrum::pKLDist(double* x,double* par)
{
  //x[0]: Pk with unit
  //par[0]: dZ with unit
  return pKLDistExit(x[0])*pKLWeight(x[0],par[0]);
}


double GsimE14Spectrum::getMaximumWeight(double z)
{
  
  double dZ=z-m_pKLDistExitZ;
  int N=100;
  double pMin=0.*GeV;
  double pMax=10.*GeV;
  double dP=(pMax-pMin)/(N*1.);

  int iMax=-1;
  double wMax=-1;
  double wPrev=-1;
  double wNext=-1;
  for(int j=0;j<3;j++) {
    for(int i=0;i<N;i++) {
      double Pk=dP*(i+1)+pMin;
      double w=pKLDistExit(Pk)*pKLWeight(Pk,dZ);
      if(w>wMax) {
	iMax=i;
	wMax=w;
      }
    }
    if(iMax==-1) break;
    
    if(iMax>=0) {
      pMin=(iMax)*dP+pMin;
    }
    if(iMax<N-1) {
      pMax=(iMax+2)*dP+pMin;
    }
    dP=(pMax-pMin)/(N*1.);
    wPrev=pKLDistExit(pMin)*pKLWeight(pMin,dZ);
    wNext=pKLDistExit(pMax)*pKLWeight(pMax,dZ);
  }
  if(iMax!=-1) {
    if(wPrev>wNext) {
      wMax=wMax+(wMax-wNext);
    } else {
      wMax=wMax+(wMax-wPrev);
    }
  }

  return wMax;
}

double GsimE14Spectrum::gaus(double x,double mean,double sigma)
{
  // Calculate a gaussian function from TMath
   if (sigma == 0) return 1.e30;
   double arg = (x-mean)/sigma;
   // for |arg| > 39  result is zero in double precision
   if (arg < -39.0 || arg > 39.0) return 0.0;
   double res = std::exp(-0.5*arg*arg);
   return res;
}


double GsimE14Spectrum::xTargetImage(double* x,double* par)
{
  // Gaussian  h1*Gaus(x,x1,sigma1)  -inf  <-> x1+e1 
  // Linear    alpha*x+beta          x1+e1 <-> x2+e2
  // Gaussian  h2*Gaus(x,x2,sigma2)  x2+e2 <-> +inf
  // 10 parameters
  
  // Boundary condition for continuous function 
  // Gaus(x1+e1,x1,sigma1)=alpha*(x1+e1)+beta
  // Gaus(x2+e2,x2,sigma1)=alpha*(x2+e2)+beta
  // Gaus'(x1+e1,x1,sigma1)=alpha*(x1+e1)
  // Gaus'(x2+e2,x2,sigma1)=alpha*(x2+e2)

  // 10-4=6 independent parameters


  double alpha=par[0];
  double beta=par[1];
  double x1=par[2];
  double sigma1=par[3];
  double x2=par[4];
  double sigma2=par[5];

  double e1=
    (
     -(alpha*x1+beta)
     +std::sqrt((alpha*x1+beta)*(alpha*x1+beta)
		-4*alpha*alpha*sigma1*sigma1
                                  )
     )/(2*alpha);

  double e2=
    (
     -(alpha*x2+beta)
     +std::sqrt((alpha*x2+beta)*(alpha*x2+beta)
		-4*alpha*alpha*sigma2*sigma2
                                  )
     )/(2*alpha);

  double h1=
    alpha/
    (
     gaus(0,e1,sigma1)*(-e1/(sigma1*sigma1))
     );

  double h2=
    alpha/
    (
     gaus(0,e2,sigma2)*(-e2/(sigma2*sigma2))
     );

  double out=0;
  if(x[0]<x1+e1) {
    out=h1*gaus( x[0],x1,sigma1);
  } else if(x[0]<x2+e2) {
    out=alpha*x[0]+beta;
  } else if(x[0]>x2+e2) {
    out=h2*gaus( x[0],x2,sigma2);
  }

  return out;
}

void GsimE14Spectrum::setBeamCondition(int yearMonth)
{
  if(m_beamCondition==yearMonth) return;

  m_beamCondition=yearMonth;

  if(m_beamCondition>=201504) {
    double xImageSize = 18.4* mm;
    double yImageSize = 5* mm;

    if(m_xTargetImageFunc) delete m_xTargetImageFunc;
    m_xTargetImageFunc = new TF1("xTargetImageFunc",
				 xTargetImage,
				 -xImageSize,xImageSize,6);
    m_xTargetImageFunc->SetNpx(1000);
    m_xTargetImageFunc
      ->SetParameters(-16.99,1357,-5.166,3.281,5.016,3.443);

    if(m_yTargetImageFunc) delete m_yTargetImageFunc;
    m_yTargetImageFunc = new TF1("yTargetImageFunc",
				 "exp(-x*x/(2*[0]*[0]))",
				 -yImageSize,yImageSize);
    m_yTargetImageFunc->SetNpx(1000);
    m_yTargetImageFunc->SetParameter(0,1.08);
    
  }
}

void GsimE14Spectrum::getSquareProfile201504(double zStart,
					     double& xStart,double& yStart,
					     double& cost,double& sint,double& phi)
{

  if(m_isFirst) {
    GsimPersistencyManager* pm = GsimPersistencyManager::getPersistencyManager();
  
    pm->setBranchOf( "eventTree", "xTarget", &m_xTarget, "xTarget/D");
    pm->setBranchOf( "eventTree", "yTarget", &m_yTarget, "yTarget/D");
    
    
    m_isFirst=false;
  }
  
  double zTarget = -21 * m;
  double zCol     = zTarget + 7*m;
  double xColSize = 15.2/2.* mm;
  double yColSize = 25.0/2.* mm;

  m_xTarget = m_xTargetImageFunc->GetRandom();
  m_yTarget = m_yTargetImageFunc->GetRandom();

  double xp=xColSize*(2*CLHEP::RandFlat::shoot()-1);
  double yp=yColSize*(2*CLHEP::RandFlat::shoot()-1);

  double vd[3]={
    xp-m_xTarget,
    yp-m_yTarget,
    zCol-zTarget
  };

  double mag=std::sqrt( vd[0]*vd[0]+vd[1]*vd[1]+vd[2]*vd[2] );
  cost=vd[2]/mag;
  sint = std::sqrt(1 -cost*cost);
  phi  = std::atan2(vd[1],vd[0]);

  double tStart = (zStart-zTarget)/cost;
  xStart = xStart+sint*cos(phi)*tStart+m_xTarget;
  yStart = yStart+sint*sin(phi)*tStart+m_yTarget;
  
}

void GsimE14Spectrum::getCoreLikeProfile(double& zEnd,
					 double& xEnd,double& yEnd,
					 double& cost,
					 double& sint,
					 double& phi)
{
  zEnd=20e3;
  int iEdge[8]={2,4,5,6,8,10,12,13};
  double areaMax=7000;

  bool isOK=false;
  while(!isOK) {

    double xAbs=
      CLHEP::RandFlat::shoot(-m_beamLineEdgeX[0],+m_beamLineEdgeX[0]);
    double yAbs=
      CLHEP::RandFlat::shoot(-m_beamLineEdgeY[0],+m_beamLineEdgeY[0]);
    double zAbs=(m_beamLineEdgeZ[0]+m_beamLineEdgeZ[1])/2.;
  
    double tanL=0;
    double tanXMax=1;
    double tanXMin=-1;
    double tanYMax=1;
    double tanYMin=-1;
    for(int i=0;i<8;i++) {
      tanL=(m_beamLineEdgeX[iEdge[i]]-xAbs)/(m_beamLineEdgeZ[iEdge[i]]-zAbs);
      if(tanL<tanXMax) tanXMax=tanL;
      
      tanL=(-m_beamLineEdgeX[iEdge[i]]-xAbs)/(m_beamLineEdgeZ[iEdge[i]]-zAbs);
      if(tanL>tanXMin) tanXMin=tanL;
      
      tanL=(m_beamLineEdgeY[iEdge[i]]-yAbs)/(m_beamLineEdgeZ[iEdge[i]]-zAbs);
      if(tanL<tanYMax) tanYMax=tanL;
      
      tanL=(-m_beamLineEdgeY[iEdge[i]]-yAbs)/(m_beamLineEdgeZ[iEdge[i]]-zAbs);
      if(tanL>tanYMin) tanYMin=tanL;
    }
    
    double xmax = tanXMax*(zEnd-zAbs)+xAbs;
    double xmin = tanXMin*(zEnd-zAbs)+xAbs;
    double ymax = tanYMax*(zEnd-zAbs)+yAbs;
    double ymin = tanYMin*(zEnd-zAbs)+yAbs;
    double area=(xmax-xmin)*(ymax-ymin);

    double prob = area/areaMax;
    double r=CLHEP::RandFlat::shoot(0.,1.);
    if(r>prob) continue;
    //r<prob : OK
    
    xEnd=CLHEP::RandFlat::shoot(xmin,xmax);
    yEnd=CLHEP::RandFlat::shoot(ymin,ymax);
    
    double vd[3]={
      xEnd-xAbs,
      yEnd-yAbs,
      zEnd-zAbs
    };

    double mag=std::sqrt( vd[0]*vd[0]+vd[1]*vd[1]+vd[2]*vd[2] );
    cost=vd[2]/mag;
    sint = std::sqrt(1 -cost*cost);
    phi  = std::atan2(vd[1],vd[0]);
    
    isOK=true;
  }
}
