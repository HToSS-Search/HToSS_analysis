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

  void BR_2GeV_refit_S() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {1.36668633475355,1.68148044581080,9.41380856720073,668.589574854577}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.758614198157975,0.940638816613940,5.33558657107868,414.905433768752};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->GetYaxis()->SetRangeUser(0.10,1500);
     //sigma2->GetYaxis()->SetRangeUser(-10,195585);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {1.15370469614246,1.40300758368201,7.93573728875395,536.569129612716};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.471650278344729,0.577143772578835,2.88593051765486,203.996012850801};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     //sigma1->GetYaxis()->SetRangeUser(0,195585);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.853003525869987,1.01920041841004,5.60760271456271,365.840463312583}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.181203766710889,0.222280178079396,0.0682233845290336,4.80915037248699}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     //obs->GetYaxis()->SetRangeUser(0,195585);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {1.04315021430758,1.26039974487193,7.44789585672007,479.768211041943}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.228514557607919,0.289296056230228,0.0796042555362792,4.03287968160016}; //error
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
     canv1->SaveAs("BR_2GeV_refit_S.pdf"); // .pdf and other formats work too! 
  }
