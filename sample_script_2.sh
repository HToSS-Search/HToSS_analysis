#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau0.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau1.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau1.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau10.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau10.root
#./bin/quickKinematics.exe -c configs/2017/mumu_HtoSS_MS2_ctau100.yaml --2018 -u true -o plots/kinematicDistributions/postLepSkim_legacy/output_HtoSS_MS2_ctau100.root
#for some reason, -u false doesn't work, default is false anyway
#should be able to run on other MC, eg, QCD
#./bin/quickIsoCustom.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/IsoDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 -w plots/weightDistributions/output_HtoSS_MS2_ctau0.root 
#./bin/quickIsoCustom.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/IsoDistributions/output_HtoSS_MS2_ctau0_new.root --flow 1 --fhigh 5
# GenPlotter
#./bin/quickGenPlotter.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/genDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 -v true -n 500
#./bin/quickGenPlotter.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/genDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5
#./bin/quickWeightGenerator.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/weightDistributions/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 --hack false
#./bin/quickIsoCustom.exe -c configs/2017/datasets/ggH/ggH_HToSS_MH125_MS2_ctauS0.yaml -o plots/IsoDistributions_new/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 -w plots/weightDistributions/output_HtoSS_MS2_ctau0.root --hack true 
./bin/quickIsoCustom.exe -c configs/2017/datasets/ggH/ggH_HToSS_MH125_MS1_ctauS0.yaml -o plots/IsoDistributions_new/output_HtoSS_MS1_ctau0.root --flow 1 --fhigh 1 -w "" --hack true 
#./bin/RDataFrames_test.exe -c configs/2017/datasets/ggH/ggH_HToSS_MH125_MS2_ctauS0.yaml -o plots/test_RDataFrames.root --flow 1 --fhigh 5 -w plots/weightDistributions/output_HtoSS_MS2_ctau0.root --hack true 
#./bin/quickIsoCustom.exe -c configs/2017/mumu_HtoSS_MS2_ctau0.yaml -o plots/IsoDistributions_new/output_HtoSS_MS2_ctau0.root --flow 1 --fhigh 5 -w plots/weightDistributions/output_HtoSS_MS2_ctau0.root --hack false
