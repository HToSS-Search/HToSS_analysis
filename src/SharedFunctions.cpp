#include "SharedFunctions.hpp"
#include "AnalysisEvent.hpp"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TASImage.h"
#include "TLatex.h"
#include "TMVA/Timer.h"
#include "TMVA/Config.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "config_parser.hpp"

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/program_options.hpp>
#include <boost/range/iterator_range.hpp>

#include <algorithm> 
#include <chrono> 
#include <fstream>
#include <iostream>
#include <regex>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace fs = boost::filesystem;

SharedFunctions::SharedFunctions(const bool MCTruth)
  : looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.}, invZMassCut_ {4.0}, chsMass_{0.13957018}, maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4}, scalarMassCut_{4.0}, higgsTolerence_ {10.}, useMCTruth_{MCTruth}, looseChsEta_{2.4}
  {}
  
SharedFunctions::~SharedFunctions()
{}
//Why not invoke / define "AnalysisEvent event" in constructor and access as global in all functions?
std::vector<int> SharedFunctions::getLooseMuons(const AnalysisEvent& event) {
    std::vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {
       if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] && std::abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
           if ( event.muonPF2PATPt[i] >= (muons.empty() ? looseMuonPtLeading_ : looseMuonPt_)) muons.emplace_back(i); //storing indices
        }
    }
    return muons; //safe to assume pT ordered if PF PAT collection is pT ordered -> it is.
}

std::vector<int> SharedFunctions::getChargedHadronTracks(const AnalysisEvent& event) {
    std::vector<int> chs;
    for (Int_t k = 0; k < event.numPackedCands; k++) {
				//if (std::abs(event.packedCandsEta[k])>2.4) continue;
        if (std::abs(event.packedCandsPdgId[k]) != 211) continue;
        if (event.packedCandsCharge[k] == 0 ) continue;
        if (event.packedCandsHasTrackDetails[k] != 1 ) continue;
        TLorentzVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
//      if (lVec.Pt() < 5.0) continue;
//      	if (std::abs(lVec.Eta()) >= looseChsEta_) continue;

        chs.emplace_back(k);
    }
    return chs;
}


bool SharedFunctions::getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth) {
    //for ( unsigned int i{0}; i < muons.size(); i++ ) {
    //    for ( unsigned int j{i+1}; j < muons.size(); j++ ) {
        
    for (int i=0;i<muons.size();i++) {
        for (int j=0;j<muons.size();j++) {
            if (muons[j]==muons[i]) continue; //ensure not to use same muon
            if (event.muonPF2PATPt[muons[i]] <= looseMuonPtLeading_) continue;
            if (event.muonPF2PATPt[muons[j]] <= looseMuonPt_) continue;

            if ( mcTruth && (!event.genMuonPF2PATDirectScalarAncestor[muons[i]] || !event.genMuonPF2PATDirectScalarAncestor[muons[j]]) ) continue;
            if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;

            TLorentzVector lepton1{event.muonPF2PATPX[muons[i]], event.muonPF2PATPY[muons[i]], event.muonPF2PATPZ[muons[i]], event.muonPF2PATE[muons[i]]};
            TLorentzVector lepton2{event.muonPF2PATPX[muons[j]], event.muonPF2PATPY[muons[j]], event.muonPF2PATPZ[muons[j]], event.muonPF2PATE[muons[j]]};
            double delR { lepton1.DeltaR(lepton2) };
            if ( delR < maxDileptonDeltaR_  ) {
                event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
                event.zPairIndex.second  = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
                event.zPairRelIso.first  = event.muonPF2PATComRelIsodBeta[muons[i]];
                event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];

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

bool SharedFunctions::getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool mcTruth ) {
    for (int i = 0;i < chs.size();i++) {
    //for ( unsigned int i{0}; i < chs.size(); i++ ) {
      
        //if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
        //if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        /*if ( mcTruth ) {
            if ( event.packedCandsElectronIndex[chs[i]] < 0  && event.packedCandsMuonIndex[chs[i]] < 0 && event.packedCandsJetIndex[chs[i]] < 0 ) continue;
            if ( event.packedCandsElectronIndex[chs[i]] >= 0 && event.genElePF2PATScalarAncestor[event.packedCandsElectronIndex[chs[i]]] < 1 ) continue;
            if ( event.packedCandsMuonIndex[chs[i]] >= 0     && event.genMuonPF2PATScalarAncestor[event.packedCandsMuonIndex[chs[i]]] < 1 ) continue;
            if ( event.packedCandsJetIndex[chs[i]] >= 0      && event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[chs[i]]] < 1 ) continue;
        }*/
        for (int j = 0;j < chs.size();j++) {
        //for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            //if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            //if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
            
            if (chs[j]==chs[i]) continue; //ensure not to use same track
            if ( mcTruth ) {
                if ( event.packedCandsElectronIndex[chs[j]] < 0  && event.packedCandsMuonIndex[chs[j]] < 0 && event.packedCandsJetIndex[chs[j]] < 0 ) continue;
                if ( event.packedCandsElectronIndex[chs[j]] >= 0 && event.genElePF2PATScalarAncestor[event.packedCandsElectronIndex[chs[j]]] < 1 ) continue;
                if ( event.packedCandsMuonIndex[chs[j]] >= 0     && event.genMuonPF2PATScalarAncestor[event.packedCandsMuonIndex[chs[j]]] < 1 ) continue;
                if ( event.packedCandsJetIndex[chs[j]] >= 0      && event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[chs[j]]] < 1 ) continue;
            }

            if (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] >= 0) continue;

            TLorentzVector chs1 {event.packedCandsPx[chs[i]], event.packedCandsPy[chs[i]], event.packedCandsPz[chs[i]], event.packedCandsE[chs[i]]};
            TLorentzVector chs2 {event.packedCandsPx[chs[j]], event.packedCandsPy[chs[j]], event.packedCandsPz[chs[j]], event.packedCandsE[chs[j]]};

            double pT { (chs1+chs2).Pt() };
            double delR { chs1.DeltaR(chs2) };
            double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };

            //if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ && pT >= 0. ) {
            //if ( delR < maxChsDeltaR_ && (chs1+chs2).DeltaPhi(event.zPairLeptons.first+event.zPairLeptons.second)>3 && pT >= 0. ) {
            if ( delR < maxChsDeltaR_ && std::abs((chs1+chs2).M()-(event.zPairLeptons.first+event.zPairLeptons.second).M()) < 0.8 && pT >= 0. ) {
            //if ( delR < maxChsDeltaR_ && pT >= 0. ) {
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

                //event.chsPairTrkIndex = getChsTrackPairIndex(event);

//                if ( (event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06) > 20. ) continue;

       	       	// If refit fails then reject event - all signal events	pass refit, but	QCD does not
                /*if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;

                TLorentzVector chsTrk1Refitted, chsTrk2Refitted;
                chsTrk1Refitted.SetPtEtaPhiE(event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                chsTrk2Refitted.SetPtEtaPhiE(event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                event.chsPairTrkVecRefitted.first  = chsTrk1Refitted;
                event.chsPairTrkVecRefitted.second = chsTrk2Refitted;*/

                return true;
            }
            else continue;
        }
    }
    return false;
}

int SharedFunctions::getMuonTrackPairIndex(const AnalysisEvent& event) { //needs to be checked
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] == event.zPairIndex.first && event.muonTkPairPF2PATIndex2[i] == event.zPairIndex.second) return i;
    }
    return -1;
}

int SharedFunctions::getChsTrackPairIndex(const AnalysisEvent& event) { //needs to be checked
    for (int i{0}; i < event.numChsTrackPairs; i++) {
        if (event.chsTkPairIndex1[i] == event.chsPairIndex.first && event.chsTkPairIndex2[i] == event.chsPairIndex.second) return i;
    }
    return -1;
}

bool SharedFunctions::scalarAncestry (const AnalysisEvent& event, const Int_t& k, const Int_t& ancestorId, const bool verbose = false) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };
    if (verbose) std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
    if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
    else if (motherId == std::abs(ancestorId)) return true; // if parent is ancestor, return true
    else if ((pdgId != 130) && (pdgId != 310) && (pdgId != 311) && (motherId != pdgId)) {
			return false; // if parent is not an excited state of same particle, return false except in the case of neutral kaons
		}
    else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
    else {
        
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
				
	    	//if (verbose) std::cout << "Enters right condition ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
        return scalarAncestry(event, motherIndex, ancestorId, verbose); // otherwise check mother's mother ...
    }
}

int SharedFunctions::MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max) {
    double minDR = 100;
    unsigned index = 0;
    double tmpDR;
    //std::cout<<"Stuck at index:"<<gen_ind<<std::endl;
    for (int j=0; j<event.numPackedCands; j++) {
        //TLorentzVector gen_mu{event.genMuonPF2PATPX[j], event.genMuonPF2PATPY[j], event.genMuonPF2PATPZ[j], event.genMuonPF2PATE[j]};
        //TLorentzVector reco_mu{event.muonPF2PATPX[j], event.muonPF2PATPY[j], event.muonPF2PATPZ[j], event.muonPF2PATE[j]};
        //tmpDR=gen_mu.DeltaR(reco_mu);
        TLorentzVector packedCand {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
        //TLorentzVector Chs {event.genPar[gen_ind], event.packedCandsPy[gen_ind], event.packedCandsPz[gen_ind], event.packedCandsE[gen_ind]};
        tmpDR=deltaR(event.genParEta[gen_ind],event.genParPhi[gen_ind],packedCand.Eta(),packedCand.Phi());
        //std::cout<<"GenIndex, Pt, Charge, ID: "<<gen_ind<<", "<<event.genParPt[gen_ind]<<", "<<event.genParCharge[gen_ind]<<", "<<event.genParId[gen_ind]<<";"<<std::endl;
        //std::cout<<"PCdIndex, Pt, Charge, ID: "<<j<<", "<<packedCand.Pt()<<",  "<<event.packedCandsCharge[j]<<", "<<event.packedCandsPdgId[j]<<";"<<std::endl;
        //std::cout<<"Check the dR on this: "<<tmpDR<<std::endl;
        //std::cout<<"GenIndex: "<<gen_ind<<", PackedCandIndex: "<<j<<std::endl;
        if (fabs(event.packedCandsPdgId[j])!=211) continue; //must be hadronic track
        if (event.genParCharge[gen_ind]!= event.packedCandsCharge[j]) continue; //must have same charge
        if (minDR < tmpDR) continue;
        
        minDR = tmpDR;
        index = j;    
    }
    if (minDR < dr_max) 
        return index;
    else
        return -99;
}

int SharedFunctions::nTrksInCone(const AnalysisEvent& event, const TLorentzVector& particle, const Int_t particle_ch, int trkPdgId,double dr_max, bool loose=true) {
    int count = 0;
    double tmpdR=999.;
    for (int i=0;i < event.numPackedCands; i++) {
        TLorentzVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        if (loose) {
          if (fabs(event.packedCandsPdgId[i]) != trkPdgId) continue;
          if ((event.packedCandsCharge[i] != particle_ch)) continue;
        }
        else {
          if (trk.Pt() < 0.5) continue;
          if ((fabs(event.packedCandsPdgId[i]) != trkPdgId) || (event.packedCandsFromPV[i] < 2)) continue; //hadronic and not PU
          if ((event.packedCandsCharge[i] != particle_ch)) continue;
        }
        tmpdR = particle.DeltaR(trk);
        if ((tmpdR < dr_max)) count++;// include 0.03 cut to ensure not the same track as candidate
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        
    }
    return count;
}

int SharedFunctions::nGenParsInCone(const AnalysisEvent& event, const TLorentzVector& particle, const Int_t particle_ch, int ParPdgId, double dr_max) {
    int count = 0;
    double tmpdR=999.;
    for (int i = 0; i < event.nGenPar; i++) {
        int pid { std::abs(event.genParId[i]) };
        int motherId { std::abs(event.genParMotherId[i]) };
        int status {event.genParStatus[i]};
        TLorentzVector GenPar;
        GenPar.SetPtEtaPhiE(event.genParPt[i], event.genParEta[i], event.genParPhi[i], event.genParE[i]);
        if (fabs(event.packedCandsPdgId[i]) != ParPdgId) continue;
        if (GenPar.Pt() > 0.5) continue;
        
        tmpdR = particle.DeltaR(GenPar);
        if ((tmpdR < dr_max)) count++;// include 0.03 cut to ensure not the same track as candidate
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        
    }
    return count;
}

bool SharedFunctions::GenLevelCheck(const AnalysisEvent& event, const bool verbose = false) {
	std::vector<int> genMuonIndex;
  std::vector<int> genMuonSortedIndex;
  std::vector<int> genPionIndex;
  std::vector<int> genKaonIndex;
  std::vector<int> genChargedKaonIndex;
  std::vector<int> genKshortIndex;
  std::vector<int> genKlongIndex;
  std::vector<int> genChsIndex;
  // gen particle loop -> only check final state particles!
  for ( Int_t k = 0; k < event.nGenPar; k++ ) {
  	const int pid { std::abs(event.genParId[k]) };
    const int motherId { std::abs(event.genParMotherId[k]) };
    const int status {event.genParStatus[k]};
    //bool verbose = false;
                
    if (verbose) {
      std::cout<<"Starting the ladder for particle (id, idx, status) - "<<pid<<", "<<k<<", "<<status<<std::endl;
                  
    }
                
    //if (pid == motherId) std::cout<<"Interesting event! Event no. "<<i<<std::endl;
    /*if (status != 1) {
    	//std::cout<<"Accessing non-final state particle!"<<std::endl;
      continue;
  	}*/ // this is an addition?
    if ((status != 1)&&(verbose)) {
    	std::cout<<"Non-final state particle (id, status, motherId, daughterId1, daughterId2) - "<<event.genParId[k]<<", "<<status<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<std::endl;
    }
		if (((pid == 130)||(pid==310)||(pid==311))&&(verbose)) {
     	std::cout<<"Kaons (id, status, motherId, daughterId1, daughterId2) - "<<event.genParId[k]<<", "<<status<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<std::endl;
    }
    const bool hasScalarAncestry {scalarAncestry(event, k, 9000006, verbose)}; //checks if at any point, it comes from scalar

    if ( pid == 13 && status == 1 && hasScalarAncestry) {
    	genMuonIndex.emplace_back(k);
    }
    //else if ( pid == 211 && (motherId < 6 || motherId == 21 || motherId == 9000006 ) ) { //checking if pion is coming from quarks, or gluon or scalar //most pions coming from meson decays though
    else if ( pid == 211 && status == 1) {
    //if ( hasScalarAncestry && motherId == 9000006 ) genPionIndex.emplace_back(k); //checking stricter criteria, just to see if pt dist changes; motherId not originally there
    	if ( hasScalarAncestry ) genPionIndex.emplace_back(k); //low stats for pions comparitively
      if ( hasScalarAncestry ) genChsIndex.emplace_back(k);
  	}
    else if ( pid == 321 && status == 1) {
    	if ( hasScalarAncestry ) {
      	genChargedKaonIndex.emplace_back(k);
        //genKaonIndex.emplace_back(k);
        genChsIndex.emplace_back(k);
      }
    }
    else if ( pid == 130) {
    	if ( hasScalarAncestry ) {
      genKlongIndex.emplace_back(k);
      //genKaonIndex.emplace_back(k);
      //genChsIndex.emplace_back(k);
    	}
    }
    else if ( pid == 310) {
      if ( hasScalarAncestry ) {
      	genKshortIndex.emplace_back(k);
        //genKaonIndex.emplace_back(k);
        //genChsIndex.emplace_back(k);
      }
    }
    else if ( pid == 311 ) {
    	if ( hasScalarAncestry ) {
      	genKaonIndex.emplace_back(k);
        //genKaonIndex.emplace_back(k);
        //genChsIndex.emplace_back(k);
      }
    }
  }	
	if (verbose) std::cout<<"GenLevelCheck (muons, hadrons) - "<<genMuonIndex.size()<<", "<<genChsIndex.size()<<std::endl;
	if (( genChsIndex.size() < 2 )||(genMuonIndex.size() < 2)) return false;
	return true;

}

float SharedFunctions::deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}

