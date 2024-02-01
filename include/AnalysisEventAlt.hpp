//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 19 17:17:07 2023 by ROOT version 6.24/02
// from TTree tree/tree
// found on file: test.root
//////////////////////////////////////////////////////////

#ifndef _AnalysisEventAlt_hpp_
#define _AnalysisEventAlt_hpp_

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>

#include <TROOT.h>
#include <iostream>
#include <string>

// Header file for the classes stored in the TTree if any.

class sample_test {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   static constexpr size_t NMUONSMAX{20};
   Int_t           numMuonPF2PAT;
   Float_t         muonPF2PATE[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATET[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPt[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPX[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPY[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPZ[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPhi[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTheta[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATEta[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATCharge[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATLooseCutId[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATMediumCutId[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATTightCutId[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPfIsoVeryLoose[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPfIsoLoose[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPfIsoMedium[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPfIsoTight[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPfIsoVeryTight[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATTkIsoLoose[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATTkIsoTight[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATMvaLoose[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATMvaMedium[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATMvaTight[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATGlobalID[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTrackID[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATChi2[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATD0[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTrackDBD0[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATDBInnerTrackD0[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATBeamSpotCorrectedD0[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATTrackNHits[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATMuonNHits[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATNDOF[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATVertX[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATVertY[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATVertZ[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkPt[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkPx[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkPy[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkPz[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkEta[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkPhi[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATChargedHadronIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATNeutralHadronIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPhotonIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTrackIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATEcalIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATHcalIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATComRelIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATComRelIsodBeta[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATChHadIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATNtHadIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATGammaIso[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATPuIso[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATIsPFMuon[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATNChambers[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATNMatches[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTkLysWithMeasurements[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATGlbTkNormChi2[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATInnerTkNormChi2[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATValidFraction[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATChi2LocalPosition[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATTrkKick[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATSegmentCompatibility[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATDBPV[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATDBPVError[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATDZPV[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATDZPVError[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATVldPixHits[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATMatchedStations[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpactTransDist[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpactTransError[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpactTransSignificance[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpact3DDist[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpact3DError[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         muonPF2PATImpact3DSignificance[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATNumSourceCandidates[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           muonPF2PATPackedCandIndex[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATPT[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATET[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATPX[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATPY[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATPZ[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATPhi[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATTheta[NMUONSMAX];   //[numMuonPF2PAT]
   Float_t         genMuonPF2PATEta[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATCharge[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATPdgId[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATMotherId[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATPromptDecayed[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATPromptFinalState[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATHardProcess[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATPythiaSixStatusThree[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATScalarAncestor[NMUONSMAX];   //[numMuonPF2PAT]
   Int_t           genMuonPF2PATDirectScalarAncestor[NMUONSMAX];   //[numMuonPF2PAT]
                   static constexpr size_t NMUONTKPAIRMAX{100};
   Int_t           numMuonTrackPairsPF2PAT;
   Int_t           muonTkPairPF2PATIndex1[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Int_t           muonTkPairPF2PATIndex2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxPx[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxPy[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxPz[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxP2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVx[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVy[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVz[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov00[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov01[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov02[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov10[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov11[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov12[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov20[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov21[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxCov22[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxChi2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxNdof[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxAngleXY[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxDistMagXY[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxDistMagXYSigma[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxAngleXYZ[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxDistMagXYZ[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxDistMagXYZSigma[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Pt[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Px[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Py[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Pz[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1P2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Eta[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Phi[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Int_t           muonTkPairPF2PATTk1Charge[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Chi2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk1Ndof[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Pt[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Px[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Py[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Pz[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2P2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Eta[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Phi[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Int_t           muonTkPairPF2PATTk2Charge[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Chi2[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTk2Ndof[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Float_t         muonTkPairPF2PATTkVtxDcaPreFit[NMUONTKPAIRMAX];   //[numMuonTrackPairsPF2PAT]
   Double_t        metPF2PATE;
   Double_t        metPF2PATEt;
   Double_t        metPF2PATEtRaw;
   Double_t        metPF2PATPhi;
   Double_t        metPF2PATPt;
   Double_t        metPF2PATPx;
   Double_t        metPF2PATPy;
   Double_t        metPF2PATPz;
   Float_t         metPF2PATScalarEt;
   Float_t         metPF2PATEtUncorrected;
   Float_t         metPF2PATPhiUncorrected;
   Float_t         metPF2PATUnclusteredEnUp;
   Float_t         metPF2PATUnclusteredEnDown;
   Float_t         genMetPF2PATE;
   Float_t         genMetPF2PATEt;
   Float_t         genMetPF2PATPhi;
   Float_t         genMetPF2PATPt;
   Float_t         genMetPF2PATPx;
   Float_t         genMetPF2PATPy;
   Float_t         genMetPF2PATPz;
   static constexpr size_t NPACKEDCANDSMAX{4000};
   Int_t           numPackedCands;
   Float_t         packedCandsPx[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPy[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPz[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsE[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsCharge[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsPdgId[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTime[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsFromPV[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsPVquality[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsVx[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsVy[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsVz[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsDz[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsDxy[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsElectronIndex[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsMuonIndex[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsPhotonIndex[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsJetIndex[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsHasTrackDetails[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsDzError[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsDxyError[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTimeError[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkPt[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkPx[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkPy[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkPz[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkEta[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkPhi[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsPseudoTrkCharge[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkVx[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkVy[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkVz[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsPseudoTrkChi2Norm[NPACKEDCANDSMAX];   //[numPackedCands]
   Int_t           packedCandsHighPurityTrack[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpactTransDist[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpactTransError[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpactTransSignificance[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpact3DDist[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpact3DError[NPACKEDCANDSMAX];   //[numPackedCands]
   Float_t         packedCandsTrkImpact3DSignificance[NPACKEDCANDSMAX];   //[numPackedCands]
   static constexpr size_t NCHSTKPAIRMAX{500};
   Int_t           numChsTrackPairs;
   Int_t           chsTkPairIndex1[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Int_t           chsTkPairIndex2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxPx[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxPy[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxPz[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxP2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVx[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVy[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVz[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov00[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov01[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov02[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov10[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov11[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov12[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov20[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov21[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxCov22[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxChi2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxNdof[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxTime[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxTimeError[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxAngleXY[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxDistMagXY[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxDistMagXYSigma[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxAngleXYZ[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxDistMagXYZ[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxDistMagXYZSigma[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Pt[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Px[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Py[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Pz[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1P2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Eta[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Phi[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Int_t           chsTkPairTk1Charge[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Chi2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk1Ndof[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Pt[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Px[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Py[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Pz[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2P2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Eta[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Phi[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Int_t           chsTkPairTk2Charge[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Chi2[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTk2Ndof[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Float_t         chsTkPairTkVtxDcaPreFit[NCHSTKPAIRMAX];   //[numChsTrackPairs]
   Int_t           isElePlusJets;
   Float_t         genPDFScale;
   Float_t         genPDFx1;
   Float_t         genPDFx2;
   Int_t           genPDFf1;
   Int_t           genPDFf2;
   Double_t        topPtReweight;
   Int_t           processId;
   Float_t         processPtHat;
   Double_t        processMCWeight;
   Float_t         beamSpotX;
   Float_t         beamSpotY;
   Float_t         beamSpotZ;

   static constexpr size_t NPVSMAX{100};
   Int_t           numPVs;
   Float_t         pvX[NPVSMAX];   //[numPVs]
   Float_t         pvY[NPVSMAX];   //[numPVs]
   Float_t         pvZ[NPVSMAX];   //[numPVs]
   Float_t         pvCov00[NPVSMAX];   //[numPVs]
   Float_t         pvCov01[NPVSMAX];   //[numPVs]
   Float_t         pvCov02[NPVSMAX];   //[numPVs]
   Float_t         pvCov10[NPVSMAX];   //[numPVs]
   Float_t         pvCov11[NPVSMAX];   //[numPVs]
   Float_t         pvCov12[NPVSMAX];   //[numPVs]
   Float_t         pvCov20[NPVSMAX];   //[numPVs]
   Float_t         pvCov21[NPVSMAX];   //[numPVs]
   Float_t         pvCov22[NPVSMAX];   //[numPVs]
   Float_t         pvRho[NPVSMAX];   //[numPVs]
   Int_t           pvIsFake[NPVSMAX];   //[numPVs]
   Float_t         pvNdof[NPVSMAX];   //[numPVs]
   Float_t         pvChi2[NPVSMAX];   //[numPVs]
   Int_t           pvNtracks[NPVSMAX];   //[numPVs]
   Int_t           pvNtracksW05[NPVSMAX];   //[numPVs]
   Float_t         pvTime[NPVSMAX];   //[numPVs]
   Float_t         pvTimeError[NPVSMAX];   //[numPVs]
   static constexpr size_t NSVSMAX{50};
   Int_t           numSVs;
   Float_t         svPt[NSVSMAX];   //[numSVs]
   Float_t         svPx[NSVSMAX];   //[numSVs]
   Float_t         svPy[NSVSMAX];   //[numSVs]
   Float_t         svPz[NSVSMAX];   //[numSVs]
   Float_t         svMass[NSVSMAX];   //[numSVs]
   Float_t         svE[NSVSMAX];   //[numSVs]
   Float_t         svEta[NSVSMAX];   //[numSVs]
   Float_t         svTheta[NSVSMAX];   //[numSVs]
   Float_t         svPhi[NSVSMAX];   //[numSVs]
   Float_t         svX[NSVSMAX];   //[numSVs]
   Float_t         svY[NSVSMAX];   //[numSVs]
   Float_t         svZ[NSVSMAX];   //[numSVs]
   Float_t         svCov00[NSVSMAX];   //[numSVs]
   Float_t         svCov01[NSVSMAX];   //[numSVs]
   Float_t         svCov02[NSVSMAX];   //[numSVs]
   Float_t         svCov10[NSVSMAX];   //[numSVs]
   Float_t         svCov11[NSVSMAX];   //[numSVs]
   Float_t         svCov12[NSVSMAX];   //[numSVs]
   Float_t         svCov20[NSVSMAX];   //[numSVs]
   Float_t         svCov21[NSVSMAX];   //[numSVs]
   Float_t         svCov22[NSVSMAX];   //[numSVs]
   Float_t         svVertexChi2[NSVSMAX];   //[numSVs]
   Float_t         svVertexNdof[NSVSMAX];   //[numSVs]
   Int_t           svNtracks[NSVSMAX];   //[numSVs]
   Float_t         svDist3D[NSVSMAX];   //[numSVs]
   Float_t         svDist3DSig[NSVSMAX];   //[numSVs]
   Float_t         svDist3DError[NSVSMAX];   //[numSVs]
   Float_t         svDistXY[NSVSMAX];   //[numSVs]
   Float_t         svDistXYSig[NSVSMAX];   //[numSVs]
   Float_t         svDistXYError[NSVSMAX];   //[numSVs]
   Float_t         svAnglePV[NSVSMAX];   //[numSVs]
   Int_t           svIsLambda[NSVSMAX];   //[numSVs]
   Int_t           svIsKshort[NSVSMAX];   //[numSVs]
   Float_t         mhtPt;
   Float_t         mhtPy;
   Float_t         mhtPx;
   Float_t         mhtPhi;
   Float_t         mhtSumEt;
   Float_t         mhtSignif;
   Int_t           nTriggerBits;
   Int_t           TriggerBits[1];   //[nTriggerBits]
   Double_t        weight_muF0p5;
   Double_t        weight_muF2;
   Double_t        weight_muR0p5;
   Double_t        weight_muR2;
   Double_t        weight_muF0p5muR0p5;
   Double_t        weight_muF2muR2;
   Double_t        origWeightForNorm;
   Double_t        weight_pdfMax;
   Double_t        weight_pdfMin;
   Double_t        weight_alphaMax;
   Double_t        weight_alphaMin;
   Double_t        isrRedHi;
   Double_t        fsrRedHi;
   Double_t        isrRedLo;
   Double_t        fsrRedLo;
   Double_t        isrDefHi;
   Double_t        fsrDefHi;
   Double_t        isrDefLo;
   Double_t        fsrDefLo;
   Double_t        isrConHi;
   Double_t        fsrConHi;
   Double_t        isrConLo;
   Double_t        fsrConLo;
   Int_t           HLT_IsoMu27_v8;
   Int_t           HLT_IsoMu27_v9;
   Int_t           HLT_IsoMu27_v10;
   Int_t           HLT_IsoMu27_v11;
   Int_t           HLT_IsoMu27_v12;
   Int_t           HLT_IsoMu27_v13;
   Int_t           HLT_IsoMu27_v14;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3;
   Int_t           HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4;
   Int_t           HLT_DoubleMu43NoFiltersNoVtx_v2;
   Int_t           HLT_DoubleMu43NoFiltersNoVtx_v3;
   Int_t           HLT_DoubleMu48NoFiltersNoVtx_v2;
   Int_t           HLT_DoubleMu48NoFiltersNoVtx_v3;
   Int_t           HLT_DoubleL2Mu50_v1;
   Int_t           HLT_DoubleL2Mu50_v2;
   Int_t           Flag_goodVertices;
   Int_t           Flag_globalTightHalo2016Filter;
   Int_t           Flag_HBHENoiseFilter;
   Int_t           Flag_HBHENoiseIsoFilter;
   Int_t           Flag_EcalDeadCellTriggerPrimitiveFilter;
   Int_t           Flag_BadPFMuonFilter;
   Int_t           Flag_BadChargedCandidateFilter;
   Int_t           Flag_eeBadScFilter;
   Int_t           Flag_ecalBadCalibFilter;
   Int_t           nGenPar;
   static constexpr size_t NGENPARMAX{1000};
   Float_t         genParEta[NGENPARMAX];   //[nGenPar]
   Float_t         genParPhi[NGENPARMAX];   //[nGenPar]
   Float_t         genParE[NGENPARMAX];   //[nGenPar]
   Float_t         genParPt[NGENPARMAX];   //[nGenPar]
   Int_t           genParId[NGENPARMAX];   //[nGenPar]
   Float_t         genParVx[NGENPARMAX];   //[nGenPar]
   Float_t         genParVy[NGENPARMAX];   //[nGenPar]
   Float_t         genParVz[NGENPARMAX];   //[nGenPar]
   Int_t           genParMotherId[NGENPARMAX];   //[nGenPar]
   Int_t           genParMotherIndex[NGENPARMAX];   //[nGenPar]
   Int_t           genParNumDaughters[NGENPARMAX];   //[nGenPar]
   Int_t           genParDaughterId1[NGENPARMAX];   //[nGenPar]
   Int_t           genParDaughterId2[NGENPARMAX];   //[nGenPar]
   Int_t           genParDaughter1Index[NGENPARMAX];   //[nGenPar]
   Int_t           genParDaughter2Index[NGENPARMAX];   //[nGenPar]
   Int_t           genParStatus[NGENPARMAX];   //[nGenPar]
   Int_t           genParCharge[NGENPARMAX];   //[nGenPar]
   Int_t           eventRun;
   Int_t           eventNum;
   Float_t         eventLumiblock;
   Int_t           numVert;

   // List of branches
   TBranch        *b_numMuonPF2PAT;   //!
   TBranch        *b_muonPF2PATE;   //!
   TBranch        *b_muonPF2PATET;   //!
   TBranch        *b_muonPF2PATPt;   //!
   TBranch        *b_muonPF2PATPX;   //!
   TBranch        *b_muonPF2PATPY;   //!
   TBranch        *b_muonPF2PATPZ;   //!
   TBranch        *b_muonPF2PATPhi;   //!
   TBranch        *b_muonPF2PATTheta;   //!
   TBranch        *b_muonPF2PATEta;   //!
   TBranch        *b_muonPF2PATCharge;   //!
   TBranch        *b_muonPF2PATLooseCutId;   //!
   TBranch        *b_muonPF2PATMediumCutId;   //!
   TBranch        *b_muonPF2PATTightCutId;   //!
   TBranch        *b_muonPF2PATPfIsoVeryLoose;   //!
   TBranch        *b_muonPF2PATPfIsoLoose;   //!
   TBranch        *b_muonPF2PATPfIsoMedium;   //!
   TBranch        *b_muonPF2PATPfIsoTight;   //!
   TBranch        *b_muonPF2PATPfIsoVeryTight;   //!
   TBranch        *b_muonPF2PATTkIsoLoose;   //!
   TBranch        *b_muonPF2PATTkIsoTight;   //!
   TBranch        *b_muonPF2PATMvaLoose;   //!
   TBranch        *b_muonPF2PATMvaMedium;   //!
   TBranch        *b_muonPF2PATMvaTight;   //!
   TBranch        *b_muonPF2PATGlobalID;   //!
   TBranch        *b_muonPF2PATTrackID;   //!
   TBranch        *b_muonPF2PATChi2;   //!
   TBranch        *b_muonPF2PATD0;   //!
   TBranch        *b_muonPF2PATTrackDBD0;   //!
   TBranch        *b_muonPF2PATDBInnerTrackD0;   //!
   TBranch        *b_muonPF2PATBeamSpotCorrectedD0;   //!
   TBranch        *b_muonPF2PATTrackNHits;   //!
   TBranch        *b_muonPF2PATMuonNHits;   //!
   TBranch        *b_muonPF2PATNDOF;   //!
   TBranch        *b_muonPF2PATVertX;   //!
   TBranch        *b_muonPF2PATVertY;   //!
   TBranch        *b_muonPF2PATVertZ;   //!
   TBranch        *b_muonPF2PATInnerTkPt;   //!
   TBranch        *b_muonPF2PATInnerTkPx;   //!
   TBranch        *b_muonPF2PATInnerTkPy;   //!
   TBranch        *b_muonPF2PATInnerTkPz;   //!
   TBranch        *b_muonPF2PATInnerTkEta;   //!
   TBranch        *b_muonPF2PATInnerTkPhi;   //!
   TBranch        *b_muonPF2PATChargedHadronIso;   //!
   TBranch        *b_muonPF2PATNeutralHadronIso;   //!
   TBranch        *b_muonPF2PATPhotonIso;   //!
   TBranch        *b_muonPF2PATTrackIso;   //!
   TBranch        *b_muonPF2PATEcalIso;   //!
   TBranch        *b_muonPF2PATHcalIso;   //!
   TBranch        *b_muonPF2PATComRelIso;   //!
   TBranch        *b_muonPF2PATComRelIsodBeta;   //!
   TBranch        *b_muonPF2PATChHadIso;   //!
   TBranch        *b_muonPF2PATNtHadIso;   //!
   TBranch        *b_muonPF2PATGammaIso;   //!
   TBranch        *b_muonPF2PATPuIso;   //!
   TBranch        *b_muonPF2PATIsPFMuon;   //!
   TBranch        *b_muonPF2PATNChambers;   //!
   TBranch        *b_muonPF2PATNMatches;   //!
   TBranch        *b_muonPF2PATTkLysWithMeasurements;   //!
   TBranch        *b_muonPF2PATGlbTkNormChi2;   //!
   TBranch        *b_muonPF2PATInnerTkNormChi2;   //!
   TBranch        *b_muonPF2PATValidFraction;   //!
   TBranch        *b_muonPF2PATChi2LocalPosition;   //!
   TBranch        *b_muonPF2PATTrkKick;   //!
   TBranch        *b_muonPF2PATSegmentCompatibility;   //!
   TBranch        *b_muonPF2PATDBPV;   //!
   TBranch        *b_muonPF2PATDBPVError;   //!
   TBranch        *b_muonPF2PATDZPV;   //!
   TBranch        *b_muonPF2PATDZPVError;   //!
   TBranch        *b_muonPF2PATVldPixHits;   //!
   TBranch        *b_muonPF2PATMatchedStations;   //!
   TBranch        *b_muonPF2PATImpactTransDist;   //!
   TBranch        *b_muonPF2PATImpactTransError;   //!
   TBranch        *b_muonPF2PATImpactTransSignificance;   //!
   TBranch        *b_muonPF2PATImpact3DDist;   //!
   TBranch        *b_muonPF2PATImpact3DError;   //!
   TBranch        *b_muonPF2PATImpact3DSignificance;   //!
   TBranch        *b_muonPF2PATNumSourceCandidates;   //!
   TBranch        *b_muonPF2PATPackedCandIndex;   //!
   TBranch        *b_genMuonPF2PATPT;   //!
   TBranch        *b_genMuonPF2PATET;   //!
   TBranch        *b_genMuonPF2PATPX;   //!
   TBranch        *b_genMuonPF2PATPY;   //!
   TBranch        *b_genMuonPF2PATPZ;   //!
   TBranch        *b_genMuonPF2PATPhi;   //!
   TBranch        *b_genMuonPF2PATTheta;   //!
   TBranch        *b_genMuonPF2PATEta;   //!
   TBranch        *b_genMuonPF2PATCharge;   //!
   TBranch        *b_genMuonPF2PATPdgId;   //!
   TBranch        *b_genMuonPF2PATMotherId;   //!
   TBranch        *b_genMuonPF2PATPromptDecayed;   //!
   TBranch        *b_genMuonPF2PATPromptFinalState;   //!
   TBranch        *b_genMuonPF2PATHardProcess;   //!
   TBranch        *b_genMuonPF2PATPythiaSixStatusThree;   //!
   TBranch        *b_genMuonPF2PATScalarAncestor;   //!
   TBranch        *b_genMuonPF2PATDirectScalarAncestor;   //!
   TBranch        *b_numMuonTrackPairsPF2PAT;   //!
   TBranch        *b_muonTkPairPF2PATIndex1;   //!
   TBranch        *b_muonTkPairPF2PATIndex2;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxPx;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxPy;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxPz;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxP2;   //!
   TBranch        *b_muonTkPairPF2PATTkVx;   //!
   TBranch        *b_muonTkPairPF2PATTkVy;   //!
   TBranch        *b_muonTkPairPF2PATTkVz;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov00;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov01;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov02;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov10;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov11;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov12;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov20;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov21;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxCov22;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxChi2;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxNdof;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxAngleXY;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxDistMagXY;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxDistMagXYSigma;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxAngleXYZ;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxDistMagXYZ;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxDistMagXYZSigma;   //!
   TBranch        *b_muonTkPairPF2PATTk1Pt;   //!
   TBranch        *b_muonTkPairPF2PATTk1Px;   //!
   TBranch        *b_muonTkPairPF2PATTk1Py;   //!
   TBranch        *b_muonTkPairPF2PATTk1Pz;   //!
   TBranch        *b_muonTkPairPF2PATTk1P2;   //!
   TBranch        *b_muonTkPairPF2PATTk1Eta;   //!
   TBranch        *b_muonTkPairPF2PATTk1Phi;   //!
   TBranch        *b_muonTkPairPF2PATTk1Charge;   //!
   TBranch        *b_muonTkPairPF2PATTk1Chi2;   //!
   TBranch        *b_muonTkPairPF2PATTk1Ndof;   //!
   TBranch        *b_muonTkPairPF2PATTk2Pt;   //!
   TBranch        *b_muonTkPairPF2PATTk2Px;   //!
   TBranch        *b_muonTkPairPF2PATTk2Py;   //!
   TBranch        *b_muonTkPairPF2PATTk2Pz;   //!
   TBranch        *b_muonTkPairPF2PATTk2P2;   //!
   TBranch        *b_muonTkPairPF2PATTk2Eta;   //!
   TBranch        *b_muonTkPairPF2PATTk2Phi;   //!
   TBranch        *b_muonTkPairPF2PATTk2Charge;   //!
   TBranch        *b_muonTkPairPF2PATTk2Chi2;   //!
   TBranch        *b_muonTkPairPF2PATTk2Ndof;   //!
   TBranch        *b_muonTkPairPF2PATTkVtxDcaPreFit;   //!
   TBranch        *b_metPF2PATE;   //!
   TBranch        *b_metPF2PATEt;   //!
   TBranch        *b_metPF2PATEtRaw;   //!
   TBranch        *b_metPF2PATPhi;   //!
   TBranch        *b_metPF2PATPt;   //!
   TBranch        *b_metPF2PATPx;   //!
   TBranch        *b_metPF2PATPy;   //!
   TBranch        *b_metPF2PATPz;   //!
   TBranch        *b_metPF2PATScalarEt;   //!
   TBranch        *b_metPF2PATEtUncorrected;   //!
   TBranch        *b_metPF2PATPhiUncorrected;   //!
   TBranch        *b_metPF2PATUnclusteredEnUp;   //!
   TBranch        *b_metPF2PATUnclusteredEnDown;   //!
   TBranch        *b_genMetPF2PATE;   //!
   TBranch        *b_genMetPF2PATEt;   //!
   TBranch        *b_genMetPF2PATPhi;   //!
   TBranch        *b_genMetPF2PATPt;   //!
   TBranch        *b_genMetPF2PATPx;   //!
   TBranch        *b_genMetPF2PATPy;   //!
   TBranch        *b_genMetPF2PATPz;   //!
   TBranch        *b_numPackedCands;   //!
   TBranch        *b_packedCandsPx;   //!
   TBranch        *b_packedCandsPy;   //!
   TBranch        *b_packedCandsPz;   //!
   TBranch        *b_packedCandsE;   //!
   TBranch        *b_packedCandsCharge;   //!
   TBranch        *b_packedCandsPdgId;   //!
   TBranch        *b_packedCandsTime;   //!
   TBranch        *b_packedCandsFromPV;   //!
   TBranch        *b_packedCandsPVquality;   //!
   TBranch        *b_packedCandsVx;   //!
   TBranch        *b_packedCandsVy;   //!
   TBranch        *b_packedCandsVz;   //!
   TBranch        *b_packedCandsDz;   //!
   TBranch        *b_packedCandsDxy;   //!
   TBranch        *b_packedCandsElectronIndex;   //!
   TBranch        *b_packedCandsMuonIndex;   //!
   TBranch        *b_packedCandsPhotonIndex;   //!
   TBranch        *b_packedCandsJetIndex;   //!
   TBranch        *b_packedCandsHasTrackDetails;   //!
   TBranch        *b_packedCandsDzError;   //!
   TBranch        *b_packedCandsDxyError;   //!
   TBranch        *b_packedCandsTimeError;   //!
   TBranch        *b_packedCandsPseudoTrkPt;   //!
   TBranch        *b_packedCandsPseudoTrkPx;   //!
   TBranch        *b_packedCandsPseudoTrkPy;   //!
   TBranch        *b_packedCandsPseudoTrkPz;   //!
   TBranch        *b_packedCandsPseudoTrkEta;   //!
   TBranch        *b_packedCandsPseudoTrkPhi;   //!
   TBranch        *b_packedCandsPseudoTrkCharge;   //!
   TBranch        *b_packedCandsPseudoTrkVx;   //!
   TBranch        *b_packedCandsPseudoTrkVy;   //!
   TBranch        *b_packedCandsPseudoTrkVz;   //!
   TBranch        *b_packedCandsPseudoTrkChi2Norm;   //!
   TBranch        *b_packedCandsHighPurityTrack;   //!
   TBranch        *b_packedCandsTrkImpactTransDist;   //!
   TBranch        *b_packedCandsTrkImpactTransError;   //!
   TBranch        *b_packedCandsTrkImpactTransSignificance;   //!
   TBranch        *b_packedCandsTrkImpact3DDist;   //!
   TBranch        *b_packedCandsTrkImpact3DError;   //!
   TBranch        *b_packedCandsTrkImpact3DSignificance;   //!
   TBranch        *b_numChsTrackPairs;   //!
   TBranch        *b_chsTkPairIndex1;   //!
   TBranch        *b_chsTkPairIndex2;   //!
   TBranch        *b_chsTkPairTkVtxPx;   //!
   TBranch        *b_chsTkPairTkVtxPy;   //!
   TBranch        *b_chsTkPairTkVtxPz;   //!
   TBranch        *b_chsTkPairTkVtxP2;   //!
   TBranch        *b_chsTkPairTkVx;   //!
   TBranch        *b_chsTkPairTkVy;   //!
   TBranch        *b_chsTkPairTkVz;   //!
   TBranch        *b_chsTkPairTkVtxCov00;   //!
   TBranch        *b_chsTkPairTkVtxCov01;   //!
   TBranch        *b_chsTkPairTkVtxCov02;   //!
   TBranch        *b_chsTkPairTkVtxCov10;   //!
   TBranch        *b_chsTkPairTkVtxCov11;   //!
   TBranch        *b_chsTkPairTkVtxCov12;   //!
   TBranch        *b_chsTkPairTkVtxCov20;   //!
   TBranch        *b_chsTkPairTkVtxCov21;   //!
   TBranch        *b_chsTkPairTkVtxCov22;   //!
   TBranch        *b_chsTkPairTkVtxChi2;   //!
   TBranch        *b_chsTkPairTkVtxNdof;   //!
   TBranch        *b_chsTkPairTkVtxTime;   //!
   TBranch        *b_chsTkPairTkVtxTimeError;   //!
   TBranch        *b_chsTkPairTkVtxAngleXY;   //!
   TBranch        *b_chsTkPairTkVtxDistMagXY;   //!
   TBranch        *b_chsTkPairTkVtxDistMagXYSigma;   //!
   TBranch        *b_chsTkPairTkVtxAngleXYZ;   //!
   TBranch        *b_chsTkPairTkVtxDistMagXYZ;   //!
   TBranch        *b_chsTkPairTkVtxDistMagXYZSigma;   //!
   TBranch        *b_chsTkPairTk1Pt;   //!
   TBranch        *b_chsTkPairTk1Px;   //!
   TBranch        *b_chsTkPairTk1Py;   //!
   TBranch        *b_chsTkPairTk1Pz;   //!
   TBranch        *b_chsTkPairTk1P2;   //!
   TBranch        *b_chsTkPairTk1Eta;   //!
   TBranch        *b_chsTkPairTk1Phi;   //!
   TBranch        *b_chsTkPairTk1Charge;   //!
   TBranch        *b_chsTkPairTk1Chi2;   //!
   TBranch        *b_chsTkPairTk1Ndof;   //!
   TBranch        *b_chsTkPairTk2Pt;   //!
   TBranch        *b_chsTkPairTk2Px;   //!
   TBranch        *b_chsTkPairTk2Py;   //!
   TBranch        *b_chsTkPairTk2Pz;   //!
   TBranch        *b_chsTkPairTk2P2;   //!
   TBranch        *b_chsTkPairTk2Eta;   //!
   TBranch        *b_chsTkPairTk2Phi;   //!
   TBranch        *b_chsTkPairTk2Charge;   //!
   TBranch        *b_chsTkPairTk2Chi2;   //!
   TBranch        *b_chsTkPairTk2Ndof;   //!
   TBranch        *b_chsTkPairTkVtxDcaPreFit;   //!
   TBranch        *b_isElePlusJets;   //!
   TBranch        *b_genPDFScale;   //!
   TBranch        *b_genPDFx1;   //!
   TBranch        *b_genPDFx2;   //!
   TBranch        *b_genPDFf1;   //!
   TBranch        *b_genPDFf2;   //!
   TBranch        *b_topPtReweight;   //!
   TBranch        *b_processId;   //!
   TBranch        *b_processPtHat;   //!
   TBranch        *b_processMCWeight;   //!
   TBranch        *b_beamSpotX;   //!
   TBranch        *b_beamSpotY;   //!
   TBranch        *b_beamSpotZ;   //!
   TBranch        *b_numPVs;   //!
   TBranch        *b_pvX;   //!
   TBranch        *b_pvY;   //!
   TBranch        *b_pvZ;   //!
   TBranch        *b_pvCov00;   //!
   TBranch        *b_pvCov01;   //!
   TBranch        *b_pvCov02;   //!
   TBranch        *b_pvCov10;   //!
   TBranch        *b_pvCov11;   //!
   TBranch        *b_pvCov12;   //!
   TBranch        *b_pvCov20;   //!
   TBranch        *b_pvCov21;   //!
   TBranch        *b_pvCov22;   //!
   TBranch        *b_pvRho;   //!
   TBranch        *b_pvIsFake;   //!
   TBranch        *b_pvNdof;   //!
   TBranch        *b_pvChi2;   //!
   TBranch        *b_pvNtracks;   //!
   TBranch        *b_pvNtracksW05;   //!
   TBranch        *b_pvTime;   //!
   TBranch        *b_pvTimeError;   //!
   TBranch        *b_numSVs;   //!
   TBranch        *b_svPt;   //!
   TBranch        *b_svPx;   //!
   TBranch        *b_svPy;   //!
   TBranch        *b_svPz;   //!
   TBranch        *b_svMass;   //!
   TBranch        *b_svE;   //!
   TBranch        *b_svEta;   //!
   TBranch        *b_svTheta;   //!
   TBranch        *b_svPhi;   //!
   TBranch        *b_svX;   //!
   TBranch        *b_svY;   //!
   TBranch        *b_svZ;   //!
   TBranch        *b_svCov00;   //!
   TBranch        *b_svCov01;   //!
   TBranch        *b_svCov02;   //!
   TBranch        *b_svCov10;   //!
   TBranch        *b_svCov11;   //!
   TBranch        *b_svCov12;   //!
   TBranch        *b_svCov20;   //!
   TBranch        *b_svCov21;   //!
   TBranch        *b_svCov22;   //!
   TBranch        *b_svVertexChi2;   //!
   TBranch        *b_svVertexNdof;   //!
   TBranch        *b_svNtracks;   //!
   TBranch        *b_svDist3D;   //!
   TBranch        *b_svDist3DSig;   //!
   TBranch        *b_svDist3DError;   //!
   TBranch        *b_svDistXY;   //!
   TBranch        *b_svDistXYSig;   //!
   TBranch        *b_svDistXYError;   //!
   TBranch        *b_svAnglePV;   //!
   TBranch        *b_svIsLambda;   //!
   TBranch        *b_svIsKshort;   //!
   TBranch        *b_mhtPt;   //!
   TBranch        *b_mhtPy;   //!
   TBranch        *b_mhtPx;   //!
   TBranch        *b_mhtPhi;   //!
   TBranch        *b_mhtSumEt;   //!
   TBranch        *b_mhtSignif;   //!
   TBranch        *b_nTriggerBits;   //!
   TBranch        *b_TriggerBits;   //!
   TBranch        *b_weight_muF0p5;   //!
   TBranch        *b_weight_muF2;   //!
   TBranch        *b_weight_muR0p5;   //!
   TBranch        *b_weight_muR2;   //!
   TBranch        *b_weight_muF0p5muR0p5;   //!
   TBranch        *b_weight_muF2muR2;   //!
   TBranch        *b_origWeightForNorm;   //!
   TBranch        *b_weight_pdfMax;   //!
   TBranch        *b_weight_pdfMin;   //!
   TBranch        *b_weight_alphaMax;   //!
   TBranch        *b_weight_alphaMin;   //!
   TBranch        *b_isrRedHi;   //!
   TBranch        *b_fsrRedHi;   //!
   TBranch        *b_isrRedLo;   //!
   TBranch        *b_fsrRedLo;   //!
   TBranch        *b_isrDefHi;   //!
   TBranch        *b_fsrDefHi;   //!
   TBranch        *b_isrDefLo;   //!
   TBranch        *b_fsrDefLo;   //!
   TBranch        *b_isrConHi;   //!
   TBranch        *b_fsrConHi;   //!
   TBranch        *b_isrConLo;   //!
   TBranch        *b_fsrConLo;   //!
   TBranch        *b_HLT_IsoMu27_v8;   //!
   TBranch        *b_HLT_IsoMu27_v9;   //!
   TBranch        *b_HLT_IsoMu27_v10;   //!
   TBranch        *b_HLT_IsoMu27_v11;   //!
   TBranch        *b_HLT_IsoMu27_v12;   //!
   TBranch        *b_HLT_IsoMu27_v13;   //!
   TBranch        *b_HLT_IsoMu27_v14;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v8;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v9;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v10;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v11;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v12;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v13;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v14;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v1;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v2;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v3;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v4;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v7;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v8;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v1;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v2;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v3;   //!
   TBranch        *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v4;   //!
   TBranch        *b_HLT_DoubleMu43NoFiltersNoVtx_v2;   //!
   TBranch        *b_HLT_DoubleMu43NoFiltersNoVtx_v3;   //!
   TBranch        *b_HLT_DoubleMu48NoFiltersNoVtx_v2;   //!
   TBranch        *b_HLT_DoubleMu48NoFiltersNoVtx_v3;   //!
   TBranch        *b_HLT_DoubleL2Mu50_v1;   //!
   TBranch        *b_HLT_DoubleL2Mu50_v2;   //!
   TBranch        *b_Flag_goodVertices;   //!
   TBranch        *b_Flag_globalTightHalo2016Filter;   //!
   TBranch        *b_Flag_HBHENoiseFilter;   //!
   TBranch        *b_Flag_HBHENoiseIsoFilter;   //!
   TBranch        *b_Flag_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_Flag_BadPFMuonFilter;   //!
   TBranch        *b_Flag_BadChargedCandidateFilter;   //!
   TBranch        *b_Flag_eeBadScFilter;   //!
   TBranch        *b_Flag_ecalBadCalibFilter;   //!
   TBranch        *b_nGenPar;   //!
   TBranch        *b_genParEta;   //!
   TBranch        *b_genParPhi;   //!
   TBranch        *b_genParE;   //!
   TBranch        *b_genParPt;   //!
   TBranch        *b_genParId;   //!
   TBranch        *b_genParVx;   //!
   TBranch        *b_genParVy;   //!
   TBranch        *b_genParVz;   //!
   TBranch        *b_genParMotherId;   //!
   TBranch        *b_genParMotherIndex;   //!
   TBranch        *b_genParNumDaughters;   //!
   TBranch        *b_genParDaughterId1;   //!
   TBranch        *b_genParDaughterId2;   //!
   TBranch        *b_genParDaughter1Index;   //!
   TBranch        *b_genParDaughter2Index;   //!
   TBranch        *b_genParStatus;   //!
   TBranch        *b_genParCharge;   //!
   TBranch        *b_eventRun;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_eventLumiblock;   //!
   TBranch        *b_numVert;   //!

   
    bool isMC_{};
    const bool is2016_{};
    const bool is2018_{};
    float eventWeight;

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

    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> zPairLeptons;
    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> zPairLeptonsRefitted;
    std::pair<float, float> zPairRelIso;
    std::pair<float, float> zPairChIso;
    std::pair<float, float> zPairNhIso;
    std::pair<float, float> zPairPhIso;
    std::pair<float, float> zPairPuIso;
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

    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> chsPairVec; // PF Candidate
    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> chsPairTrkVec; // reco::Track
    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> chsPairTrkVecRefitted; // After refit
    std::pair<float, float> chsPairRelIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairChIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairNhIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairPhIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairPuIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairTrkIso;// for PF Candidate -> switch to refitted track?
    std::pair<float, float> chsPairRelIsoDbeta;
    std::pair<float, float> chsPairTrkIsoDbeta;
    std::pair<int, int> chsPairIndex;
    int chsPairTrkIndex;
    float chsRelIso;
    float chsTrkIso;
    float chsRelIsoDbeta;
    float chsTrkIsoDbeta;

    std::pair<ROOT::Math::PxPyPzMVector, ROOT::Math::PxPyPzMVector> wPairQuarks;
    std::pair<int, int> wPairIndex;

    ROOT::Math::PxPyPzMVector wLepton;
    int wLepIndex;
    float wLeptonRelIso;

    Int_t numVert;
    TBranch* b_numVert;

    AnalysisEventAlt(bool isMC, TTree* tree, bool is2016, bool is2018);
    virtual ~AnalysisEventAlt();
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

#endif

#ifdef sample_test_cxx
sample_test::sample_test(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("test.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("test.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("test.root:/makeTopologyNtupleMiniAOD");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

sample_test::~sample_test()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t sample_test::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t sample_test::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void sample_test::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
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
   fChain->SetBranchAddress("muonPF2PATPfIsoVeryLoose", muonPF2PATPfIsoVeryLoose, &b_muonPF2PATPfIsoVeryLoose);
   fChain->SetBranchAddress("muonPF2PATPfIsoLoose", muonPF2PATPfIsoLoose, &b_muonPF2PATPfIsoLoose);
   fChain->SetBranchAddress("muonPF2PATPfIsoMedium", muonPF2PATPfIsoMedium, &b_muonPF2PATPfIsoMedium);
   fChain->SetBranchAddress("muonPF2PATPfIsoTight", muonPF2PATPfIsoTight, &b_muonPF2PATPfIsoTight);
   fChain->SetBranchAddress("muonPF2PATPfIsoVeryTight", muonPF2PATPfIsoVeryTight, &b_muonPF2PATPfIsoVeryTight);
   fChain->SetBranchAddress("muonPF2PATTkIsoLoose", muonPF2PATTkIsoLoose, &b_muonPF2PATTkIsoLoose);
   fChain->SetBranchAddress("muonPF2PATTkIsoTight", muonPF2PATTkIsoTight, &b_muonPF2PATTkIsoTight);
   fChain->SetBranchAddress("muonPF2PATMvaLoose", muonPF2PATMvaLoose, &b_muonPF2PATMvaLoose);
   fChain->SetBranchAddress("muonPF2PATMvaMedium", muonPF2PATMvaMedium, &b_muonPF2PATMvaMedium);
   fChain->SetBranchAddress("muonPF2PATMvaTight", muonPF2PATMvaTight, &b_muonPF2PATMvaTight);
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
   fChain->SetBranchAddress("muonPF2PATNumSourceCandidates", muonPF2PATNumSourceCandidates, &b_muonPF2PATNumSourceCandidates);
   fChain->SetBranchAddress("muonPF2PATPackedCandIndex", muonPF2PATPackedCandIndex, &b_muonPF2PATPackedCandIndex);
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
   fChain->SetBranchAddress("genMuonPF2PATScalarAncestor", genMuonPF2PATScalarAncestor, &b_genMuonPF2PATScalarAncestor);
   fChain->SetBranchAddress("genMuonPF2PATDirectScalarAncestor", genMuonPF2PATDirectScalarAncestor, &b_genMuonPF2PATDirectScalarAncestor);
   fChain->SetBranchAddress("numMuonTrackPairsPF2PAT", &numMuonTrackPairsPF2PAT, &b_numMuonTrackPairsPF2PAT);
   fChain->SetBranchAddress("muonTkPairPF2PATIndex1", muonTkPairPF2PATIndex1, &b_muonTkPairPF2PATIndex1);
   fChain->SetBranchAddress("muonTkPairPF2PATIndex2", muonTkPairPF2PATIndex2, &b_muonTkPairPF2PATIndex2);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxPx", muonTkPairPF2PATTkVtxPx, &b_muonTkPairPF2PATTkVtxPx);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxPy", muonTkPairPF2PATTkVtxPy, &b_muonTkPairPF2PATTkVtxPy);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxPz", muonTkPairPF2PATTkVtxPz, &b_muonTkPairPF2PATTkVtxPz);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxP2", muonTkPairPF2PATTkVtxP2, &b_muonTkPairPF2PATTkVtxP2);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVx", muonTkPairPF2PATTkVx, &b_muonTkPairPF2PATTkVx);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVy", muonTkPairPF2PATTkVy, &b_muonTkPairPF2PATTkVy);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVz", muonTkPairPF2PATTkVz, &b_muonTkPairPF2PATTkVz);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov00", muonTkPairPF2PATTkVtxCov00, &b_muonTkPairPF2PATTkVtxCov00);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov01", muonTkPairPF2PATTkVtxCov01, &b_muonTkPairPF2PATTkVtxCov01);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov02", muonTkPairPF2PATTkVtxCov02, &b_muonTkPairPF2PATTkVtxCov02);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov10", muonTkPairPF2PATTkVtxCov10, &b_muonTkPairPF2PATTkVtxCov10);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov11", muonTkPairPF2PATTkVtxCov11, &b_muonTkPairPF2PATTkVtxCov11);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov12", muonTkPairPF2PATTkVtxCov12, &b_muonTkPairPF2PATTkVtxCov12);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov20", muonTkPairPF2PATTkVtxCov20, &b_muonTkPairPF2PATTkVtxCov20);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov21", muonTkPairPF2PATTkVtxCov21, &b_muonTkPairPF2PATTkVtxCov21);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxCov22", muonTkPairPF2PATTkVtxCov22, &b_muonTkPairPF2PATTkVtxCov22);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxChi2", muonTkPairPF2PATTkVtxChi2, &b_muonTkPairPF2PATTkVtxChi2);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxNdof", muonTkPairPF2PATTkVtxNdof, &b_muonTkPairPF2PATTkVtxNdof);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxAngleXY", muonTkPairPF2PATTkVtxAngleXY, &b_muonTkPairPF2PATTkVtxAngleXY);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxDistMagXY", muonTkPairPF2PATTkVtxDistMagXY, &b_muonTkPairPF2PATTkVtxDistMagXY);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxDistMagXYSigma", muonTkPairPF2PATTkVtxDistMagXYSigma, &b_muonTkPairPF2PATTkVtxDistMagXYSigma);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxAngleXYZ", muonTkPairPF2PATTkVtxAngleXYZ, &b_muonTkPairPF2PATTkVtxAngleXYZ);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxDistMagXYZ", muonTkPairPF2PATTkVtxDistMagXYZ, &b_muonTkPairPF2PATTkVtxDistMagXYZ);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxDistMagXYZSigma", muonTkPairPF2PATTkVtxDistMagXYZSigma, &b_muonTkPairPF2PATTkVtxDistMagXYZSigma);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Pt", muonTkPairPF2PATTk1Pt, &b_muonTkPairPF2PATTk1Pt);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Px", muonTkPairPF2PATTk1Px, &b_muonTkPairPF2PATTk1Px);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Py", muonTkPairPF2PATTk1Py, &b_muonTkPairPF2PATTk1Py);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Pz", muonTkPairPF2PATTk1Pz, &b_muonTkPairPF2PATTk1Pz);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1P2", muonTkPairPF2PATTk1P2, &b_muonTkPairPF2PATTk1P2);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Eta", muonTkPairPF2PATTk1Eta, &b_muonTkPairPF2PATTk1Eta);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Phi", muonTkPairPF2PATTk1Phi, &b_muonTkPairPF2PATTk1Phi);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Charge", muonTkPairPF2PATTk1Charge, &b_muonTkPairPF2PATTk1Charge);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Chi2", muonTkPairPF2PATTk1Chi2, &b_muonTkPairPF2PATTk1Chi2);
   fChain->SetBranchAddress("muonTkPairPF2PATTk1Ndof", muonTkPairPF2PATTk1Ndof, &b_muonTkPairPF2PATTk1Ndof);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Pt", muonTkPairPF2PATTk2Pt, &b_muonTkPairPF2PATTk2Pt);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Px", muonTkPairPF2PATTk2Px, &b_muonTkPairPF2PATTk2Px);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Py", muonTkPairPF2PATTk2Py, &b_muonTkPairPF2PATTk2Py);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Pz", muonTkPairPF2PATTk2Pz, &b_muonTkPairPF2PATTk2Pz);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2P2", muonTkPairPF2PATTk2P2, &b_muonTkPairPF2PATTk2P2);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Eta", muonTkPairPF2PATTk2Eta, &b_muonTkPairPF2PATTk2Eta);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Phi", muonTkPairPF2PATTk2Phi, &b_muonTkPairPF2PATTk2Phi);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Charge", muonTkPairPF2PATTk2Charge, &b_muonTkPairPF2PATTk2Charge);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Chi2", muonTkPairPF2PATTk2Chi2, &b_muonTkPairPF2PATTk2Chi2);
   fChain->SetBranchAddress("muonTkPairPF2PATTk2Ndof", muonTkPairPF2PATTk2Ndof, &b_muonTkPairPF2PATTk2Ndof);
   fChain->SetBranchAddress("muonTkPairPF2PATTkVtxDcaPreFit", muonTkPairPF2PATTkVtxDcaPreFit, &b_muonTkPairPF2PATTkVtxDcaPreFit);
   fChain->SetBranchAddress("metPF2PATE", &metPF2PATE, &b_metPF2PATE);
   fChain->SetBranchAddress("metPF2PATEt", &metPF2PATEt, &b_metPF2PATEt);
   fChain->SetBranchAddress("metPF2PATEtRaw", &metPF2PATEtRaw, &b_metPF2PATEtRaw);
   fChain->SetBranchAddress("metPF2PATPhi", &metPF2PATPhi, &b_metPF2PATPhi);
   fChain->SetBranchAddress("metPF2PATPt", &metPF2PATPt, &b_metPF2PATPt);
   fChain->SetBranchAddress("metPF2PATPx", &metPF2PATPx, &b_metPF2PATPx);
   fChain->SetBranchAddress("metPF2PATPy", &metPF2PATPy, &b_metPF2PATPy);
   fChain->SetBranchAddress("metPF2PATPz", &metPF2PATPz, &b_metPF2PATPz);
   fChain->SetBranchAddress("metPF2PATScalarEt", &metPF2PATScalarEt, &b_metPF2PATScalarEt);
   fChain->SetBranchAddress("metPF2PATEtUncorrected", &metPF2PATEtUncorrected, &b_metPF2PATEtUncorrected);
   fChain->SetBranchAddress("metPF2PATPhiUncorrected", &metPF2PATPhiUncorrected, &b_metPF2PATPhiUncorrected);
   fChain->SetBranchAddress("metPF2PATUnclusteredEnUp", &metPF2PATUnclusteredEnUp, &b_metPF2PATUnclusteredEnUp);
   fChain->SetBranchAddress("metPF2PATUnclusteredEnDown", &metPF2PATUnclusteredEnDown, &b_metPF2PATUnclusteredEnDown);
   fChain->SetBranchAddress("genMetPF2PATE", &genMetPF2PATE, &b_genMetPF2PATE);
   fChain->SetBranchAddress("genMetPF2PATEt", &genMetPF2PATEt, &b_genMetPF2PATEt);
   fChain->SetBranchAddress("genMetPF2PATPhi", &genMetPF2PATPhi, &b_genMetPF2PATPhi);
   fChain->SetBranchAddress("genMetPF2PATPt", &genMetPF2PATPt, &b_genMetPF2PATPt);
   fChain->SetBranchAddress("genMetPF2PATPx", &genMetPF2PATPx, &b_genMetPF2PATPx);
   fChain->SetBranchAddress("genMetPF2PATPy", &genMetPF2PATPy, &b_genMetPF2PATPy);
   fChain->SetBranchAddress("genMetPF2PATPz", &genMetPF2PATPz, &b_genMetPF2PATPz);
   fChain->SetBranchAddress("numPackedCands", &numPackedCands, &b_numPackedCands);
   fChain->SetBranchAddress("packedCandsPx", packedCandsPx, &b_packedCandsPx);
   fChain->SetBranchAddress("packedCandsPy", packedCandsPy, &b_packedCandsPy);
   fChain->SetBranchAddress("packedCandsPz", packedCandsPz, &b_packedCandsPz);
   fChain->SetBranchAddress("packedCandsE", packedCandsE, &b_packedCandsE);
   fChain->SetBranchAddress("packedCandsCharge", packedCandsCharge, &b_packedCandsCharge);
   fChain->SetBranchAddress("packedCandsPdgId", packedCandsPdgId, &b_packedCandsPdgId);
   fChain->SetBranchAddress("packedCandsTime", packedCandsTime, &b_packedCandsTime);
   fChain->SetBranchAddress("packedCandsFromPV", packedCandsFromPV, &b_packedCandsFromPV);
   fChain->SetBranchAddress("packedCandsPVquality", packedCandsPVquality, &b_packedCandsPVquality);
   fChain->SetBranchAddress("packedCandsVx", packedCandsVx, &b_packedCandsVx);
   fChain->SetBranchAddress("packedCandsVy", packedCandsVy, &b_packedCandsVy);
   fChain->SetBranchAddress("packedCandsVz", packedCandsVz, &b_packedCandsVz);
   fChain->SetBranchAddress("packedCandsDz", packedCandsDz, &b_packedCandsDz);
   fChain->SetBranchAddress("packedCandsDxy", packedCandsDxy, &b_packedCandsDxy);
   fChain->SetBranchAddress("packedCandsElectronIndex", packedCandsElectronIndex, &b_packedCandsElectronIndex);
   fChain->SetBranchAddress("packedCandsMuonIndex", packedCandsMuonIndex, &b_packedCandsMuonIndex);
   fChain->SetBranchAddress("packedCandsPhotonIndex", packedCandsPhotonIndex, &b_packedCandsPhotonIndex);
   fChain->SetBranchAddress("packedCandsJetIndex", packedCandsJetIndex, &b_packedCandsJetIndex);
   fChain->SetBranchAddress("packedCandsHasTrackDetails", packedCandsHasTrackDetails, &b_packedCandsHasTrackDetails);
   fChain->SetBranchAddress("packedCandsDzError", packedCandsDzError, &b_packedCandsDzError);
   fChain->SetBranchAddress("packedCandsDxyError", packedCandsDxyError, &b_packedCandsDxyError);
   fChain->SetBranchAddress("packedCandsTimeError", packedCandsTimeError, &b_packedCandsTimeError);
   fChain->SetBranchAddress("packedCandsPseudoTrkPt", packedCandsPseudoTrkPt, &b_packedCandsPseudoTrkPt);
   fChain->SetBranchAddress("packedCandsPseudoTrkPx", packedCandsPseudoTrkPx, &b_packedCandsPseudoTrkPx);
   fChain->SetBranchAddress("packedCandsPseudoTrkPy", packedCandsPseudoTrkPy, &b_packedCandsPseudoTrkPy);
   fChain->SetBranchAddress("packedCandsPseudoTrkPz", packedCandsPseudoTrkPz, &b_packedCandsPseudoTrkPz);
   fChain->SetBranchAddress("packedCandsPseudoTrkEta", packedCandsPseudoTrkEta, &b_packedCandsPseudoTrkEta);
   fChain->SetBranchAddress("packedCandsPseudoTrkPhi", packedCandsPseudoTrkPhi, &b_packedCandsPseudoTrkPhi);
   fChain->SetBranchAddress("packedCandsPseudoTrkCharge", packedCandsPseudoTrkCharge, &b_packedCandsPseudoTrkCharge);
   fChain->SetBranchAddress("packedCandsPseudoTrkVx", packedCandsPseudoTrkVx, &b_packedCandsPseudoTrkVx);
   fChain->SetBranchAddress("packedCandsPseudoTrkVy", packedCandsPseudoTrkVy, &b_packedCandsPseudoTrkVy);
   fChain->SetBranchAddress("packedCandsPseudoTrkVz", packedCandsPseudoTrkVz, &b_packedCandsPseudoTrkVz);
   fChain->SetBranchAddress("packedCandsPseudoTrkChi2Norm", packedCandsPseudoTrkChi2Norm, &b_packedCandsPseudoTrkChi2Norm);
   fChain->SetBranchAddress("packedCandsHighPurityTrack", packedCandsHighPurityTrack, &b_packedCandsHighPurityTrack);
   fChain->SetBranchAddress("packedCandsTrkImpactTransDist", packedCandsTrkImpactTransDist, &b_packedCandsTrkImpactTransDist);
   fChain->SetBranchAddress("packedCandsTrkImpactTransError", packedCandsTrkImpactTransError, &b_packedCandsTrkImpactTransError);
   fChain->SetBranchAddress("packedCandsTrkImpactTransSignificance", packedCandsTrkImpactTransSignificance, &b_packedCandsTrkImpactTransSignificance);
   fChain->SetBranchAddress("packedCandsTrkImpact3DDist", packedCandsTrkImpact3DDist, &b_packedCandsTrkImpact3DDist);
   fChain->SetBranchAddress("packedCandsTrkImpact3DError", packedCandsTrkImpact3DError, &b_packedCandsTrkImpact3DError);
   fChain->SetBranchAddress("packedCandsTrkImpact3DSignificance", packedCandsTrkImpact3DSignificance, &b_packedCandsTrkImpact3DSignificance);
   fChain->SetBranchAddress("numChsTrackPairs", &numChsTrackPairs, &b_numChsTrackPairs);
   fChain->SetBranchAddress("chsTkPairIndex1", chsTkPairIndex1, &b_chsTkPairIndex1);
   fChain->SetBranchAddress("chsTkPairIndex2", chsTkPairIndex2, &b_chsTkPairIndex2);
   fChain->SetBranchAddress("chsTkPairTkVtxPx", chsTkPairTkVtxPx, &b_chsTkPairTkVtxPx);
   fChain->SetBranchAddress("chsTkPairTkVtxPy", chsTkPairTkVtxPy, &b_chsTkPairTkVtxPy);
   fChain->SetBranchAddress("chsTkPairTkVtxPz", chsTkPairTkVtxPz, &b_chsTkPairTkVtxPz);
   fChain->SetBranchAddress("chsTkPairTkVtxP2", chsTkPairTkVtxP2, &b_chsTkPairTkVtxP2);
   fChain->SetBranchAddress("chsTkPairTkVx", chsTkPairTkVx, &b_chsTkPairTkVx);
   fChain->SetBranchAddress("chsTkPairTkVy", chsTkPairTkVy, &b_chsTkPairTkVy);
   fChain->SetBranchAddress("chsTkPairTkVz", chsTkPairTkVz, &b_chsTkPairTkVz);
   fChain->SetBranchAddress("chsTkPairTkVtxCov00", chsTkPairTkVtxCov00, &b_chsTkPairTkVtxCov00);
   fChain->SetBranchAddress("chsTkPairTkVtxCov01", chsTkPairTkVtxCov01, &b_chsTkPairTkVtxCov01);
   fChain->SetBranchAddress("chsTkPairTkVtxCov02", chsTkPairTkVtxCov02, &b_chsTkPairTkVtxCov02);
   fChain->SetBranchAddress("chsTkPairTkVtxCov10", chsTkPairTkVtxCov10, &b_chsTkPairTkVtxCov10);
   fChain->SetBranchAddress("chsTkPairTkVtxCov11", chsTkPairTkVtxCov11, &b_chsTkPairTkVtxCov11);
   fChain->SetBranchAddress("chsTkPairTkVtxCov12", chsTkPairTkVtxCov12, &b_chsTkPairTkVtxCov12);
   fChain->SetBranchAddress("chsTkPairTkVtxCov20", chsTkPairTkVtxCov20, &b_chsTkPairTkVtxCov20);
   fChain->SetBranchAddress("chsTkPairTkVtxCov21", chsTkPairTkVtxCov21, &b_chsTkPairTkVtxCov21);
   fChain->SetBranchAddress("chsTkPairTkVtxCov22", chsTkPairTkVtxCov22, &b_chsTkPairTkVtxCov22);
   fChain->SetBranchAddress("chsTkPairTkVtxChi2", chsTkPairTkVtxChi2, &b_chsTkPairTkVtxChi2);
   fChain->SetBranchAddress("chsTkPairTkVtxNdof", chsTkPairTkVtxNdof, &b_chsTkPairTkVtxNdof);
   fChain->SetBranchAddress("chsTkPairTkVtxTime", chsTkPairTkVtxTime, &b_chsTkPairTkVtxTime);
   fChain->SetBranchAddress("chsTkPairTkVtxTimeError", chsTkPairTkVtxTimeError, &b_chsTkPairTkVtxTimeError);
   fChain->SetBranchAddress("chsTkPairTkVtxAngleXY", chsTkPairTkVtxAngleXY, &b_chsTkPairTkVtxAngleXY);
   fChain->SetBranchAddress("chsTkPairTkVtxDistMagXY", chsTkPairTkVtxDistMagXY, &b_chsTkPairTkVtxDistMagXY);
   fChain->SetBranchAddress("chsTkPairTkVtxDistMagXYSigma", chsTkPairTkVtxDistMagXYSigma, &b_chsTkPairTkVtxDistMagXYSigma);
   fChain->SetBranchAddress("chsTkPairTkVtxAngleXYZ", chsTkPairTkVtxAngleXYZ, &b_chsTkPairTkVtxAngleXYZ);
   fChain->SetBranchAddress("chsTkPairTkVtxDistMagXYZ", chsTkPairTkVtxDistMagXYZ, &b_chsTkPairTkVtxDistMagXYZ);
   fChain->SetBranchAddress("chsTkPairTkVtxDistMagXYZSigma", chsTkPairTkVtxDistMagXYZSigma, &b_chsTkPairTkVtxDistMagXYZSigma);
   fChain->SetBranchAddress("chsTkPairTk1Pt", chsTkPairTk1Pt, &b_chsTkPairTk1Pt);
   fChain->SetBranchAddress("chsTkPairTk1Px", chsTkPairTk1Px, &b_chsTkPairTk1Px);
   fChain->SetBranchAddress("chsTkPairTk1Py", chsTkPairTk1Py, &b_chsTkPairTk1Py);
   fChain->SetBranchAddress("chsTkPairTk1Pz", chsTkPairTk1Pz, &b_chsTkPairTk1Pz);
   fChain->SetBranchAddress("chsTkPairTk1P2", chsTkPairTk1P2, &b_chsTkPairTk1P2);
   fChain->SetBranchAddress("chsTkPairTk1Eta", chsTkPairTk1Eta, &b_chsTkPairTk1Eta);
   fChain->SetBranchAddress("chsTkPairTk1Phi", chsTkPairTk1Phi, &b_chsTkPairTk1Phi);
   fChain->SetBranchAddress("chsTkPairTk1Charge", chsTkPairTk1Charge, &b_chsTkPairTk1Charge);
   fChain->SetBranchAddress("chsTkPairTk1Chi2", chsTkPairTk1Chi2, &b_chsTkPairTk1Chi2);
   fChain->SetBranchAddress("chsTkPairTk1Ndof", chsTkPairTk1Ndof, &b_chsTkPairTk1Ndof);
   fChain->SetBranchAddress("chsTkPairTk2Pt", chsTkPairTk2Pt, &b_chsTkPairTk2Pt);
   fChain->SetBranchAddress("chsTkPairTk2Px", chsTkPairTk2Px, &b_chsTkPairTk2Px);
   fChain->SetBranchAddress("chsTkPairTk2Py", chsTkPairTk2Py, &b_chsTkPairTk2Py);
   fChain->SetBranchAddress("chsTkPairTk2Pz", chsTkPairTk2Pz, &b_chsTkPairTk2Pz);
   fChain->SetBranchAddress("chsTkPairTk2P2", chsTkPairTk2P2, &b_chsTkPairTk2P2);
   fChain->SetBranchAddress("chsTkPairTk2Eta", chsTkPairTk2Eta, &b_chsTkPairTk2Eta);
   fChain->SetBranchAddress("chsTkPairTk2Phi", chsTkPairTk2Phi, &b_chsTkPairTk2Phi);
   fChain->SetBranchAddress("chsTkPairTk2Charge", chsTkPairTk2Charge, &b_chsTkPairTk2Charge);
   fChain->SetBranchAddress("chsTkPairTk2Chi2", chsTkPairTk2Chi2, &b_chsTkPairTk2Chi2);
   fChain->SetBranchAddress("chsTkPairTk2Ndof", chsTkPairTk2Ndof, &b_chsTkPairTk2Ndof);
   fChain->SetBranchAddress("chsTkPairTkVtxDcaPreFit", chsTkPairTkVtxDcaPreFit, &b_chsTkPairTkVtxDcaPreFit);
   fChain->SetBranchAddress("isElePlusJets", &isElePlusJets, &b_isElePlusJets);
   fChain->SetBranchAddress("genPDFScale", &genPDFScale, &b_genPDFScale);
   fChain->SetBranchAddress("genPDFx1", &genPDFx1, &b_genPDFx1);
   fChain->SetBranchAddress("genPDFx2", &genPDFx2, &b_genPDFx2);
   fChain->SetBranchAddress("genPDFf1", &genPDFf1, &b_genPDFf1);
   fChain->SetBranchAddress("genPDFf2", &genPDFf2, &b_genPDFf2);
   fChain->SetBranchAddress("topPtReweight", &topPtReweight, &b_topPtReweight);
   fChain->SetBranchAddress("processId", &processId, &b_processId);
   fChain->SetBranchAddress("processPtHat", &processPtHat, &b_processPtHat);
   fChain->SetBranchAddress("processMCWeight", &processMCWeight, &b_processMCWeight);
   fChain->SetBranchAddress("beamSpotX", &beamSpotX, &b_beamSpotX);
   fChain->SetBranchAddress("beamSpotY", &beamSpotY, &b_beamSpotY);
   fChain->SetBranchAddress("beamSpotZ", &beamSpotZ, &b_beamSpotZ);
   fChain->SetBranchAddress("numPVs", &numPVs, &b_numPVs);
   fChain->SetBranchAddress("pvX", pvX, &b_pvX);
   fChain->SetBranchAddress("pvY", pvY, &b_pvY);
   fChain->SetBranchAddress("pvZ", pvZ, &b_pvZ);
   fChain->SetBranchAddress("pvCov00", pvCov00, &b_pvCov00);
   fChain->SetBranchAddress("pvCov01", pvCov01, &b_pvCov01);
   fChain->SetBranchAddress("pvCov02", pvCov02, &b_pvCov02);
   fChain->SetBranchAddress("pvCov10", pvCov10, &b_pvCov10);
   fChain->SetBranchAddress("pvCov11", pvCov11, &b_pvCov11);
   fChain->SetBranchAddress("pvCov12", pvCov12, &b_pvCov12);
   fChain->SetBranchAddress("pvCov20", pvCov20, &b_pvCov20);
   fChain->SetBranchAddress("pvCov21", pvCov21, &b_pvCov21);
   fChain->SetBranchAddress("pvCov22", pvCov22, &b_pvCov22);
   fChain->SetBranchAddress("pvRho", pvRho, &b_pvRho);
   fChain->SetBranchAddress("pvIsFake", pvIsFake, &b_pvIsFake);
   fChain->SetBranchAddress("pvNdof", pvNdof, &b_pvNdof);
   fChain->SetBranchAddress("pvChi2", pvChi2, &b_pvChi2);
   fChain->SetBranchAddress("pvNtracks", pvNtracks, &b_pvNtracks);
   fChain->SetBranchAddress("pvNtracksW05", pvNtracksW05, &b_pvNtracksW05);
   fChain->SetBranchAddress("pvTime", pvTime, &b_pvTime);
   fChain->SetBranchAddress("pvTimeError", pvTimeError, &b_pvTimeError);
   fChain->SetBranchAddress("numSVs", &numSVs, &b_numSVs);
   fChain->SetBranchAddress("svPt", svPt, &b_svPt);
   fChain->SetBranchAddress("svPx", svPx, &b_svPx);
   fChain->SetBranchAddress("svPy", svPy, &b_svPy);
   fChain->SetBranchAddress("svPz", svPz, &b_svPz);
   fChain->SetBranchAddress("svMass", svMass, &b_svMass);
   fChain->SetBranchAddress("svE", svE, &b_svE);
   fChain->SetBranchAddress("svEta", svEta, &b_svEta);
   fChain->SetBranchAddress("svTheta", svTheta, &b_svTheta);
   fChain->SetBranchAddress("svPhi", svPhi, &b_svPhi);
   fChain->SetBranchAddress("svX", svX, &b_svX);
   fChain->SetBranchAddress("svY", svY, &b_svY);
   fChain->SetBranchAddress("svZ", svZ, &b_svZ);
   fChain->SetBranchAddress("svCov00", svCov00, &b_svCov00);
   fChain->SetBranchAddress("svCov01", svCov01, &b_svCov01);
   fChain->SetBranchAddress("svCov02", svCov02, &b_svCov02);
   fChain->SetBranchAddress("svCov10", svCov10, &b_svCov10);
   fChain->SetBranchAddress("svCov11", svCov11, &b_svCov11);
   fChain->SetBranchAddress("svCov12", svCov12, &b_svCov12);
   fChain->SetBranchAddress("svCov20", svCov20, &b_svCov20);
   fChain->SetBranchAddress("svCov21", svCov21, &b_svCov21);
   fChain->SetBranchAddress("svCov22", svCov22, &b_svCov22);
   fChain->SetBranchAddress("svVertexChi2", svVertexChi2, &b_svVertexChi2);
   fChain->SetBranchAddress("svVertexNdof", svVertexNdof, &b_svVertexNdof);
   fChain->SetBranchAddress("svNtracks", svNtracks, &b_svNtracks);
   fChain->SetBranchAddress("svDist3D", svDist3D, &b_svDist3D);
   fChain->SetBranchAddress("svDist3DSig", svDist3DSig, &b_svDist3DSig);
   fChain->SetBranchAddress("svDist3DError", svDist3DError, &b_svDist3DError);
   fChain->SetBranchAddress("svDistXY", svDistXY, &b_svDistXY);
   fChain->SetBranchAddress("svDistXYSig", svDistXYSig, &b_svDistXYSig);
   fChain->SetBranchAddress("svDistXYError", svDistXYError, &b_svDistXYError);
   fChain->SetBranchAddress("svAnglePV", svAnglePV, &b_svAnglePV);
   fChain->SetBranchAddress("svIsLambda", svIsLambda, &b_svIsLambda);
   fChain->SetBranchAddress("svIsKshort", svIsKshort, &b_svIsKshort);
   fChain->SetBranchAddress("mhtPt", &mhtPt, &b_mhtPt);
   fChain->SetBranchAddress("mhtPy", &mhtPy, &b_mhtPy);
   fChain->SetBranchAddress("mhtPx", &mhtPx, &b_mhtPx);
   fChain->SetBranchAddress("mhtPhi", &mhtPhi, &b_mhtPhi);
   fChain->SetBranchAddress("mhtSumEt", &mhtSumEt, &b_mhtSumEt);
   fChain->SetBranchAddress("mhtSignif", &mhtSignif, &b_mhtSignif);
   fChain->SetBranchAddress("nTriggerBits", &nTriggerBits, &b_nTriggerBits);
   fChain->SetBranchAddress("TriggerBits", &TriggerBits, &b_TriggerBits);
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
   fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
   fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter, &b_Flag_globalTightHalo2016Filter);
   fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, &b_Flag_HBHENoiseFilter);
   fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter, &b_Flag_HBHENoiseIsoFilter);
   fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, &b_Flag_BadPFMuonFilter);
   fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, &b_Flag_BadChargedCandidateFilter);
   fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
   fChain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter, &b_Flag_ecalBadCalibFilter);
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
   fChain->SetBranchAddress("eventRun", &eventRun, &b_eventRun);
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   fChain->SetBranchAddress("eventLumiblock", &eventLumiblock, &b_eventLumiblock);
   fChain->SetBranchAddress("numVert", &numVert, &b_numVert);
   Notify();
}

Bool_t sample_test::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void sample_test::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t sample_test::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef sample_test_cxx
