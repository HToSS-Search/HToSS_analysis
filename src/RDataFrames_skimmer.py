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
def getEvtWeight(rdf):
	genSumOfWeights = rdf.Sum('processMCWeight').GetValue()
	return genSumOfWeights


getCh_code = '''
ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> makeLVs(ROOT::VecOps::RVec<Int_t>& packedCandsPdgId,ROOT::VecOps::RVec<Int_t>& packedCandsCharge,ROOT::VecOps::RVec<Int_t>& packedCandsHasTrackDetails,ROOT::VecOps::RVec<Float_t>& packedCandsPx,ROOT::VecOps::RVec<Float_t>& packedCandsPy,ROOT::VecOps::RVec<Float_t>& packedCandsPz, double chsMass_, bool flag = true) {
    ROOT::VecOps::RVec<ROOT::Math::PxPyPzMVector> lvs;
    // std::cout<<"check sizes:"<<packedCandsPdgId.size()<<","<<packedCandsCharge.size()<<","<<packedCandsHasTrackDetails.size()<<","<<packedCandsPx.size()<<","<<packedCandsPy.size()<<","<<packedCandsPz.size()<<std::endl;
    for (Int_t k = 0; k < packedCandsPx.size(); k++) {
        ROOT::Math::PxPyPzMVector lVec {packedCandsPx[k], packedCandsPy[k], packedCandsPz[k], chsMass_};
        lvs.push_back(lVec);
    }
    // std::cout<<"Gets out"<<std::endl;
    return lvs;
}
'''
getIndices_code = '''
using namespace ROOT::VecOps;

RVec<unsigned long> getIndices(const int num) {
	RVec<unsigned long> idx;
	for (int i = 0;i<num;i++) idx.emplace_back(i);
	return idx;
}
'''
getKinematics_code ='''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
RVec<Float_t> getKinematics(const RVec<FourVector> &tracks, TString var = "pt")
{
   auto pt = [](const FourVector &v) { return v.Pt(); };
   auto eta = [](const FourVector &v) { return v.Eta(); };
   auto phi = [](const FourVector &v) { return v.Phi(); };
   if (var.Contains("eta")) return Map(tracks, eta);
   else if (var.Contains("phi")) return Map(tracks, phi);
   else return Map(tracks, pt);
}
'''
getDileptonCand_code = '''
using FourVector = ROOT::Math::PxPyPzMVector;
using namespace ROOT::VecOps;
bool getMuonTrackPairIndex(int leadingidx, int subleadingidx, Float_t num, const RVec<Float_t>& mu_idx1, const RVec<Float_t>& mu_idx2,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz,  double muonMass_, std::vector<ROOT::Math::PxPyPzMVector> &refit_trks) { 
    int TkPairIdx = -1;
    for (int i{0}; i < num; i++) {
        if ((mu_idx1[i] == leadingidx && mu_idx2[i] == subleadingidx) || (mu_idx1[i] == subleadingidx && mu_idx2[i] == leadingidx)) {
          TkPairIdx = i;
          break;
        }
    }
    int idx = TkPairIdx;
    if (idx < 0) return false;

    if ( std::isnan(mu1_px[idx])  || std::isnan(mu2_px[idx]) ) return false;
    if ( std::isnan(mu1_py[idx])  || std::isnan(mu2_py[idx]) ) return false;
    if ( std::isnan(mu1_pz[idx]) || std::isnan(mu2_pz[idx]) ) return false;

    ROOT::Math::PxPyPzMVector muTrk1{mu1_px[idx], mu1_py[idx], mu1_pz[idx], muonMass_};
    ROOT::Math::PxPyPzMVector muTrk2{mu2_px[idx], mu2_py[idx], mu2_pz[idx], muonMass_};
    refit_trks[0]  = muTrk1.Pt() > muTrk2.Pt() ? muTrk1 : muTrk2;
    refit_trks[1] = muTrk1.Pt() > muTrk2.Pt() ? muTrk2 : muTrk1;
    return true;
}
bool getDileptonCand(const RVec<unsigned long>& mu_pt_sorted_idx, const RVec<Int_t>& mu_ch, const RVec<Float_t>& mu_px, const RVec<Float_t>& mu_py,const RVec<Float_t>& mu_pz, const Float_t mass, Float_t diMuonPt_, Float_t num, const RVec<Float_t>& mu_idx1, const RVec<Float_t>& mu_idx2,const RVec<Float_t>& mu1_px, const RVec<Float_t>& mu1_py, const RVec<Float_t>& mu1_pz, const RVec<Float_t>& mu2_px, const RVec<Float_t>& mu2_py, const RVec<Float_t>& mu2_pz, bool flag) {
	RVec<RVec<int>> mu_pairs(2);
	float maxDileptonDeltaR_ = 0.4;
    for (int i=0;i<mu_pt_sorted_idx.size();i++) {
        for (int j=i+1;j<mu_pt_sorted_idx.size();j++) { //it is Pt sorted array
            if (mu_ch[mu_pt_sorted_idx[i]] * mu_ch[mu_pt_sorted_idx[j]] >= 0) continue;
            FourVector lepton1{mu_px[mu_pt_sorted_idx[i]], mu_py[mu_pt_sorted_idx[i]], mu_pz[mu_pt_sorted_idx[i]], mass};
            FourVector lepton2{mu_px[mu_pt_sorted_idx[j]], mu_py[mu_pt_sorted_idx[j]], mu_pz[mu_pt_sorted_idx[j]], mass};
            double delR = ROOT::Math::VectorUtil::DeltaR(lepton1,lepton2);
            if ( delR > maxDileptonDeltaR_ ) continue;
            std::vector<FourVector> refittedTrks(2);
            if (!getMuonTrackPairIndex(mu_pt_sorted_idx[i], mu_pt_sorted_idx[j], num, mu_idx1, mu_idx2,mu1_px, mu1_py,mu1_pz, mu2_px, mu2_py, mu2_pz,  mass, refittedTrks)) continue; //checking if refitted tracks are present -> necessary for SV construction
            FourVector lepton1_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[0] : refittedTrks[1];
            FourVector lepton2_refit = refittedTrks[0].Pt() > refittedTrks[1].Pt() ? refittedTrks[1] : refittedTrks[0];
            double pT { (lepton1_refit+lepton2_refit).Pt() };
            if (flag) {
              if (pT < diMuonPt_) continue;
            }
		    return true;
        }
    }
    return false;
}
'''

ROOT.gInterpreter.Declare(getIndices_code)
ROOT.gInterpreter.Declare(getKinematics_code)
ROOT.gInterpreter.Declare(getDileptonCand_code)
ROOT.gInterpreter.Declare(getCh_code)

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
# parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=int)
parser.add_argument("--flow", dest="flow", help="start file number",default=1, type=int)
parser.add_argument("--fhigh", dest="fhigh", help="end file number", default=100,type=int)
parser.add_argument("--onlyweights", dest="onlyweights", help="just store MCweights weighted with cs*lumi", action='store_true')
# parser.add_argument("--total", dest="onlyweights", help="just store MCweights weighted with cs*lumi", action='store_true')

args = parser.parse_args()

fin = open(args.config,'r')
# f = open("params.txt",'w')
conf_pars = yaml.safe_load(fin)

data_loc = conf_pars['locations']
cross_section = 1 if 'Run' in args.config else conf_pars['cross_section']
sum_wts = 1 if 'Run' in args.config else conf_pars['sum_weights']
# lumi = 1 if 'Run' in args.config else 41474 #2017 for now
lumi = 1 if 'Run' in args.config else 4247.682053046 #2017D for now

isOldNtuple = False
if 'almorton' in data_loc:
	isOldNtuple = True

if data_loc[-1] != '/':
	data_loc = data_loc+'/'
if isOldNtuple:
	directories = [data_loc] #below only for old ntuples
else:
	directories = [data_loc+d+"/" for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]

print(directories)
data_name = conf_pars['name']
treeName = "makeTopologyNtupleMiniAOD/tree"
list_of_files = []
for dir in directories:
	print(dir)
	flow, fhigh = maxfilenumber(dir)
	if flow >= args.fhigh or fhigh <= args.flow:
		continue
	if args.flow >= flow: 
		flow = args.flow
	if args.fhigh <= fhigh:
		fhigh = args.fhigh
	for i in range(flow, fhigh+1):
		fno = str(i)
		fistr = dir+"output_"+fno+".root"
		if not os.path.exists(fistr):
			continue
		list_of_files.append(fistr)
# print(list_of_files)
if args.onlyweights:
	# sum_wts calculated here
	print("enters sum weights calculation")
	if not 'Run' in args.config:
		file = ROOT.TFile(list_of_files[0])
		weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
		weightPlot.SetDirectory(0)
		file.Close()
		for i,fistr in enumerate(list_of_files):
			if i==0:
				continue
			if not os.path.exists(fistr):
				continue
			try:
				# Open the ROOT file
				file = ROOT.TFile.Open(fistr)

				# Check if the file was opened successfully
				if not file or file.IsZombie() or file.TestBit(ROOT.TFile.kRecovered):
					raise Exception(f"Error opening file: {fistr}")

				# Process the file
				tmpPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
				# total_entries=total_entries+tmpPlot.GetEntries()
				weightPlot.Add(tmpPlot)
				# Close the file
				file.Close()
				# continue
			except Exception as e:
				print(f"Error processing file {fistr}: {e}")
				continue  # Continue to the next file in case of an error
		totalEvents_ = weightPlot.GetBinContent(2) - weightPlot.GetBinContent(3)
		sum_wts = totalEvents_
	sys.stderr.write("sum of weights:"+str(sum_wts)+"\n")
	# conf_pars['sum_weights'] = sum_wts
	sys.stderr.write("sum of weights written: "+str(sum_wts)+"\n")
	fout = ROOT.TFile(args.output,"RECREATE")
	weightPlot.Write()
	fout.Close()
	# fin.close()
	# fin = open(args.config,'w')
	# yaml.dump(conf_pars,fin)
	sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')

	quit()

# sys.stderr.write("sum of weights:"+str(sum_wts)+"\n")

# data_loc = conf_pars['locations'][0]
# maxfno = maxfilenumber(data_loc)
# flag = True
# if maxfno < args.fhigh:
# 	flag = False
# 	if len(conf_pars['locations']) > 1:
# 		data_loc=conf_pars['locations'][1]
# 		flag = True
# if (not flag):
# 	sys.stderr.write("Potential error due to multiple datasets"'\n')
# 	sys.stderr.flush()
# if data_loc[-1] != '/':
# 	data_loc = data_loc+'/'
# data_name = conf_pars['name']
# # list_of_files = os.listdir(data_loc)
# treeName = "makeTopologyNtupleMiniAOD/tree"
# list_of_files = []
# for i in range(args.flow, args.fhigh+1):
# 		fno = str(i)
# 		list_of_files.append(data_loc+"output_"+fno+".root")
# if args.onlyweights:
# 	rdf = ROOT.RDataFrame(treeName,data_loc+"*.root")
# 	sys.stderr.write("rdf loaded\n")
# 	sys.stderr.flush()

# 	sumofwts = getEvtWeight(rdf)
# 	conf_pars['sum_weights'] = sumofwts
# 	sys.stderr.write("sum of weights written: "+str(sumofwts)+"\n")
# 	fin.close()
# 	fin = open(args.config,'w')
# 	yaml.dump(conf_pars,fin)
# 	quit()
# else:
# 	rdf = ROOT.RDataFrame(treeName,list_of_files)
rdf = ROOT.RDataFrame(treeName,list_of_files)
# print(data_loc)
entries_total = rdf.Count()
# auto myHist2 = myDf.Histo1D<float>({"histName", "histTitle", 64u, 0., 128.}, "myColumn");
sys.stderr.write("entries total:"+str(entries_total.GetValue())+'\n')
sys.stderr.flush()

# attempt to make LorentzVector for tracks collection


trigger_cuts = 'HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13 > 0 || HLT_IsoMu27_v14 > 0'
muon_cuts = 'abs(muonPF2PATEta) <2.4 && muonPF2PATIsPFMuon && muonPF2PATLooseCutId && muonPF2PATPt > 5'
leadingmu_cut = 'Max(mu_pt)>30' #ordered collection
ch_trk_cuts_part1 = 'abs(packedCandsPdgId)==211 && packedCandsCharge!=0 && packedCandsHasTrackDetails==1'
ch_trk_cuts_part2 = 'lv_trk_pt>5 && abs(lv_trk_eta)<2.4'


chsMass_ = 0.493677 # 0.13957061;//pion mass
# trk_cuts = 'abs()'

trigger_definitions = rdf.Define('mu_trig',trigger_cuts)\
				.Define('met_filters', 'Flag_goodVertices > 0 && Flag_globalTightHalo2016Filter > 0 && Flag_HBHENoiseFilter > 0 && Flag_HBHENoiseIsoFilter > 0 && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 && Flag_BadPFMuonFilter > 0 && Flag_ecalBadCalibFilter > 0')

cut_trig = trigger_definitions.Filter('mu_trig')
cut_met = cut_trig.Filter('met_filters')

muon_definitions = cut_met.Define('mu_pt',f'muonPF2PATPt[{muon_cuts}]')\
				.Define('mu_sel','mu_pt.size()>=2' + ' && ' + leadingmu_cut)
cut_mu_sel = muon_definitions.Filter('mu_sel')

dimuon_definitions = cut_mu_sel.Define('mu_globalidx',f'getIndices(numMuonPF2PAT)[{muon_cuts}]')\
			.Define('mu_pt_sorted_globalidx_nocuts','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(muonPF2PATPt))')\
			.Define('mu_pt_sorted_globalidx','ROOT::VecOps::Intersect(mu_pt_sorted_globalidx_nocuts,mu_globalidx,false)')\
			.Define('mu_pt_sorted_idx','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(mu_pt))')\
			.Define('mu_charge',f'muonPF2PATCharge[{muon_cuts}]')\
			.Define('mu_pair_check','getDileptonCand(mu_pt_sorted_globalidx, muonPF2PATCharge, muonPF2PATPX, muonPF2PATPY, muonPF2PATPZ, 0.1056583745, 0, numMuonTrackPairsPF2PAT,muonTkPairPF2PATIndex1,muonTkPairPF2PATIndex2,muonTkPairPF2PATTk1Px,muonTkPairPF2PATTk1Py,muonTkPairPF2PATTk1Pz,muonTkPairPF2PATTk2Px,muonTkPairPF2PATTk2Py,muonTkPairPF2PATTk2Pz,true)')
cut_dimuon = dimuon_definitions.Filter('mu_pair_check')

hadron_definitions = cut_dimuon.Define('cand_globalidx','getIndices(numPackedCands)')\
			.Define('cand_trk_px',f'packedCandsPx[{ch_trk_cuts_part1}]')\
			.Define('cand_trk_py',f'packedCandsPy[{ch_trk_cuts_part1}]')\
			.Define('cand_trk_pz',f'packedCandsPz[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_pdgid',f'packedCandsPdgId[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_charge',f'packedCandsCharge[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_details',f'packedCandsHasTrackDetails[{ch_trk_cuts_part1}]')\
			.Define('ch_trk_mass',f'{chsMass_}')\
			.Define('ch_trk_sel_part1',ch_trk_cuts_part1)\
			.Define('LVs',f'makeLVs(ch_trk_pdgid, ch_trk_charge, ch_trk_details, cand_trk_px, cand_trk_py, cand_trk_pz, {chsMass_}, true)')\
			.Define('lv_trk_pt','getKinematics(LVs, "pt")')\
			.Define('lv_trk_eta','getKinematics(LVs, "eta")')\
			.Define('ch_trk_pt',f'lv_trk_pt[{ch_trk_cuts_part2}]')\
			.Define('ch_trk_eta',f'lv_trk_eta[{ch_trk_cuts_part2}]')\
			.Define('LVs_sel',f'LVs[{ch_trk_cuts_part2}]')\
			.Define('ch_trk_sel','LVs_sel.size()>=2')\

cut_hadron_sel = hadron_definitions.Filter('ch_trk_sel') 
############## Use Map to check if elements match between two vectors ###############
dihadron_definitions = cut_hadron_sel.Define('cand_globalidx_hadronsonly',f'getIndices(numPackedCands)[{ch_trk_cuts_part1}]')\
						.Define('ch_globalidx',f'cand_globalidx_hadronsonly[{ch_trk_cuts_part2}]')\
						.Define('ch_pair_check',f'getDileptonCand(ch_globalidx, packedCandsCharge, packedCandsPx, packedCandsPy, packedCandsPz, {chsMass_}, 20, numChsTrackPairs,chsTkPairIndex1,chsTkPairIndex2,chsTkPairTk1Px,chsTkPairTk1Py,chsTkPairTk1Pz,chsTkPairTk2Px,chsTkPairTk2Py,chsTkPairTk2Pz,true)')

cut_dihadron = dihadron_definitions.Filter('ch_pair_check')
# print(data_loc)
if "Run" in args.config:
    b_list = open('BranchList2017Data.txt','r').read().split('\n')
elif "v4" in data_loc:
    b_list = open('BranchList2017MC_alt.txt','r').read().split('\n')
else:
    b_list = open('BranchList2017MC.txt','r').read().split('\n')
# print(b_list)
cut_dihadron.Snapshot("makeTopologyNtupleMiniAOD/tree",args.output,b_list)

entries_trig = cut_trig.Count()
entries_met= cut_met.Count()
entries_mu_sel= cut_mu_sel.Count()
entries_dimuon= cut_dimuon.Count()
entries_hadron_sel = cut_hadron_sel.Count()
entries_dihadron= cut_dihadron.Count()
# entries_test= cut_test.Count()

sys.stderr.write ('total:'+str(entries_total.GetValue())+'\n')
sys.stderr.write('After trigger cut:'+str(entries_trig.GetValue())+'\n')
sys.stderr.write('After MET filters:'+str(entries_met.GetValue())+'\n')
sys.stderr.write('After muon selection:'+str(entries_mu_sel.GetValue())+'\n')
sys.stderr.write('After dimuon cut:'+str(entries_dimuon.GetValue())+'\n')
sys.stderr.write('After hadron selection:'+str(entries_hadron_sel.GetValue())+'\n')
sys.stderr.write('After dihadron cut:'+str(entries_dihadron.GetValue())+'\n')
sys.stderr.flush()

sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')
sys.stderr.flush()




