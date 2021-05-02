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

  void trigger() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_pt_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonPtL");
     TH1F* h_ptcut_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonCutPtSL;1");
    // TH1F* h_ptdiv_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonCutPtSL;2");
    // TH1F* h_KIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_KIsoSum1");
    // TH1F* h_KIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_KIsoSum2");
    // TH1F* h_MuonIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum1");
    // TH1F* h_MuonIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum2"); 
     
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_pt_ctau10    = (TH1F*)inFile10.Get("h_genParScalarMuonPtL");
     TH1F* h_ptcut_ctau10    = (TH1F*)inFile10.Get("h_genParScalarMuonCutPtSL;1");
   //  TH1F* h_ptdiv_ctau10    = (TH1F*)inFile10.Get("h_genParScalarMuonCutPtSL;2");
    // TH1F* h_KIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum1");
    // TH1F* h_KIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum2");
//TH1F* h_MuonIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum1");
   //  TH1F* h_MuonIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum2");
   
     TFile inFile100("ggH_MS2_ctau100.root"); 
     TH1F* h_pt_ctau100    = (TH1F*)inFile100.Get("h_genParScalarMuonPtL");
     TH1F* h_ptcut_ctau100    = (TH1F*)inFile100.Get("h_genParScalarMuonCutPtSL;1");
    // TH1F* h_ptdiv_ctau100    = (TH1F*)inFile100.Get("h_genParScalarMuonCutPtSL;2");
   //  TH1F* h_KIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum1");
   //  TH1F* h_KIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum2");
   //  TH1F* h_MuonIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum1");
   //  TH1F* h_MuonIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum2");
    
    
   
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     //h_pt_ctau0->SetTitle("Leading pion, relative isolation");
     h_pt_ctau10->GetYaxis()->SetTitle("Events");
     h_pt_ctau10->GetXaxis()->SetRangeUser(0.0,100); 
     h_pt_ctau10->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_pt_ctau10->SetMarkerColor(4);
     h_pt_ctau10->SetLineColor(4);
     h_pt_ctau10->Draw("HIST");
 
     h_ptcut_ctau10->GetYaxis()->SetTitle("Events");
     h_ptcut_ctau10->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptcut_ctau10->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptcut_ctau10->SetMarkerColor(2);
     h_ptcut_ctau10->SetLineColor(2);
     h_ptcut_ctau10->Draw("HIST same");
   
    /* h_PIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_QCD->SetMarkerColor(6);
     h_PIsoSum1_QCD->SetLineColor(6);
     h_PIsoSum1_QCD->Draw("HIST same");
   
     h_PIsoSum1_data->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_data->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_data->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_data->SetMarkerColor(84);
     h_PIsoSum1_data->SetLineColor(84);
     h_PIsoSum1_data->Draw("HIST same");*/
   
    /* TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_pt_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptcut_ctau0","c_{#tau}=10mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_data","Data","L");
     legenda->SetLineColor(84);
     legenda->SetMarkerColor(84);
     legenda->SetMarkerSize(2);
     legend1->Draw();*/
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("trigger.gif"); // .pdf and other formats work too! 
   
  }
