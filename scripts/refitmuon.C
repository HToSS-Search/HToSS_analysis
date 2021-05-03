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

  void refitmuon() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); 
     TH1F* h_refit12InvMass_ctau0    = (TH1F*)inFile0.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_Rrefit12InvMass");

     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_refit12InvMass_ctau10    = (TH1F*)inFile10.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_Rrefit12InvMass");

     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_refit12InvMass_QCD    = (TH1F*)inFileQCD.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_Rrefit12InvMass");
     TH1F* h_P20scalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PhiggsInvMass");
   
     TFile inFileData("dataRunD.root"); 
     TH1F* h_refit12InvMass_data    = (TH1F*)inFileData.Get("h_Rrefit12InvMass");
     TH1F* h_PscalarInvMass_data    = (TH1F*)inFileData.Get("h_Rrefit12InvMass");

   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     Double_t nentries=h_P20scalarInvMass_QCD->GetEntries();
   std::cout<<"entries QCD "<<nentries<<std::endl;
     Double_t n20entries=h_PscalarInvMass_QCD->GetEntries();
    std::cout<<"entries QCD 20 "<<n20entries<<std::endl;
     Double_t corr=nentries/n20entries;
    std::cout<<"correction factor "<<corr<<std::endl;
   
   
     h_PscalarInvMass_ctau0->SetTitle("Dimuon invariant mass");
     //h_refit12InvMass_ctau0->GetYaxis()->SetRangeUser(1.0, 65000.); 
     h_PscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0,4); 
    // h_PscalarInvMass_ctau0->GetYaxis()->SetRangeUser(0.1,1100); 
     h_PscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_PscalarInvMass_ctau0->SetMarkerColor(4);
     h_PscalarInvMass_ctau0->SetLineColor(4);
     h_PscalarInvMass_ctau0->Draw("HIST");
 
     h_PscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0,4); 
     h_PscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_PscalarInvMass_ctau10->SetMarkerColor(3);
     h_PscalarInvMass_ctau10->SetLineColor(3);
     h_PscalarInvMass_ctau10->Draw("HIST same");
   std:cout<<"Not refit "<<h_PscalarInvMass_ctau0->Integral(0,501)<<std::endl;
   
     //h_PscalarInvMass_QCD->Scale(corr);
     h_PscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_QCD->GetXaxis()->SetRangeUser(0,4); 
     h_PscalarInvMass_QCD->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_PscalarInvMass_QCD->SetMarkerColor(6);
     h_PscalarInvMass_QCD->SetLineColor(6);
     h_PscalarInvMass_QCD->Draw("HIST same");
   
     h_PscalarInvMass_data->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_data->GetXaxis()->SetRangeUser(0,4); 
     h_PscalarInvMass_data->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_PscalarInvMass_data->SetMarkerColor(94);
     h_PscalarInvMass_data->SetLineColor(94);
     h_PscalarInvMass_data->Draw("P* same");
   
    /* h_refit12InvMass_ctau0->SetTitle("Dimuon invariant mass without refit");
    h_refit12InvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau0->GetXaxis()->SetRangeUser(0,4); 
     h_refit12InvMass_ctau0->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_refit12InvMass_ctau0->SetMarkerColor(4);
     h_refit12InvMass_ctau0->SetLineColor(4);
     h_refit12InvMass_ctau0->Draw("HIST same");
     
     h_refit12InvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_ctau10->GetXaxis()->SetRangeUser(0,4); 
     h_refit12InvMass_ctau10->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_refit12InvMass_ctau10->SetMarkerColor(3);
     h_refit12InvMass_ctau10->SetLineColor(3);
     h_refit12InvMass_ctau10->Draw("HIST same");
     std::cout<<"Refit "<<h_refit12InvMass_ctau0->Integral(0,501)<<std::endl;

     h_refit12InvMass_QCD->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_QCD->GetXaxis()->SetRangeUser(0,4); 
     h_refit12InvMass_QCD->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_refit12InvMass_QCD->SetMarkerColor(6);
     h_refit12InvMass_QCD->SetLineColor(6);
     h_refit12InvMass_QCD->Draw("HIST same");
     
     h_refit12InvMass_data->GetYaxis()->SetTitle("Events");
     h_refit12InvMass_data->GetXaxis()->SetRangeUser(0,4); 
     h_refit12InvMass_data->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
     h_refit12InvMass_data->SetMarkerColor(94);
     h_refit12InvMass_data->SetLineColor(94);
     h_refit12InvMass_data->Draw("P* same");
 */

    
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_PscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PscalarInvMass_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PscalarInvMass_data","Data","L");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
    /* TLegendEntry *legenda = legend1->AddEntry("h_refit12InvMass_ctau0","Refit c_{#tau}=0mm","L");
     //legenda=legend1->AddEntry("h_refit12InvMass_ctau0","Refit c_{#tau}=0mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_ctau10","Refit c_{#tau}=10mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_QCD","Refit QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_refit12InvMass_data","Refit data","L");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);*/
     
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_refitmuon.gif"); // .pdf and other formats work too! 
   
  }
