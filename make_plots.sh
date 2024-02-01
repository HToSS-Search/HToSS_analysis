#!/bin/bash
inname=$1
outname=$2
yhigh=$3
fcuts=$4
./make_plotdirs.sh $outname
# tf argument below is temporary for loosened higgs mass window
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau0 -s "HToSS_MH125_MS2_ctauS0" --yhigh $yhigh --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau0p1 -s "HToSS_MH125_MS2_ctauS0p1" --yhigh $yhigh --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau1 -s "HToSS_MH125_MS2_ctauS1" --yhigh $yhigh --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau10 -s "HToSS_MH125_MS2_ctauS10" --yhigh $yhigh --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LinearYaxis/MS2_ctau100 -s "HToSS_MH125_MS2_ctauS100" --yhigh $yhigh --bkg --data --noratio -m 10 --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LogYaxis/MS2_ctau0p1 -s "HToSS_MH125_MS2_ctauS0p1" --log --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LogYaxis/MS2_ctau0 -s "HToSS_MH125_MS2_ctauS0" --log --bkg --data --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LogYaxis/MS2_ctau1 -s "HToSS_MH125_MS2_ctauS1" --log --data --bkg --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LogYaxis/MS2_ctau10 -s "HToSS_MH125_MS2_ctauS10"  --log --data --bkg --noratio --tf 0.1953945485080403
python scripts/StackedHistograms.py -y 2017 -i $inname/total -o ~/public_html/HToSS_plots/Stacked/$outname/LogYaxis/MS2_ctau100 -s "HToSS_MH125_MS2_ctauS100"  --log --data --bkg --noratio -m 10 --tf 0.1953945485080403
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
