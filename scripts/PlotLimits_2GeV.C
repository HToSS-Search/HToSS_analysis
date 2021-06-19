#include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"

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
   
     double x1[] = {0, 1, 10};
     double y1[] = {0.0369502, 0.0479211, 0.477744}; //value
     double ex1[] = {0., 0., 0.};
     double ey1[] = {0.00708996, 0.00871479, 0.00552663 }; //error
     auto obs = new TGraphErrors(3, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->Draw("ALP");
   
    
     double y2[] = {0.0290654, 0.0383966, 0.383998}; //value
     double ex2[] = {0., 0., 0.};
     double ey2[] = {0.00549035, 0.000545785, 0.00417906}; //error
     auto exp = new TGraphErrors(3, x1, y2, ex2, ey2);
     exp->SetMarkerColor(1); //black
     exp->SetMarkerStyle(21);
     exp->Draw("AP");
    
     
     /*double x[] = {0, 1, 2};
     double y[] = {0, 2, 4};
     double ex[] = {0.1, 0.2, 0.3};
     double ey[] = {1, 0.5, 1};
     auto ge = new TGraphErrors(3, x, y, ex, ey);
     up1->SetFillColor(8);
     up1->SetFillStyle(3001);
     up->Draw("A4");
   
     double x[] = {0, 1, 2};
     double y[] = {0, 2, 4};
     double ex[] = {0.1, 0.2, 0.3};
     double ey[] = {1, 0.5, 1};
     auto ge = new TGraphErrors(3, x, y, ex, ey);
     ge->SetFillColor(5);
     ge->SetFillStyle(3001);
     ge->Draw("A4");*/
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("PlotLimits_2GeV.gif"); // .pdf and other formats work too! 
  }
