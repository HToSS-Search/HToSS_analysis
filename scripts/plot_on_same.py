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
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetLineWidth(2)
ROOT.gStyle.SetLegendBorderSize(0)
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

def main():
    ROOT.TH1.AddDirectory(ROOT.kFALSE)

    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    # parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="input", help="Input file name", type=str)
    parser.add_argument("--h1", dest="h1", help="histname 1", type=str)
    parser.add_argument("--h2", dest="h2", help="histname 2", type=str)
    parser.add_argument("--leg1", dest="leg1", help="leg 1", type=str)
    parser.add_argument("--leg2", dest="leg2", help="leg 2", type=str)
    parser.add_argument("--xtitle", dest="xtitle", help="x-axis title", type=str)
    parser.add_argument("--ytitle", dest="ytitle", help="y-axis title", type=str)
    parser.add_argument("--xlow", dest="xlow", help="x-axis range", type=float)
    parser.add_argument("--xhigh", dest="xhigh", help="x-axis range", type=float)
    parser.add_argument("--rebin", dest="rebin", default=1,help="rebin factor", type=int)
    parser.add_argument("--yhigh", dest="yhigh", default=1,help="y-axis multiplicative factor for ymax", type=float)
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    parser.add_argument("--norm", dest="norm", help="true for norming, false by default", action="store_true")
    parser.add_argument("--onlyone", dest="onlyone", help="true for drawing only h1, false by default", action="store_true")
    parser.add_argument("--drawhist", dest="drawhist", help="true for drawing hist not points, false by default", action="store_true")
    parser.add_argument("--new", dest="new", help="true for new histpgrams, false by default", action="store_true")

    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()

    histo_dict = {
        # 'genDimuonDeltaR': {'hname':'h_genDimuonDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDimuonMass': {'hname':'h_genDimuonMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDimuonPt': {'hname':'h_genDimuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genLeadingMuonPt': {'hname':'h_genLeadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genSubleadingMuonPt': {'hname':'h_genSubleadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDipionDeltaR': {'hname':'h_genDipionDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDipionMass': {'hname':'h_genDipionMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDipionPt': {'hname':'h_genDipionPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genLeadingPionPt': {'hname':'h_genLeadingPionPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genSubleadingPionPt': {'hname':'h_genSubleadingPionPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDikaonDeltaR': {'hname':'h_genDikaonDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDikaonMass': {'hname':'h_genDikaonMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDikaonPt': {'hname':'h_genDikaonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genLeadingKaonPt': {'hname':'h_genLeadingKaonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genSubleadingKaonPt': {'hname':'h_genSubleadingKaonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDiChsDeltaR': {'hname':'h_genDiChsDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDiChsMass': {'hname':'h_genDiChsMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genDiChsPt': {'hname':'h_genDiChsPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genLeadingChsPt': {'hname':'h_genLeadingChsPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genSubleadingChsPt': {'hname':'h_genSubleadingChsPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'recoDimuonDeltaR': {'hname':'h_recoDimuonDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'recoDimuonMass': {'hname':'h_recoDimuonMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'recoDimuonPt': {'hname':'h_recoDimuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'recoLeadingMuonPt': {'hname':'h_recoLeadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'recoSubleadingMuonPt': {'hname':'h_recoSubleadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenDimuonDeltaR': {'hname':'h_recoGenDimuonDeltaR','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenDimuonMass': {'hname':'h_recoGenDimuonMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenDimuonPt': {'hname':'h_recoGenDimuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenDimuonEta': {'hname':'h_recoGenDimuonEta','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenLeadingMuonPt': {'hname':'h_recoGenLeadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        #### 'recoGenSubleadingMuonPt': {'hname':'h_recoGenSubleadingMuonPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsPt': {'hname':'h_diChsPt','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsMass': {'hname':'h_diChsMass','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsPt_truth': {'hname':'h_leadingChsPt_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subLeadingChsPt_truth': {'hname':'h_subLeadingChsPt_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsIso_truth': {'hname':'h_leadingChsIso_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsIso_truth': {'hname':'h_subleadingChsIso_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsSumPtCh_truth': {'hname':'h_leadingChsSumPtCh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsSumPtNh_truth': {'hname':'h_leadingChsSumPtNh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsSumPtPh_truth': {'hname':'h_leadingChsSumPtPh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsSumPtPu_truth': {'hname':'h_leadingChsSumPtPu_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsSumPtCh_truth': {'hname':'h_subleadingChsSumPtCh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsSumPtNh_truth': {'hname':'h_subleadingChsSumPtNh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsSumPtPh_truth': {'hname':'h_subleadingChsSumPtPh_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsSumPtPu_truth': {'hname':'h_subleadingChsSumPtPu_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'ChsdelR_truth': {'hname':'h_ChsdelR_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsMass_truth': {'hname':'h_diChsMass_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsPt_truth': {'hname':'h_diChsPt_truth','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'genChsdelR_failed': {'hname':'h_genChsdelR_failed','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsMass_failed': {'hname':'h_diChsMass_failed','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'diChsPt_failed': {'hname':'h_diChsPt_failed','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'leadingChsPt_failed': {'hname':'h_leadingChsPt_failed','label':"",'xlow':0,'xhigh':,'hrebin':20},
        # 'subleadingChsPt_failed': {'hname':'h_subleadingChsPt_failed','label':"",'xlow':0,'xhigh':,'hrebin':20}
        # h_leadingMuonIso
        # h_leadingMuonSumPtCh
        # h_leadingMuonSumPtNh
        # h_leadingMuonSumPtPh
        # h_leadingMuonSumPtPu
        # h_subleadingMuonIso
        # h_subleadingMuonSumPtCh
        # h_subleadingMuonSumPtNh
        # h_subleadingMuonSumPtPh
        # h_subleadingMuonSumPtPu

    }
    # datasets_dict = {
    #     'HToSS_MH125_MS'+args.mass+'_ctauS0':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau = 0mm}",'color':ROOT.kGreen-2},
    # }

    
    

    leg = ROOT.TLegend(0.68, 0.75, 0.95, 0.92)
    # leg.SetFillStyle(0)
    leg.SetNColumns(1)
    leg.SetTextSize(0.027)
    savename=args.out.split('/')[-1]
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

    # hprop = histo_dict[key]
    # histname = hprop['hname']
    # args.out
    # args.input
    # args.h1_name
    # args.h2_name
    # args.xtitle
    # args.ytitle
    # args.xlow
    # args.xhigh
    # args.yhigh
    # args.log
    colour_1=ROOT.kOrange+7
    colour_2=ROOT.kAzure+6
    fsig=ROOT.TFile(args.input, "READ")
    h_1 = fsig.Get(args.h1).Clone()
    h_1.Rebin(args.rebin)
    
    if args.new:
        h_1.Scale(41480)
    if args.norm:
        h_1.Scale(1/h_1.Integral())
        
    h_1.SetFillColor(colour_1)
    h_1.SetFillStyle(0) # hollow hist
    h_1.SetMarkerColor(colour_1)
    h_1.SetLineColor(colour_1)
    h_1.SetLineWidth(3)
    h_1.SetMarkerStyle(20)
    if args.drawhist:
        h_1.SetMarkerSize(0)
    else:
        h_1.SetMarkerSize(m_size)
    h_1.GetXaxis().SetRangeUser(args.xlow,args.xhigh)
    h_1.GetXaxis().SetTitle(args.xtitle)
    h_1.GetYaxis().SetTitle(args.ytitle+" / "+str(h_1.GetXaxis().GetBinWidth(1)))
    h_1.GetXaxis().SetTitleSize(0.05)
    h_1.GetYaxis().SetTitleSize(0.05)
    h_1.GetXaxis().SetLabelSize(0.045)
    h_1.GetYaxis().SetLabelSize(0.045)
    h_1.GetXaxis().SetTitleOffset(1.1)
    h_1.GetYaxis().SetTitleOffset(1.4)
    leg.AddEntry(h_1, args.leg1, "lpe")

    h_2 = fsig.Get(args.h2).Clone()
    h_2.Rebin(args.rebin)
    
    if args.new:
        h_2.Scale(41480)
    if args.norm:
        h_2.Scale(1/h_2.Integral())
        
    h_2.SetFillColor(colour_2)
    h_2.SetFillStyle(0) # hollow hist
    h_2.SetMarkerColor(colour_2)
    h_2.SetLineColor(colour_2)
    h_2.SetLineWidth(3)
    h_2.SetMarkerStyle(21)
    if args.drawhist:
        h_2.SetMarkerSize(0)
    else:
        h_2.SetMarkerSize(m_size)
    
    leg.AddEntry(h_2, args.leg2, "lpe")
    if args.log:
        h_1.SetMinimum(1e-4)
        h_1.SetMaximum(3)
    else:
        h_1.SetMinimum(0.)
        ymax = args.yhigh*max(h_1.GetMaximum(),h_2.GetMaximum())
        h_1.SetMaximum(ymax)
        
    if args.drawhist:
        h_1.Draw("hist e")
    else:
        h_1.Draw("pe")
    if not args.onlyone:
        if args.drawhist:
            h_2.Draw("hist e same")
        else:
            h_2.Draw("pe same")
    pad1.Modified()
    pad1.Update()
    fsig.Close()
    if not args.onlyone:
        leg.Draw("same")
    pad1.Modified()
    pad1.Update()
    if args.new:
        rtext = "#splitline{m_{S}=0.8 GeV}{c#tau = 0.1mm}"
    else:
        rtext = "#splitline{m_{S}=2 GeV}{c#tau = 0.1mm}"
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
    # c1.Modified()
    # c1.Update()
    c1.SaveAs(args.out+'.png')
    # c1.SaveAs(args.out+'.pdf')
    # c1.SaveAs(args.out+'/'+dirn+savename+'.root')
    c1.Clear()
    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
