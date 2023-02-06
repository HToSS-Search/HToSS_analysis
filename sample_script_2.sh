#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau0.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau1.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau1.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau10.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau10.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau100.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau100.root
#for some reason, -u false doesn't work, default is false anyway
#should be able to run on other MC, eg, QCD
#./bin/quickIsoCustom.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/IsoDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD.yaml -o plots/IsoDistributions/output_QCD.root
./bin/quickIsoCustom.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/IsoDistributions/output_HtoSS_MS2_ctau0_new.root --flow 1 --fhigh 5
#./bin/quickIsoCustom.exe -c configs/2017/mumu_wPlusJets.yaml -o plots/IsoDistributions/output_wPlusJets.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt-15to20_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt15to20_MuEnrichedPt5.root --flow 1 --fhigh 100
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt-20to30_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt20to30_MuEnrichedPt5.root --flow 1 --fhigh 2
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt20to30_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt20to30_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt30to50_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt30to50_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt50to80_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt50to80_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt80to120_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt80to120_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt120to170_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt120to170_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt170to300_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt170to300_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt300to470_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt300to470_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_QCD_Pt470to600_MuEnrichedPt5.yaml -o plots/IsoDistributions/output_QCD_Pt470to600_MuEnrichedPt5.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_ttbar.yaml -o plots/IsoDistributions/output_ttbar.root
#./bin/quickIsoCustom.exe -c configs/2017/mumu_zPlusPtBinned.yaml -o plots/IsoDistributions/output_zPlusPtBinned.root
# GenPlotter
#./bin/quickGenPlotter.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/genDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 -v true -n 500
#./bin/quickGenPlotter.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/genDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5

