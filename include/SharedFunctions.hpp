#ifndef _SharedFunctions_hpp_
#define _SharedFunctions_hpp_

#include "AnalysisEvent.hpp"
#include "TH1F.h"
#include "TH2F.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

//class TH1D;
//class TFile;
//class TChain;
//class TTree;
class TF1;
struct Isolation;

class AnalysisEvent;

class SharedFunctions {
  public:
  
    //Constructor
    SharedFunctions(bool MCTruth, bool debug, std::map<std::string, TH1F*> *hists_1d_ref_, std::map<std::string, TH2F*> *hists_2d_ref_);
    virtual ~SharedFunctions();
    // for all
    std::vector<int> getLooseMuons(const AnalysisEvent& event, bool flag);
    std::vector<int> getChargedHadronTracks(const AnalysisEvent& event, bool flag);
    bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool flag);
    bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool flag);
    bool DiHadronPtCut(AnalysisEvent& event);
    bool MassCompatibility(AnalysisEvent& event, TF1& f_lower, TF1& f_higher);
    bool HiggsWindow(AnalysisEvent& event);
    bool getMuonTrackPairIndex(AnalysisEvent& event);
    bool getChsTrackPairIndex(AnalysisEvent& event);
    float deltaR(float eta1, float phi1, float eta2, float phi2);
    // void massAssumption(TString type="pion");
    Isolation PFIsolation(TString ptype, int trk_ind, int gen_ind, int trk_exc, const AnalysisEvent& event, double dr_max);
    Isolation PFIsolation(TString ptype, ROOT::Math::PxPyPzMVector& trk, int gen_ind, std::vector<int>& trk_exc, const AnalysisEvent& event, double dr_max); 

    // for MCtruth
    bool scalarAncestry(const AnalysisEvent& event, const Int_t& k, const Int_t& ancestorId, const bool verbose);
    int nTrksInCone(const AnalysisEvent& event, const ROOT::Math::PxPyPzMVector& particle, const Int_t particle_ch, int trkPdgId, double dr_max, bool loose, bool selective);
    int NoTrksInCone(const AnalysisEvent& event, const ROOT::Math::PxPyPzMVector& particle, std::vector<int>& trkPdgId, double dr_max, bool verboseFlag);
    int nGenParsInCone(const AnalysisEvent& event, const ROOT::Math::PxPyPzMVector& particle, const Int_t particle_ch,int ParPdgId, double dr_max, bool selective);
    bool GenLevelCheck(const AnalysisEvent& event, const bool verbose);
    int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max);
    Isolation GenIsolation(TString ptype, int gen_ind, std::vector<int>& gen_exc, const AnalysisEvent& event, double dr_max); 


    // Lepton cut variables
    float looseMuonEta_;
    float looseMuonPt_;
    float looseMuonPtLeading_;
    float looseMuonRelIsoLeading_;
    float looseMuonRelIso_;
    float invZMassCut_;
    
    // Hadron cut variables
    float chsMass_;
    float looseChsEta_;
    float looseChsPt_;
    float looseChsSumPtCh_;
    float looseChsSumPtChLeading_;


    // Diparticle cuts
    double maxDileptonDeltaR_;
    double maxChsDeltaR_;
    double scalarMassCut_;
    double higgsTolerence_;
    float diChsPt_;
    float diMuonPt_;
    bool diMuonOppCharge_;
    bool diChsOppCharge_;
    double higgsPeakLow_;
    double higgsPeakHigh_;
    double higgsPeakSideband_;
    bool SR_;

    // all the hists from main analysis code
    std::map<std::string, TH1F*> *hists_1d_;
    std::map<std::string, TH2F*> *hists_2d_;
    // TF1* func_lowerbound_;
    // TF1* func_higherbound_;


    bool useMCTruth_;

    // debug flag
    bool verbose_;

    inline void massAssumption(TString type="pion") {
      if (type.Contains("kaon")) chsMass_ = 0.493677; //kaon mass
      else chsMass_ = 0.13957061;//pion mass
      // std::cout<<"Mass assumption - "<<type<<" ("<<chsMass_<<" GeV)"<<std::endl;
      return;
    }
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

