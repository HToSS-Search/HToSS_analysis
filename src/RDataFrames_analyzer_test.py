# from audioop import cross
# from inspect import CORO_SUSPENDED
import numpy as np
from array import array
import os,argparse
import re
import yaml
import ROOT
import time
import sys
# from multiprocessing import Pool
# import uproot
from event_selection_helper import gInterpreter_std_map, gInterpreter_Indices, gInterpreter_Kinematics, gInterpreter_ChTrkSelection,\
	gInterpreter_PFIsolation, gInterpreter_diObjectCandidate, gInterpreter_diObjectMassCheck, gInterpreter_diObjectLxy, gInterpreter_lifetime_reweight
from sf_helper import get_pileup

################# FRAMEWORK TO ACCESS OLD UNSKIMMED FILES ####################


ROOT.gROOT.SetBatch(True)

cpu_count = 4 # give the same for request_cpus on condor script
# ROOT.ROOT.EnableImplicitMT(cpu_count) #comment it when testing

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return min(n),max(n)

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
parser.add_argument("--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
# parser.add_argument("-a", "--cuts", dest="cuts",   help="Enter cuts file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination directory", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=str)
parser.add_argument("--flow", dest="flow", help="start file number",default=1, type=int)
parser.add_argument("--fhigh", dest="fhigh", help="end file number", default=100,type=int)
parser.add_argument("--onlyweights", dest="onlyweights", help="just store MCweights weighted with cs*lumi", action='store_true')
parser.add_argument("--dname", dest="dname", help="stores the name of dataset - esp. needed for LLP reweighting", type=str)

# parser.add_argument("--total", dest="onlyweights", help="just store MCweights weighted with cs*lumi", action='store_true')

args = parser.parse_args()
###################### LOADING YAML FOR PHYSICS PROCESS ############################
print(args.dname)
fin = open(args.config,'r')
# f = open("params.txt",'w')
conf_pars = yaml.safe_load(fin)

data_loc = conf_pars['locations']
cross_section = 1 if 'Run' in args.config else conf_pars['cross_section']
sum_wts = 1 if 'Run' in args.config else conf_pars['sum_weights']
# lumi = 1 if 'Run' in args.config else 41474 #2017 for now
lumi = 1 if 'Run' in args.config else 4247.682053046 #2017D for now
isData = 'true' if 'Run' in args.config else 'false'

###################### LOADING ALL FILES FOR PROCESSING ############################

isOldNtuple = False
if 'almorton' in data_loc:
	isOldNtuple = True
# data_loc = data_loc[0]
if data_loc[-1] != '/':
	data_loc = data_loc+'/'
print(data_loc)
if isOldNtuple:
	directories = [data_loc] #below only for old ntuples
else:
	directories = [data_loc+d+"/" for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]

print(directories)
# data_name = conf_pars['name']
data_name = args.dname
treeName = "makeTopologyNtupleMiniAOD/tree"
list_of_files = []
for dir in directories:
	print(dir)
	flow, fhigh = maxfilenumber(dir)
	if flow > args.fhigh or fhigh < args.flow:
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
		try:
			root_file = ROOT.TFile.Open(fistr)
			if not root_file or root_file.IsZombie() or root_file.TestBit(ROOT.TFile.kRecovered):
				raise Exception(f"Error opening file: {fistr}")
			# else:
			# 	continue
		except Exception as e:
			print(f"Error processing file {fistr}: {e}")
			continue
		list_of_files.append(fistr)

# print(list_of_files)

###################### CALCULATION OF SUM OF WEIGHTS ####################
# if True:
# 	# sum_wts calculated here
# 	print("enters sum weights calculation")
# 	print(list_of_files)
# 	if not 'Run' in args.config:
# 		if not isinstance(list_of_files, list):
# 			file = ROOT.TFile(list_of_files)
# 			weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
# 			weightPlot.SetDirectory(0)
# 			file.Close()
# 		else:
# 			file = ROOT.TFile(list_of_files[0])
# 			weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
# 			weightPlot.SetDirectory(0)
# 			file.Close()
# 			for i,fistr in enumerate(list_of_files):
# 				if i==0:
# 					continue
# 				if not os.path.exists(fistr):
# 					continue
# 				try:
# 					# Open the ROOT file
# 					file = ROOT.TFile.Open(fistr)

# 					# Check if the file was opened successfully
# 					if not file or file.IsZombie() or file.TestBit(ROOT.TFile.kRecovered):
# 						raise Exception(f"Error opening file: {fistr}")

# 					# Process the file
# 					tmpPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
# 					# total_entries=total_entries+tmpPlot.GetEntries()
# 					weightPlot.Add(tmpPlot)
# 					# Close the file
# 					file.Close()
# 					# continue
# 				except Exception as e:
# 					print(f"Error processing file {fistr}: {e}")
# 					continue  # Continue to the next file in case of an error
# 		totalEvents_ = weightPlot.GetBinContent(1) - weightPlot.GetBinContent(2)
# 		sum_wts = totalEvents_
# 	sys.stderr.write("sum of weights:"+str(sum_wts)+"\n")
	# fout = ROOT.TFile(args.output,"RECREATE")
	# weightPlot.Write()
	# fout.Close()
	sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')
	# quit()


if not isinstance(list_of_files, list):
	file = ROOT.TFile(list_of_files)
	# cutPlot = file.Get("makeTopologyNtupleMiniAOD/eventFilterAND").Clone()
	weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
	eventPlot = file.Get("makeTopologyNtupleMiniAOD/eventcount").Clone()
	weightPlot.SetDirectory(0)
	# cutPlot.SetDirectory(0)
	eventPlot.SetDirectory(0)
	file.Close()
else:
	file = ROOT.TFile(list_of_files[0])
	# cutPlot = file.Get("makeTopologyNtupleMiniAOD/eventFilterAND").Clone()
	weightPlot = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
	eventPlot = file.Get("makeTopologyNtupleMiniAOD/eventcount").Clone()
	weightPlot.SetDirectory(0)
	# cutPlot.SetDirectory(0)
	eventPlot.SetDirectory(0)
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
			# tmpPlot = file.Get("makeTopologyNtupleMiniAOD/eventFilterAND").Clone()
			# total_entries=total_entries+tmpPlot.GetEntries()
			# cutPlot.Add(tmpPlot)
			tmpPlot2 = file.Get("makeTopologyNtupleMiniAOD/weightHisto").Clone()
			weightPlot.Add(tmpPlot2)
			tmpPlot3 = file.Get("makeTopologyNtupleMiniAOD/eventcount").Clone()
			eventPlot.Add(tmpPlot3)
			# Close the file
			file.Close()
			# continue
		except Exception as e:
			print(f"Error processing file {fistr}: {e}")
			continue  # Continue to the next file in case of an error

###################### LOADING YAML FOR CUTS ############################
fcuts = open(args.cuts,'r')
# f = open("params.txt",'w')
fcut_pars = yaml.safe_load(fcuts)

cut_pars = fcut_pars['cuts']
if fcut_pars['hadronType'] == "kaon":
	chsMass_ = 0.493677 # 0.13957061;//pion mass
else:
	chsMass_ = 0.13957061
if '2017' in args.year:
	singlemu_trig_cut = 'HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13>0 || HLT_IsoMu27_v14 > 0'
else:
	singlemu_trig_cut = 'HLT_IsoMu24_v > 0'
if '2018' in args.year:
	l2mu_trig_cut = '(HLT_DoubleL2Mu23NoVtx_2Cha_v > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed_v > 0) && HLT_IsoMu24_v <= 0 '
else:
	l2mu_trig_cut = 'false'
if fcut_pars['trig']=='single':
	if '2017' in args.year:
		trigger_cuts = 'HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13>0 || HLT_IsoMu27_v14 > 0' # all ntuples are now new
	else:
		trigger_cuts = 'HLT_IsoMu24_v > 0' # all ntuples are now new
elif fcut_pars['trig']=='double':
	if '2018' in args.year:
		trigger_cuts = '(HLT_DoubleL2Mu23NoVtx_2Cha_v > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed_v > 0) && HLT_IsoMu24_v <= 0 ' # all ntuples are now new
	else:
		print('which dimuon triggers?')
		quit()
elif fcut_pars['trig']=='all':
	if '2016' in args.year:
		trigger_cuts = 'HLT_IsoMu24_v > 0' # all ntuples are now new
	elif '2017' in args.year:
		trigger_cuts = 'HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13>0 || HLT_IsoMu27_v14 > 0' # all ntuples are now new
	else:
		trigger_cuts = 'HLT_IsoMu24_v > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_v > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_CosmicSeed_v > 0' # all ntuples are now new
else:
	print('Mention which triggers to use')
	quit()


met_filter_cuts = 'Flag_goodVertices > 0 && Flag_globalTightHalo2016Filter > 0 && Flag_HBHENoiseFilter > 0 && Flag_HBHENoiseIsoFilter > 0 && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 && Flag_BadPFMuonFilter > 0 && Flag_ecalBadCalibFilter > 0'

mu_cuts = cut_pars['muons']
ch_cuts=cut_pars['hadrons']
dimuon_cuts=cut_pars['dimuon']
dihadron_cuts=cut_pars['dihadron']
recohiggs_cuts=cut_pars['recohiggs']
recoscalar_cuts=cut_pars['recoscalar']

# singlemu_pt, singlemu_ptLeading = mu_cuts['pt']['single'],mu_cuts['ptLeading']['single']
# doublemu_pt, doublemu_ptLeading = mu_cuts['pt']['double'],mu_cuts['ptLeading']['double']


if ch_cuts['id']=='None':
	ch_trk_cuts_part1 = 'abs(packedCandsPdgId)==211 && packedCandsCharge!=0 && packedCandsHasTrackDetails==1'
else:
	ch_trk_cuts_part1=''
ch_trk_cuts_part2 = 'lv_trk_pt > '+str(ch_cuts['pt'])+' && ' + 'abs(lv_trk_eta) < ' + str(ch_cuts['eta'])
nch = 'LVs_sel.size()>='+str(ch_cuts['number'])

diMuPt_ = str(dimuon_cuts['pt'])
diMudR_ = str(dimuon_cuts['dR'])
diMuFlag_ = dimuon_cuts['flag']
diChPt_ = str(dihadron_cuts['pt'])
diChdR_ = str(dihadron_cuts['dR'])
diChFlag_ = dihadron_cuts['flag']

subleadingMuRelIso_ = str(dimuon_cuts['category']['prompt']['reliso'])
leadingMuRelIso_ = str(dimuon_cuts['category']['prompt']['relisoLeading'])
subleadingChRelIso_ = str(dihadron_cuts['category']['prompt']['reliso']) 
leadingChRelIso_ = str(dihadron_cuts['category']['prompt']['relisoLeading'])

mu_isocuts = 'mu1_iso[0] < ' + leadingMuRelIso_ + ' && ' + 'mu2_iso[0] < ' + subleadingMuRelIso_
ch_isocuts = 'ch1_iso[0] < ' + leadingChRelIso_ + ' && ' + 'ch2_iso[0] < ' + subleadingChRelIso_
mu_isocuts_loose = 'mu1_iso[0] < ' + str(dihadron_cuts['category']['displaced']['relisoLeading'])
ch_isocuts_loose = 'ch1_iso[0] < ' + str(dihadron_cuts['category']['displaced']['relisoLeading'])



mumuhh_mass_low = recohiggs_cuts['massLow']
mumuhh_mass_high = recohiggs_cuts['massHigh']
unblind = recohiggs_cuts['unblind']
sideband = recohiggs_cuts['sideband']

hmass_low,hmass_high = 110,140

# higgs mass cut is always there; when flag is true, use tight mass conditions else use loose
higgs_mass_peak = 'recohiggs_mass >='+str(mumuhh_mass_low)+' && '+'recohiggs_mass <='+str(mumuhh_mass_high)
higgs_mass_peak_blinded = 'recohiggs_mass <'+str(mumuhh_mass_low)+' || '+'recohiggs_mass >'+str(mumuhh_mass_high)
higgs_mass_peak_loose = f'recohiggs_mass >{hmass_low}'+' && '+f'recohiggs_mass <{hmass_high}'
higgs_mass_peak_tight = f'recohiggs_mass > {mumuhh_mass_low-sideband}'+' && '+f'recohiggs_mass < {mumuhh_mass_high+sideband}'
# higgs_mass_peak is tighter than higgs_mass_peak_tight
higgs_mass_cuts = higgs_mass_peak_tight
higgs_mass_cuts_loose = higgs_mass_peak_loose

# scalar mass cut is always there; when flag is true, use tight mass conditions else use loose

scalar_mass_cuts = 'hh_lv.M() <= 5 && mumu_lv.M() <= 5'
params_lower = ROOT.VecOps.RVec('double')(recoscalar_cuts['lowerbound'])
params_higher = ROOT.VecOps.RVec('double')(recoscalar_cuts['higherbound'])
# recoscalar_flag = recoscalar_cuts['flag']

####### scalar mass assumption -> hard coded for now -> shift this to yaml script eventually (if needed) ########
ma_mean, ma_sigma, ma_min, ma_max = 2, 0.01364, 1.98636, 2.01364 #taking 2*sigma = 95%
scalar_mass_assumption_cuts = f"avgmass >= {ma_min} && avgmass <= {ma_max}"

#### Splitting into regions #####
mumulxysign,hhlxysign = fcut_pars['regions']['mumulxysign'],fcut_pars['regions']['hhlxysign']
R1 = f'mumu_lxysign_tmp <= {mumulxysign} && hh_lxysign_tmp <= {hhlxysign}'
R2mumu = f'mumu_lxysign_tmp > {mumulxysign} && hh_lxysign_tmp <= {hhlxysign}'
R2hh = f'mumu_lxysign_tmp <= {mumulxysign} && hh_lxysign_tmp > {hhlxysign}'
R3 = f'mumu_lxysign_tmp > {mumulxysign} && hh_lxysign_tmp > {hhlxysign}'

print('Regions:')
print(R1)
print(R2mumu)
print(R2hh)
print(R3)
print('Conditions:')
print(trigger_cuts)
print(met_filter_cuts)
# print(muon_cuts)
# print(leadingmu_cut)
# print(nmu)
print(mu_isocuts)
print(ch_trk_cuts_part1)
print(ch_trk_cuts_part2)
print(ch_isocuts)
if isData=="false":
	print(higgs_mass_peak)
else:
	print(higgs_mass_peak_blinded)
print(higgs_mass_cuts_loose)
print(higgs_mass_cuts)
print(scalar_mass_cuts)
print(params_lower)
print(params_higher)
print(scalar_mass_assumption_cuts)
# quit()

# calling C++ functions needed for event selection, below
gInterpreter_Indices()
gInterpreter_Kinematics()
gInterpreter_ChTrkSelection()
gInterpreter_PFIsolation()
gInterpreter_diObjectCandidate()
gInterpreter_diObjectMassCheck()
gInterpreter_diObjectLxy()
gInterpreter_lifetime_reweight()

rdf = ROOT.RDataFrame(treeName,list_of_files)
# print(data_loc)
entries_total = rdf.Count()
sys.stderr.write("entries total:"+str(entries_total.GetValue())+'\n')
sys.stderr.flush()
sum_wts=rdf.Sum('processMCWeight').GetValue()
print(sum_wts)
dataset_weight = cross_section/sum_wts
if (isData=='true'):
	rdf_new = rdf.Define('weight','1')
else:
	gInterpreter_std_map() # Accesses c++ map
	pileup_ratio,pileup_edges = get_pileup(args.year)
	pileupMap = {e: r for e, r in zip(pileup_edges[:-1], pileup_ratio)}
	pileupMap_c = ROOT.GetTheMap()
	for key in pileupMap:
		pileupMap_c[key]=pileupMap[key]
	rdf_new = rdf.Define('weightOnlyDataset','processMCWeight*{}'.format(dataset_weight))
	rdf_new = rdf_new.Define('PUReweight_sf','GetTheMap()[floor(numVert)]')
	rdf_new = rdf_new.Define('weight_tmp','weightOnlyDataset*PUReweight_sf')
	# print(pileupMap)
	# cols = ROOT.vector('string')(); cols.push_back("PUReweight_sf"); cols.push_back("numVert"); cols.push_back("weightOnlyDataset"); cols.push_back("weight")
	# rdf_new.Display(cols,10).Print()
	# quit()

	std_lt=[0,1,10,100]

	########## LLP lifetime reweighting #########
	if ('HToSS' in args.config):
		mass=float(data_name.split('MS')[-1].split('_')[0].replace('p','.'))
		scalar1_index = 'genParId==9000006'
		scalar2_index = 'genParId==-9000006'
		old_lt = float(data_loc.split('ctauS')[1].split('_')[0].replace('p','.'))
		if old_lt==0:
			old_lt=0.1
		# new_lt = float(data_name.split('ctauS')[1].replace('p','.'))
		print(data_name)
		new_lt = float(data_name.split('_')[-1].replace('ctauS','').replace('p','.'))

		if new_lt==0:
			new_lt=0.1
		print (old_lt,new_lt)
		# quit()
		###### status codes - 22: intermediate particles of the hardest subprocess, 
		######                44: particles produced by initial-state-showers; outgoing shifted by branching
		######                62: particles produced by beam-remnant treatment; outgoing subprocess particle with primordial kT included
		genhiggs_cut='genParId==25 && genParStatus[genParId==25]==62'
		rdf_new = rdf_new.Define('genHiggsStatus','genParStatus[genParId==25]').Define('genHiggsPt_tmp','genParPt[genParId==25]')\
						.Define('genHiggsPt','genHiggsPt_tmp[genHiggsStatus==22]')\
						.Define('scalar1_dau1_idx',f'genParDaughter1Index[{scalar1_index}]')\
						.Define('scalar2_dau1_idx',f'genParDaughter1Index[{scalar2_index}]')\
						.Define('scalar1_Vx',f'genParVx[{scalar1_index}]')\
						.Define('dau1_Vx','genParVx[scalar1_dau1_idx[0]]')\
						.Define('scalar1_Vy',f'genParVy[{scalar1_index}]')\
						.Define('dau1_Vy','genParVy[scalar1_dau1_idx[0]]')\
						.Define('scalar1_Vz',f'genParVz[{scalar1_index}]')\
						.Define('dau1_Vz','genParVz[scalar1_dau1_idx[0]]')\
						.Define('scalar1_dVx',f'genParVx[{scalar1_index}] - genParVx[scalar1_dau1_idx[0]]')\
						.Define('scalar1_dVy',f'genParVy[{scalar1_index}] - genParVy[scalar1_dau1_idx[0]]')\
						.Define('scalar1_dVz',f'genParVz[{scalar1_index}] - genParVz[scalar1_dau1_idx[0]]')\
						.Define('scalar2_dVx',f'genParVx[{scalar2_index}] - genParVx[scalar2_dau1_idx[0]]')\
						.Define('scalar2_dVy',f'genParVy[{scalar2_index}] - genParVy[scalar2_dau1_idx[0]]')\
						.Define('scalar2_dVz',f'genParVz[{scalar2_index}] - genParVz[scalar2_dau1_idx[0]]')\
						.Define('scalar1_genlxy',f'sqrt(scalar1_dVx*scalar1_dVx + scalar1_dVy*scalar1_dVy)')\
						.Define('scalar2_genlxy',f'sqrt(scalar2_dVx*scalar2_dVx + scalar2_dVy*scalar2_dVy)')\
						.Define('scalar1_dist',f'sqrt(scalar1_dVx*scalar1_dVx + scalar1_dVy*scalar1_dVy + scalar1_dVz*scalar1_dVz)')\
						.Define('scalar2_dist',f'sqrt(scalar2_dVx*scalar2_dVx + scalar2_dVy*scalar2_dVy + scalar2_dVz*scalar2_dVz)')\
						.Define('scalar1_pt',f'genParPt[{scalar1_index}]').Define('scalar1_eta',f'genParEta[{scalar1_index}]').Define('scalar1_phi',f'genParPhi[{scalar1_index}]').Define('scalar1_E',f'genParE[{scalar1_index}]')\
						.Define('scalar2_pt',f'genParPt[{scalar2_index}]').Define('scalar2_eta',f'genParEta[{scalar2_index}]').Define('scalar2_phi',f'genParPhi[{scalar2_index}]').Define('scalar2_E',f'genParE[{scalar2_index}]')\
						.Define('scalar1_vec',f'ROOT::Math::PtEtaPhiMVector(scalar1_pt[0], scalar1_eta[0], scalar1_phi[0], {mass})')\
						.Define('scalar2_vec',f'ROOT::Math::PtEtaPhiMVector(scalar2_pt[0], scalar2_eta[0], scalar2_phi[0], {mass})')\
						.Define('scalar1_mass','scalar1_vec.M()')\
						.Define('scalar2_mass','scalar2_vec.M()')\
						.Define('lifetime_weight',f'getLifetimeReweight({old_lt},{new_lt}, scalar1_dist[0], scalar2_dist[0], scalar1_vec, scalar2_vec, {mass})')
		if data_name.count('ctauS') == 1:
			rdf_new = rdf_new.Define('weight','weightOnlyDataset')
			rdf_new = rdf_new.Define('weight_lt','processMCWeight')
		else:
			print("ENTERS REWEIGHTING!")
			
			# sum_wts_new = sum_wts/eventPlot.Integral()*(old_lt**4/(new_lt-old_lt)**2) # lifetime weight integral only valid when old_lt > new_lt
			#since old samples (where sum_wts is not stored) - use this:
			entries_check = rdf_new.Count().GetValue()
			sum_wts_new_test = sum_wts/entries_check*(old_lt**4/(new_lt-old_lt)**2) # lifetime weight integral only valid when old_lt > new_lt
			sum_wts_new = sum_wts/entries_check*rdf_new.Sum('lifetime_weight').GetValue() # lifetime weight integral only valid when old_lt > new_lt
			print("check this-",sum_wts,eventPlot.Integral(),eventPlot.GetEntries(), entries_check,sum_wts/eventPlot.Integral(),rdf_new.Sum('processMCWeight').GetValue())
			# sum_wts_new = rdf_new.Define('weight_lt','processMCWeight*lifetime_weight').Sum('weight_lt')
			rdf_new = rdf_new.Define('weight',f'processMCWeight*{cross_section}*lifetime_weight/{sum_wts_new}')
			rdf_new = rdf_new.Define('weight_lt',f'processMCWeight*lifetime_weight')
			print(rdf_new.Sum('weight_lt').GetValue(),sum_wts_new_test,sum_wts_new,sum_wts/entries_check*rdf_new.Sum('lifetime_weight').GetValue())
		
		
		# rdf_test = rdf_new.Range(0,10,1)
		# rdf_test.Display({'weight','genHiggsPt_tmp','genHiggsPt','genHiggsStatus'}).Print()
		# quit()

			# NOTE: doesn't matter if daughter 1 or daughter 2 used; This is tested with dVx using 2 different daughters
	# 		rdf_test.Display({'weight','weight_tmp','lifetime_weight','scalar1_dist','scalar2_dist'},20).Print()
	# 		rdf_test.Display({'scalar1_Vx','dau1_Vx','scalar1_Vy','dau1_Vy','scalar1_Vz','dau1_Vz','scalar1_dist'},20).Print()
	# 		rdf_test.Display({'scalar1_mass','scalar2_mass'},20).Print()
	# 		sum_wts_small = rdf_new.Sum('processMCWeight').GetValue()
	# 		totentries_small = entries_total.GetValue()
	# 		print(sum_wts_small)
	# 		print()
	# 		print(sum_wts_small/)
	# 		print(sum_wts)
	# 		quit()
	else:
		rdf_new = rdf_new.Define('weight','weight_tmp')

		
###########################################
# rdf_new = rdf_new.Range(0,30,1)
del rdf
# if '2017' in args.year:
# 	trigger_definitions = rdf_new.Define('singlemu_trig',singlemu_trig_cut_2017)
# else:
# 	trigger_definitions = rdf_new.Define('singlemu_trig',singlemu_trig_cut)
# 	trigger_definitions = trigger_definitions.Define('l2mu_trig',l2mu_trig_cut)
# trigger_definitions = rdf_new.Define('singlemu_trig',singlemu_trig_cut)
# trigger_definitions = rdf_new.Define('mu_trig',)

# singlemu_trig_cut
# l2mu_trig_cut)

# singlemu_trig_cut
# l2mu_trig_cut
trigger_definitions = rdf_new.Define('mu_trig',trigger_cuts)\
				.Define('singlemu_trig_cut',singlemu_trig_cut)\
				.Define('l2mu_trig_cut',l2mu_trig_cut)\
				.Define('met_filters', met_filter_cuts)

############## TRIGGERS + MET FILTERS ########################
if fcut_pars['trig']=='single':
	cut_trig = trigger_definitions.Filter('singlemu_trig_cut','Trigger')
elif fcut_pars['trig']=='double':
	cut_trig = trigger_definitions.Filter('l2mu_trig_cut','Trigger')
elif fcut_pars['trig']=='all':
	cut_trig = trigger_definitions.Filter('mu_trig','Trigger')

cut_met = cut_trig.Filter('met_filters','MET filter')

############## MUON SEL + DIMUON ########################
single_muon_cuts = singlemu_trig_cut+' && '+'abs(muonPF2PATEta) < '+ str(mu_cuts['eta']) + ' && ' + 'muonPF2PATIsPFMuon' + ' && ' + 'muonPF2PAT'+mu_cuts['id']+'CutId'+' && ' + 'muonPF2PATPt > ' + str(mu_cuts['pt']['single'])
single_leadingmu_cut = 'Max(mu_pt) > '+str(mu_cuts['ptLeading']['single'])
single_nmu = 'mu_pt.size()>=' + str(mu_cuts['number']) + ' && ' + single_leadingmu_cut

if '2018' in args.year:
	l2mu_muon_cuts = l2mu_trig_cut+' && '+'abs(muonPF2PATEta) < '+ str(mu_cuts['eta']) + ' && ' + 'muonPF2PATIsPFMuon' + ' && ' + 'muonPF2PAT'+mu_cuts['id']+'CutId'+' && ' + 'muonPF2PATPt > ' + str(mu_cuts['pt']['double'])
	l2mu_leadingmu_cut = 'Max(mu_pt) > '+str(mu_cuts['ptLeading']['double'])
	l2mu_nmu = 'mu_pt.size()>=' + str(mu_cuts['number']) + ' && ' + l2mu_leadingmu_cut
else:
	l2mu_muon_cuts = 'false'
	l2mu_leadingmu_cut = '0'
	l2mu_nmu = 'false'
if fcut_pars['trig']=='single':
	muon_cuts = single_muon_cuts
	nmu = single_nmu
elif fcut_pars['trig']=='double':
	muon_cuts = l2mu_muon_cuts
	nmu = l2mu_nmu
elif fcut_pars['trig']=='all':
	muon_cuts = '('+single_muon_cuts+')' + ' || ' + '(' + l2mu_muon_cuts+ ')'
	nmu = '(' + single_nmu +')' + ' || ' + '(' + l2mu_nmu+ ')'


muon_definitions = cut_met.Define('mu_pt',f'muonPF2PATPt[{muon_cuts}]')\
				.Define('mu_sel',nmu)
cut_mu_sel = muon_definitions.Filter('mu_sel','mu selection')

dimuon_definitions = cut_mu_sel.Define('mu_globalidx',f'getIndices(numMuonPF2PAT)[{muon_cuts}]')\
			.Define('mu_pt_sorted_globalidx_nocuts','ROOT::VecOps::Reverse(ROOT::VecOps::Argsort(muonPF2PATPt))')\
			.Define('mu_pt_sorted_globalidx','ROOT::VecOps::Intersect(mu_pt_sorted_globalidx_nocuts,mu_globalidx,false)')\
			.Define('mu_charge',f'muonPF2PATCharge[{muon_cuts}]')\
			.Define('mu1_lv','ROOT::Math::PxPyPzMVector(0.,0.,0.,0.)')\
			.Define('mu2_lv','ROOT::Math::PxPyPzMVector(0.,0.,0.,0.)')\
			.Define('mu_pair_idx',f'''getDileptonCand(mu_pt_sorted_globalidx, muonPF2PATCharge, muonPF2PATPX, muonPF2PATPY, muonPF2PATPZ, 0.1056583745, 
		   numMuonTrackPairsPF2PAT,muonTkPairPF2PATIndex1,muonTkPairPF2PATIndex2,muonTkPairPF2PATTk1Px,muonTkPairPF2PATTk1Py,muonTkPairPF2PATTk1Pz,
		   muonTkPairPF2PATTk2Px,muonTkPairPF2PATTk2Py,muonTkPairPF2PATTk2Pz, "muon", packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, 
           packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 
		   mu1_lv, mu2_lv, {diMuPt_},{diMudR_})''')\
		    .Define('mu_pair_check','mu_pair_idx[0]>=0 && mu_pair_idx[1]>=0')
#note: mu_pair_idx has 3 things - leading, subleading & track pair index
#note: getDileptonCand does not need packed cands anymore because PFIsolation is not calculated there anymore!!! Make the modification eventually
############## HADRON SEL + DIHADRON SEL########################
cut_dimuon = dimuon_definitions.Filter('mu_pair_check','dimuon candidate')
hadron_definitions = cut_dimuon.Define('mumu_lv', 'mu1_lv + mu2_lv')\
			.Define('cand_globalidx','getIndices(numPackedCands)')\
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
			.Define('ch_trk_sel','LVs_sel.size()>=2')

cut_hadron_sel = hadron_definitions.Filter('ch_trk_sel','hadron selection')
dihadron_definitions = cut_hadron_sel.Define('cand_globalidx_hadronsonly',f'getIndices(numPackedCands)[{ch_trk_cuts_part1}]')\
						.Define('ch_globalidx',f'cand_globalidx_hadronsonly[{ch_trk_cuts_part2}]')\
						.Define('ch1_lv','ROOT::Math::PxPyPzMVector(0.,0.,0.,0.)')\
						.Define('ch2_lv','ROOT::Math::PxPyPzMVector(0.,0.,0.,0.)')\
						.Define('ch_pair_idx',f'''getDileptonCand(ch_globalidx, packedCandsCharge, packedCandsPx, packedCandsPy, packedCandsPz, {chsMass_},  
			  			numChsTrackPairs,chsTkPairIndex1,chsTkPairIndex2,chsTkPairTk1Px,chsTkPairTk1Py,chsTkPairTk1Pz,chsTkPairTk2Px,chsTkPairTk2Py,chsTkPairTk2Pz,
			  			"hadron", packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, 
						packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 
			  			ch1_lv, ch2_lv, {diChPt_}, {diChdR_})''')\
						.Define('ch_pair_check','ch_pair_idx[0]>=0 && ch_pair_idx[1]>=0')
cut_dihadron = dihadron_definitions.Filter('ch_pair_check','dihadron candidate')
cut_dihadron = cut_dihadron.Define('hh_lv','ch1_lv+ch2_lv')\
						.Define('mumu_dR_tmp','ROOT::Math::VectorUtil::DeltaR(mu1_lv,mu2_lv)')\
						.Define('hh_dR_tmp','ROOT::Math::VectorUtil::DeltaR(ch1_lv,ch2_lv)')\
						.Define('mu1_nhits_tmp','muonPF2PATTrackNHits[mu_pair_idx[0]]')\
						.Define('mu2_nhits_tmp','muonPF2PATTrackNHits[mu_pair_idx[1]]')\
						.Define('ch1_nhits_tmp','packedCandsPseudoTrkNumberOfHits[ch_pair_idx[0]]')\
						.Define('ch2_nhits_tmp','packedCandsPseudoTrkNumberOfHits[ch_pair_idx[1]]')\
						.Define('mu1_pixhits_tmp','muonPF2PATVldPixHits[mu_pair_idx[0]]')\
						.Define('mu2_pixhits_tmp','muonPF2PATVldPixHits[mu_pair_idx[1]]')
						# .Define('mu1_dz_tmp','muonPF2PATDZPV[mu_pair_idx[0]]')\
						# .Define('mu2_dz_tmp','muonPF2PATDZPV[mu_pair_idx[1]]')\
						# .Define('ch1_dz_tmp','packedCandsDz[ch_pair_idx[0]]')\
						# .Define('ch2_dz_tmp','packedCandsDz[ch_pair_idx[1]]')\
						# .Define('mu1_vz_tmp','muonPF2PATVertZ[mu_pair_idx[0]]')\
						# .Define('mu2_vz_tmp','muonPF2PATVertZ[mu_pair_idx[1]]')\
						# .Define('ch1_vz_tmp','packedCandsVz[ch_pair_idx[0]]')\
						# .Define('ch2_vz_tmp','packedCandsVz[ch_pair_idx[1]]')\

#### checking ntuple version ### 
date = int(data_loc.split('/')[-2].split('_')[0])
# print(date)
# quit()
if date < 240100:
	cut_dihadron = cut_dihadron.Define('ch1_pixhits_tmp','packedCandsPseudoTrkNumberOfPixelHits.packedCandsPseudoTrkNumberOfHits[ch_pair_idx[0]]')\
						.Define('ch2_pixhits_tmp','packedCandsPseudoTrkNumberOfPixelHits.packedCandsPseudoTrkNumberOfHits[ch_pair_idx[1]]')
else:
	cut_dihadron = cut_dihadron.Define('ch1_pixhits_tmp','packedCandsPseudoTrkNumberOfPixelHits[ch_pair_idx[0]]')\
						.Define('ch2_pixhits_tmp','packedCandsPseudoTrkNumberOfPixelHits[ch_pair_idx[1]]')
############### CAREFUL - NAMING CONVENTION FIXED FOR HITS IN NEW NTUPLES (2024) ###################
############## LXY CALCULATION FOR SCALARS ########################
pv_sel = 'pvChi2!=0 && pvNdof!=0'
vtx_definitions = cut_dihadron.Define('PVCov00',f'pvCov00[{pv_sel}][0]').Define('PVCov01',f'pvCov01[{pv_sel}][0]').Define('PVCov02',f'pvCov02[{pv_sel}][0]').Define('PVCov10',f'pvCov10[{pv_sel}][0]').Define('PVCov11',f'pvCov11[{pv_sel}][0]').Define('PVCov12',f'pvCov12[{pv_sel}][0]').Define('PVCov20',f'pvCov20[{pv_sel}][0]').Define('PVCov21',f'pvCov21[{pv_sel}][0]').Define('PVCov22',f'pvCov22[{pv_sel}][0]').Define('PVX',f'pvX[{pv_sel}][0]').Define('PVY',f'pvY[{pv_sel}][0]').Define('PVZ',f'pvZ[{pv_sel}][0]')\
						.Define('mumu_lxyInfo','''getLxy(PVCov00,PVCov01,PVCov02,PVCov10,PVCov11,PVCov12,PVCov20,PVCov21,PVCov22,PVX,PVY,
			  							muonTkPairPF2PATTkVtxCov00[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov01[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov02[mu_pair_idx[2]],
			  							muonTkPairPF2PATTkVtxCov10[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov11[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov12[mu_pair_idx[2]],
			  							muonTkPairPF2PATTkVtxCov20[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov21[mu_pair_idx[2]],muonTkPairPF2PATTkVtxCov22[mu_pair_idx[2]],
			  							muonTkPairPF2PATTkVx[mu_pair_idx[2]],muonTkPairPF2PATTkVy[mu_pair_idx[2]])''')\
						.Define('hh_lxyInfo','''getLxy(PVCov00,PVCov01,PVCov02,PVCov10,PVCov11,PVCov12,PVCov20,PVCov21,PVCov22,PVX,PVY,
			  							chsTkPairTkVtxCov00[ch_pair_idx[2]],chsTkPairTkVtxCov01[ch_pair_idx[2]],chsTkPairTkVtxCov02[ch_pair_idx[2]],
			  							chsTkPairTkVtxCov10[ch_pair_idx[2]],chsTkPairTkVtxCov11[ch_pair_idx[2]],chsTkPairTkVtxCov12[ch_pair_idx[2]],
			  							chsTkPairTkVtxCov20[ch_pair_idx[2]],chsTkPairTkVtxCov21[ch_pair_idx[2]],chsTkPairTkVtxCov22[ch_pair_idx[2]],
			  							chsTkPairTkVx[ch_pair_idx[2]],chsTkPairTkVy[ch_pair_idx[2]])''')\
						.Define('mumu_lxy_vector','ROOT::Math::XYVector(muonTkPairPF2PATTkVx[ch_pair_idx[2]] - PVX, muonTkPairPF2PATTkVy[ch_pair_idx[2]] - PVY)')\
						.Define('hh_lxy_vector','ROOT::Math::XYVector(chsTkPairTkVx[ch_pair_idx[2]] - PVX, chsTkPairTkVy[ch_pair_idx[2]] - PVY)')\
						.Define('pt_dphi','ROOT::Math::VectorUtil::DeltaPhi(mumu_lv,hh_lv)')\
						.Define('lxy_dphi','ROOT::Math::VectorUtil::DeltaPhi(mumu_lxy_vector,hh_lxy_vector)')\
						.Define('mumu_vz_tmp','muonTkPairPF2PATTkVz[mu_pair_idx[2]]')\
						.Define('mumu_chi2_tmp','muonTkPairPF2PATTkVtxChi2[mu_pair_idx[2]]')\
						.Define('mumu_ndof_tmp','muonTkPairPF2PATTkVtxNdof[mu_pair_idx[2]]')\
						.Define('mumu_dz_tmp','muonTkPairPF2PATTkVz[mu_pair_idx[2]] - PVZ')\
						.Define('hh_vz_tmp','chsTkPairTkVz[ch_pair_idx[2]]')\
						.Define('hh_chi2_tmp','chsTkPairTkVtxChi2[ch_pair_idx[2]]')\
						.Define('hh_ndof_tmp','chsTkPairTkVtxNdof[ch_pair_idx[2]]')\
						.Define('hh_dz_tmp','chsTkPairTkVz[ch_pair_idx[2]] - PVZ')\
						.Define('mumu_chi2ndof_tmp','mumu_chi2_tmp/mumu_ndof_tmp')\
						.Define('hh_chi2ndof_tmp','hh_chi2_tmp/hh_ndof_tmp')\
						.Define('mumu_nhits_tmp','abs(mu2_nhits_tmp-mu1_nhits_tmp)')\
						.Define('hh_nhits_tmp','abs(ch2_nhits_tmp-ch1_nhits_tmp)')\
						.Define('mumu_pixhits_tmp','abs(mu2_pixhits_tmp-mu1_pixhits_tmp)')\
						.Define('hh_pixhits_tmp','abs(ch2_pixhits_tmp-ch1_pixhits_tmp)')\
						.Define('mumu_lxysign_tmp','mumu_lxyInfo[2]')\
						.Define('hh_lxysign_tmp','hh_lxyInfo[2]')\
						.Define('mumu_sim_val','mumu_lxyInfo[3]')\
						.Define('hh_sim_val','hh_lxyInfo[3]')



############## RECO SCALAR MASS CHECK ########################
mass_definitions = vtx_definitions.Define('recoscalar_mass_check', scalar_mass_cuts)\
					.Define('mass_window_check',f'MassCompatibility(hh_lv.M(), mumu_lv.M(), {params_lower}, {params_higher})')
mass_definitions = mass_definitions.Filter('recoscalar_mass_check', 'm(mumu)<=5 && m(hh)<=5')

############## RECO HIGGS MASS CHECK ########################
higgs_definitions_preblinding = mass_definitions.Define('recohiggs_lv','mu1_lv + mu2_lv + ch1_lv + ch2_lv')\
					.Define('recohiggs_mass', 'recohiggs_lv.M()')\
					.Define('recohiggs_mass_peak',higgs_mass_peak)\
					.Define('recohiggs_mass_blinded',higgs_mass_peak_blinded)\
					.Define('recohiggs_mass_check_loose', higgs_mass_cuts_loose)\
					.Define('recohiggs_mass_check', higgs_mass_cuts)

############### MAKE CHANGES BELOW TO BLIND THE PEAK ACC TO DATA or MC; MAKE THE CUTS AFTER #########################
# if (unblind):
# 	if isData=='true':
# 		print("MANUALLY UNBLIND THE PEAK IN DATA - COMMENT OUT ABOVE")
# 		quit()
# 	else:
# 		higgs_definitions = higgs_definitions_preblinding
# else:
# 	higgs_definitions = higgs_definitions_blinded

# higgs_definitions_loose = higgs_definitions_blinded.Filter('recohiggs_mass_check_loose',f'm(mumuhh) in [{hmass_low},{hmass_high}]')
higgs_definitions_preblinding_loose = higgs_definitions_preblinding.Filter('recohiggs_mass_check_loose',f'm(mumuhh) in [{hmass_low},{hmass_high}]')
#### add loose isolation cuts? these would then go into displaced categories too so use LOOSE AF ####
higgs_definitions_preblinding_loose = higgs_definitions_preblinding_loose.Define('mu1_iso','''PFIsolation("muon", mu1_lv, mu_pair_idx[0], mu_pair_idx[1], packedCandsPx, packedCandsPy, packedCandsPz,packedCandsE, 
							packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4)''')\
							.Define('mu2_iso','''PFIsolation("muon", mu2_lv, mu_pair_idx[1], mu_pair_idx[0], packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, 
							packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4)''')\
							.Define('ch1_iso','''PFIsolation("hadron", ch1_lv, ch_pair_idx[0], ch_pair_idx[1], packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, 
							packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4)''')\
							.Define('ch2_iso','''PFIsolation("hadron", ch2_lv, ch_pair_idx[1], ch_pair_idx[0], packedCandsPx, packedCandsPy, packedCandsPz, packedCandsE, 
							packedCandsCharge, packedCandsPdgId, packedCandsFromPV, numPackedCands, 0.4)''')\
							.Define('ch_iso_check_loose',ch_isocuts_loose)\
							.Define('mu_iso_check_loose',mu_isocuts_loose)

#### Remove unphysical events with incorrect covariance matrices!!! ####
# rdf_test = higgs_definitions_preblinding_loose.Range(0,5000,1)
# rdf_testing = rdf_test.Filter('mumu_sim_val < 0 || hh_sim_val < 0')
# print(rdf_test.Count().GetValue(),rdf_testing.Count().GetValue())

# rdf_test.Display({'weight','weight_tmp','lifetime_weight','scalar1_dist','scalar2_dist'},20).Print()
# quit()

higgs_definitions_preblinding_loose = higgs_definitions_preblinding_loose.Filter('mumu_sim_val > 0 && hh_sim_val > 0')

####### adding loose isolation ##########
# higgs_definitions_preblinding_loose_iso = higgs_definitions_preblinding_loose.Filter('ch_iso_check_loose').Filter('mu_iso_check_loose')
higgs_definitions_preblinding_loose_iso = higgs_definitions_preblinding_loose
# print('SEE THIS:',higgs_definitions_preblinding_loose.Filter('ch1_iso[0] > 1' ).Sum('weight').GetValue())
# quit()

######## ADD ALL DEFINE'S HERE! ENSURE LEAST FILTER & DEFINE IN LOOP ###############
######## OR THE MEMORY BLOWS UP!!! #################################################
objects = {'mu1':'leadingMuon', 'mu2':'subleadingMuon', 'ch1':'leadingChHad', 'ch2':'subleadingChHad'}
kinematics = {
	'pt':{'var':'lv.Pt()','nameSuf':'Pt','titleSuf':'','nbins':5000,'minX':0.,'maxX':150.},
	'eta':{'var':'lv.Eta()','nameSuf':'Eta','titleSuf':'','nbins':5000,'minX':-3.,'maxX':3.},
	'phi':{'var':'lv.Phi()','nameSuf':'Phi','titleSuf':'','nbins':5000,'minX':-4.,'maxX':4.},
	'reliso':{'var':'iso[0]','nameSuf':'RelIso','titleSuf':'','nbins': 5000,'minX': 0.,'maxX': 10},
	# 'trackiso':{'var':'iso[]','nameSuf':'TrackIso','titleSuf':'','nbins': 5000,'minX': 0.,'maxX': 100},
	'sumpt_ch':{'var':'iso[1]','nameSuf':'SumPtCh','titleSuf':'','nbins':5000,'minX': 0.,'maxX': 100.},
	# 'sumpt_chRel':{'var':'iso[1]','nameSuf':'SumPtChRel','titleSuf':'','nbins':5000,'minX': 0.,'maxX': 10.},
	'sumpt_nh':{'var':'iso[2]','nameSuf':'SumPtNh','titleSuf':'','nbins':5000,'minX': 0.,'maxX': 100.},
	'sumpt_ph':{'var':'iso[3]','nameSuf':'SumPtPh','titleSuf':'','nbins':5000,'minX': 0.,'maxX': 100.},
	'sumpt_pu':{'var':'iso[4]','nameSuf':'SumPtPU','titleSuf':'','nbins':5000,'minX': 0.,'maxX': 100.},
	# 'dz':{'var':'dz_tmp','nameSuf':'Dz','titleSuf':'','nbins':5000, 'minX':-500., 'maxX': 500.},
	# 'vz':{'var':'vz_tmp','nameSuf':'Vz','titleSuf':'','nbins':5000, 'minX':-500., 'maxX': 500.},
	# 'nhits':{'var':'nhits_tmp','nameSuf':'TrkHits','titleSuf':'','nbins':61,'minX':-0.5,'maxX':60.5},
	# 'pixhits':{'var':'pixhits_tmp','nameSuf':'PixHits','titleSuf':'','nbins':61,'minX':-0.5,'maxX':60.5}
}
for obj in objects:
	for kin in kinematics:
		# if kin == 'pixhits' and 'ch' in obj:
		# 	continue
		higgs_definitions_preblinding_loose_iso = higgs_definitions_preblinding_loose_iso.Define(obj+'_'+kin,obj+'_'+kinematics[kin]['var'])


diobjects = {'mumu':'DiMuon','hh':'DiChHad'}
dikinematics = {
	'pt':{'var':'lv.Pt()','nameSuf':'Pt','titleSuf':'','nbins':1000,'minX':0.,'maxX':150.},
	'mass':{'var':'lv.M()','nameSuf':'Mass','titleSuf':'','nbins':5000,'minX':0.,'maxX':5.},
	'dR':{'var':'dR_tmp','nameSuf':'DeltaR','titleSuf':'','nbins':1000,'minX':0.,'maxX':2.},
	'lxy':{'var':'lxyInfo[0]','nameSuf':'VtxLxy','titleSuf':'','nbins':5000,'minX':0.,'maxX':1000.},
	'lxysigma':{'var':'lxyInfo[1]','nameSuf':'VtxSigma','titleSuf':'','nbins':1000,'minX':0.,'maxX':500.},
	'lxysign':{'var':'lxysign_tmp','nameSuf':'VtxSignificance','titleSuf':'','nbins':2000,'minX':0.,'maxX':1000.},
	# 'vz':{'var':'vz_tmp','nameSuf':'VtxVz','titleSuf':'','nbins':5000,'minX':-500.,'maxX':500.},
	# 'dz':{'var':'dz_tmp','nameSuf':'VtxDz','titleSuf':'','nbins':5000,'minX':-500.,'maxX':500.},
	# 'chi2':{'var':'chi2_tmp','nameSuf':'VtxChi2','titleSuf':'','nbins':5000,'minX':0.,'maxX':100.},
	# 'chi2ndof':{'var':'chi2ndof_tmp','nameSuf':'VtxChi2Ndof','titleSuf':'','nbins':5000,'minX':0.,'maxX':100.},
	# 'nhits':{'var':'nhits_tmp','nameSuf':'DeltaTrkHits','titleSuf':'','nbins':61,'minX':-0.5,'maxX':60.5},
	# 'pixhits':{'var':'pixhits_tmp','nameSuf':'DeltaPixHits','titleSuf':'','nbins':61,'minX':-0.5,'maxX':60.5}
}
for obj in diobjects:
	for kin in dikinematics:
		# if kin == 'pixhits' and obj=='hh':
		# 	continue
		# print(obj+'_'+dikinematics[kin]['var'])
		higgs_definitions_preblinding_loose_iso = higgs_definitions_preblinding_loose_iso.Define(obj+'_'+kin,obj+'_'+dikinematics[kin]['var'])

higgs_definitions_preblinding_loose_iso = higgs_definitions_preblinding_loose_iso.Define('avgmass','0.5*(mumu_lv.M()+hh_lv.M())')

############## CATEGORIZATION ########################
if (unblind):
	if isData=='true':
		print("MANUALLY UNBLIND THE PEAK IN DATA - COMMENT OUT ABOVE")
		quit()
	else:
		higgs_definitions_precat = higgs_definitions_preblinding_loose_iso
		higgs_definitions_precat_4test = higgs_definitions_preblinding_loose_iso
else:
	higgs_definitions_precat = higgs_definitions_preblinding_loose_iso.Filter('recohiggs_mass_blinded',f'm(mumuhh) blinded in [122.5,127.5]')
	higgs_definitions_precat_4test = higgs_definitions_preblinding_loose_iso.Filter('recohiggs_mass_blinded',f'm(mumuhh) blinded in [122.5,127.5]')

if (unblind):
	higgs_definitions_precat = higgs_definitions_precat.Filter('recohiggs_mass_peak','m(mumuhh) in [122.5,127.5] peak')
else:
	higgs_definitions_precat = higgs_definitions_precat.Filter('recohiggs_mass_check','m(mumuhh) in [120,130] sidebands')
	#tight CR condition


# higgs_definitions_preblinding_loose_iso -> unblinded, loose higgs, loose iso
# higgs_definitions_precat -> blinded (SR or CR), loose iso


outFile = ROOT.TFile(args.output, "RECREATE")
outFile.cd()

hists_1d_precat = {}
hists_2d_precat = {}

diobjects = {'mumu':'DiMuon','hh':'DiChHad'}
dikinematics = {
	'mass':{'var':'lv.M()','nameSuf':'Mass','titleSuf':'','nbins':4000,'minX':0.,'maxX':4.},
	'lxy':{'var':'lxyInfo[0]','nameSuf':'VtxLxy','titleSuf':'','nbins':5000,'minX':0.,'maxX':1000.},
	'lxysigma':{'var':'lxyInfo[1]','nameSuf':'VtxSigma','titleSuf':'','nbins':1000,'minX':0.,'maxX':500.},
	'lxysign':{'var':'lxysign_tmp','nameSuf':'VtxSignificance','titleSuf':'','nbins':2000,'minX':0.,'maxX':1000.},
}
for obj in diobjects:
	for kin in dikinematics:
		hists_1d_precat['h_'+diobjects[obj]+dikinematics[kin]['nameSuf']]=\
			higgs_definitions_precat\
			.Histo1D(('h_'+diobjects[obj]+dikinematics[kin]['nameSuf'],'',\
			dikinematics[kin]['nbins'],dikinematics[kin]['minX'],dikinematics[kin]['maxX']), obj+'_'+kin, 'weight')
hists_2d_precat["h_DiMuonVtxSignificance_DiChHadVtxSignificance"] = higgs_definitions_precat\
				.Histo2D(("h_DiMuonVtxSignificance_DiChHadVtxSignificance", "", dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'],dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'])\
				,'mumu_lxysign','hh_lxysign','weight')

higgs_definitions_precat_scalarwindow = higgs_definitions_precat_4test.Filter('mass_window_check') #loose CR

hists_2d_precat["h_DiMuonVtxSignificance_DiMuonMass_MHIso_BC"] = higgs_definitions_precat_scalarwindow\
				.Histo2D(("h_DiMuonVtxSignificance_DiMuonMass_MHIso_BC", "", dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
				,'mumu_lxysign','mumu_mass','weight')

hists_2d_precat["h_DiMuonVtxSignificance_AvgMass_mumu_hh_MHIso_BC"] = higgs_definitions_precat_scalarwindow\
				.Histo2D(("h_DiMuonVtxSignificance_AvgMass_mumu_hh_MHIso_BC", "", dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
				,'mumu_lxysign','avgmass','weight')

hists_2d_precat["h_DiChHadVtxSignificance_DiChHadMass_MHIso_BC"] = higgs_definitions_precat_scalarwindow\
				.Histo2D(("h_DiChHadVtxSignificance_DiChHadMass_MHIso_BC", "", dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
				,'hh_lxysign','hh_mass','weight')

hists_2d_precat["h_DiChHadVtxSignificance_AvgMass_mumu_hh_MHIso_BC"] = higgs_definitions_precat_scalarwindow\
				.Histo2D(("h_DiChHadVtxSignificance_AvgMass_mumu_hh_MHIso_BC", "", dikinematics['lxysign']['nbins'],dikinematics['lxysign']['minX'],dikinematics['lxysign']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
				,'hh_lxysign','avgmass','weight')


hists_2d_precat["h_DiMuonMass_DiChHadMass"] = higgs_definitions_precat\
										.Histo2D(("h_DiMuonMass_DiChHadMass", "", dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
										,'mumu_mass','hh_mass','weight')
hists_1d_precat["h_AvgMass_mumu_hh"] = higgs_definitions_precat.Histo1D(("h_AvgMass_mumu_hh_SMassBC","",5000,0.,5.), 'avgmass','weight')

if ('HToSS' in args.config):
	hists_1d_precat["h_genHiggsPt"] = rdf_new.Histo1D(("h_genHiggsPt","",320,0,1600),\
														'genHiggsPt','weight')
	hists_1d_precat["h_genScalar1Lxy"] = rdf_new.Histo1D(("h_genScalar1Lxy","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar1_genlxy','weight')
	hists_1d_precat["h_genScalar2Lxy"] = rdf_new.Histo1D(("h_genScalar2Lxy","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar2_genlxy','weight')
	hists_1d_precat["h_genScalar1L"] = rdf_new.Histo1D(("h_genScalar1L","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar1_dist','weight')
	hists_1d_precat["h_genScalar2L"] = rdf_new.Histo1D(("h_genScalar2L","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar2_dist','weight')
	hists_1d_precat["h_weights"] = rdf_new.Histo1D('weight_lt')
	hists_2d_precat["h_genScalar1L_weight"] = rdf_new.Histo2D(("h_genScalar1L_weight","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],2500,0,2500),\
														'scalar1_dist','weight')
	hists_2d_precat["h_genScalar2L_weight"] = rdf_new.Histo2D(("h_genScalar2L_weight","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],2500,0,2500),\
														'scalar2_dist','weight')

	hists_1d_precat["h_genScalar1Lxy_sel"] = higgs_definitions_precat.Histo1D(("h_genScalar1Lxy_sel","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar1_genlxy','weight')
	hists_1d_precat["h_genScalar2Lxy_sel"] = higgs_definitions_precat.Histo1D(("h_genScalar2Lxy_sel","",dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX']),\
														'scalar2_genlxy','weight')
	hists_2d_precat["h_genScalar1Lxy_genScalar2Lxy"] = rdf_new\
				.Histo2D(("h_genScalar1Lxy_genScalar2Lxy", "", dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'])\
				,'scalar1_genlxy','scalar2_genlxy','weight')




for hist in hists_1d_precat:
	hists_1d_precat[hist].Write()
for hist in hists_2d_precat:
	hists_2d_precat[hist].Write()
for hist in hists_1d_precat:
	hists_1d_precat[hist].Delete()
for hist in hists_2d_precat:
	hists_2d_precat[hist].Delete()
# del rdf_new

if (unblind): # blinding
	if isData=='true':
		print("MANUALLY UNBLIND THE PEAK IN DATA - COMMENT OUT ABOVE")
		quit()
	else:
		higgs_definitions_blinded_loose_iso = higgs_definitions_preblinding_loose_iso
else:
	higgs_definitions_blinded_loose_iso = higgs_definitions_preblinding_loose_iso.Filter('recohiggs_mass_blinded',f'm(mumuhh) blinded in [122.5,127.5]')


reg_={}
reg_['prompt'] = higgs_definitions_blinded_loose_iso.Define('prompt_check',R1).Filter('prompt_check','only prompt region')
reg_['displacedmumu'] = higgs_definitions_blinded_loose_iso.Define('displacedmumu_check',R2mumu).Filter('displacedmumu_check','only displaced mumu region')
reg_['displacedhh'] = higgs_definitions_blinded_loose_iso.Define('displacedhh_check',R2hh).Filter('displacedhh_check','only displaced hh region')
reg_['displaced'] = higgs_definitions_blinded_loose_iso.Define('displaced_check',R3).Filter('displaced_check','both displaced region')

###testing number of events after prompt###
# print('Pairs selected:',vtx_definitions.Count().GetValue())
# print('Loose mass cut selected:',mass_definitions.Count().GetValue())
# print('Loose higgs mass selected:',higgs_definitions_loose.Count().GetValue())
# print('Loose higgs SR selected:',higgs_definitions_preblinding_loose_iso.Count().GetValue())

# print('Region prompt:',reg_['prompt'].Count().GetValue())
# print('Region displacedmumu:',reg_['displacedmumu'].Count().GetValue())
# print('Region displacedhh:',reg_['displacedhh'].Count().GetValue())
# print('Region displaced:',reg_['displaced'].Count().GetValue())


############# BUG CHECKING FOR LARGE LXY SIGNIFICANCE - NAN error related to negative variance along Lxy vector #############
##### SOLVED NOW #####
# weird_evts = higgs_definitions_preblinding_loose_iso\
# 	.Define('mumu_lxy','mumu_lxyInfo[0]').Define('hh_lxy','hh_lxyInfo[0]')\
# 	.Define('mumu_lxysigma','mumu_lxyInfo[1]').Define('hh_lxysigma','hh_lxyInfo[1]')\
# 	.Define('mumu_lxysign','mumu_lxyInfo[2]').Define('hh_lxysign','hh_lxyInfo[2]')\
# 	.Define('mumu_sim_val','mumu_lxyInfo[3]').Define('hh_sim_val','hh_lxyInfo[3]')\
# 	.Filter('mumu_sim_val < 0 || hh_sim_val < 0')

# print("Weird:",weird_evts.Count().GetValue())
# print("Not Weird:",higgs_definitions_preblinding_loose.Count().GetValue())
# print("Weird Max Sigma:",weird_evts.Max("hh_lxysigma").GetValue())
# print("Weird Max LxySign:",weird_evts.Max("hh_lxysign").GetValue())
# weird_evts = weird_evts.Range(10)
# np_weird_evts = weird_evts.AsNumpy(["mumu_lxy","hh_lxy","mumu_lxysigma","hh_lxysigma","mumu_lxysign","hh_lxysign","mumu_sim_val","hh_sim_val","hh_chi2ndof_tmp","mumu_chi2ndof_tmp","hh_chi2_tmp","mumu_chi2_tmp","weight"])
# print("HH")
# print(np_weird_evts['hh_lxy'])
# print(np_weird_evts['hh_lxy'].max(),np_weird_evts['hh_lxy'].argmax())
# print(np_weird_evts['hh_lxysigma'])
# print(np_weird_evts['hh_lxysigma'].max(),np_weird_evts['hh_lxysigma'].argmax())
# print(np_weird_evts['hh_lxysign'])
# print(np_weird_evts['hh_lxysign'].max(),np_weird_evts['hh_lxysign'].argmax())
# print(np_weird_evts['hh_sim_val'])
# print(np_weird_evts['hh_sim_val'].max(),np_weird_evts['hh_sim_val'].argmax())
# print(np_weird_evts['hh_chi2ndof_tmp'])
# print(np_weird_evts['hh_chi2_tmp'])
# print(41474.989603894*np_weird_evts['weight'])
# NaNargs = np.argwhere(np.isnan(np_weird_evts['hh_lxysigma']))
# print('All the NaNs:',NaNargs)
# print('lxy at NaNs:',np_weird_evts['hh_lxy'][NaNargs])
# print('similarity value at NaNs:',np_weird_evts['hh_sim_val'][NaNargs])

# print("MuMu")
# print(np_weird_evts['mumu_lxy'])
# print(np_weird_evts['mumu_lxy'].max(),np_weird_evts['mumu_lxy'].argmax())
# print(np_weird_evts['mumu_lxysigma'].max(),np_weird_evts['mumu_lxysigma'].argmax())
# print(np_weird_evts['mumu_lxysign'])
# print(np_weird_evts['mumu_lxysign'].max(),np_weird_evts['mumu_lxysign'].argmax())
# print(np_weird_evts['mumu_sim_val'])
# print(np_weird_evts['mumu_sim_val'].max(),np_weird_evts['mumu_sim_val'].argmax())
# print(np_weird_evts['mumu_chi2ndof_tmp'])
# print(np_weird_evts['mumu_chi2_tmp'])

# # print(np_weird_evts['mumu_lxy'])
# # print(np_weird_evts['mumu_lxy'].max(),np_weird_evts['mumu_lxy'].argmax())
# # print(np_weird_evts['mumu_lxysigma'].max(),np_weird_evts['mumu_lxysigma'].argmax())
# # print(np_weird_evts['mumu_lxysign'])
# # print(np_weird_evts['mumu_lxysign'].max(),np_weird_evts['mumu_lxysign'].argmax())
# NaNargs = np.argwhere(np.isnan(np_weird_evts['mumu_lxysigma']))
# print('All the NaNs:',NaNargs)
# print('lxy at NaNs:',np_weird_evts['mumu_lxy'][NaNargs])
# print('similarity value at NaNs:',np_weird_evts['mumu_sim_val'][NaNargs])

# quit()

hists_1d_={}
hists_2d_={}
ncut_cat = {}
ncut_th = {}
ncut_ts = {}
ncut_tmi = {}
ncut_thi = {}

if (isData=='false'):
	w_mean = 1
	total_evts = 1

else:
	w_mean = 1.
	total_evts = 1
print('population mean: ', w_mean)
	# print('sample mean: ',w_mean_alt)

total_skim = 0. # just a testing macro this is, no cutplots here
trig_skim = 0. # just a testing macro this is, no cutplots here
metfilter_skim = 0. # just a testing macro this is, no cutplots here
mu_skim = 0. # just a testing macro this is, no cutplots here
ch_skim = 0. # just a testing macro this is, no cutplots here
higgs_skim = 0. # just a testing macro this is, no cutplots here
rem_skim = higgs_skim
ncut_trig = cut_trig.Sum('weight').GetValue()
trig_skim = trig_skim+(higgs_skim-ncut_trig)
ncut_mu = cut_mu_sel.Sum('weight')
ncut_dimu = cut_dimuon.Sum('weight')
ncut_h = cut_hadron_sel.Sum('weight')
ncut_dih = cut_dihadron.Sum('weight')
ncut_lsh = higgs_definitions_preblinding_loose.Sum('weight')
ncut_li = higgs_definitions_preblinding_loose_iso.Sum('weight')


# print('trg:',ncut_trig)
# print('mu:',ncut_mu.GetValue())
# print('mumu:',ncut_dimu.GetValue())

# quit()

ROOT.gInterpreter.Declare("""
	ROOT::RDF::RNode ApplyMuIsoFilter(ROOT::RDF::RNode df, float leading_iso, float subleading_iso) {
		return df.Filter([leading_iso, subleading_iso](float a, float b) { return a < leading_iso && b < subleading_iso; }, {"mu1_reliso_tmp","mu2_reliso_tmp"});
	}
	ROOT::RDF::RNode ApplyChIsoFilter(ROOT::RDF::RNode df, float leading_iso, float subleading_iso) {
		return df.Filter([leading_iso, subleading_iso](float a, float b) { return a < leading_iso && b < subleading_iso; }, {"ch1_reliso_tmp","ch2_reliso_tmp"});
	}
	ROOT::RDF::RNode ApplyHiggsFilter(ROOT::RDF::RNode df, float mumuhh_mass_low, float mumuhh_mass_high) {
		return df.Filter([mumuhh_mass_low, mumuhh_mass_high](double a) { return a >= mumuhh_mass_low && a <= mumuhh_mass_high; }, {"recohiggs_mass"});
	}
	""")

for key in reg_:
	###### Trying to use lambda function in C++ as much as possible within loop to stop memory usage from blowing up - it gets blown up due to JIT everytime Filter is called
	###### NOTE: Using too many bins in 2D histograms BLOWS UP memory by GB -> Use as less bins as possible -> There is always some memory leak from Histo2D (it looks like like)
	hists_1d_[key]={}
	hists_2d_[key]={}
	

	mu_isocuts = 'mu1_iso[0] < ' + str(dimuon_cuts['category'][key]['relisoLeading']) + ' && ' + 'mu2_iso[0] < ' + str(dimuon_cuts['category'][key]['reliso'])
	ch_isocuts = 'ch1_iso[0] < ' + str(dihadron_cuts['category'][key]['relisoLeading']) + ' && ' + 'ch2_iso[0] < ' + str(dihadron_cuts['category'][key]['reliso'])
	print('Region:', key)
	higgs_definitions = reg_[key].Define('mu_iso_check',mu_isocuts).Define('ch_iso_check',ch_isocuts)\
						.Define('mu1_reliso_tmp','mu1_iso[0]').Define('mu2_reliso_tmp','mu2_iso[0]')\
						.Define('ch1_reliso_tmp','ch1_iso[0]').Define('ch2_reliso_tmp','ch2_iso[0]')

	# higgs_mass_peak = 'recohiggs_mass >='+str(mumuhh_mass_low)+' && '+'recohiggs_mass <='+str(mumuhh_mass_high)
	# higgs_mass_peak_tight = f'recohiggs_mass > {mumuhh_mass_low-sideband}'+' && '+f'recohiggs_mass < {mumuhh_mass_high+sideband}'

	if recohiggs_cuts['flag']:
		if (unblind):
			higgs_definitions_SRCR = ROOT.ApplyHiggsFilter(ROOT.RDF.AsRNode(higgs_definitions),mumuhh_mass_low,mumuhh_mass_high)
		else:
			higgs_definitions_SRCR = ROOT.ApplyHiggsFilter(ROOT.RDF.AsRNode(higgs_definitions),mumuhh_mass_low-sideband,mumuhh_mass_high+sideband)
	else:
		higgs_definitions_SRCR = higgs_definitions

	if recoscalar_cuts['flag']:
		cut_mass_compatibility = higgs_definitions_SRCR.Filter('mass_window_check','m(mumu)~m(hh)') 
	else:
		cut_mass_compatibility = higgs_definitions_SRCR
	
	
	higgs_definitions_masswindow = higgs_definitions.Filter('mass_window_check')
	higgs_definitions_iso = ROOT.ApplyMuIsoFilter(ROOT.RDF.AsRNode(higgs_definitions),dimuon_cuts['category'][key]['relisoLeading'],dimuon_cuts['category'][key]['reliso'])
	higgs_definitions_iso = ROOT.ApplyChIsoFilter(ROOT.RDF.AsRNode(higgs_definitions_iso),dihadron_cuts['category'][key]['relisoLeading'],dihadron_cuts['category'][key]['reliso'])
	higgs_definitions_masswindow_iso = higgs_definitions_iso.Filter('mass_window_check')
	higgs_definitions_SRCR_iso = ROOT.ApplyMuIsoFilter(ROOT.RDF.AsRNode(higgs_definitions_SRCR),dimuon_cuts['category'][key]['relisoLeading'],dimuon_cuts['category'][key]['reliso'])
	higgs_definitions_SRCR_iso = ROOT.ApplyChIsoFilter(ROOT.RDF.AsRNode(higgs_definitions_SRCR_iso),dihadron_cuts['category'][key]['relisoLeading'],dihadron_cuts['category'][key]['reliso'])
	# cut_mass_compatibility -> blinded, loose higgs, loose iso, mass window
	# higgs_preblinding_scalarcut -> unblinded, full, mass window
	# higgs_definitions -> blinded, loose higgs, loose iso
	# higgs_blinded_scalarcut -> blinded, full, mass window

	############## ISOLATION CUTS ########################

	print(mu_isocuts)
	print(ch_isocuts)
	if diMuFlag_:
		cut_dimuon_iso = ROOT.ApplyMuIsoFilter(ROOT.RDF.AsRNode(cut_mass_compatibility),dimuon_cuts['category'][key]['relisoLeading'],dimuon_cuts['category'][key]['reliso'])
	else:
		cut_dimuon_iso = cut_mass_compatibility


	if diChFlag_:
		cut_dihadron_iso = ROOT.ApplyChIsoFilter(ROOT.RDF.AsRNode(cut_dimuon_iso),dihadron_cuts['category'][key]['relisoLeading'],dihadron_cuts['category'][key]['reliso'])
	else:
		cut_dihadron_iso = cut_dimuon_iso

	# print(cut_mass_compatibility.Count().GetValue(),higgs_definitions_SRCR.Count().GetValue(),higgs_definitions_SRCR_iso.Count().GetValue())
	# quit()
	# pfiso[0]=RelIso;  pfiso[1]=ch_iso;  pfiso[2]=nh_iso;  pfiso[3]=ph_iso;  pfiso[4]=pu_iso

	############## DEFINITION OF HISTOGRAMS 1D + 2D ########################

	for obj in objects:
		for kin in kinematics:
			# if kin == 'pixhits' and 'ch' in obj:
			# 	continue
			# cut_dihadron_iso = cut_dihadron_iso.Define(obj+'_'+kin,obj+'_'+kinematics[kin]['var'])
			hists_1d_[key]['h_'+objects[obj]+kinematics[kin]['nameSuf']]=\
				cut_dihadron_iso\
				.Histo1D(('h_'+objects[obj]+kinematics[kin]['nameSuf'],'',\
				kinematics[kin]['nbins'],kinematics[kin]['minX'],kinematics[kin]['maxX']), obj+'_'+kin, 'weight')

	for obj in diobjects:
		for kin in dikinematics:
			# if kin == 'pixhits' and obj=='hh':
			# 	continue
			# print(obj+'_'+dikinematics[kin]['var'])
			# cut_dihadron_iso = cut_dihadron_iso.Define(obj+'_'+kin,obj+'_'+dikinematics[kin]['var'])
			hists_1d_[key]['h_'+diobjects[obj]+dikinematics[kin]['nameSuf']]=\
				cut_dihadron_iso\
				.Histo1D(('h_'+diobjects[obj]+dikinematics[kin]['nameSuf'],'',\
				dikinematics[kin]['nbins'],dikinematics[kin]['minX'],dikinematics[kin]['maxX']), obj+'_'+kin, 'weight')

	hists_1d_[key]["h_AvgMass_mumu_hh"] = cut_dihadron_iso.Histo1D(("h_AvgMass_mumu_hh","",5000,0.,5.), 'avgmass','weight')
	hists_1d_[key]["h_recoHiggsMass"] = cut_dihadron_iso.Histo1D(("h_recoHiggsMass", "", 5000, 0., 500.), 'recohiggs_mass','weight')
	hists_1d_[key]["h_DiMuonDiChHad_DeltaPhi"] = cut_dihadron_iso.Histo1D(("h_DiMuonDiChHad_DeltaPhi", "", 1000, -3.2, 3.2), 'pt_dphi','weight')

	# hists_2d_[key]["h_DiMuonMass_DiChHadMass"] = cut_dihadron_iso\
	# 										.Histo2D(("h_DiMuonMass_DiChHadMass", "", dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
	# 										,'mumu_mass','hh_mass','weight')
	hists_2d_[key]["h_DiMuonVtxLxy_DiChHadVtxLxy"] = cut_dihadron_iso\
											.Histo2D(("h_DiMuonVtxLxy_DiChHadVtxLxy", "", dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'])\
											,'mumu_lxy','hh_lxy','weight')

	# hists_2d_[key]["h_leadingMuonRelIso_leadingChHadRelIso"] = cut_dihadron_iso\
	# 										.Histo2D(("h_leadingMuonRelIso_leadingChHadRelIso", "", kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'], kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'])\
	# 										,'mu1_reliso','ch1_reliso','weight')
	hists_2d_[key]["h_leadingMuonRelIso_subleadingMuonRelIso"] = cut_dihadron_iso\
											.Histo2D(("h_leadingMuonRelIso_subleadingMuonRelIso", "", kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'], kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'])\
											,'mu1_reliso','mu2_reliso','weight')
	hists_2d_[key]["h_leadingChHadRelIso_subleadingChHadRelIso"] = cut_dihadron_iso\
											.Histo2D(("h_leadingChHadRelIso_subleadingChHadRelIso", "", kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'], kinematics['reliso']['nbins'],kinematics['reliso']['minX'],kinematics['reliso']['maxX'])\
											,'ch1_reliso','ch2_reliso','weight')

	# hists_2d_["h_DiChHadVtxLxy_DiChHadVtxSigma"] = new TH2F("h_DiChHadVtxLxy_DiChHadVtxSigma", "", 5000, 0., 1000., 1000, 0., 500.);
	hists_1d_[key]["h_recoHiggsMass_MHMSIso_BC"] = higgs_definitions.Histo1D(("h_recoHiggsMass_MHMSIso_BC", "", 5000, 0., 500.), 'recohiggs_mass','weight')
	hists_1d_[key]["h_recoHiggsMass_MHIso_BC"] = higgs_definitions_masswindow.Histo1D(("h_recoHiggsMass_MHIso_BC", "", 5000, 0., 500.), 'recohiggs_mass','weight')
	hists_1d_[key]["h_recoHiggsMass_MHMS_BC"] = higgs_definitions_iso.Histo1D(("h_recoHiggsMass_MHIso_BC", "", 5000, 0., 500.), 'recohiggs_mass','weight')
	hists_1d_[key]["h_recoHiggsMass_MH_BC"] = higgs_definitions_masswindow_iso.Histo1D(("h_recoHiggsMass_MHIso_BC", "", 5000, 0., 500.), 'recohiggs_mass','weight')

	hists_2d_[key]["h_recoHiggsMass_AvgMass_mumu_hh_MHMSIso_BC"] = higgs_definitions\
		.Histo2D(("h_recoHiggsMass_AvgMass_mumu_hh_MHMSIso_BC", "", 1000, 100., 200.,4000,0.,4.), 'recohiggs_mass','avgmass','weight')
	hists_2d_[key]["h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC"] = higgs_definitions_masswindow\
		.Histo2D(("h_recoHiggsMass_AvgMass_mumu_hh_MHIso_BC", "", 1000, 100., 200.,4000,0.,4.), 'recohiggs_mass','avgmass','weight')
	hists_2d_[key]["h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC"] = higgs_definitions_iso\
		.Histo2D(("h_recoHiggsMass_AvgMass_mumu_hh_MHMS_BC", "", 1000, 100., 200.,4000,0.,4.), 'recohiggs_mass','avgmass','weight')
	hists_2d_[key]["h_recoHiggsMass_AvgMass_mumu_hh_MH_BC"] = higgs_definitions_masswindow_iso\
		.Histo2D(("h_recoHiggsMass_AvgMass_mumu_hh_MH_BC", "", 1000, 100., 200.,4000,0.,4.), 'recohiggs_mass','avgmass','weight')

	# hists_2d_[key]["h_DiMuonMass_DiChHadMass_MSIso_BC"] = higgs_definitions_SRCR\
	# 										.Histo2D(("h_DiMuonMass_DiChHadMass_MSIso_BC", "", dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
	# 										,'mumu_mass','hh_mass','weight')
	hists_2d_[key]["h_DiMuonMass_DiChHadMass_MS_BC"] = higgs_definitions_SRCR_iso\
											.Histo2D(("h_DiMuonMass_DiChHadMass_MS_BC", "", dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
											,'mumu_mass','hh_mass','weight')
	# hists_2d_[key]["h_DiMuonMass_DiChHadMass_Iso_BC"] = cut_mass_compatibility\
	# 										.Histo2D(("h_DiMuonMass_DiChHadMass_Iso_BC", "", dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'],dikinematics['mass']['nbins'],dikinematics['mass']['minX'],dikinematics['mass']['maxX'])\
	# 										,'mumu_mass','hh_mass','weight')

	hists_1d_[key]["h_AvgMass_mumu_hh_MSIso_BC"] = higgs_definitions_SRCR.Histo1D(("h_AvgMass_mumu_hh_MSIso_BC","",5000,0.,5.), 'avgmass','weight')
	hists_1d_[key]["h_AvgMass_mumu_hh_MS_BC"] = higgs_definitions_SRCR_iso.Histo1D(("h_AvgMass_mumu_hh_MS_BC","",5000,0.,5.), 'avgmass','weight')
	hists_1d_[key]["h_AvgMass_mumu_hh_Iso_BC"] = cut_mass_compatibility.Histo1D(("h_AvgMass_mumu_hh_Iso_BC","",5000,0.,5.), 'avgmass','weight')

	# hists_1d_["h_DiMuonDeltaR_weird"]=weird_evts.Histo1D(("h_DiMuonDeltaR_weird","",1000,0.,10.),'mumu_dR_tmp','weight')
	# hists_1d_["h_DiMuonVtxLxy_weird"]=weird_evts.Define('mumu_lxy','mumu_lxyInfo[0]').Histo1D(("h_DiMuonVtxLxy_weird","",5000,0.,1000.),'mumu_lxy','weight')
	# hists_1d_["h_DiMuonVtxSigma_weird"]=weird_evts.Define('mumu_lxysigma','mumu_lxyInfo[1]').Histo1D(("h_DiMuonVtxSigma_weird","",1000,0.,500.),'mumu_lxysigma','weight')
	# hists_1d_["h_DiMuonVtxChi2_weird"]=weird_evts.Histo1D(("h_DiMuonVtxChi2_weird","",5000,0.,100.),'mumu_chi2_tmp','weight')
	# hists_2d_["h_DiMuonVtxLxy_DiMuonVtxSigma_weird"] = weird_evts.Define('mumu_lxy','mumu_lxyInfo[0]').Define('mumu_lxysigma','mumu_lxyInfo[1]')\
	# 		        						.Histo2D(("h_DiMuonVtxLxy_DiMuonVtxSigma_weird", "", dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],dikinematics['lxysigma']['nbins'],dikinematics['lxysigma']['minX'],dikinematics['lxysigma']['maxX'])\
	# 										,'mumu_lxy','mumu_lxysigma','weight')
	# hists_1d_["h_DiChHadDeltaR_weird"]=weird_evts.Histo1D(("h_DiChHadDeltaR_weird","",1000,0.,10.),'hh_dR_tmp','weight')
	# hists_1d_["h_DiChHadVtxLxy_weird"]=weird_evts.Define('hh_lxy','hh_lxyInfo[0]').Histo1D(("h_DiChHadVtxLxy_weird","",5000,0.,1000.),'hh_lxy','weight')
	# hists_1d_["h_DiChHadVtxSigma_weird"]=weird_evts.Define('hh_lxysigma','hh_lxyInfo[1]').Histo1D(("h_DiChHadVtxSigma_weird","",1000,0.,500.),'hh_lxysigma','weight')
	# hists_1d_["h_DiChHadVtxChi2_weird"]=weird_evts.Histo1D(("h_DiChHadVtxChi2_weird","",5000,0.,100.),'hh_chi2_tmp','weight')
	# hists_2d_["h_DiChHadVtxLxy_DiChHadVtxSigma_weird"] = weird_evts.Define('hh_lxy','hh_lxyInfo[0]').Define('hh_lxysigma','hh_lxyInfo[1]')\
	# 		        						.Histo2D(("h_DiChHadVtxLxy_DiChHadVtxSigma_weird", "", dikinematics['lxy']['nbins'],dikinematics['lxy']['minX'],dikinematics['lxy']['maxX'],dikinematics['lxysigma']['nbins'],dikinematics['lxysigma']['minX'],dikinematics['lxysigma']['maxX'])\
	# 										,'hh_lxy','hh_lxysigma','weight')
	# # print('GETS BEFORE CUTFLOW DEF')
	hists_1d_[key]["Cutflow"] = ROOT.TH1D(key+"_"+"Cutflow", "Cutflow distribution", 14, 0.5, 14.5)
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(1, "TotalEvts")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(2, "TriggerSkim")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(3, "RemainingSkim")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(4, "#mu selection")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(5, "dimuon candidate")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(6, "h trk selection")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(7, "dihadron candidate")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(8, "loose scalar+higgs mass")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(9, "loose leading isolation")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(10, key)
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(11, "tight higgs mass")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(12, "m(#mu#mu) ~ m(hh)")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(13, "#mu isolation")
	hists_1d_[key]["Cutflow"].GetXaxis().SetBinLabel(14, "h isolation")

	# # print('GETS AFTER CUTFLOW DEF')

	ncut_cat[key]=higgs_definitions.Sum('weight')
	ncut_th[key]=higgs_definitions_SRCR.Sum('weight')
	ncut_ts[key]=cut_mass_compatibility.Sum('weight')
	ncut_tmi[key]=cut_dimuon_iso.Sum('weight')
	ncut_thi[key]=cut_dihadron_iso.Sum('weight')
	# outFile.mkdir(key)
	# outFile.cd(key)
	# for hist in hists_1d_[key]:
	# 	hists_1d_[key][hist].Write()
	# for hist in hists_2d_[key]:
	# 	hists_2d_[key][hist].Write()
	# for hist in hists_1d_[key]:
	# 	hists_1d_[key][hist].Delete()
	# for hist in hists_2d_[key]:
	# 	hists_2d_[key][hist].Delete()
	# del higgs_definitions;del higgs_definitions_SRCR;del cut_mass_compatibility;del cut_dimuon_iso;del cut_dihadron_iso;
############## WRITING REPORT MANUALLY ########################
	# if (isData=='false'):
	# 	report=cut_dihadron.Report()
	# 	report.Print()
		# hists_1d_["Cutflow"].Fill(1, report.At('Trigger').GetAll())
		# hists_1d_["Cutflow"].Fill(2, report.At('Trigger').GetPass())
		# hists_1d_["Cutflow"].Fill(3, report.At('MET filter').GetPass())
		# hists_1d_["Cutflow"].Fill(4, report.At('mu selection').GetPass())
		# hists_1d_["Cutflow"].Fill(5, report.At('dimuon candidate').GetPass())
		# hists_1d_["Cutflow"].Fill(6, report.At('hadron selection').GetPass())
		# hists_1d_["Cutflow"].Fill(7, report.At('dihadron candidate').GetPass())
		# if 'baseline' not in args.cuts: 
		# 	hists_1d_["Cutflow"].Fill(8, report.At('m(mumuhh) in [120,130]').GetPass())
		# 	hists_1d_["Cutflow"].Fill(9, report.At('m(mumu)~m(hh)').GetPass())
		# sys.stderr.write('After 2d mass cut:'+str(report.At('m(mumu)~m(hh)').GetAll())+'\n')

# sys.stderr.write(report.Print())
# entries_trig = cut_trig.Count()
# entries_met= cut_met.Count()
# entries_mu_sel= cut_mu_sel.Count()
# entries_dimuon= cut_dimuon.Count()
# entries_hadron_sel = cut_hadron_sel.Count()
# entries_dihadron= mass_definitions.Count()
# entries_higgsmass = cut_higgsmass.Count()
# entries_2dmass = cut_mass_compatibility.Count()
# # entries_test= cut_test.Count()

# sys.stderr.write ('total:'+str(entries_total.GetValue())+'\n')
# sys.stderr.write('After trigger cut:'+str(entries_trig.GetValue())+'\n')
# sys.stderr.write('After MET filters:'+str(entries_met.GetValue())+'\n')
# sys.stderr.write('After muon selection:'+str(entries_mu_sel.GetValue())+'\n')
# sys.stderr.write('After dimuon cut:'+str(entries_dimuon.GetValue())+'\n')
# sys.stderr.write('After hadron selection:'+str(entries_hadron_sel.GetValue())+'\n')
# sys.stderr.write('After dihadron cut:'+str(entries_dihadron.GetValue())+'\n')
# sys.stderr.write('After higgs mass cut:'+str(entries_higgsmass.GetValue())+'\n')
# sys.stderr.write('After 2d mass cut:'+str(entries_2dmass.GetValue())+'\n')
# sys.stderr.flush()

############## WRITING HISTOS ########################
# print(hists_1d_)
# quit()
print('GETS OUTSIDE KEY LOOP')


total_skim_vals = total_skim
trig_skim_vals = trig_skim
rem_skim_vals = rem_skim
ncut_mu_vals = ncut_mu.GetValue()

ncut_dimu_vals = ncut_dimu.GetValue()
ncut_h_vals = ncut_h.GetValue()
ncut_dih_vals = ncut_dih.GetValue()
ncut_lsh_vals = ncut_lsh.GetValue()
ncut_li_vals = ncut_li.GetValue()

for key in reg_:
	hists_1d_[key]["Cutflow"].Fill(1, total_skim)
	hists_1d_[key]["Cutflow"].Fill(2, trig_skim)
	hists_1d_[key]["Cutflow"].Fill(3, rem_skim)
	hists_1d_[key]["Cutflow"].Fill(4, ncut_mu_vals)
	hists_1d_[key]["Cutflow"].Fill(5, ncut_dimu_vals)
	hists_1d_[key]["Cutflow"].Fill(6, ncut_h_vals)
	hists_1d_[key]["Cutflow"].Fill(7, ncut_dih_vals)
	hists_1d_[key]["Cutflow"].Fill(8, ncut_lsh_vals)
	hists_1d_[key]["Cutflow"].Fill(9, ncut_li_vals)
	hists_1d_[key]["Cutflow"].Fill(10, ncut_cat[key].GetValue())
	hists_1d_[key]["Cutflow"].Fill(11, ncut_th[key].GetValue())
	hists_1d_[key]["Cutflow"].Fill(12, ncut_ts[key].GetValue())
	hists_1d_[key]["Cutflow"].Fill(13, ncut_tmi[key].GetValue())
	hists_1d_[key]["Cutflow"].Fill(14, ncut_thi[key].GetValue())
print('DONE WITH CUTFLOW')

# if isData=='false':
# 	cutPlot.Scale(w_mean)
# cutPlot.Write()

for key in reg_:
	outFile.mkdir(key)
	outFile.cd(key)
	for hist in hists_1d_[key]:
		hists_1d_[key][hist].Write()
	for hist in hists_2d_[key]:
		hists_2d_[key][hist].Write()
outFile.Close()

sys.stderr.write("Time taken: --- %s seconds ---" % (time.time() - start_time)+'\n')
sys.stderr.flush()




