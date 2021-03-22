//Modified macro for manipulating plots

 #include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TLegendEntry.h"
 #include "TStyle.h"
 #include "TString.h"
 #include "TLatex.h"
 #include "TPRegexp.h"

  void signalQCDMacro() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_PIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum2");
     TH1F* h_PIsoSum3_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum3");
     TH1F* h_PIsoSum4_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum4");
     TH1F* h_PscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PantiscalarInvMass");
     TH1F* h_PhiggsInvMass_ctau0    = (TH1F*)inFile0.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_ctau0    = (TH1F*)inFile0.Get("h_PhiggsDeltaR");
     TFile inFile1("ggH_MS2_ctau1.root"); 
     TH1F* h_PhiggsInvMass_ctau1    = (TH1F*)inFile1.Get("h_PhiggsInvMass");
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum2");
     TH1F* h_PIsoSum3_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum3");
     TH1F* h_PIsoSum4_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum4");
     TH1F* h_PscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PantiscalarInvMass");
     TH1F* h_PhiggsInvMass_ctau10    = (TH1F*)inFile10.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_ctau10    = (TH1F*)inFile10.Get("h_PhiggsDeltaR");
     TFile inFile1000("ggH_MS2_ctau100.root"); 
     TH1F* h_PhiggsInvMass_ctau1000    = (TH1F*)inFile1000.Get("h_PhiggsInvMass");
     TFile inFileQCD("ggH_MC_QCD.root"); 
     TH1F* h_PIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum2");
     TH1F* h_PIsoSum3_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum3");
     TH1F* h_PIsoSum4_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum4");
     TH1F* h_PscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PantiscalarInvMass");
     TH1F* h_PhiggsInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_QCD    = (TH1F*)inFileQCD.Get("h_PhiggsDeltaR");
   
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_PIsoSum1_ctau0->SetTitle("Leading pion, relative Isolation");
     h_PIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_ctau0->SetMarkerColor(2);
     h_PIsoSum1_ctau0->SetLineColor(2);
     h_PIsoSum1_ctau0->Draw("HIST");
 
     h_PIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_ctau10->SetMarkerColor(4);
     h_PIsoSum1_ctau10->SetLineColor(4);
     h_PIsoSum1_ctau10->Draw("HIST same");
   
     h_PIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum1_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum1_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum1_QCD->SetMarkerColor(6);
     h_PIsoSum1_QCD->SetLineColor(6);
     h_PIsoSum1_QCD->Draw("HIST same");
   
     TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
     legend1->SetBorderSize(1);
     TLegendEntry *legenda = legend1->AddEntry("h_PIsoSum1_ctau0","c_{#tau}=0mm","L");
     legenda->SetLineColor(2);
     legenda->SetMarkerColor(2);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_ctau10","c_{#tau}=10mm","L");
     legenda->SetLineColor(4);
     legenda->SetMarkerColor(4);
     legenda->SetMarkerSize(2);
     legenda=legend1->AddEntry("h_PIsoSum1_QCD","QCD","L");
     legenda->SetLineColor(6);
     legenda->SetMarkerColor(6);
     legenda->SetMarkerSize(2);
     legend1->Draw();
   
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);
     canv1->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv1->SaveAs("h_PIsoSum1.gif"); // .pdf and other formats work too! 
   
   
     TCanvas* canv2 = new TCanvas ("canv2", "canv2", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv2->SetLogy();
     canv2->SetLineColor(0);
     canv2->SetFillColor(0);
   
     h_PIsoSum2_ctau0->SetTitle("Subleading pion, relative Isolation");
     h_PIsoSum2_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum2_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum2_ctau0->SetMarkerColor(2);
     h_PIsoSum2_ctau0->SetLineColor(2);
     h_PIsoSum2_ctau0->Draw("HIST");
 
     h_PIsoSum2_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum2_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum2_ctau10->SetMarkerColor(4);
     h_PIsoSum2_ctau10->SetLineColor(4);
     h_PIsoSum2_ctau10->Draw("HIST same");
   
     h_PIsoSum2_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum2_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum2_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum2_QCD->SetMarkerColor(6);
     h_PIsoSum2_QCD->SetLineColor(6);
     h_PIsoSum2_QCD->Draw("HIST same");
   
     TLegend *legend2 = new TLegend(0.7,0.7,0.9,0.9);
     legend2->SetBorderSize(1);
     TLegendEntry *legendb = legend2->AddEntry("h_PIsoSum2_ctau0","c_{#tau}=0mm","L");
     legendb->SetLineColor(2);
     legendb->SetMarkerColor(2);
     legendb->SetMarkerSize(2);
     legendb=legend2->AddEntry("h_PIsoSum2_ctau10","c_{#tau}=10mm","L");
     legendb->SetLineColor(4);
     legendb->SetMarkerColor(4);
     legendb->SetMarkerSize(2);
     legendb=legend2->AddEntry("h_PIsoSum2_QCD","QCD","L");
     legendb->SetLineColor(6);
     legendb->SetMarkerColor(6);
     legendb->SetMarkerSize(2);
     legend2->Draw();
   
     canv2->Modified();
     canv2->cd();
     canv2->SetSelected(canv2);
     canv2->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv2->SaveAs("h_PIsoSum2.gif"); 
   
     
     TCanvas* canv3 = new TCanvas ("canv3", "canv3", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv3->SetLogy();
     canv3->SetLineColor(0);
     canv3->SetFillColor(0);
   
     h_PIsoSum3_ctau0->SetTitle("Leading muon, relative Isolation");
     h_PIsoSum3_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum3_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum3_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum3_ctau0->SetMarkerColor(2);
     h_PIsoSum3_ctau0->SetLineColor(2);
     h_PIsoSum3_ctau0->Draw("HIST");
 
     h_PIsoSum3_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum3_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum3_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum3_ctau10->SetMarkerColor(4);
     h_PIsoSum3_ctau10->SetLineColor(4);
     h_PIsoSum3_ctau10->Draw("HIST same");
   
     h_PIsoSum3_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum3_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum3_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum3_QCD->SetMarkerColor(6);
     h_PIsoSum3_QCD->SetLineColor(6);
     h_PIsoSum3_QCD->Draw("HIST same");
   
     TLegend *legend3 = new TLegend(0.7,0.7,0.9,0.9);
     legend3->SetBorderSize(1);
     TLegendEntry *legendc = legend3->AddEntry("h_PIsoSum3_ctau0","c_{#tau}=0mm","L");
     legendc->SetLineColor(2);
     legendc->SetMarkerColor(2);
     legendc->SetMarkerSize(2);
     legendc=legend3->AddEntry("h_PIsoSum3_ctau10","c_{#tau}=10mm","L");
     legendc->SetLineColor(4);
     legendc->SetMarkerColor(4);
     legendc->SetMarkerSize(2);
     legendc=legend3->AddEntry("h_PIsoSum3_QCD","QCD","L");
     legendc->SetLineColor(6);
     legendc->SetMarkerColor(6);
     legendc->SetMarkerSize(2);
     legend2->Draw();
   
     canv3->Modified();
     canv3->cd();
     canv3->SetSelected(canv3);
     canv3->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv3->SaveAs("h_PIsoSum3.gif"); 
   
     
     TCanvas* canv4 = new TCanvas ("canv4", "canv4", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv4->SetLogy();
     canv4->SetLineColor(0);
     canv4->SetFillColor(0);
   
     h_PIsoSum4_ctau0->SetTitle("Subleading muon, relative Isolation");
     h_PIsoSum4_ctau0->GetYaxis()->SetTitle("Events");
     h_PIsoSum4_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum4_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum4_ctau0->SetMarkerColor(2);
     h_PIsoSum4_ctau0->SetLineColor(2);
     h_PIsoSum4_ctau0->Draw("HIST");
 
     h_PIsoSum4_ctau10->GetYaxis()->SetTitle("Events");
     h_PIsoSum4_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum4_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum4_ctau10->SetMarkerColor(4);
     h_PIsoSum4_ctau10->SetLineColor(4);
     h_PIsoSum4_ctau10->Draw("HIST same");
   
     h_PIsoSum4_QCD->GetYaxis()->SetTitle("Events");
     h_PIsoSum4_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_PIsoSum4_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_PIsoSum4_QCD->SetMarkerColor(6);
     h_PIsoSum4_QCD->SetLineColor(6);
     h_PIsoSum4_QCD->Draw("HIST same");
   
     TLegend *legend4 = new TLegend(0.7,0.7,0.9,0.9);
     legend4->SetBorderSize(1);
     TLegendEntry *legendd = legend4->AddEntry("h_PIsoSum4_ctau0","c_{#tau}=0mm","L");
     legendd->SetLineColor(2);
     legendd->SetMarkerColor(2);
     legendd->SetMarkerSize(2);
     legendd=legend4->AddEntry("h_PIsoSum4_ctau10","c_{#tau}=10mm","L");
     legendd->SetLineColor(4);
     legendd->SetMarkerColor(4);
     legendd->SetMarkerSize(2);
     legendd=legend4->AddEntry("h_PIsoSum4_QCD","QCD","L");
     legendd->SetLineColor(6);
     legendd->SetMarkerColor(6);
     legendd->SetMarkerSize(2);
     legend4->Draw();
   
     canv4->Modified();
     canv4->cd();
     canv4->SetSelected(canv4);
     canv4->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv4->SaveAs("h_PIsoSum4.gif"); 
   
   
     
     TCanvas* canvs = new TCanvas ("canvs", "canvs", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvs->SetLogy();
     canvs->SetLineColor(0);
     canvs->SetFillColor(0);
   
     h_PscalarInvMass_ctau0->SetTitle("Dimuon invariant mass");
     h_PscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0.0, 4.); 
     h_PscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_ctau0->SetMarkerColor(2);
     h_PscalarInvMass_ctau0->SetLineColor(2);
     h_PscalarInvMass_ctau0->Draw("HIST");
 
     h_PscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0.0, 4.); 
     h_PscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_ctau10->SetMarkerColor(4);
     h_PscalarInvMass_ctau10->SetLineColor(4);
     h_PscalarInvMass_ctau10->Draw("HIST same");
   
     h_PscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PscalarInvMass_QCD->GetXaxis()->SetRangeUser(0.0, 4.); 
     h_PscalarInvMass_QCD->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
     h_PscalarInvMass_QCD->SetMarkerColor(6);
     h_PscalarInvMass_QCD->SetLineColor(6);
     h_PscalarInvMass_QCD->Draw("HIST same");
   
     TLegend *legends = new TLegend(0.7,0.7,0.9,0.9);
     legends->SetBorderSize(1);
     TLegendEntry *legendw = legends->AddEntry("h_PscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legendw->SetLineColor(2);
     legendw->SetMarkerColor(2);
     legendw->SetMarkerSize(2);
     legendw=legends->AddEntry("h_PscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legendw->SetLineColor(4);
     legendw->SetMarkerColor(4);
     legendw->SetMarkerSize(2);
     legendw=legends->AddEntry("h_PscalarInvMass_QCD","QCD","L");
     legendw->SetLineColor(6);
     legendw->SetMarkerColor(6);
     legendw->SetMarkerSize(2);
     legends->Draw();
   
     canvs->Modified();
     canvs->cd();
     canvs->SetSelected(canvs);
     canvs->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvs->SaveAs("h_PscalarInvMass.gif"); 
   
     TCanvas* canva = new TCanvas ("canva", "canva", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canva->SetLogy();
     canva->SetLineColor(0);
     canva->SetFillColor(0);
   
     h_PantiscalarInvMass_ctau0->SetTitle("Dihadron (pion) invariant mass");
     h_PantiscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_PantiscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_ctau0->SetMarkerColor(2);
     h_PantiscalarInvMass_ctau0->SetLineColor(2);
     h_PantiscalarInvMass_ctau0->Draw("HIST");
 
     h_PantiscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_PantiscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_ctau10->SetMarkerColor(4);
     h_PantiscalarInvMass_ctau10->SetLineColor(4);
     h_PantiscalarInvMass_ctau10->Draw("HIST same");
   
     h_PantiscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PantiscalarInvMass_QCD->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_PantiscalarInvMass_QCD->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_PantiscalarInvMass_QCD->SetMarkerColor(6);
     h_PantiscalarInvMass_QCD->SetLineColor(6);
     h_PantiscalarInvMass_QCD->Draw("HIST same");
   
     TLegend *legendan = new TLegend(0.7,0.7,0.9,0.9);
     legendan->SetBorderSize(1);
     TLegendEntry *legendx = legendan->AddEntry("h_PantiscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legendx->SetLineColor(2);
     legendx->SetMarkerColor(2);
     legendx->SetMarkerSize(2);
     legendx=legendan->AddEntry("h_PantiscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legendx->SetLineColor(4);
     legendx->SetMarkerColor(4);
     legendx->SetMarkerSize(2);
     legendx=legendan->AddEntry("h_PantiscalarInvMass_QCD","QCD","L");
     legendx->SetLineColor(6);
     legendx->SetMarkerColor(6);
     legendx->SetMarkerSize(2);
     legendan->Draw();
   
     canva->Modified();
     canva->cd();
     canva->SetSelected(canva);
     canva->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canva->SaveAs("h_PantiscalarInvMass.gif"); 
   
     TCanvas* canvh = new TCanvas ("canvh", "canvh", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvh->SetLogy();
     canvh->SetLineColor(0);
     canvh->SetFillColor(0);
   
     h_PhiggsInvMass_ctau0->SetTitle("Higgs invariant mass");
     h_PhiggsInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_ctau0->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_ctau0->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_ctau0->SetMarkerColor(2);
     h_PhiggsInvMass_ctau0->SetLineColor(2);
     h_PhiggsInvMass_ctau0->Draw("HIST");
 
     h_PhiggsInvMass_ctau1->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_ctau1->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_ctau1->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_ctau1->SetMarkerColor(4);
     h_PhiggsInvMass_ctau1->SetLineColor(4);
     h_PhiggsInvMass_ctau1->Draw("HIST same");
   
     h_PhiggsInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_ctau10->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_ctau10->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_ctau10->SetMarkerColor(6);
     h_PhiggsInvMass_ctau10->SetLineColor(6);
     h_PhiggsInvMass_ctau10->Draw("HIST same");
   
     h_PhiggsInvMass_ctau1000->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_ctau1000->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_ctau1000->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_ctau1000->SetMarkerColor(1);
     h_PhiggsInvMass_ctau1000->SetLineColor(1);
     h_PhiggsInvMass_ctau1000->Draw("HIST same");
   
     h_PhiggsInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_QCD->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_QCD->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_QCD->SetMarkerColor(84);
     h_PhiggsInvMass_QCD->SetLineColor(84);
     h_PhiggsInvMass_QCD->Draw("HIST same");
   
     TLegend *legendh = new TLegend(0.7,0.7,0.9,0.9);
     legendh->SetBorderSize(1);
     TLegendEntry *legendxx = legendh->AddEntry("h_PhiggsInvMass_ctau0","c_{#tau}=0mm","L");
     legendxx->SetLineColor(2);
     legendxx->SetMarkerColor(2);
     legendxx->SetMarkerSize(2);
     legendxx=legendh->AddEntry("h_PhiggsInvMass_ctau1","c_{#tau}=1mm","L");
     legendxx->SetLineColor(4);
     legendxx->SetMarkerColor(4);
     legendxx->SetMarkerSize(2);
     legendxx=legendh->AddEntry("h_PhiggsInvMass_ctau10","c_{#tau}=10mm","L");
     legendxx->SetLineColor(6);
     legendxx->SetMarkerColor(6);
     legendxx->SetMarkerSize(2);
     legendxx=legendh->AddEntry("h_PhiggsInvMass_ctau1000","c_{#tau}=100mm","L");
     legendxx->SetLineColor(1);
     legendxx->SetMarkerColor(1);
     legendxx->SetMarkerSize(2);
     legendxx=legendh->AddEntry("h_PhiggsInvMass_QCD","QCD","L");
     legendxx->SetLineColor(84);
     legendxx->SetMarkerColor(84);
     legendxx->SetMarkerSize(2);
     legendh->Draw();
   
     canvh->Modified();
     canvh->cd();
     canvh->SetSelected(canvh);
     canvh->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvh->SaveAs("h_PhiggsInvMass.gif");
   
     TCanvas* canvhr = new TCanvas ("canvhr", "canvhr", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvhr->SetLogy();
     canvhr->SetLineColor(0);
     canvhr->SetFillColor(0);
   
     h_PhiggsDeltaR_ctau0->SetTitle("Discalar #Delta R");
     h_PhiggsDeltaR_ctau0->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_ctau0->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_ctau0->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_ctau0->SetMarkerColor(2);
     h_PhiggsDeltaR_ctau0->SetLineColor(2);
     h_PhiggsDeltaR_ctau0->Draw("HIST");
 
     h_PhiggsDeltaR_ctau10->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_ctau10->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_ctau10->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_ctau10->SetMarkerColor(4);
     h_PhiggsDeltaR_ctau10->SetLineColor(4);
     h_PhiggsDeltaR_ctau10->Draw("HIST same");
   
     h_PhiggsDeltaR_QCD->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_QCD->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_QCD->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_QCD->SetMarkerColor(6);
     h_PhiggsDeltaR_QCD->SetLineColor(6);
     h_PhiggsDeltaR_QCD->Draw("HIST same");
   
     TLegend *legendhr = new TLegend(0.7,0.7,0.9,0.9);
     legendhr->SetBorderSize(1);
     TLegendEntry *legendr = legendhr->AddEntry("h_PhiggsDeltaR_ctau0","c_{#tau}=0mm","L");
     legendr->SetLineColor(2);
     legendr->SetMarkerColor(2);
     legendr->SetMarkerSize(2);
     legendr=legendhr->AddEntry("h_PhiggsDeltaR_ctau10","c_{#tau}=10mm","L");
     legendr->SetLineColor(4);
     legendr->SetMarkerColor(4);
     legendr->SetMarkerSize(2);
     legendr=legendhr->AddEntry("h_PhiggsDeltaR_QCD","QCD","L");
     legendr->SetLineColor(6);
     legendr->SetMarkerColor(6);
     legendr->SetMarkerSize(2);
     legendhr->Draw();
   
     canvhr->Modified();
     canvhr->cd();
     canvhr->SetSelected(canvhr);
     canvhr->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvhr->SaveAs("h_PhiggsDeltaR.gif");
     }
