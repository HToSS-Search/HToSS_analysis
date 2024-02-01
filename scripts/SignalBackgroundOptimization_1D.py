import ROOT
import os,argparse
import tdrstyle
import CMS_lumi
import numpy as np

ROOT.gROOT.SetBatch(True)
tdrstyle.setTDRStyle()

def plot_on_canvas(hist, dname, dlabel, outfolder, key, suf):
    if 'SigOverBkg' in suf:
        ROOT.gStyle.SetPaintTextFormat("1.3f")
    else:
        ROOT.gStyle.SetPaintTextFormat("1.1f")
    boundary_percent = 0.35
    ylength_c = int(2400*(1-boundary_percent+0.15))
    c1 = ROOT.TCanvas('c1', 'c1', 2200, ylength_c)
    pad1 = ROOT.TPad("pad1", "pad1", 0, 0, 1, 1)
    pad1.SetTopMargin(0.06)
    pad1.SetBottomMargin(0.15)
    pad1.SetLeftMargin(0.16)
    pad1.SetRightMargin(0.04)
    # pad1.SetLogy()
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c1.cd()
    pad1.cd()
    if 'cumulative' in suf:
        hist.Draw("HIST SAME text")
    else:
        hist.Draw("HIST SAME")

    rtext = dlabel
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
    pad1.Modified()
    pad1.Update()

    CMS_lumi.cmsText = 'CMS'
    CMS_lumi.writeExtraText = True
    CMS_lumi.extraText = 'Work in progress'
    if 'Run' in dname or 'SigOverBkg' in suf:
        CMS_lumi.lumi_13TeV = "41.4 fb^{-1}"
    else:
        CMS_lumi.lumi_13TeV = "UL2017 MC"
    
    CMS_lumi.cmsTextSize = 0.6
    CMS_lumi.lumiTextSize = 0.55
    # CMS_lumi.relPosX = 0.05
    CMS_lumi.CMS_lumi(pad1, 4, 0)
    pad1.Modified()
    pad1.Update()
    print('canvas size - ',)

    c1.SaveAs(outfolder+"/"+key+'_'+dname+'_'+suf+".png")
    # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.pdf')
    # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.root')
    c1.Clear()

def main():

    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    ROOT.gStyle.SetPaintTextFormat("1.1f");
    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-f", "--file",   dest="fname",   help="file location", type=str)
    parser.add_argument("-o", "--out",  dest="out",   help="output folder", type=str)
    parser.add_argument("-d", "--dname",  dest="dname",   help="signal dataset name", type=str)
    # parser.add_argument("--hname",   dest="hname",   help="histogram name", type=str)
    # parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    # parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    # parser.add_argument("--mc",   dest="mc",   help="is MC?", action='store_true')
    args = parser.parse_args()

    histo_dict = {
        # 'leadingMuonRelIso': {'hname':'h_leadingMuonRelIso','label': "leading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'subleadingMuonRelIso': {'hname':'h_subleadingMuonRelIso','label': "subleading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'leadingChRelIso': {'hname':'h_leadingChHadRelIso','label': "leading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},
        # 'subleadingChRelIso': {'hname':'h_subleadingChHadRelIso','label': "subleading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':10.,'hrebin':40},

        # 'leadingMuonSumPtCh': {'hname':'h_leadingMuonSumPtCh','label': "leading #mu #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingMuonSumPtCh': {'hname':'h_subleadingMuonSumPtCh','label': "subleading #mu #Sigma pp_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'leadingChSumPtCh': {'hname':'h_leadingChHadSumPtCh','label': "leading h^{#pm} #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},
        # 'subleadingChSumPtCh': {'hname':'h_subleadingChHadSumPtCh','label': "subleading h^{#pm} #Sigma p_{T}(ch. had.) (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':40},

        'DiChHadPt': {'hname':'h_DiChHadPt','label': "p_{T,h^{+}h^{-}} (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':10},
        'DiChHadMass': {'hname':'h_DiChHadMass','label': "m_{h^{+}h^{-}} (GeV)", 'xlow':0.5,'xhigh':3.5,'hrebin':40},
        'DiMuonPt': {'hname':'h_DiMuonPt','label': "p_{T,#mu#mu} (GeV)", 'xlow':0.,'xhigh':100.,'hrebin':10},
        'DiMuonMass': {'hname':'h_DiMuonMass','label': "m_{#mu#mu} (GeV)", 'xlow':0.5,'xhigh':3.5,'hrebin':40},
        'AvgMass_mumu_hh':{'hname':'h_AvgMass_mumu_hh','label':"0.5*(m_{hh}+m_{#mu#mu}) (GeV)",'xlow':0,'xhigh':3.5,'hrebin':40},

    }
    datasets_dict = {
        'HToSS_MH125_MS2_ctauS0':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 0mm}"},
        'HToSS_MH125_MS2_ctauS1':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 1mm}"},
        'HToSS_MH125_MS2_ctauS10':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 10mm}"},
        'HToSS_MH125_MS2_ctauS100':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 100mm}"},
        'Run2017':{'type':'blinded_data','label':'Run2017'}
    }
    for key in histo_dict:
        ####################################### SIGNAL SAMPLE #############################################
        # Run the function
        dataset_name = args.dname
        fin = ROOT.TFile(args.fname+'/total/'+'output_'+dataset_name+'.root',"READ")
        hist=fin.Get(histo_dict[key]['hname']).Clone()
        if 'Run' not in dataset_name:
            hist.Scale(41474.989603894)
        # xlow, ylow = 0, 0
        # xhigh, yhigh = 1000000, 1000000
        integral = hist.Integral(0,hist.GetNbinsX()+1)
        # integral_whole = calculate_integral(hist,xlow,xhigh,ylow,yhigh)
        print("\n")
        print("Cross check of integral(direct):",integral)

        hist.Rebin(histo_dict[key]['hrebin'])

        hist.GetXaxis().SetTitle(histo_dict[key]['label'])
        # hist.GetXaxis().GetBinLowEdge(1); 
        hist.GetYaxis().SetTitle("Events / "+str(hist.GetXaxis().GetBinWidth(1)))
        # hist.GetYaxis().SetTitleOffset(1.)
        hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        maxbin = hist.GetMaximumBin()
        maxval = hist.GetBinContent(maxbin)
        minbin = hist.GetMinimumBin()
        minval = hist.GetBinContent(minbin)
        hist.GetYaxis().SetRangeUser(0.1*minval,maxval*1.5)

        # hist.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
        # hist.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
        # hist.GetZaxis().SetRangeUser(9*1e-2,300.)
        # hist.GetZaxis().SetRangeUser(histo_dict[key]['zlow'],histo_dict[key]['zhigh'])

        hist.GetXaxis().SetLabelSize(0.045)
        hist.GetYaxis().SetLabelSize(0.045)
        hist.GetXaxis().SetTitleOffset(1.)
        hist.GetYaxis().SetTitleOffset(1.2)

        plot_on_canvas(hist, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'original')

        hist.Rebin(4)
        cumulative_hist = hist.Clone()
        cumulative_hist.Reset()
        for ix in range(1, hist.GetNbinsX() + 1):
            content = hist.GetBinContent(ix)
            cumulative_content = cumulative_hist.GetBinContent(ix - 1) + content
            cumulative_hist.SetBinContent(ix, cumulative_content)
        maxbin = cumulative_hist.GetMaximumBin()
        maxval = cumulative_hist.GetBinContent(maxbin)
        minbin = cumulative_hist.GetMinimumBin()
        minval = cumulative_hist.GetBinContent(minbin)
        cumulative_hist.GetYaxis().SetRangeUser(0.1*minval,maxval*1.5)
        cumulative_hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        cumulative_hist.GetYaxis().SetTitle("Events / "+str(cumulative_hist.GetXaxis().GetBinWidth(1)))


        plot_on_canvas(cumulative_hist, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'cumulative')
        cumulative_hist_percent=cumulative_hist.Clone()
        cumulative_hist_percent.Scale(1/integral*100)
        cumulative_hist_percent.GetYaxis().SetRangeUser(0.,100.)
        plot_on_canvas(cumulative_hist_percent, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'cumulative_percent')

        ####################################### DATA #############################################

        dataset_name = 'Run2017'
        fin = ROOT.TFile(args.fname+'/total/'+'output_'+dataset_name+'.root',"READ")
        hist=fin.Get(histo_dict[key]['hname']).Clone()
        if 'Run' not in dataset_name:
            hist.Scale(41474.989603894)
        integral = hist.Integral(0,hist.GetNbinsX()+1)
        # integral_whole = calculate_integral(hist,xlow,xhigh,ylow,yhigh)
        print("\n")
        print("Cross check of integral(direct):",integral)

        hist.Rebin(histo_dict[key]['hrebin'])

        hist.GetXaxis().SetTitle(histo_dict[key]['label'])
        # hist.GetXaxis().GetBinLowEdge(1); 
        hist.GetYaxis().SetTitle("Events / "+str(hist.GetXaxis().GetBinWidth(1)))
        # hist.GetYaxis().SetTitleOffset(1.)
        hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        maxbin = hist.GetMaximumBin()
        maxval = hist.GetBinContent(maxbin)
        minbin = hist.GetMinimumBin()
        minval = hist.GetBinContent(minbin)
        hist.GetYaxis().SetRangeUser(0.1*minval,maxval*1.5)

        # hist.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
        # hist.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
        # hist.GetZaxis().SetRangeUser(9*1e-2,300.)
        # hist.GetZaxis().SetRangeUser(histo_dict[key]['zlow'],histo_dict[key]['zhigh'])

        hist.GetXaxis().SetLabelSize(0.045)
        hist.GetYaxis().SetLabelSize(0.045)
        hist.GetXaxis().SetTitleOffset(1.)
        hist.GetYaxis().SetTitleOffset(1.2)

        plot_on_canvas(hist, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'original')

        hist.Rebin(4)
        cumulative_hist_data = hist.Clone()
        cumulative_hist_data.Reset()
        for ix in range(1, hist.GetNbinsX() + 1):
            content = hist.GetBinContent(ix)
            cumulative_content = cumulative_hist_data.GetBinContent(ix - 1) + content
            cumulative_hist_data.SetBinContent(ix, cumulative_content)
        maxbin = cumulative_hist_data.GetMaximumBin()
        maxval = cumulative_hist_data.GetBinContent(maxbin)
        minbin = cumulative_hist_data.GetMinimumBin()
        minval = cumulative_hist_data.GetBinContent(minbin)
        cumulative_hist_data.GetYaxis().SetRangeUser(0.1*minval,maxval*1.5)
        cumulative_hist_data.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        cumulative_hist_data.GetYaxis().SetTitle("Events / "+str(cumulative_hist_data.GetXaxis().GetBinWidth(1)))


        plot_on_canvas(cumulative_hist_data, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'cumulative')
        cumulative_hist_data_percent=cumulative_hist_data.Clone()
        cumulative_hist_data_percent.Scale(1/integral*100)
        cumulative_hist_data_percent.GetYaxis().SetRangeUser(0.,100.)
        plot_on_canvas(cumulative_hist_data_percent, dataset_name, datasets_dict[dataset_name]['label'], args.out, key,'cumulative_percent')

        cumulative_hist_SigOverBkg = cumulative_hist.Clone()
        cumulative_hist_SigOverBkg.Reset()
        for ix in range(1, cumulative_hist_data.GetNbinsX() + 1):
            sig_content = cumulative_hist.GetBinContent(ix)
            data_content = cumulative_hist_data.GetBinContent(ix)
            print('check-',sig_content,data_content)
            content = sig_content/np.sqrt(data_content) if data_content>0 else 0.0
            cumulative_hist_SigOverBkg.SetBinContent(ix, content)
        maxbin = cumulative_hist_SigOverBkg.GetMaximumBin()
        maxval = cumulative_hist_SigOverBkg.GetBinContent(maxbin)
        minbin = cumulative_hist_SigOverBkg.GetMinimumBin()
        minval = cumulative_hist_SigOverBkg.GetBinContent(minbin)
        cumulative_hist_SigOverBkg.GetYaxis().SetRangeUser(0.1*minval,maxval*1.5)
        cumulative_hist_SigOverBkg.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
        cumulative_hist_SigOverBkg.GetYaxis().SetTitle("S/#sqrt{B} / "+str(cumulative_hist_SigOverBkg.GetXaxis().GetBinWidth(1)))

        plot_on_canvas(cumulative_hist_SigOverBkg, args.dname, datasets_dict[args.dname]['label'], args.out, key,'cumulative_SigOverBkg')


if __name__ == '__main__':
    main()
