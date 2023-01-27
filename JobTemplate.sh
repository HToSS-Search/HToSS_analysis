#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "========================================="
cfg=$1
output=$2
flow=$3
fhigh=$4

echo "First arg: $cfg"
echo "Second arg: $output"
echo "Third arg: $flow"
echo "Fourth arg: $fhigh"

wd="/user/sdansana"
rel="CMSSW_10_6_27/src/HToSS_analysis"
reldir=$wd/$rel
cd $reldir


export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
eval `scram runtime -sh`
echo "CMSSW intialized to "$CMSSW_BASE
source setup.sh
#configs/2017/mumu_HtoSS_MS2_ctau0.yaml
#plots/IsoDistributions/output_HtoSS_MS2_ctau0.root
./bin/quickIsoCustom.exe -c $cfg -o $output --flow $flow --fhigh $fhigh 
