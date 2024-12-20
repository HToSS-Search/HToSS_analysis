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
import ctypes
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
    # parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    # parser.add_argument("-c","--category", dest="category",help="prompt/displacedmumu/displacedhh/displaced", type=str)
    # parser.add_argument("--tf", dest="tf", default=1,help="Transfer factor depending on CR - Check BkgEst.py", type=float)
    parser.add_argument("--yhigh", dest="yhigh", default=500,help="y-axis multiplicative factor for ymax", type=float)
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    parser.add_argument("--norm", dest="norm", help="true for plotting with normalisation, false by default", action="store_true")
    parser.add_argument("--mass", dest="mass", help="mass of scalar", type=str)
    parser.add_argument("--analysis", dest="analysis", help="true for plotting after analysis, false by default", action="store_true")
    # parser.add_argument("--trg", dest="trg", help="true for plotting trigger eff., false by default", action="store_true")

    # parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
    # add an option to plot just one plot accessible name in histo_dict; change savename accordingly
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()

    histo_dict = {
        # 'leadingMuonRelIso': {'hname':'h_leadingMuonRelIso','label': "leading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'subleadingMuonRelIso': {'hname':'h_subleadingMuonRelIso','label': "subleading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'leadingChRelIso': {'hname':'h_leadingChHadRelIso','label': "leading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'subleadingChRelIso': {'hname':'h_subleadingChHadRelIso','label': "subleading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},

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
        'genScalar1Lxy': {'hname':'h_genScalar1Lxy','label':"L_{xy}^{S} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        'genScalar2Lxy': {'hname':'h_genScalar2Lxy','label':"L_{xy}^{\\bar{S}} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        # 'DiChVtxLxy': {'hname':'h_DiChHadVtxLxy','label':"L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0,'xhigh':100,'hrebin':5},
        'genScalar1L': {'hname':'h_genScalar1L','label':"L_{xyz}^{S} (cm)", 'xlow':0,'xhigh':100,'hrebin':5},
        'genScalar2L': {'hname':'h_genScalar2L','label':"L_{xyz}^{\\bar{S}} (cm)", 'xlow':0,'xhigh':100,'hrebin':5},

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
        'HToSS_MH125_MS'+args.mass+'_ctauS0':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 0.1mm}",'color':ROOT.kBlack-2,'integral':-1,'integral_err':-1,'entries':-1},
        'HToSS_MH125_MS'+args.mass+'_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 1mm}",'color':ROOT.kBlue-7,'integral':-1,'integral_err':-1,'entries':-1},
        'HToSS_MH125_MS'+args.mass+'_ctauS10':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 10mm}",'color':ROOT.kMagenta-3,'integral':-1,'integral_err':-1,'entries':-1},
        'HToSS_MH125_MS'+args.mass+'_ctauS100':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 100mm}",'color':ROOT.kCyan+2,'integral':-1,'integral_err':-1,'entries':-1},
        # 'HToSS_MH125_MS'+args.mass+'_ctauS0_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 0.1mm#rightarrow 1mm}",'color':ROOT.kRed+1},
        # 'HToSS_MH125_MS'+args.mass+'_ctauS0_ctauS0p01':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 0.1mm#rightarrow 0.01mm}",'color':ROOT.kOrange+1},
        'HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 1mm#rightarrow 0.1mm}",'color':ROOT.kGray+3,'integral':-1,'integral_err':-1,'entries':-1},
        #'HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 1mm#rightarrow 1mm}",'color':ROOT.kYellow+1},
        'HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau =10mm#rightarrow 1mm}",'color':ROOT.kOrange+1,'integral':-1,'integral_err':-1,'entries':-1},
        'HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau =100mm#rightarrow 10mm}",'color':ROOT.kRed+1,'integral':-1,'integral_err':-1,'entries':-1},
        # 'HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau =100mm#rightarrow 1mm}",'color':ROOT.kGray+2},
    }

    # sig_mass = args.sig.split("MS")[1].split("_")[0].replace("p",".")
    # sig_ctau = args.sig.split("ctau")[1].split(".")[0].replace("p",".")

    
    lumi_scale = {'UL2016_APV': 19500, 'UL2016': 16800,'UL2017':  41480,'UL2018': 59830 } #in pb-1
    # lumi_factor = lumi_scale['UL2017']
    lumi_factor = lumi_scale[args.year]
    categories=["prompt","displacedmumu","displacedhh","displaced"]
    ###### right now transfer factors have not been applied below #######
    for cat in categories[0]: ###TESTING TESTING TESTING ####
        for key in histo_dict:
            print(key)
            fsig=[]
            leg = ROOT.TLegend(0.5, 0.65, 0.95, 0.92)
            leg.SetBorderSize(0)
            leg.SetFillStyle(0)
            leg.SetNColumns(2)
            leg.SetTextSize(0.022)
            # hsig=[]
            if args.analysis:
                savename=key+'_'+'MS'+args.mass+"_analysis"+"_test"
            else:
                savename=key+'_'+'MS'+args.mass
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
            # if dname.count("ctauS")>1 and args.trg:
            #     continue
            # print(dname)
            hprop = histo_dict[key]
            histname = hprop['hname']
            print(args.input+"/"+ "output_" + dname + ".root")
            fsig=ROOT.TFile(args.input+"/"+ "output_" + dname + ".root", "READ")
            if args.analysis:
                h_1 = fsig.Get('prompt'+'/'+histname)
                h_1.Add(fsig.Get('displaced'+'/'+histname))
                h_1.Add(fsig.Get('displacedmumu'+'/'+histname))
                h_1.Add(fsig.Get('displacedhh'+'/'+histname))
            else:
                h_1 = fsig.Get(histname).Clone()
            h_1.Rebin(hprop['hrebin'])
            h_1.Scale(lumi_factor)
            if args.norm:
                h_1.Scale(1/h_1.Integral())
            # =h_1.Integral()
            # tmp_err=ctypes.c_double(0)
            # datasets_dict[dname]['integral'] = h_1.IntegralAndError(h_1.FindBin(0),h_1.FindBin(1000),tmp_err)
            # datasets_dict[dname]['integral_error']=tmp_err.value
            wt_sum, wt_sum_err=0,0
            for i in range(1, h_1.GetNbinsX() + 1):
                xval,yval,xerr,yerr = h_1.GetBinCenter(i),h_1.GetBinContent(i),0.5*h_1.GetBinWidth(i),h_1.GetBinError(i)
                wt_sum=wt_sum+yval
                wt_sum_err=wt_sum_err+yerr**2
            datasets_dict[dname]['integral']=wt_sum
            datasets_dict[dname]['integral_error']=np.sqrt(wt_sum_err)
            datasets_dict[dname]['entries']=h_1.GetEntries()
            color_ = datasets_dict[dname]['color']
            h_1.SetFillColor(color_)
            h_1.SetFillStyle(0) # hollow hist
            h_1.SetMarkerColor(color_)
            h_1.SetLineColor(color_)
            h_1.SetMarkerStyle(8)
            h_1.SetMarkerSize(m_size)
            h_1.GetXaxis().SetRangeUser(hprop['xlow'],hprop['xhigh'])
            # if args.log:
            #     h_1.GetXaxis().SetLimits(1e-2,1e2)
            h_1.GetXaxis().SetTitle(hprop['label'])
            # h_1.GetYaxis().SetTitle("Normalized Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
            # h_1.GetYaxis().SetTitle("Events/#Sigma(wts) / "+str(h_1.GetXaxis().GetBinWidth(1)))
            h_1.GetYaxis().SetTitle("Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
            leg.AddEntry(h_1, datasets_dict[dname]['label'], "l")
            if args.log:
                if args.norm:
                    h_1.SetMinimum(1e-3)
                    h_1.SetMaximum(3)
                else:
                    if args.analysis:
                        h_1.SetMinimum(1e-7)
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

            if dname.count('ctauS')==2:
                h_1.SetLineStyle(9)
                h_1.SetLineWidth(8)

            else:
                h_1.SetLineStyle(1)
                h_1.SetLineWidth(7)



            # h_1.Draw("hist")
            # hsig.append(h_1)
            if dno==0:
                h_1.Draw("hist")
            else:
                h_1.Draw("hist same")
            # pad1.Modified()
            # pad1.Update()
            fsig.Close()
        leg.Draw("same")
        # print("WTF")
        ctauS1_ctauS0p1_frac = datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS0']['integral']
        ctauS10_ctauS1_frac = datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1']['integral']
        ctauS100_ctauS10_frac = datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10']['integral']
        # error on integral
        ctauS1_ctauS0p1_frac_err = ctauS1_ctauS0p1_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral'])**2+(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS0']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS0']['integral'])**2)
        ctauS10_ctauS1_frac_err = ctauS10_ctauS1_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral'])**2+(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1']['integral'])**2)
        ctauS100_ctauS10_frac_err = ctauS100_ctauS10_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral'])**2+(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10']['integral'])**2)
        # ctauS1_ctauS0p1_frac_err = ctauS1_ctauS0p1_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral'])**2)
        # ctauS10_ctauS1_frac_err = ctauS10_ctauS1_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral'])**2)
        # ctauS100_ctauS10_frac_err = ctauS100_ctauS10_frac*np.sqrt((datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral_error']/datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral'])**2)

        print(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS0']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['integral_error'])
        print(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['integral_error'])
        print(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10']['integral'],datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['integral_error'])

        ctauS1_ctauS0p1_entries=round(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS1_ctauS0p1']['entries'],1)
        ctauS10_ctauS1_entries=round(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS10_ctauS1']['entries'],1)
        ctauS100_ctauS10_entries=round(datasets_dict['HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS10']['entries'],1)
        rtext1='1#rightarrow 0.1 mm: '+str(round(ctauS1_ctauS0p1_frac,5))+"#pm"+str(round(ctauS1_ctauS0p1_frac_err,5))+', entries: '+str(ctauS1_ctauS0p1_entries)
        rtext2='10#rightarrow 1 mm: '+str(round(ctauS10_ctauS1_frac,5))+"#pm"+str(round(ctauS10_ctauS1_frac_err,5))+', entries: '+str(ctauS10_ctauS1_entries)
        rtext3='100#rightarrow 10 mm: '+str(round(ctauS100_ctauS10_frac,5))+"#pm"+str(round(ctauS100_ctauS10_frac_err,5))+', entries: '+str(ctauS100_ctauS10_entries)
        additional_text = []
        additional_text += [rtext1]
        additional_text += [rtext2]
        additional_text += [rtext3]
        if additional_text:
            nother = len(additional_text)
            # dims = [xleg-0.05, yleg - nother * 0.04 - 0.02, 0.85, 0.65]
            dims = [0.6, 0.6 - nother * 0.04 - 0.02, 0.6, 0.6]
            text = ROOT.TPaveText(*dims + ['NB NDC'])
            text.SetTextFont(42)
            text.SetBorderSize(0)
            text.SetFillColor(0)
            text.SetTextAlign(11)
            # if args.noratio:
            text.SetTextSize(0.02)
            # else:
                # text.SetTextSize(0.04)
            for rtext in additional_text:
                text.AddText(rtext)
            text.Draw()
        pad1.Modified()
        pad1.Update()

        CMS_lumi.cmsText = 'CMS'
        CMS_lumi.writeExtraText = True
        CMS_lumi.extraText = 'Work in Progress'
        CMS_lumi.lumi_13TeV = "UL2017 MC"
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
