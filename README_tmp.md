# Running the analysis
Below the steps for running the analysis are listed. There are several steps that need to be performed and maybe clubbed together but th attempt is to have a more or less common workflow.

## Setup the analysis
Getting the correct cmssw version and packages to be added. ```setup.sh``` contains the LCG release to be referred to. Update this to get latest versions of ROOT etc. CMSSW_10_6_27 is currently used - Update: probably not needed anymore because sourcing from cvmfs directly and no need to setup CMSSW environment.
```
source setup.sh
```

## Preparing the analysis
All the config files are stored in ```configs/year``` directory. Cuts can be modified in ```configs/year/cuts```. Dataset information is stored in ```configs/year/datasets``` - In each yaml file, several information present at the moment. The required ones are: ```locations, name, cross_section, name, sum_weights```. ```dataset_info.yaml``` contains information on ```ntuple, location, status, process``` and is linked to file in NTuplizer where it is updated manually currently according to the latest produced ntuples. This part can be automated down the line.
Updating the config files: 
```
python3 update_config_files.py -d dataset_info.yaml -y UL2018
```
To update weights on config files, add ```--weightdir, --justweights``` pointing to the weight folders. ```--justcs``` updates the BR info on the signal samples from ```BR_xsec_benchmark.yaml``` currently pointing to the file in limit setting folder. 

### Updating + generating sum of weights
The ```RDataFrames_analyzer.py``` also calculates the sum of weights and produces weight histograms with the combination of all ntuple files with the option ```--weights```. This can be used in a similar way as the rest of the framework to generate the sum of weights and then update it to the config files as mentioned above. Use ```generate_fnos_expansion.py``` to produce the weight folders / params file for condor submission. Do the condor submission as in the analysis chain. Then merge the resulting files with ```generate_hadder.py```, similar to what is done in the analysis. 

eg. below:
```
python3 generate_fnos_expansion.py -c configs/2016/all_signal_kaon.yaml --cuts configs/2016/cuts/CRCuts.yaml -n 100 -o plots/weightDistributions_UL2016 -f params_weights.txt -y UL2016
sh WeightsJob.sh [params_weights.txt] # modify condor_script.sub to use WeightsJob.sh instead of AnalyzeJob.sh
python3 generate_hadder.py -d plots/weightDistributions_UL2016 -l 1
python3 update_config_files.py -d dataset_info.yaml -y UL2016 --weightdir plots/weightDistributions_UL2016 --justweights
```



### Analysis chain
First generate the parameters to be submitted to condor. This is done using command below which uses the macro ```generate_fnos_expansion.py```. Example for 2018 is below. The 2nd argument is the suffix for the config cut file.
```
sh analysis_scripts.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240318 baseline_kaonmass 2018 UL2018
sh analysis_scripts.sh plots/UL2018/PionAssumption/HiggsCuts_Category_90pPrompt_240318 baseline_pionmass 2018 UL2018
```
Next step is to submit jobs for production (namely, ```params_signal_kaon/pion, params_data, params_sm.txt```). Simply change the params file in the sub script:
```
condor_submit condor_script.sub
```

Monitoring the ongoing analysis - ```monitor_script.py```. This checks through the error files & output root files to see the jobs that got killed/running/other reasons and then makes 3 corresponding files with the params for each of the processes falling in those reasons with suffix provided.
```
python monitor_script.py -i params_data.txt -s data_2
```
This gives - ```params_resubmit_killed_data_2.txt, params_resubmit_running_data_2.txt, params_resubmit_oth_data_2.txt```
Modify the condor_script.sub to then submit these params directly. 

Once all the files are processed, they need to merged with hadd. Either do it individually or use condor with ```generate_hadder.py```:
```
python3 generate_hadder.py -d plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240318 -l 1
```
This takes into account the ```plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240318/proc/subproc/subproc_divided_rootfiles``` and merges all ```subproc_divided_rootfiles``` into one subproc files in ```proc/subproc.root``` when run with ```-l 1```. To then merge the subproc into proc root files and transferring to ```total``` directory, run with ```-l 2```. This command produces a hadd_params_lv?.txt which can then be submitted with condor_script_hadd.sub to condor for hadd. It also reduces the job by copying non-divided rootfiles. This can be used for also merging weight histograms following the same convention.

### Plotters

Firstly, the directory to store plots is needed. Use the command below (dirname is stored as ~/public_html/HToSS_plots/Stacked/$dirname):
```
sh make_plotdirs.sh dirname
```
Instructions above have been modified. No requirement for making directories anymore (all happens internally in python scripts)

There are 4 main plotting macros (used at different stages of the analysis). 
1) SignalBackgroundOptimization_2D.py - Required for plotting/defining categories based on Lxy significance (dimuon/dihadron) and Rel. isolation (leading/subleading). Example code provided below: 
```
python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS1p2_ctauS0 --hname leadingMuonRelIso_subleadingMuonRelIso
```
2) massbounds.py - 2D mass windows constructed using signal samples (all mass points, ctau0); Ensure that both pion & kaon mass assumption is present. 

3) mass2dplotter.py - Additional where boxes are not made (just plots with window parameters in yaml file) and can be made for signal, QCD, data etc


4) StackedHistograms.py - This is the main plotting macros which makes all the final plots with MC, data all included

To run the first 3, use ```make_plots_occasional.sh``` which selects one of the 4 options - Significance, RelIso, MassCalc, MassPlot. Example code below which runs this to do mass calculation:
```
# mass calculation
sh make_plots_occasional.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240318 KaonAssumption/HiggsCuts_Category_90pPrompt_240318/massbounds_fit UL2018 MassCalc plots/UL2018/PionAssumption/HiggsCuts_Category_90pPrompt_240318
# significance plots
sh make_plots_occasional.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018 Significance

# RelIso plots
sh make_plots_occasional.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018 RelIso
# 2dmass plots
sh make_plots_occasional.sh plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017 MassPlot plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 configs/2017/cuts/CRCuts_baseline_kaonmass.yaml

```
To update yaml files with the new 2d mass window, add ```--updatecuts``` to the python line

