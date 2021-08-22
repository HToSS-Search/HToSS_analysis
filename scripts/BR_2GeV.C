#include "TFile.h"
 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TGraphPainter.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TLine.h"
 #include "TString.h"
 #include "TLatex.h"
 #include "TPRegexp.h"
 #include "TAttLine.h"

  void BR_2GeV() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {0.128075102121937,0.154822413389121,1.29234347854154,70.6057939808727}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.0847675110878661,0.102502001793186,0.756220569097430,46.9052611297071};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->GetYaxis()->SetRangeUser(0.01,100);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {0.0980350217005380,0.127227103825463,1.10764873338314,54.0105576150628};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.0438852688613270,0.0441071230842797,0.370856577734609,21.8694758517633};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.0785599904363419, 0.0823019360430365,0.627634411237298,29.6326025104603}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.0245838741781231,0.0156358710699343,0.00643398242677824,0.573108918111178}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {0.0955449456066946,0.119983392707711,1.01552668260610,49.0332701733413}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.0245838741781231,0.0100060269575613,0.00817967411835027,0.689378445905559}; //error
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
     double y5[4] = {100,100,100,100}; 
     double ex5[4] = {0., 0., 0., 0.};
     double ey5[4] = {99,99,99,99};
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
     canv1->SaveAs("BR_2GeV.pdf"); // .pdf and other formats work too! 
  }
