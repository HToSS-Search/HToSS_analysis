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

  void muonctau() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); 
     TH1F* h_refit12InvMass_ctau0    = (TH1F*)inFile0.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PscalarInvMass");
    
     TFile inFile1("ggH_MS2_ctau1.root"); 
     TH1F* h_refit12InvMass_ctau1    = (TH1F*)inFile1.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau1    = (TH1F*)inFile1.Get("h_PscalarInvMass");
     
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_refit12InvMass_ctau10    = (TH1F*)inFile10.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PscalarInvMass");
    
     TFile inFile1000("ggH_MS2_ctau1000.root"); 
     TH1F* h_refit12InvMass_ctau1000    = (TH1F*)inFile1000.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau1000    = (TH1F*)inFile1000.Get("h_PscalarInvMass");
    
     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_refit12InvMass_QCD    = (TH1F*)inFileQCD.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PscalarInvMass");
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_refit12InvMass_ctau0->SetTitle("Refitted dimuon invariant mass");
     h_refit12InvMass_ctau0->GetYaxis()->SetRangeUser(1.0, 10000.); 
     h_refit12InvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau0->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_refit12InvMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_refit12InvMass_ctau0->SetMarkerColor(2);
     h_refit12InvMass_ctau0->SetLineColor(2);
     h_refit12InvMass_ctau0->Draw("HIST");
 
     h_refit12InvMass_ctau1->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau1->GetYaxis()->SetRangeUser(1.0, 10000.); 
     h_refit12InvMass_ctau1->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_refit12InvMass_ctau1->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_refit12InvMass_ctau1->SetMarkerColor(4);
     h_refit12InvMass_ctau1->SetLineColor(4);
     h_refit12InvMass_ctau1->Draw("HIST same");
    
     h_refit12InvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau10->GetYaxis()->SetRangeUser(1.0, 10000.); 
     h_refit12InvMass_ctau10->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_refit12InvMass_ctau10->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_refit12InvMass_ctau10->SetMarkerColor(6);
     h_refit12InvMass_ctau10->SetLineColor(6);
     h_refit12InvMass_ctau10->Draw("HIST same");
    
     h_refit12InvMass_ctau1000->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau1000->GetYaxis()->SetRangeUser(1.0, 10000.); 
     h_refit12InvMass_ctau1000->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_refit12InvMass_ctau1000->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_refit12InvMass_ctau1000->SetMarkerColor(3);
     h_refit12InvMass_ctau1000->SetLineColor(3);
     h_refit12InvMass_ctau1000->Draw("HIST same");
   
     h_refit12InvMass_QCD->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_QCD->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_refit12InvMass_QCD->GetYaxis()->SetRangeUser(1.0, 10000.); 
     h_refit12InvMass_QCD->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_refit12InvMass_QCD->SetMarkerColor(94);
     h_refit12InvMass_QCD->SetLineColor(94);
     h_refit12InvMass_QCD->Draw("HIST same");
     
    /*
     h_PscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_ctau0->SetMarkerColor(3);
     h_PscalarInvMass_ctau0->SetLineColor(3);
     h_PscalarInvMass_ctau0->Draw("HIST same");
 
     h_PscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_ctau10->SetMarkerColor(94);
     h_PscalarInvMass_ctau10->SetLineColor(94);
     h_PscalarInvMass_ctau10->Draw("HIST same");
   
     h_PscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_QCD->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PscalarInvMass_QCD->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_QCD->SetMarkerColor(52);
     h_PscalarInvMass_QCD->SetLineColor(52);
     h_PscalarInvMass_QCD->Draw("HIST same");
   */
    
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_refit12InvMass_ctau0","Refit c_{#tau}=0mm","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_ctau1","Refit c_{#tau}=1mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_ctau10","Refit c_{#tau}=10mm","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_ctau1000","Refit c_{#tau}=1000mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_QCD","Refit QCD","L");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
    
    /* legenda=legend1->AddEntry("h_PscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PscalarInvMass_QCD","QCD","L");
     legenda->SetLineColor(52);
     legenda->SetMarkerColor(52);
     legenda->SetMarkerSize(2);*/
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_comparemuonctau.gif"); // .pdf and other formats work too! 
   
  }
