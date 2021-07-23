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

  void BR_1GeV_refit() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {22.5497402377676,34.8353114541284,481.678768769113,44132.0023459480}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {12.4902844235474,19.5637025328746,268.534089648318,27178.5460714832};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     //sigma2->GetYaxis()->SetRangeUser(-10,195585);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {19.5519001788991,29.9947718421254,405.691312335627,32442.8687473242};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {6.61754951452599,9.97722692469419,133.776768528287,11273.5054659786};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     //sigma1->GetYaxis()->SetRangeUser(0,195585);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {27.5676990825688,42.2401612385321,578.061839449541,46776.8074923548}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.135740814984709,0.213562071100917,4.50117993883792,321.028792813456}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     //obs->GetYaxis()->SetRangeUser(0,195585);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {17.7556980886850,27.3116281345566,374.596000764526,28852.2076452599}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.234282926605505,0.192006175076453,3.45257185015291,289.081620795107}; //error
     auto exp = new TGraphErrors(4, x1, y2, ex2, ey2);
     exp->SetMarkerColor(1); //black
     exp->SetMarkerStyle(21);
     exp->SetLineStyle(9);
     exp->SetLineColor(1);
     //exp->GetYaxis()->SetRangeUser(0,195585);
     exp->Draw("lp same");
   
     TLine *l=new TLine(0,0.34,100,0.34);
     l->SetLineColor(2);
     l->SetLineStyle(1);
     l->SetLineWidth(2);
     //l->GetYaxis()->SetRangeUser(0,195585);
     l->Draw("same");
  
     //unphysical area
     double y5[4] = {97793,97793,97793,97793}; 
     double ex5[4] = {0., 0., 0., 0.};
     double ey5[4] = {97792,97792,97792,97792};
     auto sigma3 = new TGraphErrors(4, x1, y5, ex5, ey5);
     sigma3->SetFillColor(14);
     sigma3->SetFillStyle(3244);
     sigma3->SetTitle(" ");
     //sigma3->GetYaxis()->SetRangeUser(0,195585);
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
     canv1->SaveAs("BR_1GeV_refit.pdf"); // .pdf and other formats work too! 
  }
