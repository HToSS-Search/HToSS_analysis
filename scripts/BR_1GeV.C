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
     double y3[4] = {66.2224547209480,104.386952389144,1522.94815240826,119845.717897554}; 
     double ex3[4] = {0., 0., 0., 0.};
     double ey3[4] = {37.6225246062691,58.7911744074924,852.905836123853,77549.7121788991};
     auto sigma2 = new TGraphErrors(4, x1, y3, ex3, ey3);
     sigma2->SetFillColor(5);
     sigma2->SetFillStyle(3001);
     sigma2->GetXaxis()->SetTitle("c#tau (mm)");
     sigma2->GetYaxis()->SetTitle("BR(H#rightarrow a#bar{a})");
     sigma2->GetXaxis()->SetRangeUser(0,100);
     sigma2->SetTitle(" ");
     sigma2->Draw("a3");
     
     //1 sigma band
     double y4[4] = {55.7536577068043,87.4258194839450,1266.57693799694,87396.7335626911};
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {19.0196754667431,29.4474412652905,431.569893042814,34925.6766437309};
     auto sigma1 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma1->SetFillColor(8);
     sigma1->SetFillStyle(3001);
     sigma1->Draw("3 same");
   
     //observed
     double y1[4] = {64.7519862385321,103.666584862385,1480.34908256881,99562.8524464832}; //value
     double ex1[4] = {0., 0., 0., 0.};
     double ey1[4] = {0.513143700305810,1.04285993119266,13.8231886850153,1025.93921253823}; //error
     auto obs = new TGraphErrors(4, x1, y1, ex1, ey1);
     obs->SetMarkerColor(4); //blue
     obs->SetMarkerStyle(21); //square
     obs->SetLineColor(4);
     obs->Draw("lp same");
   
     //expected (median)
     double y2[4] = {51.3826637614679,79.8554074923547,1167.30967889908,72568.5030581040}; //value
     double ex2[4] = {0., 0., 0., 0.};
     double ey2[4] = {0.670701857798165,0.890384097859327,12.4661714831804,1207.19248470948}; //error
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
     double y4[4] = {58897,58897,58897,58897}; 
     double ex4[4] = {0., 0., 0., 0.};
     double ey4[4] = {58896,58896,58896,58896};
     auto sigma3 = new TGraphErrors(4, x1, y4, ex4, ey4);
     sigma3->SetFillColor(14);
     sigma3->SetFillStyle(3244);
     sigma3->SetTitle(" ");
     sigma3->Draw("3 same");
   
     TLine *l2=new TLine(0,1,100,1);
     l2->SetLineColor(14);
     l2->SetLineStyle(1);
     l2->SetLineWidth(0.5);
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
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("BR_1GeV.png"); // .pdf and other formats work too! 
  }
