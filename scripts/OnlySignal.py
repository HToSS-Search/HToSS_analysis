# Standard importts
import os,sys,socket,argparse
import shutil
import ROOT
import math
from array import array
import numpy as np
from ROOT import TCanvas, TColor, TGaxis, TH1F, TPad
from ROOT import kBlack, kBlue, kRed
import tdrstyle
import CMS_lumi
import gc

# Collect previous objects and disable circular garbage collector (reference counter still works)
gc.collect()
gc.disable()

ROOT.gROOT.SetBatch(True)
tdrstyle.setTDRStyle()
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetLineWidth(2)
ROOT.gStyle.SetLegendBorderSize(0)
ROOT.gSystem.SetIncludePath( "-I$ROOFITSYS/include/" )

colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
markers = [20, 21, 22, 33, 47]
m_size = 2
# tdrStyle.SetPadTopMargin(0.05)
# tdrStyle.SetPadBottomMargin(0.13)
# tdrStyle.SetPadLeftMargin(0.16)
# tdrStyle.SetPadRightMargin(0.02)
def createCanvasPads(savename,boundary=0.25):
    ylength_c=2400
    c = TCanvas(savename, savename, 2200, ylength_c)
    # Upper histogram plot is pad1
    # pad1 = TPad("pad1", "pad1", 0, 0.25, 1, 1.0)
    pad1 = ROOT.TPad("pad1", "pad1", 0, boundary, 1, 1.0)
    pad1.SetTopMargin(0.07)
    pad1.SetBottomMargin(0.05)  # joins upper and lower plot
    pad1.SetLeftMargin(0.14)
    pad1.SetRightMargin(0.04)
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c.cd()  # returns to main canvas before defining pad2
    pad2 = ROOT.TPad("pad2", "pad2", 0, 0, 1, boundary+0.01)
    pad2.SetTopMargin(0.05)  # joins upper and lower plot
    pad2.SetBottomMargin(0.13/boundary)
    pad2.SetLeftMargin(0.14)
    pad2.SetRightMargin(0.04)
    # pad2.SetBottomMargin(0.1)
    #pad2.SetGridx()
    pad2.Draw()

    return c, pad1, pad2

def createRatio(h1, h2):
    # npts = h1.GetNbinsX()
    # sfx=[None]*npts
    # sfx_err=[None]*npts
    # sfy=[None]*npts
    # sfy_err=[None]*npts

    # for i in range(npts):
    #     # mcEff, dataEff = 0., 0.
    #     # h2.GetPoint(i,mcEff,sfx[i])
    #     # h1.GetPoint(i,dataEff,sfx[i])
    #     # mcErr, dataErr = h2.GetErrorY(i), h1.GetErrorY(i)
    #     # sfx[i] = h2.GetPointX(i)
    #     mcEff, mcErr, dataEff, dataErr = h2.GetY()[i], h2.GetErrorY(i), h1.GetY()[i], h1.GetErrorY(i)
    #     sfx[i] = h2.GetX()[i]
    #     sfx_err[i] = h2.GetErrorX(i)
    #     sfy[i] = dataEff/mcEff if mcEff else 0.0
    #     sfy_err[i] = 0.0
    #     if dataEff and mcEff:
    #         sfy_err[i] = sfy[i] * ((dataErr / dataEff)**2 + (mcErr / mcEff)**2)**0.5
    # h3 = ROOT.TGraphErrors(npts,array('d',sfx),array('d',sfy),array('d',sfx_err),array('d',sfy_err))
    h3 = h1.Clone()
    h3.Divide(h2)
    h3.SetLineColor(kBlack)
    h3.SetMarkerStyle(21)
    h3.SetTitle("Data/MC")
    h3.SetMinimum(0.)
    h3.SetMaximum(2.)
    # Set up plot for markers and errors
    #h3.Sumw2()
    #h3.SetStats(0)
    #for i in range(npts):
    #    h3.SetBinContent(i+1,sfy[i])
    #    h3.SetBinError(i+1,sfy_err[i])
    # Adjust y-axis settings
    y = h3.GetYaxis()
    y.SetTitle("Data/MC")
    y.SetNdivisions(505)
    y.SetTitleSize(0.15)
    y.SetTitleFont(42)
    y.SetTitleOffset(0.5)
    y.SetLabelFont(42)
    #y.SetLabelOffset(0.007)
    y.SetLabelSize(0.14)

    # Adjust x-axis settings
    x = h3.GetXaxis()
    x.SetTitleSize(40)
    x.SetTitleFont(42)
    x.SetTitleOffset(4.0)
    x.SetLabelFont(42)
    x.SetLabelSize(0.)
    #x.SetLimits(0.,105.)

    return h3

def main():
    ROOT.TH1.AddDirectory(ROOT.kFALSE)


    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="input", help="Input directory name", type=str)
    parser.add_argument("--i2","--input2", dest="input2", help="Input directory name pion", type=str)
    # parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    # parser.add_argument("-c","--category", dest="category",help="prompt/displacedmumu/displacedhh/displaced", type=str)
    # parser.add_argument("--tf", dest="tf", default=1,help="Transfer factor depending on CR - Check BkgEst.py", type=float)
    parser.add_argument("--yhigh", dest="yhigh", default=500,help="y-axis multiplicative factor for ymax", type=float)
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    parser.add_argument("--norm", dest="norm", help="true for plotting with normalisation, false by default", action="store_true")
    # parser.add_argument("--mass", dest="mass", help="mass of scalar", type=str)
    # parser.add_argument("--analysis", dest="analysis", help="true for plotting after analysis, false by default (after skim)", action="store_true")

    # parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
    # add an option to plot just one plot accessible name in histo_dict; change savename accordingly
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()

    histo_dict = {
        'leadingMuonRelIso': {'hname':'h_leadingMuonRelIso','label': "leading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':2.,'hrebin':20},
        'subleadingMuonRelIso': {'hname':'h_subleadingMuonRelIso','label': "subleading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':2.,'hrebin':20},
        'leadingChRelIso': {'hname':'h_leadingChHadRelIso','label': "leading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':2.,'hrebin':20},
        'subleadingChRelIso': {'hname':'h_subleadingChHadRelIso','label': "subleading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':2.,'hrebin':20},

        # 'leadingMuonSumPtCh': {'hname':'h_leadingMuonSumPtCh','label': "leading #mu #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingMuonSumPtCh': {'hname':'h_subleadingMuonSumPtCh','label': "subleading #mu #Sigma pp_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'leadingChSumPtCh': {'hname':'h_leadingChHadSumPtCh','label': "leading h^{#pm} #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingChSumPtCh': {'hname':'h_subleadingChHadSumPtCh','label': "subleading h^{#pm} #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},

        # 'leadingMuonSumPtNh': {'hname':'h_leadingMuonSumPtNh','label': "leading #mu #Sigma p_{T}(nt. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingMuonSumPtNh': {'hname':'h_subleadingMuonSumPtNh','label': "subleading #mu #Sigma p_{T}(nt. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'leadingChHadSumPtNh': {'hname':'h_leadingChHadSumPtNh','label': "leading h^{#pm} #Sigma p_{T}(nt. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingChHadSumPtNh': {'hname':'h_subleadingChHadSumPtNh','label': "subleading h^{#pm} #Sigma p_{T}(nt. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},

        # 'leadingMuonSumPtPh': {'hname':'h_leadingMuonSumPtPh','label': "leading #mu #Sigma p_{T}(#gamma) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingMuonSumPtPh': {'hname':'h_subleadingMuonSumPtPh','label': "subleading #mu #Sigma p_{T}(#gamma) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'leadingChHadSumPtPh': {'hname':'h_leadingChHadSumPtPh','label': "leading h^{#pm} #Sigma p_{T}(#gamma) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingChHadSumPtPh': {'hname':'h_subleadingChHadSumPtPh','label': "subleading h^{#pm} #Sigma p_{T}(#gamma) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},

        # 'DiChHadPt': {'hname':'h_DiChHadPt','label': "p_{T,h^{+}h^{-}} (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'DiChHadMass': {'hname':'h_DiChHadMass','label': "m_{h^{+}h^{-}} (GeV)", 'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'DiMuonPt': {'hname':'h_DiMuonPt','label': "p_{T,#mu#mu} (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'DiMuonMass': {'hname':'h_DiMuonMass','label': "m_{#mu#mu} (GeV)", 'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'recoHiggsMass': {'hname':'h_recoHiggsMass','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
        # 'AvgMass_mumu_hh':{'hname':'h_AvgMass_mumu_hh','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},


        # 'recoHiggsMass_MHMSIso_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMSIso_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
        # 'recoHiggsMass_MHIso_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_All_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_AtScalarMass2': {'hname':'h_recoHiggsMass_All_WithPeak_AtScalarMass2','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},        # 'recoHiggsMass_HMassSMassBC': {'hname':'h_recoHiggsMass_HMassSMassBC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},
        # 'recoHiggsMass_MHMS_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
        # 'recoHiggsMass_MH_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_All_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_AtScalarMass2': {'hname':'h_recoHiggsMass_All_WithPeak_AtScalarMass2','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},        # 'recoHiggsMass_HMassSMassBC': {'hname':'h_recoHiggsMass_HMassSMassBC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},
        # 'recoHiggsMass_MHMSIso_BC': {'hname':'h_recoHiggsMass_MHMSIso_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
        # 'recoHiggsMass_MHIso_BC': {'hname':'h_recoHiggsMass_MHIso_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_All_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_All_WithPeak_AtScalarMass2': {'hname':'h_recoHiggsMass_All_WithPeak_AtScalarMass2','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        # 'recoHiggsMass_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},        # 'recoHiggsMass_HMassSMassBC': {'hname':'h_recoHiggsMass_HMassSMassBC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},
# 
        # 'AvgMass_mumu_hh_MSIso_BC':{'hname':'h_AvgMass_mumu_hh_MSIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_Iso_BC':{'hname':'h_AvgMass_mumu_hh_Iso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MS_BC':{'hname':'h_AvgMass_mumu_hh_MS_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 
        # 'AvgMass_mumu_hh_MHMSIso_BC':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMSIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MHIso_BC':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MHMS_BC':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MH_BC':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MH_FineBinning':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':1.8,'xhigh':2.2,'hrebin':1},

        # 'AvgMass_mumu_hh_HMassBC_FineBinning':{'hname':'h_AvgMass_mumu_hh_MSIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':1.8,'xhigh':2.2,'hrebin':1},
        # 'AvgMass_mumu_hh_HMassBC_TF':{'hname':'h_AvgMass_mumu_hh_HMassBC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},

        # 'DiMuonVtxLxy': {'hname':'h_DiMuonVtxLxy','label':"L_{xy}^{#mu^{+}#mu^{-}} (cm)", 'xlow':0,'xhigh':100,'hrebin':5},
        # 'genScalar1Lxy': {'hname':'h_genScalar1Lxy','label':"L_{xy}^{S} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        # 'genScalar2Lxy': {'hname':'h_genScalar2Lxy','label':"L_{xy}^{\\bar{S}} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        # # 'DiChVtxLxy': {'hname':'h_DiChHadVtxLxy','label':"L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0,'xhigh':100,'hrebin':5},
        # 'genScalar1L': {'hname':'h_genScalar1L','label':"L_{xyz}^{S} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        # 'genScalar2L': {'hname':'h_genScalar2L','label':"L_{xyz}^{\\bar{S}} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},

        # 'DiMuonVtxSignificance': {'hname':'h_DiMuonVtxSignificance','label':"L_{xy}^{#mu^{+}#mu^{-}}/#Delta L_{xy}^{#mu^{+}#mu^{-}}", 'xlow':0,'xhigh':1000,'hrebin':40},
        # 'DiChVtxSignificance': {'hname':'h_DiChHadVtxSignificance','label':"L_{xy}^{h^{+}h^{-}}/#Delta L_{xy}^{h^{+}h^{-}}", 'xlow':0,'xhigh':1000,'hrebin':40},
        # 'DiMuonVtxSigma': {'hname':'h_DiMuonVtxSigma','label':"#Delta L_{xy}^{#mu^{+}#mu^{-}} (cm)", 'xlow':0,'xhigh':10,'hrebin':1},
        # 'DiChVtxSigma': {'hname':'h_DiChHadVtxSigma','label':"#Delta L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0,'xhigh':10,'hrebin':1},
        # 'DiMuonDiChHad_DeltaPhi': {'hname':'h_DiMuonDiChHad_DeltaPhi','label':"#Delta #phi(#mu#mu,hh)", 'xlow':-3.2,'xhigh':3.2,'hrebin':10},
        # 'DiChHadDeltaPixHits': {'hname':'h_DiChHadDeltaPixHits','label':"#Delta N^{pixel}_{hh}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'DiChHadDeltaTrkHits': {'hname':'h_DiChHadDeltaTrkHits','label':"#Delta N^{tracker}_{hh}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'DiMuonDeltaPixHits': {'hname':'h_DiMuonDeltaPixHits','label':"#Delta N^{pixel}_{#mu#mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'DiMuonDeltaTrkHits': {'hname':'h_DiMuonDeltaTrkHits','label':"#Delta N^{tracker}_{#mu#mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'leadingMuonTrkHits': {'hname':'h_leadingMuonTrkHits','label':"N^{tracker}_{leading #mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'leadingChHadTrkHits': {'hname':'h_leadingChHadTrkHits','label':"N^{tracker}_{leading h}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'subleadingMuonTrkHits': {'hname':'h_subleadingMuonTrkHits','label':"N^{tracker}_{subleading #mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'subleadingChHadTrkHits': {'hname':'h_subleadingChHadTrkHits','label':"N^{tracker}_{subleading h}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'leadingMuonPixHits': {'hname':'h_leadingMuonPixHits','label':"N^{pixel}_{leading #mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'leadingChHadPixHits': {'hname':'h_leadingChHadPixHits','label':"N^{pixel}_{leading h}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'subleadingMuonPixHits': {'hname':'h_subleadingMuonPixHits','label':"N^{pixel}_{subleading #mu}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},
        # 'subleadingChHadPixHits': {'hname':'h_subleadingChHadPixHits','label':"N^{pixel}_{subleading h}(hits)", 'xlow':-0.5,'xhigh':30.5,'hrebin':1},



        
        # 'Cutflow':{'hname':'Cutflow','label':"",'xlow':0.5,'xhigh':12.5,'hrebin':1},

        # 'leadingMuonPVAscQuality':{'hname':"h_leadingMuonPVAscQuality",'label':"leading #mu",'xlow':0.5,'xhigh':6.5,'hrebin':1}, 
        # 'subleadingMuonPVAscQuality':{'hname':"h_subleadingMuonPVAscQuality",'label':"subleading #mu",'xlow':0.5,'xhigh':6.5,'hrebin':1},
        # 'leadingChHadPVAscQuality':{'hname':"h_leadingChHadPVAscQuality",'label':"leading h^{#pm}",'xlow':0.5,'xhigh':6.5,'hrebin':1},
        # 'subleadingChHadPVAscQuality':{'hname':"h_subleadingChHadPVAscQuality",'label':"subleading h^{#pm}",'xlow':0.5,'xhigh':6.5,'hrebin':1},
        
        # 'leadingMuonDz':{'hname':"h_leadingMuonDz",'label':"leading #mu dz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'subleadingMuonDz':{'hname':"h_subleadingMuonDz",'label':"subleading #mu dz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'leadingChHadDz':{'hname':"h_leadingChHadDz",'label':"leading h^{#pm} dz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'subleadingChHadDz':{'hname':"h_subleadingChHadDz",'label':"subleading h^{#pm} dz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},

        # 'leadingMuonVz':{'hname':"h_leadingMuonVz",'label':"leading #mu vz (cm)",'xlow':-20,'xhigh':20,'hrebin':5}, 
        # 'subleadingMuonVz':{'hname':"h_subleadingMuonVz",'label':"subleading #mu vz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'leadingChHadVz':{'hname':"h_leadingChHadVz",'label':"leading h^{#pm} vz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'subleadingChHadVz':{'hname':"h_subleadingChHadVz",'label':"subleading h^{#pm} vz (cm)",'xlow':-20,'xhigh':20,'hrebin':5},

        # 'DiMuonVtxVz':{'hname':"h_DiMuonVtxVz",'label':"di-#mu SV vz  (cm)",'xlow':-20,'xhigh':20,'hrebin':5},
        # 'DiChHadVtxVz':{'hname':"h_DiChHadVtxVz",'label':"di-h^{#pm} SV vz  (cm)",'xlow':-20,'xhigh':20,'hrebin':5}, 
        # 'DiMuonVtxDz':{'hname':"h_DiMuonVtxDz",'label':"di-#mu SV dz (cm)",'xlow':-20,'xhigh':20,'hrebin':5}, 
        # 'DiChHadVtxDz':{'hname':"h_DiChHadVtxDz",'label':"di-h^{#pm} SV Vz (cm)",'xlow':-20,'xhigh':20,'hrebin':5}, 

        # 'DiMuonVtxChi2':{'hname':"h_DiMuonVtxChi2",'label':"di-#mu SV #Chi^2",'xlow':0,'xhigh':10,'hrebin':5}, 
        # 'DiChHadVtxChi2':{'hname':"h_DiChHadVtxChi2",'label':"di-h^{#pm} SV #Chi^2",'xlow':0,'xhigh':10,'hrebin':5}, 
        # 'DiMuonVtxNdof':{'hname':"h_DiMuonVtxNdof",'label':"di-#mu SV #Chi^2/ndof",'xlow':0,'xhigh':10,'hrebin':5}, 
        # 'DiChHadVtxNdof':{'hname':"h_DiChHadVtxNdof",'label':"di-h^{#pm} SV ndof",'xlow':0,'xhigh':10,'hrebin':5},
        # 'DiMuonVtxChi2Ndof':{'hname':"h_DiMuonVtxChi2Ndof",'label':"di-#mu SV #Chi^2/ndof",'xlow':0,'xhigh':10,'hrebin':5},
        # 'DiChHadVtxChi2Ndof':{'hname':"h_DiChHadVtxChi2Ndof",'label':"di-h^{#pm} SV #Chi^2/ndof",'xlow':0,'xhigh':10,'hrebin':5},
        
        # 'leadingMuonPt':{'hname':"h_leadingMuonPt",'label':"leading #mu p_{T} (GeV)",'xlow':0,'xhigh':100,'hrebin':60},
        # 'leadingChHadPt':{'hname':"h_leadingChHadPt",'label':"leading h^{#pm} p_{T} (GeV)",'xlow':0,'xhigh':100,'hrebin':60}

    }
    datasets_dict = {
        'HToSS_MH125_MS2_ctauS0':{'type':'signal','label':"#splitline{m_{S}=2 GeV,}{c#tau = 0.1mm}",'color':ROOT.kBlack-2,'integral':-1},
        'HToSS_MH125_MS2_ctauS10':{'type':'signal','label':"#splitline{m_{S}=2 GeV,}{c#tau = 10mm}",'color':ROOT.kBlack-2,'integral':-1},
        # 'HToSS_MH125_MS2_ctauS100':{'type':'signal','label':"#splitline{m_{S}=2 GeV,}{c#tau = 100mm}",'color':ROOT.kBlack-2,'integral':-1},
        'HToSS_MH125_MS1p2_ctauS0':{'type':'signal','label':"#splitline{m_{S}=1.2 GeV,}{c#tau = 0.1mm}",'color':ROOT.kOrange+1,'integral':-1},
        'HToSS_MH125_MS1p2_ctauS10':{'type':'signal','label':"#splitline{m_{S}=1.2 GeV,}{c#tau = 10mm}",'color':ROOT.kOrange+1,'integral':-1},
        # 'HToSS_MH125_MS1p2_ctauS100':{'type':'signal','label':"#splitline{m_{S}=1.2 GeV,}{c#tau = 100mm}",'color':ROOT.kOrange+1,'integral':-1},
        'HToSS_MH125_MS0p6_ctauS0':{'type':'signal','label':"#splitline{m_{S}=0.6 GeV,}{c#tau = 0.1mm}",'color':ROOT.kBlue-7,'integral':-1},
        'HToSS_MH125_MS0p6_ctauS10':{'type':'signal','label':"#splitline{m_{S}=0.6 GeV,}{c#tau = 10mm}",'color':ROOT.kBlue-7,'integral':-1},
        'Data':{'type':'data','label':"J/#Psi data (UL2017)",'color':ROOT.kGreen+3,'integral':-1},
        # 'HToSS_MH125_MS1p6_ctauS100':{'type':'signal','label':"#splitline{m_{S}=1.6 GeV,}{c#tau = 100mm}",'color':ROOT.kBlue-7,'integral':-1}
    }
    ctau_style_map = {'ctauS0':1,'ctauS1':5,'ctauS10':9,'ctauS100':10, 'Data':3}
    # sig_mass = args.sig.split("MS")[1].split("_")[0].replace("p",".")
    # sig_ctau = args.sig.split("ctau")[1].split(".")[0].replace("p",".")
    iso_dict={'leadingMuonRelIso':0.3,'subleadingMuonRelIso':0.6,'leadingChRelIso':0.6,'subleadingChRelIso':0.8}
    
    lumi_scale = {'UL2016_APV': 19500, 'UL2016': 16800,'UL2017':  41480,'UL2018': 59830 } #in pb-1
    lumi_factor = lumi_scale[args.year]
    ###### right now transfer factors have not been applied below #######
    for key in histo_dict:
        print(key)
        fsig=[]
        leg = ROOT.TLegend(0.5, 0.65, 0.95, 0.92)
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetNColumns(2)
        leg.SetTextSize(0.022)
        # hsig=[]
        savename=key+"_OnlySignal"+"_JPsi"
        boundary_percent = 0.35
        ylength_c = int(2400*(1-boundary_percent+0.15))
        c1 = TCanvas(savename, savename, 2200, ylength_c)
        pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
        pad1.SetTopMargin(0.06)
        pad1.SetBottomMargin(0.15)
        pad1.SetLeftMargin(0.16)
        pad1.SetRightMargin(0.04)
        #pad1.SetGridx()
        pad1.Draw()
        # Lower ratio plot is pad2
        c1.cd()
        pad1.cd()
        if args.log:
            # c1.SetLogy()
            pad1.SetLogy()
            # pad1.SetLogx()

        for dno,dname in enumerate(datasets_dict):
            # print(dname)
            hprop = histo_dict[key]
            histname = hprop['hname']
            mass=dname.split('MS')[-1].split('_')[0].replace('p','.')
            if "MS" in dname:
                if float(mass) < 1.1:
                    indir = args.input2
                else:
                    indir = args.input 
            else:
                    indir = args.input 
            print(indir+"/"+ "output_" + dname + ".root")
            fsig=ROOT.TFile(indir+"/"+ "output_" + dname + ".root", "READ")
            # if args.analysis:
            if datasets_dict[dname]['type']=="data":
                h_1 = fsig.Get(histname+"_MHMSIso_BC_jpsi")
            else:
                h_1 = fsig.Get('prompt'+'/'+histname)
                h_1.Add(fsig.Get('displaced'+'/'+histname))
                h_1.Add(fsig.Get('displacedmumu'+'/'+histname))
                h_1.Add(fsig.Get('displacedhh'+'/'+histname))
            print(h_1.GetName())
            # else:
            #     h_1 = fsig.Get(histname).Clone()
            h_1.Rebin(hprop['hrebin'])
            h_1.Scale(lumi_factor)
            if args.norm:
                h_1.Scale(1/h_1.Integral())
            datasets_dict[dname]['integral']=h_1.Integral()
            color_ = datasets_dict[dname]['color']
            h_1.SetFillColor(color_)
            h_1.SetFillStyle(0) # hollow hist
            h_1.SetMarkerColor(color_)
            h_1.SetLineColor(color_)
            h_1.SetMarkerStyle(8)
            h_1.SetLineStyle(ctau_style_map[dname.split('_')[-1]])
            h_1.SetLineWidth(6)
            h_1.SetMarkerSize(m_size)
            h_1.GetXaxis().SetRangeUser(hprop['xlow'],hprop['xhigh'])
            # if args.log:
            #     h_1.GetXaxis().SetLimits(1e-2,1e2)
            h_1.GetXaxis().SetTitle(hprop['label'])
            # h_1.GetYaxis().SetTitle("Normalized Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
            # h_1.GetYaxis().SetTitle("Events/#Sigma(wts) / "+str(h_1.GetXaxis().GetBinWidth(1)))
            if args.norm:
                h_1.GetYaxis().SetTitle("Normalized Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
            else:
                h_1.GetYaxis().SetTitle("Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
            leg.AddEntry(h_1, datasets_dict[dname]['label'], "l")
            if args.log:
                if args.norm:
                    h_1.SetMinimum(1e-3)
                    h_1.SetMaximum(3)
                else:
                    if args.analysis:
                        h_1.SetMinimum(1e-3)
                    else:
                        h_1.SetMinimum(1)
                    h_1.SetMaximum(1e4)
            else:
                h_1.SetMinimum(0.)
                ymax = args.yhigh*h_1.GetMaximum()
            h_1.GetXaxis().SetTitleSize(0.05)
            h_1.GetYaxis().SetTitleSize(0.05)
            h_1.GetXaxis().SetLabelSize(0.045)
            h_1.GetYaxis().SetLabelSize(0.045)
            h_1.GetXaxis().SetTitleOffset(1.1)
            h_1.GetYaxis().SetTitleOffset(1.4)

            if dno==0:
                h_1.Draw("hist")
            else:
                h_1.Draw("hist same")
            # pad1.Modified()
            # pad1.Update()
            fsig.Close()
        leg.Draw("same")
        pad1.Modified()
        pad1.Update()

        if ('RelIso' in key):
            iso_val=iso_dict[key]
            l1 = ROOT.TLine(iso_val,0,iso_val,h_1.GetMaximum())
            l1.SetLineWidth(4)
            l1.SetLineStyle(7)
            l1.SetLineColor(ROOT.kRed)
            l1.Draw("same")
        pad1.Modified()
        pad1.Update()

        CMS_lumi.cmsText = 'CMS'
        CMS_lumi.writeExtraText = True
        CMS_lumi.extraText = 'Work in Progress'
        CMS_lumi.lumi_13TeV = args.year+" MC"
        CMS_lumi.lumiTextSize = 0.5
        CMS_lumi.cmsTextSize=1.
        CMS_lumi.CMS_lumi(pad1, 4, 11)

        pad1.Modified()
        pad1.Update()
        c1.Modified()
        c1.Update()
        c1.SaveAs(args.out+'/'+savename+'.png')
        c1.SaveAs(args.out+'/'+savename+'.pdf')
        # c1.SaveAs(args.out+'/'+dirn+savename+'.root')
        # c1.Clear()
        # c1.Delete()
    gc.enable()

    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
