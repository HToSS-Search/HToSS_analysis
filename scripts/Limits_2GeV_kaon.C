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

  void Limits_2GeV_kaon() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {0.033743970,0.039742175,0.333194665,17.4952955}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.024812690,0.029282135,0.195296215,11.0901375};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("Signal strength (r)");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {0.02931587,0.034448515,0.286554515,14.0315325};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.016372220,0.019253525,0.101377005,5.3342775};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.0163244,0.0191141,0.160206,7.63754}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.00335722,0.00393096,0.00310429,0.0549811}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {0.0253023,0.0296163,0.264147,12.8136}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.00671445,0.00786191,0.00189848,0.134102}; //error
     auto exp = new TGraphErrors(4, x1, y2, ex2, ey2);
     exp->SetMarkerColor(1); //black
     exp->SetMarkerStyle(21);
     exp->SetLineStyle(9);
     exp->SetLineColor(1); 
     exp->Draw("lp same");
     
     TLegend *legend1 = new TLegend(0.1,0.7,0.3,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("obs","Observed","LP");
     legenda->SetMarkerStyle(21);
     legenda->SetMarkerColor(4);
     legenda=legend1->AddEntry("exp","Expected","LP");
     legenda->SetMarkerStyle(21);
     legenda->SetMarkerColor(1);
     legenda->SetLineStyle(9);
     legenda=legend1->AddEntry("sigma1","#pm 1#sigma band","F");
     legenda->SetFillColor(8);
     legenda->SetFillStyle(3001);
     legenda->SetMarkerColor(8);
     legenda=legend1->AddEntry("sigma2","#pm 2#sigma band","F");
     legenda->SetFillColor(5);
     legenda->SetFillStyle(3001);
     legenda->SetMarkerColor(5);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("Limits_2GeV_kaon.png"); // .pdf and other formats work too! 
  }
