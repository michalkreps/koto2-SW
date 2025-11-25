//// Test Waveform generator.
#include "PulseGenerator/PulseGenerator.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <cstdlib>

int main( int argc, char** argv ){

  //pulseGen Energy Time numberOfEvents outputFile

  if(argc!=5) {
    std::cerr << "Argument Error" << std::endl;
    std::cerr << "pulseGen Energy(MeV) Time(ns) numberOfEvents outputFile" << std::endl;
    return 1;
  }

  Double_t Energy   = std::atof(argv[1]);
  Double_t Time = std::atof(argv[2]);
  Int_t    nEvent = std::atoi(argv[3]);
  char*    ofname = argv[4];
  
  double PEperMeV=9.0; // p.e./MeV  from Jan.2012 analysis
  double CNTperMeV=8.0;// from May 2013
  double sigmaNoise=2; // Jan and May 2013
  double baseLine=500; // cnt
  
  
  PulseGenerator* gen = new PulseGenerator();
  gen->setPEperMeV(PEperMeV);
  gen->setCNTperMeV(CNTperMeV);
  gen->setSigmaNoiseCount(sigmaNoise);
  gen->setBaselineCount(baseLine);
  
  int adcOut[64];
  TFile* tf = new TFile(ofname,"RECREATE");
  TTree* tr = new TTree("tr","pulseGen");
  tr->Branch("adc",adcOut,"adc[64]/I");
  tr->Branch("energy",&Energy,"energy/D");
  
  for( int i = 0; i< nEvent; i++){
    gen->resetWaveform();
    gen->addWaveform(Energy,Time);
    gen->fillADCCounts(64,adcOut);
    tr->Fill();
  }
  tr->Write();
  tf->Close();
  
  delete gen;
  return 0;
}
