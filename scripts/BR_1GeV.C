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

  void BR_1GeV() {  
  
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     canv1->SetGrid();
     canv1->SetLogy(); canv1->SetLogx();
   
     double x1[4] = {0, 1, 10, 100};
    
     //2 sigma band
     double y3[4] = {26.7822997974006,53.4417686162079,1208.32156089450,155857.8482301222}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {15.7736594717125,30.0713417591743,695.076886047401,101062.067824924};
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
     double y4[4] = {21.5034378260703,45.7334484915902,1042.00516001529,108576.577305046};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {7.28251756995413,15.4482276269113,349.321944801223,45447.4284824159};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     //sigma1->GetYaxis()->SetRangeUser(0,195585);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {26.3405499235474,56.7164774464832,1285.10109327217,127411.178899083}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.205374153669725,0.438965590978593,9.10407966360856,1325.57642966361}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     //obs->GetYaxis()->SetRangeUser(0,195585);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {19.6895468654434,43.0254780581040,958.800000000000,93701.0756880734}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.228087622324159,0.469518788990826,10.3821742354740,1167.12642201835}; //error
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
     canv1->SaveAs("BR_1GeV.pdf"); // .pdf and other formats work too! 
  }
