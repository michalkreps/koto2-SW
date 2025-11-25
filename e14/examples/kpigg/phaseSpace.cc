#include "TFile.h"
#include "TLorentzVector.h"
#include "TGenPhaseSpace.h"
#include "TH1.h"

void PhaseSpace() {
// example of use of TGenPhaseSpace  
//Author: Valerio Filippini

  //if (!gROOT->GetClass("TGenPhaseSpace")) gSystem.Load("libPhysics");

   double m_mkl=0.49767;
   double m_mp0=0.1349630;
   TLorentzVector W(0,0,0,  m_mkl);

   //(Momentum, Energy units are Gev/C, GeV)
   Double_t masses[3] = { m_mp0, 0., 0.} ;

   TGenPhaseSpace event;
   event.SetDecay(W, 3, masses);

   TFile* tf = new TFile("ps.root","RECREATE");
   TH1D *h1 = new TH1D("h1","h1", 200,0,0.4);

   for (Int_t n=0;n<1000000;n++) {
      Double_t weight = event.Generate();

      TLorentzVector *pG1    = event.GetDecay(1);
      TLorentzVector *pG2    = event.GetDecay(2);

      TLorentzVector pGG = *pG1+*pG2;

      h1->Fill(pGG.M(),weight);
   }
   h1->Write();
   tf->Close();
}

int main(int ,char** )
{
  PhaseSpace();
  return 0;
}
