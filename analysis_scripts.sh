########## REMEMBER TO CHANGE THE CUTS FILE!!! ##########
dirname=$1
fcuts_suf=$2
mkdir $dirname
cp scripts/merge_script* $dirname
cd $dirname
sh merge_script_lv0.sh
cd ../../..
python3 generate_fnos_expansion.py -c configs/2017/all_SM_samples.yaml --cuts configs/2017/cuts/CRCuts_$fcuts_suf.yaml -n 50 -o $dirname -y UL2017 -f params_sm.txt
python3 generate_fnos_expansion.py -c configs/2017/mumu_data2017.yaml --cuts configs/2017/cuts/CRCuts_$fcuts_suf.yaml -n 50 -o $dirname -y UL2017 -f params_data.txt
python3 generate_fnos_expansion.py -c configs/2017/all_signal.yaml --cuts configs/2017/cuts/SRCuts_$fcuts_suf.yaml -n 50 -o $dirname -y UL2017 -f params_signal.txt
#### modify condor_script.sub ####
#condor_submit condor_script.sub
#cd $dirname
#sh merge_script.sh
#sh merge_script_lv2.sh
#cd $HToSS_analysis
#sh massbound_calc.sh plots/KaonMassAssumption/EventPreselection_RefittedTrks_231116/ggH plots/PionMassAssumption/EventPreselection_RefittedTrks_231117/ggH ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/EventPreselection_RefittedTrks_231116/Massbounds_95CI
