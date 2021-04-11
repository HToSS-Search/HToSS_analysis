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

  void refitvsnotpion() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); 
     TH1F* h_Rpionre12InvMass_ctau0    = (TH1F*)inFile0.Get("h_Rpionre12InvMass");
     TH1F* h_refit12InvMass_ctau0    = (TH1F*)inFile0.Get("h_refit12InvMass");
     TH1F* h_PscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PantiscalarInvMass");

     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_Rpionre12InvMass_ctau10    = (TH1F*)inFile10.Get("h_Rpionre12InvMass");
     TH1F* h_refit12InvMass_ctau10    = (TH1F*)inFile10.Get("h_refit12InvMass");
     TH1F* h_PscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PantiscalarInvMass");

     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_Rpionre12InvMass_QCD    = (TH1F*)inFileQCD.Get("h_Rpionre12InvMass");
     TH1F* h_refit12InvMass_QCD    = (TH1F*)inFileQCD.Get("h_refit12InvMass");
     TH1F* h_PscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PantiscalarInvMass");

   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_PantiscalarInvMass_ctau0->SetTitle("Dihadron (pion) invariant mass with(out) refit");
     //h_refit12InvMass_ctau0->GetYaxis()->SetRangeUser(1.0, 65000.); 
     h_PantiscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0,3); 
     h_PantiscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_ctau0->SetMarkerColor(4);
     h_PantiscalarInvMass_ctau0->SetLineColor(4);
     h_PantiscalarInvMass_ctau0->Draw("HIST");
 
     h_PantiscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0,3); 
     h_PantiscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_ctau10->SetMarkerColor(3);
     h_PantiscalarInvMass_ctau10->SetLineColor(3);
     h_PantiscalarInvMass_ctau10->Draw("HIST same");
   
     h_PantiscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_QCD->GetXaxis()->SetRangeUser(0,3); 
     h_PantiscalarInvMass_QCD->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_QCD->SetMarkerColor(94);
     h_PantiscalarInvMass_QCD->SetLineColor(94);
     h_PantiscalarInvMass_QCD->Draw("HIST same");
   
     
     h_Rpionre12InvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_Rpionre12InvMass_ctau0->GetXaxis()->SetRangeUser(0,3); 
     h_Rpionre12InvMass_ctau0->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_Rpionre12InvMass_ctau0->SetMarkerColor(2);
     h_Rpionre12InvMass_ctau0->SetLineColor(2);
     h_Rpionre12InvMass_ctau0->Draw("HIST same");
   
     h_Rpionre12InvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_Rpionre12InvMass_ctau10->GetXaxis()->SetRangeUser(0,3); 
     h_Rpionre12InvMass_ctau10->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_Rpionre12InvMass_ctau10->SetMarkerColor(1);
     h_Rpionre12InvMass_ctau10->SetLineColor(1);
     h_Rpionre12InvMass_ctau10->Draw("HIST same");
   
     h_Rpionre12InvMass_QCD->GetYaxis()->SetTitle("Events");
     h_Rpionre12InvMass_QCD->GetXaxis()->SetRangeUser(0,3); 
     h_Rpionre12InvMass_QCD->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_Rpionre12InvMass_QCD->SetMarkerColor(6);
     h_Rpionre12InvMass_QCD->SetLineColor(6);
     h_Rpionre12InvMass_QCD->Draw("HIST same");
 
     /*h_refit12InvMass_ctau10->GetYaxis()->SetTitle("Events");
     //h_refit12InvMass_ctau10->GetYaxis()->SetRangeUser(1.0, 65000.); 
     h_refit12InvMass_ctau10->GetXaxis()->SetRangeUser(1,3); 
     h_refit12InvMass_ctau10->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_refit12InvMass_ctau10->SetMarkerColor(6);
     h_refit12InvMass_ctau10->SetLineColor(6);
     h_refit12InvMass_ctau10->Draw("HIST same");
   
     h_refit12InvMass_QCD->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_QCD->GetXaxis()->SetRangeUser(1,3); 
    // h_refit12InvMass_QCD->GetYaxis()->SetRangeUser(1.0, 65000.); 
     h_refit12InvMass_QCD->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_refit12InvMass_QCD->SetMarkerColor(84);
     h_refit12InvMass_QCD->SetLineColor(84);
     h_refit12InvMass_QCD->Draw("HIST same");
     */
   
     
    
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_PantiscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PantiscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PantiscalarInvMass_QCD","QCD","L");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_Rpionre12InvMass_ctau0","Refit c_{#tau}=0mm","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_Rpionre12InvMass_ctau10","Refit c_{#tau}=10mm","L");
     legenda->SetLineColor(1);
     legenda->SetMarkerColor(1);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_Rpionre12InvMass_QCD","Refit QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_comparerefitpion.gif"); // .pdf and other formats work too! 
   
  }
