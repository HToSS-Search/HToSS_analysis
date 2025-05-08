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
import yaml

# Collect previous objects and disable circular garbage collector (reference counter still works)
# gc.collect()
# gc.disable()

ROOT.gROOT.SetBatch(True)
tdrstyle.setTDRStyle()
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetLineWidth(2)
ROOT.gStyle.SetLegendBorderSize(0)
ROOT.gSystem.SetIncludePath( "-I$ROOFITSYS/include/" )

colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kBlue,ROOT.kGreen+2,ROOT.kMagenta+1, ROOT.kOrange+1, ROOT.kTeal-1,ROOT.kRed-3, ROOT.kCyan+2]
markers = [20, 21, 22, 33, 47]
m_size = 2

def main():
    ROOT.TH1.AddDirectory(ROOT.kFALSE)


    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    # parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("--inputK", dest="inputK", help="Input directory nameK", type=str)
    parser.add_argument("--inputP", dest="inputP", help="Input directory nameP", type=str)
    parser.add_argument("-y","--year", dest="year", help="Year for processing", type=str)
    parser.add_argument("-s","--suf", dest="suf", help="Variation variable", type=str)
    # parser.add_argument("--analysis", dest="analysis", help="true for plotting after analysis, false by default (after skim)", action="store_true")

    # parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
    # add an option to plot just one plot accessible name in histo_dict; change savename accordingly
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()

    histo_dict = {
        'recoHiggsMass_AvgMass_mumu_hh_MH_BC': {'hname':'h_recoHiggsMass_AvgMass_mumu_hh_MH_BC','label':"m_{h^{+}h^{-}#mu^{+}#mu^{-}} (GeV)",'xlow':110,'xhigh':140,'hrebin':5},
    }
    datasets_dict = {
        'HToSS_MH125_MSX'+'_ctauS0':{'type':'nominal','label':"#splitline{m_{S}=X GeV,}{c#tau = 0.1mm}",'color':ROOT.kBlack-2,'integral':-1,'integral_sr':-1,'integral_cr':-1,'integral_lcr':-1},
        'HToSS_MH125_MSX'+'_ctauS0_'+args.suf+'up':{'type':'up','label':"#splitline{m_{S}=X GeV,}{c#tau = 1mm}",'color':ROOT.kBlue-7,'integral':-1,'integral_sr':-1,'integral_cr':-1,'integral_lcr':-1},
        'HToSS_MH125_MSX'+'_ctauS0_'+args.suf+'down':{'type':'down','label':"#splitline{m_{S}=X GeV,}{c#tau = 10mm}",'color':ROOT.kMagenta-3,'integral':-1,'integral_sr':-1,'integral_cr':-1,'integral_lcr':-1},
        # 'HToSS_MH125_MS'+args.mass+'_ctauS100_ctauS1':{'type':'signal','label':"#splitline{m_{S}="+args.mass.replace('p','.')+" GeV,}{c#tau =100mm#rightarrow 1mm}",'color':ROOT.kGray+2},
    }
    m_points=['0p4','0p6','0p8','0p9','1','1p1','1p2','1p4','1p6','1p8','2']
    # m_points=['2']
    # sig_mass = args.sig.split("MS")[1].split("_")[0].replace("p",".")
    # sig_ctau = args.sig.split("ctau")[1].split(".")[0].replace("p",".")

    # fout=open(args.out+'.txt','w')
    lumi_scale = {'UL2016_APV': 19500, 'UL2016': 16800,'UL2017':  41480,'UL2018': 59830 } #in pb-1
    lumi_factor = lumi_scale[args.year]
    fin2 = open('/user/sdansana/CMSSW_11_3_4/src/HToSS_analysis/CombinedLimits/bounds'+'.yaml','r')
    bound_pars = yaml.safe_load(fin2)
    fin2.close()

    with open(args.out, "r") as file:
        yield_ = yaml.safe_load(file)


    key='recoHiggsMass_AvgMass_mumu_hh_MH_BC'
    # yield_={'UL2016':{},'UL2016_APV':{},'UL2017':{},'UL2018':{}}
    ###### right now transfer factors have not been applied below #######
    m_points=['2']
    cs=0.04072082
    for m in m_points:
        print(key)
        fsig=[]
        ctau=[0.1]
        lo_bound,hi_bound=bound_pars['MS'+m][args.year]
        if float(m.replace('p','.')) < 1.1:
            indir=args.inputP
        else:
            indir=args.inputK
        yield_[args.year]['MS'+m]={'nominal':-1,'up':-1,'down':-1,'diff':-1,'diff_percent':-1}
        for dno,dname in enumerate(datasets_dict):
            dname=dname.replace('X',m)
            hprop = histo_dict[key]
            histname = hprop['hname']
            print(indir+"/"+ "output_" + dname + ".root")
            fsig=ROOT.TFile(indir+"/"+ "output_" + dname + ".root", "READ")
            h_1 = fsig.Get(histname)

            # h_1 = fsig.Get('prompt'+'/'+histname)
            # h_1.Add(fsig.Get('displaced'+'/'+histname))
            # h_1.Add(fsig.Get('displacedmumu'+'/'+histname))
            # h_1.Add(fsig.Get('displacedhh'+'/'+histname))
            # h_1.Rebin(hprop['hrebin'],)
            h_1.Scale(lumi_factor)
            # if 'muRmuF' in args.suf:
            #     h_1.Scale(1/cs)
            h_AtMass = h_1.ProjectionX("",h_1.GetYaxis().FindBin(lo_bound),h_1.GetYaxis().FindBin(hi_bound))
            yield_sr=h_AtMass.Integral(h_AtMass.GetXaxis().FindBin(122.6),h_AtMass.GetXaxis().FindBin(127.4))
            if 'up' in dname:
                yield_[args.year]['MS'+m]['up']=yield_sr
            elif 'down' in dname:
                yield_[args.year]['MS'+m]['down']=yield_sr
            else:
                yield_[args.year]['MS'+m]['nominal']=yield_sr
        yield_[args.year]['MS'+m]['diff']=abs(yield_[args.year]['MS'+m]['up']-yield_[args.year]['MS'+m]['down'])*0.5
        yield_[args.year]['MS'+m]['diff_percent']=round(yield_[args.year]['MS'+m]['diff']/yield_[args.year]['MS'+m]['nominal']*100,1)
    print(yield_)
    with open(args.out, "w") as file:
        yaml.safe_dump(yield_, file, default_flow_style=False)
        # print(datasets_dict)
        
        # c1.SaveAs(args.out+'/'+dirn+savename+'.root')
        # c1.Clear()
        # c1.Delete()

    #c.Print(args.out+savename+'.root')
if __name__ == '__main__':
    main()
