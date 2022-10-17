#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TStyle.h"

const char *lifetime[4]={"ctau0","ctau1","ctau10","ctau100"}; const int nlifetime=4;
const char *label_leg[4]={"c#tau = 0 mm", "c#tau = 1 mm", "c#tau = 10 mm", "c#tau = 100 mm"};
const Int_t col[4]={2,3,4,6}; const Int_t sty[4]={2,5,22,33};
const char *trig[3]={"muTrig","L2muTrig","muOrL2muTrig"}; const int ntrig=3;
const char *label_trig[3]={"#mu trigger efficiency","L2/no vtx #mu#mu trigger efficiency","L2/no vtx #mu#mu OR #mu trigger efficiency"};
//const char *trigger[5]={"HLT_DoubleL2Mu23NoVtx_2Cha", "HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed", "HLT_DoubleL2Mu10NoVtx_2Cha_PromptL3Mu0Veto", "HLT_DoubleL3Mu16_10NoVtx_Displaced", "HLT_ORTrig"}; const int ntrigger=5;
const char *vars[2]={"Pt","Eta"}; const int nvars=2;
TString label_vars[nvars]={"p_{T} [GeV]", "#eta"};
const double xmin[nvars]={0, -3.14};
const double xmax[nvars]={120, 3.14};
const int rebin_fact[nvars]={1, 1};
const char *mu[2]={"leadingMuon","subLeadingMuon"}; const int nmu=2;
TString label_mu[2]={"leading #mu","subleading #mu"};

void SetStyle(Bool_t graypalette=kTRUE);
void  SignalEfficiency(bool saveplot = true){
  SetStyle();
  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2();
  //TH1F *h_num, *h_den;
  TH1F *h_temp;
  TCanvas* c_jkl[nvars][ntrig][nmu];
  //TGraphAsymmErrors* gph[nlifetime][nvars][ntrigger][nmu];
  //TGraphAsymmErrors* gph = new TGraphAsymmErrors;
  //TH1F *gph[nlifetime][nvars][ntrigger][nmu];
  //TMultiGraph *mg[nvars][ntrigger][nmu];
  TLegend *leg[nvars];
  TLegendEntry *leg_ent[nvars];
  
    
    for (int j=0;j<nvars;j++) {
      leg[j] = new TLegend(0.18,0.75,0.32,0.9);
      /*if (j==1) {
        leg[j] = new TLegend(0.75,0.75,0.9,0.9);
        //gStyle->SetPadGridY(false);
      }*/
      //leg->SetFillColor(19);
      leg[j]->SetFillStyle(4100);
      leg[j]->SetTextSize(0.030);
			leg_ent[j] = new TLegendEntry;
			leg_ent[j]->SetMarkerSize(2);
      for (int k=0;k<ntrig;k++) {
        for (int l=0;l<nmu;l++) {
          c_jkl[j][k][l]=new TCanvas("","",800,600);
          for (int i=0;i<nlifetime;i++) {
            //gph[i][j][k][l]=new TH1F;
            //gph[i][j][k][l]=new TGraphAsymmErrors;
          }
          //mg[j][k][l]=new TMultiGraph;
        }
      }
    }
  
  TPaveLabel *labellumi = new TPaveLabel(0.74,0.9,1.0,1.05,"UL2018MC, #sqrt{s} = 13 TeV","brNDC");
  labellumi->SetFillColor(0);
  labellumi->SetTextColor(kBlack);
  labellumi->SetFillStyle(0);
  labellumi->SetBorderSize(0);
  labellumi->SetTextSize(0.2);
  labellumi->SetTextAlign(12);
  
  TPaveLabel *labellumi_extra = new TPaveLabel(0.135,0.9,0.4,1.05,"#it{CMS Simulation, Work in Progress}","brNDC");
  labellumi_extra->SetFillColor(0);
  labellumi_extra->SetTextColor(kBlack);
  labellumi_extra->SetFillStyle(0);
  labellumi_extra->SetBorderSize(0);
  labellumi_extra->SetTextSize(0.2);
  labellumi_extra->SetTextAlign(12);
  
  TH1F *h_eff;
  
  for (int i=nlifetime-1;i>=0;i--) { //only because the number of bins is very low in small lifetime dxy, causes axis restriction
    TFile* f_in = TFile::Open(Form("/user/sdansana/CMSSW_10_6_27/src/HToSS_analysis/plots/triggerDistributions/output_HtoSS_MS2_%s.root",lifetime[i]),"Read");
    for (int k=0;k<ntrig;k++) {
      for (int j=0;j<nvars;j++) {
        for (int l=0;l<nmu;l++) {
          c_jkl[j][k][l]->cd();
          cout<<"Histogram name - "<<Form("h_%s%s_truth_%s", mu[l], vars[j], trig[k])<<endl;
          h_temp=(TH1F*)f_in->Get(Form("h_%s%s_truth_%s", mu[l], vars[j], trig[k]));
					h_temp->SetTitle("");
          h_temp->Rebin(rebin_fact[j]);
          h_temp->SetMarkerColor(col[i]);
          h_temp->SetMarkerStyle(sty[i]);
          h_temp->GetXaxis()->SetRangeUser(xmin[j],xmax[j]);
          //mg[j][k][l]->Add(gph[i][j][k][l]);
          TString title = label_mu[l] + " Gen " + label_vars[j];
          //TString title_new = title;
          //gph[j][k][l]->GetXaxis()->SetTitle(Form(title.c_str(),label_mu[l],h_num->GetTitle()));
          //mg[j][k][l]->GetXaxis()->SetTitle(title_new.ReplaceAll("Gen.","PAT"));
          h_temp->GetXaxis()->SetTitle(title);
          h_temp->GetYaxis()->SetTitle(label_trig[k]);
          
          h_temp->SetMinimum(0.);
          h_temp->SetMaximum(1.05);
          
          if (i==nlifetime-1) h_temp->Draw("HIST p");
          else  h_temp->Draw("HIST p same");
          if(k==0 && l==0) { 
						leg_ent[j] = (TLegendEntry*)leg[j]->AddEntry(lifetime[i],label_leg[i],"p");
						leg_ent[j]->SetMarkerColor(col[i]);
						leg_ent[j]->SetMarkerStyle(sty[i]);
					}					      
        }
      }
    }
  }
  for (int j=0;j<nvars;j++) {
    for (int k=0;k<ntrig;k++) {
      for (int l=0;l<nmu;l++) {
        c_jkl[j][k][l]->cd();
        //mg[j][k][l]->Draw("AP");
        leg[j]->Draw("same");
        labellumi->Draw("same");     
        labellumi_extra->Draw("same");   
        //mg[j][k][l]->GetXaxis()->SetRangeUser(xmin[j],xmax[j]);
        //c_jkl[j][k][l]->
        //if (j==3) gPad->SetLogx();
        gPad->Modified();
        gPad->Update();
        //TString str;
        //str = str+vars[j]+"_"+trigger[k]+"_"+mu[l];
        if(saveplot) c_jkl[j][k][l]->SaveAs(Form("../plots/triggerDistributions/2018_plots/%s_%s_%s.pdf",vars[j],trig[k],mu[l]));
      }
    }
  }
  
  return;
  
 
  
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
    gStyle->SetTitleOffset(1.,"xy");
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

