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
//    TFile* inFile3  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau100.root");
//    TFile* inFile4  =  new TFile ("plots/triggerDistributions/output_HtoSS_MS2_ctau1000.root");

    // pT

    TH1F* h_leadingMuonPt_muTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_muTrig");
    TH1F* h_leadingMuonPt_muTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_muTrig");
//    TH1F* h_leadingMuonPt_muTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_muTrig");
//    TH1F* h_leadingMuonPt_muTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_muTrig");

    TCanvas *Canvas_muTrigPt = new TCanvas("Canvas_muTrigPt", "Canvas_muTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muTrigPt->SetFillColor(0);
    Canvas_muTrigPt->SetBorderMode(0);
    Canvas_muTrigPt->SetBorderSize(2);
    Canvas_muTrigPt->SetLeftMargin(0.15);
    Canvas_muTrigPt->SetRightMargin(0.05);
    Canvas_muTrigPt->SetTopMargin(0.05);
    Canvas_muTrigPt->SetBottomMargin(0.15);
    Canvas_muTrigPt->SetGridy();
    Canvas_muTrigPt->SetTickx(1);
    Canvas_muTrigPt->SetTicky(1);
    Canvas_muTrigPt->SetFrameBorderMode(0);
    Canvas_muTrigPt->cd();

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

//    h_leadingMuonPt_muTrigPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_muTrigPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_muTrigPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_muTrigPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_muTrigPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_muTrigPt_ctau1000->Draw("HIST p same");

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
//    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigPt->SetMarkerColor(1);
    entry_muTrigPt->SetMarkerStyle(2);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
//    entry_muTrigPt = leg_muTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigPt->SetMarkerColor(4);
    entry_muTrigPt->SetMarkerStyle(3);
    entry_muTrigPt->SetMarkerSize(2);
    entry_muTrigPt->SetTextFont(42);
    leg_muTrigPt->Draw("HIST");

    Canvas_muTrigPt->Modified();
    Canvas_muTrigPt->cd();
    Canvas_muTrigPt->SetSelected(Canvas_muTrigPt);
    Canvas_muTrigPt->SaveAs( "muTrig_pT_eff.pdf" );
    Canvas_muTrigPt->SaveAs( "muTrig_pT_eff.png" );

    //

    TH1F* h_leadingMuonPt_mumuTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrig");
    TH1F* h_leadingMuonPt_mumuTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrig");
//    TH1F* h_leadingMuonPt_mumuTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrig");
//    TH1F* h_leadingMuonPt_mumuTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrig");

    TCanvas *Canvas_mumuTrigPt = new TCanvas("Canvas_mumuTrigPt", "Canvas_mumuTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigPt->SetFillColor(0);
    Canvas_mumuTrigPt->SetBorderMode(0);
    Canvas_mumuTrigPt->SetBorderSize(2);
    Canvas_mumuTrigPt->SetLeftMargin(0.15);
    Canvas_mumuTrigPt->SetRightMargin(0.05);
    Canvas_mumuTrigPt->SetTopMargin(0.05);
    Canvas_mumuTrigPt->SetBottomMargin(0.15);
    Canvas_mumuTrigPt->SetGridy();
    Canvas_mumuTrigPt->SetTickx(1);
    Canvas_mumuTrigPt->SetTicky(1);
    Canvas_mumuTrigPt->SetFrameBorderMode(0);
    Canvas_mumuTrigPt->cd();

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

//    h_leadingMuonPt_mumuTrigPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_mumuTrigPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_mumuTrigPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_mumuTrigPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigPt_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(1);
    entry_mumuTrigPt->SetMarkerStyle(2);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
//    entry_mumuTrigPt = leg_mumuTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigPt->SetMarkerColor(4);
    entry_mumuTrigPt->SetMarkerStyle(3);
    entry_mumuTrigPt->SetMarkerSize(2);
    entry_mumuTrigPt->SetTextFont(42);
    leg_mumuTrigPt->Draw("HIST");

    Canvas_mumuTrigPt->Modified();
    Canvas_mumuTrigPt->cd();
    Canvas_mumuTrigPt->SetSelected(Canvas_mumuTrigPt);
    Canvas_mumuTrigPt->SaveAs( "mumuTrig_pT_eff.pdf" );
    Canvas_mumuTrigPt->SaveAs( "mumuTrig_pT_eff.png" );

    //

    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrigMassCut");
    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrigMassCut");
//    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrigMassCut");
//    TH1F* h_leadingMuonPt_mumuTrigMassCutPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrigMassCut");

    TCanvas *Canvas_mumuTrigMassCutPt = new TCanvas("Canvas_mumuTrigMassCutPt", "Canvas_mumuTrigMassCutPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigMassCutPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigMassCutPt->SetFillColor(0);
    Canvas_mumuTrigMassCutPt->SetBorderMode(0);
    Canvas_mumuTrigMassCutPt->SetBorderSize(2);
    Canvas_mumuTrigMassCutPt->SetLeftMargin(0.15);
    Canvas_mumuTrigMassCutPt->SetRightMargin(0.05);
    Canvas_mumuTrigMassCutPt->SetTopMargin(0.05);
    Canvas_mumuTrigMassCutPt->SetBottomMargin(0.15);
    Canvas_mumuTrigMassCutPt->SetGridy();
    Canvas_mumuTrigMassCutPt->SetTickx(1);
    Canvas_mumuTrigMassCutPt->SetTicky(1);
    Canvas_mumuTrigMassCutPt->SetFrameBorderMode(0);
    Canvas_mumuTrigMassCutPt->cd();

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

//    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigMassCutPt_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(1);
    entry_mumuTrigMassCutPt->SetMarkerStyle(2);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
//    entry_mumuTrigMassCutPt = leg_mumuTrigMassCutPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutPt->SetMarkerColor(4);
    entry_mumuTrigMassCutPt->SetMarkerStyle(3);
    entry_mumuTrigMassCutPt->SetMarkerSize(2);
    entry_mumuTrigMassCutPt->SetTextFont(42);
    leg_mumuTrigMassCutPt->Draw("HIST");

    Canvas_mumuTrigMassCutPt->Modified();
    Canvas_mumuTrigMassCutPt->cd();
    Canvas_mumuTrigMassCutPt->SetSelected(Canvas_mumuTrigMassCutPt);
    Canvas_mumuTrigMassCutPt->SaveAs( "mumuTrigMassCut_pT_eff.pdf" );
    Canvas_mumuTrigMassCutPt->SaveAs( "mumuTrigMassCut_pT_eff.png" );

// 

    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_mumuTrigNoMassCut");
    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_mumuTrigNoMassCut");
//    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_mumuTrigNoMassCut");
//    TH1F* h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_mumuTrigNoMassCut");

    TCanvas *Canvas_mumuTrigNoMassCutPt = new TCanvas("Canvas_mumuTrigNoMassCutPt", "Canvas_mumuTrigNoMassCutPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigNoMassCutPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigNoMassCutPt->SetFillColor(0);
    Canvas_mumuTrigNoMassCutPt->SetBorderMode(0);
    Canvas_mumuTrigNoMassCutPt->SetBorderSize(2);
    Canvas_mumuTrigNoMassCutPt->SetLeftMargin(0.15);
    Canvas_mumuTrigNoMassCutPt->SetRightMargin(0.05);
    Canvas_mumuTrigNoMassCutPt->SetTopMargin(0.05);
    Canvas_mumuTrigNoMassCutPt->SetBottomMargin(0.15);
    Canvas_mumuTrigNoMassCutPt->SetGridy();
    Canvas_mumuTrigNoMassCutPt->SetTickx(1);
    Canvas_mumuTrigNoMassCutPt->SetTicky(1);
    Canvas_mumuTrigNoMassCutPt->SetFrameBorderMode(0);
    Canvas_mumuTrigNoMassCutPt->cd();

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

//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_mumuTrigNoMassCutPt_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(1);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
//    entry_mumuTrigNoMassCutPt = leg_mumuTrigNoMassCutPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutPt->SetMarkerColor(4);
    entry_mumuTrigNoMassCutPt->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutPt->SetMarkerSize(2);
    entry_mumuTrigNoMassCutPt->SetTextFont(42);
    leg_mumuTrigNoMassCutPt->Draw("HIST");

    Canvas_mumuTrigNoMassCutPt->Modified();
    Canvas_mumuTrigNoMassCutPt->cd();
    Canvas_mumuTrigNoMassCutPt->SetSelected(Canvas_mumuTrigNoMassCutPt);
    Canvas_mumuTrigNoMassCutPt->SaveAs( "mumuTrigNoMassCut_pT_eff.pdf" );
    Canvas_mumuTrigNoMassCutPt->SaveAs( "mumuTrigNoMassCut_pT_eff.png" );

    //

    TH1F* h_leadingMuonPt_L2muTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_L2muTrig");
    TH1F* h_leadingMuonPt_L2muTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_L2muTrig");
//    TH1F* h_leadingMuonPt_L2muTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_L2muTrig");
//    TH1F* h_leadingMuonPt_L2muTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_L2muTrig");

    TCanvas *Canvas_L2muTrigPt = new TCanvas("Canvas_L2muTrigPt", "Canvas_L2muTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_L2muTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_L2muTrigPt->SetFillColor(0);
    Canvas_L2muTrigPt->SetBorderMode(0);
    Canvas_L2muTrigPt->SetBorderSize(2);
    Canvas_L2muTrigPt->SetLeftMargin(0.15);
    Canvas_L2muTrigPt->SetRightMargin(0.05);
    Canvas_L2muTrigPt->SetTopMargin(0.05);
    Canvas_L2muTrigPt->SetBottomMargin(0.15);
    Canvas_L2muTrigPt->SetGridy();
    Canvas_L2muTrigPt->SetTickx(1);
    Canvas_L2muTrigPt->SetTicky(1);
    Canvas_L2muTrigPt->SetFrameBorderMode(0);
    Canvas_L2muTrigPt->cd();

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

//    h_leadingMuonPt_L2muTrigPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_L2muTrigPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_L2muTrigPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_L2muTrigPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_L2muTrigPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_L2muTrigPt_ctau1000->Draw("HIST p same");

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
//    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(1);
    entry_L2muTrigPt->SetMarkerStyle(2);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
//    entry_L2muTrigPt = leg_L2muTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigPt->SetMarkerColor(4);
    entry_L2muTrigPt->SetMarkerStyle(3);
    entry_L2muTrigPt->SetMarkerSize(2);
    entry_L2muTrigPt->SetTextFont(42);
    leg_L2muTrigPt->Draw("HIST");

    Canvas_L2muTrigPt->Modified();
    Canvas_L2muTrigPt->cd();
    Canvas_L2muTrigPt->SetSelected(Canvas_L2muTrigPt);
    Canvas_L2muTrigPt->SaveAs( "L2muTrig_pT_eff.pdf" );
    Canvas_L2muTrigPt->SaveAs( "L2muTrig_pT_eff.png" );

    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_muOrMumuTrig");
    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_muOrMumuTrig");
//    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_muOrMumuTrig");
//    TH1F* h_leadingMuonPt_muOrMumuTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_muOrMumuTrig");

    TCanvas *Canvas_muOrMumuTrigPt = new TCanvas("Canvas_muOrMumuTrigPt", "Canvas_muOrMumuTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muOrMumuTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muOrMumuTrigPt->SetFillColor(0);
    Canvas_muOrMumuTrigPt->SetBorderMode(0);
    Canvas_muOrMumuTrigPt->SetBorderSize(2);
    Canvas_muOrMumuTrigPt->SetLeftMargin(0.15);
    Canvas_muOrMumuTrigPt->SetRightMargin(0.05);
    Canvas_muOrMumuTrigPt->SetTopMargin(0.05);
    Canvas_muOrMumuTrigPt->SetBottomMargin(0.15);
    Canvas_muOrMumuTrigPt->SetGridy();
    Canvas_muOrMumuTrigPt->SetTickx(1);
    Canvas_muOrMumuTrigPt->SetTicky(1);
    Canvas_muOrMumuTrigPt->SetFrameBorderMode(0);
    Canvas_muOrMumuTrigPt->cd();

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

//    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_muOrMumuTrigPt_ctau1000->Draw("HIST p same");

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
//    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(1);
    entry_muOrMumuTrigPt->SetMarkerStyle(2);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
//    entry_muOrMumuTrigPt = leg_muOrMumuTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigPt->SetMarkerColor(4);
    entry_muOrMumuTrigPt->SetMarkerStyle(3);
    entry_muOrMumuTrigPt->SetMarkerSize(2);
    entry_muOrMumuTrigPt->SetTextFont(42);
    leg_muOrMumuTrigPt->Draw("HIST");

    Canvas_muOrMumuTrigPt->Modified();
    Canvas_muOrMumuTrigPt->cd();
    Canvas_muOrMumuTrigPt->SetSelected(Canvas_muOrMumuTrigPt);
    Canvas_muOrMumuTrigPt->SaveAs( "muOrMumuTrig_pT_eff.pdf" );
    Canvas_muOrMumuTrigPt->SaveAs( "muOrMumuTrig_pT_eff.png" );

    TH1F* h_leadingMuonPt_ORTrigPt_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonPt_ORTrig");
    TH1F* h_leadingMuonPt_ORTrigPt_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonPt_ORTrig");
//    TH1F* h_leadingMuonPt_ORTrigPt_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonPt_ORTrig");
//    TH1F* h_leadingMuonPt_ORTrigPt_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonPt_ORTrig");

    TCanvas *Canvas_ORTrigPt = new TCanvas("Canvas_ORTrigPt", "Canvas_ORTrigPt",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_ORTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_ORTrigPt->SetFillColor(0);
    Canvas_ORTrigPt->SetBorderMode(0);
    Canvas_ORTrigPt->SetBorderSize(2);
    Canvas_ORTrigPt->SetLeftMargin(0.15);
    Canvas_ORTrigPt->SetRightMargin(0.05);
    Canvas_ORTrigPt->SetTopMargin(0.05);
    Canvas_ORTrigPt->SetBottomMargin(0.15);
    Canvas_ORTrigPt->SetGridy();
    Canvas_ORTrigPt->SetTickx(1);
    Canvas_ORTrigPt->SetTicky(1);
    Canvas_ORTrigPt->SetFrameBorderMode(0);
    Canvas_ORTrigPt->cd();

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

//    h_leadingMuonPt_ORTrigPt_ctau100->SetLineColor(1);
//    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerColor(1);
//    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerStyle(2);
//    h_leadingMuonPt_ORTrigPt_ctau100->SetMarkerSize(1);
//    h_leadingMuonPt_ORTrigPt_ctau100->Draw("HIST p same");

//    h_leadingMuonPt_ORTrigPt_ctau1000->SetLineColor(4);
//    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerColor(4);
//    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonPt_ORTrigPt_ctau1000->SetMarkerSize(1);
//    h_leadingMuonPt_ORTrigPt_ctau1000->Draw("HIST p same");

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
//    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigPt->SetMarkerColor(1);
    entry_ORTrigPt->SetMarkerStyle(2);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
//    entry_ORTrigPt = leg_ORTrigPt->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigPt->SetMarkerColor(4);
    entry_ORTrigPt->SetMarkerStyle(3);
    entry_ORTrigPt->SetMarkerSize(2);
    entry_ORTrigPt->SetTextFont(42);
    leg_ORTrigPt->Draw("HIST");

    Canvas_ORTrigPt->Modified();
    Canvas_ORTrigPt->cd();
    Canvas_ORTrigPt->SetSelected(Canvas_ORTrigPt);
    Canvas_ORTrigPt->SaveAs( "ORTrig_pT_eff.pdf" );
    Canvas_ORTrigPt->SaveAs( "ORTrig_pT_eff.png" );

    // eta

    TH1F* h_leadingMuonEta_muTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_muTrig");
    TH1F* h_leadingMuonEta_muTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_muTrig");
    TH1F* h_leadingMuonEta_muTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_muTrig");
//    TH1F* h_leadingMuonEta_muTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_muTrig");
//    TH1F* h_leadingMuonEta_muTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_muTrig");

    TCanvas *Canvas_muTrigEta = new TCanvas("Canvas_muTrigEta", "Canvas_muTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muTrigEta->SetFillColor(0);
    Canvas_muTrigEta->SetBorderMode(0);
    Canvas_muTrigEta->SetBorderSize(2);
    Canvas_muTrigEta->SetLeftMargin(0.15);
    Canvas_muTrigEta->SetRightMargin(0.05);
    Canvas_muTrigEta->SetTopMargin(0.05);
    Canvas_muTrigEta->SetBottomMargin(0.15);
    Canvas_muTrigEta->SetGridy();
    Canvas_muTrigEta->SetTickx(1);
    Canvas_muTrigEta->SetTicky(1);
    Canvas_muTrigEta->SetFrameBorderMode(0);
    Canvas_muTrigEta->cd();

    h_leadingMuonEta_muTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_muTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetTitle("#mu trigger efficiency");
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_muTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_muTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_muTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_muTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_muTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_muTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_muTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_muTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_muTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_muTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_muTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_muTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_muTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_muTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_muTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_muTrigEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_muTrigEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_muTrigEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_muTrigEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_muTrigEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_muTrigEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_muTrigEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_muTrigEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_muTrigEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_muTrigEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_muTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_muTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muTrigEta->SetBorderSize(0);
    leg_muTrigEta->SetTextSize(0.03);
    leg_muTrigEta->SetLineColor(1);
    leg_muTrigEta->SetLineStyle(1);
    leg_muTrigEta->SetLineWidth(1);
    leg_muTrigEta->SetFillColor(0);
    leg_muTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_muTrigEta = leg_muTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muTrigEta->SetMarkerColor(9);
    entry_muTrigEta->SetMarkerStyle(26);
    entry_muTrigEta->SetMarkerSize(2);
    entry_muTrigEta->SetTextFont(42);
    entry_muTrigEta = leg_muTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muTrigEta->SetMarkerColor(8);
    entry_muTrigEta->SetMarkerStyle(4);
    entry_muTrigEta->SetMarkerSize(2);
    entry_muTrigEta->SetTextFont(42);
    entry_muTrigEta = leg_muTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muTrigEta->SetMarkerColor(2);
    entry_muTrigEta->SetMarkerStyle(5);
    entry_muTrigEta->SetMarkerSize(2);
    entry_muTrigEta->SetTextFont(42);
//    entry_muTrigEta = leg_muTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigEta->SetMarkerColor(1);
    entry_muTrigEta->SetMarkerStyle(2);
    entry_muTrigEta->SetMarkerSize(2);
    entry_muTrigEta->SetTextFont(42);
//    entry_muTrigEta = leg_muTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigEta->SetMarkerColor(4);
    entry_muTrigEta->SetMarkerStyle(3);
    entry_muTrigEta->SetMarkerSize(2);
    entry_muTrigEta->SetTextFont(42);
    leg_muTrigEta->Draw("HIST");

    Canvas_muTrigEta->Modified();
    Canvas_muTrigEta->cd();
    Canvas_muTrigEta->SetSelected(Canvas_muTrigEta);
    Canvas_muTrigEta->SaveAs( "muTrig_eta_eff.pdf" );
    Canvas_muTrigEta->SaveAs( "muTrig_eta_eff.png" );

    //

    TH1F* h_leadingMuonEta_mumuTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_mumuTrig");
    TH1F* h_leadingMuonEta_mumuTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_mumuTrig");
    TH1F* h_leadingMuonEta_mumuTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_mumuTrig");
//    TH1F* h_leadingMuonEta_mumuTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_mumuTrig");
//    TH1F* h_leadingMuonEta_mumuTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_mumuTrig");

    TCanvas *Canvas_mumuTrigEta = new TCanvas("Canvas_mumuTrigEta", "Canvas_mumuTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigEta->SetFillColor(0);
    Canvas_mumuTrigEta->SetBorderMode(0);
    Canvas_mumuTrigEta->SetBorderSize(2);
    Canvas_mumuTrigEta->SetLeftMargin(0.15);
    Canvas_mumuTrigEta->SetRightMargin(0.05);
    Canvas_mumuTrigEta->SetTopMargin(0.05);
    Canvas_mumuTrigEta->SetBottomMargin(0.15);
    Canvas_mumuTrigEta->SetGridy();
    Canvas_mumuTrigEta->SetTickx(1);
    Canvas_mumuTrigEta->SetTicky(1);
    Canvas_mumuTrigEta->SetFrameBorderMode(0);
    Canvas_mumuTrigEta->cd();

    h_leadingMuonEta_mumuTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_mumuTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_mumuTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_mumuTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_mumuTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_mumuTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_mumuTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_mumuTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_mumuTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_mumuTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_mumuTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_mumuTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_mumuTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_mumuTrigEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_mumuTrigEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_mumuTrigEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_mumuTrigEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_mumuTrigEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_mumuTrigEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigEta->SetBorderSize(0);
    leg_mumuTrigEta->SetTextSize(0.03);
    leg_mumuTrigEta->SetLineColor(1);
    leg_mumuTrigEta->SetLineStyle(1);
    leg_mumuTrigEta->SetLineWidth(1);
    leg_mumuTrigEta->SetFillColor(0);
    leg_mumuTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigEta = leg_mumuTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigEta->SetMarkerColor(9);
    entry_mumuTrigEta->SetMarkerStyle(26);
    entry_mumuTrigEta->SetMarkerSize(2);
    entry_mumuTrigEta->SetTextFont(42);
    entry_mumuTrigEta = leg_mumuTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigEta->SetMarkerColor(8);
    entry_mumuTrigEta->SetMarkerStyle(4);
    entry_mumuTrigEta->SetMarkerSize(2);
    entry_mumuTrigEta->SetTextFont(42);
    entry_mumuTrigEta = leg_mumuTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigEta->SetMarkerColor(2);
    entry_mumuTrigEta->SetMarkerStyle(5);
    entry_mumuTrigEta->SetMarkerSize(2);
    entry_mumuTrigEta->SetTextFont(42);
//    entry_mumuTrigEta = leg_mumuTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigEta->SetMarkerColor(1);
    entry_mumuTrigEta->SetMarkerStyle(2);
    entry_mumuTrigEta->SetMarkerSize(2);
    entry_mumuTrigEta->SetTextFont(42);
//    entry_mumuTrigEta = leg_mumuTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigEta->SetMarkerColor(4);
    entry_mumuTrigEta->SetMarkerStyle(3);
    entry_mumuTrigEta->SetMarkerSize(2);
    entry_mumuTrigEta->SetTextFont(42);
    leg_mumuTrigEta->Draw("HIST");

    Canvas_mumuTrigEta->Modified();
    Canvas_mumuTrigEta->cd();
    Canvas_mumuTrigEta->SetSelected(Canvas_mumuTrigEta);
    Canvas_mumuTrigEta->SaveAs( "mumuTrig_eta_eff.pdf" );
    Canvas_mumuTrigEta->SaveAs( "mumuTrig_eta_eff.png" );

    //

    TH1F* h_leadingMuonEta_mumuTrigMassCutEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_mumuTrigMassCut");
    TH1F* h_leadingMuonEta_mumuTrigMassCutEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_mumuTrigMassCut");
    TH1F* h_leadingMuonEta_mumuTrigMassCutEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_mumuTrigMassCut");
//    TH1F* h_leadingMuonEta_mumuTrigMassCutEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_mumuTrigMassCut");
//    TH1F* h_leadingMuonEta_mumuTrigMassCutEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_mumuTrigMassCut");

    TCanvas *Canvas_mumuTrigMassCutEta = new TCanvas("Canvas_mumuTrigMassCutEta", "Canvas_mumuTrigMassCutEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigMassCutEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigMassCutEta->SetFillColor(0);
    Canvas_mumuTrigMassCutEta->SetBorderMode(0);
    Canvas_mumuTrigMassCutEta->SetBorderSize(2);
    Canvas_mumuTrigMassCutEta->SetLeftMargin(0.15);
    Canvas_mumuTrigMassCutEta->SetRightMargin(0.05);
    Canvas_mumuTrigMassCutEta->SetTopMargin(0.05);
    Canvas_mumuTrigMassCutEta->SetBottomMargin(0.15);
    Canvas_mumuTrigMassCutEta->SetGridy();
    Canvas_mumuTrigMassCutEta->SetTickx(1);
    Canvas_mumuTrigMassCutEta->SetTicky(1);
    Canvas_mumuTrigMassCutEta->SetFrameBorderMode(0);
    Canvas_mumuTrigMassCutEta->cd();

    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->SetTitle("");
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_mumuTrigMassCutEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_mumuTrigMassCutEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigMassCutEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigMassCutEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigMassCutEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigMassCutEta_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigMassCutEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigMassCutEta->SetBorderSize(0);
    leg_mumuTrigMassCutEta->SetTextSize(0.03);
    leg_mumuTrigMassCutEta->SetLineColor(1);
    leg_mumuTrigMassCutEta->SetLineStyle(1);
    leg_mumuTrigMassCutEta->SetLineWidth(1);
    leg_mumuTrigMassCutEta->SetFillColor(0);
    leg_mumuTrigMassCutEta->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigMassCutEta = leg_mumuTrigMassCutEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigMassCutEta->SetMarkerColor(9);
    entry_mumuTrigMassCutEta->SetMarkerStyle(26);
    entry_mumuTrigMassCutEta->SetMarkerSize(2);
    entry_mumuTrigMassCutEta->SetTextFont(42);
    entry_mumuTrigMassCutEta = leg_mumuTrigMassCutEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigMassCutEta->SetMarkerColor(8);
    entry_mumuTrigMassCutEta->SetMarkerStyle(4);
    entry_mumuTrigMassCutEta->SetMarkerSize(2);
    entry_mumuTrigMassCutEta->SetTextFont(42);
    entry_mumuTrigMassCutEta = leg_mumuTrigMassCutEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigMassCutEta->SetMarkerColor(2);
    entry_mumuTrigMassCutEta->SetMarkerStyle(5);
    entry_mumuTrigMassCutEta->SetMarkerSize(2);
    entry_mumuTrigMassCutEta->SetTextFont(42);
//    entry_mumuTrigMassCutEta = leg_mumuTrigMassCutEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutEta->SetMarkerColor(1);
    entry_mumuTrigMassCutEta->SetMarkerStyle(2);
    entry_mumuTrigMassCutEta->SetMarkerSize(2);
    entry_mumuTrigMassCutEta->SetTextFont(42);
//    entry_mumuTrigMassCutEta = leg_mumuTrigMassCutEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutEta->SetMarkerColor(4);
    entry_mumuTrigMassCutEta->SetMarkerStyle(3);
    entry_mumuTrigMassCutEta->SetMarkerSize(2);
    entry_mumuTrigMassCutEta->SetTextFont(42);
    leg_mumuTrigMassCutEta->Draw("HIST");

    Canvas_mumuTrigMassCutEta->Modified();
    Canvas_mumuTrigMassCutEta->cd();
    Canvas_mumuTrigMassCutEta->SetSelected(Canvas_mumuTrigMassCutEta);
    Canvas_mumuTrigMassCutEta->SaveAs( "mumuTrigMassCut_Eta_eff.pdf" );
    Canvas_mumuTrigMassCutEta->SaveAs( "mumuTrigMassCut_Eta_eff.png" );

// 

    TH1F* h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_mumuTrigNoMassCut");
    TH1F* h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_mumuTrigNoMassCut");
    TH1F* h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_mumuTrigNoMassCut");
//    TH1F* h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_mumuTrigNoMassCut");
//    TH1F* h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_mumuTrigNoMassCut");

    TCanvas *Canvas_mumuTrigNoMassCutEta = new TCanvas("Canvas_mumuTrigNoMassCutEta", "Canvas_mumuTrigNoMassCutEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigNoMassCutEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigNoMassCutEta->SetFillColor(0);
    Canvas_mumuTrigNoMassCutEta->SetBorderMode(0);
    Canvas_mumuTrigNoMassCutEta->SetBorderSize(2);
    Canvas_mumuTrigNoMassCutEta->SetLeftMargin(0.15);
    Canvas_mumuTrigNoMassCutEta->SetRightMargin(0.05);
    Canvas_mumuTrigNoMassCutEta->SetTopMargin(0.05);
    Canvas_mumuTrigNoMassCutEta->SetBottomMargin(0.15);
    Canvas_mumuTrigNoMassCutEta->SetGridy();
    Canvas_mumuTrigNoMassCutEta->SetTickx(1);
    Canvas_mumuTrigNoMassCutEta->SetTicky(1);
    Canvas_mumuTrigNoMassCutEta->SetFrameBorderMode(0);
    Canvas_mumuTrigNoMassCutEta->cd();

    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->SetTitle("");
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetTitle("#mu#mu trigger efficiency");
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_mumuTrigNoMassCutEta_ctau1000->Draw("HIST p same");

    TLegend *leg_mumuTrigNoMassCutEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_mumuTrigNoMassCutEta->SetBorderSize(0);
    leg_mumuTrigNoMassCutEta->SetTextSize(0.03);
    leg_mumuTrigNoMassCutEta->SetLineColor(1);
    leg_mumuTrigNoMassCutEta->SetLineStyle(1);
    leg_mumuTrigNoMassCutEta->SetLineWidth(1);
    leg_mumuTrigNoMassCutEta->SetFillColor(0);
    leg_mumuTrigNoMassCutEta->SetFillStyle(1001);
    TLegendEntry *entry_mumuTrigNoMassCutEta = leg_mumuTrigNoMassCutEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_mumuTrigNoMassCutEta->SetMarkerColor(9);
    entry_mumuTrigNoMassCutEta->SetMarkerStyle(26);
    entry_mumuTrigNoMassCutEta->SetMarkerSize(2);
    entry_mumuTrigNoMassCutEta->SetTextFont(42);
    entry_mumuTrigNoMassCutEta = leg_mumuTrigNoMassCutEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_mumuTrigNoMassCutEta->SetMarkerColor(8);
    entry_mumuTrigNoMassCutEta->SetMarkerStyle(4);
    entry_mumuTrigNoMassCutEta->SetMarkerSize(2);
    entry_mumuTrigNoMassCutEta->SetTextFont(42);
    entry_mumuTrigNoMassCutEta = leg_mumuTrigNoMassCutEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_mumuTrigNoMassCutEta->SetMarkerColor(2);
    entry_mumuTrigNoMassCutEta->SetMarkerStyle(5);
    entry_mumuTrigNoMassCutEta->SetMarkerSize(2);
    entry_mumuTrigNoMassCutEta->SetTextFont(42);
//    entry_mumuTrigNoMassCutEta = leg_mumuTrigNoMassCutEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutEta->SetMarkerColor(1);
    entry_mumuTrigNoMassCutEta->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutEta->SetMarkerSize(2);
    entry_mumuTrigNoMassCutEta->SetTextFont(42);
//    entry_mumuTrigNoMassCutEta = leg_mumuTrigNoMassCutEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutEta->SetMarkerColor(4);
    entry_mumuTrigNoMassCutEta->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutEta->SetMarkerSize(2);
    entry_mumuTrigNoMassCutEta->SetTextFont(42);
    leg_mumuTrigNoMassCutEta->Draw("HIST");

    Canvas_mumuTrigNoMassCutEta->Modified();
    Canvas_mumuTrigNoMassCutEta->cd();
    Canvas_mumuTrigNoMassCutEta->SetSelected(Canvas_mumuTrigNoMassCutEta);
    Canvas_mumuTrigNoMassCutEta->SaveAs( "mumuTrigNoMassCut_Eta_eff.pdf" );
    Canvas_mumuTrigNoMassCutEta->SaveAs( "mumuTrigNoMassCut_Eta_eff.png" );

    //

    TH1F* h_leadingMuonEta_L2muTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_L2muTrig");
    TH1F* h_leadingMuonEta_L2muTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_L2muTrig");
    TH1F* h_leadingMuonEta_L2muTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_L2muTrig");
//    TH1F* h_leadingMuonEta_L2muTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_L2muTrig");
//    TH1F* h_leadingMuonEta_L2muTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_L2muTrig");

    TCanvas *Canvas_L2muTrigEta = new TCanvas("Canvas_L2muTrigEta", "Canvas_L2muTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_L2muTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_L2muTrigEta->SetFillColor(0);
    Canvas_L2muTrigEta->SetBorderMode(0);
    Canvas_L2muTrigEta->SetBorderSize(2);
    Canvas_L2muTrigEta->SetLeftMargin(0.15);
    Canvas_L2muTrigEta->SetRightMargin(0.05);
    Canvas_L2muTrigEta->SetTopMargin(0.05);
    Canvas_L2muTrigEta->SetBottomMargin(0.15);
    Canvas_L2muTrigEta->SetGridy();
    Canvas_L2muTrigEta->SetTickx(1);
    Canvas_L2muTrigEta->SetTicky(1);
    Canvas_L2muTrigEta->SetFrameBorderMode(0);
    Canvas_L2muTrigEta->cd();

    h_leadingMuonEta_L2muTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetTitle("L2/no vtx #mu#mu trigger efficiency");
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_L2muTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_L2muTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_L2muTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_L2muTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_L2muTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_L2muTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_L2muTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_L2muTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_L2muTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_L2muTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_L2muTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_L2muTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_L2muTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_L2muTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_L2muTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_L2muTrigEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_L2muTrigEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_L2muTrigEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_L2muTrigEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_L2muTrigEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_L2muTrigEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_L2muTrigEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_L2muTrigEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_L2muTrigEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_L2muTrigEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_L2muTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_L2muTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_L2muTrigEta->SetBorderSize(0);
    leg_L2muTrigEta->SetTextSize(0.03);
    leg_L2muTrigEta->SetLineColor(1);
    leg_L2muTrigEta->SetLineStyle(1);
    leg_L2muTrigEta->SetLineWidth(1);
    leg_L2muTrigEta->SetFillColor(0);
    leg_L2muTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_L2muTrigEta = leg_L2muTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_L2muTrigEta->SetMarkerColor(9);
    entry_L2muTrigEta->SetMarkerStyle(26);
    entry_L2muTrigEta->SetMarkerSize(2);
    entry_L2muTrigEta->SetTextFont(42);
    entry_L2muTrigEta = leg_L2muTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_L2muTrigEta->SetMarkerColor(8);
    entry_L2muTrigEta->SetMarkerStyle(4);
    entry_L2muTrigEta->SetMarkerSize(2);
    entry_L2muTrigEta->SetTextFont(42);
    entry_L2muTrigEta = leg_L2muTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_L2muTrigEta->SetMarkerColor(2);
    entry_L2muTrigEta->SetMarkerStyle(5);
    entry_L2muTrigEta->SetMarkerSize(2);
    entry_L2muTrigEta->SetTextFont(42);
//    entry_L2muTrigEta = leg_L2muTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigEta->SetMarkerColor(1);
    entry_L2muTrigEta->SetMarkerStyle(2);
    entry_L2muTrigEta->SetMarkerSize(2);
    entry_L2muTrigEta->SetTextFont(42);
//    entry_L2muTrigEta = leg_L2muTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigEta->SetMarkerColor(4);
    entry_L2muTrigEta->SetMarkerStyle(3);
    entry_L2muTrigEta->SetMarkerSize(2);
    entry_L2muTrigEta->SetTextFont(42);
    leg_L2muTrigEta->Draw("HIST");

    Canvas_L2muTrigEta->Modified();
    Canvas_L2muTrigEta->cd();
    Canvas_L2muTrigEta->SetSelected(Canvas_L2muTrigEta);
    Canvas_L2muTrigEta->SaveAs( "L2muTrig_eta_eff.pdf" );
    Canvas_L2muTrigEta->SaveAs( "L2muTrig_eta_eff.png" );

    TH1F* h_leadingMuonEta_muOrMumuTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_muOrMumuTrig");
    TH1F* h_leadingMuonEta_muOrMumuTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_muOrMumuTrig");
    TH1F* h_leadingMuonEta_muOrMumuTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_muOrMumuTrig");
//    TH1F* h_leadingMuonEta_muOrMumuTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_muOrMumuTrig");
//    TH1F* h_leadingMuonEta_muOrMumuTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_muOrMumuTrig");

    TCanvas *Canvas_muOrMumuTrigEta = new TCanvas("Canvas_muOrMumuTrigEta", "Canvas_muOrMumuTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muOrMumuTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muOrMumuTrigEta->SetFillColor(0);
    Canvas_muOrMumuTrigEta->SetBorderMode(0);
    Canvas_muOrMumuTrigEta->SetBorderSize(2);
    Canvas_muOrMumuTrigEta->SetLeftMargin(0.15);
    Canvas_muOrMumuTrigEta->SetRightMargin(0.05);
    Canvas_muOrMumuTrigEta->SetTopMargin(0.05);
    Canvas_muOrMumuTrigEta->SetBottomMargin(0.15);
    Canvas_muOrMumuTrigEta->SetGridy();
    Canvas_muOrMumuTrigEta->SetTickx(1);
    Canvas_muOrMumuTrigEta->SetTicky(1);
    Canvas_muOrMumuTrigEta->SetFrameBorderMode(0);
    Canvas_muOrMumuTrigEta->cd();

    h_leadingMuonEta_muOrMumuTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetTitle("#mu#mu OR #mu trigger efficiency");
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_muOrMumuTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_muOrMumuTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_muOrMumuTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_muOrMumuTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_muOrMumuTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_muOrMumuTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_muOrMumuTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_muOrMumuTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_muOrMumuTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_muOrMumuTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_muOrMumuTrigEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_muOrMumuTrigEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_muOrMumuTrigEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_muOrMumuTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_muOrMumuTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muOrMumuTrigEta->SetBorderSize(0);
    leg_muOrMumuTrigEta->SetTextSize(0.03);
    leg_muOrMumuTrigEta->SetLineColor(1);
    leg_muOrMumuTrigEta->SetLineStyle(1);
    leg_muOrMumuTrigEta->SetLineWidth(1);
    leg_muOrMumuTrigEta->SetFillColor(0);
    leg_muOrMumuTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_muOrMumuTrigEta = leg_muOrMumuTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_muOrMumuTrigEta->SetMarkerColor(9);
    entry_muOrMumuTrigEta->SetMarkerStyle(26);
    entry_muOrMumuTrigEta->SetMarkerSize(2);
    entry_muOrMumuTrigEta->SetTextFont(42);
    entry_muOrMumuTrigEta = leg_muOrMumuTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_muOrMumuTrigEta->SetMarkerColor(8);
    entry_muOrMumuTrigEta->SetMarkerStyle(4);
    entry_muOrMumuTrigEta->SetMarkerSize(2);
    entry_muOrMumuTrigEta->SetTextFont(42);
    entry_muOrMumuTrigEta = leg_muOrMumuTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_muOrMumuTrigEta->SetMarkerColor(2);
    entry_muOrMumuTrigEta->SetMarkerStyle(5);
    entry_muOrMumuTrigEta->SetMarkerSize(2);
    entry_muOrMumuTrigEta->SetTextFont(42);
//    entry_muOrMumuTrigEta = leg_muOrMumuTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigEta->SetMarkerColor(1);
    entry_muOrMumuTrigEta->SetMarkerStyle(2);
    entry_muOrMumuTrigEta->SetMarkerSize(2);
    entry_muOrMumuTrigEta->SetTextFont(42);
//    entry_muOrMumuTrigEta = leg_muOrMumuTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigEta->SetMarkerColor(4);
    entry_muOrMumuTrigEta->SetMarkerStyle(3);
    entry_muOrMumuTrigEta->SetMarkerSize(2);
    entry_muOrMumuTrigEta->SetTextFont(42);
    leg_muOrMumuTrigEta->Draw("HIST");

    Canvas_muOrMumuTrigEta->Modified();
    Canvas_muOrMumuTrigEta->cd();
    Canvas_muOrMumuTrigEta->SetSelected(Canvas_muOrMumuTrigEta);
    Canvas_muOrMumuTrigEta->SaveAs( "muOrMumuTrig_eta_eff.pdf" );
    Canvas_muOrMumuTrigEta->SaveAs( "muOrMumuTrig_eta_eff.png" );

    TH1F* h_leadingMuonEta_ORTrigEta_ctau0    = (TH1F*)inFile0->Get("h_leadingMuonEta_ORTrig");
    TH1F* h_leadingMuonEta_ORTrigEta_ctau1    = (TH1F*)inFile1->Get("h_leadingMuonEta_ORTrig");
    TH1F* h_leadingMuonEta_ORTrigEta_ctau10   = (TH1F*)inFile2->Get("h_leadingMuonEta_ORTrig");
//    TH1F* h_leadingMuonEta_ORTrigEta_ctau100  = (TH1F*)inFile3->Get("h_leadingMuonEta_ORTrig");
//    TH1F* h_leadingMuonEta_ORTrigEta_ctau1000 = (TH1F*)inFile4->Get("h_leadingMuonEta_ORTrig");

    TCanvas *Canvas_ORTrigEta = new TCanvas("Canvas_ORTrigEta", "Canvas_ORTrigEta",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_ORTrigEta->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_ORTrigEta->SetFillColor(0);
    Canvas_ORTrigEta->SetBorderMode(0);
    Canvas_ORTrigEta->SetBorderSize(2);
    Canvas_ORTrigEta->SetLeftMargin(0.15);
    Canvas_ORTrigEta->SetRightMargin(0.05);
    Canvas_ORTrigEta->SetTopMargin(0.05);
    Canvas_ORTrigEta->SetBottomMargin(0.15);
    Canvas_ORTrigEta->SetGridy();
    Canvas_ORTrigEta->SetTickx(1);
    Canvas_ORTrigEta->SetTicky(1);
    Canvas_ORTrigEta->SetFrameBorderMode(0);
    Canvas_ORTrigEta->cd();

    h_leadingMuonEta_ORTrigEta_ctau0->SetTitle("");
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetTitle("leading muon #eta");
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetLabelFont(42);
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetLabelSize(0.05);
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetTitleSize(0.05);
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetTitleOffset(1.3);
    h_leadingMuonEta_ORTrigEta_ctau0->GetXaxis()->SetTitleFont(42);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetRangeUser(0.0, 1.0);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetTitle("OR #mu trigger efficiency");
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetLabelFont(42);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetLabelSize(0.035);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetTitleSize(0.035);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetTitleOffset(0);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetTitleFont(42);
    h_leadingMuonEta_ORTrigEta_ctau0->GetYaxis()->SetNdivisions(510);
    h_leadingMuonEta_ORTrigEta_ctau0->SetLineColor(9);
    h_leadingMuonEta_ORTrigEta_ctau0->SetMarkerColor(9);
    h_leadingMuonEta_ORTrigEta_ctau0->SetMarkerStyle(26);
    h_leadingMuonEta_ORTrigEta_ctau0->SetMarkerSize(1);
    h_leadingMuonEta_ORTrigEta_ctau0->Draw("HIST p");

    h_leadingMuonEta_ORTrigEta_ctau1->SetLineColor(8);
    h_leadingMuonEta_ORTrigEta_ctau1->SetMarkerColor(8);
    h_leadingMuonEta_ORTrigEta_ctau1->SetMarkerStyle(4);
    h_leadingMuonEta_ORTrigEta_ctau1->SetMarkerSize(1);
    h_leadingMuonEta_ORTrigEta_ctau1->Draw("HIST p same");

    h_leadingMuonEta_ORTrigEta_ctau10->SetLineColor(2);
    h_leadingMuonEta_ORTrigEta_ctau10->SetMarkerColor(2);
    h_leadingMuonEta_ORTrigEta_ctau10->SetMarkerStyle(5);
    h_leadingMuonEta_ORTrigEta_ctau10->SetMarkerSize(1);
    h_leadingMuonEta_ORTrigEta_ctau10->Draw("HIST p same");

//    h_leadingMuonEta_ORTrigEta_ctau100->SetLineColor(1);
//    h_leadingMuonEta_ORTrigEta_ctau100->SetMarkerColor(1);
//    h_leadingMuonEta_ORTrigEta_ctau100->SetMarkerStyle(2);
//    h_leadingMuonEta_ORTrigEta_ctau100->SetMarkerSize(1);
//    h_leadingMuonEta_ORTrigEta_ctau100->Draw("HIST p same");

//    h_leadingMuonEta_ORTrigEta_ctau1000->SetLineColor(4);
//    h_leadingMuonEta_ORTrigEta_ctau1000->SetMarkerColor(4);
//    h_leadingMuonEta_ORTrigEta_ctau1000->SetMarkerStyle(3);
//    h_leadingMuonEta_ORTrigEta_ctau1000->SetMarkerSize(1);
//    h_leadingMuonEta_ORTrigEta_ctau1000->Draw("HIST p same");

    TLegend *leg_ORTrigEta = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_ORTrigEta->SetBorderSize(0);
    leg_ORTrigEta->SetTextSize(0.03);
    leg_ORTrigEta->SetLineColor(1);
    leg_ORTrigEta->SetLineStyle(1);
    leg_ORTrigEta->SetLineWidth(1);
    leg_ORTrigEta->SetFillColor(0);
    leg_ORTrigEta->SetFillStyle(1001);
    TLegendEntry *entry_ORTrigEta = leg_ORTrigEta->AddEntry("ms2_ctau0", "c_{#tau} = 0 mm", "P");
    entry_ORTrigEta->SetMarkerColor(9);
    entry_ORTrigEta->SetMarkerStyle(26);
    entry_ORTrigEta->SetMarkerSize(2);
    entry_ORTrigEta->SetTextFont(42);
    entry_ORTrigEta = leg_ORTrigEta->AddEntry("ms2_ctau1", "c_{#tau} = 1 mm", "P");
    entry_ORTrigEta->SetMarkerColor(8);
    entry_ORTrigEta->SetMarkerStyle(4);
    entry_ORTrigEta->SetMarkerSize(2);
    entry_ORTrigEta->SetTextFont(42);
    entry_ORTrigEta = leg_ORTrigEta->AddEntry("ms2_ctau10", "c_{#tau} = 10 mm", "P");
    entry_ORTrigEta->SetMarkerColor(2);
    entry_ORTrigEta->SetMarkerStyle(5);
    entry_ORTrigEta->SetMarkerSize(2);
    entry_ORTrigEta->SetTextFont(42);
//    entry_ORTrigEta = leg_ORTrigEta->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigEta->SetMarkerColor(1);
    entry_ORTrigEta->SetMarkerStyle(2);
    entry_ORTrigEta->SetMarkerSize(2);
    entry_ORTrigEta->SetTextFont(42);
//    entry_ORTrigEta = leg_ORTrigEta->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigEta->SetMarkerColor(4);
    entry_ORTrigEta->SetMarkerStyle(3);
    entry_ORTrigEta->SetMarkerSize(2);
    entry_ORTrigEta->SetTextFont(42);
    leg_ORTrigEta->Draw("HIST");

    Canvas_ORTrigEta->Modified();
    Canvas_ORTrigEta->cd();
    Canvas_ORTrigEta->SetSelected(Canvas_ORTrigEta);
    Canvas_ORTrigEta->SaveAs( "ORTrig_eta_eff.pdf" );
    Canvas_ORTrigEta->SaveAs( "ORTrig_eta_eff.png" );

    // DeltaR

    TH1F* h_delR_muTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_muTrig");
    TH1F* h_delR_muTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_muTrig");
//    TH1F* h_delR_muTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_muTrig");
//    TH1F* h_delR_muTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_muTrig");

    TCanvas *Canvas_muTrigDelR = new TCanvas("Canvas_muTrigDelR", "Canvas_muTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muTrigDelR->SetFillColor(0);
    Canvas_muTrigDelR->SetBorderMode(0);
    Canvas_muTrigDelR->SetBorderSize(2);
    Canvas_muTrigDelR->SetLeftMargin(0.15);
    Canvas_muTrigDelR->SetRightMargin(0.05);
    Canvas_muTrigDelR->SetTopMargin(0.05);
    Canvas_muTrigDelR->SetBottomMargin(0.15);
    Canvas_muTrigDelR->SetGridy();
    Canvas_muTrigDelR->SetTickx(1);
    Canvas_muTrigDelR->SetTicky(1);
    Canvas_muTrigDelR->SetFrameBorderMode(0);
    Canvas_muTrigDelR->cd();

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

//    h_delR_muTrigDelR_ctau100->SetLineColor(1);
//    h_delR_muTrigDelR_ctau100->SetMarkerColor(1);
//    h_delR_muTrigDelR_ctau100->SetMarkerStyle(2);
//    h_delR_muTrigDelR_ctau100->SetMarkerSize(1);
//    h_delR_muTrigDelR_ctau100->Draw("HIST p same");

//    h_delR_muTrigDelR_ctau1000->SetLineColor(4);
//    h_delR_muTrigDelR_ctau1000->SetMarkerColor(4);
//    h_delR_muTrigDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_muTrigDelR_ctau1000->SetMarkerSize(1);
//    h_delR_muTrigDelR_ctau1000->Draw("HIST p same");

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
//    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigDelR->SetMarkerColor(1);
    entry_muTrigDelR->SetMarkerStyle(2);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
//    entry_muTrigDelR = leg_muTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigDelR->SetMarkerColor(4);
    entry_muTrigDelR->SetMarkerStyle(3);
    entry_muTrigDelR->SetMarkerSize(2);
    entry_muTrigDelR->SetTextFont(42);
    leg_muTrigDelR->Draw("HIST");

    Canvas_muTrigDelR->Modified();
    Canvas_muTrigDelR->cd();
    Canvas_muTrigDelR->SetSelected(Canvas_muTrigDelR);
    Canvas_muTrigDelR->SaveAs( "muTrig_DelR_eff.pdf" );
    Canvas_muTrigDelR->SaveAs( "muTrig_DelR_eff.png" );

    //

    TH1F* h_delR_mumuTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrig");
    TH1F* h_delR_mumuTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrig");
//    TH1F* h_delR_mumuTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrig");
//    TH1F* h_delR_mumuTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrig");

    TCanvas *Canvas_mumuTrigDelR = new TCanvas("Canvas_mumuTrigDelR", "Canvas_mumuTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigDelR->SetFillColor(0);
    Canvas_mumuTrigDelR->SetBorderMode(0);
    Canvas_mumuTrigDelR->SetBorderSize(2);
    Canvas_mumuTrigDelR->SetLeftMargin(0.15);
    Canvas_mumuTrigDelR->SetRightMargin(0.05);
    Canvas_mumuTrigDelR->SetTopMargin(0.05);
    Canvas_mumuTrigDelR->SetBottomMargin(0.15);
    Canvas_mumuTrigDelR->SetGridy();
    Canvas_mumuTrigDelR->SetTickx(1);
    Canvas_mumuTrigDelR->SetTicky(1);
    Canvas_mumuTrigDelR->SetFrameBorderMode(0);
    Canvas_mumuTrigDelR->cd();

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

//    h_delR_mumuTrigDelR_ctau100->SetLineColor(1);
//    h_delR_mumuTrigDelR_ctau100->SetMarkerColor(1);
//    h_delR_mumuTrigDelR_ctau100->SetMarkerStyle(2);
//    h_delR_mumuTrigDelR_ctau100->SetMarkerSize(1);
//    h_delR_mumuTrigDelR_ctau100->Draw("HIST p same");

//    h_delR_mumuTrigDelR_ctau1000->SetLineColor(4);
//    h_delR_mumuTrigDelR_ctau1000->SetMarkerColor(4);
//    h_delR_mumuTrigDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_mumuTrigDelR_ctau1000->SetMarkerSize(1);
//    h_delR_mumuTrigDelR_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(1);
    entry_mumuTrigDelR->SetMarkerStyle(2);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
//    entry_mumuTrigDelR = leg_mumuTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigDelR->SetMarkerColor(4);
    entry_mumuTrigDelR->SetMarkerStyle(3);
    entry_mumuTrigDelR->SetMarkerSize(2);
    entry_mumuTrigDelR->SetTextFont(42);
    leg_mumuTrigDelR->Draw("HIST");

    Canvas_mumuTrigDelR->Modified();
    Canvas_mumuTrigDelR->cd();
    Canvas_mumuTrigDelR->SetSelected(Canvas_mumuTrigDelR);
    Canvas_mumuTrigDelR->SaveAs( "mumuTrig_DelR_eff.pdf" );
    Canvas_mumuTrigDelR->SaveAs( "mumuTrig_DelR_eff.png" );

    //

    TH1F* h_delR_mumuTrigMassCutDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrigMassCut");
    TH1F* h_delR_mumuTrigMassCutDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrigMassCut");
//    TH1F* h_delR_mumuTrigMassCutDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrigMassCut");
//    TH1F* h_delR_mumuTrigMassCutDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrigMassCut");

    TCanvas *Canvas_mumuTrigMassCutDelR = new TCanvas("Canvas_mumuTrigMassCutDelR", "Canvas_mumuTrigMassCutDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigMassCutDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigMassCutDelR->SetFillColor(0);
    Canvas_mumuTrigMassCutDelR->SetBorderMode(0);
    Canvas_mumuTrigMassCutDelR->SetBorderSize(2);
    Canvas_mumuTrigMassCutDelR->SetLeftMargin(0.15);
    Canvas_mumuTrigMassCutDelR->SetRightMargin(0.05);
    Canvas_mumuTrigMassCutDelR->SetTopMargin(0.05);
    Canvas_mumuTrigMassCutDelR->SetBottomMargin(0.15);
    Canvas_mumuTrigMassCutDelR->SetGridy();
    Canvas_mumuTrigMassCutDelR->SetTickx(1);
    Canvas_mumuTrigMassCutDelR->SetTicky(1);
    Canvas_mumuTrigMassCutDelR->SetFrameBorderMode(0);
    Canvas_mumuTrigMassCutDelR->cd();

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

//    h_delR_mumuTrigMassCutDelR_ctau100->SetLineColor(1);
//    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerColor(1);
//    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerStyle(2);
//    h_delR_mumuTrigMassCutDelR_ctau100->SetMarkerSize(1);
//    h_delR_mumuTrigMassCutDelR_ctau100->Draw("HIST p same");

//    h_delR_mumuTrigMassCutDelR_ctau1000->SetLineColor(4);
//    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerColor(4);
//    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_mumuTrigMassCutDelR_ctau1000->SetMarkerSize(1);
//    h_delR_mumuTrigMassCutDelR_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(1);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(2);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
//    entry_mumuTrigMassCutDelR = leg_mumuTrigMassCutDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutDelR->SetMarkerColor(4);
    entry_mumuTrigMassCutDelR->SetMarkerStyle(3);
    entry_mumuTrigMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigMassCutDelR->SetTextFont(42);
    leg_mumuTrigMassCutDelR->Draw("HIST");

    Canvas_mumuTrigMassCutDelR->Modified();
    Canvas_mumuTrigMassCutDelR->cd();
    Canvas_mumuTrigMassCutDelR->SetSelected(Canvas_mumuTrigMassCutDelR);
    Canvas_mumuTrigMassCutDelR->SaveAs( "mumuTrigMassCut_DelR_eff.pdf" );
    Canvas_mumuTrigMassCutDelR->SaveAs( "mumuTrigMassCut_DelR_eff.png" );

    //

    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_mumuTrigNoMassCut");
    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_mumuTrigNoMassCut");
//    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_mumuTrigNoMassCut");
//    TH1F* h_delR_mumuTrigNoMassCutDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_mumuTrigNoMassCut");

    TCanvas *Canvas_mumuTrigNoMassCutDelR = new TCanvas("Canvas_mumuTrigNoMassCutDelR", "Canvas_mumuTrigNoMassCutDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigNoMassCutDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigNoMassCutDelR->SetFillColor(0);
    Canvas_mumuTrigNoMassCutDelR->SetBorderMode(0);
    Canvas_mumuTrigNoMassCutDelR->SetBorderSize(2);
    Canvas_mumuTrigNoMassCutDelR->SetLeftMargin(0.15);
    Canvas_mumuTrigNoMassCutDelR->SetRightMargin(0.05);
    Canvas_mumuTrigNoMassCutDelR->SetTopMargin(0.05);
    Canvas_mumuTrigNoMassCutDelR->SetBottomMargin(0.15);
    Canvas_mumuTrigNoMassCutDelR->SetGridy();
    Canvas_mumuTrigNoMassCutDelR->SetTickx(1);
    Canvas_mumuTrigNoMassCutDelR->SetTicky(1);
    Canvas_mumuTrigNoMassCutDelR->SetFrameBorderMode(0);
    Canvas_mumuTrigNoMassCutDelR->cd();

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

//    h_delR_mumuTrigNoMassCutDelR_ctau100->SetLineColor(1);
//    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerColor(1);
//    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerStyle(2);
//    h_delR_mumuTrigNoMassCutDelR_ctau100->SetMarkerSize(1);
//    h_delR_mumuTrigNoMassCutDelR_ctau100->Draw("HIST p same");

//    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetLineColor(4);
//    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerColor(4);
//    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_mumuTrigNoMassCutDelR_ctau1000->SetMarkerSize(1);
//    h_delR_mumuTrigNoMassCutDelR_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(1);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
//    entry_mumuTrigNoMassCutDelR = leg_mumuTrigNoMassCutDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutDelR->SetMarkerColor(4);
    entry_mumuTrigNoMassCutDelR->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutDelR->SetMarkerSize(2);
    entry_mumuTrigNoMassCutDelR->SetTextFont(42);
    leg_mumuTrigNoMassCutDelR->Draw("HIST");

    Canvas_mumuTrigNoMassCutDelR->Modified();
    Canvas_mumuTrigNoMassCutDelR->cd();
    Canvas_mumuTrigNoMassCutDelR->SetSelected(Canvas_mumuTrigNoMassCutDelR);
    Canvas_mumuTrigNoMassCutDelR->SaveAs( "mumuTrigNoMassCut_DelR_eff.pdf" );
    Canvas_mumuTrigNoMassCutDelR->SaveAs( "mumuTrigNoMassCut_DelR_eff.png" );

    //

    TH1F* h_delR_L2muTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_L2muTrig");
    TH1F* h_delR_L2muTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_L2muTrig");
//    TH1F* h_delR_L2muTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_L2muTrig");
//    TH1F* h_delR_L2muTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_L2muTrig");

    TCanvas *Canvas_L2muTrigDelR = new TCanvas("Canvas_L2muTrigDelR", "Canvas_L2muTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_L2muTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_L2muTrigDelR->SetFillColor(0);
    Canvas_L2muTrigDelR->SetBorderMode(0);
    Canvas_L2muTrigDelR->SetBorderSize(2);
    Canvas_L2muTrigDelR->SetLeftMargin(0.15);
    Canvas_L2muTrigDelR->SetRightMargin(0.05);
    Canvas_L2muTrigDelR->SetTopMargin(0.05);
    Canvas_L2muTrigDelR->SetBottomMargin(0.15);
    Canvas_L2muTrigDelR->SetGridy();
    Canvas_L2muTrigDelR->SetTickx(1);
    Canvas_L2muTrigDelR->SetTicky(1);
    Canvas_L2muTrigDelR->SetFrameBorderMode(0);
    Canvas_L2muTrigDelR->cd();

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

//    h_delR_L2muTrigDelR_ctau100->SetLineColor(1);
//    h_delR_L2muTrigDelR_ctau100->SetMarkerColor(1);
//    h_delR_L2muTrigDelR_ctau100->SetMarkerStyle(2);
//    h_delR_L2muTrigDelR_ctau100->SetMarkerSize(1);
//    h_delR_L2muTrigDelR_ctau100->Draw("HIST p same");

//    h_delR_L2muTrigDelR_ctau1000->SetLineColor(4);
//    h_delR_L2muTrigDelR_ctau1000->SetMarkerColor(4);
//    h_delR_L2muTrigDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_L2muTrigDelR_ctau1000->SetMarkerSize(1);
//    h_delR_L2muTrigDelR_ctau1000->Draw("HIST p same");

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
//    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(1);
    entry_L2muTrigDelR->SetMarkerStyle(2);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
//    entry_L2muTrigDelR = leg_L2muTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigDelR->SetMarkerColor(4);
    entry_L2muTrigDelR->SetMarkerStyle(3);
    entry_L2muTrigDelR->SetMarkerSize(2);
    entry_L2muTrigDelR->SetTextFont(42);
    leg_L2muTrigDelR->Draw("HIST");

    Canvas_L2muTrigDelR->Modified();
    Canvas_L2muTrigDelR->cd();
    Canvas_L2muTrigDelR->SetSelected(Canvas_L2muTrigDelR);
    Canvas_L2muTrigDelR->SaveAs( "L2muTrig_DelR_eff.pdf" );
    Canvas_L2muTrigDelR->SaveAs( "L2muTrig_DelR_eff.png" );

    TH1F* h_delR_muOrMumuTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_muOrMumuTrig");
    TH1F* h_delR_muOrMumuTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_muOrMumuTrig");
//    TH1F* h_delR_muOrMumuTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_muOrMumuTrig");
//    TH1F* h_delR_muOrMumuTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_muOrMumuTrig");

    TCanvas *Canvas_muOrMumuTrigDelR = new TCanvas("Canvas_muOrMumuTrigDelR", "Canvas_muOrMumuTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muOrMumuTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muOrMumuTrigDelR->SetFillColor(0);
    Canvas_muOrMumuTrigDelR->SetBorderMode(0);
    Canvas_muOrMumuTrigDelR->SetBorderSize(2);
    Canvas_muOrMumuTrigDelR->SetLeftMargin(0.15);
    Canvas_muOrMumuTrigDelR->SetRightMargin(0.05);
    Canvas_muOrMumuTrigDelR->SetTopMargin(0.05);
    Canvas_muOrMumuTrigDelR->SetBottomMargin(0.15);
    Canvas_muOrMumuTrigDelR->SetGridy();
    Canvas_muOrMumuTrigDelR->SetTickx(1);
    Canvas_muOrMumuTrigDelR->SetTicky(1);
    Canvas_muOrMumuTrigDelR->SetFrameBorderMode(0);
    Canvas_muOrMumuTrigDelR->cd();

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

//    h_delR_muOrMumuTrigDelR_ctau100->SetLineColor(1);
//    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerColor(1);
//    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerStyle(2);
//    h_delR_muOrMumuTrigDelR_ctau100->SetMarkerSize(1);
//    h_delR_muOrMumuTrigDelR_ctau100->Draw("HIST p same");

//    h_delR_muOrMumuTrigDelR_ctau1000->SetLineColor(4);
//    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerColor(4);
//    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_muOrMumuTrigDelR_ctau1000->SetMarkerSize(1);
//    h_delR_muOrMumuTrigDelR_ctau1000->Draw("HIST p same");

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
//    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(1);
    entry_muOrMumuTrigDelR->SetMarkerStyle(2);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
//    entry_muOrMumuTrigDelR = leg_muOrMumuTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigDelR->SetMarkerColor(4);
    entry_muOrMumuTrigDelR->SetMarkerStyle(3);
    entry_muOrMumuTrigDelR->SetMarkerSize(2);
    entry_muOrMumuTrigDelR->SetTextFont(42);
    leg_muOrMumuTrigDelR->Draw("HIST");

    Canvas_muOrMumuTrigDelR->Modified();
    Canvas_muOrMumuTrigDelR->cd();
    Canvas_muOrMumuTrigDelR->SetSelected(Canvas_muOrMumuTrigDelR);
    Canvas_muOrMumuTrigDelR->SaveAs( "muOrMumuTrig_DelR_eff.pdf" );
    Canvas_muOrMumuTrigDelR->SaveAs( "muOrMumuTrig_DelR_eff.png" );

    TH1F* h_delR_ORTrigDelR_ctau0    = (TH1F*)inFile0->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau1    = (TH1F*)inFile1->Get("h_delR_ORTrig");
    TH1F* h_delR_ORTrigDelR_ctau10   = (TH1F*)inFile2->Get("h_delR_ORTrig");
//    TH1F* h_delR_ORTrigDelR_ctau100  = (TH1F*)inFile3->Get("h_delR_ORTrig");
//    TH1F* h_delR_ORTrigDelR_ctau1000 = (TH1F*)inFile4->Get("h_delR_ORTrig");

    TCanvas *Canvas_ORTrigDelR = new TCanvas("Canvas_ORTrigDelR", "Canvas_ORTrigDelR",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_ORTrigDelR->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_ORTrigDelR->SetFillColor(0);
    Canvas_ORTrigDelR->SetBorderMode(0);
    Canvas_ORTrigDelR->SetBorderSize(2);
    Canvas_ORTrigDelR->SetLeftMargin(0.15);
    Canvas_ORTrigDelR->SetRightMargin(0.05);
    Canvas_ORTrigDelR->SetTopMargin(0.05);
    Canvas_ORTrigDelR->SetBottomMargin(0.15);
    Canvas_ORTrigDelR->SetGridy();
    Canvas_ORTrigDelR->SetTickx(1);
    Canvas_ORTrigDelR->SetTicky(1);
    Canvas_ORTrigDelR->SetFrameBorderMode(0);
    Canvas_ORTrigDelR->cd();

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

//    h_delR_ORTrigDelR_ctau100->SetLineColor(1);
//    h_delR_ORTrigDelR_ctau100->SetMarkerColor(1);
//    h_delR_ORTrigDelR_ctau100->SetMarkerStyle(2);
//    h_delR_ORTrigDelR_ctau100->SetMarkerSize(1);
//    h_delR_ORTrigDelR_ctau100->Draw("HIST p same");

//    h_delR_ORTrigDelR_ctau1000->SetLineColor(4);
//    h_delR_ORTrigDelR_ctau1000->SetMarkerColor(4);
//    h_delR_ORTrigDelR_ctau1000->SetMarkerStyle(3);
//    h_delR_ORTrigDelR_ctau1000->SetMarkerSize(1);
//    h_delR_ORTrigDelR_ctau1000->Draw("HIST p same");

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
//    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(1);
    entry_ORTrigDelR->SetMarkerStyle(2);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
//    entry_ORTrigDelR = leg_ORTrigDelR->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigDelR->SetMarkerColor(4);
    entry_ORTrigDelR->SetMarkerStyle(3);
    entry_ORTrigDelR->SetMarkerSize(2);
    entry_ORTrigDelR->SetTextFont(42);
    leg_ORTrigDelR->Draw("HIST");

    Canvas_ORTrigDelR->Modified();
    Canvas_ORTrigDelR->cd();
    Canvas_ORTrigDelR->SetSelected(Canvas_ORTrigDelR);
    Canvas_ORTrigDelR->SaveAs( "ORTrig_DelR_eff.pdf" );
    Canvas_ORTrigDelR->SaveAs( "ORTrig_DelR_eff.png" );

///// mass

    // mass

    TH1F* h_mass_muTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_muTrig");
    TH1F* h_mass_muTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_muTrig");
//    TH1F* h_mass_muTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_muTrig");
//    TH1F* h_mass_muTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_muTrig");

    TCanvas *Canvas_muTrigMass = new TCanvas("Canvas_muTrigMass", "Canvas_muTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muTrigMass->SetFillColor(0);
    Canvas_muTrigMass->SetBorderMode(0);
    Canvas_muTrigMass->SetBorderSize(2);
    Canvas_muTrigMass->SetLeftMargin(0.15);
    Canvas_muTrigMass->SetRightMargin(0.05);
    Canvas_muTrigMass->SetTopMargin(0.05);
    Canvas_muTrigMass->SetBottomMargin(0.15);
    Canvas_muTrigMass->SetGridy();
    Canvas_muTrigMass->SetTickx(1);
    Canvas_muTrigMass->SetTicky(1);
    Canvas_muTrigMass->SetFrameBorderMode(0);
    Canvas_muTrigMass->cd();

    h_mass_muTrigMass_ctau0->SetTitle("");
    h_mass_muTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_muTrigMass_ctau100->SetLineColor(1);
//    h_mass_muTrigMass_ctau100->SetMarkerColor(1);
//    h_mass_muTrigMass_ctau100->SetMarkerStyle(2);
//    h_mass_muTrigMass_ctau100->SetMarkerSize(1);
//    h_mass_muTrigMass_ctau100->Draw("HIST p same");

//    h_mass_muTrigMass_ctau1000->SetLineColor(4);
//    h_mass_muTrigMass_ctau1000->SetMarkerColor(4);
//    h_mass_muTrigMass_ctau1000->SetMarkerStyle(3);
//    h_mass_muTrigMass_ctau1000->SetMarkerSize(1);
//    h_mass_muTrigMass_ctau1000->Draw("HIST p same");

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
//    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muTrigMass->SetMarkerColor(1);
    entry_muTrigMass->SetMarkerStyle(2);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
//    entry_muTrigMass = leg_muTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muTrigMass->SetMarkerColor(4);
    entry_muTrigMass->SetMarkerStyle(3);
    entry_muTrigMass->SetMarkerSize(2);
    entry_muTrigMass->SetTextFont(42);
    leg_muTrigMass->Draw("HIST");

    Canvas_muTrigMass->Modified();
    Canvas_muTrigMass->cd();
    Canvas_muTrigMass->SetSelected(Canvas_muTrigMass);
    Canvas_muTrigMass->SaveAs( "muTrig_mass_eff.pdf" );
    Canvas_muTrigMass->SaveAs( "muTrig_mass_eff.png" );

    //

    TH1F* h_mass_mumuTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrig");
    TH1F* h_mass_mumuTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrig");
//    TH1F* h_mass_mumuTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrig");
//    TH1F* h_mass_mumuTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrig");

    TCanvas *Canvas_mumuTrigMass = new TCanvas("Canvas_mumuTrigMass", "Canvas_mumuTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigMass->SetFillColor(0);
    Canvas_mumuTrigMass->SetBorderMode(0);
    Canvas_mumuTrigMass->SetBorderSize(2);
    Canvas_mumuTrigMass->SetLeftMargin(0.15);
    Canvas_mumuTrigMass->SetRightMargin(0.05);
    Canvas_mumuTrigMass->SetTopMargin(0.05);
    Canvas_mumuTrigMass->SetBottomMargin(0.15);
    Canvas_mumuTrigMass->SetGridy();
    Canvas_mumuTrigMass->SetTickx(1);
    Canvas_mumuTrigMass->SetTicky(1);
    Canvas_mumuTrigMass->SetFrameBorderMode(0);
    Canvas_mumuTrigMass->cd();

    h_mass_mumuTrigMass_ctau0->SetTitle("");
    h_mass_mumuTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_mumuTrigMass_ctau100->SetLineColor(1);
//    h_mass_mumuTrigMass_ctau100->SetMarkerColor(1);
//    h_mass_mumuTrigMass_ctau100->SetMarkerStyle(2);
//    h_mass_mumuTrigMass_ctau100->SetMarkerSize(1);
//    h_mass_mumuTrigMass_ctau100->Draw("HIST p same");

//    h_mass_mumuTrigMass_ctau1000->SetLineColor(4);
//    h_mass_mumuTrigMass_ctau1000->SetMarkerColor(4);
//    h_mass_mumuTrigMass_ctau1000->SetMarkerStyle(3);
//    h_mass_mumuTrigMass_ctau1000->SetMarkerSize(1);
//    h_mass_mumuTrigMass_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(1);
    entry_mumuTrigMass->SetMarkerStyle(2);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
//    entry_mumuTrigMass = leg_mumuTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMass->SetMarkerColor(4);
    entry_mumuTrigMass->SetMarkerStyle(3);
    entry_mumuTrigMass->SetMarkerSize(2);
    entry_mumuTrigMass->SetTextFont(42);
    leg_mumuTrigMass->Draw("HIST");

    Canvas_mumuTrigMass->Modified();
    Canvas_mumuTrigMass->cd();
    Canvas_mumuTrigMass->SetSelected(Canvas_mumuTrigMass);
    Canvas_mumuTrigMass->SaveAs( "mumuTrig_mass_eff.pdf" );
    Canvas_mumuTrigMass->SaveAs( "mumuTrig_mass_eff.png" );

    //

    TH1F* h_mass_mumuTrigMassCutMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrigMassCut");
    TH1F* h_mass_mumuTrigMassCutMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrigMassCut");
//    TH1F* h_mass_mumuTrigMassCutMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrigMassCut");
//    TH1F* h_mass_mumuTrigMassCutMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrigMassCut");

    TCanvas *Canvas_mumuTrigMassCutMass = new TCanvas("Canvas_mumuTrigMassCutMass", "Canvas_mumuTrigMassCutMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigMassCutMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigMassCutMass->SetFillColor(0);
    Canvas_mumuTrigMassCutMass->SetBorderMode(0);
    Canvas_mumuTrigMassCutMass->SetBorderSize(2);
    Canvas_mumuTrigMassCutMass->SetLeftMargin(0.15);
    Canvas_mumuTrigMassCutMass->SetRightMargin(0.05);
    Canvas_mumuTrigMassCutMass->SetTopMargin(0.05);
    Canvas_mumuTrigMassCutMass->SetBottomMargin(0.15);
    Canvas_mumuTrigMassCutMass->SetGridy();
    Canvas_mumuTrigMassCutMass->SetTickx(1);
    Canvas_mumuTrigMassCutMass->SetTicky(1);
    Canvas_mumuTrigMassCutMass->SetFrameBorderMode(0);
    Canvas_mumuTrigMassCutMass->cd();

    h_mass_mumuTrigMassCutMass_ctau0->SetTitle("");
    h_mass_mumuTrigMassCutMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_mumuTrigMassCutMass_ctau100->SetLineColor(1);
//    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerColor(1);
//    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerStyle(2);
//    h_mass_mumuTrigMassCutMass_ctau100->SetMarkerSize(1);
//    h_mass_mumuTrigMassCutMass_ctau100->Draw("HIST p same");

//    h_mass_mumuTrigMassCutMass_ctau1000->SetLineColor(4);
//    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerColor(4);
//    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerStyle(3);
//    h_mass_mumuTrigMassCutMass_ctau1000->SetMarkerSize(1);
//    h_mass_mumuTrigMassCutMass_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(1);
    entry_mumuTrigMassCutMass->SetMarkerStyle(2);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
//    entry_mumuTrigMassCutMass = leg_mumuTrigMassCutMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigMassCutMass->SetMarkerColor(4);
    entry_mumuTrigMassCutMass->SetMarkerStyle(3);
    entry_mumuTrigMassCutMass->SetMarkerSize(2);
    entry_mumuTrigMassCutMass->SetTextFont(42);
    leg_mumuTrigMassCutMass->Draw("HIST");

    Canvas_mumuTrigMassCutMass->Modified();
    Canvas_mumuTrigMassCutMass->cd();
    Canvas_mumuTrigMassCutMass->SetSelected(Canvas_mumuTrigMassCutMass);
    Canvas_mumuTrigMassCutMass->SaveAs( "mumuTrigMassCut_mass_eff.pdf" );
    Canvas_mumuTrigMassCutMass->SaveAs( "mumuTrigMassCut_mass_eff.png" );

    //

    TH1F* h_mass_mumuTrigNoMassCutMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_mumuTrigNoMassCut");
    TH1F* h_mass_mumuTrigNoMassCutMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_mumuTrigNoMassCut");
//    TH1F* h_mass_mumuTrigNoMassCutMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_mumuTrigNoMassCut");
//    TH1F* h_mass_mumuTrigNoMassCutMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_mumuTrigNoMassCut");

    TCanvas *Canvas_mumuTrigNoMassCutMass = new TCanvas("Canvas_mumuTrigNoMassCutMass", "Canvas_mumuTrigNoMassCutMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_mumuTrigNoMassCutMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_mumuTrigNoMassCutMass->SetFillColor(0);
    Canvas_mumuTrigNoMassCutMass->SetBorderMode(0);
    Canvas_mumuTrigNoMassCutMass->SetBorderSize(2);
    Canvas_mumuTrigNoMassCutMass->SetLeftMargin(0.15);
    Canvas_mumuTrigNoMassCutMass->SetRightMargin(0.05);
    Canvas_mumuTrigNoMassCutMass->SetTopMargin(0.05);
    Canvas_mumuTrigNoMassCutMass->SetBottomMargin(0.15);
    Canvas_mumuTrigNoMassCutMass->SetGridy();
    Canvas_mumuTrigNoMassCutMass->SetTickx(1);
    Canvas_mumuTrigNoMassCutMass->SetTicky(1);
    Canvas_mumuTrigNoMassCutMass->SetFrameBorderMode(0);
    Canvas_mumuTrigNoMassCutMass->cd();

    h_mass_mumuTrigNoMassCutMass_ctau0->SetTitle("");
    h_mass_mumuTrigNoMassCutMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_mumuTrigNoMassCutMass_ctau100->SetLineColor(1);
//    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerColor(1);
//    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerStyle(2);
//    h_mass_mumuTrigNoMassCutMass_ctau100->SetMarkerSize(1);
//    h_mass_mumuTrigNoMassCutMass_ctau100->Draw("HIST p same");

//    h_mass_mumuTrigNoMassCutMass_ctau1000->SetLineColor(4);
//    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerColor(4);
//    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerStyle(3);
//    h_mass_mumuTrigNoMassCutMass_ctau1000->SetMarkerSize(1);
//    h_mass_mumuTrigNoMassCutMass_ctau1000->Draw("HIST p same");

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
//    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(1);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(2);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
//    entry_mumuTrigNoMassCutMass = leg_mumuTrigNoMassCutMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_mumuTrigNoMassCutMass->SetMarkerColor(4);
    entry_mumuTrigNoMassCutMass->SetMarkerStyle(3);
    entry_mumuTrigNoMassCutMass->SetMarkerSize(2);
    entry_mumuTrigNoMassCutMass->SetTextFont(42);
    leg_mumuTrigNoMassCutMass->Draw("HIST");

    Canvas_mumuTrigNoMassCutMass->Modified();
    Canvas_mumuTrigNoMassCutMass->cd();
    Canvas_mumuTrigNoMassCutMass->SetSelected(Canvas_mumuTrigNoMassCutMass);
    Canvas_mumuTrigNoMassCutMass->SaveAs( "mumuTrigNoMassCut_mass_eff.pdf" );
    Canvas_mumuTrigNoMassCutMass->SaveAs( "mumuTrigNoMassCut_mass_eff.png" );

    //

    TH1F* h_mass_L2muTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_L2muTrig");
    TH1F* h_mass_L2muTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_L2muTrig");
//    TH1F* h_mass_L2muTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_L2muTrig");
//    TH1F* h_mass_L2muTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_L2muTrig");

    TCanvas *Canvas_L2muTrigMass = new TCanvas("Canvas_L2muTrigMass", "Canvas_L2muTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_L2muTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_L2muTrigMass->SetFillColor(0);
    Canvas_L2muTrigMass->SetBorderMode(0);
    Canvas_L2muTrigMass->SetBorderSize(2);
    Canvas_L2muTrigMass->SetLeftMargin(0.15);
    Canvas_L2muTrigMass->SetRightMargin(0.05);
    Canvas_L2muTrigMass->SetTopMargin(0.05);
    Canvas_L2muTrigMass->SetBottomMargin(0.15);
    Canvas_L2muTrigMass->SetGridy();
    Canvas_L2muTrigMass->SetTickx(1);
    Canvas_L2muTrigMass->SetTicky(1);
    Canvas_L2muTrigMass->SetFrameBorderMode(0);
    Canvas_L2muTrigMass->cd();

    h_mass_L2muTrigMass_ctau0->SetTitle("");
    h_mass_L2muTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_L2muTrigMass_ctau100->SetLineColor(1);
//    h_mass_L2muTrigMass_ctau100->SetMarkerColor(1);
//    h_mass_L2muTrigMass_ctau100->SetMarkerStyle(2);
//    h_mass_L2muTrigMass_ctau100->SetMarkerSize(1);
//    h_mass_L2muTrigMass_ctau100->Draw("HIST p same");

//    h_mass_L2muTrigMass_ctau1000->SetLineColor(4);
//    h_mass_L2muTrigMass_ctau1000->SetMarkerColor(4);
//    h_mass_L2muTrigMass_ctau1000->SetMarkerStyle(3);
//    h_mass_L2muTrigMass_ctau1000->SetMarkerSize(1);
//    h_mass_L2muTrigMass_ctau1000->Draw("HIST p same");

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
//    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(1);
    entry_L2muTrigMass->SetMarkerStyle(2);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
//    entry_L2muTrigMass = leg_L2muTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_L2muTrigMass->SetMarkerColor(4);
    entry_L2muTrigMass->SetMarkerStyle(3);
    entry_L2muTrigMass->SetMarkerSize(2);
    entry_L2muTrigMass->SetTextFont(42);
    leg_L2muTrigMass->Draw("HIST");

    Canvas_L2muTrigMass->Modified();
    Canvas_L2muTrigMass->cd();
    Canvas_L2muTrigMass->SetSelected(Canvas_L2muTrigMass);
    Canvas_L2muTrigMass->SaveAs( "L2muTrig_mass_eff.pdf" );
    Canvas_L2muTrigMass->SaveAs( "L2muTrig_mass_eff.png" );

    TH1F* h_mass_muOrMumuTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_muOrMumuTrig");
    TH1F* h_mass_muOrMumuTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_muOrMumuTrig");
//    TH1F* h_mass_muOrMumuTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_muOrMumuTrig");
//    TH1F* h_mass_muOrMumuTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_muOrMumuTrig");

    TCanvas *Canvas_muOrMumuTrigMass = new TCanvas("Canvas_muOrMumuTrigMass", "Canvas_muOrMumuTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_muOrMumuTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_muOrMumuTrigMass->SetFillColor(0);
    Canvas_muOrMumuTrigMass->SetBorderMode(0);
    Canvas_muOrMumuTrigMass->SetBorderSize(2);
    Canvas_muOrMumuTrigMass->SetLeftMargin(0.15);
    Canvas_muOrMumuTrigMass->SetRightMargin(0.05);
    Canvas_muOrMumuTrigMass->SetTopMargin(0.05);
    Canvas_muOrMumuTrigMass->SetBottomMargin(0.15);
    Canvas_muOrMumuTrigMass->SetGridy();
    Canvas_muOrMumuTrigMass->SetTickx(1);
    Canvas_muOrMumuTrigMass->SetTicky(1);
    Canvas_muOrMumuTrigMass->SetFrameBorderMode(0);
    Canvas_muOrMumuTrigMass->cd();

    h_mass_muOrMumuTrigMass_ctau0->SetTitle("");
    h_mass_muOrMumuTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_muOrMumuTrigMass_ctau100->SetLineColor(1);
//    h_mass_muOrMumuTrigMass_ctau100->SetMarkerColor(1);
//    h_mass_muOrMumuTrigMass_ctau100->SetMarkerStyle(2);
//    h_mass_muOrMumuTrigMass_ctau100->SetMarkerSize(1);
//    h_mass_muOrMumuTrigMass_ctau100->Draw("HIST p same");

//    h_mass_muOrMumuTrigMass_ctau1000->SetLineColor(4);
//    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerColor(4);
//    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerStyle(3);
//    h_mass_muOrMumuTrigMass_ctau1000->SetMarkerSize(1);
//    h_mass_muOrMumuTrigMass_ctau1000->Draw("HIST p same");

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
//    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(1);
    entry_muOrMumuTrigMass->SetMarkerStyle(2);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
//    entry_muOrMumuTrigMass = leg_muOrMumuTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_muOrMumuTrigMass->SetMarkerColor(4);
    entry_muOrMumuTrigMass->SetMarkerStyle(3);
    entry_muOrMumuTrigMass->SetMarkerSize(2);
    entry_muOrMumuTrigMass->SetTextFont(42);
    leg_muOrMumuTrigMass->Draw("HIST");

    Canvas_muOrMumuTrigMass->Modified();
    Canvas_muOrMumuTrigMass->cd();
    Canvas_muOrMumuTrigMass->SetSelected(Canvas_muOrMumuTrigMass);
    Canvas_muOrMumuTrigMass->SaveAs( "muOrMumuTrig_mass_eff.pdf" );
    Canvas_muOrMumuTrigMass->SaveAs( "muOrMumuTrig_mass_eff.png" );

    TH1F* h_mass_ORTrigMass_ctau0    = (TH1F*)inFile0->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau1    = (TH1F*)inFile1->Get("h_diMuonMass_ORTrig");
    TH1F* h_mass_ORTrigMass_ctau10   = (TH1F*)inFile2->Get("h_diMuonMass_ORTrig");
//    TH1F* h_mass_ORTrigMass_ctau100  = (TH1F*)inFile3->Get("h_diMuonMass_ORTrig");
//    TH1F* h_mass_ORTrigMass_ctau1000 = (TH1F*)inFile4->Get("h_diMuonMass_ORTrig");

    TCanvas *Canvas_ORTrigMass = new TCanvas("Canvas_ORTrigMass", "Canvas_ORTrigMass",0,0,1280,925);
    gStyle->SetOptStat(0);
    Canvas_ORTrigMass->Range(-0.4860759,-0.2025316,2.551899,1.063291);
    Canvas_ORTrigMass->SetFillColor(0);
    Canvas_ORTrigMass->SetBorderMode(0);
    Canvas_ORTrigMass->SetBorderSize(2);
    Canvas_ORTrigMass->SetLeftMargin(0.15);
    Canvas_ORTrigMass->SetRightMargin(0.05);
    Canvas_ORTrigMass->SetTopMargin(0.05);
    Canvas_ORTrigMass->SetBottomMargin(0.15);
    Canvas_ORTrigMass->SetGridy();
    Canvas_ORTrigMass->SetTickx(1);
    Canvas_ORTrigMass->SetTicky(1);
    Canvas_ORTrigMass->SetFrameBorderMode(0);
    Canvas_ORTrigMass->cd();

    h_mass_ORTrigMass_ctau0->SetTitle("");
    h_mass_ORTrigMass_ctau0->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
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

//    h_mass_ORTrigMass_ctau100->SetLineColor(1);
//    h_mass_ORTrigMass_ctau100->SetMarkerColor(1);
//    h_mass_ORTrigMass_ctau100->SetMarkerStyle(2);
//    h_mass_ORTrigMass_ctau100->SetMarkerSize(1);
//    h_mass_ORTrigMass_ctau100->Draw("HIST p same");

//    h_mass_ORTrigMass_ctau1000->SetLineColor(4);
//    h_mass_ORTrigMass_ctau1000->SetMarkerColor(4);
//    h_mass_ORTrigMass_ctau1000->SetMarkerStyle(3);
//    h_mass_ORTrigMass_ctau1000->SetMarkerSize(1);
//    h_mass_ORTrigMass_ctau1000->Draw("HIST p same");

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
//    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau100", "c_{#tau} = 100 mm", "P");
    entry_ORTrigMass->SetMarkerColor(1);
    entry_ORTrigMass->SetMarkerStyle(2);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
//    entry_ORTrigMass = leg_ORTrigMass->AddEntry("ms2_ctau1000", "c_{#tau} = 1000 mm", "P");
    entry_ORTrigMass->SetMarkerColor(4);
    entry_ORTrigMass->SetMarkerStyle(3);
    entry_ORTrigMass->SetMarkerSize(2);
    entry_ORTrigMass->SetTextFont(42);
    leg_ORTrigMass->Draw("HIST");

    Canvas_ORTrigMass->Modified();
    Canvas_ORTrigMass->cd();
    Canvas_ORTrigMass->SetSelected(Canvas_ORTrigMass);
    Canvas_ORTrigMass->SaveAs( "ORTrig_mass_eff.pdf" );
    Canvas_ORTrigMass->SaveAs( "ORTrig_mass_eff.png" );

}
