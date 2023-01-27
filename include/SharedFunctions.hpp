#ifndef _SharedFunctions_hpp_
#define _SharedFunctions_hpp_

#include "AnalysisEvent.hpp"

#include <memory>
#include <string>
#include <vector>
#include <map>

//class TH1D;
//class TFile;
//class TChain;
//class TTree;
class AnalysisEvent;

class SharedFunctions {
  public:
  
    //Constructor
    SharedFunctions(bool MCTruth);
    virtual ~SharedFunctions();
    // for all
    std::vector<int> getLooseMuons(const AnalysisEvent& event);
    std::vector<int> getChargedHadronTracks(const AnalysisEvent& event);
    bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth = false);
    bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool mcTruth = false);
    int getMuonTrackPairIndex(const AnalysisEvent& event);
    int getChsTrackPairIndex(const AnalysisEvent& event);
    float deltaR(float eta1, float phi1, float eta2, float phi2);
    // for MCtruth
    bool scalarAncestry(const AnalysisEvent& event, const Int_t k, const Int_t ancestorId, const bool verbose);
    int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max);
    
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

    // debug flag
    bool useMCTruth_;

};

#endif

