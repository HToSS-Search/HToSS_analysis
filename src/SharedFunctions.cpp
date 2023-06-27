#include "AnalysisEvent.hpp"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
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
#include <Math/Vector4D.h>

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
#include "SharedFunctions.hpp"

namespace fs = boost::filesystem;

SharedFunctions::SharedFunctions(const bool MCTruth = false, const bool debug = false, std::map<std::string, TH1F*> *hists_1d_ref_ = 0, std::map<std::string, TH2F*> *hists_2d_ref_ = 0)
  : looseMuonEta_{2.4},
    looseMuonPt_{5.},
    looseMuonPtLeading_{30.},
    looseMuonRelIsoLeading_{100.},
    looseMuonRelIso_{100.},
    invZMassCut_{4.},
    chsMass_{0.13957061},
    looseChsEta_{2.4},
    looseChsPt_{5},
    looseChsSumPtCh_{500},
    looseChsSumPtChLeading_{500},
    maxDileptonDeltaR_{0.4},
    maxChsDeltaR_{0.4},
    scalarMassCut_{4.0},
    higgsTolerence_{10.0},
    diChsPt_{0.},
    diMuonPt_{0.},
    diMuonOppCharge_{"OS"},
    diChsOppCharge_{"OS"},
    higgsPeakLow_{125},
    higgsPeakHigh_{125},
    higgsPeakSideband_{2.5},
    SR_{false},
    verbose_{debug}
  {

    hists_1d_ = hists_1d_ref_;
    hists_2d_ = hists_2d_ref_;
        // func_lowerbound_ = new TF1("func_lowerbound","[1]*x+[0]",0.,10.);
    // func_higherbound_ = new TF1("func_higherbound","[1]*x+[0]",0.,10.);
  }
  
SharedFunctions::~SharedFunctions()
{}
//Why not invoke / define "AnalysisEvent event" in constructor and access as global in all functions?
std::vector<int> SharedFunctions::getLooseMuons(const AnalysisEvent& event, bool flag = true) {
  std::vector<int> muons;
  for (int i{0}; i < event.numMuonPF2PAT; i++)  {
    if (std::abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
      if (verbose_) {
        std::cout<<"in getLooseMuons(index,flag,pt,eta,PFMuon,LooseCutId):"<<i<<","<<flag<<","<<event.muonPF2PATPt[i]<<","<<event.muonPF2PATEta[i]<<","<<event.muonPF2PATIsPFMuon[i]<<","<<event.muonPF2PATLooseCutId[i]<<std::endl;
      }
      if (flag) {
        if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i]) {
          if ( event.muonPF2PATPt[i] >= (muons.empty() ? looseMuonPtLeading_ : looseMuonPt_)) muons.emplace_back(i); //storing indices
        }
      }
      else muons.emplace_back(i); //storing indices
    }
  }
  return muons; //safe to assume pT ordered if PF PAT collection is pT ordered -> it is.
}

std::vector<int> SharedFunctions::getChargedHadronTracks(const AnalysisEvent& event, bool flag = true) {
    std::vector<int> chs;
    // bool flag = false;
    for (Int_t k = 0; k < event.numPackedCands; k++) {
				//if (std::abs(event.packedCandsEta[k])>2.4) continue;
        if (std::abs(event.packedCandsPdgId[k]) != 211) continue;
        if (event.packedCandsCharge[k] == 0 ) continue;
        if (event.packedCandsHasTrackDetails[k] != 1 ) continue;
        ROOT::Math::PxPyPzMVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], chsMass_};
        if (lVec.Pt() < 0.5) continue;
        if (std::abs(lVec.Eta()) > looseChsEta_) continue;
        if (flag) {
          // if ( std::abs(lVec.Pt()) >= (chs.empty() ? looseChsPtLeading_ : looseChsPt_)) chs.emplace_back(k);
          if ( std::abs(lVec.Pt()) < looseChsPt_) continue;
        }
        chs.emplace_back(k);
        // if ( std::abs(lVec.Pt()) >= looseChsPtLeading_) flag=true;
    }
    // if (!flag) chs.erase(chs.begin()+1,chs.end());
    return chs;
}


bool SharedFunctions::getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool flag = true) {
    if (verbose_) {
      std::cout<<"See the muon indices";
      for (int i=0;i<muons.size();i++) {
        std::cout<<i<<","<<muons[i]<<",";
      }
      std::cout<<";"<<std::endl;
    }
    for (int i=0;i<muons.size();i++) {
        for (int j=i+1;j<muons.size();j++) { //it is Et sorted array
            if (muons[j]==muons[i]) continue; //ensure not to use same muon

            if (diMuonOppCharge_) {
              if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;
            }
            else { //only letting SS pass
              if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] <= 0) continue;
            }
            if (verbose_) {
              std::cout<<"muon indices:"<<muons[i]<<","<<muons[j]<<std::endl;
              std::cout<<"i,j:"<<muons[i]<<","<<muons[j]<<std::endl;
            }
            ROOT::Math::PxPyPzEVector lepton1{event.muonPF2PATPX[muons[i]], event.muonPF2PATPY[muons[i]], event.muonPF2PATPZ[muons[i]], event.muonPF2PATE[muons[i]]};
            ROOT::Math::PxPyPzEVector lepton2{event.muonPF2PATPX[muons[j]], event.muonPF2PATPY[muons[j]], event.muonPF2PATPZ[muons[j]], event.muonPF2PATE[muons[j]]};
            double pT { (lepton1+lepton2).Pt() };
            double delR = ROOT::Math::VectorUtil::DeltaR(lepton1, lepton2);
            if (flag) {
              if (event.muonPF2PATPt[muons[i]] <= looseMuonPtLeading_) continue;
              if (event.muonPF2PATPt[muons[j]] <= looseMuonPt_) continue;
              if ( delR > maxDileptonDeltaR_ ) continue;
              // if (pT < diMuonPt_) continue;
            }

            struct Isolation leadingMuonIso, subleadingMuonIso;
            int leadingidx = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
            int subleadingidx = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
            
            leadingMuonIso = PFIsolation("muon",leadingidx, -99, subleadingidx, event, 0.4);
            subleadingMuonIso = PFIsolation("muon",subleadingidx, -99, leadingidx, event, 0.4);

            // std::cout<<"Pointer smh:"<<*hists_1d_<<std::endl;
            (*hists_1d_)["h_leadingMuon_RelIso_BC"]->Fill(leadingMuonIso.reliso_,event.eventWeight);
            (*hists_1d_)["h_subleadingMuon_RelIso_BC"]->Fill(subleadingMuonIso.reliso_,event.eventWeight);
            if (flag) {
              if ( leadingMuonIso.reliso_ > looseMuonRelIsoLeading_ ) continue;
              if ( subleadingMuonIso.reliso_ > looseMuonRelIso_ ) continue;
            }

            event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
            event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
            event.zPairIndex.first = leadingidx;
            event.zPairIndex.second  = subleadingidx;

            event.zPairRelIso.first = leadingMuonIso.reliso_;
            event.zPairChIso.first = leadingMuonIso.chiso_;
            event.zPairNhIso.first = leadingMuonIso.nhiso_;
            event.zPairPhIso.first = leadingMuonIso.phiso_;
            event.zPairPuIso.first = leadingMuonIso.puiso_;
            
            event.zPairRelIso.second = subleadingMuonIso.reliso_;
            event.zPairChIso.second = subleadingMuonIso.chiso_;
            event.zPairNhIso.second = subleadingMuonIso.nhiso_;
            event.zPairPhIso.second = subleadingMuonIso.phiso_;
            event.zPairPuIso.second = subleadingMuonIso.puiso_;
            // event.zPairRelIso.first  = event.muonPF2PATComRelIsodBeta[muons[i]];
            // event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];
            
            if (verbose_) {
              std::cout<<"Check out the muons!"<<std::endl;
              std::cout<<"Leading muon (idx, pT, eta, phi, isolation):"<<event.zPairIndex.first<<","<<event.zPairLeptons.first.Pt()<<","<<event.zPairLeptons.first.Eta()<<","<<event.zPairLeptons.first.Phi()<<","<<event.zPairRelIso.first<<std::endl;
              std::cout<<"subleading muon (idx, pT, eta, phi, isolation):"<<event.zPairIndex.second<<","<<event.zPairLeptons.second.Pt()<<","<<event.zPairLeptons.second.Eta()<<","<<event.zPairLeptons.second.Phi()<<","<<event.zPairRelIso.second<<std::endl;
            }
            if (!getMuonTrackPairIndex(event)) return false; //checking if refitted tracks are present -> necessary for SV construction
            return true;
        }
    }
    return false;
}

bool SharedFunctions::getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool flag = true) {
    for (int i = 0;i < chs.size();i++) {
        for (int j = i+1;j < chs.size();j++) { //since it is not an ordered pair
        //for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            //if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            //if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
            
            if (chs[j]==chs[i]) continue; //ensure not to use same track
            if (diChsOppCharge_) {
              if (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] >= 0) continue;
            }
            else { //only letting SS pass
              if (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] <= 0) continue;
            }

            // if ( useMCTruth_ ) {
            //     if ( event.packedCandsElectronIndex[chs[j]] < 0  && event.packedCandsMuonIndex[chs[j]] < 0 && event.packedCandsJetIndex[chs[j]] < 0 ) continue;
            //     if ( event.packedCandsElectronIndex[chs[j]] >= 0 && event.genElePF2PATScalarAncestor[event.packedCandsElectronIndex[chs[j]]] < 1 ) continue;
            //     if ( event.packedCandsMuonIndex[chs[j]] >= 0     && event.genMuonPF2PATScalarAncestor[event.packedCandsMuonIndex[chs[j]]] < 1 ) continue;
            //     if ( event.packedCandsJetIndex[chs[j]] >= 0      && event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[chs[j]]] < 1 ) continue;
            // }

            
            ROOT::Math::PxPyPzMVector chs1 {event.packedCandsPx[chs[i]], event.packedCandsPy[chs[i]], event.packedCandsPz[chs[i]], chsMass_};
            ROOT::Math::PxPyPzMVector chs2 {event.packedCandsPx[chs[j]], event.packedCandsPy[chs[j]], event.packedCandsPz[chs[j]], chsMass_};

            double pT { (chs1+chs2).Pt() };
            double delR = ROOT::Math::VectorUtil::DeltaR(chs1, chs2);
            // double delR { chs1.DeltaR(chs2) };
            // double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };

            // std::vector<int> notrkID;

            // notrkID.push_back(11);notrkID.push_back(13);
            // float tmp_dr_max = 0.6;
            // int emtrks_chs1 = NoTrksInCone(event, chs1, notrkID,tmp_dr_max,false);
            // int emtrks_chs2 = NoTrksInCone(event, chs2, notrkID,tmp_dr_max,false);
            // if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ && pT >= 0. ) {
            //if ( delR < maxChsDeltaR_ && (chs1+chs2).DeltaPhi(event.zPairLeptons.first+event.zPairLeptons.second)>3 && pT >= 0. ) {
            //if ( delR < maxChsDeltaR_ && std::abs((chs1+chs2).M()-(event.zPairLeptons.first+event.zPairLeptons.second).M()) < 0.8 && pT >= 0. ) {
            // if ( delR < maxChsDeltaR_ && pT >= 0. ) {
            // if ( delR < maxChsDeltaR_ && pT >= diChsPt_ ) {
            // if ( delR < maxChsDeltaR_ ) {
            if (flag) {
              if ( delR > maxChsDeltaR_ ) continue;
              if (pT < diChsPt_) continue;
            }
            struct Isolation leadingChsIso, subleadingChsIso;
            int leadingidx = chs1.Pt() > chs2.Pt() ? chs[i] : chs[j];
            int subleadingidx = chs1.Pt() > chs2.Pt() ? chs[j] : chs[i];
            leadingChsIso = PFIsolation("hadron",leadingidx, -99, subleadingidx, event, 0.4);
            subleadingChsIso = PFIsolation("hadron",subleadingidx,-99, leadingidx, event, 0.4);
            if (flag) {
              if ( leadingChsIso.chiso_ > looseChsSumPtChLeading_ ) continue;
              if ( subleadingChsIso.chiso_ > looseChsSumPtCh_ ) continue;
            }
            event.chsPairVec.first  = chs1.Pt() > chs2.Pt() ? chs1 : chs2;
            event.chsPairVec.second = chs1.Pt() > chs2.Pt() ? chs2 : chs1;
            event.chsPairIndex.first = leadingidx;
            event.chsPairIndex.second = subleadingidx;

            event.chsPairRelIso.first = leadingChsIso.reliso_;
            event.chsPairChIso.first = leadingChsIso.chiso_;
            event.chsPairNhIso.first = leadingChsIso.nhiso_;
            event.chsPairPhIso.first = leadingChsIso.phiso_;
            event.chsPairPuIso.first = leadingChsIso.puiso_;

            event.chsPairRelIso.second = subleadingChsIso.reliso_;
            event.chsPairChIso.second = subleadingChsIso.chiso_;
            event.chsPairNhIso.second = subleadingChsIso.nhiso_;
            event.chsPairPhIso.second = subleadingChsIso.phiso_;
            event.chsPairPuIso.second = subleadingChsIso.puiso_;
            // TLorentzVector chsTrk1, chsTrk2;
            ROOT::Math::PtEtaPhiMVector chsTrk1 {event.packedCandsPseudoTrkPt[event.chsPairIndex.first], event.packedCandsPseudoTrkEta[event.chsPairIndex.first], event.packedCandsPseudoTrkPhi[event.chsPairIndex.first], chsMass_};
            ROOT::Math::PtEtaPhiMVector chsTrk2 {event.packedCandsPseudoTrkPt[event.chsPairIndex.second], event.packedCandsPseudoTrkEta[event.chsPairIndex.second], event.packedCandsPseudoTrkPhi[event.chsPairIndex.second], chsMass_};

            event.chsPairTrkVec.first  = chsTrk1;
            event.chsPairTrkVec.second = chsTrk2;


            if (verbose_) {
              std::cout<<"Check out the hadrons!"<<std::endl;
              std::cout<<"Leading hadron (pT, eta, phi, isolation):"<<event.chsPairVec.first.Pt()<<","<<event.chsPairVec.first.Eta()<<","<<event.chsPairVec.first.Phi()<<","<<event.chsPairRelIso.first<<std::endl;
              std::cout<<"subleading hadron (pT, eta, phi, isolation):"<<event.chsPairVec.second.Pt()<<","<<event.chsPairVec.second.Eta()<<","<<event.chsPairVec.second.Phi()<<","<<event.chsPairRelIso.second<<std::endl;
            }
            // If refit fails then reject event - all signal events	pass refit, but	QCD does not -> check this
            if (!getChsTrackPairIndex(event)) return false; //checking if refitted tracks are present -> necessary for SV construction
            return true;
        }
    }
    return false;
}

bool SharedFunctions::DiHadronPtCut(AnalysisEvent& event) {
  ROOT::Math::PxPyPzMVector  diChs {event.chsPairVec.first+event.chsPairVec.second};
  if (diChs.Pt() >= diChsPt_) return true;
  else return false;
}

bool SharedFunctions::MassCompatibility(AnalysisEvent& event, TF1& f_lower, TF1& f_higher) {
  double dihadron_mass = (event.chsPairVec.first + event.chsPairVec.second).M();
  double dimuon_mass = (event.zPairLeptons.first + event.zPairLeptons.second).M();
  double mass_upper = f_higher.Eval(dimuon_mass);
  double mass_lower = f_lower.Eval(dimuon_mass);
  if (verbose_) std::cout<<"Look here(dimu, dihad, low, up):"<<dimuon_mass<<","<<dihadron_mass<<","<<mass_lower<<","<<mass_upper<<std::endl;
  if ((dihadron_mass>=mass_lower) && (dihadron_mass<=mass_upper)) return true;
  else return false;
}

bool SharedFunctions::HiggsWindow(AnalysisEvent& event) {
  double higgsMass {(event.chsPairVec.first+event.chsPairVec.second+event.zPairLeptons.first+event.zPairLeptons.second).M()};
  // if ((std::abs(higgsMass - 125.35) > 2)  && (std::abs(higgsMass - 125.35) < 4)) return true;
  if (SR_) {
    if ((higgsMass >= higgsPeakLow_) && (higgsMass <= higgsPeakHigh_)) return true;
    else return false;
  }
  else {
    // std::cout<<"BLINDED HIGGS PEAK"<<std::endl;
    if ((higgsMass > higgsPeakLow_-higgsPeakSideband_) && (higgsMass < higgsPeakLow_)) return true;
    else if ((higgsMass > higgsPeakHigh_) && (higgsMass < higgsPeakHigh_+higgsPeakSideband_)) return true;
    else return false;
  }
}

bool SharedFunctions::getMuonTrackPairIndex(AnalysisEvent& event) { 
    int TkPairIdx = -1;
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] == event.zPairIndex.first && event.muonTkPairPF2PATIndex2[i] == event.zPairIndex.second) {
          TkPairIdx = i;
          break;
        }
    }
    event.mumuTrkIndex = TkPairIdx;

    // if ( (event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxNdof[event.mumuTrkIndex]+1.0e-06) > 10. ) continue;

    if ( std::isnan(event.muonTkPairPF2PATTk1Pt[event.mumuTrkIndex])  || std::isnan(event.muonTkPairPF2PATTk2Pt[event.mumuTrkIndex]) ) return false;
    if ( std::isnan(event.muonTkPairPF2PATTk1P2[event.mumuTrkIndex])  || std::isnan(event.muonTkPairPF2PATTk2P2[event.mumuTrkIndex]) ) return false;
    if ( std::isnan(event.muonTkPairPF2PATTk1Phi[event.mumuTrkIndex]) || std::isnan(event.muonTkPairPF2PATTk2Phi[event.mumuTrkIndex]) ) return false;

    event.zPairLeptonsRefitted.first  = ROOT::Math::PxPyPzEVector{event.muonTkPairPF2PATTk1Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk1P2[event.mumuTrkIndex]+std::pow(0.1057,2))};
    event.zPairLeptonsRefitted.second = ROOT::Math::PxPyPzEVector{event.muonTkPairPF2PATTk2Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk2P2[event.mumuTrkIndex]+std::pow(0.1057,2))};
    return true;
}

bool SharedFunctions::getChsTrackPairIndex(AnalysisEvent& event) { 
    int TkPairIdx = -1;
    for (int i{0}; i < event.numChsTrackPairs; i++) {
        if (event.chsTkPairIndex1[i] == event.chsPairIndex.first && event.chsTkPairIndex2[i] == event.chsPairIndex.second) {
          TkPairIdx = i;
          break;
        }
    }
    event.chsPairTrkIndex = TkPairIdx;

    // if ( (event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06) > 20. ) continue;
    // If refit fails then reject event - all signal events	pass refit, but	QCD does not
    if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) return false;
    if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
    if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;

    ROOT::Math::PxPyPzEVector chsTrk1Refitted {event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2))};
    ROOT::Math::PxPyPzEVector chsTrk2Refitted {event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2))};
    event.chsPairTrkVecRefitted.first  = chsTrk1Refitted;
    event.chsPairTrkVecRefitted.second = chsTrk2Refitted;
    return true;
}

bool SharedFunctions::scalarAncestry (const AnalysisEvent& event, const Int_t& k, const Int_t& ancestorId, const bool verbose = false) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };
    // if (verbose) std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
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
        ROOT::Math::PxPyPzEVector packedCand {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
        //TLorentzVector Chs {event.genPar[gen_ind], event.packedCandsPy[gen_ind], event.packedCandsPz[gen_ind], event.packedCandsE[gen_ind]};
        tmpDR=deltaR(event.genParEta[gen_ind],event.genParPhi[gen_ind],packedCand.Eta(),packedCand.Phi());
        //std::cout<<"GenIndex, Pt, Charge, ID: "<<gen_ind<<", "<<event.genParPt[gen_ind]<<", "<<event.genParCharge[gen_ind]<<", "<<event.genParId[gen_ind]<<";"<<std::endl;
        //std::cout<<"PCdIndex, Pt, Charge, ID: "<<j<<", "<<packedCand.Pt()<<",  "<<event.packedCandsCharge[j]<<", "<<event.packedCandsPdgId[j]<<";"<<std::endl;
        //std::cout<<"Check the dR on this: "<<tmpDR<<std::endl;
        //std::cout<<"GenIndex: "<<gen_ind<<", PackedCandIndex: "<<j<<std::endl;
        //if (fabs(event.packedCandsPdgId[j])!=211) continue; //must be hadronic track
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

int SharedFunctions::nTrksInCone(const AnalysisEvent& event,
                                 const ROOT::Math::PxPyPzMVector& particle,
                                 const Int_t particle_ch,
                                 int trkPdgId,
                                 double dr_max,
                                 bool loose = true, bool selective=true)
{
    int count = 0;
    double tmpdR=999.;
    for (int i=0;i < event.numPackedCands; i++) {
        ROOT::Math::PxPyPzMVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], chsMass_};
        if ((selective) && (fabs(event.packedCandsPdgId[i])) != trkPdgId) continue;
        if ((std::abs(event.packedCandsCharge[i]) != particle_ch)) continue;
        if (trk.Pt() < 0.5) continue;
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,trk);
        // tmpdR = particle.DeltaR(trk);
        if ((tmpdR < dr_max) && (tmpdR > 0.03)) count++;// include 0.03 cut to ensure not the same track as candidate
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        
    }
    return count;
}

int SharedFunctions::NoTrksInCone(const AnalysisEvent& event,
                                 const ROOT::Math::PxPyPzMVector& particle,
                                 std::vector<int>& trkPdgId,
                                 double dr_max, bool verboseFlag=false)
{
    if (verboseFlag) std::cout<<"DEBUG HERE"<<std::endl;
    int count = 0;
    double tmpdR=999.;
    int flag;
    for (int i=0;i < event.numPackedCands; i++) {
        ROOT::Math::PxPyPzMVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], chsMass_};
        flag=0;
        for (int k:trkPdgId) {
          if (verboseFlag) std::cout<<"Track ID within second loop, k, flag - "<<fabs(event.packedCandsPdgId[i])<<","<<k<<","<<flag<<std::endl;
          if (fabs(event.packedCandsPdgId[i]) == k) flag=1;
        }
        if (!flag) continue;
        
        if (trk.Pt() < 0.5) continue;
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,trk);
        // tmpdR = particle.DeltaR(trk);
        if (verboseFlag) std::cout<<"Track ID, dR - "<<fabs(event.packedCandsPdgId[i])<<","<<tmpdR<<std::endl;
        if ((tmpdR < dr_max)) ++count;// include 0.03 cut to ensure not the same track as candidate
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
    }
    return count;
}

int SharedFunctions::nGenParsInCone(const AnalysisEvent& event, const ROOT::Math::PxPyPzMVector& particle, const Int_t particle_ch, int ParPdgId, double dr_max, bool selective=true) {
    int count = 0;
    double tmpdR=999.;
    for (int i = 0; i < event.nGenPar; i++) {
        int pid { std::abs(event.genParId[i]) };
        int motherId { std::abs(event.genParMotherId[i]) };
        int status {event.genParStatus[i]};
        ROOT::Math::PtEtaPhiEVector GenPar {event.genParPt[i], event.genParEta[i], event.genParPhi[i], event.genParE[i]};
        if ((selective) && (fabs(event.packedCandsPdgId[i]) != ParPdgId)) continue;
        if (GenPar.Pt() < 0.5) continue;
        if (status != 1) continue;
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,GenPar);
        // tmpdR = particle.DeltaR(GenPar);
        if ((tmpdR < dr_max) && (tmpdR > 0.03)) count++;// include 0.03 cut to ensure not the same track as candidate
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
    else if ( pid == 211 && status == 1 && std::abs(event.genParEta[k])<looseChsEta_) {
    //if ( hasScalarAncestry && motherId == 9000006 ) genPionIndex.emplace_back(k); //checking stricter criteria, just to see if pt dist changes; motherId not originally there
    	if ( hasScalarAncestry ) genPionIndex.emplace_back(k); //low stats for pions comparitively
      if ( hasScalarAncestry ) genChsIndex.emplace_back(k);
  	}
    else if ( pid == 321 && status == 1 && std::abs(event.genParEta[k])<looseChsEta_) {
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

Isolation SharedFunctions::PFIsolation(TString ptype,int trk_ind, int gen_ind, int trk_exc,  const AnalysisEvent& event, double dr_max = 0.4) {
  float nh_iso {0.0};
  float ph_iso {0.0};
  float ch_iso {0.0};
  float pu_iso {0.0};
  float dr_trk_packedCandVec {99.};
  int count = 0;
  ROOT::Math::PxPyPzMVector Trk;
  if (ptype.Contains("muon")) 
    Trk.SetCoordinates(event.muonPF2PATPX[trk_ind], event.muonPF2PATPY[trk_ind], event.muonPF2PATPZ[trk_ind], 0.1056583745); //NECESSARY TO USE THE SAME LORENTZ VECTOR TYPE AND GIVE SAME QUANTITIES AS IN DEFINITION
    // Trk.SetPxPyPzE;
  else
    Trk.SetCoordinates(event.packedCandsPx[trk_ind],event.packedCandsPy[trk_ind],event.packedCandsPz[trk_ind],chsMass_);
    // Trk.SetPx(event.packedCandsPx[trk_ind]);Trk.SetPy(event.packedCandsPy[trk_ind]);Trk.SetPz(event.packedCandsPz[trk_ind]);Trk.SetM(chsMass_);
  if (verbose_) {
      std::cout<<"Check the PFCandidates for isolation!"<<std::endl;
      std::cout<<"Reco primary trk (type,index,pt,eta,phi):"<<ptype<<","<<trk_ind<<","<<Trk.Pt()<<","<<Trk.Eta()<<","<<Trk.Phi()<<std::endl;
    }
  for (int k = 0; k < event.numPackedCands; k++) {
    
    if (ptype.Contains("hadron")) {
      if ( k == trk_ind || k == trk_exc ) continue;
    }
    

    // exclude tracks from HF above? 
    
    ROOT::Math::PxPyPzEVector packedCandVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
    ROOT::Math::PxPyPzEVector packedCandTrkVec {event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]};
    // packedCandTrkVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);
    if (packedCandVec.Pt()<0.5) continue;
    dr_trk_packedCandVec = ROOT::Math::VectorUtil::DeltaR(Trk, packedCandVec);
    if ((verbose_) && (dr_trk_packedCandVec < dr_max)) {
      std::cout<<"Nearby trk (index,pt,eta,phi,pdg):"<<k<<","<<packedCandVec.Pt()<<","<<packedCandVec.Eta()<<","<<packedCandVec.Phi()<<","<<event.packedCandsPdgId[k]<<std::endl;
    }
    if ( std::abs(event.packedCandsPdgId[k]) == 11 ||  std::abs(event.packedCandsPdgId[k]) == 13) continue; //rejecting muon/ele tracks
    //const TLorentzVector Trk_avoid {event.packedCandsPx[trk_exc], event.packedCandsPy[trk_exc], event.packedCandsPz[trk_exc], event.packedCandsE[trk_exc]};
    if ( event.packedCandsCharge[k] == 0 ) {
      if ( dr_trk_packedCandVec < dr_max ) { 
        if (event.packedCandsPdgId[k] == 22) ph_iso += packedCandVec.Et();
        else nh_iso += packedCandVec.Et();
      }
    }
    else {
      if ( event.packedCandsFromPV[k] >= 2 ) {
        if ( dr_trk_packedCandVec < dr_max )  ch_iso += packedCandVec.Pt();
      }
      else {
        if ( dr_trk_packedCandVec  < dr_max )  pu_iso += packedCandVec.Pt();
      }
    }
  }
  struct Isolation pfiso;
  float iso  = ch_iso  + std::max( float(0.0), nh_iso+ph_iso - float(0.5*pu_iso)  );
  float RelIso;
  if (ptype.Contains("gen"))
    RelIso = iso/(event.genParPt[gen_ind]+1.0e-06);
  else
    RelIso = iso/(Trk.Pt()+1.0e-06);
  if (verbose_) std::cout<<"All the iso values (reliso, iso, ch_iso, ph_iso, nh_iso, pu_iso):"<<RelIso<<","<<iso<<","<<ch_iso<<","<<ph_iso<<","<<nh_iso<<","<<pu_iso<<std::endl;
  pfiso.chiso_ = ch_iso;
  pfiso.phiso_ = ph_iso;
  pfiso.nhiso_ = nh_iso;
  pfiso.puiso_ = pu_iso;
  pfiso.reliso_ = RelIso;
  return pfiso;
}

Isolation SharedFunctions::PFIsolation(TString ptype, ROOT::Math::PxPyPzMVector& trk, int gen_ind, std::vector<int>& trk_exc, const AnalysisEvent& event, double dr_max = 0.4) {
  float nh_iso {0.0};
  float ph_iso {0.0};
  float ch_iso {0.0};
  float pu_iso {0.0};
  float dr_trk_packedCandVec {99.};
  if (verbose_) {
      std::cout<<"Check the PFCandidates for isolation from 2nd func!"<<std::endl;
      std::cout<<"Reco primary trk (type,pt,eta,phi):"<<ptype<<","<<trk.Pt()<<","<<trk.Eta()<<","<<trk.Phi()<<std::endl;
    }
  int flag;
  for (int k = 0; k < event.numPackedCands; k++) {
    flag=0;
    if (ptype.Contains("hadron")) {
      for (auto i : trk_exc) {
        if ( i == k ) flag=1;
      }
    }
    if (flag) continue;
    if ( std::abs(event.packedCandsPdgId[k]) == 11 ||  std::abs(event.packedCandsPdgId[k]) == 13) continue; //rejecting muon/ele tracks
    // exclude tracks from HF above? 
    
    ROOT::Math::PxPyPzEVector packedCandVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
    ROOT::Math::PxPyPzEVector packedCandTrkVec{event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]};
    if (packedCandVec.Pt()<0.5) continue;
    dr_trk_packedCandVec = ROOT::Math::VectorUtil::DeltaR(trk, packedCandVec);
    if ((verbose_) && (dr_trk_packedCandVec < dr_max)) {
      std::cout<<"Nearby trk (index,pt,eta,phi,pdg):"<<k<<","<<packedCandVec.Pt()<<","<<packedCandVec.Eta()<<","<<packedCandVec.Phi()<<","<<event.packedCandsPdgId[k]<<std::endl;
    }
    //const TLorentzVector Trk_avoid {event.packedCandsPx[trk_exc], event.packedCandsPy[trk_exc], event.packedCandsPz[trk_exc], event.packedCandsE[trk_exc]};
    if ( event.packedCandsCharge[k] == 0 ) {
          if ( dr_trk_packedCandVec < dr_max ) { 
            if (event.packedCandsPdgId[k] == 22) ph_iso += packedCandVec.Et();
            else nh_iso += packedCandVec.Et();
          }
    }
    else {
      if ( event.packedCandsFromPV[k] >= 2 ) {
        if ( dr_trk_packedCandVec < dr_max )  ch_iso += packedCandVec.Pt();
      }
      else {
          if ( dr_trk_packedCandVec  < dr_max )  pu_iso += packedCandVec.Pt();
      }
    }
  }
  struct Isolation pfiso;
  float iso  = ch_iso  + std::max( float(0.0), nh_iso+ph_iso - float(0.5*pu_iso)  );
  float RelIso;
  if (ptype.Contains("gen"))
    RelIso = iso/(event.genParPt[gen_ind]+1.0e-06);
  else
    RelIso = iso/(trk.Pt()+1.0e-06);
  if (verbose_) std::cout<<"All the iso values (reliso, iso, ch_iso, ph_iso, nh_iso, pu_iso):"<<RelIso<<","<<iso<<","<<ch_iso<<","<<ph_iso<<","<<nh_iso<<","<<pu_iso<<std::endl;
  pfiso.chiso_ = ch_iso;
  pfiso.phiso_ = ph_iso;
  pfiso.nhiso_ = nh_iso;
  pfiso.puiso_ = pu_iso;
  pfiso.reliso_ = RelIso;
  return pfiso;
}
Isolation SharedFunctions::GenIsolation(TString ptype, int gen_ind, std::vector<int>& gen_exc, const AnalysisEvent& event, double dr_max) {
  // std::cout<<"Enters Isolation for "<<gen_ind<<std::endl;

  float nh_iso {0.0};
  float ph_iso {0.0};
  float ch_iso {0.0};
  float pu_iso {0.0};
  float dr_gen_genCand {99.};
  ROOT::Math::PtEtaPhiEVector genp {event.genParPt[gen_ind], event.genParEta[gen_ind], event.genParPhi[gen_ind], event.genParE[gen_ind]};
  // std::cout<<"Forms Vector genp"<<std::endl;
  if (verbose_) {
    std::cout<<"Check the GenParticles for isolation!"<<std::endl;
    std::cout<<"Gen. primary trk (type,index,pt,eta,phi):"<<ptype<<","<<gen_ind<<","<<genp.Pt()<<","<<genp.Eta()<<","<<genp.Phi()<<std::endl;
  }
  int flag;
  for (int k = 0; k < event.nGenPar; k++) {
    flag=0;
    if (ptype.Contains("hadron")) {
      for (auto i : gen_exc) {
        if ( i == k ) flag=1;
      }
    }
    if (flag) continue;
    if (k==gen_ind) continue;

    int pid { std::abs(event.genParId[k]) };
    int motherId { std::abs(event.genParMotherId[k]) };
    int status {event.genParStatus[k]};
    ROOT::Math::PtEtaPhiEVector GenCand{event.genParPt[k], event.genParEta[k], event.genParPhi[k], event.genParE[k]};
    // GenCand.SetPtEtaPhiE(event.genParPt[k], event.genParEta[k], event.genParPhi[k], event.genParE[k]);
    if ( pid == 11 || pid == 13) continue; //rejecting muon/ele tracks
    // exclude tracks from HF above? 
    if (GenCand.Pt() < 0.5) continue;
    if (status != 1) continue;
    dr_gen_genCand = ROOT::Math::VectorUtil::DeltaR(genp,GenCand);
    if ((verbose_) && (dr_gen_genCand < dr_max)) {
      std::cout<<"Nearby gen particle (index,pt,eta,phi,pdg):"<<k<<","<<GenCand.Pt()<<","<<GenCand.Eta()<<","<<GenCand.Phi()<<","<<pid<<std::endl;
    }
    //const TLorentzVector Trk_avoid {event.packedCandsPx[trk_exc], event.packedCandsPy[trk_exc], event.packedCandsPz[trk_exc], event.packedCandsE[trk_exc]};
    if ( event.genParCharge[k] == 0 ) {
      if ( dr_gen_genCand < dr_max ) { 
        if (pid == 22) ph_iso += GenCand.Et();
        else nh_iso += GenCand.Et();
      }
    }
    else {
      if ( dr_gen_genCand < dr_max )  ch_iso += GenCand.Pt();
    }
  }
  struct Isolation pfiso;
  // float iso  = ch_iso  + std::max( float(0.0), nh_iso+ph_iso - float(0.5*pu_iso)  );
  float iso  = ch_iso + nh_iso+ph_iso;
  float RelIso;
  RelIso = iso/(genp.Pt()+1.0e-06);
  if (verbose_) std::cout<<"All the iso values (reliso, iso, ch_iso, ph_iso, nh_iso, pu_iso):"<<RelIso<<","<<iso<<","<<ch_iso<<","<<ph_iso<<","<<nh_iso<<","<<pu_iso<<std::endl;
  pfiso.chiso_ = ch_iso;
  pfiso.phiso_ = ph_iso;
  pfiso.nhiso_ = nh_iso;
  pfiso.puiso_ = 0;
  pfiso.reliso_ = RelIso;
  return pfiso;
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

