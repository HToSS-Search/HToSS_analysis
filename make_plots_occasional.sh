#!/bin/bash
histdir1=$1
plotdir=$2
year=$3
opt=$4
histdir2=$5
fcuts=$6

# For categorization, use below:
if [[ $opt == *"Significance"* ]]; then
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS0 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS0 --hname DiMuonVtxSignificance_DiChHadVtxSignificance --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS1 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS1 --hname DiMuonVtxSignificance_DiChHadVtxSignificance --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS10 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS10 --hname DiMuonVtxSignificance_DiChHadVtxSignificance --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS100 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut --xmid 40 --ymid 30
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS2_ctauS100 --hname DiMuonVtxSignificance_DiChHadVtxSignificance --xmid 40 --ymid 30
# For isolation, use below:
elif [[ $opt == *"RelIso"* ]]; then
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS1p2_ctauS0 --hname leadingMuonRelIso_subleadingMuonRelIso --xmid 0.3 --ymid 0.6 
    python3 scripts/SignalBackgroundOptimization_2D.py -y $year -f $histdir1 -o ~/public_html/HToSS_plots/Stacked/$plotdir -d HToSS_MH125_MS1p2_ctauS0 --hname leadingChHadRelIso_subleadingChHadRelIso --xmid 0.6 --ymid 0.8
elif [[ $opt == *"MassCalc"* ]]; then
	  # add --updatecuts option to update all yaml files for that year with the new window
    mkdir -p ~/public_html/HToSS_plots/Stacked/$plotdir
    cp ~/index.php ~/public_html/HToSS_plots/Stacked/$plotdir/.
    python3 scripts/massbounds.py --input1 $histdir1 --input2 $histdir2 -o ~/public_html/HToSS_plots/Stacked/$plotdir -r 1 --xlow 0. --xhigh 2.5 --yhigh 1.5 --norm -y $year --updatecuts 
elif [[ $opt == *"MassPlot"* ]]; then
	  mkdir ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots
		cp ~/index.php ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/.
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS2_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS2_ctau0 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS2_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS2_ctau1 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS2_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS2_ctau10 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS2_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS2_ctau100 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p8_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p8_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p8_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p8_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p8_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p8_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p8_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p8_ctau100 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p6_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p6_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p6_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p6_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p6_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p6_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p6_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p6_ctau100 -c nocategory -y $year
	# python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p4_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p4_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p4_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p4_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p4_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p4_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p4_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p4_ctau100 -c nocategory -y $year
	# python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p2_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p2_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p2_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p2_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p2_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p2_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p2_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p2_ctau100 -c nocategory -y $year
	# python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p1_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p1_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p1_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p1_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p1_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p1_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_HToSS_MH125_MS1p1_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d HToSS_MH125_MS1p1_ctau100 -c nocategory -y $year
	# python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS1_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS1_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS1_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS1_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS1_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS1_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p9_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS1_ctau100 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p9_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p9_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p9_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p9_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p9_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p9_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p8_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p9_ctau100 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p8_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p8_ctau0 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p8_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p8_ctau1 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p8_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p8_ctau10 -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p8_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p8_ctau100 -c nocategory -y $year
	  python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p6_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p6_ctau0 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p6_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p6_ctau1 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p6_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p6_ctau10 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p6_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p6_ctau100 -c nocategory -y $year
	 python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p4_ctauS0.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p4_ctau0 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p4_ctauS1.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p4_ctau1 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p4_ctauS10.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p4_ctau10 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_HToSS_MH125_MS0p4_ctauS100.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d HToSS_MH125_MS0p4_ctau100 -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_QCD.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d QCD -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_QCD.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d QCD -c nocategory -y $year
    #python3 scripts/mass2dplotter.py -i $histdir1/total/output_QCD.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir -d QCD -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir1/total/output_Data.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots -d Data -c nocategory -y $year
    python3 scripts/mass2dplotter.py -i $histdir2/total/output_Data.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir/2dmassplots/Pion -d Data -c nocategory -y $year
    # python3 scripts/mass2dplotter.py -i $histdir1/total/output_Data.root --cuts $fcuts -o ~/public_html/HToSS_plots/Stacked/$plotdir -d Data -c nocategory -y $year
else
    echo "Choose either of 4 options - Significance, RelIso, MassCalc, MassPlot; Note for MassCalc provide a histdir2; for MassPlot cut config at the end"
fi
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS1 --hname leadingMuonRelIso_subleadingMuonRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS1 --hname leadingChHadRelIso_subleadingChHadRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS1 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS1 --hname DiMuonVtxSignificance_DiChHadVtxSignificance
# 
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS10 --hname leadingMuonRelIso_subleadingMuonRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS10 --hname leadingChHadRelIso_subleadingChHadRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS10 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS10 --hname DiMuonVtxSignificance_DiChHadVtxSignificance

# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS100 --hname leadingMuonRelIso_subleadingMuonRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS100 --hname leadingChHadRelIso_subleadingChHadRelIso
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS100 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS2_ctauS100 --hname DiMuonVtxSignificance_DiChHadVtxSignificance

# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS1p2_ctauS0 --hname DiMuonVtxSignificance_DiChHadVtxSignificance_ZoomedOut
# python3 scripts/SignalBackgroundOptimization_2D.py -f plots/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -o ~/public_html/HToSS_plots/Stacked/KaonMassAssumption/IsoScalarHiggsCuts_Category_90pPrompt_240211 -d HToSS_MH125_MS1p2_ctauS0 --hname DiMuonVtxSignificance_DiChHadVtxSignificance

