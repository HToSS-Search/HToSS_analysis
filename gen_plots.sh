python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_m_refit_kaon --input plots/KaonMassAssumption/EventPreselection_RefittedTrks_231006/total/output_HtoSS_MS2_ctau0.root --h1 h_DiChHadMass --h2 h_DiChHadMass --leg1 "" --leg2 "" --xtitle "Reco. m_{hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 5 --rebin 50 --yhigh 1.1 --onlyone --drawhist

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recomu_m_refit --input plots/KaonMassAssumption/EventPreselection_RefittedTrks_231006/total/output_HtoSS_MS2_ctau0.root --h1 h_DiMuonMass --h2 h_DiMuonMass --leg1 "" --leg2 "" --xtitle "Reco. m_{#mu#mu} (GeV)" --ytitle "Events" --xlow 0. --xhigh 5 --rebin 50 --yhigh 1.1 --onlyone --drawhist

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recomu_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_recoLeadingMuonPt --h2 h_recoSubleadingMuonPt --leg1 "leading #mu" --leg2 "subleading #mu" --xtitle "Reco. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5 --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/genmu_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_genLeadingMuonPt --h2 h_genSubleadingMuonPt --leg1 "leading #mu" --leg2 "subleading #mu" --xtitle "Gen. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5 --norm

# python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/genpi_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 genLeadingPionPt --h2 genSubleadingPionPt --leg1 "leading #pi^{\pm}" --leg2 "subleading #pi^{\pm}" --xtitle "Gen. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/genh_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_genLeadingChsPt --h2 h_genSubleadingChsPt --leg1 "leading h" --leg2 "subleading h" --xtitle "Gen. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5 --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_pt_matched --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsPt_truth --h2 h_subLeadingChsPt_truth --leg1 "leading h" --leg2 "subleading h" --xtitle "Reco. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 2 --yhigh 1.5 --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_pt_nonmatched --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsPt_failed --h2 h_subleadingChsPt_failed --leg1 "leading h" --leg2 "subleading h" --xtitle "Reco. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 2 --yhigh 1.5 --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_chsPt1 --h2 h_chsPt2 --leg1 "leading h" --leg2 "subleading h" --xtitle "Reco. p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5 --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/dih_pt_compare --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_diChsPt_truth --h2 h_diChsPt_failed --leg1 "gen matched" --leg2 "non-gen matched" --xtitle "Reco. p_{T,hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 100. --rebin 2 --yhigh 1.5

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/dih_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_diChsPt --h2 h_diChsPt --leg1 "" --leg2 "" --xtitle "Reco. p_{T,hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 100. --rebin 2 --yhigh 1.5 --onlyone

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/dimu_pt --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_recoDimuonPt --h2 h_recoDimuonPt --leg1 "" --leg2 "" --xtitle "Reco. p_{T,#mu#mu} (GeV)" --ytitle "Events" --xlow 0. --xhigh 100. --rebin 1 --yhigh 1.5 --onlyone

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/dih_pt_compare --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_diChsPt_truth --h2 h_diChsPt_failed --leg1 "gen matched" --leg2 "non-gen matched" --xtitle "Reco. p_{T,hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 100. --rebin 4 --yhigh 1.5

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/leading_iso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsIso_truth --h2 h_leadingMuonIso --leg1 "leading h" --leg2 "leading #mu" --xtitle "Rel. isolation leading object" --ytitle "Normalized Events" --xlow 0. --xhigh 5. --rebin 4 --yhigh 1.5 --log  --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/leading_chiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsSumPtCh_truth --h2 h_leadingMuonSumPtCh --leg1 "leading h" --leg2 "leading #mu" --xtitle "#Sigma_{ch.}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/leading_nhiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsSumPtNh_truth --h2 h_leadingMuonSumPtNh --leg1 "leading h" --leg2 "leading #mu" --xtitle "#Sigma_{nh.}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/leading_phiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsSumPtPh_truth --h2 h_leadingMuonSumPtPh --leg1 "leading h" --leg2 "leading #mu" --xtitle "#Sigma_{#gamma}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/leading_puiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_leadingChsSumPtPu_truth --h2 h_leadingMuonSumPtPu --leg1 "leading h" --leg2 "leading #mu" --xtitle "#Sigma_{PU}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm


python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_iso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsIso_truth --h2 h_subleadingMuonIso --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "Rel. isolation leading object" --ytitle "Normalized Events" --xlow 0. --xhigh 5. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_chiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsSumPtCh_truth --h2 h_subleadingMuonSumPtCh --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "#Sigma_{ch.}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_nhiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsSumPtNh_truth --h2 h_subleadingMuonSumPtNh --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "#Sigma_{nh.}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_phiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsSumPtPh_truth --h2 h_subleadingMuonSumPtPh --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "#Sigma_{#gamma}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_puiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsSumPtPu_truth --h2 h_subleadingMuonSumPtPu --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "#Sigma_{PU}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm



python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/mu_dr --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_genDimuonDeltaR --h2 h_genDimuonDeltaR --leg1 "" --leg2 "" --xtitle "Gen. #Delta R_{#mu#mu}" --ytitle "Normalized Events" --xlow 0. --xhigh 0.5 --rebin 1 --yhigh 1.5 --log --norm --onlyone

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/h_dr --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_genDiChsDeltaR --h2 h_genDiChsDeltaR --leg1 "" --leg2 "" --xtitle "Gen. #Delta R_{hh}" --ytitle "Normalized Events" --xlow 0. --xhigh 0.5 --rebin 1 --yhigh 1.5 --log --norm --onlyone


python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_dr --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_ChsdelR_truth --h2 h_ChsdelR_truth --leg1 "" --leg2 "" --xtitle "Reco. #Delta R_{hh}" --ytitle "Normalized Events" --xlow 0. --xhigh 0.5 --rebin 1 --yhigh 1.5 --log --norm --onlyone

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recomu_dr --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_recoDimuonDeltaR --h2 h_recoDimuonDeltaR --leg1 "" --leg2 "" --xtitle "Reco. #Delta R_{#mu#mu}" --ytitle "Normalized Events" --xlow 0. --xhigh 0.5 --rebin 1 --yhigh 1.5 --log --norm --onlyone


python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recomu_m --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_recoDimuonMass --h2 h_recoDimuonMass --leg1 "" --leg2 "" --xtitle "Reco. m_{#mu#mu} (GeV)" --ytitle "Events" --xlow 0. --xhigh 5 --rebin 1 --yhigh 1.1 --onlyone --drawhist

python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_m --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_diChsMass --h2 h_diChsMass --leg1 "" --leg2 "" --xtitle "Reco. m_{hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 5 --rebin 1 --yhigh 1.1 --onlyone --drawhist


python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/recoh_m_compare --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_diChsMass_truth --h2 h_diChsMass_failed --leg1 "gen matched" --leg2 "non-gen matched" --xtitle "Reco. m_{hh} (GeV)" --ytitle "Events" --xlow 0. --xhigh 5 --rebin 2 --yhigh 1.1 --drawhist



# python3 scripts/plot_on_same.py --output ~/public_html/HToSS_plots/Stacked/plots_of_old/subleading_puiso --input plots/genDistributions/output_HtoSS_MS2_ctau0.root --h1 h_subleadingChsSumPtPu_truth --h2 h_subleadingMuonSumPtPu --leg1 "subleading h" --leg2 "subleading #mu" --xtitle "#Sigma_{PU}p_{T} (GeV)" --ytitle "Normalized Events" --xlow 0. --xhigh 30. --rebin 2 --yhigh 1.5 --log --norm
