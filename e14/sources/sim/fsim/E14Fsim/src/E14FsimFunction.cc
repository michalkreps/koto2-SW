#include "E14Fsim/E14FsimFunction.h"
#include "E14Fsim/E14FsimInefficiency.h"
#include "TMath.h"
#include <iostream>
#include <cmath>


E14FsimFunction* E14FsimFunction::s_function=0;

E14FsimFunction::E14FsimFunction()
{
  if(s_function) {
    std::cerr << "E14FsimFunction is constructed twice." << std::endl;
    return;
  }
  m_CVineffPar[0][0]=9.52657e-06;
  m_CVineffPar[1][0]=3.17552e-06;
  m_CVineffPar[0][1]=0.00190467;
  m_CVineffPar[1][1]=0.000634889;
  m_CVineffPar[0][2]=-0.0356322;
  m_CVineffPar[1][2]=-0.0356322;
  m_CVineffPar[0][3]=0.000343819;
  m_CVineffPar[1][3]=0.000114606;
  m_CVineffPar[0][4]=299.184;
  m_CVineffPar[1][4]=299.184;
  m_CVineffPar[0][5]=-51.0998;
  m_CVineffPar[1][5]=-51.0998;



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
  s_function=this;

  
  m_fusionMode=0;
  m_ineff=0;
  m_BHPVConfig=0;
  m_BHPVthreshold=-1;//initialize not to be realistic
  for(int l=0;l<7;l++) {
    m_BHPVLnInefficiency[l]=0;
  }
}


E14FsimFunction::~E14FsimFunction()
{
  ;
}


E14FsimFunction* E14FsimFunction::getFunction()
{
  if(!s_function) {
    s_function  = new E14FsimFunction();
  }
  return s_function;
}



double E14FsimFunction::getCSIWeight(double Mome , int PID)
{
  double Weight = 1;

  if (PID == 22) {
    Weight *= csiineff(Mome/1000);
  } else if (PID == 11 || PID==-11 || PID == 13 || PID == -13 ) {
    Weight *= TMath::Min(6*CVineff(Mome,PID) , 1.0); // KOPIO model x 6
  } else if (PID == 211  || PID == -211 ){
    Weight *= newCVineffPi(Mome,PID);
  }
  return (Weight);
}

double E14FsimFunction::getBARWeight(double Mome , int PID , double theta,int VetoID, double z)
{
  double Weight = 1;

  if (PID == 22){
    if(VetoID==7 && m_ineff) {
      Weight *= m_ineff->getWeight(theta,Mome,z);
    } else {
      Weight *= mbineff(Mome/1000.,theta);
    }
	    
  } else if (PID == 11 || PID==-11 || PID == 13 || PID == -13 ){
    Weight *= TMath::Min(6*CVineff(Mome,PID) , 1.0); // KOPIO model x 6
  } else if (PID == 211  || PID == -211 ){
    Weight *= newCVineffPi(Mome,PID);
  }
  return (Weight);
}




double E14FsimFunction::CVineff(double p , int PID)
{
  //double DeadThick = 6.7;  // mg/cm^2
  //double EthresCV = 0.05;  // MeV
  bool UsePVboost=true;

  //veto inefficiency for e,mu
  // Same as KOPIO Fast MC

  //c -- local
  
  double Ekin=0;
  double pvboost=1;
  double ineff=1;

  //c -- 25jan05 for pv_ineff
  //  double angle;
  //  int ierr;

  //c -- assumed minimum cv ineffy due to veto blindness
  double cvmin = 1.e-5;

  //c parameters for pi+ ineffy
  //c 12jul04 reduce by X4
  //      logical FIRST/.TRUE./
  //double a,b;
  //c      double e1/185./,e2/360./,ineff1/1.e-4/,ineff2/2.e-5/
  //double e1 = 185.;
  //double e2 = 360.;
  //double ineff1 = 0.25e-4;
  //double ineff2 = 0.5e-5 ;

  double emass = 0.51099892;
  double mumass = 105.6538369;
  
  //c ------------------------------------------
  //b = (log(ineff2)-log(ineff1))/(e2-e1);
  //a = -b*e2+log(ineff2);

  //c -- species-dependent CV inefficiencies
  if (13 == PID ) {//! mu-
    ineff = cvmin;
  }
  else if (11 == PID ){ // then    ! e-
    ineff = cvmin;
  }
  else if (-11 == PID ){// then     ! e+
    ineff = 0.05/sqrt(p*p+emass * emass)/6;
  }
  else if (-13 == PID) {//then   ! mu+
    ineff = cvmin;
  }
  ineff = TMath::Max(cvmin,ineff);

  //c -- assumed improvement in CV ineffy due to detection
  //c -- of particles in PV.
  //c -- 12jul04 Allow PVboost for pi+. But not pi-

  Ekin=0;
  pvboost = 1.;
  if (UsePVboost){// .and. ID.ne.-2) then

    //c -- assume relevant energy scale in PV for charged particles
    //c -- is the kinetic energy

    if (11 == PID || -11 == PID ){ // then ! e
      Ekin = sqrt(p*p+emass*emass)-emass;
    }
    else if (13 == PID || -13 == PID) {//then !mu
      Ekin = sqrt(p*p+mumass*mumass)-mumass;
    }
    
    //c -- cannot have improvement of more than a factor 1/100
    //c -- due to PV. The factor is larger for low kinetic energies.
    if (Ekin > 10.) { //then
      
      //      if (UseTaskForcePV.eq.6){// then
      //c -- 27jan05 pv_ineff is the PV ineffy as a function of angle and energy of the
      //c          incident photon. Since pv_boost is the result of some sort of conversion
      //c          or interaction in the CV, the direction of the photon(s) that strike the
      //c          PV is unknown. So I assume that normal incidence is a good approximation.
      //               angle = 0.
      //               pvboost = pv_ineff(Ekin,angle,ierr)
      //               if (ierr.ne.0) write(LUNout,'(1x,a,i3)')'CVN: WARNING PV_INEFF returns ierr=',ierr
      //            else
      //               pvboost = pvn(Ekin)
      //            endif
      
      pvboost = PVineff(Ekin);
      pvboost = TMath::Max(1.e-2,pvboost);
    }
  }      
  double cvn = ineff * pvboost ;
  
  return (cvn);
}

double E14FsimFunction::PVineff(double Egamma)
{
  // ineff = 1 for Eg<2MeV
  // ineff = 1E-6 for Eg>1GeV
  double ineff;
  if (Egamma <= 2) ineff = 1;
  else if (Egamma >= 1000) ineff = 1e-6;
  else {
    //  Double_T_t a = 10 ** (6 * log10(2) /( 3-log10(2)));
    //  Double_T_t b = 6 / (log10(2) -3);
    double a = 4.66886;
    double b = -2.22307;
    ineff = a * pow(Egamma,b);

  }
  return (ineff) ;
}



double E14FsimFunction::newCVineffPi(double P , int PID)
{

  int iPID;
  if (-211 == PID ) iPID =0;
  else  iPID =1;
  
  double mome = P;
  double ineff_exp =  m_CVineffPar[iPID][0]
    + m_CVineffPar[iPID][1]*exp(m_CVineffPar[iPID][2] * mome);
  
  double A = mome-m_CVineffPar[iPID][4];
  double sigma = m_CVineffPar[iPID][5];
  double ineff_gaus =  m_CVineffPar[iPID][3]*exp(- A*A/(2*sigma*sigma));
  double ineff = ineff_exp + ineff_gaus;
  //  double ineff = ineff_exp;
  
  ineff = TMath::Min(1.0 , ineff);
  return ineff;
}


double E14FsimFunction::csiAngRes(double Egamma)
{
  //[GeV],[rad]
  double sigma = 0.03 / sqrt(Egamma);
  return sigma;
}

double E14FsimFunction::csiEnergyRes(double Egamma)
{
  //GeV
  //1%/E+2%/sqrt(E)
  double sigmE=
    sqrt(0.01*0.01 * Egamma*Egamma + 0.02*0.02 * Egamma );
  
  return sigmE;
}

double E14FsimFunction::csiPosRes(double Egamma)
{
  return 5. / sqrt(Egamma); // mm
}

double E14FsimFunction::fusionProb(double dgg)
{
  if(m_fusionMode==0) {
    return fusionProbKAMI(dgg);
  } else if(m_fusionMode==1) {
    return fusionProbChi2(dgg);
  } else if(m_fusionMode==-1) {
    return 0;
  } else {
    return fusionProbRMS(dgg);
  }
}

double E14FsimFunction::fusionProbKAMI(double dgg)
{
  //[mm]
  
  double prob = 0;
  double ineff[17] = {1.0, 0.8,0.725,0.625,0.550,0.450,0.375,0.300,
                        0.250,0.160,0.125,0.080,0.070,0.040,0.025,
                        0.02,0.0};
  double dist[17] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
                       34, 36, 38, 40, 50};

  int ID;
  if (dgg < 10) {
    prob = 1;
  }
  else if (dgg < 50 ){
    if (dgg < 40 ){
      ID = (int)((dgg-10.)/2.);
    }
    else {
      ID = 15;
    }
    double dist1 = dgg - dist[ID]; 
    double dist2 = dist[ID+1] - dgg; 

    prob = (dist2 * ineff[ID] + dist1 * ineff[ID+1])/ (dist1 + dist2);
  }
  else {
    prob = 0;
  }

  return prob;

}


double E14FsimFunction::fusionProbChi2(double dgg)
{
  //[mm]
  
  double prob = 0;
  double ineff[9] = {
    0.754521,
    0.72814,
    0.66229,
    0.532064,
    0.359221,
    0.215131,
    0.127594,
    0.081468,
    0.071284
  };
  double dist[9] = {5,15,25,35,45,55,65,75,85};

  int ID;
  if (dgg < 5) {
    prob = ineff[0];
  }
  else if (dgg < 85 ){
    ID = (int)((dgg-5.)/10.);
    double dist1 = dgg - dist[ID]; 
    double dist2 = dist[ID+1] - dgg; 
    prob = (dist2 * ineff[ID] + dist1 * ineff[ID+1])/ (dist1 + dist2);
  }
  else {
    prob = 0;
  }

  return prob;

}

double E14FsimFunction::fusionProbRMS(double dgg)
{
  //[mm]
  
  double prob = 0;
  double ineff[10] = {
    0.872083,
    0.867983,
    0.824325,
    0.758552,
    0.642557,
    0.462544,
    0.278677,
    0.145107,
    0.074921,
    0.046386
  };
  double dist[10] = {5,15,25,35,45,55,65,75,85,95};

  int ID;
  if (dgg < 5) {
    prob = ineff[0];
  }
  else if (dgg < 95 ){
    ID = (int)((dgg-5.)/10.);
    double dist1 = dgg - dist[ID]; 
    double dist2 = dist[ID+1] - dgg; 
    prob = (dist2 * ineff[ID] + dist1 * ineff[ID+1])/ (dist1 + dist2);
  }
  else {
    prob = 0;
  }

  return prob;

}


// MB : 5MeV<E<2GeV
// Do not call this function directly
double
E14FsimFunction::func1( double* par, double x )
{
  return  par[0] * exp(par[1] * x) + par[2] * pow(x,par[3]);
}

// MB : 0.5MeV<E<5 MeV
// Do not call this function directly
double
E14FsimFunction::func0( double* par, double x )
{
  double  f = func1(par,0.005);
  double  b = log(f)/(0.005-0.0005);
  double  a = exp(-0.0005*b);
  return  a * exp( b*x );
}

// Inefficiency Function for MB
// usage : double  f = mbineff( energy,theta )
//         energy=[GeV], theta=[degree],90deg=perpendicular to det surface.
double
E14FsimFunction::mbineff( double x, double theta )
{
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

// Inefficiency Function for CsI
// usage : double  f = csiineff( energy )
//         energy=[GeV]
double
E14FsimFunction::csiineff( double x )
{
  double  par[]= {3.5409E-7, -2.27731};
  double  f;
  if ( x > 2.0 ) {
    f= par[0] * pow(2.0,par[1]);
  }
  else{
    f = par[0] * pow(x,par[1]);
    if ( f > 1.0 ) 
      f = 1.0;
  }
  
  return f;
}


double E14FsimFunction::catcherIneff(double Egamma){
  double EgammaGeV = Egamma /1000.; //MeV to GeV
  //  double ineff = 2e-3 + exp(1.13 - 3.67*EgammaGeV);

  double ineff = 1;
  if(m_BHPVConfig==201305) {
    ineff = 4.00801e-02 + exp(4.16028e-01 - 9.73971e+00*EgammaGeV);
  } else if(m_BHPVConfig==2502) {
    //25 module No2
    ineff=catcherIneff2502(Egamma);
  } else {
    //proposal
    ineff = 0.001 + exp(0.5745 - 8.631*EgammaGeV);
  }
  ineff =TMath::Min(1.0,ineff);
  return(ineff);
}


double E14FsimFunction::catcherIneff2502(double Egamma)
{
  //calculate with MeV
  double ene[7]={100, 200, 400, 800, 1000, 2000, 4000};
  double out=0;
  {
    int l0=0;
    int l1=0;
    for(int l=0;l<7;l++) {
      if(ene[l]>=Egamma) {
	l1=l;
	if(ene[l]==Egamma || l==0) {
	  l0=l1;
	  out=TMath::Exp(m_BHPVLnInefficiency[l0]);
	} else {
	  l0=l-1;
	  double d0=Egamma-ene[l0];
	  double d1=ene[l1]-Egamma;
	  double lnineff=
	    (d1*m_BHPVLnInefficiency[l0]+d0*m_BHPVLnInefficiency[l1])/(d0+d1);
	  out = TMath::Exp(lnineff);
	}
	break;
      }
    }
  }
  return out;
}

void E14FsimFunction::setBHPVConfiguration(int config, int threshold)
{
  m_BHPVConfig=config;

  if(m_BHPVConfig==2502) {
    int th[8];
    for(int k=0;k<8;k++) {
      th[k]=3*(k+1);
    }
    
    
    double ineff[7][8]=
      {
	{0.65703, 0.91071, 0.98836, 0.99895, 0.99989, 0.99999, 1, 1},
	{0.24616, 0.59093, 0.86618, 0.97167, 0.99589, 0.99951, 0.99994, 1},
	{0.03769, 0.19378, 0.50451, 0.78512, 0.93132, 0.98141, 0.99568, 0.99886},
	{0.00298, 0.01704, 0.09839, 0.29488, 0.53789, 0.73398, 0.86221, 0.9332},
	{0.00174, 0.00588, 0.03822, 0.14793, 0.33757, 0.53837, 0.70752, 0.8303},
	{0.00146, 0.00158, 0.00211, 0.00468, 0.01465, 0.04062, 0.09145, 0.17225},
	{0.00085, 0.00093, 0.00104, 0.00116, 0.00136, 0.00159, 0.00202, 0.00303}
      };

    if(threshold<3) {
      std::cerr << "No table for BHPV threshold less than 3 p.e." << std::endl;
      return;
    }

    
    if(threshold!=m_BHPVthreshold) {
      m_BHPVthreshold=threshold;
      //updae table
      int k0=0;
      int k1=0;
      bool isMatch=false;
      for(int k=0;k<8;k++) {
	if(th[k]>=threshold) {
	  k1=k;
	  if(threshold==th[k] || k==0) {
	    k0=k1;
	    for(int l=0;l<7;l++) {
	      m_BHPVLnInefficiency[l]=TMath::Log(ineff[l][k0]);
	    }
	  } else {
	    k0=k-1;
	    for(int l=0;l<7;l++) {
	      double lnineff0=TMath::Log(ineff[l][k0]);
	      double lnineff1=TMath::Log(ineff[l][k1]);
	      double d0=threshold-th[k0];
	      double d1=th[k1]-threshold;
	      m_BHPVLnInefficiency[l]=
		(d1*lnineff0+d0*lnineff1)/(d0+d1);
	    }
	  }
	  isMatch=true;
	  break;
	}
      }
      if(!isMatch) {
	//Now threshold is larger tahn the table
	std::cerr << "No table for BHPV threshold larger than 24 p.e." << std::endl;
	std::cerr << "Set inefficiency to be 1" << std::endl;
	for(int l=0;l<7;l++) {
	  m_BHPVLnInefficiency[l]=TMath::Log(1.);
	}
      }
    }
  }
  
  
}
