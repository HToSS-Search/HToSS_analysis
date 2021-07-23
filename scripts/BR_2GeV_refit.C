#include "TFile.h"
 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TGraphPainter.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TString.h"
 #include "TLine.h"
 #include "TLatex.h"
 #include "TPRegexp.h"
 #include "TAttLine.h"

  void BR_2GeV_refit() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {0.0999721606694561,0.107075923849372,0.609330364016737,43.4180723849372}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.0616906528870293,0.0598650726359833,0.346372169874477,27.6588280334728};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {0.0753267205020921,0.0924090853556486,0.509922434309624,34.4793278661088};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.0302175707112971,0.0370679841004184,0.174092397489540,13.7376210878661};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.0557526025104603,0.0683938577405858,0.350834644351464,22.9466443514644}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.0115223481171548,0.0141348987447699,0.0107174610878661,0.358121338912134}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {0.0715835815899582,0.0878705774058578,0.474831799163180,30.9603514644352}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.0124326326359833,0.0152392803347280,0.00869075481171548,0.276297907949791}; //error
     auto exp = new TGraphErrors(4, x1, y2, ex2, ey2);
     exp->SetMarkerColor(1); //black
     exp->SetMarkerStyle(21);
     exp->SetLineStyle(9);
     exp->SetLineColor(1); 
     exp->Draw("lp same");
   
     TLine *l=new TLine(0,0.34,100,0.34);
     l->SetLineColor(2);
     l->SetLineStyle(1);
     l->SetLineWidth(2);
     l->Draw("same");
     
     //unphysical area
     double y5[4] = {60.5,60.5,60.5,60.5}; 
     double ex5[4] = {0., 0., 0., 0.};
     double ey5[4] = {59.5,59.5,59.5,59.5};
     auto sigma3 = new TGraphErrors(4, x1, y5, ex5, ey5);
     sigma3->SetFillColor(14);
     sigma3->SetFillStyle(3244);
     sigma3->SetTitle(" ");
     //sigma3->GetYaxis()->SetRangeUser(0,120);
     sigma3->Draw("3 same");
   
     TLine *l2=new TLine(0,1,100,1);
     l2->SetLineColor(14);
     l2->SetLineStyle(1);
     l2->SetLineWidth(1);
     //l2->GetYaxis()->SetRangeUser(0,195585);
     l2->Draw("same");
   
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
     legenda=legend1->AddEntry("l","Constraint BSM","L");
     legenda->SetLineWidth(2);
     legenda->SetLineColor(2);
     legenda=legend1->AddEntry("l2","Unphysical area","F");
     legenda->SetFillColor(14);
     legenda->SetFillStyle(3244);
     legenda->SetMarkerColor(14);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("BR_2GeV_refit.pdf"); // .pdf and other formats work too! 
  }
