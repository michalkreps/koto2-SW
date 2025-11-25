//// Test Waveform generator.
#include "PulseGenerator/PulseGenerator.h"
#include "TGraph.h"
#include "TH1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

int main( int argc, char** argv ){

  //pulseView Energy 
  
  TApplication* app = new TApplication("app",&argc,argv);

  argc = app->Argc();
  argv = app->Argv();

  if( !(
	argc==2 || argc ==3 || argc==4
	)
      ) {
    std::cerr << "Argument Error" << std::endl;
    std::cerr << "pulseView Energy(MeV) [number of waveform] [output ps file]" << std::endl;
    return 1;
  }
  
  Double_t Energy   = std::atof(argv[1]);
  int nWaveform = 100;
  char* ofname=0;
  
  if(argc==3 || argc==4) {
    nWaveform = std::atoi(argv[2]);
  }

  if(argc==4) {
    ofname = argv[3];
  }

  std::string psName;
  std::string datName;
  std::ofstream fo;
  if(ofname) {
    psName = ofname;
    psName+=".ps";
  
    datName = ofname;
    datName+=".dat";

    fo.open(datName.c_str());

    gROOT->SetBatch(true);
  }

  
  Double_t Time = 160; //ns

  double PEperMeV=9.0; // p.e./MeV  from Jan.2012 analysis
  double CNTperMeV=8.0;// from May 2013
  double sigmaNoise=2; // Jan and May 2013
  double baseLine=500; // cnt
  
  PulseGenerator* gen = new PulseGenerator();
  gen->setPEperMeV(PEperMeV);
  gen->setCNTperMeV(CNTperMeV);
  gen->setSigmaNoiseCount(sigmaNoise);
  gen->setBaselineCount(baseLine);

  
  TCanvas* can = new TCanvas("can","",900,900);

  TH1D* his = new TH1D("his","",64,0,64);
  {
    his->SetXTitle("Clock");
    his->SetYTitle("ADC Counts");
    his->SetStats(0);
    //Calculate histogram range
    double n=Energy*PEperMeV;
    double sigma=0;
    if(n>0)sigma=TMath::Sqrt(n)/n;
    double h=Energy*CNTperMeV;
    double hMax=baseLine+TMath::Max(4*sigmaNoise,h*(1+3*sigma));
    double hMin=baseLine-TMath::Max(4*sigmaNoise,h*0.1);
    if(hMin<0) hMin=0;
    his->SetMaximum(hMax);
    his->SetMinimum(hMin);
  }

  {
    TGraph* grWave=0;
    int adcOut[64];

    bool isFirstTime=true;
    for( int i = 0; i< nWaveform; i++){
      gen->generateWaveform(&grWave,Energy,Time,1);
      grWave->SetMarkerStyle(20);

      his->Draw();  
      grWave->Draw("P");
      can->Modified();
      can->Update();
      if(ofname) {
	if(isFirstTime) {
	  std::ostringstream ostr;
	  ostr << psName <<"[";
	  can->Modified();
	  can->Update();
	  can->Print(ostr.str().c_str());
	  isFirstTime=false;
	}
	can->Print(psName.c_str());
      }

      gen->fillADCCounts(64,adcOut);
      if(ofname) {
	for(int k=0;k<64;k++) {
	  fo << adcOut[k] << " ";
	}
	fo << std::endl;
      } else {
	for(int k=0;k<64;k++) {
	  std::cout << adcOut[k] << " ";
	}
	std::cout << std::endl;
      }

      if(!ofname)
	getchar();
    }
    if(grWave) delete grWave;
  }

  if(ofname) {
    std::ostringstream ostr;
    ostr << psName << "]";
    can->Modified();
    can->Update();
    can->Print(ostr.str().c_str());
  }

  delete his;
  delete can;
  delete gen;
  delete app;
  
  return 0;
}
