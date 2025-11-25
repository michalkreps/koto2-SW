//
{
  TFile* tf= new TFile("compare.root");
  TH1D* h1 = (TH1D*)tf->Get("h1");
  TH1D* h2 = (TH1D*)tf->Get("h2");
  TH1D* h3 = (TH1D*)tf->Get("h3");

  
  h1->Scale(1./h1->Integral());
  h2->Scale(1./h2->Integral());
  h3->Scale(1./h3->Integral());

  h3->SetTitle("m_{#gamma#gamma} distribution for K_{L}#rightarrow#pi^{0}#gamma#gamma");
  h3->SetYTitle("Arbitrary (area normalized)");
  
  h1->SetStats(0);
  h2->SetStats(0);
  h3->SetStats(0);
  h1->SetLineWidth(3);
  h2->SetLineWidth(3);
  h3->SetLineWidth(3);
  h3->Draw();
  h2->Draw("same");
  h1->Draw("same");

  
  TLegend* leg = new TLegend(0.2,0.6,0.55,0.85);
  (leg->AddEntry(h3,"6thOrder#chiPT,av=0.43","l"));
  (leg->AddEntry(h2,"6thOrder#chiPT,av=0.7","l"));
  (leg->AddEntry(h1,"PhaseSpace","l"));
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();
  gPad->Modified();
  gPad->Update();
  h3->GetYaxis()->SetTitleOffset(1.5);
  h3->GetXaxis()->SetTitleOffset(1.5);
  gPad->Print("mgg.pdf");
}
