//Modified macro for manipulating plots

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

  void signalQCDMacro() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_PIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum1");
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum1");
     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_PIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum1");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetFillColor(0);
   
     h_PIsoSum1_ctau0->SetTitle("Relative Isolation");
     h_PIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_ctau0->SetMarkerColor(2);
     h_PIsoSum1_ctau0->Draw("HIST");
 
     h_PIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_ctau10->SetMarkerColor(4);
     h_PIsoSum1_ctau10->Draw("HIST same");
   
     h_PIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_QCD->SetMarkerColor(6);
     h_PIsoSum1_QCD->Draw("HIST same");
   
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_PIsoSum1_ctau0","c_{#tau}=0","L");
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_ctau10","c_{#tau}=10","L");
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_QCD","QCD","L");
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_PIsoSum1.gif"); // .pdf and other formats work too! 
     }
