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

void bparkingEfficiencyPlotMacro() {

    TFile* inFile0  =  new TFile ("plots/bparkingDistributionsMu12IP6/output_HtoSS_MS2_ctau0.root");
    TFile* inFile1  =  new TFile ("plots/bparkingDistributionsMu12IP6/output_HtoSS_MS2_ctau1.root");
    TFile* inFile2  =  new TFile ("plots/bparkingDistributionsMu12IP6/output_HtoSS_MS2_ctau10.root");
    TFile* inFile3  =  new TFile ("plots/bparkingDistributionsMu12IP6/output_HtoSS_MS2_ctau100.root");
    TFile* inFile4  =  new TFile ("plots/bparkingDistributionsMu12IP6/output_HtoSS_MS2_ctau1000.root");

    // pT

    TH1F* h_leadingMuonPt_bParkingTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_bParkingTrig_ratio");
    TH1F* h_leadingMuonPt_bParkingTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_bParkingTrig_ratio");
    TH1F* h_leadingMuonPt_bParkingTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_bParkingTrig_ratio");
    TH1F* h_leadingMuonPt_bParkingTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_bParkingTrig_ratio");
    TH1F* h_leadingMuonPt_bParkingTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigPt = new TCanvas("Cavnas_bParkingTrigPt", "Cavnas_bParkingTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigPt->SetFillColor(0);
    Cavnas_bParkingTrigPt->SetBorderMode(0);
    Cavnas_bParkingTrigPt->SetBorderSize(2);
    Cavnas_bParkingTrigPt->SetLeftMargin(0.15);
    Cavnas_bParkingTrigPt->SetRightMargin(0.05);
    Cavnas_bParkingTrigPt->SetTopMargin(0.05);
    Cavnas_bParkingTrigPt->SetBottomMargin(0.15);
    Cavnas_bParkingTrigPt->SetGridy();
    Cavnas_bParkingTrigPt->SetTickx(1);
    Cavnas_bParkingTrigPt->SetTicky(1);
    Cavnas_bParkingTrigPt->SetFrameBorderMode(0);
    Cavnas_bParkingTrigPt->cd();

    h_leadingMuonPt_bParkingTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_bParkingTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_bParkingTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_bParkingTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_bParkingTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_bParkingTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_bParkingTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_bParkingTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_bParkingTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_bParkingTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_bParkingTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_bParkingTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_bParkingTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_bParkingTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_bParkingTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_bParkingTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_bParkingTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_bParkingTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_bParkingTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_bParkingTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_bParkingTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_bParkingTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_bParkingTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_bParkingTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_bParkingTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_bParkingTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_bParkingTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_bParkingTrigPt->SetBorderSize(0);
    leg_bParkingTrigPt->SetTextSize(0.03);
    leg_bParkingTrigPt->SetLineColor(1);
    leg_bParkingTrigPt->SetLineStyle(1);
    leg_bParkingTrigPt->SetLineWidth(1);
    leg_bParkingTrigPt->SetFillColor(0);
    leg_bParkingTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigPt = leg_bParkingTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigPt->SetMarkerColor(9);
    entry_bParkingTrigPt->SetMarkerStyle(26);
    entry_bParkingTrigPt->SetMarkerSize(2);
    entry_bParkingTrigPt->SetTextFont(42);
    entry_bParkingTrigPt = leg_bParkingTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigPt->SetMarkerColor(8);
    entry_bParkingTrigPt->SetMarkerStyle(4);
    entry_bParkingTrigPt->SetMarkerSize(2);
    entry_bParkingTrigPt->SetTextFont(42);
    entry_bParkingTrigPt = leg_bParkingTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigPt->SetMarkerColor(2);
    entry_bParkingTrigPt->SetMarkerStyle(5);
    entry_bParkingTrigPt->SetMarkerSize(2);
    entry_bParkingTrigPt->SetTextFont(42);
    entry_bParkingTrigPt = leg_bParkingTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigPt->SetMarkerColor(1);
    entry_bParkingTrigPt->SetMarkerStyle(2);
    entry_bParkingTrigPt->SetMarkerSize(2);
    entry_bParkingTrigPt->SetTextFont(42);
    entry_bParkingTrigPt = leg_bParkingTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigPt->SetMarkerColor(4);
    entry_bParkingTrigPt->SetMarkerStyle(3);
    entry_bParkingTrigPt->SetMarkerSize(2);
    entry_bParkingTrigPt->SetTextFont(42);
    leg_bParkingTrigPt->Draw("HIST");

    Cavnas_bParkingTrigPt->Modified();
    Cavnas_bParkingTrigPt->cd();
    Cavnas_bParkingTrigPt->SetSelected(Cavnas_bParkingTrigPt);
    Cavnas_bParkingTrigPt->SaveAs("bParkingTrig_pT_eff.pdf");

    // eta

    TH1F* h_leadingMuonEta_bParkingTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_bParkingTrig_ratio");
    TH1F* h_leadingMuonEta_bParkingTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_bParkingTrig_ratio");
    TH1F* h_leadingMuonEta_bParkingTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_bParkingTrig_ratio");
    TH1F* h_leadingMuonEta_bParkingTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_bParkingTrig_ratio");
    TH1F* h_leadingMuonEta_bParkingTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigEta = new TCanvas("Cavnas_bParkingTrigEta", "Cavnas_bParkingTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigEta->SetFillColor(0);
    Cavnas_bParkingTrigEta->SetBorderMode(0);
    Cavnas_bParkingTrigEta->SetBorderSize(2);
    Cavnas_bParkingTrigEta->SetLeftMargin(0.15);
    Cavnas_bParkingTrigEta->SetRightMargin(0.05);
    Cavnas_bParkingTrigEta->SetTopMargin(0.05);
    Cavnas_bParkingTrigEta->SetBottomMargin(0.15);
    Cavnas_bParkingTrigEta->SetGridy();
    Cavnas_bParkingTrigEta->SetTickx(1);
    Cavnas_bParkingTrigEta->SetTicky(1);
    Cavnas_bParkingTrigEta->SetFrameBorderMode(0);
    Cavnas_bParkingTrigEta->cd();

    h_leadingMuonEta_bParkingTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_bParkingTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_bParkingTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_bParkingTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_bParkingTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_bParkingTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_bParkingTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_bParkingTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_bParkingTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_bParkingTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_bParkingTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_bParkingTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_bParkingTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_bParkingTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_bParkingTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_bParkingTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_bParkingTrigEta_ctau10->Draw("HIST p same");

    h_leadingMuonEta_bParkingTrigEta_ctau100->SetLineColor(1);
    h_leadingMuonEta_bParkingTrigEta_ctau100->SetMarkerColor(1);
    h_leadingMuonEta_bParkingTrigEta_ctau100->SetMarkerStyle(2);
    h_leadingMuonEta_bParkingTrigEta_ctau100->SetMarkerSize(1);
    h_leadingMuonEta_bParkingTrigEta_ctau100->Draw("HIST p same");

    h_leadingMuonEta_bParkingTrigEta_ctau1000->SetLineColor(4);
    h_leadingMuonEta_bParkingTrigEta_ctau1000->SetMarkerColor(4);
    h_leadingMuonEta_bParkingTrigEta_ctau1000->SetMarkerStyle(3);
    h_leadingMuonEta_bParkingTrigEta_ctau1000->SetMarkerSize(1);
    h_leadingMuonEta_bParkingTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_bParkingTrigEta->SetBorderSize(0);
    leg_bParkingTrigEta->SetTextSize(0.03);
    leg_bParkingTrigEta->SetLineColor(1);
    leg_bParkingTrigEta->SetLineStyle(1);
    leg_bParkingTrigEta->SetLineWidth(1);
    leg_bParkingTrigEta->SetFillColor(0);
    leg_bParkingTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigEta = leg_bParkingTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigEta->SetMarkerColor(9);
    entry_bParkingTrigEta->SetMarkerStyle(26);
    entry_bParkingTrigEta->SetMarkerSize(2);
    entry_bParkingTrigEta->SetTextFont(42);
    entry_bParkingTrigEta = leg_bParkingTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigEta->SetMarkerColor(8);
    entry_bParkingTrigEta->SetMarkerStyle(4);
    entry_bParkingTrigEta->SetMarkerSize(2);
    entry_bParkingTrigEta->SetTextFont(42);
    entry_bParkingTrigEta = leg_bParkingTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigEta->SetMarkerColor(2);
    entry_bParkingTrigEta->SetMarkerStyle(5);
    entry_bParkingTrigEta->SetMarkerSize(2);
    entry_bParkingTrigEta->SetTextFont(42);
    entry_bParkingTrigEta = leg_bParkingTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigEta->SetMarkerColor(1);
    entry_bParkingTrigEta->SetMarkerStyle(2);
    entry_bParkingTrigEta->SetMarkerSize(2);
    entry_bParkingTrigEta->SetTextFont(42);
    entry_bParkingTrigEta = leg_bParkingTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigEta->SetMarkerColor(4);
    entry_bParkingTrigEta->SetMarkerStyle(3);
    entry_bParkingTrigEta->SetMarkerSize(2);
    entry_bParkingTrigEta->SetTextFont(42);
    leg_bParkingTrigEta->Draw("HIST");

    Cavnas_bParkingTrigEta->Modified();
    Cavnas_bParkingTrigEta->cd();
    Cavnas_bParkingTrigEta->SetSelected(Cavnas_bParkingTrigEta);
    Cavnas_bParkingTrigEta->SaveAs( "bParkingTrig_eta_eff.pdf" );

    // d0

    TH1F* h_leadingMuonD0_bParkingTrigD0_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonD0_bParkingTrig_ratio");
    TH1F* h_leadingMuonD0_bParkingTrigD0_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonD0_bParkingTrig_ratio");
    TH1F* h_leadingMuonD0_bParkingTrigD0_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonD0_bParkingTrig_ratio");
    TH1F* h_leadingMuonD0_bParkingTrigD0_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonD0_bParkingTrig_ratio");
    TH1F* h_leadingMuonD0_bParkingTrigD0_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonD0_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigD0 = new TCanvas("Cavnas_bParkingTrigD0", "Cavnas_bParkingTrigD0",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigD0->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigD0->SetFillColor(0);
    Cavnas_bParkingTrigD0->SetBorderMode(0);
    Cavnas_bParkingTrigD0->SetBorderSize(2);
    Cavnas_bParkingTrigD0->SetLeftMargin(0.15);
    Cavnas_bParkingTrigD0->SetRightMargin(0.05);
    Cavnas_bParkingTrigD0->SetTopMargin(0.05);
    Cavnas_bParkingTrigD0->SetBottomMargin(0.15);
    Cavnas_bParkingTrigD0->SetGridy();
    Cavnas_bParkingTrigD0->SetTickx(1);
    Cavnas_bParkingTrigD0->SetTicky(1);
    Cavnas_bParkingTrigD0->SetFrameBorderMode(0);
    Cavnas_bParkingTrigD0->cd();

    h_leadingMuonD0_bParkingTrigD0_ctau0->SetTitle("");
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetTitle("leading muon d_{0} [cm]");
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonD0_bParkingTrigD0_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonD0_bParkingTrigD0_ctau0->SetLineColor(9);
    h_leadingMuonD0_bParkingTrigD0_ctau0->SetMarkerColor(9);
    h_leadingMuonD0_bParkingTrigD0_ctau0->SetMarkerStyle(26);
    h_leadingMuonD0_bParkingTrigD0_ctau0->SetMarkerSize(1);
    h_leadingMuonD0_bParkingTrigD0_ctau0->Draw("HIST p");

    h_leadingMuonD0_bParkingTrigD0_ctau1->SetLineColor(8);
    h_leadingMuonD0_bParkingTrigD0_ctau1->SetMarkerColor(8);
    h_leadingMuonD0_bParkingTrigD0_ctau1->SetMarkerStyle(4);
    h_leadingMuonD0_bParkingTrigD0_ctau1->SetMarkerSize(1);
    h_leadingMuonD0_bParkingTrigD0_ctau1->Draw("HIST p same");

    h_leadingMuonD0_bParkingTrigD0_ctau10->SetLineColor(2);
    h_leadingMuonD0_bParkingTrigD0_ctau10->SetMarkerColor(2);
    h_leadingMuonD0_bParkingTrigD0_ctau10->SetMarkerStyle(5);
    h_leadingMuonD0_bParkingTrigD0_ctau10->SetMarkerSize(1);
    h_leadingMuonD0_bParkingTrigD0_ctau10->Draw("HIST p same");

    h_leadingMuonD0_bParkingTrigD0_ctau100->SetLineColor(1);
    h_leadingMuonD0_bParkingTrigD0_ctau100->SetMarkerColor(1);
    h_leadingMuonD0_bParkingTrigD0_ctau100->SetMarkerStyle(2);
    h_leadingMuonD0_bParkingTrigD0_ctau100->SetMarkerSize(1);
    h_leadingMuonD0_bParkingTrigD0_ctau100->Draw("HIST p same");

    h_leadingMuonD0_bParkingTrigD0_ctau1000->SetLineColor(4);
    h_leadingMuonD0_bParkingTrigD0_ctau1000->SetMarkerColor(4);
    h_leadingMuonD0_bParkingTrigD0_ctau1000->SetMarkerStyle(3);
    h_leadingMuonD0_bParkingTrigD0_ctau1000->SetMarkerSize(1);
    h_leadingMuonD0_bParkingTrigD0_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigD0 = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_bParkingTrigD0->SetBorderSize(0);
    leg_bParkingTrigD0->SetTextSize(0.03);
    leg_bParkingTrigD0->SetLineColor(1);
    leg_bParkingTrigD0->SetLineStyle(1);
    leg_bParkingTrigD0->SetLineWidth(1);
    leg_bParkingTrigD0->SetFillColor(0);
    leg_bParkingTrigD0->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigD0 = leg_bParkingTrigD0->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigD0->SetMarkerColor(9);
    entry_bParkingTrigD0->SetMarkerStyle(26);
    entry_bParkingTrigD0->SetMarkerSize(2);
    entry_bParkingTrigD0->SetTextFont(42);
    entry_bParkingTrigD0 = leg_bParkingTrigD0->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigD0->SetMarkerColor(8);
    entry_bParkingTrigD0->SetMarkerStyle(4);
    entry_bParkingTrigD0->SetMarkerSize(2);
    entry_bParkingTrigD0->SetTextFont(42);
    entry_bParkingTrigD0 = leg_bParkingTrigD0->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigD0->SetMarkerColor(2);
    entry_bParkingTrigD0->SetMarkerStyle(5);
    entry_bParkingTrigD0->SetMarkerSize(2);
    entry_bParkingTrigD0->SetTextFont(42);
    entry_bParkingTrigD0 = leg_bParkingTrigD0->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigD0->SetMarkerColor(1);
    entry_bParkingTrigD0->SetMarkerStyle(2);
    entry_bParkingTrigD0->SetMarkerSize(2);
    entry_bParkingTrigD0->SetTextFont(42);
    entry_bParkingTrigD0 = leg_bParkingTrigD0->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigD0->SetMarkerColor(4);
    entry_bParkingTrigD0->SetMarkerStyle(3);
    entry_bParkingTrigD0->SetMarkerSize(2);
    entry_bParkingTrigD0->SetTextFont(42);
    leg_bParkingTrigD0->Draw("HIST");

    Cavnas_bParkingTrigD0->Modified();
    Cavnas_bParkingTrigD0->cd();
    Cavnas_bParkingTrigD0->SetSelected(Cavnas_bParkingTrigD0);
    Cavnas_bParkingTrigD0->SaveAs("bParkingTrig_D0_eff.pdf");

    // Dz

    TH1F* h_leadingMuonDz_bParkingTrigDz_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonDz_bParkingTrig_ratio");
    TH1F* h_leadingMuonDz_bParkingTrigDz_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonDz_bParkingTrig_ratio");
    TH1F* h_leadingMuonDz_bParkingTrigDz_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonDz_bParkingTrig_ratio");
    TH1F* h_leadingMuonDz_bParkingTrigDz_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonDz_bParkingTrig_ratio");
    TH1F* h_leadingMuonDz_bParkingTrigDz_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonDz_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigDz = new TCanvas("Cavnas_bParkingTrigDz", "Cavnas_bParkingTrigDz",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigDz->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigDz->SetFillColor(0);
    Cavnas_bParkingTrigDz->SetBorderMode(0);
    Cavnas_bParkingTrigDz->SetBorderSize(2);
    Cavnas_bParkingTrigDz->SetLeftMargin(0.15);
    Cavnas_bParkingTrigDz->SetRightMargin(0.05);
    Cavnas_bParkingTrigDz->SetTopMargin(0.05);
    Cavnas_bParkingTrigDz->SetBottomMargin(0.15);
    Cavnas_bParkingTrigDz->SetGridy();
    Cavnas_bParkingTrigDz->SetTickx(1);
    Cavnas_bParkingTrigDz->SetTicky(1);
    Cavnas_bParkingTrigDz->SetFrameBorderMode(0);
    Cavnas_bParkingTrigDz->cd();

    h_leadingMuonDz_bParkingTrigDz_ctau0->SetTitle("");
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetTitle("leading muon d_{z} [cm]");
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonDz_bParkingTrigDz_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonDz_bParkingTrigDz_ctau0->SetLineColor(9);
    h_leadingMuonDz_bParkingTrigDz_ctau0->SetMarkerColor(9);
    h_leadingMuonDz_bParkingTrigDz_ctau0->SetMarkerStyle(26);
    h_leadingMuonDz_bParkingTrigDz_ctau0->SetMarkerSize(1);
    h_leadingMuonDz_bParkingTrigDz_ctau0->Draw("HIST p");

    h_leadingMuonDz_bParkingTrigDz_ctau1->SetLineColor(8);
    h_leadingMuonDz_bParkingTrigDz_ctau1->SetMarkerColor(8);
    h_leadingMuonDz_bParkingTrigDz_ctau1->SetMarkerStyle(4);
    h_leadingMuonDz_bParkingTrigDz_ctau1->SetMarkerSize(1);
    h_leadingMuonDz_bParkingTrigDz_ctau1->Draw("HIST p same");

    h_leadingMuonDz_bParkingTrigDz_ctau10->SetLineColor(2);
    h_leadingMuonDz_bParkingTrigDz_ctau10->SetMarkerColor(2);
    h_leadingMuonDz_bParkingTrigDz_ctau10->SetMarkerStyle(5);
    h_leadingMuonDz_bParkingTrigDz_ctau10->SetMarkerSize(1);
    h_leadingMuonDz_bParkingTrigDz_ctau10->Draw("HIST p same");

    h_leadingMuonDz_bParkingTrigDz_ctau100->SetLineColor(1);
    h_leadingMuonDz_bParkingTrigDz_ctau100->SetMarkerColor(1);
    h_leadingMuonDz_bParkingTrigDz_ctau100->SetMarkerStyle(2);
    h_leadingMuonDz_bParkingTrigDz_ctau100->SetMarkerSize(1);
    h_leadingMuonDz_bParkingTrigDz_ctau100->Draw("HIST p same");

    h_leadingMuonDz_bParkingTrigDz_ctau1000->SetLineColor(4);
    h_leadingMuonDz_bParkingTrigDz_ctau1000->SetMarkerColor(4);
    h_leadingMuonDz_bParkingTrigDz_ctau1000->SetMarkerStyle(3);
    h_leadingMuonDz_bParkingTrigDz_ctau1000->SetMarkerSize(1);
    h_leadingMuonDz_bParkingTrigDz_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigDz = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_bParkingTrigDz->SetBorderSize(0);
    leg_bParkingTrigDz->SetTextSize(0.03);
    leg_bParkingTrigDz->SetLineColor(1);
    leg_bParkingTrigDz->SetLineStyle(1);
    leg_bParkingTrigDz->SetLineWidth(1);
    leg_bParkingTrigDz->SetFillColor(0);
    leg_bParkingTrigDz->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigDz = leg_bParkingTrigDz->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigDz->SetMarkerColor(9);
    entry_bParkingTrigDz->SetMarkerStyle(26);
    entry_bParkingTrigDz->SetMarkerSize(2);
    entry_bParkingTrigDz->SetTextFont(42);
    entry_bParkingTrigDz = leg_bParkingTrigDz->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigDz->SetMarkerColor(8);
    entry_bParkingTrigDz->SetMarkerStyle(4);
    entry_bParkingTrigDz->SetMarkerSize(2);
    entry_bParkingTrigDz->SetTextFont(42);
    entry_bParkingTrigDz = leg_bParkingTrigDz->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigDz->SetMarkerColor(2);
    entry_bParkingTrigDz->SetMarkerStyle(5);
    entry_bParkingTrigDz->SetMarkerSize(2);
    entry_bParkingTrigDz->SetTextFont(42);
    entry_bParkingTrigDz = leg_bParkingTrigDz->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigDz->SetMarkerColor(1);
    entry_bParkingTrigDz->SetMarkerStyle(2);
    entry_bParkingTrigDz->SetMarkerSize(2);
    entry_bParkingTrigDz->SetTextFont(42);
    entry_bParkingTrigDz = leg_bParkingTrigDz->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigDz->SetMarkerColor(4);
    entry_bParkingTrigDz->SetMarkerStyle(3);
    entry_bParkingTrigDz->SetMarkerSize(2);
    entry_bParkingTrigDz->SetTextFont(42);
    leg_bParkingTrigDz->Draw("HIST");

    Cavnas_bParkingTrigDz->Modified();
    Cavnas_bParkingTrigDz->cd();
    Cavnas_bParkingTrigDz->SetSelected(Cavnas_bParkingTrigDz);
    Cavnas_bParkingTrigDz->SaveAs("bParkingTrig_Dz_eff.pdf");

    // DeltaR

    TH1F* h_delR_bParkingTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_bParkingTrig_ratio");
    TH1F* h_delR_bParkingTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_bParkingTrig_ratio");
    TH1F* h_delR_bParkingTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_bParkingTrig_ratio");
    TH1F* h_delR_bParkingTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_bParkingTrig_ratio");
    TH1F* h_delR_bParkingTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigDelR = new TCanvas("Cavnas_bParkingTrigDelR", "Cavnas_bParkingTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigDelR->SetFillColor(0);
    Cavnas_bParkingTrigDelR->SetBorderMode(0);
    Cavnas_bParkingTrigDelR->SetBorderSize(2);
    Cavnas_bParkingTrigDelR->SetLeftMargin(0.15);
    Cavnas_bParkingTrigDelR->SetRightMargin(0.05);
    Cavnas_bParkingTrigDelR->SetTopMargin(0.05);
    Cavnas_bParkingTrigDelR->SetBottomMargin(0.15);
    Cavnas_bParkingTrigDelR->SetGridy();
    Cavnas_bParkingTrigDelR->SetTickx(1);
    Cavnas_bParkingTrigDelR->SetTicky(1);
    Cavnas_bParkingTrigDelR->SetFrameBorderMode(0);
    Cavnas_bParkingTrigDelR->cd();

    h_delR_bParkingTrigDelR_ctau0->SetTitle("");
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_bParkingTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_bParkingTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_bParkingTrigDelR_ctau0->SetLineColor(9);
    h_delR_bParkingTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_bParkingTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_bParkingTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_bParkingTrigDelR_ctau0->Draw("HIST p");

    h_delR_bParkingTrigDelR_ctau1->SetLineColor(8);
    h_delR_bParkingTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_bParkingTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_bParkingTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_bParkingTrigDelR_ctau1->Draw("HIST p same");

    h_delR_bParkingTrigDelR_ctau10->SetLineColor(2);
    h_delR_bParkingTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_bParkingTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_bParkingTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_bParkingTrigDelR_ctau10->Draw("HIST p same");

    h_delR_bParkingTrigDelR_ctau100->SetLineColor(1);
    h_delR_bParkingTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_bParkingTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_bParkingTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_bParkingTrigDelR_ctau100->Draw("HIST p same");

    h_delR_bParkingTrigDelR_ctau1000->SetLineColor(4);
    h_delR_bParkingTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_bParkingTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_bParkingTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_bParkingTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigDelR->SetBorderSize(0);
    leg_bParkingTrigDelR->SetTextSize(0.03);
    leg_bParkingTrigDelR->SetLineColor(1);
    leg_bParkingTrigDelR->SetLineStyle(1);
    leg_bParkingTrigDelR->SetLineWidth(1);
    leg_bParkingTrigDelR->SetFillColor(0);
    leg_bParkingTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigDelR = leg_bParkingTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigDelR->SetMarkerColor(9);
    entry_bParkingTrigDelR->SetMarkerStyle(26);
    entry_bParkingTrigDelR->SetMarkerSize(2);
    entry_bParkingTrigDelR->SetTextFont(42);
    entry_bParkingTrigDelR = leg_bParkingTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigDelR->SetMarkerColor(8);
    entry_bParkingTrigDelR->SetMarkerStyle(4);
    entry_bParkingTrigDelR->SetMarkerSize(2);
    entry_bParkingTrigDelR->SetTextFont(42);
    entry_bParkingTrigDelR = leg_bParkingTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigDelR->SetMarkerColor(2);
    entry_bParkingTrigDelR->SetMarkerStyle(5);
    entry_bParkingTrigDelR->SetMarkerSize(2);
    entry_bParkingTrigDelR->SetTextFont(42);
    entry_bParkingTrigDelR = leg_bParkingTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigDelR->SetMarkerColor(1);
    entry_bParkingTrigDelR->SetMarkerStyle(2);
    entry_bParkingTrigDelR->SetMarkerSize(2);
    entry_bParkingTrigDelR->SetTextFont(42);
    entry_bParkingTrigDelR = leg_bParkingTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigDelR->SetMarkerColor(4);
    entry_bParkingTrigDelR->SetMarkerStyle(3);
    entry_bParkingTrigDelR->SetMarkerSize(2);
    entry_bParkingTrigDelR->SetTextFont(42);
    leg_bParkingTrigDelR->Draw("HIST");

    Cavnas_bParkingTrigDelR->Modified();
    Cavnas_bParkingTrigDelR->cd();
    Cavnas_bParkingTrigDelR->SetSelected(Cavnas_bParkingTrigDelR);
    Cavnas_bParkingTrigDelR->SaveAs( "bParkingTrig_DelR_eff.pdf" );

///// mass

    // mass

    TH1F* h_mass_bParkingTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_bParkingTrig_ratio");
    TH1F* h_mass_bParkingTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_bParkingTrig_ratio");
    TH1F* h_mass_bParkingTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_bParkingTrig_ratio");
    TH1F* h_mass_bParkingTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_bParkingTrig_ratio");
    TH1F* h_mass_bParkingTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigMass = new TCanvas("Cavnas_bParkingTrigMass", "Cavnas_bParkingTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigMass->SetFillColor(0);
    Cavnas_bParkingTrigMass->SetBorderMode(0);
    Cavnas_bParkingTrigMass->SetBorderSize(2);
    Cavnas_bParkingTrigMass->SetLeftMargin(0.15);
    Cavnas_bParkingTrigMass->SetRightMargin(0.05);
    Cavnas_bParkingTrigMass->SetTopMargin(0.05);
    Cavnas_bParkingTrigMass->SetBottomMargin(0.15);
    Cavnas_bParkingTrigMass->SetGridy();
    Cavnas_bParkingTrigMass->SetTickx(1);
    Cavnas_bParkingTrigMass->SetTicky(1);
    Cavnas_bParkingTrigMass->SetFrameBorderMode(0);
    Cavnas_bParkingTrigMass->cd();

    h_mass_bParkingTrigMass_ctau0->SetTitle("");
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_bParkingTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_bParkingTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_bParkingTrigMass_ctau0->SetLineColor(9);
    h_mass_bParkingTrigMass_ctau0->SetMarkerColor(9);
    h_mass_bParkingTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_bParkingTrigMass_ctau0->SetMarkerSize(1);
    h_mass_bParkingTrigMass_ctau0->Draw("HIST p");

    h_mass_bParkingTrigMass_ctau1->SetLineColor(8);
    h_mass_bParkingTrigMass_ctau1->SetMarkerColor(8);
    h_mass_bParkingTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_bParkingTrigMass_ctau1->SetMarkerSize(1);
    h_mass_bParkingTrigMass_ctau1->Draw("HIST p same");

    h_mass_bParkingTrigMass_ctau10->SetLineColor(2);
    h_mass_bParkingTrigMass_ctau10->SetMarkerColor(2);
    h_mass_bParkingTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_bParkingTrigMass_ctau10->SetMarkerSize(1);
    h_mass_bParkingTrigMass_ctau10->Draw("HIST p same");

    h_mass_bParkingTrigMass_ctau100->SetLineColor(1);
    h_mass_bParkingTrigMass_ctau100->SetMarkerColor(1);
    h_mass_bParkingTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_bParkingTrigMass_ctau100->SetMarkerSize(1);
    h_mass_bParkingTrigMass_ctau100->Draw("HIST p same");

    h_mass_bParkingTrigMass_ctau1000->SetLineColor(4);
    h_mass_bParkingTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_bParkingTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_bParkingTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_bParkingTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigMass->SetBorderSize(0);
    leg_bParkingTrigMass->SetTextSize(0.03);
    leg_bParkingTrigMass->SetLineColor(1);
    leg_bParkingTrigMass->SetLineStyle(1);
    leg_bParkingTrigMass->SetLineWidth(1);
    leg_bParkingTrigMass->SetFillColor(0);
    leg_bParkingTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigMass = leg_bParkingTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigMass->SetMarkerColor(9);
    entry_bParkingTrigMass->SetMarkerStyle(26);
    entry_bParkingTrigMass->SetMarkerSize(2);
    entry_bParkingTrigMass->SetTextFont(42);
    entry_bParkingTrigMass = leg_bParkingTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigMass->SetMarkerColor(8);
    entry_bParkingTrigMass->SetMarkerStyle(4);
    entry_bParkingTrigMass->SetMarkerSize(2);
    entry_bParkingTrigMass->SetTextFont(42);
    entry_bParkingTrigMass = leg_bParkingTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigMass->SetMarkerColor(2);
    entry_bParkingTrigMass->SetMarkerStyle(5);
    entry_bParkingTrigMass->SetMarkerSize(2);
    entry_bParkingTrigMass->SetTextFont(42);
    entry_bParkingTrigMass = leg_bParkingTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigMass->SetMarkerColor(1);
    entry_bParkingTrigMass->SetMarkerStyle(2);
    entry_bParkingTrigMass->SetMarkerSize(2);
    entry_bParkingTrigMass->SetTextFont(42);
    entry_bParkingTrigMass = leg_bParkingTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigMass->SetMarkerColor(4);
    entry_bParkingTrigMass->SetMarkerStyle(3);
    entry_bParkingTrigMass->SetMarkerSize(2);
    entry_bParkingTrigMass->SetTextFont(42);
    leg_bParkingTrigMass->Draw("HIST");

    Cavnas_bParkingTrigMass->Modified();
    Cavnas_bParkingTrigMass->cd();
    Cavnas_bParkingTrigMass->SetSelected(Cavnas_bParkingTrigMass);
    Cavnas_bParkingTrigMass->SaveAs( "bParkingTrig_mass_eff.pdf" );

    // Impact Trans Sig

    TH1F* h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig = new TCanvas("Cavnas_bParkingTrigImpactTransSig", "Cavnas_bParkingTrigImpactTransSig",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig->SetGridy();
    Cavnas_bParkingTrigImpactTransSig->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig->cd();

    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->SetTitle("");
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetTitle("2D Sig");
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->SetLineColor(9);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau0->Draw("HIST p");

    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1->SetLineColor(8);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1->Draw("HIST p same");

    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10->SetLineColor(2);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau10->Draw("HIST p same");

    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100->SetLineColor(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau100->Draw("HIST p same");

    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000->SetLineColor(4);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_bParkingTrigImpactTransSig_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig->SetLineColor(1);
    leg_bParkingTrigImpactTransSig->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig->SetFillColor(0);
    leg_bParkingTrigImpactTransSig->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig = leg_bParkingTrigImpactTransSig->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig->SetTextFont(42);
    entry_bParkingTrigImpactTransSig = leg_bParkingTrigImpactTransSig->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig->SetTextFont(42);
    entry_bParkingTrigImpactTransSig = leg_bParkingTrigImpactTransSig->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig->SetTextFont(42);
    entry_bParkingTrigImpactTransSig = leg_bParkingTrigImpactTransSig->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig->SetTextFont(42);
    entry_bParkingTrigImpactTransSig = leg_bParkingTrigImpactTransSig->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig->SetTextFont(42);
    leg_bParkingTrigImpactTransSig->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig->Modified();
    Cavnas_bParkingTrigImpactTransSig->cd();
    Cavnas_bParkingTrigImpactTransSig->SetSelected(Cavnas_bParkingTrigImpactTransSig);
    Cavnas_bParkingTrigImpactTransSig->SaveAs( "bParkingTrig_ImpactTransSig_eff.pdf" );

    // Impact 3D Sig
    TH1F* h_Impact3DSig_bParkingTrigImpact3DSig_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpact3DSig_bParkingTrig_ratio");
    TH1F* h_Impact3DSig_bParkingTrigImpact3DSig_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpact3DSig_bParkingTrig_ratio");
    TH1F* h_Impact3DSig_bParkingTrigImpact3DSig_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpact3DSig_bParkingTrig_ratio");
    TH1F* h_Impact3DSig_bParkingTrigImpact3DSig_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpact3DSig_bParkingTrig_ratio");
    TH1F* h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpact3DSig_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpact3DSig = new TCanvas("Cavnas_bParkingTrigImpact3DSig", "Cavnas_bParkingTrigImpact3DSig",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpact3DSig->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpact3DSig->SetFillColor(0);
    Cavnas_bParkingTrigImpact3DSig->SetBorderMode(0);
    Cavnas_bParkingTrigImpact3DSig->SetBorderSize(2);
    Cavnas_bParkingTrigImpact3DSig->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpact3DSig->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpact3DSig->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpact3DSig->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpact3DSig->SetGridy();
    Cavnas_bParkingTrigImpact3DSig->SetTickx(1);
    Cavnas_bParkingTrigImpact3DSig->SetTicky(1);
    Cavnas_bParkingTrigImpact3DSig->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpact3DSig->cd();

    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->SetTitle("");
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetTitle("3D Sig");
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetLabelFont(42);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetXaxis()->SetTitleFont(42);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetLabelFont(42);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetTitleOffset(0);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetTitleFont(42);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->GetYaxis()->SetNdivisions(510);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->SetLineColor(9);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->SetMarkerColor(9);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->SetMarkerStyle(26);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->SetMarkerSize(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau0->Draw("HIST p");

    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1->SetLineColor(8);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1->SetMarkerColor(8);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1->SetMarkerStyle(4);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1->SetMarkerSize(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1->Draw("HIST p same");

    h_Impact3DSig_bParkingTrigImpact3DSig_ctau10->SetLineColor(2);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau10->SetMarkerColor(2);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau10->SetMarkerStyle(5);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau10->SetMarkerSize(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau10->Draw("HIST p same");

    h_Impact3DSig_bParkingTrigImpact3DSig_ctau100->SetLineColor(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau100->SetMarkerColor(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau100->SetMarkerStyle(2);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau100->SetMarkerSize(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau100->Draw("HIST p same");

    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000->SetLineColor(4);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000->SetMarkerColor(4);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000->SetMarkerStyle(3);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000->SetMarkerSize(1);
    h_Impact3DSig_bParkingTrigImpact3DSig_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpact3DSig = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpact3DSig->SetBorderSize(0);
    leg_bParkingTrigImpact3DSig->SetTextSize(0.03);
    leg_bParkingTrigImpact3DSig->SetLineColor(1);
    leg_bParkingTrigImpact3DSig->SetLineStyle(1);
    leg_bParkingTrigImpact3DSig->SetLineWidth(1);
    leg_bParkingTrigImpact3DSig->SetFillColor(0);
    leg_bParkingTrigImpact3DSig->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpact3DSig = leg_bParkingTrigImpact3DSig->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpact3DSig->SetMarkerColor(9);
    entry_bParkingTrigImpact3DSig->SetMarkerStyle(26);
    entry_bParkingTrigImpact3DSig->SetMarkerSize(2);
    entry_bParkingTrigImpact3DSig->SetTextFont(42);
    entry_bParkingTrigImpact3DSig = leg_bParkingTrigImpact3DSig->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpact3DSig->SetMarkerColor(8);
    entry_bParkingTrigImpact3DSig->SetMarkerStyle(4);
    entry_bParkingTrigImpact3DSig->SetMarkerSize(2);
    entry_bParkingTrigImpact3DSig->SetTextFont(42);
    entry_bParkingTrigImpact3DSig = leg_bParkingTrigImpact3DSig->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpact3DSig->SetMarkerColor(2);
    entry_bParkingTrigImpact3DSig->SetMarkerStyle(5);
    entry_bParkingTrigImpact3DSig->SetMarkerSize(2);
    entry_bParkingTrigImpact3DSig->SetTextFont(42);
    entry_bParkingTrigImpact3DSig = leg_bParkingTrigImpact3DSig->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpact3DSig->SetMarkerColor(1);
    entry_bParkingTrigImpact3DSig->SetMarkerStyle(2);
    entry_bParkingTrigImpact3DSig->SetMarkerSize(2);
    entry_bParkingTrigImpact3DSig->SetTextFont(42);
    entry_bParkingTrigImpact3DSig = leg_bParkingTrigImpact3DSig->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpact3DSig->SetMarkerColor(4);
    entry_bParkingTrigImpact3DSig->SetMarkerStyle(3);
    entry_bParkingTrigImpact3DSig->SetMarkerSize(2);
    entry_bParkingTrigImpact3DSig->SetTextFont(42);
    leg_bParkingTrigImpact3DSig->Draw("HIST");

    Cavnas_bParkingTrigImpact3DSig->Modified();
    Cavnas_bParkingTrigImpact3DSig->cd();
    Cavnas_bParkingTrigImpact3DSig->SetSelected(Cavnas_bParkingTrigImpact3DSig);
    Cavnas_bParkingTrigImpact3DSig->SaveAs( "bParkingTrig_Impact3DSig_eff.pdf" );

    // pT (IP windows) (0to2)

    TH1F* h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_0to2 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_0to2", "Cavnas_bParkingTrigImpactTransSig_0to2",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_0to2->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_0to2->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_0to2->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_0to2->cd();

    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->SetTitle("");
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->SetLineColor(9);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau0->Draw("HIST p");

    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1->SetLineColor(8);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1->Draw("HIST p same");

    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10->SetLineColor(2);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau10->Draw("HIST p same");

    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100->SetLineColor(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau100->Draw("HIST p same");

    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000->SetLineColor(4);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_0to2_bParkingTrigImpactTransSig_0to2_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_0to2 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_0to2->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_0to2->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_0to2->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_0to2->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_0to2->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_0to2->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_0to2->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_0to2 = leg_bParkingTrigImpactTransSig_0to2->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to2->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to2 = leg_bParkingTrigImpactTransSig_0to2->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to2->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to2 = leg_bParkingTrigImpactTransSig_0to2->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to2->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to2 = leg_bParkingTrigImpactTransSig_0to2->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to2->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to2 = leg_bParkingTrigImpactTransSig_0to2->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_0to2->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to2->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_0to2->Draw("HIST");
    Cavnas_bParkingTrigImpactTransSig_0to2->Modified();
    Cavnas_bParkingTrigImpactTransSig_0to2->cd();
    Cavnas_bParkingTrigImpactTransSig_0to2->SetSelected(Cavnas_bParkingTrigImpactTransSig_0to2);
    Cavnas_bParkingTrigImpactTransSig_0to2->SaveAs( "bParkingTrig_ImpactTransSig_0to2_eff.pdf" );

    // pT (IP windows) (2to4)

    TH1F* h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_2to4 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_2to4", "Cavnas_bParkingTrigImpactTransSig_2to4",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_2to4->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_2to4->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_2to4->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_2to4->cd();

    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->SetTitle("");
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->SetLineColor(9);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau0->Draw("HIST p");

    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1->SetLineColor(8);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1->Draw("HIST p same");

    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10->SetLineColor(2);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau10->Draw("HIST p same");

    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100->SetLineColor(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau100->Draw("HIST p same");

    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000->SetLineColor(4);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_2to4_bParkingTrigImpactTransSig_2to4_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_2to4 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_2to4->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_2to4->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_2to4->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_2to4->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_2to4->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_2to4->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_2to4->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_2to4 = leg_bParkingTrigImpactTransSig_2to4->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_2to4->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_2to4 = leg_bParkingTrigImpactTransSig_2to4->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_2to4->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_2to4 = leg_bParkingTrigImpactTransSig_2to4->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_2to4->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_2to4 = leg_bParkingTrigImpactTransSig_2to4->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_2to4->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_2to4 = leg_bParkingTrigImpactTransSig_2to4->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_2to4->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_2to4->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_2to4->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_2to4->Modified();
    Cavnas_bParkingTrigImpactTransSig_2to4->cd();
    Cavnas_bParkingTrigImpactTransSig_2to4->SetSelected(Cavnas_bParkingTrigImpactTransSig_2to4);
    Cavnas_bParkingTrigImpactTransSig_2to4->SaveAs( "bParkingTrig_ImpactTransSig_2to4_eff.pdf" );

    // pT (IP windows) (4to6)

    TH1F* h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_4to6 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_4to6", "Cavnas_bParkingTrigImpactTransSig_4to6",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_4to6->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_4to6->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_4to6->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_4to6->cd();

    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->SetTitle("");
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->SetLineColor(9);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau0->Draw("HIST p");

    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1->SetLineColor(8);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1->Draw("HIST p same");

    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10->SetLineColor(2);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau10->Draw("HIST p same");

    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100->SetLineColor(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau100->Draw("HIST p same");

    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000->SetLineColor(4);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_4to6_bParkingTrigImpactTransSig_4to6_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_4to6 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_4to6->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_4to6->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_4to6->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_4to6->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_4to6->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_4to6->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_4to6->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_4to6 = leg_bParkingTrigImpactTransSig_4to6->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_4to6->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_4to6 = leg_bParkingTrigImpactTransSig_4to6->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_4to6->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_4to6 = leg_bParkingTrigImpactTransSig_4to6->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_4to6->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_4to6 = leg_bParkingTrigImpactTransSig_4to6->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_4to6->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_4to6 = leg_bParkingTrigImpactTransSig_4to6->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_4to6->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_4to6->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_4to6->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_4to6->Modified();
    Cavnas_bParkingTrigImpactTransSig_4to6->cd();
    Cavnas_bParkingTrigImpactTransSig_4to6->SetSelected(Cavnas_bParkingTrigImpactTransSig_4to6);
    Cavnas_bParkingTrigImpactTransSig_4to6->SaveAs( "bParkingTrig_ImpactTransSig_4to6_eff.pdf" );

    // pT (IP windows) (6to8)

    TH1F* h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_6to8 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_6to8", "Cavnas_bParkingTrigImpactTransSig_6to8",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_6to8->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_6to8->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_6to8->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_6to8->cd();

    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->SetTitle("");
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->SetLineColor(9);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau0->Draw("HIST p");

    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1->SetLineColor(8);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1->Draw("HIST p same");

    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10->SetLineColor(2);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau10->Draw("HIST p same");

    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100->SetLineColor(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau100->Draw("HIST p same");

    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000->SetLineColor(4);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_6to8_bParkingTrigImpactTransSig_6to8_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_6to8 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_6to8->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_6to8->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_6to8->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_6to8->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_6to8->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_6to8->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_6to8->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_6to8 = leg_bParkingTrigImpactTransSig_6to8->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_6to8->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_6to8 = leg_bParkingTrigImpactTransSig_6to8->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_6to8->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_6to8 = leg_bParkingTrigImpactTransSig_6to8->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_6to8->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_6to8 = leg_bParkingTrigImpactTransSig_6to8->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_6to8->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_6to8 = leg_bParkingTrigImpactTransSig_6to8->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_6to8->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_6to8->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_6to8->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_6to8->Modified();
    Cavnas_bParkingTrigImpactTransSig_6to8->cd();
    Cavnas_bParkingTrigImpactTransSig_6to8->SetSelected(Cavnas_bParkingTrigImpactTransSig_6to8);
    Cavnas_bParkingTrigImpactTransSig_6to8->SaveAs( "bParkingTrig_ImpactTransSig_6to8_eff.pdf" );

    // pT (IP windows) (8to10)

    TH1F* h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_8to10 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_8to10", "Cavnas_bParkingTrigImpactTransSig_8to10",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_8to10->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_8to10->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_8to10->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_8to10->cd();

    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->SetTitle("");
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->SetLineColor(9);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau0->Draw("HIST p");

    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1->SetLineColor(8);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1->Draw("HIST p same");

    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10->SetLineColor(2);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau10->Draw("HIST p same");

    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100->SetLineColor(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau100->Draw("HIST p same");

    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000->SetLineColor(4);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_8to10_bParkingTrigImpactTransSig_8to10_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_8to10 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_8to10->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_8to10->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_8to10->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_8to10->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_8to10->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_8to10->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_8to10->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_8to10 = leg_bParkingTrigImpactTransSig_8to10->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_8to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_8to10 = leg_bParkingTrigImpactTransSig_8to10->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_8to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_8to10 = leg_bParkingTrigImpactTransSig_8to10->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_8to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_8to10 = leg_bParkingTrigImpactTransSig_8to10->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_8to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_8to10 = leg_bParkingTrigImpactTransSig_8to10->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_8to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_8to10->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_8to10->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_8to10->Modified();
    Cavnas_bParkingTrigImpactTransSig_8to10->cd();
    Cavnas_bParkingTrigImpactTransSig_8to10->SetSelected(Cavnas_bParkingTrigImpactTransSig_8to10);
    Cavnas_bParkingTrigImpactTransSig_8to10->SaveAs( "bParkingTrig_ImpactTransSig_8to10_eff.pdf" );

    // pT (IP windows) (0to10)

    TH1F* h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_0to10 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_0to10", "Cavnas_bParkingTrigImpactTransSig_0to10",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_0to10->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_0to10->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_0to10->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_0to10->cd();

    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->SetTitle("");
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->SetLineColor(9);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau0->Draw("HIST p");

    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1->SetLineColor(8);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1->Draw("HIST p same");

    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10->SetLineColor(2);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau10->Draw("HIST p same");

    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100->SetLineColor(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau100->Draw("HIST p same");

    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000->SetLineColor(4);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_0to10_bParkingTrigImpactTransSig_0to10_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_0to10 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_0to10->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_0to10->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_0to10->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_0to10->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_0to10->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_0to10->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_0to10->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_0to10 = leg_bParkingTrigImpactTransSig_0to10->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to10 = leg_bParkingTrigImpactTransSig_0to10->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to10 = leg_bParkingTrigImpactTransSig_0to10->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to10 = leg_bParkingTrigImpactTransSig_0to10->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to10->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_0to10 = leg_bParkingTrigImpactTransSig_0to10->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_0to10->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_0to10->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_0to10->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_0to10->Modified();
    Cavnas_bParkingTrigImpactTransSig_0to10->cd();
    Cavnas_bParkingTrigImpactTransSig_0to10->SetSelected(Cavnas_bParkingTrigImpactTransSig_0to10);
    Cavnas_bParkingTrigImpactTransSig_0to10->SaveAs( "bParkingTrig_ImpactTransSig_0to10_eff.pdf" );

    // pT (IP windows) (10to20)

    TH1F* h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_10to20 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_10to20", "Cavnas_bParkingTrigImpactTransSig_10to20",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_10to20->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_10to20->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_10to20->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_10to20->cd();

    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->SetTitle("");
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->SetLineColor(9);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau0->Draw("HIST p");

    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1->SetLineColor(8);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1->Draw("HIST p same");

    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10->SetLineColor(2);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau10->Draw("HIST p same");

    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100->SetLineColor(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau100->Draw("HIST p same");

    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000->SetLineColor(4);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_10to20_bParkingTrigImpactTransSig_10to20_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_10to20 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_10to20->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_10to20->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_10to20->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_10to20->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_10to20->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_10to20->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_10to20->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_10to20 = leg_bParkingTrigImpactTransSig_10to20->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_10to20->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_10to20 = leg_bParkingTrigImpactTransSig_10to20->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_10to20->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_10to20 = leg_bParkingTrigImpactTransSig_10to20->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_10to20->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_10to20 = leg_bParkingTrigImpactTransSig_10to20->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_10to20->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_10to20 = leg_bParkingTrigImpactTransSig_10to20->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_10to20->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_10to20->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_10to20->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_10to20->Modified();
    Cavnas_bParkingTrigImpactTransSig_10to20->cd();
    Cavnas_bParkingTrigImpactTransSig_10to20->SetSelected(Cavnas_bParkingTrigImpactTransSig_10to20);
    Cavnas_bParkingTrigImpactTransSig_10to20->SaveAs( "bParkingTrig_ImpactTransSig_10to20_eff.pdf" );

    // pT (IP windows) (20to30)

    TH1F* h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio");
    TH1F* h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio");

    TCanvas *Cavnas_bParkingTrigImpactTransSig_20to30 = new TCanvas("Cavnas_bParkingTrigImpactTransSig_20to30", "Cavnas_bParkingTrigImpactTransSig_20to30",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_bParkingTrigImpactTransSig_20to30->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetFillColor(0);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetBorderSize(2);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetLeftMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetRightMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetTopMargin(0.05);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetBottomMargin(0.15);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetGridy();
    Cavnas_bParkingTrigImpactTransSig_20to30->SetTickx(1);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetTicky(1);
    Cavnas_bParkingTrigImpactTransSig_20to30->SetFrameBorderMode(0);
    Cavnas_bParkingTrigImpactTransSig_20to30->cd();

    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->SetTitle("");
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetLabelFont(42);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetXaxis()->SetTitleFont(42);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetTitle("trigger efficiency");
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetLabelFont(42);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetTitleOffset(0);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetTitleFont(42);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->GetYaxis()->SetNdivisions(510);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->SetLineColor(9);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->SetMarkerColor(9);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->SetMarkerStyle(26);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->SetMarkerSize(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau0->Draw("HIST p");

    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1->SetLineColor(8);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1->SetMarkerColor(8);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1->SetMarkerStyle(4);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1->SetMarkerSize(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1->Draw("HIST p same");

    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10->SetLineColor(2);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10->SetMarkerColor(2);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10->SetMarkerStyle(5);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10->SetMarkerSize(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau10->Draw("HIST p same");

    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100->SetLineColor(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100->SetMarkerColor(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100->SetMarkerStyle(2);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100->SetMarkerSize(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau100->Draw("HIST p same");

    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000->SetLineColor(4);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000->SetMarkerColor(4);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000->SetMarkerStyle(3);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000->SetMarkerSize(1);
    h_ImpactTransSig_20to30_bParkingTrigImpactTransSig_20to30_ctau1000->Draw("HIST p same");

    TLegend *leg_bParkingTrigImpactTransSig_20to30 = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_bParkingTrigImpactTransSig_20to30->SetBorderSize(0);
    leg_bParkingTrigImpactTransSig_20to30->SetTextSize(0.03);
    leg_bParkingTrigImpactTransSig_20to30->SetLineColor(1);
    leg_bParkingTrigImpactTransSig_20to30->SetLineStyle(1);
    leg_bParkingTrigImpactTransSig_20to30->SetLineWidth(1);
    leg_bParkingTrigImpactTransSig_20to30->SetFillColor(0);
    leg_bParkingTrigImpactTransSig_20to30->SetFillStyle(1001);
    TLegendEntry *entry_bParkingTrigImpactTransSig_20to30 = leg_bParkingTrigImpactTransSig_20to30->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerColor(9);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerStyle(26);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_20to30->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_20to30 = leg_bParkingTrigImpactTransSig_20to30->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerColor(8);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerStyle(4);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_20to30->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_20to30 = leg_bParkingTrigImpactTransSig_20to30->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerColor(2);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerStyle(5);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_20to30->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_20to30 = leg_bParkingTrigImpactTransSig_20to30->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerColor(1);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerStyle(2);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_20to30->SetTextFont(42);
    entry_bParkingTrigImpactTransSig_20to30 = leg_bParkingTrigImpactTransSig_20to30->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerColor(4);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerStyle(3);
    entry_bParkingTrigImpactTransSig_20to30->SetMarkerSize(2);
    entry_bParkingTrigImpactTransSig_20to30->SetTextFont(42);
    leg_bParkingTrigImpactTransSig_20to30->Draw("HIST");

    Cavnas_bParkingTrigImpactTransSig_20to30->Modified();
    Cavnas_bParkingTrigImpactTransSig_20to30->cd();
    Cavnas_bParkingTrigImpactTransSig_20to30->SetSelected(Cavnas_bParkingTrigImpactTransSig_20to30);
    Cavnas_bParkingTrigImpactTransSig_20to30->SaveAs( "bParkingTrig_ImpactTransSig_20to30_eff.pdf" );

}
