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

void triggerEfficiencyPlotMacro() {

    TFile* inFile0  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau0.root");
    TFile* inFile1  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau1.root");
    TFile* inFile2  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau10.root");
    TFile* inFile3  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau100.root");
    TFile* inFile4  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau1000.root");

    // pT

    TH1F* h_leadingMuonPt_muTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_muTrig");

    TCanvas *Cavnas_muTrigPt = new TCanvas("Cavnas_muTrigPt", "Cavnas_muTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muTrigPt->SetFillColor(0);
    Cavnas_muTrigPt->SetBorderMode(0);
    Cavnas_muTrigPt->SetBorderSize(2);
    Cavnas_muTrigPt->SetLeftMargin(0.15);
    Cavnas_muTrigPt->SetRightMargin(0.05);
    Cavnas_muTrigPt->SetTopMargin(0.05);
    Cavnas_muTrigPt->SetBottomMargin(0.15);
    Cavnas_muTrigPt->SetGridy();
    Cavnas_muTrigPt->SetTickx(1);
    Cavnas_muTrigPt->SetTicky(1);
    Cavnas_muTrigPt->SetFrameBorderMode(0);
    Cavnas_muTrigPt->cd();

    h_leadingMuonPt_muTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_muTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetTitle("#mu trigger efficiency");
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_muTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_muTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_muTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_muTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_muTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_muTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_muTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_muTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_muTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_muTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_muTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_muTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_muTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_muTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_muTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_muTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_muTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_muTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_muTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_muTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_muTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muTrigPt->SetBorderSize(0);
    leg_muTrigPt->SetTextSize(0.03);
    leg_muTrigPt->SetLineColor(1);
    leg_muTrigPt->SetLineStyle(1);
    leg_muTrigPt->SetLineWidth(1);
    leg_muTrigPt->SetFillColor(0);
    leg_muTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muTrigPt->SetMarkerColor(9);
    entry_muTrigPt->SetMarkerStyle(26);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muTrigPt->SetMarkerColor(8);
    entry_muTrigPt->SetMarkerStyle(4);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muTrigPt->SetMarkerColor(2);
    entry_muTrigPt->SetMarkerStyle(5);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigPt->SetMarkerColor(1);
    entry_muTrigPt->SetMarkerStyle(2);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigPt->SetMarkerColor(4);
    entry_muTrigPt->SetMarkerStyle(3);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    leg_muTrigPt->Draw("HIST");

    Cavnas_muTrigPt->Modified();
    Cavnas_muTrigPt->cd();
    Cavnas_muTrigPt->SetSelected(Cavnas_muTrigPt);
    Cavnas_muTrigPt->SaveAs( "muTrig_pT_eff.pdf" );

    //

    TH1F* h_leadingMuonPt_mumuTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrig");

    TCanvas *Cavnas_mumuTrigPt = new TCanvas("Cavnas_mumuTrigPt", "Cavnas_mumuTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigPt->SetFillColor(0);
    Cavnas_mumuTrigPt->SetBorderMode(0);
    Cavnas_mumuTrigPt->SetBorderSize(2);
    Cavnas_mumuTrigPt->SetLeftMargin(0.15);
    Cavnas_mumuTrigPt->SetRightMargin(0.05);
    Cavnas_mumuTrigPt->SetTopMargin(0.05);
    Cavnas_mumuTrigPt->SetBottomMargin(0.15);
    Cavnas_mumuTrigPt->SetGridy();
    Cavnas_mumuTrigPt->SetTickx(1);
    Cavnas_mumuTrigPt->SetTicky(1);
    Cavnas_mumuTrigPt->SetFrameBorderMode(0);
    Cavnas_mumuTrigPt->cd();

    h_leadingMuonPt_mumuTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_mumuTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_mumuTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_mumuTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_mumuTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_mumuTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_mumuTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_mumuTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_mumuTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_mumuTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_mumuTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_mumuTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigPt->SetBorderSize(0);
    leg_mumuTrigPt->SetTextSize(0.03);
    leg_mumuTrigPt->SetLineColor(1);
    leg_mumuTrigPt->SetLineStyle(1);
    leg_mumuTrigPt->SetLineWidth(1);
    leg_mumuTrigPt->SetFillColor(0);
    leg_mumuTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(9);
    entry_mumuTrigPt->SetMarkerStyle(26);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(8);
    entry_mumuTrigPt->SetMarkerStyle(4);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(2);
    entry_mumuTrigPt->SetMarkerStyle(5);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(1);
    entry_mumuTrigPt->SetMarkerStyle(2);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(4);
    entry_mumuTrigPt->SetMarkerStyle(3);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    leg_mumuTrigPt->Draw("HIST");

    Cavnas_mumuTrigPt->Modified();
    Cavnas_mumuTrigPt->cd();
    Cavnas_mumuTrigPt->SetSelected(Cavnas_mumuTrigPt);
    Cavnas_mumuTrigPt->SaveAs( "mumuTrig_pT_eff.pdf" );

    //

    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrigMassCut");

    TCanvas *Cavnas_mumuTrigMassCutPt = new TCanvas("Cavnas_mumuTrigMassCutPt", "Cavnas_mumuTrigMassCutPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigMassCutPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigMassCutPt->SetFillColor(0);
    Cavnas_mumuTrigMassCutPt->SetBorderMode(0);
    Cavnas_mumuTrigMassCutPt->SetBorderSize(2);
    Cavnas_mumuTrigMassCutPt->SetLeftMargin(0.15);
    Cavnas_mumuTrigMassCutPt->SetRightMargin(0.05);
    Cavnas_mumuTrigMassCutPt->SetTopMargin(0.05);
    Cavnas_mumuTrigMassCutPt->SetBottomMargin(0.15);
    Cavnas_mumuTrigMassCutPt->SetGridy();
    Cavnas_mumuTrigMassCutPt->SetTickx(1);
    Cavnas_mumuTrigMassCutPt->SetTicky(1);
    Cavnas_mumuTrigMassCutPt->SetFrameBorderMode(0);
    Cavnas_mumuTrigMassCutPt->cd();

    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->SetTitle("");
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_mumuTrigMassCutPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigMassCutPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigMassCutPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigMassCutPt->SetBorderSize(0);
    leg_mumuTrigMassCutPt->SetTextSize(0.03);
    leg_mumuTrigMassCutPt->SetLineColor(1);
    leg_mumuTrigMassCutPt->SetLineStyle(1);
    leg_mumuTrigMassCutPt->SetLineWidth(1);
    leg_mumuTrigMassCutPt->SetFillColor(0);
    leg_mumuTrigMassCutPt->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(9);
    entry_mumuTrigMassCutPt->SetMarkerStyle(26);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(8);
    entry_mumuTrigMassCutPt->SetMarkerStyle(4);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(2);
    entry_mumuTrigMassCutPt->SetMarkerStyle(5);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(1);
    entry_mumuTrigMassCutPt->SetMarkerStyle(2);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(4);
    entry_mumuTrigMassCutPt->SetMarkerStyle(3);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    leg_mumuTrigMassCutPt->Draw("HIST");

    Cavnas_mumuTrigMassCutPt->Modified();
    Cavnas_mumuTrigMassCutPt->cd();
    Cavnas_mumuTrigMassCutPt->SetSelected(Cavnas_mumuTrigMassCutPt);
    Cavnas_mumuTrigMassCutPt->SaveAs( "mumuTrigMassCut_pT_eff.pdf" );

// 

    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrigNoMassCut");

    TCanvas *Cavnas_mumuTrigNoMassCutPt = new TCanvas("Cavnas_mumuTrigNoMassCutPt", "Cavnas_mumuTrigNoMassCutPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigNoMassCutPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigNoMassCutPt->SetFillColor(0);
    Cavnas_mumuTrigNoMassCutPt->SetBorderMode(0);
    Cavnas_mumuTrigNoMassCutPt->SetBorderSize(2);
    Cavnas_mumuTrigNoMassCutPt->SetLeftMargin(0.15);
    Cavnas_mumuTrigNoMassCutPt->SetRightMargin(0.05);
    Cavnas_mumuTrigNoMassCutPt->SetTopMargin(0.05);
    Cavnas_mumuTrigNoMassCutPt->SetBottomMargin(0.15);
    Cavnas_mumuTrigNoMassCutPt->SetGridy();
    Cavnas_mumuTrigNoMassCutPt->SetTickx(1);
    Cavnas_mumuTrigNoMassCutPt->SetTicky(1);
    Cavnas_mumuTrigNoMassCutPt->SetFrameBorderMode(0);
    Cavnas_mumuTrigNoMassCutPt->cd();

    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->SetTitle("");
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigNoMassCutPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigNoMassCutPt->SetBorderSize(0);
    leg_mumuTrigNoMassCutPt->SetTextSize(0.03);
    leg_mumuTrigNoMassCutPt->SetLineColor(1);
    leg_mumuTrigNoMassCutPt->SetLineStyle(1);
    leg_mumuTrigNoMassCutPt->SetLineWidth(1);
    leg_mumuTrigNoMassCutPt->SetFillColor(0);
    leg_mumuTrigNoMassCutPt->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(9);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(26);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(8);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(4);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(2);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(5);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(1);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(4);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    leg_mumuTrigNoMassCutPt->Draw("HIST");

    Cavnas_mumuTrigNoMassCutPt->Modified();
    Cavnas_mumuTrigNoMassCutPt->cd();
    Cavnas_mumuTrigNoMassCutPt->SetSelected(Cavnas_mumuTrigNoMassCutPt);
    Cavnas_mumuTrigNoMassCutPt->SaveAs( "mumuTrigNoMassCut_pT_eff.pdf" );

    //

    TH1F* h_leadingMuonPt_L2muTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_L2muTrig");

    TCanvas *Cavnas_L2muTrigPt = new TCanvas("Cavnas_L2muTrigPt", "Cavnas_L2muTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_L2muTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_L2muTrigPt->SetFillColor(0);
    Cavnas_L2muTrigPt->SetBorderMode(0);
    Cavnas_L2muTrigPt->SetBorderSize(2);
    Cavnas_L2muTrigPt->SetLeftMargin(0.15);
    Cavnas_L2muTrigPt->SetRightMargin(0.05);
    Cavnas_L2muTrigPt->SetTopMargin(0.05);
    Cavnas_L2muTrigPt->SetBottomMargin(0.15);
    Cavnas_L2muTrigPt->SetGridy();
    Cavnas_L2muTrigPt->SetTickx(1);
    Cavnas_L2muTrigPt->SetTicky(1);
    Cavnas_L2muTrigPt->SetFrameBorderMode(0);
    Cavnas_L2muTrigPt->cd();

    h_leadingMuonPt_L2muTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetTitle("L2/no vtx #mu#mu trigger efficiency");
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_L2muTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_L2muTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_L2muTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_L2muTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_L2muTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_L2muTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_L2muTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_L2muTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_L2muTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_L2muTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_L2muTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_L2muTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_L2muTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_L2muTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_L2muTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_L2muTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_L2muTrigPt->SetBorderSize(0);
    leg_L2muTrigPt->SetTextSize(0.03);
    leg_L2muTrigPt->SetLineColor(1);
    leg_L2muTrigPt->SetLineStyle(1);
    leg_L2muTrigPt->SetLineWidth(1);
    leg_L2muTrigPt->SetFillColor(0);
    leg_L2muTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(9);
    entry_L2muTrigPt->SetMarkerStyle(26);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(8);
    entry_L2muTrigPt->SetMarkerStyle(4);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(2);
    entry_L2muTrigPt->SetMarkerStyle(5);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(1);
    entry_L2muTrigPt->SetMarkerStyle(2);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(4);
    entry_L2muTrigPt->SetMarkerStyle(3);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    leg_L2muTrigPt->Draw("HIST");

    Cavnas_L2muTrigPt->Modified();
    Cavnas_L2muTrigPt->cd();
    Cavnas_L2muTrigPt->SetSelected(Cavnas_L2muTrigPt);
    Cavnas_L2muTrigPt->SaveAs( "L2muTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_muOrMumuTrig");

    TCanvas *Cavnas_muOrMumuTrigPt = new TCanvas("Cavnas_muOrMumuTrigPt", "Cavnas_muOrMumuTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muOrMumuTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muOrMumuTrigPt->SetFillColor(0);
    Cavnas_muOrMumuTrigPt->SetBorderMode(0);
    Cavnas_muOrMumuTrigPt->SetBorderSize(2);
    Cavnas_muOrMumuTrigPt->SetLeftMargin(0.15);
    Cavnas_muOrMumuTrigPt->SetRightMargin(0.05);
    Cavnas_muOrMumuTrigPt->SetTopMargin(0.05);
    Cavnas_muOrMumuTrigPt->SetBottomMargin(0.15);
    Cavnas_muOrMumuTrigPt->SetGridy();
    Cavnas_muOrMumuTrigPt->SetTickx(1);
    Cavnas_muOrMumuTrigPt->SetTicky(1);
    Cavnas_muOrMumuTrigPt->SetFrameBorderMode(0);
    Cavnas_muOrMumuTrigPt->cd();

    h_leadingMuonPt_muOrMumuTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetTitle("#mu#mu OR #mu trigger efficiency");
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_muOrMumuTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_muOrMumuTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_muOrMumuTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_muOrMumuTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_muOrMumuTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_muOrMumuTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muOrMumuTrigPt->SetBorderSize(0);
    leg_muOrMumuTrigPt->SetTextSize(0.03);
    leg_muOrMumuTrigPt->SetLineColor(1);
    leg_muOrMumuTrigPt->SetLineStyle(1);
    leg_muOrMumuTrigPt->SetLineWidth(1);
    leg_muOrMumuTrigPt->SetFillColor(0);
    leg_muOrMumuTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(9);
    entry_muOrMumuTrigPt->SetMarkerStyle(26);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(8);
    entry_muOrMumuTrigPt->SetMarkerStyle(4);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(2);
    entry_muOrMumuTrigPt->SetMarkerStyle(5);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(1);
    entry_muOrMumuTrigPt->SetMarkerStyle(2);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(4);
    entry_muOrMumuTrigPt->SetMarkerStyle(3);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    leg_muOrMumuTrigPt->Draw("HIST");

    Cavnas_muOrMumuTrigPt->Modified();
    Cavnas_muOrMumuTrigPt->cd();
    Cavnas_muOrMumuTrigPt->SetSelected(Cavnas_muOrMumuTrigPt);
    Cavnas_muOrMumuTrigPt->SaveAs( "muOrMumuTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_ORTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_ORTrig");

    TCanvas *Cavnas_ORTrigPt = new TCanvas("Cavnas_ORTrigPt", "Cavnas_ORTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_ORTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_ORTrigPt->SetFillColor(0);
    Cavnas_ORTrigPt->SetBorderMode(0);
    Cavnas_ORTrigPt->SetBorderSize(2);
    Cavnas_ORTrigPt->SetLeftMargin(0.15);
    Cavnas_ORTrigPt->SetRightMargin(0.05);
    Cavnas_ORTrigPt->SetTopMargin(0.05);
    Cavnas_ORTrigPt->SetBottomMargin(0.15);
    Cavnas_ORTrigPt->SetGridy();
    Cavnas_ORTrigPt->SetTickx(1);
    Cavnas_ORTrigPt->SetTicky(1);
    Cavnas_ORTrigPt->SetFrameBorderMode(0);
    Cavnas_ORTrigPt->cd();

    h_leadingMuonPt_ORTrigPt_ctau0->SetTitle("");
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_ORTrigPt_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetTitle("OR #mu trigger efficiency");
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_ORTrigPt_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_ORTrigPt_ctau0->SetLineColor(9);
    h_leadingMuonPt_ORTrigPt_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_ORTrigPt_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_ORTrigPt_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_ORTrigPt_ctau0->Draw("HIST p");

    h_leadingMuonPt_ORTrigPt_ctau1->SetLineColor(8);
    h_leadingMuonPt_ORTrigPt_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_ORTrigPt_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_ORTrigPt_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_ORTrigPt_ctau1->Draw("HIST p same");

    h_leadingMuonPt_ORTrigPt_ctau10->SetLineColor(2);
    h_leadingMuonPt_ORTrigPt_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_ORTrigPt_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_ORTrigPt_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_ORTrigPt_ctau10->Draw("HIST p same");

    h_leadingMuonPt_ORTrigPt_ctau100->SetLineColor(1);
    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_ORTrigPt_ctau100->Draw("HIST p same");

    h_leadingMuonPt_ORTrigPt_ctau1000->SetLineColor(4);
    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_ORTrigPt_ctau1000->Draw("HIST p same");

    TLegend *leg_ORTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_ORTrigPt->SetBorderSize(0);
    leg_ORTrigPt->SetTextSize(0.03);
    leg_ORTrigPt->SetLineColor(1);
    leg_ORTrigPt->SetLineStyle(1);
    leg_ORTrigPt->SetLineWidth(1);
    leg_ORTrigPt->SetFillColor(0);
    leg_ORTrigPt->SetFillStyle(1001);
    TLegendEntry *entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_ORTrigPt->SetMarkerColor(9);
    entry_ORTrigPt->SetMarkerStyle(26);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_ORTrigPt->SetMarkerColor(8);
    entry_ORTrigPt->SetMarkerStyle(4);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_ORTrigPt->SetMarkerColor(2);
    entry_ORTrigPt->SetMarkerStyle(5);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigPt->SetMarkerColor(1);
    entry_ORTrigPt->SetMarkerStyle(2);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigPt->SetMarkerColor(4);
    entry_ORTrigPt->SetMarkerStyle(3);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    leg_ORTrigPt->Draw("HIST");

    Cavnas_ORTrigPt->Modified();
    Cavnas_ORTrigPt->cd();
    Cavnas_ORTrigPt->SetSelected(Cavnas_ORTrigPt);
    Cavnas_ORTrigPt->SaveAs( "ORTrig_pT_eff.pdf" );

    // DeltaR

    TH1F* h_delR_muTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_muTrig");

    TCanvas *Cavnas_muTrigDelR = new TCanvas("Cavnas_muTrigDelR", "Cavnas_muTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muTrigDelR->SetFillColor(0);
    Cavnas_muTrigDelR->SetBorderMode(0);
    Cavnas_muTrigDelR->SetBorderSize(2);
    Cavnas_muTrigDelR->SetLeftMargin(0.15);
    Cavnas_muTrigDelR->SetRightMargin(0.05);
    Cavnas_muTrigDelR->SetTopMargin(0.05);
    Cavnas_muTrigDelR->SetBottomMargin(0.15);
    Cavnas_muTrigDelR->SetGridy();
    Cavnas_muTrigDelR->SetTickx(1);
    Cavnas_muTrigDelR->SetTicky(1);
    Cavnas_muTrigDelR->SetFrameBorderMode(0);
    Cavnas_muTrigDelR->cd();

    h_delR_muTrigDelR_ctau0->SetTitle("");
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetTitle("#mu trigger efficiency");
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_muTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_muTrigDelR_ctau0->SetLineColor(9);
    h_delR_muTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_muTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_muTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_muTrigDelR_ctau0->Draw("HIST p");

    h_delR_muTrigDelR_ctau1->SetLineColor(8);
    h_delR_muTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_muTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_muTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_muTrigDelR_ctau1->Draw("HIST p same");

    h_delR_muTrigDelR_ctau10->SetLineColor(2);
    h_delR_muTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_muTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_muTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_muTrigDelR_ctau10->Draw("HIST p same");

    h_delR_muTrigDelR_ctau100->SetLineColor(1);
    h_delR_muTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_muTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_muTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_muTrigDelR_ctau100->Draw("HIST p same");

    h_delR_muTrigDelR_ctau1000->SetLineColor(4);
    h_delR_muTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_muTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_muTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_muTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_muTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_muTrigDelR->SetBorderSize(0);
    leg_muTrigDelR->SetTextSize(0.03);
    leg_muTrigDelR->SetLineColor(1);
    leg_muTrigDelR->SetLineStyle(1);
    leg_muTrigDelR->SetLineWidth(1);
    leg_muTrigDelR->SetFillColor(0);
    leg_muTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muTrigDelR->SetMarkerColor(9);
    entry_muTrigDelR->SetMarkerStyle(26);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muTrigDelR->SetMarkerColor(8);
    entry_muTrigDelR->SetMarkerStyle(4);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muTrigDelR->SetMarkerColor(2);
    entry_muTrigDelR->SetMarkerStyle(5);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigDelR->SetMarkerColor(1);
    entry_muTrigDelR->SetMarkerStyle(2);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigDelR->SetMarkerColor(4);
    entry_muTrigDelR->SetMarkerStyle(3);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    leg_muTrigDelR->Draw("HIST");

    Cavnas_muTrigDelR->Modified();
    Cavnas_muTrigDelR->cd();
    Cavnas_muTrigDelR->SetSelected(Cavnas_muTrigDelR);
    Cavnas_muTrigDelR->SaveAs( "muTrig_DelR_eff.pdf" );

    //

    TH1F* h_delR_mumuTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrig");

    TCanvas *Cavnas_mumuTrigDelR = new TCanvas("Cavnas_mumuTrigDelR", "Cavnas_mumuTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigDelR->SetFillColor(0);
    Cavnas_mumuTrigDelR->SetBorderMode(0);
    Cavnas_mumuTrigDelR->SetBorderSize(2);
    Cavnas_mumuTrigDelR->SetLeftMargin(0.15);
    Cavnas_mumuTrigDelR->SetRightMargin(0.05);
    Cavnas_mumuTrigDelR->SetTopMargin(0.05);
    Cavnas_mumuTrigDelR->SetBottomMargin(0.15);
    Cavnas_mumuTrigDelR->SetGridy();
    Cavnas_mumuTrigDelR->SetTickx(1);
    Cavnas_mumuTrigDelR->SetTicky(1);
    Cavnas_mumuTrigDelR->SetFrameBorderMode(0);
    Cavnas_mumuTrigDelR->cd();

    h_delR_mumuTrigDelR_ctau0->SetTitle("");
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_mumuTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_mumuTrigDelR_ctau0->SetLineColor(9);
    h_delR_mumuTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_mumuTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_mumuTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_mumuTrigDelR_ctau0->Draw("HIST p");

    h_delR_mumuTrigDelR_ctau1->SetLineColor(8);
    h_delR_mumuTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_mumuTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_mumuTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_mumuTrigDelR_ctau1->Draw("HIST p same");

    h_delR_mumuTrigDelR_ctau10->SetLineColor(2);
    h_delR_mumuTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_mumuTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_mumuTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_mumuTrigDelR_ctau10->Draw("HIST p same");

    h_delR_mumuTrigDelR_ctau100->SetLineColor(1);
    h_delR_mumuTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_mumuTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_mumuTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_mumuTrigDelR_ctau100->Draw("HIST p same");

    h_delR_mumuTrigDelR_ctau1000->SetLineColor(4);
    h_delR_mumuTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_mumuTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_mumuTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_mumuTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigDelR->SetBorderSize(0);
    leg_mumuTrigDelR->SetTextSize(0.03);
    leg_mumuTrigDelR->SetLineColor(1);
    leg_mumuTrigDelR->SetLineStyle(1);
    leg_mumuTrigDelR->SetLineWidth(1);
    leg_mumuTrigDelR->SetFillColor(0);
    leg_mumuTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(9);
    entry_mumuTrigDelR->SetMarkerStyle(26);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(8);
    entry_mumuTrigDelR->SetMarkerStyle(4);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(2);
    entry_mumuTrigDelR->SetMarkerStyle(5);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(1);
    entry_mumuTrigDelR->SetMarkerStyle(2);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(4);
    entry_mumuTrigDelR->SetMarkerStyle(3);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    leg_mumuTrigDelR->Draw("HIST");

    Cavnas_mumuTrigDelR->Modified();
    Cavnas_mumuTrigDelR->cd();
    Cavnas_mumuTrigDelR->SetSelected(Cavnas_mumuTrigDelR);
    Cavnas_mumuTrigDelR->SaveAs( "mumuTrig_DelR_eff.pdf" );

    //

    TH1F* h_delR_mumuTrigMassCutDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrigMassCut");

    TCanvas *Cavnas_mumuTrigMassCutDelR = new TCanvas("Cavnas_mumuTrigMassCutDelR", "Cavnas_mumuTrigMassCutDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigMassCutDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigMassCutDelR->SetFillColor(0);
    Cavnas_mumuTrigMassCutDelR->SetBorderMode(0);
    Cavnas_mumuTrigMassCutDelR->SetBorderSize(2);
    Cavnas_mumuTrigMassCutDelR->SetLeftMargin(0.15);
    Cavnas_mumuTrigMassCutDelR->SetRightMargin(0.05);
    Cavnas_mumuTrigMassCutDelR->SetTopMargin(0.05);
    Cavnas_mumuTrigMassCutDelR->SetBottomMargin(0.15);
    Cavnas_mumuTrigMassCutDelR->SetGridy();
    Cavnas_mumuTrigMassCutDelR->SetTickx(1);
    Cavnas_mumuTrigMassCutDelR->SetTicky(1);
    Cavnas_mumuTrigMassCutDelR->SetFrameBorderMode(0);
    Cavnas_mumuTrigMassCutDelR->cd();

    h_delR_mumuTrigMassCutDelR_ctau0->SetTitle("");
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_mumuTrigMassCutDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_mumuTrigMassCutDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_mumuTrigMassCutDelR_ctau0->SetLineColor(9);
    h_delR_mumuTrigMassCutDelR_ctau0->SetMarkerColor(9);
    h_delR_mumuTrigMassCutDelR_ctau0->SetMarkerStyle(26);
    h_delR_mumuTrigMassCutDelR_ctau0->SetMarkerSize(1);
    h_delR_mumuTrigMassCutDelR_ctau0->Draw("HIST p");

    h_delR_mumuTrigMassCutDelR_ctau1->SetLineColor(8);
    h_delR_mumuTrigMassCutDelR_ctau1->SetMarkerColor(8);
    h_delR_mumuTrigMassCutDelR_ctau1->SetMarkerStyle(4);
    h_delR_mumuTrigMassCutDelR_ctau1->SetMarkerSize(1);
    h_delR_mumuTrigMassCutDelR_ctau1->Draw("HIST p same");

    h_delR_mumuTrigMassCutDelR_ctau10->SetLineColor(2);
    h_delR_mumuTrigMassCutDelR_ctau10->SetMarkerColor(2);
    h_delR_mumuTrigMassCutDelR_ctau10->SetMarkerStyle(5);
    h_delR_mumuTrigMassCutDelR_ctau10->SetMarkerSize(1);
    h_delR_mumuTrigMassCutDelR_ctau10->Draw("HIST p same");

    h_delR_mumuTrigMassCutDelR_ctau100->SetLineColor(1);
    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerColor(1);
    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerStyle(2);
    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerSize(1);
    h_delR_mumuTrigMassCutDelR_ctau100->Draw("HIST p same");

    h_delR_mumuTrigMassCutDelR_ctau1000->SetLineColor(4);
    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerColor(4);
    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerStyle(3);
    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerSize(1);
    h_delR_mumuTrigMassCutDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigMassCutDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigMassCutDelR->SetBorderSize(0);
    leg_mumuTrigMassCutDelR->SetTextSize(0.03);
    leg_mumuTrigMassCutDelR->SetLineColor(1);
    leg_mumuTrigMassCutDelR->SetLineStyle(1);
    leg_mumuTrigMassCutDelR->SetLineWidth(1);
    leg_mumuTrigMassCutDelR->SetFillColor(0);
    leg_mumuTrigMassCutDelR->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(9);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(26);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(8);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(4);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(2);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(5);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(1);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(2);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(4);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(3);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    leg_mumuTrigMassCutDelR->Draw("HIST");

    Cavnas_mumuTrigMassCutDelR->Modified();
    Cavnas_mumuTrigMassCutDelR->cd();
    Cavnas_mumuTrigMassCutDelR->SetSelected(Cavnas_mumuTrigMassCutDelR);
    Cavnas_mumuTrigMassCutDelR->SaveAs( "mumuTrigMassCut_DelR_eff.pdf" );

    //

    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrigNoMassCut");

    TCanvas *Cavnas_mumuTrigNoMassCutDelR = new TCanvas("Cavnas_mumuTrigNoMassCutDelR", "Cavnas_mumuTrigNoMassCutDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigNoMassCutDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigNoMassCutDelR->SetFillColor(0);
    Cavnas_mumuTrigNoMassCutDelR->SetBorderMode(0);
    Cavnas_mumuTrigNoMassCutDelR->SetBorderSize(2);
    Cavnas_mumuTrigNoMassCutDelR->SetLeftMargin(0.15);
    Cavnas_mumuTrigNoMassCutDelR->SetRightMargin(0.05);
    Cavnas_mumuTrigNoMassCutDelR->SetTopMargin(0.05);
    Cavnas_mumuTrigNoMassCutDelR->SetBottomMargin(0.15);
    Cavnas_mumuTrigNoMassCutDelR->SetGridy();
    Cavnas_mumuTrigNoMassCutDelR->SetTickx(1);
    Cavnas_mumuTrigNoMassCutDelR->SetTicky(1);
    Cavnas_mumuTrigNoMassCutDelR->SetFrameBorderMode(0);
    Cavnas_mumuTrigNoMassCutDelR->cd();

    h_delR_mumuTrigNoMassCutDelR_ctau0->SetTitle("");
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_mumuTrigNoMassCutDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_mumuTrigNoMassCutDelR_ctau0->SetLineColor(9);
    h_delR_mumuTrigNoMassCutDelR_ctau0->SetMarkerColor(9);
    h_delR_mumuTrigNoMassCutDelR_ctau0->SetMarkerStyle(26);
    h_delR_mumuTrigNoMassCutDelR_ctau0->SetMarkerSize(1);
    h_delR_mumuTrigNoMassCutDelR_ctau0->Draw("HIST p");

    h_delR_mumuTrigNoMassCutDelR_ctau1->SetLineColor(8);
    h_delR_mumuTrigNoMassCutDelR_ctau1->SetMarkerColor(8);
    h_delR_mumuTrigNoMassCutDelR_ctau1->SetMarkerStyle(4);
    h_delR_mumuTrigNoMassCutDelR_ctau1->SetMarkerSize(1);
    h_delR_mumuTrigNoMassCutDelR_ctau1->Draw("HIST p same");

    h_delR_mumuTrigNoMassCutDelR_ctau10->SetLineColor(2);
    h_delR_mumuTrigNoMassCutDelR_ctau10->SetMarkerColor(2);
    h_delR_mumuTrigNoMassCutDelR_ctau10->SetMarkerStyle(5);
    h_delR_mumuTrigNoMassCutDelR_ctau10->SetMarkerSize(1);
    h_delR_mumuTrigNoMassCutDelR_ctau10->Draw("HIST p same");

    h_delR_mumuTrigNoMassCutDelR_ctau100->SetLineColor(1);
    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerColor(1);
    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerStyle(2);
    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerSize(1);
    h_delR_mumuTrigNoMassCutDelR_ctau100->Draw("HIST p same");

    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetLineColor(4);
    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerColor(4);
    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerStyle(3);
    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerSize(1);
    h_delR_mumuTrigNoMassCutDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigNoMassCutDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigNoMassCutDelR->SetBorderSize(0);
    leg_mumuTrigNoMassCutDelR->SetTextSize(0.03);
    leg_mumuTrigNoMassCutDelR->SetLineColor(1);
    leg_mumuTrigNoMassCutDelR->SetLineStyle(1);
    leg_mumuTrigNoMassCutDelR->SetLineWidth(1);
    leg_mumuTrigNoMassCutDelR->SetFillColor(0);
    leg_mumuTrigNoMassCutDelR->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(9);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(26);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(8);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(4);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(2);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(5);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(1);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(4);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    leg_mumuTrigNoMassCutDelR->Draw("HIST");

    Cavnas_mumuTrigNoMassCutDelR->Modified();
    Cavnas_mumuTrigNoMassCutDelR->cd();
    Cavnas_mumuTrigNoMassCutDelR->SetSelected(Cavnas_mumuTrigNoMassCutDelR);
    Cavnas_mumuTrigNoMassCutDelR->SaveAs( "mumuTrigNoMassCut_DelR_eff.pdf" );

    //

    TH1F* h_delR_L2muTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_L2muTrig");

    TCanvas *Cavnas_L2muTrigDelR = new TCanvas("Cavnas_L2muTrigDelR", "Cavnas_L2muTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_L2muTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_L2muTrigDelR->SetFillColor(0);
    Cavnas_L2muTrigDelR->SetBorderMode(0);
    Cavnas_L2muTrigDelR->SetBorderSize(2);
    Cavnas_L2muTrigDelR->SetLeftMargin(0.15);
    Cavnas_L2muTrigDelR->SetRightMargin(0.05);
    Cavnas_L2muTrigDelR->SetTopMargin(0.05);
    Cavnas_L2muTrigDelR->SetBottomMargin(0.15);
    Cavnas_L2muTrigDelR->SetGridy();
    Cavnas_L2muTrigDelR->SetTickx(1);
    Cavnas_L2muTrigDelR->SetTicky(1);
    Cavnas_L2muTrigDelR->SetFrameBorderMode(0);
    Cavnas_L2muTrigDelR->cd();

    h_delR_L2muTrigDelR_ctau0->SetTitle("");
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetTitle("L2/no vtx #mu#mu trigger efficiency");
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_L2muTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_L2muTrigDelR_ctau0->SetLineColor(9);
    h_delR_L2muTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_L2muTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_L2muTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_L2muTrigDelR_ctau0->Draw("HIST p");

    h_delR_L2muTrigDelR_ctau1->SetLineColor(8);
    h_delR_L2muTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_L2muTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_L2muTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_L2muTrigDelR_ctau1->Draw("HIST p same");

    h_delR_L2muTrigDelR_ctau10->SetLineColor(2);
    h_delR_L2muTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_L2muTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_L2muTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_L2muTrigDelR_ctau10->Draw("HIST p same");

    h_delR_L2muTrigDelR_ctau100->SetLineColor(1);
    h_delR_L2muTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_L2muTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_L2muTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_L2muTrigDelR_ctau100->Draw("HIST p same");

    h_delR_L2muTrigDelR_ctau1000->SetLineColor(4);
    h_delR_L2muTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_L2muTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_L2muTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_L2muTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_L2muTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_L2muTrigDelR->SetBorderSize(0);
    leg_L2muTrigDelR->SetTextSize(0.03);
    leg_L2muTrigDelR->SetLineColor(1);
    leg_L2muTrigDelR->SetLineStyle(1);
    leg_L2muTrigDelR->SetLineWidth(1);
    leg_L2muTrigDelR->SetFillColor(0);
    leg_L2muTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(9);
    entry_L2muTrigDelR->SetMarkerStyle(26);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(8);
    entry_L2muTrigDelR->SetMarkerStyle(4);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(2);
    entry_L2muTrigDelR->SetMarkerStyle(5);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(1);
    entry_L2muTrigDelR->SetMarkerStyle(2);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(4);
    entry_L2muTrigDelR->SetMarkerStyle(3);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    leg_L2muTrigDelR->Draw("HIST");

    Cavnas_L2muTrigDelR->Modified();
    Cavnas_L2muTrigDelR->cd();
    Cavnas_L2muTrigDelR->SetSelected(Cavnas_L2muTrigDelR);
    Cavnas_L2muTrigDelR->SaveAs( "L2muTrig_DelR_eff.pdf" );

    TH1F* h_delR_muOrMumuTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_muOrMumuTrig");

    TCanvas *Cavnas_muOrMumuTrigDelR = new TCanvas("Cavnas_muOrMumuTrigDelR", "Cavnas_muOrMumuTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muOrMumuTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muOrMumuTrigDelR->SetFillColor(0);
    Cavnas_muOrMumuTrigDelR->SetBorderMode(0);
    Cavnas_muOrMumuTrigDelR->SetBorderSize(2);
    Cavnas_muOrMumuTrigDelR->SetLeftMargin(0.15);
    Cavnas_muOrMumuTrigDelR->SetRightMargin(0.05);
    Cavnas_muOrMumuTrigDelR->SetTopMargin(0.05);
    Cavnas_muOrMumuTrigDelR->SetBottomMargin(0.15);
    Cavnas_muOrMumuTrigDelR->SetGridy();
    Cavnas_muOrMumuTrigDelR->SetTickx(1);
    Cavnas_muOrMumuTrigDelR->SetTicky(1);
    Cavnas_muOrMumuTrigDelR->SetFrameBorderMode(0);
    Cavnas_muOrMumuTrigDelR->cd();

    h_delR_muOrMumuTrigDelR_ctau0->SetTitle("");
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetTitle("#mu#mu OR #mu trigger efficiency");
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_muOrMumuTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_muOrMumuTrigDelR_ctau0->SetLineColor(9);
    h_delR_muOrMumuTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_muOrMumuTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_muOrMumuTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_muOrMumuTrigDelR_ctau0->Draw("HIST p");

    h_delR_muOrMumuTrigDelR_ctau1->SetLineColor(8);
    h_delR_muOrMumuTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_muOrMumuTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_muOrMumuTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_muOrMumuTrigDelR_ctau1->Draw("HIST p same");

    h_delR_muOrMumuTrigDelR_ctau10->SetLineColor(2);
    h_delR_muOrMumuTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_muOrMumuTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_muOrMumuTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_muOrMumuTrigDelR_ctau10->Draw("HIST p same");

    h_delR_muOrMumuTrigDelR_ctau100->SetLineColor(1);
    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_muOrMumuTrigDelR_ctau100->Draw("HIST p same");

    h_delR_muOrMumuTrigDelR_ctau1000->SetLineColor(4);
    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_muOrMumuTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_muOrMumuTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_muOrMumuTrigDelR->SetBorderSize(0);
    leg_muOrMumuTrigDelR->SetTextSize(0.03);
    leg_muOrMumuTrigDelR->SetLineColor(1);
    leg_muOrMumuTrigDelR->SetLineStyle(1);
    leg_muOrMumuTrigDelR->SetLineWidth(1);
    leg_muOrMumuTrigDelR->SetFillColor(0);
    leg_muOrMumuTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(9);
    entry_muOrMumuTrigDelR->SetMarkerStyle(26);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(8);
    entry_muOrMumuTrigDelR->SetMarkerStyle(4);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(2);
    entry_muOrMumuTrigDelR->SetMarkerStyle(5);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(1);
    entry_muOrMumuTrigDelR->SetMarkerStyle(2);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(4);
    entry_muOrMumuTrigDelR->SetMarkerStyle(3);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    leg_muOrMumuTrigDelR->Draw("HIST");

    Cavnas_muOrMumuTrigDelR->Modified();
    Cavnas_muOrMumuTrigDelR->cd();
    Cavnas_muOrMumuTrigDelR->SetSelected(Cavnas_muOrMumuTrigDelR);
    Cavnas_muOrMumuTrigDelR->SaveAs( "muOrMumuTrig_DelR_eff.pdf" );

    TH1F* h_delR_ORTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_ORTrig");

    TCanvas *Cavnas_ORTrigDelR = new TCanvas("Cavnas_ORTrigDelR", "Cavnas_ORTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_ORTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_ORTrigDelR->SetFillColor(0);
    Cavnas_ORTrigDelR->SetBorderMode(0);
    Cavnas_ORTrigDelR->SetBorderSize(2);
    Cavnas_ORTrigDelR->SetLeftMargin(0.15);
    Cavnas_ORTrigDelR->SetRightMargin(0.05);
    Cavnas_ORTrigDelR->SetTopMargin(0.05);
    Cavnas_ORTrigDelR->SetBottomMargin(0.15);
    Cavnas_ORTrigDelR->SetGridy();
    Cavnas_ORTrigDelR->SetTickx(1);
    Cavnas_ORTrigDelR->SetTicky(1);
    Cavnas_ORTrigDelR->SetFrameBorderMode(0);
    Cavnas_ORTrigDelR->cd();

    h_delR_ORTrigDelR_ctau0->SetTitle("");
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetLabelFont(42);
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetTitleFont(42);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetTitle("OR #mu trigger efficiency");
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetLabelFont(42);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetTitleOffset(0);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetTitleFont(42);
    h_delR_ORTrigDelR_ctau0->GetYaxis()->SetNdivisions(510);
    h_delR_ORTrigDelR_ctau0->SetLineColor(9);
    h_delR_ORTrigDelR_ctau0->SetMarkerColor(9);
    h_delR_ORTrigDelR_ctau0->SetMarkerStyle(26);
    h_delR_ORTrigDelR_ctau0->SetMarkerSize(1);
    h_delR_ORTrigDelR_ctau0->Draw("HIST p");

    h_delR_ORTrigDelR_ctau1->SetLineColor(8);
    h_delR_ORTrigDelR_ctau1->SetMarkerColor(8);
    h_delR_ORTrigDelR_ctau1->SetMarkerStyle(4);
    h_delR_ORTrigDelR_ctau1->SetMarkerSize(1);
    h_delR_ORTrigDelR_ctau1->Draw("HIST p same");

    h_delR_ORTrigDelR_ctau10->SetLineColor(2);
    h_delR_ORTrigDelR_ctau10->SetMarkerColor(2);
    h_delR_ORTrigDelR_ctau10->SetMarkerStyle(5);
    h_delR_ORTrigDelR_ctau10->SetMarkerSize(1);
    h_delR_ORTrigDelR_ctau10->Draw("HIST p same");

    h_delR_ORTrigDelR_ctau100->SetLineColor(1);
    h_delR_ORTrigDelR_ctau100->SetMarkerColor(1);
    h_delR_ORTrigDelR_ctau100->SetMarkerStyle(2);
    h_delR_ORTrigDelR_ctau100->SetMarkerSize(1);
    h_delR_ORTrigDelR_ctau100->Draw("HIST p same");

    h_delR_ORTrigDelR_ctau1000->SetLineColor(4);
    h_delR_ORTrigDelR_ctau1000->SetMarkerColor(4);
    h_delR_ORTrigDelR_ctau1000->SetMarkerStyle(3);
    h_delR_ORTrigDelR_ctau1000->SetMarkerSize(1);
    h_delR_ORTrigDelR_ctau1000->Draw("HIST p same");

    TLegend *leg_ORTrigDelR = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_ORTrigDelR->SetBorderSize(0);
    leg_ORTrigDelR->SetTextSize(0.03);
    leg_ORTrigDelR->SetLineColor(1);
    leg_ORTrigDelR->SetLineStyle(1);
    leg_ORTrigDelR->SetLineWidth(1);
    leg_ORTrigDelR->SetFillColor(0);
    leg_ORTrigDelR->SetFillStyle(1001);
    TLegendEntry *entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(9);
    entry_ORTrigDelR->SetMarkerStyle(26);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(8);
    entry_ORTrigDelR->SetMarkerStyle(4);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(2);
    entry_ORTrigDelR->SetMarkerStyle(5);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(1);
    entry_ORTrigDelR->SetMarkerStyle(2);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(4);
    entry_ORTrigDelR->SetMarkerStyle(3);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    leg_ORTrigDelR->Draw("HIST");

    Cavnas_ORTrigDelR->Modified();
    Cavnas_ORTrigDelR->cd();
    Cavnas_ORTrigDelR->SetSelected(Cavnas_ORTrigDelR);
    Cavnas_ORTrigDelR->SaveAs( "ORTrig_DelR_eff.pdf" );

///// mass

    // mass

    TH1F* h_mass_muTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_muTrig");

    TCanvas *Cavnas_muTrigMass = new TCanvas("Cavnas_muTrigMass", "Cavnas_muTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muTrigMass->SetFillColor(0);
    Cavnas_muTrigMass->SetBorderMode(0);
    Cavnas_muTrigMass->SetBorderSize(2);
    Cavnas_muTrigMass->SetLeftMargin(0.15);
    Cavnas_muTrigMass->SetRightMargin(0.05);
    Cavnas_muTrigMass->SetTopMargin(0.05);
    Cavnas_muTrigMass->SetBottomMargin(0.15);
    Cavnas_muTrigMass->SetGridy();
    Cavnas_muTrigMass->SetTickx(1);
    Cavnas_muTrigMass->SetTicky(1);
    Cavnas_muTrigMass->SetFrameBorderMode(0);
    Cavnas_muTrigMass->cd();

    h_mass_muTrigMass_ctau0->SetTitle("");
    h_mass_muTrigMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_muTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_muTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_muTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_muTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_muTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetTitle("#mu trigger efficiency");
    h_mass_muTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_muTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_muTrigMass_ctau0->SetLineColor(9);
    h_mass_muTrigMass_ctau0->SetMarkerColor(9);
    h_mass_muTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_muTrigMass_ctau0->SetMarkerSize(1);
    h_mass_muTrigMass_ctau0->Draw("HIST p");

    h_mass_muTrigMass_ctau1->SetLineColor(8);
    h_mass_muTrigMass_ctau1->SetMarkerColor(8);
    h_mass_muTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_muTrigMass_ctau1->SetMarkerSize(1);
    h_mass_muTrigMass_ctau1->Draw("HIST p same");

    h_mass_muTrigMass_ctau10->SetLineColor(2);
    h_mass_muTrigMass_ctau10->SetMarkerColor(2);
    h_mass_muTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_muTrigMass_ctau10->SetMarkerSize(1);
    h_mass_muTrigMass_ctau10->Draw("HIST p same");

    h_mass_muTrigMass_ctau100->SetLineColor(1);
    h_mass_muTrigMass_ctau100->SetMarkerColor(1);
    h_mass_muTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_muTrigMass_ctau100->SetMarkerSize(1);
    h_mass_muTrigMass_ctau100->Draw("HIST p same");

    h_mass_muTrigMass_ctau1000->SetLineColor(4);
    h_mass_muTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_muTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_muTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_muTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_muTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_muTrigMass->SetBorderSize(0);
    leg_muTrigMass->SetTextSize(0.03);
    leg_muTrigMass->SetLineColor(1);
    leg_muTrigMass->SetLineStyle(1);
    leg_muTrigMass->SetLineWidth(1);
    leg_muTrigMass->SetFillColor(0);
    leg_muTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muTrigMass->SetMarkerColor(9);
    entry_muTrigMass->SetMarkerStyle(26);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muTrigMass->SetMarkerColor(8);
    entry_muTrigMass->SetMarkerStyle(4);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muTrigMass->SetMarkerColor(2);
    entry_muTrigMass->SetMarkerStyle(5);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigMass->SetMarkerColor(1);
    entry_muTrigMass->SetMarkerStyle(2);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigMass->SetMarkerColor(4);
    entry_muTrigMass->SetMarkerStyle(3);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    leg_muTrigMass->Draw("HIST");

    Cavnas_muTrigMass->Modified();
    Cavnas_muTrigMass->cd();
    Cavnas_muTrigMass->SetSelected(Cavnas_muTrigMass);
    Cavnas_muTrigMass->SaveAs( "muTrig_mass_eff.pdf" );

    //

    TH1F* h_mass_mumuTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrig");

    TCanvas *Cavnas_mumuTrigMass = new TCanvas("Cavnas_mumuTrigMass", "Cavnas_mumuTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigMass->SetFillColor(0);
    Cavnas_mumuTrigMass->SetBorderMode(0);
    Cavnas_mumuTrigMass->SetBorderSize(2);
    Cavnas_mumuTrigMass->SetLeftMargin(0.15);
    Cavnas_mumuTrigMass->SetRightMargin(0.05);
    Cavnas_mumuTrigMass->SetTopMargin(0.05);
    Cavnas_mumuTrigMass->SetBottomMargin(0.15);
    Cavnas_mumuTrigMass->SetGridy();
    Cavnas_mumuTrigMass->SetTickx(1);
    Cavnas_mumuTrigMass->SetTicky(1);
    Cavnas_mumuTrigMass->SetFrameBorderMode(0);
    Cavnas_mumuTrigMass->cd();

    h_mass_mumuTrigMass_ctau0->SetTitle("");
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_mumuTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_mumuTrigMass_ctau0->SetLineColor(9);
    h_mass_mumuTrigMass_ctau0->SetMarkerColor(9);
    h_mass_mumuTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_mumuTrigMass_ctau0->SetMarkerSize(1);
    h_mass_mumuTrigMass_ctau0->Draw("HIST p");

    h_mass_mumuTrigMass_ctau1->SetLineColor(8);
    h_mass_mumuTrigMass_ctau1->SetMarkerColor(8);
    h_mass_mumuTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_mumuTrigMass_ctau1->SetMarkerSize(1);
    h_mass_mumuTrigMass_ctau1->Draw("HIST p same");

    h_mass_mumuTrigMass_ctau10->SetLineColor(2);
    h_mass_mumuTrigMass_ctau10->SetMarkerColor(2);
    h_mass_mumuTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_mumuTrigMass_ctau10->SetMarkerSize(1);
    h_mass_mumuTrigMass_ctau10->Draw("HIST p same");

    h_mass_mumuTrigMass_ctau100->SetLineColor(1);
    h_mass_mumuTrigMass_ctau100->SetMarkerColor(1);
    h_mass_mumuTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_mumuTrigMass_ctau100->SetMarkerSize(1);
    h_mass_mumuTrigMass_ctau100->Draw("HIST p same");

    h_mass_mumuTrigMass_ctau1000->SetLineColor(4);
    h_mass_mumuTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_mumuTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_mumuTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_mumuTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigMass->SetBorderSize(0);
    leg_mumuTrigMass->SetTextSize(0.03);
    leg_mumuTrigMass->SetLineColor(1);
    leg_mumuTrigMass->SetLineStyle(1);
    leg_mumuTrigMass->SetLineWidth(1);
    leg_mumuTrigMass->SetFillColor(0);
    leg_mumuTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(9);
    entry_mumuTrigMass->SetMarkerStyle(26);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(8);
    entry_mumuTrigMass->SetMarkerStyle(4);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(2);
    entry_mumuTrigMass->SetMarkerStyle(5);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(1);
    entry_mumuTrigMass->SetMarkerStyle(2);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(4);
    entry_mumuTrigMass->SetMarkerStyle(3);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    leg_mumuTrigMass->Draw("HIST");

    Cavnas_mumuTrigMass->Modified();
    Cavnas_mumuTrigMass->cd();
    Cavnas_mumuTrigMass->SetSelected(Cavnas_mumuTrigMass);
    Cavnas_mumuTrigMass->SaveAs( "mumuTrig_mass_eff.pdf" );

    //

    TH1F* h_mass_mumuTrigMassCutMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrigMassCut");

    TCanvas *Cavnas_mumuTrigMassCutMass = new TCanvas("Cavnas_mumuTrigMassCutMass", "Cavnas_mumuTrigMassCutMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigMassCutMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigMassCutMass->SetFillColor(0);
    Cavnas_mumuTrigMassCutMass->SetBorderMode(0);
    Cavnas_mumuTrigMassCutMass->SetBorderSize(2);
    Cavnas_mumuTrigMassCutMass->SetLeftMargin(0.15);
    Cavnas_mumuTrigMassCutMass->SetRightMargin(0.05);
    Cavnas_mumuTrigMassCutMass->SetTopMargin(0.05);
    Cavnas_mumuTrigMassCutMass->SetBottomMargin(0.15);
    Cavnas_mumuTrigMassCutMass->SetGridy();
    Cavnas_mumuTrigMassCutMass->SetTickx(1);
    Cavnas_mumuTrigMassCutMass->SetTicky(1);
    Cavnas_mumuTrigMassCutMass->SetFrameBorderMode(0);
    Cavnas_mumuTrigMassCutMass->cd();

    h_mass_mumuTrigMassCutMass_ctau0->SetTitle("");
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_mumuTrigMassCutMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_mumuTrigMassCutMass_ctau0->SetLineColor(9);
    h_mass_mumuTrigMassCutMass_ctau0->SetMarkerColor(9);
    h_mass_mumuTrigMassCutMass_ctau0->SetMarkerStyle(26);
    h_mass_mumuTrigMassCutMass_ctau0->SetMarkerSize(1);
    h_mass_mumuTrigMassCutMass_ctau0->Draw("HIST p");

    h_mass_mumuTrigMassCutMass_ctau1->SetLineColor(8);
    h_mass_mumuTrigMassCutMass_ctau1->SetMarkerColor(8);
    h_mass_mumuTrigMassCutMass_ctau1->SetMarkerStyle(4);
    h_mass_mumuTrigMassCutMass_ctau1->SetMarkerSize(1);
    h_mass_mumuTrigMassCutMass_ctau1->Draw("HIST p same");

    h_mass_mumuTrigMassCutMass_ctau10->SetLineColor(2);
    h_mass_mumuTrigMassCutMass_ctau10->SetMarkerColor(2);
    h_mass_mumuTrigMassCutMass_ctau10->SetMarkerStyle(5);
    h_mass_mumuTrigMassCutMass_ctau10->SetMarkerSize(1);
    h_mass_mumuTrigMassCutMass_ctau10->Draw("HIST p same");

    h_mass_mumuTrigMassCutMass_ctau100->SetLineColor(1);
    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerColor(1);
    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerStyle(2);
    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerSize(1);
    h_mass_mumuTrigMassCutMass_ctau100->Draw("HIST p same");

    h_mass_mumuTrigMassCutMass_ctau1000->SetLineColor(4);
    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerColor(4);
    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerStyle(3);
    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerSize(1);
    h_mass_mumuTrigMassCutMass_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigMassCutMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigMassCutMass->SetBorderSize(0);
    leg_mumuTrigMassCutMass->SetTextSize(0.03);
    leg_mumuTrigMassCutMass->SetLineColor(1);
    leg_mumuTrigMassCutMass->SetLineStyle(1);
    leg_mumuTrigMassCutMass->SetLineWidth(1);
    leg_mumuTrigMassCutMass->SetFillColor(0);
    leg_mumuTrigMassCutMass->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(9);
    entry_mumuTrigMassCutMass->SetMarkerStyle(26);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(8);
    entry_mumuTrigMassCutMass->SetMarkerStyle(4);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(2);
    entry_mumuTrigMassCutMass->SetMarkerStyle(5);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(1);
    entry_mumuTrigMassCutMass->SetMarkerStyle(2);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(4);
    entry_mumuTrigMassCutMass->SetMarkerStyle(3);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    leg_mumuTrigMassCutMass->Draw("HIST");

    Cavnas_mumuTrigMassCutMass->Modified();
    Cavnas_mumuTrigMassCutMass->cd();
    Cavnas_mumuTrigMassCutMass->SetSelected(Cavnas_mumuTrigMassCutMass);
    Cavnas_mumuTrigMassCutMass->SaveAs( "mumuTrigMassCut_mass_eff.pdf" );

    //

    TH1F* h_mass_mumuTrigNoMassCutMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrigNoMassCut");

    TCanvas *Cavnas_mumuTrigNoMassCutMass = new TCanvas("Cavnas_mumuTrigNoMassCutMass", "Cavnas_mumuTrigNoMassCutMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrigNoMassCutMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrigNoMassCutMass->SetFillColor(0);
    Cavnas_mumuTrigNoMassCutMass->SetBorderMode(0);
    Cavnas_mumuTrigNoMassCutMass->SetBorderSize(2);
    Cavnas_mumuTrigNoMassCutMass->SetLeftMargin(0.15);
    Cavnas_mumuTrigNoMassCutMass->SetRightMargin(0.05);
    Cavnas_mumuTrigNoMassCutMass->SetTopMargin(0.05);
    Cavnas_mumuTrigNoMassCutMass->SetBottomMargin(0.15);
    Cavnas_mumuTrigNoMassCutMass->SetGridy();
    Cavnas_mumuTrigNoMassCutMass->SetTickx(1);
    Cavnas_mumuTrigNoMassCutMass->SetTicky(1);
    Cavnas_mumuTrigNoMassCutMass->SetFrameBorderMode(0);
    Cavnas_mumuTrigNoMassCutMass->cd();

    h_mass_mumuTrigNoMassCutMass_ctau0->SetTitle("");
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_mumuTrigNoMassCutMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_mumuTrigNoMassCutMass_ctau0->SetLineColor(9);
    h_mass_mumuTrigNoMassCutMass_ctau0->SetMarkerColor(9);
    h_mass_mumuTrigNoMassCutMass_ctau0->SetMarkerStyle(26);
    h_mass_mumuTrigNoMassCutMass_ctau0->SetMarkerSize(1);
    h_mass_mumuTrigNoMassCutMass_ctau0->Draw("HIST p");

    h_mass_mumuTrigNoMassCutMass_ctau1->SetLineColor(8);
    h_mass_mumuTrigNoMassCutMass_ctau1->SetMarkerColor(8);
    h_mass_mumuTrigNoMassCutMass_ctau1->SetMarkerStyle(4);
    h_mass_mumuTrigNoMassCutMass_ctau1->SetMarkerSize(1);
    h_mass_mumuTrigNoMassCutMass_ctau1->Draw("HIST p same");

    h_mass_mumuTrigNoMassCutMass_ctau10->SetLineColor(2);
    h_mass_mumuTrigNoMassCutMass_ctau10->SetMarkerColor(2);
    h_mass_mumuTrigNoMassCutMass_ctau10->SetMarkerStyle(5);
    h_mass_mumuTrigNoMassCutMass_ctau10->SetMarkerSize(1);
    h_mass_mumuTrigNoMassCutMass_ctau10->Draw("HIST p same");

    h_mass_mumuTrigNoMassCutMass_ctau100->SetLineColor(1);
    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerColor(1);
    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerStyle(2);
    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerSize(1);
    h_mass_mumuTrigNoMassCutMass_ctau100->Draw("HIST p same");

    h_mass_mumuTrigNoMassCutMass_ctau1000->SetLineColor(4);
    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerColor(4);
    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerStyle(3);
    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerSize(1);
    h_mass_mumuTrigNoMassCutMass_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigNoMassCutMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_mumuTrigNoMassCutMass->SetBorderSize(0);
    leg_mumuTrigNoMassCutMass->SetTextSize(0.03);
    leg_mumuTrigNoMassCutMass->SetLineColor(1);
    leg_mumuTrigNoMassCutMass->SetLineStyle(1);
    leg_mumuTrigNoMassCutMass->SetLineWidth(1);
    leg_mumuTrigNoMassCutMass->SetFillColor(0);
    leg_mumuTrigNoMassCutMass->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(9);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(26);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(8);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(4);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(2);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(5);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(1);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(4);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    leg_mumuTrigNoMassCutMass->Draw("HIST");

    Cavnas_mumuTrigNoMassCutMass->Modified();
    Cavnas_mumuTrigNoMassCutMass->cd();
    Cavnas_mumuTrigNoMassCutMass->SetSelected(Cavnas_mumuTrigNoMassCutMass);
    Cavnas_mumuTrigNoMassCutMass->SaveAs( "mumuTrigNoMassCut_mass_eff.pdf" );

    //

    TH1F* h_mass_L2muTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_L2muTrig");

    TCanvas *Cavnas_L2muTrigMass = new TCanvas("Cavnas_L2muTrigMass", "Cavnas_L2muTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_L2muTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_L2muTrigMass->SetFillColor(0);
    Cavnas_L2muTrigMass->SetBorderMode(0);
    Cavnas_L2muTrigMass->SetBorderSize(2);
    Cavnas_L2muTrigMass->SetLeftMargin(0.15);
    Cavnas_L2muTrigMass->SetRightMargin(0.05);
    Cavnas_L2muTrigMass->SetTopMargin(0.05);
    Cavnas_L2muTrigMass->SetBottomMargin(0.15);
    Cavnas_L2muTrigMass->SetGridy();
    Cavnas_L2muTrigMass->SetTickx(1);
    Cavnas_L2muTrigMass->SetTicky(1);
    Cavnas_L2muTrigMass->SetFrameBorderMode(0);
    Cavnas_L2muTrigMass->cd();

    h_mass_L2muTrigMass_ctau0->SetTitle("");
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetTitle("L2/no vtx #mu#mu trigger efficiency");
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_L2muTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_L2muTrigMass_ctau0->SetLineColor(9);
    h_mass_L2muTrigMass_ctau0->SetMarkerColor(9);
    h_mass_L2muTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_L2muTrigMass_ctau0->SetMarkerSize(1);
    h_mass_L2muTrigMass_ctau0->Draw("HIST p");

    h_mass_L2muTrigMass_ctau1->SetLineColor(8);
    h_mass_L2muTrigMass_ctau1->SetMarkerColor(8);
    h_mass_L2muTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_L2muTrigMass_ctau1->SetMarkerSize(1);
    h_mass_L2muTrigMass_ctau1->Draw("HIST p same");

    h_mass_L2muTrigMass_ctau10->SetLineColor(2);
    h_mass_L2muTrigMass_ctau10->SetMarkerColor(2);
    h_mass_L2muTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_L2muTrigMass_ctau10->SetMarkerSize(1);
    h_mass_L2muTrigMass_ctau10->Draw("HIST p same");

    h_mass_L2muTrigMass_ctau100->SetLineColor(1);
    h_mass_L2muTrigMass_ctau100->SetMarkerColor(1);
    h_mass_L2muTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_L2muTrigMass_ctau100->SetMarkerSize(1);
    h_mass_L2muTrigMass_ctau100->Draw("HIST p same");

    h_mass_L2muTrigMass_ctau1000->SetLineColor(4);
    h_mass_L2muTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_L2muTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_L2muTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_L2muTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_L2muTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_L2muTrigMass->SetBorderSize(0);
    leg_L2muTrigMass->SetTextSize(0.03);
    leg_L2muTrigMass->SetLineColor(1);
    leg_L2muTrigMass->SetLineStyle(1);
    leg_L2muTrigMass->SetLineWidth(1);
    leg_L2muTrigMass->SetFillColor(0);
    leg_L2muTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(9);
    entry_L2muTrigMass->SetMarkerStyle(26);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(8);
    entry_L2muTrigMass->SetMarkerStyle(4);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(2);
    entry_L2muTrigMass->SetMarkerStyle(5);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(1);
    entry_L2muTrigMass->SetMarkerStyle(2);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(4);
    entry_L2muTrigMass->SetMarkerStyle(3);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    leg_L2muTrigMass->Draw("HIST");

    Cavnas_L2muTrigMass->Modified();
    Cavnas_L2muTrigMass->cd();
    Cavnas_L2muTrigMass->SetSelected(Cavnas_L2muTrigMass);
    Cavnas_L2muTrigMass->SaveAs( "L2muTrig_mass_eff.pdf" );

    TH1F* h_mass_muOrMumuTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_muOrMumuTrig");

    TCanvas *Cavnas_muOrMumuTrigMass = new TCanvas("Cavnas_muOrMumuTrigMass", "Cavnas_muOrMumuTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muOrMumuTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muOrMumuTrigMass->SetFillColor(0);
    Cavnas_muOrMumuTrigMass->SetBorderMode(0);
    Cavnas_muOrMumuTrigMass->SetBorderSize(2);
    Cavnas_muOrMumuTrigMass->SetLeftMargin(0.15);
    Cavnas_muOrMumuTrigMass->SetRightMargin(0.05);
    Cavnas_muOrMumuTrigMass->SetTopMargin(0.05);
    Cavnas_muOrMumuTrigMass->SetBottomMargin(0.15);
    Cavnas_muOrMumuTrigMass->SetGridy();
    Cavnas_muOrMumuTrigMass->SetTickx(1);
    Cavnas_muOrMumuTrigMass->SetTicky(1);
    Cavnas_muOrMumuTrigMass->SetFrameBorderMode(0);
    Cavnas_muOrMumuTrigMass->cd();

    h_mass_muOrMumuTrigMass_ctau0->SetTitle("");
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetTitle("#mu#mu OR #mu trigger efficiency");
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_muOrMumuTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_muOrMumuTrigMass_ctau0->SetLineColor(9);
    h_mass_muOrMumuTrigMass_ctau0->SetMarkerColor(9);
    h_mass_muOrMumuTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_muOrMumuTrigMass_ctau0->SetMarkerSize(1);
    h_mass_muOrMumuTrigMass_ctau0->Draw("HIST p");

    h_mass_muOrMumuTrigMass_ctau1->SetLineColor(8);
    h_mass_muOrMumuTrigMass_ctau1->SetMarkerColor(8);
    h_mass_muOrMumuTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_muOrMumuTrigMass_ctau1->SetMarkerSize(1);
    h_mass_muOrMumuTrigMass_ctau1->Draw("HIST p same");

    h_mass_muOrMumuTrigMass_ctau10->SetLineColor(2);
    h_mass_muOrMumuTrigMass_ctau10->SetMarkerColor(2);
    h_mass_muOrMumuTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_muOrMumuTrigMass_ctau10->SetMarkerSize(1);
    h_mass_muOrMumuTrigMass_ctau10->Draw("HIST p same");

    h_mass_muOrMumuTrigMass_ctau100->SetLineColor(1);
    h_mass_muOrMumuTrigMass_ctau100->SetMarkerColor(1);
    h_mass_muOrMumuTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_muOrMumuTrigMass_ctau100->SetMarkerSize(1);
    h_mass_muOrMumuTrigMass_ctau100->Draw("HIST p same");

    h_mass_muOrMumuTrigMass_ctau1000->SetLineColor(4);
    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_muOrMumuTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_muOrMumuTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_muOrMumuTrigMass->SetBorderSize(0);
    leg_muOrMumuTrigMass->SetTextSize(0.03);
    leg_muOrMumuTrigMass->SetLineColor(1);
    leg_muOrMumuTrigMass->SetLineStyle(1);
    leg_muOrMumuTrigMass->SetLineWidth(1);
    leg_muOrMumuTrigMass->SetFillColor(0);
    leg_muOrMumuTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(9);
    entry_muOrMumuTrigMass->SetMarkerStyle(26);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(8);
    entry_muOrMumuTrigMass->SetMarkerStyle(4);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(2);
    entry_muOrMumuTrigMass->SetMarkerStyle(5);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(1);
    entry_muOrMumuTrigMass->SetMarkerStyle(2);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(4);
    entry_muOrMumuTrigMass->SetMarkerStyle(3);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    leg_muOrMumuTrigMass->Draw("HIST");

    Cavnas_muOrMumuTrigMass->Modified();
    Cavnas_muOrMumuTrigMass->cd();
    Cavnas_muOrMumuTrigMass->SetSelected(Cavnas_muOrMumuTrigMass);
    Cavnas_muOrMumuTrigMass->SaveAs( "muOrMumuTrig_mass_eff.pdf" );

    TH1F* h_mass_ORTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_ORTrig");

    TCanvas *Cavnas_ORTrigMass = new TCanvas("Cavnas_ORTrigMass", "Cavnas_ORTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_ORTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_ORTrigMass->SetFillColor(0);
    Cavnas_ORTrigMass->SetBorderMode(0);
    Cavnas_ORTrigMass->SetBorderSize(2);
    Cavnas_ORTrigMass->SetLeftMargin(0.15);
    Cavnas_ORTrigMass->SetRightMargin(0.05);
    Cavnas_ORTrigMass->SetTopMargin(0.05);
    Cavnas_ORTrigMass->SetBottomMargin(0.15);
    Cavnas_ORTrigMass->SetGridy();
    Cavnas_ORTrigMass->SetTickx(1);
    Cavnas_ORTrigMass->SetTicky(1);
    Cavnas_ORTrigMass->SetFrameBorderMode(0);
    Cavnas_ORTrigMass->cd();

    h_mass_ORTrigMass_ctau0->SetTitle("");
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetTitle("#DeltaR (#mu#mu)");
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetLabelFont(42);
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetTitleFont(42);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetTitle("OR #mu trigger efficiency");
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetLabelFont(42);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetTitleOffset(0);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetTitleFont(42);
    h_mass_ORTrigMass_ctau0->GetYaxis()->SetNdivisions(510);
    h_mass_ORTrigMass_ctau0->SetLineColor(9);
    h_mass_ORTrigMass_ctau0->SetMarkerColor(9);
    h_mass_ORTrigMass_ctau0->SetMarkerStyle(26);
    h_mass_ORTrigMass_ctau0->SetMarkerSize(1);
    h_mass_ORTrigMass_ctau0->Draw("HIST p");

    h_mass_ORTrigMass_ctau1->SetLineColor(8);
    h_mass_ORTrigMass_ctau1->SetMarkerColor(8);
    h_mass_ORTrigMass_ctau1->SetMarkerStyle(4);
    h_mass_ORTrigMass_ctau1->SetMarkerSize(1);
    h_mass_ORTrigMass_ctau1->Draw("HIST p same");

    h_mass_ORTrigMass_ctau10->SetLineColor(2);
    h_mass_ORTrigMass_ctau10->SetMarkerColor(2);
    h_mass_ORTrigMass_ctau10->SetMarkerStyle(5);
    h_mass_ORTrigMass_ctau10->SetMarkerSize(1);
    h_mass_ORTrigMass_ctau10->Draw("HIST p same");

    h_mass_ORTrigMass_ctau100->SetLineColor(1);
    h_mass_ORTrigMass_ctau100->SetMarkerColor(1);
    h_mass_ORTrigMass_ctau100->SetMarkerStyle(2);
    h_mass_ORTrigMass_ctau100->SetMarkerSize(1);
    h_mass_ORTrigMass_ctau100->Draw("HIST p same");

    h_mass_ORTrigMass_ctau1000->SetLineColor(4);
    h_mass_ORTrigMass_ctau1000->SetMarkerColor(4);
    h_mass_ORTrigMass_ctau1000->SetMarkerStyle(3);
    h_mass_ORTrigMass_ctau1000->SetMarkerSize(1);
    h_mass_ORTrigMass_ctau1000->Draw("HIST p same");

    TLegend *leg_ORTrigMass = new TLegend(0.80,0.70,0.87,0.90,NULL,"brNDC");
    leg_ORTrigMass->SetBorderSize(0);
    leg_ORTrigMass->SetTextSize(0.03);
    leg_ORTrigMass->SetLineColor(1);
    leg_ORTrigMass->SetLineStyle(1);
    leg_ORTrigMass->SetLineWidth(1);
    leg_ORTrigMass->SetFillColor(0);
    leg_ORTrigMass->SetFillStyle(1001);
    TLegendEntry *entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_ORTrigMass->SetMarkerColor(9);
    entry_ORTrigMass->SetMarkerStyle(26);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_ORTrigMass->SetMarkerColor(8);
    entry_ORTrigMass->SetMarkerStyle(4);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_ORTrigMass->SetMarkerColor(2);
    entry_ORTrigMass->SetMarkerStyle(5);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigMass->SetMarkerColor(1);
    entry_ORTrigMass->SetMarkerStyle(2);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigMass->SetMarkerColor(4);
    entry_ORTrigMass->SetMarkerStyle(3);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    leg_ORTrigMass->Draw("HIST");

    Cavnas_ORTrigMass->Modified();
    Cavnas_ORTrigMass->cd();
    Cavnas_ORTrigMass->SetSelected(Cavnas_ORTrigMass);
    Cavnas_ORTrigMass->SaveAs( "ORTrig_mass_eff.pdf" );

}
