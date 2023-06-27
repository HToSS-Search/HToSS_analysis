import numpy as np
import os,argparse
import re
import yaml
import ROOT
import time

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return max(n)

parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=int)
args = parser.parse_args()

fin = open(args.config,'r')
# f = open("params.txt",'w')
conf_pars = yaml.safe_load(fin)
data_loc = conf_pars['locations'][0]
data_name = conf_pars['name']
# list_of_files = os.listdir(data_loc)
treeName = "makeTopologyNtupleMiniAOD/tree"
rdf = ROOT.RDataFrame(treeName,data_loc+"*.root")
entries_total = rdf.Count()
# auto myHist2 = myDf.Histo1D<float>({"histName", "histTitle", 64u, 0., 128.}, "myColumn");
filtered = rdf.Define('mu_trig','HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13 > 0 || HLT_IsoMu27_v14 > 0')\
			.Define('met_filters', 'Flag_goodVertices > 0 && Flag_globalTightHalo2016Filter > 0 && Flag_HBHENoiseFilter > 0 && Flag_HBHENoiseIsoFilter > 0 && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 && Flag_BadPFMuonFilter > 0 && Flag_ecalBadCalibFilter > 0')\
			.Define('mu_eta','muonPF2PATEta[abs(muonPF2PATEta) < 2.4]')
histB = filtered.Histo1D('mu_eta')
histB.Draw()
time.sleep(5)
entries_filtered = filtered.Count()
print ('total:',entries_total.GetValue())
print ('after filter:',entries_filtered.GetValue())
histA = rdf.Histo1D("numMuonPF2PAT")
histA.Draw()
outFile = ROOT.TFile(args.output, "RECREATE")
outFile.cd()
histA.Write()

