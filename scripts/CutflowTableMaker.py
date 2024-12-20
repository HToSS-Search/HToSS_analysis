# Standard importts
import os,sys,socket,argparse
import shutil
# import ROOT
import math
from array import array
import numpy as np

# ROOT.gROOT.SetBatch(True)
ReducedBinning = False
correctforMCPU = False
def main():
    # ROOT.TH1.AddDirectory(ROOT.kFALSE)

    parser = argparse.ArgumentParser(description='Plot stacked histogram')
    parser.add_argument("-y", "--year",   dest="year",   help="data year", type=str)
    # parser.add_argument("-s", "--signal",   dest="sig",   help="HtoSS_MS2_ctauS0 or HtoSS_MS2_ctauS1 etc", type=str)
    parser.add_argument("-o","--output", dest="out", help="Output file name", type=str)
    parser.add_argument("-i","--input", dest="indir", help="Input directory name", type=str)
    parser.add_argument("--pion", dest="pion", help="true for pion mass assumption, false by default", action="store_true")
    # parser.add_argument("-m","--multiply", dest="mult", default=1,help="Multiply signal by a factor", type=int)
    # parser.add_argument("-c","--category", dest="category",help="prompt/displacedmumu/displacedhh/displaced", type=str)
    # parser.add_argument("--tf", dest="tf", default=1,help="Transfer factor depending on CR - Check BkgEst.py", type=float)
    # parser.add_argument("--yhigh", dest="yhigh", default=500,help="y-axis multiplicative factor for ymax", type=float)
    # parser.add_argument("--log", dest="log", help="true for plotting with logY, false by default", action="store_true")
    # parser.add_argument("--norm", dest="norm", help="true for plotting with normalisation, false by default", action="store_true")
    # parser.add_argument("--mass", dest="mass", help="mass of scalar", type=str)
    # # parser.add_argument("--noratio", dest="noratio", help="true for not plotting with ratio, false by default", action="store_true")
    # add an option to plot just one plot accessible name in histo_dict; change savename accordingly
    args = parser.parse_args()
    # create required parts
    cwd = os.getcwd()
    indir = args.indir
    print(indir)
    # Navigate one folder back
    # pardir = os.path.dirname(outdir)
    # print(pardir)
    # https://homepage.iihe.ac.be/~sdansana/HToSS_plots/Stacked/UL2018/KaonAssumption/IsoScalarHiggsCuts_DoubleMu_Category_90pPrompt_240429/LogYaxis/MS2_ctau10/displacedmumu/Cutflow.txt
    categories=['prompt','displacedmumu','displacedhh','displaced']
    if args.pion:
        m_points=['0p4','0p6','0p8','0p9','1']
    else:
        m_points=['1p1','1p2','1p4','1p6','1p8','2']
        #m_points=['1p2','1p6','2']
        #m_points=['2']


    ctau_points=['0','1','10','100']
    ftmp=open(os.path.join(indir,'LogYaxis/MS'+m_points[0]+'_ctau0/prompt/Cutflow.txt'))
    ll=ftmp.readlines()
    llextra=ftmp.read()
    nlines=len(llextra.split('\n'))
    nrows=len(ll)-2
    print('see here:',nrows,nlines)
    print(ll)
    sig_counts=np.zeros((len(categories),len(m_points),len(ctau_points),nrows),dtype=float)
    # data_counts=np.zeros((len(categories),nrows),dtype=float)
    # qcd_counts=np.zeros((len(categories),nrows),dtype=float)
    data_counts=[['def' for i in range(nrows)] for j in range(len(categories))]
    qcd_counts=[['def' for i in range(nrows)] for j in range(len(categories))]
    data_counts_f=[[-99 for i in range(nrows)] for j in range(len(categories))]
    qcd_counts_f=[[-99 for i in range(nrows)] for j in range(len(categories))]
    # print([t.split('&').strip() for t in ll])
    labels=[ll[t].split('&')[0].strip() for t in range(2,nrows+2)] #leave 0, 1 entries
    # data_counts=[ll[t].split('&')[2].strip() for t in  range(2,nrows+2)] #leave 0, 1 entries;
    # qcd_counts=[ll[t].split('&')[3].strip() for t in  range(2,nrows+2)] #leave 0, 1 entries;
    ftmp.close()

    for j,m in enumerate(m_points):

        tot_bkg_count=0;skim_bkg_count=0
        net_bkg_count=0
        tot_qcd_count=0;skim_qcd_count=0
        net_qcd_count=0
        for k,ctau in enumerate(ctau_points):
            net_sig_count=0
            tot_sig_count=0;skim_sig_count=0
            for i,category in enumerate(categories):
                # try:
                fname=os.path.join(indir,'LogYaxis/'+'MS'+m+'_ctau'+ctau+'/'+category+'/Cutflow.txt')
                # print(m,ctau,fname)
                f=open(fname,'r')
                fl=f.readlines()
                for c in range(2,nrows+2):
                    # if 'hline' in fl[c]:
                    #     continue
                    # if '&' not in fl[c]:
                    #     continue
                    # print(fl[c])
                    print(fl)
                    print(c,nrows,m,ctau)
                    sig_count=float(fl[c].split('&')[1].strip())
                    # print(i,j,k,c-2)
                    # print(sig_counts.shape)
                    sig_counts[i][j][k][c-2]=float(sig_count)
                    if c==2:
                        tot_sig_count=float(sig_count)
                    if 'higgs skim' in fl[c]:
                        skim_sig_count=float(sig_count)
                    if c==nrows+1:
                        net_sig_count=net_sig_count+float(sig_count)
                    if k==0:
                        # data_counts[i][c-2]=float(fl[c].split('&')[2].replace('(exp.)','').strip())
                        # qcd_counts[i][c-2]=float(fl[c].split('&')[3].replace('\\\\','').replace('(exp.)','').strip())
                        data_counts[i][c-2]=fl[c].split('&')[2].strip()
                        qcd_counts[i][c-2]=fl[c].split('&')[3].strip()

                        data_counts_f[i][c-2]=float(fl[c].split('&')[2].replace('(exp.)','').strip())
                        qcd_counts_f[i][c-2]=float(fl[c].split('&')[3].replace('(exp.)','').strip().split(' ')[0])
                        if c==2:
                            tot_bkg_count=float(fl[c].split('&')[2].strip())
                            tot_qcd_count=float(fl[c].split('&')[3].strip().split(' ')[0])
                        if 'higgs skim' in fl[c]:
                            skim_bkg_count=float(fl[c].split('&')[2].strip())
                            skim_qcd_count=float(fl[c].split('&')[3].strip().split(' ')[0])
                        if c==nrows+1:
                            net_bkg_count=net_bkg_count+float(fl[c].split('&')[2].strip().split('(')[0])
                            net_qcd_count=net_qcd_count+float(fl[c].split('&')[3].strip().split(' ')[0].split('(')[0])
            print("With respect to total")
            print(m,ctau,':',net_sig_count,',',tot_sig_count,',',np.round(net_sig_count/tot_sig_count*100,3))
            print('Data',':',net_bkg_count,',',tot_bkg_count,',',np.round(net_bkg_count/tot_bkg_count*100,3))
            print('QCD',':',net_qcd_count,',',tot_qcd_count,',',np.round(net_qcd_count/tot_qcd_count*100,3))
            print("With respect to skim")
            print(m,ctau,':',net_sig_count,',',skim_sig_count,',',np.round(net_sig_count/skim_sig_count*100,3))
            print('Data',':',net_bkg_count,',',skim_bkg_count,',',np.round(net_bkg_count/skim_bkg_count*100,3))
            print('QCD',':',net_qcd_count,',',skim_qcd_count,',',np.round(net_qcd_count/skim_qcd_count*100,3))
    
                # except:
                #     print("some issue")
                #     quit()
        # print(sig_counts[i])
        # print(data_counts)
        # print(qcd_counts)
    fout=open(args.out+'.txt','w')
    print(labels)
    # skim cutflow
    for j,m in enumerate(m_points):
        fout.write('MS'+m+'\n')
        # for i,category in enumerate(categories):
        fout.write(category+'\n')
        fout.write('\\begin{table}[h]'+'\n')
        fout.write('\\centering'+'\n')
        fout.write('\\begin{tabular}{|c|cccc|c|c|}'+'\n')
        fout.write('\\hline'+'\n')
        fout.write('\\multirow{2}{*}{Cut} & \\multicolumn{4}{|c|}{$m_{S}='+m.replace('p','.')+'$ GeV} &  \\multirow{2}{*}{Data} & \\multirow{2}{*}{QCD} \\\\ \\cline{2-5}'+'\n')
        fout.write('& $c\\tau=0.1$ & $c\\tau=1$ & $c\\tau=10$ & $c\\tau=100$ mm &  &  \\\\ \\hline'+'\n')
        for k in range(5):
            # print(k)
            fout.write(labels[k]+' & ')
            for ct in range(len(ctau_points)):
                fout.write(str(sig_counts[i][j][ct][k])+' & ')
            fout.write(data_counts[i][k]+' & ')
            fout.write(qcd_counts[i][k])
            # if 'reco. higgs skim' in labels[k] or 'prompt' in labels[k] or 'displaced' in labels[k] or 'h isolation' in labels[k]:
            #     fout.write('\\hline')
            fout.write('\n')
        fout.write('\\end{tabular}'+'\n')
        fout.write('\\caption{Skim cutflow for '+args.year+' MS'+m+'}'+'\n')
        fout.write('\\label{tab:skim_ms'+m+'}'+'\n')
        fout.write('\\end{table}'+'\n')
    # selection cutflow
    for j,m in enumerate(m_points):
        fout.write('MS'+m+'\n')
        fout.write(category+'\n')
        fout.write('\\begin{table}[h]'+'\n')
        fout.write('\\centering'+'\n')
        fout.write('\\small'+'\n')
        fout.write('\\begin{tabular}{|c|cccc|c|c|}'+'\n')
        fout.write('\\hline'+'\n')
        fout.write('\\multirow{2}{*}{Cut} & \\multicolumn{4}{|c|}{$m_{S}='+m.replace('p','.')+'$ GeV} &  \\multirow{2}{*}{Data} & \\multirow{2}{*}{QCD} \\\\ \\cline{2-5}'+'\n')
        fout.write('& $c\\tau=0.1$ & $c\\tau=1$ & $c\\tau=10$ & $c\\tau=100$ mm &  &  \\\\ \\hline'+'\n')
        for k in range(4,nrows-5):
            # print(k)
            fout.write(labels[k]+' & ')
            for ct in range(len(ctau_points)):
                fout.write(str(sig_counts[i][j][ct][k])+' & ')
            fout.write(data_counts[i][k]+' & ')
            fout.write(qcd_counts[i][k])
            # fout.write('\\\\')
            # if k==nrows-6:
            fout.write('\n')
        fout.write(labels[k]+'(SF) & ')
        aft_sf=[None,None,None,None]
        for ct in range(len(ctau_points)):
            aft_sf[ct] = 0
            for i,category in enumerate(categories):
                aft_sf[ct]=aft_sf[ct]+sig_counts[i][j][ct][nrows-5]
            fout.write(str(round(aft_sf[ct],2))+' & ')
        fout.write(data_counts[i][nrows-6]+' & ')
        qcd_aft_sf=float(qcd_counts[0][nrows-5].replace('(exp.)','').replace('\\\\','').strip())+float(qcd_counts[1][nrows-5].replace('(exp.)','').replace('\\\\','').strip())+float(qcd_counts[2][nrows-5].replace('(exp.)','').replace('\\\\','').strip())+float(qcd_counts[3][nrows-5].replace('(exp.)','').replace('\\\\','').strip())
        fout.write(str(round(qcd_aft_sf,2))+'(exp.) \\\\')
        fout.write('\\hline\\hline')
        fout.write('\n')

        for i,category in enumerate(categories):
            for k in range(nrows-5,nrows):
                fout.write(labels[k].replace('prompt',category).replace('mumu',' $\\mu\\mu$').replace('hh',' $hh$')+' & ')
                for ct in range(len(ctau_points)):
                    fout.write(str(sig_counts[i][j][ct][k])+' & ')
                fout.write(data_counts[i][k]+' & ')
                fout.write(qcd_counts[i][k])
                if k==nrows-1:
                    fout.write('\\hline')
                fout.write('\n')
            fout.write('category efficiency & ')
            for ct in range(len(ctau_points)):
                # fout.write(str(round(sig_counts[i][j][ct][nrows-1]/sig_counts[i][j][ct][nrows-5]*100,1))+'\% & ')
                fout.write(str(round(sig_counts[i][j][ct][nrows-1]/aft_sf[ct]*100,1))+'\% & ')
            fout.write('/'+' & '+'/')
            fout.write('\\\\')
            fout.write('\\hline\\hline')
            fout.write('\n')
        # fout.write('\n')
        fout.write('\\end{tabular}'+'\n')
        fout.write('\\caption{Cutflow for '+args.year+' MS'+m+' '+category+'}'+'\n')
        fout.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout.write('\\end{table}'+'\n')
    
    fout.write('Mass window'+'\n')
    for i,category in enumerate(categories):
        fout.write(category+'\n')
        fout.write('\\begin{table}[h]'+'\n')
        fout.write('\\centering'+'\n')
        fout.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout_p.write('\\hline'+'\n')
        fout.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout_p.write()
        for j,m in enumerate(m_points):
            fout.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-3
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                        sig_p=0
                else:
                    sig_p = sig_counts[i][j][ct][k]
                # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)
                fout.write(str(np.round(sig_p,1)))
                if ct<len(ctau_points)-1:
                    fout.write(' & ')
            fout.write('\\\\ \\hline')
            fout.write('\n')
        fout.write('\\end{tabular}'+'\n')
        fout.write('\\caption{Scalar mass consistency '+category+'}'+'\n')
        fout.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout.write('\\end{table}'+'\n')
    
    # isolation cutflow
    fout.write('PFIsolation muon'+'\n')
    for i,category in enumerate(categories):
        fout.write(category+'\n')
        fout.write('\\begin{table}[h]'+'\n')
        fout.write('\\centering'+'\n')
        fout.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout.write('\\hline'+'\n')
        fout.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout.write()
        for j,m in enumerate(m_points):
            fout.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-2
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                        sig_p=0
                else:
                    sig_p = sig_counts[i][j][ct][k]
                # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)
                fout.write(str(np.round(sig_p,1)))
                if ct<len(ctau_points)-1:
                    fout.write(' & ')
            fout.write('\\\\ \\hline')
            fout.write('\n')
        fout.write('\\end{tabular}'+'\n')
        fout.write('\\caption{PFIsolation muon'+category+'}'+'\n')
        fout.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout.write('\\end{table}'+'\n')
    # fout.close()

    fout.write('PFIsolation hadron'+'\n')
    for i,category in enumerate(categories):
        fout.write(category+'\n')
        fout.write('\\begin{table}[h]'+'\n')
        fout.write('\\centering'+'\n')
        fout.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout.write('\\hline'+'\n')
        fout.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout.write()
        for j,m in enumerate(m_points):
            fout.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-1
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
                    # print(labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][4])
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                    sig_p=0
                else:
                    # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                    sig_p = sig_counts[i][j][ct][k]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)

                fout.write(str(np.round(sig_p,1)))
                if ct<len(ctau_points)-1:
                    fout.write(' & ')
            fout.write('\\\\ \\hline')
            fout.write('\n')
        fout.write('\\end{tabular}'+'\n')
        fout.write('\\caption{PFIsolation hadron '+category+'}'+'\n')
        fout.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout.write('\\end{table}'+'\n')
    fout.close()    
    # fout.close()
    

    fout_p=open(args.out+'_percent'+'.txt','w')
    print(labels)
    # skim cutflow
    for j,m in enumerate(m_points):
        fout_p.write('MS'+m+'\n')
        # for i,category in enumerate(categories):
        fout_p.write(category+'\n')
        fout_p.write('\\begin{table}[tbh!]'+'\n')
        fout_p.write('\\centering'+'\n')
        fout_p.write('\\small'+'\n')
        fout_p.write('\\begin{tabular}{|c|cccc|c|c|}'+'\n')
        fout_p.write('\\hline'+'\n')
        fout_p.write('\\multirow{2}{*}{Cut} & \\multicolumn{4}{|c|}{$m_{S}='+m.replace('p','.')+'$ GeV} &  \\multirow{2}{*}{Data} & \\multirow{2}{*}{QCD} \\\\ \\cline{2-5}'+'\n')
        fout_p.write('& $c\\tau=0.1$ & $c\\tau=1$ & $c\\tau=10$ & $c\\tau=100$ mm &  &  \\\\ \\hline'+'\n')
        for k in range(5):
            # print(k)
            fout_p.write(labels[k]+' & ')
            for ct in range(len(ctau_points)):
                sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][0]
                fout_p.write(str(np.round(sig_p*100,1))+'\%'+' & ')
            # fout_p.write(data_counts[i][k]+' & ')
            # fout_p.write(qcd_counts[i][k])
            data_p = data_counts_f[i][k]/data_counts_f[i][0]
            qcd_p = qcd_counts_f[i][k]/qcd_counts_f[i][0]
            fout_p.write(str(np.round(data_p*100,1))+'\%'+' & ')
            fout_p.write(str(np.round(qcd_p*100,1))+'\%'+' \\\\')
            # if 'reco. higgs skim' in labels[k] or 'prompt' in labels[k] or 'displaced' in labels[k] or 'h isolation' in labels[k]:
            #     fout_p.write('\\hline')
            fout_p.write('\n')
        fout_p.write('\\end{tabular}'+'\n')
        fout_p.write('\\caption{Skim cutflow for '+args.year+' MS'+m+'}'+'\n')
        fout_p.write('\\label{tab:skim_ms'+m+'}'+'\n')
        fout_p.write('\\end{table}'+'\n')
    # selection cutflow
    for j,m in enumerate(m_points):
        fout_p.write('MS'+m+'\n')
        for i,category in enumerate(categories):
            fout_p.write(category+'\n')
            fout_p.write('\\begin{table}[h]'+'\n')
            fout_p.write('\\centering'+'\n')
            fout_p.write('\\begin{tabular}{|c|cccc|c|c|}'+'\n')
            fout_p.write('\\hline'+'\n')
            fout_p.write('\\multirow{2}{*}{Cut} & \\multicolumn{4}{|c|}{$m_{S}='+m.replace('p','.')+'$ GeV} &  \\multirow{2}{*}{Data} & \\multirow{2}{*}{QCD} \\\\ \\cline{2-5}'+'\n')
            fout_p.write('& $c\\tau=0.1$ & $c\\tau=1$ & $c\\tau=10$ & $c\\tau=100$ mm &  &  \\\\ \\hline'+'\n')
            for k in range(4,nrows):
                # print(k)
                fout_p.write(labels[k]+' & ')
                for ct in range(len(ctau_points)):
                    # if ct == 0 and m=='2':
                        # print(labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][4])
                    sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][4]
                    fout_p.write(str(np.round(sig_p*100,1))+'\%'+' & ')

                # fout_p.write(data_counts[i][k]+' & ')
                # fout_p.write(qcd_counts[i][k])

                data_p = data_counts_f[i][k]/data_counts_f[i][4]
                qcd_p = qcd_counts_f[i][k]/qcd_counts_f[i][4]
                fout_p.write(str(np.round(data_p*100,1))+'\%'+' & ')
                fout_p.write(str(np.round(qcd_p*100,1))+'\%'+' \\\\')
                # if 'reco. higgs skim' in labels[k]:
                if 'reco. higgs skim' in labels[k] or 'prompt' in labels[k] or 'displaced' in labels[k] or 'h isolation' in labels[k]:
                    fout_p.write('\\hline')
                fout_p.write('\n')
            fout_p.write('\\end{tabular}'+'\n')
            fout_p.write('\\caption{Cutflow for '+args.year+' MS'+m+' '+category+'}'+'\n')
            fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
            fout_p.write('\\end{table}'+'\n')
    # fout_p.close()

    # selection cutflow
    for j,m in enumerate(m_points):
        fout_p.write('MS'+m+'\n')
        for i,category in enumerate(categories):
            fout_p.write(category+'\n')
            fout_p.write('\\begin{table}[h]'+'\n')
            fout_p.write('\\centering'+'\n')
            fout_p.write('\\begin{tabular}{|c|cccc|c|c|}'+'\n')
            fout_p.write('\\hline'+'\n')
            fout_p.write('\\multirow{2}{*}{Cut} & \\multicolumn{4}{|c|}{$m_{S}='+m.replace('p','.')+'$ GeV} &  \\multirow{2}{*}{Data} & \\multirow{2}{*}{QCD} \\\\ \\cline{2-5}'+'\n')
            fout_p.write('& $c\\tau=0.1$ & $c\\tau=1$ & $c\\tau=10$ & $c\\tau=100$ mm &  &  \\\\ \\hline'+'\n')
            for k in range(4,nrows):
                # print(k)
                fout_p.write(labels[k]+' & ')
                for ct in range(len(ctau_points)):
                    # if ct == 0 and m=='2':
                        # print(labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][4])
                    sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][4]
                    fout_p.write(str(np.round(sig_p*100,1))+'\%'+' & ')

                # fout_p.write(data_counts[i][k]+' & ')
                # fout_p.write(qcd_counts[i][k])

                data_p = data_counts_f[i][k]/data_counts_f[i][4]
                qcd_p = qcd_counts_f[i][k]/qcd_counts_f[i][4]
                fout_p.write(str(np.round(data_p*100,1))+'\%'+' & ')
                fout_p.write(str(np.round(qcd_p*100,1))+'\%'+' \\\\')
                # if 'reco. higgs skim' in labels[k]:
                if 'reco. higgs skim' in labels[k] or 'prompt' in labels[k] or 'displaced' in labels[k] or 'h isolation' in labels[k]:
                    fout_p.write('\\hline')
                fout_p.write('\n')
            fout_p.write('\\end{tabular}'+'\n')
            fout_p.write('\\caption{Cutflow for '+args.year+' MS'+m+' '+category+'}'+'\n')
            fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
            fout_p.write('\\end{table}'+'\n')
    # fout_p.close()

    # scalar mass window cutflow
    fout_p.write('Mass window'+'\n')
    for i,category in enumerate(categories):
        fout_p.write(category+'\n')
        fout_p.write('\\begin{table}[h]'+'\n')
        fout_p.write('\\centering'+'\n')
        fout_p.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout_p.write('\\hline'+'\n')
        fout_p.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout_p.write()
        for j,m in enumerate(m_points):
            fout_p.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-3
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                        sig_p=0
                else:
                    sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)
                fout_p.write(str(np.round(sig_p*100,1))+'\%')
                if ct<len(ctau_points)-1:
                    fout_p.write(' & ')
            fout_p.write('\\\\ \\hline')
            fout_p.write('\n')
        fout_p.write('\\end{tabular}'+'\n')
        fout_p.write('\\caption{Scalar mass consistency '+category+'}'+'\n')
        fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout_p.write('\\end{table}'+'\n')

    fout_p.write('Mass window combined in all categories'+'\n')
    # for i,category in enumerate(categories):
    fout_p.write(category+'\n')
    fout_p.write('\\begin{table}[h]'+'\n')
    fout_p.write('\\centering'+'\n')
    fout_p.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
    # fout_p.write('\\hline'+'\n')
    fout_p.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout_p.write()
    for j,m in enumerate(m_points):
        fout_p.write(m.replace('p','.')+' GeV'+' & ')
        # for k in range(nrows-3,nrows):
        k=nrows-3
        for ct in range(len(ctau_points)):
            # if ct == 0 and m=='2':
        # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
            if sig_counts[i][j][ct][k-1]==0:
                    sig_p=0
            else:
                sig_p = (sig_counts[0][j][ct][k]+sig_counts[1][j][ct][k]+sig_counts[2][j][ct][k]+sig_counts[3][j][ct][k])/(sig_counts[0][j][ct][k-1]+sig_counts[1][j][ct][k-1]+sig_counts[2][j][ct][k-1]+sig_counts[3][j][ct][k-1])
            # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
            print(m,ct,labels[k],sig_counts[0][j][ct][k],sig_counts[0][j][ct][k-1],sig_p)
            fout_p.write(str(np.round(sig_p*100,1))+'\%')
            if ct<len(ctau_points)-1:
                fout_p.write(' & ')
        fout_p.write('\\\\ \\hline')
        fout_p.write('\n')
    fout_p.write('\\end{tabular}'+'\n')
    fout_p.write('\\caption{Scalar mass consistency combined}'+'\n')
    fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
    fout_p.write('\\end{table}'+'\n')

    # isolation cutflow
    fout_p.write('PFIsolation muon'+'\n')
    for i,category in enumerate(categories):
        fout_p.write(category+'\n')
        fout_p.write('\\begin{table}[h]'+'\n')
        fout_p.write('\\centering'+'\n')
        fout_p.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout_p.write('\\hline'+'\n')
        fout_p.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout_p.write()
        for j,m in enumerate(m_points):
            fout_p.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-2
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                        sig_p=0
                else:
                    sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                # sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)
                fout_p.write(str(np.round(sig_p*100,1))+'\%')
                if ct<len(ctau_points)-1:
                    fout_p.write(' & ')
            fout_p.write('\\\\ \\hline')
            fout_p.write('\n')
        fout_p.write('\\end{tabular}'+'\n')
        fout_p.write('\\caption{PFIsolation muon'+category+'}'+'\n')
        fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout_p.write('\\end{table}'+'\n')
    # fout_p.close()

    fout_p.write('PFIsolation hadron'+'\n')
    for i,category in enumerate(categories):
        fout_p.write(category+'\n')
        fout_p.write('\\begin{table}[h]'+'\n')
        fout_p.write('\\centering'+'\n')
        fout_p.write('\\begin{tabular}{c|c|c|c|c}'+'\n')
        # fout_p.write('\\hline'+'\n')
        fout_p.write('$m_{S}$/c$\\tau$  & 0.1mm & 1mm & 10mm & 100mm \\\\ \\hline'+'\n')
        # fout_p.write()
        for j,m in enumerate(m_points):
            fout_p.write(m.replace('p','.')+' GeV'+' & ')
            # for k in range(nrows-3,nrows):
            k=nrows-1
            for ct in range(len(ctau_points)):
                # if ct == 0 and m=='2':
                    # print(labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][4])
            # sig_p = sig_counts[i][j][0][k]/sig_counts[i][j][0][4]
                if sig_counts[i][j][ct][k-1]==0:
                    sig_p=0
                else:
                    sig_p = sig_counts[i][j][ct][k]/sig_counts[i][j][ct][k-1]
                print(m,ct,labels[k],sig_counts[i][j][ct][k],sig_counts[i][j][ct][k-1],sig_p)

                fout_p.write(str(np.round(sig_p*100,1))+'\%')
                if ct<len(ctau_points)-1:
                    fout_p.write(' & ')

            fout_p.write('\\\\ \\hline')
            fout_p.write('\n')
        fout_p.write('\\end{tabular}'+'\n')
        fout_p.write('\\caption{PFIsolation hadron '+category+'}'+'\n')
        fout_p.write('\\label{tab:cutflow_ms'+m+'}'+'\n')
        fout_p.write('\\end{table}'+'\n')
    fout_p.close()
    # fout.
if __name__ == '__main__':
    main()
