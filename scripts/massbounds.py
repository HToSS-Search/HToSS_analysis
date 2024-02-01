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
# colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
colors = [45, 85]
markers = [20, 21, 22, 33, 47]

def XPercentInterval(hist, alpha, verbose=False):
    bin1, binN = hist.FindFirstBinAbove(), hist.FindLastBinAbove() #default is 0
    norm = hist.Integral(bin1,binN)
    #print("Check this too - ",fracIntegral,hist.Integral(bin1,binN),norm)
    bin_low, bin_high = bin1, binN
    fracIntegral_low = hist.Integral(bin1, bin_low)/norm
    fracIntegral_high = hist.Integral(bin_high,binN)/norm
    while (fracIntegral_low<alpha):
        bin_low = bin_low+1
        fracIntegral_low = hist.Integral(bin1, bin_low)/norm
    if bin_low!=bin1:
        minX = hist.GetBinLowEdge(bin_low) + hist.GetBinWidth(bin_low)*(alpha*norm-(hist.Integral(bin1, bin_low-1)))/hist.GetBinContent(bin_low)
    else:
        minX = hist.GetBinLowEdge(bin_low) + hist.GetBinWidth(bin_low)*alpha*norm/hist.GetBinContent(bin_low)
    while (fracIntegral_high<alpha):
        bin_high = bin_high - 1
        fracIntegral_high = hist.Integral(bin_high, binN) / norm
    if bin_high!=binN:
        maxX = hist.GetBinLowEdge(bin_high)+hist.GetBinWidth(bin_high)-hist.GetBinWidth(bin_high)*(alpha*norm-(hist.Integral(bin_high,binN)))/hist.GetBinContent(bin_high)
    else:
        maxX = hist.GetBinLowEdge(bin_high) + hist.GetBinWidth(bin_high) - (hist.GetBinWidth(bin_high) * alpha * norm) / hist.GetBinContent(bin_high)
    if verbose:
        print ("Check this-",bin_low,bin_high,hist.FindFirstBinAbove(),hist.FindLastBinAbove())
        print (minX, maxX,hist.Integral(bin1,bin_low)/norm,hist.Integral(bin_high,binN)/norm)
    return minX, maxX

def GaussFit(hist, masspoint, ctau, output):
    print("Performing a fit using a gaussian to get the mean and the width for mass assumption")
    # Declare the observable mean, and import the histogram to a RooDataHist

    tmp_sigma, tmp_mean = hist.GetRMS(), hist.GetMean()
    xlabel = "0.5*(m_{hh}+m_{#mu#mu}) (GeV)"
    mass = ROOT.RooRealVar("mass", xlabel, 0., 3.);
    dh_hist = ROOT.RooDataHist("dh_hist", "dh_hist", ROOT.RooArgList(mass),
                                    ROOT.RooFit.Import(hist));

    # plot the data hist with error from sum of weighted events
    frame = mass.frame(ROOT.RooFit.Title("AvgMass"))
    dh_hist.plotOn(frame, ROOT.RooFit.DataError(ROOT.RooAbsData.SumW2), ROOT.RooFit.MarkerColor(8), ROOT.RooFit.Name("hist"))
    
    # Convert them to pdf

    # create a simple gaussian pdf
    gauss_mean = ROOT.RooRealVar("mean", "mean", tmp_mean, tmp_mean-0.5, tmp_mean+0.5)
    gauss_sigma = ROOT.RooRealVar("sigma jer", "sigma gauss", tmp_sigma, 0.001, 0.1)
    gauss = ROOT.RooGaussian("gauss", "gauss", mass, gauss_mean, gauss_sigma)

    alp=0.20
    minX, maxX = XPercentInterval(hist,alp/2,verbose=True) #removing x% from each side
    width = maxX - minX

    gauss.fitTo(dh_hist,ROOT.RooFit.Save(),ROOT.RooFit.SumW2Error(True) ,ROOT.RooFit.Range(minX,maxX))
    gauss.plotOn(frame, ROOT.RooFit.Name("fit"))


    frame.SetMaximum(frame.GetMaximum() * 1.25)
    frame.SetMinimum(0)
    frame.getAttMarker().SetMarkerColor(ROOT.kBlue)
    frame.getAttMarker().SetMarkerSize(1.5)
    frame.getAttLine().SetLineColor(ROOT.kBlue)


    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2, 0.6, 0.2, 0.88, "NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(frame.chiSquare(),2))
    chi2_text.AddText("#sigma "+"= {} #pm {}".format(round(gauss_sigma.getVal(),3), round(gauss_sigma.getError(),3)))
    chi2_text.AddText("#mu "+"= {} #pm {}".format(round(gauss_mean.getVal(),3), round(gauss_mean.getError(),3)))

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
    frame.GetXaxis().SetRangeUser(tmp_mean-0.1,tmp_mean+0.1)
    frame.Draw()

    legxlow, legxhigh = 0.75,0.9
    legend = ROOT.TLegend(legxlow,0.7,legxhigh,0.9)
    legend.AddEntry(frame.findObject("hist"),"ggH","lep")
    legend.AddEntry(frame.findObject("fit"),"gauss","l")
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.03)
    legend.Draw("same")
    
    rtext = "#splitline{m_{a} ="+str(masspoint)+" GeV}{c#tau = "+str(ctau)+"mm}"
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
    CMS_lumi.lumi_13TeV = "UL2017MC"
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
    fit_filename = "fit_MS" + str(masspoint).replace(".","p") + "_ctauS" + ctau
    # if not os.path.exists(fit_plot_directory): os.makedirs(fit_plot_directory)
    cfit.SaveAs(os.path.join(output, fit_filename + ".pdf"))
    cfit.SaveAs(os.path.join(output, fit_filename + ".png"))
    del cfit

    return round(gauss_sigma.getVal(),3), round(gauss_sigma.getError(),3), round(gauss_mean.getVal(),3), round(gauss_mean.getError(),3)

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
    # h3.SetMinimum(0.)
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
def txt_to_pts(f_in):
    next(f_in)
    # print(f_in.readlines())
    ms, width,minX,maxX=[],[],[],[]
    for line in f_in.readlines():
        ms.append(float(line.split("\t")[0]))
        width.append(float(line.split("\t")[1]))
        minX.append(float(line.split("\t")[2]))
        maxX.append(float(line.split("\t")[3].split('\n')[0]))
    print(ms,width,minX,maxX)
    ms, width,minX,maxX=np.array(ms),np.array(width),np.array(minX),np.array(maxX)
    y_up = maxX
    y_down = minX
    gr_up=ROOT.TGraph(len(ms),ms,y_up)
    gr_down=ROOT.TGraph(len(ms),ms,y_down)
    gr_up.Fit("pol1","0")
    gr_down.Fit("pol1","0")
    fit_l = gr_down.GetFunction("pol1")
    fit_h = gr_up.GetFunction("pol1")
    func_l = ROOT.TF1("func_l","[1]*x+[0]",0.,5.)
    func_h = ROOT.TF1("func_h","[1]*x+[0]",0.,5.)
    func_l.FixParameter(0,fit_l.GetParameter(0))
    func_l.FixParameter(1,fit_l.GetParameter(1))
    func_h.FixParameter(0,fit_h.GetParameter(0))
    func_h.FixParameter(1,fit_h.GetParameter(1))
    return gr_up, gr_down, func_l, func_h, ms
def main():
    #ROOT.TH1.AddDirectory(ROOT.kFALSE)

    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("--input1",  dest="fin1",   help="input folder 1", type=str)
    parser.add_argument("--input2",  dest="fin2",   help="input folder 2", type=str)
    # parser.add_argument("--masstype",  dest="h1",   help="dihadron or dimuon?", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-r","--rebinfactor", dest="rfactor", help="Rebin Factor", type=int)
    parser.add_argument("--xlow", dest="xlow", help="Set xaxis range", type=float)
    parser.add_argument("--xhigh", dest="xhigh", help="Set xaxis range", type=float)
    parser.add_argument("-y","--setlogY", dest="log", help="Set log Y or not", default=0, type=int)
    parser.add_argument("--log", dest="log", help="Set log Y or not", default=0, type=int)
    parser.add_argument("--yhigh", dest="yhigh", help="Set yaxis high (in terms of frac/out of 1)", type=float)
    parser.add_argument("-n","--normalize",dest="norm", help="To norm or not to norm",  default=1,type=int)
    parser.add_argument("--updatecuts", dest="updatecuts",   help="update cuts with mass bounds derived here", action='store_true')

    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()
    # Get the list of all files and directories
    path1 = args.fin1
    path2 = args.fin2
    dir_path1 = cwd+"/"+path1
    dir_path2 = cwd+"/"+path2
    dir_list1 = os.listdir(dir_path1)
    dir_list2 = os.listdir(dir_path2)
    
    # print(dir_list1)
    fpaths = []
    for fname in dir_list1:
        if ".root" not in fname:
            continue
        if "ctauS0" not in fname:
            continue
        fpaths.append(dir_path1+"/"+fname)
    for fname in dir_list2:
        if ".root" not in fname:
            continue
        if "ctauS0" not in fname:
            continue
        fpaths.append(dir_path2+"/"+fname)
    print(fpaths)
    # quit()
    # hlist={"dihadron":"h_DiChHadMass","dimuon":"h_DiMuonMass","avgmass":"h_AvgMass_mumu_hh"}
    hlist={"dihadron":"h_DiChHadMass","dimuon":"h_DiMuonMass"}
    for h1 in hlist:
        txtfile=open(args.out+'/'+'bounds_'+h1+'.txt','w')
        txtfile.write("MS\twidth\tminX\tmaxX")
        for fpath in fpaths:
            f_in = ROOT.TFile(fpath,"READ")
            ROOT.gStyle.SetOptStat(0)
            # ROOT.gStyle.SetPalette(ROOT.kBird)
            fname = fpath.split("/")[-1]
            mass=fname.split("MS")[1].split("_")[0].replace('p','.')
            ctau=fname.split("ctauS")[1].split(".root")[0]
            m_size = 1.5
            hname = hlist[h1]
            print(hname)
            h_1 = f_in.Get(hname)
            if args.rfactor != 0: 
                h_1.Rebin(args.rfactor)
            if (args.norm):
                h_1.Scale(1./h_1.Integral())
            if "avgmass" in h1:
                sigma,sigma_err,mean,mean_err= GaussFit(h_1,mass,ctau,args.out)
                txtfile.write("\n")
                txtfile.write(mass+"\t"+str(sigma*3)+"\t"+str(mean-1.5*sigma)+"\t"+str(mean+1.5*sigma))
            else:
                c = ROOT.TCanvas("c", "stacked hists", 1200, 1000)
                if args.log: 
                    print("enters log")
                    ROOT.gPad.SetLogy(1)
                    c.SetLogy(1)
                else:
                    ROOT.gPad.SetLogy(0)
                    c.SetLogy(0)

                c.cd()
                xleg,yleg = 0.7, 0.8
                leg = ROOT.TLegend(xleg, yleg, 0.9, 0.9)

                alp=0.05 # 2 sigma
                sigmas = [0.99, 0.98, 0.95, 0.87, 0.68]
                alphas = [0.01, 0.02, 0.05, 0.13, 0.32]
                conversions = [2.576, 2.326, 1.960, 1.514, 0.9945]
                # Exclusion from graph below
                minXt, maxX = XPercentInterval(h_1,alp/2,verbose=True) #removing x% from each side
                width = maxX - minXt
                print("Check selected interval:",minXt, maxX, width)

                txtfile.write("\n")
                txtfile.write(mass+"\t"+str(width)+"\t"+str(minXt)+"\t"+str(maxX))

                h_1.SetMarkerColor(colors[0])
                h_1.SetLineColor(colors[0])
                h_1.SetMarkerStyle(markers[0])
                h_1.SetMarkerSize(m_size)
                h_1.GetXaxis().SetLabelSize(0.04)
                h_1.GetYaxis().SetLabelSize(0.04)
                h_1.GetXaxis().SetTitleSize(0.05)
                h_1.GetYaxis().SetTitleSize(0.05)
                # h_1.SetAxisRange(args.xlow,args.xhigh)
                h_1.SetAxisRange(h_1.GetMean()-0.5,h_1.GetMean()+0.5)
                # h_1.GetXaxis().SetLimits(args.xlow,args.xhigh)
                if "hadron" in h1:
                    xlabel = "m_{h^{+}h^{-}} (GeV)"
                else:
                    xlabel = "m_{#mu^{+}#mu^{-}} (GeV)"
                h_1.GetXaxis().SetTitle(xlabel)
                binwidth = h_1.GetBinWidth(1)
                if args.norm:
                    h_1.GetYaxis().SetTitle("Normalized Events" + " / "+str(binwidth)+" GeV")
                else:
                    h_1.GetYaxis().SetTitle("Events" + " / "+str(binwidth)+" GeV")

                # y-axis range modification
                if args.log:
                    ymin, ymax = 1e-3, 0.2
                else:
                    ymin, ymax = 0, h_1.GetMaximum()
                h_1.GetYaxis().SetRangeUser(ymin,(args.yhigh)*ymax)

                l1 = ROOT.TLine(minXt,ymin,minXt,(args.yhigh)*ymax)
                l2 = ROOT.TLine(maxX,ymin,maxX,(args.yhigh)*ymax)

                # leg.AddEntry(h_1, args.l1, "lep")
                # c1.SetLogy()

                h_1.Draw("pe1 PLC PMC")
                leg.Draw("same")
                l1.Draw("same")
                l2.Draw("same")
                c.Update()

                rtext = "#splitline{m_{a} ="+mass+" GeV}{c#tau = "+ctau+"mm}"
                additional_text = []
                additional_text += [rtext]
                if additional_text:
                    nother = len(additional_text)
                    dims = [xleg-0.05, yleg - nother * 0.04 - 0.02, 0.85, 0.65]
                    text = ROOT.TPaveText(*dims + ['NB NDC'])
                    text.SetTextFont(42)
                    text.SetBorderSize(0)
                    text.SetFillColor(0)
                    text.SetTextAlign(11)
                    text.SetTextSize(0.04)
                    for rtext in additional_text:
                        text.AddText(rtext)
                    text.Draw()

                CMS_lumi.cmsText = 'CMS'
                CMS_lumi.writeExtraText = True
                CMS_lumi.extraText = 'Simulation'
                CMS_lumi.lumi_13TeV = "UL2017 MC"
                # CMS_lumi.lumiTextSize = 0.5
                CMS_lumi.cmsTextSize=1.
                CMS_lumi.CMS_lumi(c, 4, 11)
                c.Modified()
                c.Update()

                frtext = "MS"+mass.replace('.','p')+"_"+"ctau"+ctau.replace('.','p')+"_"+h1
                if args.log:
                    frtext+="_"+"log"
                savename = args.out+"/"+frtext
                c.SaveAs(savename+'.png')
                # c.SaveAs(savename+'.pdf')
                # c.SaveAs(savename+'.root')
                c.Clear()
        txtfile.close()


    f_width_mumu=open(args.out+"/"+"bounds_dimuon.txt","r")
    f_width_hh=open(args.out+"/"+"bounds_dihadron.txt","r")
    gr_up_hh, gr_down_hh, func_l_hh, func_h_hh, ms = txt_to_pts(f_width_hh)
    gr_up_mumu_tmp, gr_down_mumu_tmp, func_l_mumu, func_h_mumu, ms = txt_to_pts(f_width_mumu)
    gr_up_mumu=ROOT.TGraph(len(gr_up_mumu_tmp.GetY()),gr_up_mumu_tmp.GetY(),gr_up_mumu_tmp.GetX()) #reversing axis
    gr_down_mumu=ROOT.TGraph(len(gr_down_mumu_tmp.GetY()),gr_down_mumu_tmp.GetY(),gr_down_mumu_tmp.GetX()) #reversing axis
    gr_up_mumu.SetMarkerStyle(21);gr_down_mumu.SetMarkerSize(0.5);func_l_mumu.SetLineStyle(2);
    gr_down_mumu.SetMarkerStyle(21);gr_up_mumu.SetMarkerSize(0.5);func_h_mumu.SetLineStyle(2);

    ex = np.array([(func_h_mumu.Eval(ms_i) - func_l_mumu.Eval(ms_i))/2 for ms_i in ms])
    ey = np.array([(func_h_hh.Eval(ms_i) - func_l_hh.Eval(ms_i))/2 for ms_i in ms])
    y = np.array([(func_h_hh.Eval(ms_i) + func_l_hh.Eval(ms_i))/2 for ms_i in ms])
    gr_bounds=ROOT.TGraphErrors(len(ms),np.array(ms),y,ex,ey)
    # gr_bounds.SetMarkerStyle(21);gr_bounds.SetMarkerSize(1.25);gr_bounds.SetLineStyle(2);
    gr_bounds.SetFillStyle(0);gr_bounds.SetLineWidth(3);gr_bounds.SetLineColor(2);

    x_lt, x_rb = np.array(ms)-ex, np.array(ms)+ex
    y_lt, y_rb = y+ey, y-ey
    gr_lt = ROOT.TGraph(len(ms),x_lt,y_lt)
    gr_lt.SetFillStyle(0);gr_lt.SetLineWidth(3);gr_lt.SetLineColor(2);
    gr_rb = ROOT.TGraph(len(ms),x_rb,y_rb)
    gr_rb.SetFillStyle(0);gr_rb.SetLineWidth(3);gr_rb.SetLineColor(2);

    gr_lt.Fit("pol1","0")
    gr_rb.Fit("pol1","0")
    fit_lt = gr_lt.GetFunction("pol1")
    fit_rb = gr_rb.GetFunction("pol1")

    # lowerbound=[-0.05644029416162604, 0.9833371896663816]
    # higherbound=[0.0733982700792378, 1.006827105815873]
    # params_lower = ROOT.VecOps.RVec('double')(lowerbound)
    # params_higher = ROOT.VecOps.RVec('double')(higherbound)
    params_lower = fit_rb.GetParameters()
    params_higher = fit_lt.GetParameters()
    func_l = ROOT.TF1("func_l","[1]*x+[0]",0.,3.)
    func_h = ROOT.TF1("func_h","[1]*x+[0]",0.,3.)
    func_l.FixParameter(0,params_lower[0])
    func_l.FixParameter(1,params_lower[1])
    func_h.FixParameter(0,params_higher[0])
    func_h.FixParameter(1,params_higher[1])

    f_out=open(args.out+"/"+"function_combined.txt","w")
    f_out.write("function\t[0]\t+(x*)[1]")
    f_out.write("\n")
    f_out.write("LowerBound"+"\t"+str(fit_rb.GetParameter(0))+"\t"+str(fit_rb.GetParameter(1)))
    f_out.write("\n")
    f_out.write("HigherBound"+"\t"+str(fit_lt.GetParameter(0))+"\t"+str(fit_lt.GetParameter(1)))
    f_out.close()
    if args.updatecuts:
        # cwd = os.getcwd()
        # Get the list of all files and directories
        path_cuts = "configs/2017/cuts"
        dir_path_cuts = cwd+"/"+path_cuts
        dir_list_cuts = os.listdir(dir_path_cuts)        
        # print(dir_list1)
        fcuts = []
        for fname_cuts in dir_list_cuts:
            if ".yaml" not in fname_cuts:
                continue
            fcuts.append(dir_path_cuts+"/"+fname_cuts)

        lowerbound_arr,higherbound_arr = [], []
        lowerbound_arr.append(params_lower[0]); lowerbound_arr.append(params_lower[1])
        higherbound_arr.append(params_higher[0]); higherbound_arr.append(params_higher[1])

        for filestr in fcuts:
            cuts_file = open(filestr,'r')
            cut_pars = yaml.safe_load(cuts_file)
            cut_pars['cuts']['recoscalar']['lowerbound'] = lowerbound_arr
            cut_pars['cuts']['recoscalar']['higherbound'] = higherbound_arr
            cuts_file.close()
            print(cut_pars['cuts']['recoscalar'])
            cuts_file = open(filestr,'w')
            yaml.dump(cut_pars,cuts_file)
            cuts_file.close()

    gr_up_hh.SetMarkerStyle(21);gr_up_hh.SetMarkerSize(0.5);func_h_hh.SetLineStyle(2);gr_up_hh.SetMarkerColor(2);gr_up_hh.SetLineColor(2);func_h_hh.SetLineColor(2);
    gr_down_hh.SetMarkerStyle(21);gr_down_hh.SetMarkerSize(0.5);func_l_hh.SetLineStyle(2);gr_down_hh.SetMarkerColor(2);gr_down_hh.SetLineColor(2);func_l_hh.SetLineColor(2);

    leg.AddEntry(gr_up_hh, "m_{hh} bounds", "lep")
    leg.AddEntry(gr_up_mumu, "m_{#mu#mu} bounds", "lep")

    for fpath in fpaths:
        fname = fpath.split("/")[-1]
        m_scalar = fname.split("MS")[1].split("_")[0].replace('p','.')
        ctau=fname.split("ctauS")[1].split(".root")[0]
        c2 = ROOT.TCanvas("c2", "Mass2d", 1400, 1200)
        c2.SetLogz()
        c2.SetRightMargin(0.15)
        f_in = ROOT.TFile(fpath,"READ")
        

        print("file open:",f_in.GetName())
        h_2 = f_in.Get("h_DiMuonMass_DiChHadMass")
        h_2.Scale(41474.989603894)
        h_2.Rebin2D(20,20)
        h_2.GetXaxis().SetTitle("m_{#mu#mu} (GeV)")
        h_2.GetYaxis().SetTitle("m_{h^{+}h^{-}} (GeV)")
        # h_2.GetXaxis().GetBinLowEdge(1); 
        h_2.GetZaxis().SetTitle("Events / "+str(h_2.GetXaxis().GetBinWidth(1))+" GeV")
        h_2.GetZaxis().SetTitleOffset(1.)
        h_2.GetXaxis().SetRangeUser(0.,3.)
        h_2.GetYaxis().SetRangeUser(0.,3.)
        h_2.GetZaxis().SetRangeUser(9*1e-2,300.)

        h_2.GetXaxis().SetLabelSize(0.04)
        h_2.GetYaxis().SetLabelSize(0.04)
        h_2.GetZaxis().SetLabelSize(0.04)
        h_2.GetXaxis().SetTitleSize(0.05)
        h_2.GetYaxis().SetTitleSize(0.05)
        h_2.GetZaxis().SetTitleSize(0.05)


        c2.cd()
        h_2.Draw("colz")
        gr_bounds.Draw("5 same")
        gr_lt.Draw("p same")
        gr_rb.Draw("p same")
        func_l.Draw("same")
        func_h.Draw("same")

        # func_l_mumu.Draw("same")
        # func_h_mumu.Draw("same")
        # gr_down_mumu.Draw("p same")
        # gr_up_mumu.Draw("p same")
        # func_l_hh.Draw("psame")
        # func_h_hh.Draw("psame")
        # gr_down_hh.Draw("p same")
        # gr_up_hh.Draw("p same")
        # leg.Draw("same")

        rtext = "#splitline{m_{a} ="+m_scalar+" GeV}{c#tau = "+ctau+"mm}"
        additional_text = []
        additional_text += [rtext]
        if additional_text:
            nother = len(additional_text)
            # dims = [xleg-0.05, yleg - nother * 0.04 - 0.02, 0.85, 0.65]
            dims = [0.2, yleg - nother * 0.04 - 0.02, 0.25, 0.65]
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
        CMS_lumi.lumi_13TeV = "UL2017 MC"
        # CMS_lumi.lumiTextSize = 0.5
        CMS_lumi.cmsTextSize=1.
        CMS_lumi.CMS_lumi(c2, 4, 11)
        c2.Modified()
        c2.Update()
    
        c2.SaveAs(args.out+"/"+"Mass2d_dist_MS"+m_scalar+"_"+"ctau"+ctau+'.png')
        # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.pdf')
        # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.root')
        c2.Clear()


if __name__ == '__main__':
    main()
