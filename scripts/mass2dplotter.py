# Standard importts
import os,sys,socket,argparse
import os
import ROOT
import math
from array import array
import numpy as np
from ROOT import TCanvas, TColor, TGaxis, TH1F, TPad
from ROOT import kBlack, kBlue, kRed
import tdrstyle
import CMS_lumi
import re
import yaml
import ctypes

ROOT.gROOT.SetBatch(True)
ReducedBinning = False
correctforMCPU = False
tdrstyle.setTDRStyle()
'''
Still to do 
merge bins at high pt high eta
add - eta side

check impact with tight id
'''
# RooFit
ROOT.gSystem.Load("libRooFit.so")
ROOT.gSystem.Load("libRooFitCore.so")
#ROOT.gROOT.SetStyle("Plain") # Not sure this is needed
ROOT.gSystem.SetIncludePath( "-I$ROOFITSYS/include/" )
# colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
colors = [45, 85]
markers = [20, 21, 22, 33, 47]

def createCanvasPads(savename):
    c = TCanvas(savename, savename, 800, 1000)
    # Upper histogram plot is pad1
    pad1 = TPad("pad1", "pad1", 0, 0.25, 1, 1.0)
    pad1.SetBottomMargin(0.15)  # joins upper and lower plot
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c.cd()  # returns to main canvas before defining pad2
    pad2 = TPad("pad2", "pad2", 0, 0.03, 1, 0.25)
    pad2.SetTopMargin(0.1)  # joins upper and lower plot
    pad2.SetBottomMargin(0.1)
    #pad2.SetGridx()
    pad2.Draw()

    return c, pad1, pad2

def main():
    #ROOT.TH1.AddDirectory(ROOT.kFALSE)

    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-i","--input",  dest="fin",   help="input file", type=str)
    parser.add_argument("--cuts",  dest="cuts",   help="cuts file used", type=str)
    parser.add_argument("-o", "--out",  dest="out",   help="output folder", type=str)
    parser.add_argument("-d", "--dataset",  dest="dataset",   help="name of dataset", type=str)
    # parser.add_argument("-b","--bkg",dest="bkg", help="draw with bkg process, specify file",  default="None",type=str)

    args = parser.parse_args()
    # create required parts
    fcuts = open(args.cuts,'r')
    fcut_pars = yaml.safe_load(fcuts)
    params_lower = ROOT.VecOps.RVec('double')(fcut_pars['cuts']['recoscalar']['lowerbound'])
    params_higher = ROOT.VecOps.RVec('double')(fcut_pars['cuts']['recoscalar']['higherbound'])

    histo_dict = {
        'DiMuonMass_DiChHadMass': {'hname':'h_DiMuonMass_DiChHadMass','xlabel': "m_{#mu#mu} (GeV)", 'ylabel': "m_{h^{+}h^{-}}(GeV)", 'xlow':0.,'xhigh':3., 'ylow':0.,'yhigh':3., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-2,'zhigh':300},
        # 'DiMuonMass_DiChHadMass_HMassSMassBC': {'hname':'h_DiMuonMass_DiChHadMass_HMassSMassBC','xlabel': "m_{#mu#mu} (GeV)", 'ylabel': "m_{h^{+}h^{-}}(GeV)", 'xlow':0.,'xhigh':3., 'ylow':0.,'yhigh':3., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-1,'zhigh':300},
        # 'DiMuonMass_DiChHadMass_HMassBC': {'hname':'h_DiMuonMass_DiChHadMass_HMassBC','xlabel': "m_{#mu#mu} (GeV)", 'ylabel': "m_{h^{+}h^{-}} (GeV)", 'xlow':0.,'xhigh':3., 'ylow':0.,'yhigh':3., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-1,'zhigh':300},
        # 'DiMuonMass_DiChHadMass_SMassBC': {'hname':'h_DiMuonMass_DiChHadMass_SMassBC','xlabel': "m_{#mu#mu} (GeV)", 'ylabel': "m_{h^{+}h^{-}} (GeV)", 'xlow':0.,'xhigh':3., 'ylow':0.,'yhigh':3., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-1,'zhigh':300},
        'DiMuonVtxSignificance_DiChHadVtxSignificance': {'hname':'h_DiMuonVtxSignificance_DiChHadVtxSignificance','xlabel': "L_{xy}^{#mu^{+}#mu^{-}}/#Delta L_{xy}^{#mu^{+}#mu^{-}}", 'ylabel': "L_{xy}^{h^{+}h^{-}}/#Delta L_{xy}^{h^{+}h^{-}}", 'xlow':0.,'xhigh':1000., 'ylow':0.,'yhigh':1000., 'hrebinx':20, 'hrebiny':20,'zlow':1e-2,'zhigh':300},
        # 'DiMuonVtxLxy_DiChHadVtxLxy': {'hname':'h_DiMuonVtxLxy_DiChHadVtxLxy','xlabel': "L_{xy}^{#mu^{+}#mu^{-}} (cm)", 'ylabel': "L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0.,'xhigh':60., 'ylow':0.,'yhigh':60., 'hrebinx':2, 'hrebiny':2,'zlow':1e-2,'zhigh':300},
        # 'DiMuonVtxLxy_DiMuonVtxSigma': {'hname':'h_DiMuonVtxLxy_DiMuonVtxSigma', 'xlabel': "L_{xy}^{#mu^{+}#mu^{-}} (cm)",'ylabel': "#Delta L_{xy}^{#mu^{+}#mu^{-}} (cm)", 'xlow':0.,'xhigh':60., 'ylow':0.,'yhigh':10., 'hrebinx':5, 'hrebiny':1,'zlow':1e-2,'zhigh':300},
        # 'DiChHadVtxLxy_DiChHadVtxSigma': {'hname':'h_DiChHadVtxLxy_DiChHadVtxSigma', 'xlabel': "L_{xy}^{h^{+}h^{-}} (cm)",'ylabel': "#Delta L_{xy}^{h^{+}h^{-}} (cm)", 'xlow':0.,'xhigh':60., 'ylow':0.,'yhigh':10., 'hrebinx':5, 'hrebiny':1,'zlow':1e-2,'zhigh':300}
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
        'HToSS_MH125_MS2_ctau0':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 0mm}"},
        'HToSS_MH125_MS2_ctau1':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 1mm}"},
        'HToSS_MH125_MS2_ctau10':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 10mm}"},
        'HToSS_MH125_MS2_ctau100':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 100mm}"},
        'Run2017':{'type':'blinded_data','label':'Run2017'}
    }


    xleg,yleg = 0.6, 0.2
    leg = ROOT.TLegend(xleg, yleg, 0.8, 0.4)
    leg.SetTextSize(0.027)


    # lowerbound=[-0.05644029416162604, 0.9833371896663816]
    # higherbound=[0.0733982700792378, 1.006827105815873]
    # params_lower = ROOT.VecOps.RVec('double')(fit_rb.GetParameters())
    # params_higher = ROOT.VecOps.RVec('double')(fit_lt.GetParameters())
    func_l = ROOT.TF1("func_l","[1]*x+[0]",0.,5.)
    func_h = ROOT.TF1("func_h","[1]*x+[0]",0.,5.)
    func_l.FixParameter(0,params_lower[0])
    func_l.FixParameter(1,params_lower[1])
    func_h.FixParameter(0,params_higher[0])
    func_h.FixParameter(1,params_higher[1])

    leg.AddEntry(func_h, "upper bounds", "lep")
    leg.AddEntry(func_l, "lower bounds", "lep")

    f_in = ROOT.TFile(args.fin,"READ")
    # dirname = {'prompt', 'displacedmumu', 'displacedhh', 'displaced'}

    # for dirn in dirname:
    for key in histo_dict:
        c2 = ROOT.TCanvas("c2", "c2", 1400, 1200)
        c2.SetLogz()
        c2.SetRightMargin(0.15)
        

        print("file open:",f_in.GetName())
        print(histo_dict[key]['hname'])
        h_2 = f_in.Get(histo_dict[key]['hname'])
        if 'Run' not in args.dataset:
            h_2.Scale(41474.989603894)
        h_2.Rebin2D(histo_dict[key]['hrebinx'],histo_dict[key]['hrebiny'])
        h_2.GetXaxis().SetTitle(histo_dict[key]['xlabel'])
        h_2.GetYaxis().SetTitle(histo_dict[key]['ylabel'])
        # h_2.GetXaxis().GetBinLowEdge(1); 
        h_2.GetZaxis().SetTitle("Events / "+str(h_2.GetXaxis().GetBinWidth(1)))
        h_2.GetZaxis().SetTitleOffset(1.)
        h_2.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        h_2.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
        maxbin = h_2.GetMaximumBin()
        maxval = h_2.GetBinContent(maxbin)
        minbin = h_2.GetMinimumBin()
        minval = h_2.GetBinContent(minbin)
        # h_2.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
        # h_2.GetZaxis().SetRangeUser(9*1e-2,300.)
        h_2.GetZaxis().SetRangeUser(histo_dict[key]['zlow'],histo_dict[key]['zhigh'])

        h_2.GetXaxis().SetLabelSize(0.04)
        h_2.GetYaxis().SetLabelSize(0.04)
        h_2.GetZaxis().SetLabelSize(0.04)
        h_2.GetXaxis().SetTitleSize(0.05)
        h_2.GetYaxis().SetTitleSize(0.05)
        h_2.GetZaxis().SetTitleSize(0.05)

        c2.cd()
        h_2.Draw("colz")
        if "DiMuonMass_DiChHadMass" in key:
            func_l.Draw("same")
            func_h.Draw("same")
            # leg.Draw("same")

        rtext = datasets_dict[args.dataset]['label']
        additional_text = []
        additional_text += [rtext]
        if additional_text:
            nother = len(additional_text)
            # dims = [xleg-0.05, yleg - nother * 0.04 - 0.02, 0.85, 0.65]
            dims = [0.2, 0.8 - nother * 0.04 - 0.02, 0.2, 0.65]
            text = ROOT.TPaveText(*dims + ['NB NDC'])
            text.SetTextFont(42)
            text.SetBorderSize(0)
            text.SetFillColor(0)
            text.SetTextAlign(11)
            text.SetTextSize(0.04)
            for rtext in additional_text:
                text.AddText(rtext)
            text.Draw()

        # text.Draw()
        c2.Modified()
        c2.Update()

        CMS_lumi.cmsText = 'CMS'
        CMS_lumi.writeExtraText = True
        CMS_lumi.extraText = 'Work in progress'
        if 'Run' in args.dataset:
            CMS_lumi.lumi_13TeV = "41.4 fb^{-1}"
        else:
            CMS_lumi.lumi_13TeV = "UL2017 MC"
        # CMS_lumi.lumiTextSize = 0.5
        CMS_lumi.cmsTextSize=1.
        CMS_lumi.CMS_lumi(c2, 4, 11)
        c2.Modified()
        c2.Update()
        # c2.SaveAs(args.out+'/'+dirn+'/'+key+'_'+args.dataset+".png")
        c2.SaveAs(args.out+'/'+key+'_'+args.dataset+".png")
        # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.pdf')
        # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.root')
        c2.Clear()
if __name__ == '__main__':
    main()
