//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 23 14:04:00 2014 by ROOT version 5.32/00
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef _MiniMuonAnalysisEvent_hpp_
#define _MiniMuonAnalysisEvent_hpp_

#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <iostream>
#include <string>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MiniMuonAnalysisEvent
{
    public:
    TTree* fChain; //! pointer to the analyzed TTree or TChain
    Int_t fCurrent; //! current Tree number in a TChain

    static constexpr size_t NMUONSMAX{20};
    Int_t numMuonPF2PAT;
    Float_t muonPF2PATE[NMUONSMAX];
    Float_t muonPF2PATET[NMUONSMAX];
    Float_t muonPF2PATPt[NMUONSMAX];
    Float_t muonPF2PATPX[NMUONSMAX];
    Float_t muonPF2PATPY[NMUONSMAX];
    Float_t muonPF2PATPZ[NMUONSMAX];
    Float_t muonPF2PATPhi[NMUONSMAX];
    Float_t muonPF2PATTheta[NMUONSMAX];
    Float_t muonPF2PATEta[NMUONSMAX];
    Int_t muonPF2PATCharge[NMUONSMAX];
    Int_t muonPF2PATLooseCutId[NMUONSMAX];
    Int_t muonPF2PATMediumCutId[NMUONSMAX];
    Int_t muonPF2PATTightCutId[NMUONSMAX];
    Float_t muonPF2PATGlobalID[NMUONSMAX];
    Float_t muonPF2PATTrackID[NMUONSMAX];
    Float_t muonPF2PATChi2[NMUONSMAX];
    Float_t muonPF2PATD0[NMUONSMAX];
    Float_t muonPF2PATTrackDBD0[NMUONSMAX];
    Float_t muonPF2PATDBInnerTrackD0[NMUONSMAX];
    Float_t muonPF2PATBeamSpotCorrectedD0[NMUONSMAX];
    Int_t muonPF2PATTrackNHits[NMUONSMAX];
    Int_t muonPF2PATMuonNHits[NMUONSMAX];
    Float_t muonPF2PATNDOF[NMUONSMAX];
    Float_t muonPF2PATVertX[NMUONSMAX];
    Float_t muonPF2PATVertY[NMUONSMAX];
    Float_t muonPF2PATVertZ[NMUONSMAX];
    Float_t muonPF2PATInnerTkPt[NMUONSMAX];	
    Float_t muonPF2PATInnerTkPx[NMUONSMAX];	
    Float_t muonPF2PATInnerTkPy[NMUONSMAX];	
    Float_t muonPF2PATInnerTkPz[NMUONSMAX];	
    Float_t muonPF2PATInnerTkEta[NMUONSMAX];	
    Float_t muonPF2PATInnerTkPhi[NMUONSMAX];	
    Float_t muonPF2PATChargedHadronIso[NMUONSMAX];
    Float_t muonPF2PATNeutralHadronIso[NMUONSMAX];
    Float_t muonPF2PATPhotonIso[NMUONSMAX];
    Float_t muonPF2PATTrackIso[NMUONSMAX];
    Float_t muonPF2PATEcalIso[NMUONSMAX];
    Float_t muonPF2PATHcalIso[NMUONSMAX];
    Float_t muonPF2PATComRelIso[NMUONSMAX];
    Float_t muonPF2PATComRelIsodBeta[NMUONSMAX];
    Float_t muonPF2PATChHadIso[NMUONSMAX];
    Float_t muonPF2PATNtHadIso[NMUONSMAX];
    Float_t muonPF2PATGammaIso[NMUONSMAX];
    Float_t muonPF2PATPuIso[NMUONSMAX];
    Int_t muonPF2PATIsPFMuon[NMUONSMAX];
    Int_t muonPF2PATNChambers[NMUONSMAX];
    Int_t muonPF2PATNMatches[NMUONSMAX];
    Int_t muonPF2PATTkLysWithMeasurements[NMUONSMAX];
    Int_t muonPF2PATVldPixHits[NMUONSMAX];
    Int_t muonPF2PATMatchedStations[NMUONSMAX];
    Float_t muonPF2PATImpactTransDist[NMUONSMAX];
    Float_t muonPF2PATImpactTransError[NMUONSMAX];
    Float_t muonPF2PATImpactTransSignificance[NMUONSMAX];
    Float_t muonPF2PATImpact3DDist[NMUONSMAX];
    Float_t muonPF2PATImpact3DError[NMUONSMAX];
    Float_t muonPF2PATImpact3DSignificance[NMUONSMAX];
    Float_t muonPF2PATGlbTkNormChi2[NMUONSMAX];
    Float_t muonPF2PATInnerTkNormChi2[NMUONSMAX];	
    Float_t muonPF2PATValidFraction[NMUONSMAX];
    Float_t muonPF2PATChi2LocalPosition[NMUONSMAX];
    Float_t muonPF2PATTrkKick[NMUONSMAX];
    Float_t muonPF2PATSegmentCompatibility[NMUONSMAX];
    Float_t muonPF2PATDBPV[NMUONSMAX];
    Float_t muonPF2PATDBPVError[NMUONSMAX];
    Float_t muonPF2PATDZPV[NMUONSMAX];
    Float_t muonPF2PATDZPVError[NMUONSMAX];
    Float_t genMuonPF2PATPT[NMUONSMAX];
    Float_t genMuonPF2PATET[NMUONSMAX];
    Float_t genMuonPF2PATPX[NMUONSMAX];
    Float_t genMuonPF2PATPY[NMUONSMAX];
    Float_t genMuonPF2PATPZ[NMUONSMAX];
    Float_t genMuonPF2PATPhi[NMUONSMAX];
    Float_t genMuonPF2PATTheta[NMUONSMAX];
    Float_t genMuonPF2PATEta[NMUONSMAX];
    Int_t genMuonPF2PATCharge[NMUONSMAX];
    Int_t genMuonPF2PATPdgId[NMUONSMAX];
    Int_t genMuonPF2PATMotherId[NMUONSMAX];
    Int_t genMuonPF2PATPromptDecayed[NMUONSMAX];
    Int_t genMuonPF2PATPromptFinalState[NMUONSMAX];
    Int_t genMuonPF2PATHardProcess[NMUONSMAX];
    Int_t genMuonPF2PATPythiaSixStatusThree[NMUONSMAX];

    Int_t isElePlusJets;
    Float_t genPDFScale;
    Float_t genPDFx1;
    Float_t genPDFx2;
    Int_t genPDFf1;
    Int_t genPDFf2;
    Double_t topPtReweight;
    Int_t processId;
    Float_t processPtHat;
    Double_t processMCWeight;
    Float_t beamSpotX;
    Float_t beamSpotY;
    Float_t beamSpotZ;

    Float_t mhtPt;
    Float_t mhtPy;
    Float_t mhtPx;
    Float_t mhtPhi;
    Float_t mhtSumEt;
    Float_t mhtSignif;

    static constexpr size_t NTRIGGERBITSMAX{700};
    Int_t nTriggerBits;
    Int_t TriggerBits[NTRIGGERBITSMAX];

    Double_t weight_muF0p5;
    Double_t weight_muF2;
    Double_t weight_muR0p5;
    Double_t weight_muR2;
    Double_t weight_muF0p5muR0p5;
    Double_t weight_muF2muR2;
    Double_t origWeightForNorm;
    Double_t weight_pdfMax;
    Double_t weight_pdfMin;
    Double_t weight_alphaMax;
    Double_t weight_alphaMin;
    Double_t isrRedHi{};
    Double_t fsrRedHi{};
    Double_t isrRedLo{};
    Double_t fsrRedLo{};
    Double_t isrDefHi{};
    Double_t fsrDefHi{};
    Double_t isrDefLo{};
    Double_t fsrDefLo{};
    Double_t isrConHi{};
    Double_t fsrConHi{};
    Double_t isrConLo{};
    Double_t fsrConLo{};
    //   Int_t           numVert;

    // 2016 single muon triggers
    Int_t HLT_IsoMu24_v1;
    Int_t HLT_IsoMu24_v2;
    Int_t HLT_IsoMu24_v4;
    Int_t HLT_IsoTkMu24_v1;
    Int_t HLT_IsoTkMu24_v2;
    Int_t HLT_IsoTkMu24_v3;
    Int_t HLT_IsoTkMu24_v4;
    Int_t HLT_IsoMu27_v3;
    Int_t HLT_IsoMu27_v4;
    Int_t HLT_IsoMu27_v5;
    Int_t HLT_IsoMu27_v7;
    Int_t HLT_IsoTkMu27_v3;
    Int_t HLT_IsoTkMu27_v4;
    Int_t HLT_IsoTkMu27_v5;
    Int_t HLT_IsoTkMu27_v6;
    Int_t HLT_IsoTkMu27_v7;

    // 2016 double muon triggers
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7;
    Int_t HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2;
    Int_t HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3;
    Int_t HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6;

    // 2017 single muon triggers
    Int_t HLT_IsoMu27_v8;
    Int_t HLT_IsoMu27_v9;
    Int_t HLT_IsoMu27_v10;
    Int_t HLT_IsoMu27_v11;
    Int_t HLT_IsoMu27_v12;
    Int_t HLT_IsoMu27_v13;
    Int_t HLT_IsoMu27_v14; // Also 2018

    // 2017 double muon triggers
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4; // 2018
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4; // also 2018

    // 2018 single muon triggers
    Int_t HLT_IsoMu24_v11;
    Int_t HLT_IsoMu24_v12;
    Int_t HLT_IsoMu24_v13;
    Int_t HLT_IsoMu27_v15;
    Int_t HLT_IsoMu27_v16;

    // 2018 double muon triggers
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5;
    Int_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5;

    // Displaced muon triggers

    // 2016
    Int_t HLT_DoubleMu33NoFiltersNoVtx_v2;
    Int_t HLT_DoubleMu33NoFiltersNoVtx_v3;
    Int_t HLT_DoubleMu33NoFiltersNoVtx_v5;
    Int_t HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2;
    Int_t HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3;
    Int_t HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5;
    Int_t HLT_L2DoubleMu23_NoVertex_v2;
    Int_t HLT_L2DoubleMu23_NoVertex_v3;
    Int_t HLT_L2DoubleMu23_NoVertex_v4;
    Int_t HLT_L2DoubleMu23_NoVertex_v6;

    // 2017
    Int_t HLT_DoubleMu43NoFiltersNoVtx_v2;
    Int_t HLT_DoubleMu43NoFiltersNoVtx_v3; // also 2018
    Int_t HLT_DoubleMu48NoFiltersNoVtx_v2;
    Int_t HLT_DoubleMu48NoFiltersNoVtx_v3; // also 2018
    Int_t HLT_DoubleL2Mu50_v1;
    Int_t HLT_DoubleL2Mu50_v2;

    // 2018
    Int_t HLT_DoubleL2Mu23NoVtx_2Cha_v1;
    Int_t HLT_DoubleL2Mu23NoVtx_2Cha_v2;
    Int_t HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1;
    Int_t HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2;
    Int_t HLT_DoubleL2Mu25NoVtx_2Cha_v1;
    Int_t HLT_DoubleL2Mu25NoVtx_2Cha_v2;
    Int_t HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1;
    Int_t HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2;
    Int_t HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1;
    Int_t HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1;
    Int_t HLT_DoubleMu43NoFiltersNoVtx_v4; 
    Int_t HLT_DoubleMu48NoFiltersNoVtx_v4;

    // 2018 b parking triggers
    Int_t HLT_Mu12_IP6_part0_v2;
    Int_t HLT_Mu12_IP6_part1_v2;
    Int_t HLT_Mu12_IP6_part2_v2;
    Int_t HLT_Mu12_IP6_part3_v2;
    Int_t HLT_Mu12_IP6_part4_v2;
    Int_t HLT_Mu9_IP5_part0_v2;
    Int_t HLT_Mu9_IP5_part1_v2;
    Int_t HLT_Mu9_IP5_part2_v2;
    Int_t HLT_Mu9_IP5_part3_v2;
    Int_t HLT_Mu9_IP5_part4_v2;
    Int_t HLT_Mu7_IP4_part0_v2;
    Int_t HLT_Mu7_IP4_part1_v2;
    Int_t HLT_Mu7_IP4_part2_v2;
    Int_t HLT_Mu7_IP4_part3_v2;
    Int_t HLT_Mu7_IP4_part4_v2;
    Int_t HLT_Mu8_IP5_part0_v2;
    Int_t HLT_Mu8_IP5_part1_v2;
    Int_t HLT_Mu8_IP5_part2_v2;
    Int_t HLT_Mu8_IP5_part3_v2;
    Int_t HLT_Mu8_IP5_part4_v2;
    Int_t HLT_Mu8_IP6_part0_v2;
    Int_t HLT_Mu8_IP6_part1_v2;
    Int_t HLT_Mu8_IP6_part2_v2;
    Int_t HLT_Mu8_IP6_part3_v2;
    Int_t HLT_Mu8_IP6_part4_v2;
    Int_t HLT_Mu9_IP6_part0_v1;
    Int_t HLT_Mu9_IP6_part1_v1;
    Int_t HLT_Mu9_IP6_part2_v1;
    Int_t HLT_Mu9_IP6_part3_v1;
    Int_t HLT_Mu9_IP6_part4_v1;
    Int_t HLT_Mu9_IP6_part0_v2;
    Int_t HLT_Mu9_IP6_part1_v2;
    Int_t HLT_Mu9_IP6_part2_v2;
    Int_t HLT_Mu9_IP6_part3_v2;
    Int_t HLT_Mu9_IP6_part4_v2;
    Int_t HLT_Mu9_IP6_part0_v3;
    Int_t HLT_Mu9_IP6_part1_v3;
    Int_t HLT_Mu9_IP6_part2_v3;
    Int_t HLT_Mu9_IP6_part3_v3;
    Int_t HLT_Mu9_IP6_part4_v3;
	
/*
    // 2017 Data scouting triggers
    Int_t DST_HT250_CaloScouting_v6;
    Int_t DST_HT250_CaloScouting_v7;
    Int_t DST_HT250_CaloScouting_v8;
    Int_t DST_HT250_CaloScouting_v9;
    Int_t DST_HT250_CaloBTagScouting_v5;
    Int_t DST_HT250_CaloBTagScouting_v6;
    Int_t DST_HT250_CaloBTagScouting_v7;
    Int_t DST_HT250_CaloBTagScouting_v8;
    Int_t DST_HT410_PFScouting_v9;
    Int_t DST_HT410_PFScouting_v10;
    Int_t DST_HT410_PFScouting_v11;
    Int_t DST_HT410_PFScouting_v12;
    Int_t DST_HT410_PFScouting_v13;
    Int_t DST_HT410_PFScouting_v14;
    Int_t DST_HT410_BTagScouting_v10;
    Int_t DST_HT410_BTagScouting_v11;
    Int_t DST_HT410_BTagScouting_v12;
    Int_t DST_HT410_BTagScouting_v13;
    Int_t DST_HT410_BTagScouting_v14;
    Int_t DST_DoubleMu3_noVtx_CaloScouting_v2;
    Int_t DST_DoubleMu3_noVtx_CaloScouting_v3;
    Int_t DST_DoubleMu3_noVtx_CaloScouting_v4;
    Int_t DST_DoubleMu3_noVtx_CaloScouting_v5;
*/
    // MET Filters
    Int_t Flag_goodVertices;
    Int_t Flag_globalTightHalo2016Filter;
    Int_t Flag_HBHENoiseFilter;
    Int_t Flag_HBHENoiseIsoFilter;
    Int_t Flag_EcalDeadCellTriggerPrimitiveFilter;
    Int_t Flag_BadPFMuonFilter;
    Int_t Flag_BadChargedCandidateFilter;
    Int_t Flag_eeBadScFilter;
    Int_t Flag_ecalBadCalibFilter;

    // Gen info
    static constexpr size_t NGENPARMAX{1000};
    Int_t nGenPar;
    Float_t genParEta[NGENPARMAX];
    Float_t genParPhi[NGENPARMAX];
    Float_t genParE[NGENPARMAX];
    Float_t genParPt[NGENPARMAX];
    Int_t genParId[NGENPARMAX];
    Float_t genParVx[NGENPARMAX];
    Float_t genParVy[NGENPARMAX];
    Float_t genParVz[NGENPARMAX];
    Int_t genParMotherId[NGENPARMAX];
    Int_t genParMotherIndex[NGENPARMAX];
    Int_t genParNumDaughters[NGENPARMAX];
    Int_t genParDaughterId1[NGENPARMAX];
    Int_t genParDaughterId2[NGENPARMAX];
    Int_t genParDaughter1Index[NGENPARMAX];
    Int_t genParDaughter2Index[NGENPARMAX];
    Int_t genParStatus[NGENPARMAX];
    Int_t genParCharge[NGENPARMAX];
    Int_t eventRun;
    Int_t eventNum;
    Float_t eventLumiblock;

    TBranch* b_numMuonPF2PAT; //!
    TBranch* b_muonPF2PATE; //!
    TBranch* b_muonPF2PATET; //!
    TBranch* b_muonPF2PATPt; //!
    TBranch* b_muonPF2PATPX; //!
    TBranch* b_muonPF2PATPY; //!
    TBranch* b_muonPF2PATPZ; //!
    TBranch* b_muonPF2PATPhi; //!
    TBranch* b_muonPF2PATTheta; //!
    TBranch* b_muonPF2PATEta; //!
    TBranch* b_muonPF2PATCharge; //!
    TBranch* b_muonPF2PATLooseCutId; //!
    TBranch* b_muonPF2PATMediumCutId; //!
    TBranch* b_muonPF2PATTightCutId; //!
    TBranch* b_muonPF2PATGlobalID; //!
    TBranch* b_muonPF2PATTrackID; //!
    TBranch* b_muonPF2PATChi2; //!
    TBranch* b_muonPF2PATD0; //!
    TBranch* b_muonPF2PATTrackDBD0; //!
    TBranch* b_muonPF2PATDBInnerTrackD0; //!
    TBranch* b_muonPF2PATBeamSpotCorrectedD0; //!
    TBranch* b_muonPF2PATTrackNHits; //!
    TBranch* b_muonPF2PATMuonNHits; //!
    TBranch* b_muonPF2PATNDOF; //!
    TBranch* b_muonPF2PATVertX; //!
    TBranch* b_muonPF2PATVertY; //!
    TBranch* b_muonPF2PATVertZ; //!
    TBranch* b_muonPF2PATInnerTkPt; //!	
    TBranch* b_muonPF2PATInnerTkPx; //!	
    TBranch* b_muonPF2PATInnerTkPy; //!	
    TBranch* b_muonPF2PATInnerTkPz; //!            	
    TBranch* b_muonPF2PATInnerTkEta; //!	
    TBranch* b_muonPF2PATInnerTkPhi; //!	
    TBranch* b_muonPF2PATChargedHadronIso; //!
    TBranch* b_muonPF2PATNeutralHadronIso; //!
    TBranch* b_muonPF2PATPhotonIso; //!
    TBranch* b_muonPF2PATTrackIso; //!
    TBranch* b_muonPF2PATEcalIso; //!
    TBranch* b_muonPF2PATHcalIso; //!
    TBranch* b_muonPF2PATComRelIso; //!
    TBranch* b_muonPF2PATComRelIsodBeta; //!
    TBranch* b_muonPF2PATChHadIso; //!
    TBranch* b_muonPF2PATNtHadIso; //!
    TBranch* b_muonPF2PATGammaIso; //!
    TBranch* b_muonPF2PATPuIso; //!
    TBranch* b_muonPF2PATIsPFMuon; //!
    TBranch* b_muonPF2PATNChambers; //!
    TBranch* b_muonPF2PATNMatches; //!
    TBranch* b_muonPF2PATTkLysWithMeasurements; //!
    TBranch* b_muonPF2PATVldPixHits; //!
    TBranch* b_muonPF2PATMatchedStations; //!
    TBranch* b_muonPF2PATImpactTransDist; //!
    TBranch* b_muonPF2PATImpactTransError; //!
    TBranch* b_muonPF2PATImpactTransSignificance; //!
    TBranch* b_muonPF2PATImpact3DDist; //!
    TBranch* b_muonPF2PATImpact3DError; //!
    TBranch* b_muonPF2PATImpact3DSignificance; //!
    TBranch* b_muonPF2PATGlbTkNormChi2; //!
    TBranch* b_muonPF2PATInnerTkNormChi2; //!	
    TBranch* b_muonPF2PATValidFraction; //!
    TBranch* b_muonPF2PATChi2LocalPosition; //!
    TBranch* b_muonPF2PATTrkKick; //!
    TBranch* b_muonPF2PATSegmentCompatibility; //!
    TBranch* b_muonPF2PATDBPV; //!
    TBranch* b_muonPF2PATDBPVError; //!
    TBranch* b_muonPF2PATDZPV; //!
    TBranch* b_muonPF2PATDZPVError; //!
    TBranch* b_genMuonPF2PATPT; //!
    TBranch* b_genMuonPF2PATET; //!
    TBranch* b_genMuonPF2PATPX; //!
    TBranch* b_genMuonPF2PATPY; //!
    TBranch* b_genMuonPF2PATPZ; //!
    TBranch* b_genMuonPF2PATPhi; //!
    TBranch* b_genMuonPF2PATTheta; //!
    TBranch* b_genMuonPF2PATEta; //!
    TBranch* b_genMuonPF2PATCharge; //!
    TBranch* b_genMuonPF2PATPdgId; //!
    TBranch* b_genMuonPF2PATMotherId; //!
    TBranch* b_genMuonPF2PATPromptDecayed; //!
    TBranch* b_genMuonPF2PATPromptFinalState; //!
    TBranch* b_genMuonPF2PATHardProcess; //!
    TBranch* b_genMuonPF2PATPythiaSixStatusThree; //!   
    TBranch* b_isElePlusJets; //!
    TBranch* b_genPDFScale; //!
    TBranch* b_genPDFx1; //!
    TBranch* b_genPDFx2; //!
    TBranch* b_genPDFf1; //!
    TBranch* b_genPDFf2; //!
    TBranch* b_topPtReweight; //!
    TBranch* b_processId; //!
    TBranch* b_processPtHat; //!
    TBranch* b_processMCWeight; //!
    TBranch* b_beamSpotX; //!
    TBranch* b_beamSpotY; //!
    TBranch* b_beamSpotZ; //!
    TBranch* b_mhtPt; //!
    TBranch* b_mhtPy; //!
    TBranch* b_mhtPx; //!
    TBranch* b_mhtPhi; //!
    TBranch* b_mhtSumEt; //!
    TBranch* b_mhtSignif; //!
    TBranch* b_nTriggerBits; //!
    TBranch* b_TriggerBits; //!
    TBranch* b_weight_muF0p5; //!
    TBranch* b_weight_muF2; //!
    TBranch* b_weight_muR0p5; //!
    TBranch* b_weight_muR2; //!
    TBranch* b_weight_muF0p5muR0p5; //!
    TBranch* b_weight_muF2muR2; //!
    TBranch* b_origWeightForNorm; //!
    TBranch* b_weight_pdfMax; //!
    TBranch* b_weight_pdfMin; //!
    TBranch* b_weight_alphaMax; //!
    TBranch* b_weight_alphaMin; //!
    TBranch* b_isrRedHi;
    TBranch* b_fsrRedHi;
    TBranch* b_isrRedLo;
    TBranch* b_fsrRedLo;
    TBranch* b_isrDefHi;
    TBranch* b_fsrDefHi;
    TBranch* b_isrDefLo;
    TBranch* b_fsrDefLo;
    TBranch* b_isrConHi;
    TBranch* b_fsrConHi;
    TBranch* b_isrConLo;
    TBranch* b_fsrConLo;
    //   TBranch        *b_numVert;    //!

    TBranch* b_HLT_IsoMu24_v1;
    TBranch* b_HLT_IsoMu24_v2;
    TBranch* b_HLT_IsoMu24_v4;
    TBranch* b_HLT_IsoTkMu24_v1;
    TBranch* b_HLT_IsoTkMu24_v2;
    TBranch* b_HLT_IsoTkMu24_v3;
    TBranch* b_HLT_IsoTkMu24_v4;
    TBranch* b_HLT_IsoMu27_v3;
    TBranch* b_HLT_IsoMu27_v4;
    TBranch* b_HLT_IsoMu27_v5;
    TBranch* b_HLT_IsoMu27_v7;
    TBranch* b_HLT_IsoTkMu27_v3;
    TBranch* b_HLT_IsoTkMu27_v4;
    TBranch* b_HLT_IsoTkMu27_v5;
    TBranch* b_HLT_IsoTkMu27_v6;
    TBranch* b_HLT_IsoTkMu27_v7;

    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7;
    TBranch* b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2;
    TBranch* b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3;
    TBranch* b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6;

    TBranch* b_HLT_IsoMu27_v8;
    TBranch* b_HLT_IsoMu27_v9;
    TBranch* b_HLT_IsoMu27_v10;
    TBranch* b_HLT_IsoMu27_v11;
    TBranch* b_HLT_IsoMu27_v12;
    TBranch* b_HLT_IsoMu27_v13;
    TBranch* b_HLT_IsoMu27_v14;
    TBranch* b_HLT_IsoMu24_v11; 
    TBranch* b_HLT_IsoMu24_v12; 
    TBranch* b_HLT_IsoMu24_v13;
    TBranch* b_HLT_IsoMu27_v15;
    TBranch* b_HLT_IsoMu27_v16;

    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5;

    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3;
    TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4;

    TBranch* b_HLT_DoubleMu33NoFiltersNoVtx_v2;
    TBranch* b_HLT_DoubleMu33NoFiltersNoVtx_v3;
    TBranch* b_HLT_DoubleMu33NoFiltersNoVtx_v5;
    TBranch* b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2;
    TBranch* b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3;
    TBranch* b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5;
    TBranch* b_HLT_L2DoubleMu23_NoVertex_v2;
    TBranch* b_HLT_L2DoubleMu23_NoVertex_v3;
    TBranch* b_HLT_L2DoubleMu23_NoVertex_v4;
    TBranch* b_HLT_L2DoubleMu23_NoVertex_v6;

    TBranch* b_HLT_DoubleMu43NoFiltersNoVtx_v2;
    TBranch* b_HLT_DoubleMu43NoFiltersNoVtx_v3;
    TBranch* b_HLT_DoubleMu48NoFiltersNoVtx_v2;
    TBranch* b_HLT_DoubleMu48NoFiltersNoVtx_v3;
    TBranch* b_HLT_DoubleL2Mu50_v1;
    TBranch* b_HLT_DoubleL2Mu50_v2;

    TBranch* b_HLT_DoubleL2Mu23NoVtx_2Cha_v1;
    TBranch* b_HLT_DoubleL2Mu23NoVtx_2Cha_v2;
    TBranch* b_HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1;
    TBranch* b_HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2;
    TBranch* b_HLT_DoubleL2Mu25NoVtx_2Cha_v1;
    TBranch* b_HLT_DoubleL2Mu25NoVtx_2Cha_v2;
    TBranch* b_HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1;
    TBranch* b_HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2;
    TBranch* b_HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1;
    TBranch* b_HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1;
    TBranch* b_HLT_DoubleMu43NoFiltersNoVtx_v4; 
    TBranch* b_HLT_DoubleMu48NoFiltersNoVtx_v4;

    TBranch* b_HLT_Mu12_IP6_part0_v2;
    TBranch* b_HLT_Mu12_IP6_part1_v2;
    TBranch* b_HLT_Mu12_IP6_part2_v2;
    TBranch* b_HLT_Mu12_IP6_part3_v2;
    TBranch* b_HLT_Mu12_IP6_part4_v2;
    TBranch* b_HLT_Mu9_IP5_part0_v2;
    TBranch* b_HLT_Mu9_IP5_part1_v2;
    TBranch* b_HLT_Mu9_IP5_part2_v2;
    TBranch* b_HLT_Mu9_IP5_part3_v2;
    TBranch* b_HLT_Mu9_IP5_part4_v2;
    TBranch* b_HLT_Mu7_IP4_part0_v2;
    TBranch* b_HLT_Mu7_IP4_part1_v2;
    TBranch* b_HLT_Mu7_IP4_part2_v2;
    TBranch* b_HLT_Mu7_IP4_part3_v2;
    TBranch* b_HLT_Mu7_IP4_part4_v2;
    TBranch* b_HLT_Mu8_IP5_part0_v2;
    TBranch* b_HLT_Mu8_IP5_part1_v2;
    TBranch* b_HLT_Mu8_IP5_part2_v2;
    TBranch* b_HLT_Mu8_IP5_part3_v2;
    TBranch* b_HLT_Mu8_IP5_part4_v2;
    TBranch* b_HLT_Mu8_IP6_part0_v2;
    TBranch* b_HLT_Mu8_IP6_part1_v2;
    TBranch* b_HLT_Mu8_IP6_part2_v2;
    TBranch* b_HLT_Mu8_IP6_part3_v2;
    TBranch* b_HLT_Mu8_IP6_part4_v2;
    TBranch* b_HLT_Mu9_IP6_part0_v1;
    TBranch* b_HLT_Mu9_IP6_part1_v1;
    TBranch* b_HLT_Mu9_IP6_part2_v1;
    TBranch* b_HLT_Mu9_IP6_part3_v1;
    TBranch* b_HLT_Mu9_IP6_part4_v1;
    TBranch* b_HLT_Mu9_IP6_part0_v2;
    TBranch* b_HLT_Mu9_IP6_part1_v2;
    TBranch* b_HLT_Mu9_IP6_part2_v2;
    TBranch* b_HLT_Mu9_IP6_part3_v2;
    TBranch* b_HLT_Mu9_IP6_part4_v2;
    TBranch* b_HLT_Mu9_IP6_part0_v3;
    TBranch* b_HLT_Mu9_IP6_part1_v3;
    TBranch* b_HLT_Mu9_IP6_part2_v3;
    TBranch* b_HLT_Mu9_IP6_part3_v3;
    TBranch* b_HLT_Mu9_IP6_part4_v3;

/*
    TBranch* b_DST_HT250_CaloScouting_v6;
    TBranch* b_DST_HT250_CaloScouting_v7;
    TBranch* b_DST_HT250_CaloScouting_v8;
    TBranch* b_DST_HT250_CaloScouting_v9;
    TBranch* b_DST_HT250_CaloBTagScouting_v5;
    TBranch* b_DST_HT250_CaloBTagScouting_v6;
    TBranch* b_DST_HT250_CaloBTagScouting_v7;
    TBranch* b_DST_HT250_CaloBTagScouting_v8;
    TBranch* b_DST_HT410_PFScouting_v9;
    TBranch* b_DST_HT410_PFScouting_v10;
    TBranch* b_DST_HT410_PFScouting_v11;
    TBranch* b_DST_HT410_PFScouting_v12;
    TBranch* b_DST_HT410_PFScouting_v13;
    TBranch* b_DST_HT410_PFScouting_v14;
    TBranch* b_DST_HT410_BTagScouting_v10;
    TBranch* b_DST_HT410_BTagScouting_v11;
    TBranch* b_DST_HT410_BTagScouting_v12;
    TBranch* b_DST_HT410_BTagScouting_v13;
    TBranch* b_DST_HT410_BTagScouting_v14;
    TBranch* b_DST_DoubleMu3_noVtx_CaloScouting_v2;
    TBranch* b_DST_DoubleMu3_noVtx_CaloScouting_v3;
    TBranch* b_DST_DoubleMu3_noVtx_CaloScouting_v4;
    TBranch* b_DST_DoubleMu3_noVtx_CaloScouting_v5;
*/
    TBranch* b_Flag_goodVertices;
    TBranch* b_Flag_globalTightHalo2016Filter;
    TBranch* b_Flag_HBHENoiseFilter;
    TBranch* b_Flag_HBHENoiseIsoFilter;
    TBranch* b_Flag_EcalDeadCellTriggerPrimitiveFilter;
    TBranch* b_Flag_BadPFMuonFilter;
    TBranch* b_Flag_BadChargedCandidateFilter;
    TBranch* b_Flag_eeBadScFilter;
    TBranch* b_Flag_ecalBadCalibFilter;

    TBranch* b_nGenPar; //!
    TBranch* b_genParEta; //!
    TBranch* b_genParPhi; //!
    TBranch* b_genParE; //!
    TBranch* b_genParPt; //!
    TBranch* b_genParId; //!
    TBranch* b_genParVx; //!
    TBranch* b_genParVy; //!
    TBranch* b_genParVz; //!
    TBranch* b_genParMotherId; //!
    TBranch* b_genParMotherIndex; //!
    TBranch* b_genParNumDaughters; //!
    TBranch* b_genParDaughterId1; //!
    TBranch* b_genParDaughterId2; //!
    TBranch* b_genParDaughter1Index; //!
    TBranch* b_genParDaughter2Index; //!
    TBranch* b_genParStatus; //!
    TBranch* b_genParCharge; //!
    TBranch* b_eventRun; //!
    TBranch* b_eventNum; //!
    TBranch* b_eventLumiblock; //!

    bool isMC_{};
    const bool is2016_{};
    const bool is2018_{};

    std::vector<double> muonMomentumSF;
    std::vector<double> jetSmearValue;

    std::vector<int> electronIndexTight;
    std::vector<int> electronIndexLoose;
    std::vector<int> muonIndexTight;
    std::vector<int> muonIndexLoose;
    std::vector<int> photonIndexTight;
    std::vector<int> photonIndexLoose;
    std::vector<int> jetIndex;
    std::vector<int> displacedJetIndex;
    std::vector<int> chsIndex;
    double totalJetHt;
    double selectedJetIncHt;
    double selectedJetTracksHt;
    std::vector<int> bTagIndex;

    std::pair<TLorentzVector, TLorentzVector> zPairLeptons;
    std::pair<TLorentzVector, TLorentzVector> zPairLeptonsRefitted;
    std::pair<float, float> zPairRelIso;
    std::pair<float, float> zPairNewRelIso;
    std::pair<float, float> zPairNewTrkIso;
    std::pair<float, float> zPairRelIsoDbeta;
    std::pair<float, float> zPairNewRelIsoDbeta;
    std::pair<float, float> zPairNewTrkIsoDbeta;
    std::pair<int, int> zPairIndex;
    int mumuTrkIndex;
    float zRelIso;
    float zTrkIso;
    float zRelIsoDbeta;
    float zTrkIsoDbeta;

    std::pair<TLorentzVector, TLorentzVector> chsPairVec;
    std::pair<TLorentzVector, TLorentzVector> chsPairTrkVec;
    std::pair<TLorentzVector, TLorentzVector> chsPairTrkVecRefitted;
    std::pair<float, float> chsPairRelIso;
    std::pair<float, float> chsPairTrkIso;
    std::pair<float, float> chsPairRelIsoDbeta;
    std::pair<float, float> chsPairTrkIsoDbeta;
    std::pair<int, int> chsPairIndex;
    int chsPairTrkIndex;
    float chsRelIso;
    float chsTrkIso;
    float chsRelIsoDbeta;
    float chsTrkIsoDbeta;

    std::pair<TLorentzVector, TLorentzVector> wPairQuarks;
    std::pair<int, int> wPairIndex;

    TLorentzVector wLepton;
    int wLepIndex;
    float wLeptonRelIso;

    Int_t numVert;
    TBranch* b_numVert;

    MiniMuonAnalysisEvent(bool isMC, TTree* tree, bool is2016, bool is2018);
    virtual ~MiniMuonAnalysisEvent();
    Int_t GetEntry(const Long64_t entry);
    Long64_t LoadTree(const Long64_t entry);
    void Loop();
    void Show(const Long64_t entry = -1) const;
    bool muTrig() const;
    bool mumuTrig() const;
    bool mumuTrig_noMassCut() const;
    bool mumuTrig_massCut() const;
    bool mumuL2Trig() const;
    bool mumuNoVtxTrig() const;
    bool bParkingTrigInclusive() const;
    bool bParkingTrig_Mu9_IP5() const;
    bool bParkingTrig_Mu9_IP6() const;
    bool bParkingTrig_Mu12_IP6() const;
//    bool dataScoutingTrig() const;

    bool metFilters() const;

};

inline MiniMuonAnalysisEvent::MiniMuonAnalysisEvent(const bool isMC, TTree* tree, const bool is2016, const bool is2018) : fChain{nullptr}, isMC_{isMC}, is2016_{is2016}, is2018_{is2018} {
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == nullptr) {
#ifdef SINGLE_TREE
        // The following code should be used if you want this class to access
        // a single tree instead of a chain
        TFile* f = (TFile*)gROOT->GetListOfFiles()->FindObject(
            "/data1/tW2012/mc/ttbarInclusive/MC_Ntuple_out_9_0_MJP_skim.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("/data1/tW2012/mc/ttbarInclusive/"
                          "MC_Ntuple_out_9_0_MJP_skim.root");
        }
        f->GetObject("tree", tree);

#else // SINGLE_TREE

        // The following code should be used if you want this class to access a
        // chain of trees.
        TChain* chain = new TChain("tree", "");
        chain->Add("/data1/tW2012/mc/ttbarInclusive/"
                   "MC_Ntuple_out_100_0_Gu6_skim.root/tree");
        tree = chain;
#endif // SINGLE_TREE
    }

    // clang-format off
   if (!tree) {
       return;
   }
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("numMuonPF2PAT", &numMuonPF2PAT, &b_numMuonPF2PAT);
   fChain->SetBranchAddress("muonPF2PATE", muonPF2PATE, &b_muonPF2PATE);
   fChain->SetBranchAddress("muonPF2PATET", muonPF2PATET, &b_muonPF2PATET);
   fChain->SetBranchAddress("muonPF2PATPt", muonPF2PATPt, &b_muonPF2PATPt);
   fChain->SetBranchAddress("muonPF2PATPX", muonPF2PATPX, &b_muonPF2PATPX);
   fChain->SetBranchAddress("muonPF2PATPY", muonPF2PATPY, &b_muonPF2PATPY);
   fChain->SetBranchAddress("muonPF2PATPZ", muonPF2PATPZ, &b_muonPF2PATPZ);
   fChain->SetBranchAddress("muonPF2PATPhi", muonPF2PATPhi, &b_muonPF2PATPhi);
   fChain->SetBranchAddress("muonPF2PATTheta", muonPF2PATTheta, &b_muonPF2PATTheta);
   fChain->SetBranchAddress("muonPF2PATEta", muonPF2PATEta, &b_muonPF2PATEta);
   fChain->SetBranchAddress("muonPF2PATCharge", muonPF2PATCharge, &b_muonPF2PATCharge);
   fChain->SetBranchAddress("muonPF2PATLooseCutId", muonPF2PATLooseCutId, &b_muonPF2PATLooseCutId);
   fChain->SetBranchAddress("muonPF2PATMediumCutId", muonPF2PATMediumCutId, &b_muonPF2PATMediumCutId);
   fChain->SetBranchAddress("muonPF2PATTightCutId", muonPF2PATTightCutId, &b_muonPF2PATTightCutId);
   fChain->SetBranchAddress("muonPF2PATGlobalID", muonPF2PATGlobalID, &b_muonPF2PATGlobalID);
   fChain->SetBranchAddress("muonPF2PATTrackID", muonPF2PATTrackID, &b_muonPF2PATTrackID);
   fChain->SetBranchAddress("muonPF2PATChi2", muonPF2PATChi2, &b_muonPF2PATChi2);
   fChain->SetBranchAddress("muonPF2PATD0", muonPF2PATD0, &b_muonPF2PATD0);
   fChain->SetBranchAddress("muonPF2PATTrackDBD0", muonPF2PATTrackDBD0, &b_muonPF2PATTrackDBD0);
   fChain->SetBranchAddress("muonPF2PATDBInnerTrackD0", muonPF2PATDBInnerTrackD0, &b_muonPF2PATDBInnerTrackD0);
   fChain->SetBranchAddress("muonPF2PATBeamSpotCorrectedD0", muonPF2PATBeamSpotCorrectedD0, &b_muonPF2PATBeamSpotCorrectedD0);
   fChain->SetBranchAddress("muonPF2PATTrackNHits", muonPF2PATTrackNHits, &b_muonPF2PATTrackNHits);
   fChain->SetBranchAddress("muonPF2PATMuonNHits", muonPF2PATMuonNHits, &b_muonPF2PATMuonNHits);
   fChain->SetBranchAddress("muonPF2PATNDOF", muonPF2PATNDOF, &b_muonPF2PATNDOF);
   fChain->SetBranchAddress("muonPF2PATVertX", muonPF2PATVertX, &b_muonPF2PATVertX);
   fChain->SetBranchAddress("muonPF2PATVertY", muonPF2PATVertY, &b_muonPF2PATVertY);
   fChain->SetBranchAddress("muonPF2PATVertZ", muonPF2PATVertZ, &b_muonPF2PATVertZ);
   fChain->SetBranchAddress("muonPF2PATInnerTkPt", muonPF2PATInnerTkPt, &b_muonPF2PATInnerTkPt);	
   fChain->SetBranchAddress("muonPF2PATInnerTkPx", muonPF2PATInnerTkPx, &b_muonPF2PATInnerTkPx);	
   fChain->SetBranchAddress("muonPF2PATInnerTkPy", muonPF2PATInnerTkPy, &b_muonPF2PATInnerTkPy);	
   fChain->SetBranchAddress("muonPF2PATInnerTkPz", muonPF2PATInnerTkPz, &b_muonPF2PATInnerTkPz);	
   fChain->SetBranchAddress("muonPF2PATInnerTkEta", muonPF2PATInnerTkEta, &b_muonPF2PATInnerTkEta);	
   fChain->SetBranchAddress("muonPF2PATInnerTkPhi", muonPF2PATInnerTkPhi, &b_muonPF2PATInnerTkPhi);	
   fChain->SetBranchAddress("muonPF2PATChargedHadronIso", muonPF2PATChargedHadronIso, &b_muonPF2PATChargedHadronIso);
   fChain->SetBranchAddress("muonPF2PATNeutralHadronIso", muonPF2PATNeutralHadronIso, &b_muonPF2PATNeutralHadronIso);
   fChain->SetBranchAddress("muonPF2PATPhotonIso", muonPF2PATPhotonIso, &b_muonPF2PATPhotonIso);
   fChain->SetBranchAddress("muonPF2PATTrackIso", muonPF2PATTrackIso, &b_muonPF2PATTrackIso);
   fChain->SetBranchAddress("muonPF2PATEcalIso", muonPF2PATEcalIso, &b_muonPF2PATEcalIso);
   fChain->SetBranchAddress("muonPF2PATHcalIso", muonPF2PATHcalIso, &b_muonPF2PATHcalIso);
   fChain->SetBranchAddress("muonPF2PATComRelIso", muonPF2PATComRelIso, &b_muonPF2PATComRelIso);
   fChain->SetBranchAddress("muonPF2PATComRelIsodBeta", muonPF2PATComRelIsodBeta, &b_muonPF2PATComRelIsodBeta);
   fChain->SetBranchAddress("muonPF2PATChHadIso", muonPF2PATChHadIso, &b_muonPF2PATChHadIso);
   fChain->SetBranchAddress("muonPF2PATNtHadIso", muonPF2PATNtHadIso, &b_muonPF2PATNtHadIso);
   fChain->SetBranchAddress("muonPF2PATGammaIso", muonPF2PATGammaIso, &b_muonPF2PATGammaIso);
   fChain->SetBranchAddress("muonPF2PATPuIso", muonPF2PATPuIso, &b_muonPF2PATPuIso);
   fChain->SetBranchAddress("muonPF2PATIsPFMuon", muonPF2PATIsPFMuon, &b_muonPF2PATIsPFMuon);
   fChain->SetBranchAddress("muonPF2PATNChambers", muonPF2PATNChambers, &b_muonPF2PATNChambers);
   fChain->SetBranchAddress("muonPF2PATNMatches", muonPF2PATNMatches, &b_muonPF2PATNMatches);
   fChain->SetBranchAddress("muonPF2PATTkLysWithMeasurements", muonPF2PATTkLysWithMeasurements, &b_muonPF2PATTkLysWithMeasurements);
   fChain->SetBranchAddress("muonPF2PATGlbTkNormChi2", muonPF2PATGlbTkNormChi2, &b_muonPF2PATGlbTkNormChi2);
   fChain->SetBranchAddress("muonPF2PATInnerTkNormChi2", muonPF2PATInnerTkNormChi2, &b_muonPF2PATInnerTkNormChi2);	
   fChain->SetBranchAddress("muonPF2PATValidFraction", muonPF2PATValidFraction, &b_muonPF2PATValidFraction);
   fChain->SetBranchAddress("muonPF2PATChi2LocalPosition", muonPF2PATChi2LocalPosition, &b_muonPF2PATChi2LocalPosition);
   fChain->SetBranchAddress("muonPF2PATTrkKick", muonPF2PATTrkKick, &b_muonPF2PATTrkKick);
   fChain->SetBranchAddress("muonPF2PATSegmentCompatibility", muonPF2PATSegmentCompatibility, &b_muonPF2PATSegmentCompatibility);
   fChain->SetBranchAddress("muonPF2PATDBPV", muonPF2PATDBPV, &b_muonPF2PATDBPV);
   fChain->SetBranchAddress("muonPF2PATDBPVError", muonPF2PATDBPVError, &b_muonPF2PATDBPVError);
   fChain->SetBranchAddress("muonPF2PATDZPV", muonPF2PATDZPV, &b_muonPF2PATDZPV);
   fChain->SetBranchAddress("muonPF2PATDZPVError", muonPF2PATDZPVError, &b_muonPF2PATDZPVError);
   fChain->SetBranchAddress("muonPF2PATVldPixHits", muonPF2PATVldPixHits, &b_muonPF2PATVldPixHits);
   fChain->SetBranchAddress("muonPF2PATMatchedStations", muonPF2PATMatchedStations, &b_muonPF2PATMatchedStations);
   fChain->SetBranchAddress("muonPF2PATImpactTransDist", muonPF2PATImpactTransDist, &b_muonPF2PATImpactTransDist);
   fChain->SetBranchAddress("muonPF2PATImpactTransError", muonPF2PATImpactTransError, &b_muonPF2PATImpactTransError);
   fChain->SetBranchAddress("muonPF2PATImpactTransSignificance", muonPF2PATImpactTransSignificance, &b_muonPF2PATImpactTransSignificance);
   fChain->SetBranchAddress("muonPF2PATImpact3DDist", muonPF2PATImpact3DDist, &b_muonPF2PATImpact3DDist);
   fChain->SetBranchAddress("muonPF2PATImpact3DError", muonPF2PATImpact3DError, &b_muonPF2PATImpact3DError);
   fChain->SetBranchAddress("muonPF2PATImpact3DSignificance", muonPF2PATImpact3DSignificance, &b_muonPF2PATImpact3DSignificance);
   if (isMC)
   {
       fChain->SetBranchAddress("genMuonPF2PATPT", genMuonPF2PATPT, &b_genMuonPF2PATPT);
       fChain->SetBranchAddress("genMuonPF2PATET", genMuonPF2PATET, &b_genMuonPF2PATET);
       fChain->SetBranchAddress("genMuonPF2PATPX", genMuonPF2PATPX, &b_genMuonPF2PATPX);
       fChain->SetBranchAddress("genMuonPF2PATPY", genMuonPF2PATPY, &b_genMuonPF2PATPY);
       fChain->SetBranchAddress("genMuonPF2PATPZ", genMuonPF2PATPZ, &b_genMuonPF2PATPZ);
       fChain->SetBranchAddress("genMuonPF2PATPhi", genMuonPF2PATPhi, &b_genMuonPF2PATPhi);
       fChain->SetBranchAddress("genMuonPF2PATTheta", genMuonPF2PATTheta, &b_genMuonPF2PATTheta);
       fChain->SetBranchAddress("genMuonPF2PATEta", genMuonPF2PATEta, &b_genMuonPF2PATEta);
       fChain->SetBranchAddress("genMuonPF2PATCharge", genMuonPF2PATCharge, &b_genMuonPF2PATCharge);
       fChain->SetBranchAddress("genMuonPF2PATPdgId", genMuonPF2PATPdgId, &b_genMuonPF2PATPdgId);
       fChain->SetBranchAddress("genMuonPF2PATMotherId", genMuonPF2PATMotherId, &b_genMuonPF2PATMotherId);
       fChain->SetBranchAddress("genMuonPF2PATPromptDecayed", genMuonPF2PATPromptDecayed, &b_genMuonPF2PATPromptDecayed);
       fChain->SetBranchAddress("genMuonPF2PATPromptFinalState", genMuonPF2PATPromptFinalState, &b_genMuonPF2PATPromptFinalState);
       fChain->SetBranchAddress("genMuonPF2PATHardProcess", genMuonPF2PATHardProcess, &b_genMuonPF2PATHardProcess);
       fChain->SetBranchAddress("genMuonPF2PATPythiaSixStatusThree", genMuonPF2PATPythiaSixStatusThree, &b_genMuonPF2PATPythiaSixStatusThree);
   }

   if (isMC)
   {
       fChain->SetBranchAddress("isElePlusJets", &isElePlusJets, &b_isElePlusJets);
       fChain->SetBranchAddress("genPDFScale", &genPDFScale, &b_genPDFScale);
       fChain->SetBranchAddress("genPDFx1", &genPDFx1, &b_genPDFx1);
       fChain->SetBranchAddress("genPDFx2", &genPDFx2, &b_genPDFx2);
       fChain->SetBranchAddress("genPDFf1", &genPDFf1, &b_genPDFf1);
       fChain->SetBranchAddress("genPDFf2", &genPDFf2, &b_genPDFf2);
       fChain->SetBranchAddress("topPtReweight", &topPtReweight, &b_topPtReweight);
   }
   fChain->SetBranchAddress("processId", &processId, &b_processId);
   fChain->SetBranchAddress("processPtHat", &processPtHat, &b_processPtHat);
   fChain->SetBranchAddress("processMCWeight", &processMCWeight, &b_processMCWeight);
   fChain->SetBranchAddress("beamSpotX", &beamSpotX, &b_beamSpotX);
   fChain->SetBranchAddress("beamSpotY", &beamSpotY, &b_beamSpotY);
   fChain->SetBranchAddress("beamSpotZ", &beamSpotZ, &b_beamSpotZ);
   fChain->SetBranchAddress("mhtPt", &mhtPt, &b_mhtPt);
   fChain->SetBranchAddress("mhtPy", &mhtPy, &b_mhtPy);
   fChain->SetBranchAddress("mhtPx", &mhtPx, &b_mhtPx);
   fChain->SetBranchAddress("mhtPhi", &mhtPhi, &b_mhtPhi);
   fChain->SetBranchAddress("mhtSumEt", &mhtSumEt, &b_mhtSumEt);
   fChain->SetBranchAddress("mhtSignif", &mhtSignif, &b_mhtSignif);
   fChain->SetBranchAddress("nTriggerBits", &nTriggerBits, &b_nTriggerBits);
   fChain->SetBranchAddress("TriggerBits", &TriggerBits, &b_TriggerBits);
   if (isMC){
       fChain->SetBranchAddress("weight_muF0p5", &weight_muF0p5, &b_weight_muF0p5);
       fChain->SetBranchAddress("weight_muF2", &weight_muF2, &b_weight_muF2);
       fChain->SetBranchAddress("weight_muR0p5", &weight_muR0p5, &b_weight_muR0p5);
       fChain->SetBranchAddress("weight_muR2", &weight_muR2, &b_weight_muR2);
       fChain->SetBranchAddress("weight_muF0p5muR0p5", &weight_muF0p5muR0p5, &b_weight_muF0p5muR0p5);
       fChain->SetBranchAddress("weight_muF2muR2", &weight_muF2muR2, &b_weight_muF2muR2);
       fChain->SetBranchAddress("origWeightForNorm", &origWeightForNorm, &b_origWeightForNorm);
       fChain->SetBranchAddress("weight_pdfMax", &weight_pdfMax, &b_weight_pdfMax);
       fChain->SetBranchAddress("weight_pdfMin", &weight_pdfMin, &b_weight_pdfMin);
       fChain->SetBranchAddress("weight_alphaMax", &weight_alphaMax, &b_weight_alphaMax);
       fChain->SetBranchAddress("weight_alphaMin", &weight_alphaMin, &b_weight_alphaMin);
       if (!is2016) {
           fChain->SetBranchAddress("isrRedHi", &isrRedHi, &b_isrRedHi);
           fChain->SetBranchAddress("fsrRedHi", &fsrRedHi, &b_fsrRedHi);
           fChain->SetBranchAddress("isrRedLo", &isrRedLo, &b_isrRedLo);
           fChain->SetBranchAddress("fsrRedLo", &fsrRedLo, &b_fsrRedLo);
           fChain->SetBranchAddress("isrDefHi", &isrDefHi, &b_isrDefHi);
           fChain->SetBranchAddress("fsrDefHi", &fsrDefHi, &b_fsrDefHi);
           fChain->SetBranchAddress("isrDefLo", &isrDefLo, &b_isrDefLo);
           fChain->SetBranchAddress("fsrDefLo", &fsrDefLo, &b_fsrDefLo);
           fChain->SetBranchAddress("isrConHi", &isrConHi, &b_isrConHi);
           fChain->SetBranchAddress("fsrConHi", &fsrConHi, &b_fsrConHi);
           fChain->SetBranchAddress("isrConLo", &isrConLo, &b_isrConLo);
           fChain->SetBranchAddress("fsrConLo", &fsrConLo, &b_fsrConLo);
       }
   }
   //Trigger branches
   if (is2016) {
       fChain->SetBranchAddress("HLT_IsoMu24_v1", &HLT_IsoMu24_v1, &b_HLT_IsoMu24_v1);
       fChain->SetBranchAddress("HLT_IsoMu24_v2", &HLT_IsoMu24_v2, &b_HLT_IsoMu24_v2);
       fChain->SetBranchAddress("HLT_IsoMu24_v4", &HLT_IsoMu24_v4, &b_HLT_IsoMu24_v4);
       fChain->SetBranchAddress("HLT_IsoTkMu24_v1", &HLT_IsoTkMu24_v1, &b_HLT_IsoTkMu24_v1);
       fChain->SetBranchAddress("HLT_IsoTkMu24_v2", &HLT_IsoTkMu24_v2, &b_HLT_IsoTkMu24_v2);
       fChain->SetBranchAddress("HLT_IsoTkMu24_v3", &HLT_IsoTkMu24_v3, &b_HLT_IsoTkMu24_v3);
       fChain->SetBranchAddress("HLT_IsoTkMu24_v4", &HLT_IsoTkMu24_v4, &b_HLT_IsoTkMu24_v4);
       fChain->SetBranchAddress("HLT_IsoMu27_v3", &HLT_IsoMu27_v3, &b_HLT_IsoMu27_v3);
       fChain->SetBranchAddress("HLT_IsoMu27_v4", &HLT_IsoMu27_v4, &b_HLT_IsoMu27_v4);
       fChain->SetBranchAddress("HLT_IsoMu27_v5", &HLT_IsoMu27_v5, &b_HLT_IsoMu27_v5);
       fChain->SetBranchAddress("HLT_IsoMu27_v7", &HLT_IsoMu27_v7, &b_HLT_IsoMu27_v7);
       fChain->SetBranchAddress("HLT_IsoTkMu27_v3", &HLT_IsoTkMu27_v3, &b_HLT_IsoTkMu27_v3);
       fChain->SetBranchAddress("HLT_IsoTkMu27_v4", &HLT_IsoTkMu27_v4, &b_HLT_IsoTkMu27_v4);
       fChain->SetBranchAddress("HLT_IsoTkMu27_v5", &HLT_IsoTkMu27_v5, &b_HLT_IsoTkMu27_v5);
       fChain->SetBranchAddress("HLT_IsoTkMu27_v6", &HLT_IsoTkMu27_v6, &b_HLT_IsoTkMu27_v6);
       fChain->SetBranchAddress("HLT_IsoTkMu27_v7", &HLT_IsoTkMu27_v7, &b_HLT_IsoTkMu27_v7);

       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2", &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2, &b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2", &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2, &b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3", &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3, &b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6", &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6, &b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6);

       fChain->SetBranchAddress("HLT_DoubleMu33NoFiltersNoVtx_v2", &HLT_DoubleMu33NoFiltersNoVtx_v2, &b_HLT_DoubleMu33NoFiltersNoVtx_v2);
       fChain->SetBranchAddress("HLT_DoubleMu33NoFiltersNoVtx_v3", &HLT_DoubleMu33NoFiltersNoVtx_v3, &b_HLT_DoubleMu33NoFiltersNoVtx_v3);
       fChain->SetBranchAddress("HLT_DoubleMu33NoFiltersNoVtx_v5", &HLT_DoubleMu33NoFiltersNoVtx_v5, &b_HLT_DoubleMu33NoFiltersNoVtx_v5);
       fChain->SetBranchAddress("HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2", &HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2, &b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2);
       fChain->SetBranchAddress("HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3", &HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3, &b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3);
       fChain->SetBranchAddress("HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5", &HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5, &b_HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5);
       fChain->SetBranchAddress("HLT_L2DoubleMu23_NoVertex_v2", &HLT_L2DoubleMu23_NoVertex_v2, &b_HLT_L2DoubleMu23_NoVertex_v2);
       fChain->SetBranchAddress("HLT_L2DoubleMu23_NoVertex_v3", &HLT_L2DoubleMu23_NoVertex_v3, &b_HLT_L2DoubleMu23_NoVertex_v3);
       fChain->SetBranchAddress("HLT_L2DoubleMu23_NoVertex_v4", &HLT_L2DoubleMu23_NoVertex_v4, &b_HLT_L2DoubleMu23_NoVertex_v4);
       fChain->SetBranchAddress("HLT_L2DoubleMu23_NoVertex_v6", &HLT_L2DoubleMu23_NoVertex_v6, &b_HLT_L2DoubleMu23_NoVertex_v6);
   }
   else if (is2018) {
       fChain->SetBranchAddress("HLT_IsoMu24_v11", &HLT_IsoMu24_v11, &b_HLT_IsoMu24_v13);
       fChain->SetBranchAddress("HLT_IsoMu24_v12", &HLT_IsoMu24_v12, &b_HLT_IsoMu24_v13);
       fChain->SetBranchAddress("HLT_IsoMu24_v13", &HLT_IsoMu24_v13, &b_HLT_IsoMu24_v13);
       fChain->SetBranchAddress("HLT_IsoMu27_v14", &HLT_IsoMu27_v14, &b_HLT_IsoMu27_v14);
       fChain->SetBranchAddress("HLT_IsoMu27_v15", &HLT_IsoMu27_v15, &b_HLT_IsoMu27_v15);
       fChain->SetBranchAddress("HLT_IsoMu27_v16", &HLT_IsoMu27_v16, &b_HLT_IsoMu27_v16);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5);

       fChain->SetBranchAddress("HLT_DoubleL2Mu50_v2", &HLT_DoubleL2Mu50_v2, &b_HLT_DoubleL2Mu50_v2);
       fChain->SetBranchAddress("HLT_DoubleL2Mu23NoVtx_2Cha_v1", &HLT_DoubleL2Mu23NoVtx_2Cha_v1, &b_HLT_DoubleL2Mu23NoVtx_2Cha_v1);
       fChain->SetBranchAddress("HLT_DoubleL2Mu23NoVtx_2Cha_v2", &HLT_DoubleL2Mu23NoVtx_2Cha_v2, &b_HLT_DoubleL2Mu23NoVtx_2Cha_v2);
       fChain->SetBranchAddress("HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1", &HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1, &b_HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1);
       fChain->SetBranchAddress("HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2", &HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2, &b_HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2);
       fChain->SetBranchAddress("HLT_DoubleL2Mu25NoVtx_2Cha_v1", &HLT_DoubleL2Mu25NoVtx_2Cha_v1, &b_HLT_DoubleL2Mu25NoVtx_2Cha_v1);
       fChain->SetBranchAddress("HLT_DoubleL2Mu25NoVtx_2Cha_v2", &HLT_DoubleL2Mu25NoVtx_2Cha_v2, &b_HLT_DoubleL2Mu25NoVtx_2Cha_v2);
       fChain->SetBranchAddress("HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1", &HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1, &b_HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1);
       fChain->SetBranchAddress("HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2", &HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2, &b_HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2);
       fChain->SetBranchAddress("HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1", &HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1, &b_HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1);
       fChain->SetBranchAddress("HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1", &HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1, &b_HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1);
       fChain->SetBranchAddress("HLT_DoubleMu43NoFiltersNoVtx_v3", &HLT_DoubleMu43NoFiltersNoVtx_v3, &b_HLT_DoubleMu43NoFiltersNoVtx_v3);
       fChain->SetBranchAddress("HLT_DoubleMu43NoFiltersNoVtx_v4", &HLT_DoubleMu43NoFiltersNoVtx_v4, &b_HLT_DoubleMu43NoFiltersNoVtx_v4);
       fChain->SetBranchAddress("HLT_DoubleMu48NoFiltersNoVtx_v3", &HLT_DoubleMu48NoFiltersNoVtx_v3, &b_HLT_DoubleMu48NoFiltersNoVtx_v3);
       fChain->SetBranchAddress("HLT_DoubleMu48NoFiltersNoVtx_v4", &HLT_DoubleMu48NoFiltersNoVtx_v4, &b_HLT_DoubleMu48NoFiltersNoVtx_v4);
   }
   else {
       fChain->SetBranchAddress("HLT_IsoMu27_v8", &HLT_IsoMu27_v8, &b_HLT_IsoMu27_v8);
       fChain->SetBranchAddress("HLT_IsoMu27_v9", &HLT_IsoMu27_v9, &b_HLT_IsoMu27_v9);
       fChain->SetBranchAddress("HLT_IsoMu27_v10", &HLT_IsoMu27_v10, &b_HLT_IsoMu27_v10);
       fChain->SetBranchAddress("HLT_IsoMu27_v11", &HLT_IsoMu27_v11, &b_HLT_IsoMu27_v11);
       fChain->SetBranchAddress("HLT_IsoMu27_v12", &HLT_IsoMu27_v12, &b_HLT_IsoMu27_v12);
       fChain->SetBranchAddress("HLT_IsoMu27_v13", &HLT_IsoMu27_v13, &b_HLT_IsoMu27_v13);
       fChain->SetBranchAddress("HLT_IsoMu27_v14", &HLT_IsoMu27_v14, &b_HLT_IsoMu27_v14);

       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3);
       fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4);

       fChain->SetBranchAddress("HLT_DoubleMu43NoFiltersNoVtx_v2", &HLT_DoubleMu43NoFiltersNoVtx_v2, &b_HLT_DoubleMu43NoFiltersNoVtx_v2);
       fChain->SetBranchAddress("HLT_DoubleMu43NoFiltersNoVtx_v3", &HLT_DoubleMu43NoFiltersNoVtx_v3, &b_HLT_DoubleMu43NoFiltersNoVtx_v3);
       fChain->SetBranchAddress("HLT_DoubleMu48NoFiltersNoVtx_v2", &HLT_DoubleMu48NoFiltersNoVtx_v2, &b_HLT_DoubleMu48NoFiltersNoVtx_v2);
       fChain->SetBranchAddress("HLT_DoubleMu48NoFiltersNoVtx_v3", &HLT_DoubleMu48NoFiltersNoVtx_v3, &b_HLT_DoubleMu48NoFiltersNoVtx_v3);
       fChain->SetBranchAddress("HLT_DoubleL2Mu50_v1", &HLT_DoubleL2Mu50_v1, &b_HLT_DoubleL2Mu50_v1);
       fChain->SetBranchAddress("HLT_DoubleL2Mu50_v2", &HLT_DoubleL2Mu50_v2, &b_HLT_DoubleL2Mu50_v2);
   }


   // bParking branches
   if (is2018) {
       fChain->SetBranchAddress("HLT_Mu12_IP6_part0_v2", &HLT_Mu12_IP6_part0_v2, &b_HLT_Mu12_IP6_part0_v2);
       fChain->SetBranchAddress("HLT_Mu12_IP6_part1_v2", &HLT_Mu12_IP6_part1_v2, &b_HLT_Mu12_IP6_part1_v2);
       fChain->SetBranchAddress("HLT_Mu12_IP6_part2_v2", &HLT_Mu12_IP6_part2_v2, &b_HLT_Mu12_IP6_part2_v2);
       fChain->SetBranchAddress("HLT_Mu12_IP6_part3_v2", &HLT_Mu12_IP6_part3_v2, &b_HLT_Mu12_IP6_part3_v2);
       fChain->SetBranchAddress("HLT_Mu12_IP6_part4_v2", &HLT_Mu12_IP6_part4_v2, &b_HLT_Mu12_IP6_part4_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP5_part0_v2", &HLT_Mu9_IP5_part0_v2, &b_HLT_Mu9_IP5_part0_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP5_part1_v2", &HLT_Mu9_IP5_part1_v2, &b_HLT_Mu9_IP5_part1_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP5_part2_v2", &HLT_Mu9_IP5_part2_v2, &b_HLT_Mu9_IP5_part2_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP5_part3_v2", &HLT_Mu9_IP5_part3_v2, &b_HLT_Mu9_IP5_part3_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP5_part4_v2", &HLT_Mu9_IP5_part4_v2, &b_HLT_Mu9_IP5_part4_v2);
       fChain->SetBranchAddress("HLT_Mu7_IP4_part0_v2", &HLT_Mu7_IP4_part0_v2, &b_HLT_Mu7_IP4_part0_v2);
       fChain->SetBranchAddress("HLT_Mu7_IP4_part1_v2", &HLT_Mu7_IP4_part1_v2, &b_HLT_Mu7_IP4_part1_v2);
       fChain->SetBranchAddress("HLT_Mu7_IP4_part2_v2", &HLT_Mu7_IP4_part2_v2, &b_HLT_Mu7_IP4_part2_v2);
       fChain->SetBranchAddress("HLT_Mu7_IP4_part3_v2", &HLT_Mu7_IP4_part3_v2, &b_HLT_Mu7_IP4_part3_v2);
       fChain->SetBranchAddress("HLT_Mu7_IP4_part4_v2", &HLT_Mu7_IP4_part4_v2, &b_HLT_Mu7_IP4_part4_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP5_part0_v2", &HLT_Mu8_IP5_part0_v2, &b_HLT_Mu8_IP5_part0_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP5_part1_v2", &HLT_Mu8_IP5_part1_v2, &b_HLT_Mu8_IP5_part1_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP5_part2_v2", &HLT_Mu8_IP5_part2_v2, &b_HLT_Mu8_IP5_part2_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP5_part3_v2", &HLT_Mu8_IP5_part3_v2, &b_HLT_Mu8_IP5_part3_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP5_part4_v2", &HLT_Mu8_IP5_part4_v2, &b_HLT_Mu8_IP5_part4_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP6_part0_v2", &HLT_Mu8_IP6_part0_v2, &b_HLT_Mu8_IP6_part0_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP6_part1_v2", &HLT_Mu8_IP6_part1_v2, &b_HLT_Mu8_IP6_part1_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP6_part2_v2", &HLT_Mu8_IP6_part2_v2, &b_HLT_Mu8_IP6_part2_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP6_part3_v2", &HLT_Mu8_IP6_part3_v2, &b_HLT_Mu8_IP6_part3_v2);
       fChain->SetBranchAddress("HLT_Mu8_IP6_part4_v2", &HLT_Mu8_IP6_part4_v2, &b_HLT_Mu8_IP6_part4_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part0_v1", &HLT_Mu9_IP6_part0_v1, &b_HLT_Mu9_IP6_part0_v1);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part1_v1", &HLT_Mu9_IP6_part1_v1, &b_HLT_Mu9_IP6_part1_v1);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part2_v1", &HLT_Mu9_IP6_part2_v1, &b_HLT_Mu9_IP6_part2_v1);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part3_v1", &HLT_Mu9_IP6_part3_v1, &b_HLT_Mu9_IP6_part3_v1);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part4_v1", &HLT_Mu9_IP6_part4_v1, &b_HLT_Mu9_IP6_part4_v1);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part0_v2", &HLT_Mu9_IP6_part0_v2, &b_HLT_Mu9_IP6_part0_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part1_v2", &HLT_Mu9_IP6_part1_v2, &b_HLT_Mu9_IP6_part1_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part2_v2", &HLT_Mu9_IP6_part2_v2, &b_HLT_Mu9_IP6_part2_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part3_v2", &HLT_Mu9_IP6_part3_v2, &b_HLT_Mu9_IP6_part3_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part4_v2", &HLT_Mu9_IP6_part4_v2, &b_HLT_Mu9_IP6_part4_v2);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part0_v3", &HLT_Mu9_IP6_part0_v3, &b_HLT_Mu9_IP6_part0_v3);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part1_v3", &HLT_Mu9_IP6_part1_v3, &b_HLT_Mu9_IP6_part1_v3);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part2_v3", &HLT_Mu9_IP6_part2_v3, &b_HLT_Mu9_IP6_part2_v3);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part3_v3", &HLT_Mu9_IP6_part3_v3, &b_HLT_Mu9_IP6_part3_v3);
       fChain->SetBranchAddress("HLT_Mu9_IP6_part4_v3", &HLT_Mu9_IP6_part4_v3, &b_HLT_Mu9_IP6_part4_v3);
   }
/*
   // DST branches
   if ( !is2016 ) {
       fChain->SetBranchAddress("DST_HT250_CaloScouting_v6", &DST_HT250_CaloScouting_v6, &b_DST_HT250_CaloScouting_v6);
       fChain->SetBranchAddress("DST_HT250_CaloScouting_v7", &DST_HT250_CaloScouting_v7, &b_DST_HT250_CaloScouting_v7);
       fChain->SetBranchAddress("DST_HT250_CaloScouting_v8", &DST_HT250_CaloScouting_v8, &b_DST_HT250_CaloScouting_v8);
       fChain->SetBranchAddress("DST_HT250_CaloScouting_v9", &DST_HT250_CaloScouting_v9, &b_DST_HT250_CaloScouting_v9);
       fChain->SetBranchAddress("DST_HT250_CaloBTagScouting_v5", &DST_HT250_CaloBTagScouting_v5, &b_DST_HT250_CaloBTagScouting_v5);
       fChain->SetBranchAddress("DST_HT250_CaloBTagScouting_v6", &DST_HT250_CaloBTagScouting_v6, &b_DST_HT250_CaloBTagScouting_v6);
       fChain->SetBranchAddress("DST_HT250_CaloBTagScouting_v7", &DST_HT250_CaloBTagScouting_v7, &b_DST_HT250_CaloBTagScouting_v7);
       fChain->SetBranchAddress("DST_HT250_CaloBTagScouting_v8", &DST_HT250_CaloBTagScouting_v8, &b_DST_HT250_CaloBTagScouting_v8);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v9", &DST_HT410_PFScouting_v9, &b_DST_HT410_PFScouting_v9);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v10", &DST_HT410_PFScouting_v10, &b_DST_HT410_PFScouting_v10);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v11", &DST_HT410_PFScouting_v11, &b_DST_HT410_PFScouting_v11);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v12", &DST_HT410_PFScouting_v12, &b_DST_HT410_PFScouting_v12);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v13", &DST_HT410_PFScouting_v13, &b_DST_HT410_PFScouting_v13);
       fChain->SetBranchAddress("DST_HT410_PFScouting_v14", &DST_HT410_PFScouting_v14, &b_DST_HT410_PFScouting_v14);
       fChain->SetBranchAddress("DST_HT410_BTagScouting_v10", &DST_HT410_BTagScouting_v10, &b_DST_HT410_BTagScouting_v10);
       fChain->SetBranchAddress("DST_HT410_BTagScouting_v11", &DST_HT410_BTagScouting_v11, &b_DST_HT410_BTagScouting_v11);
       fChain->SetBranchAddress("DST_HT410_BTagScouting_v12", &DST_HT410_BTagScouting_v12, &b_DST_HT410_BTagScouting_v12);
       fChain->SetBranchAddress("DST_HT410_BTagScouting_v13", &DST_HT410_BTagScouting_v13, &b_DST_HT410_BTagScouting_v13);
       fChain->SetBranchAddress("DST_HT410_BTagScouting_v14", &DST_HT410_BTagScouting_v14, &b_DST_HT410_BTagScouting_v14);
       fChain->SetBranchAddress("DST_DoubleMu3_noVtx_CaloScouting_v2", &DST_DoubleMu3_noVtx_CaloScouting_v2, &b_DST_DoubleMu3_noVtx_CaloScouting_v2);
       fChain->SetBranchAddress("DST_DoubleMu3_noVtx_CaloScouting_v3", &DST_DoubleMu3_noVtx_CaloScouting_v3, &b_DST_DoubleMu3_noVtx_CaloScouting_v3);
       fChain->SetBranchAddress("DST_DoubleMu3_noVtx_CaloScouting_v4", &DST_DoubleMu3_noVtx_CaloScouting_v4, &b_DST_DoubleMu3_noVtx_CaloScouting_v4);
       fChain->SetBranchAddress("DST_DoubleMu3_noVtx_CaloScouting_v5", &DST_DoubleMu3_noVtx_CaloScouting_v5, &b_DST_DoubleMu3_noVtx_CaloScouting_v5);
   }
*/
   //MET filter branches
   fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
   fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter, &b_Flag_globalTightHalo2016Filter);
   fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, &b_Flag_HBHENoiseFilter);
   fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter, &b_Flag_HBHENoiseIsoFilter);
   fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, &b_Flag_BadPFMuonFilter);
   fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, &b_Flag_BadChargedCandidateFilter);
   fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
   fChain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter, &b_Flag_ecalBadCalibFilter);

   if (isMC) {
       fChain->SetBranchAddress("nGenPar", &nGenPar, &b_nGenPar);
       fChain->SetBranchAddress("genParEta", genParEta, &b_genParEta);
       fChain->SetBranchAddress("genParPhi", genParPhi, &b_genParPhi);
       fChain->SetBranchAddress("genParE", genParE, &b_genParE);
       fChain->SetBranchAddress("genParPt", genParPt, &b_genParPt);
       fChain->SetBranchAddress("genParId", genParId, &b_genParId);
       fChain->SetBranchAddress("genParVx", genParVx, &b_genParVx);
       fChain->SetBranchAddress("genParVy", genParVy, &b_genParVy);
       fChain->SetBranchAddress("genParVz", genParVz, &b_genParVz);
       fChain->SetBranchAddress("genParMotherId", genParMotherId, &b_genParMotherId);
       fChain->SetBranchAddress("genParMotherIndex", genParMotherIndex, &b_genParMotherIndex);
       fChain->SetBranchAddress("genParNumDaughters", genParNumDaughters, &b_genParNumDaughters);
       fChain->SetBranchAddress("genParDaughterId1", genParDaughterId1, &b_genParDaughterId1);
       fChain->SetBranchAddress("genParDaughterId2", genParDaughterId2, &b_genParDaughterId2);
       fChain->SetBranchAddress("genParDaughter1Index", genParDaughter1Index, &b_genParDaughter1Index);
       fChain->SetBranchAddress("genParDaughter2Index", genParDaughter2Index, &b_genParDaughter2Index);
       fChain->SetBranchAddress("genParStatus", genParStatus, &b_genParStatus);
       fChain->SetBranchAddress("genParCharge", genParCharge, &b_genParCharge);
   }
   fChain->SetBranchAddress("eventRun", &eventRun, &b_eventRun);
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   fChain->SetBranchAddress("eventLumiblock", &eventLumiblock, &b_eventLumiblock);
   fChain->SetBranchAddress("numVert", &numVert, &b_numVert);

  muonMomentumSF = {};
  jetSmearValue = {};

  electronIndexTight = {};
  electronIndexLoose = {};
  muonIndexTight = {};
  muonIndexLoose = {};
  photonIndexTight = {};
  photonIndexLoose = {};
  jetIndex = {};
  displacedJetIndex = {};
  chsIndex = {};
  totalJetHt = -1.0;
  selectedJetIncHt = -1.0;
  selectedJetTracksHt = -1.0;
  bTagIndex = {};

  std::pair<TLorentzVector, TLorentzVector> zPairLeptons = {};
  std::pair<TLorentzVector, TLorentzVector> zPairLeptonsRefitted = {};
  std::pair<float, float> zPairRelIso = {};
  std::pair<float, float> zPairNewRelIso = {};
  std::pair<float, float> zPairNewTrkIso = {};
  std::pair<int, int> zPairIndex = {};
  mumuTrkIndex = -1;
  zRelIso = -1.;
  zTrkIso = -1.;

  std::pair<TLorentzVector, TLorentzVector> chsPairVec = {};
  std::pair<TLorentzVector, TLorentzVector> chsPairVecRefitted = {};
  std::pair<float, float> chsPairRelIso = {};
  std::pair<float, float> chsPairTrkIso = {};
  std::pair<int, int> chsPairIndex = {};
  chsPairTrkIndex = -1;
  chsRelIso = -1;
  chsTrkIso = -1;

  std::pair<TLorentzVector, TLorentzVector> wPairQuarks = {};
  std::pair<int, int> wPairIndex = {};

  wLepton = {};
  wLepIndex = -1;
  wLeptonRelIso = -1.;

}
// clang-format on

inline MiniMuonAnalysisEvent::~MiniMuonAnalysisEvent() {
    if (!fChain) {
        return;
    }
    delete fChain->GetCurrentFile();
}

inline Int_t MiniMuonAnalysisEvent::GetEntry(const Long64_t entry) {
    // Read contents of entry.
    if (!fChain) {
        return 0;
    }
    return fChain->GetEntry(entry);
}

inline Long64_t MiniMuonAnalysisEvent::LoadTree(const Long64_t entry) {
    // Set the environment to read one entry
    if (!fChain) {
        return -5;
    }
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) {
        return centry;
    }
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
    }
    return centry;
}

inline void MiniMuonAnalysisEvent::Show(const Long64_t entry) const {
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) {
        return;
    }
    fChain->Show(entry);
}

inline void MiniMuonAnalysisEvent::Loop() {
    if (fChain == nullptr)
    {
        return;
    }

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0;
    Long64_t nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
        {
            break;
        }
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
    }
}

inline bool MiniMuonAnalysisEvent::muTrig() const {
    if (is2016_) { // 2016
        if ( HLT_IsoMu24_v1 > 0 || HLT_IsoMu24_v2 > 0 || HLT_IsoMu24_v4 > 0 || HLT_IsoTkMu24_v1 > 0 || HLT_IsoTkMu24_v2 > 0 || HLT_IsoTkMu24_v3 > 0 || HLT_IsoTkMu24_v4 > 0 
          || HLT_IsoMu27_v3 > 0 || HLT_IsoMu27_v4 > 0 || HLT_IsoMu27_v5 > 0 || HLT_IsoMu27_v7 > 0 
          || HLT_IsoTkMu27_v3 > 0 || HLT_IsoTkMu27_v4 > 0 || HLT_IsoTkMu27_v5 > 0 || HLT_IsoTkMu27_v6 > 0 || HLT_IsoTkMu27_v7 > 0 ) return true;
        else return false;
    }
    else if (is2018_) { // 2018 
       if ( HLT_IsoMu24_v11 > 0 || HLT_IsoMu24_v12 > 0 || HLT_IsoMu24_v13  
         || HLT_IsoMu27_v14 > 0 || HLT_IsoMu27_v15 > 0 || HLT_IsoMu27_v16 > 0 ) return true;
        else return false;
    }
    else { // 2017
        if ( HLT_IsoMu27_v8 > 0 || HLT_IsoMu27_v9 > 0 || HLT_IsoMu27_v10 > 0 || HLT_IsoMu27_v11 > 0 || HLT_IsoMu27_v12 > 0 || HLT_IsoMu27_v13 > 0 || HLT_IsoMu27_v14 > 0 ) return true;
        else return false;
    }
}

inline bool MiniMuonAnalysisEvent::mumuTrig() const {

    if (is2016_) { // 2016
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7 > 0
          || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2 > 0 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3 > 0 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6 > 0 ) return true;
        else return false;
    }
    else if (is2018_) { // 2018
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5 > 0 ) return true;
        else return false;
    }
    else { // 2017
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11 > 0
          || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14 > 0 
          || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4 > 0
          || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1 > 0 
          || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4 > 0 ) return true;
        else return false;
   }
}

inline bool MiniMuonAnalysisEvent::mumuTrig_noMassCut() const {
    if (is2016_) { // 2016
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7 > 0
          || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2 > 0 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3 > 0 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6 > 0 ) return true;
        else return false;
    }
    else if (is2018_) { // 2018
        return false;
    }
    else { // 2017
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8 > 0	|| HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10 > 0 ||	HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11 > 0   
      	   || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14 > 0 ) return true;
        else return false;
   }
}

inline bool MiniMuonAnalysisEvent::mumuTrig_massCut() const {
    if (is2016_) return false; // 2016
    else if (is2018_) { // 2018
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v5 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5 > 0 ) return true;
        return false;
    }
    else { // 2017
        if ( HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4 > 0
       	  || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1 > 0	
          || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2 > 0	|| HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3 > 0 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4 > 0 ) return true;
        else return false;
   }
}

inline bool MiniMuonAnalysisEvent::mumuL2Trig() const {
    if (is2016_) { // 2016
        if ( HLT_L2DoubleMu23_NoVertex_v2 > 0 || HLT_L2DoubleMu23_NoVertex_v3 > 0 || HLT_L2DoubleMu23_NoVertex_v4 > 0 || HLT_L2DoubleMu23_NoVertex_v6 > 0 ) return true;
        else return false;
    }
    else if (is2018_) { // 2018
        if ( HLT_DoubleL2Mu50_v2 > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_v1 > 0 || HLT_DoubleL2Mu23NoVtx_2Cha_v2 > 0 || HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v1 > 0 || HLT_DoubleL2Mu25NoVtx_2Cha_Eta2p4_v2 > 0
          || HLT_DoubleL2Mu25NoVtx_2Cha_v1 > 0 || HLT_DoubleL2Mu25NoVtx_2Cha_v2 > 0 || HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v1 > 0 || HLT_DoubleL2Mu30NoVtx_2Cha_Eta2p4_v2 > 0 ) return true;
        else return false;
    }
    else { // 2017
        if ( HLT_DoubleL2Mu50_v1 > 0 || HLT_DoubleL2Mu50_v2 > 0 ) return true;
        else return false;
    }
}

inline bool MiniMuonAnalysisEvent::mumuNoVtxTrig() const {
    if (is2016_) { // 2016
        if ( HLT_DoubleMu33NoFiltersNoVtx_v2 > 0 || HLT_DoubleMu33NoFiltersNoVtx_v3 > 0 || HLT_DoubleMu33NoFiltersNoVtx_v5 > 0 
          || HLT_DoubleMu23NoFiltersNoVtxDisplaced_v2 > 0 || HLT_DoubleMu23NoFiltersNoVtxDisplaced_v3 > 0 || HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5 ) return true;
       	else return false;
    }
    else if (is2018_) { // 2018
        if ( HLT_DoubleMu33NoFiltersNoVtxDisplaced_v1 > 0 || HLT_DoubleMu40NoFiltersNoVtxDisplaced_v1 > 0 || HLT_DoubleMu43NoFiltersNoVtx_v3 > 0 
          || HLT_DoubleMu43NoFiltersNoVtx_v4 > 0 || HLT_DoubleMu48NoFiltersNoVtx_v3 > 0 || HLT_DoubleMu48NoFiltersNoVtx_v4 > 0 ) return true;
       	else return false;
    }
    else { // 2017
        if ( HLT_DoubleMu43NoFiltersNoVtx_v2 > 0 || HLT_DoubleMu43NoFiltersNoVtx_v3 > 0 || HLT_DoubleMu48NoFiltersNoVtx_v2 > 0 || HLT_DoubleMu48NoFiltersNoVtx_v3 > 0 ) return true;
        else return false;
    }
}


inline bool MiniMuonAnalysisEvent::bParkingTrigInclusive() const {
    if ( !is2018_ ) return false;
    else {
        if ( HLT_Mu12_IP6_part0_v2 > 0 || HLT_Mu12_IP6_part1_v2 > 0 || HLT_Mu12_IP6_part2_v2 > 0 || HLT_Mu12_IP6_part3_v2 > 0 || HLT_Mu12_IP6_part4_v2 > 0
          || HLT_Mu9_IP5_part0_v2 > 0 || HLT_Mu9_IP5_part1_v2 > 0 || HLT_Mu9_IP5_part2_v2 > 0 || HLT_Mu9_IP5_part3_v2 > 0 || HLT_Mu9_IP5_part4_v2 > 0
          || HLT_Mu7_IP4_part0_v2 > 0 || HLT_Mu7_IP4_part1_v2 > 0 || HLT_Mu7_IP4_part2_v2 > 0 || HLT_Mu7_IP4_part3_v2 > 0 || HLT_Mu7_IP4_part4_v2 > 0
          || HLT_Mu8_IP5_part0_v2 > 0 || HLT_Mu8_IP5_part1_v2 > 0 || HLT_Mu8_IP5_part2_v2 > 0 || HLT_Mu8_IP5_part3_v2 > 0 || HLT_Mu8_IP5_part4_v2 > 0
          || HLT_Mu8_IP6_part0_v2 > 0 || HLT_Mu8_IP6_part1_v2 > 0 || HLT_Mu8_IP6_part2_v2 > 0 || HLT_Mu8_IP6_part3_v2 > 0 || HLT_Mu8_IP6_part4_v2 > 0
          || HLT_Mu9_IP6_part0_v3 > 0 || HLT_Mu9_IP6_part1_v3 > 0 || HLT_Mu9_IP6_part2_v3 > 0 || HLT_Mu9_IP6_part3_v3 > 0 || HLT_Mu9_IP6_part4_v3 > 0 ) return true;
        else return false;
    }
}

inline bool MiniMuonAnalysisEvent::bParkingTrig_Mu9_IP5() const {
    if ( !is2018_ ) return false;
    else {
        if ( HLT_Mu9_IP5_part0_v2 > 0 
          || HLT_Mu9_IP5_part1_v2 > 0  
          || HLT_Mu9_IP5_part2_v2 > 0  
          || HLT_Mu9_IP5_part3_v2 > 0  
          || HLT_Mu9_IP5_part4_v2 > 0 ) return true;
        else return false;
    }
}

inline bool MiniMuonAnalysisEvent::bParkingTrig_Mu9_IP6() const {
    if ( !is2018_ ) return false;
    else {
        if ( HLT_Mu9_IP6_part0_v3 > 0 
          || HLT_Mu9_IP6_part1_v3 > 0  
          || HLT_Mu9_IP6_part2_v3 > 0  
          || HLT_Mu9_IP6_part3_v3 > 0  
          || HLT_Mu9_IP6_part4_v3 > 0 ) return true;
        else return false;
    }
}

inline bool MiniMuonAnalysisEvent::bParkingTrig_Mu12_IP6() const {
    if ( !is2018_ ) return false;
    else {
        if ( HLT_Mu12_IP6_part0_v2 > 0 
          || HLT_Mu12_IP6_part1_v2 > 0  
          || HLT_Mu12_IP6_part2_v2 > 0  
          || HLT_Mu12_IP6_part3_v2 > 0  
          || HLT_Mu12_IP6_part4_v2 > 0 ) return true;
        else return false;
    }
}

/*
inline bool MiniMuonAnalysisEvent::dataScoutingTrig() const {
    return is2016_ ?
// 2016 triggers
                   0
//2017 triggers
                   : DST_HT250_CaloScouting_v6 > 0
                         || DST_HT250_CaloScouting_v7 > 0
                         || DST_HT250_CaloScouting_v8 > 0
                         || DST_HT250_CaloScouting_v9 > 0
                         || DST_HT250_CaloBTagScouting_v5 > 0
                         || DST_HT250_CaloBTagScouting_v6 > 0
                         || DST_HT250_CaloBTagScouting_v7 > 0
                         || DST_HT250_CaloBTagScouting_v8 > 0
                         || DST_HT410_PFScouting_v9 > 0
                         || DST_HT410_PFScouting_v10 > 0
                         || DST_HT410_PFScouting_v11 > 0
                         || DST_HT410_PFScouting_v12 > 0
                         || DST_HT410_PFScouting_v13 > 0
                         || DST_HT410_PFScouting_v14 > 0
                         || DST_HT410_BTagScouting_v10 > 0
                         || DST_HT410_BTagScouting_v11 > 0
                         || DST_HT410_BTagScouting_v12 > 0
                         || DST_HT410_BTagScouting_v13 > 0
                         || DST_HT410_BTagScouting_v14 > 0
                         || DST_DoubleMu3_noVtx_CaloScouting_v2 > 0
                         || DST_DoubleMu3_noVtx_CaloScouting_v3 > 0
                         || DST_DoubleMu3_noVtx_CaloScouting_v4 > 0
                         || DST_DoubleMu3_noVtx_CaloScouting_v5 > 0;
}
*/
inline bool MiniMuonAnalysisEvent::metFilters() const {
    return is2016_ ?
// 2016 filters
               Flag_goodVertices > 0 // Recommended 16, 17, 18
            && Flag_globalTightHalo2016Filter > 0 // Recommended 16, 17, 18
            && Flag_HBHENoiseFilter > 0 // Recommended 16, 17, 18
            && Flag_HBHENoiseIsoFilter > 0 // Recommended 16, 17, 18
            && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 // Recommended 16, 17, 18
            && Flag_BadPFMuonFilter > 0 // Recommended 16, 17, 18

//2017 + 2018 filters
                   : 
               Flag_goodVertices > 0 // Recommended 16, 17, 18
            && Flag_globalTightHalo2016Filter > 0 // Recommended 16, 17, 18
            && Flag_HBHENoiseFilter > 0 // Recommended 16, 17, 18
            && Flag_HBHENoiseIsoFilter > 0 // Recommended 16, 17, 18
            && Flag_EcalDeadCellTriggerPrimitiveFilter > 0 // Recommended 16, 17, 18
            && Flag_BadPFMuonFilter > 0 // Recommended 16, 17, 18
            && Flag_ecalBadCalibFilter > 0; // Recommended 17, 18 (no comment on 16)

//            && Flag_BadChargedCandidateFilter > 0 // NOT Recommended
//            && Flag_eeBadScFilter > 0 // NOT Recommended
}

#endif
