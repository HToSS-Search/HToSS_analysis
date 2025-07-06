import ROOT
# from config import plot_dir,dc_dir
import tdrstyle
import CMS_lumi
import os,sys,socket,argparse
import yaml
import shutil
import numpy as np
import csv
import ctypes

import gc ##### IMPORTANT: fix for error with ROOT + python segfaults -> Something to do with memory allocation
##### Above fixed maybe with newer versions -> issue because of canvases in loop


ROOT.gROOT.SetBatch(True)
tdrstyle.setTDRStyle()
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetLineWidth(2)
ROOT.gStyle.SetOptStat(0)
lumi_scale = {'UL2016_APV': 19500, 'UL2016': 16800,'UL2017':  41480,'UL2018': 59830 } #in pb-1
extraInfo = False
extraPlots = False

# RooFit
# ROOT.gSystem.Load("libRooFit.so")
# ROOT.gSystem.Load("libRooFitCore.so")
# #ROOT.gROOT.SetStyle("Plain") # Not sure this is needed
# ROOT.gSystem.SetIncludePath( "-I$ROOFITSYS/include/" )

def createCanvasPads(savename, boundary=0.25):
    # tdrStyle.SetPadTopMargin(0.05)
    # tdrStyle.SetPadBottomMargin(0.13)
    # tdrStyle.SetPadLeftMargin(0.16)
    # tdrStyle.SetPadRightMargin(0.02)
    ylength_c = 2400
    c = ROOT.TCanvas(savename, savename, 2200, ylength_c)
    # Upper histogram plot is pad1
    pad1 = ROOT.TPad("pad1", "pad1", 0, boundary, 1, 1.0)
    pad1.SetTopMargin(0.07)
    pad1.SetBottomMargin(0.05)  # joins upper and lower plot
    pad1.SetLeftMargin(0.16)
    pad1.SetRightMargin(0.02)
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c.cd()  # returns to main canvas before defining pad2
    pad2 = ROOT.TPad("pad2", "pad2", 0, 0, 1, boundary+0.01)
    pad2.SetTopMargin(0.)  # joins upper and lower plot
    pad2.SetBottomMargin(0.13/boundary)
    pad2.SetLeftMargin(0.16)
    pad2.SetRightMargin(0.02)

    #pad2.SetGridx()
    pad2.Draw()

    return c, pad1, pad2

def LandauFit(lumi_factor,hist, output,mass_low, mass_high, htype='kaon', masspoint_str="2",suffix="", conv=True, fixparams=False,me=-1,sig=-1):
    print("Performing a fit using a Landau")
    print(htype)
    # Declare the observable mean, and import the histogram to a RooDataHist

    # tmp_sigma, tmp_mean = hist.GetRMS(), hist.GetMean()
    xlabel = "0.5*(m_{hh}+m_{#mu#mu}) (GeV)"
    mass = ROOT.RooRealVar("mass", xlabel, 0., 3.)
    dh_hist = ROOT.RooDataHist("dh_hist", "dh_hist", ROOT.RooArgList(mass),
                                    ROOT.RooFit.Import(hist))

    # plot the data hist with error from sum of weighted events
    frame = mass.frame(ROOT.RooFit.Title("AvgMass"))
    dh_hist.plotOn(frame, ROOT.RooFit.DataError(ROOT.RooAbsData.SumW2), ROOT.RooFit.MarkerSize(1), ROOT.RooFit.MarkerColor(1), ROOT.RooFit.LineColor(1), ROOT.RooFit.Name("hist"))

    if htype=="kaon":
            
        tmp_mean = hist.GetBinLowEdge(hist.GetMaximumBin())
        tmp_sigma = 0.1
    else:
        tmp_mean = hist.GetBinLowEdge(hist.GetMaximumBin())
        tmp_sigma = 0.5
    if me!=-1 and sig!=-1:
        tmp_mean = me
        tmp_sigma = sig
    # tmp_mean = 0
    # create a Landau pdf
    if htype=='kaon':
        landau_mean = ROOT.RooRealVar("lmean", "lmean", tmp_mean, tmp_mean-0.3, tmp_mean+0.3)
        landau_sigma = ROOT.RooRealVar("lsigma", "lsigma", tmp_sigma, 0.05, 0.3)
    else:
        landau_mean = ROOT.RooRealVar("lmean", "lmean", tmp_mean, tmp_mean-3, tmp_mean+3)
        landau_sigma = ROOT.RooRealVar("lsigma", "lsigma", tmp_sigma, 0.05, 3)

    if fixparams:
        landau_mean.setConstant(True)
        landau_sigma.setConstant(True)
    landau = ROOT.RooLandau("landau", "landau", mass, landau_mean, landau_sigma)

    # create a simple gaussian pdf
    gauss_mean = ROOT.RooRealVar("mean", "mean", 0, -0.5, 0.5)
    gauss_sigma = ROOT.RooRealVar("sigma jer", "sigma gauss", 0.1, 0.05, 0.3)
    gauss = ROOT.RooGaussian("gauss", "gauss", mass, gauss_mean, gauss_sigma)

    lanxg = ROOT.RooFFTConvPdf("lanxg","landau x gauss" ,mass,landau, gauss)
    lanxg.setBufferFraction(0.7)
    
    nentries=hist.Integral()
    n=ROOT.RooRealVar("n", "n", 0, nentries)
    if conv:
        extpdf_lanxg = ROOT.RooExtendPdf("extpdf_lanxg", "extpdf_lanxg", lanxg, n)
    else:
        extpdf_lanxg = ROOT.RooExtendPdf("extpdf_lanxg", "extpdf_lanxg", landau, n)

    alp=0.3
    # minX, maxX = XPercentInterval(hist,alp/2,verbose=True) #removing x% from each side
    # width = maxX - minX
    if htype=='kaon':
        minX, maxX = 1,2.7
    else:
        minX, maxX = 0.3,2.7

    extpdf_lanxg.fitTo(dh_hist,ROOT.RooFit.Save(),ROOT.RooFit.SumW2Error(True) ,ROOT.RooFit.Range(minX,maxX))

    set_mass = ROOT.RooArgSet(mass)
    mass.setRange("atMass", mass_low, mass_high )
    integral_atMass = extpdf_lanxg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("atMass")).getVal()
    integ_atmass = integral_atMass*n.getVal()
    print(integ_atmass)

    extpdf_lanxg.plotOn(frame, ROOT.RooFit.Name("fit"))

    # gauss.fitTo(dh_hist,ROOT.RooFit.Save(),ROOT.RooFit.SumW2Error(True) ,ROOT.RooFit.Range(minX,maxX))
    # gauss.plotOn(frame, ROOT.RooFit.Name("fit"))


    frame.SetMaximum(frame.GetMaximum() * 1.25)
    frame.SetMinimum(0)
    frame.getAttMarker().SetMarkerColor(ROOT.kBlue)
    frame.getAttMarker().SetMarkerSize(1.5)
    frame.getAttLine().SetLineColor(ROOT.kBlue)

    # minXl,maxXl = float(masspoint)-2*gauss_sigma.getVal(), float(masspoint)+2*gauss_sigma.getVal()
    # # print('why is this not making line:',minXl,maxXl)
    # l1 = ROOT.TLine(minXl,0,minXl,frame.GetMaximum())
    # l2 = ROOT.TLine(maxXl,0,maxXl,frame.GetMaximum())
    # l1.SetLineWidth(2);l2.SetLineWidth(2);
    #l1.Draw("same")
    #l2.Draw("same")
    # frame.addObject(l1)
    # frame.addObject(l2)


    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2, 0.6, 0.2, 0.88, "NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(frame.chiSquare(),2))
    chi2_text.AddText("#sigma "+"= {} #pm {}".format(round(landau_sigma.getVal(),3), round(landau_sigma.getError(),3)))
    chi2_text.AddText("#mu "+"= {} #pm {}".format(round(landau_mean.getVal(),3), round(landau_mean.getError(),3)))
    chi2_text.AddText("N "+"= {} #pm {}".format(round(n.getVal(),3), round(n.getError(),3)))
    chi2_text.AddText("N_{m(S)="+masspoint_str+"} "+"= {}".format(round(integ_atmass,1)))
    # chi2_text.AddText("bounds={}, {}".format(round(minXl,2),round(maxXl,2)))
    # chi2_text.AddText("#sigma "+"= {} #pm {}".format(round(gauss_sigma.getVal(),3), round(gauss_sigma.getError(),3)))
    # chi2_text.AddText("#mu "+"= {} #pm {}".format(round(gauss_mean.getVal(),3), round(gauss_mean.getError(),3)))
    # chi2_text.AddText("bounds={}, {}".format(round(minXl,2),round(maxXl,2)))
    chi2_text.SetTextSize(0.03)
    chi2_text.SetTextColor(2)
    chi2_text.SetShadowColor(0)
    chi2_text.SetFillColor(0)
    chi2_text.SetLineColor(0)
    frame.addObject(chi2_text)

    l1 = ROOT.TLine(mass_low,0,mass_low,frame.GetMaximum())
    l2 = ROOT.TLine(mass_high,0,mass_high,frame.GetMaximum())
    l1.SetLineWidth(2);l2.SetLineWidth(2);
    l1.SetLineColor(ROOT.kRed);l2.SetLineColor(ROOT.kRed);
    #l1.Draw("same")
    #l2.Draw("same")
    frame.addObject(l1)
    frame.addObject(l2)
    
    xlow,xhigh = 0,3
    # cfit = ROOT.TCanvas("cfit","cfit",1200,1200)
    # cfit.SetLogx(False)
    cfit, pad1, pad2 = createCanvasPads("cfit",boundary=0.3)
    pad1.cd()
    pad1.SetLogx(False)
    frame.GetYaxis().SetTitleSize(0.045)
    frame.GetXaxis().SetTitleSize(0.045)
    frame.GetYaxis().SetLabelSize(0.045)
    frame.GetXaxis().SetLabelSize(0.)
    frame.GetXaxis().SetTitleOffset(1.2)
    frame.GetYaxis().SetTitleOffset(1.6)
    frame.GetXaxis().SetRangeUser(xlow,xhigh)
    frame.Draw()

    legxlow, legxhigh = 0.75,0.9
    legend = ROOT.TLegend(legxlow,0.7,legxhigh,0.9)
    legend.AddEntry(frame.findObject("hist"),"ggH","lep")
    legend.AddEntry(frame.findObject("fit"),"landau","l")
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.03)
    legend.Draw("same")
    
    # rtext = "#splitline{m_{S} ="+str(masspoint)+" GeV}{c#tau = "+str(ctau)+"mm}"
    if htype=='kaon':
        rtext = "kaon mass assumption"
    else:
        rtext = "pion mass assumption"
    additional_text = []
    additional_text += [rtext]
    if additional_text:
        nother = len(additional_text)
        # dims = [legxlow, 0.65 - nother * 0.04 - 0.02, 0.83, 0.65]
        dims = [legxlow-0.05, 0.7 - nother * 0.04 - 0.02, 0.85, 0.65]

        text = ROOT.TPaveText(*dims + ['NDC'])
        text.SetTextFont(42)
        text.SetBorderSize(0)
        text.SetFillColor(0)
        text.SetTextAlign(21)
        text.SetTextSize(0.04)
        # text.SetTextColor(46)
        for rtext in additional_text:
            text.AddText(rtext)
        text.Draw()
    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in Progress'
    CMS_lumi.lumi_13TeV = str(round(lumi_factor/1000,1)) + " fb^{-1}"
    # CMS_lumi.lumi_13TeV = ""
    CMS_lumi.cmsTextSize = 0.6
    CMS_lumi.lumiTextSize = 0.55
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(cfit, 4, 0)

    pad2.cd()
    pad2.SetLogx(False)
    hpull = frame.pullHist("hist","fit")
    frame3 = mass.frame(ROOT.RooFit.Title("Pull Distribution"));
    hpull.SetMarkerSize(1.5)
    frame3.addPlotable(hpull,"P") ;
    frame3.GetYaxis().SetTitle("pull")
    frame3.GetXaxis().SetTitle(xlabel)
    frame3.GetYaxis().SetTitleSize(0.1)
    frame3.GetXaxis().SetTitleSize(0.1)
    frame3.GetYaxis().SetLabelSize(0.1)
    frame3.GetXaxis().SetLabelSize(0.1)
    frame3.GetXaxis().SetTitleOffset(1.2)
    frame3.GetYaxis().SetTitleOffset(0.6)
    frame3.GetXaxis().SetRangeUser(xlow,xhigh)
    frame3.Draw()
    # fit_filename = "fit_MS" + str(masspoint).replace(".","p") + "_ctauS" + ctau + suffix
    fit_filename = "fit_test_"+suffix
    # if not os.path.exists(fit_plot_directory): os.makedirs(fit_plot_directory)
    # cfit.SaveAs(os.path.join(output, fit_filename + ".pdf"))
    if masspoint_str in ['1.6','2','0.8']:
        cfit.SaveAs(os.path.join(output, fit_filename + ".png"))
    l_sigma,l_sigmaerr,l_mean,l_meanerr= round(landau_sigma.getVal(),3), round(landau_sigma.getError(),3), round(landau_mean.getVal(),3), round(landau_mean.getError(),3)
    # del frame3;
    # del frame;
    # del mass;
    # del dh_hist;
    # del gauss_mean;
    # del gauss_sigma;
    # del gauss;
    
    N,N_err=n.getVal(), n.getError()

    return l_sigma,l_sigmaerr,l_mean,l_meanerr,integ_atmass,N,N_err

def GaussFit(year,hist, masspoint, ctau, output, suffix=""):
    print("Performing a fit using a gaussian to get the mean and the width for mass assumption")
    # Declare the observable mean, and import the histogram to a RooDataHist

    tmp_sigma, tmp_mean = hist.GetRMS(), float(masspoint)
    if "hadron" in suffix:
        xlabel = "m_{hh} (GeV)"
    elif "muon" in suffix:
        xlabel = "m_{#mu#mu} (GeV)"
    else:
        xlabel = "0.5*(m_{hh}+m_{#mu#mu}) (GeV)"
    mass = ROOT.RooRealVar("mass", xlabel, 0., 3.);
    dh_hist = ROOT.RooDataHist("dh_hist", "dh_hist", ROOT.RooArgList(mass),
                                    ROOT.RooFit.Import(hist));

    # plot the data hist with error from sum of weighted events
    frame = mass.frame(ROOT.RooFit.Title("AvgMass"))
    dh_hist.plotOn(frame, ROOT.RooFit.DataError(ROOT.RooAbsData.SumW2), ROOT.RooFit.MarkerSize(1), ROOT.RooFit.MarkerColor(1), ROOT.RooFit.LineColor(1), ROOT.RooFit.Name("hist"))
    
    # Convert them to pdf

    # create a simple gaussian pdf
    gauss_mean = ROOT.RooRealVar("mean", "mean", float(masspoint), tmp_mean-0.002, tmp_mean+0.002)
    gauss_sigma = ROOT.RooRealVar("sigma jer", "sigma gauss", 0.002, 0.001, 0.03)
    gauss = ROOT.RooGaussian("gauss", "gauss", mass, gauss_mean, gauss_sigma)

    alp=0.3
    # minX, maxX = XPercentInterval(hist,alp/2,verbose=True) #removing x% from each side
    # width = maxX - minX

    if float(masspoint) in [2,1.8,1.6]:
        minX, maxX = float(masspoint)-1.5*tmp_sigma, float(masspoint)+1.5*tmp_sigma
    elif float(masspoint) == 1.1:
        minX, maxX = float(masspoint)-1.5*tmp_sigma, float(masspoint)+1.5*tmp_sigma
    elif float(masspoint) in [1.2,1]:
        minX, maxX = float(masspoint)-1.5*tmp_sigma, float(masspoint)+1.5*tmp_sigma
    else:
        minX, maxX = float(masspoint)-2*tmp_sigma, float(masspoint)+2*tmp_sigma

    print("SEE THIS-",minX,maxX)

    gauss.fitTo(dh_hist,ROOT.RooFit.Save(),ROOT.RooFit.SumW2Error(True) ,ROOT.RooFit.Range(minX,maxX))
    gauss.plotOn(frame, ROOT.RooFit.Name("fit"))


    frame.SetMaximum(frame.GetMaximum() * 1.25)
    frame.SetMinimum(0)
    frame.getAttMarker().SetMarkerColor(ROOT.kBlue)
    frame.getAttMarker().SetMarkerSize(1.5)
    frame.getAttLine().SetLineColor(ROOT.kBlue)

    minXl,maxXl = float(masspoint)-2*gauss_sigma.getVal(), float(masspoint)+2*gauss_sigma.getVal()
    # print('why is this not making line:',minXl,maxXl)
    l1 = ROOT.TLine(minXl,0,minXl,frame.GetMaximum())
    l2 = ROOT.TLine(maxXl,0,maxXl,frame.GetMaximum())
    l1.SetLineWidth(2);l2.SetLineWidth(2);
    #l1.Draw("same")
    #l2.Draw("same")
    frame.addObject(l1)
    frame.addObject(l2)


    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2, 0.6, 0.2, 0.88, "NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(frame.chiSquare(),2))
    chi2_text.AddText("#sigma "+"= {} #pm {}".format(round(gauss_sigma.getVal(),3), round(gauss_sigma.getError(),3)))
    chi2_text.AddText("#mu "+"= {} #pm {}".format(round(gauss_mean.getVal(),3), round(gauss_mean.getError(),3)))
    chi2_text.AddText("bounds={}, {}".format(round(minXl,2),round(maxXl,2)))
    chi2_text.SetTextSize(0.03)
    chi2_text.SetTextColor(2)
    chi2_text.SetShadowColor(0)
    chi2_text.SetFillColor(0)
    chi2_text.SetLineColor(0)
    frame.addObject(chi2_text)

    # cfit = ROOT.TCanvas("cfit","cfit",1200,1200)
    # cfit.SetLogx(False)
    cfit, pad1, pad2 = createCanvasPads("cfit",boundary=0.3)
    pad1.cd()
    pad1.SetLogx(False)
    frame.GetYaxis().SetTitleSize(0.045)
    frame.GetXaxis().SetTitleSize(0.045)
    frame.GetYaxis().SetLabelSize(0.045)
    frame.GetXaxis().SetLabelSize(0.)
    frame.GetXaxis().SetTitleOffset(1.2)
    frame.GetYaxis().SetTitleOffset(1.6)
    frame.GetXaxis().SetRangeUser(float(masspoint)-0.1,float(masspoint)+0.1)
    frame.Draw()

    legxlow, legxhigh = 0.75,0.9
    legend = ROOT.TLegend(legxlow,0.7,legxhigh,0.9)
    legend.AddEntry(frame.findObject("hist"),"ggH","lep")
    legend.AddEntry(frame.findObject("fit"),"gauss","l")
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.03)
    legend.Draw("same")
    
    rtext = "#splitline{m_{S} ="+str(masspoint)+" GeV}{c#tau = "+str(ctau)+"mm}"
    additional_text = []
    additional_text += [rtext]
    if additional_text:
        nother = len(additional_text)
        dims = [legxlow, 0.65 - nother * 0.04 - 0.02, 0.83, 0.65]
        dims = [legxlow-0.05, legxhigh - nother * 0.04 - 0.02, 0.85, 0.65]

        text = ROOT.TPaveText(*dims + ['NDC'])
        text.SetTextFont(42)
        text.SetBorderSize(0)
        text.SetFillColor(0)
        text.SetTextAlign(21)
        text.SetTextSize(0.04)
        # text.SetTextColor(46)
        for rtext in additional_text:
            text.AddText(rtext)
        text.Draw()
    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in Progress'
    CMS_lumi.lumi_13TeV = year+" MC"
    # CMS_lumi.lumi_13TeV = "UL2017MC"
    CMS_lumi.cmsTextSize = 0.6
    CMS_lumi.lumiTextSize = 0.55
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(cfit, 4, 0)

    pad2.cd()
    pad2.SetLogx(False)
    hpull = frame.pullHist("hist","fit")
    frame3 = mass.frame(ROOT.RooFit.Title("Pull Distribution")) ;
    hpull.SetMarkerSize(1.5)
    frame3.addPlotable(hpull,"P") ;
    frame3.GetYaxis().SetTitle("pull")
    frame3.GetXaxis().SetTitle(xlabel)
    frame3.GetYaxis().SetTitleSize(0.1)
    frame3.GetXaxis().SetTitleSize(0.1)
    frame3.GetYaxis().SetLabelSize(0.1)
    frame3.GetXaxis().SetLabelSize(0.1)
    frame3.GetXaxis().SetTitleOffset(1.2)
    frame3.GetYaxis().SetTitleOffset(0.6)
    frame3.GetXaxis().SetRangeUser(tmp_mean-0.1,tmp_mean+0.1)
    frame3.Draw()
    fit_filename = "fit_MS" + str(masspoint).replace(".","p") + "_ctauS" + ctau + suffix
    # if not os.path.exists(fit_plot_directory): os.makedirs(fit_plot_directory)
    # cfit.SaveAs(os.path.join(output, fit_filename + ".pdf"))
    cfit.SaveAs(os.path.join(output, fit_filename + ".png"))
    g_sigma,g_sigmaerr,g_mean,g_meanerr= round(gauss_sigma.getVal(),3), round(gauss_sigma.getError(),3), round(gauss_mean.getVal(),3), round(gauss_mean.getError(),3)
    # del frame3;
    # del frame;
    # del mass;
    # del dh_hist;
    # del gauss_mean;
    # del gauss_sigma;
    # del gauss;

    return g_sigma,g_sigmaerr,g_mean,g_meanerr
def SignalYield(key,datasets_dict_key,plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound,eff_dict):
    # fname = datasets_dict[key]['fname']
    
    fname=datasets_dict_key['fname']
    masspoint=fname.split("MS")[1].split("_")[0].replace('p','.')
    masspoint_str="MS"+fname.split("MS")[1].split("_")[0]
    ctau=key.split("_")[-1].replace('ctau','').replace('p','.')

    if ctau=="0":
        geneff=eff_dict[0.1]
    else:
        geneff=eff_dict[float(ctau)]
    ################################
    print('see here - ',ctau,geneff)
    # quit()
    

    # Signal modelling
    f_mc = ROOT.TFile(plot_src+"/"+fname,"r")
    # setting the bounds to ctau=1 & prompt category
    h2_recoHiggs_AvgMass_sig = f_mc.Get(category+"/"+hname).Clone()
    h2_recoHiggs_AvgMass_sig.Scale(geneff)
    # print('Entries early:',h2_recoHiggs_AvgMass_sig.GetEntries())
    # print('Entries avgmass:',h2_recoHiggs_AvgMass_sig.ProjectionY("",h2_recoHiggs_AvgMass_sig.GetXaxis().FindBin(122.5),h2_recoHiggs_AvgMass_sig.GetXaxis().FindBin(127.5)).GetEntries())
    # print('Entries integral:',h2_recoHiggs_AvgMass_sig.ProjectionY("",h2_recoHiggs_AvgMass_sig.GetXaxis().FindBin(122.5),h2_recoHiggs_AvgMass_sig.GetXaxis().FindBin(127.5)).Integral())
    # print('Entries avgmass all:',h2_recoHiggs_AvgMass_sig.ProjectionY().GetEntries())
    # print(lo_bound,hi_bound)
    h_recoHiggs_sig_atMass = h2_recoHiggs_AvgMass_sig.ProjectionX("",h2_recoHiggs_AvgMass_sig.GetYaxis().FindBin(lo_bound),h2_recoHiggs_AvgMass_sig.GetYaxis().FindBin(hi_bound))
    # print(h_recoHiggs_sig_all.Integral(),h_recoHiggs_sig_atMass.Integral())
    h_recoHiggs_sig = h_recoHiggs_sig_atMass
    # print('Entries:',h_recoHiggs_sig.GetEntries())
    h_recoHiggs_sig.Rebin(5)
    h_recoHiggs_sig.Scale(lumi)
    h_recoHiggs_sig.Sumw2()
    print(h_recoHiggs_sig.Integral())
    # Define mass and weight variables
    mass = ROOT.RooRealVar("CMS_hgg_mass", "CMS_hgg_mass", 125, 110, 140)
    mc = ROOT.RooDataHist("ggH_"+category,"ggH_"+category, ROOT.RooArgList(mass),ROOT.RooFit.Import(h_recoHiggs_sig))

    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Introduce a RooRealVar into the workspace for the Higgs mass
    MH = ROOT.RooRealVar("MH", "MH", 125, 120, 130 )
    MH.setConstant(True)
    # Signal peak width
    sigma = ROOT.RooRealVar("sigma_ggH_"+category, "sigma_ggH_"+category, 2, 0.2, 5)
    # Define a model with a variable mean
    dMH = ROOT.RooRealVar("dMH_ggH_"+category, "dMH_ggH_"+category, 0, -0.05, 0.05 )
    mean = ROOT.RooFormulaVar("mean_ggH_"+category, "mean_ggH_"+category, "(@0+@1)", ROOT.RooArgList(MH,dMH))
    model = ROOT.RooGaussian( "model_ggH_"+category, "model_ggH_"+category, mass, mean, sigma )

    # npeak = h_recoHiggs_sig.Integral(h_recoHiggs_sig.FindBin(122.5),h_recoHiggs_sig.FindBin(127.5))
    npeak_err=ctypes.c_double(0)
    npeak = h_recoHiggs_sig.IntegralAndError(h_recoHiggs_sig.FindBin(122.5),h_recoHiggs_sig.FindBin(127.5),npeak_err)
    nfull = h_recoHiggs_sig.Integral() # Technical doubt - use nfull or npeak for distribution? nfull probably since full dist is fed in
    norm_sig = ROOT.RooRealVar("model_ggH_"+category+"_norm", "Normalisation term for ggH in Tag 0", nfull)
    norm_sig.setConstant(True)

    bin_lo,bin_lomid,bin_himid,bin_hi=h_recoHiggs_sig.FindBin(110),h_recoHiggs_sig.FindBin(122.5),h_recoHiggs_sig.FindBin(127.5),h_recoHiggs_sig.FindBin(140)
    # Fit Gaussian to MC events and plot
    mass.setRange("loM", 110, 122.5 )
    mass.setRange("hiM", 127.5, 140 )
    mass.setRange("loSB", 120, 122.5 )
    mass.setRange("hiSB", 127.5, 130 )
    mass.setRange("peak", 122.5, 127.5 )
    mass.setRange("peakcore", 124.2, 125.8 )
    mass.setRange("full", 110, 140 )
    mass.setRange("full", 110, 140 )
    mass.setBins(int(bin_lomid-bin_lo),"loM")
    mass.setBins(int(bin_hi-bin_himid),"hiM")
    print(int(bin_lomid-bin_lo))
    print(int(bin_hi-bin_himid))
    fit_range = "loSB,hiSB"


    # can = ROOT.TCanvas()
    boundary_percent = 0.35
    ylength_c = int(2400*(1-boundary_percent+0.15))
    can = ROOT.TCanvas("", "", 2200, ylength_c)
    pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
    pad1.SetTopMargin(0.06)
    pad1.SetBottomMargin(0.15)
    pad1.SetLeftMargin(0.16)
    pad1.SetRightMargin(0.04)
    #pad1.SetGridx()
    pad1.Draw()
    can.cd()
    pad1.cd()
    plot = mass.frame()
    plot.GetXaxis().SetTitle("m(hh#mu#mu) (GeV)")
    mc.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0) );
    rf = model.fitTo(mc, ROOT.RooFit.Range("peak"),ROOT.RooFit.SumW2Error(True))
    print(rf)
    model.plotOn( plot, ROOT.RooFit.LineColor(2),ROOT.RooFit.Range("peak"),ROOT.RooFit.Name("fit"), ROOT.RooFit.LineColor(ROOT.kRed), ROOT.RooFit.LineStyle(1),ROOT.RooFit.LineWidth(5))
    mc.plotOn( plot, ROOT.RooFit.Name("dist"),ROOT.RooFit.MarkerSize(2) )
    # Set the number of bins in the sidebands such that the bin boundaries
    # are compatible with the binning for the full range:
    set_mass = ROOT.RooArgSet(mass)
    normSet = ROOT.RooFit.NormSet(set_mass)
    integral_full = model.createIntegral(mass,normSet,ROOT.RooFit.Range("full")).getVal()*h_recoHiggs_sig.Integral()
    intObj_peak = model.createIntegral(mass,normSet,ROOT.RooFit.Range("peak"))
    integral_peak = intObj_peak.getVal()*h_recoHiggs_sig.Integral()
    # integral_peak_err = intObj_peak.getPropagatedError(rf,mass)*h_recoHiggs_sig.Integral()
    integral_peak_err = npeak_err.value
    # npeak_err
    # npeak
    n_sig_peak_exp,n_sig_peak_exp_err = integral_peak,integral_peak_err
    n_sig_peak,n_sig_peak_err = npeak,npeak_err.value
    print(npeak,integral_full,integral_peak,n_sig_peak_exp,n_sig_peak_exp_err)
    # quit()



    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2,0.6,0.2,0.9,"NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(plot.chiSquare("fit","dist"),2))
    chi2_text.AddText("#mu "+"= {} #pm {}".format(round(MH.getVal(),3), round(MH.getError(),3)) )
    chi2_text.AddText("d#mu "+"= {} #pm {}".format(round(dMH.getVal(),3), round(dMH.getError(),3)) )
    chi2_text.AddText("#sigma "+"= {} #pm {}".format(round(sigma.getVal(),3), round(sigma.getError(),3)) )
    chi2_text.AddText("Yield, error = "+"{} #pm {}".format(round(n_sig_peak,2),round(n_sig_peak_err,2)))
    # chi2_text.AddText("Yield, error (fit) = "+"{} #pm {}".format(round(n_sig_peak_exp,2),round(n_sig_peak_exp_err,2)))

    chi2_text.AddText(datasets_dict_key['label'])
    chi2_text.SetTextSize(0.03)
    chi2_text.SetTextColor(2)
    chi2_text.SetShadowColor(0)
    chi2_text.SetFillColor(0)
    chi2_text.SetLineColor(0)
    chi2_text.SetName("chi2_text")
    plot.addObject(chi2_text)


    l1 = ROOT.TLine(122.5,0,122.5,plot.GetMaximum())
    l2 = ROOT.TLine(127.5,0,127.5,plot.GetMaximum())
    l1.SetLineWidth(2);l2.SetLineWidth(2);
    l1.SetLineColor(ROOT.kBlue);l2.SetLineColor(ROOT.kBlue);
    #l1.Draw("same")
    #l2.Draw("same")
    plot.addObject(l1)
    plot.addObject(l2)

    h_recoHiggs_sig.GetXaxis().SetRangeUser(110,140)
    h_recoHiggs_sig.GetXaxis().SetTitle("m(hh#mu#mu) [GeV]")
    h_recoHiggs_sig.GetYaxis().SetTitle("Events/"+str(h_recoHiggs_sig.GetXaxis().GetBinWidth(1)))
    h_recoHiggs_sig.SetMarkerStyle(20)
    h_recoHiggs_sig.SetMarkerSize(1)
    h_recoHiggs_sig.SetLineColor(ROOT.kBlack)
    h_recoHiggs_sig.GetXaxis().SetLabelSize(0.05)
    h_recoHiggs_sig.GetXaxis().SetTitleSize(0.06)
    # h_recoHiggs_sig.Draw("pe")
    plot.Draw("same")

    legxlow, legxhigh = 0.7,0.9
    legend = ROOT.TLegend(legxlow,0.7,legxhigh,0.9)
    legend.AddEntry(h_recoHiggs_sig,"Signal MC","lep")
    # legend.AddEntry(plot.findObject("fit"),"RooGaussian","l")
    legend.AddEntry(plot.findObject("fit"),"gaussian fit","l")
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.03)
    legend.Draw("same")

    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in Progress'
    CMS_lumi.lumi_13TeV = str(round(lumi/1000,1)) + " fb^{-1}"
    # CMS_lumi.lumi_13TeV = ""
    CMS_lumi.cmsTextSize = 0.65
    CMS_lumi.lumiTextSize = 0.6
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(can, 4, 0)
    can.Update()
    if round(float(ctau),3) in [0,1,10,100,0.774,0.129,1.292,12.915,0.077]:
        can.Draw()
        can.SaveAs(plot_dir+"/"+"fit_"+key+"_sig.png")

    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Save the model to a workspace
    MH.setVal(125)
    dMH.setConstant(True)
    sigma.setConstant(True)
    # f_out = ROOT.TFile(dc_dir+"/"+"workspace_sig_"+key+".root", "RECREATE")
    # w_sig = ROOT.RooWorkspace("workspace_sig","workspace_sig")
    # getattr(w_sig, "import")(model)
    # getattr(w_sig, "import")(norm_sig)
    # w_sig.Print()
    # w_sig.Write()
    # f_out.Close()
    nentries = h_recoHiggs_sig.GetEntries()
    return n_sig_peak,n_sig_peak_err
    # if nentries<=10:
    #     return n_sig_peak
    # else:
    #     return n_sig_peak_exp
def BkgYield(key,datasets_dict_key,plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound):
    # Background modelling
    # f_data = ROOT.TFile(plot_src+"/"+"output_Run2017.root","r")
    fname=datasets_dict_key['fname']
    masspoint=fname.split("MS")[1].split("_")[0].replace('p','.')
    masspoint_str="MS"+fname.split("MS")[1].split("_")[0]
    # ctau=key.split("ctau")[1].replace('p','.')
    ctau=key.split("_")[-1].replace('ctau','').replace('p','.')

    f_data = ROOT.TFile(plot_src+"/"+"output_Data.root","r")
    hname_MHMS_BC = "h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC"
    hname_MHIso_BC = "h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC"
    mass = ROOT.RooRealVar("CMS_hgg_mass", "CMS_hgg_mass", 125, 110, 140)
    h2_recoHiggs_AvgMass_bkg_cat = f_data.Get(category+"/"+hname).Clone()
    
    if 'prompt' in category:
    # if True: ########### TESTING TESTING TESTING ###############
        # h2_recoHiggs_AvgMass_bkg = f_data.Get("prompt"+"/"+hname).Clone()
        # h2_recoHiggs_AvgMass_bkg_BC=f_data.Get("prompt"+"/"+hname_MHIso_BC).Clone()
        h2_recoHiggs_AvgMass_bkg = f_data.Get(category+"/"+hname).Clone()
        h2_recoHiggs_AvgMass_bkg_BC=f_data.Get(category+"/"+hname_MHIso_BC).Clone()
        # h2_recoHiggs_AvgMass_bkg=f_data.Get("prompt"+"/"+hname).Clone()
        h_AvgMass_bkg_BC = h2_recoHiggs_AvgMass_bkg_BC.ProjectionY("",h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(110),h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(122.5))
        h_AvgMass_bkg_BC_2 = h2_recoHiggs_AvgMass_bkg_BC.ProjectionY("",h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(127.5),h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(140))
        h_AvgMass_bkg_BC.Add(h_AvgMass_bkg_BC_2)
        h_AvgMass_bkg = h2_recoHiggs_AvgMass_bkg.ProjectionY("",h2_recoHiggs_AvgMass_bkg.GetXaxis().FindBin(110),h2_recoHiggs_AvgMass_bkg.GetXaxis().FindBin(122.5))
        h_AvgMass_bkg_2 = h2_recoHiggs_AvgMass_bkg.ProjectionY("",h2_recoHiggs_AvgMass_bkg.GetXaxis().FindBin(127.5),h2_recoHiggs_AvgMass_bkg.GetXaxis().FindBin(140))
        h_AvgMass_bkg_BC.Add(h_AvgMass_bkg_2)
        h_AvgMass_bkg_BC.Rebin(40)
        h_AvgMass_bkg.Rebin(40)
        l_sigma_BC,l_sigmaerr_BC,l_mean_BC,l_meanerr_BC,n_atmass_BC,_,_ = LandauFit(lumi,h_AvgMass_bkg_BC, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), category,False,False)
        if True:
            if 'prompt' not in category:
                if 'Kaon' in plot_src:
                    l_mean_BC,l_sigma_BC=1.376,0.222
                else:
                    l_mean_BC,l_sigma_BC=1.388,0.412
        l_sigma,l_sigmaerr,l_mean,l_meanerr,n_atmass,_,_ = LandauFit(lumi,h_AvgMass_bkg, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), category+"_MH_nofixparams",False,False,l_mean_BC,l_sigma_BC)
        l_sigma,l_sigmaerr,l_mean,l_meanerr,n_atmass,_,_ = LandauFit(lumi,h_AvgMass_bkg, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), category+"_MH_fixparams",False,True,l_mean_BC,l_sigma_BC)
        l_sigma,l_sigmaerr,l_mean,l_meanerr,n_atmass,_,_ = LandauFit(lumi,h_AvgMass_bkg_BC, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), category+"_MHIso_fixparams",False,True,l_mean_BC,l_sigma_BC)

        nfit_val,nfit_err=1,1
        nfit_val_BC,nfit_err_BC=1,1
        print("TransferFactor:",n_atmass_BC,n_atmass,n_atmass/n_atmass_BC)
        # quit()
    else:
        h2_recoHiggs_AvgMass_bkg_BC = f_data.Get("displacedmumu"+"/"+hname_MHIso_BC).Clone()
        h2_recoHiggs_AvgMass_bkg_BC.Add(f_data.Get("displacedhh"+"/"+hname_MHIso_BC).Clone())
        h2_recoHiggs_AvgMass_bkg_BC.Add(f_data.Get("displaced"+"/"+hname_MHIso_BC).Clone())
        h_AvgMass_bkg_MHIso_BC = h2_recoHiggs_AvgMass_bkg_BC.ProjectionY("",h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(110),h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(122.5))
        h_AvgMass_bkg_MHIso_BC_2 = h2_recoHiggs_AvgMass_bkg_BC.ProjectionY("",h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(127.5),h2_recoHiggs_AvgMass_bkg_BC.GetXaxis().FindBin(140))
        h_AvgMass_bkg_MHIso_BC.Add(h_AvgMass_bkg_MHIso_BC_2)
        # h_AvgMass_bkg_MHIso_BC = h2_recoHiggs_AvgMass_bkg_BC.ProjectionY()
        h2_recoHiggs_AvgMass_bkg_dispmerged = f_data.Get("displacedmumu"+"/"+hname).Clone()
        h2_recoHiggs_AvgMass_bkg_dispmerged.Add(f_data.Get("displacedhh"+"/"+hname).Clone())
        h2_recoHiggs_AvgMass_bkg_dispmerged.Add(f_data.Get("displaced"+"/"+hname).Clone())
        h_AvgMass_bkg_MH_BC = h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionY("",h2_recoHiggs_AvgMass_bkg_dispmerged.GetXaxis().FindBin(110),h2_recoHiggs_AvgMass_bkg_dispmerged.GetXaxis().FindBin(122.5))
        h_AvgMass_bkg_MH_BC_2 = h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionY("",h2_recoHiggs_AvgMass_bkg_dispmerged.GetXaxis().FindBin(127.5),h2_recoHiggs_AvgMass_bkg_dispmerged.GetXaxis().FindBin(140))
        h_AvgMass_bkg_MH_BC.Add(h_AvgMass_bkg_MH_BC_2)
        h_AvgMass_bkg_MH_BC = h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionY()
        h_AvgMass_bkg_MHIso_BC.Rebin(40)
        h_AvgMass_bkg_MH_BC.Rebin(40)

        nfit_val,nfit_err=1,1
        nfit_val_BC,nfit_err_BC=1,1
        l_sigma_BC,l_sigmaerr_BC,l_mean_BC,l_meanerr_BC,n_atmass_BC,nfit_val_BC,nfit_err_BC = LandauFit(lumi,h_AvgMass_bkg_MHIso_BC, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), "dispmerged_MHIso",False,False)
        l_sigma,l_sigmaerr,l_mean,l_meanerr,n_atmass,nfit_val,nfit_err = LandauFit(lumi,h_AvgMass_bkg_MH_BC, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), "dispmerged_MH_nofixparams",False,False,l_mean_BC,l_sigma_BC)
        l_sigma,l_sigmaerr,l_mean,l_meanerr,n_atmass,nfit_val,nfit_err = LandauFit(lumi,h_AvgMass_bkg_MH_BC, plot_dir,lo_bound,hi_bound,htype,masspoint.replace('p','.'), "dispmerged_MH_fixparams",False,True,l_mean_BC,l_sigma_BC)

        print("TransferFactor:",n_atmass_BC,n_atmass,n_atmass/n_atmass_BC)
        h2_recoHiggs_AvgMass_bkg = h2_recoHiggs_AvgMass_bkg_BC

    h_recoHiggs_bkg = h2_recoHiggs_AvgMass_bkg.ProjectionX()
    #h_recoHiggs_bkg = h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionX() #for displaced merged with all cuts
    h_recoHiggs_bkg.Rebin(5)
    data = ROOT.RooDataHist("data_"+category,"data_"+category, ROOT.RooArgList(mass),ROOT.RooFit.Import(h_recoHiggs_bkg) )
    bin_lo,bin_lomid,bin_himid,bin_hi=h_recoHiggs_bkg.FindBin(110),h_recoHiggs_bkg.FindBin(122.5),h_recoHiggs_bkg.FindBin(127.5),h_recoHiggs_bkg.FindBin(140)
    # Fit Gaussian to MC events and plot
    mass.setRange("loM", 110, 122.5 )
    mass.setRange("hiM", 127.5, 140 )
    mass.setRange("loSB", 120, 122.5 )
    mass.setRange("hiSB", 127.5, 130 )
    mass.setRange("peak", 122.5, 127.5 )
    mass.setRange("peakcore", 124.2, 125.8 )
    # mass.setRange("full", 110, 140 )
    mass.setRange("full", 110, 140 )
    mass.setBins(int(bin_lomid-bin_lo),"loM")
    mass.setBins(int(bin_hi-bin_himid),"hiM")
    mass.setBins(int(bin_himid-bin_lomid),"peak")
    print(int(bin_lomid-bin_lo))
    print(int(bin_hi-bin_himid))
    fit_range = "loSB,hiSB"

    can = ROOT.TCanvas()
    plot = mass.frame()
    # mc.plotOn( plot,ROOT.RooFit.Range("peak"),ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0) )
    data.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0) )
    chi2_text = ROOT.TPaveText(0.2, 0.6, 0.2, 0.88, "NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("N = " + str(h_recoHiggs_bkg.Integral()))
    plot.addObject(chi2_text)

    h_recoHiggs_bkg.GetXaxis().SetRangeUser(110,140)
    h_recoHiggs_bkg.GetXaxis().SetTitle("m(hh#mu#mu) [GeV]")
    h_recoHiggs_bkg.GetYaxis().SetTitle("Events/"+str(h_recoHiggs_bkg.GetXaxis().GetBinWidth(1)))
    h_recoHiggs_bkg.SetMarkerStyle(20)
    h_recoHiggs_bkg.SetMarkerSize(1)
    h_recoHiggs_bkg.SetLineColor(ROOT.kBlack)
    h_recoHiggs_bkg.GetXaxis().SetLabelSize(0.05)
    h_recoHiggs_bkg.GetXaxis().SetTitleSize(0.06)
    h_recoHiggs_bkg.Draw("pe")
    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in Progress'
    CMS_lumi.lumi_13TeV = str(round(lumi/1000,1)) + " fb^{-1}"
    # CMS_lumi.lumi_13TeV = ""
    CMS_lumi.cmsTextSize = 0.6
    CMS_lumi.lumiTextSize = 0.55
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(can, 4, 0)
    plot.Draw("same")
    can.Update()
    if float(ctau) in [0,1,10,100] and masspoint in ['1.6','2','0.8']:
        can.Draw()
        can.SaveAs(plot_dir+"/"+"fit_higgs_nofit.png")
        # quit()

        
    h_recoHiggs_bkg_atMass = h2_recoHiggs_AvgMass_bkg.ProjectionX("",h2_recoHiggs_AvgMass_bkg.GetYaxis().FindBin(lo_bound),h2_recoHiggs_AvgMass_bkg.GetYaxis().FindBin(hi_bound))
    if 'prompt' not in category:
    # if False: ############ TESTING TESTING TESTING ############
        tf_BC = n_atmass/n_atmass_BC
        tf_BC_err = np.sqrt((nfit_err_BC/nfit_val_BC)**2+(nfit_err/nfit_val)**2)*tf_BC
        print(tf_BC_err,tf_BC,nfit_val_BC,nfit_err_BC,nfit_val,nfit_err)
        # quit()

        n_bkg_cat_nomass_err=ctypes.c_double(0)
        n_bkg_cat_nomass = h2_recoHiggs_AvgMass_bkg_cat.ProjectionX().IntegralAndError(h2_recoHiggs_AvgMass_bkg_cat.ProjectionX().FindBin(110.1),h2_recoHiggs_AvgMass_bkg_cat.ProjectionX().FindBin(139.9),n_bkg_cat_nomass_err)
        # npeak = h_recoHiggs_sig.IntegralAndError(h_recoHiggs_sig.FindBin(122.5),h_recoHiggs_sig.FindBin(127.5),npeak_err)
        # print("bug2:"+args.category,h_recoHiggs_bkg_cat.Integral(),h2_recoHiggs_AvgMass_bkg_cat.Integral())
        
        n_bkg_dispmerged_nomass_err=ctypes.c_double(0)
        n_bkg_dispmerged_nomass=h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionX().IntegralAndError(h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionX().FindBin(110.1),h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionX().FindBin(139.9),n_bkg_dispmerged_nomass_err)
        n_bkg_dispmerged_nomass = h2_recoHiggs_AvgMass_bkg_dispmerged.ProjectionX().Integral()
        # print("bug3:"+args.category,h_recoHiggs_bkg_cat.Integral(),h2_recoHiggs_AvgMass_bkg_cat.Integral())
        
        # cat_frac = h_recoHiggs_bkg_cat.Integral()/h_recoHiggs_bkg_merged.Integral()
        ### FOR SOME REASON, HISTOGRAM DISPMERGED GETS WRITTEN ONTO CATEGORY-SPECIFIC!?!?!?! HENCE STORE INTEGRAL BEFORE DEFINING NEXT HISTOGRAM #####
        cat_frac = n_bkg_cat_nomass/n_bkg_dispmerged_nomass
        # cat_frac_err = np.sqrt(1/n_bkg_cat_nomass+1/n_bkg_dispmerged_nomass)*cat_frac
        if n_bkg_cat_nomass!=0 and n_bkg_dispmerged_nomass!=0:
            cat_frac_err = np.sqrt((n_bkg_cat_nomass_err.value/n_bkg_cat_nomass)**2+(n_bkg_dispmerged_nomass_err.value/n_bkg_dispmerged_nomass)**2)*cat_frac
        else:
            cat_frac_err=cat_frac
        print("bug:"+category,h2_recoHiggs_AvgMass_bkg_cat.Integral(),h2_recoHiggs_AvgMass_bkg_dispmerged.Integral())
    else:
        tf_BC = 1
        tf_BC_err=0
        cat_frac=1
        cat_frac_err=0
        # h_recoHiggs_bkg = h2_recoHiggs_AvgMass_bkg.ProjectionX()
    print(cat_frac)
    # print(h_recoHiggs_bkg.Integral())
    print(h2_recoHiggs_AvgMass_bkg.ProjectionY().Integral())
    print(lo_bound,hi_bound)
    print(h2_recoHiggs_AvgMass_bkg.GetYaxis().FindBin(lo_bound),h2_recoHiggs_AvgMass_bkg.GetYaxis().FindBin(hi_bound))

    # h_recoHiggs_bkg = f_data.Get("prompt/h_recoHiggsMass_All_AtScalarMass2").Clone()
    h_recoHiggs_bkg = h_recoHiggs_bkg_atMass
    h_recoHiggs_bkg.Rebin(5)
    print(h_recoHiggs_bkg_atMass.Integral())
    n_bkg_sb_BC = h_recoHiggs_bkg.Integral(h_recoHiggs_bkg.GetXaxis().FindBin(110.1),h_recoHiggs_bkg.GetXaxis().FindBin(122.4))+h_recoHiggs_bkg.Integral(h_recoHiggs_bkg.GetXaxis().FindBin(127.6),h_recoHiggs_bkg.GetXaxis().FindBin(139.9))
    # quit()
    if 'prompt' not in category:
        h_recoHiggs_bkg.Scale(tf_BC)
    data = ROOT.RooDataHist("data_"+category,"data_"+category, ROOT.RooArgList(mass),ROOT.RooFit.Import(h_recoHiggs_bkg) )
        
    # Define mass sideband ranges on the mass variable
    fit_range = "loM,hiM"
    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Define the background model slope parameter
    # if not plot_all:
    alp_ini = -0.1
    alpha = ROOT.RooRealVar("alpha", "alpha", alp_ini, -1, 0.)
    # alpha.setConstant(True)
    model_bkg = ROOT.RooExponential("model_bkg_"+category, "model_bkg_"+category, mass, alpha )
    # alt_model_bkg = ROOT.RooExponential("model_bkg_"+category, "model_bkg_"+category, mass, alpha )
    N=ROOT.RooRealVar("N", "N", 0, 500);
    extmodel_bkg=ROOT.RooExtendPdf("extmodel_bkg_"+category, "extmodel_bkg_"+category, model_bkg, N, "full");


    # Uniform function
    a1 = ROOT.RooRealVar("a1", "Slope", 0)
    a1.setConstant(True)
    a0 = ROOT.RooRealVar("a0", "Intercept", 0, 0, 100)
    N_linear=ROOT.RooRealVar("N_linear", "N_linear", 0, 500);
    linear_bkg = ROOT.RooPolynomial("linear_bkg_"+category, "linear_bkg_"+category, mass, ROOT.RooArgList(a1))
    extlinear_bkg=ROOT.RooExtendPdf("extlinear_bkg_"+category, "extlinear_bkg_"+category, linear_bkg, N_linear, "full");
    # Power law function
    pow_1 = ROOT.RooRealVar("pow_1","Exponent of power law", -3, -10, -0.0001)
    N_pow=ROOT.RooRealVar("N_pow", "N_pow", 0, 500);
    pow_bkg = ROOT.RooGenericPdf("pow_bkg_"+category, "TMath::Power(@0,@1)", ROOT.RooArgList(mass,pow_1) )
    extpow_bkg=ROOT.RooExtendPdf("extpow_bkg_"+category, "extpow_bkg_"+category, pow_bkg, N_pow, "full");
    # RooChebychev polynomial: 4th order
    N_poly=ROOT.RooRealVar("N_poly", "N_poly", 0, 500);
    poly_1 = ROOT.RooRealVar("poly_1","T1 of chebychev polynomial", 0.01, 0, 10)
    poly_2 = ROOT.RooRealVar("poly_2","T2 of chebychev polynomial", 0.01, 0, 10)
    poly_3 = ROOT.RooRealVar("poly_3","T3 of chebychev polynomial", 0.01, -10, 10)
    poly_4 = ROOT.RooRealVar("poly_4","T4 of chebychev polynomial", 0.01, -10, 10)
    # poly_bkg = ROOT.RooChebychev("poly_bkg", "poly_bkg", mass, ROOT.RooArgList(poly_1,poly_2,poly_3,poly_4) )
    poly_bkg = ROOT.RooChebychev("poly_bkg", "poly_bkg", mass, ROOT.RooArgList(poly_1,poly_2) )
    # poly_bkg = ROOT.RooChebychev("poly_bkg", "poly_bkg", mass, ROOT.RooArgList(poly_1,poly_2,poly_3) )
    extpoly_bkg=ROOT.RooExtendPdf("extpoly_bkg_"+category, "extpoly_bkg_"+category, poly_bkg, N_poly, "full");


    n_bkg_sb = n_bkg_sb_BC
    fit_range = "loM,hiM"
    # Fit model to data sidebands
    model_bkg.fitTo( data, ROOT.RooFit.Range(fit_range) ) #NOTE: N gets normalized over sidebands only
    # Let's plot the model fit to the data
    boundary_percent = 0.35
    ylength_c = int(2400*(1-boundary_percent+0.15))
    can = ROOT.TCanvas("", "", 2200, ylength_c)
    pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
    pad1.SetTopMargin(0.06)
    pad1.SetBottomMargin(0.15)
    pad1.SetLeftMargin(0.16)
    pad1.SetRightMargin(0.04)
    #pad1.SetGridx()
    pad1.Draw()
    can.cd()
    pad1.cd()
    plot = mass.frame()
    plot.GetXaxis().SetTitle("m(hh#mu#mu) (GeV)")
    # We have to be careful with the normalisation as we only fit over sidebands
    # First do an invisible plot of the full data set
    # data.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0), ROOT.RooFit.Binning("loM") )
    # data.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0), ROOT.RooFit.Binning("hiM") )
    data.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerSize(0), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineWidth(0), ROOT.RooFit.LineColor(0) )
    # data.plotOn( plot, ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineColor(0) )
    set_mass = ROOT.RooArgSet(mass)
    extmodel_bkg.fitTo(data, ROOT.RooFit.Range(fit_range) )
    extlinear_bkg.fitTo(data, ROOT.RooFit.Range(fit_range))
    extpow_bkg.fitTo(data, ROOT.RooFit.Range(fit_range))
    # extpoly_bkg.fitTo(data, ROOT.RooFit.Range(fit_range))
    extmodel_bkg.plotOn( plot, ROOT.RooFit.NormRange(fit_range), ROOT.RooFit.Range("full"), ROOT.RooFit.LineColor(ROOT.kRed), ROOT.RooFit.Name("extfit"), ROOT.RooFit.LineStyle(1),ROOT.RooFit.LineWidth(8))
    extlinear_bkg.plotOn( plot, ROOT.RooFit.NormRange(fit_range), ROOT.RooFit.Range("full"), ROOT.RooFit.LineColor(ROOT.kAzure), ROOT.RooFit.Name("extlinearfit"), ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineWidth(8))
    extpow_bkg.plotOn( plot, ROOT.RooFit.NormRange(fit_range), ROOT.RooFit.Range("full"), ROOT.RooFit.LineColor(ROOT.kCyan), ROOT.RooFit.Name("extpow_bkg"), ROOT.RooFit.LineStyle(7),ROOT.RooFit.LineWidth(8))
    # extpoly_bkg.plotOn( plot, ROOT.RooFit.NormRange(fit_range), ROOT.RooFit.Range("full"), ROOT.RooFit.LineColor(ROOT.kViolet), ROOT.RooFit.Name("extpoly_bkg"), ROOT.RooFit.LineStyle(7),ROOT.RooFit.LineWidth(8))
    data.plotOn( plot, ROOT.RooFit.CutRange(fit_range),ROOT.RooFit.Name("dist"), ROOT.RooFit.Binning("loM"),ROOT.RooFit.MarkerSize(2) )
    data.plotOn( plot, ROOT.RooFit.CutRange(fit_range),ROOT.RooFit.Name("dist"), ROOT.RooFit.Binning("hiM"),ROOT.RooFit.MarkerSize(2) )
    data.plotOn( plot, ROOT.RooFit.CutRange("peak"),ROOT.RooFit.Name("dist"), ROOT.RooFit.Binning("peak"), ROOT.RooFit.MarkerSize(2) )

    integral_sb = extmodel_bkg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range(fit_range)).getVal()
    # n_bkg_exp = n_bkg_sb/integral_sb
    integral_bkg_peak = extmodel_bkg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("peak")).getVal()
    n_bkg_peak_exp = integral_bkg_peak*N.getVal()
    n_bkg_peak_exp_up = integral_bkg_peak*(N.getVal()+N.getError())
    n_bkg_peak_exp_down = integral_bkg_peak*(N.getVal()-N.getError())
    n_bkg_peak_exp_err = 0.5*(n_bkg_peak_exp_up-n_bkg_peak_exp_down)
    print(N.getVal())
    # print(N.getVal()/integral_sb)
    print(n_bkg_peak_exp,integral_sb*N.getVal())
    print(integral_bkg_peak,integral_sb)
    # quit()

    n_bkg_peak_exp = integral_bkg_peak*N.getVal()
    integral_bkg_peak_linear = extlinear_bkg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("peak")).getVal()
    n_bkg_peak_linear = integral_bkg_peak_linear*N_linear.getVal()
    n_bkg_peak_linear_up = integral_bkg_peak_linear*(N_linear.getVal()+N_linear.getError())
    n_bkg_peak_linear_down = integral_bkg_peak_linear*(N_linear.getVal()-N_linear.getError())
    n_bkg_peak_linear_err = 0.5*(n_bkg_peak_linear_up-n_bkg_peak_linear_down)
    integral_bkg_peak_pow = extpow_bkg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("peak")).getVal()
    n_bkg_peak_pow = integral_bkg_peak_pow*N_pow.getVal()
    n_bkg_peak_pow_up = integral_bkg_peak_pow*(N_pow.getVal()+N_pow.getError())
    n_bkg_peak_pow_down = integral_bkg_peak_pow*(N_pow.getVal()-N_pow.getError())
    n_bkg_peak_pow_err = 0.5*(n_bkg_peak_pow_up-n_bkg_peak_pow_down)
    # integral_bkg_peak_poly = extpoly_bkg.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("peak")).getVal()
    # n_bkg_peak_poly = integral_bkg_peak_poly*N_poly.getVal()
    n_bkg_sb_exp = integral_sb*N.getVal()
    
    err_unc=1/np.sqrt(int(round(n_bkg_sb,1))+1)*100
    expo_lin_diff=abs(n_bkg_peak_exp-n_bkg_peak_linear)/n_bkg_peak_exp*100
    expo_pow_diff=abs(n_bkg_peak_exp-n_bkg_peak_pow)/n_bkg_peak_exp*100
    if float(ctau)==0:
        with open("bkg_pdfvar_"+category+".txt", "a") as file:
            file.write("%s & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.1f%% & %.1f%% & %.1f%% \\\\ \n"%(masspoint,n_bkg_peak_exp,n_bkg_peak_exp_err,n_bkg_peak_linear,n_bkg_peak_linear_err,n_bkg_peak_pow,n_bkg_peak_pow_err,expo_lin_diff,expo_pow_diff,err_unc))
            # file.write("%s,%.3f,%.3f,%.3f,%.3f\n"%(masspoint,n_bkg_peak_exp,n_bkg_peak_linear,n_bkg_peak_pow,n_bkg_peak_poly))  # Appends text with a newline
    # integral_bkg_peak_ls.append(integral_bkg_peak)
    # N_val_ls.append(N.getVal())
    print(integral_sb*N.getVal(),integral_bkg_peak*N.getVal(),n_bkg_sb,N)

    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2,0.6,0.2,0.9,"NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(plot.chiSquare("extfit","dist"),2))
    chi2_text.AddText("#alpha "+"= {} #pm {}".format(round(alpha.getVal(),3), round(alpha.getError(),3)) )
    chi2_text.AddText("Int^{expo}_{full}"+"= {} #pm {}".format(round(N.getVal(),3), round(N.getError(),3)) )
    chi2_text.AddText("Int^{linear}_{full}"+"= {} #pm {}".format(round(N_linear.getVal(),3), round(N_linear.getError(),3)) )
    chi2_text.AddText("Int^{expo}_{SR}"+"= {} #pm {}".format(round(n_bkg_peak_exp,3), round(n_bkg_peak_exp_err,3)) )
    chi2_text.AddText("Int^{linear}_{SR}"+"= {} #pm {}".format(round(n_bkg_peak_linear,3), round(n_bkg_peak_linear_err,3)) )
    chi2_text.AddText("f2"+"= {}".format(round(tf_BC,3)) )
    # chi2_text.AddText("Int^{expo}_{sideband}, N_{sideband} "+"= {}, {}".format(round(n_bkg_sb_exp,3), round(n_bkg_sb,3)))
    # chi2_text.AddText("a0,a1"+"= {}, {}".format(round(a0.getVal(),3), round(a1.getError(),3)) )
    chi2_text.AddText('m_{S}^{reco}~'+masspoint+" GeV")
    # chi2_text.AddText(datasets_dict[key]['label'])
    chi2_text.SetTextSize(0.03)
    chi2_text.SetTextColor(2)
    chi2_text.SetShadowColor(0)
    chi2_text.SetFillColor(0)
    chi2_text.SetLineColor(0)
    chi2_text.SetName("chi2_text")
    plot.addObject(chi2_text)

    l1 = ROOT.TLine(122.5,0,122.5,plot.GetMaximum())
    l2 = ROOT.TLine(127.5,0,127.5,plot.GetMaximum())
    l1.SetLineWidth(2);l2.SetLineWidth(2);
    l1.SetLineColor(ROOT.kBlue);l2.SetLineColor(ROOT.kBlue);
    #l1.Draw("same")
    #l2.Draw("same")
    plot.addObject(l1)
    plot.addObject(l2)

    h_recoHiggs_bkg.GetXaxis().SetRangeUser(110,140)
    h_recoHiggs_bkg.GetXaxis().SetTitle("m(hh#mu#mu)")
    h_recoHiggs_bkg.GetYaxis().SetTitle("Events/"+str(h_recoHiggs_bkg.GetXaxis().GetBinWidth(1)))
    h_recoHiggs_bkg.SetMarkerStyle(20)
    h_recoHiggs_bkg.SetMarkerSize(1)
    h_recoHiggs_bkg.SetLineColor(ROOT.kBlack)
    # h_recoHiggs_bkg.Draw("pe")
    plot.Draw("same")
    legxlow, legxhigh = 0.7,0.9
    legend = ROOT.TLegend(legxlow,0.7,legxhigh,0.9)
    # legend.AddEntry(h_recoHiggs_bkg,"Blinded data","lep")
    legend.AddEntry(h_recoHiggs_bkg,"Data","lep")
    # legend.AddEntry(plot.findObject("fit"),"RooGaussian","l")
    legend.AddEntry(plot.findObject("extfit"),"exponential fit","l")
    legend.AddEntry(plot.findObject("extlinearfit"),"linear fit","l")
    legend.AddEntry(plot.findObject("extpow_bkg"),"power law fit","l")
    # legend.AddEntry(plot.findObject("extpoly_bkg"),"poly-2nd fit","l")
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.03)
    legend.Draw("same")
    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in Progress'
    CMS_lumi.lumi_13TeV = str(round(lumi/1000,1)) + " fb^{-1}"
    # CMS_lumi.lumi_13TeV = ""
    CMS_lumi.cmsTextSize = 0.65
    CMS_lumi.lumiTextSize = 0.6
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(can, 4, 0)
    can.Update()
    if float(ctau) in [0,1,10,100]:
        can.Draw()
        can.SaveAs(plot_dir+"/"+"fit_"+key+"_bkg"+"_scaled"+".png")

    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Define background model normalisation term, which is freely floating
    norm = ROOT.RooRealVar("model_bkg_"+category+"_norm", "Number of background events in "+category, data.numEntries(), 0, 3*data.numEntries() )
    alpha.setConstant(False)
    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # Save the background model and data set to a RooWorkspace
    # f_out = ROOT.TFile(dc_dir+"/"+"workspace_bkg_MS"+masspoint.replace('.','p')+".root", "RECREATE") # common bkg for all ctau
    # w_bkg = ROOT.RooWorkspace("workspace_bkg","workspace_bkg")
    # getattr(w_bkg, "import")(data)
    # getattr(w_bkg, "import")(norm)
    # getattr(w_bkg, "import")(model_bkg)
    # w_bkg.Print()
    # w_bkg.Write()
    # f_out.Close()
    # w_bkg.Delete();f_out.Delete();
    # del w_bkg, f_out
    # del plot;
    # del set_mass; del alpha;del model_bkg;del norm;del w_bkg;
    n_bkg_peak_exp=n_bkg_peak_exp*cat_frac #tf_BC already applied when fitting higgs mass
    # n_bkg_peak_exp=n_bkg_peak_exp*cat_frac*tf_BC #above is not the case anymore since the unweighted CR is now fitted
    print('SEE HERE:',cat_frac,n_bkg_peak_exp)
    print('SEE HERE:',integral_bkg_peak/integral_sb,tf_BC,cat_frac) # From sidebands to SR
    print(n_bkg_peak_exp)
    #quit()
    return n_bkg_peak_exp,cat_frac,cat_frac_err,tf_BC,tf_BC_err,(integral_bkg_peak/integral_sb),n_bkg_sb

def ObsYield(key,datasets_dict_key,plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound):
    fname=datasets_dict_key['fname']
    masspoint=fname.split("MS")[1].split("_")[0].replace('p','.')
    masspoint_str="MS"+fname.split("MS")[1].split("_")[0]
    ctau=key.split("_")[-1].replace('ctau','').replace('p','.')
    

    # Signal modelling
    f_data = ROOT.TFile(plot_src+"/"+"output_Data.root","r")
    mass = ROOT.RooRealVar("CMS_hgg_mass", "CMS_hgg_mass", 125, 110, 140)
    h2_recoHiggs_AvgMass_SR = f_data.Get(category+"/"+hname).Clone()

    h_recoHiggs_SR_atMass = h2_recoHiggs_AvgMass_SR.ProjectionX("",h2_recoHiggs_AvgMass_SR.GetYaxis().FindBin(lo_bound),h2_recoHiggs_AvgMass_SR.GetYaxis().FindBin(hi_bound))
    # print(h_recoHiggs_sig_all.Integral(),h_recoHiggs_sig_atMass.Integral())
    h_recoHiggs_SR = h_recoHiggs_SR_atMass
    # print('Entries:',h_recoHiggs_sig.GetEntries())
    h_recoHiggs_SR.Rebin(5)
    h_recoHiggs_SR.Sumw2()
    print(h_recoHiggs_SR.Integral())
    n_obs_peak_err=ctypes.c_double(0)
    n_obs_peak=h_recoHiggs_SR.IntegralAndError(h_recoHiggs_SR.GetXaxis().FindBin(122.6),h_recoHiggs_SR.GetXaxis().FindBin(127.4),n_obs_peak_err)
    return n_obs_peak,n_obs_peak_err
    # if nentries<=10:
    #     return n_sig_peak
    # else:
    #     return n_sig_peak_exp
def GetKShortsUnc(k,category,src,yr):
    sf_src=src.split('plots')[0]+'scale_factors'+'/'+yr
    prefix="rpvsig_vs_pt"
    yr_suf=yr.replace('UL','').replace('_APV','')
    if yr == "UL2016":
        yr_suf="2016"
    if "mass" in k:
        suf="_confidence_data_vs_data"
    else:
        suf=""
    f_in = ROOT.TFile(sf_src+"/"+prefix+suf+"_"+yr_suf+".root","r")
    htmp = f_in.Get("histratio")
    ptbin=htmp.GetYaxis().GetLast()
    if "prompt" in category:
        lxybin=1
    elif "mumu" in category:
        if "hh" in k:
            lxybin=1
        else:
            lxybin=[2,3,4]
    elif "hh" in category:
        if "hh" in k:
            lxybin=[2,3,4]
        else:
            lxybin=1
    else:
        lxybin=[2,3,4]
    if isinstance(lxybin,list):
        vals,errs=[],[]
        for b in lxybin:
            tmpval,tmperr=htmp.GetBinContent(b,ptbin),htmp.GetBinError(b,ptbin)
            vals.append(tmpval)
            errs.append(tmperr/tmpval)
        return max(errs)
    else:
        val,err=htmp.GetBinContent(lxybin,ptbin),htmp.GetBinError(lxybin,ptbin)
        print("Kshort unc:",val,err,htmp.GetXaxis().GetBinCenter(lxybin),htmp.GetYaxis().GetBinCenter(ptbin))
        return err/val
    
# def GetTrgUnc(fsigname,src,yr):
#     sf_src=src.split('plots')[0]+'scale_factors'+'/'+yr
#     # fname="NUM_HLT_IsoMu27_DEN_LooseID_Iso040_pair_dR_pt_cut_dRFineBins"
#     if "2017" in yr:
#         prefix="NUM_HLT_IsoMu27_DEN_LooseID_Iso040_pair_dR_pt_cut"
#     else:
#         prefix="NUM_HLT_IsoMu24_DEN_LooseID_Iso040_pair_dR_pt_cut"
#     suf = "_dRFineBins"
#     f_in = ROOT.TFile(sf_src+"/"+prefix+suf+".root","r")
#     htmp = f_in.Get(prefix)

#     f_mc = ROOT.TFile(src+"/"+fsigname,"r")
#     dr_sig=f_mc.Get("h_DiMuonDeltaR").Clone()
#     # bin_info = []
#     binerr=0
#     binval=0
#     binsum=0
#     sum_wts=dr_sig.Integral()
#     for bin_idx in range(1, dr_sig.GetNbinsX() + 1):
#         bin_center = dr_sig.GetBinCenter(bin_idx)
#         bin_content = dr_sig.GetBinContent(bin_idx)
#         binno=htmp.GetXaxis().FindBin(bin_center)
#         if binno==0:
#             binno=1
#         if binno==htmp.GetXaxis().GetNbins() + 1:
#             binno=htmp.GetXaxis().GetNbins()
#         binerr=binerr+(htmp.GetBinError(binno,1)*bin_content/sum_wts)**2
#         binval=binval+(bin_content/sum_wts*htmp.GetBinContent(binno,1))
#         binsum=binsum+bin_content
#     binerr=np.sqrt(binerr)
#     print(binval, sum_wts)
#     print(binsum/sum_wts)
#     return binerr/binval

def main():
    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    ROOT.RooFit.PrintLevel(5)
    # add arguments below
    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-i", "--indir",   dest="indir",   help="hist file location", type=str)
    parser.add_argument("-o", "--outdir",   dest="outdir",   help="plotting location", type=str)
    parser.add_argument("-p", "--prefix",   dest="prefix",   help="limit files location", type=str)
    parser.add_argument("--htype",   dest="htype",   help="kaon or pion", type=str)
    parser.add_argument("--counting", dest="countinglims", help="true for generating counting datacards, false by default", action="store_true")
    parser.add_argument("--category", dest="category", help="prompt,displacedhh,displacedmumu,displaced", type=str)
    parser.add_argument("--mass", dest="mass", help="scalar mass", type=str)
    parser.add_argument("--ctau",dest="ctau",help="ctau",type=str)
    parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    parser.add_argument("--unblind", dest="unblind", help="true for using observed SR, false by default", action="store_true")

    # parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    # parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    args = parser.parse_args()
    cwd = os.getcwd()

    global lumi 
    lumi = lumi_scale[args.year]

    datasets_dict = {}
    # m_point = str(args.mass).replace('.','p')
    # ctau = f'{args.ctau:g}'.replace('.','p')
    m_point=args.mass
    if '_' in args.ctau:
        if args.ctau.count('_') == 2:
            ctau_old1,ctau_old2,ctau_new=args.ctau.split('_')
            ctau=ctau_new
            fname='output_HToSS_MH125_MS'+m_point+'_ctauS'+ctau_old1+'_ctauS'+ctau_old2+'_ctauS'+ctau+'.root'
            key='HToSS_MS'+m_point+'_ctau'+ctau_old1+'_ctau'+ctau_old2+'_ctau'+ctau_new

        else:
            ctau_old,ctau_new=args.ctau.split('_')
            ctau=ctau_new
            fname='output_HToSS_MH125_MS'+m_point+'_ctauS'+ctau_old+'_ctauS'+ctau+'.root'
            key='HToSS_MS'+m_point+'_ctau'+ctau_old+'_ctau'+ctau_new
    else:
        ctau=args.ctau
        fname='output_HToSS_MH125_MS'+m_point+'_ctauS'+ctau+'.root'
        key='HToSS_MS'+m_point+'_ctau'+ctau
    datasets_dict[key]={'fname':'','label':''}
    datasets_dict[key]['fname']=fname
    # datasets_dict[key]['label']="#splitline{H#rightarrow SS}{m_{S}="+m_point.replace('p','.')+" GeV,c#tau = "+ctau.replace('p','.')+"mm}"
    if ctau=="0":
        datasets_dict[key]['label']="m_{S}^{reco}="+m_point.replace('p','.')+" GeV,c#tau = 0.1mm"
    else:
        datasets_dict[key]['label']="m_{S}^{reco}="+m_point.replace('p','.')+" GeV,c#tau = "+ctau.replace('p','.')+"mm"
    print(ctau,fname)
    print(datasets_dict)
    # quit()
    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # plot_src = "/user/sdansana/CMSSW_10_6_27/src/HToSS_analysis/plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211/total"
    plot_src = args.indir
    plot_dir_orig = args.outdir
    plot_all = True
    fin = open('BR_xsec_benchmark.yaml','r')
    br_pars = yaml.safe_load(fin)
    xsec_ggH = br_pars['xsec_ggH']
    br_Haa = br_pars['br_Haa']
    global htype
    htype=args.htype

    fin2 = open('bounds'+'.yaml','r')
    #fin2 = open('bounds_test'+'.yaml','r')
    bound_pars = yaml.safe_load(fin2)
    lo_bound,hi_bound=bound_pars['MS'+m_point][args.year]
    print(lo_bound,hi_bound)
    fin2.close()
    # quit()    

    categories = ["prompt","displacedhh","displacedmumu","displaced"]
    category = args.category
    dc_dir_orig=os.getcwd()

    print("IN CATEGORY:", category)
    #dc_dir_orig = os.path.join(dc_dir_orig,"HiggsPtShape_test") # TESTING TESTING TESTING
    dc_dir_orig = os.path.join(dc_dir_orig,args.prefix) # TESTING TESTING TESTING
    dc_dir_orig = os.path.join(dc_dir_orig,args.year)
    dc_dir = os.path.join(dc_dir_orig,category)
    if htype!="kaon" and m_point=="1p1":
        dc_dir = os.path.join(dc_dir,'MS'+m_point+"_pion")
    else:
        dc_dir = os.path.join(dc_dir,'MS'+m_point)
    if not os.path.exists(dc_dir):
        os.makedirs(dc_dir)
    plot_dir = os.path.join(plot_dir_orig,category)
    print(dc_dir)
    if not os.path.exists(plot_dir):
        os.makedirs(plot_dir)
    shutil.copy(os.path.join(cwd,"index.php"),os.path.join(plot_dir,"index.php"))


    feff_fit_dict={}
    #### TESTING TESTING TESTING ####
    feff_fit=open("gen_eff_fit_"+htype+".txt","r")
    geneff=1
    for line in feff_fit:
        # print(ctau,float(line.split('\t')[0].strip()))
        feff_fit_dict[float(line.split('\t')[0].strip())]=float(line.split('\t')[1].replace('\n',''))

    # lo_bound,hi_bound=0.2,2.5
    # lo_bound,hi_bound=lo_bound*
    mass_prev = ""
    hname = "h_recoHiggsMass_AvgMass_mumu_hh_MH_BC"
    n_bkg_list=[]
    integral_bkg_peak_ls=[]
    N_val_ls=[]
    # key='HToSS_MS1p6_ctau1'
    masspoint=key.split("MS")[1].split("_")[0]
    sig_rate,sig_err=SignalYield(key,datasets_dict[key],plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound,feff_fit_dict)
    print('SEETHIS:',sig_rate,sig_err)
    bkg_rate,f1,f1err,f2,f2err,f3,n_CR=BkgYield(key,datasets_dict[key],plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound)
    # bkg_rate,f1,f1err,f2,f2err,f3,n_CR=ObservedYield(key,datasets_dict[key],plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound)
    if args.unblind:
        obs_rate,obs_err=ObsYield(key,datasets_dict[key],plot_src,plot_dir,dc_dir,category,hname,lo_bound,hi_bound)
    # print(bkg_rate,f1,f1err,f2,f2err,f3,n_CR)

    # quit() ############### TESTING TESTING TESTING ##################

    # for key in datasets_dict:
        
    #     # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #     quit()
        
        # if 'prompt' not in args.category:

    if args.countinglims:
        fname_suf = '_counting'
    else:
        fname_suf = ''
    datacard_txt = open(dc_dir+"/"+"datacard_"+key+fname_suf+".txt","w")
    datacard_txt_category = open(dc_dir+"/"+"datacard_"+key+fname_suf+"_"+category+".txt","w")
    datacard_txt.write('--------------------------------------------------------------\n')
    datacard_txt.write('imax 1\n')
    datacard_txt.write('jmax 1\n')
    datacard_txt.write('kmax *\n')
    datacard_txt.write('--------------------------------------------------------------\n')
    datacard_txt.write('shapes * * FAKE\n')
    
    datacard_txt_category.write('--------------------------------------------------------------\n')
    datacard_txt_category.write('imax 1\n')
    datacard_txt_category.write('jmax 1\n')
    datacard_txt_category.write('kmax *\n')
    datacard_txt_category.write('--------------------------------------------------------------\n')
    datacard_txt_category.write('shapes * * FAKE\n')
    # datacard_txt.write('--------------------------------------------------------------\n')
    
    #pdf shape workspace making + naming needs to be fixed later
    shapes = {'signal':{'rate':lumi,'process_idx':0,'process':'ggH','channel':category,'file':"workspace_sig_"+key+".root",'workspace':'workspace_sig:model_ggH'+category},\
            'bkg':{'rate':1.0,'process_idx':1,'process':'bkg','channel':category,'file':"workspace_bkg_MS"+masspoint.replace('.','p')+".root",'workspace':'workspace_bkg:model_bkg'+category},\
            'data':{'rate':-1,'process_idx':-1,'process':'data_obs','channel':category,'file':"workspace_bkg_MS"+masspoint.replace('.','p')+".root",'workspace':'workspace_bkg:data'+category}}
    # TESTING TESTING TESTING
    ct_new=float(ctau.split('_')[-1].replace('p','.'))
    m_float=float(args.mass.replace('p','.'))
    if ct_new==0:
        ct_new=0.1

    with open('r_wt_dict.yaml', "r") as file:
        r_wts_dict = yaml.safe_load(file)
    r_wt,r_wt_cat=float(r_wts_dict["combined"]["MS"+args.mass][ct_new]),float(r_wts_dict[category]["MS"+args.mass][ct_new])
    # r_wt,r_wt_cat=float(r_wts_dict["combined"]["MS1p2"][ctau_new]),float(r_wts_dict[category]["MS1p2"][ctau_new])
    print(r_wt,r_wt_cat)
    print(sig_rate,category)
    # print("END")
    # quit()

    shapes['signal']['rate']=sig_rate*r_wt

    if args.countinglims:
        if round(shapes['signal']['rate'],3) == 0:
            shapes['signal']['rate'] = 0.001
        else:
            shapes['signal']['rate'] = round(shapes['signal']['rate'],3)
        if round(bkg_rate,3) == 0:
            shapes['bkg']['rate'] = 0.001
        else:
            shapes['bkg']['rate'] = round(bkg_rate,3)
        
        shapes['data']['rate'] = -1
    # print(sig_rate,shapes['signal']['rate'],category)
    # print("END")
    # quit()
    
    unc_dict = {}
    with open('UncertaintiesList.tsv', newline='') as csv_f:
        for row in csv.DictReader(csv_f, delimiter='\t'):
            #fullname = row['systname'] + ' ' + row['lname']
            unc_dict[row['syst_name']] = {'type':row['type'],'vals':{'extra':'','signal':row[args.year] if float(row['signal'])>0 else '-','bkg':row[args.year] if float(row['bkg'])>0 else '-'}}
            # unc_dict[row['syst_name']] = row[args.year]
    print()
    f_lt=open('signal_lifetime_reweighting_table'+'_'+args.year.replace('UL','')+'.yaml','r')
    ltwt_pars = yaml.safe_load(f_lt)
    if args.ctau.count('_') == 2:
        ctau_old1=float(ctau_old1.replace('p','.'))
        ctau_old2=float(ctau_old2.replace('p','.'))
        ctau_new=float(ctau_new.replace('p','.'))
        if (ctau_old1==0):
            ctau_old1=0.1
        if (ctau_old2==0):
            ctau_old2=0.1
        if (ctau_new==0):
            ctau_new=0.1
        sum_wts1,sum_wts1_err=ltwt_pars['MS'+m_point][ctau_old1][ctau_new][0],ltwt_pars['MS'+m_point][ctau_old1][ctau_new][1]
        sum_wts2,sum_wts2_err=ltwt_pars['MS'+m_point][ctau_old2][ctau_new][0],ltwt_pars['MS'+m_point][ctau_old2][ctau_new][1]
        mc_err_rel1=sum_wts1_err/sum_wts1
        mc_err_rel2=sum_wts2_err/sum_wts2
        if mc_err_rel1 > mc_err_rel2:
            mc_sum_err_rel=mc_err_rel1
        else:
            mc_sum_err_rel=mc_err_rel2
    elif args.ctau.count('_') >= 1:
        ctau_old=float(ctau_old.replace('p','.'))
        ctau_new=float(ctau_new.replace('p','.'))
        if (ctau_old==0):
            ctau_old=0.1
        if (ctau_new==0):
            ctau_new=0.1
        print(ctau_new,ctau_old)
        sum_wts,sum_wts_err=ltwt_pars['MS'+m_point][ctau_old][ctau_new][0],ltwt_pars['MS'+m_point][ctau_old][ctau_new][1]
        mc_sum_err_rel=sum_wts_err/sum_wts
    else:
        mc_sum_err_rel=0
    # mc_sum_err_rel=0 # TESTING TESTING TESTING

    # trg_relerr=GetTrgUnc(datasets_dict[key]['fname'],plot_src,args.year)
    # print(datasets_dict[key]['fname'],' - trigger error - ',trg_relerr)
    # quit()

    # keys_list = ['lumi_13TeV','close_muons','muon_id','trigger','h_reco','cs_ggH','bkg_norm','test']
    # keys_list = ['lumi_13TeV','close_muons','muon_id','trigger','h_reco','cs_ggH','bkg_norm']
    # keys_list = ['lumi_13TeV','close_muons','muon_id','trigger','h_reco','cs_ggH','bkg_norm','mc_stat']
    # keys_list = ['lumi_13TeV','pileup','close_muons','muon_id','trigger','h_reco','mumu_sv_reco','hh_sv_reco','dimu_mass','dih_mass','muon_iso','h_iso','dih_mass','displaced_mu','cs_ggH','bkg_norm','signal_stat','higgs_pt']
    keys_list = ['lumi_13TeV','pileup','close_muons','muon_id','trigger','h_reco','mumu_sv_reco','hh_sv_reco','dimu_mass','dih_mass','muon_iso','h_iso','dih_mass','displaced_mu','bkg_norm','signal_stat','higgs_pt','f1_unc','f2_unc']
    # keys_list = ['lumi_13TeV','close_muons','muon_id','trigger','h_reco','cs_ggH','bkg_norm']
    # keys_list = ['bkg_norm','mc_stat']
    # keys_list = ['bkg_norm']
    systs = {}
    for k in keys_list:
        if 'bkg_norm' in k:
            systs[k+'_'+category+'_'+args.year]=unc_dict[k]
            systs[k+'_'+category+'_'+args.year]['vals']['extra']=int(round(n_CR,1))
            systs[k+'_'+category+'_'+args.year]['vals']['signal']='-'
            systs[k+'_'+category+'_'+args.year]['vals']['bkg']=round(f1*f2*f3,5)
        elif 'signal_stat' in k:
            if shapes['signal']['rate']>0.001:
                systs[k+'_'+category+'_'+args.year]=unc_dict[k]
                err=np.sqrt(mc_sum_err_rel**2 + (sig_err/sig_rate)**2)
                # systs[k+'_'+category]['vals']['signal']=1+(1/np.sqrt(shapes['signal']['rate']))
                # systs[k+'_'+category+'_'+args.year]['vals']['signal']=1+(sig_err/shapes['signal']['rate'])
                systs[k+'_'+category+'_'+args.year]['vals']['signal']=1+round(err,3)
                systs[k+'_'+category+'_'+args.year]['vals']['bkg']='-'
        elif 'sv_reco' in k or 'mass' in k:
            systs[k]=unc_dict[k]
            systs[k]['vals']['signal']=1+round(GetKShortsUnc(k,category,plot_src,args.year),3)
            systs[k]['vals']['bkg']='-'
        elif 'iso' in k:
            # systs[k+'_'+args.year]=unc_dict[k] 
            # systs[k+'_'+args.year]['vals']['signal']=1+0.2 #random assignment of 20% uncertainty for now
            # systs[k+'_'+args.year]['vals']['bkg']='-'
            # mumu_iso_unc
            if 'prompt' in category:
                with open('Uncertainties_mumu_Iso.tsv', newline='') as csv_f:
                    for row in csv.DictReader(csv_f, delimiter='\t'):
                        #fullname = row['systname'] + ' ' + row['lname']
                        if row['mS'] == args.mass.replace('p','.'):
                            mumu_iso_unc = round(float(row[args.year]),3)
                with open('Uncertainties_hh_Iso.tsv', newline='') as csv_f:
                    for row in csv.DictReader(csv_f, delimiter='\t'):
                        #fullname = row['systname'] + ' ' + row['lname']
                        if row['mS'] == args.mass.replace('p','.'):
                            hh_iso_unc = round(float(row[args.year]),3)
                print('mumu:',mumu_iso_unc)
                print('hh:',hh_iso_unc)
                systs[k]=unc_dict[k] 
                if 'h' in k:
                    systs[k]['vals']['signal']=hh_iso_unc 
                else:
                    systs[k]['vals']['signal']=mumu_iso_unc 
                systs[k]['vals']['bkg']='-'
            else:
                systs[k]=unc_dict[k] 
                if 'h' in k:
                    systs[k]['vals']['signal']=1+0.1 # assignment of 10% uncertainty for now
                else:
                    systs[k]['vals']['signal']=1+0.025 # assignment of 2.5% uncertainty for now
                systs[k]['vals']['bkg']='-'
        elif 'trigger' in k:
            with open('trgSF_table.yaml', "r") as file:
                yield_unc = yaml.safe_load(file)
            unc_percent=yield_unc[args.year]['MS'+m_point]['diff_percent']
            systs[k]=unc_dict[k] 
            systs[k]['vals']['signal']=round(1+unc_percent/100,3)
            systs[k]['vals']['bkg']='-'
        elif 'f1_unc' in k:
            if category != "prompt":
                if f1==0:
                    unc_percent=1
                else:
                    unc_percent=f1err/f1
                systs['f1'+'_'+category+'_'+args.year]=unc_dict[k]
                systs['f1'+'_'+category+'_'+args.year]['vals']['signal']='-'
                systs['f1'+'_'+category+'_'+args.year]['vals']['bkg']=round(1+unc_percent,3)
        elif 'f2_unc' in k:
            if category != "prompt":
                unc_percent=f2err/f2
                systs['f2'+'_'+args.year]=unc_dict[k]
                systs['f2'+'_'+args.year]['vals']['signal']='-'
                systs['f2'+'_'+args.year]['vals']['bkg']=round(1+unc_percent,3)
            # quit()
        else:
            systs[k]=unc_dict[k]
    print(systs)
    # systs = {'lumi_13TeV':{'type':'lnN','vals':{'extra':'','signal':'1.023','bkg':'-'}},
    #         #  'bkg_norm'+'_'+category:{'type':'gmN','vals':{'extra':int(round(n_bkg_sb_exp,1)),'signal':'-','bkg':round(integral_bkg_peak/integral_sb,3)}}}
    #             'bkg_norm'+'_'+category:{'type':'gmN','vals':{'extra':int(round(n_CR,1)),'signal':'-','bkg':round(f1*f2*f3,5)}}}
    # systs['bkg_norm']['vals']['extra'] = round(n_bkg_sb_exp,1)
    # systs['bkg_norm']['vals']['bkg'] = 
    
    print("ENDS HERE")
    # quit()
    # quit()
    procs = list(shapes.keys())[0:-1]
    if not args.countinglims:
        for k in shapes:
            datacard_txt.write("shapes\t{:<20}\t{:<20}\t{:<20}\t{:<20}\n".format(shapes[k]['process'],shapes[k]['channel'],shapes[k]['file'],shapes[k]['workspace']))
            datacard_txt_category.write("shapes\t{:<20}\t{:<20}\t{:<20}\t{:<20}\n".format(shapes[k]['process'],shapes[k]['channel'],shapes[k]['file'],shapes[k]['workspace']))
    datacard_txt.write('--------------------------------------------------------------\n')
    datacard_txt_category.write('--------------------------------------------------------------\n')
    bins = {'bin':category}
    observations = {'observation':'-1'}
    if args.unblind:
        observations['observation']=str(obs_rate)
    else:
        observations['observation']='-1'
    datacard_txt.write("{:<20}".format('bin'))
    datacard_txt_category.write("{:<20}".format('bin'))
    for b in bins:
        datacard_txt.write("\t{:<20}".format(bins[b]))
        datacard_txt_category.write("\t{:<20}".format(bins[b]))
    datacard_txt.write('\n')
    datacard_txt.write("{:<20}".format('observation'))
    datacard_txt_category.write('\n')
    datacard_txt_category.write("{:<20}".format('observation'))
    for o in observations:
        datacard_txt.write("\t{:<20}".format(observations[o]))
        datacard_txt_category.write("\t{:<20}".format(observations[o]))
    datacard_txt.write('\n--------------------------------------------------------------\n')
    datacard_txt.write("{:<20}".format('bin'))
    datacard_txt_category.write('\n--------------------------------------------------------------\n')
    datacard_txt_category.write("{:<20}".format('bin'))
    # processes=[shapes[tmp]['process'] for tmp in shapes[:-1]]
    for b in bins:
        for k in procs:
            datacard_txt.write("\t{:<20}".format(bins[b]))
            datacard_txt_category.write("\t{:<20}".format(bins[b]))
    datacard_txt.write('\n')
    datacard_txt.write("{:<20}".format('process'))
    datacard_txt_category.write('\n')
    datacard_txt_category.write("{:<20}".format('process'))
    for b in bins:
        for k in procs:
            p = shapes[k]['process']
            datacard_txt.write("\t{:<20}".format(p))
            datacard_txt_category.write("\t{:<20}".format(p))
    datacard_txt.write('\n')
    datacard_txt.write("{:<20}".format('process'))
    datacard_txt_category.write('\n')
    datacard_txt_category.write("{:<20}".format('process'))

    for b in bins:
        for k in procs:
            idx = shapes[k]['process_idx']
            datacard_txt.write("\t{:<20}".format(str(idx)))
            datacard_txt_category.write("\t{:<20}".format(str(idx)))
    datacard_txt.write('\n')
    datacard_txt.write("{:<20}".format('rate'))
    datacard_txt_category.write('\n')
    datacard_txt_category.write("{:<20}".format('rate'))

    for b in bins:
        for k in procs:
            r = shapes[k]['rate']
            print(k,r)
            datacard_txt.write("\t{:<20}".format(str(r)))
            if k=='signal':
                # print(r_wt_cat*r_wt*shapes[k]['rate'])
                r=sig_rate*r_wt_cat # remove previous weighting and add new
                if round(r,3) == 0:
                    r = 0.001
                # else:
                #     r = round(r,3)
            datacard_txt_category.write("\t{:<20}".format(str(r)))
    datacard_txt.write('\n--------------------------------------------------------------\n')
    datacard_txt_category.write('\n--------------------------------------------------------------\n')
    for syst in systs:
        datacard_txt.write("{:<10} {:<4} {:<4}".format(syst,systs[syst]['type'],systs[syst]['vals']['extra']))
        datacard_txt_category.write("{:<10} {:<4} {:<4}".format(syst,systs[syst]['type'],systs[syst]['vals']['extra']))
        for b in bins:
            for k in procs:
                datacard_txt.write("\t{:<20}".format(systs[syst]['vals'][k]))
                datacard_txt_category.write("\t{:<20}".format(systs[syst]['vals'][k]))
        datacard_txt.write('\n')
        datacard_txt_category.write('\n')
                
    # datacard_txt.write('\n---------------------------------------------')
    datacard_txt.close()
    datacard_txt_category.close()
    print('saved at ',dc_dir+"/"+"datacard_"+key+fname_suf+".txt")
    print('saved at ',dc_dir+"/"+"datacard_"+key+fname_suf+"_"+category+".txt")
    # del datacard_txt
    # # plot.Delete();

    # del mass;del mc;del MH;del sigma;del dMH;
    # del mean;del model;del norm_sig;
    # del w_sig;del data;
    # gc.enable()
    print('check:'+args.category+'f1, f2, f3:',f1,f2,round(f3,5))
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# # Extension: binned likelihood model
# f = ROOT.TFile("data_part1.root","r")
# t = f.Get("data_"+category)
# 
# # Convert TTree to a RooDataSet
# data = ROOT.RooDataSet("data_"+category, "data_"+category, t, ROOT.RooArgSet(mass,weight), "", "weight")
# 
# # Set bin number for mass variables
# mass.setBins(320)
# data_hist = ROOT.RooDataHist("data_hist_"+category, "data_hist_"+category, mass, data)
# 
# # Save the background model with the RooDataHist instead
# f_out = ROOT.TFile("workspace_bkg_binned.root", "RECREATE")
# w_bkg = ROOT.RooWorkspace("workspace_bkg","workspace_bkg")
# getattr(w_bkg, "import")(data_hist)
# getattr(w_bkg, "import")(norm)
# getattr(w_bkg, "import")(model_bkg)
# w_bkg.Print()
# w_bkg.Write()
# f_out.Close()


if __name__ == '__main__':
    main()
