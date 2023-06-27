#ifndef _SharedFunctions_secondary_hpp_
#define _SharedFunctions_secondary_hpp_

#include "AnalysisEvent.hpp"

#include <memory>
#include <string>
#include <vector>
#include <map>

//class TH1D;
//class TFile;
//class TChain;
//class TTree;
struct Isolation;

class AnalysisEvent;

class SharedFunctions_secondary {
  public:
  
    //Constructor
    SharedFunctions_secondary(bool MCTruth, bool debug);
    virtual ~SharedFunctions_secondary();
    // for all
    std::vector<int> getLooseMuons(const AnalysisEvent& event);
    std::vector<int> getChargedHadronTracks(const AnalysisEvent& event);
    bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons);
    bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs);
    bool DiHadronPtCut(AnalysisEvent& event);
    bool MassCompatibility(AnalysisEvent& event);
    bool HiggsWindow(AnalysisEvent& event);
    bool getMuonTrackPairIndex(AnalysisEvent& event);
    bool getChsTrackPairIndex(AnalysisEvent& event);
    float deltaR(float eta1, float phi1, float eta2, float phi2);
    // for MCtruth
    bool scalarAncestry(const AnalysisEvent& event, const Int_t& k, const Int_t& ancestorId, const bool verbose);
    int nTrksInCone(const AnalysisEvent& event, const TLorentzVector& particle, const Int_t particle_ch, int trkPdgId, double dr_max, bool loose, bool selective);
    int NoTrksInCone(const AnalysisEvent& event, const TLorentzVector& particle, std::vector<int>& trkPdgId, double dr_max, bool verboseFlag);
    int nGenParsInCone(const AnalysisEvent& event, const TLorentzVector& particle, const Int_t particle_ch,int ParPdgId, double dr_max, bool selective);
    bool GenLevelCheck(const AnalysisEvent& event, const bool verbose);
    int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max);
    Isolation PFIsolation(TString ptype, int trk_ind, int gen_ind, int trk_exc, const AnalysisEvent& event, double dr_max);
    Isolation PFIsolation(TString ptype, TLorentzVector& trk, int gen_ind, std::vector<int>& trk_exc, const AnalysisEvent& event, double dr_max); 
    Isolation GenIsolation(TString ptype, int gen_ind, std::vector<int>& gen_exc, const AnalysisEvent& event, double dr_max); 

    // Lepton cut variables
    float looseMuonEta_;
    float looseMuonPt_;
    float looseMuonPtLeading_;
    float looseMuonRelIso_;
    float invZMassCut_;
    float chsMass_;

    // Diparticle cuts
    double maxDileptonDeltaR_;
    double maxChsDeltaR_;
    double scalarMassCut_;
    double higgsTolerence_;
    float looseChsEta_;
    float looseChsPt_;
    float looseChsPtLeading_;
    float diChsPt_;

    bool useMCTruth_;

    // debug flag
    bool verbose_;

};

struct Isolation
{
  float chiso_; // charged hadrons from PV
  float nhiso_; // neutral hadrons
  float phiso_; // photons
  float puiso_; // pileup contribution (charged hadrons not from PV)   
  float reliso_; // relative isolation
  float lep_count; //no. of leptons in dr=0.4 cone
};


#endif

