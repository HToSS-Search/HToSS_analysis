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

  void BR_2GeV_kaon() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {0.278527308786611,0.335449133054393,1.92885138075314,98.1104065271967}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {0.155560545606695,0.191724292887029,1.17327822594142,63.9194443514644};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {0.235211683682008,0.279156039330544,1.65352702343096,74.4131764853557};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {0.0792761991631799,0.100670297071130,0.564193157322176,29.9478810878661};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {0.203570878661088,0.235088870292887,1.37967799163180,60.1829790794979}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.00636177405857741,0.00365391464435147,0.0183408937238494,0.756464937238494}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {0.222093221757322,0.266376736401674,1.55708083682008,65.9740418410042}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.00193509790794979,0.00236767866108787,0.0171414610878661,1.28863481171548}; //error
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
     legenda->SetFillColor(2);
     legenda->SetFillStyle(1);
     legenda->SetMarkerColor(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("BR_2GeV_kaon.png"); // .pdf and other formats work too! 
  }
