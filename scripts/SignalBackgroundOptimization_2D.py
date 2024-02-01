import ROOT
import os,argparse
import tdrstyle
import CMS_lumi
import numpy as np

ROOT.gROOT.SetBatch(True)
tdrstyle.setTDRStyle()

def calculate_integral(hist, xmin, xmax, ymin, ymax):

    bin_x1 = hist.GetXaxis().FindBin(xmin)
    bin_x2 = hist.GetXaxis().FindBin(xmax)
    bin_y1 = hist.GetYaxis().FindBin(ymin)
    bin_y2 = hist.GetYaxis().FindBin(ymax)

    # Calculate the integral within the specified range
    integral = hist.Integral(bin_x1, bin_x2, bin_y1, bin_y2)

    # Print the result
    return integral

def plot_on_canvas(hist, dname, dlabel, outfolder, key, suf):
    if 'SigOverBkg' in suf:
        ROOT.gStyle.SetPaintTextFormat("1.3f")
    else:
        ROOT.gStyle.SetPaintTextFormat("1.1f")


    c2 = ROOT.TCanvas("c2", "c2", 1400, 1200)
    # c2.SetLogz()
    c2.SetRightMargin(0.15)
    c2.cd()
    if 'cumulative' in suf:
        hist.Draw("colz text")
    else:
        hist.Draw("colz")

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
    c2.Modified()
    c2.Update()

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
    CMS_lumi.CMS_lumi(c2, 4, 0)
    c2.Modified()
    c2.Update()

    c2.SaveAs(outfolder+"/"+key+'_'+dname+'_'+suf+".png")
    # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.pdf')
    # c2.SaveAs("Mass2d_dist_MS"+m_scalar+'.root')
    c2.Clear()

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
        'leadingMuonRelIso_subleadingMuonRelIso': {'hname':'h_leadingMuonRelIso_subleadingMuonRelIso','xlabel': "leading #mu PF Rel. Isolation", 'ylabel': "subleading #mu PF Rel. Isolation", 'xlow':0.,'xhigh':2., 'ylow':0.,'yhigh':2., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-2,'zhigh':1000},
        'leadingChHadRelIso_subleadingChHadRelIso': {'hname':'h_leadingChHadRelIso_subleadingChHadRelIso','xlabel': "leading h^{#pm} PF Rel. Isolation", 'ylabel': "subleading h^{#pm} PF Rel. Isolation", 'xlow':0.,'xhigh':2., 'ylow':0.,'yhigh':2., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-2,'zhigh':1000},
        'DiMuonVtxSignificance_DiChHadVtxSignificance': {'hname':'h_DiMuonVtxSignificance_DiChHadVtxSignificance','xlabel': "L_{xy}^{#mu^{+}#mu^{-}}/#Delta L_{xy}^{#mu^{+}#mu^{-}}", 'ylabel': "L_{xy}^{h^{+}h^{-}}/#Delta L_{xy}^{h^{+}h^{-}}", 'xlow':0.,'xhigh':300., 'ylow':0.,'yhigh':300., 'hrebinx':20, 'hrebiny':20,'zlow':9*1e-2,'zhigh':1000},
    }
    datasets_dict = {
        'HToSS_MH125_MS2_ctauS0':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 0mm}"},
        'HToSS_MH125_MS1p2_ctauS0':{'type':'signal','label':"#splitline{m_{a}=1.2 GeV}{c#tau = 0mm}"},
        'HToSS_MH125_MS2_ctauS1':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 1mm}"},
        'HToSS_MH125_MS2_ctauS10':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 10mm}"},
        'HToSS_MH125_MS2_ctauS100':{'type':'signal','label':"#splitline{m_{a}=2 GeV}{c#tau = 100mm}"},
        'Run2017':{'type':'blinded_data','label':'Run2017'}
    }
    dirname = {'prompt', 'displacedmumu', 'displacedhh', 'displaced'}
    for key in histo_dict:
        for dirn in dirname:
            if "Significance" in key:
                outdir = args.out
                dirn = ''
            else:
                outdir = args.out+'/'+'LinearYaxis'+'/'+args.dname.replace('HToSS_MH125_','').replace('ctauS','ctau')+'/'+dirn
                dirn = dirn+'/'
            
            ####################################### SIGNAL SAMPLE #############################################
            # Run the function
            dataset_name = args.dname
            fin = ROOT.TFile(args.fname+'/total/'+'output_'+dataset_name+'.root',"READ")
            hist=fin.Get(dirn+histo_dict[key]['hname']).Clone()
            if 'Run' not in dataset_name:
                hist.Scale(41474.989603894)
            xlow, ylow = 0, 0
            xhigh, yhigh = 1000000, 1000000
            integral = hist.Integral(0,hist.GetNbinsX()+1, 0, hist.GetNbinsY()+1)
            integral_whole = calculate_integral(hist,xlow,xhigh,ylow,yhigh)
            print("\n")
            print("Cross check of integral(measured, direct):",integral_whole,integral)
            integral_test = calculate_integral(hist,0,40,0,30)
            print("Region 1:",integral_test,integral,(100*integral_test/integral))


            hist.Rebin2D(histo_dict[key]['hrebinx'],histo_dict[key]['hrebiny'])
            hist.GetXaxis().SetTitle(histo_dict[key]['xlabel'])
            hist.GetYaxis().SetTitle(histo_dict[key]['ylabel'])
            # hist.GetXaxis().GetBinLowEdge(1); 
            hist.GetZaxis().SetTitle("Events / "+str(hist.GetXaxis().GetBinWidth(1)))
            hist.GetZaxis().SetTitleOffset(1.)
            hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
            hist.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
            # maxbin = hist.GetMaximumBin()
            # maxval = hist.GetBinContent(maxbin)
            # minbin = hist.GetMinimumBin()
            # minval = hist.GetBinContent(minbin)
            # hist.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
            # hist.GetZaxis().SetRangeUser(9*1e-2,300.)
            hist.GetZaxis().SetRangeUser(histo_dict[key]['zlow'],histo_dict[key]['zhigh'])

            hist.GetXaxis().SetLabelSize(0.04)
            hist.GetYaxis().SetLabelSize(0.04)
            hist.GetZaxis().SetLabelSize(0.04)
            hist.GetXaxis().SetTitleSize(0.05)
            hist.GetYaxis().SetTitleSize(0.05)
            hist.GetZaxis().SetTitleSize(0.05)

            plot_on_canvas(hist, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'original')

            hist.Rebin2D(4,4)
            cumulative_hist = hist.Clone()
            cumulative_hist.Reset()
            for ix in range(1, hist.GetNbinsX() + 1):
                for iy in range(1, hist.GetNbinsY() + 1):
                    content = hist.GetBinContent(ix, iy)
                    cumulative_content = cumulative_hist.GetBinContent(ix, iy - 1) + cumulative_hist.GetBinContent(ix - 1, iy) - cumulative_hist.GetBinContent(ix - 1, iy - 1) + content
                    cumulative_hist.SetBinContent(ix, iy, cumulative_content)
            # cumulative_hist.Rebin2D(4,4)
            maxbin = cumulative_hist.GetMaximumBin()
            maxval = cumulative_hist.GetBinContent(maxbin)
            minbin = cumulative_hist.GetMinimumBin()
            minval = cumulative_hist.GetBinContent(minbin)
            cumulative_hist.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
            cumulative_hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
            cumulative_hist.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
            cumulative_hist.GetZaxis().SetTitle("Events / "+str(cumulative_hist.GetXaxis().GetBinWidth(1)))

            # cumulative_hist.GetZaxis().SetRangeUser(100,100)

            plot_on_canvas(cumulative_hist, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'cumulative')
            cumulative_hist_percent=cumulative_hist.Clone()
            if integral>0:
                cumulative_hist_percent.Scale(1/integral*100)
            cumulative_hist_percent.GetZaxis().SetRangeUser(0.,100.)
            print('Integral:',integral)
            plot_on_canvas(cumulative_hist_percent, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'cumulative_percent')
            # quit()
            ####################################### DATA #############################################

            dataset_name = 'Run2017'
            fin = ROOT.TFile(args.fname+'/total/'+'output_'+dataset_name+'.root',"READ")
            hist=fin.Get(dirn+histo_dict[key]['hname']).Clone()
            if 'Run' not in dataset_name:
                hist.Scale(41474.989603894)
            xlow, ylow = 0, 0
            xhigh, yhigh = 1000000, 1000000
            integral = hist.Integral(0,hist.GetNbinsX()+1, 0, hist.GetNbinsY()+1)
            integral_whole = calculate_integral(hist,xlow,xhigh,ylow,yhigh)
            print("\n")
            print("Cross check of integral(measured, direct):",integral_whole,integral)

            hist.Rebin2D(histo_dict[key]['hrebinx'],histo_dict[key]['hrebiny'])
            hist.GetXaxis().SetTitle(histo_dict[key]['xlabel'])
            hist.GetYaxis().SetTitle(histo_dict[key]['ylabel'])
            # hist.GetXaxis().GetBinLowEdge(1); 
            hist.GetZaxis().SetTitle("Events / "+str(hist.GetXaxis().GetBinWidth(1)))
            hist.GetZaxis().SetTitleOffset(1.)
            hist.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
            hist.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
            # maxbin = hist.GetMaximumBin()
            # maxval = hist.GetBinContent(maxbin)
            # minbin = hist.GetMinimumBin()
            # minval = hist.GetBinContent(minbin)
            # hist.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
            # hist.GetZaxis().SetRangeUser(9*1e-2,300.)
            hist.GetZaxis().SetRangeUser(histo_dict[key]['zlow'],histo_dict[key]['zhigh'])

            hist.GetXaxis().SetLabelSize(0.04)
            hist.GetYaxis().SetLabelSize(0.04)
            hist.GetZaxis().SetLabelSize(0.04)
            hist.GetXaxis().SetTitleSize(0.05)
            hist.GetYaxis().SetTitleSize(0.05)
            hist.GetZaxis().SetTitleSize(0.05)

            plot_on_canvas(hist, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'original')

            hist.Rebin2D(4,4)
            cumulative_hist_data = hist.Clone()
            cumulative_hist_data.Reset()
            for ix in range(1, hist.GetNbinsX() + 1):
                for iy in range(1, hist.GetNbinsY() + 1):
                    content = hist.GetBinContent(ix, iy)
                    cumulative_content = cumulative_hist_data.GetBinContent(ix, iy - 1) + cumulative_hist_data.GetBinContent(ix - 1, iy) - cumulative_hist_data.GetBinContent(ix - 1, iy - 1) + content
                    cumulative_hist_data.SetBinContent(ix, iy, cumulative_content)
            # cumulative_hist_data.Rebin2D(4,4)
            maxbin = cumulative_hist_data.GetMaximumBin()
            maxval = cumulative_hist_data.GetBinContent(maxbin)
            minbin = cumulative_hist_data.GetMinimumBin()
            minval = cumulative_hist_data.GetBinContent(minbin)
            cumulative_hist_data.GetZaxis().SetRangeUser(0.1*minval,maxval*1.5)
            cumulative_hist_data.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
            cumulative_hist_data.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
            cumulative_hist_data.GetZaxis().SetTitle("Events / "+str(cumulative_hist_data.GetXaxis().GetBinWidth(1)))

            # cumulative_hist_data.GetZaxis().SetRangeUser(100,100)

            plot_on_canvas(cumulative_hist_data, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'cumulative')
            cumulative_hist_data_percent=cumulative_hist_data.Clone()
            if integral>0:
                cumulative_hist_data_percent.Scale(1/integral*100)
            cumulative_hist_data_percent.GetZaxis().SetRangeUser(0.,100.)
            plot_on_canvas(cumulative_hist_data_percent, dataset_name, datasets_dict[dataset_name]['label'], outdir, key,'cumulative_percent')

            cumulative_hist_SigOverBkg = cumulative_hist.Clone()
            cumulative_hist_SigOverBkg.Reset()
            for ix in range(1, cumulative_hist.GetNbinsX() + 1):
                for iy in range(1, cumulative_hist.GetNbinsY() + 1):
                    sig_content = cumulative_hist.GetBinContent(ix, iy)
                    data_content = cumulative_hist_data.GetBinContent(ix, iy)
                    content = sig_content/np.sqrt(data_content)
                    cumulative_hist_SigOverBkg.SetBinContent(ix, iy, content)
            maxbin = cumulative_hist_SigOverBkg.GetMaximumBin()
            maxval = cumulative_hist_SigOverBkg.GetBinContent(maxbin)
            minbin = cumulative_hist_SigOverBkg.GetMinimumBin()
            minval = cumulative_hist_SigOverBkg.GetBinContent(minbin)
            cumulative_hist_SigOverBkg.GetZaxis().SetRangeUser(minval,maxval)
            cumulative_hist_SigOverBkg.GetXaxis().SetRangeUser(histo_dict[key]['xlow'],histo_dict[key]['xhigh'])
            cumulative_hist_SigOverBkg.GetYaxis().SetRangeUser(histo_dict[key]['ylow'],histo_dict[key]['yhigh'])
            cumulative_hist_SigOverBkg.GetZaxis().SetTitle("S/#sqrt{B} / "+str(cumulative_hist_SigOverBkg.GetXaxis().GetBinWidth(1)))
            plot_on_canvas(cumulative_hist_SigOverBkg, args.dname, datasets_dict[args.dname]['label'], outdir, key,'cumulative_SigOverBkg')

        


if __name__ == '__main__':
    main()
