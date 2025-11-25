#include <iostream>
#include <cmath>
#include <cstdio>
#include "GsimTools/GsimBetaDecay.h"
#include "TApplication.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"


int main(int argc,char** argv)
{
  new TApplication("app",&argc,argv);


  ///////////////////////////////////////////
  
  int Acu=63;
  int Zcu=29;
  double Qplus=0.653;
  double Qminus=0.579;

  int Zplus =+(Zcu-1); // Posivive
  int Zminus=-(Zcu+1); // Negative !!!!
  ///////////////////////////////////////////


  ///////////////////////////////////////////
  GsimBetaDecay bPlus("64CuBetaPlus");
  GsimBetaDecay bMinus("64CuBetaMinus");
  GsimBetaDecay bPlusWO("64CuBetaPlusWOCD");
  bPlusWO.setCoulombDistortion(false);
  GsimBetaDecay bMinusWO("64CuBetaMinusWOCD");
  bMinusWO.setCoulombDistortion(false);
  ///////////////////////////////////////////
  
  
  
  ///////////////////////////////////////////
  bPlus.fillBetaSpectrum(Acu,Zplus,Qplus);
  TH1D* hPlus=0;
  bPlus.cloneHistogram("hPlus",&hPlus);

  bPlusWO.fillBetaSpectrum(Acu,Zplus,Qplus);
  TH1D* hPlusWO=0;
  bPlusWO.cloneHistogram("hPlusWO",&hPlusWO);

  bMinus.fillBetaSpectrum(Acu,Zminus,Qminus);
  TH1D* hMinus=0;
  bMinus.cloneHistogram("hMinus",&hMinus);

  bMinusWO.fillBetaSpectrum(Acu,Zminus,Qminus);
  TH1D* hMinusWO=0;
  bMinusWO.cloneHistogram("hMinusWO",&hMinusWO);
  ///////////////////////////////////////////


  
  //Drawing 
  TH1D* frame = new TH1D("frame","beta spectrum of {}^{64}Cu",10,0,0.7);
  frame->SetMaximum(0.25);
  frame->SetMinimum(0.);
  frame->SetXTitle("Kinetic energy (MeV)");
  frame->SetYTitle("dN/dE (a.u.)");
  frame->SetStats(0);
  frame->Draw("0");
  
  
  hPlus->SetLineColor(kRed);
  hPlus->SetLineWidth(3);
  hPlus->Draw("same");
  
  hPlusWO->SetLineColor(kRed-8);
  hPlusWO->SetLineWidth(3);
  hPlusWO->Draw("same");

  hMinus->SetLineWidth(3);
  hMinus->SetLineColor(kBlue);
  hMinus->Draw("same");

  hMinusWO->SetLineWidth(3);
  hMinusWO->SetLineColor(kBlue-8);
  hMinusWO->Draw("same");


  TLegend* leg = new TLegend(0.55,0.55,0.88,0.88);
  leg->AddEntry(hPlus,"#beta^{+} decay","l");
  leg->AddEntry(hPlusWO,"#beta^{+} decay w/o CD","l");
  leg->AddEntry(hMinus,"#beta^{-} decay","l");
  leg->AddEntry(hMinusWO,"#beta^{-} decay w/o CD","l");
  leg->SetFillStyle(0);
  leg->Draw();
  
  gPad->Modified();
  gPad->Update();
  gPad->Print("betaSpectrum64Cu.pdf");

  return 0;
}
