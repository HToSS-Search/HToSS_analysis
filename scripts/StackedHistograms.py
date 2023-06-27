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
colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
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

def createRatio(h1, h2):
    npts = h1.GetN()
    sfx=[None]*npts
    sfx_err=[None]*npts
    sfy=[None]*npts
    sfy_err=[None]*npts

    for i in range(npts):
        # mcEff, dataEff = 0., 0.
        # h2.GetPoint(i,mcEff,sfx[i])
        # h1.GetPoint(i,dataEff,sfx[i])
        # mcErr, dataErr = h2.GetErrorY(i), h1.GetErrorY(i)
        # sfx[i] = h2.GetPointX(i)
        mcEff, mcErr, dataEff, dataErr = h2.GetY()[i], h2.GetErrorY(i), h1.GetY()[i], h1.GetErrorY(i)
        sfx[i] = h2.GetX()[i]
        sfx_err[i] = h2.GetErrorX(i)
        sfy[i] = dataEff/mcEff if mcEff else 0.0
        sfy_err[i] = 0.0
        if dataEff and mcEff:
            sfy_err[i] = sfy[i] * ((dataErr / dataEff)**2 + (mcErr / mcEff)**2)**0.5
    h3 = ROOT.TGraphErrors(npts,array('d',sfx),array('d',sfy),array('d',sfx_err),array('d',sfy_err))
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
    parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctau0 or HtoSS_MS2_ctau1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="input", help="Input directory name", type=str)
    parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    parser.add_argument("-d","--data", dest="withdata", help="true for plotting with data, false by default", action="store_true")
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()

    # datasets_bkg = ["QCD_MuEnrichedPt5", "wPlusJets", "DYJetsToLL", "ttbar" , "VV", "singleTop", "ttV", "VVV"]
    datasets_data = ["Run2017"]
    datasets_bkg = ["VVV","ttV","singleTop","VV","ttbar" ,"DYJetsToLL","wPlusJets","QCD_MuEnrichedPt5"]
    datasets = ["HtoSS_MS2_ctau0","HtoSS_MS2_ctau1","HtoSS_MS2_ctau10","HtoSS_MS2_ctau100"]
    # datasets_label = ["H#rightarrow aa m_{a}=2 GeV, c#tau = 0","QCD", "W+jets", "Z+jets", "t#bar{t}", "VV", "Single top", "t#bar{t}V", "VVV"]
    datasets_label = ["H#rightarrow aa m_{a}=2 GeV, c#tau = 0","VVV", "t#bar{t}V", "Single top", "VV", "t#bar{t}", "Z+jets", "W+jets","QCD"]
    histos = ["h_leadingMuonRelIso", "h_subleadingMuonRelIso","h_leadingChHadRelIso","h_subleadingChHadRelIso",\
              "h_leadingMuonSumPtCh","h_subleadingMuonSumPtCh","h_leadingChHadSumPtCh","h_subleadingChHadSumPtCh",\
              "h_leadingMuonSumPtNh","h_subleadingMuonSumPtNh","h_leadingChHadSumPtNh","h_subleadingChHadSumPtNh", \
              "h_leadingMuonSumPtPh","h_subleadingMuonSumPtPh", "h_leadingChHadSumPtPh", "h_subleadingChHadSumPtPh",\
              "h_DiChHadPt", "h_DiChHadMass", "h_DiMuonPt", "h_DiMuonMass",\
              "h_recoHiggsMass", "h_DiChHadVtxLxy", "h_DiMuonVtxLxy",\
              "h_DiMuonVtxSignificance", "h_DiChHadVtxSignificance",\
              "h_DiMuonVtxSigma", "h_DiChHadVtxSigma",\
              "Cutflow","h_AvgMass_mumu_hh"]
    histos_label = ["leading #mu PF Rel. Isolation", "subleading #mu PF Rel. Isolation", "leading h^{#pm} PF Rel. Isolation", "subleading h^{#pm} PF Rel. Isolation",\
                    "leading #mu #Sigma p_{T}(ch. had.)", "subleading #mu #Sigma pp_{T}(ch. had.)", "leading h^{#pm} #Sigma p_{T}(ch. had.)", "subleading h^{#pm} #Sigma p_{T}(ch. had.)",\
                    "leading #mu #Sigma p_{T}(nt. had.)", "subleading #mu #Sigma p_{T}(nt. had.)", "leading h^{#pm} #Sigma p_{T}(nt. had.)", "subleading h^{#pm} #Sigma p_{T}(nt. had.)",\
                    "leading #mu #Sigma p_{T}(#gamma)", "subleading #mu #Sigma p_{T}(#gamma)", "leading h^{#pm} #Sigma p_{T}(#gamma)", "subleading h^{#pm} #Sigma p_{T}(#gamma)",\
                    "p_{T,h^{+}h^{-}}", "m_{h^{+}h^{-}}", "p_{T,#mu#mu}", "m_{#mu#mu}",\
                    "m_{h^{+}h^{-}#mu^{+}#mu^{-}}", "L_{xy}^{h^{+}h^{-}} (cm)", "L_{xy}^{#mu^{+}#mu^{-}} (cm)",\
                    "L_{xy}^{#mu^{+}#mu^{-}}/#Delta L_{xy}^{#mu^{+}#mu^{-}}","L_{xy}^{h^{+}h^{-}}/#Delta L_{xy}^{h^{+}h^{-}}",\
                    "#Delta L_{xy}^{#mu^{+}#mu^{-}}","#Delta L_{xy}^{h^{+}h^{-}}",\
                    "","0.5*(m_{hh}+m_{#mu#mu})"]
    histos_xlow = [0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0,0,0,0,110,0,0,0,0,0,0,2,0]
    histos_xhigh= [10.,10.,10.,10.,100.,100.,100.,100.,100.,100.,100.,100.,100.,100.,100.,100.,100,10,100,10,140,100,100,1000,1000,10,10,9,5]
    histos_rebin = [40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,5,5,5,5,1,5,5,20,20,1,1,1,4]
    custom_a, custom_b, custom_c = np.arange(100,122.51,0.2), np.arange(122.5,127.51,0.2), np.arange(127.5,200.01,0.2) # change accordingly when higgs hist is modified
    custom_bins = np.unique(np.append(np.append(custom_a,custom_b),custom_c))
    print(custom_bins)
    #histos_rebin = [1,1,1,1,1,1,1,1,1,1,1,1,1,1]

    #fh_1 = ROOT.TFile("output_"+datasets[0]+".root", "READ")
    #h_0 = [None] * len(datasets)
    # ROOT.gStyle.SetPalette(ROOT.kBird)
    # ROOT.TColor.InvertPalette()
    for j, histname in enumerate(histos):
        # if (j>=4): continue
        leg = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
        hs = ROOT.THStack("hs","stacked hist")
        c1 = ROOT.TCanvas("c1", "stacked hists", 1600, 1000)
        if args.log:
            c1.SetLogy()
        #c1.Range(0,10,10.,10000)
        #print(i)
        for i, dataset in enumerate(datasets_bkg):
            fh = ROOT.TFile(args.input+"/"+"output_" + dataset + ".root", "READ")
            print(fh.GetName())
            print(histname)
            #print(hs[0].GetName())
            h_0 = fh.Get(histname).Clone()
            #h_0.SetDirectory(0)
            if ("Higgs" not in histname):
                h_0.Rebin(histos_rebin[j])
            else:
                print ("ENTERS HERE")
                h_0 = h_0.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
            # h_0.SetFillColor(colors[i+2])
            # h_0.SetMarkerColor(colors[i+2])
            # h_0.SetLineColor(colors[i+2])
            # h_0.SetFillStyle(1) # hollow hist
            h_0.SetMinimum(10.)
            hs.Add(h_0)
            leg.AddEntry(h_0, datasets_label[i+1], "f")
            fh.Close()
        sig_ind = np.where(np.array(datasets) == args.sig)
        fsig = ROOT.TFile(args.input+"/"+"output_" + datasets[sig_ind[0].item()] + ".root", "READ")
        h_1 = fsig.Get(histname).Clone()
        if args.mult != 1:
            h_1.Scale(args.mult)
        if ("Higgs" not in histname):
            h_1.Rebin(histos_rebin[j])
        else:
            h_1 = h_1.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
        h_1.SetFillColor(colors[1])
        h_1.SetFillStyle(0) # hollow hist
        h_1.SetMarkerColor(colors[1])
        h_1.SetLineColor(colors[1])
        h_1.SetMarkerStyle(8)
        h_1.SetMarkerSize(0.3)
        if args.mult != 1:
            leg.AddEntry(h_1, "("+datasets_label[0]+")*"+str(args.mult), "lep")
        else:
            leg.AddEntry(h_1, datasets_label[0], "lep")
        fdata = ROOT.TFile(args.input+"/"+"output_" + datasets_data[0] + ".root", "READ")
        h_2 = fdata.Get(histname).Clone()
        if ("Higgs" not in histname):
            h_2.Rebin(histos_rebin[j])
        else:
            h_2 = h_2.Rebin(len(custom_bins)-1,"h_recoHiggsMass_rebinned",custom_bins)
        # h_2.Rebin(histos_rebin[j])
        h_2.SetFillColor(colors[1])
        h_2.SetFillStyle(0) # hollow hist
        h_2.SetMarkerColor(ROOT.kBlack)
        h_2.SetLineColor(ROOT.kBlack)
        h_2.SetMarkerStyle(8)
        h_2.SetMarkerSize(1.)
        leg.AddEntry(h_2, "Run2017","ep")
        print(hs.GetNhists())
        hs.Draw("PFC PLC PMC HIST")
        hs.GetXaxis().SetRangeUser(histos_xlow[j],histos_xhigh[j])
        hs.GetXaxis().SetTitle(histos_label[j])
        hs.GetYaxis().SetTitle("Events")
        leg.Draw("same")
        # hs.GetYaxis().SetLimits(10.,10000)
        # hs.GetYaxis().SetRangeUser(10.,10000)
        # h_1.GetYaxis().SetLimits(10.,10000)
        # h_1.GetYaxis().SetRangeUser(10.,10000)
        if ("Cutflow" not in histname):
            if args.log:
                hs.SetMinimum(0.01)
                h_1.SetMinimum(0.01)
                hs.SetMaximum(100000.)
                h_1.SetMaximum(100000.)
            else:
                hs.SetMinimum(0.)
                h_1.SetMinimum(0.)
                hs.SetMaximum(5000.)
                h_1.SetMaximum(5000.)

        else:
            hs.SetMinimum(1)
            h_1.SetMinimum(1)
            hs.SetMaximum(1e9)
            h_1.SetMaximum(1e9)

        h_1.Draw("HIST SAME")
        h_2.Draw("EPSAME")
        c1.Modified()
        c1.Update()
        CMS_lumi.cmsText = 'CMS'
        CMS_lumi.writeExtraText = True
        CMS_lumi.extraText = 'Simulation'
        CMS_lumi.lumi_13TeV = "UL2017 MC, #sqrt{s} = 13TeV"
        # CMS_lumi.lumiTextSize = 0.5
        CMS_lumi.CMS_lumi(c1, 4, 11)
        c1.Modified()
        c1.Update()
        savename = histname
        c1.SaveAs(args.out+'/'+savename+'.png')
        c1.SaveAs(args.out+'/'+savename+'.pdf')
        c1.SaveAs(args.out+'/'+savename+'.root')
        c1.Clear()
        # hs.Clear()

    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
