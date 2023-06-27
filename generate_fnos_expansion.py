import numpy as np
import os,argparse
import re
import yaml


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
	"WZZ": "plots/weightDistributions/VVV/WZZ/",
	"ZZZ": "plots/weightDistributions/VVV/ZZZ/"
}

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)

parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-n","--njobs", dest="njobs", help="No. of files to process in one job", type=int)
parser.add_argument("-o","--output", dest="outdir", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=int)
parser.add_argument("-w","--weight",dest="isWeight", help="Generate weight dist? true or false", default=0, type=int)
parser.add_argument("-d","--data",dest="isData", help="Data or not", default=0, type=int)
args = parser.parse_args()

fin = open(args.config,'r')
f = open("params.txt",'w')
pars = yaml.safe_load(fin)
for conf in pars['datasets']:
	f_conf = open(conf,'r')
	conf_pars = yaml.safe_load(f_conf) 
	data_loc = conf_pars['locations']
	data_name = conf_pars['name']
	for loc in data_loc:
		for i in range(1, maxfilenumber(loc), args.njobs):
			start = str(i)
			end = str(min(maxfilenumber(loc),i+args.njobs-1))
			sample_string = loc.split("/")
			out_name = sample_string[len(sample_string)-2] if sample_string[len(sample_string)-1]=='' else sample_string[len(sample_string)-1]
			sample_string_2 = conf.split("/")
			sample_type = sample_string_2[len(sample_string_2)-2] if ".yaml" in sample_string_2[len(sample_string_2)-1] else sample_string_2[len(sample_string_2)-1]
			outdir_extra = conf.split("/datasets/")[1].split(".yaml")[0]
			if (args.isData):
				print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name + "," + "dummy")
				line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name + "," + "dummy"
			elif (args.isWeight): # generating only weight distribution
				print (conf + "," + args.cuts + "," +args.outdir+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name)
				line = conf + "," + args.cuts + "," +args.outdir+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name
			else:
				# $(cfg) $(output) $(flow) $(fhigh) $(dataset) $(weight)
				weightFile = "plots/weightDistributions/"+sample_type+"/output_"+data_name+".root"
				print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name + "," + weightFile)
				line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name + "," + weightFile
			
			f.write("\n"+line)
f.close()

