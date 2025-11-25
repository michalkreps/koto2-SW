#include "TFile.h"
#include "TH1.h"

int main(int ,char** )
{
  
  TFile* tf0 = new TFile("../Pi0GG/av070.root");
  TH1D*  hMGG070 = (TH1D*)tf0->Get("hMGGout");

  TFile* tf1 = new TFile("../Pi0GG/av043.root");
  TH1D*  hMGG043 = (TH1D*)tf1->Get("hMGGout");

  
  
  TFile* tf2 = new TFile("ps.root");
  TH1D*  h1 = (TH1D*)tf2->Get("h1");

  TFile* tfo = new TFile("compare.root","RECREATE");
  
  h1->Draw();
  TH1D* h2  = new TH1D("h2","",200,0,0.4);
  h2->SetXTitle(hMGG070->GetXaxis()->GetTitle());
  h2->SetYTitle(hMGG070->GetYaxis()->GetTitle());
  h2->SetTitle(hMGG070->GetTitle());
  for(int i=0;i<100e4;i++) {
    h2->Fill(hMGG070->GetRandom());
  }
  h2->SetLineColor(4);
  h2->Draw("same");
  h2->Write();

  TH1D* h3  = new TH1D("h3","",200,0,0.4);
  h3->SetXTitle(hMGG043->GetXaxis()->GetTitle());
  h3->SetYTitle(hMGG043->GetYaxis()->GetTitle());
  h3->SetTitle(hMGG043->GetTitle());
  for(int i=0;i<100e4;i++) {
    h3->Fill(hMGG043->GetRandom());
  }
  h3->SetLineColor(2);
  h3->Draw("same");
  h3->Write();

  
  h1->Write();
  tfo->Close();
  return 0;
}
