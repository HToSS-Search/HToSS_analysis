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

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {4.0}, chsMass_{0.13957018};

// Diparticle cuts
const double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4};
const double scalarMassCut_{4.0}, higgsTolerence_ {10.};

// debug flag
const bool useMCTruth_{false};

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
		int flow, fhigh;
		bool verbose_{false};
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Gen Histos

    TProfile* p_genHadronicDecayFractions {new TProfile ("p_genHadronicDecayFractions", "Hadronic scalar decay mode fractions", 5, 0.5, 5.5, "ymax = 1.0")};
    p_genHadronicDecayFractions->GetXaxis()->SetBinLabel(1, "#pi#pi");
    p_genHadronicDecayFractions->GetXaxis()->SetBinLabel(2, "K^{0}K^{0}");
    p_genHadronicDecayFractions->GetXaxis()->SetBinLabel(3, "K_{L}^{0}K_{L}^{0}");
    p_genHadronicDecayFractions->GetXaxis()->SetBinLabel(4, "K_{S}^{0}K_{S}^{0}");
    p_genHadronicDecayFractions->GetXaxis()->SetBinLabel(5, "K^{+}K^{-}");
    
    TProfile* p_HadronicDecayFractions_AtTail {new TProfile ("p_HadronicDecayFractions_AtTail", "Hadronic scalar decay mode fractions at m_{chs,chs} < 1.5 GeV", 4, 0.5, 4.5, "ymax = 1.0")};
    p_HadronicDecayFractions_AtTail->GetXaxis()->SetBinLabel(1, "#pi#pi");
    p_HadronicDecayFractions_AtTail->GetXaxis()->SetBinLabel(2, "KK");
    p_HadronicDecayFractions_AtTail->GetXaxis()->SetBinLabel(3, "K_{S}^{0}K_{S}^{0}");
    p_HadronicDecayFractions_AtTail->GetXaxis()->SetBinLabel(4, "K^{+}K^{-}");
    

    TH1F* h_scalarEnergy          {new TH1F("h_scalarEnergy",          "scalar energy; [GeV]", 5000, 0., 2500)};

    TH1F* h_genDimuonDeltaR       {new TH1F("h_genDimuonDeltaR",       "Dimuon gen deltaR", 50, 0., 1.)};
    TH1F* h_genDimuonMass         {new TH1F("h_genDimuonMass",         "Dimuon gen mass", 30, 0., 11.)};
    TH1F* h_genDimuonPt           {new TH1F("h_genDimuonPt",           "Dimuon gen Pt",  200, 0., 200)}; 
    TH1F* h_genDimuonEta          {new TH1F("h_genDimuonEta",          "Dimuon gen Eta", 60, 0., 5.)};
    TH1F* h_genLeadingMuonPt      {new TH1F("h_genLeadingMuonPt",      "Leading generator muon", 300, 0., 150.)};
    TH1F* h_genSubleadingMuonPt   {new TH1F("h_genSubleadingMuonPt",   "Subleading generator muon", 300, 0., 150.)};

    TH1F* h_genDipionDeltaR       {new TH1F("h_genDipionDeltaR",       "Dipion gen deltaR", 50, 0., 1.)};
    TH1F* h_genDipionMass         {new TH1F("h_genDipionMass",         "Dipion gen mass", 30, 0., 11.)};
    TH1F* h_genDipionPt           {new TH1F("h_genDipionPt",           "Dipion gen Pt",  200, 0., 200)}; 
    TH1F* h_genDipionEta          {new TH1F("h_genDipionEta",          "Dipion gen Eta", 60, 0., 5.)};
    TH1F* h_genLeadingPionPt      {new TH1F("h_genLeadingPionPt",      "Leading generator Pion p_{T}", 300, 0., 150.)};
    TH1F* h_genSubleadingPionPt   {new TH1F("h_genSubleadingPionPt",   "Subleading generator Pion p_{T}", 300, 0., 150.)};
    TH1F* h_genLeadingPionEta     {new TH1F("h_genLeadingPionEta",     "Leading generator Pion #eta", 100, 0., 5.)};
    TH1F* h_genSubleadingPionEta  {new TH1F("h_genSubleadingPionEta",  "Subleading generator Pion #eta", 100, 0., 5.)};
    TH1F* h_genLeadingPionPhi     {new TH1F("h_genLeadingPionPhi",     "Leading generator Pion #phi", 300, -3.5, 3.5)};
    TH1F* h_genSubleadingPionPhi  {new TH1F("h_genSubleadingPionPhi",  "Subleading generator Pion #phi", 300, -3.5, 3.5)};
    TH1F* h_genLeadingVxPion      {new TH1F("h_genLeadingVxPion",      "Leading gen pion v_{x}",     200, -50., 50.)};
    TH1F* h_genLeadingVyPion      {new TH1F("h_genLeadingVyPion",      "Leading gen pion v_{y}",     200, -50., 50.)};
    TH1F* h_genLeadingVzPion      {new TH1F("h_genLeadingVzPion",      "Leading gen pion v_{z}",     200, -50., 50.)};
    TH1F* h_genLeadingVxyPion     {new TH1F("h_genLeadingVxyPion",     "Leading gen pion v_{xy}",    200, 0., 100.)};
    TH1F* h_genLeadingVdPion      {new TH1F("h_genLeadingVdPion",      "Leading gen pion v_{d}",     200, 0., 200.)};
    TH1F* h_genSubleadingVxPion   {new TH1F("h_genSubleadingVxPion",   "Subleading gen pion v_{x}",  200, -50., 50.)};
    TH1F* h_genSubleadingVyPion   {new TH1F("h_genSubleadingVyPion",   "Subleading gen pion v_{y}",  200, -50., 50.)};
    TH1F* h_genSubleadingVzPion   {new TH1F("h_genSubleadingVzPion",   "Subleading gen pion v_{z}",  200, -50., 50.)};
    TH1F* h_genSubleadingVxyPion  {new TH1F("h_genSubleadingVxyPion",  "Subleading gen pion v_{xy}", 200, 0., 100.)};
    TH1F* h_genSubleadingVdPion   {new TH1F("h_genSubleadingVdPion",   "Subleading gen pion v_{d}",  200, 0., 100.)};

    TH1F* h_genDikaonDeltaR       {new TH1F("h_genDikaonDeltaR",       "Dikaon gen deltaR", 50, 0., 1.)};
    TH1F* h_genDikaonMass         {new TH1F("h_genDikaonMass",         "Dikaon gen mass", 30, 0., 11.)};
    TH1F* h_genDikaonPt           {new TH1F("h_genDikaonPt",           "Dikaon gen Pt",  200, 0., 200)}; 
    TH1F* h_genDikaonEta          {new TH1F("h_genDikaonEta",          "Dikaon gen Eta", 60, 0., 5.)};
    TH1F* h_genLeadingKaonPt      {new TH1F("h_genLeadingKaonPt",      "Leading generator Kaon p_{T}", 300, 0., 150.)};
    TH1F* h_genSubleadingKaonPt   {new TH1F("h_genSubleadingKaonPt",   "Subleading generator Kaon p_{T}", 300, 0., 150.)};
    TH1F* h_genLeadingKaonEta     {new TH1F("h_genLeadingKaonEta",     "Leading generator Kaon #eta", 100, 0., 5.)};
    TH1F* h_genSubleadingKaonEta  {new TH1F("h_genSubleadingKaonEta",  "Subleading generator Kaon #eta", 100, 0., 5.)};
    TH1F* h_genLeadingKaonPhi     {new TH1F("h_genLeadingKaonPhi",     "Leading generator Kaon #phi", 300, -3.5, 3.5)};
    TH1F* h_genSubleadingKaonPhi  {new TH1F("h_genSubleadingKaonPhi",  "Subleading generator Kaon #phi", 300, -3.5, 3.5)};
    TH1F* h_genLeadingVxKaon      {new TH1F("h_genLeadingVxKaon",      "Leading gen kaon v_{x}",     200, -50., 50.)};
    TH1F* h_genLeadingVyKaon      {new TH1F("h_genLeadingVyKaon",      "Leading gen kaon v_{y}",     200, -50., 50.)};
    TH1F* h_genLeadingVzKaon      {new TH1F("h_genLeadingVzKaon",      "Leading gen kaon v_{z}",     200, -50., 50.)};
    TH1F* h_genLeadingVxyKaon     {new TH1F("h_genLeadingVxyKaon",     "Leading gen kaon v_{xy}",    200, 0., 100.)};
    TH1F* h_genLeadingVdKaon      {new TH1F("h_genLeadingVdKaon",      "Leading gen kaon v_{d}",     200, 0., 100.)};
    TH1F* h_genSubleadingVxKaon   {new TH1F("h_genSubleadingVxKaon",   "Subleading gen kaon v_{x}",  200, -50., 50.)};
    TH1F* h_genSubleadingVyKaon   {new TH1F("h_genSubleadingVyKaon",   "Subleading gen kaon v_{y}",  200, -50., 50.)};
    TH1F* h_genSubleadingVzKaon   {new TH1F("h_genSubleadingVzKaon",   "Subleading gen kaon v_{z}",  200, -50., 50.)};
    TH1F* h_genSubleadingVxyKaon  {new TH1F("h_genSubleadingVxyKaon",  "Subleading gen kaon v_{xy}", 200, 0., 100.)};
    TH1F* h_genSubleadingVdKaon   {new TH1F("h_genSubleadingVdKaon",   "Subleading gen kaon v_{d}",  200, 0., 100.)};
    
    TH1F* h_genDiChsDeltaR       {new TH1F("h_genDiChsDeltaR",       "DiChs gen deltaR", 50, 0., 1.)};
    TH1F* h_genDiChsMass         {new TH1F("h_genDiChsMass",         "DiChs gen mass", 30, 0., 11.)};
    TH1F* h_genDiChsPt           {new TH1F("h_genDiChsPt",           "DiChs gen Pt",  200, 0., 200)}; 
    TH1F* h_genDiChsEta          {new TH1F("h_genDiChsEta",          "DiChs gen Eta", 60, 0., 5.)};
    TH1F* h_genLeadingChsPt      {new TH1F("h_genLeadingChsPt",      "Leading generator Chs p_{T}", 300, 0., 150.)};
    TH1F* h_genSubleadingChsPt   {new TH1F("h_genSubleadingChsPt",   "Subleading generator Chs p_{T}", 300, 0., 150.)};
    TH1F* h_genLeadingChsEta     {new TH1F("h_genLeadingChsEta",     "Leading generator Chs #eta", 100, 0., 5.)};
    TH1F* h_genSubleadingChsEta  {new TH1F("h_genSubleadingChsEta",  "Subleading generator Chs #eta", 100, 0., 5.)};
    TH1F* h_genLeadingChsPhi     {new TH1F("h_genLeadingChsPhi",     "Leading generator Chs #phi", 300, -3.5, 3.5)};
    TH1F* h_genSubleadingChsPhi  {new TH1F("h_genSubleadingChsPhi",  "Subleading generator Chs #phi", 300, -3.5, 3.5)};
    TH1F* h_genLeadingVxChs      {new TH1F("h_genLeadingVxChs",      "Leading gen Chs v_{x}",     200, -50., 50.)};
    TH1F* h_genLeadingVyChs      {new TH1F("h_genLeadingVyChs",      "Leading gen Chs v_{y}",     200, -50., 50.)};
    TH1F* h_genLeadingVzChs      {new TH1F("h_genLeadingVzChs",      "Leading gen Chs v_{z}",     200, -50., 50.)};
    TH1F* h_genLeadingVxyChs     {new TH1F("h_genLeadingVxyChs",     "Leading gen Chs v_{xy}",    200, 0., 100.)};
    TH1F* h_genLeadingVdChs      {new TH1F("h_genLeadingVdChs",      "Leading gen Chs v_{d}",     200, 0., 100.)};
    TH1F* h_genSubleadingVxChs   {new TH1F("h_genSubleadingVxChs",   "Subleading gen Chs v_{x}",  200, -50., 50.)};
    TH1F* h_genSubleadingVyChs   {new TH1F("h_genSubleadingVyChs",   "Subleading gen Chs v_{y}",  200, -50., 50.)};
    TH1F* h_genSubleadingVzChs   {new TH1F("h_genSubleadingVzChs",   "Subleading gen Chs v_{z}",  200, -50., 50.)};
    TH1F* h_genSubleadingVxyChs  {new TH1F("h_genSubleadingVxyChs",  "Subleading gen Chs v_{xy}", 200, 0., 100.)};
    TH1F* h_genSubleadingVdChs   {new TH1F("h_genSubleadingVdChs",   "Subleading gen Chs v_{d}",  200, 0., 100.)};

    TH1F* h_genDiscalarDeltaR_mumu_pipi     {new TH1F("h_genDiscalarDeltaR_mumu_pipi",      "#DeltaR_{#mu#mu#pi#pi}^{gen}", 500, 0., 10.)};
    TH1F* h_genDiscalarDeltaR_mumu_kaonkaon {new TH1F("h_genDiscalarDeltaR_mumu_kaonkaon",  "#DeltaR_{#mu#muKK}^{gen}", 500, 0., 10.)};
    TH1F* h_genDiscalarDeltaR_mumu_ChsChs {new TH1F("h_genDiscalarDeltaR_mumu_ChsChs",  "#DeltaR_{#mu#muChsChs}^{gen}", 500, 0., 10.)};


    TH2F* h_genLeadingPionPt_genSubleadingPionPt {new TH2F("h_genLeadingPionPt_genSubleadingPionPt", "gen Pions p_{T}; Leading gen Pion p_{T}; Subleading gen Pion p_{T}", 300, 0., 150., 300, 0., 150.)};
    TH2F* h_genLeadingKaonPt_genSubleadingKaonPt {new TH2F("h_genLeadingKaonPt_genSubleadingKaonPt", "gen Kaons p_{T}; Leading gen Kaon p_{T}; Subleading gen Kaon p_{T}", 300, 0., 150., 300, 0., 150.)};
    
    // scalar ratio plots
    TH1F* h_genScalarMassRatio    {new TH1F("h_genScalarMassRatio",     "Ratio of m_{hadron+hadron}/m_{#mu#mu}", 200, 0., 2.0)};
    TH2F* h_genScalarMass2D       {new TH2F("h_genScalarMass2D",        "scalar mass 2D plots; m_{#mu#mu}; m_{hadron+hadron}", 400, 0., 4.0, 400, 0., 4.0)};

    // Reco plots

    TH1F* h_numLooseMuons             {new TH1F("h_numLooseMuons",            "Number of loose muons IDed", 10, -0.5, 9.5)};
    TH1F* h_numRecoScalarMuons        {new TH1F("h_numRecoScalarMuons",       "Number of loose muons from scalar decay", 10, -0.5, 9.5)};
    TH1F* h_numRecoDirectScalarMuons  {new TH1F("h_numRecoDirectScalarMuons", "Number of loose muons directly from scalar decay", 10, -0.5, 9.5)};

    TH1F* h_recoDimuonDeltaR        {new TH1F("h_recoDimuonDeltaR",      "Dimuon reco deltaR", 50, 0., 1.)};
    TH1F* h_recoDimuonMass          {new TH1F("h_recoDimuonMass",        "Dimuon reco mass", 30, 0., 11.)};
    TH1F* h_recoDimuonPt            {new TH1F("h_recoDimuonPt",          "Dimuon reco Pt",  200, 0., 200)}; 
    TH1F* h_recoDimuonEta           {new TH1F("h_recoDimuonEta",         "Dimuon reco Eta", 60, 0., 5.)};
    TH1F* h_recoLeadingMuonPt       {new TH1F("h_recoLeadingMuonPt",     "Leading generator muon", 300, 0., 150.)};
    TH1F* h_recoSubleadingMuonPt    {new TH1F("h_recoSubleadingMuonPt",  "Subleading generator muon", 300, 0., 150.)};

    TProfile* p_numPromptMuons            {new TProfile ("p_numPromptMuons", "Muon flags for selected reco muons", 6, 0.5, 6.5, "ymax = 1.0")};
    p_numPromptMuons->GetXaxis()->SetBinLabel(1, "Prompt Decayed");
    p_numPromptMuons->GetXaxis()->SetBinLabel(2, "Prompt Final State");
    p_numPromptMuons->GetXaxis()->SetBinLabel(3, "Hard Process");
    p_numPromptMuons->GetXaxis()->SetBinLabel(4, "Likely Pythia6 Status 3");
    p_numPromptMuons->GetXaxis()->SetBinLabel(5, "ScalarAncestor");
    p_numPromptMuons->GetXaxis()->SetBinLabel(6, "DirectScalarAncestor");

    TProfile* p_recoSelectedMuonMatching  {new TProfile ("p_recoSelectedMuonMatching", "Selected leading/subleading reco muon ancestry", 8, 0.5, 8.5, "ymax = 1.0")};
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(1, "Both muons genuine");
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(2, "Leading muon geunine");
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(3, "Subleading muon genuine");
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(4, "Both muons fake");
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(5, "Both muons from leptonic scalar decays");  
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(6, "Leading muon from leptonic scalar decay");  
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(7, "Subleading muon from leptonic scalar decay");  
    p_recoSelectedMuonMatching->GetXaxis()->SetBinLabel(8, "Both muons not from leptonic scalar decays");  

    TProfile* p_leadingMuonIso            {new TProfile ("p_leadingMuonIso", "Leading muon iso cuts", 8, 0.5, 8.5, "ymax = 1.0")};
    p_leadingMuonIso->GetXaxis()->SetBinLabel(1, "PF Iso Very Loose");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(2, "PF Iso Loose");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(3, "PF Iso Medium");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(4, "PF Iso Tight");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(5, "PF Iso Very Tight");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(6, "PF Iso Very Very Tight");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(7, "Tk Iso Loose");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(8, "Tk Iso Tight");

    TProfile* p_subleadingMuonIso         {new TProfile ("p_subleadingMuonIso", "Subleading muon iso cuts", 8, 0.5, 8.5, "ymax = 1.0")};
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(1, "PF Iso Very Loose");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(2, "PF Iso Loose");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(3, "PF Iso Medium");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(4, "PF Iso Tight");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(5, "PF Iso Very Tight");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(6, "PF Iso Very Very Tight");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(7, "Tk Iso Loose");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(8, "Tk Iso Tight");

    TH1F* h_recoGenDimuonDeltaR        {new TH1F("h_recoGenDimuonDeltaR",     "Dimuon recoGen deltaR", 50, 0., 1.)};
    TH1F* h_recoGenDimuonMass          {new TH1F("h_recoGenDimuonMass",       "Dimuon recoGen mass", 30, 0., 11.)};
    TH1F* h_recoGenDimuonPt            {new TH1F("h_recoGenDimuonPt",         "Dimuon recoGen Pt",  200, 0., 200)}; 
    TH1F* h_recoGenDimuonEta           {new TH1F("h_recoGenDimuonEta",        "Dimuon recoGen Eta", 60, 0., 5.)};
    TH1F* h_recoGenLeadingMuonPt       {new TH1F("h_recoGenLeadingMuonPt",    "Leading recoGen muon", 300, 0., 150.)};
    TH1F* h_recoGenSubleadingMuonPt    {new TH1F("h_recoGenSubleadingMuonPt", "Subleading recoGen muon", 300, 0., 150.)};

    TH1F* h_recoMuonVx                 {new TH1F("h_recoMuonVx",  "v_{x}",      200, -50., 50.)};
    TH1F* h_recoMuonVy                 {new TH1F("h_recoMuonVy",  "v_{y}",      200, -50., 50.)};
    TH1F* h_recoMuonVz                 {new TH1F("h_recoMuonVz",  "v_{z}",      200, -50., 50.)};
    TH1F* h_recoMuonVxy                {new TH1F("h_recoMuonVxy", "v_{xy}",     200,  0.,  100.)};
    TH1F* h_recoMuonVd                 {new TH1F("h_recoMuonVd",  "v_{d}",      200,  0., 100.)};

    TH1F* h_muonRefittedTkPairChi2Ndof               {new TH1F("h_muonRefittedTkPairChi2Ndof",              "Refitted charged muon track pair #chi^{2}/Ndof", 200, .0, 10.0)};

    TProfile* p_fakeSelected           {new TProfile("p_fakeSelected",  "Debug info for why fake #mu#mu pair was selected", 8, 0.5, 8.5, "ymax = 1.0")};
    p_fakeSelected->GetXaxis()->SetBinLabel(1, "0 #mu from a in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(2, "1 #mu from a in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(3, "2 #mu from a in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(4, "2+ #mu from a in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(5, "0 #mu from #mu in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(6, "1 #mu from #mu in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(7, "2 #mu from #mu in event");
    p_fakeSelected->GetXaxis()->SetBinLabel(8, "2+ #mu from #mu in event");

    TH1F* h_numMatchedChsTracks            {new TH1F("h_numMatchedChsTracks",            "Number of charged hadron PF cands matched to PF objects", 100, -0.5, 99.5)};
    TH1F* h_numMatchedChsTracksFromScalars {new TH1F("h_numMatchedChsTracksFromScalars", "Number of matched charged hadron PF cands from scalars", 100, -0.5, 99.5)};
    TH1F* h_numMatchedChsFromScalarsExcMu  {new TH1F("h_numMatchedChsFromScalarsExcMu",  "Number of matched charged hadron PF cands from scalars (exc selected muons)", 100, -0.5, 99.5)};

    TH1F* h_diScalarDeltaR                      {new TH1F("h_diScalarDeltaR",     "#DeltaR between scalar candidates;#DeltaR", 300, 0., 6.)};
    TH1F* h_diScalarGenDeltaR                   {new TH1F("h_diScalarGenDeltaR",  "#DeltaR between genuine scalar candidates;#DeltaR", 300, 0., 6.)};
    TH1F* h_diScalarFakeDeltaR                  {new TH1F("h_diScalarFakeDeltaR", "#DeltaR between fake scalar candidates;#DeltaR", 300, 0., 6.)};
    TH1F* h_diScalarDeltaR_refittedMuons        {new TH1F("h_diScalarDeltaR_refittedMuons",       "#DeltaR between scalar candidates (refitted #mu#mu);#DeltaR", 500, 0., 10.)};
    TH1F* h_diScalarGenDeltaR_refittedMuons     {new TH1F("h_diScalarGenDeltaR_refittedMuons",    "#DeltaR between genuine scalar candidates (refitted #mu#mu);#DeltaR", 500, 0., 10.)};
    TH1F* h_diScalarDeltaR_refittedMuonsChs     {new TH1F("h_diScalarDeltaR_refittedMuonsChs",    "#DeltaR between scalar candidates (refitted #mumu + qq);#DeltaR", 500, 0., 10.)};
    TH1F* h_diScalarGenDeltaR_refittedMuonsChs  {new TH1F("h_diScalarGenDeltaR_refittedMuonsChs", "#DeltaR between genuine scalar candidates (refitted #mumu + qq);#DeltaR", 500, 0., 10.)};

    TH2F* h_diScalarDeltaRvsPt                  {new TH2F("h_diScalarDeltaRvsPt",        "#DeltaR vs p_{T} between scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarGenDeltaRvsPt               {new TH2F("h_diScalarGenDeltaRvsPt",     "#DeltaR vs p_{T} between genuine scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarFakeDeltaRvsPt              {new TH2F("h_diScalarFakeDeltaRvsPt",    "#DeltaR vs p_{T} between fake scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};

    TH2F* h_diScalarDeltaRvsLepPt               {new TH2F("h_diScalarDeltaRvsLepPt",     "#DeltaR vs (leptonic scalar) p_{T} between scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarGenDeltaRvsLepPt            {new TH2F("h_diScalarGenDeltaRvsLepPt",  "#DeltaR vs (leptonic scalar) p_{T} between genuine scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarFakeDeltaRvsLepPt           {new TH2F("h_diScalarFakeDeltaRvsLepPt", "#DeltaR vs (leptonic scalar) p_{T} between fake scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};

    TH2F* h_diScalarDeltaRvsHadPt               {new TH2F("h_diScalarDeltaRvsHadPt",     "#DeltaR vs (hadronic scalar) p_{T} between scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarGenDeltaRvsHadPt            {new TH2F("h_diScalarGenDeltaRvsHadPt",  "#DeltaR vs (hadronic scalar) p_{T} between genuine scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};
    TH2F* h_diScalarFakeDeltaRvsHadPt           {new TH2F("h_diScalarFakeDeltaRvsHadPt", "#DeltaR vs (hadronic scalar) p_{T} between fake scalar candidates;p_{T};#DeltaR", 200, 0., 100., 300, 0., 6.)};

    TH2F* h_scalarVsScalarPt                    {new TH2F("h_scalarVsScalarPt",          "hadronic vs leptonic scalar p_{T};p_{T} (leptonic);p_{T} (hadronic)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_scalarVsScalarPtGen                 {new TH2F("h_scalarVsScalarPtGen",       "hadronic vs leptonic scalar p_{T} - both hadrons genuine;p_{T} (leptonic);p_{T} (hadronic)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_scalarVsScalarPtFake                {new TH2F("h_scalarVsScalarPtFake",      "hadronic vs leptonic scalar p_{T} - both hadrons fake;p_{T} (leptonic);p_{T} (hadronic)", 200, 0., 100., 200, 0., 100.)};
	
    TProfile* p_selectedChsMatching {new TProfile ("p_selectedChsMatching", "Ancestry of chs cands matched to PAT objects", 6, 0.5, 6.5, "ymax = 1.0")};
    p_selectedChsMatching->GetXaxis()->SetBinLabel(1, "Both tracks genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(2, "Leading track genuine, subleading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(3, "Subleading track genuine, leading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(4, "Both tracks fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(5, "Leading track genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(6, "Subleading track genuine");
    
    TProfile* p_MatchedToPAT {new TProfile ("p_MatchedToPAT", "Chs cands matched to PAT objects", 3, 0.5, 3.5, "ymax = 1.0")};
    p_selectedChsMatching->GetXaxis()->SetBinLabel(1, "Both tracks genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(2, "Leading track genuine, subleading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(3, "Subleading track genuine, leading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(4, "Both tracks fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(5, "Leading track genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(6, "Subleading track genuine");
    
    TH1F* Cutflow {new TH1F ("Cutflow", "Cutflow distribution", 9, 0.5, 9.5)};
    Cutflow->GetXaxis()->SetBinLabel(1, "GeneratedEvts");
    Cutflow->GetXaxis()->SetBinLabel(2, "Gen. chs >= 2");
    Cutflow->GetXaxis()->SetBinLabel(3, "Gen. #mu >= 2");
    Cutflow->GetXaxis()->SetBinLabel(4, "HLT_IsoMu27");
    Cutflow->GetXaxis()->SetBinLabel(5, "MET Filters");
    Cutflow->GetXaxis()->SetBinLabel(6, "#mu selection");
    Cutflow->GetXaxis()->SetBinLabel(7, "dimuon candidate");
    Cutflow->GetXaxis()->SetBinLabel(8, "chs Trk selection");
    Cutflow->GetXaxis()->SetBinLabel(9, "dichs candidate");
    //EvtSelHist->GetXaxis()->SetBinLabel(6, "Higgs window");
    
    
    
    

    TH1F* h_diChsPt                  {new TH1F("h_diChsPt",                "DiChs p_{T}", 200, 0.0, 100.)};
    TH1F* h_diChsPtBothGen           {new TH1F("h_diChsPtBothGen",         "DiChs p_{T}", 200, 0.0, 100.)};
    TH1F* h_diChsPtLeadingGen        {new TH1F("h_diChsPtLeadingGen",      "DiChs p_{T}", 200, 0.0, 100.)};
    TH1F* h_diChsPtSubleadingGen     {new TH1F("h_diChsPtSubleadingGen",   "DiChs p_{T}", 200, 0.0, 100.)};
    TH1F* h_diChsPtBothFake          {new TH1F("h_diChsPtBothFake",        "DiChs p_{T}", 200, 0.0, 100.)};
    
    TH1F* h_diChsMass                  {new TH1F("h_diChsMass",                "DiChs Mass", 200, 0.0, 11.)};
    TH1F* h_diChsMassBothGen           {new TH1F("h_diChsMassBothGen",         "DiChs Mass", 200, 0.0, 11.)};
    TH1F* h_diChsMassLeadingGen        {new TH1F("h_diChsMassLeadingGen",      "DiChs Mass", 200, 0.0, 11.)};
    TH1F* h_diChsMassSubleadingGen     {new TH1F("h_diChsMassSubleadingGen",   "DiChs Mass", 200, 0.0, 11.)};
    TH1F* h_diChsMassBothFake          {new TH1F("h_diChsMassBothFake",        "DiChs Mass", 200, 0.0, 11.)};

    TH1F* h_leadingChsPt_truth                  {new TH1F("h_leadingChsPt_truth",     "Leading truth reco Chs; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_subLeadingChsPt_truth               {new TH1F("h_subLeadingChsPt_truth", "SubLeading truth reco Chs; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_leadingChsEta_truth                 {new TH1F("h_leadingMuonEta_truth",     "Leading truth reco Chs; #eta; Events",       300, -3., 3.)};
    TH1F* h_subLeadingChsEta_truth              {new TH1F("h_subLeadingChsEta_truth",  "SubLeading truth reco Chs; #eta; Events",    300, -3., 3.)};
    TH1F* h_ChsdelR_truth                       {new TH1F("h_ChsdelR_truth",               "#DeltaR truth reco; #DeltaR(chs,chs); Events",      100, 0., 1.0)};
    TH1F* h_diChsMass_truth                     {new TH1F("h_diChsMass_truth",         "Mass dist. truth reco; m_{chs,chs}; Events", 200, 0., 10.)}; 
    
    TH1F* h_genleadingChs_TrksInCone_AtTail            {new TH1F("h_genleadingChs_TrksInCone_AtTail", "Hadronic Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_TrksInCone_AtTail         {new TH1F("h_gensubLeadingChs_TrksInCone_AtTail", "Hadronic Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_TrksInCone_AtPeaks            {new TH1F("h_genleadingChs_TrksInCone_AtPeaks", "Hadronic Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_TrksInCone_AtPeaks         {new TH1F("h_gensubLeadingChs_TrksInCone_AtPeaks", "Hadronic Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_TrksInCone_AtTail_Tight            {new TH1F("h_genleadingChs_TrksInCone_AtTail_Tight", "Tight Hadronic Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_TrksInCone_AtTail_Tight         {new TH1F("h_gensubLeadingChs_TrksInCone_AtTail_Tight", "Tight Hadronic Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_TrksInCone_AtPeaks_Tight            {new TH1F("h_genleadingChs_TrksInCone_AtPeaks_Tight", "Tight Hadronic Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_TrksInCone_AtPeaks_Tight         {new TH1F("h_gensubLeadingChs_TrksInCone_AtPeaks_Tight", "Tight Hadronic Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_PhTrksInCone_AtTail            {new TH1F("h_genleadingChs_PhTrksInCone_AtTail", "Photon Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_PhTrksInCone_AtTail         {new TH1F("h_gensubLeadingChs_PhTrksInCone_AtTail", "Photon Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_PhTrksInCone_AtPeaks            {new TH1F("h_genleadingChs_PhTrksInCone_AtPeaks", "Photon Tracks around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_gensubLeadingChs_PhTrksInCone_AtPeaks         {new TH1F("h_gensubLeadingChs_PhTrksInCone_AtPeaks", "Photon Tracks around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{trks}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_genleadingChs_PhGenInCone_AtTail            {new TH1F("h_genleadingChs_PhGenInCone_AtTail", "Gen. Photons around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_gensubLeadingChs_PhGenInCone_AtTail         {new TH1F("h_gensubLeadingChs_PhGenInCone_AtTail", "Gen. Photons around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_genleadingChs_PhGenInCone_AtPeaks            {new TH1F("h_genleadingChs_PhGenInCone_AtPeaks", "Gen. Photons around gen leading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_gensubLeadingChs_PhGenInCone_AtPeaks         {new TH1F("h_gensubLeadingChs_PhGenInCone_AtPeaks", "Gen. Photons around gen subleading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    
    TH1F* h_leadingChs_PhGenInCone_AtTail            {new TH1F("h_leadingChs_PhGenInCone_AtTail", "Gen. Photons around reco leading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_subLeadingChs_PhGenInCone_AtTail         {new TH1F("h_subLeadingChs_PhGenInCone_AtTail", "Gen. Photons around reco subleading hadron (#Delta R < 0.03) for m_{chs,chs}<1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_leadingChs_PhGenInCone_AtPeaks            {new TH1F("h_leadingChs_PhGenInCone_AtPeaks", "Gen. Photons around reco leading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    TH1F* h_subLeadingChs_PhGenInCone_AtPeaks         {new TH1F("h_subLeadingChs_PhGenInCone_AtPeaks", "Gen. Photons around reco subleading hadron (#Delta R < 0.03) for m_{chs,chs}>1.5 GeV; N_{gen}; Events", 101, -0.5, 100.5)};
    
    TH2F* h_diChsMass_truth_dR                  {new TH2F("h_diChsMass_truth_dR","Mass dist. truth reco; m_{chs,chs}; #Delta R_{chs,chs}", 200, 0., 10., 100, 0., 1.)}; 
    
    
    TH2F* h_diChsMass_truth_leadingChsPt_truth      {new TH2F("h_diChsMass_truth_leadingChsPt_truth","Mass dist. truth reco; m_{chs,chs}; Leading CHS trk p_{T} truth (GeV)", 200, 0., 10., 200, 0., 100.)}; 
    TH2F* h_diChsMass_truth_subLeadingChsPt_truth   {new TH2F("h_diChsMass_truth_subLeadingChsPt_truth", "Mass dist. truth reco; m_{chs,chs}; subLeading CHS trk p_{T} truth (GeV)", 200, 0., 10., 200, 0., 100.)};
    TH2F* h_leadingChsPt_truth_genleadingChsPt_AtTail   {new TH2F("h_leadingChsPt_truth_genleadingChsPt_AtTail", "Reco p_{T} and Gen p_{T} at m_{chs,chs}<1.5 GeV; leading CHS trk p_{T} truth (GeV); gen leading had. p_{T} (GeV)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_leadingChsPt_truth_genleadingChsPt_AtPeaks   {new TH2F("h_leadingChsPt_truth_genleadingChsPt_AtPeaks", "Reco p_{T} and Gen p_{T} at m_{chs,chs}>1.5 GeV; leading CHS trk p_{T} truth (GeV); gen leading had. p_{T} (GeV)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_subLeadingChsPt_truth_gensubLeadingChsPt_AtTail   {new TH2F("h_subLeadingChsPt_truth_gensubLeadingChsPt_AtTail", "Reco p_{T} and Gen p_{T} at m_{chs,chs}<1.5 GeV; subleading CHS trk p_{T} truth (GeV); gen subleading had. p_{T} (GeV)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_subLeadingChsPt_truth_gensubLeadingChsPt_AtPeaks   {new TH2F("h_subLeadingChsPt_truth_gensubLeadingChsPt_AtPeaks", "Reco p_{T} and Gen p_{T} at m_{chs,chs}>1.5 GeV; subleading CHS trk p_{T} truth (GeV); gen subleading had. p_{T} (GeV)", 200, 0., 100., 200, 0., 100.)};
    TH2F* h_leadingChsPtDiff_genleadingChsPt_AtTail   {new TH2F("h_leadingChsPtDiff_genleadingChsPt_AtTail", "Gen p_{T} AND p_{T}^{gen} - p_{T}^{reco} (leading trk) at m_{chs,chs}<1.5 GeV; #Delta p_{T}^{gen,reco};gen leading CHS trk p_{T} (GeV)", 160, -20, 20, 200., 0., 100.)}; 
    TH2F* h_subLeadingChsPtDiff_gensubLeadingChsPt_AtTail   {new TH2F("h_subLeadingChsPtDiff_gensubLeadingChsPt_AtTail", "Gen p_{T} AND p_{T}^{gen} - p_{T}^{reco} (subleading trk) at m_{chs,chs}<1.5 GeV; #Delta p_{T}^{gen,reco};gen subleading CHS trk p_{T} (GeV)", 160, -20, 20, 200., 0., 100.)};
    //TH2F* h_diChsMass_truth_subLeadingChsPt_truth   {new TH2F("h_diChsMass_truth_subLeadingChsPt_truth", "Mass dist. truth reco; m_{chs,chs}; subLeading CHS trk p_{T} truth (GeV)", 200, 0., 10., 200, 0., 100.)}; 
    
    TH1F* h_genleadingChsPt_failed                  {new TH1F("h_genleadingChsPt_failed",     "Leading failed gen Chs; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_gensubLeadingChsPt_failed               {new TH1F("h_gensubLeadingChsPt_failed", "SubLeading failed gen Chs; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_genleadingChsEta_failed                 {new TH1F("h_genleadingChsEta_failed",     "Leading failed gen Chs; #eta; Events",       300, -3., 3.)};
    TH1F* h_gensubLeadingChsEta_failed              {new TH1F("h_gensubLeadingChsEta_failed",  "SubLeading failed gen Chs; #eta; Events",    300, -3., 3.)};
    TH1F* h_genChsdelR_failed                       {new TH1F("h_genChsdelR_failed",        "#DeltaR failed gen; #DeltaR(chs,chs); Events",      100, 0., 1.0)};
    TH1F* h_genChsdelR_SamePFCand                   {new TH1F("h_genChsdelR_SamePFCand",   "gen Chs #DeltaR for match to same PFCand.; #DeltaR(chs,chs)", 100, 0., 1.)};
    TH1F* h_gendiChsMass_failed                     {new TH1F("h_gendiChsMass_failed",         "Mass dist. failed gen; m_{chs,chs}; Events", 200, 0., 10.)}; 

    TH1F* h_chsPt1                   {new TH1F("h_chsPt1",                 "Leading charged hadron track p_{T}", 200, 0.0, 100.)};
    TH1F* h_chsPt2                   {new TH1F("h_chsPt2",                 "Subleading charged hadron track p_{T}", 200, 0.0, 100.)};
    TH1F* h_chsPtBothGen1            {new TH1F("h_chsPtBothGen1",          "Leading charged hadron track p_{T} - both tracks genuine", 200, 0.0, 100.)};
    TH1F* h_chsPtBothGen2            {new TH1F("h_chsPtBothGen2",          "Subleading charged hadron track p_{T} - both tracks genuine", 200, 0.0, 100.)};
    TH1F* h_chsPtLeadingGen1         {new TH1F("h_chsPtLeadingGen1",       "Leading charged hadron track p_{T} - leading genuine/subleading fake", 200, 0.0, 100.)};
    TH1F* h_chsPtLeadingGen2         {new TH1F("h_chsPtLeadingGen2",       "Subleading charged hadron track p_{T} - leading genuine/subleading fake", 200, 0.0, 100.)};
    TH1F* h_chsPtSubleadingGen1      {new TH1F("h_chsPtSubleadingGen1",    "Leading charged hadron track p_{T} - subleading genuine/leading fake", 200, 0.0, 100.)};
    TH1F* h_chsPtSubleadingGen2      {new TH1F("h_chsPtSubleadingGen2",    "Subleading charged hadron track p_{T} - subleading genuine/leading fake", 200, 0.0, 100.)};
    TH1F* h_chsPtBothFake1           {new TH1F("h_chsPtBothFake1",         "Leading charged hadron track p_{T} - both tracks fake", 200, 0.0, 100.)};
    TH1F* h_chsPtBothFake2           {new TH1F("h_chsPtBothFake2",         "Subleading charged hadron track p_{T} - both tracks fake", 200, 0.0, 100.)};
    TH1F* h_chsPtGen1                {new TH1F("h_chsPtGen1",              "Leading genuine charged hadron track p_{T}", 200, 0.0, 100.)};
    TH1F* h_chsPtGen2                {new TH1F("h_chsPtGen2",              "Subleading genuine charged hadron track p_{T}", 200, 0.0, 100.)};
    TH1F* h_chsPtFake1               {new TH1F("h_chsPtFake1",             "Leading fake charged hadron track p_{T}", 200, 0.0, 100.)};
    TH1F* h_chsPtFake2               {new TH1F("h_chsPtFake2",             "Subleading fake charged hadron track p_{T}", 200, 0.0, 100.)};

    TH1F* h_chsEta1                  {new TH1F("h_chsEta1",                "Leading charged hadron track #eta", 300, -3.0, 3.0)};
    TH1F* h_chsEta2                  {new TH1F("h_chsEta2",                "Subleading charged hadron track #eta", 300, -3.0, 3.0)};
    TH1F* h_chsEtaBothGen1           {new TH1F("h_chsEtaBothGen1",         "Leading charged hadron track #eta - both tracks genuine", 300, -3.0, 3.0)};
    TH1F* h_chsEtaBothGen2           {new TH1F("h_chsEtaBothGen2",         "Subleading charged hadron track #eta - both tracks genuine", 300, -3.0, 3.0)};
    TH1F* h_chsEtaLeadingGen1        {new TH1F("h_chsEtaLeadingGen1",	   "Leading charged hadron track #eta - leading genuine/subleading fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaLeadingGen2        {new TH1F("h_chsEtaLeadingGen2",	   "Subleading charged hadron track #eta - leading genuine/subleading fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaSubleadingGen1     {new TH1F("h_chsEtaSubleadingGen1",   "Leading charged hadron track #eta - subleading genuine/leading fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaSubleadingGen2     {new TH1F("h_chsEtaSubleadingGen2",   "Subleading charged hadron track #eta - subleading genuine/leading fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaBothFake1          {new TH1F("h_chsEtaBothFake1",        "Leading charged hadron track #eta - both tracks fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaBothFake2          {new TH1F("h_chsEtaBothFake2",        "Subleading charged hadron track #eta - both tracks fake", 300, -3.0, 3.0)};
    TH1F* h_chsEtaGen1               {new TH1F("h_chsEtaGen1",             "Leading genuine charged hadron track #eta", 300, -3.0, 3.0)};
    TH1F* h_chsEtaGen2               {new TH1F("h_chsEtaGen2",             "Subleading genuine charged hadron track #eta", 300, -3.0, 3.0)};
    TH1F* h_chsEtaFake1              {new TH1F("h_chsEtaFake1",            "Leading fake charged hadron track #eta", 300, -3.0, 3.0)};
    TH1F* h_chsEtaFake2              {new TH1F("h_chsEtaFake2",            "Subleading fake charged hadron track #eta", 300, -3.0, 3.0)};

    TH1F* h_chsIso1                  {new TH1F("h_chsIso1",                "Leading charged hadron iso", 600, .0, 30.0)};
    TH1F* h_chsIso2                  {new TH1F("h_chsIso2",                "Subleading charged hadron iso", 600, .0, 30.0)};
    TH1F* h_chsIsoBothGen1           {new TH1F("h_chsIsoBothGen1",         "Leading charged hadron iso - both tracks genuine", 600, .0, 30.0)};
    TH1F* h_chsIsoBothGen2           {new TH1F("h_chsIsoBothGen2",         "Subleading charged hadron iso - both tracks genuine", 600, .0, 30.0)};
    TH1F* h_chsIsoLeadingGen1        {new TH1F("h_chsIsoLeadingGen1",	   "Leading charged hadron iso - leading genuine/subleading fake", 600, .0, 30.0)};
    TH1F* h_chsIsoLeadingGen2        {new TH1F("h_chsIsoLeadingGen2",	   "Subleading charged hadron iso - leading genuine/subleading fake", 600, .0, 30.0)};
    TH1F* h_chsIsoSubleadingGen1     {new TH1F("h_chsIsoSubleadingGen1",   "Leading charged hadron iso - subleading genuine/leading fake", 600, .0, 30.0)};
    TH1F* h_chsIsoSubleadingGen2     {new TH1F("h_chsIsoSubleadingGen2",   "Subleading charged hadron iso - subleading genuine/leading fake", 600, .0, 30.0)};
    TH1F* h_chsIsoBothFake1          {new TH1F("h_chsIsoBothFake1",        "Leading charged hadron iso - both tracks fake", 600, .0, 30.0)};
    TH1F* h_chsIsoBothFake2          {new TH1F("h_chsIsoBothFake2",        "Subleading charged hadron iso - both tracks fake", 600, .0, 30.0)};
    TH1F* h_chsIsoGen1               {new TH1F("h_chsIsoGen1",             "Leading genuine charged hadron iso", 600, .0, 30.0)};
    TH1F* h_chsIsoGen2               {new TH1F("h_chsIsoGen2",             "Subleading genuine charged hadron iso", 600, .0, 30.0)};
    TH1F* h_chsIsoFake1              {new TH1F("h_chsIsoFake1",            "Leading fake charged hadron iso", 600, .0, 30.0)};
    TH1F* h_chsIsoFake2              {new TH1F("h_chsIsoFake2",            "Subleading fake charged hadron iso", 600, .0, 30.0)};

    TH2F* h_chsIsoPt1                {new TH2F("h_chsIsoPt1",              "Leading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPt2                {new TH2F("h_chsIsoPt2",              "Subleading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtBothGen1         {new TH2F("h_chsIsoPtBothGen1",       "Leading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtBothGen2         {new TH2F("h_chsIsoPtBothGen2",       "Subleading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtLeadingGen1      {new TH2F("h_chsIsoPtLeadingGen1",    "Leading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtLeadingGen2      {new TH2F("h_chsIsoPtLeadingGen2",    "Subleading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtSubleadingGen1   {new TH2F("h_chsIsoPtSubleadingGen1", "Leading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtSubleadingGen2   {new TH2F("h_chsIsoPtSubleadingGen2", "Subleading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtFake1            {new TH2F("h_chsIsoPtFake1",          "Leading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoPtFake2            {new TH2F("h_chsIsoPtFake2",          "Subleading charged hadron iso vs p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};

    TH2F* h_chsIsoScalarPt1                {new TH2F("h_chsIsoScalarPt1",              "Leading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPt2                {new TH2F("h_chsIsoScalarPt2",              "Subleading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtBothGen1         {new TH2F("h_chsIsoScalarPtBothGen1",       "Leading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtBothGen2         {new TH2F("h_chsIsoScalarPtBothGen2",       "Subleading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtLeadingGen1      {new TH2F("h_chsIsoScalarPtLeadingGen1",    "Leading charged hadron iso vs scalarp_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtLeadingGen2      {new TH2F("h_chsIsoScalarPtLeadingGen2",    "Subleading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtSubleadingGen1   {new TH2F("h_chsIsoScalarPtSubleadingGen1", "Leading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtSubleadingGen2   {new TH2F("h_chsIsoScalarPtSubleadingGen2", "Subleading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtFake1            {new TH2F("h_chsIsoScalarPtFake1",          "Leading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};
    TH2F* h_chsIsoScalarPtFake2            {new TH2F("h_chsIsoScalarPtFake2",          "Subleading charged hadron iso vs scalar p_{T};p_{T};isolation", 200, 0.0, 100., 600, .0, 30.0)};

    TH1F* h_chsDeltaR                {new TH1F("h_chsDeltaR",              "#DeltaR between selected chs tracks", 50, 0., 1.)};
    TH1F* h_chsBothGenDeltaR         {new TH1F("h_chsBothGenDeltaR",       "#DeltaR between both genuine chs tracks", 50, 0., 1.)};
    TH1F* h_chsLeadingGenDeltaR      {new TH1F("h_chsLeadingGenDeltaR",    "#DeltaR between leading gen/subleading fake chs tracks", 50, 0., 1.)};
    TH1F* h_chsSubleadingGenDeltaR   {new TH1F("h_chsSubleadingGenDeltaR", "#DeltaR between leading fake/subleading gen chs tracks", 50, 0., 1.)};
    TH1F* h_chsBothFakeDeltaR        {new TH1F("h_chsBothFakeDeltaR",      "#DeltaR between both fake chs tracks", 50, 0., 1.)};

    TH1F* h_chsVx                            {new TH1F("h_chsVx",            "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVy                            {new TH1F("h_chsVy",            "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVz                            {new TH1F("h_chsVz",            "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxy                           {new TH1F("h_chsVxy",           "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVd                            {new TH1F("h_chsVd",            "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVxSig                         {new TH1F("h_chsVxSig",         "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVySig                         {new TH1F("h_chsVySig",         "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzSig                         {new TH1F("h_chsVzSig",         "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxySig                        {new TH1F("h_chsVxySig",        "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdSig                         {new TH1F("h_chsVdSig",         "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVtxAngleXY                    {new TH1F("h_chsVtxAngleXY",    "angle XY", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXY                  {new TH1F("h_chsVtxDistSigXY",  "angle XY signficance", 100, 0., 1.)};
    TH1F* h_chsVtxAngleXYZ                   {new TH1F("h_chsVtxAngleXYZ",   "angle XYZ", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYZ                 {new TH1F("h_chsVtxDistSigXYZ", "angle XYZ signficance", 100, 0., 1.)};

    TH1F* h_chsVxBothGen                     {new TH1F("h_chsVxBothGen",        "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVyBothGen                     {new TH1F("h_chsVyBothGen",        "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzBothGen                     {new TH1F("h_chsVzBothGen",        "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxyBothGen                    {new TH1F("h_chsVxyBothGen",       "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdBothGen                     {new TH1F("h_chsVdBothGen",        "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVxSigGen                      {new TH1F("h_chsVxSigGen",         "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVySigGen                      {new TH1F("h_chsVySigGen",         "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzSigGen                      {new TH1F("h_chsVzSigGen",         "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxySigGen                     {new TH1F("h_chsVxySigGen",        "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdSigGen                      {new TH1F("h_chsVdSigGen",         "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVtxAngleXYGen                 {new TH1F("h_chsVtxAngleXYGen",    "angle XY", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYGen               {new TH1F("h_chsVtxDistSigXYGen",  "angle XY signficance", 100, 0., 1.)};
    TH1F* h_chsVtxAngleXYZGen                {new TH1F("h_chsVtxAngleXYZGen",   "angle XYZ", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYZGen              {new TH1F("h_chsVtxDistSigXYZGen", "angle XYZ signficance", 100, 0., 1.)};

    TH1F* h_chsVxComb                        {new TH1F("h_chsVxComb",            "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVyComb                        {new TH1F("h_chsVyComb",            "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzComb                        {new TH1F("h_chsVzComb",            "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxyComb                       {new TH1F("h_chsVxyComb",           "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdComb                        {new TH1F("h_chsVdComb",            "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVxSigComb                     {new TH1F("h_chsVxSigComb",         "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVySigComb                     {new TH1F("h_chsVySigComb",         "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzSigComb                     {new TH1F("h_chsVzSigComb",         "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxySigComb                    {new TH1F("h_chsVxySigComb",        "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdSigComb                     {new TH1F("h_chsVdSigComb",         "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVtxAngleXYComb                {new TH1F("h_chsVtxAngleXYComb",    "angle XY", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYComb              {new TH1F("h_chsVtxDistSigXYComb",  "angle XY signficance", 100, 0., 1.)};
    TH1F* h_chsVtxAngleXYZComb               {new TH1F("h_chsVtxAngleXYZComb",   "angle XYZ", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYZComb             {new TH1F("h_chsVtxDistSigXYZComb", "angle XYZ signficance", 100, 0., 1.)};

    TH1F* h_chsVxBothFake                    {new TH1F("h_chsVxBothFake",        "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVyBothFake                    {new TH1F("h_chsVyBothFake",        "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzBothFake                    {new TH1F("h_chsVzBothFake",        "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxyBothFake                   {new TH1F("h_chsVxyBothFake",       "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdBothFake                    {new TH1F("h_chsVdBothFake",        "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVxSigFake                     {new TH1F("h_chsVxSigFake",         "v_{x}",      200, -50., 50.)};
    TH1F* h_chsVySigFake                     {new TH1F("h_chsVySigFake",         "v_{y}",      200, -50., 50.)};
    TH1F* h_chsVzSigFake                     {new TH1F("h_chsVzSigFake",         "v_{z}",      200, -50., 50.)};
    TH1F* h_chsVxySigFake                    {new TH1F("h_chsVxySigFake",        "v_{xy}",     200,  0.,  100.)};
    TH1F* h_chsVdSigFake                     {new TH1F("h_chsVdSigFake",         "v_{d}",      200,  0., 100.)};
    TH1F* h_chsVtxAngleXYFake                {new TH1F("h_chsVtxAngleXYFake",    "angle XY", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYFake              {new TH1F("h_chsVtxDistSigXYFake",  "angle XY signficance", 100, 0., 1.)};
    TH1F* h_chsVtxAngleXYZFake               {new TH1F("h_chsVtxAngleXYZFake",   "angle XYZ", 250, -1.2, 1.2)};
    TH1F* h_chsVtxDistSigXYZFake             {new TH1F("h_chsVtxDistSigXYZFake", "angle XYZ signficance", 100, 0., 1.)};

    TH1F* h_chsTkChi2Ndof1                          {new TH1F("h_chsTkChi2Ndof1",                "Leading charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsTkChi2Ndof2                          {new TH1F("h_chsTkChi2Ndof2",                "Subleading charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofBothGen1                   {new TH1F("h_chsTkChi2NdofBothGen1",         "Leading charged hadron track #chi^{2}/Ndof - both tracks genuine", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofBothGen2                   {new TH1F("h_chsTkChi2NdofBothGen2",         "Subleading charged hadron track #chi^{2}/Ndof - both tracks genuine", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofLeadingGen1                {new TH1F("h_chsTkChi2NdofLeadingGen1",	 "Leading charged hadron track #chi^{2}/Ndof - leading genuine/subleading fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofLeadingGen2                {new TH1F("h_chsTkChi2NdofLeadingGen2",	 "Subleading charged hadron track #chi^{2}/Ndof - leading genuine/subleading fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofSubleadingGen1             {new TH1F("h_chsTkChi2NdofSubleadingGen1",   "Leading charged hadron track #chi^{2}/Ndof - subleading genuine/leading fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofSubleadingGen2             {new TH1F("h_chsTkChi2NdofSubleadingGen2",   "Subleading charged hadron track #chi^{2}/Ndof - subleading genuine/leading fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofBothFake1                  {new TH1F("h_chsTkChi2NdofBothFake1",        "Leading charged hadron track #chi^{2}/Ndof - both tracks fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofBothFake2                  {new TH1F("h_chsTkChi2NdofBothFake2",        "Subleading charged hadron track #chi^{2}/Ndof - both tracks fake", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofGen1                       {new TH1F("h_chsTkChi2NdofGen1",             "Leading genuine charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofGen2                       {new TH1F("h_chsTkChi2NdofGen2",             "Subleading genuine charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofFake1                      {new TH1F("h_chsTkChi2NdofFake1",            "Leading fake charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsTkChi2NdofFake2                      {new TH1F("h_chsTkChi2NdofFake2",            "Subleading fake charged hadron track #chi^{2}/Ndof", 200, .0, 10.0)};

    TH1F* h_chsRefittedTkChi2Ndof1                  {new TH1F("h_chsRefittedTkChi2Ndof1",                "Leading charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2Ndof2                  {new TH1F("h_chsRefittedTkChi2Ndof2",                "Subleading charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofBothGen1           {new TH1F("h_chsRefittedTkChi2NdofBothGen1",         "Leading charged hadron refitted track #chi^{2}/Ndof - both tracks genuine", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofBothGen2           {new TH1F("h_chsRefittedTkChi2NdofBothGen2",         "Subleading charged hadron refitted track #chi^{2}/Ndof - both tracks genuine", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofLeadingGen1        {new TH1F("h_chsRefittedTkChi2NdofLeadingGen1",	 "Leading charged hadron refitted track #chi^{2}/Ndof - leading genuine/subleading fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofLeadingGen2        {new TH1F("h_chsRefittedTkChi2NdofLeadingGen2",	 "Subleading charged hadron refitted track #chi^{2}/Ndof - leading genuine/subleading fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofSubleadingGen1     {new TH1F("h_chsRefittedTkChi2NdofSubleadingGen1",   "Leading charged hadron refitted track #chi^{2}/Ndof - subleading genuine/leading fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofSubleadingGen2     {new TH1F("h_chsRefittedTkChi2NdofSubleadingGen2",   "Subleading charged hadron refitted track #chi^{2}/Ndof - subleading genuine/leading fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofBothFake1          {new TH1F("h_chsRefittedTkChi2NdofBothFake1",        "Leading charged hadron refitted track #chi^{2}/Ndof - both tracks fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofBothFake2          {new TH1F("h_chsRefittedTkChi2NdofBothFake2",        "Subleading charged hadron refitted track #chi^{2}/Ndof - both tracks fake", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofGen1               {new TH1F("h_chsRefittedTkChi2NdofGen1",             "Leading genuine charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofGen2               {new TH1F("h_chsRefittedTkChi2NdofGen2",             "Subleading genuine charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofFake1              {new TH1F("h_chsRefittedTkChi2NdofFake1",            "Leading fake charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkChi2NdofFake2              {new TH1F("h_chsRefittedTkChi2NdofFake2",            "Subleading fake charged hadron refitted track #chi^{2}/Ndof", 200, .0, 10.0)};

    TH1F* h_chsRefittedTkPairChi2Ndof               {new TH1F("h_chsRefittedTkPairChi2Ndof",              "Refitted charged hadron track pair #chi^{2}/Ndof", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkPairChi2NdofBothGen        {new TH1F("h_chsRefittedTkPairChi2NdofBothGen",       "Refitted charged hadron track pair #chi^{2}/Ndof (Both gen)", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkPairChi2NdofLeadingGen     {new TH1F("h_chsRefittedTkPairChi2NdofLeadingGen",    "Refitted charged hadron track pair #chi^{2}/Ndof (Leading gen)", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkPairChi2NdofSubleadingGen  {new TH1F("h_chsRefittedTkPairChi2NdofSubleadingGen", "Refitted charged hadron track pair #chi^{2}/Ndof (Subleading gen)", 200, .0, 10.0)};
    TH1F* h_chsRefittedTkPairChi2NdofFake           {new TH1F("h_chsRefittedTkPairChi2NdofFake",          "Refitted charged hadron track pair #chi^{2}/Ndof (Both fake)", 200, .0, 10.0)};

    // packed PF cands info

    TProfile* p_packedCandUsage1           {new TProfile("p_packedCandUsage1",     "Pointers to physics objects assoc with leading packed pf cand", 16, 0.5, 16.5)};
    TProfile* p_packedCandUsageGen1        {new TProfile("p_packedCandUsageGen1",  "Pointers to physics objects assoc with genuine leading packed pf cand", 16, 0.5, 16.5)};
    TProfile* p_packedCandUsageFake1       {new TProfile("p_packedCandUsageFake1", "Pointers to physics objects assoc with fake leading packed pf cand", 16, 0.5, 16.5)};
    TProfile* p_packedCandUsage2           {new TProfile("p_packedCandUsage2",     "Pointers to physics objects assoc with subleading packed pf cand", 16, 0.5, 16.5)};
    TProfile* p_packedCandUsageGen2        {new TProfile("p_packedCandUsageGen2",  "Pointers to physics objects assoc with genuine subleading packed pf cand", 16, 0.5, 16.5)};
    TProfile* p_packedCandUsageFake2       {new TProfile("p_packedCandUsageFake2", "Pointers to physics objects assoc with fake subleading packed pf cand", 16, 0.5, 16.5)};

    p_packedCandUsage1->GetXaxis()->SetBinLabel(1,  "None");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsage1->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(1, "None");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsageGen1->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(1, "None");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsageFake1->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    p_packedCandUsage2->GetXaxis()->SetBinLabel(1, "None");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsage2->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(1, "None");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsageGen2->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(1, "None");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(2,  "e");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(3,  "#mu");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(4,  "jets");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(5,  "#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(6,  "e+#mu");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(7,  "e+jets");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(8,  "e+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(9,  "#mu+jets");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(10, "#mu+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(11, "jets+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(12, "e+#mu+jets");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(13, "e+#mu+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(14, "e+jets+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(15, "#mu+jets+#gamma");
    p_packedCandUsageFake2->GetXaxis()->SetBinLabel(16, "e+#mu+jets+#gamma");

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(41528.0),
        "Lumi to scale MC plots to.")(
        "outfile,o",
        po::value<std::string>(&outFileString)->default_value(outFileString),
        "Output file for plots.")(
        ",n",
        po::value<Long64_t>(&nEvents)->default_value(0),
        "The number of events to be run over. All if set to 0.")(
        ",u",
        po::bool_switch(&usePostLepTree),
        "Use post lepton selection trees.")(
        "2016", po::bool_switch(&is2016_), "Use 2016 conditions (SFs, et al.).")(
        "2018", po::bool_switch(&is2018_), "Use 2018 conditions (SFs, et al.).")(
				"flow", po::value<int>(&flow)->default_value(0), "Starting file no. - ")(
				"fhigh", po::value<int>(&fhigh)->default_value(10000), "Ending file no. -")(
				"verbose,v", po::bool_switch(&verbose_), "Print extra info");
    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc;
            return 0;
        }

        po::notify(vm);

        if ( is2016_ && is2018_ ) {
            throw std::logic_error(
                "Default condition is to use 2017. One cannot set "
                "condition to be BOTH 2016 AND 2018! Chose only "
                " one or none!");
        }
    }
    catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    // Some vectors that will be filled in the parsing
    totalLumi = 0;

    try {
        Parser::parse_config(config,
                             datasets,
                             totalLumi,
                             usePostLepTree);
    }
    catch (const std::exception) {
        std::cerr << "ERROR Problem with a confugration file, see previous "
                     "errors for more details. If this is the only error, the "
                     "problem is with the main configuration file."
                  << std::endl;
        throw;
    }

    if (totalLumi == 0.)
        totalLumi = usePreLumi;

    std::cout << "Using lumi: " << totalLumi << std::endl;

    bool datasetFilled{false};

    std::string era {""};
    if (is2016_) era = "2016";
    else if (is2018_) era = "2018";
    else era = "2017";
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};
//    const std::string postLepSelSkimInputDir{std::string{"/user/almorton/HToSS_analysis/postLepSkims"} + era + "/"};

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);

        std::cerr << "Processing dataset " << dataset->name() << std::endl;
        //int flow = 1;
        //int fhigh = 10000; //to be changed to take input from condor

        if (!usePostLepTree) {
            if (!datasetFilled) {
                if (!dataset->fillChain(datasetChain,  flow, fhigh)) {
                    std::cerr << "There was a problem constructing the chain for " << dataset->name() << ". Continuing with next dataset.\n";
                    continue;
                }
                datasetFilled=true;
            }
        }
        else {
            std::cout << postLepSelSkimInputDir + dataset->name() + "mumuSmallSkim.root" << std::endl;
            datasetChain->Add((postLepSelSkimInputDir + dataset->name() + "mumuSmallSkim.root").c_str());
        }

        // extract the dataset weight. MC = (lumi*crossSection)/(totalEvents), data = 1.0
        float datasetWeight{dataset->getDatasetWeight(totalLumi)};
        std::cout << datasetChain->GetEntries() << " number of items in tree. Dataset weight: " << datasetWeight << std::endl;
        if (datasetChain->GetEntries() == 0) {
            std::cout << "No entries in tree, skipping..." << std::endl;
            continue;
        }

        AnalysisEvent event{dataset->isMC(), datasetChain, is2016_, is2018_};

        Long64_t numberOfEvents{datasetChain->GetEntries()};
        if (nEvents && nEvents < numberOfEvents) numberOfEvents = nEvents;
     	
     	  TMVA::gConfig().SetDrawProgressBar(true);	 
        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        totalEvents += numberOfEvents;
        
        
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");

            event.GetEntry(i);
            
            SharedFunctions shf{useMCTruth_};
            
            if (i%10000 == 0) std::cout<<"Event no. - "<<i<<std::endl;
            float eventWeight = 1.;
            //eventWeight *= datasetWeight;
             
            Cutflow->Fill(1, eventWeight);
    //EvtSelHist->GetXaxis()->SetBinLabel(6, "Higgs window");

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
                if (motherId == 9000006) h_scalarEnergy->Fill(event.genParE[k]);
                
                if (verbose_) {
                  std::cout<<"Starting the ladder for particle (id, idx, status, eventno.) - "<<pid<<", "<<k<<", "<<status<<", "<<i<<std::endl;
                  
                }
                
                //if (pid == motherId) std::cout<<"Interesting event! Event no. "<<i<<std::endl;
                /*if (status != 1) {
                  //std::cout<<"Accessing non-final state particle!"<<std::endl;
                  continue;
                }*/ // this is an addition?
                if ((status != 1)&&(verbose_)) {
                  std::cout<<"Non-final state particle (id, status, motherId, daughterId1, daughterId2, eventno.) - "<<event.genParId[k]<<", "<<status<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<i<<std::endl;
                }
								if (((pid == 130)||(pid==310)||(pid==311))&&(verbose_)) {
                  std::cout<<"Kaons (id, status, motherId, daughterId1, daughterId2, eventno.) - "<<event.genParId[k]<<", "<<status<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<i<<std::endl;
                }
                const bool hasScalarAncestry {shf.scalarAncestry(event, k, 9000006, verbose_)}; //checks if at any point, it comes from scalar

                if ( pid == 13 && hasScalarAncestry && status == 1) {
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

            p_genHadronicDecayFractions->Fill(1.0, bool (genPionIndex.size() >= 2) );
            p_genHadronicDecayFractions->Fill(2.0, bool (genKaonIndex.size() >= 2 ) );
            p_genHadronicDecayFractions->Fill(3.0, bool (genKlongIndex.size() >= 2) );
            p_genHadronicDecayFractions->Fill(4.0, bool (genKshortIndex.size() >= 2) );
            p_genHadronicDecayFractions->Fill(5.0, bool (genChargedKaonIndex.size() >= 2) );

            if ((genPionIndex.size() > 2) || (genKaonIndex.size() > 2 )  || (genKshortIndex.size() > 2) || (genChargedKaonIndex.size() > 2) ) std::cout<<"Event with more no. of daughters: "<<i<<std::endl;
            TLorentzVector genMuon1, genMuon2;
            if ( genMuonIndex.size() >= 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genMuonIndex[0]] > event.genParPt[genMuonIndex[1]] ) firstLeading = true;
                if (firstLeading) {
                    genMuon1.SetPtEtaPhiE(event.genParPt[genMuonIndex[0]], event.genParEta[genMuonIndex[0]], event.genParPhi[genMuonIndex[0]], event.genParE[genMuonIndex[0]]);
                    genMuon2.SetPtEtaPhiE(event.genParPt[genMuonIndex[1]], event.genParEta[genMuonIndex[1]], event.genParPhi[genMuonIndex[1]], event.genParE[genMuonIndex[1]]);
                }
                else {
                    genMuon1.SetPtEtaPhiE(event.genParPt[genMuonIndex[1]], event.genParEta[genMuonIndex[1]], event.genParPhi[genMuonIndex[1]], event.genParE[genMuonIndex[1]]);
                    genMuon2.SetPtEtaPhiE(event.genParPt[genMuonIndex[0]], event.genParEta[genMuonIndex[0]], event.genParPhi[genMuonIndex[0]], event.genParE[genMuonIndex[0]]);
                }
                if ( genMuon1.Pt() > looseMuonPtLeading_ && firstLeading ) genMuonSortedIndex.emplace_back(genMuonIndex[0]);
                else if ( genMuon1.Pt() > looseMuonPtLeading_ && !firstLeading ) genMuonSortedIndex.emplace_back(genMuonIndex[1]);
                if ( genMuon2.Pt() > looseMuonPt_ && !firstLeading ) genMuonSortedIndex.emplace_back(genMuonIndex[1]);
                if ( genMuon2.Pt() > looseMuonPt_ && firstLeading ) genMuonSortedIndex.emplace_back(genMuonIndex[0]);
            }

            if ( genMuonSortedIndex.size() >= 2 ) {
                h_genDimuonDeltaR->Fill( genMuon1.DeltaR(genMuon2), eventWeight);
                h_genDimuonMass->Fill( (genMuon1+genMuon2).M(), eventWeight);
                h_genDimuonPt->Fill( (genMuon1+genMuon2).Pt(), eventWeight);
                h_genDimuonEta->Fill( (genMuon1+genMuon2).Eta(), eventWeight);
                h_genLeadingMuonPt->Fill( genMuon1.Pt(), eventWeight);
                h_genSubleadingMuonPt->Fill( genMuon2.Pt(), eventWeight);
            }

            if ( genPionIndex.size() >= 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genPionIndex[0]] > event.genParPt[genPionIndex[1]] ) firstLeading = true;
                TLorentzVector genPion1, genPion2;
                if (firstLeading) {
                    genPion1.SetPtEtaPhiE(event.genParPt[genPionIndex[0]], event.genParEta[genPionIndex[0]], event.genParPhi[genPionIndex[0]], event.genParE[genPionIndex[0]]);
                    genPion2.SetPtEtaPhiE(event.genParPt[genPionIndex[1]], event.genParEta[genPionIndex[1]], event.genParPhi[genPionIndex[1]], event.genParE[genPionIndex[1]]);
                }
                else {
                    genPion1.SetPtEtaPhiE(event.genParPt[genPionIndex[1]], event.genParEta[genPionIndex[1]], event.genParPhi[genPionIndex[1]], event.genParE[genPionIndex[1]]);
                    genPion2.SetPtEtaPhiE(event.genParPt[genPionIndex[0]], event.genParEta[genPionIndex[0]], event.genParPhi[genPionIndex[0]], event.genParE[genPionIndex[0]]);
                }
                //if ( genPion1.Pt() < 0.5 || genPion2.Pt() < 0.5 ) continue;
                h_genDipionDeltaR->Fill( genPion1.DeltaR(genPion2) );
                h_genDipionMass->Fill( (genPion1+genPion2).M() );
                h_genDipionPt->Fill( (genPion1+genPion2).Pt() );
                h_genDipionEta->Fill( (genPion1+genPion2).Eta() );
                h_genLeadingPionPt->Fill( genPion1.Pt() );
                h_genSubleadingPionPt->Fill( genPion2.Pt() );
                h_genLeadingPionPt_genSubleadingPionPt->Fill(genPion1.Pt(), genPion2.Pt());
                h_genLeadingPionEta->Fill( genPion1.Eta() );
                h_genSubleadingPionEta->Fill( genPion2.Eta() );
                h_genLeadingPionPhi->Fill( genPion1.Phi() );
                h_genSubleadingPionPhi->Fill( genPion2.Phi() );
                h_genLeadingVxPion->Fill( event.genParVx[genPionIndex[0]] );
                h_genLeadingVyPion->Fill( event.genParVy[genPionIndex[0]] );
                h_genLeadingVzPion->Fill( event.genParVz[genPionIndex[0]] );
                h_genLeadingVxyPion->Fill( std::sqrt(event.genParVx[genPionIndex[0]]*event.genParVx[genPionIndex[0]] + event.genParVy[genPionIndex[0]]*event.genParVy[genPionIndex[0]]) );
                h_genLeadingVdPion->Fill(  std::sqrt(event.genParVx[genPionIndex[0]]*event.genParVx[genPionIndex[0]] + event.genParVy[genPionIndex[0]]*event.genParVy[genPionIndex[0]] + event.genParVz[genPionIndex[0]]*event.genParVz[genPionIndex[0]]) );
                h_genSubleadingVxPion->Fill( event.genParVx[genPionIndex[1]] );
                h_genSubleadingVyPion->Fill( event.genParVy[genPionIndex[1]] );
                h_genSubleadingVzPion->Fill( event.genParVz[genPionIndex[1]] );
                h_genSubleadingVxyPion->Fill( std::sqrt(event.genParVx[genPionIndex[1]]*event.genParVx[genPionIndex[1]] + event.genParVy[genPionIndex[1]]*event.genParVy[genPionIndex[1]]) );
                h_genSubleadingVdPion->Fill( std::sqrt(event.genParVx[genPionIndex[1]]*event.genParVx[genPionIndex[1]] + event.genParVy[genPionIndex[1]]*event.genParVy[genPionIndex[1]] + event.genParVz[genPionIndex[1]]*event.genParVz[genPionIndex[1]]) );
                if ( genMuonSortedIndex.size() == 2 ) {
                  h_genDiscalarDeltaR_mumu_pipi->Fill( (genMuon1+genMuon2).DeltaR( (genPion1+genPion2) ) );
                  h_genScalarMassRatio->Fill( (genPion1+genPion2).M()/( (genMuon1+genMuon2).M() + 1.0e-06) );
                  h_genScalarMass2D->Fill( (genMuon1+genMuon2).M(), (genPion1+genPion2).M() );
                }
            }

           if ( genKaonIndex.size() == 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genKaonIndex[0]] > event.genParPt[genKaonIndex[1]] ) firstLeading = true;
                TLorentzVector genKaon1, genKaon2;
                if (firstLeading) {
                    genKaon1.SetPtEtaPhiE(event.genParPt[genKaonIndex[0]], event.genParEta[genKaonIndex[0]], event.genParPhi[genKaonIndex[0]], event.genParE[genKaonIndex[0]]);
                    genKaon2.SetPtEtaPhiE(event.genParPt[genKaonIndex[1]], event.genParEta[genKaonIndex[1]], event.genParPhi[genKaonIndex[1]], event.genParE[genKaonIndex[1]]);
                }
                else {
                    genKaon1.SetPtEtaPhiE(event.genParPt[genKaonIndex[1]], event.genParEta[genKaonIndex[1]], event.genParPhi[genKaonIndex[1]], event.genParE[genKaonIndex[1]]);
                    genKaon2.SetPtEtaPhiE(event.genParPt[genKaonIndex[0]], event.genParEta[genKaonIndex[0]], event.genParPhi[genKaonIndex[0]], event.genParE[genKaonIndex[0]]);
                }
                //if ( genKaon1.Pt() < 0.5 || genKaon2.Pt() < 0.5 ) continue;
                h_genDikaonDeltaR->Fill( genKaon1.DeltaR(genKaon2) );
                h_genDikaonMass->Fill( (genKaon1+genKaon2).M() );
                h_genDikaonPt->Fill( (genKaon1+genKaon2).Pt() );
                h_genDikaonEta->Fill( (genKaon1+genKaon2).Eta() );
                h_genLeadingKaonPt->Fill( genKaon1.Pt() );
                h_genSubleadingKaonPt->Fill( genKaon2.Pt() );
                h_genLeadingKaonPt_genSubleadingKaonPt->Fill(genKaon1.Pt(), genKaon2.Pt());
                h_genLeadingKaonEta->Fill( genKaon1.Eta() );
                h_genSubleadingKaonEta->Fill( genKaon2.Eta() );
                h_genLeadingKaonPhi->Fill( genKaon1.Phi() );
                h_genSubleadingKaonPhi->Fill( genKaon2.Phi() );
                h_genLeadingVxKaon->Fill( event.genParVx[genKaonIndex[0]] );
                h_genLeadingVyKaon->Fill( event.genParVy[genKaonIndex[0]] );
                h_genLeadingVzKaon->Fill( event.genParVz[genKaonIndex[0]] );
                h_genLeadingVxyKaon->Fill( std::sqrt(event.genParVx[genKaonIndex[0]]*event.genParVx[genKaonIndex[0]] + event.genParVy[genKaonIndex[0]]*event.genParVy[genKaonIndex[0]]) );
                h_genLeadingVdKaon->Fill( std::sqrt(event.genParVx[genKaonIndex[0]]*event.genParVx[genKaonIndex[0]] + event.genParVy[genKaonIndex[0]]*event.genParVy[genKaonIndex[0]] + event.genParVz[genKaonIndex[0]]*event.genParVz[genKaonIndex[0]]) );
                h_genSubleadingVxKaon->Fill( event.genParVx[genKaonIndex[1]] );
                h_genSubleadingVyKaon->Fill( event.genParVy[genKaonIndex[1]] );
                h_genSubleadingVzKaon->Fill( event.genParVz[genKaonIndex[1]] );
                h_genSubleadingVxyKaon->Fill( std::sqrt(event.genParVx[genKaonIndex[1]]*event.genParVx[genKaonIndex[1]] + event.genParVy[genKaonIndex[1]]*event.genParVy[genKaonIndex[1]]) );
                h_genSubleadingVdKaon->Fill( std::sqrt(event.genParVx[genKaonIndex[1]]*event.genParVx[genKaonIndex[1]] + event.genParVy[genKaonIndex[1]]*event.genParVy[genKaonIndex[1]] + event.genParVz[genKaonIndex[1]]*event.genParVz[genKaonIndex[1]]) );
                if ( genMuonSortedIndex.size() == 2 ) {
                  h_genDiscalarDeltaR_mumu_kaonkaon->Fill( (genMuon1+genMuon2).DeltaR( (genKaon1+genKaon2) ) );
                  h_genScalarMassRatio->Fill( (genKaon1+genKaon2).M()/( (genMuon1+genMuon2).M() + 1.0e-06) );
                  h_genScalarMass2D->Fill( (genMuon1+genMuon2).M(), (genKaon1+genKaon2).M() );
                }
            }
            
            if ( genChsIndex.size() >= 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ) firstLeading = true;
                TLorentzVector genChs1, genChs2; //genChs1 is leading, genChs2 is subleading
                if (firstLeading) {
                    genChs1.SetPtEtaPhiE(event.genParPt[genChsIndex[0]], event.genParEta[genChsIndex[0]], event.genParPhi[genChsIndex[0]], event.genParE[genChsIndex[0]]);
                    genChs2.SetPtEtaPhiE(event.genParPt[genChsIndex[1]], event.genParEta[genChsIndex[1]], event.genParPhi[genChsIndex[1]], event.genParE[genChsIndex[1]]);
                }
                else {
                    genChs1.SetPtEtaPhiE(event.genParPt[genChsIndex[1]], event.genParEta[genChsIndex[1]], event.genParPhi[genChsIndex[1]], event.genParE[genChsIndex[1]]);
                    genChs2.SetPtEtaPhiE(event.genParPt[genChsIndex[0]], event.genParEta[genChsIndex[0]], event.genParPhi[genChsIndex[0]], event.genParE[genChsIndex[0]]);
                }
                //if ( genChs1.Pt() < 0.5 || genChs2.Pt() < 0.5 ) continue;
                h_genDiChsDeltaR->Fill( genChs1.DeltaR(genChs2) );
                h_genDiChsMass->Fill( (genChs1+genChs2).M() );
                h_genDiChsPt->Fill( (genChs1+genChs2).Pt() );
                h_genDiChsEta->Fill( (genChs1+genChs2).Eta() );
                 
                h_genLeadingChsPt->Fill( genChs1.Pt() );
                h_genSubleadingChsPt->Fill( genChs2.Pt() );
                h_genLeadingChsEta->Fill( genChs1.Eta() );
                h_genSubleadingChsEta->Fill( genChs2.Eta() );
                h_genLeadingChsPhi->Fill( genChs1.Phi() );
                h_genSubleadingChsPhi->Fill( genChs2.Phi() );
                h_genLeadingVxChs->Fill( event.genParVx[genChsIndex[0]] );
                h_genLeadingVyChs->Fill( event.genParVy[genChsIndex[0]] );
                h_genLeadingVzChs->Fill( event.genParVz[genChsIndex[0]] );
                h_genLeadingVxyChs->Fill( std::sqrt(event.genParVx[genChsIndex[0]]*event.genParVx[genChsIndex[0]] + event.genParVy[genChsIndex[0]]*event.genParVy[genChsIndex[0]]) );
                h_genLeadingVdChs->Fill( std::sqrt(event.genParVx[genChsIndex[0]]*event.genParVx[genChsIndex[0]] + event.genParVy[genChsIndex[0]]*event.genParVy[genChsIndex[0]] + event.genParVz[genChsIndex[0]]*event.genParVz[genChsIndex[0]]) );
                h_genSubleadingVxChs->Fill( event.genParVx[genChsIndex[1]] );
                h_genSubleadingVyChs->Fill( event.genParVy[genChsIndex[1]] );
                h_genSubleadingVzChs->Fill( event.genParVz[genChsIndex[1]] );
                h_genSubleadingVxyChs->Fill( std::sqrt(event.genParVx[genChsIndex[1]]*event.genParVx[genChsIndex[1]] + event.genParVy[genChsIndex[1]]*event.genParVy[genChsIndex[1]]) );
                h_genSubleadingVdChs->Fill( std::sqrt(event.genParVx[genChsIndex[1]]*event.genParVx[genChsIndex[1]] + event.genParVy[genChsIndex[1]]*event.genParVy[genChsIndex[1]] + event.genParVz[genChsIndex[1]]*event.genParVz[genChsIndex[1]]) );
                if ( genMuonSortedIndex.size() == 2 ) {
                  h_genDiscalarDeltaR_mumu_ChsChs->Fill( (genMuon1+genMuon2).DeltaR( (genChs1+genChs2) ) );
                  h_genScalarMassRatio->Fill( (genChs1+genChs2).M()/( (genMuon1+genMuon2).M() + 1.0e-06) );
                  h_genScalarMass2D->Fill( (genMuon1+genMuon2).M(), (genChs1+genChs2).M() );
                }
                
            }
            
            if ( genChsIndex.size() < 2 ) continue;
            Cutflow->Fill(2, eventWeight);
            if (genMuonIndex.size() < 2) continue;
            Cutflow->Fill(3, eventWeight);
            
            bool firstLeading {false};
                if ( event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ) firstLeading = true;
                TLorentzVector genChs1, genChs2; //genChs1 is leading, genChs2 is subleading
                if (firstLeading) {
                    genChs1.SetPtEtaPhiE(event.genParPt[genChsIndex[0]], event.genParEta[genChsIndex[0]], event.genParPhi[genChsIndex[0]], event.genParE[genChsIndex[0]]);
                    genChs2.SetPtEtaPhiE(event.genParPt[genChsIndex[1]], event.genParEta[genChsIndex[1]], event.genParPhi[genChsIndex[1]], event.genParE[genChsIndex[1]]);
                }
                else {
                    genChs1.SetPtEtaPhiE(event.genParPt[genChsIndex[1]], event.genParEta[genChsIndex[1]], event.genParPhi[genChsIndex[1]], event.genParE[genChsIndex[1]]);
                    genChs2.SetPtEtaPhiE(event.genParPt[genChsIndex[0]], event.genParEta[genChsIndex[0]], event.genParPhi[genChsIndex[0]], event.genParE[genChsIndex[0]]);
                }
            
            double match_dR = 0.03; //0.03 taken from MuonAnalyzer code used by MuonPOG
            //int mu1=-99, mu2=-99;
            if ((genChsIndex.size() >= 2) && (event.numPackedCands>=2)) { //Condition needed to avoid segmentation fault
              int MatchedChs1Index=-99, MatchedChs2Index=-99;
              if (firstLeading) {
                MatchedChs1Index = shf.MatchReco(genChsIndex[0], event, match_dR);
                MatchedChs2Index = shf.MatchReco(genChsIndex[1], event, match_dR);
              }
              else {
                MatchedChs1Index = shf.MatchReco(genChsIndex[1], event, match_dR);
                MatchedChs2Index = shf.MatchReco(genChsIndex[0], event, match_dR);
              }
              
              if ((MatchedChs1Index==MatchedChs2Index) && (MatchedChs1Index!=-99)) {
                std::cout<<"Matched to same recos"<<std::endl;
                std::cout<<"Indices-"<<genChsIndex[0]<<", "<<genChsIndex[1]<<std::endl;
                std::cout<<"Pt of gen-"<<event.genParPt[genChsIndex[0]]<<", "<<event.genParPt[genChsIndex[1]]<<std::endl;
                std::cout<<"Chs1, Chs2: "<<MatchedChs1Index<<", "<<MatchedChs2Index<<std::endl;
                h_genChsdelR_SamePFCand->Fill(genChs1.DeltaR(genChs2));
                //continue;
              }
              
              if ((MatchedChs1Index<0)||(MatchedChs2Index<0)) {
                
                   h_genleadingChsPt_failed->Fill(genChs1.Pt());
                   h_gensubLeadingChsPt_failed->Fill(genChs2.Pt());
                   h_genleadingChsEta_failed->Fill(genChs1.Eta());
                   h_gensubLeadingChsEta_failed->Fill(genChs2.Eta());
                   h_genChsdelR_failed->Fill(genChs1.DeltaR(genChs2));
                   h_gendiChsMass_failed->Fill((genChs1+genChs2).M());
                   
                   
                    //checking if pT ordered or not
                    TLorentzVector packedCand1 {event.packedCandsPx[0], event.packedCandsPy[0], event.packedCandsPz[0], event.packedCandsE[0]};
                    TLorentzVector packedCand2 {event.packedCandsPx[1], event.packedCandsPy[1], event.packedCandsPz[1], event.packedCandsE[1]};
                    
                    
                   //std::cout<<"Gen Chs Pt: (Pt1,Pt2,PtReco1,PtReco2): "<<genChs1.Pt()<<", "<<genChs2.Pt()<<", "<<packedCand1.Pt()<<", "<<packedCand2.Pt()<<std::endl;
                    //TLorentzVector packedCand3 {event.packedCandsPx[2], event.packedCandsPy[2], event.packedCandsPz[2], event.packedCandsE[2]};
                    for (int j=1; j<event.numPackedCands; j++) {
                      TLorentzVector packedCand {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
                      //if ((packedCand.Pt() > packedCand1.Pt()) || (packedCand.Pt() > packedCand2.Pt()) || (packedCand.Pt() > packedCand3.Pt()))
                      //if ((packedCand.Pt() > packedCand1.Pt()) || (packedCand.Pt() > packedCand2.Pt()))
                        //std::cout<<"Not Pt-ordered (Pt1,Pt2,Pt3,PtX): "<<packedCand1.Pt()<<", "<<packedCand2.Pt()<<", "<<packedCand3.Pt()<<", "<<packedCand.Pt()<<std::endl;
                        //std::cout<<"Not Pt-ordered (Pt1,Pt2,Pt3,PtX): "<<packedCand1.Pt()<<", "<<packedCand2.Pt()<<", "<<packedCand.Pt()<<std::endl;
                      //if ((packedCand.DeltaR(genChs1)<0.4) || (packedCand.DeltaR(genChs2)<0.4))
                        //std::cout<<"Pt: (genPt1,genPt2,PtX, dR1, dR2): "<<genChs1.Pt()<<", "<<genChs2.Pt()<<", "<<packedCand.Pt()<<", "<<packedCand.DeltaR(genChs1)<<", "<<packedCand.DeltaR(genChs2)<<std::endl;
                        
                    }
                  
                  
                  //std::cout<<"Matched to same recos"<<std::endl;
                  //std::cout<<"Indices-"<<genChsIndex[0]<<", "<<genChsIndex[1]<<std::endl;
                  //std::cout<<"Pt of gen-"<<event.genParPt[genChsIndex[0]]<<", "<<event.genParPt[genChsIndex[1]]<<std::endl;
                  //std::cout<<"Chs1, Chs2: "<<MatchedChs1Index<<", "<<MatchedChs2Index<<std::endl;
                  
                  
                   
                  }
              else {
                //std::cout<<"Two Matched CHS found"<<std::endl;
                //genChsIndex[0]
                //const int leadingGenChsIndex {genChs1.Pt()>genChs2.Pt() ? MatchedChs1Index : MatchedChs2Index};
                
                //const int subleadingGenChsIndex {genChs1.Pt()>genChs2.Pt() ? MatchedChs2Index : MatchedChs1Index};
                
                const TLorentzVector chs1_truth {event.packedCandsPx[MatchedChs1Index], event.packedCandsPy[MatchedChs1Index], event.packedCandsPz[MatchedChs1Index], event.packedCandsE[MatchedChs1Index]};
                const TLorentzVector chs2_truth {event.packedCandsPx[MatchedChs2Index], event.packedCandsPy[MatchedChs2Index], event.packedCandsPz[MatchedChs2Index], event.packedCandsE[MatchedChs2Index]};
                
                h_leadingChsPt_truth->Fill(chs1_truth.Pt());
                h_subLeadingChsPt_truth->Fill(chs2_truth.Pt());
                h_leadingChsEta_truth->Fill(chs1_truth.Eta());
                h_subLeadingChsEta_truth->Fill(chs2_truth.Pt());
                h_ChsdelR_truth->Fill(chs2_truth.DeltaR(chs1_truth));
                h_diChsMass_truth->Fill((chs2_truth+chs1_truth).M());
                h_diChsMass_truth_dR->Fill((chs2_truth+chs1_truth).M(),chs2_truth.DeltaR(chs1_truth));
                h_diChsMass_truth_leadingChsPt_truth->Fill((chs2_truth+chs1_truth).M(), chs1_truth.Pt());
                h_diChsMass_truth_subLeadingChsPt_truth->Fill((chs2_truth+chs1_truth).M(), chs2_truth.Pt());
                
                if ((chs2_truth+chs1_truth).M()<=1.5) {
                  h_leadingChsPt_truth_genleadingChsPt_AtTail->Fill(chs1_truth.Pt(), genChs1.Pt()); //Note: chs1_truth corresponds to trk matched to leading gen chs
                  h_leadingChsPtDiff_genleadingChsPt_AtTail->Fill(genChs1.Pt() - chs1_truth.Pt(), genChs1.Pt());
                  h_subLeadingChsPt_truth_gensubLeadingChsPt_AtTail->Fill(chs2_truth.Pt(), genChs2.Pt());
                  h_subLeadingChsPtDiff_gensubLeadingChsPt_AtTail->Fill(genChs2.Pt() - chs2_truth.Pt(), genChs2.Pt());
                  
                  h_genleadingChs_TrksInCone_AtTail->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 211, 0.03, true));
                  h_gensubLeadingChs_TrksInCone_AtTail->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]],211, 0.03, true));
                  
                  h_genleadingChs_TrksInCone_AtTail_Tight->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]],211, 0.03, false));
                  h_gensubLeadingChs_TrksInCone_AtTail_Tight->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]],211, 0.03, false));
                  
                  h_genleadingChs_PhTrksInCone_AtTail->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 22, 0.4, true));
                  h_gensubLeadingChs_PhTrksInCone_AtTail->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 22, 0.4, true));
                  
                  h_genleadingChs_PhGenInCone_AtTail->Fill(shf.nGenParsInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 22, 0.4));
                  h_gensubLeadingChs_PhGenInCone_AtTail->Fill(shf.nGenParsInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 22, 0.4));
                  
                  h_leadingChs_PhGenInCone_AtTail->Fill(shf.nGenParsInCone(event, chs1_truth, event.packedCandsCharge[MatchedChs1Index], 22, 0.4));
                  h_subLeadingChs_PhGenInCone_AtTail->Fill(shf.nGenParsInCone(event, chs2_truth, event.packedCandsCharge[MatchedChs2Index], 22, 0.4));
    
                  
                  p_HadronicDecayFractions_AtTail->Fill(1.0, bool (genPionIndex.size() >= 2) );
                  p_HadronicDecayFractions_AtTail->Fill(2.0, bool (genKaonIndex.size() >= 2 ) );
                  p_HadronicDecayFractions_AtTail->Fill(3.0, bool (genKshortIndex.size() >= 2) );
                  p_HadronicDecayFractions_AtTail->Fill(4.0, bool (genChargedKaonIndex.size() >= 2) );
                }
                else {
                  h_leadingChsPt_truth_genleadingChsPt_AtPeaks->Fill(chs1_truth.Pt(), genChs1.Pt()); //Note: chs1_truth corresponds to trk matched to leading gen chs
                  h_subLeadingChsPt_truth_gensubLeadingChsPt_AtPeaks->Fill(chs2_truth.Pt(), genChs2.Pt());
                  h_genleadingChs_TrksInCone_AtPeaks->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 211, 0.03, true));
                  h_gensubLeadingChs_TrksInCone_AtPeaks->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 211, 0.03, true));
                  
                  h_genleadingChs_TrksInCone_AtPeaks_Tight->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 211, 0.03, false));
                  h_gensubLeadingChs_TrksInCone_AtPeaks_Tight->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 211, 0.03, false));
                  
                  
                  h_genleadingChs_PhTrksInCone_AtPeaks->Fill(shf.nTrksInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 22, 0.4, true));
                  h_gensubLeadingChs_PhTrksInCone_AtPeaks->Fill(shf.nTrksInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 22, 0.4, true));
                  h_genleadingChs_PhGenInCone_AtPeaks->Fill(shf.nGenParsInCone(event, genChs1, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[0]] : event.genParCharge[genChsIndex[1]], 22, 0.4));
                  h_gensubLeadingChs_PhGenInCone_AtPeaks->Fill(shf.nGenParsInCone(event, genChs2, event.genParPt[genChsIndex[0]] > event.genParPt[genChsIndex[1]] ? event.genParCharge[genChsIndex[1]] : event.genParCharge[genChsIndex[0]], 22, 0.4));
                  
                  h_leadingChs_PhGenInCone_AtPeaks->Fill(shf.nGenParsInCone(event, chs1_truth, event.packedCandsCharge[MatchedChs1Index], 22, 0.4));
                  h_subLeadingChs_PhGenInCone_AtPeaks->Fill(shf.nGenParsInCone(event, chs2_truth, event.packedCandsCharge[MatchedChs2Index], 22, 0.4));
                }
              }
            }
            
            
            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

//            if (! ( passDimuonTrigger || passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger ) ) continue;
            if ( !passSingleMuonTrigger ) continue;
            Cutflow->Fill(4, eventWeight);
            if (!event.metFilters()) continue;
            Cutflow->Fill(5, eventWeight);
 
            event.muonIndexLoose = shf.getLooseMuons(event);
            std::vector<int> chsIndex = shf.getChargedHadronTracks(event);

            h_numLooseMuons->Fill(event.muonIndexLoose.size());

            if ( event.muonIndexLoose.size() < 2 ) continue;
            
            Cutflow->Fill(6, eventWeight);
/*
            std::cout << "event.muonIndexLoose.size(): " << event.muonIndexLoose.size() << std::endl;
            if (event.muonIndexLoose.size() == 2) {
                std::cout << "deltaR: " << event.zPairLeptons.first.DeltaR(event.zPairLeptons.second) << std::endl;
            }
            else if (event.muonIndexLoose.size() == 3) {
                TLorentzVector lepton1{event.muonPF2PATPX[event.muonIndexLoose[0]], event.muonPF2PATPY[event.muonIndexLoose[0]], event.muonPF2PATPZ[event.muonIndexLoose[0]], event.muonPF2PATE[event.muonIndexLoose[0]]};
                TLorentzVector lepton2{event.muonPF2PATPX[event.muonIndexLoose[1]], event.muonPF2PATPY[event.muonIndexLoose[1]], event.muonPF2PATPZ[event.muonIndexLoose[1]], event.muonPF2PATE[event.muonIndexLoose[1]]};
                TLorentzVector lepton3{event.muonPF2PATPX[event.muonIndexLoose[2]], event.muonPF2PATPY[event.muonIndexLoose[2]], event.muonPF2PATPZ[event.muonIndexLoose[2]], event.muonPF2PATE[event.muonIndexLoose[2]]};

       	       	std::cout << "lep1+lep2 deltaR: " << lepton1.DeltaR(lepton2) << std::endl;
       	       	std::cout << "lep1+lep3 deltaR: " << lepton1.DeltaR(lepton3) << std::endl;
       	       	std::cout << "lep2+lep3 deltaR: " << lepton2.DeltaR(lepton3) << std::endl;

       	    }
*/    
            
            int nScalarMuons {0}, nDirectScalarMuons {0};
            for ( int i = 0; i < event.muonIndexLoose.size(); i++ ) {
                if (event.genMuonPF2PATScalarAncestor[event.muonIndexLoose[i]]) nScalarMuons++; 
                if (event.genMuonPF2PATDirectScalarAncestor[event.muonIndexLoose[i]]) nDirectScalarMuons++; 
            }

            h_numRecoScalarMuons->Fill(nScalarMuons);
            h_numRecoDirectScalarMuons->Fill(nDirectScalarMuons);

            if ( !shf.getDileptonCand( event, event.muonIndexLoose, useMCTruth_ ) ) continue;
            
            Cutflow->Fill(7, eventWeight);
            const double dileptonMass {(event.zPairLeptons.first + event.zPairLeptons.second).M()};
            //if (dileptonMass > scalarMassCut_) continue;

            h_recoDimuonDeltaR->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
            h_recoDimuonMass->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).M() );
            h_recoDimuonPt->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).Pt() );
            h_recoDimuonEta->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Eta() );
            h_recoLeadingMuonPt->Fill( (event.zPairLeptons.first).Pt() );
            h_recoSubleadingMuonPt->Fill( (event.zPairLeptons.second).Pt() );

            // Add comparison between leptonic and hadronic scalar decayed muons
            /*
            const bool muonFromScalar1 {event.genMuonPF2PATScalarAncestor[event.zPairIndex.first]}, muonFromScalar2 {event.genMuonPF2PATScalarAncestor[event.zPairIndex.second]};
            const bool muonDirectFromScalar1 {event.genMuonPF2PATDirectScalarAncestor[event.zPairIndex.first]}, muonDirectFromScalar2 {event.genMuonPF2PATDirectScalarAncestor[event.zPairIndex.second]};

            p_numPromptMuons->Fill( 1.0, event.genMuonPF2PATPromptDecayed[event.zPairIndex.first] );
            p_numPromptMuons->Fill( 1.0, event.genMuonPF2PATPromptDecayed[event.zPairIndex.second] );
            p_numPromptMuons->Fill( 2.0, event.genMuonPF2PATPromptFinalState[event.zPairIndex.first] );
            p_numPromptMuons->Fill( 2.0, event.genMuonPF2PATPromptFinalState[event.zPairIndex.second] );
            p_numPromptMuons->Fill( 3.0, event.genMuonPF2PATHardProcess[event.zPairIndex.first] );
            p_numPromptMuons->Fill( 3.0, event.genMuonPF2PATHardProcess[event.zPairIndex.second] );
            p_numPromptMuons->Fill( 4.0, event.genMuonPF2PATPythiaSixStatusThree[event.zPairIndex.first] );
            p_numPromptMuons->Fill( 4.0, event.genMuonPF2PATPythiaSixStatusThree[event.zPairIndex.second] );
            p_numPromptMuons->Fill( 5.0, int (muonFromScalar1) );
            p_numPromptMuons->Fill( 5.0, int (muonFromScalar2) );
            p_numPromptMuons->Fill( 6.0, int (muonDirectFromScalar1) );
            p_numPromptMuons->Fill( 6.0, int (muonDirectFromScalar2) );

            p_recoSelectedMuonMatching->Fill( 1.0, bool (muonFromScalar1 && muonFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 2.0, bool (muonFromScalar1 && !muonFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 3.0, bool (!muonFromScalar1 && muonFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 4.0, bool (!muonFromScalar1 && !muonFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 5.0, bool (muonDirectFromScalar1 && muonDirectFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 6.0, bool (muonDirectFromScalar1 && !muonDirectFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 7.0, bool (!muonDirectFromScalar1 && muonDirectFromScalar2) );
            p_recoSelectedMuonMatching->Fill( 8.0, bool (!muonDirectFromScalar1 && !muonDirectFromScalar2) );

            bool leadingVeryVeryTightIso {event.muonPF2PATComRelIsodBeta[event.zPairIndex.first] < 0.05 ? true : false};
            bool subleadingVeryVeryTightIso {event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] < 0.05 ? true : false};

            p_leadingMuonIso->Fill( 1.0, event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 2.0, event.muonPF2PATPfIsoLoose[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 3.0, event.muonPF2PATPfIsoMedium[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 4.0, event.muonPF2PATPfIsoTight[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 5.0, event.muonPF2PATPfIsoVeryTight[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 6.0, int(leadingVeryVeryTightIso) );
            p_leadingMuonIso->Fill( 7.0, event.muonPF2PATTkIsoLoose[event.zPairIndex.first] );
            p_leadingMuonIso->Fill( 8.0, event.muonPF2PATTkIsoTight[event.zPairIndex.first] );

            p_subleadingMuonIso->Fill( 1.0, event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 2.0, event.muonPF2PATPfIsoLoose[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 3.0, event.muonPF2PATPfIsoMedium[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 4.0, event.muonPF2PATPfIsoTight[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 5.0, event.muonPF2PATPfIsoVeryTight[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 6.0, int(subleadingVeryVeryTightIso) );
            p_subleadingMuonIso->Fill( 7.0, event.muonPF2PATTkIsoLoose[event.zPairIndex.second] );
            p_subleadingMuonIso->Fill( 8.0, event.muonPF2PATTkIsoTight[event.zPairIndex.second] );

            h_recoGenDimuonDeltaR->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second), eventWeight);
            h_recoGenDimuonMass->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).M(), eventWeight);
            h_recoGenDimuonPt->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).Pt(), eventWeight);
            h_recoGenDimuonEta->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Eta(), eventWeight);
            h_recoGenLeadingMuonPt->Fill( (event.zPairLeptons.first).Pt() );
            h_recoGenSubleadingMuonPt->Fill( (event.zPairLeptons.second).Pt() );

            h_recoMuonVx->Fill( event.muonTkPairPF2PATTkVx[event.mumuTrkIndex] );
            h_recoMuonVy->Fill( event.muonTkPairPF2PATTkVy[event.mumuTrkIndex] );
            h_recoMuonVz->Fill( event.muonTkPairPF2PATTkVz[event.mumuTrkIndex] );
            h_recoMuonVxy->Fill( std::sqrt(event.muonTkPairPF2PATTkVx[event.mumuTrkIndex]*event.muonTkPairPF2PATTkVx[event.mumuTrkIndex] + event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]*event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]) );
            h_recoMuonVd->Fill( std::sqrt(event.muonTkPairPF2PATTkVx[event.mumuTrkIndex]*event.muonTkPairPF2PATTkVx[event.mumuTrkIndex] + event.muonTkPairPF2PATTkVy[event.mumuTrkIndex]*event.muonTkPairPF2PATTkVy[event.mumuTrkIndex] + event.muonTkPairPF2PATTkVz[event.mumuTrkIndex]*event.muonTkPairPF2PATTkVz[event.mumuTrkIndex])  );

            h_muonRefittedTkPairChi2Ndof->Fill(event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex]);

            if ( !muonFromScalar1 && !muonFromScalar2 ) {
                unsigned int nGenuineMuons {0}, nMuonsFromMuons{0};
                for ( auto it = event.muonIndexLoose.begin(); it != event.muonIndexLoose.end(); it++ ){
                    if ( std::abs(event.genMuonPF2PATMotherId[*it]) == 9000006 ) nGenuineMuons++;
                    if ( std::abs(event.genMuonPF2PATMotherId[*it]) == 13 ) nMuonsFromMuons++;
                }
                p_fakeSelected->Fill( 1.0, bool ( nGenuineMuons == 0 ) );
                p_fakeSelected->Fill( 2.0, bool ( nGenuineMuons == 1 ) );
                p_fakeSelected->Fill( 3.0, bool ( nGenuineMuons == 2 ) );
                p_fakeSelected->Fill( 4.0, bool ( nGenuineMuons > 2 ) );
                p_fakeSelected->Fill( 5.0, bool ( nMuonsFromMuons == 0 ) );
                p_fakeSelected->Fill( 6.0, bool ( nMuonsFromMuons == 1 ) );
                p_fakeSelected->Fill( 7.0, bool ( nMuonsFromMuons == 2 ) );
                p_fakeSelected->Fill( 8.0, bool ( nMuonsFromMuons > 2 ) );
            }

            unsigned int nMatchedTracks {0}, nMatchedGenuineTracks {0}, nMatchedGenuineExclusiveTracks {0};
            for (int i = 0; i < chsIndex.size(); i++) {
                bool validEle {event.packedCandsElectronIndex[chsIndex[i]]>-1}, validMuon {event.packedCandsMuonIndex[chsIndex[i]]>-1}, validJet {event.packedCandsJetIndex[chsIndex[i]]>-1}, validPhoton {event.packedCandsPhotonIndex[chsIndex[i]]>-1};
                if ( validEle || validMuon || validJet || validPhoton ) {
                    nMatchedTracks++;
                    if ( validMuon && event.genMuonPF2PATScalarAncestor[event.packedCandsElectronIndex[chsIndex[i]]] > 0 ) { 
                        nMatchedGenuineTracks++;
                        if (event.packedCandsMuonIndex[chsIndex[i]] != event.muonPF2PATPackedCandIndex[event.zPairIndex.first] 
                               && event.packedCandsMuonIndex[chsIndex[i]] != event.muonPF2PATPackedCandIndex[event.zPairIndex.second]) nMatchedGenuineExclusiveTracks++;
                    }
                    else if ( validEle && event.genElePF2PATScalarAncestor[event.packedCandsMuonIndex[chsIndex[i]]] > 0 ) {
                        nMatchedGenuineTracks++;
                        nMatchedGenuineExclusiveTracks++;
                    }
                    else if ( validJet && event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[chsIndex[i]]] > 0 ) {
                        nMatchedGenuineTracks++;
                        nMatchedGenuineExclusiveTracks++;
       	       	    }
                    else if ( validPhoton && event.genPhoPF2PATScalarAncestor[event.packedCandsPhotonIndex[chsIndex[i]]] > 0 ) {
       	       	       	nMatchedGenuineTracks++;
                        nMatchedGenuineExclusiveTracks++;
       	       	    }
                }
            }

            h_numMatchedChsTracks->Fill(nMatchedTracks);

            h_numMatchedChsTracksFromScalars->Fill(nMatchedGenuineTracks);
            h_numMatchedChsFromScalarsExcMu->Fill(nMatchedGenuineExclusiveTracks);
            */

            if ( chsIndex.size() < 2 ) continue;
            
            Cutflow->Fill(8, eventWeight);
            
                        
            if (!shf.getDihadronCand(event, chsIndex, useMCTruth_)) continue;
            
            Cutflow->Fill(9, eventWeight);
            
            //if ((mu1<0)||(mu2<0)) {
              //cout<<"reaches this"<<endl;
            //  continue;
            //}
            

            // CHS bit
            const int idx1 {event.chsPairIndex.first}, idx2 {event.chsPairIndex.second};

            const bool validEle1    {event.packedCandsElectronIndex[idx1] > -1},  validEle2    {event.packedCandsElectronIndex[idx2] > -1};
            const bool validMuon1   {event.packedCandsMuonIndex[idx1]     > -1 }, validMuon2   {event.packedCandsMuonIndex[idx2]     > -1};
            const bool validJet1    {event.packedCandsJetIndex[idx1]      > -1},  validJet2    {event.packedCandsJetIndex[idx2]      > -1};
            const bool validPhoton1 {event.packedCandsPhotonIndex[idx1]   > -1},  validPhoton2 {event.packedCandsPhotonIndex[idx2]   > -1}; 

            const bool leadingValid { (validEle1 || validMuon1 || validJet1 || validPhoton1) };
            const bool subleadingValid { (validEle2 || validMuon2 || validJet2 || validPhoton2) };

            
            //Printing info for truth and fake
            //std::cout<<"      - indices, Pt, Charge, ID"<<std::endl;
            //std::cout<<"Truth -  "<<event.chsPairIndex.first<<", "<<

            if (!leadingValid || !subleadingValid) continue;

            const int eleIndex1    {validEle1    ? event.packedCandsElectronIndex[idx1] : -1}, eleIndex2    {validEle2    ? event.packedCandsElectronIndex[idx2] : -1};
            const int muonIndex1   {validMuon1   ? event.packedCandsMuonIndex[idx1]     : -1}, muonIndex2   {validMuon2   ? event.packedCandsMuonIndex[idx2]     : -1};
            const int jetIndex1    {validJet1    ? event.packedCandsJetIndex[idx1]      : -1}, jetIndex2    {validJet2    ? event.packedCandsJetIndex[idx2]      : -1};
            const int photonIndex1 {validPhoton1 ? event.packedCandsPhotonIndex[idx1]   : -1}, photonIndex2 {validPhoton2 ? event.packedCandsPhotonIndex[idx2]   : -1};

            const bool matchedEle1    {validEle1    ? event.genElePF2PATScalarAncestor[eleIndex1]    : false}, matchedEle2    {validEle2    ? event.genElePF2PATScalarAncestor[eleIndex2]    : false};
            const bool matchedMuon1   {validMuon1   ? event.genMuonPF2PATScalarAncestor[validMuon1]  : false}, matchedMuon2   {validMuon2   ? event.genMuonPF2PATScalarAncestor[validMuon2] : false};
            const bool matchedJet1    {validJet1    ? event.genJetPF2PATScalarAncestor[jetIndex1]    : false}, matchedJet2    {validJet2    ? event.genJetPF2PATScalarAncestor[jetIndex2]  	: false};
//            const bool matchedPhoton1 {validPhoton1 ? event.genPhoPF2PATScalarAncestor[photonIndex1] : false}, matchedPhoton2 {validPhoton2 ? event.genPhoPF2PATScalarAncestor[photonIndex2] : false};

            const float chsDeltaR { float (event.chsPairVec.first.DeltaR(event.chsPairVec.second)) };

            const bool leadingGen    { (matchedEle1 || matchedMuon1 || matchedJet1 /*|| matchedPhoton1*/) };
            const bool subleadingGen { (matchedEle2 || matchedMuon2 || matchedJet2 /*|| matchedPhoton2*/) };

            const TLorentzVector leptonicScalarVec {event.zPairLeptons.first + event.zPairLeptons.second}, hadronicScalarVec {event.chsPairVec.first + event.chsPairVec.second};
            const TLorentzVector refittedLeptonicScalarVec {event.zPairLeptonsRefitted.first + event.zPairLeptonsRefitted.second};
            const TLorentzVector refittedHadronicScalarVec {event.chsPairTrkVecRefitted.first + event.chsPairTrkVecRefitted.second};

            h_diScalarDeltaR->Fill( leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR->Fill( leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( !leadingGen && !subleadingGen ) h_diScalarFakeDeltaR->Fill( leptonicScalarVec.DeltaR(hadronicScalarVec) );
            h_diScalarDeltaR_refittedMuons->Fill( refittedLeptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR_refittedMuons->Fill( refittedLeptonicScalarVec.DeltaR(hadronicScalarVec) );
            h_diScalarDeltaR_refittedMuonsChs->Fill( refittedLeptonicScalarVec.DeltaR(refittedHadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR_refittedMuonsChs->Fill( refittedLeptonicScalarVec.DeltaR(refittedHadronicScalarVec) );

            h_diScalarDeltaRvsPt->Fill( (leptonicScalarVec+hadronicScalarVec).Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen )   h_diScalarGenDeltaRvsPt->Fill(	(leptonicScalarVec+hadronicScalarVec).Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( !leadingGen && !subleadingGen ) h_diScalarFakeDeltaRvsPt->Fill( (leptonicScalarVec+hadronicScalarVec).Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );

            h_diScalarDeltaRvsLepPt->Fill( leptonicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
       	    if ( leadingGen && subleadingGen ) 	 h_diScalarGenDeltaRvsLepPt->Fill( leptonicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
       	    if ( !leadingGen && !subleadingGen ) h_diScalarFakeDeltaRvsLepPt->Fill( leptonicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );

            h_diScalarDeltaRvsHadPt->Fill( hadronicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen )	 h_diScalarGenDeltaRvsHadPt->Fill( hadronicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( !leadingGen && !subleadingGen ) h_diScalarFakeDeltaRvsHadPt->Fill( hadronicScalarVec.Pt(), leptonicScalarVec.DeltaR(hadronicScalarVec) );

            h_scalarVsScalarPt->Fill( leptonicScalarVec.Pt(), hadronicScalarVec.Pt() );
            if ( leadingGen && subleadingGen )   h_scalarVsScalarPtGen->Fill( leptonicScalarVec.Pt(), hadronicScalarVec.Pt() );
            if ( !leadingGen && !subleadingGen ) h_scalarVsScalarPtFake->Fill( leptonicScalarVec.Pt(), hadronicScalarVec.Pt() );

            p_selectedChsMatching->Fill( 1.0, bool (  leadingGen &&  subleadingGen ) );
            p_selectedChsMatching->Fill( 2.0, bool (  leadingGen && !subleadingGen ) );
            p_selectedChsMatching->Fill( 3.0, bool ( !leadingGen &&  subleadingGen ) );
            p_selectedChsMatching->Fill( 4.0, bool ( !leadingGen && !subleadingGen ) );
            p_selectedChsMatching->Fill( 5.0, leadingGen );
            p_selectedChsMatching->Fill( 6.0, subleadingGen );

            h_diChsPt->Fill(hadronicScalarVec.Pt());
            if ( leadingGen &&  subleadingGen) h_diChsPtBothGen->Fill(hadronicScalarVec.Pt());
            if ( leadingGen && !subleadingGen) h_diChsPtLeadingGen->Fill(hadronicScalarVec.Pt());
            if (!leadingGen &&  subleadingGen) h_diChsPtSubleadingGen->Fill(hadronicScalarVec.Pt());
            if (!leadingGen && !subleadingGen) h_diChsPtBothFake->Fill(hadronicScalarVec.Pt());
            
            h_diChsMass->Fill(hadronicScalarVec.M());
            if ( leadingGen &&  subleadingGen) h_diChsMassBothGen->Fill(hadronicScalarVec.M());
            if ( leadingGen && !subleadingGen) h_diChsMassLeadingGen->Fill(hadronicScalarVec.M());
            if (!leadingGen &&  subleadingGen) h_diChsMassSubleadingGen->Fill(hadronicScalarVec.M());
            if (!leadingGen && !subleadingGen) h_diChsMassBothFake->Fill(hadronicScalarVec.M());

            h_chsPt1->Fill(event.chsPairVec.first.Pt());
            h_chsPt2->Fill(event.chsPairVec.second.Pt());
            if ( leadingGen &&  subleadingGen) h_chsPtBothGen1->Fill(event.chsPairVec.first.Pt());
            if ( leadingGen &&  subleadingGen) h_chsPtBothGen2->Fill(event.chsPairVec.second.Pt());
            if ( leadingGen && !subleadingGen) h_chsPtLeadingGen1->Fill(event.chsPairVec.first.Pt());
            if ( leadingGen && !subleadingGen) h_chsPtLeadingGen2->Fill(event.chsPairVec.second.Pt());
            if (!leadingGen &&  subleadingGen) h_chsPtSubleadingGen1->Fill(event.chsPairVec.first.Pt());
            if (!leadingGen &&  subleadingGen) h_chsPtSubleadingGen2->Fill(event.chsPairVec.second.Pt());
            if (!leadingGen && !subleadingGen) h_chsPtBothFake1->Fill(event.chsPairVec.first.Pt());
            if (!leadingGen && !subleadingGen) h_chsPtBothFake2->Fill(event.chsPairVec.second.Pt());
            if (  leadingGen )    h_chsPtGen1->Fill(event.chsPairVec.first.Pt());
            if (  subleadingGen ) h_chsPtGen2->Fill(event.chsPairVec.second.Pt());
            if ( !leadingGen )    h_chsPtFake1->Fill(event.chsPairVec.first.Pt());
            if ( !subleadingGen ) h_chsPtFake2->Fill(event.chsPairVec.second.Pt());

            h_chsIso1->Fill(event.chsPairTrkIso.first);
            h_chsIso2->Fill(event.chsPairTrkIso.second);
            if ( leadingGen &&  subleadingGen) h_chsIsoBothGen1->Fill(event.chsPairTrkIso.first);
            if ( leadingGen &&  subleadingGen) h_chsIsoBothGen2->Fill(event.chsPairTrkIso.second);
            if ( leadingGen && !subleadingGen) h_chsIsoLeadingGen1->Fill(event.chsPairTrkIso.first);
            if ( leadingGen && !subleadingGen) h_chsIsoLeadingGen2->Fill(event.chsPairTrkIso.second);
            if (!leadingGen &&  subleadingGen) h_chsIsoSubleadingGen1->Fill(event.chsPairTrkIso.first);
            if (!leadingGen &&  subleadingGen) h_chsIsoSubleadingGen2->Fill(event.chsPairTrkIso.second);
            if (!leadingGen && !subleadingGen) h_chsIsoBothFake1->Fill(event.chsPairTrkIso.first);
            if (!leadingGen && !subleadingGen) h_chsIsoBothFake2->Fill(event.chsPairTrkIso.second);
            if (  leadingGen )    h_chsIsoGen1->Fill(event.chsPairTrkIso.first);
            if (  subleadingGen ) h_chsIsoGen2->Fill(event.chsPairTrkIso.second);
            if ( !leadingGen )    h_chsIsoFake1->Fill(event.chsPairTrkIso.first);
            if ( !subleadingGen ) h_chsIsoFake2->Fill(event.chsPairTrkIso.second);

            h_chsIsoPt1->Fill(event.chsPairVec.first.Pt(), event.chsPairTrkIso.first);
            h_chsIsoPt2->Fill(event.chsPairVec.second.Pt(), event.chsPairTrkIso.second);
            if ( leadingGen &&  subleadingGen) h_chsIsoPtBothGen1->Fill(event.chsPairVec.first.Pt(), event.chsPairTrkIso.first);
            if ( leadingGen &&  subleadingGen) h_chsIsoPtBothGen2->Fill(event.chsPairVec.second.Pt(), event.chsPairTrkIso.second);
            if ( leadingGen && !subleadingGen) h_chsIsoPtLeadingGen1->Fill(event.chsPairVec.first.Pt(), event.chsPairTrkIso.first);
            if ( leadingGen && !subleadingGen) h_chsIsoPtLeadingGen2->Fill(event.chsPairVec.second.Pt(), event.chsPairTrkIso.second);
            if (!leadingGen &&  subleadingGen) h_chsIsoPtSubleadingGen1->Fill(event.chsPairVec.first.Pt(), event.chsPairTrkIso.first);
            if (!leadingGen &&  subleadingGen) h_chsIsoPtSubleadingGen2->Fill(event.chsPairVec.second.Pt(), event.chsPairTrkIso.second);
            if (!leadingGen && !subleadingGen) h_chsIsoPtFake1->Fill(event.chsPairVec.first.Pt(), event.chsPairTrkIso.first);
            if (!leadingGen && !subleadingGen) h_chsIsoPtFake2->Fill(event.chsPairVec.second.Pt(), event.chsPairTrkIso.second);

            h_chsIsoScalarPt1->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.first);
            h_chsIsoScalarPt2->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.second);
            if ( leadingGen &&  subleadingGen) h_chsIsoScalarPtBothGen1->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.first);
            if ( leadingGen &&  subleadingGen) h_chsIsoScalarPtBothGen2->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.second);
            if ( leadingGen && !subleadingGen) h_chsIsoScalarPtLeadingGen1->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.first);
            if ( leadingGen && !subleadingGen) h_chsIsoScalarPtLeadingGen2->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.second);
            if (!leadingGen &&  subleadingGen) h_chsIsoScalarPtSubleadingGen1->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.first);
            if (!leadingGen &&  subleadingGen) h_chsIsoScalarPtSubleadingGen2->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.second);
            if (!leadingGen && !subleadingGen) h_chsIsoScalarPtFake1->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.first);
            if (!leadingGen && !subleadingGen) h_chsIsoScalarPtFake2->Fill(hadronicScalarVec.Pt(), event.chsPairTrkIso.second);

            h_chsEta1->Fill(event.chsPairVec.first.Eta());
            h_chsEta2->Fill(event.chsPairVec.second.Eta());
            if ( leadingGen &&  subleadingGen) h_chsEtaBothGen1->Fill(event.chsPairVec.first.Eta());
            if ( leadingGen &&  subleadingGen) h_chsEtaBothGen2->Fill(event.chsPairVec.second.Eta());
            if ( leadingGen && !subleadingGen) h_chsEtaLeadingGen1->Fill(event.chsPairVec.first.Eta());
            if ( leadingGen && !subleadingGen) h_chsEtaLeadingGen2->Fill(event.chsPairVec.second.Eta());
            if (!leadingGen &&  subleadingGen) h_chsEtaSubleadingGen1->Fill(event.chsPairVec.first.Eta());
            if (!leadingGen &&  subleadingGen) h_chsEtaSubleadingGen2->Fill(event.chsPairVec.second.Eta());
            if (!leadingGen && !subleadingGen) h_chsEtaBothFake1->Fill(event.chsPairVec.first.Eta());
            if (!leadingGen && !subleadingGen) h_chsEtaBothFake2->Fill(event.chsPairVec.second.Eta());
            if (  leadingGen )    h_chsEtaGen1->Fill(event.chsPairVec.first.Eta());
            if (  subleadingGen ) h_chsEtaGen2->Fill(event.chsPairVec.second.Eta());
            if ( !leadingGen )    h_chsEtaFake1->Fill(event.chsPairVec.first.Eta());
            if ( !subleadingGen ) h_chsEtaFake2->Fill(event.chsPairVec.second.Eta());

            h_chsTkChi2Ndof1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            h_chsTkChi2Ndof2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if ( leadingGen &&  subleadingGen) h_chsTkChi2NdofBothGen1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if ( leadingGen &&  subleadingGen) h_chsTkChi2NdofBothGen2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if ( leadingGen && !subleadingGen) h_chsTkChi2NdofLeadingGen1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if ( leadingGen && !subleadingGen) h_chsTkChi2NdofLeadingGen2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if (!leadingGen &&  subleadingGen) h_chsTkChi2NdofSubleadingGen1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if (!leadingGen &&  subleadingGen) h_chsTkChi2NdofSubleadingGen2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if (!leadingGen && !subleadingGen) h_chsTkChi2NdofBothFake1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if (!leadingGen && !subleadingGen) h_chsTkChi2NdofBothFake2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if (  leadingGen )    h_chsTkChi2NdofGen1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if (  subleadingGen ) h_chsTkChi2NdofGen2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);
            if ( !leadingGen )    h_chsTkChi2NdofFake1->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.first]);
            if ( !subleadingGen ) h_chsTkChi2NdofFake2->Fill(event.packedCandsPseudoTrkChi2Norm[event.chsPairIndex.second]);

            const int chsTkPairIndex {event.chsPairTrkIndex};
            const float refittedChsTk1Chi2Ndof { float (event.chsTkPairTk1Chi2[chsTkPairIndex]/(event.chsTkPairTk1Ndof[chsTkPairIndex] + 1.0e-06)) };
            const float refittedChsTk2Chi2Ndof { float (event.chsTkPairTk2Chi2[chsTkPairIndex]/(event.chsTkPairTk2Ndof[chsTkPairIndex] + 1.0e-06)) };

            h_chsRefittedTkChi2Ndof1->Fill(refittedChsTk1Chi2Ndof);
            h_chsRefittedTkChi2Ndof2->Fill(refittedChsTk2Chi2Ndof);
            if ( leadingGen &&  subleadingGen) h_chsRefittedTkChi2NdofBothGen1->Fill(refittedChsTk1Chi2Ndof);
            if ( leadingGen &&  subleadingGen) h_chsRefittedTkChi2NdofBothGen2->Fill(refittedChsTk2Chi2Ndof);
            if ( leadingGen && !subleadingGen) h_chsRefittedTkChi2NdofLeadingGen1->Fill(refittedChsTk1Chi2Ndof);
            if ( leadingGen && !subleadingGen) h_chsRefittedTkChi2NdofLeadingGen2->Fill(refittedChsTk2Chi2Ndof);
            if (!leadingGen &&  subleadingGen) h_chsRefittedTkChi2NdofSubleadingGen1->Fill(refittedChsTk1Chi2Ndof);
            if (!leadingGen &&  subleadingGen) h_chsRefittedTkChi2NdofSubleadingGen2->Fill(refittedChsTk2Chi2Ndof);
            if (!leadingGen && !subleadingGen) h_chsRefittedTkChi2NdofBothFake1->Fill(refittedChsTk1Chi2Ndof);
            if (!leadingGen && !subleadingGen) h_chsRefittedTkChi2NdofBothFake2->Fill(refittedChsTk2Chi2Ndof);
            if (  leadingGen )    h_chsRefittedTkChi2NdofGen1->Fill(refittedChsTk1Chi2Ndof);
            if (  subleadingGen ) h_chsRefittedTkChi2NdofGen2->Fill(refittedChsTk2Chi2Ndof);
            if ( !leadingGen )    h_chsRefittedTkChi2NdofFake1->Fill(refittedChsTk1Chi2Ndof);
            if ( !subleadingGen ) h_chsRefittedTkChi2NdofFake2->Fill(refittedChsTk2Chi2Ndof);

            h_chsRefittedTkPairChi2Ndof->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);
            if ( leadingGen &&  subleadingGen) h_chsRefittedTkPairChi2NdofBothGen->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);
            if ( leadingGen && !subleadingGen) h_chsRefittedTkPairChi2NdofLeadingGen->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);
            if (!leadingGen &&  subleadingGen) h_chsRefittedTkPairChi2NdofSubleadingGen->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);
            if (!leadingGen && !subleadingGen) h_chsRefittedTkPairChi2NdofFake->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);

            h_chsDeltaR->Fill(chsDeltaR);

            if (  leadingGen &&  subleadingGen )
                h_chsBothGenDeltaR->Fill(chsDeltaR);
            if (  leadingGen && !subleadingGen )
                h_chsLeadingGenDeltaR->Fill(chsDeltaR);
            if ( !leadingGen && subleadingGen )
                h_chsSubleadingGenDeltaR->Fill(chsDeltaR);
            if ( !leadingGen && !subleadingGen ) 
                h_chsBothFakeDeltaR->Fill(chsDeltaR);

            const float vx {event.chsTkPairTkVx[event.chsPairTrkIndex]}, vy {event.chsTkPairTkVy[event.chsPairTrkIndex]}, vz {event.chsTkPairTkVz[event.chsPairTrkIndex]};
            const float vxy {std::sqrt( vx*vx* + vy*vy )}, vd {std::sqrt( vx*vx* + vy*vy + vz*vz )};

            const float vxSig  {vx/( float (event.chsTkPairTkVtxCov00[event.chsPairTrkIndex] + 1.0e-06)) }, vySig {vy/( float (event.chsTkPairTkVtxCov11[event.chsPairTrkIndex] + 1.0e-06)) }, vzSig {vz/( float (event.chsTkPairTkVtxCov22[event.chsPairTrkIndex] + 1.0e-06)) };
            const float vxySig {vxy/( float (std::sqrt(event.chsTkPairTkVtxCov00[event.chsPairTrkIndex]*event.chsTkPairTkVtxCov00[event.chsPairTrkIndex]+event.chsTkPairTkVtxCov11[event.chsPairTrkIndex]*event.chsTkPairTkVtxCov11[event.chsPairTrkIndex] - 2*event.chsTkPairTkVtxCov01[event.chsPairTrkIndex]*event.chsTkPairTkVtxCov10[event.chsPairTrkIndex]) + 1.0e-06)) };
            const float vdSig {0};

            const float angleXY   {event.chsTkPairTkVtxAngleXY[event.chsPairTrkIndex]},  angleXYZ   { float (event.chsTkPairTkVtxDistMagXY[event.chsPairTrkIndex]/(event.chsTkPairTkVtxDistMagXYSigma[event.chsPairTrkIndex]+1.0e-06)) };
            const float distSigXY {event.chsTkPairTkVtxAngleXYZ[event.chsPairTrkIndex]}, distSigXYZ { float (event.chsTkPairTkVtxDistMagXYZ[event.chsPairTrkIndex]/(event.chsTkPairTkVtxDistMagXYZSigma[event.chsPairTrkIndex]+1.0e-06)) };

            h_chsVx->Fill(vx);
            h_chsVy->Fill(vy);
            h_chsVz->Fill(vz);
            h_chsVxy->Fill(vxy);
            h_chsVd->Fill(vd);
            h_chsVxSig->Fill(vxSig);
            h_chsVySig->Fill(vxSig);
            h_chsVzSig->Fill(vxSig);
            h_chsVxySig->Fill(vxSig);
            h_chsVdSig->Fill(vxSig);
            h_chsVtxAngleXY->Fill(angleXY);
       	    h_chsVtxDistSigXY->Fill(distSigXY);
       	    h_chsVtxAngleXYZ->Fill(angleXYZ);
       	    h_chsVtxDistSigXYZ->Fill(distSigXYZ);

            if ( leadingGen &&  subleadingGen ) {
                h_chsVxBothGen->Fill(vx);
                h_chsVyBothGen->Fill(vy);
                h_chsVzBothGen->Fill(vz);
                h_chsVxyBothGen->Fill(vxy);
                h_chsVdBothGen->Fill(vd);
                h_chsVxSigGen->Fill(vxSig);
                h_chsVySigGen->Fill(vySig);
                h_chsVzSigGen->Fill(vzSig);
                h_chsVxySigGen->Fill(vxySig);
                h_chsVdSigGen->Fill(vdSig);
                h_chsVtxAngleXYGen->Fill(angleXY);
                h_chsVtxDistSigXYGen->Fill(distSigXY);
                h_chsVtxAngleXYZGen->Fill(angleXYZ);
                h_chsVtxDistSigXYZGen->Fill(distSigXYZ);
            }
            else if ( (leadingGen && !subleadingGen) || (!leadingGen &&  subleadingGen) ) {
                h_chsVxComb->Fill(vx);
                h_chsVyComb->Fill(vy);
                h_chsVzComb->Fill(vz);
                h_chsVxyComb->Fill(vxy);
                h_chsVdComb->Fill(vd);
                h_chsVxSigComb->Fill(vxSig);
                h_chsVySigComb->Fill(vySig);
                h_chsVzSigComb->Fill(vzSig);
                h_chsVxySigComb->Fill(vxySig);
                h_chsVdSigComb->Fill(vdSig);
                h_chsVtxAngleXYComb->Fill(angleXY);
                h_chsVtxDistSigXYComb->Fill(distSigXY);
                h_chsVtxAngleXYZComb->Fill(angleXYZ);
                h_chsVtxDistSigXYZComb->Fill(distSigXYZ);
            }
            else if (!leadingGen && !subleadingGen) {
                h_chsVxBothFake->Fill(vx);
                h_chsVyBothFake->Fill(vy);
                h_chsVzBothFake->Fill(vz);
                h_chsVxyBothFake->Fill(vxy);
                h_chsVdBothFake->Fill(vd);
                h_chsVxSigFake->Fill(vxSig);
                h_chsVySigFake->Fill(vySig);
                h_chsVzSigFake->Fill(vzSig);
                h_chsVxySigFake->Fill(vxySig);
                h_chsVdSigFake->Fill(vdSig);
                h_chsVtxAngleXYFake->Fill(angleXY);
                h_chsVtxDistSigXYFake->Fill(distSigXY);
                h_chsVtxAngleXYZFake->Fill(angleXYZ);
                h_chsVtxDistSigXYZFake->Fill(distSigXYZ);
            }

            p_packedCandUsage1->Fill( 1.0,  (!validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 2.0,  (validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 3.0,  (!validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 4.0,  (!validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 5.0,  (!validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 6.0,  (validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 7.0,  (validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 8.0,  (validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 9.0,  (!validEle1 && validMuon1 && validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 10.0, (!validEle1 && validMuon1 && !validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 11.0, (!validEle1 && !validMuon1 && validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 12.0, (validEle1 && validMuon1 && validJet1 && !validPhoton1) );
            p_packedCandUsage1->Fill( 13.0, (validEle1 && validMuon1 && !validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 14.0, (validEle1 && !validMuon1 && validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 15.0, (!validEle1 && validMuon1 && validJet1 && validPhoton1) );
            p_packedCandUsage1->Fill( 16.0, (validEle1 && validMuon1 && validJet1 && validPhoton1) );

            p_packedCandUsage2->Fill( 1.0,  (!validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 2.0,  (validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 3.0,  (!validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 4.0,  (!validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 5.0,  (!validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 6.0,  (validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 7.0,  (validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 8.0,  (validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 9.0,  (!validEle2 && validMuon2 && validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 10.0, (!validEle2 && validMuon2 && !validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 11.0, (!validEle2 && !validMuon2 && validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 12.0, (validEle2 && validMuon2 && validJet2 && !validPhoton2) );
            p_packedCandUsage2->Fill( 13.0, (validEle2 && validMuon2 && !validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 14.0, (validEle2 && !validMuon2 && validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 15.0, (!validEle2 && validMuon2 && validJet2 && validPhoton2) );
            p_packedCandUsage2->Fill( 16.0, (validEle2 && validMuon2 && validJet2 && validPhoton2) );


            if ( leadingGen ) { 
                p_packedCandUsageGen1->Fill( 1.0,  (!validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 2.0,  (validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 3.0,  (!validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 4.0,  (!validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 5.0,  (!validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 6.0,  (validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 7.0,  (validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 8.0,  (validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 9.0,  (!validEle1 && validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 10.0, (!validEle1 && validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 11.0, (!validEle1 && !validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 12.0, (validEle1 && validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageGen1->Fill( 13.0, (validEle1 && validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 14.0, (validEle1 && !validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 15.0, (!validEle1 && validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageGen1->Fill( 16.0, (validEle1 && validMuon1 && validJet1 && validPhoton1) );
            }
            else {
                p_packedCandUsageFake1->Fill( 1.0,  (!validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 2.0,  (validEle1 && !validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 3.0,  (!validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 4.0,  (!validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 5.0,  (!validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 6.0,  (validEle1 && validMuon1 && !validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 7.0,  (validEle1 && !validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 8.0,  (validEle1 && !validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 9.0,  (!validEle1 && validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 10.0, (!validEle1 && validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 11.0, (!validEle1 && !validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 12.0, (validEle1 && validMuon1 && validJet1 && !validPhoton1) );
                p_packedCandUsageFake1->Fill( 13.0, (validEle1 && validMuon1 && !validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 14.0, (validEle1 && !validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 15.0, (!validEle1 && validMuon1 && validJet1 && validPhoton1) );
                p_packedCandUsageFake1->Fill( 16.0, (validEle1 && validMuon1 && validJet1 && validPhoton1) );
            }
            if ( subleadingGen ) { 
                p_packedCandUsageGen2->Fill( 1.0,  (!validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 2.0,  (validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 3.0,  (!validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 4.0,  (!validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 5.0,  (!validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 6.0,  (validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 7.0,  (validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 8.0,  (validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 9.0,  (!validEle2 && validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 10.0, (!validEle2 && validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 11.0, (!validEle2 && !validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 12.0, (validEle2 && validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageGen2->Fill( 13.0, (validEle2 && validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 14.0, (validEle2 && !validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 15.0, (!validEle2 && validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageGen2->Fill( 16.0, (validEle2 && validMuon2 && validJet2 && validPhoton2) );
            }
            else {
                p_packedCandUsageFake2->Fill( 1.0,  (!validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 2.0,  (validEle2 && !validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 3.0,  (!validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 4.0,  (!validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 5.0,  (!validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 6.0,  (validEle2 && validMuon2 && !validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 7.0,  (validEle2 && !validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 8.0,  (validEle2 && !validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 9.0,  (!validEle2 && validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 10.0, (!validEle2 && validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 11.0, (!validEle2 && !validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 12.0, (validEle2 && validMuon2 && validJet2 && !validPhoton2) );
                p_packedCandUsageFake2->Fill( 13.0, (validEle2 && validMuon2 && !validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 14.0, (validEle2 && !validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 15.0, (!validEle2 && validMuon2 && validJet2 && validPhoton2) );
                p_packedCandUsageFake2->Fill( 16.0, (validEle2 && validMuon2 && validJet2 && validPhoton2) );
            }

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    p_genHadronicDecayFractions->Write();

    h_scalarEnergy->Write();

    h_genDimuonDeltaR->Write();
    h_genDimuonMass->Write();
    h_genDimuonPt->Write();
    h_genDimuonEta->Write();
    h_genLeadingMuonPt->Write();
    h_genSubleadingMuonPt->Write();

    h_genDipionDeltaR->Write();
    h_genDipionMass->Write();
    h_genDipionPt->Write();
    h_genDipionEta->Write();
    h_genLeadingPionPt->Write();
    h_genSubleadingPionPt->Write();
    h_genLeadingPionPt_genSubleadingPionPt->Write();
    h_genLeadingPionEta->Write();
    h_genSubleadingPionEta->Write();
    h_genLeadingPionPhi->Write();
    h_genSubleadingPionPhi->Write();
    h_genLeadingVxPion->Write();
    h_genLeadingVyPion->Write();
    h_genLeadingVzPion->Write();
    h_genLeadingVxyPion->Write();
    h_genLeadingVdPion->Write();
    h_genSubleadingVxPion->Write();
    h_genSubleadingVyPion->Write();
    h_genSubleadingVzPion->Write();
    h_genSubleadingVxyPion->Write();
    h_genSubleadingVdPion->Write();

    h_genDikaonDeltaR->Write();
    h_genDikaonMass->Write();
    h_genDikaonPt->Write();
    h_genDikaonEta->Write();
    h_genLeadingKaonPt->Write();
    h_genSubleadingKaonPt->Write();
    h_genLeadingKaonPt_genSubleadingKaonPt->Write();
    h_genLeadingKaonEta->Write();
    h_genSubleadingKaonEta->Write();
    h_genLeadingKaonPhi->Write();
    h_genSubleadingKaonPhi->Write();
    h_genLeadingVxKaon->Write();
    h_genLeadingVyKaon->Write();
    h_genLeadingVzKaon->Write();
    h_genLeadingVxyKaon->Write();
    h_genLeadingVdKaon->Write();
    h_genSubleadingVxKaon->Write();
    h_genSubleadingVyKaon->Write();
    h_genSubleadingVzKaon->Write();
    h_genSubleadingVxyKaon->Write();
    h_genSubleadingVdKaon->Write();
    
    h_genDiChsDeltaR->Write();
    h_genDiChsMass->Write();
    h_genDiChsPt->Write();
    h_genDiChsEta->Write();
    h_genLeadingChsPt->Write();
    h_genSubleadingChsPt->Write();
    h_genLeadingChsEta->Write();
    h_genSubleadingChsEta->Write();
    h_genLeadingChsPhi->Write();
    h_genSubleadingChsPhi->Write();
    h_genLeadingVxChs->Write();
    h_genLeadingVyChs->Write();
    h_genLeadingVzChs->Write();
    h_genLeadingVxyChs->Write();
    h_genLeadingVdChs->Write();
    h_genSubleadingVxChs->Write();
    h_genSubleadingVyChs->Write();
    h_genSubleadingVzChs->Write();
    h_genSubleadingVxyChs->Write();
    h_genSubleadingVdChs->Write();

    h_genDiscalarDeltaR_mumu_pipi->Write();
    h_genDiscalarDeltaR_mumu_kaonkaon->Write();
    h_genDiscalarDeltaR_mumu_ChsChs->Write();

    h_genScalarMassRatio->Write();
    h_genScalarMass2D->Write();

    h_numLooseMuons->Write();
    h_numRecoScalarMuons->Write();
    h_numRecoDirectScalarMuons->Write();

    h_recoDimuonDeltaR->Write();
    h_recoDimuonMass->Write();
    h_recoDimuonPt->Write();
    h_recoDimuonEta->Write();
    h_recoLeadingMuonPt->Write();
    h_recoSubleadingMuonPt->Write();

    p_numPromptMuons->Write();
    p_recoSelectedMuonMatching->Write();

    p_leadingMuonIso->Write();
    p_subleadingMuonIso->Write();

    h_recoGenDimuonDeltaR->Write();
    h_recoGenDimuonMass->Write();
    h_recoGenDimuonPt->Write();
    h_recoGenDimuonEta->Write();
    h_recoGenLeadingMuonPt->Write();
    h_recoGenSubleadingMuonPt->Write();

    h_recoMuonVx->Write();
    h_recoMuonVy->Write();
    h_recoMuonVz->Write();
    h_recoMuonVxy->Write();
    h_recoMuonVd->Write();

    h_muonRefittedTkPairChi2Ndof->Write();

    p_fakeSelected->Write();

    h_numMatchedChsTracks->Write();
    h_numMatchedChsTracksFromScalars->Write();
    h_numMatchedChsFromScalarsExcMu->Write();

    h_diScalarDeltaR->Write();
    h_diScalarGenDeltaR->Write();
    h_diScalarFakeDeltaR->Write();
    h_diScalarDeltaR_refittedMuons->Write();
    h_diScalarGenDeltaR_refittedMuons->Write();
    h_diScalarDeltaR_refittedMuonsChs->Write();
    h_diScalarGenDeltaR_refittedMuonsChs->Write();

    h_diScalarDeltaRvsPt->Write();
    h_diScalarGenDeltaRvsPt->Write();
    h_diScalarFakeDeltaRvsPt->Write();
    h_diScalarDeltaRvsLepPt->Write();
    h_diScalarGenDeltaRvsLepPt->Write();
    h_diScalarFakeDeltaRvsLepPt->Write();
    h_diScalarDeltaRvsHadPt->Write();
    h_diScalarGenDeltaRvsHadPt->Write();
    h_diScalarFakeDeltaRvsHadPt->Write();
    h_scalarVsScalarPt->Write();
    h_scalarVsScalarPtGen->Write();
    h_scalarVsScalarPtFake->Write();

    p_selectedChsMatching->Write();

    h_diChsPt->Write();
    h_diChsPtBothGen->Write();
    h_diChsPtLeadingGen->Write();
    h_diChsPtSubleadingGen->Write();
    h_diChsPtBothFake->Write();
    
    h_diChsMass->Write();
    h_diChsMassBothGen->Write();
    h_diChsMassLeadingGen->Write();
    h_diChsMassSubleadingGen->Write();
    h_diChsMassBothFake->Write();
    
    h_leadingChsPt_truth->Write();
    h_subLeadingChsPt_truth->Write();
    h_leadingChsEta_truth->Write();
    h_subLeadingChsEta_truth->Write();
    h_ChsdelR_truth->Write();
    h_diChsMass_truth->Write();
    h_diChsMass_truth_dR->Write();
    h_diChsMass_truth_leadingChsPt_truth->Write();
    h_diChsMass_truth_subLeadingChsPt_truth->Write();
    h_leadingChsPt_truth_genleadingChsPt_AtTail->Write();
    //h_leadingChsPtDiff_genleadingChsPt_AtTail->Write();
    h_subLeadingChsPt_truth_gensubLeadingChsPt_AtTail->Write();
    //h_subLeadingChsPtDiff_gensubLeadingChsPt_AtTail->Write();
    h_leadingChsPt_truth_genleadingChsPt_AtPeaks->Write();
    h_subLeadingChsPt_truth_gensubLeadingChsPt_AtPeaks->Write();
    h_genleadingChs_TrksInCone_AtPeaks->Write();
    h_genleadingChs_TrksInCone_AtTail->Write();
    h_gensubLeadingChs_TrksInCone_AtPeaks->Write();
    h_gensubLeadingChs_TrksInCone_AtTail->Write();
    //h_genleadingChs_TrksInCone_AtPeaks_Tight->Write();
    //h_genleadingChs_TrksInCone_AtTail_Tight->Write();
    //h_gensubLeadingChs_TrksInCone_AtPeaks_Tight->Write();
    //h_gensubLeadingChs_TrksInCone_AtTail_Tight->Write();
    h_genleadingChs_PhTrksInCone_AtTail->Write();
    h_gensubLeadingChs_PhTrksInCone_AtTail->Write();
    h_genleadingChs_PhGenInCone_AtTail->Write();
    h_gensubLeadingChs_PhGenInCone_AtTail->Write();
    h_genleadingChs_PhTrksInCone_AtPeaks->Write();
    h_gensubLeadingChs_PhTrksInCone_AtPeaks->Write();
    h_genleadingChs_PhGenInCone_AtPeaks->Write();
    h_gensubLeadingChs_PhGenInCone_AtPeaks->Write();
    h_leadingChs_PhGenInCone_AtTail->Write();
    h_subLeadingChs_PhGenInCone_AtTail->Write();
    h_leadingChs_PhGenInCone_AtPeaks->Write();
    h_subLeadingChs_PhGenInCone_AtPeaks->Write();
    p_HadronicDecayFractions_AtTail->Write();
    Cutflow->Write();
    
    h_genleadingChsPt_failed->Write();
    h_gensubLeadingChsPt_failed->Write();
    h_genleadingChsEta_failed->Write();
    h_gensubLeadingChsEta_failed->Write();
    h_genChsdelR_failed->Write();
    h_genChsdelR_SamePFCand->Write();
    h_gendiChsMass_failed->Write();

    h_chsPt1->Write();
    h_chsPt2->Write();
    h_chsPtBothGen1->Write();
    h_chsPtBothGen2->Write();
    h_chsPtLeadingGen1->Write();
    h_chsPtLeadingGen2->Write();
    h_chsPtSubleadingGen1->Write();
    h_chsPtSubleadingGen2->Write();
    h_chsPtBothFake1->Write();
    h_chsPtBothFake2->Write();
    h_chsPtGen1->Write();
    h_chsPtGen2->Write();
    h_chsPtFake1->Write();
    h_chsPtFake2->Write();

    h_chsEta1->Write();
    h_chsEta2->Write();
    h_chsEtaBothGen1->Write();
    h_chsEtaBothGen2->Write();
    h_chsEtaLeadingGen1->Write();
    h_chsEtaLeadingGen2->Write();
    h_chsEtaSubleadingGen1->Write();
    h_chsEtaSubleadingGen2->Write();
    h_chsEtaBothFake1->Write();
    h_chsEtaBothFake2->Write();
    h_chsEtaGen1->Write();
    h_chsEtaGen2->Write();
    h_chsEtaFake1->Write();
    h_chsEtaFake2->Write();

    h_chsIso1->Write();
    h_chsIso2->Write();
    h_chsIsoBothGen1->Write();
    h_chsIsoBothGen2->Write();
    h_chsIsoLeadingGen1->Write();
    h_chsIsoLeadingGen2->Write();
    h_chsIsoSubleadingGen1->Write();
    h_chsIsoSubleadingGen2->Write();
    h_chsIsoBothFake1->Write();
    h_chsIsoBothFake2->Write();
    h_chsIsoGen1->Write();
    h_chsIsoGen2->Write();
    h_chsIsoFake1->Write();
    h_chsIsoFake2->Write();

    h_chsIsoPt1->Write();
    h_chsIsoPt2->Write();
    h_chsIsoPtBothGen1->Write();
    h_chsIsoPtBothGen2->Write();
    h_chsIsoPtLeadingGen1->Write();
    h_chsIsoPtLeadingGen2->Write();
    h_chsIsoPtSubleadingGen1->Write();
    h_chsIsoPtSubleadingGen2->Write();
    h_chsIsoPtFake1->Write();
    h_chsIsoPtFake2->Write();

    h_chsIsoScalarPt1->Write();
    h_chsIsoScalarPt2->Write();
    h_chsIsoScalarPtBothGen1->Write();
    h_chsIsoScalarPtBothGen2->Write();
    h_chsIsoScalarPtLeadingGen1->Write();
    h_chsIsoScalarPtLeadingGen2->Write();
    h_chsIsoScalarPtSubleadingGen1->Write();
    h_chsIsoScalarPtSubleadingGen2->Write();
    h_chsIsoScalarPtFake1->Write();
    h_chsIsoScalarPtFake2->Write();

    h_chsDeltaR->Write();
    h_chsBothGenDeltaR->Write();
    h_chsLeadingGenDeltaR->Write();
    h_chsSubleadingGenDeltaR->Write();
    h_chsBothFakeDeltaR->Write();

    h_chsVx->Write();
    h_chsVy->Write();
    h_chsVz->Write();
    h_chsVxy->Write();
    h_chsVd->Write();
    h_chsVxSig->Write();
    h_chsVySig->Write();
    h_chsVzSig->Write();
    h_chsVxySig->Write();
    h_chsVdSig->Write();
    h_chsVtxAngleXY->Write();
    h_chsVtxDistSigXY->Write();
    h_chsVtxAngleXYZ->Write();
    h_chsVtxDistSigXYZ->Write();

    h_chsVxBothGen->Write();
    h_chsVyBothGen->Write();
    h_chsVzBothGen->Write();
    h_chsVxyBothGen->Write();
    h_chsVdBothGen->Write();
    h_chsVxSigGen->Write();
    h_chsVySigGen->Write();
    h_chsVzSigGen->Write();
    h_chsVxySigGen->Write();
    h_chsVdSigGen->Write();
    h_chsVtxAngleXYGen->Write();
    h_chsVtxDistSigXYGen->Write();
    h_chsVtxAngleXYZGen->Write();
    h_chsVtxDistSigXYZGen->Write();

    h_chsVxComb->Write();
    h_chsVyComb->Write();
    h_chsVzComb->Write();
    h_chsVxyComb->Write();
    h_chsVdComb->Write();
    h_chsVxSigComb->Write();
    h_chsVySigComb->Write();
    h_chsVzSigComb->Write();
    h_chsVxySigComb->Write();
    h_chsVdSigComb->Write();
    h_chsVtxAngleXYComb->Write();
    h_chsVtxDistSigXYComb->Write();
    h_chsVtxAngleXYZComb->Write();
    h_chsVtxDistSigXYZComb->Write();

    h_chsVxBothFake->Write();
    h_chsVyBothFake->Write();
    h_chsVzBothFake->Write();
    h_chsVxyBothFake->Write();
    h_chsVdBothFake->Write();
    h_chsVxSigFake->Write();
    h_chsVySigFake->Write();
    h_chsVzSigFake->Write();
    h_chsVxySigFake->Write();
    h_chsVdSigFake->Write();
    h_chsVtxAngleXYFake->Write();
    h_chsVtxDistSigXYFake->Write();
    h_chsVtxAngleXYZFake->Write();
    h_chsVtxDistSigXYZFake->Write();

    h_chsTkChi2Ndof1->Write();
    h_chsTkChi2Ndof2->Write();
    h_chsTkChi2NdofBothGen1->Write();
    h_chsTkChi2NdofBothGen2->Write();
    h_chsTkChi2NdofLeadingGen1->Write();
    h_chsTkChi2NdofLeadingGen2->Write();
    h_chsTkChi2NdofSubleadingGen1->Write();
    h_chsTkChi2NdofSubleadingGen2->Write();
    h_chsTkChi2NdofBothFake1->Write();
    h_chsTkChi2NdofBothFake2->Write();
    h_chsTkChi2NdofGen1->Write();
    h_chsTkChi2NdofGen2->Write();
    h_chsTkChi2NdofFake1->Write();
    h_chsTkChi2NdofFake2->Write();

    h_chsRefittedTkChi2Ndof1->Write();
    h_chsRefittedTkChi2Ndof2->Write();
    h_chsRefittedTkChi2NdofBothGen1->Write();
    h_chsRefittedTkChi2NdofBothGen2->Write();
    h_chsRefittedTkChi2NdofLeadingGen1->Write();
    h_chsRefittedTkChi2NdofLeadingGen2->Write();
    h_chsRefittedTkChi2NdofSubleadingGen1->Write();
    h_chsRefittedTkChi2NdofSubleadingGen2->Write();
    h_chsRefittedTkChi2NdofBothFake1->Write();
    h_chsRefittedTkChi2NdofBothFake2->Write();
    h_chsRefittedTkChi2NdofGen1->Write();
    h_chsRefittedTkChi2NdofGen2->Write();
    h_chsRefittedTkChi2NdofFake1->Write();
    h_chsRefittedTkChi2NdofFake2->Write();

    h_chsRefittedTkPairChi2Ndof->Write();
    h_chsRefittedTkPairChi2NdofBothGen->Write();
    h_chsRefittedTkPairChi2NdofLeadingGen->Write();
    h_chsRefittedTkPairChi2NdofSubleadingGen->Write();
    h_chsRefittedTkPairChi2NdofFake->Write();

    p_packedCandUsage1->Write();
    p_packedCandUsageGen1->Write();
    p_packedCandUsageFake1->Write();
    p_packedCandUsage2->Write();
    p_packedCandUsageGen2->Write();
    p_packedCandUsageFake2->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}
/*
std::vector<int> getLooseMuons(const AnalysisEvent& event) {
    std::vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {
       if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] && std::abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
           if ( event.muonPF2PATPt[i] >= muons.empty() ? looseMuonPtLeading_ : looseMuonPt_) muons.emplace_back(i);
        }
    }
    return muons;
}

std::vector<int> getChargedHadronTracks(const AnalysisEvent& event) {
    std::vector<int> chs;
    for (Int_t k = 0; k < event.numPackedCands; k++) {
        if (std::abs(event.packedCandsPdgId[k]) != 211) continue;
        if (event.packedCandsCharge[k] == 0 ) continue;
        if (event.packedCandsHasTrackDetails[k] != 1 ) continue;
        TLorentzVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
//        if (lVec.Pt() < 5.0) continue;

        chs.emplace_back(k);
    }
    return chs;
}


bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth) {
    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

            if (event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
            if (event.muonPF2PATPt[j] <= looseMuonPt_) continue;

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

bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool mcTruth ) {

    for ( unsigned int i{0}; i < chs.size(); i++ ) {

        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        if ( mcTruth ) {
            if ( event.packedCandsElectronIndex[chs[i]] < 0  && event.packedCandsMuonIndex[chs[i]] < 0 && event.packedCandsJetIndex[chs[i]] < 0 ) continue;
            if ( event.packedCandsElectronIndex[chs[i]] >= 0 && event.genElePF2PATScalarAncestor[event.packedCandsElectronIndex[chs[i]]] < 1 ) continue;
            if ( event.packedCandsMuonIndex[chs[i]] >= 0     && event.genMuonPF2PATScalarAncestor[event.packedCandsMuonIndex[chs[i]]] < 1 ) continue;
            if ( event.packedCandsJetIndex[chs[i]] >= 0      && event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[chs[i]]] < 1 ) continue;
        }

        for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

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

            if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ && pT >= 0. ) {
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

                event.chsPairTrkIndex = getChsTrackPairIndex(event);

//                if ( (event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06) > 20. ) continue;

       	       	// If refit fails then reject event - all signal events	pass refit, but	QCD does not
                if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;

                TLorentzVector chsTrk1Refitted, chsTrk2Refitted;
                chsTrk1Refitted.SetPtEtaPhiE(event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                chsTrk2Refitted.SetPtEtaPhiE(event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                event.chsPairTrkVecRefitted.first  = chsTrk1Refitted;
                event.chsPairTrkVecRefitted.second = chsTrk2Refitted;

                return true;
            }
            else continue;
        }
    }
    return false;
}

int getMuonTrackPairIndex(const AnalysisEvent& event) { 
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] == event.zPairIndex.first && event.muonTkPairPF2PATIndex2[i] == event.zPairIndex.second) return i;
    }
    return -1;
}

int getChsTrackPairIndex(const AnalysisEvent& event) {
    for (int i{0}; i < event.numChsTrackPairs; i++) {
        if (event.chsTkPairIndex1[i] == event.chsPairIndex.first && event.chsTkPairIndex2[i] == event.chsPairIndex.second) return i;
    }
    return -1;
}

bool scalarAncestry (const AnalysisEvent& event, const Int_t k, const Int_t AncestryId) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };

    //std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
    if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
    else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
    else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
    else {
        
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
        return scalarAncestry(event, motherIndex, grandparentId); // otherwise check mother's mother ...
    }
}

int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max) {
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
        //if (fabs(event.packedCandsPdgId[j])!=211) continue; //must be hadronic track
        //if (event.genParCharge[gen_ind]!= event.packedCandsCharge[j]) continue; //must have same charge
        if (minDR < tmpDR) continue;
        
        minDR = tmpDR;
        index = j;    
    }
    if (minDR < dr_max) 
        return index;
    else
        return -99;
}

float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}
*/

