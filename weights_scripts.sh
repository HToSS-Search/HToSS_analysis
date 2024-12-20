########## REMEMBER TO CHANGE THE CUTS FILE!!! ##########
dirname=$1
fcuts_suf=$2
year=$3
year_off=$4
#mkdir $dirname
#cp scripts/merge_script* $dirname
#cd $dirname
#sh merge_script_lv0.sh
cd $CMSSW_BASE/src/HToSS_analysis
python3 generate_fnos_expansion.py -c configs/$year/all_SM_samples.yaml --cuts configs/$year/cuts/CRCuts_$fcuts_suf.yaml -n 50 -o $dirname -f params_sm.txt -y $year_off -j
### add code here to switch between kaon signal and pion signal
if [[ $fcuts_suf == *"kaon"* ]]; then
	python3 generate_fnos_expansion.py -c configs/$year/all_signal_kaon.yaml --cuts configs/$year/cuts/SRCuts_$fcuts_suf.yaml -n 20 -o $dirname -f params_signal_kaon.txt -y $year_off -j
else
	python3 generate_fnos_expansion.py -c configs/$year/all_signal_pion.yaml --cuts configs/$year/cuts/SRCuts_$fcuts_suf.yaml -n 20 -o $dirname -f params_signal_pion.txt -y $year_off -j
fi

#### modify condor_script.sub ####
#condor_submit condor_script.sub
#cd $dirname
#sh merge_script.sh
#sh merge_script_lv2.sh
#cd $HToSS_analysis
#sh massbound_calc.sh plots/KaonMassAssumption/EventPreselection_RefittedTrks_231116/ggH plots/PionMassAssumption/EventPreselection_RefittedTrks_231117/ggH ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/EventPreselection_RefittedTrks_231116/Massbounds_95CI
