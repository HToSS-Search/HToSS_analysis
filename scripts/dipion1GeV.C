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

  void dipion1GeV() {  
  
     TFile inFile0("ggH_MS1_ctau0.root"); 
     TH1F* h_antiscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_TestPantiscalarInvMass");
     
     TFile inFile10("ggH_MS1_ctau10.root"); 
     TH1F* h_antiscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_TestPantiscalarInvMass");
 
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);

     //h_refit12InvMass_ctau0->GetYaxis()->SetRangeUser(1.0, 65000.); 
     h_antiscalarInvMass_ctau0->SetTitle("");
     h_antiscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_antiscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0,3);  
     h_antiscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{#pi#pi} (GeV/c^{2})");
     h_antiscalarInvMass_ctau0->SetMarkerColor(4);
     h_antiscalarInvMass_ctau0->SetLineColor(4);
     h_antiscalarInvMass_ctau0->Draw("HIST");
 
     h_antiscalarInvMass_ctau10->SetTitle("");
     h_antiscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_antiscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0,3); 
     h_antiscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{#pi#pi} (GeV/c^{2})");
     h_antiscalarInvMass_ctau10->SetMarkerColor(3);
     h_antiscalarInvMass_ctau10->SetLineColor(3);
     h_antiscalarInvMass_ctau10->Draw("HIST same");
    
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_antiscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_antiscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);

     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_dipion1GeV.pdf"); // .pdf and other formats work too! 
   
  }
