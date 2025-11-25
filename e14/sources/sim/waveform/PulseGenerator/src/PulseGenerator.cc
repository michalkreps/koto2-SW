#include "PulseGenerator/PulseGenerator.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TGraph.h"
#include <iostream>

PulseGenerator::PulseGenerator()
{

  m_tSample= 8;
  m_nSample = 64;
  m_fSeg    = 10;

  m_PEperMeV=9.0;        // Jan.2013 analysis
  m_CNTperMeV=8.0;       // May 2013
  m_sigmaNoiseCount=2;   // Jan and May 2013
  m_baselineCount=500;   // ~400-600
  m_lambdaPMT=14.;       //from Iwai and Lee
  
  m_singleWaveformTemplate = new double[m_nSample*m_tSample*m_fSeg];

  m_outputWaveform      = new double [m_nSample];
  m_outputWaveformCount = new int [m_nSample];
  m_outputWaveformTime  = new int [m_nSample];
  m_outputWaveformClock = new int [m_nSample];
  for(int i=0;i<m_nSample;i++) {
    m_outputWaveformTime[i]=i*m_tSample;
    m_outputWaveformClock[i]=i;
  }

  makeTemplate();
}


PulseGenerator::~PulseGenerator(){
  delete [] m_singleWaveformTemplate;
  delete [] m_outputWaveform;
  delete [] m_outputWaveformCount;
  delete [] m_outputWaveformTime;
  delete [] m_outputWaveformClock;
}

void PulseGenerator::makeTemplate() {

  //Single P.E. waveform PDF
  double maxT=m_nSample*m_tSample;
  m_singleWaveformPDF = new TF1("singleWaveformPDF", agaus, 0.,maxT , 4 );
  m_singleWaveformPDF->SetParameter( 0, 1);        //height
  m_singleWaveformPDF->SetParameter( 1, maxT*0.5); //mean(cetner of the range)
  m_singleWaveformPDF->SetParameter( 2, 0.044321); //a
  m_singleWaveformPDF->SetParameter( 3, 23.71 );   //b
  for( int index = 0; index < m_fSeg*m_tSample*m_nSample; index++){
    m_singleWaveformTemplate[index] = m_singleWaveformPDF->Eval(1./m_fSeg*index );
  }

  
  // un-doped CsI photo-emission PDF
  double pdfPar[5];
  pdfPar[0] = 3.28113;
  pdfPar[1] = 0.541526;
  pdfPar[2] = -0.00582465;
  pdfPar[3] = 4.53179e-05;
  pdfPar[4] = -1.16776e-07;

  double corPar[5];
  corPar[0] = -0.197912;
  corPar[1] = 0.0695265;
  corPar[2] = 0.161864;
  corPar[3] = 0.193783;
  corPar[4] = 0.232639;
  
  m_photoEmissionPDF = new TF1("pePDF", asymmetric, -50., 250., 8 );
  m_photoEmissionPDF->SetParameter( 0, 1.);     //height
  m_photoEmissionPDF->SetParameter( 1, 0 );     //mean
  m_photoEmissionPDF->SetParameter( 2, 0.);     //ped
  for( int i = 0; i< 5; i++){
    m_photoEmissionPDF
      ->SetParameter( 3+i , pdfPar[i]*(1+corPar[i] ));
    // Sigma of Asymmetric gaussian
  }  
  
}

void PulseGenerator::resetWaveform()
{
  for(int i=0;i<m_nSample;i++) {
    double out =0;
    out +=gRandom->Gaus(0.,m_sigmaNoiseCount); // add noise
    out +=m_baselineCount;                     // add bseline
    m_outputWaveform[i]=out;
  }

  
}

int PulseGenerator::generateNumberOfPE(double Energy)
{
  return gRandom->Poisson(Energy*m_PEperMeV);
}

double PulseGenerator::generatePhotonTiming(double Time)
{
  return m_photoEmissionPDF->GetRandom() + Time;
}

double PulseGenerator::generateGainFluctuation()
{
  double gainFluctuation =-1;
  while( gainFluctuation <0) {
    gainFluctuation =
      gRandom->PoissonD( m_lambdaPMT )+
      gRandom->Uniform(-0.5,0.5);
  }
  gainFluctuation /= m_lambdaPMT;
  
  return gainFluctuation;
}

void PulseGenerator::addPhotonWaveform(double photonGain,double photonTime)
{
  double maxT=m_nSample*m_tSample;
  for(int iSample =0;iSample<m_nSample;iSample++) {

    double out =0;
    double outTime = iSample*m_tSample;
    int templateIndex = int( ((outTime-photonTime)+maxT*0.5)*m_fSeg );
    if ( templateIndex>=0 && templateIndex<m_nSample*m_tSample*m_fSeg ){

      out += m_singleWaveformTemplate[templateIndex];
      out /= m_PEperMeV;  // (out : PE -> MeV)
      out *= m_CNTperMeV; // (out : MeV -> Count)
      out *= 80./64.;     // (correction factor from 1p.e. peak to covoluted peak)
      out *= photonGain;  // add gain fluctuation
    }
    
    
    m_outputWaveform[iSample]+=out;
    
  }
}

void PulseGenerator::addWaveform(double Energy, double Time)
{
  int nPE=generateNumberOfPE(Energy);
  for(int iPE=0;iPE<nPE;iPE++) {
    double photonGain=generateGainFluctuation();
    double photonTime=generatePhotonTiming(Time);
    addPhotonWaveform(photonGain,photonTime);
  }
}



double PulseGenerator::agaus(double* x, double* par){
    double t=x[0];
    double height=par[0];
    double mean=par[1];
    double a=par[2];
    double b=par[3];
    double t_fcn=0.;    
    // asynmetric gaussian
    double sigma=a*(t-mean)+b;
    if ( sigma<0 ){ return 0.; }
    t_fcn=height*TMath::Gaus(t,mean,sigma);    
    return t_fcn;
}

double PulseGenerator::asymmetric( double *x, double *par ){
  
  double t      = x[0];
  double height = par[0];
  double mean   = par[1];
  double ped    = par[2];

  double t_par[5];
  for ( int i=0;i<5;i++ ){
    t_par[i]=par[3+i];
  }
  
  // asynmetric gaussian
  double sigma=0.;
  // Sigma is pol4 of (t-mean) 
  for ( int i=0;i<5;i++ ){
    sigma=sigma*(t-mean)+t_par[4-i];
    //sigma=sigma*(t-mean)+par[7-i];
  }
  if ( sigma<0 ){ return ped; }
  return height*TMath::Gaus(t,mean,sigma)+ped;
}



void PulseGenerator::generateWaveform(TGraph** gr,double Energy, double Time, int option)
{
  resetWaveform();
  addWaveform(Energy, Time);

  for(int i=0;i<m_nSample;i++) {
    m_outputWaveformCount[i]=static_cast<int>(m_outputWaveform[i]);
  }
  
  if((*gr)) delete (*gr);
  if(option==0) {
    (*gr) = new TGraph(m_nSample,m_outputWaveformTime,m_outputWaveformCount);
  } else {
    (*gr) = new TGraph(m_nSample,m_outputWaveformClock,m_outputWaveformCount);
  }
}

  
void PulseGenerator::fillADCCounts(int nSample,int* array)
{
  if(array==0) return;
  if(nSample>m_nSample) nSample=m_nSample;

  for(int i=0;i<nSample;i++) {
    m_outputWaveformCount[i]=static_cast<int>(m_outputWaveform[i]);
    array[i]=m_outputWaveformCount[i];
    
  }
}



void PulseGenerator::drawPhotoEmissionPDF()
{
  if(m_photoEmissionPDF) {
    m_photoEmissionPDF->Draw("L");
  }
}
void PulseGenerator::drawSingleWaveformTemplate()
{
  if(m_singleWaveformPDF) {
    m_singleWaveformPDF->Draw("L");
  }
}
