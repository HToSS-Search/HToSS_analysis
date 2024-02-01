#!/bin/bash
echo "========================================="
cfg=$1
output=$2
flow=$3
fhigh=$4
dataset=$5

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

python3 src/RDataFrames_analyzer.py -c $cfg -o $output -y 2017 --flow $flow --fhigh $fhigh --onlyweights

