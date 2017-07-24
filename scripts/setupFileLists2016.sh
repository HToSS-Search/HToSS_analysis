#!/bin/bash
echo Updating filelists for datasets used ...
echo First deleting old filelists ...

if [ -z "$TQZ_TOOLS_PATH" ]; then
    echo '$TQZ_TOOLS_PATH not set, setting to .'
    export TQZ_TOOLS_PATH='.'
fi

rm $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/*

echo Done!
echo Now outputting the lists of the dataset directories into their relevant files ...


# Normal datasets

ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/eeRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/eeRun2016Files.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/emuRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/emuRun2016Files.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016Files.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart1.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart1.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart1.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart1.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart1.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart2.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FilesPart2.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016BFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016CFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016DFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016EFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016GFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016HFiles.txt

ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016B_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016BFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016C_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016CFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016D_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016DFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016E_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016EFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016F_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016FFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016G_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016GFebFiles.txt
ls /data0/data/TopPhysics/postTriggerSkims2016/mumuRun2016H_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/mumuRun2016HFebFiles.txt

ls /scratch/data/tZqSkimsRun2016/metRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Files.txt

ls /scratch/data/tZqSkimsRun2016/metRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part1Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part1Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part1Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part1Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part1Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part2Files.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016Part2Files.txt

ls /scratch/data/tZqSkimsRun2016/metRun2016B/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016BFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016C/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016CFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016D/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016DFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016E/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016EFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016F/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016FFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016G/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016GFiles.txt
ls /scratch/data/tZqSkimsRun2016/metRun2016H/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/metRun2016HFiles.txt

ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-10to50_amcatnlo/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets10To50Files.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-10to50_amcatnlo_ext/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets10To50Files.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-10to50_madgraph/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets10To50MadgraphFiles.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_amcatnlo/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50Files.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_amcatnlo_ext2/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50Files.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_madgraph/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50MadgraphFiles.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_madgraph_ext2/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50MadgraphFiles.txt

ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_madgraph_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50MadgraphFebFiles.txt
ls /scratch/data/tZqSkimsRun2016/DYJetsToLL_M-50_madgraph_ext2_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/DYJets50MadgraphFebFiles.txt

ls /scratch/data/tZqSkimsRun2016/sChannel_4f/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/sChannelFiles.txt
ls /scratch/data/tZqSkimsRun2016/tbarChannel_4f/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tbarChannelFiles.txt
ls /scratch/data/tZqSkimsRun2016/tW_antitop_5f/* -1d  >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tbarWInclusiveFiles.txt
ls /scratch/data/tZqSkimsRun2016/tWZ_5f/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tWZ_ll_Files.txt
ls /scratch/data/tZqSkimsRun2016/tChannel_4f/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tChannelFiles.txt
ls /scratch/data/tZqSkimsRun2016/ttbarDilepton/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbarDileptonFiles.txt
ls /scratch/data/tZqSkimsRun2016/ttbarInclusive_powerheg/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbarInclusivePowerhegFiles.txt

ls /scratch/data/tZqSkimsRun2016/ttbarInclusive_powerheg_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbarInclusivePowerhegFebFiles.txt

ls /scratch/data/tZqSkimsRun2016/ttWlnu/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttWlnuFiles.txt
ls /scratch/data/tZqSkimsRun2016/ttW2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttW2qFiles.txt
ls /scratch/data/tZqSkimsRun2016/ttZ2l2nu/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttZ2l2nuFiles.txt
ls /scratch/data/tZqSkimsRun2016/ttZ2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttZ2qFiles.txt
ls /scratch/data/tZqSkimsRun2016/tW_top_5f/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tWInclusiveFiles.txt

ls /scratch/data/tZqSkimsRun2016/tZq_ll_4Flavour3Lepton/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZqFiles.txt

ls /scratch/data/tZqSkimsRun2016/tZq_ll_4Flavour3Lepton_feb/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZqFebFiles.txt

ls /scratch/data/tZqSkimsRun2016/tHq/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tHqFiles.txt
ls /scratch/data/tZqSkimsRun2016/wPlusJets/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/wPlusJetsFiles.txt
ls /scratch/data/tZqSkimsRun2016/WW1l1nu2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/WW1l1nu2qFiles.txt
ls /scratch/data/tZqSkimsRun2016/WW2l2nu/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/WW2l2nuFiles.txt
ls /scratch/data/tZqSkimsRun2016/WZ1l1nu2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/WZ1l1nu2q.txt
ls /scratch/data/tZqSkimsRun2016/WZ2l2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/WZ2l2q.txt
ls /scratch/data/tZqSkimsRun2016/WZJets/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/WZjets.txt
ls /scratch/data/tZqSkimsRun2016/ZZ2l2nu/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ZZ2l2nuFiles.txt
ls /scratch/data/tZqSkimsRun2016/ZZ2l2q/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ZZ2l2qFiles.txt
ls /scratch/data/tZqSkimsRun2016/ZZ4l/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ZZ4lFiles.txt

# FCNC Datasets

ls /scratch/data/tZqSkimsRun2016/tZq_lll_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_lll_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/tZq_lll_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_lll_Kappa_Zut.txt
ls /scratch/data/tZqSkimsRun2016/tZq_lll_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_lll_Zeta_Zct.txt

ls /scratch/data/tZqSkimsRun2016/tZq_ll_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_ll_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/tZq_ll_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_ll_Kappa_Zut.txt
ls /scratch/data/tZqSkimsRun2016/tZq_ll_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_ll_Zeta_Zct.txt
ls /scratch/data/tZqSkimsRun2016/tZq_ll_Zeta_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZq_ll_Zeta_Zut.txt

ls /scratch/data/tZqSkimsRun2016/ttbar_tLeptonic_FCNC_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tLeptonic_FCNC_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tLeptonic_FCNC_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tLeptonic_FCNC_Kappa_Zut.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarLeptonic_FCNC_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarLeptonic_FCNC_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarLeptonic_FCNC_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarLeptonic_FCNC_Kappa_Zut.txt

ls /scratch/data/tZqSkimsRun2016/ttbar_tLeptonic_FCNC_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tLeptonic_FCNC_Zeta_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tLeptonic_FCNC_Zeta_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tLeptonic_FCNC_Zeta_Zut.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarLeptonic_FCNC_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarLeptonic_FCNC_Zeta_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarLeptonic_FCNC_Zeta_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarLeptonic_FCNC_Zeta_Zut.txt

ls /scratch/data/tZqSkimsRun2016/ttbar_tHadronic_FCNC_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tHadronic_FCNC_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tHadronic_FCNC_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tHadronic_FCNC_Kappa_Zut.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarHadronic_FCNC_Kappa_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarHadronic_FCNC_Kappa_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarHadronic_FCNC_Kappa_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarHadronic_FCNC_Kappa_Zut.txt

ls /scratch/data/tZqSkimsRun2016/ttbar_tHadronic_FCNC_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tHadronic_FCNC_Zeta_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tHadronic_FCNC_Zeta_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tHadronic_FCNC_Zeta_Zut.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarHadronic_FCNC_Zeta_Zct/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarHadronic_FCNC_Zeta_Zct.txt
ls /scratch/data/tZqSkimsRun2016/ttbar_tbarHadronic_FCNC_Zeta_Zut/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/ttbar_tbarHadronic_FCNC_Zeta_Zut.txt

# Synchronisation files
ls /scratch/data/tZqSkimsRun2016/synch/tZq/* -1d >> $TQZ_TOOLS_PATH/configs/2016/datasets/fileLists/tZqSynchFiles.txt

echo Done!
echo Filelists have been updated.
