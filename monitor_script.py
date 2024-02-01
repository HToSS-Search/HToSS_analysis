import numpy as np
import os,argparse
import re
import yaml

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)

parser = argparse.ArgumentParser(description='arguments')
parser.add_argument("-i", "--input", dest="input",   help="Enter params file to process", type=str)
parser.add_argument("-s", "--suffix", dest="suf",   help="Enter suffix for output file", type=str)
parser.add_argument("-d", "--data", dest="isData",   help="Special mod for data", action='store_true')
parser.add_argument("--skim", dest="isSkim",   help="Special mod for skim", action='store_true')
args = parser.parse_args()
fparams = open(args.input,'r')
error_files = os.listdir('error')
log_files = os.listdir('log')
fresub_illegal = open('params_resubmit_ill_'+args.suf+'.txt','w')
fresub_other = open('params_resubmit_oth_'+args.suf+'.txt','w')
# alternative approach below
for l_no, line in enumerate(fparams):
	arguments = line.split(',')
	root_fname = [i for i in arguments if ".root" in i and "weight" not in i][0]
	if args.isSkim:
		error_fname = [i.split('/')[-1].replace('output_','error/error_skim_').replace('.root','.txt') for i in arguments if ".root" in i and "weight" not in i][0]
		error_fname = 'error/error_skim_'+arguments[-1].replace('\n','')+'_'+arguments[-3]+'-'+arguments[-2]+'.txt'
		# error_skim_$(dataset)_$(flow)-$(fhigh).txt
	else:
		if (args.isData):
			error_fname = [i.split('/')[-1].replace('output_','error/error_mumu').replace('.root','.txt') for i in arguments if ".root" in i and "weight" not in i][0]
		else:
			error_fname = [i.split('/')[-1].replace('output_','error/error_').replace('.root','.txt') for i in arguments if ".root" in i and "weight" not in i][0]
	
	if ('tWz' in error_fname):
		error_fname = error_fname.replace('tWz','tWZ').replace('_tWll_','_')
	if ('tZq' in error_fname):
		error_fname = error_fname.replace('tZq_ll_4f_ckm_','tZq_')
	if ('Wjets' in error_fname):
		error_fname = error_fname.replace('Wjets','wPlusJets')
	if ('ttW_qq' in error_fname):
		error_fname = error_fname.replace('ttW_qq','ttW2q')
	if ('ttZ_ll' in error_fname):
		error_fname = error_fname.replace('ttZ_ll','ttZToll')
	if ('ttZ_qq' in error_fname):
		error_fname = error_fname.replace('ttZ_qq','ttZ2q')
	if ('WZZ_ext' in error_fname):
		error_fname = error_fname.replace('WZZ_ext','WZZ')

	fin = open(error_fname,'r')
	content = fin.read()
	if 'Illegal instruction' in content:
		# print(error_fname)
		fresub_illegal.write(line)
	elif os.path.getsize(root_fname) < 1000 and os.path.getsize(root_fname) > 0:
		# print(root_fname, os.path.getsize(root_fname))
		fresub_other.write(line)

# print(resub_list)
	# print()
