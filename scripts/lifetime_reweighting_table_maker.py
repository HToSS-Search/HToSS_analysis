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

ROOT.gROOT.SetBatch(True)

def maxfilenumber(path):
	list_of_files = os.listdir(path)
	n = [int(re.findall("\d+", str(file))[0]) for file in list_of_files]
	return min(n),max(n)

start_time = time.time()
parser = argparse.ArgumentParser(description='Plot stacked histogram')
# parser.add_argument("-c", "--config", dest="config",   help="Enter config file to process", type=str)
parser.add_argument("-o","--output", dest="output", help="Destination file", type=str)
parser.add_argument("-y","--year", dest="year", help="Year for processing", type=str)

# parser.add_argument("--total", dest="onlyweights", help="just store MCweights weighted with cs*lumi", action='store_true')

args = parser.parse_args()
p_dir ='/pnfs/iihe/cms/store/user/sdansana/HToSS/MC/nTuples'
f_dict = {
	'MS0p4': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131209',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161653',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124141',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_182559'
},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131254',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161737',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124227',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_182646'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131240',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161722',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124213',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_182623'
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131225',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161707',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124158',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_180906'

		}
	},
	'MS0p6': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131107',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161548',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124036',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221931'

		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131153',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161636',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124121',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_222016'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131138',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161622',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124106',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_222001'

		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131122',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161607',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124051',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221946'
		}
	},

	'MS0p8': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131006',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161447',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123934',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221830'
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131051',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161532',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124020',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221916'
},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131035',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161517',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_124005',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221901'

		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_131020',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161502',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123950',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221845'

		}
	},

	'MS0p9': {
		'ctauS0': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130904',
			'UL2016_APV': 'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161346',
			'UL2017': 'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123832',
			'UL2018': 'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221729'
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130950',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161431',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123918',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221814'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130935',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161416',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123903',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221800'			
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130920',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161401',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123848',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS0p9_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221745'
		}
	},
	'MS1': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130759',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161243',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123728',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221619'
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130849',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161329',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123817',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221708'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130833',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedPion_LT/240812_161314',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123801',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221652'
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedPion_LT/240812_130818',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedPion_LT/240812_161259',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/240811_123746',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedPion_LT/240807_221636'

		}
	},

	'MS1p1': {
		'ctauS0': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130653',
			'UL2016_APV': 'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161138',
			'UL2017': 'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123617',
			'UL2018': 'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221517'
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130743',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161222',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123705',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221602'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130724',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161207',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123650',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221547'
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130708',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161152',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123634',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221532'
		}
	},

	'MS1p1_pion': {
		'ctauS0': {
			'UL2016': '',
			'UL2016_APV': '',
			'UL2017': 'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/241009_074302',
			'UL2018': ''
		},
		'ctauS100': {
			'UL2016':'',
			'UL2016_APV':'',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/241009_075846',
			'UL2018':''
		},
		'ctauS10': {
			'UL2016':'',
			'UL2016_APV':'',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/241009_075415',
			'UL2018':''
		},
		'ctauS1': {
			'UL2016':'',
			'UL2016_APV':'',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p1_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedPion_LT/241009_074942',
			'UL2018':''
		}
	},

	'MS1p2': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130549',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161033',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123504',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221416'
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130635',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161121',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123555',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221501'
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130620',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161106',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123534',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221445'
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130605',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161049',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123519',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221431'
		}
	},
	'MS1p4': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130445',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160932',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123356',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221313'

		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130533',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161017',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123448',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221359',
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130517',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_161002',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123427',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221344',
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130502',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160947',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123411',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p4_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221328',
		}
	},
	'MS1p6': {
		'ctauS0': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130340',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160830',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123256',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221207',
		},
		'ctauS100': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130428',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160916',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123340',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221257',
		},
		'ctauS10': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130410',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160900',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123325',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221242',
		},
		'ctauS1': {
			'UL2016': 'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130355',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160845',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123311',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p6_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221222',
		}
	},

	'MS1p8': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130235',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160728',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123150',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221052',
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130322',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160813',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123239',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221146',
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130308',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160758',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123224',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221126',
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130250',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160743',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123209',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS1p8_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221107',
		}
	},
	'MS2': {
		'ctauS0': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130121',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160625',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123041',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS0_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_220949',
		},
		'ctauS100': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v1_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130211',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160713',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123134',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS100_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221036',
		},
		'ctauS10': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130154',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v1_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160658',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240607_135303',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS10_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221021',
		},
		'ctauS1': {
			'UL2016':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAOD-106X_mcRun2_asymptotic_v13-v2_MCUL2016nonAPV_HToSS_ChargedKaon_LT/240812_130138',
			'UL2016_APV':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPV-106X_mcRun2_asymptotic_preVFP_v8-v2_MCUL2016APV_HToSS_ChargedKaon_LT/240812_160642',
			'UL2017':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1_MCUL2017_HToSS_ChargedKaon_LT/240811_123057',
			'UL2018':'HToSSTo2Mu2Hadrons_MH125_MS2_ctauS1_TuneCP2_13TeV-powheg-pythia8/RunIISummer20UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1_MCUL2018_HToSS_ChargedKaon_LT/240807_221006',
		}
	},
}

###################### LOADING YAML FOR PHYSICS PROCESS ############################
# fin = open(args.config,'r')
conf_pars = f_dict
# ms_list = [0.4,0.6,0.8,0.9,1,1.1,1.2,1.4,1.6,1.8,2] # complete
# ms_list = [0.4,0.6,0.8,0.9,1,1.1,1.2,1.4,1.6,1.8,2]
if '2017' in args.year:
	ms_list = ['0.4','0.6','0.8','0.9','1','1.1','1.1_pion','1.2','1.4','1.6','1.8','2']
else:
	ms_list = ['0.4','0.6','0.8','0.9','1','1.1','1.2','1.4','1.6','1.8','2']
ct_orig_list=[0,1,10,100]
ms_ct_old_ct_new_vals=len(ms_list)*[len(ct_orig_list)*[None]]
treeName = "makeTopologyNtupleMiniAOD_LT/tree"
new_cts=[]
lt_dict={}
for mass in ms_list:
	mass = 'MS'+mass.replace('.','p')
	lt_dict[mass]={}
	print(mass)
	for ct in conf_pars[mass]:
		# mass_point=float(mass.replace('MS','').replace('p','.'))
		old_ct=float(ct.replace('ctauS',''))
		if old_ct == 0:
			old_ct=0.1
		lt_dict[mass][old_ct]={}
		try:
			print(mass,conf_pars[mass][ct][args.year])
		except KeyError:
			print('mass not available, going to next-',mass)
			continue
		data_loc=os.path.join(p_dir,conf_pars[mass][ct][args.year])
		directories = [os.path.join(data_loc,d) for d in os.listdir(data_loc) if os.path.isdir(os.path.join(data_loc, d))]
		# print(directories)
###################### LOADING ALL FILES FOR PROCESSING ############################
# data_name = conf_pars['name']
		list_of_files = []
		for dir in directories:
			print(dir)
			flow, fhigh = maxfilenumber(dir)
			for i in range(flow, fhigh+1):
				fno = str(i)
				fistr = os.path.join(dir,"output_"+fno+".root")
				if not os.path.exists(fistr):
					continue
				try:
					root_file = ROOT.TFile.Open(fistr)
					if not root_file or root_file.IsZombie() or root_file.TestBit(ROOT.TFile.kRecovered):
						raise Exception(f"Error opening file: {fistr}")
				except Exception as e:
					print(f"Error processing file {fistr}: {e}")
					continue
				list_of_files.append(fistr)
				print(fistr)
		print(list_of_files)
		if not isinstance(list_of_files, list):
			file = ROOT.TFile(list_of_files)
			weightPlot = file.Get("makeTopologyNtupleMiniAOD_LT/weightHisto").Clone()
			weightPlot.SetDirectory(0)
			file.Close()
		else:
			file = ROOT.TFile(list_of_files[0])
			weightPlot = file.Get("makeTopologyNtupleMiniAOD_LT/weightHisto").Clone()
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
					tmpPlot2 = file.Get("makeTopologyNtupleMiniAOD_LT/weightHisto").Clone()
					weightPlot.Add(tmpPlot2)
					# Close the file
					file.Close()
					# continue
				except Exception as e:
					print(f"Error processing file {fistr}: {e}")
					continue  # Continue to the next file in case of an error
		# new_ct_list=weightPlot.GetNbinsX()*[None]
		# vals=weightPlot.GetNbinsX()*[None]
		for k in range(1,weightPlot.GetNbinsX()+1):
			new_ct=float(weightPlot.GetXaxis().GetBinLabel(k))
			val=weightPlot.GetBinContent(k)
			err=weightPlot.GetBinError(k)
			lt_dict[mass][old_ct][new_ct]=[val,err]
		# print(new_ct_list)
		# print(vals)
		# ms_ct_old_ct_new_vals[nm][nct]=len(new_ct_list)*[None]
		# print(weightPlot.GetXaxis().GetBinLabel(k))
		# print(weightPlot.GetBinContent(k))
			
		# for k in range(1,weightPlot.GetNbinsX()):
		# print(weightPlot.GetX())
		# print(weightPlot.GetY())
fin = open(args.output,'w')
yaml.dump(lt_dict,fin)
print(lt_dict)
fin.close()