
#ifndef _SimpleAnalysis_hpp_
#define _SimpleAnalysis_hpp_

#include "AnalysisEvent.hpp"

class Dataset;

class TF1;
class TH1F;
class TH2F;
class TH1I;
class TH2I;

class SimpleAnalysis 
{
    public:
    SimpleAnalysis();
    ~SimpleAnalysis();

    void parseCommandLineArguements(int argc, char* argv[]);
    void setupPlots();
    void runMainAnalysis();
    void savePlots();

    private:
    // functions
    std::string pdgIdCode (const Int_t pdgId, const bool unicode = false) const; // declaring function called below main(); pdgIdCode translate stored numerical pdgId code into a string: unicode for output to string, or latex style for ROOT plotting
    bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_) const ;

    std::pair<int, int> getPatMuonPair (const AnalysisEvent& event) const;
    std::pair<int, int> getPackedCandMuonPair(const AnalysisEvent& event, const int& muon1, const int& muon2) const;
    std::pair<int, int> getDihadronPair(const AnalysisEvent& event, const int& muon1, const int& muon2) const;

    // grab the muon track pair index for selected muons
    int getMuonTrackPairIndex(const AnalysisEvent& event, const int& muon1, const int& muon2) const;
    // grab the chs track pair index for selected muons
    int getChsTrackPairIndex(const AnalysisEvent& event, const int& chs1, const int& chs2) const;

    void fillGeneratorPlots(const AnalysisEvent& event) const;
    void fillMuonReconstructionPlots(const AnalysisEvent& event, double& eventWeight, const int& muIndex1, const int& muIndex2) const;
    void fillPackedCandidatePlots(const AnalysisEvent& event, double& eventWeight, const int& muIndex1, const int& muIndex2, const int& pionIndex1, const int& pionIndex2) const;
    void fillMuonMomentumComparisonPlots(const AnalysisEvent& event, double& eventWeight, const int& patMuonIndex1, const int& patMuonIndex2, const int& packedMuonIndex1, const int& packedMuonIndex2, const int& pionIndex1, const int& pionIndex2) const;

    // variables

     const bool debug_;

    double muonPtLeading_;
    double muonPt_;
    double muonEta_;
    double packedCandMuonDeltaR_;
    double packedCandPionDeltaR_;
    double packedCandKaonDeltaR_;
    double isoConeSize_;
    double higgsMass_;
    double higgsMassWindow_;
    double higgsMassWideWindow_;

    std::string config;
    std::vector<Dataset> datasets;

    double totalLumi;
    double usePreLumi;
    bool usePostLepTree;

    std::string outFileString;
    bool is2016_; // analysis framework is setup to run over multiple years - as we are considering 2017 conditions currently, this is set to false for safety.
    bool is2018_; // analysis framework is setup to run over multiple years - as we are considering 2017 conditions currently, this is set to false for safety.
    Long64_t nEvents; // Max number of events to consider per dataset. Default is set in config file, but can be overriden with command line arguements

      // Histos
    TH1F* h_genParPt;
    TH1F* h_genParEta;
    TH1F* h_genParPhi;
    TH1F* h_genParE;
    TH1F* h_VertexPosR;

    //Higgs boson
    TH1F* h_genParHiggsPt;
    TH1F* h_genParHiggsEta;
    TH1F* h_genParHiggsPhi;
    TH1F* h_genParHiggsE;
    TH1F* h_HiggsInvMass;

    //Scalar decay
    TH1F* h_genParScalarPt;
    TH1F* h_genParScalarEta;
    TH1F* h_genParScalarPhi;
    TH1F* h_genParScalarE;
    TH1F* h_ScalarDeltaR;
    TH1F* h_ScalarDeltaPhi;
    TH1F* h_ScalarInvMass;
    TH1F* h_Scalar3DAngle;

    //Muon from scalar decay
    TH1F* h_genParScalarMuonPtL;
    TH1F* h_genParScalarMuonPtS;
    TH1F* h_genParScalarMuonCutPtSL;
    TH1F* h_genParScalarMuonDivPtSL;
    TH1F* h_genParScalarMuonCutPtDL;
    TH1F* h_genParScalarMuonDivPtDL;
    TH1F* h_genParScalarMuonCutPtDS;
    TH1F* h_genParScalarMuonDivPtDS;

    TH1F* h_genParScalarMuonEta;
    TH1F* h_genParScalarMuonPhi;
    TH1F* h_genParScalarMuonE;
    TH1F* h_MuonDeltaR;
    TH1F* h_MuonDeltaPhi;
    TH1F* h_MuonInvMass;
    TH1F* h_Muon3DAngle;

    TH1F* h_genParScalarCKaonPt;
    TH1F* h_genParScalarCKaonEta;
    TH1F* h_genParScalarCKaonPhi;
    TH1F* h_genParScalarCKaonE;
    TH1F* h_KaonDeltaR;
    TH1F* h_KaonDeltaPhi;
    TH1F* h_KaonInvMass;
    TH1F* h_Kaon3DAngle;

    //K short from scalar decay
    TH1F* h_genParScalarKShortPt;
    TH1F* h_genParScalarKShortEta;
    TH1F* h_genParScalarKShortPhi;
    TH1F* h_genParScalarKShortE;
    TH1F* h_KShortDeltaR;
    TH1F* h_KShortDeltaPhi;
    TH1F* h_KShortInvMass;
    TH1F* h_KShort3DAngle;

    //Pion from scalar decay
    TH1F* h_genParScalarCPionPt;
    TH1F* h_genParScalarCPionEta;
    TH1F* h_genParScalarCPionPhi;
    TH1F* h_genParScalarCPionE;
    TH1F* h_genParScalarNPionPt;
    TH1F* h_genParScalarNPionEta;
    TH1F* h_genParScalarNPionPhi;
    TH1F* h_genParScalarNPionE;
    TH1F* h_PionDeltaR;
    TH1F* h_PionDeltaPhi;
    TH1F* h_PionInvMass;
    TH1F* h_Pion3DAngle;


    //Vertex position: muons, kaons, kshort, pions
    TH2F* h_VertexPosXY;
    TH2F* h_VertexPosRZ;

    //PAT Muon reconstruction
    TH1F* h_muonRecPt;
    TH1F* h_muonRecPtL;
    TH1F* h_muonRecPtS;
    TH1F* h_muonRecEta;
    TH1F* h_muonRecPhi;
    TH1F* h_muonRecE;
    TH1F* h_muonRecDeltaR;
    TH1F* h_muonRecDeltaPhi;
    TH1F* h_muonRecInvMass;
    TF1* Gaussian1;
    TH1F* h_muonCutSingleL;
    TH1F* h_muonCutDoubleL;
    TH1F* h_muonDivSingleL;
    TH1F* h_muonDivDoubleL;
    TH1F* h_muonCutDoubleS;
    TH1F* h_muonDivDoubleS;


    //Packed candidates
    TH1F* h_packedCDxy;
    TH1F* h_packedCDz;
    TH1F* h_packedCVx;
    TH1F* h_packedCVy;
    TH1F* h_packedCVz;
    TH2I* h_displacedXY;
    TH2I* h_displacedRZ;

    TH2F* h_HVertexPosXY;
    TH2F* h_HVertexPosRZ;

    //Kaon mass assumption
    TH1F* h_KhadronDeltaR;
    TH1F* h_KmuonsDeltaR;
    TH1F* h_KIsoSum1;
    TH1F* h_KIsoSum2;
    TH2F* h_KIso2;
    TH1F* h_MuonIsoSum1;
    TH1F* h_MuonIsoSum2;
    TH1F* h_KhadronInvMass;
    TH1F* h_KhadronInvMass2;
    TH1F* h_KmuonsInvMass;
    TH2F* h_Kinvmass;

    TH1F* h_KantiscalarInvMass;
    TH1F* h_KscalarInvMass;
    TH1F* h_KhiggsInvMass;
    TH1F* h_KhiggsDeltaR;

    //Pion mass assumption
    TH1F* h_PhadronDeltaR;
    TH1F* h_PmuonsDeltaR;
    TH1F* h_PIsoSum1;
    TH1F* h_PIsoSum2;
    TH2F* h_PIso2;
    TH1F* h_PIsoSum3;
    TH1F* h_PIsoSum4;
    TH2F* h_PIso4;
    TH1F* h_PhadronInvMass;
    TH1F* h_PhadronInvMass2;
    TH1F* h_PmuonsInvMass;
    TF1* Gaussian2;
    TH2F* h_Pinvmass;

    TH1F* h_PantiscalarInvMass;
    TH1F* h_PscalarInvMass;
    TF1* Gaussian3;
    TH1F* h_PhiggsInvMass;
    TH1F* h_PhiggsDeltaR;
    TH1F* h_Rrefit12InvMass;
    TH1F* h_Rpionre12InvMass;
    TH1F* h_Rkaonre12InvMass;

    TH1F* h_P20antiscalarInvMass;
    TH1F* h_P20scalarInvMass;
    TH1F* h_P20higgsInvMass;

    TH2F* h_massassump;
    TH2F* h_pmassassump;
    TH2F* h_higgsassump;

    //Comparison muon momenta

    //PAT muons
    TH1F* h_muoniRecPtTrk;
    TH1F* h_muon1RecPtTrk;
    TH1F* h_muon2RecPtTrk;
    TH1F* h_muoniRecPt;
    TH1F* h_muon1RecPt;
    TH1F* h_muon2RecPt;
    TH1F* h_muon1RecInvMass;
    TH1F* h_muon2RecInvMass;
    TH1F* h_muon12RecInvMass;

    //Packed muons
    TH1F* h_muonipackedPt;
    TH1F* h_muonipackedInvMass;;
    TH1F* h_muonipackedPtTrk;
    TH1F* h_muon1packedPt;
    TH1F* h_muon1packedInvMass;
    TH1F* h_muon1packedPtTrk;
    TH1F* h_muon2packedPt;
    TH1F* h_muon2packedInvMass;
    TH1F* h_muon2packedPtTrk;
    TH1F* h_muon12packedInvMass;

    //Refitted tracks muons
    TH1F* h_muon1PairsPt;
    TH1F* h_muon2PairsPt;
    TH1F* h_muon1refitInvMass;
    TH1F* h_muon2refitInvMass;
    TH1F* h_muon12refitInvMass;
    TH1F* h_refit1InvMass;
    TH1F* h_refit2InvMass;
    TH1F* h_refit12InvMass;

    //Refitted tracks pions
    TH1F* h_pionre12InvMass;
    TH2F* h_muonPairsXY;
    TH2F* h_muonPairsRZ;

};

#endif

