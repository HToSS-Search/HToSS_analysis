############## USE PYTHON NOT PYTHON3 -> SOME RANDOM BUG, CODE GETS STUCK IN PYTHON3 -> Just do cmsenv before running (DO NOT SOURCE SETUP) ###################

import ROOT
import os,argparse

# Define the exponential function
def exponential_fit(x, par):
    return par[0] * ROOT.TMath.Exp(-par[1] * x[0])

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
    # parser.add_argument("--xmid",   dest="xmid",   help="dividing point x-axis", type=float)
    # parser.add_argument("--ymid",   dest="ymid",   help="dividing point y-axis", type=float)
    args = parser.parse_args()

    fin = ROOT.TFile(args.fname,"READ")
    hist=fin.Get(args.hname).Clone()
    if 'Run' not in args.fname:
        hist.Scale(41474.989603894)
    hist.Rebin(5)
    hist.GetXaxis().SetRangeUser(110,140)
    # xlow, ylow = 0, 0
    # xhigh, yhigh = 1000000, 1000000
    integral = hist.Integral(-1,hist.GetNbinsX()+1)
    print("\n")
    print("Total integral(direct):",integral)
    # xlow_CR1, xhigh_CR1 = 120,122.5
    # xlow_SR, xhigh_SR = 122.5,127.5
    # xlow_CR2, xhigh_CR2 = 127.5, 130
    # xlowest, xhighest = 120, 130

    xlow_CR1, xhigh_CR1 = 110,122.5
    xlow_SR, xhigh_SR = 122.5,127.5
    xlow_CR2, xhigh_CR2 = 127.5, 140
    xlowest, xhighest = 110, 140

    if 'Run' not in args.fname:
        integral_CR1 = hist.Integral(hist.FindBin(xlow_CR1),hist.FindBin(xhigh_CR1))
        integral_SR = hist.Integral(hist.FindBin(xlow_SR),hist.FindBin(xhigh_SR))
        integral_CR2 = hist.Integral(hist.FindBin(xlow_CR2),hist.FindBin(xhigh_CR2))
        integral_loose = hist.Integral(hist.FindBin(xlowest),hist.FindBin(xhighest))
        print("Integral({%s},{%s}):{%s}"%(xlow_CR1, xhigh_CR1,integral_CR1))
        print("Integral({%s},{%s}):{%s}"%(xlow_SR, xhigh_SR,integral_SR))
        print("Integral({%s},{%s}):{%s}"%(xlow_CR2, xhigh_CR2,integral_CR2))
        print("Integral({%s},{%s}):{%s}"%(xlowest, xhighest,integral_loose))
    else:
        # Create a TF1 function for fitting
        # fit_function = ROOT.TF1("exponential_fit", "expo(0)", xlow_CR1, xhigh_CR2)
        # expo_fit = exponential_fit()
        fit_function = ROOT.TF1("exponential_fit", "expo", 110, 140)
        fit_function.SetParameters(3, -0.1)  # Initial parameters for the fit
        print("Fit initialized")

        # Fit the histogram with the exponential function
        hist.Fit(fit_function, "R", "", xlow_CR1, xhigh_CR2)
        # hist.Fit(fit_function, "R", "", 115, 135)

        print(hist.Integral())

        # Access fit results
        fit_results = hist.GetFunction("exponential_fit")
        print("Manages to fit?")

        # Plot the histogram and the fit function
        canvas = ROOT.TCanvas(args.hname, "Exponential Fit Canvas", 800, 600)
        hist.Draw("pe")
        fit_results.Draw("same")

        # Print fit results
        print("Fit Results:")
        print("Normalization constant (parameter 0):", fit_results.GetParameter(0))
        print("Decay constant (parameter 1):", fit_results.GetParameter(1))
        print("Chi2/ndof :", fit_results.GetChisquare()/fit_results.GetNDF())

        integral_CR1 = fit_results.Integral(xlow_CR1,xhigh_CR1)
        integral_SR = fit_results.Integral(xlow_SR,xhigh_SR)
        integral_CR2 = fit_results.Integral(xlow_CR2,xhigh_CR2)
        integral_loose = fit_results.Integral(xlowest,xhighest)
        print("Integral(%s,%s):%s"%(xlow_CR1,xhigh_CR1,integral_CR1))
        print("Integral(%s,%s):%s"%(xlow_SR,xhigh_SR,integral_SR))
        print("Integral(%s,%s):%s"%(xlow_CR2,xhigh_CR2,integral_CR2))
        print("Integral(%s,%s):%s"%(xlowest,xhighest,integral_loose))
        transfer_factor = integral_SR/(integral_CR1+integral_CR2)
        print("Transfer Factor = ",transfer_factor)

        
        # Show the canvas
        canvas.Update()
        canvas.SaveAs(args.hname.split('/')[1]+".png")

if __name__ == '__main__':
    main()