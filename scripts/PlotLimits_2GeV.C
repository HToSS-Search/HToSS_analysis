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

  void PlotLimits_2GeV() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     double x1[] = {0, 1, 10, 100};
     double y1[] = {0.477744, 2, 4, 1};
     double ex1[] = {0., 0., 0., 0.};
     double ey1[] = {0.00552663, 0.005, 0.005, 0.005};
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->Draw("ap");
    
     double y2[] = {0.383998, 2, 4, 1};
     double ex2[] = {0., 0., 0., 0.};
     double ey2[] = {0.00417906, 0.005, 0.005, 0.005};
     auto exp = new TGraphErrors(4, x1, y2, ex2, ey2);
     exp->Draw("ap same");
    
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("PlotLimits_2GeV.gif"); // .pdf and other formats work too! 
  }
