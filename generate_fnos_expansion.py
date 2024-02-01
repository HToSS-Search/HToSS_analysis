import numpy as np
import os,argparse
import re
import yaml

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)

parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file (with all processes)", type=str)
parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-n","--njobs", dest="njobs", help="No. of files to process in one job", type=int)
parser.add_argument("-o","--output", dest="outdir", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=str) #UL2017
parser.add_argument("-f","--fname", dest="fname", help="Name of params file", type=str)
parser.add_argument("-d","--data",dest="isData", help="Data or not; false by default", action="store_true")
# parser.add_argument("-s","--skim",dest="justSkim", help="Skimming or not; false by default", action="store_true")
# parser.add_argument("-w","--weight",dest="justWeights", help="Generate only weight dist? true or false; false by default", action="store_true")
parser.add_argument("-j","--justWeights",dest="justWeights", help="Measure just weights; false by default", action="store_true")
# parser.add_argument("--withSkim",dest="withSkim", help="Use skimmed files instead of bare ntuples; false by default", action="store_true")

args = parser.parse_args()

fin = open(args.config,'r')
f = open(args.fname,'w')
pars = yaml.safe_load(fin)
for conf in pars['datasets']:
	f_conf = open(conf,'r')
	conf_pars = yaml.safe_load(f_conf) 
	data_loc = conf_pars['locations']
	if isinstance(data_loc,list):
		loc=data_loc[0]
		nmax = maxfilenumber(loc)
		out_name = sample_string[len(sample_string)-2] if sample_string[len(sample_string)-1]=='' else sample_string[len(sample_string)-1]
	else:
		loc = data_loc
		directories = [loc+"/"+d+"/" for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]
		# print(directories)
		nmax = maxfilenumber(directories[-1])
		out_name = conf.split("/")[-1].split(".yaml")[0]


	data_name = conf_pars['name']
	# for loc in data_loc:
	sample_string = loc.split("/")
	sample_string_2 = conf.split("/")
	sample_type = sample_string_2[len(sample_string_2)-2] if ".yaml" in sample_string_2[len(sample_string_2)-1] else sample_string_2[len(sample_string_2)-1]
	outdir_extra = conf.split("/datasets/")[1].split(".yaml")[0]
	start, end = str(0),str(0) #dummy values
	# if (args.justWeights):
	# 	print (conf + "," +"test_weights" +".root" + "," + str(1) + "," + str(maxfilenumber(loc)) + "," + data_name)
	# 	line = conf + "," +"test_weights" + ".root" + "," + str(1) + "," + str(maxfilenumber(loc)) + "," + data_name
	# 	f.write("\n"+line)
	# 	continue
	# if (args.withSkim):
	# 	print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name + ".root" + "," + start + "," + end + "," + data_name + "," + "dummy")
	# 	line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +  ".root" + "," + start + "," + end + "," + data_name + "," + "dummy"
	# 	f.write("\n"+line)
	# 	continue
	if 'DYJetsToLL_Pt-650ToInf' in data_name or 'DYJetsToLL_Pt-400To650' in data_name:
		njobs = 2
	else:
		njobs = args.njobs
	for i in range(1, nmax+1, njobs):
		start = str(i)
		end = str(min(nmax,i+njobs-1))
		if (args.isData):
			# print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name + "," + "dummy")
			# line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name + "," + "dummy"
			print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name)
			line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name

		elif (args.justWeights): # generating only weight distribution
			print (conf + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name)
			line = conf + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name
		# elif (args.justSkim):
		# 	print (conf + "," +args.outdir+"/"+outdir_extra+"/output_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name)
		# 	line = conf + "," +args.outdir+"/"+outdir_extra+"/output_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name
		else:
			# $(cfg) $(output) $(flow) $(fhigh) $(dataset) $(weight)
			weightFile = "plots/weightDistributions/"+sample_type+"/output_"+data_name+".root"
			# print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name + "," + weightFile)
			# line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name + "," + weightFile
			print (conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end + ".root" + "," + start + "," + end + "," + data_name)
			line = conf + "," + args.cuts + "," +args.outdir+"/"+outdir_extra+"/output_"+out_name +"_" + start +"-"+ end +  ".root" + "," + start + "," + end + "," + data_name
		f.write("\n"+line)
f.close()

