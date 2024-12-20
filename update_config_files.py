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
parser.add_argument("-y","--year", dest="year", help="UL2017, UL2018, UL2016, UL2016_APV", type=str)
parser.add_argument("-w","--weightdir", dest="wtdir", help="weights directory: contains info on weight directory etc", type=str)
parser.add_argument("--justweights", dest="justWeights",   help="Just update weight", action='store_true')
parser.add_argument("--justcs", dest="justcs",   help="Just update cross-section", action='store_true')
# parser.add_argument("--hack", dest="hack",   help="hack for restoring config files", action='store_true')
args = parser.parse_args()

fdataset = open(args.dataset,'r')
dict_dataset = yaml.safe_load(fdataset)
yr = args.year.split('UL')[1]
print(yr)

for key in dict_dataset:
	if ('SingleMuon' in key):
		if 'APV' in yr:
			if 'Flate' in key or 'G' in key or 'H' in key:
				continue
	elif yr not in key:
		continue
	# print(key)
	
	# if 'SingleMuon' in key:
	if (dict_dataset[key]['process']=='Data'):
		proc_str = 'mumu'+key.split('_')[-1]
	else:
		proc_str = '_'.join(key.split('_')[:-1])
	if 'APV' in yr:
		proc_str=proc_str.replace('_2016','').replace('_APV','')
	proc_str = proc_str.replace('_SmuonHadronFiltered_','_')
	fin_str = 'configs/'+yr+'/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str+'.yaml'
	# print(proc_str)
	if args.justWeights:
		conf_str = 'configs/'+yr+'/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str+'.yaml'	
		weight_str = args.wtdir+'/'+dict_dataset[key]['process']+'/'+'output_'+proc_str+'.root'
		conf_path = os.path.join(os.getcwd(),conf_str)
		weight_path = os.path.join(os.getcwd(),weight_str)
		# print(path)
		# print (weight_path, conf_path)
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
		fin.close()
		print('Updating weight on:', conf_str)
		continue
	if args.justcs:
		if 'MS' not in proc_str:
			continue
		# print(proc_str)
		# quit()
		f_br = open('BR_xsec_benchmark.yaml','r')
		br_pars=yaml.safe_load(f_br)
		m_str = proc_str.split("_")[-2]
		m = float(proc_str.split("_")[-2].replace('MS','').replace('p','.'))
		ctau = float(proc_str.split("_")[-1].replace('ctauS','').replace('p','.'))
		# f = open("params.txt",'w')
		xsec_ggH = br_pars['xsec_ggH']
		br_Haa = br_pars['br_Haa']
		br_a = br_pars['br_a'][proc_str.split("_")[-2]]
		br_amumu = br_a[2]
		if m >= 1.1:
			br_ahh = br_a[1]
		else:
			br_ahh = br_a[0]
		xsec_br = xsec_ggH*br_Haa*br_amumu*br_ahh

		path = os.path.join(os.getcwd(),fin_str)
		# print(path)
		if not os.path.exists(path):
			continue
		print('Updating:', fin_str)
		fin = open(path,'r')
		conf_pars = yaml.safe_load(fin)
		conf_pars['cross_section'] = xsec_br
		fin.close()
		fin = open(path,'w')
		yaml.dump(conf_pars,fin)
		fin.close()
		# quit()

		continue
		

		# if args.hack:
		# 	proc_str_tmp = proc_str.replace('ctauS1','ctauS0p1')
		# 	fin_str_tmp = 'configs/'+yr+'/'+'datasets/'+dict_dataset[key]['process']+'/'+proc_str_tmp+'.yaml'
		# 	path = os.path.join(os.getcwd(),fin_str_tmp)
		# 	if not os.path.exists(path):
		# 		continue
		# 	if 'MS' not in fin_str_tmp or 'ctauS0p1.yaml' not in fin_str_tmp:
		# 		continue
		# 	print(proc_str_tmp)
			
		# 	fin = open(path,'r')
		# 	conf_pars = yaml.safe_load(fin)
		# 	# print(conf_pars)
		# 	fin.close()
		# 	ctau = ['0','1','10','100']
		# 	for ct in ctau:
		# 		path_tmp = path.replace('ctauS0p1.yaml','ctauS'+ct+'.yaml')
		# 		if not os.path.exists(path_tmp):
		# 			continue
		# 		fin_tmp = open(path_tmp,'w')
		# 		# print(conf_pars)
		# 		conf_pars['histogram'] = proc_str_tmp.replace('ctauS0p1','ctauS'+ct)
		# 		conf_pars['name'] = proc_str_tmp.replace('ctauS0p1','ctauS'+ct)
		# 		yaml.dump(conf_pars,fin_tmp)
		# 		fin_tmp.close()
		# 		print(path_tmp)
				# pars = yaml.safe_load(fin_tmp)
			# quit()
			
	path = os.path.join(os.getcwd(),fin_str)
	# print(path)
	if not os.path.exists(path):
		continue
	if ((dict_dataset[key]['status']!='DONE')):
		continue
	# if 'QCD' in key:
		# continue #for now not updating qcd samples
	print('Updating:', fin_str)
	fin = open(path,'r')
	conf_pars = yaml.safe_load(fin)
	conf_pars['locations'] = dict_dataset[key]['ntuple']
	fin.close()
	fin = open(path,'w')
	yaml.dump(conf_pars,fin)
	# print(conf_pars)
	# if '_ctauS1.yaml' in fin_str:
	# 	conf_pars['name'] = conf_pars['name'].replace('ctauS1','ctauS0p1')
	# 	path2=path.replace('ctauS1','ctauS0p1')
	# 	# print(conf_pars)
	# 	fin2 = open(path2,'w')
	# 	yaml.dump(conf_pars,fin2)
	# 	print('Updating special:', path2)


