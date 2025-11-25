#include "E14Fsim/E14FsimIBInefficiency.h"
#include "E14Fsim/E14FsimMBProposalInefficiency.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include <cstdio>
#include <iostream>

int main(int ,char** )
{
  
  Int_t fon=22;
  
  gROOT->SetStyle("Plain");
  //gStyle->SetOptTitle(0);
  gStyle->SetOptTitle(1);
  //gStyle->SetOptStat(0000000);
  gStyle->SetOptStat(1112210);
  //gStyle->SetOptFit(0000);
  gStyle->SetOptFit(1111);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.3);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetTitleXOffset(1.5);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetHistLineWidth(3);
  gStyle->SetTitleYOffset(1.5);
  gStyle->SetTitleFont(fon);
  gStyle->SetTitleFont(fon,"Y");
  gStyle->SetTitleFont(fon,"Z");
  gStyle->SetLabelFont(fon);
  gStyle->SetLabelFont(fon,"Y");
  gStyle->SetLabelFont(fon,"Z");
  gStyle->SetTextFont(fon);
  gStyle->SetStatFont(fon);
  gStyle->SetStatFontSize(0.03);
  //gStyle->SetPadTickX(1);
  //gStyle->SetPadTickY(1);
  //gStyle->SetStatW(0.28);
  //gStyle->SetStatH(0.3);
  gStyle->SetStatW(0.18);
  gStyle->SetPalette(1);

  bool isFirst = true;
  double vTheta[5]={85,65,45,25,5};
  char tit[256];
  E14FsimMBProposalInefficiency mb;
  E14FsimIBInefficiency ib;
  TGraphErrors* grES171=0;
  for(int i=0;i<5;i++) {

    double theta = vTheta[i];
    mb.drawFrame();
    TGraph* grP = mb.draw(theta);
    if(i==0) {
      grES171 = mb.drawES171();
    }
    grES171->Draw("P");

    
    

    TGraph* gr0 = ib.draw(0.5,theta);
    gr0->SetLineWidth(2);

    
    TGraph* gr1 =ib.draw(1.,theta);
    gr1->SetLineWidth(2);
    gr1->SetLineColor(2);
    
    TGraph* gr2 = ib.draw(2.,theta);
    gr2->SetLineColor(3);
    gr2->SetLineWidth(2);
    
    
    TGraph* gr3 = ib.draw(3.,theta);
    gr3->SetLineColor(4);
    gr3->SetLineWidth(2);

    TLegend leg(0.2,0.2,0.5,0.5);
    std::sprintf(tit,"Proposal (#theta = %d^{#circ})",int(vTheta[i]));
    (leg.AddEntry(grP,tit,"P"))->SetTextFont(22);

    if(i==0) {
      std::sprintf(tit,"ES171 (Evis_{th}=0.3 MeV)");
      (leg.AddEntry(grES171,tit,"P"))->SetTextFont(22);
    }

    std::sprintf(tit,"KOTO IB (E_{th} = 0.5 MeV)");
    (leg.AddEntry(gr0,tit,"L"))->SetTextFont(22);

    std::sprintf(tit,"KOTO IB (E_{th} = 1 MeV)");
    (leg.AddEntry(gr1,tit,"L"))->SetTextFont(22);

    std::sprintf(tit,"KOTO IB (E_{th} = 2 MeV)");
    (leg.AddEntry(gr2,tit,"L"))->SetTextFont(22);

    std::sprintf(tit,"KOTO IB (E_{th} = 3 MeV)");
    (leg.AddEntry(gr3,tit,"L"))->SetTextFont(22);
    
    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.Draw();
    gPad->SetGrid(0,0);
    gPad->SetLogx();
    gPad->SetLogy();
    if(isFirst) {
      gPad->Print("drawIBIneffi.pdf[");
      isFirst=false;
    }
    gPad->Print("drawIBIneffi.pdf");
  }
  gPad->Print("drawIBIneffi.pdf]");
  
  
  
  return 0;
}
