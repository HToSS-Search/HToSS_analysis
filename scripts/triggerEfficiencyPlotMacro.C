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
    h_delR_muTrigDelR_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
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

    TLegend *leg_muTrigDelR = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
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
    h_delR_mumuTrigDelR_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
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

    TLegend *leg_mumuTrigDelR = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
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
    h_delR_L2muTrigDelR_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
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

    TLegend *leg_L2muTrigDelR = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
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
    h_delR_muOrMumuTrigDelR_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
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

    TLegend *leg_muOrMumuTrigDelR = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
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
    h_delR_ORTrigDelR_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
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

    TLegend *leg_ORTrigDelR = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
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

}
