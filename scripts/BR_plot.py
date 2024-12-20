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
import yaml

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


def main():
    # ROOT.TH1.AddDirectory(ROOT.kFALSE)
    f_br = open('BR_xsec_benchmark.yaml','r')
    br_pars=yaml.safe_load(f_br)
    # f = open("params.txt",'w')
    xsec_ggH = br_pars['xsec_ggH']
    br_Haa = br_pars['br_Haa']
    br_a = br_pars['br_a']
    mass,br_amumu,br_akk,br_apipi,br_ahh=[],[],[],[],[]
    for tmp in br_a:
        m=float(tmp.replace('MS','').replace('p','.'))
        amumu=br_a[tmp][2]
        akk=br_a[tmp][1]
        apipi=br_a[tmp][0]
        mass.append(m)
        br_amumu.append(amumu)
        br_akk.append(akk)
        br_apipi.append(apipi)
        br_ahh.append(akk+apipi)
    print(mass)
    print(br_amumu)
    print(br_akk)
    print(br_apipi)
    print(br_ahh)
    

    leg = ROOT.TLegend(0.8, 0.2, 0.95, 0.4)
    leg.SetFillStyle(0)
    # leg.SetNColumns(2)
    leg.SetTextSize(0.027)

    # Create a TGraph object and fill it with the data points
    gr_amumu = ROOT.TGraph(len(mass), array('d', mass), array('d', br_amumu))
    gr_amumu.SetMarkerColor(ROOT.kAzure+6)
    gr_amumu.SetLineColor(ROOT.kAzure+6)
    gr_amumu.SetLineWidth(4)
    gr_amumu.SetMarkerStyle(8)
    gr_amumu.SetMarkerSize(m_size)
    gr_amumu.GetXaxis().SetTitle('m_{S} (GeV)')
    gr_amumu.GetYaxis().SetTitle('Branching Ratio')
    leg.AddEntry(gr_amumu,'#mu^{+}#mu^{-}','pl')
    gr_akk = ROOT.TGraph(len(mass), array('d', mass), array('d', br_akk))
    gr_akk.SetLineWidth(4)
    gr_akk.SetMarkerColor(ROOT.kOrange+1)
    gr_akk.SetLineColor(ROOT.kOrange+1)
    gr_akk.SetMarkerStyle(8)
    gr_akk.SetMarkerSize(m_size)
    gr_akk.GetXaxis().SetTitle('m_{S} (GeV)')
    gr_akk.GetYaxis().SetTitle('Branching Ratio')
    leg.AddEntry(gr_akk,'K^{+}K^{-}','pl')
    gr_apipi = ROOT.TGraph(len(mass), array('d', mass), array('d', br_apipi))
    gr_apipi.SetLineWidth(4)
    gr_apipi.SetMarkerColor(ROOT.kOrange+10)
    gr_apipi.SetLineColor(ROOT.kOrange+10)
    gr_apipi.SetMarkerStyle(8)
    gr_apipi.SetMarkerSize(m_size)
    gr_apipi.GetXaxis().SetTitle('m_{S} (GeV)')
    gr_apipi.GetYaxis().SetTitle('Branching Ratio')
    leg.AddEntry(gr_apipi,'#pi^{+}#pi^{-}','pl')
    
    gr_ahh = ROOT.TGraph(len(mass), array('d', mass), array('d', br_ahh))
    gr_ahh.SetLineWidth(4)
    gr_ahh.SetMarkerColor(ROOT.kBlack)
    gr_ahh.SetLineColor(ROOT.kBlack)
    gr_ahh.SetMarkerStyle(8)
    gr_ahh.SetLineStyle(9)
    gr_ahh.SetMarkerSize(0)
    gr_ahh.GetXaxis().SetTitle('m_{S} (GeV)')
    gr_ahh.GetYaxis().SetTitle('Branching Ratio')
    leg.AddEntry(gr_ahh,'h^{+}h^{-}','pl')

    savename='br_plot'
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
    # # Lower ratio plot is pad2
    c1.cd()
    pad1.cd()

    # Change labels for the x-axis
    x_labels = ["0.4", "0.6", "0.8", "0.9", "1","1.1","1.2", "1.4", "1.6", "1.8", "2"]
    # for i in range(len(x_labels)):
    #     gr_amumu.GetXaxis().SetBinLabel(i + 1, x_labels[i])
    #     gr_akk.GetXaxis().SetBinLabel(i + 1, x_labels[i])
    #     gr_apipi.GetXaxis().SetBinLabel(i + 1, x_labels[i])
    # for i in range(1,len(mass)+1):
    #     gr_amumu.GetXaxis().ChangeLabel(i,-1,-1,-1,-1,-1,str(mass[i-1]))
    #     gr_akk.GetXaxis().ChangeLabel(i,-1,-1,-1,-1,-1,str(mass[i-1]))
    #     gr_apipi.GetXaxis().ChangeLabel(i,-1,-1,-1,-1,-1,str(mass[i-1]))

    # pad1.SetLogx()
    pad1.SetLogy()
    # gr_amumu.GetXaxis().SetMoreLogLabels()
    
    gr_amumu.GetXaxis().SetRangeUser(0.3,2.1)
    gr_amumu.GetXaxis().SetLimits(0.3,2.1)
    gr_amumu.SetMinimum(1e-3)
    gr_amumu.SetMaximum(2)
    gr_amumu.GetXaxis().SetTitleSize(0.05)
    gr_amumu.GetYaxis().SetTitleSize(0.05)
    gr_amumu.GetXaxis().SetLabelSize(0.045)
    gr_amumu.GetYaxis().SetLabelSize(0.045)
    gr_amumu.GetXaxis().SetTitleOffset(1.5)
    gr_amumu.GetYaxis().SetTitleOffset(1.4)
    # gr_amumu.GetXaxis().SetNdivisions(502,ROOT.kFALSE)

    # gr_amumu.GetXaxis().ChangeLabel(1,-1,-1,-1,-1,-1,"0.4")
    # gr_amumu.GetXaxis().ChangeLabel(2,-1,-1,-1,-1,-1,"0.6")
    # gr_amumu.GetXaxis().ChangeLabel(3,-1,-1,-1,-1,-1,"0.8")
    # # gr_amumu.GetXaxis().ChangeLabel(6,-1,-1,-1,-1,-1,"0.9")
    # gr_amumu.GetXaxis().ChangeLabel(,-1,-1,-1,-1,-1,"1")
    # # gr_amumu.GetXaxis().ChangeLabel(6,-1,-1,-1,-1,-1,"1.1")
    # # gr_amumu.GetXaxis().ChangeLabel(7,-1,-1,-1,-1,-1,"1.2")
    # # gr_amumu.GetXaxis().ChangeLabel(8,-1,-1,-1,-1,-1,"1.4")
    # # gr_amumu.GetXaxis().ChangeLabel(9,-1,-1,-1,-1,-1,"1.6")
    # # gr_amumu.GetXaxis().ChangeLabel(10,-1,-1,-1,-1,-1,"1.8")
    # gr_amumu.GetXaxis().ChangeLabel(8,-1,-1,-1,-1,-1,"2")

    gr_amumu.Draw("apl")
    gr_apipi.Draw("pl same")
    gr_akk.Draw("pl same")
    gr_ahh.Draw("pl same")
    leg.Draw("same")
    pad1.Modified()
    pad1.Update()
    c1.SaveAs(savename+'.png')
    # c1.SaveAs(args.out+'/'+savename+'.pdf')
    # # c1.SaveAs(args.out+'/'+dirn+savename+'.root')
    # c1.Clear()
    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
