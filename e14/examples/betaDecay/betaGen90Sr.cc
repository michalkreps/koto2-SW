#include "GsimTools/GsimBetaDecay.h"
#include "TFile.h"
#include "TTree.h"

int main(int ,char** )
{
  GsimBetaDecay bSr("90Sr");
  bSr.fillBetaSpectrum(90,-39,0.546);


  GsimBetaDecay bY("90Y");
  bY.fillBetaSpectrum(90,-40,2.282);


  TFile* tf = new TFile("betaGen90Sr.root","RECREATE");
  TTree* tr = new TTree("tr","tr");
  double p=0;
  tr->Branch("p",&p,"p/D");

  for(int i=0;i<10000;i++) {
    p=bSr.generateBetaMomentum();
    tr->Fill();
    p=bY.generateBetaMomentum();
    tr->Fill();
  }
  tr->Write();
  tf->Close();
    
  return 0;
}
