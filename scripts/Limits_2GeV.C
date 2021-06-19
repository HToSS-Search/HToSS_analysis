 #include "TFile.h"
 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TGraphPainter.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TString.h"
 #include "TLatex.h"
 #include "TPRegexp.h"
 #include "TAttLine.h"

  void Limits_2GeV() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
   
     double x1[3] = {0, 1, 10};
    
     //2 sigma band
     double y3[3] = {0.03863480, 0.04905365, 0.49274297}; 
     double ex3[3] = {0., 0., 0.};
     double ey3[3] = {0.02339717, 0.02965788, 0.27229374};
     auto sigma2 = new TGraphErrors(3, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[3] = {0.03444700,0.04362106,0.41502756};
     double ex4[3] = {0., 0., 0.};
     double ey4[3] = {0.01427847,0.01799699,0.14057155};
     auto sigma1 = new TGraphErrors(3, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->GetXaxis()->SetTitle("c#tau (mm)");
     sigma1->GetYaxis()->SetTitle("Signal strength (r)");
     sigma1->Draw("3 same");
   
     //observed
     double y1[3] = {0.0369502, 0.0479211, 0.477744}; //value
     double ex1[3] = {0., 0., 0.};
     double ey1[3] = {0.00708996, 0.00871479, 0.00552663 }; //error
     auto obs = new TGraphErrors(3, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[3] = {0.0290654, 0.0383966, 0.383998}; //value
     double ex2[3] = {0., 0., 0.};
     double ey2[3] = {0.00549035, 0.000545785, 0.00417906}; //error
     auto exp = new TGraphErrors(3, x1, y2, ex2, ey2);
     exp->SetMarkerColor(1); //black
     exp->SetMarkerStyle(21);
     exp->SetLineStyle(9);
     exp->SetLineColor(1); 
     exp->Draw("lp same");
     
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("obs","Observed","L");
     legenda->SetLineColor(1);
     legenda->SetMarkerStyle(21);
     legenda->SetMarkerColor(1);
     legenda->SetLineStyle(9);
     legenda->SetLineColor(1); 
     legenda=legend1->AddEntry("exp","Expected","L");
     legenda->SetLineColor(3);
     legenda->SetMarkerColor(3);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("sigma1","1#sigma","F");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("sigma2","2#sigma","F");
     legenda->SetLineColor(94);
     legenda->SetMarkerColor(94);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("Limits_2GeV.gif"); // .pdf and other formats work too! 
  }
