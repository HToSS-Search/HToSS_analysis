import numpy as np
import os,argparse
import re

parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-d", "--dataset", dest="dataset",   help="Enter dataset to process", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=int)
parser.add_argument("-n","--njobs", dest="njobs", help="No. of files to process in one job", type=int)
parser.add_argument("-o","--output", dest="outdir", help="Destination directory", type=str)
args = parser.parse_args()

pathnamesdict = {
	"HtoSS_MS2_ctau0": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ggH/NLO_ggH_HToSS_SmuonHadronFiltered_M125_MS2_ctauS0/",
	# QCD
	"QCD_Pt-15to20_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-15to20_MuEnrichedPt5/",
	"QCD_Pt-20to30_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-20to30_MuEnrichedPt5/",
	"QCD_Pt-30to50_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-30to50_MuEnrichedPt5/",
	"QCD_Pt-50to80_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-50to80_MuEnrichedPt5/",
	"QCD_Pt-80to120_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-80to120_MuEnrichedPt5/",
	"QCD_Pt-120to170_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-120to170_MuEnrichedPt5/",
	"QCD_Pt-170to300_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-170to300_MuEnrichedPt5/",
	"QCD_Pt-300to470_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-300to470_MuEnrichedPt5/",
	"QCD_Pt-470to600_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-470to600_MuEnrichedPt5/",
	"QCD_Pt-600to800_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-600to800_MuEnrichedPt5/",
	"QCD_Pt-800to1000_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-800to1000_MuEnrichedPt5/",
	"QCD_Pt-1000toInf_MuEnrichedPt5": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/QCD/QCD_Pt-1000toInf_MuEnrichedPt5/",
	# W+jets
	"wPlusJets": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/Wjets/Wjets/"
}
pathweightsdict = {
	"HtoSS_MS2_ctau0": "plots/weightDistributions/output_HtoSS_MS2_ctau0.root",
	# QCD
	"QCD_Pt-15to20_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-15to20_MuEnrichedPt5.root",
	"QCD_Pt-20to30_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-20to30_MuEnrichedPt5.root",
	"QCD_Pt-30to50_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-30to50_MuEnrichedPt5.root",
	"QCD_Pt-50to80_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-50to80_MuEnrichedPt5.root",
	"QCD_Pt-80to120_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-80to120_MuEnrichedPt5.root",
	"QCD_Pt-120to170_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-120to170_MuEnrichedPt5.root",
	"QCD_Pt-170to300_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-170to300_MuEnrichedPt5.root",
	"QCD_Pt-300to470_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-300to470_MuEnrichedPt5.root",
	"QCD_Pt-470to600_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-470to600_MuEnrichedPt5.root",
	"QCD_Pt-600to800_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-600to800_MuEnrichedPt5.root",
	"QCD_Pt-800to1000_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-800to1000_MuEnrichedPt5.root",
	"QCD_Pt-1000toInf_MuEnrichedPt5": "plots/weightDistributions/QCD/output_QCD_Pt-1000toInf_MuEnrichedPt5.root",
	# W+jets
	"wPlusJets": "plots/weightDistributions/wPlusJets/output_wPlusJets.root"
}

def maxfilenumber(pathname):
	list_of_files = os.listdir(pathnamesdict[pathname])
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)
f = open("params.txt",'a')
for i in range(1, maxfilenumber(args.dataset), args.njobs):
	start = str(i)
	end = str(min(maxfilenumber(args.dataset),i+args.njobs-1))
	print ("configs/"+str(args.year)+"/mumu_"+args.dataset+".yaml"+ "," +args.outdir+"/output_"+args.dataset +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + args.dataset + "," + pathweightsdict[args.dataset])
	line = "configs/"+str(args.year)+"/mumu_"+args.dataset+".yaml"+ "," +args.outdir+"/output_"+args.dataset +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + args.dataset + "," + pathweightsdict[args.dataset]
	f.write("\n"+line)
f.close()
