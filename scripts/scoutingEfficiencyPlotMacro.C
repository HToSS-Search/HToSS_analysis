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
    Cavnas_HT250_CaloTrigPt->SaveAs( "HT250_CaloTrig_pT_eff.pdf" );

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
    h_leadingMuonPt_HT250_CaloBtagTrigPt_ctau0->GetYaxis()->SetTitle(H_{T} > 250 calo btag trigger efficiency");
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

    TH1F* h_leadingMuonPt_HT410_PFTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT410_PFTrig");
    TH1F* h_leadingMuonPt_HT410_PFTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT410_PFTrig");
    TH1F* h_leadingMuonPt_HT410_PFTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT410_PFTrig");
    TH1F* h_leadingMuonPt_HT410_PFTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT410_PFTrig");
    TH1F* h_leadingMuonPt_HT410_PFTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT410_PFTrig");

    TCanvas *Cavnas_HT410_PFTrigPt = new TCanvas("Cavnas_HT410_PFTrigPt", "Cavnas_HT410_PFTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFTrigPt->SetFillColor(0);
    Cavnas_HT410_PFTrigPt->SetBorderMode(0);
    Cavnas_HT410_PFTrigPt->SetBorderSize(2);
    Cavnas_HT410_PFTrigPt->SetLeftMargin(0.15);
    Cavnas_HT410_PFTrigPt->SetRightMargin(0.05);
    Cavnas_HT410_PFTrigPt->SetTopMargin(0.05);
    Cavnas_HT410_PFTrigPt->SetBottomMargin(0.15);
    Cavnas_HT410_PFTrigPt->SetGridy();
    Cavnas_HT410_PFTrigPt->SetTickx(1);
    Cavnas_HT410_PFTrigPt->SetTicky(1);
    Cavnas_HT410_PFTrigPt->SetFrameBorderMode(0);
    Cavnas_HT410_PFTrigPt->cd();

    h_leadingMuonPt_HT410_PFTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT410_PFTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT410_PFTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT410_PFTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT410_PFTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT410_PFTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT410_PFTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT410_PFTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_PFTrigPt->SetBorderSize(0);
    leg_HT410_PFTrigPt->SetTextSize(0.03);
    leg_HT410_PFTrigPt->SetLineColor(1);
    leg_HT410_PFTrigPt->SetLineStyle(1);
    leg_HT410_PFTrigPt->SetLineWidth(1);
    leg_HT410_PFTrigPt->SetFillColor(0);
    leg_HT410_PFTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFTrigPt = leg_HT410_PFTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFTrigPt->SetMarkerColor(9);
    entry_HT410_PFTrigPt->SetMarkerStyle(26);
    entry_HT410_PFTrigPt->SetMarkerSize(2);
    entry_HT410_PFTrigPt->SetTextFont(42);
    entry_HT410_PFTrigPt = leg_HT410_PFTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFTrigPt->SetMarkerColor(8);
    entry_HT410_PFTrigPt->SetMarkerStyle(4);
    entry_HT410_PFTrigPt->SetMarkerSize(2);
    entry_HT410_PFTrigPt->SetTextFont(42);
    entry_HT410_PFTrigPt = leg_HT410_PFTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFTrigPt->SetMarkerColor(2);
    entry_HT410_PFTrigPt->SetMarkerStyle(5);
    entry_HT410_PFTrigPt->SetMarkerSize(2);
    entry_HT410_PFTrigPt->SetTextFont(42);
    entry_HT410_PFTrigPt = leg_HT410_PFTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFTrigPt->SetMarkerColor(1);
    entry_HT410_PFTrigPt->SetMarkerStyle(2);
    entry_HT410_PFTrigPt->SetMarkerSize(2);
    entry_HT410_PFTrigPt->SetTextFont(42);
    entry_HT410_PFTrigPt = leg_HT410_PFTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFTrigPt->SetMarkerColor(4);
    entry_HT410_PFTrigPt->SetMarkerStyle(3);
    entry_HT410_PFTrigPt->SetMarkerSize(2);
    entry_HT410_PFTrigPt->SetTextFont(42);
    leg_HT410_PFTrigPt->Draw("HIST");

    Cavnas_HT410_PFTrigPt->Modified();
    Cavnas_HT410_PFTrigPt->cd();
    Cavnas_HT410_PFTrigPt->SetSelected(Cavnas_HT410_PFTrigPt);
    Cavnas_HT410_PFTrigPt->SaveAs( "HT410_PFTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_HT410_BtagTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_HT410_BtagTrig");
    TH1F* h_leadingMuonPt_HT410_BtagTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_HT410_BtagTrig");
    TH1F* h_leadingMuonPt_HT410_BtagTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_HT410_BtagTrig");
    TH1F* h_leadingMuonPt_HT410_BtagTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_HT410_BtagTrig");
    TH1F* h_leadingMuonPt_HT410_BtagTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_HT410_BtagTrig");

    TCanvas *Cavnas_HT410_BtagTrigPt = new TCanvas("Cavnas_HT410_BtagTrigPt", "Cavnas_HT410_BtagTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagTrigPt->SetFillColor(0);
    Cavnas_HT410_BtagTrigPt->SetBorderMode(0);
    Cavnas_HT410_BtagTrigPt->SetBorderSize(2);
    Cavnas_HT410_BtagTrigPt->SetLeftMargin(0.15);
    Cavnas_HT410_BtagTrigPt->SetRightMargin(0.05);
    Cavnas_HT410_BtagTrigPt->SetTopMargin(0.05);
    Cavnas_HT410_BtagTrigPt->SetBottomMargin(0.15);
    Cavnas_HT410_BtagTrigPt->SetGridy();
    Cavnas_HT410_BtagTrigPt->SetTickx(1);
    Cavnas_HT410_BtagTrigPt->SetTicky(1);
    Cavnas_HT410_BtagTrigPt->SetFrameBorderMode(0);
    Cavnas_HT410_BtagTrigPt->cd();

    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_HT410_BtagTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_HT410_BtagTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_HT410_BtagTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_BtagTrigPt->SetBorderSize(0);
    leg_HT410_BtagTrigPt->SetTextSize(0.03);
    leg_HT410_BtagTrigPt->SetLineColor(1);
    leg_HT410_BtagTrigPt->SetLineStyle(1);
    leg_HT410_BtagTrigPt->SetLineWidth(1);
    leg_HT410_BtagTrigPt->SetFillColor(0);
    leg_HT410_BtagTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagTrigPt = leg_HT410_BtagTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagTrigPt->SetMarkerColor(9);
    entry_HT410_BtagTrigPt->SetMarkerStyle(26);
    entry_HT410_BtagTrigPt->SetMarkerSize(2);
    entry_HT410_BtagTrigPt->SetTextFont(42);
    entry_HT410_BtagTrigPt = leg_HT410_BtagTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagTrigPt->SetMarkerColor(8);
    entry_HT410_BtagTrigPt->SetMarkerStyle(4);
    entry_HT410_BtagTrigPt->SetMarkerSize(2);
    entry_HT410_BtagTrigPt->SetTextFont(42);
    entry_HT410_BtagTrigPt = leg_HT410_BtagTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagTrigPt->SetMarkerColor(2);
    entry_HT410_BtagTrigPt->SetMarkerStyle(5);
    entry_HT410_BtagTrigPt->SetMarkerSize(2);
    entry_HT410_BtagTrigPt->SetTextFont(42);
    entry_HT410_BtagTrigPt = leg_HT410_BtagTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagTrigPt->SetMarkerColor(1);
    entry_HT410_BtagTrigPt->SetMarkerStyle(2);
    entry_HT410_BtagTrigPt->SetMarkerSize(2);
    entry_HT410_BtagTrigPt->SetTextFont(42);
    entry_HT410_BtagTrigPt = leg_HT410_BtagTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagTrigPt->SetMarkerColor(4);
    entry_HT410_BtagTrigPt->SetMarkerStyle(3);
    entry_HT410_BtagTrigPt->SetMarkerSize(2);
    entry_HT410_BtagTrigPt->SetTextFont(42);
    leg_HT410_BtagTrigPt->Draw("HIST");

    Cavnas_HT410_BtagTrigPt->Modified();
    Cavnas_HT410_BtagTrigPt->cd();
    Cavnas_HT410_BtagTrigPt->SetSelected(Cavnas_HT410_BtagTrigPt);
    Cavnas_HT410_BtagTrigPt->SaveAs( "HT410_BtagTrig_pT_eff.pdf" );

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
    h_leadingMuonEta_HT250_CaloBtagTrigEta_ctau0->GetYaxis()->SetTitle(H_{T} > 250 calo btag trigger efficiency");
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

    TH1F* h_leadingMuonEta_HT410_PFTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT410_PFTrig");
    TH1F* h_leadingMuonEta_HT410_PFTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT410_PFTrig");
    TH1F* h_leadingMuonEta_HT410_PFTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT410_PFTrig");
    TH1F* h_leadingMuonEta_HT410_PFTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT410_PFTrig");
    TH1F* h_leadingMuonEta_HT410_PFTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT410_PFTrig");

    TCanvas *Cavnas_HT410_PFTrigEta = new TCanvas("Cavnas_HT410_PFTrigEta", "Cavnas_HT410_PFTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFTrigEta->SetFillColor(0);
    Cavnas_HT410_PFTrigEta->SetBorderMode(0);
    Cavnas_HT410_PFTrigEta->SetBorderSize(2);
    Cavnas_HT410_PFTrigEta->SetLeftMargin(0.15);
    Cavnas_HT410_PFTrigEta->SetRightMargin(0.05);
    Cavnas_HT410_PFTrigEta->SetTopMargin(0.05);
    Cavnas_HT410_PFTrigEta->SetBottomMargin(0.15);
    Cavnas_HT410_PFTrigEta->SetGridy();
    Cavnas_HT410_PFTrigEta->SetTickx(1);
    Cavnas_HT410_PFTrigEta->SetTicky(1);
    Cavnas_HT410_PFTrigEta->SetFrameBorderMode(0);
    Cavnas_HT410_PFTrigEta->cd();

    h_leadingMuonEta_HT410_PFTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT410_PFTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT410_PFTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT410_PFTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT410_PFTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT410_PFTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT410_PFTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT410_PFTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_PFTrigEta->SetBorderSize(0);
    leg_HT410_PFTrigEta->SetTextSize(0.03);
    leg_HT410_PFTrigEta->SetLineColor(1);
    leg_HT410_PFTrigEta->SetLineStyle(1);
    leg_HT410_PFTrigEta->SetLineWidth(1);
    leg_HT410_PFTrigEta->SetFillColor(0);
    leg_HT410_PFTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFTrigEta = leg_HT410_PFTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFTrigEta->SetMarkerColor(9);
    entry_HT410_PFTrigEta->SetMarkerStyle(26);
    entry_HT410_PFTrigEta->SetMarkerSize(2);
    entry_HT410_PFTrigEta->SetTextFont(42);
    entry_HT410_PFTrigEta = leg_HT410_PFTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFTrigEta->SetMarkerColor(8);
    entry_HT410_PFTrigEta->SetMarkerStyle(4);
    entry_HT410_PFTrigEta->SetMarkerSize(2);
    entry_HT410_PFTrigEta->SetTextFont(42);
    entry_HT410_PFTrigEta = leg_HT410_PFTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFTrigEta->SetMarkerColor(2);
    entry_HT410_PFTrigEta->SetMarkerStyle(5);
    entry_HT410_PFTrigEta->SetMarkerSize(2);
    entry_HT410_PFTrigEta->SetTextFont(42);
    entry_HT410_PFTrigEta = leg_HT410_PFTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFTrigEta->SetMarkerColor(1);
    entry_HT410_PFTrigEta->SetMarkerStyle(2);
    entry_HT410_PFTrigEta->SetMarkerSize(2);
    entry_HT410_PFTrigEta->SetTextFont(42);
    entry_HT410_PFTrigEta = leg_HT410_PFTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFTrigEta->SetMarkerColor(4);
    entry_HT410_PFTrigEta->SetMarkerStyle(3);
    entry_HT410_PFTrigEta->SetMarkerSize(2);
    entry_HT410_PFTrigEta->SetTextFont(42);
    leg_HT410_PFTrigEta->Draw("HIST");

    Cavnas_HT410_PFTrigEta->Modified();
    Cavnas_HT410_PFTrigEta->cd();
    Cavnas_HT410_PFTrigEta->SetSelected(Cavnas_HT410_PFTrigEta);
    Cavnas_HT410_PFTrigEta->SaveAs( "HT410_PFTrig_eta_eff.pdf" );

    TH1F* h_leadingMuonEta_HT410_BtagTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_HT410_BtagTrig");
    TH1F* h_leadingMuonEta_HT410_BtagTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_HT410_BtagTrig");
    TH1F* h_leadingMuonEta_HT410_BtagTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_HT410_BtagTrig");
    TH1F* h_leadingMuonEta_HT410_BtagTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_HT410_BtagTrig");
    TH1F* h_leadingMuonEta_HT410_BtagTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_HT410_BtagTrig");

    TCanvas *Cavnas_HT410_BtagTrigEta = new TCanvas("Cavnas_HT410_BtagTrigEta", "Cavnas_HT410_BtagTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagTrigEta->SetFillColor(0);
    Cavnas_HT410_BtagTrigEta->SetBorderMode(0);
    Cavnas_HT410_BtagTrigEta->SetBorderSize(2);
    Cavnas_HT410_BtagTrigEta->SetLeftMargin(0.15);
    Cavnas_HT410_BtagTrigEta->SetRightMargin(0.05);
    Cavnas_HT410_BtagTrigEta->SetTopMargin(0.05);
    Cavnas_HT410_BtagTrigEta->SetBottomMargin(0.15);
    Cavnas_HT410_BtagTrigEta->SetGridy();
    Cavnas_HT410_BtagTrigEta->SetTickx(1);
    Cavnas_HT410_BtagTrigEta->SetTicky(1);
    Cavnas_HT410_BtagTrigEta->SetFrameBorderMode(0);
    Cavnas_HT410_BtagTrigEta->cd();

    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_HT410_BtagTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_HT410_BtagTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_HT410_BtagTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_HT410_BtagTrigEta->SetBorderSize(0);
    leg_HT410_BtagTrigEta->SetTextSize(0.03);
    leg_HT410_BtagTrigEta->SetLineColor(1);
    leg_HT410_BtagTrigEta->SetLineStyle(1);
    leg_HT410_BtagTrigEta->SetLineWidth(1);
    leg_HT410_BtagTrigEta->SetFillColor(0);
    leg_HT410_BtagTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagTrigEta = leg_HT410_BtagTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagTrigEta->SetMarkerColor(9);
    entry_HT410_BtagTrigEta->SetMarkerStyle(26);
    entry_HT410_BtagTrigEta->SetMarkerSize(2);
    entry_HT410_BtagTrigEta->SetTextFont(42);
    entry_HT410_BtagTrigEta = leg_HT410_BtagTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagTrigEta->SetMarkerColor(8);
    entry_HT410_BtagTrigEta->SetMarkerStyle(4);
    entry_HT410_BtagTrigEta->SetMarkerSize(2);
    entry_HT410_BtagTrigEta->SetTextFont(42);
    entry_HT410_BtagTrigEta = leg_HT410_BtagTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagTrigEta->SetMarkerColor(2);
    entry_HT410_BtagTrigEta->SetMarkerStyle(5);
    entry_HT410_BtagTrigEta->SetMarkerSize(2);
    entry_HT410_BtagTrigEta->SetTextFont(42);
    entry_HT410_BtagTrigEta = leg_HT410_BtagTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagTrigEta->SetMarkerColor(1);
    entry_HT410_BtagTrigEta->SetMarkerStyle(2);
    entry_HT410_BtagTrigEta->SetMarkerSize(2);
    entry_HT410_BtagTrigEta->SetTextFont(42);
    entry_HT410_BtagTrigEta = leg_HT410_BtagTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagTrigEta->SetMarkerColor(4);
    entry_HT410_BtagTrigEta->SetMarkerStyle(3);
    entry_HT410_BtagTrigEta->SetMarkerSize(2);
    entry_HT410_BtagTrigEta->SetTextFont(42);
    leg_HT410_BtagTrigEta->Draw("HIST");

    Cavnas_HT410_BtagTrigEta->Modified();
    Cavnas_HT410_BtagTrigEta->cd();
    Cavnas_HT410_BtagTrigEta->SetSelected(Cavnas_HT410_BtagTrigEta);
    Cavnas_HT410_BtagTrigEta->SaveAs( "HT410_BtagTrig_eta_eff.pdf" );

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
    h_delR_HT250_CaloBtagTrigDelR_ctau0->GetYaxis()->SetTitle(H_{T} > 250 calo btag trigger efficiency");
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

    TH1F* h_delR_HT410_PFTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT410_PFTrig");
    TH1F* h_delR_HT410_PFTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT410_PFTrig");
    TH1F* h_delR_HT410_PFTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT410_PFTrig");
    TH1F* h_delR_HT410_PFTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT410_PFTrig");
    TH1F* h_delR_HT410_PFTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT410_PFTrig");

    TCanvas *Cavnas_HT410_PFTrigDelR = new TCanvas("Cavnas_HT410_PFTrigDelR", "Cavnas_HT410_PFTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFTrigDelR->SetFillColor(0);
    Cavnas_HT410_PFTrigDelR->SetBorderMode(0);
    Cavnas_HT410_PFTrigDelR->SetBorderSize(2);
    Cavnas_HT410_PFTrigDelR->SetLeftMargin(0.15);
    Cavnas_HT410_PFTrigDelR->SetRightMargin(0.05);
    Cavnas_HT410_PFTrigDelR->SetTopMargin(0.05);
    Cavnas_HT410_PFTrigDelR->SetBottomMargin(0.15);
    Cavnas_HT410_PFTrigDelR->SetGridy();
    Cavnas_HT410_PFTrigDelR->SetTickx(1);
    Cavnas_HT410_PFTrigDelR->SetTicky(1);
    Cavnas_HT410_PFTrigDelR->SetFrameBorderMode(0);
    Cavnas_HT410_PFTrigDelR->cd();

    h_delR_HT410_PFTrigDelR_ctau0->SetTitle("");
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT410_PFTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT410_PFTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT410_PFTrigDelR_ctau0->SetLineColor(9);
    h_delR_HT410_PFTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_HT410_PFTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT410_PFTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_HT410_PFTrigDelR_ctau0->Draw("HIST p");

    h_delR_HT410_PFTrigDelR_ctau1->SetLineColor(8);
    h_delR_HT410_PFTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_HT410_PFTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT410_PFTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_HT410_PFTrigDelR_ctau1->Draw("HIST p same");

    h_delR_HT410_PFTrigDelR_ctau10->SetLineColor(2);
    h_delR_HT410_PFTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_HT410_PFTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT410_PFTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_HT410_PFTrigDelR_ctau10->Draw("HIST p same");

    h_delR_HT410_PFTrigDelR_ctau100->SetLineColor(1);
    h_delR_HT410_PFTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_HT410_PFTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT410_PFTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_HT410_PFTrigDelR_ctau100->Draw("HIST p same");

    h_delR_HT410_PFTrigDelR_ctau1000->SetLineColor(4);
    h_delR_HT410_PFTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT410_PFTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT410_PFTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT410_PFTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_PFTrigDelR->SetBorderSize(0);
    leg_HT410_PFTrigDelR->SetTextSize(0.03);
    leg_HT410_PFTrigDelR->SetLineColor(1);
    leg_HT410_PFTrigDelR->SetLineStyle(1);
    leg_HT410_PFTrigDelR->SetLineWidth(1);
    leg_HT410_PFTrigDelR->SetFillColor(0);
    leg_HT410_PFTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFTrigDelR = leg_HT410_PFTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFTrigDelR->SetMarkerColor(9);
    entry_HT410_PFTrigDelR->SetMarkerStyle(26);
    entry_HT410_PFTrigDelR->SetMarkerSize(2);
    entry_HT410_PFTrigDelR->SetTextFont(42);
    entry_HT410_PFTrigDelR = leg_HT410_PFTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFTrigDelR->SetMarkerColor(8);
    entry_HT410_PFTrigDelR->SetMarkerStyle(4);
    entry_HT410_PFTrigDelR->SetMarkerSize(2);
    entry_HT410_PFTrigDelR->SetTextFont(42);
    entry_HT410_PFTrigDelR = leg_HT410_PFTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFTrigDelR->SetMarkerColor(2);
    entry_HT410_PFTrigDelR->SetMarkerStyle(5);
    entry_HT410_PFTrigDelR->SetMarkerSize(2);
    entry_HT410_PFTrigDelR->SetTextFont(42);
    entry_HT410_PFTrigDelR = leg_HT410_PFTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFTrigDelR->SetMarkerColor(1);
    entry_HT410_PFTrigDelR->SetMarkerStyle(2);
    entry_HT410_PFTrigDelR->SetMarkerSize(2);
    entry_HT410_PFTrigDelR->SetTextFont(42);
    entry_HT410_PFTrigDelR = leg_HT410_PFTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFTrigDelR->SetMarkerColor(4);
    entry_HT410_PFTrigDelR->SetMarkerStyle(3);
    entry_HT410_PFTrigDelR->SetMarkerSize(2);
    entry_HT410_PFTrigDelR->SetTextFont(42);
    leg_HT410_PFTrigDelR->Draw("HIST");

    Cavnas_HT410_PFTrigDelR->Modified();
    Cavnas_HT410_PFTrigDelR->cd();
    Cavnas_HT410_PFTrigDelR->SetSelected(Cavnas_HT410_PFTrigDelR);
    Cavnas_HT410_PFTrigDelR->SaveAs( "HT410_PFTrig_DelR_eff.pdf" );

    TH1F* h_delR_HT410_BtagTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_HT410_BtagTrig");
    TH1F* h_delR_HT410_BtagTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_HT410_BtagTrig");
    TH1F* h_delR_HT410_BtagTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_HT410_BtagTrig");
    TH1F* h_delR_HT410_BtagTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_HT410_BtagTrig");
    TH1F* h_delR_HT410_BtagTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_HT410_BtagTrig");

    TCanvas *Cavnas_HT410_BtagTrigDelR = new TCanvas("Cavnas_HT410_BtagTrigDelR", "Cavnas_HT410_BtagTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagTrigDelR->SetFillColor(0);
    Cavnas_HT410_BtagTrigDelR->SetBorderMode(0);
    Cavnas_HT410_BtagTrigDelR->SetBorderSize(2);
    Cavnas_HT410_BtagTrigDelR->SetLeftMargin(0.15);
    Cavnas_HT410_BtagTrigDelR->SetRightMargin(0.05);
    Cavnas_HT410_BtagTrigDelR->SetTopMargin(0.05);
    Cavnas_HT410_BtagTrigDelR->SetBottomMargin(0.15);
    Cavnas_HT410_BtagTrigDelR->SetGridy();
    Cavnas_HT410_BtagTrigDelR->SetTickx(1);
    Cavnas_HT410_BtagTrigDelR->SetTicky(1);
    Cavnas_HT410_BtagTrigDelR->SetFrameBorderMode(0);
    Cavnas_HT410_BtagTrigDelR->cd();

    h_delR_HT410_BtagTrigDelR_ctau0->SetTitle("");
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_HT410_BtagTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_HT410_BtagTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_HT410_BtagTrigDelR_ctau0->SetLineColor(9);
    h_delR_HT410_BtagTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_HT410_BtagTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_HT410_BtagTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_HT410_BtagTrigDelR_ctau0->Draw("HIST p");

    h_delR_HT410_BtagTrigDelR_ctau1->SetLineColor(8);
    h_delR_HT410_BtagTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_HT410_BtagTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_HT410_BtagTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_HT410_BtagTrigDelR_ctau1->Draw("HIST p same");

    h_delR_HT410_BtagTrigDelR_ctau10->SetLineColor(2);
    h_delR_HT410_BtagTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_HT410_BtagTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_HT410_BtagTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_HT410_BtagTrigDelR_ctau10->Draw("HIST p same");

    h_delR_HT410_BtagTrigDelR_ctau100->SetLineColor(1);
    h_delR_HT410_BtagTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_HT410_BtagTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_HT410_BtagTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_HT410_BtagTrigDelR_ctau100->Draw("HIST p same");

    h_delR_HT410_BtagTrigDelR_ctau1000->SetLineColor(4);
    h_delR_HT410_BtagTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_HT410_BtagTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_HT410_BtagTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_HT410_BtagTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_BtagTrigDelR->SetBorderSize(0);
    leg_HT410_BtagTrigDelR->SetTextSize(0.03);
    leg_HT410_BtagTrigDelR->SetLineColor(1);
    leg_HT410_BtagTrigDelR->SetLineStyle(1);
    leg_HT410_BtagTrigDelR->SetLineWidth(1);
    leg_HT410_BtagTrigDelR->SetFillColor(0);
    leg_HT410_BtagTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagTrigDelR = leg_HT410_BtagTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagTrigDelR->SetMarkerColor(9);
    entry_HT410_BtagTrigDelR->SetMarkerStyle(26);
    entry_HT410_BtagTrigDelR->SetMarkerSize(2);
    entry_HT410_BtagTrigDelR->SetTextFont(42);
    entry_HT410_BtagTrigDelR = leg_HT410_BtagTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagTrigDelR->SetMarkerColor(8);
    entry_HT410_BtagTrigDelR->SetMarkerStyle(4);
    entry_HT410_BtagTrigDelR->SetMarkerSize(2);
    entry_HT410_BtagTrigDelR->SetTextFont(42);
    entry_HT410_BtagTrigDelR = leg_HT410_BtagTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagTrigDelR->SetMarkerColor(2);
    entry_HT410_BtagTrigDelR->SetMarkerStyle(5);
    entry_HT410_BtagTrigDelR->SetMarkerSize(2);
    entry_HT410_BtagTrigDelR->SetTextFont(42);
    entry_HT410_BtagTrigDelR = leg_HT410_BtagTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagTrigDelR->SetMarkerColor(1);
    entry_HT410_BtagTrigDelR->SetMarkerStyle(2);
    entry_HT410_BtagTrigDelR->SetMarkerSize(2);
    entry_HT410_BtagTrigDelR->SetTextFont(42);
    entry_HT410_BtagTrigDelR = leg_HT410_BtagTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagTrigDelR->SetMarkerColor(4);
    entry_HT410_BtagTrigDelR->SetMarkerStyle(3);
    entry_HT410_BtagTrigDelR->SetMarkerSize(2);
    entry_HT410_BtagTrigDelR->SetTextFont(42);
    leg_HT410_BtagTrigDelR->Draw("HIST");

    Cavnas_HT410_BtagTrigDelR->Modified();
    Cavnas_HT410_BtagTrigDelR->cd();
    Cavnas_HT410_BtagTrigDelR->SetSelected(Cavnas_HT410_BtagTrigDelR);
    Cavnas_HT410_BtagTrigDelR->SaveAs( "HT410_BtagTrig_DelR_eff.pdf" );

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
    h_mass_HT250_CaloBtagTrigMass_ctau0->GetYaxis()->SetTitle(H_{T} > 250 calo btag trigger efficiency");
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


    TH1F* h_mass_HT410_PFTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT410_PFTrig");
    TH1F* h_mass_HT410_PFTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT410_PFTrig");
    TH1F* h_mass_HT410_PFTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT410_PFTrig");
    TH1F* h_mass_HT410_PFTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT410_PFTrig");
    TH1F* h_mass_HT410_PFTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT410_PFTrig");

    TCanvas *Cavnas_HT410_PFTrigMass = new TCanvas("Cavnas_HT410_PFTrigMass", "Cavnas_HT410_PFTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_PFTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_PFTrigMass->SetFillColor(0);
    Cavnas_HT410_PFTrigMass->SetBorderMode(0);
    Cavnas_HT410_PFTrigMass->SetBorderSize(2);
    Cavnas_HT410_PFTrigMass->SetLeftMargin(0.15);
    Cavnas_HT410_PFTrigMass->SetRightMargin(0.05);
    Cavnas_HT410_PFTrigMass->SetTopMargin(0.05);
    Cavnas_HT410_PFTrigMass->SetBottomMargin(0.15);
    Cavnas_HT410_PFTrigMass->SetGridy();
    Cavnas_HT410_PFTrigMass->SetTickx(1);
    Cavnas_HT410_PFTrigMass->SetTicky(1);
    Cavnas_HT410_PFTrigMass->SetFrameBorderMode(0);
    Cavnas_HT410_PFTrigMass->cd();

    h_mass_HT410_PFTrigMass_ctau0->SetTitle("");
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT410_PFTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetTitle("H_{T} > 410 PF trigger efficiency");
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT410_PFTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT410_PFTrigMass_ctau0->SetLineColor(9);
    h_mass_HT410_PFTrigMass_ctau0->SetMarkerColor(9);
    h_mass_HT410_PFTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_HT410_PFTrigMass_ctau0->SetMarkerSize(1);
    h_mass_HT410_PFTrigMass_ctau0->Draw("HIST p");

    h_mass_HT410_PFTrigMass_ctau1->SetLineColor(8);
    h_mass_HT410_PFTrigMass_ctau1->SetMarkerColor(8);
    h_mass_HT410_PFTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_HT410_PFTrigMass_ctau1->SetMarkerSize(1);
    h_mass_HT410_PFTrigMass_ctau1->Draw("HIST p same");

    h_mass_HT410_PFTrigMass_ctau10->SetLineColor(2);
    h_mass_HT410_PFTrigMass_ctau10->SetMarkerColor(2);
    h_mass_HT410_PFTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_HT410_PFTrigMass_ctau10->SetMarkerSize(1);
    h_mass_HT410_PFTrigMass_ctau10->Draw("HIST p same");

    h_mass_HT410_PFTrigMass_ctau100->SetLineColor(1);
    h_mass_HT410_PFTrigMass_ctau100->SetMarkerColor(1);
    h_mass_HT410_PFTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_HT410_PFTrigMass_ctau100->SetMarkerSize(1);
    h_mass_HT410_PFTrigMass_ctau100->Draw("HIST p same");

    h_mass_HT410_PFTrigMass_ctau1000->SetLineColor(4);
    h_mass_HT410_PFTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_HT410_PFTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT410_PFTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_HT410_PFTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_PFTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_PFTrigMass->SetBorderSize(0);
    leg_HT410_PFTrigMass->SetTextSize(0.03);
    leg_HT410_PFTrigMass->SetLineColor(1);
    leg_HT410_PFTrigMass->SetLineStyle(1);
    leg_HT410_PFTrigMass->SetLineWidth(1);
    leg_HT410_PFTrigMass->SetFillColor(0);
    leg_HT410_PFTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_HT410_PFTrigMass = leg_HT410_PFTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_PFTrigMass->SetMarkerColor(9);
    entry_HT410_PFTrigMass->SetMarkerStyle(26);
    entry_HT410_PFTrigMass->SetMarkerSize(2);
    entry_HT410_PFTrigMass->SetTextFont(42);
    entry_HT410_PFTrigMass = leg_HT410_PFTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_PFTrigMass->SetMarkerColor(8);
    entry_HT410_PFTrigMass->SetMarkerStyle(4);
    entry_HT410_PFTrigMass->SetMarkerSize(2);
    entry_HT410_PFTrigMass->SetTextFont(42);
    entry_HT410_PFTrigMass = leg_HT410_PFTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_PFTrigMass->SetMarkerColor(2);
    entry_HT410_PFTrigMass->SetMarkerStyle(5);
    entry_HT410_PFTrigMass->SetMarkerSize(2);
    entry_HT410_PFTrigMass->SetTextFont(42);
    entry_HT410_PFTrigMass = leg_HT410_PFTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_PFTrigMass->SetMarkerColor(1);
    entry_HT410_PFTrigMass->SetMarkerStyle(2);
    entry_HT410_PFTrigMass->SetMarkerSize(2);
    entry_HT410_PFTrigMass->SetTextFont(42);
    entry_HT410_PFTrigMass = leg_HT410_PFTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_PFTrigMass->SetMarkerColor(4);
    entry_HT410_PFTrigMass->SetMarkerStyle(3);
    entry_HT410_PFTrigMass->SetMarkerSize(2);
    entry_HT410_PFTrigMass->SetTextFont(42);
    leg_HT410_PFTrigMass->Draw("HIST");

    Cavnas_HT410_PFTrigMass->Modified();
    Cavnas_HT410_PFTrigMass->cd();
    Cavnas_HT410_PFTrigMass->SetSelected(Cavnas_HT410_PFTrigMass);
    Cavnas_HT410_PFTrigMass->SaveAs( "HT410_PFTrig_mass_eff.pdf" );

    TH1F* h_mass_HT410_BtagTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_HT410_BtagTrig");
    TH1F* h_mass_HT410_BtagTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_HT410_BtagTrig");
    TH1F* h_mass_HT410_BtagTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_HT410_BtagTrig");
    TH1F* h_mass_HT410_BtagTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_HT410_BtagTrig");
    TH1F* h_mass_HT410_BtagTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_HT410_BtagTrig");

    TCanvas *Cavnas_HT410_BtagTrigMass = new TCanvas("Cavnas_HT410_BtagTrigMass", "Cavnas_HT410_BtagTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_HT410_BtagTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_HT410_BtagTrigMass->SetFillColor(0);
    Cavnas_HT410_BtagTrigMass->SetBorderMode(0);
    Cavnas_HT410_BtagTrigMass->SetBorderSize(2);
    Cavnas_HT410_BtagTrigMass->SetLeftMargin(0.15);
    Cavnas_HT410_BtagTrigMass->SetRightMargin(0.05);
    Cavnas_HT410_BtagTrigMass->SetTopMargin(0.05);
    Cavnas_HT410_BtagTrigMass->SetBottomMargin(0.15);
    Cavnas_HT410_BtagTrigMass->SetGridy();
    Cavnas_HT410_BtagTrigMass->SetTickx(1);
    Cavnas_HT410_BtagTrigMass->SetTicky(1);
    Cavnas_HT410_BtagTrigMass->SetFrameBorderMode(0);
    Cavnas_HT410_BtagTrigMass->cd();

    h_mass_HT410_BtagTrigMass_ctau0->SetTitle("");
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_HT410_BtagTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetTitle("H_{T} > 410 Btag tirgger efficiency");
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_HT410_BtagTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_HT410_BtagTrigMass_ctau0->SetLineColor(9);
    h_mass_HT410_BtagTrigMass_ctau0->SetMarkerColor(9);
    h_mass_HT410_BtagTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_HT410_BtagTrigMass_ctau0->SetMarkerSize(1);
    h_mass_HT410_BtagTrigMass_ctau0->Draw("HIST p");

    h_mass_HT410_BtagTrigMass_ctau1->SetLineColor(8);
    h_mass_HT410_BtagTrigMass_ctau1->SetMarkerColor(8);
    h_mass_HT410_BtagTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_HT410_BtagTrigMass_ctau1->SetMarkerSize(1);
    h_mass_HT410_BtagTrigMass_ctau1->Draw("HIST p same");

    h_mass_HT410_BtagTrigMass_ctau10->SetLineColor(2);
    h_mass_HT410_BtagTrigMass_ctau10->SetMarkerColor(2);
    h_mass_HT410_BtagTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_HT410_BtagTrigMass_ctau10->SetMarkerSize(1);
    h_mass_HT410_BtagTrigMass_ctau10->Draw("HIST p same");

    h_mass_HT410_BtagTrigMass_ctau100->SetLineColor(1);
    h_mass_HT410_BtagTrigMass_ctau100->SetMarkerColor(1);
    h_mass_HT410_BtagTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_HT410_BtagTrigMass_ctau100->SetMarkerSize(1);
    h_mass_HT410_BtagTrigMass_ctau100->Draw("HIST p same");

    h_mass_HT410_BtagTrigMass_ctau1000->SetLineColor(4);
    h_mass_HT410_BtagTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_HT410_BtagTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_HT410_BtagTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_HT410_BtagTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_HT410_BtagTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_HT410_BtagTrigMass->SetBorderSize(0);
    leg_HT410_BtagTrigMass->SetTextSize(0.03);
    leg_HT410_BtagTrigMass->SetLineColor(1);
    leg_HT410_BtagTrigMass->SetLineStyle(1);
    leg_HT410_BtagTrigMass->SetLineWidth(1);
    leg_HT410_BtagTrigMass->SetFillColor(0);
    leg_HT410_BtagTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_HT410_BtagTrigMass = leg_HT410_BtagTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_HT410_BtagTrigMass->SetMarkerColor(9);
    entry_HT410_BtagTrigMass->SetMarkerStyle(26);
    entry_HT410_BtagTrigMass->SetMarkerSize(2);
    entry_HT410_BtagTrigMass->SetTextFont(42);
    entry_HT410_BtagTrigMass = leg_HT410_BtagTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_HT410_BtagTrigMass->SetMarkerColor(8);
    entry_HT410_BtagTrigMass->SetMarkerStyle(4);
    entry_HT410_BtagTrigMass->SetMarkerSize(2);
    entry_HT410_BtagTrigMass->SetTextFont(42);
    entry_HT410_BtagTrigMass = leg_HT410_BtagTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_HT410_BtagTrigMass->SetMarkerColor(2);
    entry_HT410_BtagTrigMass->SetMarkerStyle(5);
    entry_HT410_BtagTrigMass->SetMarkerSize(2);
    entry_HT410_BtagTrigMass->SetTextFont(42);
    entry_HT410_BtagTrigMass = leg_HT410_BtagTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_HT410_BtagTrigMass->SetMarkerColor(1);
    entry_HT410_BtagTrigMass->SetMarkerStyle(2);
    entry_HT410_BtagTrigMass->SetMarkerSize(2);
    entry_HT410_BtagTrigMass->SetTextFont(42);
    entry_HT410_BtagTrigMass = leg_HT410_BtagTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_HT410_BtagTrigMass->SetMarkerColor(4);
    entry_HT410_BtagTrigMass->SetMarkerStyle(3);
    entry_HT410_BtagTrigMass->SetMarkerSize(2);
    entry_HT410_BtagTrigMass->SetTextFont(42);
    leg_HT410_BtagTrigMass->Draw("HIST");

    Cavnas_HT410_BtagTrigMass->Modified();
    Cavnas_HT410_BtagTrigMass->cd();
    Cavnas_HT410_BtagTrigMass->SetSelected(Cavnas_HT410_BtagTrigMass);
    Cavnas_HT410_BtagTrigMass->SaveAs( "HT410_BtagTrig_mass_eff.pdf" );

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
