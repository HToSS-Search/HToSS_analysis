import ROOT
import os,argparse


def calculate_integral(hist, xmin, xmax, ymin, ymax):

    bin_x1 = hist.GetXaxis().FindBin(xmin)
    bin_x2 = hist.GetXaxis().FindBin(xmax)
    bin_y1 = hist.GetYaxis().FindBin(ymin)
    bin_y2 = hist.GetYaxis().FindBin(ymax)

    # Calculate the integral within the specified range
    integral = hist.Integral(bin_x1, bin_x2, bin_y1, bin_y2)

    # Print the result
    return integral

def main():

    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-f", "--file",   dest="fname",   help="file location", type=str)
    parser.add_argument("--hname",   dest="hname",   help="histogram name", type=str)
    parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    # parser.add_argument("--mc",   dest="mc",   help="is MC?", action='store_true')
    args = parser.parse_args()
    # Run the function
    fin = ROOT.TFile(args.fname,"READ")
    hist=fin.Get(args.hname).Clone()
    if 'Data' not in args.fname:
        hist.Scale(41480)
    xlow, ylow = 0, 0
    xhigh, yhigh = 1000000, 1000000
    integral = hist.Integral(0,hist.GetNbinsX()+1, 0, hist.GetNbinsY()+1)
    integral_whole = calculate_integral(hist,xlow,xhigh,ylow,yhigh)
    print("\n")
    print("Cross check of integral(measured, direct):",integral_whole,integral)
    integral_R1 = calculate_integral(hist,xlow,args.xmid,ylow,args.ymid)
    integral_R1_percent = integral_R1/integral_whole
    print(f"Integral R1: {integral_R1},{integral_R1_percent}")
    integral_R2_y = calculate_integral(hist,xlow,args.xmid,args.ymid,yhigh)
    integral_R2_y_percent = integral_R2_y/integral_whole
    print(f"Integral R2 along Y: {integral_R2_y},{integral_R2_y_percent}")
    integral_R2_x = calculate_integral(hist,args.xmid,xhigh,ylow,args.ymid)
    integral_R2_x_percent = integral_R2_x/integral_whole
    print(f"Integral R2 along X: {integral_R2_x},{integral_R2_x_percent}")
    integral_R3 = calculate_integral(hist,args.xmid,xhigh,args.ymid,yhigh)
    integral_R3_percent = integral_R3/integral_whole
    print(f"Integral R3: {integral_R3},{integral_R3_percent}")
    integral_R1bar=integral_R2_y+integral_R2_x+integral_R3
    integral_R1bar_percent = integral_R1bar/integral_whole
    print(f"Integral R2+R3:{integral_R1bar},{integral_R1bar_percent}")
    print("\n")
    


if __name__ == '__main__':
    main()
