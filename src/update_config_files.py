import numpy as np
import os,argparse
import re
import yaml
import ROOT
import time
import sys

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
# parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-p","--params", dest="params", help="param file: contains info on skim location", type=str)
args = parser.parse_args()

fparams = open(args.params,'r')
tmp_str = ''
for line in fparams:
	if line=='':
		continue
	elements = line.split(',')
	conf = elements[0]
	skim_loc = elements[1].split('output')[0]
	dataset_name = elements[-1]
	if dataset_name==tmp_str:
		continue
	fin = open(conf,'r')
	conf_pars = yaml.safe_load(fin)
	conf_pars['skim_location'] = [skim_loc]
	fin.close()
	fin = open(conf,'w')
	yaml.dump(conf_pars,fin)
	tmp_str = dataset_name
