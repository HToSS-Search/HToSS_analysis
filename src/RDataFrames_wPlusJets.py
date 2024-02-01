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
getLV_code = '''
ROOT::Math::PtEtaPhiMVector makeLVs(Double_t mu_pt,Double_t mu_eta,Double_t mu_phi, Double_t mu_mass) {
    ROOT::VecOps::RVec<ROOT::Math::PtEtaPhiMVector> lvs;
    ROOT::Math::PtEtaPhiMVector lVec1 {mu_pt, mu_eta, mu_phi, mu_mass};
    // lvs.push_back(lVec1);
    return lVec1;
}
ROOT::Math::PtEtaPhiMVector makeLVs_alt(ROOT::VecOps::RVec<Float_t>& mu_pt,ROOT::VecOps::RVec<Float_t>& mu_eta,ROOT::VecOps::RVec<Float_t>& mu_phi, double mu_mass) {
    // std::cout<<"check this:"<<mu_pt.size()<<","<<mu_eta.size()<<","<<mu_phi.size()<<std::endl;
    ROOT::VecOps::RVec<ROOT::Math::PtEtaPhiMVector> lvs;
    ROOT::Math::PtEtaPhiMVector lVec1 {mu_pt[0], mu_eta[0], mu_phi[0], mu_mass};
    lvs.push_back(lVec1);
    return lVec1;
}
'''
getKinematics_code ='''
using FourVector = ROOT::Math::PtEtaPhiMVector;
using namespace ROOT::VecOps;
RVec<Float_t> getKinematics(const RVec<FourVector> &tracks, TString var = "pt")
{
   auto pt = [](const FourVector &v) { return v.Pt(); };
   auto eta = [](const FourVector &v) { return v.Eta(); };
   auto phi = [](const FourVector &v) { return v.Phi(); };
   auto mass = [](const FourVector &v) { return v.M(); };
   auto mt = [](const FourVector &v) { return v.Mt(); };
   if (var.Contains("eta")) return Map(tracks, eta);
   else if (var.Contains("phi")) return Map(tracks, phi);
   else if (var.Contains("mass")) return Map(tracks, mass);
   else if (var.Contains("mt")) return Map(tracks, mt);
   else return Map(tracks, pt);
}
'''
ROOT.gInterpreter.Declare(getKinematics_code)
ROOT.gInterpreter.Declare(getLV_code)
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
if ('Run' not in args.config):
	directories = [data_loc+d+"/" for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]
# else:
	# directories = [data_loc for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]
else:
	directories = [data_loc]
data_name = conf_pars['name']
treeName = "makeTopologyNtupleMiniAOD/tree"
list_of_files = []
for dir in directories:
	flow, fhigh = maxfilenumber(dir)
	for i in range(flow, fhigh+1):
		fno = str(i)
		list_of_files.append(dir+"output_"+fno+".root")
if not 'Run' in args.config:
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
# print("sum of entries from weight plots:",total_entries)
rdf = ROOT.RDataFrame(treeName,list_of_files)
# print('RDF loaded')
entries_total = rdf.Count()
# auto myHist2 = myDf.Histo1D<float>({"histName", "histTitle", 64u, 0., 128.}, "myColumn");
sys.stderr.write("entries total:"+str(entries_total.GetValue())+'\n')
sys.stderr.flush()

# attempt to make LorentzVector for tracks collection


trigger_cuts = 'HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13 > 0 || HLT_IsoMu27_v14 > 0'
veto_muon = 'abs(muonPF2PATEta) <2.4 && muonPF2PATLooseCutId && muonPF2PATPt > 10 && muonPF2PATDBPV < 0.05'
veto_mu_add = 'mu_pt[0] > 29 && mu_pt[0] < 55 && mu_globalid[0] && mu_pfiso[0]<0.15 && mu_medium[0]' #ordered collection
final_selection = 'W_mt > 40' +'&&'+ veto_mu_add
veto_ele = 'abs(elePF2PATEta) <2.4 && elePF2PATPT > 10 && elePF2PATBeamSpotCorrectedTrackD0 < 0.05 && elePF2PATTrackDz < 0.2 && elePF2PATCutIdVeto' # to be worked on if needed
# dilepton_cut = 'dilep_mass > 70 && dilep_mass < 110'

trigger_definitions = rdf.Define('mu_trig',trigger_cuts)\
				.Define('met_filters', 'Flag_goodVertices > 0 && Flag_globalTightHalo2016Filter > 0 && Flag_HBHENoiseFilter > 0 && Flag_HBHENoiseIsoFilter > 0 && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 && Flag_BadPFMuonFilter > 0 && Flag_ecalBadCalibFilter > 0')
if not '2017B' in args.config:
	cut_trig = trigger_definitions.Filter('mu_trig')
else:
	cut_trig = trigger_definitions
cut_met = cut_trig.Filter('met_filters')
# muon_definitions = cut_met.Define('veto_mu')
muon_definitions = cut_met.Define('mu_pt',f'muonPF2PATPt[{veto_muon}]')\
				.Define('mu_ch',f'muonPF2PATCharge[{veto_muon}]')\
				.Define('mu_px',f'muonPF2PATPX[{veto_muon}]')\
				.Define('mu_py',f'muonPF2PATPY[{veto_muon}]')\
				.Define('mu_pz',f'muonPF2PATPZ[{veto_muon}]')\
				.Define('mu_eta',f'muonPF2PATEta[{veto_muon}]')\
				.Define('mu_phi',f'muonPF2PATPhi[{veto_muon}]')\
				.Define('mu_medium',f'muonPF2PATMediumCutId[{veto_muon}]')\
				.Define('mu_pfiso',f'muonPF2PATComRelIsodBeta[{veto_muon}]')\
				.Define('mu_globalid',f'muonPF2PATGlobalID[{veto_muon}]')\
				.Define('mu_sel','mu_pt.size()==1')
cut_mu_veto = muon_definitions.Filter('mu_sel')
# ele_definitions = cut_mu_veto.Define('ele_pt',f'elePF2PATPT[{veto_ele}]')\
				# .Define('ele_sel','ele_pt.size()==0')
# cut_ele_veto = ele_definitions.Filter('ele_sel')
cut_ele_veto = cut_mu_veto
veto_muon_definitions = cut_ele_veto.Define('met_pt',f'metPF2PATPt')\
				.Define('met_eta',f'mu_eta[0]')\
				.Define('met_phi',f'metPF2PATPhi')\
				.Define('mu_lv','makeLVs(mu_pt[0],mu_eta[0],mu_phi[0],0.105)')\
				.Define('met_lv','makeLVs(met_pt,met_eta,met_phi,0.)')\
				.Define('W_lv','mu_lv+met_lv')\
				.Define('W_mt','W_lv.Mt()')\
				.Define('W_mu_pt','mu_pt[0]')\
				.Define('W_mu_eta','mu_eta[0]')\
				.Define('final_sel',final_selection)\
				.Define('evt_wt',f'({cross_section}*{lumi}/{sum_wts})*processMCWeight')\
				.Define('sample_wt',f'({cross_section}*{lumi}/{sum_wts})')
cut_final = veto_muon_definitions.Filter('final_sel')

# disp = cut_final.Display(("processMCWeight","evt_wt"),100)

entries_trig = cut_trig.Count()
entries_met= cut_met.Count()
entries_mu_sel= cut_mu_veto.Count()
entries_ele_sel= cut_ele_veto.Count()
entries_final= cut_final.Count()
# entries_test= cut_test.Count()
sample_wt = cross_section*lumi/sum_wts
sys.stderr.write ('total:'+str(entries_total.GetValue())+'\n')
sys.stderr.write('After trigger cut:'+str(entries_trig.GetValue())+'\n')
sys.stderr.write('After MET filters:'+str(entries_met.GetValue())+'\n')
sys.stderr.write('After muon selection:'+str(entries_mu_sel.GetValue())+'\n')
sys.stderr.write('After electron selection:'+str(entries_ele_sel.GetValue())+'\n')
sys.stderr.write('After dimuon cut:'+str(entries_final.GetValue())+'\n')
# disp.Print()
sys.stderr.write('Sample weight:'+str(sample_wt)+'\n')
sys.stderr.flush()

if not 'Run' in args.config:
	histA = cut_final.Histo1D(("mu_pt","mu_pt",400,29,55),"W_mu_pt","evt_wt")
	histB = cut_final.Histo1D(("mu_pt_unwtd","mu_pt_unwtd",400,29,55),"W_mu_pt")
	histC = cut_final.Histo1D("evt_wt")
	histD = cut_final.Histo1D(("processMCWeight","processMCWeight",100,0,20),"processMCWeight")
	histE = cut_final.Histo1D(("mu_pt_sample_wtd","mu_pt_sample_wtd",400,29,55),"W_mu_pt","sample_wt")
	histF = cut_final.Histo1D(("mu_eta","mu_eta",800,-2.4,2.4),"W_mu_eta","evt_wt")
	histG = cut_final.Histo1D(("mu_eta_unwtd","mu_eta_unwtd",800,-2.4,2.4),"W_mu_eta")
	histH = cut_final.Histo1D(("mu_mt","mu_mt",800,0,400),"W_mt","evt_wt")
	
else:
	histA = cut_final.Histo1D(("mu_pt","mu_pt",400,29,55),"W_mu_pt")
	histF = cut_final.Histo1D(("mu_eta","mu_eta",800,-2.4,2.4),"W_mu_eta")
	histH = cut_final.Histo1D(("mu_mt","mu_mt",800,0,400),"W_mt")

# histA.Draw()
# time.sleep(10)
outFile = ROOT.TFile(args.output, "RECREATE")
outFile.cd()
histA.Write()
histF.Write()
histH.Write()
if not 'Run' in args.config:
	weightPlot.Write()
	histB.Write()
	histC.Write()
	histD.Write()
	histE.Write()
	histG.Write()



sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')
sys.stderr.flush()





