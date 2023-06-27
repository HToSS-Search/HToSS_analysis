# mkdir -p ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau0
# mkdir -p ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau1
# mkdir -p ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau10
# mkdir -p ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau100
# cp ~/index.php ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau0/.
# cp ~/index.php ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau1/.
# cp ~/index.php ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau10/.
# cp ~/index.php ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau100/.
python scripts/StackedHistograms.py -y 2018 -i plots/IsolationAndScalarMassCut_230620/total -o ~/public_html/HToSS_plots/Stacked/IsolationAndScalarMassCut_230622/LinearYaxis/MS2_ctau0 -s "HtoSS_MS2_ctau0"
python scripts/StackedHistograms.py -y 2018 -i plots/IsolationAndScalarMassCut_230620/total -o ~/public_html/HToSS_plots/Stacked/IsolationAndScalarMassCut_230622/LinearYaxis/MS2_ctau1 -s "HtoSS_MS2_ctau1"
python scripts/StackedHistograms.py -y 2018 -i plots/IsolationAndScalarMassCut_230620/total -o ~/public_html/HToSS_plots/Stacked/IsolationAndScalarMassCut_230622/LinearYaxis/MS2_ctau10 -s "HtoSS_MS2_ctau10"
python scripts/StackedHistograms.py -y 2018 -i plots/IsolationAndScalarMassCut_230620/total -o ~/public_html/HToSS_plots/Stacked/IsolationAndScalarMassCut_230622/LinearYaxis/MS2_ctau100 -s "HtoSS_MS2_ctau100"
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau1 -s "HtoSS_MS2_ctau1"
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau10 -s "HtoSS_MS2_ctau10" -m 100
# python scripts/StackedHistograms.py -y 2018 -i plots/BlindedWithIsolation/total -o ~/public_html/HToSS_plots/Stacked/BlindedAndIsolated/MS2_ctau100 -s "HtoSS_MS2_ctau100" -m 1000
