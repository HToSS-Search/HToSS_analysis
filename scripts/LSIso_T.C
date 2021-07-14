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

  void LSIso_T() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_PIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum1");
     TH1F* h_PIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum1");

     TH1F* h_MuonIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum2");
     TH1F* h_MuonIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum2");
     
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum1");
     TH1F* h_PIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum1");

     TH1F* h_MuonIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum2");
     TH1F* h_MuonIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum2");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_PIsoSum1_ctau10->SetTitle("");
     h_PIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau10->GetYaxis()->SetRangeUser(0.05, 50.); 
     h_PIsoSum1_ctau10->GetXaxis()->SetTitle("Relative isolation");
     h_PIsoSum1_ctau10->SetMarkerColor(4);
     h_PIsoSum1_ctau10->SetLineColor(4);
     h_PIsoSum1_ctau10->Draw("HIST");
 
     h_PIsoSum2_ctau10->SetTitle("");
     h_PIsoSum2_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum2_ctau10->GetXaxis()->SetTitle("Relative isolation");
     h_PIsoSum2_ctau10->SetMarkerColor(2);
     h_PIsoSum2_ctau10->SetLineColor(2);
     h_PIsoSum2_ctau10->Draw("HIST same");
   
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_PIsoSum1_ctau10","Muon","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum2_ctau10","Hadron","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_LIso.pdf"); // .pdf and other formats work too! 
   
     
     TCanvas* canv3 = new TCanvas ("canv3", "canv3", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv3->SetLogy();
     canv3->SetLineColor(0);
     canv3->SetFillColor(0);

   
     h_MuonIsoSum1_ctau10->SetTitle("");
     h_MuonIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum1_ctau10->GetYaxis()->SetRangeUser(0.05, 50.); 
     h_MuonIsoSum1_ctau10->GetXaxis()->SetTitle("Relative isolation");
     h_MuonIsoSum1_ctau10->SetMarkerColor(4);
     h_MuonIsoSum1_ctau10->SetLineColor(4);
     h_MuonIsoSum1_ctau10->Draw("HIST");
     
     h_MuonIsoSum2_ctau10->SetTitle("");
     h_MuonIsoSum2_ctau10->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum2_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum2_ctau10->GetXaxis()->SetTitle("Relative isolation");
     h_MuonIsoSum2_ctau10->SetMarkerColor(2);
     h_MuonIsoSum2_ctau10->SetLineColor(2);
     h_MuonIsoSum2_ctau10->Draw("HIST same");
   
     TLegend *legend3 = new TLegend(0.7,0.7,0.9,0.9);
     legend3->SetBorderSize(1);
     TLegendEntry *legendc = legend3->AddEntry("h_MuonIsoSum1_ctau10","Muon","L");
     legendc->SetLineColor(4);
     legendc->SetMarkerColor(4);
     legendc->SetMarkerSize(2);
     legendc=legend3->AddEntry("h_MuonIsoSum2_ctau10","Hadron","L");
     legendc->SetLineColor(2);
     legendc->SetMarkerColor(2);
     legendc->SetMarkerSize(2);
     legend3->Draw();
   
     canv3->Modified();
     canv3->cd();
     canv3->SetSelected(canv3);
     canv3->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv3->SaveAs("h_SIso.pdf"); // .pdf and other formats work too! 
    
  }
