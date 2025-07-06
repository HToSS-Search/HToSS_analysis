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

def SignalYield(key,fname,plot_src,category,hname,lo_bound,hi_bound,eff_dict,apply_eff=True):
    # fname = datasets_dict[key]['fname']
    
    # fname=datasets_dict_key['fname']
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
    if apply_eff:
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
    # if round(float(ctau),3) in [0,1,10,100,0.774,0.129,1.292,12.915,0.077]:
    #     can.Draw()
    #     can.SaveAs(plot_dir+"/"+"fit_"+key+"_sig.png")

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

def main():
    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    ROOT.RooFit.PrintLevel(5)
    # add arguments below
    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-i", "--indir",   dest="indir",   help="hist file location", type=str)
    # parser.add_argument("-o", "--outdir",   dest="outdir",   help="plotting location", type=str)
    # parser.add_argument("-p", "--prefix",   dest="prefix",   help="limit files location", type=str)
    parser.add_argument("--htype",   dest="htype",   help="kaon or pion", type=str)
    parser.add_argument("--mass", dest="mass", help="scalar mass", type=str)
    parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("--counting", dest="countinglims", help="true for generating counting datacards, false by default", action="store_true")
    # parser.add_argument("--category", dest="category", help="prompt,displacedhh,displacedmumu,displaced", type=str)
    # parser.add_argument("--ctau",dest="ctau",help="ctau",type=str)

    # parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    # parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    args = parser.parse_args()
    cwd = os.getcwd()

    global lumi 
    lumi = lumi_scale[args.year]

    # datasets_dict = {}
    # m_point = str(args.mass).replace('.','p')
    # ctau = f'{args.ctau:g}'.replace('.','p')
    m_point=args.mass
    # ctaus=['0','1','10','100']
    ctaus=['0']
    categories=['prompt','displacedmumu','displacedhh','displaced']
    yields={}
    yields_vbf={}

    yields_err={}
    yields_vbf_err={}

    plot_src = args.indir
    fin = open('BR_xsec_benchmark.yaml','r')
    br_pars = yaml.safe_load(fin)
    xsec_ggH = br_pars['xsec_ggH']
    br_Haa = br_pars['br_Haa']
    global htype
    htype=args.htype
    fin2 = open('bounds'+'.yaml','r')
    bound_pars = yaml.safe_load(fin2)
    lo_bound,hi_bound=bound_pars['MS'+m_point][args.year]
    print(lo_bound,hi_bound)
    fin2.close()
    for category in categories:
        yields[category]={}
        yields_vbf[category]={}
        yields_err[category]={}
        yields_vbf_err[category]={}
        for ct in ctaus:
            fname='output_HToSS_MH125_MS'+m_point+'_ctauS'+ct+'.root'
            fname_vbf='output_HToSS_MH125_MS'+m_point+'_ctauS'+ct+'_prefire.root'
            key='HToSS_MS'+m_point+'_ctau'+ct
            feff_fit_dict={}
            #### TESTING TESTING TESTING ####
            feff_fit=open("gen_eff_fit_"+htype+".txt","r")
            geneff=1
            for line in feff_fit:
                # print(ctau,float(line.split('\t')[0].strip()))
                feff_fit_dict[float(line.split('\t')[0].strip())]=float(line.split('\t')[1].replace('\n',''))
            mass_prev = ""
            hname = "h_recoHiggsMass_AvgMass_mumu_hh_MH_BC"
            masspoint=key.split("MS")[1].split("_")[0]
            sig_rate,sig_err=SignalYield(key,fname,plot_src,category,hname,lo_bound,hi_bound,feff_fit_dict)
            sig_rate_vbf,sig_err_vbf=SignalYield(key,fname_vbf,plot_src,category,hname,lo_bound,hi_bound,feff_fit_dict,True)
            print('SEETHIS:',sig_rate,sig_err)
            yields[category][ct]=round(sig_rate,4)
            yields_vbf[category][ct]=round(sig_rate_vbf,4)
            yields_err[category][ct]=round(sig_err,4)
            yields_vbf_err[category][ct]=round(sig_err_vbf,4)
    print(yields)
    print(yields_vbf)

    def format_latex_table(data):
        headers = [''] + list(next(iter(data.values())).keys())
        rows = [[name] + [f"{v:.3f}" for v in data[name].values()] for name in data]
        latex = "\\begin{tabular}{|c|c|c|c|c|}\n\\hline\n"
        latex += " & ".join(headers) + " \\\\\n\\hline\n"
        for row in rows:
            latex += " & ".join(row) + " \\\\\n\\hline\n"
        latex += "\\end{tabular}"

        return latex
    yield_sum={}
    yield_sum_err={}
    yield_vbf_sum={}
    yield_vbf_sum_err={}
    eff_inc={}
    eff_inc_err={}
    for ct in ctaus:
        yield_sum[ct]=0
        yield_vbf_sum[ct]=0
        yield_sum_err[ct]=0
        yield_vbf_sum_err[ct]=0
        eff_inc_err[ct]=0
        for category in categories:
            yield_sum[ct]=yield_sum[ct]+yields[category][ct]
            yield_vbf_sum[ct]=yield_vbf_sum[ct]+yields_vbf[category][ct]
            yield_sum_err[ct]=yield_sum_err[ct]+yields_err[category][ct]**2
            yield_vbf_sum_err[ct]=yield_vbf_sum_err[ct]+yields_vbf_err[category][ct]**2
            if ct=='0':
                print(category,ct,yields[category][ct])
                print(category,ct,yields_vbf[category][ct])
        yield_vbf_sum_err[ct]=round(np.sqrt(yield_vbf_sum_err[ct]),4)
        yield_sum_err[ct]=round(np.sqrt(yield_sum_err[ct]),4)
        yield_vbf_sum[ct]=round(yield_vbf_sum[ct],4)
        yield_sum[ct]=round(yield_sum[ct],4)
        eff_inc[ct]=round(abs(yield_sum[ct]-yield_vbf_sum[ct])/yield_sum[ct],4)
        eff_inc_err[ct]=round(np.sqrt((yield_vbf_sum_err[ct]/yield_vbf_sum[ct])**2+(yield_sum_err[ct]/yield_sum[ct])**2),2)
    
    print(yield_vbf_sum)
    print(yield_sum)
    print(eff_inc)
    quit()

    fyaml=open('Prefire_effect.yaml','r')
    yaml_d = yaml.safe_load(fyaml)
    fyaml.close()
    yaml_d['MS'+args.mass]['ggH']={}
    yaml_d['MS'+args.mass]['Prefiring']={}
    yaml_d['MS'+args.mass]['inc']={}
    for ct in ctaus:
        yaml_d['MS'+args.mass]['ggH'][ct]=[float(yield_sum[ct]),float(yield_sum_err[ct])]
        yaml_d['MS'+args.mass]['Prefiring'][ct]=[float(yield_vbf_sum[ct]),float(yield_vbf_sum_err[ct])]
        yaml_d['MS'+args.mass]['inc'][ct]=[float(eff_inc[ct]),float(eff_inc_err[ct])]
    print(yaml_d)
    fyaml=open('Prefire_effect.yaml','w')
    yaml.safe_dump(yaml_d, fyaml, sort_keys=False, default_flow_style=False)
    fyaml.close()
    fout=open('Prefiring_compare.txt','w')
    latex_code = format_latex_table(yields)
    fout.write('Prefiring; MS'+args.mass)
    fout.write('\n')
    fout.write(latex_code)
    fout.write('\n')
    fout.write('ggH; MS'+args.mass)
    fout.write('\n')
    fout.write(format_latex_table(yields_vbf))
    fout.close()
if __name__ == '__main__':
    main()
