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
    # custom_bins = [0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,\
    #                170,180,190, 200, 210, 220, 230, 240, 250, 260, 270, 280, \
    #                290, 300, 310, 320,330, 340, 350, 360, 370, 380, 390, 400,\
    #                410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520,\
    #                530,540, 550, 560, 570, 580, 590, 600, 625, 650, 675, 700,\
    #                750, 800, 1000, 1600]
    custom_bins = [0,10,20,30,40,50,60,70,80,90,100,110,150,\
                   190, 230, 270, \
                   310, 350, 390, \
                   430, 470, 510, \
                   550, 590, \
                   800, 1600]
    color_t,color_s_r,color_s=ROOT.kBlack,ROOT.kRed, ROOT.kBlue
    xlow,xhigh=0,1600
    ylow,yhigh=1e-9,30
    xlabel="gen. Higgs p_{T} (GeV)"
    # basedir = os.path.join(cwd,'scale_factors/HiggsPtReweighting_tmp')
    # f_target=ROOT.TFile(basedir+"/"+ "ggH_HiggsPtTarget_NNLOPS.root", "READ")
    # h_target=f_target.Get('higgsPt').Clone()
    # h_target=h_target.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
    # # h_target.SetFillColor(color_t)
    # # h_target.SetFillStyle(0) # hollow hist
    # h_target.SetMarkerColor(color_t)
    # h_target.SetLineColor(color_t)
    # h_target.SetLineWidth(2)
    # h_target.SetMarkerStyle(8)
    # h_target.SetMarkerSize(m_size)
    # h_target.GetXaxis().SetRangeUser(xlow,xhigh)
    # # h_target.GetYaxis().SetRangeUser(ylow,yhigh)
    # h_target.GetYaxis().SetLimits(ylow,yhigh)
    # h_target.GetXaxis().SetTitle(xlabel)
    # h_target.GetYaxis().SetTitle("Events")
    
    # f_src_ref=ROOT.TFile(basedir+"/"+ "ggH_HiggsPtSource.root", "READ")
    # h_src_ref=f_src_ref.Get('higgsPt').Clone()
    # h_src_ref=h_src_ref.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
    # h_src_ref.Scale(1/h_src_ref.Integral())
    # # h_target.SetFillColor(color_s_r2)
    # # h_target.SetFillStyle(0) # hollow hist
    # h_src_ref.SetMarkerColor(color_s_r)
    # h_src_ref.SetLineColor(color_s_r)
    # h_src_ref.SetLineWidth(2)
    # h_src_ref.SetMarkerStyle(8)
    # h_src_ref.SetMarkerSize(m_size)
    # h_src_ref.GetXaxis().SetRangeUser(xlow,xhigh)
    # # h_src_ref.GetYaxis().SetRangeUser(ylow,yhigh)
    # h_src_ref.GetYaxis().SetLimits(ylow,yhigh)
    # h_src_ref.GetXaxis().SetTitle(xlabel)
    # h_src_ref.GetYaxis().SetTitle("Events")


    leg = ROOT.TLegend(0.5, 0.65, 0.95, 0.92)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetNColumns(2)
    leg.SetTextSize(0.022)

    f_src=ROOT.TFile(cwd+"/"+ args.input, "READ")
    weightID_dict={\
        "nominal":{'col':ROOT.kMagenta,'label':'nominal'},\
        "muF2":{'col':ROOT.kOrange+3,'label':'#mu_{R},#mu_{F}=(1.0,2.0)'},\
        "muF0p5":{'col':ROOT.kBlack,'label':'#mu_{R},#mu_{F}=(1.0,0.5)'},\
        "muR2":{'col':ROOT.kCyan,'label':'#mu_{R},#mu_{F}=(2.0,1.0)'},\
        "muF2muR2":{'col':ROOT.kViolet-5,'label':'#mu_{R},#mu_{F}=(2.0,2.0)'},\
        "muF0p5muR2":{'col':ROOT.kGreen+2,'label':'#mu_{R},#mu_{F}=(2,0.5)'},\
        "muR0p5":{'col':ROOT.kBlue,'label':'#mu_{R},#mu_{F}=(0.5,1.0)'},\
        "muF2muR0p5":{'col':ROOT.kOrange-8,'label':'#mu_{R},#mu_{F}=(0.5,2)'},\
        "muF0p5muR0p5":{'col':ROOT.kRed,'label':'#mu_{R},#mu_{F}=(0.5,0.5)'}\
    }
    h_src={} #dict of hist
    for key in weightID_dict:
        htmp=f_src.Get('h_genHiggsPt_'+key).Clone()
        htmp=htmp.Rebin(len(custom_bins)-1,"rebinned",array('d',custom_bins))
        # htmp.Scale(1/htmp.Integral())
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
            h_src[key]=hclone
        else:
            h_src[key]=htmp
        # h_src[key]=hclone
        # h_src[key]=htmp
        # h_target.SetFillColor(color_s_r2)
        # h_target.SetFillStyle(0) # hollow hist
        h_src[key].SetMarkerColor(weightID_dict[key]['col'])
        h_src[key].SetLineColor(weightID_dict[key]['col'])
        h_src[key].SetLineWidth(2)
        h_src[key].SetMarkerStyle(1)
        h_src[key].SetMarkerSize(m_size)
        h_src[key].GetXaxis().SetRangeUser(xlow,xhigh)
        h_src[key].GetYaxis().SetLimits(ylow,yhigh)
        h_src[key].GetXaxis().SetTitle(xlabel)
        h_src[key].GetYaxis().SetTitle("Events")
        leg.AddEntry(h_src[key], weightID_dict[key]['label'], "pel")
    

    savename='GenHiggsPt_unc'
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
    # leg.AddEntry(h_target, 'Target', "pel")
    # leg.AddEntry(h_src_ref, 'Source ref.', "pel")
    # leg.AddEntry(h_src, 'Source', "pel")

    h_src['nominal'].GetXaxis().SetLabelSize(0.)
    h_src['nominal'].GetXaxis().SetTitleSize(0.)
    h_src['nominal'].GetYaxis().SetLabelSize(0.055)
    h_src['nominal'].GetYaxis().SetTitleOffset(1.05)
    h_src['nominal'].GetYaxis().SetTitleSize(0.07)
    h_src['nominal'].Draw('pe')
    for key in h_src:
        if 'nominal' in key: 
            continue
        h_src[key].Draw("p e same")
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
    h_ratio={}
    for key in h_src:
        h_ratio[key]=h_src[key].Clone()
        h_ratio[key].Divide(h_src['nominal'])
        h_ratio[key].SetStats(0)
        h_ratio[key].GetXaxis().SetRangeUser(xlow,xhigh)
        h_ratio[key].GetXaxis().SetLabelSize(0.1)
        h_ratio[key].GetYaxis().SetLabelSize(0.1)
        h_ratio[key].GetXaxis().SetTitleSize(0.12)
        h_ratio[key].GetYaxis().SetTitleSize(0.12)
        h_ratio[key].GetXaxis().SetTitleOffset(1.2)
        h_ratio[key].GetYaxis().SetTitleOffset(0.6)
        h_ratio[key].GetYaxis().SetNdivisions(505)
        # hd_src_ref.GetYaxis().SetLabelOffset(hs.GetYaxis().GetLabelOffset())
        h_ratio[key].GetYaxis().SetTitle("Ratio with nominal")
        h_ratio[key].GetXaxis().SetTitle(xlabel)
        h_ratio[key].SetMinimum(0.5)
        h_ratio[key].SetMaximum(1.5)
    pad2.Modified()
    pad2.Update()
    h_ratio['nominal'].Draw('pe')
    for key in h_src:
        if 'nominal' in key: 
            continue
        h_ratio[key].Draw("pe same")
    # line = ROOT.TLine(xlow, 1.,xhigh, 1.)
    # line.SetLineStyle(7)
    # line.SetLineWidth(2)
    # line.SetLineColor(kRed)
    # line.Draw("same")

    c1.Modified()
    c1.Update()
    c1.SaveAs(args.out+'/'+savename+'.png')
    c1.SaveAs(args.out+'/'+savename+'.pdf')
    


    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
