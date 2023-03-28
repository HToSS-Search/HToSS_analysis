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
	"wPlusJets": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/Wjets/Wjets/",
	#DYJetsToLL
	"DYJetsToLL_Pt-0To50": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/DYJetsToLL_Pt/DYJetsToLL_Pt-0To50/",
	"DYJetsToLL_Pt-50To100": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/DYJetsToLL_Pt/DYJetsToLL_Pt-50To100",
	"DYJetsToLL_Pt-100To250": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/DYJetsToLL_Pt/DYJetsToLL_Pt-100To250",
	"DYJetsToLL_Pt-250To400": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/DYJetsToLL_Pt/DYJetsToLL_Pt-250To400",
	"DYJetsToLL_Pt-400To650": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/DYJetsToLL_Pt/DYJetsToLL_Pt-400To650",
	"DYJetsToLL_Pt-650ToInf": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/DYJetsToLL_Pt/DYJetsToLL_Pt-650ToInf",
	# single Top
	"tbar_t_channel": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/tbar_t_channel/",
	"tbarW": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/tbarW/",
	"tHq": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/singleTop/tHq/",
	"t_s_channel": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/t_s_channel/",
	"t_t_channel": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/t_t_channel/",
	"tW": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/tW/",
	"tWZ": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/singleTop/tWz_tWll/",
	"tZq": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/singleTop/tZq_ll_4f_ckm/",
	#ttbar
	"ttbar_2l2nu": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttbar/ttbar_2l2nu/",
	"ttbar_hadronic": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttbar/ttbar_hadronic/",
	"ttbar_semileptonic": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttbar/ttbar_semileptonic/",
	#ttV
	"ttgamma": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttgamma/",
	"ttH_bb": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttH_bb/",
	"ttH_nonbb": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttH_nonbb/",
	"ttW2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttW_qq/",
	"ttWTolnu": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttW_lnu/",
	"ttZ2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttZ_qq/",
	"ttZToll": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/ttV/ttZ_ll/",
	#VV
	"WG_lnug": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/WG_lnug/",
	"WW_2l2nu": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/WW_2l2nu/",
	"WW_lnu2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/VV/WW_lnu2q/",
	"WZ_2l2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/WZ_2l2q/",
	"WZ_3lnu": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/WZ_3lnu/",
	"WZ_lnu2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/VV/WZ_lnu2q/",
	"ZG_lnug": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/ZG_llg",
	"ZZ_2l2nu": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/ZZ_2l2nu/",
	"ZZ_2l2q": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v4/VV/ZZ_2l2q/",
	"ZZ_4l": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VV/ZZ_4l/",
	#VVV
	"WWW": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VVV/WWW/",
	"WWZ": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VVV/WWZ/",
	#"WZZ": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VVV/WZZ_ext/"
	#"WZZ": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VVV/WZZ/"
	"ZZZ": "/pnfs/iihe/cms/store/user/almorton/MC/skims2017_v5/VVV/ZZZ/"

	

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
	"wPlusJets": "plots/weightDistributions/wPlusJets/output_wPlusJets.root",
	#DYJetsToLL
	"DYJetsToLL_Pt-0To50": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-0To50.root",
	"DYJetsToLL_Pt-50To100": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-50To100.root",
	"DYJetsToLL_Pt-100To250": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-100To250.root",
	"DYJetsToLL_Pt-250To400": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-250To400.root",
	"DYJetsToLL_Pt-400To650": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-400To650.root",
	"DYJetsToLL_Pt-650ToInf": "plots/weightDistributions/DYJetsToLL_Pt/DYJetsToLL_Pt-650ToInf.root",
	# single Top
	"tbar_t_channel": "plots/weightDistributions/singleTop/tbar_t_channel/",
	"tbarW": "plots/weightDistributions/singleTop/tbarW/",
	"tHq": "plots/weightDistributions/singleTop/tHq/",
	"t_s_channel": "plots/weightDistributions/singleTop/t_s_channel/",
	"t_t_channel": "plots/weightDistributions/singleTop/t_t_channel/",
	"tW": "plots/weightDistributions/singleTop/tW/",
	"tWZ": "plots/weightDistributions/singleTop/tWz_tWll/",
	"tZq": "plots/weightDistributions/singleTop/tZq_ll_4f_ckm/",
	#ttbar
	"ttbar_2l2nu": "plots/weightDistributions/ttbar/ttbar_2l2nu/",
	"ttbar_hadronic": "plots/weightDistributions/ttbar/ttbar_hadronic/",
	"ttbar_semileptonic": "plots/weightDistributions/ttbar/ttbar_semileptonic/",
	#ttV
	"ttgamma": "plots/weightDistributions/ttV/ttgamma/",
	"ttH_bb": "plots/weightDistributions/ttV/ttH_bb/",
	"ttH_nonbb": "plots/weightDistributions/ttV/ttH_nonbb/",
	"ttW2q": "plots/weightDistributions/ttV/ttW_qq/",
	"ttWTolnu": "plots/weightDistributions/ttV/ttW_lnu/",
	"ttZ2q": "plots/weightDistributions/ttV/ttZ_qq/",
	"ttZToll": "plots/weightDistributions/ttV/ttZ_ll/",
	#VV
	"WG_lnug": "plots/weightDistributions/VV/WG_lnug/",
	"WW_2l2nu": "plots/weightDistributions/VV/WW_2l2nu/",
	"WW_lnu2q": "plots/weightDistributions/VV/WW_lnu2q/",
	"WZ_2l2q": "plots/weightDistributions/VV/WZ_2l2q/",
	"WZ_3lnu": "plots/weightDistributions/VV/WZ_3lnu/",
	"WZ_lnu2q": "plots/weightDistributions/VV/WZ_lnu2q/",
	"ZG_lnug": "plots/weightDistributions/VV/ZG_llg",
	"ZZ_2l2nu": "plots/weightDistributions/VV/ZZ_2l2nu/",
	"ZZ_2l2q": "plots/weightDistributions/VV/ZZ_2l2q/",
	"ZZ_4l": "plots/weightDistributions/VV/ZZ_4l/",
	#VVV
	"WWW": "plots/weightDistributions/VVV/WWW/",
	"WWZ": "plots/weightDistributions/VVV/WWZ/",
	#"WZZ": "plots/weightDistributions/VVV/WZZ_ext/"
	#"WZZ": "plots/weightDistributions/VVV/WZZ/"
	"ZZZ": "plots/weightDistributions/VVV/ZZZ/"
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
