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
   
     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_PIsoSum2_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum1");
     TH1F* h_PIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_MuonIsoSum1");

     TH1F* h_MuonIsoSum2_QCD   = (TH1F*)inFileQCD.Get("h_PIsoSum2");
     TH1F* h_MuonIsoSum1_QCD   = (TH1F*)inFileQCD.Get("h_MuonIsoSum2");
     
     TFile inFileData("dataRunD_2GeV20.root"); 
     TH1F* h_PIsoSum2_data    = (TH1F*)inFileData.Get("h_PIsoSum1");
     TH1F* h_PIsoSum1_data    = (TH1F*)inFileData.Get("h_MuonIsoSum1");

     TH1F* h_MuonIsoSum2_data    = (TH1F*)inFileData.Get("h_PIsoSum2");
     TH1F* h_MuonIsoSum1_data    = (TH1F*)inFileData.Get("h_MuonIsoSum2");
     
   

     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_PIsoSum1_ctau0->SetTitle("");
     h_PIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau0->GetYaxis()->SetRangeUser(0.1, 100); 
     h_PIsoSum1_ctau0->GetXaxis()->SetTitle("Relative isolation");
     h_PIsoSum1_ctau0->SetMarkerColor(4);
     h_PIsoSum1_ctau0->SetLineColor(4);
     h_PIsoSum1_ctau0->Draw("HIST");
 
     h_PIsoSum2_ctau0->SetTitle("");
     h_PIsoSum2_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum2_ctau0->GetXaxis()->SetTitle("Relative isolation");
     h_PIsoSum2_ctau0->SetMarkerColor(2);
     h_PIsoSum2_ctau0->SetLineColor(2);
     h_PIsoSum2_ctau0->Draw("HIST same");
   
     h_PIsoSum1_QCD->SetTitle("");
     h_PIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_QCD->GetXaxis()->SetRangeUser(0,2); 
     //h_PIsoSum1_QCD->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
     h_PIsoSum1_QCD->SetMarkerColor(6);
     h_PIsoSum1_QCD->SetLineColor(6);
     h_PIsoSum1_QCD->Draw("HIST same");
   
     h_PIsoSum2_QCD->SetTitle("");
     h_PIsoSum2_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_QCD->GetXaxis()->SetRangeUser(0,2); 
     //h_PIsoSum1_QCD->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
     h_PIsoSum2_QCD->SetMarkerColor(6);
     h_PIsoSum2_QCD->SetLineColor(6);
     h_PIsoSum2_QCD->Draw("HIST same");
   
     h_PIsoSum1_data->SetTitle("");
     h_PIsoSum1_data->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_data->GetXaxis()->SetRangeUser(0,2); 
     h_PIsoSum1_data->SetMarkerColor(94);
     h_PIsoSum1_data->SetMarkerStyle(21);
     h_PIsoSum1_data->SetMarkerSize(0.25);
     h_PIsoSum1_data->SetLineColor(94);
     h_PIsoSum1_data->Draw("P same");
   
     h_PIsoSum2_data->SetTitle("");
     h_PIsoSum2_data->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_data->GetXaxis()->SetRangeUser(0,2); 
     h_PIsoSum2_data->SetMarkerColor(94);
     h_PIsoSum2_data->SetMarkerStyle(21);
     h_PIsoSum2_data->SetMarkerSize(0.25);
     h_PIsoSum2_data->SetLineColor(94);
     h_PIsoSum2_data->Draw("P same");
   
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
     legenda=legend1->AddEntry("h_PIsoSum1_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum2_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_data","Observation","P");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum2_data","Observation","P");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
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

   
     h_MuonIsoSum1_ctau0->SetTitle("");
     h_MuonIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     //h_MuonIsoSum1_ctau0->GetYaxis()->SetRangeUser(0.1, 1.); 
     h_MuonIsoSum1_ctau0->GetXaxis()->SetTitle("Relative isolation");
     h_MuonIsoSum1_ctau0->SetMarkerColor(4);
     h_MuonIsoSum1_ctau0->SetLineColor(4);
     h_MuonIsoSum1_ctau0->Draw("HIST");
     
     h_MuonIsoSum2_ctau0->SetTitle("");
     h_MuonIsoSum2_ctau0->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum2_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum2_ctau0->GetXaxis()->SetTitle("Relative isolation");
     h_MuonIsoSum2_ctau0->SetMarkerColor(2);
     h_MuonIsoSum2_ctau0->SetLineColor(2);
     h_MuonIsoSum2_ctau0->Draw("HIST same");
   
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
