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

    TH1F* h_leadingMuonPt_muTrig_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrig_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrig_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrig_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrig_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_muTrig");

    TCanvas *Cavnas_muTrig = new TCanvas("Cavnas_muTrig", "Cavnas_muTrig",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_muTrig->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_muTrig->SetFillColor(0);
    Cavnas_muTrig->SetBorderMode(0);
    Cavnas_muTrig->SetBorderSize(2);
    Cavnas_muTrig->SetLeftMargin(0.15);
    Cavnas_muTrig->SetRightMargin(0.05);
    Cavnas_muTrig->SetTopMargin(0.05);
    Cavnas_muTrig->SetBottomMargin(0.15);
    Cavnas_muTrig->SetGridy();
    Cavnas_muTrig->SetTickx(1);
    Cavnas_muTrig->SetTicky(1);
    Cavnas_muTrig->SetFrameBorderMode(0);
    Cavnas_muTrig->cd();

    h_leadingMuonPt_muTrig_ctau0->SetTitle("");
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_muTrig_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetTitle("#mu trigger efficiency");
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_muTrig_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_muTrig_ctau0->SetLineColor(9);
    h_leadingMuonPt_muTrig_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_muTrig_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_muTrig_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_muTrig_ctau0->Draw("HIST p");

    h_leadingMuonPt_muTrig_ctau1->SetLineColor(8);
    h_leadingMuonPt_muTrig_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_muTrig_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_muTrig_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_muTrig_ctau1->Draw("HIST p same");

    h_leadingMuonPt_muTrig_ctau10->SetLineColor(2);
    h_leadingMuonPt_muTrig_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_muTrig_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_muTrig_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_muTrig_ctau10->Draw("HIST p same");

    h_leadingMuonPt_muTrig_ctau100->SetLineColor(1);
    h_leadingMuonPt_muTrig_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_muTrig_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_muTrig_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_muTrig_ctau100->Draw("HIST p same");

    h_leadingMuonPt_muTrig_ctau1000->SetLineColor(4);
    h_leadingMuonPt_muTrig_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_muTrig_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_muTrig_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_muTrig_ctau1000->Draw("HIST p same");

    TLegend *leg_muTrig = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muTrig->SetBorderSize(0);
    leg_muTrig->SetTextSize(0.03);
    leg_muTrig->SetLineColor(1);
    leg_muTrig->SetLineStyle(1);
    leg_muTrig->SetLineWidth(1);
    leg_muTrig->SetFillColor(0);
    leg_muTrig->SetFillStyle(1001);
    TLegendEntry *entry_muTrig = leg_muTrig->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muTrig->SetMarkerColor(9);
    entry_muTrig->SetMarkerStyle(26);
    entry_muTrig->SetMarkerSize(2);
    entry_muTrig->SetTextFont(42);
    entry_muTrig = leg_muTrig->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muTrig->SetMarkerColor(8);
    entry_muTrig->SetMarkerStyle(4);
    entry_muTrig->SetMarkerSize(2);
    entry_muTrig->SetTextFont(42);
    entry_muTrig = leg_muTrig->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muTrig->SetMarkerColor(2);
    entry_muTrig->SetMarkerStyle(5);
    entry_muTrig->SetMarkerSize(2);
    entry_muTrig->SetTextFont(42);
    entry_muTrig = leg_muTrig->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrig->SetMarkerColor(1);
    entry_muTrig->SetMarkerStyle(2);
    entry_muTrig->SetMarkerSize(2);
    entry_muTrig->SetTextFont(42);
    entry_muTrig = leg_muTrig->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrig->SetMarkerColor(4);
    entry_muTrig->SetMarkerStyle(3);
    entry_muTrig->SetMarkerSize(2);
    entry_muTrig->SetTextFont(42);
    leg_muTrig->Draw("HIST");

    Cavnas_muTrig->Modified();
    Cavnas_muTrig->cd();
    Cavnas_muTrig->SetSelected(Cavnas_muTrig);
    Cavnas_muTrig->SaveAs( "muTrig_pT_eff.pdf" );

    //

    TH1F* h_leadingMuonPt_mumuTrig_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrig_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrig_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrig_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrig_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrig");

    TCanvas *Cavnas_mumuTrig = new TCanvas("Cavnas_mumuTrig", "Cavnas_mumuTrig",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_mumuTrig->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_mumuTrig->SetFillColor(0);
    Cavnas_mumuTrig->SetBorderMode(0);
    Cavnas_mumuTrig->SetBorderSize(2);
    Cavnas_mumuTrig->SetLeftMargin(0.15);
    Cavnas_mumuTrig->SetRightMargin(0.05);
    Cavnas_mumuTrig->SetTopMargin(0.05);
    Cavnas_mumuTrig->SetBottomMargin(0.15);
    Cavnas_mumuTrig->SetGridy();
    Cavnas_mumuTrig->SetTickx(1);
    Cavnas_mumuTrig->SetTicky(1);
    Cavnas_mumuTrig->SetFrameBorderMode(0);
    Cavnas_mumuTrig->cd();

    h_leadingMuonPt_mumuTrig_ctau0->SetTitle("");
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_mumuTrig_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_mumuTrig_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_mumuTrig_ctau0->SetLineColor(9);
    h_leadingMuonPt_mumuTrig_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_mumuTrig_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_mumuTrig_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrig_ctau0->Draw("HIST p");

    h_leadingMuonPt_mumuTrig_ctau1->SetLineColor(8);
    h_leadingMuonPt_mumuTrig_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_mumuTrig_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_mumuTrig_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrig_ctau1->Draw("HIST p same");

    h_leadingMuonPt_mumuTrig_ctau10->SetLineColor(2);
    h_leadingMuonPt_mumuTrig_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_mumuTrig_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_mumuTrig_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrig_ctau10->Draw("HIST p same");

    h_leadingMuonPt_mumuTrig_ctau100->SetLineColor(1);
    h_leadingMuonPt_mumuTrig_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_mumuTrig_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_mumuTrig_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrig_ctau100->Draw("HIST p same");

    h_leadingMuonPt_mumuTrig_ctau1000->SetLineColor(4);
    h_leadingMuonPt_mumuTrig_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_mumuTrig_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_mumuTrig_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_mumuTrig_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrig = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrig->SetBorderSize(0);
    leg_mumuTrig->SetTextSize(0.03);
    leg_mumuTrig->SetLineColor(1);
    leg_mumuTrig->SetLineStyle(1);
    leg_mumuTrig->SetLineWidth(1);
    leg_mumuTrig->SetFillColor(0);
    leg_mumuTrig->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrig = leg_mumuTrig->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrig->SetMarkerColor(9);
    entry_mumuTrig->SetMarkerStyle(26);
    entry_mumuTrig->SetMarkerSize(2);
    entry_mumuTrig->SetTextFont(42);
    entry_mumuTrig = leg_mumuTrig->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrig->SetMarkerColor(8);
    entry_mumuTrig->SetMarkerStyle(4);
    entry_mumuTrig->SetMarkerSize(2);
    entry_mumuTrig->SetTextFont(42);
    entry_mumuTrig = leg_mumuTrig->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrig->SetMarkerColor(2);
    entry_mumuTrig->SetMarkerStyle(5);
    entry_mumuTrig->SetMarkerSize(2);
    entry_mumuTrig->SetTextFont(42);
    entry_mumuTrig = leg_mumuTrig->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrig->SetMarkerColor(1);
    entry_mumuTrig->SetMarkerStyle(2);
    entry_mumuTrig->SetMarkerSize(2);
    entry_mumuTrig->SetTextFont(42);
    entry_mumuTrig = leg_mumuTrig->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrig->SetMarkerColor(4);
    entry_mumuTrig->SetMarkerStyle(3);
    entry_mumuTrig->SetMarkerSize(2);
    entry_mumuTrig->SetTextFont(42);
    leg_mumuTrig->Draw("HIST");

    Cavnas_mumuTrig->Modified();
    Cavnas_mumuTrig->cd();
    Cavnas_mumuTrig->SetSelected(Cavnas_mumuTrig);
    Cavnas_mumuTrig->SaveAs( "mumuTrig_pT_eff.pdf" );

    TH1F* h_leadingMuonPt_L2muTrig_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrig_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrig_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrig_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrig_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_L2muTrig");

    TCanvas *Cavnas_L2muTrig = new TCanvas("Cavnas_L2muTrig", "Cavnas_L2muTrig",0,0,1280,925);
    gStyle->SetOptStat(0);
    Cavnas_L2muTrig->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Cavnas_L2muTrig->SetFillColor(0);
    Cavnas_L2muTrig->SetBorderMode(0);
    Cavnas_L2muTrig->SetBorderSize(2);
    Cavnas_L2muTrig->SetLeftMargin(0.15);
    Cavnas_L2muTrig->SetRightMargin(0.05);
    Cavnas_L2muTrig->SetTopMargin(0.05);
    Cavnas_L2muTrig->SetBottomMargin(0.15);
    Cavnas_L2muTrig->SetGridy();
    Cavnas_L2muTrig->SetTickx(1);
    Cavnas_L2muTrig->SetTicky(1);
    Cavnas_L2muTrig->SetFrameBorderMode(0);
    Cavnas_L2muTrig->cd();

    h_leadingMuonPt_L2muTrig_ctau0->SetTitle("");
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonPt_L2muTrig_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetTitle("L2/no vtx #mu#mu trigger efficiency");
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonPt_L2muTrig_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonPt_L2muTrig_ctau0->SetLineColor(9);
    h_leadingMuonPt_L2muTrig_ctau0->SetMarkerColor(9);
    h_leadingMuonPt_L2muTrig_ctau0->SetMarkerStyle(26);
    h_leadingMuonPt_L2muTrig_ctau0->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrig_ctau0->Draw("HIST p");

    h_leadingMuonPt_L2muTrig_ctau1->SetLineColor(8);
    h_leadingMuonPt_L2muTrig_ctau1->SetMarkerColor(8);
    h_leadingMuonPt_L2muTrig_ctau1->SetMarkerStyle(4);
    h_leadingMuonPt_L2muTrig_ctau1->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrig_ctau1->Draw("HIST p same");

    h_leadingMuonPt_L2muTrig_ctau10->SetLineColor(2);
    h_leadingMuonPt_L2muTrig_ctau10->SetMarkerColor(2);
    h_leadingMuonPt_L2muTrig_ctau10->SetMarkerStyle(5);
    h_leadingMuonPt_L2muTrig_ctau10->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrig_ctau10->Draw("HIST p same");

    h_leadingMuonPt_L2muTrig_ctau100->SetLineColor(1);
    h_leadingMuonPt_L2muTrig_ctau100->SetMarkerColor(1);
    h_leadingMuonPt_L2muTrig_ctau100->SetMarkerStyle(2);
    h_leadingMuonPt_L2muTrig_ctau100->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrig_ctau100->Draw("HIST p same");

    h_leadingMuonPt_L2muTrig_ctau1000->SetLineColor(4);
    h_leadingMuonPt_L2muTrig_ctau1000->SetMarkerColor(4);
    h_leadingMuonPt_L2muTrig_ctau1000->SetMarkerStyle(3);
    h_leadingMuonPt_L2muTrig_ctau1000->SetMarkerSize(1);
    h_leadingMuonPt_L2muTrig_ctau1000->Draw("HIST p same");

    TLegend *leg_L2muTrig = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_L2muTrig->SetBorderSize(0);
    leg_L2muTrig->SetTextSize(0.03);
    leg_L2muTrig->SetLineColor(1);
    leg_L2muTrig->SetLineStyle(1);
    leg_L2muTrig->SetLineWidth(1);
    leg_L2muTrig->SetFillColor(0);
    leg_L2muTrig->SetFillStyle(1001);
    TLegendEntry *entry_L2muTrig = leg_L2muTrig->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_L2muTrig->SetMarkerColor(9);
    entry_L2muTrig->SetMarkerStyle(26);
    entry_L2muTrig->SetMarkerSize(2);
    entry_L2muTrig->SetTextFont(42);
    entry_L2muTrig = leg_L2muTrig->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_L2muTrig->SetMarkerColor(8);
    entry_L2muTrig->SetMarkerStyle(4);
    entry_L2muTrig->SetMarkerSize(2);
    entry_L2muTrig->SetTextFont(42);
    entry_L2muTrig = leg_L2muTrig->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_L2muTrig->SetMarkerColor(2);
    entry_L2muTrig->SetMarkerStyle(5);
    entry_L2muTrig->SetMarkerSize(2);
    entry_L2muTrig->SetTextFont(42);
    entry_L2muTrig = leg_L2muTrig->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrig->SetMarkerColor(1);
    entry_L2muTrig->SetMarkerStyle(2);
    entry_L2muTrig->SetMarkerSize(2);
    entry_L2muTrig->SetTextFont(42);
    entry_L2muTrig = leg_L2muTrig->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrig->SetMarkerColor(4);
    entry_L2muTrig->SetMarkerStyle(3);
    entry_L2muTrig->SetMarkerSize(2);
    entry_L2muTrig->SetTextFont(42);
    leg_L2muTrig->Draw("HIST");

    Cavnas_L2muTrig->Modified();
    Cavnas_L2muTrig->cd();
    Cavnas_L2muTrig->SetSelected(Cavnas_L2muTrig);
    Cavnas_L2muTrig->SaveAs( "L2muTrig_pT_eff.pdf" );

}
