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
//#include "TPRegexp.h"
const int nvar = 3;
const int ini = 1;
const int fin = 2;
const char *fname[nvar] = {"plots/kinematicDistributions/output_%s.root", "plots/kinematicDistributions/output_%s.root", "plots/kinematicDistributions/output_%s.root"};
const char *cent[nvar] = {"HtoSS_MS2_ctau0", "HtoSS_MS2_ctau1", "HtoSS_MS2_ctau10"};
int m_sty[nvar] = {26,4,5};
int m_col[nvar] = {9,8,2};
int l_col[nvar] = {9,8,2};
const char *leg_ent[nvar] = {"c_{#tau} = 0 mm","c_{#tau} = 1 mm","c_{#tau} = 10 mm"};

void SetStyle(Bool_t graypalette=kTRUE);
void KinematicsPlotMacro() {
    SetStyle();
    TCanvas *Canvas_muTrigPt = new TCanvas("Canvas_muTrigPt", "Canvas_muTrigPt",0,0,1280,925);
    
    TLegend *leg_muTrigPt = new TLegend(0.20,0.70,0.27,0.90,NULL,"brNDC");
    leg_muTrigPt->SetFillStyle(1001);
    leg_muTrigPt->SetLineColor(1);
    leg_muTrigPt->SetLineStyle(1);
    leg_muTrigPt->SetLineWidth(1);
    
    for (int i=ini;i<fin;i++) {
        TFile* infile =  new TFile (Form(fname[i],cent[i]));
        // pT
        TH1F* h_leadingMuonPt_muTrigPt = (TH1F*)infile->Get("h_leadingMuonPt_muTrig");
        //Canvas_muTrigPt->Range(-0.4860759,-0.2025316,2.551899,1.063291);
        Canvas_muTrigPt->cd();
        
        h_leadingMuonPt_muTrigPt->SetTitle("");
        h_leadingMuonPt_muTrigPt->GetXaxis()->SetTitle("leading muon p_{T} [GeV]");
        //h_leadingMuonPt_muTrigPt->GetYaxis()->SetRangeUser(0.0, 1.0);
        h_leadingMuonPt_muTrigPt->SetLabelFont(42,"xy");
        h_leadingMuonPt_muTrigPt->SetTitleFont(42,"xy");
        h_leadingMuonPt_muTrigPt->SetLabelSize(0.035,"y");
        h_leadingMuonPt_muTrigPt->SetTitleSize(0.035,"y");
        h_leadingMuonPt_muTrigPt->SetTitleOffset(1.3,"xy");
        h_leadingMuonPt_muTrigPt->SetLabelSize(0.05,"x");
        h_leadingMuonPt_muTrigPt->SetTitleSize(0.05,"x");
        
        h_leadingMuonPt_muTrigPt->GetYaxis()->SetTitle("Passing #mu trigger");
        h_leadingMuonPt_muTrigPt->SetLineColor(l_col[i]);
        h_leadingMuonPt_muTrigPt->SetMarkerColor(m_col[i]);
        h_leadingMuonPt_muTrigPt->SetMarkerStyle(m_sty[i]);
        h_leadingMuonPt_muTrigPt->Draw("HIST p same");
        //h_leadingMuonPt_muTrigPt->Draw("HIST p");

        leg_muTrigPt->AddEntry(h_leadingMuonPt_muTrigPt, leg_ent[i], "P");
        leg_muTrigPt->Draw("HIST");
        
        Canvas_muTrigPt->Modified();
    }
    Canvas_muTrigPt->cd();
    Canvas_muTrigPt->SetSelected(Canvas_muTrigPt);
    Canvas_muTrigPt->SaveAs( "muTrig_pT.pdf" );
    Canvas_muTrigPt->SaveAs( "muTrig_pT.png" );
    

}
void SetStyle(Bool_t graypalette) {
    cout << "Setting style!" << endl;
    //gStyle->Reset("Plain");
    //gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    if(graypalette) gStyle->SetPalette(8,0);
    else gStyle->SetPalette(1);
    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(2);
    gStyle->SetFrameLineWidth(1);
    gStyle->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadGridY(true);
    gStyle->SetLabelFont(42,"xy");
    gStyle->SetTitleFont(42,"xy");
    gStyle->SetLabelSize(0.06,"x");
    gStyle->SetTitleSize(0.06,"x");
    gStyle->SetTitleSize(0.05,"pad");
    gStyle->SetTitleOffset(1.,"y");
    gStyle->SetTitleOffset(1.2,"x");
    gStyle->SetLabelSize(0.06,"y");
    gStyle->SetTitleSize(0.06,"y");
    gStyle->SetNdivisions(510,"xy");
    gStyle->SetMarkerSize(1.2);
    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendTextSize(0.04);
    gStyle->SetLegendFillColor(0);
    gStyle->SetLegendFont(42);
    /*
    gStyle->SetHistLineWidth(1);
    gStyle->SetHistLineColor(kRed);
    gStyle->SetFuncWidth(2);
    gStyle->SetFuncColor(kBlack);
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.045,"xyz");
    gStyle->SetLabelOffdset(0.015,"y");
    gStyle->SetLabelOffdset(0.01,"x");
    gStyle->SetLabelColor(kBlack,"xyz");
    gStyle->SetTitleSize(0.05,"xyz");
    gStyle->SetTitleOffdset(1.81,"y");
    gStyle->SetTitleOffdset(1.25,"x");
    gStyle->SetTitleFillColor(kWhite);
    gStyle->SetTextSizePixels(26);
    gStyle->SetTextFont(42);
    gStyle->SetTickLength(0.03,"X");
    gStyle->SetTickLength(0.03,"Y"); 
    gStyle->SetLegendFillColor(kWhite);
    gStyle->SetLegendFont(42);*/
}
