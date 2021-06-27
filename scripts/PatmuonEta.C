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

  void PATmuonEta() {  
 
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PATmuonEta_ctau10    = (TH1F*)inFile10.Get("h_PATmuonEta");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     //canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     //h_PIsoSum1_ctau0->SetTitle("Leading pion, relative isolation");
     h_PATmuonEta_ctau10->GetYaxis()->SetTitle("Events");
     h_PATmuonEta_ctau10->GetXaxis()->SetRangeUser(-2.5, 2.5); 
     h_PATmuonEta_ctau10->GetXaxis()->SetTitle("#eta");
     h_PATmuonEta_ctau10->SetMarkerColor(4);
     h_PATmuonEta_ctau10->SetLineColor(4);
     h_PATmuonEta_ctau10->Draw("HIST");

     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_PATmuonEta.png"); // .pdf and other formats work too! 
   
    
    
    
