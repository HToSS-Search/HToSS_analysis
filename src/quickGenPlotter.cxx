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


std::vector<int> getLooseMuons(const AnalysisEvent& event);
std::vector<int> getChargedHadronTracks(const AnalysisEvent& event);
bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth = false);
bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs, bool mcTruth = false);
int getMuonTrackPairIndex(const AnalysisEvent& event);
int getChsTrackPairIndex(const AnalysisEvent& event);
bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);
float deltaR(float eta1, float phi1, float eta2, float phi2);

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.8}, looseMuonPt_ {6.}, looseMuonPtLeading_ {15.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0}, chsMass_{0.13957018};

// Diparticle cuts
double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4};
double higgsTolerence_ {10.};


int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Gen Histos

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
    TH1F* h_genLeadingPionPt      {new TH1F("h_genLeadingPionPt",      "Leading generator Pion", 300, 0., 150.)};
    TH1F* h_genSubleadingPionPt   {new TH1F("h_genSubleadingPionPt",   "Subleading generator Pion", 300, 0., 150.)};

    TH1F* h_genDikaonDeltaR       {new TH1F("h_genDikaonDeltaR",       "Dikaon gen deltaR", 50, 0., 1.)};
    TH1F* h_genDikaonMass         {new TH1F("h_genDikaonMass",         "Dikaon gen mass", 30, 0., 11.)};
    TH1F* h_genDikaonPt           {new TH1F("h_genDikaonPt",           "Dikaon gen Pt",  200, 0., 200)}; 
    TH1F* h_genDikaonEta          {new TH1F("h_genDikaonEta",          "Dikaon gen Eta", 60, 0., 5.)};
    TH1F* h_genLeadingKaonPt      {new TH1F("h_genLeadingKaonPt",      "Leading generator Kaon", 300, 0., 150.)};
    TH1F* h_genSubleadingKaonPt   {new TH1F("h_genSubleadingKaonPt",   "Subleading generator Kaon", 300, 0., 150.)};

    TH1F* h_genDiscalarDeltaR_mumu_pipi     {new TH1F("h_genDiscalarDeltaR_mumu_pipi",      "#DeltaR_{#mu#mu#pi#pi}^{gen}", 500, 0., 10.)};
    TH1F* h_genDiscalarDeltaR_mumu_kaonkaon {new TH1F("h_genDiscalarDeltaR_mumu_kaonkaon",  "#DeltaR_{#mu#muKK}^{gen}", 500, 0., 10.)};

    // Reco plots

    TH1F* h_numLooseMuons           {new TH1F("h_numLooseMuons",         "Number of loose muons IDed", 10, -0.5, 9.5)};

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

    TH1F* h_diScalarDeltaR     {new TH1F("h_diScalarDeltaR",    "#DeltaR between scalar candidates", 500, 0., 10.)};
    TH1F* h_diScalarGenDeltaR  {new TH1F("h_diScalarGenDeltaR", "#DeltaR between genuine scalar candidates", 500, 0., 10.)};

    TH1F* h_diScalarDeltaR_refittedMuons        {new TH1F("h_diScalarDeltaR_refittedMuons",       "#DeltaR between scalar candidates (refitted #mu#mu)", 500, 0., 10.)};
    TH1F* h_diScalarGenDeltaR_refittedMuons     {new TH1F("h_diScalarGenDeltaR_refittedMuons",    "#DeltaR between genuine scalar candidates (refitted #mu#mu)", 500, 0., 10.)};

    TH1F* h_diScalarDeltaR_refittedMuonsChs     {new TH1F("h_diScalarDeltaR_refittedMuonsChs",    "#DeltaR between scalar candidates (refitted #mumu + qq)", 500, 0., 10.)};
    TH1F* h_diScalarGenDeltaR_refittedMuonsChs  {new TH1F("h_diScalarGenDeltaR_refittedMuonsChs", "#DeltaR between genuine scalar candidates (refitted #mumu + qq)", 500, 0., 10.)};

    TProfile* p_selectedChsMatching {new TProfile ("p_selectedChsMatching", "Ancestry of chs cands matched to PAT objects", 6, 0.5, 6.5, "ymax = 1.0")};
    p_selectedChsMatching->GetXaxis()->SetBinLabel(1, "Both tracks genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(2, "Leading track genuine, subleading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(3, "Subleading track genuine, leading fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(4, "Both tracks fake");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(5, "Leading track genuine");
    p_selectedChsMatching->GetXaxis()->SetBinLabel(6, "Subleading track genuine");

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

    TH1F* h_chsDeltaR                {new TH1F("h_chsDeltaR",              "#DeltaR between selected chs tracks", 50, 0., 1.)};
    TH1F* h_chsBothGenDeltaR         {new TH1F("h_chsBothGenDeltaR",       "#DeltaR between both genuine chs tracks", 50, 0., 1.)};
    TH1F* h_chsLeadingGenDeltaR      {new TH1F("h_chsLeadingGenDeltaR",    "#DeltaR between leading gen/subleading fake chs tracks", 50, 0., 1.)};
    TH1F* h_chsSubleadingGenDeltaR   {new TH1F("h_chsSubleadingGenDeltaR", "#DeltaR between leading fake/subleading gen chs tracks", 50, 0., 1.)};
    TH1F* h_chsBothFakeDeltaR        {new TH1F("h_chsBothFakeDeltaR",      "#DeltaR between both fake chs tracks", 50, 0., 1.)};

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
        "2018", po::bool_switch(&is2018_), "Use 2018 conditions (SFs, et al.).");
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
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "/"};
//    const std::string postLepSelSkimInputDir{std::string{"/user/almorton/HToSS_analysis/postLepSkims"} + era + "/"};

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);

        std::cerr << "Processing dataset " << dataset->name() << std::endl;
        if (!usePostLepTree) {
            if (!datasetFilled) {
                if (!dataset->fillChain(datasetChain)) {
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

        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        totalEvents += numberOfEvents;
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");

            event.GetEntry(i);

            float eventWeight = 1.;
             eventWeight *= datasetWeight;

            std::vector<int> genMuonIndex;
            std::vector<int> genMuonSortedIndex;
            std::vector<int> genPionIndex;
            std::vector<int> genKaonIndex;

            // gen particle loop
            for ( Int_t k = 0; k < event.nGenPar; k++ ) {
                const int pid { std::abs(event.genParId[k]) };
                const int motherId { std::abs(event.genParMotherId[k]) };
                if ( pid == 13 && motherId == 9000006) {
                    genMuonIndex.emplace_back(k);
                }
                else if ( pid == 211 && motherId < 6 && motherId > 0 ) {
                    if ( scalarGrandparent(event, k, 9000006) ) genPionIndex.emplace_back(k);
                }
                else if ( pid == 321  && motherId < 6 && motherId > 0 ) {
                    if ( scalarGrandparent(event, k, 9000006) ) genKaonIndex.emplace_back(k);
                }
            }

            TLorentzVector genMuon1, genMuon2;
            if ( genMuonIndex.size() == 2 ) {
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

            if ( genMuonSortedIndex.size() == 2 ) {
                h_genDimuonDeltaR->Fill( genMuon1.DeltaR(genMuon2) );
                h_genDimuonMass->Fill( (genMuon1+genMuon2).M() );
                h_genDimuonPt->Fill( (genMuon1+genMuon2).Pt() );
                h_genDimuonEta->Fill( (genMuon1+genMuon2).Eta() );
                h_genLeadingMuonPt->Fill( genMuon1.Pt() );
                h_genSubleadingMuonPt->Fill( genMuon2.Pt() );
            }

            if ( genPionIndex.size() == 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genPionIndex[0]] > event.genParPt[genPionIndex[1]] ) firstLeading = true;
                TLorentzVector genPion1, genPion2;
                if (firstLeading) {
                    genPion1.SetPtEtaPhiE(event.genParPt[genPionIndex[0]], event.genParEta[genPionIndex[0]], event.genParPhi[genPionIndex[0]], event.genParE[genPionIndex[0]]);
                    genPion2.SetPtEtaPhiE(event.genParPt[genPionIndex[1]], event.genParEta[genPionIndex[1]], event.genParPhi[genPionIndex[1]], event.genParE[genPionIndex[1]]);
                    if ( genPion1.Pt() < 0.5 || genPion2.Pt() < 0.5 ) continue;
                    h_genDipionDeltaR->Fill( genPion1.DeltaR(genPion2) );
                    h_genDipionMass->Fill( (genPion1+genPion2).M() );
                    h_genDipionPt->Fill( (genPion1+genPion2).Pt() );
                    h_genDipionEta->Fill( (genPion1+genPion2).Eta() );
                    h_genLeadingPionPt->Fill( genPion1.Pt() );
                    h_genSubleadingPionPt->Fill( genPion2.Pt() );
                    if ( genMuonSortedIndex.size() == 2 ) h_genDiscalarDeltaR_mumu_pipi->Fill( (genMuon1+genMuon2).DeltaR( (genPion1+genPion2) ) );
                }
                else {
                    genPion1.SetPtEtaPhiE(event.genParPt[genPionIndex[1]], event.genParEta[genPionIndex[1]], event.genParPhi[genPionIndex[1]], event.genParE[genPionIndex[1]]);
                    genPion2.SetPtEtaPhiE(event.genParPt[genPionIndex[0]], event.genParEta[genPionIndex[0]], event.genParPhi[genPionIndex[0]], event.genParE[genPionIndex[0]]);
                    if ( genPion1.Pt() < 0.5 || genPion2.Pt() < 0.5 ) continue;
                    h_genDipionDeltaR->Fill( genPion1.DeltaR(genPion2) );
                    h_genDipionMass->Fill( (genPion1+genPion2).M() );
                    h_genDipionPt->Fill( (genPion1+genPion2).Pt() );
                    h_genDipionEta->Fill( (genPion1+genPion2).Eta() );
                    h_genLeadingPionPt->Fill( genPion2.Pt() );
                    h_genSubleadingPionPt->Fill( genPion1.Pt() );
                    if ( genMuonSortedIndex.size() == 2 ) h_genDiscalarDeltaR_mumu_pipi->Fill( (genMuon1+genMuon2).DeltaR( (genPion1+genPion2) ) );
                }
            }

           if ( genKaonIndex.size() == 2 ) {
                bool firstLeading {false};
                if ( event.genParPt[genKaonIndex[0]] > event.genParPt[genKaonIndex[1]] ) firstLeading = true;
                TLorentzVector genKaon1, genKaon2;
                if (firstLeading) {
                    genKaon1.SetPtEtaPhiE(event.genParPt[genKaonIndex[0]], event.genParEta[genKaonIndex[0]], event.genParPhi[genKaonIndex[0]], event.genParE[genKaonIndex[0]]);
                    genKaon2.SetPtEtaPhiE(event.genParPt[genKaonIndex[1]], event.genParEta[genKaonIndex[1]], event.genParPhi[genKaonIndex[1]], event.genParE[genKaonIndex[1]]);
                    if ( genKaon1.Pt() < 0.5 || genKaon2.Pt() < 0.5 ) continue;
                    h_genDikaonDeltaR->Fill( genKaon1.DeltaR(genKaon2) );
                    h_genDikaonMass->Fill( (genKaon1+genKaon2).M() );
                    h_genDikaonPt->Fill( (genKaon1+genKaon2).Pt() );
                    h_genDikaonEta->Fill( (genKaon1+genKaon2).Eta() );
                    h_genLeadingKaonPt->Fill( genKaon1.Pt() );
                    h_genSubleadingKaonPt->Fill( genKaon2.Pt() );
                    if ( genMuonSortedIndex.size() == 2 ) h_genDiscalarDeltaR_mumu_kaonkaon->Fill( (genMuon1+genMuon2).DeltaR( (genKaon1+genKaon2) ) );
                }
                else {
                    genKaon1.SetPtEtaPhiE(event.genParPt[genKaonIndex[1]], event.genParEta[genKaonIndex[1]], event.genParPhi[genKaonIndex[1]], event.genParE[genKaonIndex[1]]);
                    genKaon2.SetPtEtaPhiE(event.genParPt[genKaonIndex[0]], event.genParEta[genKaonIndex[0]], event.genParPhi[genKaonIndex[0]], event.genParE[genKaonIndex[0]]);
                    if ( genKaon1.Pt() < 0.5 || genKaon2.Pt() < 0.5 ) continue;
                    h_genDikaonDeltaR->Fill( genKaon1.DeltaR(genKaon2) );
                    h_genDikaonMass->Fill( (genKaon1+genKaon2).M() );
                    h_genDikaonPt->Fill( (genKaon1+genKaon2).Pt() );
                    h_genDikaonEta->Fill( (genKaon1+genKaon2).Eta() );
                    h_genLeadingKaonPt->Fill( genKaon1.Pt() );
                    h_genSubleadingKaonPt->Fill( genKaon2.Pt() );
                    if ( genMuonSortedIndex.size() == 2 ) h_genDiscalarDeltaR_mumu_kaonkaon->Fill( (genMuon1+genMuon2).DeltaR( (genKaon1+genKaon2) ) );
                }
            }

            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

            if (! ( passDimuonTrigger || passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger ) ) continue;
            if (!event.metFilters()) continue;
 
            event.muonIndexLoose = getLooseMuons(event);
            std::vector<int> chsIndex = getChargedHadronTracks(event);

            h_numLooseMuons->Fill(event.muonIndexLoose.size());

            if ( event.muonIndexLoose.size() < 2 ) continue;

            if ( !getDileptonCand( event, event.muonIndexLoose, false ) ) continue;

            h_recoDimuonDeltaR->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
            h_recoDimuonMass->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).M() );
            h_recoDimuonPt->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).Pt() );
            h_recoDimuonEta->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Eta() );
            h_recoLeadingMuonPt->Fill( (event.zPairLeptons.first).Pt() );
            h_recoSubleadingMuonPt->Fill( (event.zPairLeptons.second).Pt() );

            // Add comparison between leptonic and hadronic scalar decayed muons

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

            h_recoGenDimuonDeltaR->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
            h_recoGenDimuonMass->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).M() );
            h_recoGenDimuonPt->Fill( (event.zPairLeptons.first + event.zPairLeptons.second).Pt() );
            h_recoGenDimuonEta->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Eta() );
            h_recoGenLeadingMuonPt->Fill( (event.zPairLeptons.first).Pt() );
            h_recoGenSubleadingMuonPt->Fill( (event.zPairLeptons.second).Pt() );

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


            if ( chsIndex.size() < 2 ) continue;
            if (!getDihadronCand(event, chsIndex, false)) continue;

            // CHS bit
            const int idx1 {event.chsPairIndex.first}, idx2 {event.chsPairIndex.second};

            const bool validEle1    {event.packedCandsElectronIndex[idx1] > -1},  validEle2    {event.packedCandsElectronIndex[idx2] > -1};
            const bool validMuon1   {event.packedCandsMuonIndex[idx1]     > -1 }, validMuon2   {event.packedCandsMuonIndex[idx2]     > -1};
            const bool validJet1    {event.packedCandsJetIndex[idx1]      > -1},  validJet2    {event.packedCandsJetIndex[idx2]      > -1};
            const bool validPhoton1 {event.packedCandsPhotonIndex[idx1]   > -1},  validPhoton2 {event.packedCandsPhotonIndex[idx2]   > -1}; 

            const bool leadingValid { (validEle1 || validMuon1 || validJet1 || validPhoton1) };
            const bool subleadingValid { (validEle2 || validMuon2 || validJet2 || validPhoton2) };

            if (!leadingValid || !subleadingValid) continue;

            const int eleIndex1    {validEle1    ? event.packedCandsElectronIndex[idx1] : -1}, eleIndex2    {validEle2    ? event.packedCandsElectronIndex[idx2] : -1};
            const int muonIndex1   {validMuon1   ? event.packedCandsMuonIndex[idx1]     : -1}, muonIndex2   {validMuon2   ? event.packedCandsMuonIndex[idx2]     : -1};
            const int jetIndex1    {validJet1    ? event.packedCandsJetIndex[idx1]      : -1}, jetIndex2    {validJet2    ? event.packedCandsJetIndex[idx2]      : -1};
            const int photonIndex1 {validPhoton1 ? event.packedCandsPhotonIndex[idx1]   : -1}, photonIndex2 {validPhoton2 ? event.packedCandsPhotonIndex[idx2]   : -1};

            const bool matchedEle1    {validEle1    ? event.genElePF2PATScalarAncestor[eleIndex1]    : false}, matchedEle2    {validEle2    ? event.genElePF2PATScalarAncestor[eleIndex2]    : false};
            const bool matchedMuon1   {validMuon1   ? event.genMuonPF2PATScalarAncestor[validMuon1]  : false}, matchedMuon2   {validMuon2   ? event.genMuonPF2PATScalarAncestor[validMuon2] : false};
            const bool matchedJet1    {validJet1    ? event.genJetPF2PATScalarAncestor[jetIndex1]    : false}, matchedJet2    {validJet2    ? event.genJetPF2PATScalarAncestor[jetIndex2]  	: false};
            const bool matchedPhoton1 {validPhoton1 ? event.genPhoPF2PATScalarAncestor[photonIndex1] : false}, matchedPhoton2 {validPhoton2 ? event.genPhoPF2PATScalarAncestor[photonIndex2] : false};

            const float chsDeltaR {event.chsPairVec.first.DeltaR(event.chsPairVec.second)};

            const bool leadingGen    { (matchedEle1 || matchedMuon1 || matchedJet1 || matchedPhoton1) };
            const bool subleadingGen { (matchedEle2 || matchedMuon2 || matchedJet2 || matchedPhoton2) };

            const TLorentzVector leptonicScalarVec {event.zPairLeptons.first + event.zPairLeptons.second}, hadronicScalarVec {event.chsPairVec.first + event.chsPairVec.second};
            const TLorentzVector refittedLeptonicScalarVec {event.zPairLeptonsRefitted.first + event.zPairLeptonsRefitted.second};
            const TLorentzVector refittedHadronicScalarVec {event.chsPairVecRefitted.first + event.chsPairVecRefitted.second};

            h_diScalarDeltaR->Fill( leptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR->Fill( leptonicScalarVec.DeltaR(hadronicScalarVec) );
            h_diScalarDeltaR_refittedMuons->Fill( refittedLeptonicScalarVec.DeltaR(hadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR_refittedMuons->Fill( refittedLeptonicScalarVec.DeltaR(hadronicScalarVec) );
            h_diScalarDeltaR_refittedMuonsChs->Fill( refittedLeptonicScalarVec.DeltaR(refittedHadronicScalarVec) );
            if ( leadingGen && subleadingGen ) h_diScalarGenDeltaR_refittedMuonsChs->Fill( refittedLeptonicScalarVec.DeltaR(refittedHadronicScalarVec) );

            p_selectedChsMatching->Fill( 1.0, bool (  leadingGen &&  subleadingGen ) );
            p_selectedChsMatching->Fill( 2.0, bool (  leadingGen && !subleadingGen ) );
            p_selectedChsMatching->Fill( 3.0, bool ( !leadingGen &&  subleadingGen ) );
            p_selectedChsMatching->Fill( 4.0, bool ( !leadingGen && !subleadingGen ) );
            p_selectedChsMatching->Fill( 5.0, leadingGen );
            p_selectedChsMatching->Fill( 6.0, subleadingGen );

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
            const float refittedChsTk1Chi2Ndof {event.chsTkPairTk1Chi2[chsTkPairIndex]/(event.chsTkPairTk1Ndof[chsTkPairIndex] + 1.0e-06)};
            const float refittedChsTk2Chi2Ndof {event.chsTkPairTk2Chi2[chsTkPairIndex]/(event.chsTkPairTk2Ndof[chsTkPairIndex] + 1.0e-06)};

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
            if (!leadingGen && !subleadingGen) h_chsRefittedTkPairChi2NdofSubleadingGen->Fill(event.chsTkPairTkVtxChi2[chsTkPairIndex]);
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

    h_genDikaonDeltaR->Write();
    h_genDikaonMass->Write();
    h_genDikaonPt->Write();
    h_genDikaonEta->Write();
    h_genLeadingKaonPt->Write();
    h_genSubleadingKaonPt->Write();

    h_genDiscalarDeltaR_mumu_pipi->Write();
    h_genDiscalarDeltaR_mumu_kaonkaon->Write();

    h_numLooseMuons->Write();

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

    p_fakeSelected->Write();

    h_numMatchedChsTracks->Write();
    h_numMatchedChsTracksFromScalars->Write();
    h_numMatchedChsFromScalarsExcMu->Write();

    h_diScalarDeltaR->Write();
    h_diScalarGenDeltaR->Write();
    h_diScalarDeltaR_refittedMuons->Write();
    h_diScalarGenDeltaR_refittedMuons->Write();
    h_diScalarDeltaR_refittedMuonsChs->Write();
    h_diScalarGenDeltaR_refittedMuonsChs->Write();

    p_selectedChsMatching->Write();

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

    h_chsDeltaR->Write();
    h_chsBothGenDeltaR->Write();
    h_chsLeadingGenDeltaR->Write();
    h_chsSubleadingGenDeltaR->Write();
    h_chsBothFakeDeltaR->Write();

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

std::vector<int> getLooseMuons(const AnalysisEvent& event) {
    std::vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {
       if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] /*&& event.muonPF2PATPfIsoLoose[i]*/ && std::abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
           if (event.muonPF2PATPt[i] >= (muons.empty() ? looseMuonPtLeading_ : looseMuonPt_)) muons.emplace_back(i);
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

        chs.emplace_back(k);
    }
    return chs;
}


bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth) {
    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

            if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;
            if ( mcTruth && (!event.genMuonPF2PATDirectScalarAncestor[muons[i]] || !event.genMuonPF2PATDirectScalarAncestor[muons[j]]) ) continue;

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

                event.mumuTrkIndex = getMuonTrackPairIndex(event);

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

            double delR { chs1.DeltaR(chs2) };
            double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };

            if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ ) {
                event.chsPairVec.first  = chs1.Pt() > chs2.Pt() ? chs1 : chs2;
                event.chsPairVec.second = chs1.Pt() > chs2.Pt() ? chs2 : chs1;
                event.chsPairIndex.first = chs1.Pt() > chs2.Pt() ? chs[i] : chs[j];
                event.chsPairIndex.second = chs1.Pt() > chs2.Pt() ? chs[j] : chs[i];

                event.chsPairTrkIndex = getChsTrackPairIndex(event);

                event.chsPairVecRefitted.first  = TLorentzVector{event.chsTkPairTk1Px[event.chsPairTrkIndex], event.chsTkPairTk1Py[event.chsPairTrkIndex], event.chsTkPairTk1Pz[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2))};
                event.chsPairVecRefitted.second = TLorentzVector{event.chsTkPairTk2Px[event.chsPairTrkIndex], event.chsTkPairTk2Py[event.chsPairTrkIndex], event.chsTkPairTk2Pz[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2))};
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

bool scalarGrandparent (const AnalysisEvent& event, const Int_t& k, const Int_t& grandparentId) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };


    if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
    else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
    else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
    else {
//        std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
        return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
    }
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

