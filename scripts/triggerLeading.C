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

  void triggerLeading() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_pt_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonPtL");
     TH1F* h_ptcut_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonCutPtSL;1");
     TH1F* h_ptcut2_ctau0    = (TH1F*)inFile0.Get("h_genParScalarMuonCutPtDL;1");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_pt_ctau0->GetYaxis()->SetTitle("Events");
     h_pt_ctau0->GetXaxis()->SetRangeUser(0.0,100); 
     h_pt_ctau0->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_pt_ctau0->SetMarkerColor(4);
     h_pt_ctau0->SetLineColor(4);
     h_pt_ctau0->Draw("HIST");
 
     h_ptcut_ctau0->GetYaxis()->SetTitle("Events");
     h_ptcut_ctau0->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptcut_ctau0->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptcut_ctau0->SetMarkerColor(2);
     h_ptcut_ctau0->SetLineColor(2);
     h_ptcut_ctau0->Draw("HIST same");
   
     h_ptcut2_ctau0->GetYaxis()->SetTitle("Events");
     h_ptcut2_ctau0->GetXaxis()->SetRangeUser(0.0,100); 
     h_ptcut2_ctau0->GetXaxis()->SetTitle("p_{T} (GeV)");
     h_ptcut2_ctau0->SetMarkerColor(3);
     h_ptcut2_ctau0->SetLineColor(3);
     h_ptcut2_ctau0->Draw("HIST same");
   
    
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_pt_ctau10","Leading p_T distribution","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptcut_ctau10","Leading, single muon trigger","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_ptcut2_ctau10","Leading, double muon trigger","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("triggerLeading.png"); // .pdf and other formats work too! 
   
  }
