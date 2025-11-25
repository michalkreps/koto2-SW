#include "E14Fsim/E14FsimMBProposalInefficiency.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include <iostream>
#include <cmath>

E14FsimMBProposalInefficiency::E14FsimMBProposalInefficiency()
{
  /////////////////////////////////////////////
  // For proposal inefficiency function
  double tmpv[9][4]={
    {0.491215, -85.3823,  4.3708e-7,-1.83026},
    {0.645205, -130.363,  4.3708e-7,-1.83026},
    {0.729022, -162.020,  4.3708e-7,-1.83026},
    {0.783803, -180.183,  4.3708e-7,-1.83026},
    {0.821742, -191.685,  4.3708e-7,-1.83026},
    {0.844931, -199.065,  4.3708e-7,-1.83026},
    {0.860766, -203.985,  4.3708e-7,-1.83026},
    {0.868633, -206.584,  4.3708e-7,-1.83026},
    {0.872535, -207.804,  4.3708e-7,-1.83026},
  };
  for(int i=0;i<9;i++) {
    for(int j=0;j<4;j++) {
      m_fpar[i][j]=tmpv[i][j];
    }
  }
  m_frame=0;
}

E14FsimMBProposalInefficiency::~E14FsimMBProposalInefficiency()
{
  delete m_frame;
}

double E14FsimMBProposalInefficiency::getWeight(double Mome,double theta)
{
  double x=Mome;
  x=x*1e-3;
  
  if ( x < 0.0005 )
    return 1.0;
  else if ( x > 2.0 )
    return func1(m_fpar[0],2.0);
  
  // 
  int     tindex = (int)floor((theta-5)/10.);
  double  tmod   = (theta-5)/10. - tindex;
  
  int  t1 = tindex;
  int  t2 = t1 + 1;
  if ( t1 < 0 ) t1 = 0;
  if ( t2 < 0 ) t2 = 0;
  if ( t1 > 8 ) t1 = 8;
  if ( t2 > 8 ) t2 = 8;
  
  double  f1,f2;  
  if ( x < 0.005 ){
    f1 = func0( m_fpar[t1],x );
    f2 = func0( m_fpar[t2],x );
  }
  else{
    f1 = func1( m_fpar[t1],x );
    f2 = func1( m_fpar[t2],x );
  }
  double  f = exp(log(f1)*(1.-tmod) + log(f2)*tmod);
  
  return f;
}

double E14FsimMBProposalInefficiency::getWeight(double Mome,double theta,double)
{
  return getWeight(Mome,theta);
}



// MB : 5MeV<E<2GeV
// Do not call this function directly
double
E14FsimMBProposalInefficiency::func1( double* par, double x )
{
  return  par[0] * exp(par[1] * x) + par[2] * pow(x,par[3]);
}

// MB : 0.5MeV<E<5 MeV
// Do not call this function directly
double
E14FsimMBProposalInefficiency::func0( double* par, double x )
{
  double  f = func1(par,0.005);
  double  b = log(f)/(0.005-0.0005);
  double  a = exp(-0.0005*b);
  return  a * exp( b*x );
}

void    E14FsimMBProposalInefficiency::drawFrame()
{
  if(!m_frame) {
    m_frame = new TH1D("E14FsimMBProposalInefficiency_frame","",11,0.1,1.2e3);
    m_frame->SetMaximum(1.);
    m_frame->SetMinimum(1e-7);
    m_frame->SetStats(0);
    m_frame->SetXTitle("Energy (MeV)");
    m_frame->SetYTitle("Inefficiency");
  }
  m_frame->Draw("0");
}


TGraph* E14FsimMBProposalInefficiency::draw(double theta)
{
  double x[100];
  double yProposal[100];
  
  int cnt=0;
  for(int i=0;i<4;i++) {
    double target=std::pow(10,i-1);
    for(int j=0;j<10;j++) {
      x[cnt]=target*j;
      cnt++;
    }
  }

  for(int i=0;i<cnt;i++) {
    yProposal[i]=getWeight(x[i],theta,-999);
  }
  TGraph* gr = new TGraph(100,x,yProposal);
  gr->SetMarkerStyle(24);
  gr->SetMarkerColor(3);
  gr->Draw("P");
  m_graphList.push_back(gr);
  return gr;
}


TGraphErrors* E14FsimMBProposalInefficiency::drawES171()
{


  //ES171 data
  // Pb1mm/Sc5mm
  // Eth_{scale} = 1 MeV
  // E_{visible} = 1 MeV / (1*5)/(11.35*1+1*5)=0.3058;


  
  double xvT[24]={ 0.0224086, 0.0559061, 0.0934969, 0.131975, 0.170802, 0.207658, 0.244666, 0.28146, 0.327216, 0.368459, 0.406463, 0.443562, 0.480411, 0.517354, 0.554648, 0.592122, 0.700305, 0.700305, 0.77589, 0.77589, 0.84967, 0.84967, 0.92391, 0.92391 };
  double yv[24]={ 0.2778994E-04, 0.9429071E-04, 0.3573097E-04, 0.1297182E-04, 0.1562095E-04, 0.1030423E-04, 0.1308490E-04, 0.5221169E-05, 0.3596421E-05, 0.2326253E-05, 0.3456075E-05, 0.7194312E-06, 0.1499488E-05, 0.1336396E-05, 0.000000, 0.1301703E-05, 0.000000, 0.000000, 0.4939464E-06, 0.4939464E-06, 0.1521854E-05, 0.1521854E-05, 0.5663539E-06, 0.5663539E-06 };
  double xeT[24]={ 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04 };
  double ye[24]={ 0.3066304E-04, 0.2221464E-04, 0.1090892E-04, 0.4438076E-05, 0.6048974E-05, 0.4310387E-05, 0.5118095E-05, 0.3320615E-05, 0.1914124E-05, 0.1363181E-05, 0.1863598E-05, 0.7904913E-06, 0.1210690E-05, 0.1079278E-05, 0.000000, 0.1045090E-05, 0.000000, 0.000000, 0.5241814E-06, 0.5241814E-06, 0.9714544E-06, 0.9714544E-06, 0.6002078E-06, 0.6002078E-06 };

  double xv[24];
  double xe[24];
  for(int i=0;i<24;i++) {
    xv[i]=xvT[i]*1e3;
    xe[i]=xeT[i]*1e3;
  }
  
  TGraphErrors* gr = new TGraphErrors(24,xv,yv,xe,ye);
  gr->SetMarkerStyle(24);
  gr->SetMarkerColor(4);
  gr->Draw("P");
  m_graphList.push_back(gr);
  return gr;
}


  
