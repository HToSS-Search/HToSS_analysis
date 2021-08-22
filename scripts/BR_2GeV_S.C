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

  void BR_2GeV_S() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {1.44339067328299,1.74482958669252,14.5645523032963,795.718627768140}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.955319440504133,1.15518497295642,8.52251294979079,528.616532962547};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,0.1);
     sigma2->GetYaxis()->SetRangeUser(10,1500);
     //sigma2->GetYaxis()->SetRangeUser(-10,195585);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {1.10484265585774,1.43383383661598,12.4830652058884,608.692351821615};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.494581590953158,0.497081860802123,4.17950808985611,246.466307276253};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     //sigma1->GetYaxis()->SetRangeUser(0,195585);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.885361445045413,0.927532712521686,7.07336274109603,333.955791407286}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.277057243086029,0.176214346872130,0.0725101918563119,6.45886712929891}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     //obs->GetYaxis()->SetRangeUser(0,195585);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {1.07677980406164,1.35219810184713,11.4448610062251,552.598933564649}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.277057243086029,0.112766695070926,0.0921839228492703,7.76921042963568}; //error
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
     double y5[4] = {750,750,750,750}; 
     double ex5[4] = {0., 0., 0., 0.};
     double ey5[4] = {749,749,749,749};
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
     canv1->SaveAs("BR_2GeV_S.pdf"); // .pdf and other formats work too! 
  }
