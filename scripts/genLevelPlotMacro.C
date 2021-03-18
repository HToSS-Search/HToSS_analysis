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


    TFile* inFile0  =  new TFile ("plots/genLevelAnalysis/HtoSS_MS2_ctau0_current.root", "READ");
    TFile* inFile1  =  new TFile ("plots/genLevelAnalysis/HtoSS_MS2_ctau0_test.root", "READ");

    TFile* outFile  =  new TFile ("./output.root", "RECREATE");

    TH1F* h_genParKaonPt0  = (TH1F*)inFile0->Get("h_genParKaonPt");
    TH1F* h_genParKaonPt1  = (TH1F*)inFile1->Get("h_genParKaonPt");

    TCanvas *c_genParKaonPt = new TCanvas("c_genParKaonPt", "",0,0,1280,925);
    c_genParKaonPt->cd();
    h_genParKaonPt0->SetLineColor(9);
    h_genParKaonPt1->SetLineColor(8);
    h_genParKaonPt0->Draw("");
    h_genParKaonPt1->Draw("same");

    outFile->cd();
    c_genParKaonPt->Write();

    TH1F* h_genParKaonEta0  = (TH1F*)inFile0->Get("h_genParKaonEta");
    TH1F* h_genParKaonEta1  = (TH1F*)inFile1->Get("h_genParKaonEta");

    TCanvas *c_genParKaonEta = new TCanvas("c_genParKaonEta", "",0,0,1280,925);
    c_genParKaonEta->cd();
    h_genParKaonEta0->SetLineColor(9);
    h_genParKaonEta1->SetLineColor(8);
    h_genParKaonEta0->Draw("");
    h_genParKaonEta1->Draw("same");

    outFile->cd();
    c_genParKaonEta->Write();


/*
    h_genParKaonPhi->Write();
    h_genParKaonE->Write();
    h_genParChargedKaonPt->Write();
    h_genParChargedKaonEta->Write();
    h_genParChargedKaonPhi->Write();
    h_genParChargedKaonE->Write();
    h_genParNeutralKaonPt->Write();
    h_genParNeutralKaonEta->Write();
    h_genParNeutralKaonPhi->Write();
    h_genParNeutralKaonE->Write();
    h_genParKshortPt->Write();
    h_genParKshortEta->Write();
    h_genParKshortPhi->Write();
    h_genParKshortE->Write();
    h_genParKlongPt->Write();
    h_genParKlongEta->Write();
    h_genParKlongPhi->Write();
    h_genParKlongE->Write();
    h_genParPionPt->Write();
    h_genParPionEta->Write();
    h_genParPionPhi->Write();
    h_genParPionE->Write();
    h_genParChargedPionPt->Write();
    h_genParChargedPionEta->Write();
    h_genParChargedPionPhi->Write();
    h_genParChargedPionE->Write();
    h_genParNeutralPionPt->Write();
    h_genParNeutralPionEta->Write();
    h_genParNeutralPionPhi->Write();
    h_genParNeutralPionE->Write();

    h_packedCandDxy->Write();
    h_packedCandDz->Write();
    h_packedCandVx->Write();
    h_packedCandVy->Write();
    h_packedCandVxy->Write();
    h_packedCandVz->Write();

    h_scalarDR->Write();
    h_scalarDPhi->Write();
    h_scalarDrEtaPhi->Write();

    h_genParKaonDR->Write();
    h_genParKaonDPhi->Write();
    h_genParKaonDrEtaPhi->Write();
    h_genJetKaonDR->Write();
    h_genJetKaonDPhi->Write();
    h_genJetKaonDrEtaPhi->Write();

    h_genParKshortDR->Write();
    h_genParKshortDPhi->Write();
    h_genParKshortDrEtaPhi->Write();
    h_genJetKshortDR->Write();
    h_genJetKshortDPhi->Write();
    h_genJetKshortDrEtaPhi->Write();


    h_genJetScalarSumInvMass->Write();
    h_gen2JetScalarSumInvMass->Write();
    h_gen2JetMinScalarSumInvMass->Write();
    h_genJetScalarInvMass->Write();
    h_genJetScalarPt->Write();
    h_genJetScalarEta->Write();

    h_genJetPionInvMass->Write();
    h_genJetPionScalarInvMass->Write();
    h_genJetPionPt->Write();
    h_genJetPionEta->Write();
    h_genJetKaonInvMass->Write();
    h_genJetKaonScalarInvMass->Write();
    h_genJetKaonPt->Write();
    h_genJetKaonEta->Write();

    h_genJetMass->Write();
    h_genJet1InvMass->Write();
    h_genJet1Mass->Write();
    h_genJet2InvMass->Write();
    h_genJet2Mass->Write();
*/

    outFile->Close();
}
