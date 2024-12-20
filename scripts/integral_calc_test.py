import ROOT
import os,argparse
import numpy as np
import ctypes
import yaml
# def calculate_integral(hist, xmin, xmax, ymin, ymax):

#     bin_x1 = hist.GetXaxis().FindBin(xmin)
#     bin_x2 = hist.GetXaxis().FindBin(xmax)
#     bin_y1 = hist.GetYaxis().FindBin(ymin)
#     bin_y2 = hist.GetYaxis().FindBin(ymax)

#     # Calculate the integral within the specified range
#     integral = hist.Integral(bin_x1, bin_x2, bin_y1, bin_y2)

#     # Print the result
#     return integral

def calculate_integral(hist):

    wt_sum, wt_sum_err=0,0
    for i in range(1, hist.GetNbinsX() + 1):
        xval,yval,xerr = hist.GetBinCenter(i),hist.GetBinContent(i),hist.GetBinContent(i)
        wt_sum=wt_sum+xval*yval
        wt_sum_err=wt_sum_err+(xval*yval)**2
    wt_sum_err=np.sqrt(wt_sum_err)
    # Print the result
    return wt_sum, wt_sum_err

def main():

    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-f", "--file",   dest="fname",   help="file location", type=str)
    parser.add_argument("--onlyN", dest="onlyN", help="true for only entries after analysis, false by default", action="store_true")

    # parser.add_argument("--hname",   dest="hname",   help="histogram name", type=str)
    # parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    # parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    # parser.add_argument("--mc",   dest="mc",   help="is MC?", action='store_true')
    lumi = 41480
    args = parser.parse_args()
    # Run the function
    fin = ROOT.TFile(args.fname,"READ")
    if args.onlyN:
        h_tmp=fin.Get("prompt/h_Weights_noSF").Clone()
        h_tmp.Add(fin.Get("displacedmumu/h_Weights_noSF").Clone())
        h_tmp.Add(fin.Get("displacedhh/h_Weights_noSF").Clone())
        h_tmp.Add(fin.Get("displaced/h_Weights_noSF").Clone())
        h_tmp.Scale(lumi)
        print("Entries after analysis:",h_tmp.GetEntries())
        quit()

    h_weights_bare=fin.Get("prompt/h_Weights_bare").Clone()
    h_weights_bare.Add(fin.Get("displacedmumu/h_Weights_bare").Clone())
    h_weights_bare.Add(fin.Get("displacedhh/h_Weights_bare").Clone())
    h_weights_bare.Add(fin.Get("displaced/h_Weights_bare").Clone())
    h_weights_lumi=fin.Get("prompt/h_Weights_noSF").Clone()
    h_weights_lumi.Add(fin.Get("displacedmumu/h_Weights_noSF").Clone())
    h_weights_lumi.Add(fin.Get("displacedhh/h_Weights_noSF").Clone())
    h_weights_lumi.Add(fin.Get("displaced/h_Weights_noSF").Clone())
    h_genL=fin.Get("prompt/h_genScalar1L").Clone()
    h_genL.Add(fin.Get("displacedmumu/h_genScalar1L").Clone())
    h_genL.Add(fin.Get("displacedhh/h_genScalar1L").Clone())
    h_genL.Add(fin.Get("displaced/h_genScalar1L").Clone())
    h_genL.Scale(lumi)
    canv = ROOT.TCanvas()
    canv.cd()
    h_weights_bare.Draw()
    canv.SaveAs('h_weights_bare.png')
    h_weights_bare.SaveAs('h_weights_bare.root')
    canv = ROOT.TCanvas()
    canv.cd()
    h_weights_lumi.Draw()
    canv.SaveAs('h_weights_lumi.png')
    h_weights_lumi.SaveAs('h_weights_lumi.root')
    canv = ROOT.TCanvas()
    canv.cd()
    h_genL.Draw()
    canv.SaveAs('h_genL.png')
    h_genL.SaveAs('h_genL.root')


    f_ltwts = open('signal_lifetime_reweighting_table'+'_'+'2017'+'.yaml','r')
    ltwt_pars = yaml.safe_load(f_ltwts)
    old_lt = float(args.fname.split('ctauS')[1].split('_')[0].replace('p','.').replace('.root',''))
    new_lt = float(args.fname.split('_')[-1].replace('ctauS','').replace('p','.').replace('.root',''))
    m_point = args.fname.split('/')[-1].split('_')[3]
    print(m_point,old_lt,new_lt)
    sum_wts_lt,sum_wts_lt_err = ltwt_pars[m_point][old_lt][new_lt]
    # h_weights_lumi.Add(fin.Get("displacedmumu/h_Weights_noSF").Clone())
    # h_weights_lumi.Add(fin.Get("displacedhh/h_Weights_noSF").Clone())
    # h_weights_lumi.Add(fin.Get("displaced/h_Weights_noSF").Clone())
    integ_bare, integ_err_bare = calculate_integral(h_weights_bare)
    integ, integ_err = calculate_integral(h_weights_lumi)
    tmp_err=ctypes.c_double(0)
    integ_L = h_genL.IntegralAndError(h_genL.FindBin(0),h_genL.FindBin(1000),tmp_err)
    integ_err_L=tmp_err.value
            
    # integ_L,integ_err_L = h_genL.Integral(),
    print("Bare:",h_weights_bare.GetEntries(),integ_bare, integ_err_bare,(integ_err_bare/integ_bare*100),"%")
    print("Normalized:",h_weights_lumi.GetEntries(),integ, integ_err,(integ_err/integ*100),"%")
    print("genL:",h_genL.GetEntries(),integ_L, integ_err_L,(integ_err_L/integ_L*100),"%")
    print("sum_wts_lt:",sum_wts_lt, sum_wts_lt_err,(sum_wts_lt_err/sum_wts_lt*100),"%")
    print("\n")
    


if __name__ == '__main__':
    main()
