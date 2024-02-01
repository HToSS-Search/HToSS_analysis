#!/bin/bash
echo "========================================="
cfg=$1
output=$2
dataset=$3
year=$4

echo "First arg: $cfg"
echo "Second arg: $output"

wd="/user/sdansana"
rel="CMSSW_10_6_27/src/HToSS_analysis"
reldir=$wd/$rel
cd $reldir
echo ""$reldir


#export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
eval `scramv1 runtime -sh`
#source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "CMSSW intialized to "$CMSSW_BASE
source ./setup.sh
#configs/2017/mumu_HtoSS_MS2_ctau0.yaml
#plots/IsoDistributions/output_HtoSS_MS2_ctau0.root
#python3 src/RDataFrames_Zpeak_alt.py -c $cfg -y $year -o $output
python3 src/RDataFrames_Zpeak.py -c $cfg -o $output
#./bin/quickWeightGenerator.exe -c $cfg -o $output --flow $flow --fhigh $fhigh -v false --hack true 
