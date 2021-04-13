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

  void relativeIso() {  
  
     TFile inFile0("ggH_MS2_ctau0.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_PIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_PIsoSum2");
     TH1F* h_KIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_KIsoSum1");
     TH1F* h_KIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_KIsoSum2");
     TH1F* h_MuonIsoSum1_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum1");
     TH1F* h_MuonIsoSum2_ctau0    = (TH1F*)inFile0.Get("h_MuonIsoSum2");
  
   
     
     TFile inFile10("ggH_MS2_ctau10.root"); 
     TH1F* h_PIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_PIsoSum2");
     TH1F* h_KIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum1");
     TH1F* h_KIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_KIsoSum2");
     TH1F* h_MuonIsoSum1_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum1");
     TH1F* h_MuonIsoSum2_ctau10    = (TH1F*)inFile10.Get("h_MuonIsoSum2");
   
    
    
     TFile inFileQCD("ggH_MC_QCD_kaon.root"); 
     TH1F* h_PIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum1");
     TH1F* h_PIsoSum2_QCD    = (TH1F*)inFileQCD.Get("h_PIsoSum2");
     TH1F* h_KIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_KIsoSum1");
     TH1F* h_KIsoSum2_QCD    = (TH1F*)inFileQCD.Get("h_KIsoSum2");
     TH1F* h_MuonIsoSum1_QCD    = (TH1F*)inFileQCD.Get("h_MuonIsoSum1");
     TH1F* h_MuonIsoSum2_QCD    = (TH1F*)inFileQCD.Get("h_MuonIsoSum2");
   
    
   
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv1->SetLogy();
     canv1->SetLineColor(0);
     canv1->SetFillColor(0);
   
     h_PIsoSum1_ctau0->SetTitle("Leading pion, relative isolation");
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
   
     h_PIsoSum2_ctau0->SetTitle("Subleading pion, relative isolation");
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
   
     h_KIsoSum1_ctau0->SetTitle("Leading kaon, relative isolation");
     h_KIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_KIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum1_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum1_ctau0->SetMarkerColor(2);
     h_KIsoSum1_ctau0->SetLineColor(2);
     h_KIsoSum1_ctau0->Draw("HIST");
 
     h_KIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_KIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum1_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum1_ctau10->SetMarkerColor(4);
     h_KIsoSum1_ctau10->SetLineColor(4);
     h_KIsoSum1_ctau10->Draw("HIST same");
   
     h_KIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_KIsoSum1_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum1_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum1_QCD->SetMarkerColor(6);
     h_KIsoSum1_QCD->SetLineColor(6);
     h_KIsoSum1_QCD->Draw("HIST same");
   
     TLegend *legend3 = new TLegend(0.7,0.7,0.9,0.9);
     legend3->SetBorderSize(1);
     TLegendEntry *legendc = legend3->AddEntry("h_KIsoSum1_ctau0","c_{#tau}=0mm","L");
     legendc->SetLineColor(2);
     legendc->SetMarkerColor(2);
     legendc->SetMarkerSize(2);
     legendc=legend3->AddEntry("h_KIsoSum1_ctau10","c_{#tau}=10mm","L");
     legendc->SetLineColor(4);
     legendc->SetMarkerColor(4);
     legendc->SetMarkerSize(2);
     legendc=legend3->AddEntry("h_KIsoSum1_QCD","QCD","L");
     legendc->SetLineColor(6);
     legendc->SetMarkerColor(6);
     legendc->SetMarkerSize(2);
     legend2->Draw();
   
     canv3->Modified();
     canv3->cd();
     canv3->SetSelected(canv3);
     canv3->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv3->SaveAs("h_KIsoSum1.gif"); 
   
     
     TCanvas* canv4 = new TCanvas ("canv4", "canv4", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv4->SetLogy();
     canv4->SetLineColor(0);
     canv4->SetFillColor(0);
   
     h_KIsoSum2_ctau0->SetTitle("Subleading kaon, relative isolation");
     h_KIsoSum2_ctau0->GetYaxis()->SetTitle("Events");
     h_KIsoSum2_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum2_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum2_ctau0->SetMarkerColor(2);
     h_KIsoSum2_ctau0->SetLineColor(2);
     h_KIsoSum2_ctau0->Draw("HIST");
 
     h_KIsoSum2_ctau10->GetYaxis()->SetTitle("Events");
     h_KIsoSum2_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum2_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum2_ctau10->SetMarkerColor(4);
     h_KIsoSum2_ctau10->SetLineColor(4);
     h_KIsoSum2_ctau10->Draw("HIST same");
   
     h_KIsoSum2_QCD->GetYaxis()->SetTitle("Events");
     h_KIsoSum2_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_KIsoSum2_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_KIsoSum2_QCD->SetMarkerColor(6);
     h_KIsoSum2_QCD->SetLineColor(6);
     h_KIsoSum2_QCD->Draw("HIST same");
   
     TLegend *legend4 = new TLegend(0.7,0.7,0.9,0.9);
     legend4->SetBorderSize(1);
     TLegendEntry *legendd = legend4->AddEntry("h_KIsoSum2_ctau0","c_{#tau}=0mm","L");
     legendd->SetLineColor(2);
     legendd->SetMarkerColor(2);
     legendd->SetMarkerSize(2);
     legendd=legend4->AddEntry("h_KIsoSum2_ctau10","c_{#tau}=10mm","L");
     legendd->SetLineColor(4);
     legendd->SetMarkerColor(4);
     legendd->SetMarkerSize(2);
     legendd=legend4->AddEntry("h_KIsoSum2_QCD","QCD","L");
     legendd->SetLineColor(6);
     legendd->SetMarkerColor(6);
     legendd->SetMarkerSize(2);
     legend4->Draw();
   
     canv4->Modified();
     canv4->cd();
     canv4->SetSelected(canv4);
     canv4->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv4->SaveAs("h_KIsoSum2.gif"); 
   
   
     TCanvas* canv5 = new TCanvas ("canv5", "canv5", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv5->SetLogy();
     canv5->SetLineColor(0);
     canv5->SetFillColor(0);
   
     h_MuonIsoSum1_ctau0->SetTitle("Leading muon, relative isolation");
     h_MuonIsoSum1_ctau0->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum1_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum1_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum1_ctau0->SetMarkerColor(2);
     h_MuonIsoSum1_ctau0->SetLineColor(2);
     h_MuonIsoSum1_ctau0->Draw("HIST");
 
     h_MuonIsoSum1_ctau10->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum1_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum1_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum1_ctau10->SetMarkerColor(4);
     h_MuonIsoSum1_ctau10->SetLineColor(4);
     h_MuonIsoSum1_ctau10->Draw("HIST same");
   
     h_MuonIsoSum1_QCD->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum1_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum1_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum1_QCD->SetMarkerColor(6);
     h_MuonIsoSum1_QCD->SetLineColor(6);
     h_MuonIsoSum1_QCD->Draw("HIST same");
   
     TLegend *legend1m = new TLegend(0.7,0.7,0.9,0.9);
     legend1m->SetBorderSize(1);
     TLegendEntry *legendam = legend1m->AddEntry("h_MuonIsoSum1_ctau0","c_{#tau}=0mm","L");
     legendam->SetLineColor(2);
     legendam->SetMarkerColor(2);
     legendam->SetMarkerSize(2);
     legendam=legend1m->AddEntry("h_MuonIsoSum1_ctau10","c_{#tau}=10mm","L");
     legendam->SetLineColor(4);
     legendam->SetMarkerColor(4);
     legendam->SetMarkerSize(2);
     legendam=legend1m->AddEntry("h_MuonIsoSum1_QCD","QCD","L");
     legendam->SetLineColor(6);
     legendam->SetMarkerColor(6);
     legendam->SetMarkerSize(2);
     legend1m->Draw();
   
     canv5->Modified();
     canv5->cd();
     canv5->SetSelected(canv5);
     canv5->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv5->SaveAs("h_MuonIsoSum1.gif"); // .pdf and other formats work too! 
   
   
     TCanvas* canv6 = new TCanvas ("canv6", "canv6", 50, 50, 800, 600); // Canvas to draw histogram on
     gStyle->SetOptStat(0);
     canv6->SetLogy();
     canv6->SetLineColor(0);
     canv6->SetFillColor(0);
   
     h_MuonIsoSum2_ctau0->SetTitle("Subleading muon, relative isolation");
     h_MuonIsoSum2_ctau0->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum2_ctau0->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum2_ctau0->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum2_ctau0->SetMarkerColor(2);
     h_MuonIsoSum2_ctau0->SetLineColor(2);
     h_MuonIsoSum2_ctau0->Draw("HIST");
 
     h_MuonIsoSum2_ctau10->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum2_ctau10->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum2_ctau10->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum2_ctau10->SetMarkerColor(4);
     h_MuonIsoSum2_ctau10->SetLineColor(4);
     h_MuonIsoSum2_ctau10->Draw("HIST same");
   
     h_MuonIsoSum2_QCD->GetYaxis()->SetTitle("Events");
     h_MuonIsoSum2_QCD->GetXaxis()->SetRangeUser(0.0, 2.); 
     h_MuonIsoSum2_QCD->GetXaxis()->SetTitle("Rel. iso");
     h_MuonIsoSum2_QCD->SetMarkerColor(6);
     h_MuonIsoSum2_QCD->SetLineColor(6);
     h_MuonIsoSum2_QCD->Draw("HIST same");
   
     TLegend *legend1mm = new TLegend(0.7,0.7,0.9,0.9);
     legend1mm->SetBorderSize(1);
     TLegendEntry *legendamm = legend1mm->AddEntry("h_MuonIsoSum2_ctau0","c_{#tau}=0mm","L");
     legendamm->SetLineColor(2);
     legendamm->SetMarkerColor(2);
     legendamm->SetMarkerSize(2);
     legendamm=legend1mm->AddEntry("h_MuonIsoSum2_ctau10","c_{#tau}=10mm","L");
     legendamm->SetLineColor(4);
     legendamm->SetMarkerColor(4);
     legendamm->SetMarkerSize(2);
     legendamm=legend1mm->AddEntry("h_MuonIsoSum2_QCD","QCD","L");
     legendamm->SetLineColor(6);
     legendamm->SetMarkerColor(6);
     legendamm->SetMarkerSize(2);
     legend1mm->Draw();
  
     canv6->Modified();
     canv6->cd();
     canv6->SetSelected(canv6);
     canv6->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv6->SaveAs("h_MuonIsoSum2.gif"); // .pdf and other formats work too! 
   
  }
