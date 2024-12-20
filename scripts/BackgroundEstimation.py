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

def gInterpreter_transferfactor():
    getTF_code = '''
        #include "RooRealVar.h"
        #include "RooDataSet.h"
        #include "RooGaussian.h"
        #include "RooConstVar.h"
        #include "RooProdPdf.h"
        #include "RooAddPdf.h"
        #include "RooPolynomial.h"
        #include "TCanvas.h"
        #include "TAxis.h"
        #include "RooPlot.h"
        #include "RooExtendPdf.h"
        #include "RooTrace.h"
        #include <cmath>
        #include "tdrstyle.C"
        #include "CMS_lumi.C"
        using namespace std;
        using namespace RooFit;
        
        std::vector<Double_t> TransferFactor(TString outdir, TString category, TH1* hist, double xlow_CR1=110, double xhigh_CR1 =122.5, double xlow_SR=122.5, double xhigh_SR=127.5, double xlow_CR2=127.5, double xhigh_CR2=140) {

            setTDRStyle();

            RooTrace::active(1);
            hist->Rebin(5);
            Double_t total = hist->Integral();
            std::unique_ptr<RooRealVar> mass{new RooRealVar("mass", "m(hh#mu#mu) [GeV]", 125, 110, 140)};
            mass->setRange("loM", 110, 122.5 );
            mass->setRange("hiM", 127.5, 140 );
            mass->setRange("loSB", xlow_CR1, xhigh_CR1 );
            mass->setRange("hiSB", xlow_CR2, xhigh_CR2 );
            mass->setRange("peak", 122.5, 127.5 );
            mass->setRange("peakcore", 124.2, 125.8 );
            mass->setRange("full", 110, 140 );
            TString fit_range = "loM,hiM";
            std::unique_ptr<RooDataHist> data_all{new RooDataHist("data_all","data_all", RooArgList(*mass),Import(*hist))};
            //Define the background model slope parameter
            std::unique_ptr<RooRealVar> alpha_all{new RooRealVar("alpha_all", "alpha_all", -0.1, -0.2, 0.)};
            std::unique_ptr<RooExponential> model_bkg_all{ new RooExponential("model_bkg_all", "model_bkg_all", *mass, *alpha_all )};
            std::unique_ptr<RooRealVar> N_all{new RooRealVar("N_all", "N_all", 0, 5*total)};
            std::unique_ptr<RooExtendPdf> extmodel_bkg_all{new RooExtendPdf("extmodel_bkg_all", "extmodel_bkg_all", *model_bkg_all, *N_all, "full")};
            extmodel_bkg_all->fitTo(*data_all, Range(fit_range));

            std::unique_ptr<const RooAbsReal> gx_loSB{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range("loSB"))};
            std::unique_ptr<const RooAbsReal> gx_hiSB{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range("hiSB"))};
            std::unique_ptr<const RooAbsReal> gx_loM{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range("loM"))};
            std::unique_ptr<const RooAbsReal> gx_hiM{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range("hiM"))};
            std::unique_ptr<const RooAbsReal> gx_fitrange{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range(fit_range))};
            std::unique_ptr<const RooAbsReal> gx_sr{extmodel_bkg_all->createIntegral(RooArgSet(*mass),NormSet(RooArgSet(*mass)),Range("peak"))};
            Double_t extintegral_loSB = gx_loSB->getVal();
            Double_t extintegral_hiSB = gx_hiSB->getVal();
            Double_t extintegral_loM = gx_loM->getVal();
            Double_t extintegral_hiM = gx_hiM->getVal();
            Double_t extintegral_fitrange = gx_fitrange->getVal();
            Double_t extintegral_sr = gx_sr->getVal();
            cout<<Form("Integral(%.2f,%.2f):%.2f",xlow_CR1,xhigh_CR1,extintegral_loSB)<<endl;
            cout<<Form("Integral(%.2f,%.2f):%.2f",xlow_SR,xhigh_SR,extintegral_hiSB)<<endl;
            cout<<Form("Integral(%.2f,%.2f):%.2f",xlow_CR2,xhigh_CR2,extintegral_sr)<<endl;
            Double_t tf_loose = extintegral_sr/(extintegral_loM+extintegral_hiM);
            Double_t tf_CR = extintegral_sr/(extintegral_loSB+extintegral_hiSB);
            std::vector tf_list = {tf_loose,tf_CR};

            TCanvas *canvas = new TCanvas(hist->GetName(), "Exponential Fit Canvas", 800, 600);
            RooPlot *plot = (RooPlot*)mass->frame();
            data_all->plotOn( plot,Name("dist"), MarkerSize(0), MarkerColor(0), LineWidth(0), LineColor(0) );
            extmodel_bkg_all->plotOn( plot, NormRange(fit_range), Range("full"), LineColor(kRed), Name("extfit"), LineStyle(2));

            // add chi2 info
            TPaveText *chi2_text = new TPaveText(0.7,0.7,0.7,0.9,"NBNDC");
            chi2_text->SetTextAlign(11);
            chi2_text->AddText(Form("#chi^{2} fit = %.2f", plot->chiSquare("extfit","dist")));
            chi2_text->AddText(Form("#alpha = %.3f #pm %.3f",alpha_all->getVal(),alpha_all->getError()));
            chi2_text->AddText(Form("N = %.3f #pm %.3f",N_all->getVal(),N_all->getError()));
            chi2_text->AddText(Form("tf(loose CR) = %.3f",tf_loose));
            chi2_text->AddText(Form("tf(tight CR) = %.3f",tf_CR));
            chi2_text->SetTextSize(0.03);
            chi2_text->SetTextColor(2);
            chi2_text->SetShadowColor(0);
            chi2_text->SetFillColor(0);
            chi2_text->SetLineColor(0);
            chi2_text->SetName("chi2_text");
            plot->addObject(chi2_text);

            // Plot the histogram and the fit function
            hist->GetXaxis()->SetRangeUser(110,140);
            hist->GetXaxis()->SetTitle("m(hh#mu#mu)");
            hist->SetMarkerStyle(8);
            hist->SetMarkerSize(1);
            hist->Draw("pe");
            plot->Draw("same");
            
            // // Show the canvas
            canvas->Update();
            cmsText = "CMS";
            writeExtraText = true;
            extraText =  "      Work in Progress";
            if (outdir.Contains("2017")) {
                lumi_13TeV = "41.5 fb^{-1}";
            }
            else if (outdir.Contains("2018")) {
                lumi_13TeV = "59.8 fb^{-1}";
            }
            else if (outdir.Contains("APV")) {
                lumi_13TeV = "19.5 fb^{-1}";
            }
            else {
                lumi_13TeV = "16.8 fb^{-1}";
            }
            //cmsTextSize=1.;
            CMS_lumi( canvas, 4, 0 );

            canvas->SaveAs(outdir + "/" + category+"_"+hist->GetName()+".png");
            
            // RooArgSet::cleanup();
            // RooDataSet::cleanup();
            // RooTrace::dump();
            // Activate verbose mode
            // RooTrace::verbose(true);
            // Show object added to memory since marker
            // RooTrace::printObjectCounts();
            // ProcInfo_t pinfo; 
            // gSystem->GetProcInfo(&pinfo);
            // std::cout <<"  memory usage " << pinfo.fMemResident << "  " << pinfo.fMemVirtual << std::endl;
            return tf_list;
        }
        '''
    ROOT.gInterpreter.Declare(getTF_code)

def transfer_factor(category,hist, xlow_CR1=110, xhigh_CR1 =122.5, xlow_SR=122.5, xhigh_SR=127.5, xlow_CR2=127.5, xhigh_CR2=140):
    hist.Rebin(5)
    total = hist.Integral()
    mass = ROOT.RooRealVar("mass", "m(hh#mu#mu)", 125, 110, 140)
    mass.setRange("loM", 110, 122.5 )
    mass.setRange("hiM", 127.5, 140 )
    mass.setRange("loSB", 120, 122.5 )
    mass.setRange("hiSB", 127.5, 130 )
    mass.setRange("peak", 122.5, 127.5 )
    mass.setRange("peakcore", 124.2, 125.8 )
    mass.setRange("full", 110, 140 )
    fit_range = "loM,hiM"
    data_all = ROOT.RooDataHist("data_all","data_all", ROOT.RooArgList(mass),ROOT.RooFit.Import(hist) )
    # Define the background model slope parameter
    alpha_all = ROOT.RooRealVar("alpha_all", "alpha_all", -0.1, -0.2, 0.)
    # alpha_all = ROOT.RooRealVar("alpha_all", "alpha_all", -0.01, -1, -0.005)
    model_bkg_all = ROOT.RooExponential("model_bkg_all", "model_bkg_all", mass, alpha_all )
    N_all=ROOT.RooRealVar("N_all", "N_all", 0, 5*total);
    extmodel_bkg_all=ROOT.RooExtendPdf("extmodel_bkg_all", "extmodel_bkg_all", model_bkg_all, N_all, "full");
    extmodel_bkg_all.fitTo(data_all, ROOT.RooFit.Range(fit_range))

    canvas = ROOT.TCanvas(hist.GetName(), "Exponential Fit Canvas", 800, 600)
    plot = mass.frame()
    data_all.plotOn( plot,ROOT.RooFit.Name("dist"), ROOT.RooFit.MarkerColor(0), ROOT.RooFit.LineColor(0) )
    extmodel_bkg_all.plotOn( plot, ROOT.RooFit.NormRange(fit_range), ROOT.RooFit.Range("full"), ROOT.RooFit.LineColor(ROOT.kRed), ROOT.RooFit.Name("extfit"), ROOT.RooFit.LineStyle(2))

    # add chi2 info
    chi2_text = ROOT.TPaveText(0.2,0.7,0.2,0.9,"NBNDC")
    chi2_text.SetTextAlign(11)
    chi2_text.AddText("#chi^{2} fit = %s" %round(plot.chiSquare("extfit","dist"),2))
    chi2_text.AddText("#alpha "+"= {} #pm {}".format(round(alpha_all.getVal(),3), round(alpha_all.getError(),3)) )
    chi2_text.AddText("N "+"= {} #pm {}".format(round(N_all.getVal(),3), round(N_all.getError(),3)) )
    # chi2_text.AddText("Int_SB, N_SB "+"= {}, {}".format(round(n_bkg_sb,3), round(extintegral_sb*N_all.getVal(),3)) )

    # chi2_text.AddText("Mass assumption")
    # chi2_text.AddText(datasets_dict[key]['label'])
    chi2_text.SetTextSize(0.03)
    chi2_text.SetTextColor(2)
    chi2_text.SetShadowColor(0)
    chi2_text.SetFillColor(0)
    chi2_text.SetLineColor(0)
    chi2_text.SetName("chi2_text")
    plot.addObject(chi2_text)

    # # Access fit results
    # fit_results = hist.GetFunction("exponential_fit")
    # print("Manages to fit?")

    # # Plot the histogram and the fit function
    hist.GetXaxis().SetRangeUser(110,140)
    hist.GetXaxis().SetTitle("m(hh#mu#mu)")
    hist.SetMarkerStyle(8)
    hist.SetMarkerSize(2)

    # norm,norm_err = fit_results.GetParameter(0), fit_results.GetParError(1)
    # alpha, alpha_err = fit_results.GetParameter(1), fit_results.GetParError(1)
    # chisq = fit_results.GetChisquare()/fit_results.GetNDF()
    # # add chi2 info
    # chi2_text = ROOT.TPaveText(0.2,0.7,0.2,0.9,"NBNDC")
    # chi2_text.SetTextAlign(11)
    # chi2_text.AddText("#chi^{2} fit = %s" %round(chisq,2))
    # chi2_text.AddText("#alpha "+"= {} #pm {}".format(round(alpha,3), round(alpha_err,3)) )
    # chi2_text.AddText("N "+"= {} #pm {}".format(round(norm,3), round(norm_err,3)) )
    # # chi2_text.AddText("Int_SB, N_SB "+"= {}, {}".format(round(n_bkg_sb,3), round(extintegral_sb*N_all.getVal(),3)) )

    # # chi2_text.AddText("Mass assumption")
    # # chi2_text.AddText(datasets_dict[key]['label'])
    # chi2_text.SetTextSize(0.03)
    # chi2_text.SetTextColor(2)
    # chi2_text.SetShadowColor(0)
    # chi2_text.SetFillColor(0)
    # chi2_text.SetLineColor(0)
    # chi2_text.SetName("chi2_text")
    hist.Draw("pe")
    plot.Draw("same")
    # fit_results.Draw("same")
    chi2_text.Draw("same")

    # Print fit results
    # print("Fit Results:")
    # print("Normalization constant (parameter 0):", fit_results.GetParameter(0))
    # print("Decay constant (parameter 1):", fit_results.GetParameter(1))
    # print("Chi2/ndof :", fit_results.GetChisquare()/fit_results.GetNDF())

    set_mass = ROOT.RooArgSet(mass)
    ROOT.SetOwnership(set_mass, True) # this makes Python delete set_mass
    ROOT.SetOwnership(mass,True)
    ROOT.SetOwnership(data_all,True)
    ROOT.SetOwnership(alpha_all,True)
    ROOT.SetOwnership(model_bkg_all,True)
    ROOT.SetOwnership(N_all,True)
    ROOT.SetOwnership(plot,True)
    ROOT.SetOwnership(extmodel_bkg_all,True)
            
    # extintegral_loM = extmodel_bkg_all.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("loM")).getVal()
    # extintegral_hiM = extmodel_bkg_all.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("hiM")).getVal()
    # extintegral_sb = extmodel_bkg_all.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range(fit_range)).getVal()
    # extintegral_sr = extmodel_bkg_all.createIntegral(mass,ROOT.RooFit.NormSet(set_mass),ROOT.RooFit.Range("peak")).getVal()
    # # integral_CR1 = fit_results.Integral(xlow_CR1,xhigh_CR1)
    # # integral_SR = fit_results.Integral(xlow_SR,xhigh_SR)
    # # integral_CR2 = fit_results.Integral(xlow_CR2,xhigh_CR2)
    # # integral_loose = fit_results.Integral(xlowest,xhighest)
    # print("Integral(%s,%s):%s"%(xlow_CR1,xhigh_CR1,extintegral_loM))
    # print("Integral(%s,%s):%s"%(xlow_SR,xhigh_SR,extintegral_hiM))
    # print("Integral(%s,%s):%s"%(xlow_CR2,xhigh_CR2,extintegral_sr))
    # # print("Integral(%s,%s):%s"%(xlowest,xhighest,integral_loose))
    # tf = extintegral_sr/(extintegral_loM+extintegral_hiM)
    tf=1
    print("Transfer Factor = ",tf)

    
    # Show the canvas
    canvas.Update()
    canvas.SaveAs(category+"_"+hist.GetName()+".png")
    set_mass.Delete();mass.Delete();data_all.Delete();alpha_all.Delete();model_bkg_all.Delete();N_all.Delete();plot.Delete();extmodel_bkg_all.Delete();
    del set_mass;del mass;del data_all;del alpha_all;del model_bkg_all;del N_all;del plot;del extmodel_bkg_all;
    return tf

# if __name__ == '__main__':
    # main()