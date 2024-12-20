#!/bin/bash
echo "========================================="
cfg=$1
cuts=$2
output=$3
flow=$4
fhigh=$5
dataset=$6
weight=$7

echo "First arg: $cfg"
echo "Second arg: $cuts"
echo "Third arg: $output"
echo "Fourth arg: $flow"
echo "Fifth arg: $fhigh"
echo "Sixth arg: $dataset"
echo "Seventh arg: $weight"

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
./bin/quickIsoCustom.exe -c $cfg --cuts $cuts -o $output --flow $flow --fhigh $fhigh -w $weight --hack true
#./bin/quickWeightGenerator.exe -c $cfg -o $output --flow $flow --fhigh $fhigh -v false --hack true 
