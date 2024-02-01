import numpy as np
import os,argparse
import re
import yaml
import ROOT
import time
import sys

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return min(n),max(n)

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
# parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination directory", type=str)
parser.add_argument("-t","--totalfromcfg", dest="total", help="Take total from config or not", action='store_true')


args = parser.parse_args()

fin = open(args.config,'r')
conf_pars = yaml.safe_load(fin)

data_loc = conf_pars['locations'][0]
cross_section = 1 if 'Run' in args.config else conf_pars['cross_section']
sum_wts = 1 if 'Run' in args.config else conf_pars['sum_weights']
lumi = 1 if 'Run' in args.config else 41474.989603894  #  full 2017
total_fromconfig = 1 if 'Run' in args.config else conf_pars['total_events']
#4247.682053046 #2017D only for now
if data_loc[-1] != '/':
	data_loc = data_loc+'/'
directories = [data_loc+d+"/" for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]
data_name = conf_pars['name']
treeName = "makeTopologyNtupleMiniAOD/tree"
list_of_files = []
if not 'Run' in args.config:
	for dir in directories:
		flow, fhigh = maxfilenumber(dir)
		for i in range(flow, fhigh+1):
			fno = str(i)
			list_of_files.append(dir+"output_"+fno+".root")

	file = ROOT.TFile(list_of_files[0])
	weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
	weightPlot.SetDirectory(0)
	file.Close()
	# print(weightPlot)
	# total_entries = 0
	for i,fistr in enumerate(list_of_files):
		if i==0:
			continue
		if not os.path.exists(fistr):
			continue
		# print("Processing: ",fistr)
		file = ROOT.TFile(fistr)
		tmpPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
		# total_entries=total_entries+tmpPlot.GetEntries()
		weightPlot.Add(tmpPlot)
		file.Close()

	totalEvents_ = weightPlot.GetBinContent(1) - weightPlot.GetBinContent(2)
	if args.total:
		sum_wts = total_fromconfig
	else:
		sum_wts = totalEvents_
	sys.stderr.write("sum of weights:"+str(sum_wts))


sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')
sys.stderr.flush()





