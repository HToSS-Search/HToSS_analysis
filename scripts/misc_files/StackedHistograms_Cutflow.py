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
from BackgroundEstimation import transfer_factor, gInterpreter_transferfactor

ROOT.gROOT.SetBatch(True)
ReducedBinning = False
correctforMCPU = False
tdrstyle.setTDRStyle()
# RooFit
ROOT.gSystem.Load("libRooFit.so")
ROOT.gSystem.Load("libRooFitCore.so")
#ROOT.gROOT.SetStyle("Plain") # Not sure this is needed
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
    parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="input", help="Input directory name", type=str)
    parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    parser.add_argument("-c","--category", dest="category",help="prompt/displacedmumu/displacedhh/displaced", type=str)
    # parser.add_argument("--tf", dest="tf", default=1,help="Transfer factor depending on CR - Check BkgEst.py", type=float)
    parser.add_argument("--yhigh", dest="yhigh", default=500,help="y-axis multiplicative factor for ymax", type=float)
    parser.add_argument("-d","--data", dest="withdata", help="true for plotting with data, false by default", action="store_true")
    parser.add_argument("-b","--bkg", dest="withbkg", help="true for plotting with bkg MC, false by default", action="store_true")
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
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
        # 'recoHiggsMass_MHIso_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        
        # 'recoHiggsMass_All_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_All_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        
        # 'recoHiggsMass_All_WithPeak_AtScalarMass2': {'hname':'h_recoHiggsMass_All_WithPeak_AtScalarMass2','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        
        # 'recoHiggsMass_WithPeak_ScalarCut': {'hname':'h_recoHiggsMass_WithPeak_ScalarCut','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},        
        # 'recoHiggsMass_HMassSMassBC': {'hname':'h_recoHiggsMass_HMassSMassBC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':115,'xhigh':135,'hrebin':5},
        # 'recoHiggsMass_MHMS_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
        # 'recoHiggsMass_MH_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        
        'recoHiggsMass_MH_BC_MassAssumption': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},        
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

        # 'AvgMass_mumu_hh_MHMSIso_BC_TF':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMSIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MHIso_BC_TF':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MHMS_BC_TF':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MH_BC_TF':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},
        # 'AvgMass_mumu_hh_MH_FineBinning_TF':{'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':1.8,'xhigh':2.2,'hrebin':1},

        # 'AvgMass_mumu_hh_HMassBC_FineBinning':{'hname':'h_AvgMass_mumu_hh_MSIso_BC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':1.8,'xhigh':2.2,'hrebin':1},
        # 'AvgMass_mumu_hh_HMassBC_TF':{'hname':'h_AvgMass_mumu_hh_HMassBC','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0.2,'xhigh':2.5,'hrebin':40},

        # 'DiMuonVtxLxy': {'hname':'h_DiMuonVtxLxy','label':"L_{xy}^{#mu^{+}#mu^{-}} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
        # 'DiChVtxLxy': {'hname':'h_DiChHadVtxLxy','label':"L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
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
        'QCD':{'type':'MC_bkg','label':"QCD"},
        'wPlusJets':{'type':'MC_bkg','label':"W+jets"},
        'DYJetsToLL':{'type':'MC_bkg','label':"Z+jets"},
        'ttbar': {'type':'MC_bkg','label':"t#bar{t}"},
        'VV':{'type':'MC_bkg','label':"VV"},
        'singleTop':{'type':'MC_bkg','label':"Single top"},
        'ttV':{'type':'MC_bkg','label':"t#bar{t}V"},
        'VVV':{'type':'MC_bkg','label':"VVV"},
        'HToSS_MH125_MS2_ctau0':{'type':'signal','label':"H#rightarrow aa m_{a}=2 GeV, c#tau = 0mm"},
        'HToSS_MH125_MS2_ctau1':{'type':'signal','label':"H#rightarrow aa m_{a}=2 GeV, c#tau = 1mm"},
        'HToSS_MH125_MS2_ctau10':{'type':'signal','label':"H#rightarrow aa m_{a}=2 GeV, c#tau = 10mm"},
        'HToSS_MH125_MS2_ctau100':{'type':'signal','label':"H#rightarrow aa m_{a}=2 GeV, c#tau = 100mm"},
        'Run2017':{'type':'blinded_data','label':'Run2017'}
    }
    # datasets_bkg = [ "wPlusJets", "DYJetsToLL", "ttbar" , "VVV", "singleTop","QCD_MuEnrichedPt5"]
    datasets_data = ["Run2017"]
    # CHECK VV dataset -> LARGE WEIGHTS OR SOMETHING? WRONG CROSS-SECTION?
    datasets_bkg = ["VVV","ttV","singleTop","VV","ttbar" ,"DYJetsToLL","wPlusJets","QCD_MuEnrichedPt5"]
    # datasets_bkg = ["ttbar" ,"wPlusJets","QCD_MuEnrichedPt5"]
    # datasets_bkg = ["QCD_MuEnrichedPt5"]
    # datasets_bkg = ["QCD_MuEnrichedPt5"]
    #datasets = ["HToSS_MH125_MS2_ctauS0","HToSS_MH125_MS2_ctauS1","HToSS_MH125_MS2_ctauS10","HToSS_MH125_MS2_ctauS100"]
    datasets = ["HToSS_MH125_MS1_ctauS0","HToSS_MH125_MS0p9_ctauS0","HToSS_MH125_MS0p8_ctauS0","HToSS_MH125_MS0p6_ctauS0","HToSS_MH125_MS0p4_ctauS0"]

    # sig_mass = args.sig.split("_")[1].split("MS")[1].replace(".","p")
    # sig_ctau = args.sig.split("_")[2].split("ctau")[1]

    sig_mass = args.sig.split("MS")[1].split("_")[0].replace("p",".")
    # sig_ctau = args.sig.split("ctauS")[1].split(".")[0].replace("p",".")
    sig_ctau = args.sig.split("ctau")[1].split(".")[0].replace("p",".")

    datasets_label = ["#splitline{m_{S}="+sig_mass+" GeV,}{ c#tau = "+sig_ctau+"mm}","VVV", "t#bar{t}V", "Single top", "VV", "t#bar{t}", "Z+jets", "W+jets","QCD"]
    # datasets_label = ["#splitline{m_{a}="+sig_mass+" GeV,}{ c#tau = "+sig_ctau+"mm}","QCD"]
    # datasets_label = ["#splitline{H#rightarrow aa m_{a}="+sig_mass+" GeV}{, c#tau = "+sig_ctau+"mm}","VVV", "t#bar{t}V", "Single top", "t#bar{t}", "Z+jets", "W+jets","QCD"]
    
    # datasets_label = ["H#rightarrow aa m_{a}="+sig_mass+" GeV, c#tau = "+sig_ctau+"mm","t#bar{t}","W+jets","QCD"]
    #h_leadingMuon_RelIso_BC -> for before cuts plots

    # custom_a, custom_b, custom_c = np.arange(0.,122.52,0.48), np.arange(122.52,127.5,0.48), np.arange(127.5,300.,0.48) # change accordingly when higgs hist is modified
    custom_a, custom_b, custom_c = np.arange(0.,122.5,0.5), np.arange(122.5,127.5,0.5), np.arange(127.5,500.,0.5) # change accordingly when higgs hist is modified
    custom_bins = np.unique(np.append(np.append(custom_a,custom_b),custom_c))
    print("Custom bins: ",custom_bins)
    #histos_rebin = [1,1,1,1,1,1,1,1,1,1,1,1,1,1]

    #fh_1 = ROOT.TFile("output_"+datasets[0]+".root", "READ")
    #h_0 = [None] * len(datasets)
    # ROOT.gStyle.SetPalette(ROOT.kBird)
    # ROOT.TColor.InvertPalette()
    higgsmass = "h_recoHiggsMass"
    hstemp = ROOT.THStack("hstemp","stacked hist")
    dirname = {'prompt', 'displacedmumu', 'displacedhh', 'displaced'}
    # dirname = {'prompt'}
    tf_loose_list = {}
    tf_CR_list = {}
    fh = ROOT.TFile(args.input+"/"+"output_" + datasets_data[0] + ".root", "READ")
    if 'nocategory' in args.category:
        dirn=''
    else:
        dirn = args.category+'/'

    gInterpreter_transferfactor()
    # dirn = dirn+'/'
    for key in histo_dict:
        hprop = histo_dict[key]
        histname = hprop['hname']
        if "MH" not in histname or "Higgs" not in histname:
            continue
        if not args.withdata:
            print("no data provided - no TF")
            break
        cuts = key.split('_')[-2]
        print(histname)
        outdir = args.out
        print(outdir)
        # Navigate one folder back
        pardir = os.path.dirname(os.path.dirname(outdir))
        print(pardir)
        # Check if the "xyz" folder exists
        outdir = os.path.join(pardir, "TransferFactorFits")
        print(outdir)
        if not os.path.exists(outdir):
            # Create the "xyz" folder if it does not exist
            os.makedirs(outdir)
            shutil.copy(os.path.join(cwd,"scripts/index.php"),os.path.join(outdir,"index.php"))
        print(dirn+histname)
        h2_recoHiggs_AvgMass = fh.Get(dirn+histname)
        h_recoHiggs_all = h2_recoHiggs_AvgMass.ProjectionX()
        # tf_loose = ROOT.TransferFactor(outdir,dirn,h_recoHiggs_all,110,122.5,122.5,127.5,127.5,140)
        tf_list = ROOT.TransferFactor(outdir,args.category,h_recoHiggs_all,120,122.5,122.5,127.5,127.5,130)
        tf_loose_list[cuts]=tf_list[0]
        tf_CR_list[cuts]=tf_list[1]
        # tf = transfer_factor(dirn,h_recoHiggs_all,110,122.5,122.5,127.5,127.5,140)
        # print('Transfer factor: ',tf)
        h2_recoHiggs_AvgMass.Delete();h_recoHiggs_all.Delete(); #ALWAYS REMEMBER TO DELETE!!!
    fh.Close()
    print(tf_loose_list)
    print(tf_CR_list)
    # quit()
    ###### right now transfer factors have not been applied below #######

    fsig = ROOT.TFile(args.input+"/"+"output_" + "HToSS_MH125_MS"+str(sig_mass).replace('.','p')+"_"+"ctauS"+str(sig_ctau).replace('.','p') + ".root", "READ")
    fdata = ROOT.TFile(args.input+"/"+"output_" + datasets_data[0] + ".root", "READ")
    # fdatasets_bkg = []
    # for i, dataset in enumerate(datasets_bkg):
    #     ftmp = ROOT.TFile(args.input+"/"+"output_" + dataset + ".root", "READ")
    #     fdatasets_bkg.append(ftmp)
    # for dirn in dirname:
    list_of_hist = ['DiMuonVtxLxy','DiChVtxLxy','DiMuonVtxSignificance','DiChVtxSignificance','DiMuonVtxSigma','DiChVtxSigma'] # for nocategory
    lo_bound,hi_bound=1.98,2.02
    for key in histo_dict:
        folder = args.category
        hprop = histo_dict[key]
        histname = hprop['hname']
        cuts = key.split('_')[-2]
        if 'nocategory' in args.category:
            bool_list = [item not in key for item in list_of_hist]
            if all(bool_list):
                continue

        # if ("recoHiggsMass" not in histname):
        #     continue
        # if ("AvgMass" not in key):
        #     continue
        # if ('PVAscQuality' in histname):
        #     continue
        # if ('VtxNdof' in histname):
        #     continue
        # if (j>=4): continue
        print(histname)
        leg = ROOT.TLegend(0.68, 0.65, 0.95, 0.92)
        leg.SetFillStyle(0)
        leg.SetNColumns(2)
        leg.SetTextSize(0.027)
        hs = ROOT.THStack("hs","stacked hist")
        # hs_normed = ROOT.THStack("hs_normed","stacked hist")
        savename = key
        # if "TF" in key:
        #     savename = savename + "_TF"

        #c1.Range(0,10,10.,10000)
        #print(i)
        if args.withbkg:
            for i, dataset in enumerate(datasets_bkg):
                fh = ROOT.TFile(args.input+"/"+"output_" + dataset + ".root", "READ")
                # fh = fdatasets_bkg[i]
                print(fh.GetName())
                print(dirn)
                print(histname)
                #print(hs[0].GetName())
                h_test = fh.Get(dirn+histname)
                if h_test.InheritsFrom(ROOT.TH2.Class()):
                    if histname.split('_')[1] in key:
                        if 'MassAssumption' in key:
                            h_0 = h_test.ProjectionX("",h_test.GetYaxis().FindBin(lo_bound),h_test.GetYaxis().FindBin(hi_bound)).Clone()
                        else:
                            h_0 = h_test.ProjectionX().Clone()
                    else:
                        h_0 = h_test.ProjectionY().Clone()
                    h_test.Delete()
                else:
                    h_0 = h_test.Clone()
                
                if ('TF' in key):
                    h_0.Scale(tf_loose_list[cuts])
                
                # h_tmp = TH2Check(histname,key,h_test)
                # h_0.Delete()
                # h_0=h_tmp.Clone();h_tmp.Delete();
                # quit()
                # nBins = h_0.GetNbinsX()
                # h_0.SetBinContent(nBins,h_0.GetBinContent(nBins)+h_0.GetBinContent(nBins+1))
                # h_0.SetBinContent(1,h_0.GetBinContent(nBins)+h_0.GetBinContent(nBins+1))
                if ("Higgs" not in key):
                    h_0.Rebin(hprop['hrebin'])
                else:
                    print ("ENTERS HERE")
                    lowestBinEdge = h_0.GetXaxis().GetBinLowEdge(1)
                    highestBinEdge = h_0.GetXaxis().GetBinUpEdge(h_0.GetNbinsX())
                    binWidth = h_0.GetXaxis().GetBinWidth(1)
                    print("Current bin width: ",lowestBinEdge, highestBinEdge, binWidth)
                    h_0 = h_0.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
                h_0.SetName(dataset)
                if key == "AvgMass_mumu_hh_HMassBC_TF":
                    h_0.Scale(args.tf)
                # print("First occurence:",h_0.GetName())
                hs.Add(h_0)
                leg.AddEntry(h_0, datasets_label[i+1], "f")
                fh.Close()
        #sig_ind = np.where(np.array(datasets) == args.sig)
        #fsig = ROOT.TFile(args.input+"/"+"output_" + datasets[sig_ind[0].item()] + ".root", "READ")
        #fsig = ROOT.TFile(args.input+"/"+datasets[sig_ind[0].item()] + ".root", "READ")
        h_test = fsig.Get(dirn+histname)
        if h_test.InheritsFrom(ROOT.TH2.Class()):
            if histname.split('_')[1] in key:
                h_1 = h_test.ProjectionX().Clone()
            else:
                h_1 = h_test.ProjectionY().Clone()
            h_test.Delete()
        else:
            h_1 = h_test.Clone()
        if args.mult != 1:
            h_1.Scale(args.mult)
        if ("Higgs" not in key):
            h_1.Rebin(hprop['hrebin'])
        else:
            h_1 = h_1.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
        h_1.SetFillColor(colors[1])
        h_1.SetFillStyle(0) # hollow hist
        h_1.SetMarkerColor(colors[1])
        h_1.SetLineColor(colors[1])
        h_1.SetMarkerStyle(8)
        h_1.SetMarkerSize(m_size)
        if args.mult != 1:
            leg.AddEntry(h_1, "("+datasets_label[0]+")*"+str(args.mult), "l")
        else:
            leg.AddEntry(h_1, datasets_label[0], "l")
        if args.withdata:
            print("LOOK HERE:",histname)
            if "recoHiggsMass" in histname:
                histname_tmp = histname.replace('_WithPeak','')
                h_test = fdata.Get(dirn+histname_tmp)
            else:
                h_test = fdata.Get(dirn+histname)
            if h_test.InheritsFrom(ROOT.TH2.Class()):
                if histname.split('_')[1] in key: #higgs part
                    if 'MassAssumption' in key:
                        h_2 = h_test.ProjectionX("",h_test.GetYaxis().FindBin(lo_bound),h_test.GetYaxis().FindBin(hi_bound)).Clone()
                    else:
                        h_2 = h_test.ProjectionX().Clone()
                else:
                    h_2 = h_test.ProjectionY().Clone()
                h_test.Delete()
            else:
                h_2 = h_test.Clone()
            if ('TF' in key):
                h_2.Scale(tf_loose_list[cuts])
            if ("Higgs" not in key):
                h_2.Rebin(hprop['hrebin'])
            else:
                print('random')
                h_2 = h_2.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
            # h_2.Rebin(histos_rebin[j])
            h_2.SetFillColor(colors[1])
            h_2.SetFillStyle(0) # hollow hist
            h_2.SetMarkerColor(ROOT.kBlack)
            h_2.SetLineColor(ROOT.kBlack)
            h_2.SetMarkerStyle(8)
            h_2.SetMarkerSize(m_size)
            print(h_0.GetNbinsX(),h_1.GetNbinsX(),h_2.GetNbinsX())
            # quit()
            if key == "AvgMass_mumu_hh_HMassBC_TF":
                h_2.Scale(args.tf)
            leg.AddEntry(h_2, "Run2017","ep")
        # if args.withbkg:
            # print(hs.GetNhists())
        if not args.withbkg:
            hdummy = h_1.Clone()
            hdummy.Reset()
            hs.Add(hdummy)
        if args.withbkg:
            # sum_histogram = None
            # for histo in hs.GetHists():
            #     if sum_histogram is None:
            #         sum_histogram = histo.Clone()
            #     else:
            #         sum_histogram.Add(histo)
        # if "h_recoHiggsMass_BC" not in histname:
            # sf = 1.
            # for i,histo in enumerate(hs.GetHists()):
            #     if "Higgs" not in histname:
            #         continue
            #     data_sum = h_2.Integral()
            #     low_mc_sum = sum_histogram.Integral(sum_histogram.GetBin(0.),sum_histogram.GetBin(122.4))
            #     high_mc_sum = sum_histogram.Integral(sum_histogram.GetBin(127.6),sum_histogram.GetBin(500.))
            #     sf = data_sum/(high_mc_sum+low_mc_sum)
            # print("data/MC: ",sf)
                # histo.Scale(sf)
                # hs_clone.Add(histo) #IDEALLY, calculate this sf and scale all histograms with this sf
            
            hs_clone = ROOT.THStack("hs_clone","stacked hist")
            h_1.Scale(41474.989603894) # NOTE: Do only when NEWNTUPLE for signal sample is used
            for i,histo in enumerate(hs.GetHists()):
                # histo.Scale(sf)
                histo.Scale(41474.989603894) #lumi-scaling
                hs_clone.Add(histo)
            print("LOOK AT INTEGRAL: ", )
            sum_histogram_tmp = None
            for histo in hs_clone.GetHists():
                if sum_histogram_tmp is None:
                    sum_histogram_tmp = histo.Clone()
                else:
                    sum_histogram_tmp.Add(histo)
            data_sum = h_2.Integral()
            low_data_sum = h_2.Integral(h_2.FindBin(0.),h_2.FindBin(122.4))
            high_data_sum = h_2.Integral(h_2.FindBin(127.6),h_2.FindBin(500.))
            low_mc_sum = sum_histogram_tmp.Integral(sum_histogram_tmp.FindBin(0.),sum_histogram_tmp.FindBin(122.4))
            high_mc_sum = sum_histogram_tmp.Integral(sum_histogram_tmp.FindBin(127.6),sum_histogram_tmp.FindBin(500.))
            print("low_data_sum, high_data_sum, low_mc_sum, high_mc_sum: ",low_data_sum,high_data_sum, low_mc_sum, high_mc_sum)
            # h3 = createRatio(h_2, sum_histogram)
            # h_2.GetXaxis().SetLimits(histos_xlow[j],histos_xhigh[j])
            h3=h_2.Clone()
            h3.Divide(sum_histogram_tmp)

        boundary_percent = 0.35
        
        if args.noratio:
            ylength_c = int(2400*(1-boundary_percent+0.15))
            c1 = TCanvas(savename, savename, 2200, ylength_c)
            pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
            pad1.SetTopMargin(0.06)
            if 'Cutflow' in histname:
                pad1.SetBottomMargin(0.25)
            else:
                pad1.SetBottomMargin(0.15)
            pad1.SetLeftMargin(0.16)
            pad1.SetRightMargin(0.04)
            #pad1.SetGridx()
            pad1.Draw()
            # Lower ratio plot is pad2
            c1.cd()
        else:
            c1, pad1, pad2 = createCanvasPads(savename,boundary=boundary_percent)


        # c1 = ROOT.TCanvas("c1", "stacked hists", 1600, 1000)
        
        pad1.cd()
        if args.log:
            # c1.SetLogy()
            pad1.SetLogy()
        #     hs.Clear()
        hs = hs_clone
        hs.Draw("PFC PLC PMC HIST")
        hs.GetXaxis().SetRangeUser(hprop['xlow'],hprop['xhigh'])
        hs.GetXaxis().SetTitle(hprop['label'])
        hs.GetYaxis().SetTitle("Events / "+str(h_1.GetXaxis().GetBinWidth(1)))
        if ("Cutflow" in histname): # to be modified to include trigger separately from met filters
            hs.GetXaxis().ChangeLabel(1,45,-1,-1,-1,-1,"Total")
            hs.GetXaxis().ChangeLabel(2,45,-1,-1,-1,-1,"trigger+MET filter")
            hs.GetXaxis().ChangeLabel(3,45,-1,-1,-1,-1,"#mu p_{T},#eta + m(#mu#mu)#leq5 skim")
            hs.GetXaxis().ChangeLabel(4,45,-1,-1,-1,-1,"h p_{T},#eta + m(hh)#leq5 skim")
            hs.GetXaxis().ChangeLabel(5,45,-1,-1,-1,-1,"m(#mu#muhh)>50 skim")
            hs.GetXaxis().ChangeLabel(6,45,-1,-1,-1,-1,"#mu selection")
            hs.GetXaxis().ChangeLabel(7,45,-1,-1,-1,-1,"di-#mu candidate")
            hs.GetXaxis().ChangeLabel(8,45,-1,-1,-1,-1,"h selection")
            hs.GetXaxis().ChangeLabel(9,45,-1,-1,-1,-1,"di-h candidate")
            hs.GetXaxis().ChangeLabel(10,45,-1,-1,-1,-10,"loose scalar+higgs mass")
            hs.GetXaxis().ChangeLabel(11,45,-1,-1,-1,-11,"tight scalar mass")
            hs.GetXaxis().ChangeLabel(12,45,-1,-1,-1,-12,"tight higgs mass")
            hs.GetXaxis().LabelsOption("v")
            h_1.GetXaxis().LabelsOption("v")
        if ("Cutflow" not in histname):
            if args.log:
                hs.SetMinimum(1)
                h_1.SetMinimum(1)
                ymax = max(hs.GetMaximum(),h_1.GetMaximum())

                if 'AvgMass' in key:
                    hs.SetMaximum(2*1e3)
                    h_1.SetMaximum(2*1e3)
                else:
                    hs.SetMaximum(1e2*ymax)   
                    h_1.SetMaximum(1e2*ymax)  
            else:
                hs.SetMinimum(0.)
                h_1.SetMinimum(0.)
                ymax = max(hs.GetMaximum(),h_1.GetMaximum())
                if "FineBinning" in key:
                    hs.SetMaximum(20)
                    h_1.SetMaximum(20)
                elif "AvgMass_mumu_hh" in key:
                    ymax_special = h_1.GetMaximum()
                    hs.SetMaximum(350)
                    h_1.SetMaximum(350)
                else:
                    hs.SetMaximum(args.yhigh*ymax)
                    h_1.SetMaximum(args.yhigh*ymax)
        else:
            if args.log:
                hs.SetMinimum(1)
                h_1.SetMinimum(1)
                ymax = max(hs.GetMaximum(),h_1.GetMaximum())
                hs.SetMaximum(args.yhigh*ymax)
                h_1.SetMaximum(args.yhigh*ymax)
                

            else:
                hs.SetMinimum(0.)
                h_1.SetMinimum(0.)
                ymax = max(hs.GetMaximum(),h_1.GetMaximum())
                hs.SetMaximum(args.yhigh*h_1.GetMaximum())
                h_1.SetMaximum(args.yhigh*h_1.GetMaximum())
        
        if args.noratio:
            hs.GetXaxis().SetLabelSize(0.045)
            # hs.GetXaxis().SetTitleSize(0.)
            h_1.GetXaxis().SetLabelSize(0.045)
            # h_1.GetXaxis().SetTitleSize(0.)
            hs.GetYaxis().SetLabelSize(0.045)
            h_1.GetYaxis().SetLabelSize(0.045)

            hs.GetXaxis().SetTitleOffset(1.)
            h_1.GetXaxis().SetTitleOffset(1.)
            hs.GetYaxis().SetTitleOffset(1.2)
            h_1.GetYaxis().SetTitleOffset(1.2)
        else:
            hs.GetXaxis().SetLabelSize(0.)
            hs.GetXaxis().SetTitleSize(0.)
            h_1.GetXaxis().SetLabelSize(0.)
            h_1.GetXaxis().SetTitleSize(0.)

            hs.GetYaxis().SetLabelSize(0.055)
            h_1.GetYaxis().SetLabelSize(0.055)

            hs.GetYaxis().SetTitleOffset(1.1)
            h_1.GetYaxis().SetTitleOffset(1.1)

        if args.withdata and "Cutflow" not in histname:
            h_2.Draw("EPSAME")
        h_1.SetLineWidth(5)
        h_1.Draw("HIST SAME")
        # sum_histogram_tmp.Rebin()
        # sum_histogram_tmp.SetMarkerStyle(20)
        ex = np.array([sum_histogram_tmp.GetBinWidth(i)/2 for i in range(1,sum_histogram_tmp.GetNbinsX()+1)])
        ey = np.array([sum_histogram_tmp.GetBinError(i)/2 for i in range(1,sum_histogram_tmp.GetNbinsX()+1)])
        y = np.array([sum_histogram_tmp.GetBinContent(i) for i in range(1,sum_histogram_tmp.GetNbinsX()+1)])
        x = np.array([sum_histogram_tmp.GetBinCenter(i) for i in range(1,sum_histogram_tmp.GetNbinsX()+1)])
        gr_bounds=ROOT.TGraphErrors(len(x),x,y,ex,ey)
        # gr_bounds.SetMarkerStyle(21);gr_bounds.SetMarkerSize(1.25);gr_bounds.SetLineStyle(2);
        gr_bounds.SetFillColor(ROOT.kGray+3);gr_bounds.SetFillStyle(3244);gr_bounds.SetLineWidth(0);gr_bounds.SetLineColor(2);
        ################ UNCOMMENT BELOW TO ADD STAT UNC. ##############
        # gr_bounds.Draw("5 same")
        # leg.AddEntry(gr_bounds,"Stat. Unc. MC","f")
        
        leg.Draw("same")
        pad1.Modified()
        pad1.Update()
        CMS_lumi.cmsText = 'CMS'
        CMS_lumi.writeExtraText = True
        if args.withdata:
            CMS_lumi.extraText = 'Work in Progress'
            CMS_lumi.lumi_13TeV = "41.4 fb^{-1}"
        else:
            CMS_lumi.extraText = 'Work in Progress'
            CMS_lumi.lumi_13TeV = "UL2017 MC"
        # CMS_lumi.lumiTextSize = 0.5
        CMS_lumi.cmsTextSize=1.
        CMS_lumi.CMS_lumi(pad1, 4, 11)
        pad1.Modified()
        pad1.Update()

        if not args.noratio:
            pad2.cd()
            h3.SetMarkerSize(m_size)
            # h3.GetXaxis().SetMoreLogLabels()
            # h3.GetXaxis().SetLimits(histos_xlow[j],histos_xhigh[j])
            print("RANGES h3:",hprop['xlow'],hprop['xhigh'])
            # testing below #
            # h3.GetXaxis().SetLimits(30,xhigh)
            h3.GetXaxis().SetRangeUser(hprop['xlow'],hprop['xhigh'])
            # h3.GetXaxis().SetRange(histos_xlow[j],histos_xhigh[j])
            # h3.GetXaxis().SetTitle(xlabel)
            h3.GetXaxis().SetLabelSize(0.1)
            h3.GetYaxis().SetLabelSize(0.1)
            h3.GetXaxis().SetTitleSize(0.12)
            h3.GetYaxis().SetTitleSize(0.12)
            h3.GetXaxis().SetTitleOffset(1.2)
            h3.GetYaxis().SetTitleOffset(0.6)
            h3.GetYaxis().SetNdivisions(505)
            h3.GetYaxis().SetLabelOffset(hs.GetYaxis().GetLabelOffset())
            h3.GetYaxis().SetTitle("Data/MC")
            h3.GetXaxis().SetTitle(hprop['label'])
            h3.SetMinimum(0.)
            h3.SetMaximum(2)
            pad2.Modified()
            pad2.Update()
            h3.Draw("pe")
            line = ROOT.TLine(hprop['xlow'], 1.,hprop['xhigh'], 1.)
            line.SetLineStyle(7)
            line.SetLineWidth(2)
            line.SetLineColor(kRed)
            line.Draw("same")
        # c1.Modified()
        # c1.Update()
        # if 
        c1.SaveAs(args.out+'/'+dirn+savename+'.png')
        c1.SaveAs(args.out+'/'+dirn+savename+'.pdf')
        # c1.SaveAs(args.out+'/'+dirn+savename+'.root')
        c1.Clear()
    # for i, dataset in enumerate(datasets_bkg):
    #     fdatasets_bkg[i].Close()
    fsig.Close()
    fdata.Close()
            # hs.Clear()

    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
