#include "E14Fsim/E14FsimInefficiency.h"
#include <iostream>
#include <cmath>
#include <list>
#include "TGraph.h"

E14FsimInefficiency::E14FsimInefficiency()
{
  m_ineffiDef=0;
  m_thetaDef=0;
  m_momeDef=0;
  m_threDef=0;
  //Call in inherited class
  // initialize(nTheta,nMome,nThreshold,
  // 	     ineffiSingle,thetaTemp,momeTemp,threTemp);
}

void E14FsimInefficiency::initialize(int nTheta,
				     int nMome,
				     int nThreshold,
				     double* ineffiSingle,
				     double* thetaTemp,
				     double* momeTemp,
				     double* threTemp)
{
  m_2ndFilled=false;
  m_nTheta=nTheta;
  m_nMome=nMome;
  m_nThreshold=nThreshold;

  if(m_ineffiDef) delete [] m_ineffiDef;
  m_ineffiDef  = new double[m_nTheta*m_nMome*m_nThreshold];

  if(m_thetaDef) delete [] m_thetaDef;
  m_thetaDef   = new double[m_nTheta];
  
  if(m_momeDef) delete [] m_momeDef;
  m_momeDef    = new double[m_nMome];

  if(m_threDef) delete [] m_threDef;
  m_threDef    = new double[m_nThreshold];

  int cnt=0;
  for(int i=0;i<m_nTheta;i++) {
    for(int j=0;j<m_nMome;j++) {
      for(int k=0;k<m_nThreshold;k++) {
	m_ineffiDef[cnt]=ineffiSingle[cnt];
	cnt++;
      }
    }
  }
  for(int i=0;i<m_nTheta;i++) {
    m_thetaDef[i]=thetaTemp[i];
  }
  for(int i=0;i<m_nMome;i++) {
    m_momeDef[i]=momeTemp[i];
  }
  for(int i=0;i<m_nThreshold;i++) {
    m_threDef[i]=threTemp[i];
  }
  
  for(int i=0;i<100;i++) {
    for(int j=0;j<100;j++) {
      m_ineffi[i][j]=1;
    }
  }
  if(m_nTheta>100 || m_nMome>100) {
    std::cerr << "# of bin exceeds array size" << std::endl;
  }

  //Initialization
  m_threshold=m_threDef[0];
  for(int i=0;i<m_nTheta;i++) {
    for(int j=0;j<m_nMome;j++) {
      m_ineffi[i][j]=m_ineffiDef[getIndex(i,j,0)];
    }
  }

}

E14FsimInefficiency::~E14FsimInefficiency()
{

  for(std::list<TGraph*>::iterator it = m_graphList.begin();
      it!=m_graphList.end();it++) {
    if( (*it) ) delete (*it);
  }

  
  delete [] m_threDef;
  delete [] m_momeDef;
  delete [] m_thetaDef;
  delete [] m_ineffiDef;

}

bool E14FsimInefficiency::setupThreshold(double vth)
{
  if(m_nTheta>100 || m_nMome>100) {
    return false;
  }
  if(m_nThreshold<1) {
    std::cerr << "Size of pre-defined lists is < 1 " 
	      << vth << std::endl;
    return false;
  }

  if(m_nThreshold==1 && m_threDef[0]!=vth) {
    std::cerr << "Size of pre-defined lists is <=1 " 
	      << vth << " "
	      << m_threDef[0]
	      << std::endl;
    return false;
  }

  if(m_nThreshold==1 && m_threDef[0]==vth) {
    for(int i=0;i<m_nTheta;i++) {
      for(int j=0;j<m_nMome;j++) {
	m_ineffi[i][j]=m_ineffiDef[getIndex(i,j,0)];
      }
    }
    return true;
  }

  

  int iMin=0;
  if(vth>m_threDef[m_nThreshold-1])  {
    iMin=m_nThreshold-2;
    std::cerr << "Invalid threshold larger than pre-defined lists " 
	      << vth << std::endl;
    return false;
  }
  
  for(int i=0;i<m_nThreshold;i++) {
    if(m_threDef[i]>vth) {
      iMin=i-1;
      break;
    }
  }

  
  //m_threDef[iMin]       -- vth -- m_threDef[iMin+1]
  //m_ineffiDef[][][iMin]     ?     m_ineffiDef[][][iMin+1]
  
  // iMin==-1
  //  0       -- vth -- m_threDef[iMin+1]
  //  0           ?     m_ineffiDef[][][iMin+1]

  for(int i=0;i<m_nTheta;i++) {
    for(int j=0;j<m_nMome;j++) {
      double x=vth;
      double x0=0;
      if(iMin==-1) {
	//Threshold 0<->m_threDef[0]
	x0=0;
      } else {
	x0=m_threDef[iMin];
      }
      double x1=m_threDef[iMin+1];
      double y0=0;
      if(iMin==-1) {
	//Inefficiency 0<->m_ineffiDef[i][j][0];
	y0=0;
      } else {
	y0=m_ineffiDef[getIndex(i,j,iMin)];
      }
      double y1=m_ineffiDef[getIndex(i,j,iMin+1)];
    
      m_ineffi[i][j]=
	getInterpolate(x,x0,y0,x1,y1);
    }
  }
  
  return true;
}



bool E14FsimInefficiency::setup2ndThreshold(double vth, double z0, double z1)
{
  if(m_nTheta>100 || m_nMome>100) {
    return false;
  }
  if(m_nThreshold<=1) {
    std::cerr << "Size of pre-defined lists is <=1" 
	      << vth << std::endl;
    return false;
  }

  int iMin=0;
  if(vth>m_threDef[m_nThreshold-1])  {
    iMin=m_nThreshold-2;
    std::cerr << "Invalid threshold larger than pre-defined lists " 
	      << vth << std::endl;
    return false;
  }

  m_z0=z0;
  m_z1=z1;
  for(int i=0;i<m_nThreshold;i++) {
    if(m_threDef[i]>vth) {
      iMin=i-1;
      break;
    }
  }

  
  //m_threDef[iMin]       -- vth -- m_threDef[iMin+1]
  //m_ineffiDef[][][iMin]     ?     m_ineffiDef[][][iMin+1]
  
  // iMin==-1
  //  0       -- vth -- m_threDef[iMin+1]
  //  0           ?     m_ineffiDef[][][iMin+1]

  for(int i=0;i<m_nTheta;i++) {
    for(int j=0;j<m_nMome;j++) {
      double x=vth;
      double x0=0;
      if(iMin==-1) {
	//Threshold 0<->m_threDef[0]
	x0=0;
      } else {
	x0=m_threDef[iMin];
      }
      double x1=m_threDef[iMin+1];
      double y0=0;
      if(iMin==-1) {
	//Inefficiency 0<->m_ineffiDef[i][j][0];
	y0=0;
      } else {
	y0=m_ineffiDef[getIndex(i,j,iMin)];
      }
      double y1=m_ineffiDef[getIndex(i,j,iMin+1)];
    
      m_ineffi2[i][j]=
	getInterpolate(x,x0,y0,x1,y1);
    }
  }
  m_2ndFilled=true;
  return true;
}


double E14FsimInefficiency::getWeight(double theta,double Mome)  //[degree],[MeV]
{

  ////////////////////////////////
  //Angle
  int iMin=0;//theta min
  int iMax=0;//theta max
  for(int i=0;i<m_nTheta;i++) {
    if(m_thetaDef[i]>=theta) {
      iMin=i-1;
      iMax=i;
      break;
    }
  }
  
  if(iMin==-1) {
    //Under flow => set minimum
    iMin=0;
    iMax=0;
  }
  
  if(theta>=m_thetaDef[m_nTheta-1]) {
    //Over flow => set maximum
    iMin=m_nTheta-1;
    iMax=m_nTheta-1;
  }

  ////////////////////////////////
  // Mome
  int jMin=0;//Mome minimum
  int jMax=0;//Mome maximum
  for(int j=0;j<m_nMome;j++) {
    if(m_momeDef[j]>=Mome) {
      jMin=j-1;
      jMax=j;
      break;
    }
  }
  if(jMin==-1) {
    //Under flow => ene=0;ineffi=1;
  }
  if(Mome>=m_momeDef[m_nMome-1]) {
    //Over flow => set maximum
    jMin=m_nMome-1;
    jMax=m_nMome-1;
  }
  ////////////////////////////////
  //Interpolate for theta
  double z0=0;
  double z1=0;
  {
    double x=theta;
    double x0=m_thetaDef[iMin];
    double x1=m_thetaDef[iMax];

    double y0=0;
    double y1=0;
    if(jMin==-1) {
      //inefficiency for incident energy 0;
      y0=1;
      y1=1;
    } else {
      y0=m_ineffi[iMin][jMin];
      y1=m_ineffi[iMax][jMin];
    }
    z0=getInterpolate(x,x0,y0,x1,y1);
    
    y0=m_ineffi[iMin][jMax];
    y1=m_ineffi[iMax][jMax];
    z1=getInterpolate(x,x0,y0,x1,y1);
  }
  ////////////////////////////////
  //Interpolate for Mome
  double weight=0;
  {
    double x=Mome;
    double x0=0;
    if(jMin==-1) {
      //incident energy 0
      x0=0;
    } else {
      x0=m_momeDef[jMin];
    }
    double x1=m_momeDef[jMax];
    weight=
      getLogInterpolate(x,x0,z0,x1,z1);
  }
  
  return weight;
}




double E14FsimInefficiency::getWeight2nd(double theta,double Mome)  //[degree],[MeV]
{
  
  
  if(!m_2ndFilled) {
    return 1;
  }
  ////////////////////////////////
  //Angle
  int iMin=0;//theta min
  int iMax=0;//theta max
  for(int i=0;i<m_nTheta;i++) {
    if(m_thetaDef[i]>=theta) {
      iMin=i-1;
      iMax=i;
      break;
    }
  }
  
  if(iMin==-1) {
    //Under flow => set minimum
    iMin=0;
    iMax=0;
  }
  
  if(theta>=m_thetaDef[m_nTheta-1]) {
    //Over flow => set maximum
    iMin=m_nTheta-1;
    iMax=m_nTheta-1;
  }

  ////////////////////////////////
  // Mome
  int jMin=0;//Mome minimum
  int jMax=0;//Mome maximum
  for(int j=0;j<m_nMome;j++) {
    if(m_momeDef[j]>=Mome) {
      jMin=j-1;
      jMax=j;
      break;
    }
  }
  if(jMin==-1) {
    //Under flow => ene=0;ineffi=1;
  }
  if(Mome>=m_momeDef[m_nMome-1]) {
    //Over flow => set maximum
    jMin=m_nMome-1;
    jMax=m_nMome-1;
  }
  ////////////////////////////////
  //Interpolate for theta
  double z0=0;
  double z1=0;
  {
    double x=theta;
    double x0=m_thetaDef[iMin];
    double x1=m_thetaDef[iMax];

    double y0=0;
    double y1=0;
    if(jMin==-1) {
      //inefficiency for incident energy 0;
      y0=1;
      y1=1;
    } else {
      y0=m_ineffi2[iMin][jMin];
      y1=m_ineffi2[iMax][jMin];
    }
    z0=getInterpolate(x,x0,y0,x1,y1);
    
    y0=m_ineffi2[iMin][jMax];
    y1=m_ineffi2[iMax][jMax];
    z1=getInterpolate(x,x0,y0,x1,y1);
  }
  ////////////////////////////////
  //Interpolate for Mome
  double weight=0;
  {
    double x=Mome;
    double x0=0;
    if(jMin==-1) {
      //incident energy 0
      x0=0;
    } else {
      x0=m_momeDef[jMin];
    }
    double x1=m_momeDef[jMax];
    weight=
      getLogInterpolate(x,x0,z0,x1,z1);
  }
  
  return weight;
}


double E14FsimInefficiency::getWeight(double theta,double Mome,double z)  //[degree],[MeV]
{
  double weight=0;
  if(!m_2ndFilled) {
    weight=getWeight(theta,Mome);
  } else {
    if(z>=m_z0 && z<m_z1) {
      weight=getWeight2nd(theta,Mome);
    } else {
      weight=getWeight(theta,Mome);
    }
  }
  
  return weight;
}

//Interpolation
double E14FsimInefficiency::getLogInterpolate(double x,
					      double x1,double y1,
					      double x2,double y2)
{
  if(x1==0) return y1;
  if(x2==x1 && y1==y2) return y1;
  
  double w = ( log(x) - log(x1) ) / ( log(x2) - log(x1) );
  return exp(( log(y1)*(1.-w) + log(y2)* w));
}




//Interpolation
double E14FsimInefficiency::getInterpolate(double x,
					   double x1,double y1,
					   double x2,double y2)
{
  if(x2==x1 && y1==y2) return y1;
  
  double w = ( x - x1 ) / ( x2 - x1 );
  return y1*(1.-w) + y2* w;
}

TGraph* E14FsimInefficiency::draw(int iThreshold,int iTheta) 
{
  if(! (iThreshold>=0 && iThreshold<m_nThreshold) ) {
    std::cerr << "Out of range : " << iThreshold << std::endl;
    return 0;
  }
  if(! (iTheta>=0 && iTheta<m_nTheta) ) {
    std::cerr << "Out of range : " << iTheta << std::endl;
    return 0;
  }
  
  int i=iTheta;
  TGraph* gr = new TGraph();
  for(int j=0;j<m_nMome;j++) {
    gr->SetPoint(j,
		 m_momeDef[j],
		 m_ineffiDef[getIndex(i,j,iThreshold)]
		 );
  }
  gr->SetMarkerColor(1);
  gr->SetLineColor(1);
  gr->SetLineWidth(2);
  gr->Draw("lp");
  m_graphList.push_back(gr);
  
  return gr;
}


TGraph* E14FsimInefficiency::draw(double threshold,double theta) 
{
  if(threshold>m_threDef[m_nThreshold-1]) {
    std::cerr << "Out of range : " << threshold << std::endl;
    return 0;
  }

  double preThreshold=m_threshold;
  setupThreshold(threshold);

  int cnt=0;
  TGraph* gr = new TGraph();
  for(int j=-1;j<m_nMome;j++) {
    if(j==-1) {
      for(int k=0;k<10;k++) {
	double mome=0;
	mome=0+m_momeDef[0]/10*k;
	gr->SetPoint(cnt,mome,getWeight(theta,mome));
	cnt++;
      }
    } else if(j==m_nMome-1) {
      double mome=m_momeDef[j];
      gr->SetPoint(cnt,mome,getWeight(theta,mome));
      cnt++;
    } else {
      for(int k=0;k<10;k++) {
	double mome=0;
	mome=m_momeDef[j]+(m_momeDef[j+1]-m_momeDef[j])/10*k;
	gr->SetPoint(cnt,mome,getWeight(theta,mome));
	cnt++;
      }
    }
  }
  gr->SetMarkerColor(1);
  gr->SetLineColor(1);
  gr->SetLineWidth(2);
  gr->Draw("lp");
  m_graphList.push_back(gr);
  

  setupThreshold(preThreshold);
  return gr;
}




  
