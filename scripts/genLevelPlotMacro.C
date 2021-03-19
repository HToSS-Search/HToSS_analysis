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

void genLevelPlotMacro() {


    TFile* inFile0 = new TFile ("plots/genLevelAnalysis/HtoSS_MS2_ctau0_current.root", "READ");
    TFile* inFile1 = new TFile ("plots/genLevelAnalysis/HtoSS_MS2_ctau0_test.root", "READ");

    TFile* outFile = new TFile ("./output.root", "RECREATE");

// start

// all kaons

    TH1F* h_genParKaonPt0 = (TH1F*)inFile0->Get("h_genParKaonPt");
    TH1F* h_genParKaonPt1 = (TH1F*)inFile1->Get("h_genParKaonPt");

    TCanvas *c_genParKaonPt = new TCanvas("c_genParKaonPt", "",0,0,1280,925);
    c_genParKaonPt->cd();
    h_genParKaonPt0->SetLineColor(9);
    h_genParKaonPt1->SetLineColor(8);
    h_genParKaonPt0->Draw("");
    h_genParKaonPt1->Draw("same");

    outFile->cd();
    c_genParKaonPt->Write();

    TH1F* h_genParKaonEta0 = (TH1F*)inFile0->Get("h_genParKaonEta");
    TH1F* h_genParKaonEta1 = (TH1F*)inFile1->Get("h_genParKaonEta");

    TCanvas *c_genParKaonEta = new TCanvas("c_genParKaonEta", "",0,0,1280,925);
    c_genParKaonEta->cd();
    h_genParKaonEta0->SetLineColor(9);
    h_genParKaonEta1->SetLineColor(8);
    h_genParKaonEta0->Draw("");
    h_genParKaonEta1->Draw("same");

    outFile->cd();
    c_genParKaonEta->Write();

    TH1F* h_genParKaonPhi0 = (TH1F*)inFile0->Get("h_genParKaonPhi");
    TH1F* h_genParKaonPhi1 = (TH1F*)inFile1->Get("h_genParKaonPhi");

    TCanvas *c_genParKaonPhi = new TCanvas("c_genParKaonPhi", "",0,0,1280,925);
    c_genParKaonPhi->cd();
    h_genParKaonPhi0->SetLineColor(9);
    h_genParKaonPhi1->SetLineColor(8);
    h_genParKaonPhi0->Draw("");
    h_genParKaonPhi1->Draw("same");

    outFile->cd();
    c_genParKaonPhi->Write();

    TH1F* h_genParKaonE0 = (TH1F*)inFile0->Get("h_genParKaonE");
    TH1F* h_genParKaonE1 = (TH1F*)inFile1->Get("h_genParKaonE");

    TCanvas *c_genParKaonE = new TCanvas("c_genParKaonE", "",0,0,1280,925);
    c_genParKaonE->cd();
    h_genParKaonE0->SetLineColor(9);
    h_genParKaonE1->SetLineColor(8);
    h_genParKaonE0->Draw("");
    h_genParKaonE1->Draw("same");

    outFile->cd();
    c_genParKaonE->Write();

// charged kaons

    TH1F* h_genParChargedKaonPt0 = (TH1F*)inFile0->Get("h_genParChargedKaonPt");
    TH1F* h_genParChargedKaonPt1 = (TH1F*)inFile1->Get("h_genParChargedKaonPt");

    TCanvas *c_genParChargedKaonPt = new TCanvas("c_genParChargedKaonPt", "",0,0,1280,925);
    c_genParChargedKaonPt->cd();
    h_genParChargedKaonPt0->SetLineColor(9);
    h_genParChargedKaonPt1->SetLineColor(8);
    h_genParChargedKaonPt0->Draw("");
    h_genParChargedKaonPt1->Draw("same");

    outFile->cd();
    c_genParChargedKaonPt->Write();

    TH1F* h_genParChargedKaonEta0 = (TH1F*)inFile0->Get("h_genParChargedKaonEta");
    TH1F* h_genParChargedKaonEta1 = (TH1F*)inFile1->Get("h_genParChargedKaonEta");

    TCanvas *c_genParChargedKaonEta = new TCanvas("c_genParChargedKaonEta", "",0,0,1280,925);
    c_genParChargedKaonEta->cd();
    h_genParChargedKaonEta0->SetLineColor(9);
    h_genParChargedKaonEta1->SetLineColor(8);
    h_genParChargedKaonEta0->Draw("");
    h_genParChargedKaonEta1->Draw("same");

    outFile->cd();
    c_genParChargedKaonEta->Write();

    TH1F* h_genParChargedKaonPhi0 = (TH1F*)inFile0->Get("h_genParChargedKaonPhi");
    TH1F* h_genParChargedKaonPhi1 = (TH1F*)inFile1->Get("h_genParChargedKaonPhi");

    TCanvas *c_genParChargedKaonPhi = new TCanvas("c_genParChargedKaonPhi", "",0,0,1280,925);
    c_genParChargedKaonPhi->cd();
    h_genParChargedKaonPhi0->SetLineColor(9);
    h_genParChargedKaonPhi1->SetLineColor(8);
    h_genParChargedKaonPhi0->Draw("");
    h_genParChargedKaonPhi1->Draw("same");

    outFile->cd();
    c_genParChargedKaonPhi->Write();

    TH1F* h_genParChargedKaonE0 = (TH1F*)inFile0->Get("h_genParChargedKaonE");
    TH1F* h_genParChargedKaonE1 = (TH1F*)inFile1->Get("h_genParChargedKaonE");

    TCanvas *c_genParChargedKaonE = new TCanvas("c_genParChargedKaonE", "",0,0,1280,925);
    c_genParChargedKaonE->cd();
    h_genParChargedKaonE0->SetLineColor(9);
    h_genParChargedKaonE1->SetLineColor(8);
    h_genParChargedKaonE0->Draw("");
    h_genParChargedKaonE1->Draw("same");

    outFile->cd();
    c_genParChargedKaonE->Write();

// neutral kaons

    TH1F* h_genParNeutralKaonPt0 = (TH1F*)inFile0->Get("h_genParNeutralKaonPt");
    TH1F* h_genParNeutralKaonPt1 = (TH1F*)inFile1->Get("h_genParNeutralKaonPt");

    TCanvas *c_genParNeutralKaonPt = new TCanvas("c_genParNeutralKaonPt", "",0,0,1280,925);
    c_genParNeutralKaonPt->cd();
    h_genParNeutralKaonPt0->SetLineColor(9);
    h_genParNeutralKaonPt1->SetLineColor(8);
    h_genParNeutralKaonPt0->Draw("");
    h_genParNeutralKaonPt1->Draw("same");

    outFile->cd();
    c_genParNeutralKaonPt->Write();

    TH1F* h_genParNeutralKaonEta0 = (TH1F*)inFile0->Get("h_genParNeutralKaonEta");
    TH1F* h_genParNeutralKaonEta1 = (TH1F*)inFile1->Get("h_genParNeutralKaonEta");

    TCanvas *c_genParNeutralKaonEta = new TCanvas("c_genParNeutralKaonEta", "",0,0,1280,925);
    c_genParNeutralKaonEta->cd();
    h_genParNeutralKaonEta0->SetLineColor(9);
    h_genParNeutralKaonEta1->SetLineColor(8);
    h_genParNeutralKaonEta0->Draw("");
    h_genParNeutralKaonEta1->Draw("same");

    outFile->cd();
    c_genParNeutralKaonEta->Write();

    TH1F* h_genParNeutralKaonPhi0 = (TH1F*)inFile0->Get("h_genParNeutralKaonPhi");
    TH1F* h_genParNeutralKaonPhi1 = (TH1F*)inFile1->Get("h_genParNeutralKaonPhi");

    TCanvas *c_genParNeutralKaonPhi = new TCanvas("c_genParNeutralKaonPhi", "",0,0,1280,925);
    c_genParNeutralKaonPhi->cd();
    h_genParNeutralKaonPhi0->SetLineColor(9);
    h_genParNeutralKaonPhi1->SetLineColor(8);
    h_genParNeutralKaonPhi0->Draw("");
    h_genParNeutralKaonPhi1->Draw("same");

    outFile->cd();
    c_genParNeutralKaonPhi->Write();

    TH1F* h_genParNeutralKaonE0 = (TH1F*)inFile0->Get("h_genParNeutralKaonE");
    TH1F* h_genParNeutralKaonE1 = (TH1F*)inFile1->Get("h_genParNeutralKaonE");

    TCanvas *c_genParNeutralKaonE = new TCanvas("c_genParNeutralKaonE", "",0,0,1280,925);
    c_genParNeutralKaonE->cd();
    h_genParNeutralKaonE0->SetLineColor(9);
    h_genParNeutralKaonE1->SetLineColor(8);
    h_genParNeutralKaonE0->Draw("");
    h_genParNeutralKaonE1->Draw("same");

    outFile->cd();
    c_genParNeutralKaonE->Write();

// kshorts

    TH1F* h_genParKshortPt0 = (TH1F*)inFile0->Get("h_genParKshortPt");
    TH1F* h_genParKshortPt1 = (TH1F*)inFile1->Get("h_genParKshortPt");

    TCanvas *c_genParKshortPt = new TCanvas("c_genParKshortPt", "",0,0,1280,925);
    c_genParKshortPt->cd();
    h_genParKshortPt0->SetLineColor(9);
    h_genParKshortPt1->SetLineColor(8);
    h_genParKshortPt0->Draw("");
    h_genParKshortPt1->Draw("same");

    outFile->cd();
    c_genParKshortPt->Write();

    TH1F* h_genParKshortEta0 = (TH1F*)inFile0->Get("h_genParKshortEta");
    TH1F* h_genParKshortEta1 = (TH1F*)inFile1->Get("h_genParKshortEta");

    TCanvas *c_genParKshortEta = new TCanvas("c_genParKshortEta", "",0,0,1280,925);
    c_genParKshortEta->cd();
    h_genParKshortEta0->SetLineColor(9);
    h_genParKshortEta1->SetLineColor(8);
    h_genParKshortEta0->Draw("");
    h_genParKshortEta1->Draw("same");

    outFile->cd();
    c_genParKshortEta->Write();

    TH1F* h_genParKshortPhi0 = (TH1F*)inFile0->Get("h_genParKshortPhi");
    TH1F* h_genParKshortPhi1 = (TH1F*)inFile1->Get("h_genParKshortPhi");

    TCanvas *c_genParKshortPhi = new TCanvas("c_genParKshortPhi", "",0,0,1280,925);
    c_genParKshortPhi->cd();
    h_genParKshortPhi0->SetLineColor(9);
    h_genParKshortPhi1->SetLineColor(8);
    h_genParKshortPhi0->Draw("");
    h_genParKshortPhi1->Draw("same");

    outFile->cd();
    c_genParKshortPhi->Write();

    TH1F* h_genParKshortE0 = (TH1F*)inFile0->Get("h_genParKshortE");
    TH1F* h_genParKshortE1 = (TH1F*)inFile1->Get("h_genParKshortE");

    TCanvas *c_genParKshortE = new TCanvas("c_genParKshortE", "",0,0,1280,925);
    c_genParKshortE->cd();
    h_genParKshortE0->SetLineColor(9);
    h_genParKshortE1->SetLineColor(8);
    h_genParKshortE0->Draw("");
    h_genParKshortE1->Draw("same");

    outFile->cd();
    c_genParKshortE->Write();

// pions
    TH1F* h_genParPionPt0 = (TH1F*)inFile0->Get("h_genParPionPt");
    TH1F* h_genParPionPt1 = (TH1F*)inFile1->Get("h_genParPionPt");

    TCanvas *c_genParPionPt = new TCanvas("c_genParPionPt", "",0,0,1280,925);
    c_genParPionPt->cd();
    h_genParPionPt0->SetLineColor(9);
    h_genParPionPt1->SetLineColor(8);
    h_genParPionPt0->Draw("");
    h_genParPionPt1->Draw("same");

    outFile->cd();
    c_genParPionPt->Write();

    TH1F* h_genParPionEta0 = (TH1F*)inFile0->Get("h_genParPionEta");
    TH1F* h_genParPionEta1 = (TH1F*)inFile1->Get("h_genParPionEta");

    TCanvas *c_genParPionEta = new TCanvas("c_genParPionEta", "",0,0,1280,925);
    c_genParPionEta->cd();
    h_genParPionEta0->SetLineColor(9);
    h_genParPionEta1->SetLineColor(8);
    h_genParPionEta0->Draw("");
    h_genParPionEta1->Draw("same");

    outFile->cd();
    c_genParPionEta->Write();

    TH1F* h_genParPionPhi0 = (TH1F*)inFile0->Get("h_genParPionPhi");
    TH1F* h_genParPionPhi1 = (TH1F*)inFile1->Get("h_genParPionPhi");

    TCanvas *c_genParPionPhi = new TCanvas("c_genParPionPhi", "",0,0,1280,925);
    c_genParPionPhi->cd();
    h_genParPionPhi0->SetLineColor(9);
    h_genParPionPhi1->SetLineColor(8);
    h_genParPionPhi0->Draw("");
    h_genParPionPhi1->Draw("same");

    outFile->cd();
    c_genParPionPhi->Write();

    TH1F* h_genParPionE0 = (TH1F*)inFile0->Get("h_genParPionE");
    TH1F* h_genParPionE1 = (TH1F*)inFile1->Get("h_genParPionE");

    TCanvas *c_genParPionE = new TCanvas("c_genParPionE", "",0,0,1280,925);
    c_genParPionE->cd();
    h_genParPionE0->SetLineColor(9);
    h_genParPionE1->SetLineColor(8);
    h_genParPionE0->Draw("");
    h_genParPionE1->Draw("same");

    outFile->cd();
    c_genParPionE->Write();

// charged pions
    TH1F* h_genParChargedPionPt0 = (TH1F*)inFile0->Get("h_genParChargedPionPt");
    TH1F* h_genParChargedPionPt1 = (TH1F*)inFile1->Get("h_genParChargedPionPt");

    TCanvas *c_genParChargedPionPt = new TCanvas("c_genParChargedPionPt", "",0,0,1280,925);
    c_genParChargedPionPt->cd();
    h_genParChargedPionPt0->SetLineColor(9);
    h_genParChargedPionPt1->SetLineColor(8);
    h_genParChargedPionPt0->Draw("");
    h_genParChargedPionPt1->Draw("same");

    outFile->cd();
    c_genParChargedPionPt->Write();

    TH1F* h_genParChargedPionEta0 = (TH1F*)inFile0->Get("h_genParChargedPionEta");
    TH1F* h_genParChargedPionEta1 = (TH1F*)inFile1->Get("h_genParChargedPionEta");

    TCanvas *c_genParChargedPionEta = new TCanvas("c_genParChargedPionEta", "",0,0,1280,925);
    c_genParChargedPionEta->cd();
    h_genParChargedPionEta0->SetLineColor(9);
    h_genParChargedPionEta1->SetLineColor(8);
    h_genParChargedPionEta0->Draw("");
    h_genParChargedPionEta1->Draw("same");

    outFile->cd();
    c_genParChargedPionEta->Write();

    TH1F* h_genParChargedPionPhi0 = (TH1F*)inFile0->Get("h_genParChargedPionPhi");
    TH1F* h_genParChargedPionPhi1 = (TH1F*)inFile1->Get("h_genParChargedPionPhi");

    TCanvas *c_genParChargedPionPhi = new TCanvas("c_genParChargedPionPhi", "",0,0,1280,925);
    c_genParChargedPionPhi->cd();
    h_genParChargedPionPhi0->SetLineColor(9);
    h_genParChargedPionPhi1->SetLineColor(8);
    h_genParChargedPionPhi0->Draw("");
    h_genParChargedPionPhi1->Draw("same");

    outFile->cd();
    c_genParChargedPionPhi->Write();

    TH1F* h_genParChargedPionE0 = (TH1F*)inFile0->Get("h_genParChargedPionE");
    TH1F* h_genParChargedPionE1 = (TH1F*)inFile1->Get("h_genParChargedPionE");

    TCanvas *c_genParChargedPionE = new TCanvas("c_genParChargedPionE", "",0,0,1280,925);
    c_genParChargedPionE->cd();
    h_genParChargedPionE0->SetLineColor(9);
    h_genParChargedPionE1->SetLineColor(8);
    h_genParChargedPionE0->Draw("");
    h_genParChargedPionE1->Draw("same");

    outFile->cd();
    c_genParChargedPionE->Write();

// neutral pions
    TH1F* h_genParNeutralPionPt0 = (TH1F*)inFile0->Get("h_genParNeutralPionPt");
    TH1F* h_genParNeutralPionPt1 = (TH1F*)inFile1->Get("h_genParNeutralPionPt");

    TCanvas *c_genParNeutralPionPt = new TCanvas("c_genParNeutralPionPt", "",0,0,1280,925);
    c_genParNeutralPionPt->cd();
    h_genParNeutralPionPt0->SetLineColor(9);
    h_genParNeutralPionPt1->SetLineColor(8);
    h_genParNeutralPionPt0->Draw("");
    h_genParNeutralPionPt1->Draw("same");

    outFile->cd();
    c_genParNeutralPionPt->Write();

    TH1F* h_genParNeutralPionEta0 = (TH1F*)inFile0->Get("h_genParNeutralPionEta");
    TH1F* h_genParNeutralPionEta1 = (TH1F*)inFile1->Get("h_genParNeutralPionEta");

    TCanvas *c_genParNeutralPionEta = new TCanvas("c_genParNeutralPionEta", "",0,0,1280,925);
    c_genParNeutralPionEta->cd();
    h_genParNeutralPionEta0->SetLineColor(9);
    h_genParNeutralPionEta1->SetLineColor(8);
    h_genParNeutralPionEta0->Draw("");
    h_genParNeutralPionEta1->Draw("same");

    outFile->cd();
    c_genParNeutralPionEta->Write();

    TH1F* h_genParNeutralPionPhi0 = (TH1F*)inFile0->Get("h_genParNeutralPionPhi");
    TH1F* h_genParNeutralPionPhi1 = (TH1F*)inFile1->Get("h_genParNeutralPionPhi");

    TCanvas *c_genParNeutralPionPhi = new TCanvas("c_genParNeutralPionPhi", "",0,0,1280,925);
    c_genParNeutralPionPhi->cd();
    h_genParNeutralPionPhi0->SetLineColor(9);
    h_genParNeutralPionPhi1->SetLineColor(8);
    h_genParNeutralPionPhi0->Draw("");
    h_genParNeutralPionPhi1->Draw("same");

    outFile->cd();
    c_genParNeutralPionPhi->Write();

    TH1F* h_genParNeutralPionE0 = (TH1F*)inFile0->Get("h_genParNeutralPionE");
    TH1F* h_genParNeutralPionE1 = (TH1F*)inFile1->Get("h_genParNeutralPionE");

    TCanvas *c_genParNeutralPionE = new TCanvas("c_genParNeutralPionE", "",0,0,1280,925);
    c_genParNeutralPionE->cd();
    h_genParNeutralPionE0->SetLineColor(9);
    h_genParNeutralPionE1->SetLineColor(8);
    h_genParNeutralPionE0->Draw("");
    h_genParNeutralPionE1->Draw("same");

    outFile->cd();
    c_genParNeutralPionE->Write();

// Packed Cands
    TH1F* h_packedCandDxy0 = (TH1F*)inFile0->Get("h_packedCandDxy");
    TH1F* h_packedCandDxy1 = (TH1F*)inFile1->Get("h_packedCandDxy");

    TCanvas *c_packedCandDxy = new TCanvas("c_packedCandDxy", "",0,0,1280,925);
    c_packedCandDxy->cd();
    h_packedCandDxy0->SetLineColor(9);
    h_packedCandDxy1->SetLineColor(8);
    h_packedCandDxy0->Draw("");
    h_packedCandDxy1->Draw("same");

    outFile->cd();
    c_packedCandDxy->Write();

    TH1F* h_packedCandDz0 = (TH1F*)inFile0->Get("h_packedCandDz");
    TH1F* h_packedCandDz1 = (TH1F*)inFile1->Get("h_packedCandDz");

    TCanvas *c_packedCandDz = new TCanvas("c_packedCandDz", "",0,0,1280,925);
    c_packedCandDz->cd();
    h_packedCandDz0->SetLineColor(9);
    h_packedCandDz1->SetLineColor(8);
    h_packedCandDz0->Draw("");
    h_packedCandDz1->Draw("same");

    outFile->cd();
    c_packedCandDz->Write();

    TH1F* h_packedCandVx0 = (TH1F*)inFile0->Get("h_packedCandVx");
    TH1F* h_packedCandVx1 = (TH1F*)inFile1->Get("h_packedCandVx");

    TCanvas *c_packedCandVx = new TCanvas("c_packedCandVx", "",0,0,1280,925);
    c_packedCandVx->cd();
    h_packedCandVx0->SetLineColor(9);
    h_packedCandVx1->SetLineColor(8);
    h_packedCandVx0->Draw("");
    h_packedCandVx1->Draw("same");

    outFile->cd();
    c_packedCandVx->Write();

    TH1F* h_packedCandVy0 = (TH1F*)inFile0->Get("h_packedCandVy");
    TH1F* h_packedCandVy1 = (TH1F*)inFile1->Get("h_packedCandVy");

    TCanvas *c_packedCandVy = new TCanvas("c_packedCandVy", "",0,0,1280,925);
    c_packedCandVy->cd();
    h_packedCandVy0->SetLineColor(9);
    h_packedCandVy1->SetLineColor(8);
    h_packedCandVy0->Draw("");
    h_packedCandVy1->Draw("same");

    outFile->cd();
    c_packedCandVy->Write();

    TH1F* h_packedCandVxy0 = (TH1F*)inFile0->Get("h_packedCandVxy");
    TH1F* h_packedCandVxy1 = (TH1F*)inFile1->Get("h_packedCandVxy");

    TCanvas *c_packedCandVxy = new TCanvas("c_packedCandVxy", "",0,0,1280,925);
    c_packedCandVxy->cd();
    h_packedCandVxy0->SetLineColor(9);
    h_packedCandVxy1->SetLineColor(8);
    h_packedCandVxy0->Draw("");
    h_packedCandVxy1->Draw("same");

    outFile->cd();
    c_packedCandVxy->Write();

    TH1F* h_packedCandVz0 = (TH1F*)inFile0->Get("h_packedCandVz");
    TH1F* h_packedCandVz1 = (TH1F*)inFile1->Get("h_packedCandVz");

    TCanvas *c_packedCandVz = new TCanvas("c_packedCandVz", "",0,0,1280,925);
    c_packedCandVz->cd();
    h_packedCandVz0->SetLineColor(9);
    h_packedCandVz1->SetLineColor(8);
    h_packedCandVz0->Draw("");
    h_packedCandVz1->Draw("same");

    outFile->cd();
    c_packedCandVz->Write();

// scalar
    TH1F* h_scalarDR0 = (TH1F*)inFile0->Get("h_scalarDR");
    TH1F* h_scalarDR1 = (TH1F*)inFile1->Get("h_scalarDR");

    TCanvas *c_scalarDR = new TCanvas("c_scalarDR", "",0,0,1280,925);
    c_scalarDR->cd();
    h_scalarDR0->SetLineColor(9);
    h_scalarDR1->SetLineColor(8);
    h_scalarDR0->Draw("");
    h_scalarDR1->Draw("same");

    outFile->cd();
    c_scalarDR->Write();

    TH1F* h_scalarDPhi0 = (TH1F*)inFile0->Get("h_scalarDPhi");
    TH1F* h_scalarDPhi1 = (TH1F*)inFile1->Get("h_scalarDPhi");

    TCanvas *c_scalarDPhi = new TCanvas("c_scalarDPhi", "",0,0,1280,925);
    c_scalarDPhi->cd();
    h_scalarDPhi0->SetLineColor(9);
    h_scalarDPhi1->SetLineColor(8);
    h_scalarDPhi0->Draw("");
    h_scalarDPhi1->Draw("same");

    outFile->cd();
    c_scalarDPhi->Write();

    TH1F* h_scalarDrEtaPhi0 = (TH1F*)inFile0->Get("h_scalarDrEtaPhi");
    TH1F* h_scalarDrEtaPhi1 = (TH1F*)inFile1->Get("h_scalarDrEtaPhi");

    TCanvas *c_scalarDrEtaPhi = new TCanvas("c_scalarDrEtaPhi", "",0,0,1280,925);
    c_scalarDrEtaPhi->cd();
    h_scalarDrEtaPhi0->SetLineColor(9);
    h_scalarDrEtaPhi1->SetLineColor(8);
    h_scalarDrEtaPhi0->Draw("");
    h_scalarDrEtaPhi1->Draw("same");

    outFile->cd();
    c_scalarDrEtaPhi->Write();
 
// Kaon
    TH1F* h_genParKaonDR0 = (TH1F*)inFile0->Get("h_genParKaonDR");
    TH1F* h_genParKaonDR1 = (TH1F*)inFile1->Get("h_genParKaonDR");

    TCanvas *c_genParKaonDR = new TCanvas("c_genParKaonDR", "",0,0,1280,925);
    c_genParKaonDR->cd();
    h_genParKaonDR0->SetLineColor(9);
    h_genParKaonDR1->SetLineColor(8);
    h_genParKaonDR0->Draw("");
    h_genParKaonDR1->Draw("same");

    outFile->cd();
    c_genParKaonDR->Write();

    TH1F* h_genParKaonDPhi0 = (TH1F*)inFile0->Get("h_genParKaonDPhi");
    TH1F* h_genParKaonDPhi1 = (TH1F*)inFile1->Get("h_genParKaonDPhi");

    TCanvas *c_genParKaonDPhi = new TCanvas("c_genParKaonDPhi", "",0,0,1280,925);
    c_genParKaonDPhi->cd();
    h_genParKaonDPhi0->SetLineColor(9);
    h_genParKaonDPhi1->SetLineColor(8);
    h_genParKaonDPhi0->Draw("");
    h_genParKaonDPhi1->Draw("same");

    outFile->cd();
    c_genParKaonDPhi->Write();

    TH1F* h_genParKaonDrEtaPhi0 = (TH1F*)inFile0->Get("h_genParKaonDrEtaPhi");
    TH1F* h_genParKaonDrEtaPhi1 = (TH1F*)inFile1->Get("h_genParKaonDrEtaPhi");

    TCanvas *c_genParKaonDrEtaPhi = new TCanvas("c_genParKaonDrEtaPhi", "",0,0,1280,925);
    c_genParKaonDrEtaPhi->cd();
    h_genParKaonDrEtaPhi0->SetLineColor(9);
    h_genParKaonDrEtaPhi1->SetLineColor(8);
    h_genParKaonDrEtaPhi0->Draw("");
    h_genParKaonDrEtaPhi1->Draw("same");

    outFile->cd();
    c_genParKaonDrEtaPhi->Write();

    TH1F* h_genJetKaonDR0 = (TH1F*)inFile0->Get("h_genJetKaonDR");
    TH1F* h_genJetKaonDR1 = (TH1F*)inFile1->Get("h_genJetKaonDR");

    TCanvas *c_genJetKaonDR = new TCanvas("c_genJetKaonDR", "",0,0,1280,925);
    c_genJetKaonDR->cd();
    h_genJetKaonDR0->SetLineColor(9);
    h_genJetKaonDR1->SetLineColor(8);
    h_genJetKaonDR0->Draw("");
    h_genJetKaonDR1->Draw("same");

    outFile->cd();
    c_genJetKaonDR->Write();

    TH1F* h_genJetKaonDPhi0 = (TH1F*)inFile0->Get("h_genJetKaonDPhi");
    TH1F* h_genJetKaonDPhi1 = (TH1F*)inFile1->Get("h_genJetKaonDPhi");

    TCanvas *c_genJetKaonDPhi = new TCanvas("c_genJetKaonDPhi", "",0,0,1280,925);
    c_genJetKaonDPhi->cd();
    h_genJetKaonDPhi0->SetLineColor(9);
    h_genJetKaonDPhi1->SetLineColor(8);
    h_genJetKaonDPhi0->Draw("");
    h_genJetKaonDPhi1->Draw("same");

    outFile->cd();
    c_genJetKaonDPhi->Write();

    TH1F* h_genJetKaonDrEtaPhi0 = (TH1F*)inFile0->Get("h_genJetKaonDrEtaPhi");
    TH1F* h_genJetKaonDrEtaPhi1 = (TH1F*)inFile1->Get("h_genJetKaonDrEtaPhi");

    TCanvas *c_genJetKaonDrEtaPhi = new TCanvas("c_genJetKaonDrEtaPhi", "",0,0,1280,925);
    c_genJetKaonDrEtaPhi->cd();
    h_genJetKaonDrEtaPhi0->SetLineColor(9);
    h_genJetKaonDrEtaPhi1->SetLineColor(8);
    h_genJetKaonDrEtaPhi0->Draw("");
    h_genJetKaonDrEtaPhi1->Draw("same");

    outFile->cd();
    c_genJetKaonDrEtaPhi->Write();

// Kshorts
    TH1F* h_genParKshortDR0 = (TH1F*)inFile0->Get("h_genParKshortDR");
    TH1F* h_genParKshortDR1 = (TH1F*)inFile1->Get("h_genParKshortDR");

    TCanvas *c_genParKshortDR = new TCanvas("c_genParKshortDR", "",0,0,1280,925);
    c_genParKshortDR->cd();
    h_genParKshortDR0->SetLineColor(9);
    h_genParKshortDR1->SetLineColor(8);
    h_genParKshortDR0->Draw("");
    h_genParKshortDR1->Draw("same");

    outFile->cd();
    c_genParKshortDR->Write();

    TH1F* h_genParKshortDPhi0 = (TH1F*)inFile0->Get("h_genParKshortDPhi");
    TH1F* h_genParKshortDPhi1 = (TH1F*)inFile1->Get("h_genParKshortDPhi");

    TCanvas *c_genParKshortDPhi = new TCanvas("c_genParKshortDPhi", "",0,0,1280,925);
    c_genParKshortDPhi->cd();
    h_genParKshortDPhi0->SetLineColor(9);
    h_genParKshortDPhi1->SetLineColor(8);
    h_genParKshortDPhi0->Draw("");
    h_genParKshortDPhi1->Draw("same");

    outFile->cd();
    c_genParKshortDPhi->Write();

    TH1F* h_genParKshortDrEtaPhi0 = (TH1F*)inFile0->Get("h_genParKshortDrEtaPhi");
    TH1F* h_genParKshortDrEtaPhi1 = (TH1F*)inFile1->Get("h_genParKshortDrEtaPhi");

    TCanvas *c_genParKshortDrEtaPhi = new TCanvas("c_genParKshortDrEtaPhi", "",0,0,1280,925);
    c_genParKshortDrEtaPhi->cd();
    h_genParKshortDrEtaPhi0->SetLineColor(9);
    h_genParKshortDrEtaPhi1->SetLineColor(8);
    h_genParKshortDrEtaPhi0->Draw("");
    h_genParKshortDrEtaPhi1->Draw("same");

    outFile->cd();
    c_genParKshortDrEtaPhi->Write();

    TH1F* h_genJetKshortDR0 = (TH1F*)inFile0->Get("h_genJetKshortDR");
    TH1F* h_genJetKshortDR1 = (TH1F*)inFile1->Get("h_genJetKshortDR");

    TCanvas *c_genJetKshortDR = new TCanvas("c_genJetKshortDR", "",0,0,1280,925);
    c_genJetKshortDR->cd();
    h_genJetKshortDR0->SetLineColor(9);
    h_genJetKshortDR1->SetLineColor(8);
    h_genJetKshortDR0->Draw("");
    h_genJetKshortDR1->Draw("same");

    outFile->cd();
    c_genJetKshortDR->Write();

    TH1F* h_genJetKshortDPhi0 = (TH1F*)inFile0->Get("h_genJetKshortDPhi");
    TH1F* h_genJetKshortDPhi1 = (TH1F*)inFile1->Get("h_genJetKshortDPhi");

    TCanvas *c_genJetKshortDPhi = new TCanvas("c_genJetKshortDPhi", "",0,0,1280,925);
    c_genJetKshortDPhi->cd();
    h_genJetKshortDPhi0->SetLineColor(9);
    h_genJetKshortDPhi1->SetLineColor(8);
    h_genJetKshortDPhi0->Draw("");
    h_genJetKshortDPhi1->Draw("same");

    outFile->cd();
    c_genJetKshortDPhi->Write();

    TH1F* h_genJetKshortDrEtaPhi0 = (TH1F*)inFile0->Get("h_genJetKshortDrEtaPhi");
    TH1F* h_genJetKshortDrEtaPhi1 = (TH1F*)inFile1->Get("h_genJetKshortDrEtaPhi");

    TCanvas *c_genJetKshortDrEtaPhi = new TCanvas("c_genJetKshortDrEtaPhi", "",0,0,1280,925);
    c_genJetKshortDrEtaPhi->cd();
    h_genJetKshortDrEtaPhi0->SetLineColor(9);
    h_genJetKshortDrEtaPhi1->SetLineColor(8);
    h_genJetKshortDrEtaPhi0->Draw("");
    h_genJetKshortDrEtaPhi1->Draw("same");

    outFile->cd();
    c_genJetKshortDrEtaPhi->Write();

// genJets
    TH1F* h_genJetScalarSumInvMass0 = (TH1F*)inFile0->Get("h_genJetScalarSumInvMass");
    TH1F* h_genJetScalarSumInvMass1 = (TH1F*)inFile1->Get("h_genJetScalarSumInvMass");

    TCanvas *c_genJetScalarSumInvMass = new TCanvas("c_genJetScalarSumInvMass", "",0,0,1280,925);
    c_genJetScalarSumInvMass->cd();
    h_genJetScalarSumInvMass0->SetLineColor(9);
    h_genJetScalarSumInvMass1->SetLineColor(8);
    h_genJetScalarSumInvMass0->Draw("");
    h_genJetScalarSumInvMass1->Draw("same");

    outFile->cd();
    c_genJetScalarSumInvMass->Write();

    TH1F* h_gen2JetScalarSumInvMass0 = (TH1F*)inFile0->Get("h_gen2JetScalarSumInvMass");
    TH1F* h_gen2JetScalarSumInvMass1 = (TH1F*)inFile1->Get("h_gen2JetScalarSumInvMass");

    TCanvas *c_gen2JetScalarSumInvMass = new TCanvas("c_gen2JetScalarSumInvMass", "",0,0,1280,925);
    c_gen2JetScalarSumInvMass->cd();
    h_gen2JetScalarSumInvMass0->SetLineColor(9);
    h_gen2JetScalarSumInvMass1->SetLineColor(8);
    h_gen2JetScalarSumInvMass0->Draw("");
    h_gen2JetScalarSumInvMass1->Draw("same");

    outFile->cd();
    c_gen2JetScalarSumInvMass->Write();

    TH1F* h_genJetScalarInvMass0 = (TH1F*)inFile0->Get("h_genJetScalarInvMass");
    TH1F* h_genJetScalarInvMass1 = (TH1F*)inFile1->Get("h_genJetScalarInvMass");

    TCanvas *c_genJetScalarInvMass = new TCanvas("c_genJetScalarInvMass", "",0,0,1280,925);
    c_genJetScalarInvMass->cd();
    h_genJetScalarInvMass0->SetLineColor(9);
    h_genJetScalarInvMass1->SetLineColor(8);
    h_genJetScalarInvMass0->Draw("");
    h_genJetScalarInvMass1->Draw("same");

    outFile->cd();
    c_genJetScalarInvMass->Write();

    TH1F* h_genJetScalarPt0 = (TH1F*)inFile0->Get("h_genJetScalarPt");
    TH1F* h_genJetScalarPt1 = (TH1F*)inFile1->Get("h_genJetScalarPt");

    TCanvas *c_genJetScalarPt = new TCanvas("c_genJetScalarPt", "",0,0,1280,925);
    c_genJetScalarPt->cd();
    h_genJetScalarPt0->SetLineColor(9);
    h_genJetScalarPt1->SetLineColor(8);
    h_genJetScalarPt0->Draw("");
    h_genJetScalarPt1->Draw("same");

    outFile->cd();
    c_genJetScalarPt->Write();

    TH1F* h_genJetScalarEta0 = (TH1F*)inFile0->Get("h_genJetScalarEta");
    TH1F* h_genJetScalarEta1 = (TH1F*)inFile1->Get("h_genJetScalarEta");

    TCanvas *c_genJetScalarEta = new TCanvas("c_genJetScalarEta", "",0,0,1280,925);
    c_genJetScalarEta->cd();
    h_genJetScalarEta0->SetLineColor(9);
    h_genJetScalarEta1->SetLineColor(8);
    h_genJetScalarEta0->Draw("");
    h_genJetScalarEta1->Draw("same");

    outFile->cd();
    c_genJetScalarEta->Write();

    TH1F* h_genJetMass0 = (TH1F*)inFile0->Get("h_genJetMass");
    TH1F* h_genJetMass1 = (TH1F*)inFile1->Get("h_genJetMass");

    TCanvas *c_genJetMass = new TCanvas("c_genJetMass", "",0,0,1280,925);
    c_genJetMass->cd();
    h_genJetMass0->SetLineColor(9);
    h_genJetMass1->SetLineColor(8);
    h_genJetMass0->Draw("");
    h_genJetMass1->Draw("same");

    outFile->cd();
    c_genJetMass->Write();

    TH1F* h_genJet1InvMass0 = (TH1F*)inFile0->Get("h_genJet1InvMass");
    TH1F* h_genJet1InvMass1 = (TH1F*)inFile1->Get("h_genJet1InvMass");

    TCanvas *c_genJet1InvMass = new TCanvas("c_genJet1InvMass", "",0,0,1280,925);
    c_genJet1InvMass->cd();
    h_genJet1InvMass0->SetLineColor(9);
    h_genJet1InvMass1->SetLineColor(8);
    h_genJet1InvMass0->Draw("");
    h_genJet1InvMass1->Draw("same");

    outFile->cd();
    c_genJet1InvMass->Write();

    TH1F* h_genJet1Mass0 = (TH1F*)inFile0->Get("h_genJet1Mass");
    TH1F* h_genJet1Mass1 = (TH1F*)inFile1->Get("h_genJet1Mass");

    TCanvas *c_genJet1Mass = new TCanvas("c_genJet1Mass", "",0,0,1280,925);
    c_genJet1Mass->cd();
    h_genJet1Mass0->SetLineColor(9);
    h_genJet1Mass1->SetLineColor(8);
    h_genJet1Mass0->Draw("");
    h_genJet1Mass1->Draw("same");

    outFile->cd();
    c_genJet1Mass->Write();

    TH1F* h_genJet2InvMass0 = (TH1F*)inFile0->Get("h_genJet2InvMass");
    TH1F* h_genJet2InvMass1 = (TH1F*)inFile1->Get("h_genJet2InvMass");

    TCanvas *c_genJet2InvMass = new TCanvas("c_genJet2InvMass", "",0,0,1280,925);
    c_genJet2InvMass->cd();
    h_genJet2InvMass0->SetLineColor(9);
    h_genJet2InvMass1->SetLineColor(8);
    h_genJet2InvMass0->Draw("");
    h_genJet2InvMass1->Draw("same");

    outFile->cd();
    c_genJet2InvMass->Write();

    TH1F* h_genJet2Mass0 = (TH1F*)inFile0->Get("h_genJet2Mass");
    TH1F* h_genJet2Mass1 = (TH1F*)inFile1->Get("h_genJet2Mass");

    TCanvas *c_genJet2Mass = new TCanvas("c_genJet2Mass", "",0,0,1280,925);
    c_genJet2Mass->cd();
    h_genJet2Mass0->SetLineColor(9);
    h_genJet2Mass1->SetLineColor(8);
    h_genJet2Mass0->Draw("");
    h_genJet2Mass1->Draw("same");

    outFile->cd();
    c_genJet2Mass->Write();

// genJetsPions

    TH1F* h_genJetPionInvMass0 = (TH1F*)inFile0->Get("h_genJetPionInvMass");
    TH1F* h_genJetPionInvMass1 = (TH1F*)inFile1->Get("h_genJetPionInvMass");

    TCanvas *c_genJetPionInvMass = new TCanvas("c_genJetPionInvMass", "",0,0,1280,925);
    c_genJetPionInvMass->cd();
    h_genJetPionInvMass0->SetLineColor(9);
    h_genJetPionInvMass1->SetLineColor(8);
    h_genJetPionInvMass0->Draw("");
    h_genJetPionInvMass1->Draw("same");

    outFile->cd();
    c_genJetPionInvMass->Write();

    TH1F* h_genJetPionScalarInvMass0 = (TH1F*)inFile0->Get("h_genJetPionScalarInvMass");
    TH1F* h_genJetPionScalarInvMass1 = (TH1F*)inFile1->Get("h_genJetPionScalarInvMass");

    TCanvas *c_genJetPionScalarInvMass = new TCanvas("c_genJetPionScalarInvMass", "",0,0,1280,925);
    c_genJetPionScalarInvMass->cd();
    h_genJetPionScalarInvMass0->SetLineColor(9);
    h_genJetPionScalarInvMass1->SetLineColor(8);
    h_genJetPionScalarInvMass0->Draw("");
    h_genJetPionScalarInvMass1->Draw("same");

    outFile->cd();
    c_genJetPionScalarInvMass->Write();

    TH1F* h_genJetPionEta0 = (TH1F*)inFile0->Get("h_genJetPionEta");
    TH1F* h_genJetPionEta1 = (TH1F*)inFile1->Get("h_genJetPionEta");

    TCanvas *c_genJetPionEta = new TCanvas("c_genJetPionEta", "",0,0,1280,925);
    c_genJetPionEta->cd();
    h_genJetPionEta0->SetLineColor(9);
    h_genJetPionEta1->SetLineColor(8);
    h_genJetPionEta0->Draw("");
    h_genJetPionEta1->Draw("same");

    outFile->cd();
    c_genJetPionEta->Write();

// genJetsKaons

    TH1F* h_genJetKaonInvMass0 = (TH1F*)inFile0->Get("h_genJetKaonInvMass");
    TH1F* h_genJetKaonInvMass1 = (TH1F*)inFile1->Get("h_genJetKaonInvMass");

    TCanvas *c_genJetKaonInvMass = new TCanvas("c_genJetKaonInvMass", "",0,0,1280,925);
    c_genJetKaonInvMass->cd();
    h_genJetKaonInvMass0->SetLineColor(9);
    h_genJetKaonInvMass1->SetLineColor(8);
    h_genJetKaonInvMass0->Draw("");
    h_genJetKaonInvMass1->Draw("same");

    outFile->cd();
    c_genJetKaonInvMass->Write();

    TH1F* h_genJetKaonScalarInvMass0 = (TH1F*)inFile0->Get("h_genJetKaonScalarInvMass");
    TH1F* h_genJetKaonScalarInvMass1 = (TH1F*)inFile1->Get("h_genJetKaonScalarInvMass");

    TCanvas *c_genJetKaonScalarInvMass = new TCanvas("c_genJetKaonScalarInvMass", "",0,0,1280,925);
    c_genJetKaonScalarInvMass->cd();
    h_genJetKaonScalarInvMass0->SetLineColor(9);
    h_genJetKaonScalarInvMass1->SetLineColor(8);
    h_genJetKaonScalarInvMass0->Draw("");
    h_genJetKaonScalarInvMass1->Draw("same");

    outFile->cd();
    c_genJetKaonScalarInvMass->Write();

    TH1F* h_genJetKaonEta0 = (TH1F*)inFile0->Get("h_genJetKaonEta");
    TH1F* h_genJetKaonEta1 = (TH1F*)inFile1->Get("h_genJetKaonEta");

    TCanvas *c_genJetKaonEta = new TCanvas("c_genJetKaonEta", "",0,0,1280,925);
    c_genJetKaonEta->cd();
    h_genJetKaonEta0->SetLineColor(9);
    h_genJetKaonEta1->SetLineColor(8);
    h_genJetKaonEta0->Draw("");
    h_genJetKaonEta1->Draw("same");

    outFile->cd();
    c_genJetKaonEta->Write();

/////////

    outFile->Close();
}
