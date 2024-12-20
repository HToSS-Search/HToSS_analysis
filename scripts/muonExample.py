## example how to read the muon format v2
## (Adapted from JMAR and EGM examples)
from correctionlib import _core
import numpy as np

############################
## Example A: 2016postVFP ##
############################

# Load CorrectionSet
fname = "scale_factors/UL2017/muon_Z.json"
if fname.endswith(".json.gz"):
    import gzip
    with gzip.open(fname,'rt') as file:
        data = file.read().strip()
        evaluator = _core.CorrectionSet.from_string(data)
else:
    evaluator = _core.CorrectionSet.from_file(fname)

ptvals = np.linspace(5,200,10)
etavals = np.array([0.4,1,1.7,2.2])
print(evaluator.items())
print(evaluator["NUM_TrackerMuons_DEN_genTracks"].evaluate(ptvals, etavals, "nominal"))
# TrackerMuon Reconstruction UL scale factor ==> NOTE the year key has been removed, for consistency with Run 3
valsf = evaluator["NUM_TrackerMuons_DEN_genTracks"].evaluate(1.1, 50.0, "nominal")
print("sf is: " + str(valsf))

# Medium ID UL scale factor, down variation ==> NOTE the year key has been removed, for consistency with Run 3
valsf = evaluator["NUM_MediumID_DEN_TrackerMuons"].evaluate(0.8, 35.0, "systdown")
print("systdown is: " + str(valsf))

# Medium ID UL scale factor, up variation ==> NOTE the year key has been removed, for consistency with Run 3
valsf = evaluator["NUM_MediumID_DEN_TrackerMuons"].evaluate(0.8, 35.0, "systup")
print("systup is: " + str(valsf))

# Trigger UL systematic uncertainty only ==> NOTE the year key has been removed, for consistency with Run 3
valsyst = evaluator["NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight"].evaluate(1.8, 54.0, "syst")
print("syst is: " + str(valsyst))
valstat = evaluator["NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight"].evaluate(1.8, 54.0, "stat")
print("syst is: " + str(valstat))

##########################
## Example B: 2022preEE ##
##########################
# fname = "scale_factors/UL2017/muon_Z.json"
# if fname.endswith(".json.gz"):
#     import gzip
#     with gzip.open(fname,'rt') as file:
#         data = file.read().strip()
#         evaluator = _core.CorrectionSet.from_string(data)
# else:
#     evaluator = _core.CorrectionSet.from_file(fname)

# # Medium ID 2022 scale factor using eta as input
# valsf_eta = evaluator["NUM_LooseID_DEN_TrackerMuons"].evaluate(-1.1, 45.0, "nominal")
# print("sf for eta = -1.1: " + str(valsf_eta))

# # Medium ID 2022 scale factor using eta as input ==> Note that this value should be the same
# # as the previous one, since even though the input can be signed eta, the SFs for 2022 were
# # computed for |eta|. This is valid for ALL the years and jsons
# valsf_eta = evaluator["NUM_LooseID_DEN_TrackerMuons"].evaluate(1.1, 45.0, "nominal")
# print("sf for eta = 1.1 " + str(valsf_eta))

# # Trigger 2022 systematic uncertainty only 
# valsyst = evaluator["NUM_IsoMu27_DEN_CutBasedIdMedium_and_PFIsoMedium"].evaluate(-1.8, 54.0, "syst")
# print("syst is: " + str(valsyst))
