#include "TH1D.h"
#include "TLorentzVector.h"
#include "cutClass.hpp"
#include "plots.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>

Plots::Plots(const std::vector<std::string> titles, const std::vector<std::string> names, const std::vector<float> xMins, const std::vector<float> xMaxs, const std::vector<int> nBins, const std::vector<std::string> fillExps, const std::vector<std::string> xAxisLabels,
             const std::vector<int> cutStage, const unsigned thisCutStage,  const std::string postfixName) { // Get the function pointer map for later custopmisation. This is gonna be great, I promise.
    const auto functionMap{getFncMap()};

    plotPoint = std::vector<plot>(names.size());
    for (unsigned i{0}; i < names.size(); i++) {
        std::string plotName = names[i] + "_" + postfixName;
        plotPoint[i].name = plotName;
        plotPoint[i].title = titles[i];
        plotPoint[i].fillExp = functionMap.at(fillExps[i]);
        plotPoint[i].xAxisLabel = xAxisLabels[i];
        plotPoint[i].plotHist =
            new TH1D{plotName.c_str(), (plotName + ";" + plotPoint[i].xAxisLabel).c_str(), nBins[i], xMins[i], xMaxs[i]};
        plotPoint[i].fillPlot = boost::numeric_cast<unsigned>(cutStage[i]) <= thisCutStage;
    }
}

Plots::~Plots() {
    for (unsigned i{0}; i < plotPoint.size(); i++)
        delete plotPoint[i].plotHist;
}

std::unordered_map<std::string, std::function<std::vector<float>(const AnalysisEvent&)>> Plots::getFncMap() const {
    return {
        {"lep1Pt", [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 TLorentzVector tempVec{
                     event.elePF2PATPX[event.electronIndexTight[0]],
                     event.elePF2PATPY[event.electronIndexTight[0]],
                     event.elePF2PATPZ[event.electronIndexTight[0]],
                     event.elePF2PATE[event.electronIndexTight[0]]};
                 return { float (tempVec.Pt()) };
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[0]],
                     event.muonPF2PATPY[event.muonIndexTight[0]],
                     event.muonPF2PATPZ[event.muonIndexTight[0]],
                     event.muonPF2PATE[event.muonIndexTight[0]]};
//                 tempVec *= event.muonMomentumSF[0];
                 return { float (tempVec.Pt()) };
             }
         }},
        {"lep2Pt", [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 TLorentzVector tempVec{
                     event.elePF2PATPX[event.electronIndexTight[1]],
                     event.elePF2PATPY[event.electronIndexTight[1]],
                     event.elePF2PATPZ[event.electronIndexTight[1]],
                     event.elePF2PATE[event.electronIndexTight[1]]};
                 return { float (tempVec.Pt()) };
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[1]],
                     event.muonPF2PATPY[event.muonIndexTight[1]],
                     event.muonPF2PATPZ[event.muonIndexTight[1]],
                     event.muonPF2PATE[event.muonIndexTight[1]]};
//                 tempVec *= event.muonMomentumSF[1];
                 return { float (tempVec.Pt()) };
             }
         }},
        {"lep1Eta", [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {std::abs(event.elePF2PATSCEta[event.electronIndexTight[0]])};
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[0]],
                     event.muonPF2PATPY[event.muonIndexTight[0]],
                     event.muonPF2PATPZ[event.muonIndexTight[0]],
                     event.muonPF2PATE[event.muonIndexTight[0]]};
//                 tempVec *= event.muonMomentumSF[0];
                 return { float (tempVec.Eta()) };
             }
         }},
        {"lep2Eta", [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {std::abs(event.elePF2PATSCEta[event.electronIndexTight[1]])};
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[1]],
                     event.muonPF2PATPY[event.muonIndexTight[1]],
                     event.muonPF2PATPZ[event.muonIndexTight[1]],
                     event.muonPF2PATE[event.muonIndexTight[1]]};
//                 tempVec *= event.muonMomentumSF[1];
                 return { float (tempVec.Eta()) };
             }
         }},
        {"lep1RelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 TLorentzVector tempVec{ event.elePF2PATPX[event.electronIndexTight[0]], event.elePF2PATPY[event.electronIndexTight[0]], event.elePF2PATPZ[event.electronIndexTight[0]], event.elePF2PATE[event.electronIndexTight[0]]};
                 return { ( ( event.elePF2PATChHadIso[event.electronIndexTight[0]] + event.elePF2PATNtHadIso[event.electronIndexTight[0]] + event.elePF2PATGammaIso[event.electronIndexTight[0]] ) / tempVec.Pt() ) };
             }
             else {
                 TLorentzVector tempVec{event.muonPF2PATPX[event.muonIndexTight[0]], event.muonPF2PATPY[event.muonIndexTight[0]], event.muonPF2PATPZ[event.muonIndexTight[0]], event.muonPF2PATE[event.muonIndexTight[0]]};
//                 tempVec *= event.muonMomentumSF[0];
                 return { ( ( event.muonPF2PATChHadIso[event.muonIndexTight[0]], event.muonPF2PATNtHadIso[event.muonIndexTight[0]], event.muonPF2PATGammaIso[event.muonIndexTight[0]] ) / tempVec.Pt() ) };
             }
         }},
        {"lep2RelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 TLorentzVector tempVec{ event.elePF2PATPX[event.electronIndexTight[1]], event.elePF2PATPY[event.electronIndexTight[1]], event.elePF2PATPZ[event.electronIndexTight[1]], event.elePF2PATE[event.electronIndexTight[1]]};
                 return { ( ( event.elePF2PATChHadIso[event.electronIndexTight[1]] + event.elePF2PATNtHadIso[event.electronIndexTight[1]] + event.elePF2PATGammaIso[event.electronIndexTight[1]] ) / tempVec.Pt() ) };
             }
             else {
                 TLorentzVector tempVec{event.muonPF2PATPX[event.muonIndexTight[1]], event.muonPF2PATPY[event.muonIndexTight[1]], event.muonPF2PATPZ[event.muonIndexTight[1]], event.muonPF2PATE[event.muonIndexTight[1]]};
//                 tempVec *= event.muonMomentumSF[1];
                 return { ( ( event.muonPF2PATChHadIso[event.muonIndexTight[1]], event.muonPF2PATNtHadIso[event.muonIndexTight[1]], event.muonPF2PATGammaIso[event.muonIndexTight[1]] ) / tempVec.Pt() ) };
             }
         }},
        {"lep1RelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {
                     event.elePF2PATComRelIsoRho[event.electronIndexTight[0]]};
             }
             else {
                 return {
                     event.muonPF2PATComRelIsodBeta[event.muonIndexTight[0]]};
             }
         }},
        {"lep2RelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {
                     event.elePF2PATComRelIsoRho[event.electronIndexTight[1]]};
             }
             else {
                 return {
                     event.muonPF2PATComRelIsodBeta[event.muonIndexTight[1]]};
             }
         }},
        {"lep1Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATPhi[event.electronIndexTight[0]]};
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[0]],
                     event.muonPF2PATPY[event.muonIndexTight[0]],
                     event.muonPF2PATPZ[event.muonIndexTight[0]],
                     event.muonPF2PATE[event.muonIndexTight[0]]};
//                 tempVec *= event.muonMomentumSF[0];
                 return { float (tempVec.Phi()) };
             }
         }},
        {"lep2Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATPhi[event.electronIndexTight[1]]};
             }
             else {
                 TLorentzVector tempVec{
                     event.muonPF2PATPX[event.muonIndexTight[1]],
                     event.muonPF2PATPY[event.muonIndexTight[1]],
                     event.muonPF2PATPZ[event.muonIndexTight[1]],
                     event.muonPF2PATE[event.muonIndexTight[1]]};
//                 tempVec *= event.muonMomentumSF[1];
                 return { float (tempVec.Phi()) };
             }
         }},
        {"wQuark1Pt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.wPairQuarks.first.Pt()) };
         }},
        {"wQuark1Eta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.wPairQuarks.first.Eta()) };
         }},
        {"wQuark1Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.wPairQuarks.first.Phi()) };
         }},
        {"wQuark2Pt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.wPairQuarks.second.Pt()) };
         }},
        {"wQuark2Eta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.wPairQuarks.second.Eta())) };
         }},
        {"wQuark2Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.wPairQuarks.second.Phi()) };
         }},
        {"chs1Pt", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.first.Pt()) };
         }},
        {"chs1Eta", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.chsPairVec.first.Eta())) };
         }},
        {"chs1Phi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.first.Phi()) };
         }},
        {"chs1RelIso", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsPairRelIso.first};
         }},
        {"chs1RelIsoDbeta", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsPairRelIsoDbeta.first};
         }},
        {"chs2Pt", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.second.Pt()) };
         }},
        {"chs2Eta", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.chsPairVec.second.Eta())) };
         }},
        {"chs2Phi", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float (event.chsPairVec.second.Phi()) };
         }},
        {"chs2RelIso", [](const AnalysisEvent&event) -> std::vector<float> {
             return {event.chsPairRelIso.second};
         }},
        {"chs2RelIsoDbeta", [](const AnalysisEvent&event) -> std::vector<float> {
             return {event.chsPairRelIsoDbeta.second};
         }},
        {"chs1TrkPt", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.first.Pt()) };
         }},
        {"chs1TrkEta", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.chsPairTrkVec.first.Eta())) };
         }},
        {"chs1TrkPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.first.Phi()) };
         }},
        {"chs1TrkIso", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsPairTrkIso.first};
         }},
        {"chs1TrkIsoDbeta", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsPairTrkIsoDbeta.first};
         }},
        {"chs2TrkPt", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.second.Pt()) };
         }},
        {"chs2TrkEta", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.chsPairTrkVec.second.Eta())) };
         }},
        {"chs2TrkPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float (event.chsPairTrkVec.second.Phi()) };
         }},
        {"chs2TrkIso", [](const AnalysisEvent&event) -> std::vector<float> {
             return {event.chsPairTrkIso.second};
         }},
        {"chs2TrkIsoDbeta", [](const AnalysisEvent&event) -> std::vector<float> {
             return {event.chsPairTrkIsoDbeta.second};
         }},
        {"met",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.metPF2PATEt) };
         }},
        {"numbJets",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.jetIndex.size()) };
         }},
        {"totalJetMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totalJet;
             if (event.jetIndex.size() > 0)
             {
                 for (auto jetIt = event.jetIndex.begin();
                      jetIt != event.jetIndex.end();
                      ++jetIt)
                 {
                     TLorentzVector tempJet;
                     float smearValue = event.jetSmearValue[*jetIt];
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt],
                                        event.jetPF2PATPy[*jetIt],
                                        event.jetPF2PATPz[*jetIt],
                                        event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totalJet += tempJet;
                 }
                 return { float (totalJet.M()) };
             }
             else
             {
                 return {};
             }
         }},
        {"totalJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totalJet;
             if (event.jetIndex.size() > 0)
             {
                 for (auto jetIt = event.jetIndex.begin();
                      jetIt != event.jetIndex.end();
                      ++jetIt)
                 {
                     TLorentzVector tempJet;
                     float smearValue = event.jetSmearValue[*jetIt];
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt],
                                        event.jetPF2PATPy[*jetIt],
                                        event.jetPF2PATPz[*jetIt],
                                        event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totalJet += tempJet;
                 }
                 return { float (totalJet.Pt()) };
             }
             else
             {
                 return {};
             }
         }},
        {"totalJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totalJet;
             if (event.jetIndex.size() > 0)
             {
                 for (auto jetIt = event.jetIndex.begin();
                      jetIt != event.jetIndex.end();
                      ++jetIt)
                 {
                     TLorentzVector tempJet;
                     float smearValue = event.jetSmearValue[*jetIt];
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt],
                                        event.jetPF2PATPy[*jetIt],
                                        event.jetPF2PATPz[*jetIt],
                                        event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totalJet += tempJet;
                 }
                 return { float (totalJet.Eta()) };
             }
             else
             {
                 return {};
             }
         }},
        {"totalJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totalJet;
             if (event.jetIndex.size() > 0)
             {
                 for (auto jetIt = event.jetIndex.begin();
                      jetIt != event.jetIndex.end();
                      ++jetIt)
                 {
                     TLorentzVector tempJet;
                     float smearValue = event.jetSmearValue[*jetIt];
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt],
                                        event.jetPF2PATPy[*jetIt],
                                        event.jetPF2PATPz[*jetIt],
                                        event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totalJet += tempJet;
                 }
                 return { float (totalJet.Phi())};
             }
             else
             {
                 return {};
             }
         }},
        {"leadingJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 0)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[0]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]],
                                    event.jetPF2PATPy[event.jetIndex[0]],
                                    event.jetPF2PATPz[event.jetIndex[0]],
                                    event.jetPF2PATE[event.jetIndex[0]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Pt()) };
             }
             else
             {
                 return {};
             }
         }},
        {"leadingJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 0)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[0]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]],
                                    event.jetPF2PATPy[event.jetIndex[0]],
                                    event.jetPF2PATPz[event.jetIndex[0]],
                                    event.jetPF2PATE[event.jetIndex[0]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Eta()) };
             }
             else
             {
                 return {};
             }
         }},
        {"leadingJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 0)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[0]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]],
                                    event.jetPF2PATPy[event.jetIndex[0]],
                                    event.jetPF2PATPz[event.jetIndex[0]],
                                    event.jetPF2PATE[event.jetIndex[0]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Phi()) };
             }
             else
             {
                 return {};
             }
         }},
        {"leadingJetDeltaRLep",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 0)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[0]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]],
                                    event.jetPF2PATPy[event.jetIndex[0]],
                                    event.jetPF2PATPz[event.jetIndex[0]],
                                    event.jetPF2PATE[event.jetIndex[0]]);
                 tempJet *= smearValue;
                 return { float (std::min(Cuts::deltaR(event.zPairLeptons.first.Eta(), event.zPairLeptons.first.Phi(), tempJet.Eta(), tempJet.Phi()),
                                  Cuts::deltaR(event.zPairLeptons.second.Eta(), event.zPairLeptons.second.Phi(), tempJet.Eta(), tempJet.Phi()))) };
             }
             else
             {
                 return {};
             }
         }},
        {"leadingJetBDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 0)
             {
                 return {
                     event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags
                         [event.jetIndex[0]]};
             }
             else
             {
                 return {};
             }
         }},
        {"secondJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[1]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]],
                                    event.jetPF2PATPy[event.jetIndex[1]],
                                    event.jetPF2PATPz[event.jetIndex[1]],
                                    event.jetPF2PATE[event.jetIndex[1]]);
                 tempJet *= smearValue;
                 return {float (tempJet.Pt()) };
             }
             else
             {
                 return {};
             }
         }},
        {"secondJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[1]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]],
                                    event.jetPF2PATPy[event.jetIndex[1]],
                                    event.jetPF2PATPz[event.jetIndex[1]],
                                    event.jetPF2PATE[event.jetIndex[1]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Eta()) };
             }
             else
             {
                 return {};
             }
         }},
        {"secondJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[1]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]],
                                    event.jetPF2PATPy[event.jetIndex[1]],
                                    event.jetPF2PATPz[event.jetIndex[1]],
                                    event.jetPF2PATE[event.jetIndex[1]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Phi()) };
             }
             else
             {
                 return {};
             }
         }},
        {"secondJetBDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 return {event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[event.jetIndex[1]]};
             }
             else
             {
                 return {};
             }
         }},
        {"secondJetDeltaRLep",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[1]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]], event.jetPF2PATPy[event.jetIndex[1]], event.jetPF2PATPz[event.jetIndex[1]], event.jetPF2PATE[event.jetIndex[1]]);
                 tempJet *= smearValue;
                 return { float (std::min(Cuts::deltaR(event.zPairLeptons.first.Eta(), event.zPairLeptons.first.Phi(), tempJet.Eta(), tempJet.Phi()),
                                  Cuts::deltaR(event.zPairLeptons.second.Eta(), event.zPairLeptons.second.Phi(), tempJet.Eta(), tempJet.Phi()))) };
             }
             else
             {
                 return {};
             }
         }},
        {"thirdJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 2)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[2]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[2]], event.jetPF2PATPy[event.jetIndex[2]], event.jetPF2PATPz[event.jetIndex[2]], event.jetPF2PATE[event.jetIndex[2]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Pt()) };
             }
             else
             {
                 return {};
             }
         }},
        {"thirdJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 2)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[2]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[2]], event.jetPF2PATPy[event.jetIndex[2]], event.jetPF2PATPz[event.jetIndex[2]], event.jetPF2PATE[event.jetIndex[2]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Eta()) };
             }
             else
             {
                 return {};
             }
         }},
        {"thirdJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 2)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[2]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[2]], event.jetPF2PATPy[event.jetIndex[2]], event.jetPF2PATPz[event.jetIndex[2]], event.jetPF2PATE[event.jetIndex[2]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Phi()) };
             }
             else
             {
                 return {};
             }
         }},
        {"thirdJetBDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 2)
             {
                 return {
                     event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[event.jetIndex[2]]};
             }
             else
             {
                 return {};
             }
         }},
        {"thirdJetDeltaRLep",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 2)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[2]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[2]], event.jetPF2PATPy[event.jetIndex[2]], event.jetPF2PATPz[event.jetIndex[2]], event.jetPF2PATE[event.jetIndex[2]]);
                 tempJet *= smearValue;
                 return { float (std::min(Cuts::deltaR(event.zPairLeptons.first.Eta(), event.zPairLeptons.first.Phi(), tempJet.Eta(), tempJet.Phi()),
                                  Cuts::deltaR(event.zPairLeptons.second.Eta(), event.zPairLeptons.second.Phi(), tempJet.Eta(), tempJet.Phi()))) };
             }
             else
             {
                 return {};
             }
         }},
        {"fourthJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 3)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[3]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[3]], event.jetPF2PATPy[event.jetIndex[3]], event.jetPF2PATPz[event.jetIndex[3]], event.jetPF2PATE[event.jetIndex[3]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Pt()) };
             }
             else
             {
                 return {};
             }
         }},
        {"fourthJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 3)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[3]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[3]], event.jetPF2PATPy[event.jetIndex[3]], event.jetPF2PATPz[event.jetIndex[3]], event.jetPF2PATE[event.jetIndex[3]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Eta()) };
             }
             else
             {
                 return {};
             }
         }},
        {"fourthJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 3)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[3]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[3]], event.jetPF2PATPy[event.jetIndex[3]], event.jetPF2PATPz[event.jetIndex[3]], event.jetPF2PATE[event.jetIndex[3]]);
                 tempJet *= smearValue;
                 return { float (tempJet.Phi()) };
             }
             else
             {
                 return {};
             }
         }},
        {"fourthJetBDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 3)
             {
                 return {event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[event.jetIndex[3]]};
             }
             else
             {
                 return {};
             }
         }},
        {"fourthJetDeltaRLep",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() > 1)
             {
                 TLorentzVector tempJet;
                 float smearValue = event.jetSmearValue[event.jetIndex[1]];
                 tempJet.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]], event.jetPF2PATPy[event.jetIndex[1]], event.jetPF2PATPz[event.jetIndex[1]], event.jetPF2PATE[event.jetIndex[1]]);
                 tempJet *= smearValue;
                 return { float (std::min(Cuts::deltaR(event.zPairLeptons.first.Eta(), event.zPairLeptons.first.Phi(), tempJet.Eta(), tempJet.Phi()),
                                  Cuts::deltaR(event.zPairLeptons.second.Eta(), event.zPairLeptons.second.Phi(), tempJet.Eta(), tempJet.Phi()))) };
             }
             else
             {
                 return {};
             }
         }},
        {"numbBJets",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.bTagIndex.size()) };
         }},
        {"bTagDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() > 0)
             {
                 return {event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags[event.jetIndex[event.bTagIndex[0]]]};
             }
             return {};
         }},
        {"zLepton1Pt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.Pt()) };
         }},
        {"zLepton1Eta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.zPairLeptons.first.Eta())) };
         }},
        {"zLepton1Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.Phi()) };
         }},
        {"zLepton1RelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairRelIso.first};
         }},
        {"zLepton1RelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairRelIsoDbeta.first};
         }},
        {"zLepton1NewRelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewRelIso.first};
         }},
        {"zLepton1NewRelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewRelIsoDbeta.first};
         }},
        {"zLepton1NewTrkIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewTrkIso.first};
         }},
        {"zLepton1NewTrkIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewTrkIsoDbeta.first};
         }},
        {"zLepton2Pt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.Pt()) };
         }},
        {"zLepton2Eta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs(event.zPairLeptons.second.Eta())) };
         }},
        {"zLepton2Phi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.Phi()) };
         }},
        {"zLepton2RelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairRelIso.second};
         }},
        {"zLepton2RelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairRelIsoDbeta.second};
         }},
        {"zLepton2NewRelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewRelIso.second};
         }},
        {"zLepton2NewRelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewRelIsoDbeta.second};
         }},
        {"zLepton2NewTrkIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewTrkIso.second};
         }},
        {"zLepton2NewTrkIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zPairNewTrkIsoDbeta.second};
         }},
        {"zPairMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).M()) };
         }},
        {"zPairPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Pt()) };
         }},
        {"zPairEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs((event.zPairLeptons.first + event.zPairLeptons.second).Eta())) };
         }},
        {"zPairPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Phi()) };
         }},
        {"zPairRelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zRelIso};
         }},        
        {"zPairRelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zRelIsoDbeta};
         }},        
        {"zPairTrkIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zTrkIso};
         }},        
        {"zPairTrkIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.zTrkIsoDbeta};
         }},        
        {"zPairMassRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
                 return { float ((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).M()) };
         }},
        {"zPairPtRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
                 return { float ((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).Pt()) };
         }},
        {"zPairEtaRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
                 return { float ((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).Eta()) };
         }},
        {"zPairPhiRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
                 return { float ((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).Phi()) };
         }},
	{"chsPairMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairVec.first + event.chsPairVec.second).M()) };
         }},
	{"chsPairPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairVec.first + event.chsPairVec.second).Pt()) };
         }},
	{"chsPairEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs((event.chsPairVec.first + event.chsPairVec.second).Eta())) };
         }},
	{"chsPairPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairVec.first + event.chsPairVec.second).Phi()) };
         }},
        {"chsPairRelIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsRelIso};
         }},        
        {"chsPairRelIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsRelIsoDbeta};
         }},        
        {"chsPairDeltaR", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.first.DeltaR(event.chsPairVec.second)) };
         }},
        {"chsPairDeltaPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.first.DeltaPhi(event.chsPairVec.second)) };
         }},
        {"chsPairDeltaZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairVec.first.Z() - event.chsPairVec.second.Z()) };
         }},
	{"chsTrkPairMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVec.first + event.chsPairTrkVec.second).M()) };
         }},
	{"chsTrkPairPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVec.first + event.chsPairTrkVec.second).Pt()) };
         }},
	{"chsTrkPairEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs((event.chsPairTrkVec.first + event.chsPairTrkVec.second).Eta())) };
         }},
	{"chsTrkPairPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVec.first + event.chsPairTrkVec.second).Phi()) };
         }},
        {"chsTrkPairTrkIso",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTrkIso};
         }},        
        {"chsTrkPairTrkIsoDbeta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTrkIsoDbeta};
         }},        
        {"chsTrkPairDeltaR", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.first.DeltaR(event.chsPairTrkVec.second)) };
         }},
        {"chsTrkPairDeltaPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.first.DeltaPhi(event.chsPairTrkVec.second)) };
         }},
        {"chsTrkPairDeltaZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVec.first.Z() - event.chsPairTrkVec.second.Z()) };
         }},
	{"chsTrkPairMassRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).M()) };
         }},
	{"chsTrkPairPtRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Pt()) };
         }},
	{"chsTrkPairEtaRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::abs((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Eta()))};
         }},
	{"chsTrkPairPhiRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Phi())};
         }},
        {"chsTrkPairDeltaRRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVecRefitted.first.DeltaR(event.chsPairTrkVecRefitted.second))};
         }},
        {"chsTrkPairDeltaPhiRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVecRefitted.first.DeltaPhi(event.chsPairTrkVecRefitted.second))};
         }},
        {"chsTrkPairDeltaZRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.chsPairTrkVecRefitted.first.Z() - event.chsPairTrkVecRefitted.second.Z())};
         }},
        {"wPairMass", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.wPairQuarks.first + event.wPairQuarks.second).M()) };
         }},
        {"discalarMass", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).M()) };
         }},
        {"discalarMassNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1+mu2 + event.chsPairVec.first + event.chsPairVec.second).M()) };
//             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).M()) };
         }},
        {"discalarDeltaMass", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).M() - (event.chsPairVec.first + event.chsPairVec.second).M()) };
         }},
        {"discalarDeltaMassNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1 + mu2).M() - (event.chsPairVec.first + event.chsPairVec.second).M()) };
         }},
        {"discalarPt", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).Pt()) };
         }},
        {"discalarEta", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).Eta()) };
         }},
        {"discalarPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairVec.first + event.chsPairVec.second).Phi()) };
         }},
        {"discalarDeltaR", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR((event.chsPairVec.first + event.chsPairVec.second))) };
         }},
        {"discalarDeltaRNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1+mu2).DeltaR((event.chsPairVec.first + event.chsPairVec.second))) };
         }},
        {"discalarDeltaPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi((event.chsPairVec.first + event.chsPairVec.second))) };
         }},
        {"discalarDeltaZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Z() - (event.chsPairVec.first + event.chsPairVec.second).Z()) };
         }},
        {"discalarMassRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).M()) };
         }},
        {"discalarMassRefitNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1+mu2 + event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).M()) };
         }},
        {"discalarDeltaMassRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).M() - (event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).M()) };
         }},
        {"discalarDeltaMassRefitNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1+mu2).M() - (event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).M()) };
         }},
        {"discalarPtRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Pt()) };
         }},
        {"discalarEtaRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Eta()) };
         }},
        {"discalarPhiRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second + event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Phi()) };
         }},
        {"discalarDeltaRRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second))) };
         }},
        {"discalarDeltaRRefitNew", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector mu1, mu2;
             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};
             mu1.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]);
             mu2.SetPxPyPzE(event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]);
             return { float ((mu1+mu2).DeltaR((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second))) };
         }},
        {"discalarDeltaPhiRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi((event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second))) };
         }},
        {"discalarDeltaZRefit", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Z() - (event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second).Z()) };
         }},

        {"topMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() > 0) {
                 TLorentzVector tempBjet;
                 float smearValue = event.jetSmearValue[event.bTagIndex[0]];
                 tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
                 tempBjet *= smearValue;
                 return { float ((tempBjet + event.wPairQuarks.first + event.wPairQuarks.second).M()) };
             }
             else {
                 return {};
             }
         }},
        {"topPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() > 0) {
                 TLorentzVector tempBjet;
                 float smearValue = event.jetSmearValue[event.bTagIndex[0]];
                 tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
                 tempBjet *= smearValue;
                 return { float ((tempBjet + event.wPairQuarks.first + event.wPairQuarks.second).Pt()) };
             }
             else {
                 return {};
             }
         }},
        {"topEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() > 0) {
                 TLorentzVector tempBjet;
                 float smearValue = event.jetSmearValue[event.bTagIndex[0]];
                 tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
                 tempBjet *= smearValue;
                 return { float (std::abs((tempBjet + event.wPairQuarks.first + event.wPairQuarks.second).Eta())) };
             }
             else {
                 return {};
             }
         }},
        {"topPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() > 0) {
                 TLorentzVector tempBjet;
                 float smearValue = event.jetSmearValue[event.bTagIndex[0]];
                 tempBjet.SetPtEtaPhiE(
                     event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]],
                     event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]],
                     event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]],
                     event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
                 tempBjet *= smearValue;
                 return { float ((tempBjet + event.wPairQuarks.first + event.wPairQuarks.second).Phi()) };
             }
             else {
                 return {};
             }
         }},
        {"lep1D0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATD0PV[event.electronIndexTight[0]]};
             }
             else {
              	 return {event.muonPF2PATDBPV[event.muonIndexTight[0]]};
             }
         }},
        {"lep1D0Sig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATImpactTransSignificance[event.electronIndexTight[0]]};
             }
             else {
                 return { float ((std::abs(event.muonPF2PATDBPV[event.muonIndexTight[0]]))/(event.muonPF2PATDBPVError[event.muonIndexTight[0]] + 1.0e-06)) };
             }
         }},
        {"lep2D0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATD0PV[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATDBPV[event.muonIndexTight[1]]};
             }
         }},
        {"lep2D0Sig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATImpactTransSignificance[event.electronIndexTight[1]]};
             }
             else {
              	 return { float ((std::abs(event.muonPF2PATDBPV[event.muonIndexTight[1]]))/(event.muonPF2PATDBPVError[event.muonIndexTight[1]] + 1.0e-06)) };
             }
         }},
        {"lep1DZ",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATDZPV[event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATDZPV[event.muonIndexTight[0]]};
             }
         }},
        {"lep1DZSig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATImpact3DSignificance[event.electronIndexTight[0]]};
             }
             else {
              	 return { float ((std::abs(event.muonPF2PATDZPV[event.muonIndexTight[0]]))/(event.muonPF2PATDZPVError[event.muonIndexTight[0]] + 1.0e-06)) };
             }
         }},
        {"lep2DZ",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATDZPV[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATDZPV[event.muonIndexTight[1]]};
             }
         }},
        {"lep2DZSig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATImpact3DSignificance[event.electronIndexTight[1]]};
             }
             else {
              	 return { float ((std::abs(event.muonPF2PATDZPV[event.muonIndexTight[1]]))/(event.muonPF2PATDZPVError[event.muonIndexTight[1]] + 1.0e-06)) };
             }
         }},
        {"lep1DBD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATTrackDBD0[event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATTrackDBD0[event.muonIndexTight[0]]};
             }
         }},
        {"lep2DBD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATTrackDBD0[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATTrackDBD0[event.muonIndexTight[1]]};
             }
         }},
        {"lep1BeamSpotCorrectedD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATBeamSpotCorrectedTrackD0
                             [event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATBeamSpotCorrectedD0[event.muonIndexTight[0]]};
             }
         }},
        {"lep2BeamSpotCorrectedD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {event.elePF2PATBeamSpotCorrectedTrackD0[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATBeamSpotCorrectedD0[event.muonIndexTight[1]]};
             }
         }},
        {"lep1InnerTrackD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {};
             }
             else {
                 return {
                     event.muonPF2PATDBInnerTrackD0[event.muonIndexTight[0]]};
             }
         }},
        {"lep2InnerTrackD0",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.electronIndexTight.size() > 1) {
                 return {};
             }
             else {
                 return {event.muonPF2PATDBInnerTrackD0[event.muonIndexTight[1]]};
             }
         }},
        {"lep1ImpactTransDist",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransDist[event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATImpactTransDist[event.muonIndexTight[0]]};
             }
         }},
        {"lep2ImpactTransDist",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransDist[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATImpactTransDist[event.muonIndexTight[1]]};
             }
         }},
	{"lep1ImpactTransError",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransError[event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATImpactTransError[event.muonIndexTight[0]]};
             }
         }},
        {"lep2ImpactTransError",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransError[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATImpactTransError[event.muonIndexTight[1]]};
             }
         }},
	{"lep1ImpactTransSig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransSignificance[event.electronIndexTight[0]]};
             }
             else {
                 return {event.muonPF2PATImpactTransSignificance[event.muonIndexTight[0]]};
             }
         }},
        {"lep2ImpactTransSig",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if ( event.electronIndexTight.size() > 1 ) {
                 return {event.elePF2PATImpactTransSignificance[event.electronIndexTight[1]]};
             }
             else {
                 return {event.muonPF2PATImpactTransSignificance[event.muonIndexTight[1]]};
             }
         }},
        {"wTransverseMass",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (std::sqrt(2 * event.wPairQuarks.first.Pt() * event.wPairQuarks.second.Pt() * (1 - std::cos(event.wPairQuarks.first.Phi() - event.wPairQuarks.second.Phi()))))};
         }},
        {"jjDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempJet1;
             TLorentzVector tempJet2;
             if (event.jetIndex.size() < 2) {
                 return {};
             }
             float smearValue1 = event.jetSmearValue[event.jetIndex[0]];
             float smearValue2 = event.jetSmearValue[event.jetIndex[1]];
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]], event.jetPF2PATPy[event.jetIndex[0]], event.jetPF2PATPz[event.jetIndex[0]], event.jetPF2PATE[event.jetIndex[0]]);
             tempJet2.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]], event.jetPF2PATPy[event.jetIndex[1]], event.jetPF2PATPz[event.jetIndex[1]], event.jetPF2PATE[event.jetIndex[1]]);
             tempJet1 *= smearValue1;
             tempJet2 *= smearValue2;
             return { float (tempJet1.DeltaR(tempJet2)) };
         }},
        {"jjDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempJet1;
             TLorentzVector tempJet2;
             if (event.jetIndex.size() < 2) {
                 return {};
             }
             float smearValue1 = event.jetSmearValue[event.jetIndex[0]];
             float smearValue2 = event.jetSmearValue[event.jetIndex[1]];
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[0]], event.jetPF2PATPy[event.jetIndex[0]], event.jetPF2PATPz[event.jetIndex[0]], event.jetPF2PATE[event.jetIndex[0]]);
             tempJet2.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[1]], event.jetPF2PATPy[event.jetIndex[1]], event.jetPF2PATPz[event.jetIndex[1]], event.jetPF2PATE[event.jetIndex[1]]);
             tempJet1 *= smearValue1;
             tempJet2 *= smearValue2;
             return { float (tempJet1.DeltaPhi(tempJet2)) };
         }},
        {"wwDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() < 3) {
                 return {};
             }
             return { float (event.wPairQuarks.first.DeltaR(event.wPairQuarks.second)) };
         }},
        {"wwDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.jetIndex.size() < 3) {
                 return {};
             }
             return { float (event.wPairQuarks.first.DeltaPhi(event.wPairQuarks.second)) };
         }},
        {"lbDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempJet1;
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             float smearValue = event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]];
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (tempJet1.DeltaR(event.wLepton)) };
         }},
        {"lbDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempJet1;
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             float smearValue = event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]];
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (tempJet1.DeltaPhi(event.wLepton)) };
         }},
        {"zLepDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaR(event.zPairLeptons.second)) };
         }},
        {"zLepDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaPhi(event.zPairLeptons.second)) };
         }},
        {"zLepDelZ",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.Z() - event.zPairLeptons.second.Z()) };
         }},
        {"zLepDelRRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return {float (event.zPairLeptonsRefitted.first.DeltaR(event.zPairLeptonsRefitted.second)) };
         }},
        {"zLepDelPhiRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptonsRefitted.first.DeltaPhi(event.zPairLeptonsRefitted.second)) };
         }},
        {"zLepDelZRefit",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptonsRefitted.first.Z() - event.zPairLeptonsRefitted.second.Z()) };
         }},
        {"zLep1Quark1DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaR(event.wPairQuarks.first)) };
         }},
        {"zLep1Quark1DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaPhi(event.wPairQuarks.first)) };
         }},
        {"zLep1Quark2DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaR(event.wPairQuarks.second)) };
         }},
        {"zLep1Quark2DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.first.DeltaPhi(event.wPairQuarks.second)) };
         }},
        {"zLep2Quark1DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.DeltaR(event.wPairQuarks.first)) };
         }},
        {"zLep2Quark1DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.DeltaPhi(event.wPairQuarks.first)) };
         }},
        {"zLep2Quark2DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.DeltaR(event.wPairQuarks.second)) };
         }},
        {"zLep2Quark2DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float (event.zPairLeptons.second.DeltaPhi(event.wPairQuarks.second)) };
         }},
        {"zLep1BjetDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             TLorentzVector tempJet1;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]]) };
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (event.zPairLeptons.first.DeltaR(tempJet1)) };
         }},
        {"zLep1BjetDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             TLorentzVector tempJet1;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]]) };
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (event.zPairLeptons.first.DeltaPhi(tempJet1)) };
         }},
        {"zLep2BjetDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             TLorentzVector tempJet1;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]]) };
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (event.zPairLeptons.second.DeltaR(tempJet1)) };
         }},
        {"zLep2BjetDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             if (event.bTagIndex.size() < 1) {
                 return {};
             }
             TLorentzVector tempJet1;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]]) };
             tempJet1.SetPxPyPzE(event.jetPF2PATPx[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPy[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPz[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempJet1 *= smearValue;
             return { float (event.zPairLeptons.second.DeltaPhi(tempJet1)) };
         }},
        {"lepHt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Pt()) };
         }},
        {"wQuarkHt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).Pt()) };
         }},
        {"jetHt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             float jetHt{0.0};
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt = event.jetIndex.begin(); jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     jetHt += float (tempJet.Pt());
                 }
             }
             return { jetHt};
         }},
        {"totHt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             float totHt{0.0};
             totHt +=
                 (event.zPairLeptons.first + event.zPairLeptons.second).Pt();
             if (event.jetIndex.size() > 0)
             {
                 for (auto jetIt{event.jetIndex.begin()};
                      jetIt != event.jetIndex.end();
                      ++jetIt)
                 {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totHt += float (tempJet.Pt());
                 }
             }
             return {totHt};
         }},
        {"totHtOverPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             float totHt{0.0};
             totHt += float ((event.zPairLeptons.first + event.zPairLeptons.second).Pt());
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt{event.jetIndex.begin()}; jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totHt += float (tempJet.Pt());
                 }
             }
             float totPx{0.0};
             float totPy{0.0};
             totPx += float ((event.zPairLeptons.first + event.zPairLeptons.second).Px());
             totPy += float ((event.zPairLeptons.first + event.zPairLeptons.second).Py());
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt{event.jetIndex.begin()}; jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totPx += float (tempJet.Px());
                     totPy += float (tempJet.Py());
                 }
             }

             return {totHt / std::sqrt(totPx * totPx + totPy * totPy)};
         }},
        {"totPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             float totPx{0.0};
             float totPy{0.0};
             totPx += float ((event.zPairLeptons.first + event.zPairLeptons.second).Px());
             totPy += float ((event.zPairLeptons.first + event.zPairLeptons.second).Py());
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt{event.jetIndex.begin()}; jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totPx += float (tempJet.Px());
                     totPy += float (tempJet.Py());
                 }
             }
             return {std::sqrt(totPx * totPx + totPy * totPy)};
         }},
        {"totEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totVec;
             totVec = event.zPairLeptons.first + event.zPairLeptons.second;
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt{event.jetIndex.begin()}; jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                 }
             }
             return { float (std::abs(totVec.Eta())) };
         }},
        {"totM",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector totVec;
             totVec = event.zPairLeptons.first + event.zPairLeptons.second;
             if (event.jetIndex.size() > 0) {
                 for (auto jetIt{event.jetIndex.begin()}; jetIt != event.jetIndex.end(); ++jetIt) {
                     TLorentzVector tempJet;
                     float smearValue{ float (event.jetSmearValue[*jetIt]) };
                     tempJet.SetPxPyPzE(event.jetPF2PATPx[*jetIt], event.jetPF2PATPy[*jetIt], event.jetPF2PATPz[*jetIt], event.jetPF2PATE[*jetIt]);
                     tempJet *= smearValue;
                     totVec += tempJet;
                 }
             }
             return { float (totVec.M()) };
         }},
        {"wzDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR(event.wPairQuarks.first + event.wPairQuarks.second))};
         }},
        {"wzDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi(event.wPairQuarks.first + event.wPairQuarks.second))};
         }},
        {"zQuark1DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR(event.wPairQuarks.first))};
         }},
        {"zQuark1DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi(event.wPairQuarks.first))};
         }},
        {"zQuark2DelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR(event.wPairQuarks.second)) };
         }},
        {"zQuark2DelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi(event.wPairQuarks.second)) };
         }},
        {"zTopDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]])};
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaR(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"zTopDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]])};
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.first + event.zPairLeptons.second).DeltaPhi(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"zl1TopDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]])};
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.first).DeltaR(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"zl1TopDelPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]])};
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.first).DeltaPhi(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"zl2TopDelR",
         [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float (event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]])};
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.second).DeltaR(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"allJetEta",
         [](const AnalysisEvent& event) -> std::vector<float> {
             std::vector<float> etas;
             for (const auto& i : event.jetIndex) {
                 float smearValue = event.jetSmearValue[i];
                 TLorentzVector tempJet{event.jetPF2PATPx[i], event.jetPF2PATPy[i], event.jetPF2PATPz[i], event.jetPF2PATE[i]};
                 tempJet *= smearValue;
                 etas.emplace_back(float (tempJet.Eta()) );
             }
             return etas;
         }},
        {"allJetPhi",
         [](const AnalysisEvent& event) -> std::vector<float> {
             std::vector<float> phis;
             for (const auto& i : event.jetIndex) {
                 float smearValue = event.jetSmearValue[i];
                 TLorentzVector tempJet{event.jetPF2PATPx[i], event.jetPF2PATPy[i], event.jetPF2PATPz[i], event.jetPF2PATE[i]};
                 tempJet *= smearValue;
                 phis.emplace_back(float (tempJet.Phi()));
             }
             return phis;
         }},
        {"allJetPt",
         [](const AnalysisEvent& event) -> std::vector<float> {
             std::vector<float> pts;
             for (const auto& i : event.jetIndex) {
                 float smearValue = event.jetSmearValue[i];
                 TLorentzVector tempJet{event.jetPF2PATPx[i],
                                        event.jetPF2PATPy[i],
                                        event.jetPF2PATPz[i],
                                        event.jetPF2PATE[i]};
                 tempJet *= smearValue;
                 pts.emplace_back(tempJet.Pt());
             }
             return pts;
         }},
        {"allJetDeltaRLep",
         [](const AnalysisEvent& event) -> std::vector<float> {
             std::vector<float> dRs;
             for (const auto& i : event.jetIndex)
             {
                 float smearValue = event.jetSmearValue[i];
                 TLorentzVector tempJet{event.jetPF2PATPx[i],
                                        event.jetPF2PATPy[i],
                                        event.jetPF2PATPz[i],
                                        event.jetPF2PATE[i]};
                 tempJet *= smearValue;
                 dRs.emplace_back(
                     std::min(Cuts::deltaR(event.zPairLeptons.first.Eta(),
                                           event.zPairLeptons.first.Phi(),
                                           tempJet.Eta(),
                                           tempJet.Phi()),
                              Cuts::deltaR(event.zPairLeptons.second.Eta(),
                                           event.zPairLeptons.second.Phi(),
                                           tempJet.Eta(),
                                           tempJet.Phi())));
             }
             return dRs;
         }},
        {"allJetBDisc",
         [](const AnalysisEvent& event) -> std::vector<float> {
             std::vector<float> discs;
             for (const auto& i : event.jetIndex)
             {
                 discs.emplace_back(
                     event.jetPF2PATpfCombinedInclusiveSecondaryVertexV2BJetTags
                         [i]);
             }
             return discs;
         }},
        {"zl2TopDelPhi", [](const AnalysisEvent& event) -> std::vector<float> {
             TLorentzVector tempBjet;
             float smearValue{ float(event.jetSmearValue[event.jetIndex[event.bTagIndex[0]]]) };
             tempBjet.SetPtEtaPhiE(event.jetPF2PATPt[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATEta[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATPhi[event.jetIndex[event.bTagIndex[0]]], event.jetPF2PATE[event.jetIndex[event.bTagIndex[0]]]);
             tempBjet *= smearValue;
             return { float ((event.zPairLeptons.second).DeltaPhi(event.wPairQuarks.first + event.wPairQuarks.second + tempBjet)) };
         }},
        {"mumuVtxPx", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVtxPx[event.mumuTrkIndex]};
         }},
        {"mumuVtxPy", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVtxPy[event.mumuTrkIndex]};
         }},
        {"mumuVtxPz", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVtxPz[event.mumuTrkIndex]};
         }},
        {"mumuVtxP", [](const AnalysisEvent& event) -> std::vector<float> {
             return {std::sqrt( event.muonTkPairPF2PATTkVtxP2[event.mumuTrkIndex] )};
         }},
	{"mumuVx", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVx[event.mumuTrkIndex]};
         }},
	{"mumuVy", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]};
         }},
	{"mumuVz", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTkVz[event.mumuTrkIndex]};
         }},
	{"mumuVxy", [](const AnalysisEvent& event) -> std::vector<float> {
             float vx {event.muonTkPairPF2PATTkVx[event.mumuTrkIndex]}, vy {event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]};
             return { std::sqrt(vx*vx + vy*vy) };
         }},
	{"mumuVabs", [](const AnalysisEvent& event) -> std::vector<float> {
             float vx {event.muonTkPairPF2PATTkVx[event.mumuTrkIndex]}, vy {event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]}, vz {event.muonTkPairPF2PATTkVz[event.mumuTrkIndex]};
             return { std::sqrt(vx*vx + vy*vy + vz*vz) };
         }},
	{"mumuVtxChi2Ndof", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxNdof[event.mumuTrkIndex]+1.0e-06)) };
         }},
	{"mumuVtxAngleXY", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxAngleXY[event.mumuTrkIndex])) };
         }},
        {"mumuVtxAngleXYZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxAngleXYZ[event.mumuTrkIndex])) };
         }},
        {"mumuVtxSigXY", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxDistMagXY[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxDistMagXYSigma[event.mumuTrkIndex]+1.0e-06)) };
         }},
	{"mumuVtxSigXYZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxDistMagXYZ[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxDistMagXYZSigma[event.mumuTrkIndex]+1.0e-06)) };
         }},
	{"mumuVtxDca", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTkVtxDcaPreFit[event.mumuTrkIndex])) };
         }},
        {"mumuTrkInnerPtOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkPt[event.zPairIndex.first]};
         }},
        {"mumuTrkInnerEtaOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkEta[event.zPairIndex.first]};
         }},
        {"mumuTrkInnerChi2NdofOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkNormChi2[event.zPairIndex.first]};
         }},
        {"mumuTrkInnerPtOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkPt[event.zPairIndex.second]};
         }},
        {"mumuTrkInnerEtaOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkEta[event.zPairIndex.second]};
         }},
        {"mumuTrkInnerChi2NdofOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonPF2PATInnerTkNormChi2[event.zPairIndex.second]};
         }},
	{"mumuTrkInnerPtNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTk1Pt[event.mumuTrkIndex]};
         }},
	{"mumuTrkInnerEtaNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTk2Eta[event.mumuTrkIndex]};
         }},
	{"mumuTrkInnerChi2NdofNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTk1Chi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTk1Ndof[event.mumuTrkIndex]+1.0e-06)) };
         }},
	{"mumuTrkInnerPtNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTk2Pt[event.mumuTrkIndex]};
         }},
	{"mumuTrkInnerEtaNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.muonTkPairPF2PATTk2Eta[event.mumuTrkIndex]};
         }},
	{"mumuTrkInnerChi2NdofNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.muonTkPairPF2PATTk2Chi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTk2Ndof[event.mumuTrkIndex]+1.0e-06)) };
         }},
        {"dichsVtxPx", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVtxPx[event.chsPairTrkIndex]};
         }},
        {"dichsVtxPy", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVtxPy[event.chsPairTrkIndex]};
         }},
        {"dichsVtxPz", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVtxPz[event.chsPairTrkIndex]};
         }},
        {"dichsVtxP", [](const AnalysisEvent& event) -> std::vector<float> {
             return {std::sqrt( event.chsTkPairTkVtxP2[event.chsPairTrkIndex] )};
         }},
	{"dichsVx", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVx[event.chsPairTrkIndex]};
         }},
	{"dichsVy", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVy[event.chsPairTrkIndex]};
         }},
	{"dichsVz", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTkVz[event.chsPairTrkIndex]};
         }},
	{"dichsVxy", [](const AnalysisEvent& event) -> std::vector<float> {
             float vx {event.chsTkPairTkVx[event.chsPairTrkIndex]}, vy {event.chsTkPairTkVy[event.chsPairTrkIndex]};
             return { std::sqrt(vx*vx + vy*vy) };
         }},
	{"dichsVabs", [](const AnalysisEvent& event) -> std::vector<float> {
             float vx {event.chsTkPairTkVx[event.chsPairTrkIndex]}, vy {event.chsTkPairTkVy[event.chsPairTrkIndex]}, vz {event.chsTkPairTkVz[event.chsPairTrkIndex]};
             return { std::sqrt(vx*vx + vy*vy + vz*vz) };
         }},
	{"dichsVtxChi2Ndof", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06)) };
         }},
	{"dichsVtxAngleXY", [](const AnalysisEvent& event) -> std::vector<float> {
             return {(event.chsTkPairTkVtxAngleXY[event.chsPairTrkIndex])};
         }},
        {"dichsVtxAngleXYZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return {(event.chsTkPairTkVtxAngleXYZ[event.chsPairTrkIndex])};         
         }},
        {"dichsVtxSigXY", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsTkPairTkVtxDistMagXY[event.chsPairTrkIndex])/(event.chsTkPairTkVtxDistMagXYSigma[event.chsPairTrkIndex]+1.0e-06)) };
         }},
	{"dichsVtxSigXYZ", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsTkPairTkVtxDistMagXYZ[event.chsPairTrkIndex])/(event.chsTkPairTkVtxDistMagXYZSigma[event.chsPairTrkIndex]+1.0e-06)) };
         }},
	{"dichsVtxDca", [](const AnalysisEvent& event) -> std::vector<float> {
             return {(event.chsTkPairTkVtxDcaPreFit[event.chsPairTrkIndex])};
         }},
        {"dichsTrkInnerPtOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float(event.chsPairTrkVec.first.Pt())};
         }},
        {"dichsTrkInnerEtaOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float(event.chsPairTrkVec.first.Eta())};
         }},
        {"dichsTrkInnerChi2NdofOld1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]};
         }},
        {"dichsTrkInnerPtOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float(event.chsPairTrkVec.second.Pt())};
         }},
        {"dichsTrkInnerEtaOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {float(event.chsPairTrkVec.second.Eta())};
         }},
        {"dichsTrkInnerChi2NdofOld2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]};
         }},
	{"dichsTrkInnerPtNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTk1Pt[event.chsPairTrkIndex]};
         }},
	{"dichsTrkInnerEtaNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTk2Eta[event.chsPairTrkIndex]};
         }},
	{"dichsTrkInnerChi2NdofNew1", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsTkPairTk1Chi2[event.chsPairTrkIndex])/(event.chsTkPairTk1Ndof[event.chsPairTrkIndex]+1.0e-06)) };
         }},
	{"dichsTrkInnerPtNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTk2Pt[event.chsPairTrkIndex]};
         }},
	{"dichsTrkInnerEtaNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return {event.chsTkPairTk2Eta[event.chsPairTrkIndex]};
         }},
	{"dichsTrkInnerChi2NdofNew2", [](const AnalysisEvent& event) -> std::vector<float> {
             return { float ((event.chsTkPairTk2Chi2[event.chsPairTrkIndex])/(event.chsTkPairTk2Ndof[event.chsPairTrkIndex]+1.0e-06)) };
         }}
    };
}

void Plots::fillAllPlots(const AnalysisEvent& event, const double eventWeight)
{
    for (unsigned i{0}; i < plotPoint.size(); i++) {
        if (plotPoint[i].fillPlot) {
            for (const auto& val : (this->plotPoint[i].fillExp)(event)) {
                plotPoint[i].plotHist->Fill(val, eventWeight);
            }
        }
    }
}

void Plots::saveAllPlots()
{
    for (unsigned i{0}; i < plotPoint.size(); i++)
    {
        plotPoint[i].plotHist->SaveAs(("plots/" + plotPoint[i].name + ".pdf").c_str());
        plotPoint[i].plotHist->SaveAs(("plots/" + plotPoint[i].name + ".root").c_str());
    }
}
