#ifndef _cutClass_hpp_
#define _cutClass_hpp_

#include "AnalysisEvent.hpp"
#include "RoccoR.h"
#include "plots.hpp"

#include <TH1F.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TLorentzVector.h>
#include <fstream>
#include <map>
#include <vector>

class Cuts
{
    private:
    bool makeLeptonCuts(AnalysisEvent& event,
                        double& eventWeight,
                        std::map<std::string, std::shared_ptr<Plots>>& plotMap,
                        TH1D& cutFlow,
                        const int& syst);
    std::pair<std::vector<int>, std::vector<double>> makeJetCuts(const AnalysisEvent& event,
                    const int syst,
                    double& eventWeight,
                    const bool isProper = true) const;
    [[gnu::pure]] std::vector<int> makeBCuts(const AnalysisEvent& event,
                                             const std::vector<int> jets,
                                             const int syst = 0) const;

    [[gnu::pure]] std::vector<int> getTightEles(const AnalysisEvent& event) const;
    [[gnu::pure]] std::vector<int> getLooseEles(const AnalysisEvent& event) const;
    [[gnu::pure]] std::vector<int> getTightMuons(const AnalysisEvent& event) const;
    [[gnu::pure]] std::vector<int> getLooseMuons(const AnalysisEvent& event) const;
    [[gnu::pure]] std::vector<int> getChargedHadronTracks(const AnalysisEvent& event) const;
    bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons) const;
    bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chs) const;
    double getWbosonQuarksCand(AnalysisEvent& event,
                               const std::vector<int>& jets,
                               const int& syst) const;

    [[gnu::const]] double getTopMass(const AnalysisEvent& event) const;
    bool triggerCuts(const AnalysisEvent& event,
                     double& eventWeight,
                     const int& syst = 0) const;
    [[gnu::const]] bool metFilters(const AnalysisEvent& event) const;

    std::vector<double> getRochesterSFs(const AnalysisEvent& event) const;
    // Function to get lepton SF
    double getLeptonWeight(const AnalysisEvent& event, const int& syst) const;
    double eleSF(const double& pt, const double& eta, const int& syst) const;
    double muonSF(const double& pt, const double& eta, const int& syst, const bool& leadingMuon) const;

    // grab the muon track pair index for selected muons
    int getMuonTrackPairIndex(const AnalysisEvent& event) const;
    // grab the chs track pair index for selected muons
    int getChsTrackPairIndex(const AnalysisEvent& event) const;

    // set to true to fill in histograms/spit out other info
    bool doPlots_;
    bool fillCutFlow_; // Fill cut flows
    bool invertLepCharge_; // For background estimation
    bool invertLepIso_; // For background estimation
    bool makeEventDump_;
    const bool is2016_;
    const bool is2016APV_;
    const bool is2018_;
    bool usingBparking_;


    // Tight electron cuts
    unsigned numTightEle_;
    double tightElePt_;
    double tightElePtLeading_;
    double tightEleEta_;
    double tightEleEtaLeading_;
    double tightEleRelIso_;

    // Loose electron cuts
    unsigned numLooseEle_;
    double looseElePt_;
    double looseElePtLeading_;
    double looseEleEta_;
    double looseEleEtaLeading_;
    double looseEleRelIso_;

    // Tight muon cuts
    unsigned numTightMu_;
    double tightMuonPt_;
    double tightMuonPtLeading_;
    double tightMuonEta_;
    double tightMuonEtaLeading_;
    double tightMuonRelIso_;

    // Loose muon cuts
    unsigned numLooseMu_;
    double looseMuonPt_;
    double looseMuonPtLeading_;
    double looseMuonEta_;
    double looseMuonEtaLeading_;
    double looseMuonRelIso_;

    double scalarMassCut_;
    double maxDileptonDeltaR_;
    double chsMass_;
    double maxChsDeltaR_;
    double higgsMassCut_;
    double higgsTolerence_;
    double invWMassCut_;

    // Tight jet cuts
    unsigned numJets_;
    unsigned maxJets_;
    double jetPt_;
    double jetEta_;
    bool jetIDDo_;

    // B-Disc cut
    unsigned numbJets_;
    unsigned maxbJets_;
    double maxbJetEta_;
    double bDiscCut_;

    // C-Disc cut
    unsigned numcJets_;

    // Rochester Corrections
    RoccoR rc_;

    // Some things that will be used for JEC uncertainties.
    std::vector<double> ptMinJEC_;
    std::vector<double> ptMaxJEC_;
    std::vector<double> etaMinJEC_;
    std::vector<double> etaMaxJEC_;
    std::vector<std::vector<double>> jecSFUp_;
    std::vector<std::vector<double>> jecSFDown_;
    void initialiseJECCors();
    [[gnu::pure]] double getJECUncertainty(const double& pt,
                                           const double& eta,
                                           const int& syst) const;
    std::pair<TLorentzVector, double> getJetLVec(const AnalysisEvent& event,
                                                 const int& index,
                                                 const int& syst,
                                                 const bool& initialRun) const;
    static double jet2016PtSimRes(const double& pt, const double& eta, const double& rho);
    static double jet2017PtSimRes(const double& pt, const double& eta, const double& rho);
    [[gnu::const]] static std::pair<double, double> jet2016SFs(const double& eta);
    static std::pair<double, double> jet2017SFs(const double& eta);

    // Sets whether to do MC or data cuts. Set every time a new dataset is
    // processed in the main loop.
    bool isMC_;
    std::string triggerFlag_;
    std::string postfixName_;
    // Set the flag used to reject non-prompt leptons when making the ABCD shapes for plotting purposes
    bool isABCD_;
    // Set the flag used to use the Z+jets CR
    bool isZplusCR_;

    // For producing post-lepsel skims
    TTree* postLepSelTree_;

    // For removing trigger cuts. Will be set to false by default
    bool skipTrigger_;
    // For skipping scalar mass cuts. Will be set to false by default
    bool skipScalarMassCut_;

    // For making b-tagging efficiencies. Needed for reweighting and
    // systematics.
    bool makeBTagEffPlots_;
    // And the efficiency plots.
    std::vector<TH2D*> bTagEffPlots_;
    bool getBTagWeight_;

    [[gnu::const]] double
        getBSF(const int flavour, const int type, const double pt) const;

    void getBWeight(const AnalysisEvent& event,
                    const TLorentzVector& jet,
                    const int& index,
                    double& mcTag,
                    double& mcNoTag,
                    double& dataTag,
                    double& dataNoTag,
                    double& err1,
                    double& err2,
                    double& err3,
                    double& err4) const;

    // met and mtw cut values
    double metDileptonCut_;

    // unblinding flag
    bool unblind_;

    // Sets trigger from config file
    std::string cutConfTrigLabel_;

    TFile* electronHltFile;
    TFile* electronSFsFile;
    TFile* electronRecoFile;
    TH2F* h_eleHlt;
    TH2F* h_eleSFs;
    TH2F* h_eleReco;

    double maxEleIdPt_;
    double maxEleIdReco_;
    double minEleIdPt_;
    double minEleIdReco_;

    TFile* muonHltFile;
    TFile* muonIdFile;
    TFile* muonIsoFile;

    TH2F* h_muonHlt;
    TH2F* h_muonId;
    TH2F* h_muonIdSyst;
    TH2F* h_muonIdStat;
    TH2F* h_muonIso;
    TH2F* h_muonIsoSyst;
    TH2F* h_muonIsoStat;

    double maxMuonIdPt_;
    double maxMuonIsoPt_;
    double maxMuonHltPt_;

    double minMuonIdPt_;
    double minMuonIsoPt_;
    double minMuonHltPt_;

    public:
    Cuts(const bool doPlots, const bool fillCutFlows, const bool invertLepCharge, const bool invertLepIso, const bool is2016, const bool is2016APV, const bool is2018);
    ~Cuts();
    bool makeCuts(AnalysisEvent& event, double& eventWeight, std::map<std::string, std::shared_ptr<Plots>>& plotMap, TH1D& cutFlow, const int systToRun);
    void setMC(bool isMC) {
        isMC_ = isMC;
    }
    void setCloneTree(TTree* tree) {
        postLepSelTree_ = tree;
    }
    void setNumLeps(const unsigned tightMu, const unsigned looseMu, const unsigned tightEle, const unsigned looseEle) {
        numTightEle_ = tightEle;
        numLooseEle_ = looseEle;
        numTightMu_ = tightMu;
        numLooseMu_ = looseMu;
    }
    void setCutConfTrigLabel(std::string newLabel) {
        cutConfTrigLabel_ = newLabel;
    }
    void setInvLepCharge(bool invLepCharge) {
        invertLepCharge_ = invLepCharge;
    }
    void setInvLepIso(bool invLepIso) {
        invertLepIso_ = invLepIso;
    }
    void setTriggerFlag(std::string triggerFlag) {
        triggerFlag_ = triggerFlag;
    }
    void setBTagPlots(std::vector<TH2D*> vec, bool makePlotsOrRead) {
        makeBTagEffPlots_ = makePlotsOrRead;
        bTagEffPlots_ = vec;
        getBTagWeight_ = !makePlotsOrRead;
    }
    void setSkipTrig(bool skip) {
        skipTrigger_ = skip;
    }
    void setSkipScalarCut(bool skip) {
        skipScalarMassCut_ = skip;
    }
    void setMetCut(double cut) {
        metDileptonCut_ = cut;
    }
    void setMWCut(double cut) {
        invWMassCut_ = cut;
    }
    void setScalarCut(double cut) {
        scalarMassCut_ = cut;
    }
    void setMaxDileptonDeltaR(double cut) {
        maxDileptonDeltaR_ = cut;
    }
    void setHiggsCut(double cut) {
        higgsMassCut_ = cut;
    }
    void setChsCandidateMass(double mass) {
        chsMass_ = mass/1000.;
    }
    void setBlindingFlag(bool unblind) {
        unblind_ = unblind;
    }
    void setJetRegion(const unsigned nJets, const unsigned nBets, const unsigned maxJets, const unsigned maxBJets) {
        numJets_ = nJets;
        numbJets_ = nBets;
        maxJets_ = maxJets;
        maxbJets_ = maxBJets;
    }
    void parse_config(const std::string confName);
    void setBparking(bool usingBparking) {
        usingBparking_ = usingBparking;
    }
    void setAbcdFlag(bool isABCD) {
        isABCD_ = isABCD;
    }
    void setZplusControlRegionFlag(bool isZplusCR) {
        isZplusCR_ = isZplusCR;
    }

    // Simple deltaR function, because the reco namespace doesn't work or
    // something
    [[gnu::const]] static double deltaR(const double& eta1, const double& phi1, const double& eta2, const double& phi2);
    [[gnu::const]] static double deltaPhi(const double& phi1, const double& phi2);

};

#endif
