#!/bin/bash
inname=$1
outname=$2
yhigh=$3
fcuts=$4
item=$5
cat=$6
year=$7
wd="/user/sdansana"
rel="CMSSW_10_6_27/src/HToSS_analysis"
reldir=$wd/$rel
cd $reldir
echo ""$reldir


#export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
#eval `scramv1 runtime -sh`
#source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "CMSSW intialized to "$CMSSW_BASE
source ./plotting_setup.sh
#source ./setup.sh

mkdir -p ~/public_html/HToSS_plots/Stacked/$outname
# cat_list=("prompt" "displacedmumu" "displacedhh" "displaced")
# Loop over the list # this code can be reduced by using nested loops (for later)
echo "Processing item: $item"
# for category in "${cat_list[@]}"
# do
# Add your code to process each item here
mkdir -p $wd/public_html/HToSS_plots/Stacked/$outname/LogYaxis/$item/$cat
mkdir -p $wd/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/$item/$cat
cp $wd/index.php $wd/public_html/HToSS_plots/Stacked/$outname/LogYaxis/$item/$cat/.
cp $wd/index.php $wd/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/$item/$cat/.
# done
if [[ $cat == *"no"* ]]; then
# Loop over the list
	python scripts/StackedHistograms.py -y $year -i $inname/total -o $wd/public_html/HToSS_plots/Stacked/$outname/LogYaxis/$item -s "HToSS_MH125_$item" --log --bkg --data --category $cat 
	python scripts/StackedHistograms.py -y $year -i $inname/total -o $wd/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/$item -s "HToSS_MH125_$item" --yhigh $yhigh --bkg --data --category $cat 
else
## below for without ratio ##
	python scripts/StackedHistograms.py -y $year -i $inname/total -o $wd/public_html/HToSS_plots/Stacked/$outname/LogYaxis/$item -s "HToSS_MH125_$item" --log --bkg --data --noratio --category $cat 
	python scripts/StackedHistograms.py -y $year -i $inname/total -o $wd/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/$item -s "HToSS_MH125_$item" --yhigh $yhigh --bkg --data --noratio --category $cat
fi
######python3 scripts/mass2dplotter.py -i $inname/total/HToSS_MH125_MS2_ctauS0.root --cuts configs/2017/cuts/SRCuts_kaonmass.yaml -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsolationAndScalarMassCut_withRefitted_231116/LinearYaxis/MS2_ctau0 -d HToSS_MH125_MS2_ctau0
#python3 scripts/mass2dplotter.py -i $inname/total/output_HToSS_MH125_MS2_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau0 -d HToSS_MH125_MS2_ctau0
#python3 scripts/mass2dplotter.py -i $inname/total/output_HToSS_MH125_MS2_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau1 -d HToSS_MH125_MS2_ctau1
#python3 scripts/mass2dplotter.py -i $inname/total/output_HToSS_MH125_MS2_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau10 -d HToSS_MH125_MS2_ctau10
#python3 scripts/mass2dplotter.py -i $inname/total/output_HToSS_MH125_MS2_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau100 -d HToSS_MH125_MS2_ctau100
#python3 scripts/mass2dplotter.py -i $inname/total/output_QCD_MuEnrichedPt5.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau0 -d QCD
#python3 scripts/mass2dplotter.py -i $inname/total/output_Run2017.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau0 -d Run2017

# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LogYaxis/MS2_ctau0 -s "HtoSS_MS2_ctau0" --log
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LogYaxis/MS2_ctau1 -s "HtoSS_MS2_ctau1" --log
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LogYaxis/MS2_ctau10 -s "HtoSS_MS2_ctau10"  --log
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LogYaxis/MS2_ctau100 -s "HtoSS_MS2_ctau100"  --log
# 
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LinearYaxis/MS2_ctau0 -s "HtoSS_MS2_ctau0" --yhigh 200 
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LinearYaxis/MS2_ctau1 -s "HtoSS_MS2_ctau1" --yhigh 200 
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LinearYaxis/MS2_ctau10 -s "HtoSS_MS2_ctau10"  --yhigh 200 -m 10
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/LinearYaxis/MS2_ctau100 -s "HtoSS_MS2_ctau100"  --yhigh 200 -m 100

# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau10 -s "HtoSS_MS2_ctau10" -m 100
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau100 -s "HtoSS_MS2_ctau100" -m 1000
