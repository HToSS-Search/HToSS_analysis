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

  void InvMass() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_PscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_PantiscalarInvMass");
     TH1F* h_KantiscalarInvMass_ctau0    = (TH1F*)inFile0.Get("h_KantiscalarInvMass");
   
     TH1F* h_PhiggsInvMass_ctau0    = (TH1F*)inFile0.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_ctau0    = (TH1F*)inFile0.Get("h_PhiggsDeltaR");
     TH1F* h_KhiggsInvMass_ctau0    = (TH1F*)inFile0.Get("h_KhiggsInvMass");
     TH1F* h_KhiggsDeltaR_ctau0    = (TH1F*)inFile0.Get("h_KhiggsDeltaR");
   
   
     TFile inFile1("ggH_MS2_ctau1.root"); 
     TH1F* h_PhiggsInvMass_ctau1    = (TH1F*)inFile1.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_ctau1    = (TH1F*)inFile1.Get("h_PhiggsDeltaR");
     TH1F* h_KhiggsInvMass_ctau1    = (TH1F*)inFile1.Get("h_KhiggsInvMass");
     TH1F* h_KhiggsDeltaR_ctau1    = (TH1F*)inFile1.Get("h_KhiggsDeltaR");
   
   
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_PantiscalarInvMass");
     TH1F* h_KantiscalarInvMass_ctau10    = (TH1F*)inFile10.Get("h_KantiscalarInvMass");
   
     TH1F* h_PhiggsInvMass_ctau10    = (TH1F*)inFile10.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_ctau10    = (TH1F*)inFile10.Get("h_PhiggsDeltaR");
     TH1F* h_KhiggsInvMass_ctau10    = (TH1F*)inFile10.Get("h_KhiggsInvMass");
     TH1F* h_KhiggsDeltaR_ctau10    = (TH1F*)inFile10.Get("h_KhiggsDeltaR");
   
   
     TFile inFile1000("ggH_MS2_ctau100.root"); 
     TH1F* h_PhiggsInvMass_ctau1000    = (TH1F*)inFile1000.Get("h_PhiggsInvMass");
   
   
     TFile inFileQCD("ggH_MC_QCD_kaon.root"); 
     TH1F* h_PscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PscalarInvMass");
     TH1F* h_PantiscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PantiscalarInvMass");
     TH1F* h_KantiscalarInvMass_QCD    = (TH1F*)inFileQCD.Get("h_KantiscalarInvMass");
   
     TH1F* h_PhiggsInvMass_QCD    = (TH1F*)inFileQCD.Get("h_PhiggsInvMass");
     TH1F* h_PhiggsDeltaR_QCD    = (TH1F*)inFileQCD.Get("h_PhiggsDeltaR");
     TH1F* h_KhiggsInvMass_QCD    = (TH1F*)inFileQCD.Get("h_KhiggsInvMass");
     TH1F* h_KhiggsDeltaR_QCD    = (TH1F*)inFileQCD.Get("h_KhiggsDeltaR");
   
   
    
    
     
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
   
   
   
   
     TCanvas* canvaq = new TCanvas ("canvaq", "canvaq", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvaq->SetLogy();
     canvaq->SetLineColor(0);
     canvaq->SetFillColor(0);
   
     h_KantiscalarInvMass_ctau0->SetTitle("Dihadron (kaon) invariant mass");
     h_KantiscalarInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_KantiscalarInvMass_ctau0->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_KantiscalarInvMass_ctau0->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_KantiscalarInvMass_ctau0->SetMarkerColor(2);
     h_KantiscalarInvMass_ctau0->SetLineColor(2);
     h_KantiscalarInvMass_ctau0->Draw("HIST");
 
     h_KantiscalarInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_KantiscalarInvMass_ctau10->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_KantiscalarInvMass_ctau10->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_KantiscalarInvMass_ctau10->SetMarkerColor(4);
     h_KantiscalarInvMass_ctau10->SetLineColor(4);
     h_KantiscalarInvMass_ctau10->Draw("HIST same");
   
     h_KantiscalarInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_KantiscalarInvMass_QCD->GetXaxis()->SetRangeUser(0.0, 3.); 
     h_KantiscalarInvMass_QCD->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
     h_KantiscalarInvMass_QCD->SetMarkerColor(6);
     h_KantiscalarInvMass_QCD->SetLineColor(6);
     h_KantiscalarInvMass_QCD->Draw("HIST same");
   
     TLegend *legendanq = new TLegend(0.7,0.7,0.9,0.9);
     legendanq->SetBorderSize(1);
     TLegendEntry *legendxq = legendanq->AddEntry("h_KantiscalarInvMass_ctau0","c_{#tau}=0mm","L");
     legendxq->SetLineColor(2);
     legendxq->SetMarkerColor(2);
     legendxq->SetMarkerSize(2);
     legendxq=legendanq->AddEntry("h_KantiscalarInvMass_ctau10","c_{#tau}=10mm","L");
     legendxq->SetLineColor(4);
     legendxq->SetMarkerColor(4);
     legendxq->SetMarkerSize(2);
     legendxq=legendanq->AddEntry("h_KantiscalarInvMass_QCD","QCD","L");
     legendxq->SetLineColor(6);
     legendxq->SetMarkerColor(6);
     legendxq->SetMarkerSize(2);
     legendanq->Draw();
   
     canvaq->Modified();
     canvaq->cd();
     canvaq->SetSelected(canvaq);
     canvaq->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canva->SaveAs("h_KantiscalarInvMass.gif"); 
   
   
   
   
   
   
   
   
     TCanvas* canvh = new TCanvas ("canvh", "canvh", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvh->SetLogy();
     canvh->SetLineColor(0);
     canvh->SetFillColor(0);
   
     h_PhiggsInvMass_ctau0->SetTitle("Higgs invariant mass (pion assumption)");
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
   
     /*h_PhiggsInvMass_ctau1000->GetYaxis()->SetTitle("Events");
     h_PhiggsInvMass_ctau1000->GetXaxis()->SetRangeUser(115., 135.); 
     h_PhiggsInvMass_ctau1000->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_PhiggsInvMass_ctau1000->SetMarkerColor(1);
     h_PhiggsInvMass_ctau1000->SetLineColor(1);
     h_PhiggsInvMass_ctau1000->Draw("HIST same");*/
   
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
     /*legendxx=legendh->AddEntry("h_PhiggsInvMass_ctau1000","c_{#tau}=100mm","L");
     legendxx->SetLineColor(1);
     legendxx->SetMarkerColor(1);
     legendxx->SetMarkerSize(2);*/
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
    
    
   
     TCanvas* canvhk = new TCanvas ("canvhk", "canvhk", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvhk->SetLogy();
     canvhk->SetLineColor(0);
     canvhk->SetFillColor(0);
   
     h_KhiggsInvMass_ctau0->SetTitle("Higgs invariant mass (kaon assumption)");
     h_KhiggsInvMass_ctau0->GetYaxis()->SetTitle("Events");
     h_KhiggsInvMass_ctau0->GetXaxis()->SetRangeUser(115., 135.); 
     h_KhiggsInvMass_ctau0->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_KhiggsInvMass_ctau0->SetMarkerColor(2);
     h_KhiggsInvMass_ctau0->SetLineColor(2);
     h_KhiggsInvMass_ctau0->Draw("HIST");
 
     h_KhiggsInvMass_ctau1->GetYaxis()->SetTitle("Events");
     h_KhiggsInvMass_ctau1->GetXaxis()->SetRangeUser(115., 135.); 
     h_KhiggsInvMass_ctau1->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_KhiggsInvMass_ctau1->SetMarkerColor(4);
     h_KhiggsInvMass_ctau1->SetLineColor(4);
     h_KhiggsInvMass_ctau1->Draw("HIST same");
   
     h_KhiggsInvMass_ctau10->GetYaxis()->SetTitle("Events");
     h_KhiggsInvMass_ctau10->GetXaxis()->SetRangeUser(115., 135.); 
     h_KhiggsInvMass_ctau10->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_KhiggsInvMass_ctau10->SetMarkerColor(6);
     h_KhiggsInvMass_ctau10->SetLineColor(6);
     h_KhiggsInvMass_ctau10->Draw("HIST same");
   
     /*h_KhiggsInvMass_ctau1000->GetYaxis()->SetTitle("Events");
     h_KhiggsInvMass_ctau1000->GetXaxis()->SetRangeUser(115., 135.); 
     h_KhiggsInvMass_ctau1000->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_KhiggsInvMass_ctau1000->SetMarkerColor(1);
     h_KhiggsInvMass_ctau1000->SetLineColor(1);
     h_KhiggsInvMass_ctau1000->Draw("HIST same");*/
   
     h_KhiggsInvMass_QCD->GetYaxis()->SetTitle("Events");
     h_KhiggsInvMass_QCD->GetXaxis()->SetRangeUser(115., 135.); 
     h_KhiggsInvMass_QCD->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
     h_KhiggsInvMass_QCD->SetMarkerColor(84);
     h_KhiggsInvMass_QCD->SetLineColor(84);
     h_KhiggsInvMass_QCD->Draw("HIST same");
   
     TLegend *legendhk = new TLegend(0.7,0.7,0.9,0.9);
     legendhk->SetBorderSize(1);
     TLegendEntry *legendxxk = legendhk->AddEntry("h_KhiggsInvMass_ctau0","c_{#tau}=0mm","L");
     legendxxk->SetLineColor(2);
     legendxxk->SetMarkerColor(2);
     legendxxk->SetMarkerSize(2);
     legendxxk=legendhk->AddEntry("h_KhiggsInvMass_ctau1","c_{#tau}=1mm","L");
     legendxxk->SetLineColor(4);
     legendxxk->SetMarkerColor(4);
     legendxxk->SetMarkerSize(2);
     legendxxk=legendhk->AddEntry("h_KhiggsInvMass_ctau10","c_{#tau}=10mm","L");
     legendxxk->SetLineColor(6);
     legendxxk->SetMarkerColor(6);
     legendxxk->SetMarkerSize(2);
     /*legendxxk=legendhk->AddEntry("h_KhiggsInvMass_ctau1000","c_{#tau}=100mm","L");
     legendxxk->SetLineColor(1);
     legendxxk->SetMarkerColor(1);
     legendxxk->SetMarkerSize(2);*/
     legendxxk=legendhk->AddEntry("h_KhiggsInvMass_QCD","QCD","L");
     legendxxk->SetLineColor(84);
     legendxxk->SetMarkerColor(84);
     legendxxk->SetMarkerSize(2);
     legendhk->Draw();
   
     canvhk->Modified();
     canvhk->cd();
     canvhk->SetSelected(canvhk);
     canvhk->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvhk->SaveAs("h_KhiggsInvMass.gif");
   
    
    
    
    
     TCanvas* canvhr = new TCanvas ("canvhr", "canvhr", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvhr->SetLogy();
     canvhr->SetLineColor(0);
     canvhr->SetFillColor(0);
   
     h_PhiggsDeltaR_ctau0->SetTitle("Discalar #Delta R (pion assumption)");
     h_PhiggsDeltaR_ctau0->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_ctau0->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_ctau0->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_ctau0->SetMarkerColor(2);
     h_PhiggsDeltaR_ctau0->SetLineColor(2);
     h_PhiggsDeltaR_ctau0->Draw("HIST");
 
     h_PhiggsDeltaR_ctau1->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_ctau1->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_ctau1->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_ctau1->SetMarkerColor(4);
     h_PhiggsDeltaR_ctau1->SetLineColor(4);
     h_PhiggsDeltaR_ctau1->Draw("HIST same");
   
     h_PhiggsDeltaR_ctau10->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_ctau10->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_ctau10->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_ctau10->SetMarkerColor(6);
     h_PhiggsDeltaR_ctau10->SetLineColor(6);
     h_PhiggsDeltaR_ctau10->Draw("HIST same");
   
     h_PhiggsDeltaR_QCD->GetYaxis()->SetTitle("Events");
     h_PhiggsDeltaR_QCD->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_PhiggsDeltaR_QCD->GetXaxis()->SetTitle("Radians");
     h_PhiggsDeltaR_QCD->SetMarkerColor(84);
     h_PhiggsDeltaR_QCD->SetLineColor(84);
     h_PhiggsDeltaR_QCD->Draw("HIST same");
   
     TLegend *legendhr = new TLegend(0.7,0.7,0.9,0.9);
     legendhr->SetBorderSize(1);
     TLegendEntry *legendr = legendhr->AddEntry("h_PhiggsDeltaR_ctau0","c_{#tau}=0mm","L");
     legendr->SetLineColor(2);
     legendr->SetMarkerColor(2);
     legendr->SetMarkerSize(2);
     legendr=legendhr->AddEntry("h_PhiggsDeltaR_ctau1","c_{#tau}=1mm","L");
     legendr->SetLineColor(4);
     legendr->SetMarkerColor(4);
     legendr->SetMarkerSize(2);
     legendr=legendhr->AddEntry("h_PhiggsDeltaR_ctau10","c_{#tau}=10mm","L");
     legendr->SetLineColor(6);
     legendr->SetMarkerColor(6);
     legendr->SetMarkerSize(2);
     legendr=legendhr->AddEntry("h_PhiggsDeltaR_QCD","QCD","L");
     legendr->SetLineColor(84);
     legendr->SetMarkerColor(84);
     legendr->SetMarkerSize(2);
     legendhr->Draw();
   
     canvhr->Modified();
     canvhr->cd();
     canvhr->SetSelected(canvhr);
     canvhr->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvhr->SaveAs("h_PhiggsDeltaR.gif");
    
    
    
    
     TCanvas* canvhs = new TCanvas ("canvhs", "canvhs", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canvhs->SetLogy();
     canvhs->SetLineColor(0);
     canvhs->SetFillColor(0);
   
     h_KhiggsDeltaR_ctau0->SetTitle("Discalar #Delta R (kaon assumption)");
     h_KhiggsDeltaR_ctau0->GetYaxis()->SetTitle("Events");
     h_KhiggsDeltaR_ctau0->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_KhiggsDeltaR_ctau0->GetXaxis()->SetTitle("Radians");
     h_KhiggsDeltaR_ctau0->SetMarkerColor(2);
     h_KhiggsDeltaR_ctau0->SetLineColor(2);
     h_KhiggsDeltaR_ctau0->Draw("HIST");
 
     h_KhiggsDeltaR_ctau1->GetYaxis()->SetTitle("Events");
     h_KhiggsDeltaR_ctau1->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_KhiggsDeltaR_ctau1->GetXaxis()->SetTitle("Radians");
     h_KhiggsDeltaR_ctau1->SetMarkerColor(4);
     h_KhiggsDeltaR_ctau1->SetLineColor(4);
     h_KhiggsDeltaR_ctau1->Draw("HIST same");
  
     h_KhiggsDeltaR_ctau10->GetYaxis()->SetTitle("Events");
     h_KhiggsDeltaR_ctau10->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_KhiggsDeltaR_ctau10->GetXaxis()->SetTitle("Radians");
     h_KhiggsDeltaR_ctau10->SetMarkerColor(6);
     h_KhiggsDeltaR_ctau10->SetLineColor(6);
     h_KhiggsDeltaR_ctau10->Draw("HIST same");
   
     h_KhiggsDeltaR_QCD->GetYaxis()->SetTitle("Events");
     h_KhiggsDeltaR_QCD->GetXaxis()->SetRangeUser(0.0, 5.); 
     h_KhiggsDeltaR_QCD->GetXaxis()->SetTitle("Radians");
     h_KhiggsDeltaR_QCD->SetMarkerColor(84);
     h_KhiggsDeltaR_QCD->SetLineColor(84);
     h_KhiggsDeltaR_QCD->Draw("HIST same");
   
     TLegend *legendhs = new TLegend(0.7,0.7,0.9,0.9);
     legendhs->SetBorderSize(1);
     TLegendEntry *legends = legendhs->AddEntry("h_KhiggsDeltaR_ctau0","c_{#tau}=0mm","L");
     legends->SetLineColor(2);
     legends->SetMarkerColor(2);
     legends->SetMarkerSize(2);
     legends=legendhs->AddEntry("h_KhiggsDeltaR_ctau1","c_{#tau}=1mm","L");
     legends->SetLineColor(4);
     legends->SetMarkerColor(4);
     legends->SetMarkerSize(2);
     legends=legendhs->AddEntry("h_KhiggsDeltaR_ctau10","c_{#tau}=10mm","L");
     legends->SetLineColor(6);
     legends->SetMarkerColor(6);
     legends->SetMarkerSize(2);
     legends=legendhs->AddEntry("h_KhiggsDeltaR_QCD","QCD","L");
     legends->SetLineColor(84);
     legends->SetMarkerColor(84);
     legends->SetMarkerSize(2);
     legendhs->Draw();
   
     canvhs->Modified();
     canvhs->cd();
     canvhs->SetSelected(canvhs);
     canvhs->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvhr->SaveAs("h_KhiggsDeltaR.gif");
     }
