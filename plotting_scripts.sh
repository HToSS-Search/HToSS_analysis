################# Preselection #########################
sh make_plots_new.sh plots/UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 1.5 configs/2018/cuts/SRCuts_kaonmass.yaml MS2_ctau0 displacedmumu UL2017
sh make_plots_new.sh plots/UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 1.5 configs/2018/cuts/SRCuts_kaonmass.yaml MS2_ctau0 displacedhh UL2017
sh make_plots_new.sh plots/UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 1.5 configs/2018/cuts/SRCuts_kaonmass.yaml MS2_ctau0 displaced UL2017
sh make_plots_new.sh plots/UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 1.5 configs/2018/cuts/SRCuts_kaonmass.yaml MS1p2_ctau100 prompt UL2017
sh make_plots_new.sh plots/UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 UL2017/KaonAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_240603 1.5 configs/2018/cuts/SRCuts_kaonmass.yaml MS1p2_ctau100 nocategory UL2017
# mass calculation
sh make_plots_occasional.sh plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610/massbounds_fit UL2017 MassCalc plots/UL2017/PionAssumption/Preselection_Category_90pPrompt_SF_240610
# significance plots
sh make_plots_occasional.sh plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017 Significance
# RelIso plots
sh make_plots_occasional.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018 RelIso
# 2dmass plots
sh make_plots_occasional.sh plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 UL2017 MassPlot plots/UL2017/KaonAssumption/Preselection_Category_90pPrompt_SF_240610 configs/2017/cuts/CRCuts_baseline_kaonmass.yaml
############### Isolation optimization #################
# Preselection + scalar mass window
# RelIso plots
sh make_plots_occasional.sh plots/UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018/KaonAssumption/HiggsCuts_Category_90pPrompt_240321 UL2018 RelIso
############### All cuts ###############################
# all normal plots
