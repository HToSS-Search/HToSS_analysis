#include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TString.h"
 #include "TLatex.h"
 #include "TPRegexp.h"

  void efficiency_DS() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_ptdiv_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonCutPtDS;2");
   
     TFile inFile1("ggH_MS2_ctau1.root"); 
     TH1F* h_ptdiv_ctau1    = (TH1F*)inFile1.Get("h_genParScalarMuonCutPtDS;2");
   
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_ptdiv_ctau10    = (TH1F*)inFile10.Get("h_genParScalarMuonCutPtDS;2");
   
     TFile inFile100("ggH_MS2_ctau100.root"); 
     TH1F* h_ptdiv_ctau100    = (TH1F*)inFile100.Get("h_genParScalarMuonCutPtDS;2");
   
     TFile inFile1000("ggH_MS2_ctau1000.root"); 
     TH1F* h_ptdiv_ctau1000    = (TH1F*)inFile1000.Get("h_genParScalarMuonCutPtDS;2");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_ptdiv_ctau0->SetTitle("");
     h_ptdiv_ctau0->GetXaxis()->SetRangeUser(0.0,100);
     h_ptdiv_ctau0->GetYaxis()->SetRangeUser(0.0,1);
     h_ptdiv_ctau0->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptdiv_ctau0->SetMarkerColor(4);
     h_ptdiv_ctau0->SetLineColor(4);
     h_ptdiv_ctau0->Draw("HIST");
   
     h_ptdiv_ctau1->SetTitle("");
     h_ptdiv_ctau1->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptdiv_ctau1->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptdiv_ctau1->SetMarkerColor(2);
     h_ptdiv_ctau1->SetLineColor(2);
     h_ptdiv_ctau1->Draw("HIST same");
 
     h_ptdiv_ctau10->SetTitle("");
     h_ptdiv_ctau10->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptdiv_ctau10->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptdiv_ctau10->SetMarkerColor(3);
     h_ptdiv_ctau10->SetLineColor(3);
     h_ptdiv_ctau10->Draw("HIST same");

     h_ptdiv_ctau100->SetTitle("");
     h_ptdiv_ctau100->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptdiv_ctau100->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptdiv_ctau100->SetMarkerColor(6);
     h_ptdiv_ctau100->SetLineColor(6);
     h_ptdiv_ctau100->Draw("HIST same");

     h_ptdiv_ctau1000->SetTitle("");
     h_ptdiv_ctau1000->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptdiv_ctau1000->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptdiv_ctau1000->SetMarkerColor(94);
     h_ptdiv_ctau1000->SetLineColor(94);
     h_ptdiv_ctau1000->Draw("HIST same");
    
   
     TLegend *legend1 = new TLegend(.1,.7,.3,.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_ptdiv_ctau0","c_{#tau}=0mm","P*");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptdiv_ctau1","c_{#tau}=1mm","P*");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptdiv_ctau10","c_{#tau}=10mm","P*");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptdiv_ctau100","c_{#tau}=100mm","P*");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptdiv_ctau1000","c_{#tau}=1000mm","P*");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("efficiency_DS.png"); // .pdf and other formats work too! 
   
  }
