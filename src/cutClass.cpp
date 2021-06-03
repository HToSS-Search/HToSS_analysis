#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "cutClass.hpp"

#include <boost/functional/hash.hpp>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <yaml-cpp/yaml.h>

Cuts::Cuts(const bool doPlots,
           const bool fillCutFlows,
           const bool invertLepCut,
           const bool is2016,
           const bool is2016APV,
           const bool is2018)
    : doPlots_{doPlots}
    , fillCutFlow_{fillCutFlows}
    , invertLepCut_{invertLepCut}
    , is2016_{is2016}
    , is2016APV_{is2016APV}
    , is2018_{is2018}
    , usingBparking_{false}

    , numTightEle_{0}
    , tightElePt_{0.}
    , tightElePtLeading_{0.}
    , tightEleEta_{5.0}
    , tightEleEtaLeading_{5.0}
    , tightEleRelIso_{0.107587}

    , numLooseEle_{0}
    , looseElePt_{0.}
    , looseElePtLeading_{0.}
    , looseEleEta_{5.0}
    , looseEleEtaLeading_{5.0}
    , looseEleRelIso_{0.15}

    , numTightMu_{0}
    , tightMuonPt_{0.}
    , tightMuonPtLeading_{0.}
    , tightMuonEta_{5.0}
    , tightMuonEtaLeading_{5.0}
    , tightMuonRelIso_{0.15}

    , numLooseMu_{0}
    , looseMuonPt_{0.}
    , looseMuonPtLeading_{0.}
    , looseMuonEta_{5.0}
    , looseMuonEtaLeading_{5.0}
    , looseMuonRelIso_{0.25}

    , scalarMassCut_{4.}
    , maxDileptonDeltaR_{0.4}
    , chsMass_{0.13957018}
    , maxChsDeltaR_ {0.4}
    , higgsMassCut_{999.}
    , higgsTolerence_ {10.}
    , invWMassCut_{999999.}
    , unblind_{false}

    , numJets_{0}
    , maxJets_{20}
    , jetPt_{0.}
    , jetEta_{5.0}
    , jetIDDo_{true}

    , numbJets_{0}
    , maxbJets_{20}
    , maxbJetEta_{5.0}

    , bDiscCut_{is2016 ? 0.8484f : 0.8838f}

    , numcJets_{1}

    , rc_{is2016 ? "scaleFactors/2016/RoccoR2016.txt"
                 : "scaleFactors/2017/RoccoR2017.txt"}

    , isMC_{true}

    , isNPL_{false}
    , isZplusCR_{false}

    , postLepSelTree_{nullptr}

    // Skips running trigger stuff
    , skipTrigger_{false}
    // Skips scalar mass cuts
    , skipScalarMassCut_{false}

    // Are we making b-tag efficiency plots?
    , makeBTagEffPlots_{false}
    , getBTagWeight_{false}

    // MET and mTW cuts go here.
    , metDileptonCut_{50.0}

{
    std::cout << "\nInitialises fine" << std::endl;
    initialiseJECCors();
    std::cout << "Gets past JEC Cors" << std::endl;

    if (is2016_) { // 2016 G-H
/*
        std::cout << "\nLoad 2016 (G-H) electron SFs from root file ... " << std::endl;

        // Single electron HLT SF
        electronHltFile = new TFile("scaleFactors/2016/HLT_Ele32_eta2p1_WPTight_Gsf_FullRunRange.root");
        h_eleHlt = dynamic_cast<TH2F*>(electronHltFile->Get("SF"));

        // Electron cut-based ID
        electronSFsFile = new TFile("scaleFactors/2016/egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root");
        h_eleSFs = dynamic_cast<TH2F*>(electronSFsFile->Get("EGamma_SF2D"));

        // Electron reco SF
        electronRecoFile = new TFile{"scaleFactors/2016/egammaEffi_ptAbove20.txt_EGM2D_UL2016postVFP.root"};
        h_eleReco = dynamic_cast<TH2F*>(electronRecoFile->Get("EGamma_SF2D"));

        maxEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmax() - 0.1};
        maxEleIdReco_ = {h_eleReco->GetXaxis()->GetXmax() - 0.1};
        minEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmin() + 0.1};
        minEleIdReco_ = {h_eleReco->GetXaxis()->GetXmin() + 0.1};

        std::cout << "Got 2016 (G-H) electron SFs!\n" << std::endl;
*/
        std::cout << "Load 2016 (G-H) muon SFs from root file ... " << std::endl;

        muonHltFile = new TFile{"scaleFactors/2016/HLT_Mu24_EfficienciesAndSF_RunGtoH.root"};
        muonIdFile  = new TFile{"scaleFactors/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root"};
        muonIsoFile = new TFile{"scaleFactors/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ISO.root"};

        // Single Muon HLT SF
        muonHltFile->cd("IsoMu24_OR_IsoTkMu24_PtEtaBins");
        h_muonHlt = dynamic_cast<TH2F*>(muonHltFile->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio"));

        // Loose ID
        h_muonId     = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt"));
        h_muonIdSyst = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_syst"));
        h_muonIdStat = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_stat"));

        // Loose Iso
        h_muonIso     = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt"));
        h_muonIsoSyst = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_syst"));
        h_muonIsoStat = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_stat"));

        maxMuonIdPt_  = {h_muonId->GetXaxis()->GetXmax() - 0.1};
        maxMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmax() - 0.1};
        maxMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmax() - 0.1};

        minMuonIdPt_  = {h_muonId->GetXaxis()->GetXmin() + 0.1};
        minMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmin() + 0.1};
        minMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmin() +  0.1};

        std::cout << "Got 2016 (G-H) muon SFs!\n" << std::endl;
    }

    else if (is2016APV_) { // 2016 B-F
/*
  	std::cout << "\nLoad 2016 (B-F) electron SFs from root file ... " << std::endl;

        // Single electron HLT SF
        electronHltFile = new TFile("scaleFactors/2016/HLT_Ele32_eta2p1_WPTight_Gsf_FullRunRange.root");
        h_eleHlt = dynamic_cast<TH2F*>(electronHltFile->Get("SF"));

        // Electron cut-based ID
        electronSFsFile = new TFile("scaleFactors/2016/egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root");
        h_eleSFs = dynamic_cast<TH2F*>(electronSFsFile->Get("EGamma_SF2D"));

        // Electron reco SF
        electronRecoFile = new TFile{"scaleFactors/2016/egammaEffi_ptAbove20.txt_EGM2D_UL2016preVFP.root"};
        h_eleReco = dynamic_cast<TH2F*>(electronRecoFile->Get("EGamma_SF2D"));

        maxEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmax() - 0.1};
        maxEleIdReco_ = {h_eleReco->GetXaxis()->GetXmax() - 0.1};
        minEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmin() + 0.1};
        minEleIdReco_ = {h_eleReco->GetXaxis()->GetXmin() + 0.1};

        std::cout << "Got 2016 (B-F) electron SFs!\n" << std::endl;
*/
        std::cout << "Load 2016 (B-F) muon SFs from root file ... " << std::endl;

//        muonHltFile = new TFile{"scaleFactors/2016/HLT_Mu24_EfficienciesAndSF_RunBtoF.root"};
        muonIdFile  = new TFile{"scaleFactors/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root"};
        muonIsoFile = new TFile{"scaleFactors/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ISO.root"};

        // Single Muon HLT SF
//        muonHltFile->cd("IsoMu24_OR_IsoTkMu24_PtEtaBins");
//        h_muonHlt = dynamic_cast<TH2F*>(muonHltFile->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio"));

        // Loose ID
        h_muonId     = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt"));
        h_muonIdSyst = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_syst"));
        h_muonIdStat = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_stat"));

        // Loose Iso
        h_muonIso     = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt"));
        h_muonIsoSyst = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_syst"));
        h_muonIsoStat = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_stat"));

        maxMuonIdPt_  = {h_muonId->GetXaxis()->GetXmax() - 0.1};
        maxMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmax() - 0.1};
//        maxMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmax() - 0.1};

        minMuonIdPt_  = {h_muonId->GetXaxis()->GetXmin() + 0.1};
        minMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmin() + 0.1};
//        minMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmin() +  0.1};

        std::cout << "Got 2016 (B-F) muon SFs!\n" << std::endl;
    }

    else if (!is2016_ && !is2016APV_ && !is2018_) { // 2017
/*
  	std::cout << "\nLoad 2017 electron SFs from root file ... " << std::endl;

        // Single electron HLT SF
        electronHltFile = new TFile("scaleFactors/2016/HLT_Ele32_eta2p1_WPTight_Gsf_FullRunRange.root");
        h_eleHlt = dynamic_cast<TH2F*>(electronHltFile->Get("SF"));

        // Electron cut-based ID
        electronSFsFile = new TFile("scaleFactors/2017/egammaEffi.txt_EGM2D_Tight_UL17.root");
        h_eleSFs = dynamic_cast<TH2F*>(electronSFsFile->Get("EGamma_SF2D"));

        // Electron reco SF
        electronRecoFile = new TFile{"scaleFactors/2017/egammaEffi_ptAbove20.txt_EGM2D_UL2017"};
        h_eleReco = dynamic_cast<TH2F*>(electronRecoFile->Get("EGamma_SF2D"));

        maxEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmax() - 0.1};
        maxEleIdReco_ = {h_eleReco->GetXaxis()->GetXmax() - 0.1};
        minEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmin() + 0.1};
        minEleIdReco_ = {h_eleReco->GetXaxis()->GetXmin() + 0.1};

        std::cout << "Got 2017 electron SFs!\n" << std::endl;
*/

        std::cout << "Load 2017 muon SFs from root file ... " << std::endl;

//        muonHltFile = new TFile{"scaleFactors/2017/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root"};
        muonIdFile  = new TFile{"scaleFactors/2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root"};
        muonIsoFile = new TFile{"scaleFactors/2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ISO.root"};

        // Single Muon HLT SF
//        muonHltFile->cd("IsoMu27_PtEtaBins");
//        h_muonHlt = dynamic_cast<TH2F*>(muonHltFile->Get("IsoMu27_PtEtaBins/abseta_pt_ratio"));

        // Loose ID
        h_muonId     = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt"));
        h_muonIdSyst = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_syst"));
        h_muonIdStat = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_stat"));

        // Loose Iso
        h_muonIso     = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt"));
        h_muonIsoSyst = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_syst"));
        h_muonIsoStat = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_stat"));

        maxMuonIdPt_  = h_muonId->GetXaxis()->GetXmax() - 0.1;
        maxMuonIsoPt_ = h_muonIso->GetXaxis()->GetXmax() - 0.1;
//	    maxMuonHltPt_ = h_muonHlt->GetYaxis()->GetXmax() - 0.1;

        minMuonIdPt_  = h_muonId->GetXaxis()->GetXmin() + 0.1;
        minMuonIsoPt_ = h_muonIso->GetXaxis()->GetXmin() + 0.1;
//        minMuonHltPt_ = h_muonHlt->GetYaxis()->GetXmin() +  0.1;

        std::cout << "Got 2017 muon SFs!\n" << std::endl;
    }
    else { // 2018
/*
        std::cout << "\nLoad 2018 electron SFs from root file ... " << std::endl;

        // Single electron HLT SF
        electronHltFile = new TFile("scaleFactors/2016/HLT_Ele32_eta2p1_WPTight_Gsf_FullRunRange.root");
        h_eleHlt = dynamic_cast<TH2F*>(electronHltFile->Get("SF"));

        // Electron cut-based ID
        electronSFsFile = new TFile("scaleFactors/2018/egammaEffi.txt_EGM2D_Tight_UL18.root");
        h_eleSFs = dynamic_cast<TH2F*>(electronSFsFile->Get("EGamma_SF2D"));

        // Electron reco SF
        electronRecoFile = new TFile{"scaleFactors/2018/egammaEffi_ptAbove20.txt_EGM2D_UL2018"};
        h_eleReco = dynamic_cast<TH2F*>(electronRecoFile->Get("EGamma_SF2D"));

        maxEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmax() - 0.1};
        maxEleIdReco_ = {h_eleReco->GetXaxis()->GetXmax() - 0.1};
        minEleIdPt_   = {h_eleSFs->GetXaxis()->GetXmin() + 0.1};
        minEleIdReco_ = {h_eleReco->GetXaxis()->GetXmin() + 0.1};

        std::cout << "Got 2017 electron SFs!\n" << std::endl;
*/

        std::cout << "Load 2018 muon SFs from root file ... " << std::endl;
//        muonHltFile = new TFile{"scaleFactors/2017/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root"};
        muonIdFile  = new TFile{"scaleFactors/2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root"};
        muonIsoFile = new TFile{"scaleFactors/2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ISO.root"};

        // Single Muon HLT SF
//        muonHltFile->cd("IsoMu27_PtEtaBins");
//        h_muonHlt = dynamic_cast<TH2F*>(muonHltFile->Get("IsoMu27_PtEtaBins/abseta_pt_ratio"));

        // Loose ID
        h_muonId     = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt"));
        h_muonIdSyst = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_syst"));
        h_muonIdStat = dynamic_cast<TH2F*>(muonIdFile->Get("NUM_LooseID_DEN_TrackerMuons_abseta_pt_stat"));

        // Loose Iso
        h_muonIso     = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt"));
        h_muonIsoSyst = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_syst"));
        h_muonIsoStat = dynamic_cast<TH2F*>(muonIsoFile->Get("NUM_LooseRelIso_DEN_LooseID_abseta_pt_stat"));

        maxMuonIdPt_  = {h_muonId->GetXaxis()->GetXmax() - 0.1};
        maxMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmax() - 0.1};
//        maxMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmax() - 0.1};

        minMuonIdPt_  = {h_muonId->GetXaxis()->GetXmin() + 0.1};
        minMuonIsoPt_ = {h_muonIso->GetXaxis()->GetXmin() + 0.1};
//        minMuonHltPt_ = {h_muonHlt->GetYaxis()->GetXmin() +  0.1};

        std::cout << "Got 2018 muon SFs!\n" << std::endl;
    }
}

Cuts::~Cuts()
{
//    electronSFsFile->Close();
//    electronRecoFile->Close();
    muonHltFile->Close();
    muonIdFile->Close();
    muonIsoFile->Close();
}

void Cuts::parse_config(const std::string confName)
{
    // Get the configuration file
    const YAML::Node config{YAML::LoadFile(confName)};

    if (config["trigLabel"]) {
        cutConfTrigLabel_ = config["trigLabel"].as<std::string>();
    }
    if (config["plotPostfix"]) {
        postfixName_ = config["plotPostfix"].as<std::string>();
    }

    const YAML::Node cuts{config["cuts"]};

    const YAML::Node tight_eles{cuts["tightElectrons"]};
    tightElePt_ = tight_eles["pt"].as<double>();
    tightElePtLeading_ = tight_eles["ptLeading"].as<double>();
    tightEleEta_ = tight_eles["eta"].as<double>();
    tightEleEtaLeading_ = tight_eles["etaLeading"].as<double>();
    tightEleRelIso_ = tight_eles["relIso"].as<double>();
    numTightEle_ = tight_eles["number"].as<unsigned>();

    const YAML::Node loose_eles{cuts["looseElectrons"]};
    looseElePt_ = loose_eles["pt"].as<double>();
    looseElePtLeading_ = loose_eles["ptLeading"].as<double>();
    looseEleEta_ = loose_eles["eta"].as<double>();
    looseEleEtaLeading_ = loose_eles["etaLeading"].as<double>();
    looseEleRelIso_ = loose_eles["relIso"].as<double>();
    numLooseEle_ = loose_eles["number"].as<unsigned>();

    const YAML::Node tight_mus{cuts["tightMuons"]};
    tightMuonPt_ = tight_mus["pt"].as<double>();
    tightMuonPtLeading_ = tight_mus["ptLeading"].as<double>();
    tightMuonEta_ = tight_mus["eta"].as<double>();
    tightMuonEtaLeading_ = tight_mus["etaLeading"].as<double>();
    tightMuonRelIso_ = tight_mus["relIso"].as<double>();
    numTightMu_ = tight_mus["number"].as<unsigned>();

    const YAML::Node loose_mus{cuts["looseMuons"]};
    looseMuonPt_ = loose_mus["pt"].as<double>();
    looseMuonPtLeading_ = loose_mus["ptLeading"].as<double>();
    looseMuonEta_ = loose_mus["eta"].as<double>();
    looseMuonEtaLeading_ = loose_mus["etaLeading"].as<double>();
    looseMuonRelIso_ = loose_mus["relIso"].as<double>();
    numLooseMu_ = loose_mus["number"].as<unsigned>();

    const YAML::Node jets{cuts["jets"]};
    jetPt_ = jets["pt"].as<double>();
    jetEta_ = jets["eta"].as<double>();
    numJets_ = jets["numJets"].as<unsigned>();
    maxJets_ = jets["maxJets"].as<unsigned>();
    numbJets_ = jets["numbJets"].as<unsigned>();
    maxbJets_ = jets["maxbJets"].as<unsigned>();
    maxbJetEta_ = jets["maxbJetEta"].as<double>();
    // numcJets_ = jets["numcJets"].as<unsigned>();

    std::cerr << "And so it's looking for " << numTightMu_ << " muons and "
              << numTightEle_ << " electrons" << std::endl;
}

bool Cuts::makeCuts(AnalysisEvent& event, double& eventWeight, std::map<std::string, std::shared_ptr<Plots>>& plotMap, TH1D& cutFlow, const int systToRun) {

    if (!skipTrigger_) {
        if (!triggerCuts(event, eventWeight, systToRun))  return false; // Do trigger cuts
    }

    if ( !event.metFilters() ) return false;

    // Make lepton cuts. If the trigLabel contains d, we are in the ttbar CR so the Z mass cut is skipped
    ////  Do this outside original function because this is simpler for HToSS unlike in tZq
    ////  if (!makeLeptonCuts(event, eventWeight, plotMap, cutFlow, systToRun)) return false;

    event.muonIndexTight = getLooseMuons(event);
    if (event.muonIndexTight.size() < numTightMu_) return false;

    const bool validDileptonCand = getDileptonCand(event, event.muonIndexTight);
    if ( !validDileptonCand ) return false;

    const double dileptonMass {(event.zPairLeptons.first + event.zPairLeptons.second).M()};

    // This is to make some skims for faster running. Do lepSel and save some files. If flag is true, scalar mass cuts are applied, and dilepton mass <= threshold, fill tree
    if (postLepSelTree_ && dileptonMass <= scalarMassCut_ && !skipScalarMassCut_) postLepSelTree_->Fill();

////    eventWeight *= getLeptonWeight(event, systToRun);
//    event.muonMomentumSF = getRochesterSFs(event);

    // Get CHS
    event.chsIndex = getChargedHadronTracks(event);
    if ( event.chsIndex.size() < 2 ) return false;

    const bool validDihadronCand = getDihadronCand(event, event.chsIndex);

    if (doPlots_ || fillCutFlow_) std::tie(event.jetIndex, event.jetSmearValue) = makeJetCuts(event, systToRun, eventWeight, false);
    if (doPlots_) plotMap["lepSel"]->fillAllPlots(event, eventWeight);
    if (doPlots_ || fillCutFlow_) cutFlow.Fill(0.5, eventWeight);

    // If dilepton mass is greater than threshold value, return false
    if ( dileptonMass > scalarMassCut_ && !skipScalarMassCut_ ) return false;

    if (doPlots_ || fillCutFlow_) std::tie(event.jetIndex, event.jetSmearValue) = makeJetCuts(event, systToRun, eventWeight, false);
    if (doPlots_) plotMap["zMass"]->fillAllPlots(event, eventWeight);
    if (doPlots_ || fillCutFlow_) cutFlow.Fill(1.5, eventWeight);

/////

    std::tie(event.jetIndex, event.jetSmearValue) =  makeJetCuts(event, systToRun, eventWeight, true);
    event.bTagIndex = makeBCuts(event, event.jetIndex, systToRun);

    if (!validDihadronCand) return false;
    if ( (event.chsPairVec.first + event.chsPairVec.second).M() > scalarMassCut_ && !skipScalarMassCut_ ) return false;

    //// Apply side band cut for data
    if (!isMC_ && !unblind_) {
        float muScalarMass ( (event.zPairLeptons.first + event.zPairLeptons.second).M() ), hadScalarMass ( (event.chsPairTrkVec.first + event.chsPairTrkVec.second).M() );
        if ( muScalarMass  < hadScalarMass*1.05 && muScalarMass  >= hadScalarMass*0.95 ) return false;
        else if ( hadScalarMass < muScalarMass*1.05  && hadScalarMass >= muScalarMass*0.83  ) return false;
    }

    if (doPlots_ || fillCutFlow_) cutFlow.Fill(2.5, eventWeight);
    if (doPlots_) plotMap["trackSel"]->fillAllPlots(event, eventWeight);

//    if (event.jetIndex.size() < numJets_) return false;
//    if (event.jetIndex.size() > maxJets_) return false;
//    if (event.bTagIndex.size() < numbJets_) return false;
//    if (event.bTagIndex.size() > maxbJets_) return false;

//    if ( event.zPairLeptons.first.DeltaR(event.zPairLeptons.second) < 0.3 ) return false;
//    if ( event.chsPairVec.first.DeltaR(event.chsPairVec.second) < 0.3 ) return false;

//    if ( ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).M() - 125.2) > higgsMassCut_ && !skipScalarMassCut_ ) return false;

//    if (doPlots_) plotMap["higgsSel"]->fillAllPlots(event, eventWeight);
//    if (doPlots_ || fillCutFlow_) cutFlow.Fill(3.5, eventWeight);

    return true;
}

std::vector<double> Cuts::getRochesterSFs(const AnalysisEvent& event) const {
    std::vector<double> SFs{};

    for (auto muonIt = event.muonIndexTight.begin(); muonIt != event.muonIndexTight.end(); muonIt++) {
        double tempSF{1.0};
        if (isMC_) {
            if (event.genMuonPF2PATPT[*muonIt] > 0) { // matched gen muon
                tempSF = rc_.kSpreadMC(event.muonPF2PATCharge[*muonIt], event.muonPF2PATPt[*muonIt], event.muonPF2PATEta[*muonIt], event.muonPF2PATPhi[*muonIt], event.genMuonPF2PATPT[*muonIt]);
            }
            else {
                static std::uniform_real_distribution<> u{0, 1};

                // We need a uniformly distributed "random" number, but this
                // should be the same every time, e.g. when we are looking at
                // systematics. So we will seed the random number generator
                // with a hash combining the properties of the muon (and make
                // the hopefully safe assumption no two muons have EXACTLY
                // the same properties within the same event)
                size_t seed{0};
                boost::hash_combine(seed, event.muonPF2PATCharge[*muonIt]);
                boost::hash_combine(seed, event.muonPF2PATPt[*muonIt]);
                boost::hash_combine(seed, event.muonPF2PATEta[*muonIt]);
                boost::hash_combine(seed, event.muonPF2PATPhi[*muonIt]);
                boost::hash_combine(seed, event.muonPF2PATTkLysWithMeasurements[*muonIt]);
                boost::hash_combine(seed, event.eventNum);

                std::mt19937 gen(seed);

                tempSF = rc_.kSmearMC(event.muonPF2PATCharge[*muonIt], event.muonPF2PATPt[*muonIt], event.muonPF2PATEta[*muonIt], event.muonPF2PATPhi[*muonIt], event.muonPF2PATTkLysWithMeasurements[*muonIt], u(gen));
            }
        }
        else {
            tempSF = rc_.kScaleDT(event.muonPF2PATCharge[*muonIt], event.muonPF2PATPt[*muonIt], event.muonPF2PATEta[*muonIt], event.muonPF2PATPhi[*muonIt]);
        }
        SFs.emplace_back(tempSF);
    }

    return SFs;
}

// Make lepton cuts. Will become customisable in a config later on.
bool Cuts::makeLeptonCuts( AnalysisEvent& event, double& eventWeight, std::map<std::string, std::shared_ptr<Plots>>& plotMap, TH1D& cutFlow, const int& syst ) {

    ////Do lepton selection.

//    event.muonIndexTight = getTightMuons(event);
    event.muonIndexTight = getLooseMuons(event);
    if (event.muonIndexTight.size() < numTightMu_) return false;

    event.muonIndexLoose = getLooseMuons(event);
    if (event.muonIndexLoose.size() < numLooseMu_) return false;

    // This is to make some skims for faster running. Do lepSel and save some
    // files.
    // if (postLepSelTree_) postLepSelTree_->Fill();

//    event.muonMomentumSF = getRochesterSFs(event);

    if ( !getDileptonCand(event, event.muonIndexTight) ) return false;

    // Get CHS
    event.chsIndex = getChargedHadronTracks(event);
    if ( event.chsIndex.size() < 2 ) return false;

    getDihadronCand(event, event.chsIndex);
//    if ( !getDihadronCand(event) ) return false;

//// Apply side band cut for data
    if (!isMC_ && !unblind_) {
        float muScalarMass ( (event.zPairLeptons.first + event.zPairLeptons.second).M() ), hadScalarMass ( (event.chsPairTrkVec.first + event.chsPairTrkVec.second).M() );
        if ( muScalarMass  < hadScalarMass*1.05 && muScalarMass  >= hadScalarMass*.95 ) return false;
        else if ( hadScalarMass < muScalarMass*1.05  && hadScalarMass >= muScalarMass*.95  ) return false;
    }

//    eventWeight *= getLeptonWeight(event, syst);

    if (doPlots_ || fillCutFlow_) std::tie(event.jetIndex, event.jetSmearValue) = makeJetCuts(event, syst, eventWeight, false);
    if (doPlots_) plotMap["lepSel"]->fillAllPlots(event, eventWeight);
    if (doPlots_ || fillCutFlow_) cutFlow.Fill(0.5, eventWeight);


    if (isNPL_) { // if is NPL channel
        double eeWeight{1.0};
        double mumuWeight{1.0};
        double emuWeight{1.0};

        if (numTightEle_ == 2) {
            eventWeight *= eeWeight;
        }
        if (numTightMu_ == 2) {
            eventWeight *= mumuWeight;
        }
        if (numTightEle_ == 1 && numTightMu_ == 1) {
            eventWeight *= emuWeight;
        }
    }

    if ( (event.zPairLeptons.first + event.zPairLeptons.second).M() > scalarMassCut_ && !skipScalarMassCut_ ) return false;

    if (doPlots_ || fillCutFlow_) std::tie(event.jetIndex, event.jetSmearValue) = makeJetCuts(event, syst, eventWeight, false);
    if (doPlots_) plotMap["zMass"]->fillAllPlots(event, eventWeight);
    if (doPlots_ || fillCutFlow_) cutFlow.Fill(1.5, eventWeight);

    return true;
}

std::vector<int> Cuts::getTightEles(const AnalysisEvent& event) const {
    std::vector<int> electrons;

    for (int i{0}; i < event.numElePF2PAT; i++) {
        if (!event.elePF2PATIsGsf[i]) continue;

        if (electrons.size() < 1 && event.elePF2PATPT[i] <= tightElePtLeading_) continue;
        else if (electrons.size() >= 1 && event.elePF2PATPT[i] <= tightElePt_) continue;

        if (electrons.size() < 1 && std::abs(event.elePF2PATSCEta[i]) > tightEleEtaLeading_) continue;
        else if (electrons.size() >= 1 && std::abs(event.elePF2PATSCEta[i]) > tightEleEta_) continue;
 
        // Ensure we aren't in the barrel/endcap gap and below the max safe eta
        // range
        if ((std::abs(event.elePF2PATSCEta[i]) > 1.4442 && std::abs(event.elePF2PATSCEta[i]) < 1.566) || std::abs(event.elePF2PATSCEta[i]) > 2.50) continue;

        // VID cut
        if (event.elePF2PATCutIdTight[i] < 1) continue;

        // Cuts not part of the tuned ID
        if (std::abs(event.elePF2PATSCEta[i]) <= 1.479) {
            if (std::abs(event.elePF2PATD0PV[i]) >= 0.05) continue;
            if (std::abs(event.elePF2PATDZPV[i]) >= 0.10) continue;
        }
        else if (std::abs(event.elePF2PATSCEta[i]) > 1.479 && std::abs(event.elePF2PATSCEta[i]) < 2.50) {
            if (std::abs(event.elePF2PATD0PV[i]) >= 0.10) continue;
            if (std::abs(event.elePF2PATDZPV[i]) >= 0.20) continue;
        }
        electrons.emplace_back(i);
    }
    return electrons;
}

std::vector<int> Cuts::getLooseEles(const AnalysisEvent& event) const {
    std::vector<int> electrons;
    for (int i{0}; i < event.numElePF2PAT; i++) {

        if (electrons.size() < 1 && event.elePF2PATPT[i] <= looseElePtLeading_) continue;
        else if (electrons.size() >= 1 && event.elePF2PATPT[i] <= looseElePt_) continue;

        if (electrons.size() < 1 && std::abs(event.elePF2PATSCEta[i]) > tightEleEtaLeading_) continue;
        else if (electrons.size() >= 1 && std::abs(event.elePF2PATSCEta[i]) > tightEleEta_) continue;

        // Ensure we aren't in the barrel/endcap gap and below the max safe eta range
        if ((std::abs(event.elePF2PATSCEta[i]) > 1.4442 && std::abs(event.elePF2PATSCEta[i]) < 1.566) || std::abs(event.elePF2PATSCEta[i]) > 2.50) continue;

        // VID cut
        if (!event.elePF2PATCutIdVeto[i]) continue;

        // Cuts not part of the tuned ID
        if (std::abs(event.elePF2PATSCEta[i]) <= 1.479) {
            if (std::abs(event.elePF2PATD0PV[i]) >= 0.05) continue;
            if (std::abs(event.elePF2PATDZPV[i]) >= 0.10) continue;
        }
        else if (std::abs(event.elePF2PATSCEta[i]) > 1.479 && std::abs(event.elePF2PATSCEta[i]) < 2.50) {
            if (std::abs(event.elePF2PATD0PV[i]) >= 0.10) continue;
            if (std::abs(event.elePF2PATDZPV[i]) >= 0.20) continue;
        }
        electrons.emplace_back(i);
    }
    return electrons;
}

std::vector<int> Cuts::getTightMuons(const AnalysisEvent& event) const {
    std::vector<int> muons;
    if (is2016_ || is2016APV_) {
        for (int i{0}; i < event.numMuonPF2PAT; i++) {
            if (!event.muonPF2PATIsPFMuon[i])
                continue;

            if (muons.size() < 1 && event.muonPF2PATPt[i] <= tightMuonPtLeading_) continue;
            else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= tightMuonPt_) continue;

            if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) >= tightMuonEtaLeading_) continue;
            else if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) >= tightMuonEta_) continue;

            if (event.muonPF2PATComRelIsodBeta[i] >= tightMuonRelIso_) continue;

            // Tight ID Cut
            if (!event.muonPF2PATTrackID[i]) continue;
            if (!event.muonPF2PATGlobalID[i]) continue;
            if (event.muonPF2PATGlbTkNormChi2[i] >= 10.) continue;
            if (event.muonPF2PATMatchedStations[i] < 2) continue;
            if (std::abs(event.muonPF2PATDBPV[i]) >= 0.2) continue;
            if (std::abs(event.muonPF2PATDZPV[i]) >= 0.5) continue;
            if (event.muonPF2PATMuonNHits[i] < 1) continue;
            if (event.muonPF2PATVldPixHits[i] < 1) continue;
            if (event.muonPF2PATTkLysWithMeasurements[i] <= 5) continue;
            muons.emplace_back(i);
        }
    }
    else {
        for (int i{0}; i < event.numMuonPF2PAT; i++) {
            if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATTightCutId[i] /*&& event.muonPF2PATPfIsoTight[i]*/ ) {

                if (muons.size() < 1 && event.muonPF2PATPt[i] <= tightMuonPtLeading_) continue;
                else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= tightMuonPt_) continue;

                if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) <= tightMuonEtaLeading_) continue;
                if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) <= tightMuonEta_) continue;

                muons.emplace_back(i);
            }
        }
    }
    return muons;
}

std::vector<int> Cuts::getLooseMuons(const AnalysisEvent& event) const {
    std::vector<int> muons;
    if (is2016_ || is2016APV_) {
        for (int i{0}; i < event.numMuonPF2PAT; i++) {
            if (!event.muonPF2PATIsPFMuon[i]) continue;

            if (muons.size() < 1 && event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
            else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= looseMuonPt_) continue;

            if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEtaLeading_) continue;
            else if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;

            if (event.muonPF2PATComRelIsodBeta[i] >= looseMuonRelIso_)  continue;
            if (event.muonPF2PATGlobalID[i] || event.muonPF2PATTrackID[i])  muons.emplace_back(i);
        }
    }
    else {
        for (int i{0}; i < event.numMuonPF2PAT; i++)  {
            if (event.muonPF2PATIsPFMuon[i] /*&& event.muonPF2PATLooseCutId[i]*/ /* && event.muonPF2PATPfIsoVeryLoose[i] */ ) {

               if (muons.size() < 1 && event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
                else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= looseMuonPt_) continue;

                if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEtaLeading_) continue;
                else if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;

                muons.emplace_back(i);
            }
        }
    }
    return muons;
}

std::vector<int> Cuts::getChargedHadronTracks(const AnalysisEvent& event) const {
    std::vector<int> chs;
    for (Int_t k = 0; k < event.numPackedCands; k++) {
        if (std::abs(event.packedCandsPdgId[k]) != 211) continue;
        if (event.packedCandsCharge[k] == 0 ) continue;
        if (event.packedCandsHasTrackDetails[k] != 1 ) continue;
        TLorentzVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
        if (lVec.Pt() < 1.0) continue;

        chs.emplace_back(k);
    }
    return chs;
}

bool Cuts::getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons) const {    // Check if there are at least two electrons first. Otherwise use muons.

    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

            if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;

            TLorentzVector lepton1{event.muonPF2PATPX[muons[i]], event.muonPF2PATPY[muons[i]], event.muonPF2PATPZ[muons[i]], event.muonPF2PATE[muons[i]]};
            TLorentzVector lepton2{event.muonPF2PATPX[muons[j]], event.muonPF2PATPY[muons[j]], event.muonPF2PATPZ[muons[j]], event.muonPF2PATE[muons[j]]};
            const double delR { lepton1.DeltaR(lepton2) };
            if ( delR < maxDileptonDeltaR_  ) {
                event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
                event.zPairIndex.second  = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
                event.zPairRelIso.first  = event.muonPF2PATComRelIsodBeta[muons[i]];
                event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];

                if (!event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first]) continue;
                if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] > 1.0) continue;

                // pf quantities
                float neutral_iso {0.0}, neutral_iso1 {0.0}, neutral_iso2 {0.0};
                float ch_iso {0.0}, ch_iso1 {0.0}, ch_iso2 {0.0};
                float pu_iso {0.0}, pu_iso1 {0.0}, pu_iso2 {0.0};

                // trk quantities
                float neutral_trkiso {0.0}, neutral_trkiso1 {0.0}, neutral_trkiso2 {0.0};
                float ch_trkiso {0.0}, ch_trkiso1 {0.0}, ch_trkiso2 {0.0};
                float pu_trkiso {0.0}, pu_trkiso1 {0.0}, pu_trkiso2 {0.0};
   
                for (int k = 0; k < event.numPackedCands; k++) {
                    if ( k == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] || k == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

                    TLorentzVector packedCandVec, packedCandTrkVec;
                    packedCandVec.SetPxPyPzE      (event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]);
                    packedCandTrkVec.SetPtEtaPhiE (event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);

                    // Charge hadron contributions have to have pT > 0.5
                    if ( event.packedCandsCharge[k] == 0 ) {
                        if ( packedCandVec.Pt() >= 0.5 ) {
                            if ( event.zPairLeptons.first.DeltaR(packedCandVec)   < 0.4 )  neutral_iso1 += packedCandVec.Et();
                            if ( event.zPairLeptons.second.DeltaR(packedCandVec)  < 0.4 )  neutral_iso2 += packedCandVec.Et();
                            if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec)  < 0.4 ) neutral_iso += packedCandVec.Et();
                        }
                        if ( packedCandTrkVec.Pt() >= 0.5 ) {
                            if ( event.zPairLeptons.first.DeltaR(packedCandTrkVec)   < 0.4 )  neutral_trkiso1 += packedCandTrkVec.Et();
                            if ( event.zPairLeptons.second.DeltaR(packedCandTrkVec)  < 0.4 )  neutral_trkiso2 += packedCandTrkVec.Et();
                            if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandTrkVec)  < 0.4 ) neutral_trkiso += packedCandTrkVec.Et();
                        }
                    }
                    else {
                        if ( event.packedCandsFromPV[k] >= 2 ) {
                            if ( event.zPairLeptons.first.DeltaR(packedCandVec)   < 0.4 )  ch_iso1 += packedCandVec.Pt();
                            if ( event.zPairLeptons.second.DeltaR(packedCandVec)  < 0.4 )  ch_iso2 += packedCandVec.Pt();
                            if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec)  < 0.4 ) ch_iso += packedCandVec.Pt();

                            if ( event.zPairLeptons.first.DeltaR(packedCandTrkVec)   < 0.4 )  ch_trkiso1 += packedCandTrkVec.Pt();
                            if ( event.zPairLeptons.second.DeltaR(packedCandTrkVec)  < 0.4 )  ch_trkiso2 += packedCandTrkVec.Pt();
                            if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandTrkVec)  < 0.4 ) ch_trkiso += packedCandTrkVec.Pt();
                        }
                        else {
                            if ( packedCandVec.Pt() >= 0.5 ) {
                                if ( event.zPairLeptons.first.DeltaR(packedCandVec)   < 0.4 )  pu_iso1 += packedCandVec.Pt();
                                if ( event.zPairLeptons.second.DeltaR(packedCandVec)  < 0.4 )  pu_iso2 += packedCandVec.Pt();
                                if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec)  < 0.4 ) pu_iso += packedCandVec.Pt();
                            }
       	                    if ( packedCandTrkVec.Pt() >= 0.5 ) {
                                if ( event.zPairLeptons.first.DeltaR(packedCandTrkVec)   < 0.4 )  pu_trkiso1 += packedCandTrkVec.Pt();
                                if ( event.zPairLeptons.second.DeltaR(packedCandTrkVec)  < 0.4 )  pu_trkiso2 += packedCandTrkVec.Pt();
                                if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandTrkVec)  < 0.4 ) pu_trkiso += packedCandTrkVec.Pt();
                            }
                        }
                    }
                }

                const float iso1 = ch_iso1 + std::max( float(0.0), neutral_iso1 - float(0.5*pu_iso1) );
                const float iso2 = ch_iso2 + std::max( float(0.0), neutral_iso2 - float(0.5*pu_iso2) );
                const float iso  = ch_iso  + std::max( float(0.0), neutral_iso  - float(0.5*pu_iso)  );

                const float trkiso1 = ch_trkiso1 + std::max( float(0.0), neutral_trkiso1 - float(0.5*pu_trkiso1) );
                const float trkiso2 = ch_trkiso2 + std::max( float(0.0), neutral_trkiso2 - float(0.5*pu_trkiso2) );
                const float trkiso  = ch_trkiso  + std::max( float(0.0), neutral_trkiso  - float(0.5*pu_trkiso)  );

                event.zPairNewRelIso.first  = iso1/(event.zPairLeptons.first.Pt() + 1.0e-06);
                event.zPairNewRelIso.second = iso2/(event.zPairLeptons.second.Pt() + 1.0e-06);
                event.zRelIso = iso/((event.zPairLeptons.first+event.zPairLeptons.second).Pt() + 1.0e-06);

                event.zPairNewTrkIso.first  = trkiso1/(event.zPairLeptons.first.Pt() + 1.0e-06);
                event.zPairNewTrkIso.second = trkiso2/(event.zPairLeptons.second.Pt() + 1.0e-06);
                event.zTrkIso = trkiso/((event.zPairLeptons.first+event.zPairLeptons.second).Pt() + 1.0e-06);

//                if ( event.zRelIso > 0.2 ) continue;

                event.mumuTrkIndex = getMuonTrackPairIndex(event);

//                if ( (event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxNdof[event.mumuTrkIndex]+1.0e-06) > 10. ) continue;

                event.zPairLeptonsRefitted.first  = TLorentzVector{event.muonTkPairPF2PATTk1Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk1P2[event.mumuTrkIndex]+std::pow(0.1057,2))};
                event.zPairLeptonsRefitted.second = TLorentzVector{event.muonTkPairPF2PATTk2Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk2P2[event.mumuTrkIndex]+std::pow(0.1057,2))};

                return true;
            }
        }
    }
    return false;
}

bool Cuts::getDihadronCand(AnalysisEvent& event, const std::vector<int>& chs) const {

    for ( unsigned int i{0}; i < chs.size(); i++ ) {

        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

            if (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] >= 0) continue;

            TLorentzVector chs1 {event.packedCandsPx[chs[i]], event.packedCandsPy[chs[i]], event.packedCandsPz[chs[i]], event.packedCandsE[chs[i]]};
            TLorentzVector chs2 {event.packedCandsPx[chs[j]], event.packedCandsPy[chs[j]], event.packedCandsPz[chs[j]], event.packedCandsE[chs[j]]};

            double pT { (chs1+chs2).Pt() };
            double delR { chs1.DeltaR(chs2) };
            double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };

            if ( delR < maxChsDeltaR_ ) {
                event.chsPairVec.first  = chs1.Pt() > chs2.Pt() ? chs1 : chs2;
                event.chsPairVec.second = chs1.Pt() > chs2.Pt() ? chs2 : chs1;
                event.chsPairIndex.first = chs1.Pt() > chs2.Pt() ? chs[i] : chs[j];
                event.chsPairIndex.second = chs1.Pt() > chs2.Pt() ? chs[j] : chs[i];

                TLorentzVector chsTrk1, chsTrk2;
                chsTrk1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.first], event.packedCandsPseudoTrkEta[event.chsPairIndex.first], event.packedCandsPseudoTrkPhi[event.chsPairIndex.first], event.packedCandsE[event.chsPairIndex.first]);
                chsTrk2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.second], event.packedCandsPseudoTrkEta[event.chsPairIndex.second], event.packedCandsPseudoTrkPhi[event.chsPairIndex.second], event.packedCandsE[event.chsPairIndex.second]);

                event.chsPairTrkVec.first  = chsTrk1;
                event.chsPairTrkVec.second = chsTrk2;

                float neutral_iso {0.0}, neutral_iso1 {0.0}, neutral_iso2 {0.0};
                float ch_iso {0.0}, ch_iso1 {0.0}, ch_iso2 {0.0};
                float pu_iso {0.0}, pu_iso1 {0.0}, pu_iso2 {0.0};

                float neutral_trkiso {0.0}, neutral_trkiso1 {0.0}, neutral_trkiso2 {0.0};
                float ch_trkiso {0.0}, ch_trkiso1 {0.0}, ch_trkiso2 {0.0};
                float pu_trkiso {0.0}, pu_trkiso1 {0.0}, pu_trkiso2 {0.0};

                for (int k = 0; k < event.numPackedCands; k++) {
                    if ( k == event.chsPairIndex.first || k == event.chsPairIndex.second ) continue;

                    TLorentzVector packedCandVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
                    TLorentzVector packedCandTrkVec;
                    packedCandTrkVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);

                    if ( event.packedCandsCharge[k] == 0 ) {
                        if ( packedCandVec.Pt() >= 0.5 ) {
                            if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.3 )  neutral_iso1 += packedCandVec.Et();
                            if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.3 )  neutral_iso2 += packedCandVec.Et();
                            if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.3 ) neutral_iso += packedCandVec.Et();
                        }
                        if ( packedCandTrkVec.Pt() >= 0.5 ) {
                            if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)  < 0.3 ) neutral_trkiso1 += packedCandTrkVec.Et();
                            if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec) < 0.3 ) neutral_trkiso2 += packedCandTrkVec.Et();
                            if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec) ) neutral_trkiso += packedCandVec.Et();
                        }
                    }
                    else {
                        if ( event.packedCandsFromPV[k] >= 2 ) {
                            if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.3 )  ch_iso1 += packedCandVec.Pt();
                            if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.3 )  ch_iso2 += packedCandVec.Pt();
                            if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.3 ) ch_iso += packedCandVec.Pt();

                            if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)   < 0.3 )  ch_trkiso1 += packedCandTrkVec.Pt();
                            if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec)  < 0.3 )  ch_trkiso2 += packedCandTrkVec.Pt();
                            if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec)  < 0.3 ) ch_trkiso += packedCandTrkVec.Pt();
                        }
                        else {
                            if ( packedCandVec.Pt() >= 0.5 ) {
                                if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.3 )  pu_iso1 += packedCandVec.Pt();
                                if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.3 )  pu_iso2 += packedCandVec.Pt();
                                if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.3 ) pu_iso += packedCandVec.Pt();
                            }
                            if ( packedCandTrkVec.Pt() >= 0.5 ) {
                                if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)   < 0.3 )  pu_trkiso1 += packedCandTrkVec.Pt();
                                if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec)  < 0.3 )  pu_trkiso2 += packedCandTrkVec.Pt();
                                if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec)  < 0.3 ) pu_trkiso += packedCandTrkVec.Pt();
                            }
                        }
                    }
                }

                const float iso1 = ch_iso1 + std::max( float(0.0), neutral_iso1 - float(0.5*pu_iso1) );
                const float iso2 = ch_iso2 + std::max( float(0.0), neutral_iso2 - float(0.5*pu_iso2) );
                const float iso  = ch_iso  + std::max( float(0.0), neutral_iso  - float(0.5*pu_iso)  );

                const float trkiso1 = ch_trkiso1 + std::max( float(0.0), neutral_trkiso1 - float(0.5*pu_trkiso1) );
                const float trkiso2 = ch_trkiso2 + std::max( float(0.0), neutral_trkiso2 - float(0.5*pu_trkiso2) );
                const float trkiso  = ch_trkiso  + std::max( float(0.0), neutral_trkiso  - float(0.5*pu_trkiso)  );

                event.chsPairRelIso.first = iso1/(event.chsPairVec.first.Pt() + 1.0e-06);
                event.chsPairRelIso.second = iso2/(event.chsPairVec.second.Pt() + 1.0e-06);
                event.chsRelIso = iso/((event.chsPairVec.first+event.chsPairVec.second).Pt() + 1.0e-06);

                event.chsPairTrkIso.first = trkiso1/(event.chsPairTrkVec.first.Pt() + 1.0e-06);
                event.chsPairTrkIso.second = trkiso2/(event.chsPairTrkVec.second.Pt() + 1.0e-06);
                event.chsTrkIso = trkiso/((event.chsPairTrkVec.first+event.chsPairTrkVec.second).Pt() + 1.0e-06);

//                if ( event.chsTrkIso > 0.4 ) continue;

                event.chsPairTrkIndex = getChsTrackPairIndex(event);

                // If refit fails then reject event - all signal events pass refit, but QCD does not
                if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;

                TLorentzVector chsTrk1Refitted, chsTrk2Refitted;
                chsTrk1Refitted.SetPtEtaPhiE(event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                chsTrk2Refitted.SetPtEtaPhiE(event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));	

                event.chsPairTrkVecRefitted.first  = chsTrk1Refitted;
                event.chsPairTrkVecRefitted.second = chsTrk2Refitted;

                return true;
            }
            else continue;
        }
    }
    return false;
}

double Cuts::getWbosonQuarksCand(AnalysisEvent& event, const std::vector<int>& jets,  const int& syst) const {
    auto closestWmass{std::numeric_limits<double>::infinity()};
    if (jets.size() > 2) {
        for (unsigned k{0}; k < jets.size(); k++) {
            for (unsigned l{k + 1}; l < jets.size(); l++) {
                // Now ensure that the leading b jet isn't one of these!
                if (event.bTagIndex.size() > 0) {
                    if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[jets[k]] > bDiscCut_) {
                        if (event.jetIndex[event.bTagIndex[0]] == jets[k])
                            continue;
                    }
                    else if (
                        event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[jets[l]] > bDiscCut_) {
                            if (event.jetIndex[event.bTagIndex[0]] == jets[l]) continue;
                    }
                }
                const TLorentzVector jetVec1{getJetLVec(event, jets[k], syst, false).first};
                const TLorentzVector jetVec2{getJetLVec(event, jets[l], syst, false).first};

                double invWbosonMass{(jetVec1 + jetVec2).M() - 80.385};

                if (std::abs(invWbosonMass) < std::abs(closestWmass)) {
                    event.wPairQuarks.first = jetVec1.Pt() > jetVec2.Pt() ? jetVec1 : jetVec2;
                    event.wPairIndex.first = jetVec1.Pt() > jetVec2.Pt() ? k : l;
                    event.wPairQuarks.second = jetVec1.Pt() > jetVec2.Pt() ? jetVec2 : jetVec1;
                    event.wPairIndex.second = jetVec1.Pt() > jetVec2.Pt() ? l : k;
                    closestWmass = invWbosonMass;
                }
            }
        }
    }
    return closestWmass;
}

double Cuts::getTopMass(const AnalysisEvent& event) const
{
    TLorentzVector bVec(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]],
                        event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]],
                        event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]],
                        event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
    return (bVec + event.wPairQuarks.first + event.wPairQuarks.second).M();
}

std::pair<std::vector<int>, std::vector<double>>
    Cuts::makeJetCuts(const AnalysisEvent& event,
                      const int syst,
                      double& eventWeight,
                      const bool isProper) const
{
    std::vector<int> jets;
    std::vector<double> smears;

    double mcTag{1.};
    double mcNoTag{1.};
    double dataTag{1.};
    double dataNoTag{1.};
    // b-tagging errors
    double err1{0.};
    double err2{0.};
    double err3{0.};
    double err4{0.};

    for (int i{0}; i < event.numJetPF2PAT; i++)
    {
        auto [jetVec, smear] = getJetLVec(event, i, syst, true);
        smears.emplace_back(smear);

        if (jetVec.Pt() <= jetPt_ || jetVec.Eta() >= jetEta_)
        {
            continue;
        }

        bool jetId{true};

        if (jetIDDo_ && isProper)
        {
            if (is2016_ || is2016APV_)
            {
                // Jet ID == loose
                if (std::abs(jetVec.Eta()) <= 2.7)
                { // for cases where jet eta <= 2.7

                    // for all jets with eta <= 2.7
                    if (event.jetPF2PATNeutralHadronEnergyFraction[i] >= 0.99)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] >= 0.99)
                    {
                        jetId = false;
                    }
                    if ((event.jetPF2PATChargedMultiplicity[i]
                         + event.jetPF2PATNeutralMultiplicity[i])
                        <= 1)
                    {
                        jetId = false;
                    }

                    // for jets with eta <= 2.40
                    if (std::abs(jetVec.Eta()) <= 2.40)
                    {
                        if (event.jetPF2PATChargedHadronEnergyFraction[i]
                            <= 0.0)
                        {
                            jetId = false;
                        }
                        if (event.jetPF2PATChargedMultiplicity[i] <= 0.0)
                        {
                            jetId = false;
                        }
                        if (event.jetPF2PATChargedEmEnergyFraction[i] >= 0.99)
                        {
                            jetId = false;
                        }
                    }
                }
                else if (std::abs(jetVec.Eta()) <= 3.0
                         && std::abs(jetVec.Eta()) > 2.70)
                {
                    if (event.jetPF2PATNeutralHadronEnergyFraction[i] >= 0.98)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] <= 0.01)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralMultiplicity[i] <= 2)
                    {
                        jetId = false;
                    }
                }
                else if (std::abs(jetVec.Eta()) > 3.0)
                { // for cases where jet eta > 3.0 and less than 5.0 (or max).
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] >= 0.90)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralMultiplicity[i] <= 10)
                    {
                        jetId = false;
                    }
                }
            }
            else
            {
                // Jet ID == tightLepVeto (loose is deprecated)
                // https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2017
                if (std::abs(jetVec.Eta()) <= 2.7)
                { // for cases where jet eta <= 2.7

                    // for all jets with eta <= 2.7
                    if (event.jetPF2PATNeutralHadronEnergyFraction[i] >= 0.90)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] >= 0.90)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNConstituents[i] <= 1)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATMuonFraction[i] >= 0.8)
                    {
                        jetId = false;
                    }

                    // for jets with eta <= 2.40
                    if (std::abs(jetVec.Eta()) <= 2.40)
                    {
                        if (event.jetPF2PATChargedHadronEnergyFraction[i]
                            <= 0.0)
                        {
                            jetId = false;
                        }
                        if (event.jetPF2PATChargedMultiplicity[i] <= 0.0)
                        {
                            jetId = false;
                        }
                        if (event.jetPF2PATChargedEmEnergyFraction[i] >= 0.8)
                        {
                            jetId = false;
                        }
                    }
                }
                else if (std::abs(jetVec.Eta()) <= 3.0
                         && std::abs(jetVec.Eta()) > 2.70)
                {
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] <= 0.02
                        || event.jetPF2PATNeutralEmEnergyFraction[i] >= 0.99)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralMultiplicity[i] <= 2)
                    {
                        jetId = false;
                    }
                }
                else if (std::abs(jetVec.Eta()) > 3.0)
                { // for cases where jet eta > 3.0 and less than 5.0 (or max).
                    if (event.jetPF2PATNeutralEmEnergyFraction[i] >= 0.90)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralHadronEnergyFraction[i] <= 0.02)
                    {
                        jetId = false;
                    }
                    if (event.jetPF2PATNeutralMultiplicity[i] <= 10)
                    {
                        jetId = false;
                    }
                }
            }
        }

        if (!jetId)
        {
            continue;
        }

        const double deltaLep{std::min(deltaR(event.zPairLeptons.first.Eta(),
                                              event.zPairLeptons.first.Phi(),
                                              jetVec.Eta(),
                                              jetVec.Phi()),
                                       deltaR(event.zPairLeptons.second.Eta(),
                                              event.zPairLeptons.second.Phi(),
                                              jetVec.Eta(),
                                              jetVec.Phi()))};

        if (deltaLep < 0.4 && isProper) continue;

        // if (event.jetPF2PATdRClosestLepton[i] < 0.5) continue;
        if (isMC_ && makeBTagEffPlots_ && isProper) {
            // Fill eff info here if needed.
            if (std::abs(event.jetPF2PATPID[i]) == 5) { // b-jets
                bTagEffPlots_[0]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[i] > bDiscCut_) {
                    bTagEffPlots_[4]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                }
            }
            if (std::abs(event.jetPF2PATPID[i]) == 4)
            { // charm
                bTagEffPlots_[1]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags
                        [i]
                    > bDiscCut_)
                {
                    bTagEffPlots_[5]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                }
            }
            if (std::abs(event.jetPF2PATPID[i]) > 0
                && std::abs(event.jetPF2PATPID[i]) < 4)
            { // light jets
                bTagEffPlots_[2]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags
                        [i]
                    > bDiscCut_)
                {
                    bTagEffPlots_[6]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                }
            }
            if (std::abs(event.jetPF2PATPID[i]) == 21)
            { // gluons
                bTagEffPlots_[3]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags
                        [i]
                    > bDiscCut_)
                {
                    bTagEffPlots_[7]->Fill(jetVec.Pt(), std::abs(jetVec.Eta()));
                }
            }
        }

        jets.emplace_back(i);

        if (getBTagWeight_)
        {
            getBWeight(event,
                       jetVec,
                       i,
                       mcTag,
                       mcNoTag,
                       dataTag,
                       dataNoTag,
                       err1,
                       err2,
                       err3,
                       err4);
        }
    }
    // Evaluate b-tag weight for event here.
    if (getBTagWeight_ && isProper)
    {
        double bWeight{(dataNoTag * dataTag) / (mcNoTag * mcTag)};
        if (mcNoTag == 0 || mcTag == 0 || dataNoTag == 0 || dataTag == 0
            || mcNoTag != mcNoTag || mcTag != mcTag || dataTag != dataTag
            || dataNoTag != dataNoTag)
        {
            bWeight = 1.;
        }
        const double bWeightErr{
            std::sqrt(pow(err1 + err2, 2) + pow(err3 + err4, 2)) * bWeight};
        if (syst == 256)
        {
            bWeight += bWeightErr;
        }
        if (syst == 512)
        {
            bWeight -= bWeightErr;
        }

        eventWeight *= bWeight;
    }

    return {jets, smears};
}

std::vector<int> Cuts::makeBCuts(const AnalysisEvent& event, const std::vector<int> jets, const int syst) const {
    std::vector<int> bJets;
    for (unsigned int i = 0; i < jets.size(); i++) {
        const TLorentzVector jetVec{getJetLVec(event, jets[i], syst, false).first};
        const float bDisc{event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[jets[i]]};

        if (bDisc <= bDiscCut_)
            continue;
        if (jetVec.Eta() >= maxbJetEta_)
            continue;
        bJets.emplace_back(i);
    }
    return bJets;
}

bool Cuts::triggerCuts(const AnalysisEvent& event, double& eventWeight, const int& syst) const {
    if (skipTrigger_) {
        return true;
    }

    // TRIGGER LOGIC

//    const bool muEGTrig{event.muEGTrig()};
//    const bool eeTrig{event.eeTrig()};

    // double muon triggers
//    const bool mumuTrig{event.mumuTrig()};
//    const bool mumuL2Trig{event.mumuL2Trig()};
//    const bool mumuNoVtxTrig{event.mumuNoVtxTrig()};

    // single electron triggers
//    const bool eTrig{event.eTrig()};
//    const bool eTrig{true};

    // single muon triggers
    const bool muTrig{event.muTrig()};

    // B-parking triggers
//    const bool bParkingMu9IP5 {event.bParkingTrig_Mu9_IP5()};
//    const bool bParkingMu9IP6 {event.bParkingTrig_Mu9_IP6()};
    const bool bParkingMu12IP6 {event.bParkingTrig_Mu12_IP6()};

    std::string channel = "";

    // Dilepton channels
    if (cutConfTrigLabel_.find("e") != std::string::npos) {
        channel = "ee";
    }
    if (cutConfTrigLabel_.find("d") != std::string::npos) {
        channel = "emu";
    }
    if (cutConfTrigLabel_.find("m") != std::string::npos){
        channel = "mumu";
    }

    // Check which trigger fired and if it correctly corresponds to the channel being scanned over.
    if (channel == "mumu") {
        if (postLepSelTree_ && is2018_) {
            if (muTrig || bParkingMu12IP6) return true;
        }

        if ( muTrig && !usingBparking_ ) {
//            if (isMC_) eventWeight *= twgt; // trigger weight should be unchanged for data anyway, but good practice to explicitly not apply it.
            return true;
        }
        else if (bParkingMu12IP6 && usingBparking_){
//            if (isMC_) eventWeight *= twgt; // trigger weight should be unchanged for data anyway, but good practice to explicitly not apply it.
            return true;
        }
    }
    return false;
}

double Cuts::deltaPhi(const double& phi1, const double& phi2) {
    return std::atan2(std::sin(phi1 - phi2), std::cos(phi1 - phi2));
}

double Cuts::deltaR(const double& eta1, const double& phi1, const double& eta2, const double& phi2) {
    return std::sqrt(std::pow(eta1 - eta2, 2) + std::pow(deltaPhi(phi1, phi2), 2));
}

double Cuts::getLeptonWeight(const AnalysisEvent& event, const int& syst) const {
    // If number of electrons is > 1  then both z pair are electrons, so get
    // their weight
    if (!isMC_) {
        return 1.;
    }

    double leptonWeight{1.};

    if (numTightEle_ == 2) {
        leptonWeight *= eleSF(event.zPairLeptons.first.Pt(), event.elePF2PATSCEta[event.zPairIndex.first], syst);
        leptonWeight *= eleSF(event.zPairLeptons.second.Pt(), event.elePF2PATSCEta[event.zPairIndex.second], syst);
    }
    else if (numTightMu_ == 2) {
        leptonWeight *= muonSF(event.zPairLeptons.first.Pt(), event.zPairLeptons.first.Eta(), syst, true);
        leptonWeight *= muonSF(event.zPairLeptons.second.Pt(), event.zPairLeptons.second.Eta(), syst, false);
    }
    else if (numTightEle_ == 1 && numTightMu_ == 1) {
        leptonWeight *= eleSF(event.elePF2PATPT[event.electronIndexTight[0]], event.elePF2PATSCEta[event.electronIndexTight[0]], syst);
        leptonWeight *= muonSF(event.muonPF2PATPt[event.muonIndexTight[0]], event.muonPF2PATEta[event.muonIndexTight[0]], syst, false);
    }

    return leptonWeight;
}

double Cuts::eleSF(const double& pt, const double& eta, const int& syst) const {
    return 1.0;
/*
    int binId{0};
    int binReco{0};

    if (pt <= maxEleIdPt_ && pt > minEleIdPt_) binId = h_eleSFs->FindBin(eta, pt);
    else if (pt > maxElePt) binId = h_eleSFs->FindBin(eta, maxEleIdPt_);
    else binId = h_eleSFs->FindBin(eta, minEleIdPt_);

    if ( pt <= maxEleRecoPt && pt > minEleRecoPt_ ) binReco = h_eleReco->FindBin(eta, pt);
    else if ( pt > maxEleRecoPt ) binReco = h_eleReco->FindBin(eta, maxEleRecoPt);
    else binReco = h_eleReco->FindBin(eta, minEleRecoPt_);

    double eleIdSF{h_eleSFs->GetBinContent(binId)};
    double eleRecoSF{h_eleReco->GetBinContent(binReco)};

    if (syst == 1) {
        eleIdSF += h_eleSFs->GetBinError(binId);
        eleRecoSF += h_eleReco->GetBinError(binReco);
        if (pt > 80.0 || pt <= 20.0) {
            eleRecoSF += 0.01;
        }
    }

    if (syst == 2) {
        eleIdSF -= h_eleSFs->GetBinError(binId);
        eleRecoSF -= h_eleReco->GetBinError(binReco);
        if (pt > 80.0 || pt <= 20.0) {
            eleRecoSF -= 0.01;
        }
    }

    return eleIdSF * eleRecoSF;
*/
}

double Cuts::muonSF(const double& pt, const double& eta, const int& syst, const bool& leadingMuon) const {

    int binId{0}, binIso{0}, binHlt{0};
    double muonIdSF{1.0}, muonIsoSF{1.0}, muonHltSF {1.0};

    if (pt > maxMuonIdPt_) binId = h_muonId->FindBin(std::abs(eta), maxMuonIdPt_);
    else if (pt < minMuonIdPt_) binId = h_muonId->FindBin(std::abs(eta), minMuonIdPt_);
    else binId = h_muonId->FindBin(std::abs(eta), pt);

    if (pt > maxMuonIsoPt_) binIso = h_muonIso->FindBin(std::abs(eta), maxMuonIsoPt_);
    else if (pt < minMuonIsoPt_) binIso = h_muonIso->FindBin(std::abs(eta), minMuonIsoPt_);
    else binIso = h_muonIso->FindBin(std::abs(eta), pt);

//    if (pt > maxMuonHltPt_) binHlt = h_muonHlt->FindBin(std::abs(eta), maxMuonHltPt_);
//    else if (pt < minMuonHltPt_) binHlt = h_muonHlt->FindBin(std::abs(eta), minMuonHltPt_);
//    else binHlt = h_muonHlt->FindBin(std::abs(eta), pt);

    muonIdSF  = h_muonId->GetBinContent(binId);
    if (leadingMuon) muonIsoSF = h_muonIso->GetBinContent(binIso);
//    if (leadingMuon) muonHltSF   = h_muonHlt->GetBinContent(binHlt);

    if (syst == 1 || syst == 2) {

        double idSystUncert = h_muonIdSyst->GetBinError(binId);
        double idStatUncert = h_muonIdStat->GetBinError(binId);
        double idUncert = std::sqrt(idStatUncert*idStatUncert + idSystUncert*idSystUncert);

        double isoSystUncert = h_muonIsoSyst->GetBinError(binId);
        double isoStatUncert = h_muonIso->GetBinError(binId);
        double isoUncert = std::sqrt(isoStatUncert*isoStatUncert + isoSystUncert*isoSystUncert);

        if (syst == 1) {
            muonIdSF += idUncert;
            if (leadingMuon) muonIsoSF += isoUncert;
//            if (leadingMuon) muonHltSF += h_muonHlt->GetBinError(binHlt);
        }
        else if (syst == 2) {
            muonIdSF -= idUncert;
            if (leadingMuon) muonIsoSF -= isoUncert;
//            if (leadingMuon) muonHltSF   -= h_muonHlt->GetBinError(binHlt);
        }
        return muonIdSF * muonIsoSF * muonHltSF;
    }
    else {
        return muonIdSF * muonIsoSF * muonHltSF;
    }
}

int Cuts::getMuonTrackPairIndex(const AnalysisEvent& event) const { 
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] == event.zPairIndex.first && event.muonTkPairPF2PATIndex2[i] == event.zPairIndex.second) return i;
    }
    return -1;
}

int Cuts::getChsTrackPairIndex(const AnalysisEvent& event) const { 
    for (int i{0}; i < event.numChsTrackPairs; i++) {
        if (event.chsTkPairIndex1[i] == event.chsPairIndex.first && event.chsTkPairIndex2[i] == event.chsPairIndex.second) return i;
    }
    return -1;
}

void Cuts::initialiseJECCors() {
    std::ifstream jecFile;
    if (!is2016_) {
        jecFile.open("scaleFactors/2017/Fall17_17Nov2017_V32_MC_Uncertainty_AK4PFchs.txt", std::ifstream::in);
    }
    else {
        jecFile.open("scaleFactors/2016/Summer16_23Sep2016V4_MC_Uncertainty_AK4PFchs.txt", std::ifstream::in);
    }
    std::string line;
    bool first{true};

    if (!jecFile.is_open()) {
        std::cout << "Unable to open jecFile." << std::endl;
        std::exit(0);
    }

    while (getline(jecFile, line)) {
        std::vector<std::string> tempVec;
        std::stringstream lineStream{line};
        std::string item;
        while (std::getline(lineStream, item, ' ')) {
            tempVec.emplace_back(item);
        }
        std::vector<double> tempUp;
        std::vector<double> tempDown;

        etaMinJEC_.emplace_back(std::stof(tempVec[0]));
        etaMaxJEC_.emplace_back(std::stof(tempVec[1]));
        for (unsigned i{1}; i < tempVec.size() / 3; i++) {
            unsigned ind{i * 3};
            if (first) {
                ptMinJEC_.emplace_back(std::stof(tempVec[ind]));
                ptMaxJEC_.emplace_back((ind + 3 >= tempVec.size()
                                            ? 10000.
                                            : std::stof(tempVec[ind + 3])));
            }
            tempUp.emplace_back(std::stof(tempVec[ind + 1]));
            tempDown.emplace_back(std::stof(tempVec[ind + 2]));
        }
        jecSFUp_.emplace_back(tempUp);
        jecSFDown_.emplace_back(tempDown);
        first = false;
    }
}

double Cuts::getJECUncertainty(const double& pt, const double& eta, const int& syst) const {
    if (!(syst == 4 || syst == 8)) {
        return 0.;
    }
    unsigned ptBin{0};
    unsigned etaBin{0};
    for (unsigned i{0}; i < ptMinJEC_.size(); i++) {
        if (pt > ptMinJEC_[i] && pt < ptMaxJEC_[i]) {
            ptBin = i;
            break;
        }
    }
    for (unsigned i{0}; i < etaMinJEC_.size(); i++) {
        if (eta > etaMinJEC_[i] && eta < etaMaxJEC_[i]) {
            etaBin = i;
            break;
        }
    }

    const double lowFact{syst == 4 ? jecSFUp_[etaBin][ptBin] : jecSFDown_[etaBin][ptBin]};
    const double hiFact{syst == 4 ? jecSFUp_[etaBin][ptBin + 1] : jecSFDown_[etaBin][ptBin + 1]};

    // Now do some interpolation
    const double a{(hiFact - lowFact) / (ptMaxJEC_[ptBin] - ptMinJEC_[ptBin])};
    const double b{(lowFact * (ptMaxJEC_[ptBin]) - hiFact * ptMinJEC_[ptBin]) / (ptMaxJEC_[ptBin] - ptMinJEC_[ptBin])};
    return (syst == 4 ? a * pt + b : -(a * pt + b));
}

std::pair<TLorentzVector, double> Cuts::getJetLVec(const AnalysisEvent& event, const int& index, const int& syst, const bool& initialRun) const {
    static constexpr double MIN_JET_ENERGY{1e-2};
    TLorentzVector returnJet;
    double newSmearValue{1.0};

    if (!initialRun) {
        newSmearValue = event.jetSmearValue.at(index);
        returnJet.SetPxPyPzE(event.jetPF2PATPx[index], event.jetPF2PATPy[index], event.jetPF2PATPz[index], event.jetPF2PATE[index]);
        returnJet *= newSmearValue;

        if (isMC_) {
            double jerUncer{
                getJECUncertainty(returnJet.Pt(), returnJet.Eta(), syst)};
            returnJet *= 1 + jerUncer;
        }

        return {returnJet, newSmearValue};
    }

    if (!isMC_) {
        returnJet.SetPxPyPzE(event.jetPF2PATPx[index], event.jetPF2PATPy[index], event.jetPF2PATPz[index], event.jetPF2PATE[index]);
        return {returnJet, newSmearValue};
    }

    // Not initial run, smear must be calculated

    // TODO: Check this is correct
    // For now, just leave jets of too large/small pT, large rho, or large η
    // untouched
    const double rho{(is2016_ || is2016APV_) ? event.elePF2PATRhoIso[0]
                             : event.fixedGridRhoFastjetAll};
    if (event.jetPF2PATPtRaw[index] < 15 || event.jetPF2PATPtRaw[index] > 3000 || rho > ((is2016_||is2016APV_) ? 40.9 : 42.52) || std::abs(event.jetPF2PATEta[index]) > 4.7) {
        returnJet.SetPxPyPzE(event.jetPF2PATPx[index],
                             event.jetPF2PATPy[index],
                             event.jetPF2PATPz[index],
                             event.jetPF2PATE[index]);
        return {returnJet, newSmearValue};
    }

    // TODO: Should this be gen or reco level?
    // I think reco because gen might not exist? (does not exist when
    // smearing)
    const double ptRes{(is2016_ || is2016APV_) ? jet2016PtSimRes(event.jetPF2PATPtRaw[index], event.jetPF2PATEta[index], rho)
                               : jet2017PtSimRes(event.jetPF2PATPtRaw[index], event.jetPF2PATEta[index], rho)};

    auto [jerSF, jerSigma] = (is2016_ || is2016APV_) ? jet2016SFs(std::abs(event.jetPF2PATEta[index])) : jet2017SFs(std::abs(event.jetPF2PATEta[index]));

    if (syst == 16) jerSF += jerSigma;
    else if (syst == 32) jerSF -= jerSigma;

    std::optional<size_t> matchingGenIndex{std::nullopt};
    for (size_t genIndex{0}; genIndex < event.NJETSMAX; ++genIndex) {
        const double dR{deltaR(event.genJetPF2PATEta[genIndex], event.genJetPF2PATPhi[genIndex], event.jetPF2PATEta[index], event.jetPF2PATPhi[index])};
        const double dPt{event.jetPF2PATPtRaw[index] - event.genJetPF2PATPT[genIndex]};

        if (event.genJetPF2PATPT[genIndex] > 0 && dR < (0.4 / 2.0) && std::abs(dPt) < 3.0 * ptRes * event.jetPF2PATPtRaw[index]) {
            matchingGenIndex = genIndex;
            break;
        }
    }

    if (matchingGenIndex.has_value()) { // If matching from GEN to RECO using dR<Rcone/2 and dPt < 3*sigma, just scale
        const double dPt{event.jetPF2PATPtRaw[index] - event.genJetPF2PATPT[matchingGenIndex.value()]};
        newSmearValue =
            std::max(1. + (jerSF - 1.) * dPt / event.jetPF2PATPtRaw[index], 0.);
    }
    else { // If not matched to a gen jet, randomly smear
        std::normal_distribution<> d(0, ptRes * std::sqrt(std::max(jerSF * jerSF - 1, 0.)));

        // Like with the Rochester corrections, seed the random number
        // generator with event (jet) properties so that each jet is smeared
        // the same way every time it is processed
        size_t seed{0};
        boost::hash_combine(seed, event.jetPF2PATPtRaw[index]);
        boost::hash_combine(seed, event.jetPF2PATEta[index]);
        boost::hash_combine(seed, event.jetPF2PATPhi[index]);
        boost::hash_combine(seed, event.eventNum);
        std::mt19937 gen(seed);

        newSmearValue = 1.0 + d(gen);
    }

    if (event.jetPF2PATE[index] * newSmearValue < MIN_JET_ENERGY) {
    // Negative or too small scale factor
        newSmearValue = MIN_JET_ENERGY / event.jetPF2PATE[index];
    }

    returnJet.SetPxPyPzE(event.jetPF2PATPx[index], event.jetPF2PATPy[index], event.jetPF2PATPz[index], event.jetPF2PATE[index]);
    returnJet *= newSmearValue;

    if (isMC_) {
        double jerUncer{getJECUncertainty(returnJet.Pt(), returnJet.Eta(), syst)};
        returnJet *= 1 + jerUncer;
    }

    return {returnJet, newSmearValue};
}

double Cuts::jet2016PtSimRes(const double& pt, const double& eta, const double& rho) {
    if (pt < 15 || pt > 3000) {
        throw std::runtime_error("pT " + std::to_string(pt) + " out of range to assign resolution");
    }

    static constexpr std::array<double, 14> etaBinEdges{0, 0.5, 0.8, 1.1, 1.3, 1.7, 1.9, 2.1, 2.3, 2.5, 2.8, 3, 3.2, 4.7};
    static constexpr std::array<double, 8> rhoBinEdges{0, 6.69, 12.39, 18.09, 23.79, 29.49, 35.19, 40.9};
    const auto res = [pt](const double p0, const double p1, const double p2, const double p3) {
        return (sqrt(p0 * abs(p0) / (pt * pt) + p1 * p1 * pow(pt, p3) + p2 * p2));
    };
    const auto etaBin{std::distance(etaBinEdges.begin(), std::upper_bound(etaBinEdges.begin(), etaBinEdges.end(), std::abs(eta)))};
    const auto rhoBin{std::distance(rhoBinEdges.begin(), std::upper_bound(rhoBinEdges.begin(), rhoBinEdges.end(), rho))};

    // https://github.com/cms-jet/JRDatabase/blob/master/textFiles/Summer16_25nsV1_MC/Summer16_25nsV1_MC_PtResolution_AK4PFchs.txt
    switch (etaBin) {
        case 1:
            switch (rhoBin) {
                case 1: return res(0.6172, 0.3908, 0.02003, -0.6407);
                case 2: return res(1.775, 0.4231, 0.02199, -0.6701);
                case 3: return res(2.457, 0.4626, 0.02416, -0.7045);
                case 4: return res(2.996, 0.5242, 0.02689, -0.7508);
                case 5: return res(3.623, 0.5591, 0.0288, -0.7747);
                case 6: return res(4.167, 0.6365, 0.03045, -0.8179);
                case 7: return res(4.795, 0.6819, 0.03145, -0.8408);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 2:
            switch (rhoBin) {
                case 1: return res(1.003, 0.4142, 0.02486, -0.6698);
                case 2: return res(2.134, 0.3971, 0.02264, -0.6469);
                case 3: return res(2.66, 0.4566, 0.02755, -0.7058);
                case 4: return res(3.264, 0.4799, 0.02702, -0.7156);
                case 5: return res(3.877, 0.5249, 0.02923, -0.7479);
                case 6: return res(4.441, 0.581, 0.03045, -0.7804);
                case 7: return res(4.742, 0.8003, 0.03613, -0.9062);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 3:
            switch (rhoBin) {
                case 1: return res(1.423, 0.4736, 0.03233, -0.7093);
                case 2: return res(2.249, 0.5041, 0.03355, -0.7316);
                case 3: return res(2.961, 0.4889, 0.03129, -0.7091);
                case 4: return res(3.4, 0.5757, 0.03541, -0.7742);
                case 5: return res(3.884, 0.6457, 0.03731, -0.8146);
                case 6: return res(4.433, 0.7524, 0.03962, -0.8672);
                case 7: return res(4.681, 0.9075, 0.04182, -0.9304);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 4:
            switch (rhoBin) {
                case 1: return res(-0.7275, 0.8099, 0.04885, -0.9097);
                case 2: return res(1.829, 0.8156, 0.04991, -0.9145);
                case 3: return res(2.72, 0.8454, 0.05036, -0.9215);
                case 4: return res(3.07, 0.9201, 0.05067, -0.9439);
                case 5: return res(3.991, 0.8715, 0.05041, -0.9151);
                case 6: return res(4.001, 1.14, 0.05214, -0.9987);
                case 7: return res(4.522, 1.22, 0.05122, -1);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 5:
            switch (rhoBin) {
                case 1: return res(-1.692, 1.192, 0.05049, -1.06);
                case 2: return res(-1.804, 1.48, 0.05315, -1.145);
                case 3: return res(1.673, 1.402, 0.0536, -1.116);
                case 4: return res(2.906, 1.305, 0.05377, -1.076);
                case 5: return res(2.766, 1.613, 0.05511, -1.137);
                case 6: return res(3.409, 1.746, 0.05585, -1.143);
                case 7: return res(3.086, 2.034, 0.05795, -1.181);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 6:
            switch (rhoBin) {
                case 1: return res(-0.8823, 1.092, 0.03599, -1.062);
                case 2: return res(2.193, 0.9891, 0.03382, -1.012);
                case 3: return res(2.9, 1.043, 0.03477, -1.019);
                case 4: return res(2.371, 1.488, -0.04053, -1.145);
                case 5: return res(3.75, 1.458, 0.04346, -1.122);
                case 6: return res(3.722, 1.808, 0.04668, -1.177);
                case 7: return res(4.836, 1.47, 0.03875, -1.047);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 7:
            switch (rhoBin) {
                case 1: return res(1.184, 0.8944, 0.03233, -1.005);
                case 2: return res(1.691, 1.124, 0.03736, -1.094);
                case 3: return res(2.837, 1.077, 0.03437, -1.046);
                case 4: return res(2.459, 1.589, -0.04007, -1.18);
                case 5: return res(4.058, 1.369, -0.03922, -1.087);
                case 6: return res(4.231, 1.679, 0.0432, -1.13);
                case 7: return res(2.635, 2.648, 0.04929, -1.28);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 8:
            switch (rhoBin) {
                case 1: return res(0.3022, 1.127, 0.03826, -1.134);
                case 2: return res(2.161, 1.217, 0.03826, -1.142);
                case 3: return res(3.218, 1.21, 0.03662, -1.112);
                case 4: return res(3.328, 1.638, 0.04398, -1.216);
                case 5: return res(5.506, 1.173, 0.04403, -1.054);
                case 6: return res(-2.444, 3.613, 0.05639, -1.437);
                case 7: return res(2.217, 3.133, 0.05032, -1.338);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 9:
            switch (rhoBin) {
                case 1: return res(3.125, 0.6026, 0.02576, -0.8702);
                case 2: return res(3.935, 0.6533, 0.02587, -0.889);
                case 3: return res(4.198, 1.024, 0.03618, -1.069);
                case 4: return res(2.948, 2.386, 0.04771, -1.382);
                case 5: return res(4.415, 2.086, 0.04704, -1.294);
                case 6: return res(-3.084, 4.156, 0.05366, -1.503);
                case 7: return res(-6.144, 5.969, 0.05633, -1.602);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 10:
            switch (rhoBin) {
                case 1: return res(4.244, 0.2766, -1.86e-08, -0.5068);
                case 2: return res(4.919, 0.3193, 5.463e-06, -0.58);
                case 3: return res(5.909, 0.2752, 4.144e-06, -0.5272);
                case 4: return res(-47.31, 47.18, 0.05853, -1.991);
                case 5: return res(-46.49, 46.33, 0.05698, -1.989);
                case 6: return res(8.651, 0.2522, 6.592e-06, -0.4835);
                case 7: return res(7.716, 2.481, 0.0531, -1.455);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 11:
            switch (rhoBin) {
                case 1: return res(4.467, 0.1997, -3.491e-06, -0.2623);
                case 2: return res(4.17, 0.928, 0.07702, -1.063);
                case 3: return res(-0.04491, 3.67, 0.08704, -1.641);
                case 4: return res(5.528, 1.286, 0.07962, -1.187);
                case 5: return res(-78.36, 78.23, 0.08448, -1.996);
                case 6: return res(7.559, 1.147, 0.07023, -1.134);
                case 7: return res(-59.03, 59.03, -0.08184, -1.992);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 12:
            switch (rhoBin) {
                case 1: return res(0.0002851, 3.01, 0.1382, -1.702);
                case 2: return res(-33.01, 33.04, 0.1343, -1.991);
                case 3: return res(-67.94, 67.8, 0.1342, -1.996);
                case 4: return res(-47.81, 48, 0.1391, -1.996);
                case 5: return res(7.162, 0.9211, 0.1395, -1.209);
                case 6: return res(8.193, 0.1995, 2.822e-05, -0.132);
                case 7: return res(8.133, 0.9983, 0.1349, -1.181);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        case 13:
            switch (rhoBin) {
                case 1: return res(2.511, 0.3167, 0.09085, -0.7407);
                case 2: return res(3.297, 0.2091, 6.258e-05, -0.2755);
                case 3: return res(1.85, 2.281, 0.1042, -1.635);
                case 4: return res(3.869, 1.001, 0.09955, -1.266);
                case 5: return res(-23.98, 24.11, 0.1057, -1.988);
                case 6: return res(5.403, 0.2371, 1.5e-05, -0.3177);
                case 7: return res(5.753, 0.2337, 0.0002982, -0.3108);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + "out of range");
            }
        default: throw std::runtime_error("Eta " + std::to_string(eta) + " out of range");
    }
}

double Cuts::jet2017PtSimRes(const double& pt, const double& eta, const double& rho) {
    if (pt < 15 || pt > 3000) {
        throw std::runtime_error("pT " + std::to_string(pt) + " out of range to assign resolution");
    }

    static constexpr std::array<double, 14> etaBinEdges{0, 0.5, 0.8, 1.1, 1.3, 1.7, 1.9, 2.1, 2.3, 2.5, 2.8, 3, 3.2, 4.7};
    static constexpr std::array<double, 8> rhoBinEdges{0, 6.37, 12.4, 18.42, 24.45, 30.47, 36.49, 42.52};
    const auto res = [pt](const double p0, const double p1, const double p2, const double p3) {
        return (std::sqrt(p0 * std::abs(p0) / (pt * pt) + p1 * p1 * std::pow(pt, p3) + p2 * p2));
    };
    const auto etaBin{std::distance(etaBinEdges.begin(), std::upper_bound(etaBinEdges.begin(), etaBinEdges.end(), std::abs(eta)))};
    const auto rhoBin{std::distance(rhoBinEdges.begin(), std::upper_bound(rhoBinEdges.begin(), rhoBinEdges.end(), rho))};

    // https://github.com/cms-jet/JRDatabase/blob/master/textFiles/Fall17_V3_MC/Fall17_V3_MC_PtResolution_AK4PFchs.txt
    switch (etaBin) {
        case 1:
            switch (rhoBin) {
                case 1: return res(-1.515, 0.5971, 0.03046, -0.7901);
                case 2: return res(-0.7966, 0.6589, 0.03119, -0.8237);
                case 3: return res(1.387, 0.6885, 0.03145, -0.8378);
                case 4: return res(2.151, 0.7185, 0.03168, -0.8502);
                case 5: return res(2.73, 0.7361, 0.03184, -0.8548);
                case 6: return res(3.603, 0.7318, 0.03227, -0.855);
                case 7: return res(3.897, 0.7882, 0.03282, -0.8746);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 2:
            switch (rhoBin) {
                case 1: return res(-0.9395, 0.4556, 0.02738, -0.6909);
                case 2: return res(1.339, 0.4621, 0.02785, -0.6965);
                case 3: return res(1.597, 0.5254, 0.02952, -0.7407);
                case 4: return res(2.527, 0.5042, 0.02842, -0.723);
                case 5: return res(2.896, 0.5428, 0.03001, -0.7476);
                case 6: return res(3.514, 0.5437, 0.03055, -0.7486);
                case 7: return res(3.678, 0.6372, 0.03325, -0.8053);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 3:
            switch (rhoBin) {
                case 1: return res(-0.8118, 0.491, 0.03583, -0.7149);
                case 2: return res(1.289, 0.49, 0.03539, -0.7073);
                case 3: return res(1.953, 0.5161, 0.03658, -0.7295);
                case 4: return res(2.347, 0.5396, 0.03576, -0.7339);
                case 5: return res(2.794, 0.5687, 0.03825, -0.7602);
                case 6: return res(2.796, 0.7203, 0.04074, -0.8431);
                case 7: return res(3.788, 0.6287, 0.04156, -0.7959);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 4:
            switch (rhoBin) {
                case 1: return res(0.6707, 0.5839, 0.04697, -0.752);
                case 2: return res(1.395, 0.6702, 0.0496, -0.8152);
                case 3: return res(2.43, 0.5712, 0.04572, -0.7345);
                case 4: return res(2.439, 0.6623, 0.04496, -0.7771);
                case 5: return res(3.353, 0.5924, 0.04617, -0.7384);
                case 6: return res(3.465, 0.7579, 0.05328, -0.8435);
                case 7: return res(1.982, 1.148, 0.05664, -0.9626);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 5:
            switch (rhoBin) {
                case 1: return res(-1.469, 0.9562, 0.05101, -0.955);
                case 2: return res(-1.377, 1.078, 0.05427, -1.003);
                case 3: return res(1.501, 1.072, 0.05498, -1.001);
                case 4: return res(1.53, 1.158, 0.05396, -1.021);
                case 5: return res(1.621, 1.358, 0.0578, -1.078);
                case 6: return res(3.163, 1.131, 0.05725, -0.9809);
                case 7: return res(2.818, 1.326, 0.05893, -0.9977);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 6:
            switch (rhoBin) {
                case 1: return res(1.227, 0.8407, -0.0232, -0.9284);
                case 2: return res(-1.339, 1.218, -0.03479, -1.076);
                case 3: return res(-2.011, 1.435, -0.03565, -1.124);
                case 4: return res(3.324, 0.8102, -0.02662, -0.8923);
                case 5: return res(2.188, 1.365, -0.0375, -1.088);
                case 6: return res(2.884, 1.306, 0.03685, -1.038);
                case 7: return res(4.03, 1.141, 0.03059, -0.9262);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 7:
            switch (rhoBin) {
                case 1: return res(-1.979, 1.193, -0.03497, -1.109);
                case 2: return res(-2.528, 1.44, -0.03273, -1.143);
                case 3: return res(1.95, 1.118, -0.03202, -1.054);
                case 4: return res(2.377, 1.166, -0.03593, -1.061);
                case 5: return res(3.122, 1.107, -0.0292, -1.005);
                case 6: return res(-1.899, 1.944, 0.03736, -1.185);
                case 7: return res(4.168, 1.452, 0.03836, -1.019);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 8:
            switch (rhoBin) {
                case 1: return res(1.947, 0.9639, -0.02799, -1.024);
                case 2: return res(2.643, 0.9054, -0.02701, -0.9753);
                case 3: return res(-3.209, 2.521, -0.04442, -1.385);
                case 4: return res(-5.368, 3.81, -0.04587, -1.525);
                case 5: return res(-2.344, 2.207, 0.03446, -1.265);
                case 6: return res(-11.01, 8.354, 0.05639, -1.706);
                case 7: return res(6.282, 1.064, 8.482e-06, -0.8687);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 9:
            switch (rhoBin) {
                case 1: return res(3.639, 0.6502, -0.01427, -0.8624);
                case 2: return res(2.391, 1.635, -0.0378, -1.251);
                case 3: return res(3.431, 1.985, 0.04609, -1.359);
                case 4: return res(5.095, 0.8757, -0.02736, -0.9761);
                case 5: return res(5.034, 1.479, -0.03479, -1.175);
                case 6: return res(6.694, 1.325, 0.03374, -1.101);
                case 7: return res(7.444, 1.137, 4.258e-05, -0.9531);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 10:
            switch (rhoBin) {
                case 1: return res(6.114, 0.2385, 1.741e-05, -0.5054);
                case 2: return res(6.931, 0.1964, 7.465e-06, -0.4335);
                case 3: return res(7.858, 0.2435, 6.026e-07, -0.5235);
                case 4: return res(8.713, 0.1314, 8.441e-06, -0.3028);
                case 5: return res(9.413, 0.2792, 1.217e-06, -0.5729);
                case 6: return res(10.51, 0.1659, 1.277e-06, -0.4276);
                case 7: return res(11.77, 8.547e-07, 0.05169, -1.197);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 11:
            switch (rhoBin) {
                case 1: return res(6.048, 0.1992, -3.559e-06, -0.2953);
                case 2: return res(6.867, 0.2036, 1.946e-05, -0.3068);
                case 3: return res(8.198, 0.0001314, 0.08772, -1.252);
                case 4: return res(8.756, 0.134, -0.07197, -0.2968);
                case 5: return res(9.615, 0.0001533, -0.08793, -1.445);
                case 6: return res(10.01, 0.1524, 3.815e-05, -0.2422);
                case 7: return res(10.05, 0.1932, 0.0001734, -0.2739);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 12:
            switch (rhoBin) {
                case 1: return res(-35.12, 35.21, 0.1466, -1.993);
                case 2: return res(6.573, 0.2026, 6.573e-05, -0.1564);
                case 3: return res(0.004144, 6.019, 0.1549, -1.854);
                case 4: return res(8.341, 0.0001012, 0.1526, -1.689);
                case 5: return res(9.115, 0.0002242, 0.1518, -1.362);
                case 6: return res(9.86, -2.112e-05, 0.1438, -1.114);
                case 7: return res(10.45, 0.0001536, 0.1398, -1.271);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        case 13:
            switch (rhoBin) {
                case 1: return res(-29.87, 29.84, 0.1045, -1.995);
                case 2: return res(-23.2, 23.09, 0.1051, -1.987);
                case 3: return res(4.337, 0.2253, 0.06986, -0.4215);
                case 4: return res(4.088, 2.746, 0.1136, -1.959);
                case 5: return res(5.624, 0.1291, 0.002663, -0.04825);
                case 6: return res(6.152, 6.125e-05, 0.1128, -1.319);
                case 7: return res(6.235, 0.1408, 0.0001266, -0.08163);
                default: throw std::runtime_error("Rho " + std::to_string(rho) + " out of range");
            }
        default: throw std::runtime_error("Eta " + std::to_string(eta) + " out of range");
    }
}

std::pair<double, double> Cuts::jet2016SFs(const double& eta) {
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
    constexpr std::array<double, 14> etaBinEdges{0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.319, 5.191};

    switch (std::distance(etaBinEdges.begin(), std::upper_bound(etaBinEdges.begin(), etaBinEdges.end(), eta))) {
        case 1: return {1.1685, 0.0645};
        case 2: return {1.1948, 0.0652};
        case 3: return {1.1464, 0.0632};
        case 4: return {1.1609, 0.1025};
        case 5: return {1.1278, 0.0986};
        case 6: return {1.1000, 0.1079};
        case 7: return {1.1426, 0.1214};
        case 8: return {1.1512, 0.1440};
        case 9: return {1.2963, 0.2371};
        case 10: return {1.3418, 0.2091};
        case 11: return {1.7788, 0.2008};
        case 12: return {1.1869, 0.1243};
        case 13: return {1.1922, 0.1448};
        default: throw std::runtime_error("Eta " + std::to_string(eta) + " out of range");
    }
}

std::pair<double, double> Cuts::jet2017SFs(const double& eta)
{
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Uncertainty
    constexpr std::array<double, 14> etaBinEdges{0, 0.522, 0.783, 1.131, 1.305, 1.740, 1.930, 2.043, 2.322, 2.5, 2.853, 2.964, 3.319, 5.191};

    switch (std::distance(etaBinEdges.begin(), std::upper_bound(etaBinEdges.begin(), etaBinEdges.end(), eta))) {
        case 1: return {1.1432, 0.0222};
        case 2: return {1.1815, 0.0484};
        case 3: return {1.0989, 0.0456};
        case 4: return {1.1137, 0.1397};
        case 5: return {1.1307, 0.147};
        case 6: return {1.16, 0.0976};
        case 7: return {1.2393, 0.1909};
        case 8: return {1.2604, 0.1501};
        case 9: return {1.4085, 0.2020};
        case 10: return {1.9909, 0.5684};
        case 11: return {2.2923, 0.3743};
        case 12: return {1.2696, 0.1089};
        case 13: return {1.1542, 0.1524};
        default: throw std::runtime_error("Eta " + std::to_string(eta) + " out of range");
    }
}

void Cuts::getBWeight(const AnalysisEvent& event, const TLorentzVector& jet, const int& index, double& mcTag, double& mcNoTag, double& dataTag, double& dataNoTag, double& err1, double& err2, double& err3, double& err4) const {
    // Use b-tagging efficiencies and scale factors.
    // Firstly get efficiency for pt/eta bin here.
    double eff{1.};
    int partonFlavour{std::abs(event.jetPF2PATPID[index])};

    if (partonFlavour == 0)
        return;
    if (partonFlavour == 5) {
        eff = bTagEffPlots_[4]->GetBinContent( bTagEffPlots_[4]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[4]->GetYaxis()->FindBin(std::abs(jet.Eta())) ) 
            / bTagEffPlots_[0]->GetBinContent( bTagEffPlots_[0]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[0]->GetYaxis()->FindBin(std::abs(jet.Eta())) );
    }
    if (partonFlavour == 4)
   {    eff = bTagEffPlots_[5]->GetBinContent( bTagEffPlots_[5]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[5]->GetYaxis()->FindBin(std::abs(jet.Eta())) )
            / bTagEffPlots_[1]->GetBinContent( bTagEffPlots_[1]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[1]->GetYaxis()->FindBin(std::abs(jet.Eta())) );
    }
    if (partonFlavour < 4) {
        eff = bTagEffPlots_[6]->GetBinContent( bTagEffPlots_[6]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[6]->GetYaxis()->FindBin(std::abs(jet.Eta())) )
            / bTagEffPlots_[2]->GetBinContent( bTagEffPlots_[2]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[2]->GetYaxis()->FindBin(std::abs(jet.Eta())) );
    }
    if (partonFlavour == 21) {
        eff = bTagEffPlots_[7]->GetBinContent( bTagEffPlots_[7]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[7]->GetYaxis()->FindBin(std::abs(jet.Eta())) )
            / bTagEffPlots_[3]->GetBinContent( bTagEffPlots_[3]->GetXaxis()->FindBin(jet.Pt()), bTagEffPlots_[3]->GetYaxis()->FindBin(std::abs(jet.Eta())) );
    }

    if (std::isnan(eff)) {
        std::cerr << "WARN: NaN encountered calculating bTag efficiency, setting to 1.0. Check efficiency plots." << std::endl;
        eff = 1;
    }

    // Get SF
    // Initalise variables.
    double jet_scalefactor{1.};
    double jet_scalefactor_up{1.};
    double jet_scalefactor_do{1.};

    double SFerr{0.};
    double jetPt{jet.Pt()};
    constexpr double maxBjetPt{670};
    constexpr double maxLjetPt{1000.0};
    bool doubleUncertainty{false};
    // Do some things if it's a b or c

    if (partonFlavour == 5) {
        if (jetPt > maxBjetPt) {
            jetPt = maxBjetPt;
            doubleUncertainty = true;
        }
        jet_scalefactor = getBSF(0, 0, jetPt);
        jet_scalefactor_up = getBSF(0, 1, jetPt);
        jet_scalefactor_do = getBSF(0, -1, jetPt);
    }

    else if (partonFlavour == 4) {
        if (jetPt > maxBjetPt) {
            jetPt = maxBjetPt;
            doubleUncertainty = true;
        }
        jet_scalefactor = getBSF(1, 0, jetPt);
        jet_scalefactor_up = getBSF(1, 1, jetPt);
        jet_scalefactor_do = getBSF(1, -1, jetPt);
    }

    // Light jets
    else {
        if (jetPt > maxLjetPt) {
            jetPt = maxLjetPt;
            doubleUncertainty = true;
        }
        jet_scalefactor = getBSF(2, 0, jetPt);
        jet_scalefactor_up = getBSF(2, 1, jetPt);
        jet_scalefactor_do = getBSF(2, -1, jetPt);
    }

    if (doubleUncertainty) {
        jet_scalefactor_up = 2 * (jet_scalefactor_up - jet_scalefactor) + jet_scalefactor;
        jet_scalefactor_do = 2 * (jet_scalefactor_do - jet_scalefactor) + jet_scalefactor;
    }

    SFerr = std::abs(jet_scalefactor_up - jet_scalefactor) > std::abs(jet_scalefactor_do - jet_scalefactor) ? std::abs(jet_scalefactor_up - jet_scalefactor) : std::abs(jet_scalefactor_do - jet_scalefactor);

    // Apply the weight of the jet and set the error
    if (event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[index] > bDiscCut_) {
        mcTag *= eff;
        dataTag *= eff * jet_scalefactor;

        if (partonFlavour == 5 || partonFlavour == 4) {
            err1 += SFerr / jet_scalefactor;
        }
        else {
            err3 += SFerr / jet_scalefactor;
        }
    }
    else {
        mcNoTag *= (1 - eff);
        dataNoTag *= (1 - eff * jet_scalefactor);

        if (partonFlavour == 5 || partonFlavour == 4) {
            err2 += (-eff * SFerr) / (1 - eff * jet_scalefactor);
        }
        else {
            err4 += (-eff * SFerr) / (1 - eff * jet_scalefactor);
        }
    }
}

// Backup temporary method to do Btag Scale Factors whilst debugging is ongoing. TODO: F1X TH1S

double Cuts::getBSF(const int flavour, const int type, const double pt) const {
 if (!is2016_) { // is 2017
        // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
        // MEDIUM

        static constexpr std::array<double, 10> ptBinEdges{20, 30, 50, 70, 100, 140, 200, 300, 600, 1000};
        const auto ptBin{std::distance(ptBinEdges.begin(), std::upper_bound(ptBinEdges.begin(), ptBinEdges.end(), pt))};
        const auto mujets_sf = [pt, type](const double p0) {return (0.941966 * ((1 + 0.24108 * pt) / (1 + 0.248776 * pt))) + type * p0;
        };
        const auto incl_sf = [pt, type]() {return (0.949449 + 0.000516201 * pt + 7.13398e-08 * pt * pt + -3.55644e-10 * pt * pt * pt) * (1 + type * 0.082197);
        };

        switch (flavour) {
            case 0: // B flavour
                if (type == 0) {
                    return mujets_sf(0);
                }
                else if (std::abs(type) == 1) {
                    switch (ptBin) {
                        case 1: return mujets_sf(0.051529459655284882);
                        case 2: return mujets_sf(0.017671864479780197);
                        case 3: return mujets_sf(0.022306634113192558);
                        case 4: return mujets_sf(0.023042259737849236);
                        case 5: return mujets_sf(0.039661582559347153);
                        case 6: return mujets_sf(0.061514820903539658);
                        case 7: return mujets_sf(0.071018315851688385);
                        case 8: return mujets_sf(0.054169680923223495);
                        case 9: return mujets_sf(0.063008971512317657);
                        default: throw std::runtime_error("pT out of range of b tag SFs");
                    }
                }
                else {
                    throw std::runtime_error("Unknown b tag systematic type");
                }
            case 1: // C flavour
                if (type == 0) {
                    return mujets_sf(0);
                }
                else if (std::abs(type) == 1) {
                    switch (ptBin) {
                        case 1: return mujets_sf(0.15458837151527405);
                        case 2: return mujets_sf(0.053015593439340591);
                        case 3: return mujets_sf(0.066919900476932526);
                        case 4: return mujets_sf(0.069126777350902557);
                        case 5: return mujets_sf(0.11898474395275116);
                        case 6: return mujets_sf(0.18454445898532867);
                        case 7: return mujets_sf(0.21305495500564575);
                        case 8: return mujets_sf(0.16250903904438019);
                        case 9: return mujets_sf(0.18902692198753357);
                        default: throw std::runtime_error("pT out of range of b tag SFs");
                    }
                }
                else {
                    throw std::runtime_error("Unknown b tag systematic type");
                }
            case 2: // UDSG flavour
                if (std::abs(type) <= 1) {
                    return incl_sf();
                }
                else {
                    throw std::runtime_error("Unknown b tag systematic type");
                }
            default:
                throw std::runtime_error("Unknown b tag systematic flavour");
        }
    }
    else { // is 2016

        double sf{1.0};
        const double& x{pt};

        // MEDIUM
        if (flavour == 0) { // B flavour
            if (type == 0) sf = 0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)));
            if (type == 1) {
                if (pt < 30.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.040554910898208618;
                if (pt < 50.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.01836167648434639;
                if (pt < 70.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.016199169680476189;
                if (pt < 100.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.014634267427027225;
                if (pt < 140.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.014198922552168369;
                if (pt < 200.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.016547618433833122;
                if (pt < 300.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.02140621654689312;
                if (pt < 600.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.023563217371702194;
                else sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.034716218709945679;
            }
            if (type == -1) {
                if (pt < 30.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.040554910898208618;
                if (pt < 50.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.01836167648434639;
                if (pt < 70.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.016199169680476189;
                if (pt < 100.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.014634267427027225;
                if (pt < 140.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.014198922552168369;
                if (pt < 200.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.016547618433833122;
                if (pt < 300.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.02140621654689312;
                if (pt < 600.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.023563217371702194;
                else sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.034716218709945679;
            }
        }
        if (flavour == 1) { // C flavour
            if (type == 0) sf = 0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)));
            if (type == 1) {
                if (pt < 30.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.12166473269462585;
                if (pt < 50.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.055085029453039169;
                if (pt < 70.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.048597507178783417;
                if (pt < 100.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.043902803212404251;
                if (pt < 140.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.042596768587827682;
                if (pt < 200.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.049642853438854218;
                if (pt < 300.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.06421864777803421;
                if (pt < 600.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.070689648389816284;
                else sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) + 0.10414865612983704;
            }
            if (type == -1) {
                if (pt < 30.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.12166473269462585;
                if (pt < 50.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.055085029453039169;
                if (pt < 70.0)  sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.048597507178783417;
                if (pt < 100.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.043902803212404251;
                if (pt < 140.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.042596768587827682;
                if (pt < 200.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.049642853438854218;
                if (pt < 300.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.06421864777803421;
                if (pt < 600.0) sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.070689648389816284;
                else sf = (0.718014 * ((1. + (0.0685826 * x)) / (1. + (0.0475779 * x)))) - 0.10414865612983704;
            }
        }
        if (flavour == 2) { // UDSG flavour
            if (type == 0)  sf =  1.0589 + 0.000382569 * x + -2.4252e-07 * x * x + 2.20966e-10 * x * x * x;
            if (type == 1)  sf = (1.0589 + 0.000382569 * x + -2.4252e-07 * x * x + 2.20966e-10 * x * x * x) * (1 + (0.100485 + 3.95509e-05 * x + -4.90326e-08 * x * x));
            if (type == -1) sf = (1.0589 + 0.000382569 * x + -2.4252e-07 * x * x + 2.20966e-10 * x * x * x) * (1 - (0.100485 + 3.95509e-05 * x + -4.90326e-08 * x * x));
        }

        return sf;
    }
}
