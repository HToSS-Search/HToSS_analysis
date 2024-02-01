import ROOT
import uproot

def get_pileup(era): # code below taken from spark tnp
	'''
	Get the pileup distribution scalefactors to apply to simulation
	for a given era.
	'''

	# get the pileup
	dataPileup = {
		'UL2016_HIPM': 'pileup/UL2016/dataPileup_nominal.root',
		'UL2016': 'pileup/UL2016/dataPileup_nominal.root',
		'UL2017': 'pileup/UL2017/dataPileup_nominal.root',
		'UL2018': 'pileup/UL2018/dataPileup_nominal.root'
	}
	mcPileup = {
		'UL2016_HIPM': 'pileup/UL2016/mcPileup.root',
		'UL2016': 'pileup/UL2016/mcPileup.root',
		'UL2017': 'pileup/UL2017/mcPileup.root',
		'UL2018': 'pileup/UL2018/mcPileup.root'
	}
	# get absolute path
	#    baseDir = os.path.dirname(__file__)
	#    dataPileup = {k: os.path.join(baseDir, dataPileup[k]) for k in dataPileup}
	#    mcPileup = {k: os.path.join(baseDir, mcPileup[k]) for k in mcPileup}
	with uproot.open(dataPileup[era]) as f:
		data_edges = f['pileup'].axis(0).edges()
		data_pileup = f['pileup'].values()
		data_pileup /= sum(data_pileup)
	with uproot.open(mcPileup[era]) as f:
		mc_edges = f['pileup'].axis(0).edges()
		mc_pileup = f['pileup'].values()
		mc_pileup /= sum(mc_pileup)
	pileup_edges = data_edges if len(data_edges) < len(mc_edges) else mc_edges
	pileup_ratio = [d/m if m else 1.0 for d, m in zip(
		data_pileup[:len(pileup_edges)-1], mc_pileup[:len(pileup_edges)-1])]

	return pileup_ratio, pileup_edges
