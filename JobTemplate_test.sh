#!/bin/bash
echo "========================================="
cfg=$1
cuts=$2
output=$3
flow=$4
fhigh=$5
dataset=$6

echo "First arg: $cfg"
echo "Second arg: $cuts"
echo "Third arg: $output"
echo "Fourth arg: $flow"
echo "Fifth arg: $fhigh"
echo "Sixth arg: $dataset"

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

python3 src/RDataFrames_analyzer_test.py -c $cfg --cuts $cuts -o $output -y UL2017 --flow $flow --fhigh $fhigh
