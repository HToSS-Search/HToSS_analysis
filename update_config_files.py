import numpy as np
import os,argparse
import re
import yaml
import ROOT
import time
import sys

start_time = time.time()
parser = argparse.ArgumentParser(description='Update config files')
parser.add_argument("-d","--dataset", dest="dataset", help="dataset file: contains info on locations etc", type=str)
parser.add_argument("-w","--weightdir", dest="wtdir", help="weights directory: contains info on weight directory etc", type=str)
parser.add_argument("--justweights", dest="justWeights",   help="Just update weight", action='store_true')
args = parser.parse_args()

fdataset = open(args.dataset,'r')
dict_dataset = yaml.safe_load(fdataset)
for key in dict_dataset:
	if '2017' in key:
		if (dict_dataset[key]['process']=='data'):
			proc_str = 'mumuRun2017'+ key.split('_')[-1][-1]
		else:
			proc_str = '_'.join(key.split('_')[:-1])
		if args.justWeights:
			proc_str = proc_str.replace('_SmuonHadronFiltered_','_')
			fin_str = 'configs/'+'2017/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str+'.yaml'
			conf_str = 'configs/'+'2017/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str+'.yaml'	
			weight_str = args.wtdir+'/'+dict_dataset[key]['process']+'/'+'output_'+proc_str+'.root'
			conf_path = os.path.join(os.getcwd(),conf_str)
			weight_path = os.path.join(os.getcwd(),weight_str)
			# print(path)
			print (weight_path, conf_path)
			if not os.path.exists(conf_path) or not os.path.exists(weight_path):
				continue
			
			froot = ROOT.TFile(weight_path,"READ")
			weightHisto = froot.Get("weightHisto").Clone()
			sum_wts = weightHisto.GetBinContent(2)-weightHisto.GetBinContent(3)
			froot.Close()

			fin = open(conf_path,'r')
			conf_pars = yaml.safe_load(fin)
			conf_pars['sum_weights'] = sum_wts
			fin.close()

			fin = open(conf_path,'w')
			yaml.dump(conf_pars,fin)
			print('Updating weight on:', conf_str)
			continue
		
		# if 'SmuonHadronFiltered' in proc_str:
		proc_str = proc_str.replace('_SmuonHadronFiltered_','_')
		fin_str = 'configs/'+'2017/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str+'.yaml'
		#print(fin_str)
		path = os.path.join(os.getcwd(),fin_str)
		# print(path)
		if not os.path.exists(path):
			continue
		if (dict_dataset[key]['status']!='DONE'):
			continue
		# if 'QCD' in key:
			# continue #for now not updating qcd samples
		fin = open(path,'r')
		conf_pars = yaml.safe_load(fin)
		conf_pars['locations'] = dict_dataset[key]['ntuple']
		fin.close()
		fin = open(path,'w')
		yaml.dump(conf_pars,fin)
		print('Updating:', fin_str)

# tmp_str = ''
# for line in fparams:
# 	if line=='':
# 		continue
# 	elements = line.split(',')
# 	conf = elements[0]
# 	skim_loc = elements[1].split('output')[0]
# 	dataset_name = elements[-1]
# 	if dataset_name==tmp_str:
# 		continue
# 	fin = open(conf,'r')
# 	conf_pars = yaml.safe_load(fin)
# 	conf_pars['skim_location'] = [skim_loc]
# 	fin.close()
# 	fin = open(conf,'w')
# 	yaml.dump(conf_pars,fin)
# 	tmp_str = dataset_name
