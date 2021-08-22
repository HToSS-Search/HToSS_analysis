#include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TObject.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TString.h"
 #include "TLatex.h"
 #include "TPRegexp.h"

  void RrecoHiggs_kaon() {  
  
     //TFile inFile0("ggH_MS2_ctau10.root"); 
     
     
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_higgsInvMass_ctau0    = (TH1F*)inFile10.Get("h_TestKhiggsInvMass");
     TH1F* h_higgsInvMass_ctau10    = (TH1F*)inFile10.Get("h_RTestKhiggsInvMass");
 
     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_higgsInvMass_QCD    = (TH1F*)inFileQCD.Get("h_RTestKhiggsInvMass");
       
     TFile inFileData("dataRunD_2GeV20.root"); 
     TH1F* h_higgsInvMass_data    = (TH1F*)inFileData.Get("h_RTestKhiggsInvMass");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);

     h_higgsInvMass_ctau0->GetYaxis()->SetRangeUser(0.01, 80.);
     h_higgsInvMass_ctau0->SetTitle("");
     h_higgsInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_higgsInvMass_ctau0->GetXaxis()->SetRangeUser(120,130);  
     h_higgsInvMass_ctau0->GetXaxis()->SetTitle("m_{Higgs} (GeV)");
     h_higgsInvMass_ctau0->SetMarkerColor(4);
     h_higgsInvMass_ctau0->SetLineColor(4);
     h_higgsInvMass_ctau0->Draw("HIST");
 
     h_higgsInvMass_ctau10->SetTitle("");
     h_higgsInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_higgsInvMass_ctau10->GetXaxis()->SetRangeUser(120,130); 
     h_higgsInvMass_ctau10->GetXaxis()->SetTitle("m_{Higgs} (GeV)");
     h_higgsInvMass_ctau10->SetMarkerColor(3);
     h_higgsInvMass_ctau10->SetLineColor(3);
     h_higgsInvMass_ctau10->Draw("HIST");
    
     h_higgsInvMass_QCD->SetTitle("");
     h_higgsInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_higgsInvMass_QCD->GetXaxis()->SetRangeUser(120,130); 
     h_higgsInvMass_QCD->GetXaxis()->SetTitle("m_{Higgs} (GeV)");
     h_higgsInvMass_QCD->SetMarkerColor(6);
     h_higgsInvMass_QCD->SetLineColor(6);
     h_higgsInvMass_QCD->Draw("HIST same");
   
     h_higgsInvMass_data->SetTitle("");
     h_higgsInvMass_data->GetYaxis()->SetTitle("Events");
     h_higgsInvMass_data->GetXaxis()->SetRangeUser(120,130); 
     h_higgsInvMass_data->GetXaxis()->SetTitle("m_{Higgs} (GeV)");
     h_higgsInvMass_data->SetMarkerColor(94);
     h_higgsInvMass_data->SetMarkerStyle(21);
     h_higgsInvMass_data->SetMarkerSize(0.25);
     h_higgsInvMass_data->SetLineColor(94);
     h_higgsInvMass_data->Draw("P same");
   
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_higgsInvMass_ctau0","c#tau=10mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_higgsInvMass_ctau10","c#tau=10mm (refitted)","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_higgsInvMass_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_higgsInvMass_data","Observation","P");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);

     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_RrecoHiggs_kaon.pdf"); // .pdf and other formats work too! 
   
  }

