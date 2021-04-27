//An example macro for manipulating plots
#include "TFile.h"

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TStyle.h"
#include "TLatex.h"

#include "TString.h"
#include "TPRegexp.h"

void scoutingEfficiencyPlotMacro() {

    TFile* inFile0  =  new TFile ("plots/scoutingDistributions/output_HtoSS_MS2_ctau0.root");
    TFile* inFile1  =  new TFile ("plots/scoutingDistributions/output_HtoSS_MS2_ctau1.root");
    TFile* inFile2  =  new TFile ("plots/scoutingDistributions/output_HtoSS_MS2_ctau10.root");
    TFile* inFile3  =  new TFile ("plots/scoutingDistributions/output_HtoSS_MS2_ctau100.root");
    TFile* inFile4  =  new TFile ("plots/scoutingDistributions/output_HtoSS_MS2_ctau1000.root");

    // pT

    TH1F* h_leadingMuonPt_HT250_CaloTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT250_CaloTrig");
    TH1F* h_leadingMuonPt_HT250_CaloTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT250_CaloTrig");
    TH1F* h_leadingMuonPt_HT250_CaloTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT250_CaloTrig");
    TH1F* h_leadingMuonPt_HT250_CaloTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT250_CaloTrig");
    TH1F* h_leadingMuonPt_HT250_CaloTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT250_CaloTrig");

    TCanvas *Cavnas_HT250_CaloTrigPt = new TCanvas("Cavnas_HT250_CaloTrigPt", "Cavnas_HT250_CaloTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloTrigPt->SetFillColor(0);
    Cavnas_HT250_CaloTrigPt->SetBorderMode(0);
    Cavnas_HT250_CaloTrigPt->SetBorderSize(2);
    Cavnas_HT250_CaloTrigPt->SetLeftMargin(0.15);
    Cavnas_HT250_CaloTrigPt->SetRightMargin(0.05);
    Cavnas_HT250_CaloTrigPt->SetTopMargin(0.05);
    Cavnas_HT250_CaloTrigPt->SetBottomMargin(0.15);
    Cavnas_HT250_CaloTrigPt->SetGridy();
    Cavnas_HT250_CaloTrigPt->SetTickx(1);
    Cavnas_HT250_CaloTrigPt->SetTicky(1);
    Cavnas_HT250_CaloTrigPt->SetFrameBorderMode(0);
    Cavnas_HT250_CaloTrigPt->cd();

    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo trigger efficiency");
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT250_CaloTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT250_CaloTrigPt->SetBorderSize(0);
    leg_HT250_CaloTrigPt->SetTextSize(0.03);
    leg_HT250_CaloTrigPt->SetLineColor(1);
    leg_HT250_CaloTrigPt->SetLineStyle(1);
    leg_HT250_CaloTrigPt->SetLineWidth(1);
    leg_HT250_CaloTrigPt->SetFillColor(0);
    leg_HT250_CaloTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloTrigPt = leg_HT250_CaloTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloTrigPt->SetMarkerColor(9);
    entry_HT250_CaloTrigPt->SetMarkerStyle(26);
    entry_HT250_CaloTrigPt->SetMarkerSize(2);
    entry_HT250_CaloTrigPt->SetTextFont(42);
    entry_HT250_CaloTrigPt = leg_HT250_CaloTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloTrigPt->SetMarkerColor(8);
    entry_HT250_CaloTrigPt->SetMarkerStyle(4);
    entry_HT250_CaloTrigPt->SetMarkerSize(2);
    entry_HT250_CaloTrigPt->SetTextFont(42);
    entry_HT250_CaloTrigPt = leg_HT250_CaloTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloTrigPt->SetMarkerColor(2);
    entry_HT250_CaloTrigPt->SetMarkerStyle(5);
    entry_HT250_CaloTrigPt->SetMarkerSize(2);
    entry_HT250_CaloTrigPt->SetTextFont(42);
    entry_HT250_CaloTrigPt = leg_HT250_CaloTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloTrigPt->SetMarkerColor(1);
    entry_HT250_CaloTrigPt->SetMarkerStyle(2);
    entry_HT250_CaloTrigPt->SetMarkerSize(2);
    entry_HT250_CaloTrigPt->SetTextFont(42);
    entry_HT250_CaloTrigPt = leg_HT250_CaloTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloTrigPt->SetMarkerColor(4);
    entry_HT250_CaloTrigPt->SetMarkerStyle(3);
    entry_HT250_CaloTrigPt->SetMarkerSize(2);
    entry_HT250_CaloTrigPt->SetTextFont(42);
    leg_HT250_CaloTrigPt->Draw("HIST");

    Cavnas_HT250_CaloTrigPt->Modified();
    Cavnas_HT250_CaloTrigPt->cd();
    Cavnas_HT250_CaloTrigPt->SetSelected(Cavnas_HT250_CaloTrigPt);
    Cavnas_HT250_CaloTrigPt->SaveAs("HT250_CaloTrig_pT_eff.pdf");

    //

    TH1F* h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT250_CaloBtagTrig");

    TCanvas *Cavnas_HT250_CaloBtagTrigPt = new TCanvas("Cavnas_HT250_CaloBtagTrigPt", "Cavnas_HT250_CaloBtagTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloBtagTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloBtagTrigPt->SetFillColor(0);
    Cavnas_HT250_CaloBtagTrigPt->SetBorderMode(0);
    Cavnas_HT250_CaloBtagTrigPt->SetBorderSize(2);
    Cavnas_HT250_CaloBtagTrigPt->SetLeftMargin(0.15);
    Cavnas_HT250_CaloBtagTrigPt->SetRightMargin(0.05);
    Cavnas_HT250_CaloBtagTrigPt->SetTopMargin(0.05);
    Cavnas_HT250_CaloBtagTrigPt->SetBottomMargin(0.15);
    Cavnas_HT250_CaloBtagTrigPt->SetGridy();
    Cavnas_HT250_CaloBtagTrigPt->SetTickx(1);
    Cavnas_HT250_CaloBtagTrigPt->SetTicky(1);
    Cavnas_HT250_CaloBtagTrigPt->SetFrameBorderMode(0);
    Cavnas_HT250_CaloBtagTrigPt->cd();

    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo btag trigger efficiency");
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloBtagTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT250_CaloBtagTrigPt->SetBorderSize(0);
    leg_HT250_CaloBtagTrigPt->SetTextSize(0.03);
    leg_HT250_CaloBtagTrigPt->SetLineColor(1);
    leg_HT250_CaloBtagTrigPt->SetLineStyle(1);
    leg_HT250_CaloBtagTrigPt->SetLineWidth(1);
    leg_HT250_CaloBtagTrigPt->SetFillColor(0);
    leg_HT250_CaloBtagTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloBtagTrigPt = leg_HT250_CaloBtagTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloBtagTrigPt->SetMarkerColor(9);
    entry_HT250_CaloBtagTrigPt->SetMarkerStyle(26);
    entry_HT250_CaloBtagTrigPt->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigPt->SetTextFont(42);
    entry_HT250_CaloBtagTrigPt = leg_HT250_CaloBtagTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloBtagTrigPt->SetMarkerColor(8);
    entry_HT250_CaloBtagTrigPt->SetMarkerStyle(4);
    entry_HT250_CaloBtagTrigPt->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigPt->SetTextFont(42);
    entry_HT250_CaloBtagTrigPt = leg_HT250_CaloBtagTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloBtagTrigPt->SetMarkerColor(2);
    entry_HT250_CaloBtagTrigPt->SetMarkerStyle(5);
    entry_HT250_CaloBtagTrigPt->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigPt->SetTextFont(42);
    entry_HT250_CaloBtagTrigPt = leg_HT250_CaloBtagTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloBtagTrigPt->SetMarkerColor(1);
    entry_HT250_CaloBtagTrigPt->SetMarkerStyle(2);
    entry_HT250_CaloBtagTrigPt->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigPt->SetTextFont(42);
    entry_HT250_CaloBtagTrigPt = leg_HT250_CaloBtagTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloBtagTrigPt->SetMarkerColor(4);
    entry_HT250_CaloBtagTrigPt->SetMarkerStyle(3);
    entry_HT250_CaloBtagTrigPt->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigPt->SetTextFont(42);
    leg_HT250_CaloBtagTrigPt->Draw("HIST");

    Cavnas_HT250_CaloBtagTrigPt->Modified();
    Cavnas_HT250_CaloBtagTrigPt->cd();
    Cavnas_HT250_CaloBtagTrigPt->SetSelected(Cavnas_HT250_CaloBtagTrigPt);
    Cavnas_HT250_CaloBtagTrigPt->SaveAs( "HT250_CaloBtagTrig_pT_eff.pdf" );

    //

    TH1F* h_leadingMuonPt_HT410_PFPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT410_PF");
    TH1F* h_leadingMuonPt_HT410_PFPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT410_PF");
    TH1F* h_leadingMuonPt_HT410_PFPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT410_PF");
    TH1F* h_leadingMuonPt_HT410_PFPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT410_PF");
    TH1F* h_leadingMuonPt_HT410_PFPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT410_PF");

    TCanvas *Cavnas_HT410_PFPt = new TCanvas("Cavnas_HT410_PFPt", "Cavnas_HT410_PFPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFPt->SetFillColor(0);
    Cavnas_HT410_PFPt->SetBorderMode(0);
    Cavnas_HT410_PFPt->SetBorderSize(2);
    Cavnas_HT410_PFPt->SetLeftMargin(0.15);
    Cavnas_HT410_PFPt->SetRightMargin(0.05);
    Cavnas_HT410_PFPt->SetTopMargin(0.05);
    Cavnas_HT410_PFPt->SetBottomMargin(0.15);
    Cavnas_HT410_PFPt->SetGridy();
    Cavnas_HT410_PFPt->SetTickx(1);
    Cavnas_HT410_PFPt->SetTicky(1);
    Cavnas_HT410_PFPt->SetFrameBorderMode(0);
    Cavnas_HT410_PFPt->cd();

    h_leadingMuonPt_HT410_PFPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_PFPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT410_PFPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT410_PFPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT410_PFPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT410_PFPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT410_PFPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT410_PFPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT410_PFPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT410_PFPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT410_PFPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT410_PFPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT410_PFPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT410_PFPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT410_PFPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT410_PFPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT410_PFPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT410_PFPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT410_PFPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_PFPt->SetBorderSize(0);
    leg_HT410_PFPt->SetTextSize(0.03);
    leg_HT410_PFPt->SetLineColor(1);
    leg_HT410_PFPt->SetLineStyle(1);
    leg_HT410_PFPt->SetLineWidth(1);
    leg_HT410_PFPt->SetFillColor(0);
    leg_HT410_PFPt->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFPt = leg_HT410_PFPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFPt->SetMarkerColor(9);
    entry_HT410_PFPt->SetMarkerStyle(26);
    entry_HT410_PFPt->SetMarkerSize(2);
    entry_HT410_PFPt->SetTextFont(42);
    entry_HT410_PFPt = leg_HT410_PFPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFPt->SetMarkerColor(8);
    entry_HT410_PFPt->SetMarkerStyle(4);
    entry_HT410_PFPt->SetMarkerSize(2);
    entry_HT410_PFPt->SetTextFont(42);
    entry_HT410_PFPt = leg_HT410_PFPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFPt->SetMarkerColor(2);
    entry_HT410_PFPt->SetMarkerStyle(5);
    entry_HT410_PFPt->SetMarkerSize(2);
    entry_HT410_PFPt->SetTextFont(42);
    entry_HT410_PFPt = leg_HT410_PFPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFPt->SetMarkerColor(1);
    entry_HT410_PFPt->SetMarkerStyle(2);
    entry_HT410_PFPt->SetMarkerSize(2);
    entry_HT410_PFPt->SetTextFont(42);
    entry_HT410_PFPt = leg_HT410_PFPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFPt->SetMarkerColor(4);
    entry_HT410_PFPt->SetMarkerStyle(3);
    entry_HT410_PFPt->SetMarkerSize(2);
    entry_HT410_PFPt->SetTextFont(42);
    leg_HT410_PFPt->Draw("HIST");

    Cavnas_HT410_PFPt->Modified();
    Cavnas_HT410_PFPt->cd();
    Cavnas_HT410_PFPt->SetSelected(Cavnas_HT410_PFPt);
    Cavnas_HT410_PFPt->SaveAs( "HT410_PFTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_HT410_BtagPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT410_Btag");
    TH1F* h_leadingMuonPt_HT410_BtagPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT410_Btag");
    TH1F* h_leadingMuonPt_HT410_BtagPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT410_Btag");
    TH1F* h_leadingMuonPt_HT410_BtagPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT410_Btag");
    TH1F* h_leadingMuonPt_HT410_BtagPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT410_Btag");

    TCanvas *Cavnas_HT410_BtagPt = new TCanvas("Cavnas_HT410_BtagPt", "Cavnas_HT410_BtagPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagPt->SetFillColor(0);
    Cavnas_HT410_BtagPt->SetBorderMode(0);
    Cavnas_HT410_BtagPt->SetBorderSize(2);
    Cavnas_HT410_BtagPt->SetLeftMargin(0.15);
    Cavnas_HT410_BtagPt->SetRightMargin(0.05);
    Cavnas_HT410_BtagPt->SetTopMargin(0.05);
    Cavnas_HT410_BtagPt->SetBottomMargin(0.15);
    Cavnas_HT410_BtagPt->SetGridy();
    Cavnas_HT410_BtagPt->SetTickx(1);
    Cavnas_HT410_BtagPt->SetTicky(1);
    Cavnas_HT410_BtagPt->SetFrameBorderMode(0);
    Cavnas_HT410_BtagPt->cd();

    h_leadingMuonPt_HT410_BtagPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_BtagPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT410_BtagPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT410_BtagPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT410_BtagPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT410_BtagPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT410_BtagPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT410_BtagPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT410_BtagPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT410_BtagPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT410_BtagPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT410_BtagPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT410_BtagPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT410_BtagPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT410_BtagPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT410_BtagPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT410_BtagPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT410_BtagPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT410_BtagPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_BtagPt->SetBorderSize(0);
    leg_HT410_BtagPt->SetTextSize(0.03);
    leg_HT410_BtagPt->SetLineColor(1);
    leg_HT410_BtagPt->SetLineStyle(1);
    leg_HT410_BtagPt->SetLineWidth(1);
    leg_HT410_BtagPt->SetFillColor(0);
    leg_HT410_BtagPt->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagPt = leg_HT410_BtagPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagPt->SetMarkerColor(9);
    entry_HT410_BtagPt->SetMarkerStyle(26);
    entry_HT410_BtagPt->SetMarkerSize(2);
    entry_HT410_BtagPt->SetTextFont(42);
    entry_HT410_BtagPt = leg_HT410_BtagPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagPt->SetMarkerColor(8);
    entry_HT410_BtagPt->SetMarkerStyle(4);
    entry_HT410_BtagPt->SetMarkerSize(2);
    entry_HT410_BtagPt->SetTextFont(42);
    entry_HT410_BtagPt = leg_HT410_BtagPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagPt->SetMarkerColor(2);
    entry_HT410_BtagPt->SetMarkerStyle(5);
    entry_HT410_BtagPt->SetMarkerSize(2);
    entry_HT410_BtagPt->SetTextFont(42);
    entry_HT410_BtagPt = leg_HT410_BtagPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagPt->SetMarkerColor(1);
    entry_HT410_BtagPt->SetMarkerStyle(2);
    entry_HT410_BtagPt->SetMarkerSize(2);
    entry_HT410_BtagPt->SetTextFont(42);
    entry_HT410_BtagPt = leg_HT410_BtagPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagPt->SetMarkerColor(4);
    entry_HT410_BtagPt->SetMarkerStyle(3);
    entry_HT410_BtagPt->SetMarkerSize(2);
    entry_HT410_BtagPt->SetTextFont(42);
    leg_HT410_BtagPt->Draw("HIST");

    Cavnas_HT410_BtagPt->Modified();
    Cavnas_HT410_BtagPt->cd();
    Cavnas_HT410_BtagPt->SetSelected(Cavnas_HT410_BtagPt);
    Cavnas_HT410_BtagPt->SaveAs( "HT410_BtagTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_DoubleMuNoVtxTrig");

    TCanvas *Cavnas_DoubleMuNoVtxTrigPt = new TCanvas("Cavnas_DoubleMuNoVtxTrigPt", "Cavnas_DoubleMuNoVtxTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_DoubleMuNoVtxTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_DoubleMuNoVtxTrigPt->SetFillColor(0);
    Cavnas_DoubleMuNoVtxTrigPt->SetBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigPt->SetBorderSize(2);
    Cavnas_DoubleMuNoVtxTrigPt->SetLeftMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigPt->SetRightMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigPt->SetTopMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigPt->SetBottomMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigPt->SetGridy();
    Cavnas_DoubleMuNoVtxTrigPt->SetTickx(1);
    Cavnas_DoubleMuNoVtxTrigPt->SetTicky(1);
    Cavnas_DoubleMuNoVtxTrigPt->SetFrameBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigPt->cd();

    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetTitle("#mu#mu no vtx trigger efficiency");
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_DoubleMuNoVtxTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_DoubleMuNoVtxTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_DoubleMuNoVtxTrigPt->SetBorderSize(0);
    leg_DoubleMuNoVtxTrigPt->SetTextSize(0.03);
    leg_DoubleMuNoVtxTrigPt->SetLineColor(1);
    leg_DoubleMuNoVtxTrigPt->SetLineStyle(1);
    leg_DoubleMuNoVtxTrigPt->SetLineWidth(1);
    leg_DoubleMuNoVtxTrigPt->SetFillColor(0);
    leg_DoubleMuNoVtxTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_DoubleMuNoVtxTrigPt = leg_DoubleMuNoVtxTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_DoubleMuNoVtxTrigPt->SetMarkerColor(9);
    entry_DoubleMuNoVtxTrigPt->SetMarkerStyle(26);
    entry_DoubleMuNoVtxTrigPt->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigPt->SetTextFont(42);
    entry_DoubleMuNoVtxTrigPt = leg_DoubleMuNoVtxTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_DoubleMuNoVtxTrigPt->SetMarkerColor(8);
    entry_DoubleMuNoVtxTrigPt->SetMarkerStyle(4);
    entry_DoubleMuNoVtxTrigPt->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigPt->SetTextFont(42);
    entry_DoubleMuNoVtxTrigPt = leg_DoubleMuNoVtxTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_DoubleMuNoVtxTrigPt->SetMarkerColor(2);
    entry_DoubleMuNoVtxTrigPt->SetMarkerStyle(5);
    entry_DoubleMuNoVtxTrigPt->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigPt->SetTextFont(42);
    entry_DoubleMuNoVtxTrigPt = leg_DoubleMuNoVtxTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_DoubleMuNoVtxTrigPt->SetMarkerColor(1);
    entry_DoubleMuNoVtxTrigPt->SetMarkerStyle(2);
    entry_DoubleMuNoVtxTrigPt->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigPt->SetTextFont(42);
    entry_DoubleMuNoVtxTrigPt = leg_DoubleMuNoVtxTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_DoubleMuNoVtxTrigPt->SetMarkerColor(4);
    entry_DoubleMuNoVtxTrigPt->SetMarkerStyle(3);
    entry_DoubleMuNoVtxTrigPt->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigPt->SetTextFont(42);
    leg_DoubleMuNoVtxTrigPt->Draw("HIST");

    Cavnas_DoubleMuNoVtxTrigPt->Modified();
    Cavnas_DoubleMuNoVtxTrigPt->cd();
    Cavnas_DoubleMuNoVtxTrigPt->SetSelected(Cavnas_DoubleMuNoVtxTrigPt);
    Cavnas_DoubleMuNoVtxTrigPt->SaveAs( "DoubleMuNoVtxTrig_pT_eff.pdf" );

    // eta

    TH1F* h_leadingMuonEta_HT250_CaloTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT250_CaloTrig");
    TH1F* h_leadingMuonEta_HT250_CaloTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT250_CaloTrig");
    TH1F* h_leadingMuonEta_HT250_CaloTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT250_CaloTrig");
    TH1F* h_leadingMuonEta_HT250_CaloTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT250_CaloTrig");
    TH1F* h_leadingMuonEta_HT250_CaloTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT250_CaloTrig");

    TCanvas *Cavnas_HT250_CaloTrigEta = new TCanvas("Cavnas_HT250_CaloTrigEta", "Cavnas_HT250_CaloTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloTrigEta->SetFillColor(0);
    Cavnas_HT250_CaloTrigEta->SetBorderMode(0);
    Cavnas_HT250_CaloTrigEta->SetBorderSize(2);
    Cavnas_HT250_CaloTrigEta->SetLeftMargin(0.15);
    Cavnas_HT250_CaloTrigEta->SetRightMargin(0.05);
    Cavnas_HT250_CaloTrigEta->SetTopMargin(0.05);
    Cavnas_HT250_CaloTrigEta->SetBottomMargin(0.15);
    Cavnas_HT250_CaloTrigEta->SetGridy();
    Cavnas_HT250_CaloTrigEta->SetTickx(1);
    Cavnas_HT250_CaloTrigEta->SetTicky(1);
    Cavnas_HT250_CaloTrigEta->SetFrameBorderMode(0);
    Cavnas_HT250_CaloTrigEta->cd();

    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo trigger efficiency");
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT250_CaloTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT250_CaloTrigEta->SetBorderSize(0);
    leg_HT250_CaloTrigEta->SetTextSize(0.03);
    leg_HT250_CaloTrigEta->SetLineColor(1);
    leg_HT250_CaloTrigEta->SetLineStyle(1);
    leg_HT250_CaloTrigEta->SetLineWidth(1);
    leg_HT250_CaloTrigEta->SetFillColor(0);
    leg_HT250_CaloTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloTrigEta = leg_HT250_CaloTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloTrigEta->SetMarkerColor(9);
    entry_HT250_CaloTrigEta->SetMarkerStyle(26);
    entry_HT250_CaloTrigEta->SetMarkerSize(2);
    entry_HT250_CaloTrigEta->SetTextFont(42);
    entry_HT250_CaloTrigEta = leg_HT250_CaloTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloTrigEta->SetMarkerColor(8);
    entry_HT250_CaloTrigEta->SetMarkerStyle(4);
    entry_HT250_CaloTrigEta->SetMarkerSize(2);
    entry_HT250_CaloTrigEta->SetTextFont(42);
    entry_HT250_CaloTrigEta = leg_HT250_CaloTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloTrigEta->SetMarkerColor(2);
    entry_HT250_CaloTrigEta->SetMarkerStyle(5);
    entry_HT250_CaloTrigEta->SetMarkerSize(2);
    entry_HT250_CaloTrigEta->SetTextFont(42);
    entry_HT250_CaloTrigEta = leg_HT250_CaloTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloTrigEta->SetMarkerColor(1);
    entry_HT250_CaloTrigEta->SetMarkerStyle(2);
    entry_HT250_CaloTrigEta->SetMarkerSize(2);
    entry_HT250_CaloTrigEta->SetTextFont(42);
    entry_HT250_CaloTrigEta = leg_HT250_CaloTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloTrigEta->SetMarkerColor(4);
    entry_HT250_CaloTrigEta->SetMarkerStyle(3);
    entry_HT250_CaloTrigEta->SetMarkerSize(2);
    entry_HT250_CaloTrigEta->SetTextFont(42);
    leg_HT250_CaloTrigEta->Draw("HIST");

    Cavnas_HT250_CaloTrigEta->Modified();
    Cavnas_HT250_CaloTrigEta->cd();
    Cavnas_HT250_CaloTrigEta->SetSelected(Cavnas_HT250_CaloTrigEta);
    Cavnas_HT250_CaloTrigEta->SaveAs( "muTrig_eta_eff.pdf" );

    //

    TH1F* h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT250_CaloBtagTrig");
    TH1F* h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT250_CaloBtagTrig");

    TCanvas *Cavnas_HT250_CaloBtagTrigEta = new TCanvas("Cavnas_HT250_CaloBtagTrigEta", "Cavnas_HT250_CaloBtagTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloBtagTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloBtagTrigEta->SetFillColor(0);
    Cavnas_HT250_CaloBtagTrigEta->SetBorderMode(0);
    Cavnas_HT250_CaloBtagTrigEta->SetBorderSize(2);
    Cavnas_HT250_CaloBtagTrigEta->SetLeftMargin(0.15);
    Cavnas_HT250_CaloBtagTrigEta->SetRightMargin(0.05);
    Cavnas_HT250_CaloBtagTrigEta->SetTopMargin(0.05);
    Cavnas_HT250_CaloBtagTrigEta->SetBottomMargin(0.15);
    Cavnas_HT250_CaloBtagTrigEta->SetGridy();
    Cavnas_HT250_CaloBtagTrigEta->SetTickx(1);
    Cavnas_HT250_CaloBtagTrigEta->SetTicky(1);
    Cavnas_HT250_CaloBtagTrigEta->SetFrameBorderMode(0);
    Cavnas_HT250_CaloBtagTrigEta->cd();

    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo btag trigger efficiency");
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloBtagTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT250_CaloBtagTrigEta->SetBorderSize(0);
    leg_HT250_CaloBtagTrigEta->SetTextSize(0.03);
    leg_HT250_CaloBtagTrigEta->SetLineColor(1);
    leg_HT250_CaloBtagTrigEta->SetLineStyle(1);
    leg_HT250_CaloBtagTrigEta->SetLineWidth(1);
    leg_HT250_CaloBtagTrigEta->SetFillColor(0);
    leg_HT250_CaloBtagTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloBtagTrigEta = leg_HT250_CaloBtagTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloBtagTrigEta->SetMarkerColor(9);
    entry_HT250_CaloBtagTrigEta->SetMarkerStyle(26);
    entry_HT250_CaloBtagTrigEta->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigEta->SetTextFont(42);
    entry_HT250_CaloBtagTrigEta = leg_HT250_CaloBtagTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloBtagTrigEta->SetMarkerColor(8);
    entry_HT250_CaloBtagTrigEta->SetMarkerStyle(4);
    entry_HT250_CaloBtagTrigEta->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigEta->SetTextFont(42);
    entry_HT250_CaloBtagTrigEta = leg_HT250_CaloBtagTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloBtagTrigEta->SetMarkerColor(2);
    entry_HT250_CaloBtagTrigEta->SetMarkerStyle(5);
    entry_HT250_CaloBtagTrigEta->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigEta->SetTextFont(42);
    entry_HT250_CaloBtagTrigEta = leg_HT250_CaloBtagTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloBtagTrigEta->SetMarkerColor(1);
    entry_HT250_CaloBtagTrigEta->SetMarkerStyle(2);
    entry_HT250_CaloBtagTrigEta->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigEta->SetTextFont(42);
    entry_HT250_CaloBtagTrigEta = leg_HT250_CaloBtagTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloBtagTrigEta->SetMarkerColor(4);
    entry_HT250_CaloBtagTrigEta->SetMarkerStyle(3);
    entry_HT250_CaloBtagTrigEta->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigEta->SetTextFont(42);
    leg_HT250_CaloBtagTrigEta->Draw("HIST");

    Cavnas_HT250_CaloBtagTrigEta->Modified();
    Cavnas_HT250_CaloBtagTrigEta->cd();
    Cavnas_HT250_CaloBtagTrigEta->SetSelected(Cavnas_HT250_CaloBtagTrigEta);
    Cavnas_HT250_CaloBtagTrigEta->SaveAs( "HT250_CaloBtagTrig_eta_eff.pdf" );

    //

    TH1F* h_leadingMuonEta_HT410_PFEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT410_PF");
    TH1F* h_leadingMuonEta_HT410_PFEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT410_PF");
    TH1F* h_leadingMuonEta_HT410_PFEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT410_PF");
    TH1F* h_leadingMuonEta_HT410_PFEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT410_PF");
    TH1F* h_leadingMuonEta_HT410_PFEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT410_PF");

    TCanvas *Cavnas_HT410_PFEta = new TCanvas("Cavnas_HT410_PFEta", "Cavnas_HT410_PFEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFEta->SetFillColor(0);
    Cavnas_HT410_PFEta->SetBorderMode(0);
    Cavnas_HT410_PFEta->SetBorderSize(2);
    Cavnas_HT410_PFEta->SetLeftMargin(0.15);
    Cavnas_HT410_PFEta->SetRightMargin(0.05);
    Cavnas_HT410_PFEta->SetTopMargin(0.05);
    Cavnas_HT410_PFEta->SetBottomMargin(0.15);
    Cavnas_HT410_PFEta->SetGridy();
    Cavnas_HT410_PFEta->SetTickx(1);
    Cavnas_HT410_PFEta->SetTicky(1);
    Cavnas_HT410_PFEta->SetFrameBorderMode(0);
    Cavnas_HT410_PFEta->cd();

    h_leadingMuonEta_HT410_PFEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_PFEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT410_PFEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT410_PFEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT410_PFEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT410_PFEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT410_PFEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT410_PFEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT410_PFEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT410_PFEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT410_PFEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT410_PFEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT410_PFEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT410_PFEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT410_PFEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT410_PFEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT410_PFEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT410_PFEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT410_PFEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_PFEta->SetBorderSize(0);
    leg_HT410_PFEta->SetTextSize(0.03);
    leg_HT410_PFEta->SetLineColor(1);
    leg_HT410_PFEta->SetLineStyle(1);
    leg_HT410_PFEta->SetLineWidth(1);
    leg_HT410_PFEta->SetFillColor(0);
    leg_HT410_PFEta->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFEta = leg_HT410_PFEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFEta->SetMarkerColor(9);
    entry_HT410_PFEta->SetMarkerStyle(26);
    entry_HT410_PFEta->SetMarkerSize(2);
    entry_HT410_PFEta->SetTextFont(42);
    entry_HT410_PFEta = leg_HT410_PFEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFEta->SetMarkerColor(8);
    entry_HT410_PFEta->SetMarkerStyle(4);
    entry_HT410_PFEta->SetMarkerSize(2);
    entry_HT410_PFEta->SetTextFont(42);
    entry_HT410_PFEta = leg_HT410_PFEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFEta->SetMarkerColor(2);
    entry_HT410_PFEta->SetMarkerStyle(5);
    entry_HT410_PFEta->SetMarkerSize(2);
    entry_HT410_PFEta->SetTextFont(42);
    entry_HT410_PFEta = leg_HT410_PFEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFEta->SetMarkerColor(1);
    entry_HT410_PFEta->SetMarkerStyle(2);
    entry_HT410_PFEta->SetMarkerSize(2);
    entry_HT410_PFEta->SetTextFont(42);
    entry_HT410_PFEta = leg_HT410_PFEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFEta->SetMarkerColor(4);
    entry_HT410_PFEta->SetMarkerStyle(3);
    entry_HT410_PFEta->SetMarkerSize(2);
    entry_HT410_PFEta->SetTextFont(42);
    leg_HT410_PFEta->Draw("HIST");

    Cavnas_HT410_PFEta->Modified();
    Cavnas_HT410_PFEta->cd();
    Cavnas_HT410_PFEta->SetSelected(Cavnas_HT410_PFEta);
    Cavnas_HT410_PFEta->SaveAs( "HT410_PFTrig_eta_eff.pdf" );

    TH1F* h_leadingMuonEta_HT410_BtagEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT410_Btag");
    TH1F* h_leadingMuonEta_HT410_BtagEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT410_Btag");
    TH1F* h_leadingMuonEta_HT410_BtagEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT410_Btag");
    TH1F* h_leadingMuonEta_HT410_BtagEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT410_Btag");
    TH1F* h_leadingMuonEta_HT410_BtagEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT410_Btag");

    TCanvas *Cavnas_HT410_BtagEta = new TCanvas("Cavnas_HT410_BtagEta", "Cavnas_HT410_BtagEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagEta->SetFillColor(0);
    Cavnas_HT410_BtagEta->SetBorderMode(0);
    Cavnas_HT410_BtagEta->SetBorderSize(2);
    Cavnas_HT410_BtagEta->SetLeftMargin(0.15);
    Cavnas_HT410_BtagEta->SetRightMargin(0.05);
    Cavnas_HT410_BtagEta->SetTopMargin(0.05);
    Cavnas_HT410_BtagEta->SetBottomMargin(0.15);
    Cavnas_HT410_BtagEta->SetGridy();
    Cavnas_HT410_BtagEta->SetTickx(1);
    Cavnas_HT410_BtagEta->SetTicky(1);
    Cavnas_HT410_BtagEta->SetFrameBorderMode(0);
    Cavnas_HT410_BtagEta->cd();

    h_leadingMuonEta_HT410_BtagEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_BtagEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT410_BtagEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT410_BtagEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT410_BtagEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT410_BtagEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT410_BtagEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT410_BtagEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT410_BtagEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT410_BtagEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT410_BtagEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT410_BtagEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT410_BtagEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT410_BtagEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT410_BtagEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT410_BtagEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT410_BtagEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT410_BtagEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT410_BtagEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_BtagEta->SetBorderSize(0);
    leg_HT410_BtagEta->SetTextSize(0.03);
    leg_HT410_BtagEta->SetLineColor(1);
    leg_HT410_BtagEta->SetLineStyle(1);
    leg_HT410_BtagEta->SetLineWidth(1);
    leg_HT410_BtagEta->SetFillColor(0);
    leg_HT410_BtagEta->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagEta = leg_HT410_BtagEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagEta->SetMarkerColor(9);
    entry_HT410_BtagEta->SetMarkerStyle(26);
    entry_HT410_BtagEta->SetMarkerSize(2);
    entry_HT410_BtagEta->SetTextFont(42);
    entry_HT410_BtagEta = leg_HT410_BtagEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagEta->SetMarkerColor(8);
    entry_HT410_BtagEta->SetMarkerStyle(4);
    entry_HT410_BtagEta->SetMarkerSize(2);
    entry_HT410_BtagEta->SetTextFont(42);
    entry_HT410_BtagEta = leg_HT410_BtagEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagEta->SetMarkerColor(2);
    entry_HT410_BtagEta->SetMarkerStyle(5);
    entry_HT410_BtagEta->SetMarkerSize(2);
    entry_HT410_BtagEta->SetTextFont(42);
    entry_HT410_BtagEta = leg_HT410_BtagEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagEta->SetMarkerColor(1);
    entry_HT410_BtagEta->SetMarkerStyle(2);
    entry_HT410_BtagEta->SetMarkerSize(2);
    entry_HT410_BtagEta->SetTextFont(42);
    entry_HT410_BtagEta = leg_HT410_BtagEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagEta->SetMarkerColor(4);
    entry_HT410_BtagEta->SetMarkerStyle(3);
    entry_HT410_BtagEta->SetMarkerSize(2);
    entry_HT410_BtagEta->SetTextFont(42);
    leg_HT410_BtagEta->Draw("HIST");

    Cavnas_HT410_BtagEta->Modified();
    Cavnas_HT410_BtagEta->cd();
    Cavnas_HT410_BtagEta->SetSelected(Cavnas_HT410_BtagEta);
    Cavnas_HT410_BtagEta->SaveAs( "HT410_BtagTrig_eta_eff.pdf" );

    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_DoubleMuNoVtxTrig");
    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_DoubleMuNoVtxTrig");

    TCanvas *Cavnas_DoubleMuNoVtxTrigEta = new TCanvas("Cavnas_DoubleMuNoVtxTrigEta", "Cavnas_DoubleMuNoVtxTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_DoubleMuNoVtxTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_DoubleMuNoVtxTrigEta->SetFillColor(0);
    Cavnas_DoubleMuNoVtxTrigEta->SetBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigEta->SetBorderSize(2);
    Cavnas_DoubleMuNoVtxTrigEta->SetLeftMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigEta->SetRightMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigEta->SetTopMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigEta->SetBottomMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigEta->SetGridy();
    Cavnas_DoubleMuNoVtxTrigEta->SetTickx(1);
    Cavnas_DoubleMuNoVtxTrigEta->SetTicky(1);
    Cavnas_DoubleMuNoVtxTrigEta->SetFrameBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigEta->cd();

    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetTitle("#mu#mu no vtx trigger efficiency");
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_DoubleMuNoVtxTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_DoubleMuNoVtxTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_DoubleMuNoVtxTrigEta->SetBorderSize(0);
    leg_DoubleMuNoVtxTrigEta->SetTextSize(0.03);
    leg_DoubleMuNoVtxTrigEta->SetLineColor(1);
    leg_DoubleMuNoVtxTrigEta->SetLineStyle(1);
    leg_DoubleMuNoVtxTrigEta->SetLineWidth(1);
    leg_DoubleMuNoVtxTrigEta->SetFillColor(0);
    leg_DoubleMuNoVtxTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_DoubleMuNoVtxTrigEta = leg_DoubleMuNoVtxTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_DoubleMuNoVtxTrigEta->SetMarkerColor(9);
    entry_DoubleMuNoVtxTrigEta->SetMarkerStyle(26);
    entry_DoubleMuNoVtxTrigEta->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigEta->SetTextFont(42);
    entry_DoubleMuNoVtxTrigEta = leg_DoubleMuNoVtxTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_DoubleMuNoVtxTrigEta->SetMarkerColor(8);
    entry_DoubleMuNoVtxTrigEta->SetMarkerStyle(4);
    entry_DoubleMuNoVtxTrigEta->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigEta->SetTextFont(42);
    entry_DoubleMuNoVtxTrigEta = leg_DoubleMuNoVtxTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_DoubleMuNoVtxTrigEta->SetMarkerColor(2);
    entry_DoubleMuNoVtxTrigEta->SetMarkerStyle(5);
    entry_DoubleMuNoVtxTrigEta->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigEta->SetTextFont(42);
    entry_DoubleMuNoVtxTrigEta = leg_DoubleMuNoVtxTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_DoubleMuNoVtxTrigEta->SetMarkerColor(1);
    entry_DoubleMuNoVtxTrigEta->SetMarkerStyle(2);
    entry_DoubleMuNoVtxTrigEta->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigEta->SetTextFont(42);
    entry_DoubleMuNoVtxTrigEta = leg_DoubleMuNoVtxTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_DoubleMuNoVtxTrigEta->SetMarkerColor(4);
    entry_DoubleMuNoVtxTrigEta->SetMarkerStyle(3);
    entry_DoubleMuNoVtxTrigEta->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigEta->SetTextFont(42);
    leg_DoubleMuNoVtxTrigEta->Draw("HIST");

    Cavnas_DoubleMuNoVtxTrigEta->Modified();
    Cavnas_DoubleMuNoVtxTrigEta->cd();
    Cavnas_DoubleMuNoVtxTrigEta->SetSelected(Cavnas_DoubleMuNoVtxTrigEta);
    Cavnas_DoubleMuNoVtxTrigEta->SaveAs( "DoubleMuNoVtxTrig_eta_eff.pdf" );

    // DeltaR

    TH1F* h_delR_HT250_CaloTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT250_CaloTrig");
    TH1F* h_delR_HT250_CaloTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT250_CaloTrig");
    TH1F* h_delR_HT250_CaloTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT250_CaloTrig");
    TH1F* h_delR_HT250_CaloTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT250_CaloTrig");
    TH1F* h_delR_HT250_CaloTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT250_CaloTrig");

    TCanvas *Cavnas_HT250_CaloTrigDelR = new TCanvas("Cavnas_HT250_CaloTrigDelR", "Cavnas_HT250_CaloTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloTrigDelR->SetFillColor(0);
    Cavnas_HT250_CaloTrigDelR->SetBorderMode(0);
    Cavnas_HT250_CaloTrigDelR->SetBorderSize(2);
    Cavnas_HT250_CaloTrigDelR->SetLeftMargin(0.15);
    Cavnas_HT250_CaloTrigDelR->SetRightMargin(0.05);
    Cavnas_HT250_CaloTrigDelR->SetTopMargin(0.05);
    Cavnas_HT250_CaloTrigDelR->SetBottomMargin(0.15);
    Cavnas_HT250_CaloTrigDelR->SetGridy();
    Cavnas_HT250_CaloTrigDelR->SetTickx(1);
    Cavnas_HT250_CaloTrigDelR->SetTicky(1);
    Cavnas_HT250_CaloTrigDelR->SetFrameBorderMode(0);
    Cavnas_HT250_CaloTrigDelR->cd();

    h_delR_HT250_CaloTrigDelR_ctau0->SetTitle("");
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT250_CaloTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo trigger efficiency");
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT250_CaloTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT250_CaloTrigDelR_ctau0->SetLineColor(9);
    h_delR_HT250_CaloTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_HT250_CaloTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT250_CaloTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_HT250_CaloTrigDelR_ctau0->Draw("HIST p");

    h_delR_HT250_CaloTrigDelR_ctau1->SetLineColor(8);
    h_delR_HT250_CaloTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_HT250_CaloTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT250_CaloTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_HT250_CaloTrigDelR_ctau1->Draw("HIST p same");

    h_delR_HT250_CaloTrigDelR_ctau10->SetLineColor(2);
    h_delR_HT250_CaloTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_HT250_CaloTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT250_CaloTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_HT250_CaloTrigDelR_ctau10->Draw("HIST p same");

    h_delR_HT250_CaloTrigDelR_ctau100->SetLineColor(1);
    h_delR_HT250_CaloTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_HT250_CaloTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT250_CaloTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_HT250_CaloTrigDelR_ctau100->Draw("HIST p same");

    h_delR_HT250_CaloTrigDelR_ctau1000->SetLineColor(4);
    h_delR_HT250_CaloTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT250_CaloTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT250_CaloTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT250_CaloTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT250_CaloTrigDelR->SetBorderSize(0);
    leg_HT250_CaloTrigDelR->SetTextSize(0.03);
    leg_HT250_CaloTrigDelR->SetLineColor(1);
    leg_HT250_CaloTrigDelR->SetLineStyle(1);
    leg_HT250_CaloTrigDelR->SetLineWidth(1);
    leg_HT250_CaloTrigDelR->SetFillColor(0);
    leg_HT250_CaloTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloTrigDelR = leg_HT250_CaloTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloTrigDelR->SetMarkerColor(9);
    entry_HT250_CaloTrigDelR->SetMarkerStyle(26);
    entry_HT250_CaloTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloTrigDelR->SetTextFont(42);
    entry_HT250_CaloTrigDelR = leg_HT250_CaloTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloTrigDelR->SetMarkerColor(8);
    entry_HT250_CaloTrigDelR->SetMarkerStyle(4);
    entry_HT250_CaloTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloTrigDelR->SetTextFont(42);
    entry_HT250_CaloTrigDelR = leg_HT250_CaloTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloTrigDelR->SetMarkerColor(2);
    entry_HT250_CaloTrigDelR->SetMarkerStyle(5);
    entry_HT250_CaloTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloTrigDelR->SetTextFont(42);
    entry_HT250_CaloTrigDelR = leg_HT250_CaloTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloTrigDelR->SetMarkerColor(1);
    entry_HT250_CaloTrigDelR->SetMarkerStyle(2);
    entry_HT250_CaloTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloTrigDelR->SetTextFont(42);
    entry_HT250_CaloTrigDelR = leg_HT250_CaloTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloTrigDelR->SetMarkerColor(4);
    entry_HT250_CaloTrigDelR->SetMarkerStyle(3);
    entry_HT250_CaloTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloTrigDelR->SetTextFont(42);
    leg_HT250_CaloTrigDelR->Draw("HIST");

    Cavnas_HT250_CaloTrigDelR->Modified();
    Cavnas_HT250_CaloTrigDelR->cd();
    Cavnas_HT250_CaloTrigDelR->SetSelected(Cavnas_HT250_CaloTrigDelR);
    Cavnas_HT250_CaloTrigDelR->SaveAs( "muTrig_DelR_eff.pdf" );

    //

    TH1F* h_delR_HT250_CaloBtagTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT250_CaloBtagTrig");
    TH1F* h_delR_HT250_CaloBtagTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT250_CaloBtagTrig");
    TH1F* h_delR_HT250_CaloBtagTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT250_CaloBtagTrig");
    TH1F* h_delR_HT250_CaloBtagTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT250_CaloBtagTrig");
    TH1F* h_delR_HT250_CaloBtagTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT250_CaloBtagTrig");

    TCanvas *Cavnas_HT250_CaloBtagTrigDelR = new TCanvas("Cavnas_HT250_CaloBtagTrigDelR", "Cavnas_HT250_CaloBtagTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloBtagTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloBtagTrigDelR->SetFillColor(0);
    Cavnas_HT250_CaloBtagTrigDelR->SetBorderMode(0);
    Cavnas_HT250_CaloBtagTrigDelR->SetBorderSize(2);
    Cavnas_HT250_CaloBtagTrigDelR->SetLeftMargin(0.15);
    Cavnas_HT250_CaloBtagTrigDelR->SetRightMargin(0.05);
    Cavnas_HT250_CaloBtagTrigDelR->SetTopMargin(0.05);
    Cavnas_HT250_CaloBtagTrigDelR->SetBottomMargin(0.15);
    Cavnas_HT250_CaloBtagTrigDelR->SetGridy();
    Cavnas_HT250_CaloBtagTrigDelR->SetTickx(1);
    Cavnas_HT250_CaloBtagTrigDelR->SetTicky(1);
    Cavnas_HT250_CaloBtagTrigDelR->SetFrameBorderMode(0);
    Cavnas_HT250_CaloBtagTrigDelR->cd();

    h_delR_HT250_CaloBtagTrigDelR_ctau0->SetTitle("");
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo btag trigger efficiency");
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->SetLineColor(9);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau0->Draw("HIST p");

    h_delR_HT250_CaloBtagTrigDelR_ctau1->SetLineColor(8);
    h_delR_HT250_CaloBtagTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_HT250_CaloBtagTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT250_CaloBtagTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau1->Draw("HIST p same");

    h_delR_HT250_CaloBtagTrigDelR_ctau10->SetLineColor(2);
    h_delR_HT250_CaloBtagTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_HT250_CaloBtagTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT250_CaloBtagTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau10->Draw("HIST p same");

    h_delR_HT250_CaloBtagTrigDelR_ctau100->SetLineColor(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT250_CaloBtagTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau100->Draw("HIST p same");

    h_delR_HT250_CaloBtagTrigDelR_ctau1000->SetLineColor(4);
    h_delR_HT250_CaloBtagTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT250_CaloBtagTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT250_CaloBtagTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT250_CaloBtagTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloBtagTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT250_CaloBtagTrigDelR->SetBorderSize(0);
    leg_HT250_CaloBtagTrigDelR->SetTextSize(0.03);
    leg_HT250_CaloBtagTrigDelR->SetLineColor(1);
    leg_HT250_CaloBtagTrigDelR->SetLineStyle(1);
    leg_HT250_CaloBtagTrigDelR->SetLineWidth(1);
    leg_HT250_CaloBtagTrigDelR->SetFillColor(0);
    leg_HT250_CaloBtagTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloBtagTrigDelR = leg_HT250_CaloBtagTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloBtagTrigDelR->SetMarkerColor(9);
    entry_HT250_CaloBtagTrigDelR->SetMarkerStyle(26);
    entry_HT250_CaloBtagTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigDelR->SetTextFont(42);
    entry_HT250_CaloBtagTrigDelR = leg_HT250_CaloBtagTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloBtagTrigDelR->SetMarkerColor(8);
    entry_HT250_CaloBtagTrigDelR->SetMarkerStyle(4);
    entry_HT250_CaloBtagTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigDelR->SetTextFont(42);
    entry_HT250_CaloBtagTrigDelR = leg_HT250_CaloBtagTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloBtagTrigDelR->SetMarkerColor(2);
    entry_HT250_CaloBtagTrigDelR->SetMarkerStyle(5);
    entry_HT250_CaloBtagTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigDelR->SetTextFont(42);
    entry_HT250_CaloBtagTrigDelR = leg_HT250_CaloBtagTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloBtagTrigDelR->SetMarkerColor(1);
    entry_HT250_CaloBtagTrigDelR->SetMarkerStyle(2);
    entry_HT250_CaloBtagTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigDelR->SetTextFont(42);
    entry_HT250_CaloBtagTrigDelR = leg_HT250_CaloBtagTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloBtagTrigDelR->SetMarkerColor(4);
    entry_HT250_CaloBtagTrigDelR->SetMarkerStyle(3);
    entry_HT250_CaloBtagTrigDelR->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigDelR->SetTextFont(42);
    leg_HT250_CaloBtagTrigDelR->Draw("HIST");

    Cavnas_HT250_CaloBtagTrigDelR->Modified();
    Cavnas_HT250_CaloBtagTrigDelR->cd();
    Cavnas_HT250_CaloBtagTrigDelR->SetSelected(Cavnas_HT250_CaloBtagTrigDelR);
    Cavnas_HT250_CaloBtagTrigDelR->SaveAs( "mumuTrig_DelR_eff.pdf" );

    //

    TH1F* h_delR_HT410_PFDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT410_PF");
    TH1F* h_delR_HT410_PFDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT410_PF");
    TH1F* h_delR_HT410_PFDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT410_PF");
    TH1F* h_delR_HT410_PFDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT410_PF");
    TH1F* h_delR_HT410_PFDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT410_PF");

    TCanvas *Cavnas_HT410_PFDelR = new TCanvas("Cavnas_HT410_PFDelR", "Cavnas_HT410_PFDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFDelR->SetFillColor(0);
    Cavnas_HT410_PFDelR->SetBorderMode(0);
    Cavnas_HT410_PFDelR->SetBorderSize(2);
    Cavnas_HT410_PFDelR->SetLeftMargin(0.15);
    Cavnas_HT410_PFDelR->SetRightMargin(0.05);
    Cavnas_HT410_PFDelR->SetTopMargin(0.05);
    Cavnas_HT410_PFDelR->SetBottomMargin(0.15);
    Cavnas_HT410_PFDelR->SetGridy();
    Cavnas_HT410_PFDelR->SetTickx(1);
    Cavnas_HT410_PFDelR->SetTicky(1);
    Cavnas_HT410_PFDelR->SetFrameBorderMode(0);
    Cavnas_HT410_PFDelR->cd();

    h_delR_HT410_PFDelR_ctau0->SetTitle("");
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT410_PFDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT410_PFDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT410_PFDelR_ctau0->SetLineColor(9);
    h_delR_HT410_PFDelR_ctau0->SetMarkerColor(9);
    h_delR_HT410_PFDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT410_PFDelR_ctau0->SetMarkerSize(1);
    h_delR_HT410_PFDelR_ctau0->Draw("HIST p");

    h_delR_HT410_PFDelR_ctau1->SetLineColor(8);
    h_delR_HT410_PFDelR_ctau1->SetMarkerColor(8);
    h_delR_HT410_PFDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT410_PFDelR_ctau1->SetMarkerSize(1);
    h_delR_HT410_PFDelR_ctau1->Draw("HIST p same");

    h_delR_HT410_PFDelR_ctau10->SetLineColor(2);
    h_delR_HT410_PFDelR_ctau10->SetMarkerColor(2);
    h_delR_HT410_PFDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT410_PFDelR_ctau10->SetMarkerSize(1);
    h_delR_HT410_PFDelR_ctau10->Draw("HIST p same");

    h_delR_HT410_PFDelR_ctau100->SetLineColor(1);
    h_delR_HT410_PFDelR_ctau100->SetMarkerColor(1);
    h_delR_HT410_PFDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT410_PFDelR_ctau100->SetMarkerSize(1);
    h_delR_HT410_PFDelR_ctau100->Draw("HIST p same");

    h_delR_HT410_PFDelR_ctau1000->SetLineColor(4);
    h_delR_HT410_PFDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT410_PFDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT410_PFDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT410_PFDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_PFDelR->SetBorderSize(0);
    leg_HT410_PFDelR->SetTextSize(0.03);
    leg_HT410_PFDelR->SetLineColor(1);
    leg_HT410_PFDelR->SetLineStyle(1);
    leg_HT410_PFDelR->SetLineWidth(1);
    leg_HT410_PFDelR->SetFillColor(0);
    leg_HT410_PFDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFDelR = leg_HT410_PFDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFDelR->SetMarkerColor(9);
    entry_HT410_PFDelR->SetMarkerStyle(26);
    entry_HT410_PFDelR->SetMarkerSize(2);
    entry_HT410_PFDelR->SetTextFont(42);
    entry_HT410_PFDelR = leg_HT410_PFDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFDelR->SetMarkerColor(8);
    entry_HT410_PFDelR->SetMarkerStyle(4);
    entry_HT410_PFDelR->SetMarkerSize(2);
    entry_HT410_PFDelR->SetTextFont(42);
    entry_HT410_PFDelR = leg_HT410_PFDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFDelR->SetMarkerColor(2);
    entry_HT410_PFDelR->SetMarkerStyle(5);
    entry_HT410_PFDelR->SetMarkerSize(2);
    entry_HT410_PFDelR->SetTextFont(42);
    entry_HT410_PFDelR = leg_HT410_PFDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFDelR->SetMarkerColor(1);
    entry_HT410_PFDelR->SetMarkerStyle(2);
    entry_HT410_PFDelR->SetMarkerSize(2);
    entry_HT410_PFDelR->SetTextFont(42);
    entry_HT410_PFDelR = leg_HT410_PFDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFDelR->SetMarkerColor(4);
    entry_HT410_PFDelR->SetMarkerStyle(3);
    entry_HT410_PFDelR->SetMarkerSize(2);
    entry_HT410_PFDelR->SetTextFont(42);
    leg_HT410_PFDelR->Draw("HIST");

    Cavnas_HT410_PFDelR->Modified();
    Cavnas_HT410_PFDelR->cd();
    Cavnas_HT410_PFDelR->SetSelected(Cavnas_HT410_PFDelR);
    Cavnas_HT410_PFDelR->SaveAs( "HT410_PFTrig_DelR_eff.pdf" );

    TH1F* h_delR_HT410_BtagDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT410_Btag");
    TH1F* h_delR_HT410_BtagDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT410_Btag");
    TH1F* h_delR_HT410_BtagDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT410_Btag");
    TH1F* h_delR_HT410_BtagDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT410_Btag");
    TH1F* h_delR_HT410_BtagDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT410_Btag");

    TCanvas *Cavnas_HT410_BtagDelR = new TCanvas("Cavnas_HT410_BtagDelR", "Cavnas_HT410_BtagDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagDelR->SetFillColor(0);
    Cavnas_HT410_BtagDelR->SetBorderMode(0);
    Cavnas_HT410_BtagDelR->SetBorderSize(2);
    Cavnas_HT410_BtagDelR->SetLeftMargin(0.15);
    Cavnas_HT410_BtagDelR->SetRightMargin(0.05);
    Cavnas_HT410_BtagDelR->SetTopMargin(0.05);
    Cavnas_HT410_BtagDelR->SetBottomMargin(0.15);
    Cavnas_HT410_BtagDelR->SetGridy();
    Cavnas_HT410_BtagDelR->SetTickx(1);
    Cavnas_HT410_BtagDelR->SetTicky(1);
    Cavnas_HT410_BtagDelR->SetFrameBorderMode(0);
    Cavnas_HT410_BtagDelR->cd();

    h_delR_HT410_BtagDelR_ctau0->SetTitle("");
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT410_BtagDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT410_BtagDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT410_BtagDelR_ctau0->SetLineColor(9);
    h_delR_HT410_BtagDelR_ctau0->SetMarkerColor(9);
    h_delR_HT410_BtagDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT410_BtagDelR_ctau0->SetMarkerSize(1);
    h_delR_HT410_BtagDelR_ctau0->Draw("HIST p");

    h_delR_HT410_BtagDelR_ctau1->SetLineColor(8);
    h_delR_HT410_BtagDelR_ctau1->SetMarkerColor(8);
    h_delR_HT410_BtagDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT410_BtagDelR_ctau1->SetMarkerSize(1);
    h_delR_HT410_BtagDelR_ctau1->Draw("HIST p same");

    h_delR_HT410_BtagDelR_ctau10->SetLineColor(2);
    h_delR_HT410_BtagDelR_ctau10->SetMarkerColor(2);
    h_delR_HT410_BtagDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT410_BtagDelR_ctau10->SetMarkerSize(1);
    h_delR_HT410_BtagDelR_ctau10->Draw("HIST p same");

    h_delR_HT410_BtagDelR_ctau100->SetLineColor(1);
    h_delR_HT410_BtagDelR_ctau100->SetMarkerColor(1);
    h_delR_HT410_BtagDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT410_BtagDelR_ctau100->SetMarkerSize(1);
    h_delR_HT410_BtagDelR_ctau100->Draw("HIST p same");

    h_delR_HT410_BtagDelR_ctau1000->SetLineColor(4);
    h_delR_HT410_BtagDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT410_BtagDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT410_BtagDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT410_BtagDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_BtagDelR->SetBorderSize(0);
    leg_HT410_BtagDelR->SetTextSize(0.03);
    leg_HT410_BtagDelR->SetLineColor(1);
    leg_HT410_BtagDelR->SetLineStyle(1);
    leg_HT410_BtagDelR->SetLineWidth(1);
    leg_HT410_BtagDelR->SetFillColor(0);
    leg_HT410_BtagDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagDelR = leg_HT410_BtagDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagDelR->SetMarkerColor(9);
    entry_HT410_BtagDelR->SetMarkerStyle(26);
    entry_HT410_BtagDelR->SetMarkerSize(2);
    entry_HT410_BtagDelR->SetTextFont(42);
    entry_HT410_BtagDelR = leg_HT410_BtagDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagDelR->SetMarkerColor(8);
    entry_HT410_BtagDelR->SetMarkerStyle(4);
    entry_HT410_BtagDelR->SetMarkerSize(2);
    entry_HT410_BtagDelR->SetTextFont(42);
    entry_HT410_BtagDelR = leg_HT410_BtagDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagDelR->SetMarkerColor(2);
    entry_HT410_BtagDelR->SetMarkerStyle(5);
    entry_HT410_BtagDelR->SetMarkerSize(2);
    entry_HT410_BtagDelR->SetTextFont(42);
    entry_HT410_BtagDelR = leg_HT410_BtagDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagDelR->SetMarkerColor(1);
    entry_HT410_BtagDelR->SetMarkerStyle(2);
    entry_HT410_BtagDelR->SetMarkerSize(2);
    entry_HT410_BtagDelR->SetTextFont(42);
    entry_HT410_BtagDelR = leg_HT410_BtagDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagDelR->SetMarkerColor(4);
    entry_HT410_BtagDelR->SetMarkerStyle(3);
    entry_HT410_BtagDelR->SetMarkerSize(2);
    entry_HT410_BtagDelR->SetTextFont(42);
    leg_HT410_BtagDelR->Draw("HIST");

    Cavnas_HT410_BtagDelR->Modified();
    Cavnas_HT410_BtagDelR->cd();
    Cavnas_HT410_BtagDelR->SetSelected(Cavnas_HT410_BtagDelR);
    Cavnas_HT410_BtagDelR->SaveAs( "HT410_BtagTrig_DelR_eff.pdf" );

    TH1F* h_delR_DoubleMuNoVtxTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_DoubleMuNoVtxTrig");
    TH1F* h_delR_DoubleMuNoVtxTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_DoubleMuNoVtxTrig");
    TH1F* h_delR_DoubleMuNoVtxTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_DoubleMuNoVtxTrig");
    TH1F* h_delR_DoubleMuNoVtxTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_DoubleMuNoVtxTrig");
    TH1F* h_delR_DoubleMuNoVtxTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_DoubleMuNoVtxTrig");

    TCanvas *Cavnas_DoubleMuNoVtxTrigDelR = new TCanvas("Cavnas_DoubleMuNoVtxTrigDelR", "Cavnas_DoubleMuNoVtxTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_DoubleMuNoVtxTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_DoubleMuNoVtxTrigDelR->SetFillColor(0);
    Cavnas_DoubleMuNoVtxTrigDelR->SetBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigDelR->SetBorderSize(2);
    Cavnas_DoubleMuNoVtxTrigDelR->SetLeftMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigDelR->SetRightMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigDelR->SetTopMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigDelR->SetBottomMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigDelR->SetGridy();
    Cavnas_DoubleMuNoVtxTrigDelR->SetTickx(1);
    Cavnas_DoubleMuNoVtxTrigDelR->SetTicky(1);
    Cavnas_DoubleMuNoVtxTrigDelR->SetFrameBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigDelR->cd();

    h_delR_DoubleMuNoVtxTrigDelR_ctau0->SetTitle("");
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetTitle("#mu#mu no vtx trigger efficiency");
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->SetLineColor(9);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau0->Draw("HIST p");

    h_delR_DoubleMuNoVtxTrigDelR_ctau1->SetLineColor(8);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1->Draw("HIST p same");

    h_delR_DoubleMuNoVtxTrigDelR_ctau10->SetLineColor(2);
    h_delR_DoubleMuNoVtxTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_DoubleMuNoVtxTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_DoubleMuNoVtxTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau10->Draw("HIST p same");

    h_delR_DoubleMuNoVtxTrigDelR_ctau100->SetLineColor(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_DoubleMuNoVtxTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau100->Draw("HIST p same");

    h_delR_DoubleMuNoVtxTrigDelR_ctau1000->SetLineColor(4);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_DoubleMuNoVtxTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_DoubleMuNoVtxTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_DoubleMuNoVtxTrigDelR->SetBorderSize(0);
    leg_DoubleMuNoVtxTrigDelR->SetTextSize(0.03);
    leg_DoubleMuNoVtxTrigDelR->SetLineColor(1);
    leg_DoubleMuNoVtxTrigDelR->SetLineStyle(1);
    leg_DoubleMuNoVtxTrigDelR->SetLineWidth(1);
    leg_DoubleMuNoVtxTrigDelR->SetFillColor(0);
    leg_DoubleMuNoVtxTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_DoubleMuNoVtxTrigDelR = leg_DoubleMuNoVtxTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_DoubleMuNoVtxTrigDelR->SetMarkerColor(9);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerStyle(26);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigDelR->SetTextFont(42);
    entry_DoubleMuNoVtxTrigDelR = leg_DoubleMuNoVtxTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_DoubleMuNoVtxTrigDelR->SetMarkerColor(8);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerStyle(4);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigDelR->SetTextFont(42);
    entry_DoubleMuNoVtxTrigDelR = leg_DoubleMuNoVtxTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_DoubleMuNoVtxTrigDelR->SetMarkerColor(2);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerStyle(5);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigDelR->SetTextFont(42);
    entry_DoubleMuNoVtxTrigDelR = leg_DoubleMuNoVtxTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_DoubleMuNoVtxTrigDelR->SetMarkerColor(1);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerStyle(2);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigDelR->SetTextFont(42);
    entry_DoubleMuNoVtxTrigDelR = leg_DoubleMuNoVtxTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_DoubleMuNoVtxTrigDelR->SetMarkerColor(4);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerStyle(3);
    entry_DoubleMuNoVtxTrigDelR->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigDelR->SetTextFont(42);
    leg_DoubleMuNoVtxTrigDelR->Draw("HIST");

    Cavnas_DoubleMuNoVtxTrigDelR->Modified();
    Cavnas_DoubleMuNoVtxTrigDelR->cd();
    Cavnas_DoubleMuNoVtxTrigDelR->SetSelected(Cavnas_DoubleMuNoVtxTrigDelR);
    Cavnas_DoubleMuNoVtxTrigDelR->SaveAs( "DoubleMuNoVtxTrig_DelR_eff.pdf" );

///// mass

    // mass

    TH1F* h_mass_HT250_CaloTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT250_CaloTrig");
    TH1F* h_mass_HT250_CaloTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT250_CaloTrig");
    TH1F* h_mass_HT250_CaloTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT250_CaloTrig");
    TH1F* h_mass_HT250_CaloTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT250_CaloTrig");
    TH1F* h_mass_HT250_CaloTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT250_CaloTrig");

    TCanvas *Cavnas_HT250_CaloTrigMass = new TCanvas("Cavnas_HT250_CaloTrigMass", "Cavnas_HT250_CaloTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloTrigMass->SetFillColor(0);
    Cavnas_HT250_CaloTrigMass->SetBorderMode(0);
    Cavnas_HT250_CaloTrigMass->SetBorderSize(2);
    Cavnas_HT250_CaloTrigMass->SetLeftMargin(0.15);
    Cavnas_HT250_CaloTrigMass->SetRightMargin(0.05);
    Cavnas_HT250_CaloTrigMass->SetTopMargin(0.05);
    Cavnas_HT250_CaloTrigMass->SetBottomMargin(0.15);
    Cavnas_HT250_CaloTrigMass->SetGridy();
    Cavnas_HT250_CaloTrigMass->SetTickx(1);
    Cavnas_HT250_CaloTrigMass->SetTicky(1);
    Cavnas_HT250_CaloTrigMass->SetFrameBorderMode(0);
    Cavnas_HT250_CaloTrigMass->cd();

    h_mass_HT250_CaloTrigMass_ctau0->SetTitle("");
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT250_CaloTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo trigger efficiency");
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT250_CaloTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT250_CaloTrigMass_ctau0->SetLineColor(9);
    h_mass_HT250_CaloTrigMass_ctau0->SetMarkerColor(9);
    h_mass_HT250_CaloTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_HT250_CaloTrigMass_ctau0->SetMarkerSize(1);
    h_mass_HT250_CaloTrigMass_ctau0->Draw("HIST p");

    h_mass_HT250_CaloTrigMass_ctau1->SetLineColor(8);
    h_mass_HT250_CaloTrigMass_ctau1->SetMarkerColor(8);
    h_mass_HT250_CaloTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_HT250_CaloTrigMass_ctau1->SetMarkerSize(1);
    h_mass_HT250_CaloTrigMass_ctau1->Draw("HIST p same");

    h_mass_HT250_CaloTrigMass_ctau10->SetLineColor(2);
    h_mass_HT250_CaloTrigMass_ctau10->SetMarkerColor(2);
    h_mass_HT250_CaloTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_HT250_CaloTrigMass_ctau10->SetMarkerSize(1);
    h_mass_HT250_CaloTrigMass_ctau10->Draw("HIST p same");

    h_mass_HT250_CaloTrigMass_ctau100->SetLineColor(1);
    h_mass_HT250_CaloTrigMass_ctau100->SetMarkerColor(1);
    h_mass_HT250_CaloTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_HT250_CaloTrigMass_ctau100->SetMarkerSize(1);
    h_mass_HT250_CaloTrigMass_ctau100->Draw("HIST p same");

    h_mass_HT250_CaloTrigMass_ctau1000->SetLineColor(4);
    h_mass_HT250_CaloTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_HT250_CaloTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT250_CaloTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_HT250_CaloTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT250_CaloTrigMass->SetBorderSize(0);
    leg_HT250_CaloTrigMass->SetTextSize(0.03);
    leg_HT250_CaloTrigMass->SetLineColor(1);
    leg_HT250_CaloTrigMass->SetLineStyle(1);
    leg_HT250_CaloTrigMass->SetLineWidth(1);
    leg_HT250_CaloTrigMass->SetFillColor(0);
    leg_HT250_CaloTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloTrigMass = leg_HT250_CaloTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloTrigMass->SetMarkerColor(9);
    entry_HT250_CaloTrigMass->SetMarkerStyle(26);
    entry_HT250_CaloTrigMass->SetMarkerSize(2);
    entry_HT250_CaloTrigMass->SetTextFont(42);
    entry_HT250_CaloTrigMass = leg_HT250_CaloTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloTrigMass->SetMarkerColor(8);
    entry_HT250_CaloTrigMass->SetMarkerStyle(4);
    entry_HT250_CaloTrigMass->SetMarkerSize(2);
    entry_HT250_CaloTrigMass->SetTextFont(42);
    entry_HT250_CaloTrigMass = leg_HT250_CaloTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloTrigMass->SetMarkerColor(2);
    entry_HT250_CaloTrigMass->SetMarkerStyle(5);
    entry_HT250_CaloTrigMass->SetMarkerSize(2);
    entry_HT250_CaloTrigMass->SetTextFont(42);
    entry_HT250_CaloTrigMass = leg_HT250_CaloTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloTrigMass->SetMarkerColor(1);
    entry_HT250_CaloTrigMass->SetMarkerStyle(2);
    entry_HT250_CaloTrigMass->SetMarkerSize(2);
    entry_HT250_CaloTrigMass->SetTextFont(42);
    entry_HT250_CaloTrigMass = leg_HT250_CaloTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloTrigMass->SetMarkerColor(4);
    entry_HT250_CaloTrigMass->SetMarkerStyle(3);
    entry_HT250_CaloTrigMass->SetMarkerSize(2);
    entry_HT250_CaloTrigMass->SetTextFont(42);
    leg_HT250_CaloTrigMass->Draw("HIST");

    Cavnas_HT250_CaloTrigMass->Modified();
    Cavnas_HT250_CaloTrigMass->cd();
    Cavnas_HT250_CaloTrigMass->SetSelected(Cavnas_HT250_CaloTrigMass);
    Cavnas_HT250_CaloTrigMass->SaveAs( "muTrig_mass_eff.pdf" );

    //

    TH1F* h_mass_HT250_CaloBtagTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT250_CaloBtagTrig");
    TH1F* h_mass_HT250_CaloBtagTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT250_CaloBtagTrig");
    TH1F* h_mass_HT250_CaloBtagTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT250_CaloBtagTrig");
    TH1F* h_mass_HT250_CaloBtagTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT250_CaloBtagTrig");
    TH1F* h_mass_HT250_CaloBtagTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT250_CaloBtagTrig");

    TCanvas *Cavnas_HT250_CaloBtagTrigMass = new TCanvas("Cavnas_HT250_CaloBtagTrigMass", "Cavnas_HT250_CaloBtagTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT250_CaloBtagTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT250_CaloBtagTrigMass->SetFillColor(0);
    Cavnas_HT250_CaloBtagTrigMass->SetBorderMode(0);
    Cavnas_HT250_CaloBtagTrigMass->SetBorderSize(2);
    Cavnas_HT250_CaloBtagTrigMass->SetLeftMargin(0.15);
    Cavnas_HT250_CaloBtagTrigMass->SetRightMargin(0.05);
    Cavnas_HT250_CaloBtagTrigMass->SetTopMargin(0.05);
    Cavnas_HT250_CaloBtagTrigMass->SetBottomMargin(0.15);
    Cavnas_HT250_CaloBtagTrigMass->SetGridy();
    Cavnas_HT250_CaloBtagTrigMass->SetTickx(1);
    Cavnas_HT250_CaloBtagTrigMass->SetTicky(1);
    Cavnas_HT250_CaloBtagTrigMass->SetFrameBorderMode(0);
    Cavnas_HT250_CaloBtagTrigMass->cd();

    h_mass_HT250_CaloBtagTrigMass_ctau0->SetTitle("");
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetTitle("H_{T} > 250 calo btag trigger efficiency");
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT250_CaloBtagTrigMass_ctau0->SetLineColor(9);
    h_mass_HT250_CaloBtagTrigMass_ctau0->SetMarkerColor(9);
    h_mass_HT250_CaloBtagTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_HT250_CaloBtagTrigMass_ctau0->SetMarkerSize(1);
    h_mass_HT250_CaloBtagTrigMass_ctau0->Draw("HIST p");

    h_mass_HT250_CaloBtagTrigMass_ctau1->SetLineColor(8);
    h_mass_HT250_CaloBtagTrigMass_ctau1->SetMarkerColor(8);
    h_mass_HT250_CaloBtagTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_HT250_CaloBtagTrigMass_ctau1->SetMarkerSize(1);
    h_mass_HT250_CaloBtagTrigMass_ctau1->Draw("HIST p same");

    h_mass_HT250_CaloBtagTrigMass_ctau10->SetLineColor(2);
    h_mass_HT250_CaloBtagTrigMass_ctau10->SetMarkerColor(2);
    h_mass_HT250_CaloBtagTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_HT250_CaloBtagTrigMass_ctau10->SetMarkerSize(1);
    h_mass_HT250_CaloBtagTrigMass_ctau10->Draw("HIST p same");

    h_mass_HT250_CaloBtagTrigMass_ctau100->SetLineColor(1);
    h_mass_HT250_CaloBtagTrigMass_ctau100->SetMarkerColor(1);
    h_mass_HT250_CaloBtagTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_HT250_CaloBtagTrigMass_ctau100->SetMarkerSize(1);
    h_mass_HT250_CaloBtagTrigMass_ctau100->Draw("HIST p same");

    h_mass_HT250_CaloBtagTrigMass_ctau1000->SetLineColor(4);
    h_mass_HT250_CaloBtagTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_HT250_CaloBtagTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT250_CaloBtagTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_HT250_CaloBtagTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT250_CaloBtagTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT250_CaloBtagTrigMass->SetBorderSize(0);
    leg_HT250_CaloBtagTrigMass->SetTextSize(0.03);
    leg_HT250_CaloBtagTrigMass->SetLineColor(1);
    leg_HT250_CaloBtagTrigMass->SetLineStyle(1);
    leg_HT250_CaloBtagTrigMass->SetLineWidth(1);
    leg_HT250_CaloBtagTrigMass->SetFillColor(0);
    leg_HT250_CaloBtagTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_HT250_CaloBtagTrigMass = leg_HT250_CaloBtagTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT250_CaloBtagTrigMass->SetMarkerColor(9);
    entry_HT250_CaloBtagTrigMass->SetMarkerStyle(26);
    entry_HT250_CaloBtagTrigMass->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigMass->SetTextFont(42);
    entry_HT250_CaloBtagTrigMass = leg_HT250_CaloBtagTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT250_CaloBtagTrigMass->SetMarkerColor(8);
    entry_HT250_CaloBtagTrigMass->SetMarkerStyle(4);
    entry_HT250_CaloBtagTrigMass->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigMass->SetTextFont(42);
    entry_HT250_CaloBtagTrigMass = leg_HT250_CaloBtagTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT250_CaloBtagTrigMass->SetMarkerColor(2);
    entry_HT250_CaloBtagTrigMass->SetMarkerStyle(5);
    entry_HT250_CaloBtagTrigMass->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigMass->SetTextFont(42);
    entry_HT250_CaloBtagTrigMass = leg_HT250_CaloBtagTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT250_CaloBtagTrigMass->SetMarkerColor(1);
    entry_HT250_CaloBtagTrigMass->SetMarkerStyle(2);
    entry_HT250_CaloBtagTrigMass->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigMass->SetTextFont(42);
    entry_HT250_CaloBtagTrigMass = leg_HT250_CaloBtagTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT250_CaloBtagTrigMass->SetMarkerColor(4);
    entry_HT250_CaloBtagTrigMass->SetMarkerStyle(3);
    entry_HT250_CaloBtagTrigMass->SetMarkerSize(2);
    entry_HT250_CaloBtagTrigMass->SetTextFont(42);
    leg_HT250_CaloBtagTrigMass->Draw("HIST");

    Cavnas_HT250_CaloBtagTrigMass->Modified();
    Cavnas_HT250_CaloBtagTrigMass->cd();
    Cavnas_HT250_CaloBtagTrigMass->SetSelected(Cavnas_HT250_CaloBtagTrigMass);
    Cavnas_HT250_CaloBtagTrigMass->SaveAs( "mumuTrig_mass_eff.pdf" );

    //


    TH1F* h_mass_HT410_PFMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT410_PF");
    TH1F* h_mass_HT410_PFMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT410_PF");
    TH1F* h_mass_HT410_PFMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT410_PF");
    TH1F* h_mass_HT410_PFMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT410_PF");
    TH1F* h_mass_HT410_PFMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT410_PF");

    TCanvas *Cavnas_HT410_PFMass = new TCanvas("Cavnas_HT410_PFMass", "Cavnas_HT410_PFMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFMass->SetFillColor(0);
    Cavnas_HT410_PFMass->SetBorderMode(0);
    Cavnas_HT410_PFMass->SetBorderSize(2);
    Cavnas_HT410_PFMass->SetLeftMargin(0.15);
    Cavnas_HT410_PFMass->SetRightMargin(0.05);
    Cavnas_HT410_PFMass->SetTopMargin(0.05);
    Cavnas_HT410_PFMass->SetBottomMargin(0.15);
    Cavnas_HT410_PFMass->SetGridy();
    Cavnas_HT410_PFMass->SetTickx(1);
    Cavnas_HT410_PFMass->SetTicky(1);
    Cavnas_HT410_PFMass->SetFrameBorderMode(0);
    Cavnas_HT410_PFMass->cd();

    h_mass_HT410_PFMass_ctau0->SetTitle("");
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT410_PFMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT410_PFMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT410_PFMass_ctau0->SetLineColor(9);
    h_mass_HT410_PFMass_ctau0->SetMarkerColor(9);
    h_mass_HT410_PFMass_ctau0->SetMarkerStyle(26);
    h_mass_HT410_PFMass_ctau0->SetMarkerSize(1);
    h_mass_HT410_PFMass_ctau0->Draw("HIST p");

    h_mass_HT410_PFMass_ctau1->SetLineColor(8);
    h_mass_HT410_PFMass_ctau1->SetMarkerColor(8);
    h_mass_HT410_PFMass_ctau1->SetMarkerStyle(4);
    h_mass_HT410_PFMass_ctau1->SetMarkerSize(1);
    h_mass_HT410_PFMass_ctau1->Draw("HIST p same");

    h_mass_HT410_PFMass_ctau10->SetLineColor(2);
    h_mass_HT410_PFMass_ctau10->SetMarkerColor(2);
    h_mass_HT410_PFMass_ctau10->SetMarkerStyle(5);
    h_mass_HT410_PFMass_ctau10->SetMarkerSize(1);
    h_mass_HT410_PFMass_ctau10->Draw("HIST p same");

    h_mass_HT410_PFMass_ctau100->SetLineColor(1);
    h_mass_HT410_PFMass_ctau100->SetMarkerColor(1);
    h_mass_HT410_PFMass_ctau100->SetMarkerStyle(2);
    h_mass_HT410_PFMass_ctau100->SetMarkerSize(1);
    h_mass_HT410_PFMass_ctau100->Draw("HIST p same");

    h_mass_HT410_PFMass_ctau1000->SetLineColor(4);
    h_mass_HT410_PFMass_ctau1000->SetMarkerColor(4);
    h_mass_HT410_PFMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT410_PFMass_ctau1000->SetMarkerSize(1);
    h_mass_HT410_PFMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_PFMass->SetBorderSize(0);
    leg_HT410_PFMass->SetTextSize(0.03);
    leg_HT410_PFMass->SetLineColor(1);
    leg_HT410_PFMass->SetLineStyle(1);
    leg_HT410_PFMass->SetLineWidth(1);
    leg_HT410_PFMass->SetFillColor(0);
    leg_HT410_PFMass->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFMass = leg_HT410_PFMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFMass->SetMarkerColor(9);
    entry_HT410_PFMass->SetMarkerStyle(26);
    entry_HT410_PFMass->SetMarkerSize(2);
    entry_HT410_PFMass->SetTextFont(42);
    entry_HT410_PFMass = leg_HT410_PFMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFMass->SetMarkerColor(8);
    entry_HT410_PFMass->SetMarkerStyle(4);
    entry_HT410_PFMass->SetMarkerSize(2);
    entry_HT410_PFMass->SetTextFont(42);
    entry_HT410_PFMass = leg_HT410_PFMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFMass->SetMarkerColor(2);
    entry_HT410_PFMass->SetMarkerStyle(5);
    entry_HT410_PFMass->SetMarkerSize(2);
    entry_HT410_PFMass->SetTextFont(42);
    entry_HT410_PFMass = leg_HT410_PFMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFMass->SetMarkerColor(1);
    entry_HT410_PFMass->SetMarkerStyle(2);
    entry_HT410_PFMass->SetMarkerSize(2);
    entry_HT410_PFMass->SetTextFont(42);
    entry_HT410_PFMass = leg_HT410_PFMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFMass->SetMarkerColor(4);
    entry_HT410_PFMass->SetMarkerStyle(3);
    entry_HT410_PFMass->SetMarkerSize(2);
    entry_HT410_PFMass->SetTextFont(42);
    leg_HT410_PFMass->Draw("HIST");

    Cavnas_HT410_PFMass->Modified();
    Cavnas_HT410_PFMass->cd();
    Cavnas_HT410_PFMass->SetSelected(Cavnas_HT410_PFMass);
    Cavnas_HT410_PFMass->SaveAs( "HT410_PFTrig_mass_eff.pdf" );

    TH1F* h_mass_HT410_BtagMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT410_Btag");
    TH1F* h_mass_HT410_BtagMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT410_Btag");
    TH1F* h_mass_HT410_BtagMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT410_Btag");
    TH1F* h_mass_HT410_BtagMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT410_Btag");
    TH1F* h_mass_HT410_BtagMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT410_Btag");

    TCanvas *Cavnas_HT410_BtagMass = new TCanvas("Cavnas_HT410_BtagMass", "Cavnas_HT410_BtagMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagMass->SetFillColor(0);
    Cavnas_HT410_BtagMass->SetBorderMode(0);
    Cavnas_HT410_BtagMass->SetBorderSize(2);
    Cavnas_HT410_BtagMass->SetLeftMargin(0.15);
    Cavnas_HT410_BtagMass->SetRightMargin(0.05);
    Cavnas_HT410_BtagMass->SetTopMargin(0.05);
    Cavnas_HT410_BtagMass->SetBottomMargin(0.15);
    Cavnas_HT410_BtagMass->SetGridy();
    Cavnas_HT410_BtagMass->SetTickx(1);
    Cavnas_HT410_BtagMass->SetTicky(1);
    Cavnas_HT410_BtagMass->SetFrameBorderMode(0);
    Cavnas_HT410_BtagMass->cd();

    h_mass_HT410_BtagMass_ctau0->SetTitle("");
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT410_BtagMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT410_BtagMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT410_BtagMass_ctau0->SetLineColor(9);
    h_mass_HT410_BtagMass_ctau0->SetMarkerColor(9);
    h_mass_HT410_BtagMass_ctau0->SetMarkerStyle(26);
    h_mass_HT410_BtagMass_ctau0->SetMarkerSize(1);
    h_mass_HT410_BtagMass_ctau0->Draw("HIST p");

    h_mass_HT410_BtagMass_ctau1->SetLineColor(8);
    h_mass_HT410_BtagMass_ctau1->SetMarkerColor(8);
    h_mass_HT410_BtagMass_ctau1->SetMarkerStyle(4);
    h_mass_HT410_BtagMass_ctau1->SetMarkerSize(1);
    h_mass_HT410_BtagMass_ctau1->Draw("HIST p same");

    h_mass_HT410_BtagMass_ctau10->SetLineColor(2);
    h_mass_HT410_BtagMass_ctau10->SetMarkerColor(2);
    h_mass_HT410_BtagMass_ctau10->SetMarkerStyle(5);
    h_mass_HT410_BtagMass_ctau10->SetMarkerSize(1);
    h_mass_HT410_BtagMass_ctau10->Draw("HIST p same");

    h_mass_HT410_BtagMass_ctau100->SetLineColor(1);
    h_mass_HT410_BtagMass_ctau100->SetMarkerColor(1);
    h_mass_HT410_BtagMass_ctau100->SetMarkerStyle(2);
    h_mass_HT410_BtagMass_ctau100->SetMarkerSize(1);
    h_mass_HT410_BtagMass_ctau100->Draw("HIST p same");

    h_mass_HT410_BtagMass_ctau1000->SetLineColor(4);
    h_mass_HT410_BtagMass_ctau1000->SetMarkerColor(4);
    h_mass_HT410_BtagMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT410_BtagMass_ctau1000->SetMarkerSize(1);
    h_mass_HT410_BtagMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_BtagMass->SetBorderSize(0);
    leg_HT410_BtagMass->SetTextSize(0.03);
    leg_HT410_BtagMass->SetLineColor(1);
    leg_HT410_BtagMass->SetLineStyle(1);
    leg_HT410_BtagMass->SetLineWidth(1);
    leg_HT410_BtagMass->SetFillColor(0);
    leg_HT410_BtagMass->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagMass = leg_HT410_BtagMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagMass->SetMarkerColor(9);
    entry_HT410_BtagMass->SetMarkerStyle(26);
    entry_HT410_BtagMass->SetMarkerSize(2);
    entry_HT410_BtagMass->SetTextFont(42);
    entry_HT410_BtagMass = leg_HT410_BtagMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagMass->SetMarkerColor(8);
    entry_HT410_BtagMass->SetMarkerStyle(4);
    entry_HT410_BtagMass->SetMarkerSize(2);
    entry_HT410_BtagMass->SetTextFont(42);
    entry_HT410_BtagMass = leg_HT410_BtagMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagMass->SetMarkerColor(2);
    entry_HT410_BtagMass->SetMarkerStyle(5);
    entry_HT410_BtagMass->SetMarkerSize(2);
    entry_HT410_BtagMass->SetTextFont(42);
    entry_HT410_BtagMass = leg_HT410_BtagMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagMass->SetMarkerColor(1);
    entry_HT410_BtagMass->SetMarkerStyle(2);
    entry_HT410_BtagMass->SetMarkerSize(2);
    entry_HT410_BtagMass->SetTextFont(42);
    entry_HT410_BtagMass = leg_HT410_BtagMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagMass->SetMarkerColor(4);
    entry_HT410_BtagMass->SetMarkerStyle(3);
    entry_HT410_BtagMass->SetMarkerSize(2);
    entry_HT410_BtagMass->SetTextFont(42);
    leg_HT410_BtagMass->Draw("HIST");

    Cavnas_HT410_BtagMass->Modified();
    Cavnas_HT410_BtagMass->cd();
    Cavnas_HT410_BtagMass->SetSelected(Cavnas_HT410_BtagMass);
    Cavnas_HT410_BtagMass->SaveAs( "HT410_BtagTrig_mass_eff.pdf" );

    TH1F* h_mass_DoubleMuNoVtxTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_DoubleMuNoVtxTrig");
    TH1F* h_mass_DoubleMuNoVtxTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_DoubleMuNoVtxTrig");
    TH1F* h_mass_DoubleMuNoVtxTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_DoubleMuNoVtxTrig");
    TH1F* h_mass_DoubleMuNoVtxTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_DoubleMuNoVtxTrig");
    TH1F* h_mass_DoubleMuNoVtxTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_DoubleMuNoVtxTrig");

    TCanvas *Cavnas_DoubleMuNoVtxTrigMass = new TCanvas("Cavnas_DoubleMuNoVtxTrigMass", "Cavnas_DoubleMuNoVtxTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_DoubleMuNoVtxTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_DoubleMuNoVtxTrigMass->SetFillColor(0);
    Cavnas_DoubleMuNoVtxTrigMass->SetBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigMass->SetBorderSize(2);
    Cavnas_DoubleMuNoVtxTrigMass->SetLeftMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigMass->SetRightMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigMass->SetTopMargin(0.05);
    Cavnas_DoubleMuNoVtxTrigMass->SetBottomMargin(0.15);
    Cavnas_DoubleMuNoVtxTrigMass->SetGridy();
    Cavnas_DoubleMuNoVtxTrigMass->SetTickx(1);
    Cavnas_DoubleMuNoVtxTrigMass->SetTicky(1);
    Cavnas_DoubleMuNoVtxTrigMass->SetFrameBorderMode(0);
    Cavnas_DoubleMuNoVtxTrigMass->cd();

    h_mass_DoubleMuNoVtxTrigMass_ctau0->SetTitle("");
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetTitle("#mu#mu no vtx trigger efficiency");
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->SetLineColor(9);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->SetMarkerColor(9);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->SetMarkerSize(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau0->Draw("HIST p");

    h_mass_DoubleMuNoVtxTrigMass_ctau1->SetLineColor(8);
    h_mass_DoubleMuNoVtxTrigMass_ctau1->SetMarkerColor(8);
    h_mass_DoubleMuNoVtxTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_DoubleMuNoVtxTrigMass_ctau1->SetMarkerSize(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau1->Draw("HIST p same");

    h_mass_DoubleMuNoVtxTrigMass_ctau10->SetLineColor(2);
    h_mass_DoubleMuNoVtxTrigMass_ctau10->SetMarkerColor(2);
    h_mass_DoubleMuNoVtxTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_DoubleMuNoVtxTrigMass_ctau10->SetMarkerSize(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau10->Draw("HIST p same");

    h_mass_DoubleMuNoVtxTrigMass_ctau100->SetLineColor(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau100->SetMarkerColor(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_DoubleMuNoVtxTrigMass_ctau100->SetMarkerSize(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau100->Draw("HIST p same");

    h_mass_DoubleMuNoVtxTrigMass_ctau1000->SetLineColor(4);
    h_mass_DoubleMuNoVtxTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_DoubleMuNoVtxTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_DoubleMuNoVtxTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_DoubleMuNoVtxTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_DoubleMuNoVtxTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_DoubleMuNoVtxTrigMass->SetBorderSize(0);
    leg_DoubleMuNoVtxTrigMass->SetTextSize(0.03);
    leg_DoubleMuNoVtxTrigMass->SetLineColor(1);
    leg_DoubleMuNoVtxTrigMass->SetLineStyle(1);
    leg_DoubleMuNoVtxTrigMass->SetLineWidth(1);
    leg_DoubleMuNoVtxTrigMass->SetFillColor(0);
    leg_DoubleMuNoVtxTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_DoubleMuNoVtxTrigMass = leg_DoubleMuNoVtxTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_DoubleMuNoVtxTrigMass->SetMarkerColor(9);
    entry_DoubleMuNoVtxTrigMass->SetMarkerStyle(26);
    entry_DoubleMuNoVtxTrigMass->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigMass->SetTextFont(42);
    entry_DoubleMuNoVtxTrigMass = leg_DoubleMuNoVtxTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_DoubleMuNoVtxTrigMass->SetMarkerColor(8);
    entry_DoubleMuNoVtxTrigMass->SetMarkerStyle(4);
    entry_DoubleMuNoVtxTrigMass->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigMass->SetTextFont(42);
    entry_DoubleMuNoVtxTrigMass = leg_DoubleMuNoVtxTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_DoubleMuNoVtxTrigMass->SetMarkerColor(2);
    entry_DoubleMuNoVtxTrigMass->SetMarkerStyle(5);
    entry_DoubleMuNoVtxTrigMass->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigMass->SetTextFont(42);
    entry_DoubleMuNoVtxTrigMass = leg_DoubleMuNoVtxTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_DoubleMuNoVtxTrigMass->SetMarkerColor(1);
    entry_DoubleMuNoVtxTrigMass->SetMarkerStyle(2);
    entry_DoubleMuNoVtxTrigMass->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigMass->SetTextFont(42);
    entry_DoubleMuNoVtxTrigMass = leg_DoubleMuNoVtxTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_DoubleMuNoVtxTrigMass->SetMarkerColor(4);
    entry_DoubleMuNoVtxTrigMass->SetMarkerStyle(3);
    entry_DoubleMuNoVtxTrigMass->SetMarkerSize(2);
    entry_DoubleMuNoVtxTrigMass->SetTextFont(42);
    leg_DoubleMuNoVtxTrigMass->Draw("HIST");

    Cavnas_DoubleMuNoVtxTrigMass->Modified();
    Cavnas_DoubleMuNoVtxTrigMass->cd();
    Cavnas_DoubleMuNoVtxTrigMass->SetSelected(Cavnas_DoubleMuNoVtxTrigMass);
    Cavnas_DoubleMuNoVtxTrigMass->SaveAs( "DoubleMuNoVtxTrig_mass_eff.pdf" );

}
