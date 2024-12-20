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
ROOT.gStyle.SetErrorX()
ROOT.gSystem.SetIncludePath( "-I$ROOFITSYS/include/" )

colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
markers = [20, 21, 22, 33, 47]
m_size = 1
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
    # ROOT.TH1.Sumw2()


    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    # parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="input", help="Input directory name", type=str)
    # parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    # parser.add_argument("-c","--category", dest="category",help="prompt/displacedmumu/displacedhh/displaced", type=str)
    # parser.add_argument("--tf", dest="tf", default=1,help="Transfer factor depending on CR - Check BkgEst.py", type=float)
    # parser.add_argument("--yhigh", dest="yhigh", default=500,help="y-axis multiplicative factor for ymax", type=float)
    parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    parser.add_argument("--coarse", dest="coarse", help="true for plotting with coarse bins, false by default", action="store_true")
    parser.add_argument("--dividebywidth", dest="dividebywidth", help="true for dividing with bin width, false by default", action="store_true")
    # parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
    # add an option to plot just one plot accessible name in histo_dict; change savename accordingly
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()



    # histo_dict = {
    #     'genHiggsPt': {'hname':'','label':"L_{xy}^{S} (cm)", 'xlow':0,'xhigh':60,'hrebin':5},
    # }

    custom_a, custom_b = np.arange(0.,80,0.5), np.arange(80,200,2.5) # change accordingly when higgs hist is modified
    # custom_bins = np.unique(np.append(custom_a,custom_b))
    if not args.coarse:
        custom_bins = [0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,\
                    170,180,190, 200, 210, 220, 230, 240, 250, 260, 270, 280, \
                    290, 300, 310, 320,330, 340, 350, 360, 370, 380, 390, 400,\
                    410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520,\
                    530,540, 550, 560, 570, 580, 590, 600, 625, 650, 675, 700,\
                    750, 800, 1000, 1600]
    else:
        custom_bins = [0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,\
                    170,180,190, 230, 270, \
                    310, 350, 390, \
                    430, 470, 510, \
                    550, 590, 650, 700,\
                    800, 1600]
    color_t,color_s_r,color_s=ROOT.kBlack,ROOT.kRed, ROOT.kBlue
    xlow,xhigh=0,1600
    ylow,yhigh=1e-9,30
    xlabel="gen. Higgs p_{T} (GeV)"
    basedir = os.path.join(cwd,'scale_factors/HiggsPtReweighting_tmp')
    f_target=ROOT.TFile(basedir+"/"+ "ggH_HiggsPtTarget_NNLOPS.root", "READ")
    htmp=f_target.Get('higgsPt').Clone()
    htmp=htmp.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
    hclone = htmp.Clone()
    hclone.Sumw2()
    for bin_idx in range(1, htmp.GetNbinsX() + 1):
        # Get the bin content and bin width
        bin_content = htmp.GetBinContent(bin_idx)
        bin_width = htmp.GetBinWidth(bin_idx)
        # Avoid division by zero (if bin width is 0, skip)
        if bin_width != 0:
            new_bin_content = bin_content / bin_width
            hclone.SetBinContent(bin_idx, new_bin_content)
    if args.dividebywidth:
        h_target=hclone
    else:
        h_target=htmp
    # h_target.SetFillColor(color_t)
    # h_target.SetFillStyle(0) # hollow hist
    h_target.SetMarkerColor(color_t)
    h_target.SetLineColor(color_t)
    h_target.SetLineWidth(2)
    h_target.SetMarkerStyle(8)
    h_target.SetMarkerSize(m_size)
    h_target.GetXaxis().SetRangeUser(xlow,xhigh)
    # h_target.GetYaxis().SetRangeUser(ylow,yhigh)
    h_target.GetYaxis().SetLimits(ylow,yhigh)
    h_target.GetXaxis().SetTitle(xlabel)
    h_target.GetYaxis().SetTitle("Events")
    
    f_src_ref=ROOT.TFile(basedir+"/"+ "ggH_HiggsPtSource.root", "READ")
    htmp=f_src_ref.Get('higgsPt').Clone()
    htmp=htmp.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
    htmp.Scale(1/htmp.Integral())
    hclone = htmp.Clone()
    for bin_idx in range(1, htmp.GetNbinsX() + 1):
        # Get the bin content and bin width
        bin_content = htmp.GetBinContent(bin_idx)
        bin_width = htmp.GetBinWidth(bin_idx)
        # Avoid division by zero (if bin width is 0, skip)
        if bin_width != 0:
            new_bin_content = bin_content / bin_width
            hclone.SetBinContent(bin_idx, new_bin_content)
    if args.dividebywidth:
        h_src_ref=hclone
    else:
        h_src_ref=htmp
    
    # h_target.SetFillColor(color_s_r2)
    # h_target.SetFillStyle(0) # hollow hist
    h_src_ref.SetMarkerColor(color_s_r)
    h_src_ref.SetLineColor(color_s_r)
    h_src_ref.SetLineWidth(2)
    h_src_ref.SetMarkerStyle(8)
    h_src_ref.SetMarkerSize(m_size)
    h_src_ref.GetXaxis().SetRangeUser(xlow,xhigh)
    # h_src_ref.GetYaxis().SetRangeUser(ylow,yhigh)
    h_src_ref.GetYaxis().SetLimits(ylow,yhigh)
    h_src_ref.GetXaxis().SetTitle(xlabel)
    h_src_ref.GetYaxis().SetTitle("Events")


    f_src=ROOT.TFile(cwd+"/"+ args.input, "READ")
    # htmp=f_src.Get('h_genHiggsPt_nominal').Clone()
    htmp=f_src.Get('h_genHiggsPt').Clone()
    # h_src=f_src.Get('h_genHiggsPt').Clone()
    htmp=htmp.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
    htmp.Scale(1/htmp.Integral())
    hclone = htmp.Clone()
    for bin_idx in range(1, htmp.GetNbinsX() + 1):
        # Get the bin content and bin width
        bin_content = htmp.GetBinContent(bin_idx)
        bin_width = htmp.GetBinWidth(bin_idx)
        # Avoid division by zero (if bin width is 0, skip)
        if bin_width != 0:
            new_bin_content = bin_content / bin_width
            hclone.SetBinContent(bin_idx, new_bin_content)
    if args.dividebywidth:
        h_src=hclone
    else:
        h_src=htmp
    # h_target.SetFillColor(color_s_r2)
    # h_target.SetFillStyle(0) # hollow hist
    h_src.SetMarkerColor(color_s)
    h_src.SetLineColor(color_s)
    h_src.SetLineWidth(2)
    h_src.SetMarkerStyle(8)
    h_src.SetMarkerSize(m_size)
    h_src.GetXaxis().SetRangeUser(xlow,xhigh)
    # h_src_ref.GetYaxis().SetRangeUser(ylow,yhigh)
    h_src.GetYaxis().SetLimits(ylow,yhigh)
    h_src.GetXaxis().SetTitle(xlabel)
    h_src.GetYaxis().SetTitle("Events")


    leg = ROOT.TLegend(0.5, 0.65, 0.95, 0.92)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetNColumns(2)
    leg.SetTextSize(0.03)
    savename='GenHiggsPt_status_22'
    if args.coarse:
        savename=savename+"_coarse"
    if args.dividebywidth:
        savename=savename+"_dividebywidth"


    boundary_percent = 0.35
    c1, pad1, pad2 = createCanvasPads(savename,boundary=boundary_percent)
    # ylength_c = int(2400*(1-boundary_percent+0.15))
    # c1 = TCanvas(savename, savename, 2200, ylength_c)
    # pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
    # pad1.SetTopMargin(0.06)
    # pad1.SetBottomMargin(0.15)
    # pad1.SetLeftMargin(0.16)
    # pad1.SetRightMargin(0.04)
    #pad1.SetGridx()
    # pad1.Draw()
    # Lower ratio plot is pad2
    
    c1.cd()
    pad1.cd()
    if args.log:
        pad1.SetLogy()
    leg.AddEntry(h_target, 'NNLOPS', "pel")
    leg.AddEntry(h_src_ref, 'Source EXO-21-008', "pel")
    leg.AddEntry(h_src, 'Source', "pel")

    h_target.GetXaxis().SetLabelSize(0.)
    h_target.GetXaxis().SetTitleSize(0.)
    h_target.GetYaxis().SetLabelSize(0.055)
    h_target.GetYaxis().SetTitleOffset(1.05)
    h_target.GetYaxis().SetTitleSize(0.07)



    h_target.Draw("pe")
    h_src_ref.Draw("p e same")
    h_src.Draw("pe same")
    leg.Draw("same")

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

    pad2.cd()
    hd_src_ref=h_target.Clone()
    hd_src_ref.Divide(h_src_ref)
    hd_src_ref.SetStats(0)
    hd_src_ref.SetLineColor(color_s_r)
    hd_src_ref.SetMarkerColor(color_s_r)
    hd_src=h_target.Clone()
    hd_src.Divide(h_src)
    hd_src.SetStats(0)
    hd_src.SetLineColor(color_s)
    hd_src.SetMarkerColor(color_s)
    # h3.GetXaxis().SetMoreLogLabels()
    # h3.GetXaxis().SetLimits(histos_xlow[j],histos_xhigh[j])
    # testing below #
    # h3.GetXaxis().SetLimits(30,xhigh)
    hd_src_ref.GetXaxis().SetRangeUser(xlow,xhigh)
    # h3.GetXaxis().SetRange(histos_xlow[j],histos_xhigh[j])
    # h3.GetXaxis().SetTitle(xlabel)
    hd_src_ref.GetXaxis().SetLabelSize(0.1)
    hd_src_ref.GetYaxis().SetLabelSize(0.1)
    hd_src_ref.GetXaxis().SetTitleSize(0.12)
    hd_src_ref.GetYaxis().SetTitleSize(0.12)
    hd_src_ref.GetXaxis().SetTitleOffset(1.2)
    hd_src_ref.GetYaxis().SetTitleOffset(0.6)
    hd_src_ref.GetYaxis().SetNdivisions(505)
    # hd_src_ref.GetYaxis().SetLabelOffset(hs.GetYaxis().GetLabelOffset())
    hd_src_ref.GetYaxis().SetTitle("NNLOPS/Source")
    hd_src_ref.GetXaxis().SetTitle(xlabel)
    hd_src_ref.SetMinimum(0.)
    hd_src_ref.SetMaximum(2)
    pad2.Modified()
    pad2.Update()
    hd_src_ref.Draw("pe")
    hd_src.Draw("pe same")
    line = ROOT.TLine(xlow, 1.,xhigh, 1.)
    line.SetLineStyle(7)
    line.SetLineWidth(2)
    line.SetLineColor(kRed)
    line.Draw("same")

    c1.Modified()
    c1.Update()
    c1.SaveAs(args.out+'/'+savename+'.png')
    c1.SaveAs(args.out+'/'+savename+'.pdf')
    


            # h_1.GetXaxis().SetTitleSize(0.05)
            # h_1.GetYaxis().SetTitleSize(0.05)
            # h_1.GetXaxis().SetLabelSize(0.045)
            # h_1.GetYaxis().SetLabelSize(0.045)
            # h_1.GetXaxis().SetTitleOffset(1.1)
            # h_1.GetYaxis().SetTitleOffset(1.4)
    # f_src=

    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
