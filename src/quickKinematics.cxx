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
#include <numeric>
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

using namespace std;
vector<int> getLooseMuons(const AnalysisEvent& event);
vector<int> getPromptMuons(const AnalysisEvent& event, const vector<int>& muonIndex, const bool getPrompt );

bool getDileptonCand(AnalysisEvent& event, const vector<int>& muons, bool mcTruth = false);
int getMuonTrackPairIndex(const AnalysisEvent& event);
float deltaR(float eta1, float phi1, float eta2, float phi2);
void SetStyle(Bool_t graypalette=kTRUE);

pair<int, int> FindGen(const AnalysisEvent& event, int numGenMuons, vector<size_t>& idx);
int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max);
namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.8}, looseMuonPt_ {6.}, looseMuonPtLeading_ {15.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0};
// Diparticle cuts
double maxDileptonDeltaR_ {0.2};

int main(int argc, char* argv[])
{
    //SetStyle();
    auto timerStart = chrono::high_resolution_clock::now(); 

    string config;
    vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
    
    
    string plotConfig{"configs/plots/plotKinematics.yaml"};
    std::vector<std::string> plotTitles;
    std::vector<std::string> plotNames;
    std::vector<float> xMin;
    std::vector<float> xMax;
    std::vector<int> nBins;
    std::vector<std::string> fillExp;
    std::vector<std::string> xAxisLabels;
    std::vector<int> cutStage;
   
    string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const regex mask{".*\\.root"};

    // Quick and dirty trigger plots
    //genLevel plots
    TH1F* h_leadingMuonPt_gen                    {new TH1F("h_leadingMuonPt_gen",         "Leading GenMuon; p_{T} (GeV); Events",        200, 0., 100.)};
    TH1F* h_subleadingMuonPt_gen                 {new TH1F("h_subleadingMuonPt_gen",      "SubLeading GenMuon; p_{T} (GeV); Events",     200, 0., 100.)}; 
    TH1F* h_delR_gen                             {new TH1F("h_delR_gen",                  "#DeltaR GenLevel; #DeltaR; Events",         100, 0., 1.)};
    TH1F* h_diMuonMass_gen                       {new TH1F("h_diMuonMass_gen",            "Mass dist. GenLevel; m_{#mu#mu}; Events",    200, 0., 100.)};
    TH1F* h_numMuon_gen                          {new TH1F("h_numMuon_gen",               "No. of GenLevel muons; numMuon; Events",      10, 0., 10.)};
    
    // denom
    TH1F* h_leadingMuonPt_truth                  {new TH1F("h_leadingMuonPt_truth",     "Leading truth reco #mu; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth               {new TH1F("h_subLeadingMuonPt_truth", "SubLeading truth reco #mu; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_leadingMuonPt                        {new TH1F("h_leadingMuonPt",            "Leading PAT reco #mu; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt                     {new TH1F("h_subLeadingMuonPt",         "SubLeading PAT reco #mu; p_{T} (GeV); Events", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth                 {new TH1F("h_leadingMuonEta_truth",     "Leading truth reco #mu; #eta; Events",       300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth              {new TH1F("h_subLeadingMuonEta_truth",  "SubLeading truth reco #mu; #eta; Events",    300, -3., 3.)};
    TH1F* h_leadingMuonEta                       {new TH1F("h_leadingMuonEta",           "Leading PAT reco #mu; #eta; Events",        300, -3., 3.)};
    TH1F* h_subLeadingMuonEta                    {new TH1F("h_subLeadingMuonEta",        "SubLeading PAT reco #mu; #eta; Events",      300, -3., 3.)};
    TH1F* h_delR_truth                           {new TH1F("h_delR_truth",               "#DeltaR truth reco; #DeltaR; Events",      100, 0., 1.0)};
    TH1F* h_delR                                 {new TH1F("h_delR",                     "#DeltaR PAT reco; #DeltaR; Events",         100, 0., 1.0)};
    TH1F* h_diMuonMass_truth                     {new TH1F("h_diMuonMass_truth",         "Mass dist. truth reco; m_{#mu#mu}; Events", 100, 0., 10.)}; 
    TH1F* h_diMuonMass                           {new TH1F("h_diMuonMass",               "Mass dist. PAT reco; m_{#mu#mu}; Events",   100, 0., 10.)};
    TH1F* h_numMuonPF2PAT                        {new TH1F("h_numMuonPF2PAT",             "No. of reco muons; numMuonPF2PAT; Events",   10, 0., 10.)};
    TH1F* h_leadingmuonPF2PATD0_truth                  {new TH1F("h_leadingmuonPF2PATD0_truth",         "PAT reco muons; Leading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_leadingmuonPF2PATDBPV_truth                  {new TH1F("h_leadingmuonPF2PATDBPV_truth",     "PAT reco muons; Leading #mu d_{xy}; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATD0_truth                  {new TH1F("h_subleadingmuonPF2PATD0_truth",         "PAT reco muons; subLeading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATDBPV_truth                  {new TH1F("h_subleadingmuonPF2PATDBPV_truth",     "PAT reco muons; subLeading #mu d_{xy}; Events",   600, -10., 10.)};
    TH1F* h_muonPF2PATMatchedStations_truth       {new TH1F("h_muonPF2PATMatchedStations_truth",     "PAT reco muons; MatchedStations; Events",   600, -10., 10.)};
    
    TH1F* h_leadingMuon_pfIso_truth              {new TH1F("h_leadingMuon_pfIso_truth",  "Leading truth reco #mu; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_subLeadingMuon_pfIso_truth           {new TH1F("h_subLeading_pfIso_truth", "SubLeading truth reco #mu; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_leadingMuon_trkIso_truth          {new TH1F("h_leadingMuon_trkIso_truth", "Leading truth reco #mu; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_subLeadingMuon_trkIso_truth       {new TH1F("h_subLeading_trkIso_truth", "SubLeading truth reco #mu; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_leadingMuon_trkIso_orig_truth          {new TH1F("h_leadingMuon_trkIso_orig_truth", "Leading truth reco #mu; TrkIsolation_original; Events", 500, 0., 100.)};
    TH1F* h_subLeadingMuon_trkIso_orig_truth       {new TH1F("h_subLeading_trkIso_orig_truth", "SubLeading truth reco #mu; TrkIsolation_original; Events", 500, 0., 100.)};
    
    TH2F* h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth {new TH2F("h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth", "PFIsolation truth reco; Leading #mu PFIsolation; Subleading #mu PFIsolation", 100, 0.0, 1.0, 100, 0., 1.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth", "TrkIsolation truth reco; Leading #mu TrkIsolation; Subleading #mu TrkIsolation", 200, 0.0, 10.0, 200, 0., 10.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth", "TrkIsolation_original truth reco; Leading #mu TrkIsolation_original; Subleading #mu TrkIsolation_original", 500, 0., 100., 500, 0., 100.)};
    TH2F* h_leadingMuon_Pt_subLeadingMuon_Pt_truth {new TH2F("h_leadingMuon_Pt_subLeadingMuon_Pt_truth", "p_{T} truth reco; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    TH2F* h_leadingMuon_Pt_delR_truth {new TH2F("h_leadingMuon_Pt_delR_truth", "Truth reco; Leading #mu p_{T} (GeV); #DeltaR",       200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_subLeadingMuon_Pt_delR_truth {new TH2F("h_subLeadingMuon_Pt_delR_truth", "Truth reco; SubLeading #mu p_{T} (GeV); #DeltaR", 200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_leadingMuon_Pt_leadingMuon_trkIso_truth {new TH2F("h_leadingMuon_Pt_leadingMuon_trkIso_truth", "Truth reco; Leading #mu p_{T} (GeV); TrkIsolation",       200, 0.0, 100, 200, 0., 10.)};
    TH2F* h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth {new TH2F("h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth", "Truth reco; SubLeading #mu p_{T} (GeV); TrkIsolation", 200, 0.0, 100, 200, 0., 10.)};
    
    
    // numerator - single mu
    TH1F* h_leadingMuonPt_truth_muTrig           {new TH1F("h_leadingMuonPt_truth_muTrig",      "Trigger turn-on for signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_muTrig        {new TH1F("h_subLeadingMuonPt_truth_muTrig",   "Trigger turn-on for signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_muTrig                 {new TH1F("h_leadingMuonPt_muTrig",            "Trigger turn-on for signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_muTrig              {new TH1F("h_subLeadingMuonPt_muTrig",         "Trigger turn-on for signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_muTrig          {new TH1F("h_leadingMuonEta_truth_muTrig",     "Trigger turn-on for signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_muTrig       {new TH1F("h_subLeadingMuonEta_truth_muTrig",  "Trigger turn-on for signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_muTrig                {new TH1F("h_leadingMuonEta_muTrig",           "Trigger turn-on for signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_muTrig             {new TH1F("h_subLeadingMuonEta_muTrig",        "Trigger turn-on for signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_muTrig                    {new TH1F("h_delR_truth_muTrig",               "Trigger turn-on for signal; #Delta R; #mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_muTrig                          {new TH1F("h_delR_muTrig",                     "Trigger turn-on for signal; #Delta R; #mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_muTrig              {new TH1F("h_diMuonMass_truth_muTrig",         "Trigger turn-on for signal; m_{#mu#mu}; #mu trigger #epsilon", 100, 0., 10.)};
    TH1F* h_diMuonMass_muTrig                    {new TH1F("h_diMuonMass_muTrig",               "Trigger turn-on for signal; m_{#mu#mu}; #mu trigger #epsilon", 100, 0., 10.)};
    TH1F* h_numMuonPF2PAT_muTrig                        {new TH1F("h_numMuonPF2PAT_muTrig",      "Trigger turn-on for signal; No. of reco muons; numMuonPF2PAT; Events",   10, 0., 10.)};
    TH1F* h_leadingmuonPF2PATD0_truth_muTrig                   {new TH1F("h_leadingmuonPF2PATD0_truth_muTrig",  "Trigger turn-on for signal; Leading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_leadingmuonPF2PATDBPV_truth_muTrig                  {new TH1F("h_leadingmuonPF2PATDBPV_truth_muTrig",     "Trigger turn-on for signal; Leading #mu d_{xy}; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATD0_truth_muTrig                   {new TH1F("h_subleadingmuonPF2PATD0_truth_muTrig",  "Trigger turn-on for signal; subLeading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATDBPV_truth_muTrig                  {new TH1F("h_subleadingmuonPF2PATDBPV_truth_muTrig",     "Trigger turn-on for signal; subLeading #mu d_{xy}; Events",   600, -10., 10.)};
    
    TH1F* h_leadingMuon_pfIso_truth_muTrig              {new TH1F("h_leadingMuon_pfIso_truth_muTrig",  "Leading truth reco #mu for Passing; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_subLeadingMuon_pfIso_truth_muTrig           {new TH1F("h_subLeading_pfIso_truth_muTrig", "SubLeading truth reco #mu for Passing; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_leadingMuon_trkIso_truth_muTrig          {new TH1F("h_leadingMuon_trkIso_truth_muTrig", "Leading truth reco #mu for Passing; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_subLeadingMuon_trkIso_truth_muTrig       {new TH1F("h_subLeading_trkIso_truth_muTrig", "SubLeading truth reco #mu for Passing; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_leadingMuon_trkIso_orig_truth_muTrig          {new TH1F("h_leadingMuon_trkIso_orig_truth_muTrig", "Leading truth reco #mu for Passing; TrkIsolation_original; Events", 500, 0., 100.)};
    TH1F* h_subLeadingMuon_trkIso_orig_truth_muTrig       {new TH1F("h_subLeading_trkIso_orig_truth_muTrig", "SubLeading truth reco #mu for Passing; TrkIsolation_original; Events", 500, 0., 100.)};
    
    TH2F* h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig {new TH2F("h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig", "PFIsolation truth reco for Passing; Leading #mu PFIsolation; Subleading #mu PFIsolation", 100, 0.0, 1.0, 100, 0., 1.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig", "TrkIsolation truth reco for Passing; Leading #mu TrkIsolation; Subleading #mu TrkIsolation", 200, 0.0, 10.0, 200, 0., 10.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig", "TrkIsolation_original truth reco for Passing; Leading #mu TrkIsolation_original; Subleading #mu TrkIsolation_original", 500, 0., 100., 500, 0., 100.)};
    TH2F* h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig {new TH2F("h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig", "p_{T} truth reco for Passing; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    TH2F* h_leadingMuon_Pt_delR_truth_muTrig {new TH2F("h_leadingMuon_Pt_delR_truth_muTrig", "Truth reco for Passing; Leading #mu p_{T} (GeV); #DeltaR",       200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_subLeadingMuon_Pt_delR_truth_muTrig {new TH2F("h_subLeadingMuon_Pt_delR_truth_muTrig", "Truth reco for Passing; SubLeading #mu p_{T} (GeV); #DeltaR", 200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig {new TH2F("h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig", "Truth reco for Passing; Leading #mu p_{T} (GeV); TrkIsolation",       200, 0.0, 100, 200, 0., 10.)};
    TH2F* h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig {new TH2F("h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig", "Truth reco for Passing; SubLeading #mu p_{T} (GeV); TrkIsolation", 200, 0.0, 100, 200, 0., 10.)};
    
    // Failing single mu trigger
    TH1F* h_leadingMuonPt_truth_muTrig_fail           {new TH1F("h_leadingMuonPt_truth_muTrig_fail",      "Failing signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_muTrig_fail        {new TH1F("h_subLeadingMuonPt_truth_muTrig_fail",   "Failing signal; p_{T} (GeV); #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_muTrig_fail          {new TH1F("h_leadingMuonEta_truth_muTrig_fail",     "Failing signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_muTrig_fail       {new TH1F("h_subLeadingMuonEta_truth_muTrig_fail",  "Failing signal; #eta; #mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_muTrig_fail                    {new TH1F("h_delR_truth_muTrig_fail",               "Failing signal; #Delta R; #mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_muTrig_fail              {new TH1F("h_diMuonMass_truth_muTrig_fail",         "Failing signal; m_{#mu#mu}; #mu trigger #epsilon", 100, 0., 10.)};
    TH1F* h_numMuonPF2PAT_muTrig_fail                        {new TH1F("h_numMuonPF2PAT_muTrig_fail",      "Failing signal; No. of reco muons; numMuonPF2PAT; Events",   10, 0., 10.)};
    TH1F* h_leadingmuonPF2PATD0_truth_muTrig_fail                   {new TH1F("h_leadingmuonPF2PATD0_truth_muTrig_fail",  "Failing singal; Leading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_leadingmuonPF2PATDBPV_truth_muTrig_fail                {new TH1F("h_leadingmuonPF2PATDBPV_truth_muTrig_fail",     "Failing signal; Leading #mu d_{xy}; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATD0_truth_muTrig_fail                   {new TH1F("h_subleadingmuonPF2PATD0_truth_muTrig_fail",  "Failing singal; subLeading #mu D0; Events",   600, -10., 10.)};
    TH1F* h_subleadingmuonPF2PATDBPV_truth_muTrig_fail                {new TH1F("h_subleadingmuonPF2PATDBPV_truth_muTrig_fail",     "Failing signal; subLeading #mu d_{xy}; Events",   600, -10., 10.)};
    
    TH1F* h_leadingMuon_pfIso_truth_muTrig_fail              {new TH1F("h_leadingMuon_pfIso_truth_muTrig_fail",  "Leading truth reco #mu for Failing; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_subLeadingMuon_pfIso_truth_muTrig_fail           {new TH1F("h_subLeading_pfIso_truth_muTrig_fail", "SubLeading truth reco #mu for Failing; PFIsolation; Events", 100, 0., 1.)};
    TH1F* h_leadingMuon_trkIso_truth_muTrig_fail          {new TH1F("h_leadingMuon_trkIso_truth_muTrig_fail", "Leading truth reco #mu for Failing; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_subLeadingMuon_trkIso_truth_muTrig_fail       {new TH1F("h_subLeading_trkIso_truth_muTrig_fail", "SubLeading truth reco #mu for Failing; TrkIsolation; Events", 200, 0., 10.)};
    TH1F* h_leadingMuon_trkIso_orig_truth_muTrig_fail     {new TH1F("h_leadingMuon_trkIso_orig_truth_muTrig_fail", "Leading truth reco #mu for Passing; TrkIsolation_original; Events", 500, 0., 100.)};
    TH1F* h_subLeadingMuon_trkIso_orig_truth_muTrig_fail  {new TH1F("h_subLeading_trkIso_orig_truth_muTrig_fail", "SubLeading truth reco #mu for Passing; TrkIsolation_original; Events", 500, 0., 100.)};
    
    TH2F* h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig_fail {new TH2F("h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig_fail", "PFIsolation truth reco for Failing; Leading #mu PFIsolation; Subleading #mu PFIsolation", 100, 0.0, 1.0, 100, 0., 1.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig_fail {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig_fail", "TrkIsolation truth reco for Failing; Leading #mu TrkIsolation; Subleading #mu TrkIsolation", 200, 0.0, 10.0, 200, 0., 10.0)};
    TH2F* h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig_fail {new TH2F("h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig_fail", "TrkIsolation_original truth reco for Failing; Leading #mu TrkIsolation_original; Subleading #mu TrkIsolation_original", 500, 0., 100., 500, 0., 100.)};
    TH2F* h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig_fail {new TH2F("h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig_fail", "p_{T} truth reco for Failing; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    TH2F* h_leadingMuon_Pt_delR_truth_muTrig_fail {new TH2F("h_leadingMuon_Pt_delR_truth_muTrig_fail", "Truth reco for Failing; Leading #mu p_{T} (GeV); #DeltaR",       200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_subLeadingMuon_Pt_delR_truth_muTrig_fail {new TH2F("h_subLeadingMuon_Pt_delR_truth_muTrig_fail", "Truth reco for Failing; SubLeading #mu p_{T} (GeV); #DeltaR", 200, 0.0, 100, 100, 0., 1.)};
    TH2F* h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig_fail {new TH2F("h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig_fail", "Truth reco for Failing; Leading #mu p_{T} (GeV); TrkIsolation",       200, 0.0, 100, 200, 0., 10.)};
    TH2F* h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig_fail {new TH2F("h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig_fail", "Truth reco for Failing; SubLeading #mu p_{T} (GeV); TrkIsolation", 200, 0.0, 100, 200, 0., 10.)};

    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<string>(&config)->required(),
        "The configuration file to be used.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(41528.0),
        "Lumi to scale MC plots to.")(
        "outfile,o",
        po::value<string>(&outFileString)->default_value(outFileString),
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

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            cout << desc;
            return 0;
        }

        po::notify(vm);
        if ( is2016_ && is2018_ ) {
            throw logic_error(
                "Default condition is to use 2017. One cannot set "
                "condition to be BOTH 2016 AND 2018! Chose only "
                " one or none!");
        }
    }
    catch (po::error& e)
    {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    // Some vectors that will be filled in the parsing
    totalLumi = 0;

    try
    {
        Parser::parse_config(config,
                             datasets,
                             totalLumi,
                             usePostLepTree);
    }
    catch (const exception)
    {
        cerr << "ERROR Problem with a confugration file, see previous "
                     "errors for more details. If this is the only error, the "
                     "problem is with the main configuration file."
                  << endl;
        throw;
    }

    if (totalLumi == 0.)
    {
        totalLumi = usePreLumi;
    }
    cout << "Using lumi: " << totalLumi << endl;

    bool datasetFilled{false};
    string era {""};
    if (is2016_) era = "2016";
    else if (is2018_) era = "2018";
    else era = "2017";
    const string postLepSelSkimInputDir{string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};
//    const string postLepSelSkimInputDir{string{"/user/almorton/HToSS_analysis/postLepSkims"} + era + "/"};

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset)
    {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);
        int flow = 1;
        int fhigh = 10000; //to be changed to take input from condor

        cerr << "Processing dataset " << dataset->name() << endl;
        if (!usePostLepTree) {
            if (!datasetFilled) {
                if (!dataset->fillChain(datasetChain, flow, fhigh)) {
                    cerr << "There was a problem constructing the chain for " << dataset->name() << ". Continuing with next dataset.\n";
                    continue;
                }
                datasetFilled=true;
            }
        }
        else {
            cout << postLepSelSkimInputDir + dataset->name() + "mumuSmallSkim.root" << endl;
            datasetChain->Add((postLepSelSkimInputDir + dataset->name() + "mumuSmallSkim.root").c_str());
        }

        // extract the dataset weight. MC = (lumi*crossSection)/(totalEvents), data = 1.0
        float datasetWeight{dataset->getDatasetWeight(totalLumi)};
        cout << datasetChain->GetEntries() << " number of items in tree. Dataset weight: " << datasetWeight << endl;
        if (datasetChain->GetEntries() == 0) {
            cout << "No entries in tree, skipping..." << endl;
            continue;
        }

        AnalysisEvent event{dataset->isMC(), datasetChain, is2016_, is2018_};

        Long64_t numberOfEvents{datasetChain->GetEntries()};
        if (nEvents && nEvents < numberOfEvents) numberOfEvents = nEvents;

        int foundEvents{0};
        double foundEventsNorm{0.0};

        TH1I* generatorWeightPlot{nullptr};
        if (dataset->isMC()) {
            if (usePostLepTree) {
                string inputPostfix{"mumu"};
                TFile* datasetFileForHists;
                datasetFileForHists = new TFile((postLepSelSkimInputDir + dataset->name()  + inputPostfix + "SmallSkim.root").c_str(), "READ");
                generatorWeightPlot = dynamic_cast<TH1I*>(datasetFileForHists->Get("weightHisto")->Clone());
                generatorWeightPlot->SetDirectory(nullptr);
                datasetFileForHists->Close();
            }
            else {
                generatorWeightPlot = dynamic_cast<TH1I*>(dataset->getGeneratorWeightHistogram()->Clone());
                generatorWeightPlot->SetDirectory(nullptr);
            }
        }

        double sumPositiveWeights_          = generatorWeightPlot->GetBinContent(1)  + generatorWeightPlot->GetBinContent(2);
        double sumNegativeWeights_          = generatorWeightPlot->GetBinContent(1)  - generatorWeightPlot->GetBinContent(2);

        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        totalEvents += numberOfEvents;
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");

            event.GetEntry(i);
            //if (i>0) event.Show(i);
            float eventWeight = 1.;

            if (!event.metFilters()) continue;

            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};
            const bool passDiMuonNoMassCutTrigger {event.mumuTrig_noMassCut()}, passDiMuonMassCutTrigger {event.mumuTrig_massCut()};
            
            if ( event.numMuonPF2PAT > 1 ) {
                // fill muon pT plots pre-triggers
                //// ID requirements PF muon? no pT cut
                //// reco pT 
                
                /*int mu1_old {-1}, mu2_old{-1};
                for ( Int_t k{0}; k < event.numMuonPF2PAT; k++ ) {
                    if ( event.genMuonPF2PATScalarAncestor[k] && mu1_old < 0 ) mu1_old = k;
                    else if ( event.genMuonPF2PATScalarAncestor[k] && mu2_old < 0 ) mu2_old = k;
                    else if (mu1_old >= 0 && mu2_old > 0) break;
                }
                //cout<<"NGenMuons="<<(sizeof(event.genMuonPF2PATPX)/sizeof(event.genMuonPF2PATPX[0]))<<endl;
                if ((mu1_old<0)||(mu2_old<0)) {
                    cout<<"Event found with old technique: "<<i<<endl;
                    continue;
                }*/
                int numGenMuons=0;
                /*cout<<"Muon no. - PromptDecayed, PromptFinalState, HardProcess, MotherId, PDG, Charge, Pt"<<endl;
                for (int k=0;k<20;k++) {// 20 is max nmuons
                    if (event.genMuonPF2PATPromptFinalState[k]==1) numGenMuons+=1;
                    cout<<"Muon #"<<k<<", "<<event.genMuonPF2PATPromptDecayed[k]<<", "<<event.genMuonPF2PATPromptFinalState[k]<<", "<<event.genMuonPF2PATHardProcess[k]<<", "<<event.genMuonPF2PATMotherId[k]<<", "<<event.genMuonPF2PATPdgId[k]<<", "<<event.genMuonPF2PATCharge[k]<<", "<<event.genMuonPF2PATPT[k]<<endl;
                }*/
                //cout<<"Event no. - "<<i<<endl;
                //cout<<"Particle no. - Charge, MotherId, DaughterId1, DaughterId2, Pt"<<endl;
                vector<int> gen_mu_ind;
                for (int k=0;k<event.nGenPar;k++) {
                    if (event.genParStatus[k]!=1) continue; //final state
                    if (abs(event.genParId[k])!=13) continue; //Muon PDG ID
                    if ((abs(event.genParMotherId[k])==13) || (abs(event.genParMotherId[k])==9000006)) {
                        numGenMuons++;
                        //cout<<"Particle #"<<k<<", "<<event.genParCharge[k]<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<event.genParPt[k]<<endl;
                        gen_mu_ind.push_back(k);
                    }
                }
                //cout<<"number of GenMuons:"<<numGenMuons<<" and "<<gen_mu_ind.size()<<endl;
                if (numGenMuons!=2) cout<<"YO SOMETHING IS UP HERE : "<<i<<endl;
                if (numGenMuons!=2) continue; //strict condition to only select events with 2 genMuons
                
                //continue;
                
                //pT ordering genmuons
                /*vector<double> genMuonPt(event.genMuonPF2PATPT, event.genMuonPF2PATPT+numGenMuons);
                vector<size_t> idx(numGenMuons); 
                iota(idx.begin(), idx.end(), 0);
                sort(idx.begin(), idx.end(), [&genMuonPt](size_t i1, size_t i2) {return genMuonPt[i1] > genMuonPt[i2];});
                          
                            
                
                //idx contains pT-ordered genMuon indices
                
                auto gen_mu_ind = FindGen(event, numGenMuons, idx);*/
                //cout<<"NGenMuons="<<numGenMuons<<endl;
                int tmp1;
                if (event.genParPt[gen_mu_ind[1]]>event.genParPt[gen_mu_ind[0]]) {
                    /*gen_mu_ind[0]=gen_mu_ind[0]+gen_mu_ind[1];
                    gen_mu_ind[1]=gen_mu_ind[0]-gen_mu_ind[1];
                    gen_mu_ind[0]=gen_mu_ind[0]-gen_mu_ind[1];*/
                    tmp1=gen_mu_ind[0];
                    gen_mu_ind[0]=gen_mu_ind[1];
                    gen_mu_ind[1]=tmp1;
                }
                
                TLorentzVector gen_mu1, gen_mu2;
                double mu_mass=0.10565837; //in GeV
                gen_mu1.SetPtEtaPhiM(event.genParPt[gen_mu_ind[0]], event.genParEta[gen_mu_ind[0]], event.genParPhi[gen_mu_ind[0]], mu_mass);
                gen_mu2.SetPtEtaPhiM(event.genParPt[gen_mu_ind[1]], event.genParEta[gen_mu_ind[1]], event.genParPhi[gen_mu_ind[1]], mu_mass);       
                
                //if (numGenMuons>2) cout<<"YO SOMETHING IS UP"<<endl;
                
                double match_dR = 0.03; //0.03 taken from MuonAnalyzer code used by MuonPOG
                //int mu1=-99, mu2=-99;
                int mu1 = MatchReco(gen_mu_ind[0], event, match_dR);
                int mu2 = MatchReco(gen_mu_ind[1], event, match_dR);
                
                if ((mu1==mu2) && (mu1!=-99)) {
                    cout<<"SOMETHING IS UP HERE TOO"<<endl;
                    cout<<"Indices-"<<gen_mu_ind[0]<<", "<<gen_mu_ind[1]<<endl;
                    cout<<"Pt of gen-"<<event.genParPt[gen_mu_ind[0]]<<", "<<event.genParPt[gen_mu_ind[1]]<<endl;
                    cout<<"mu1, mu2"<<mu1<<", "<<mu2<<endl;
                    continue;
                }
         
                if ((mu1<0)||(mu2<0)) {
                    //cout<<"reaches this"<<endl;
                    continue;
                }
                //continue;
                //int mu1 = reco_match_genmu1.second;
                //int mu2 = reco_match_genmu2.second;
                
                const TLorentzVector muon1_truth {event.muonPF2PATPX[mu1], event.muonPF2PATPY[mu1], event.muonPF2PATPZ[mu1], event.muonPF2PATE[mu1]};
                const TLorentzVector muon2_truth {event.muonPF2PATPX[mu2], event.muonPF2PATPY[mu2], event.muonPF2PATPZ[mu2], event.muonPF2PATE[mu2]};
                const TLorentzVector muon1 {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
                const TLorentzVector muon2 {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]}; 
	
                const float delR_truth = deltaR(event.muonPF2PATEta[mu1], event.muonPF2PATPhi[mu1], event.muonPF2PATEta[mu2], event.muonPF2PATPhi[mu2]);
                const float delR       = deltaR(event.muonPF2PATEta[0], event.muonPF2PATPhi[0], event.muonPF2PATEta[1], event.muonPF2PATPhi[1]);
                const float mass_truth = (muon1_truth + muon2_truth).M();
                const float mass       = (muon1 + muon2).M();
                //float muon1_truth_trkIso = (event.muonPF2PATTrackIso[mu1]-event.muonPF2PATPt[mu1])/event.muonPF2PATPt[mu1];
                //float muon2_truth_trkIso = (event.muonPF2PATTrackIso[mu2]-event.muonPF2PATPt[mu2])/event.muonPF2PATPt[mu2]; 
                float muon1_truth_trkIso = (event.muonPF2PATTrackIso[mu1]);
                float muon2_truth_trkIso = (event.muonPF2PATTrackIso[mu2]); 
                //float muon1_truth_pfIso_new = event.muonPF2PATComRelIsodBeta[mu1];
                //float muon2_truth_pfIso_new = event.muonPF2PATComRelIsodBeta[mu2];
                if (delR_truth<0.3) { //other muon within cone of dR=0.3   
                    //muon1_truth_trkIso = muon1_truth_trkIso-(event.muonPF2PATPt[mu2]/event.muonPF2PATPt[mu1]);
                    //muon2_truth_trkIso = muon2_truth_trkIso-(event.muonPF2PATPt[mu1]/event.muonPF2PATPt[mu2]);
                    muon1_truth_trkIso = muon1_truth_trkIso-(event.muonPF2PATPt[mu2]);
                    muon2_truth_trkIso = muon2_truth_trkIso-(event.muonPF2PATPt[mu1]);
                }
                /*if (delR_truth<0.4) { //other muon within cone of dR=0.4   
                    muon1_truth_pfIso_new = muon1_truth_pfIso_new-(event.muonPF2PATPt[mu2]/event.muonPF2PATPt[mu1]);
                    muon2_truth_pfIso_new = muon2_truth_pfIso_new-(event.muonPF2PATPt[mu1]/event.muonPF2PATPt[mu2]);
                }*/
                //cout<<"Pt vals: "<<muon1_truth.Pt()<<", "<<muon2_truth.Pt()<<endl;
                //cout<<"PFIsolation new vals: "<<event.muonPF2PATComRelIsodBeta[mu1]<<", "<<event.muonPF2PATComRelIsodBeta[mu2]<<endl;
                //cout<<"TrkIsolation new vals: "<<muon1_truth_trkIso<<", "<<muon2_truth_trkIso<<endl;

                
                //testing area
                //if (i>350) 
                //    cout<<"EventNo.:"<<i<<"dR:"<<muon1_truth.DeltaR(gen_mu1)<<", "<<muon2_truth.DeltaR(gen_mu2)<<", mass_gen:"<<(gen_mu1+gen_mu2).M()<<", "<<mass_truth<<endl;             
                //continue;
                
                //continue;
                
                double eventWeight = 1;
                eventWeight *= (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / abs(event.origWeightForNorm));
                eventWeight *= datasetWeight;
                
                // Fill gen level stuff
                h_numMuon_gen->Fill(numGenMuons);
                h_leadingMuonPt_gen->Fill(gen_mu1.Pt());
                h_subleadingMuonPt_gen->Fill(gen_mu2.Pt()); 
                h_delR_gen->Fill(gen_mu1.DeltaR(gen_mu2));
                h_diMuonMass_gen->Fill((gen_mu1+gen_mu2).M()); 
                
                //if ((event.muonPF2PATLooseCutId[mu1]!=1) || ( event.muonPF2PATLooseCutId[mu2]!=1)) continue;
                // Fill general pT/dR (with and without scalar parentage)
                h_leadingMuonPt_truth->Fill(event.muonPF2PATPt[mu1] );
                h_subLeadingMuonPt_truth->Fill(event.muonPF2PATPt[mu2] );
                h_leadingMuonPt->Fill(event.muonPF2PATPt[0] );
                h_subLeadingMuonPt->Fill(event.muonPF2PATPt[1] );
                h_leadingMuonEta_truth->Fill(event.muonPF2PATEta[mu1] );
                h_subLeadingMuonEta_truth->Fill(event.muonPF2PATEta[mu2] );
                h_leadingMuonEta->Fill(event.muonPF2PATEta[0] );
                h_subLeadingMuonEta->Fill(event.muonPF2PATEta[1] );
                h_delR_truth->Fill(delR_truth );
                h_delR->Fill(delR );
                h_diMuonMass_truth->Fill(mass_truth );
                h_diMuonMass->Fill(mass );
                h_numMuonPF2PAT->Fill(event.numMuonPF2PAT);
                h_leadingmuonPF2PATD0_truth->Fill(event.muonPF2PATD0[mu1]);
                h_leadingmuonPF2PATDBPV_truth->Fill(event.muonPF2PATDBPV[mu1]);
                h_subleadingmuonPF2PATD0_truth->Fill(event.muonPF2PATD0[mu2]);
                h_subleadingmuonPF2PATDBPV_truth->Fill(event.muonPF2PATDBPV[mu2]);
                
                h_leadingMuon_pfIso_truth->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                h_subLeadingMuon_pfIso_truth->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                h_leadingMuon_trkIso_truth->Fill(muon1_truth_trkIso);
                h_subLeadingMuon_trkIso_truth->Fill(muon2_truth_trkIso);
                h_leadingMuon_trkIso_orig_truth->Fill(event.muonPF2PATTrackIso[mu1]);
                h_subLeadingMuon_trkIso_orig_truth->Fill(event.muonPF2PATTrackIso[mu2]);
                h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth->Fill(event.muonPF2PATComRelIsodBeta[mu1], event.muonPF2PATComRelIsodBeta[mu2]);
                h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth->Fill(muon1_truth_trkIso, muon2_truth_trkIso);
                h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth->Fill(event.muonPF2PATTrackIso[mu1], event.muonPF2PATTrackIso[mu2]);
                h_leadingMuon_Pt_subLeadingMuon_Pt_truth->Fill(event.muonPF2PATPt[mu1],event.muonPF2PATPt[mu2]);
                h_leadingMuon_Pt_delR_truth->Fill(event.muonPF2PATPt[mu1], delR_truth);
                h_subLeadingMuon_Pt_delR_truth->Fill(event.muonPF2PATPt[mu2], delR_truth);
                h_leadingMuon_Pt_leadingMuon_trkIso_truth->Fill(event.muonPF2PATPt[mu1], muon1_truth_trkIso);
                h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth->Fill(event.muonPF2PATPt[mu2], muon2_truth_trkIso);
                
                

                // Fill pT post trigger (with and without scalar parentage)
                if (is2018_==true) {
                if (passL2MuonTrigger) {


                    

                    foundEvents++;
                    foundEventsNorm += eventWeight;

                    h_leadingMuonPt_truth_muTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_muTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_muTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_muTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_muTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_muTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_muTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_muTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_muTrig->Fill(delR_truth);
                    h_delR_muTrig->Fill(delR);
                    h_diMuonMass_truth_muTrig->Fill(mass_truth);
                    h_diMuonMass_muTrig->Fill(mass);
                    h_numMuonPF2PAT_muTrig->Fill(event.numMuonPF2PAT);
                    h_leadingmuonPF2PATD0_truth_muTrig->Fill(event.muonPF2PATD0[mu1]);
                    h_leadingmuonPF2PATDBPV_truth_muTrig->Fill(event.muonPF2PATDBPV[mu1]);
                    h_subleadingmuonPF2PATD0_truth_muTrig->Fill(event.muonPF2PATD0[mu2]);
                    h_subleadingmuonPF2PATDBPV_truth_muTrig->Fill(event.muonPF2PATDBPV[mu2]);
                    
                    
                    h_leadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_truth_muTrig->Fill(muon1_truth_trkIso);
                    h_subLeadingMuon_trkIso_truth_muTrig->Fill(muon2_truth_trkIso);
                    h_leadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu1]);
                    h_subLeadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu1], event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig->Fill(muon1_truth_trkIso, muon2_truth_trkIso);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu1], event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig->Fill(event.muonPF2PATPt[mu1],event.muonPF2PATPt[mu2]);
                    h_leadingMuon_Pt_delR_truth_muTrig->Fill(event.muonPF2PATPt[mu1], delR_truth);
                    h_subLeadingMuon_Pt_delR_truth_muTrig->Fill(event.muonPF2PATPt[mu2], delR_truth);
                    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig->Fill(event.muonPF2PATPt[mu1], muon1_truth_trkIso);
                    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig->Fill(event.muonPF2PATPt[mu2], muon2_truth_trkIso);
                    }
                else {
                    /*double eventWeight = 1;

                    eventWeight *= (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / abs(event.origWeightForNorm));
                    eventWeight *= datasetWeight;*/
                    
                    h_leadingMuonPt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonEta_truth_muTrig_fail->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_muTrig_fail->Fill(event.muonPF2PATEta[mu2]);
                    h_delR_truth_muTrig_fail->Fill(delR_truth);
                    h_diMuonMass_truth_muTrig_fail->Fill(mass_truth);
                    h_numMuonPF2PAT_muTrig_fail->Fill(event.numMuonPF2PAT);
                    h_leadingmuonPF2PATD0_truth_muTrig_fail->Fill(event.muonPF2PATD0[mu1]);
                    h_leadingmuonPF2PATDBPV_truth_muTrig_fail->Fill(event.muonPF2PATDBPV[mu1]);
                    h_subleadingmuonPF2PATD0_truth_muTrig_fail->Fill(event.muonPF2PATD0[mu2]);
                    h_subleadingmuonPF2PATDBPV_truth_muTrig_fail->Fill(event.muonPF2PATDBPV[mu2]);
                    
                    h_leadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_truth_muTrig_fail->Fill(muon1_truth_trkIso);
                    h_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(muon2_truth_trkIso);
                    h_leadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1], event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(muon1_truth_trkIso, muon2_truth_trkIso);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATTrackIso[mu1], event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1],event.muonPF2PATPt[mu2]);
                    h_leadingMuon_Pt_delR_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1], delR_truth);
                    h_subLeadingMuon_Pt_delR_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2], delR_truth);
                    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1], muon1_truth_trkIso);
                    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2], muon2_truth_trkIso);
                    
                 }
              }
               else { //for 2017 SingleMu trigger
                if (passSingleMuonTrigger) {


                    

                    foundEvents++;
                    foundEventsNorm += eventWeight;

                    h_leadingMuonPt_truth_muTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_muTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_muTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_muTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_muTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_muTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_muTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_muTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_muTrig->Fill(delR_truth);
                    h_delR_muTrig->Fill(delR);
                    h_diMuonMass_truth_muTrig->Fill(mass_truth);
                    h_diMuonMass_muTrig->Fill(mass);
                    
                    
                    h_leadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_truth_muTrig->Fill(muon1_truth_trkIso);
                    h_subLeadingMuon_trkIso_truth_muTrig->Fill(muon2_truth_trkIso);
                    h_leadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu1]);
                    h_subLeadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig->Fill(event.muonPF2PATComRelIsodBeta[mu1], event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig->Fill(muon1_truth_trkIso, muon2_truth_trkIso);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig->Fill(event.muonPF2PATTrackIso[mu1], event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig->Fill(event.muonPF2PATPt[mu1],event.muonPF2PATPt[mu2]);
                    h_leadingMuon_Pt_delR_truth_muTrig->Fill(event.muonPF2PATPt[mu1], delR_truth);
                    h_subLeadingMuon_Pt_delR_truth_muTrig->Fill(event.muonPF2PATPt[mu2], delR_truth);
                    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig->Fill(event.muonPF2PATPt[mu1], muon1_truth_trkIso);
                    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig->Fill(event.muonPF2PATPt[mu2], muon2_truth_trkIso);
                    }
                else {
                    /*double eventWeight = 1;

                    eventWeight *= (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / abs(event.origWeightForNorm));
                    eventWeight *= datasetWeight;*/
                    
                    h_leadingMuonPt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonEta_truth_muTrig_fail->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_muTrig_fail->Fill(event.muonPF2PATEta[mu2]);
                    h_delR_truth_muTrig_fail->Fill(delR_truth);
                    h_diMuonMass_truth_muTrig_fail->Fill(mass_truth);
                    
                    h_leadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_truth_muTrig_fail->Fill(muon1_truth_trkIso);
                    h_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(muon2_truth_trkIso);
                    h_leadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1]);
                    h_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig_fail->Fill(event.muonPF2PATComRelIsodBeta[mu1], event.muonPF2PATComRelIsodBeta[mu2]);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(muon1_truth_trkIso, muon2_truth_trkIso);
                    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Fill(event.muonPF2PATTrackIso[mu1], event.muonPF2PATTrackIso[mu2]);
                    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1],event.muonPF2PATPt[mu2]);
                    h_leadingMuon_Pt_delR_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1], delR_truth);
                    h_subLeadingMuon_Pt_delR_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2], delR_truth);
                    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu1], muon1_truth_trkIso);
                    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig_fail->Fill(event.muonPF2PATPt[mu2], muon2_truth_trkIso);
                 }
              }
                
            }

//            if (! ( passDimuonTrigger || passSingleMuonTrigger ) ) continue;

//            vector<int> looseMuonIndex = getLooseMuons(event);
//            vector<int> promptLooseMuonIndex     = getPromptMuons(event, looseMuonIndex, true);
//            vector<int> nonpromptLooseMuonIndex  = getPromptMuons(event, looseMuonIndex, false);


        } // end event loop
        cerr << "\nFound " << foundEvents << " that pass single muon trigger in " << dataset->name() << endl;
        cerr << "Found " << foundEventsNorm << " after normalisation that pass single muon trigger in " << dataset->name() << endl;
        cerr << "\n\n";
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();
    /*
    h_leadingMuonPt_truth_muTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_muTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_muTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_muTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_muTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_muTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_muTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_muTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_muTrig->Divide(h_delR_truth);
    h_delR_muTrig->Divide(h_delR);
    h_diMuonMass_truth_muTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_muTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_mumuTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_mumuTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_mumuTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_mumuTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_mumuTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_mumuTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_mumuTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_mumuTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_mumuTrig->Divide(h_delR_truth);
    h_delR_mumuTrig->Divide(h_delR);
    h_diMuonMass_truth_mumuTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_mumuTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_mumuTrigNoMassCut->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_mumuTrigNoMassCut->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_mumuTrigNoMassCut->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_mumuTrigNoMassCut->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_mumuTrigNoMassCut->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_mumuTrigNoMassCut->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_mumuTrigNoMassCut->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_mumuTrigNoMassCut->Divide(h_subLeadingMuonEta);
    h_delR_truth_mumuTrigNoMassCut->Divide(h_delR_truth);
    h_delR_mumuTrigNoMassCut->Divide(h_delR);
    h_diMuonMass_truth_mumuTrigNoMassCut->Divide(h_diMuonMass_truth);
    h_diMuonMass_mumuTrigNoMassCut->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_mumuTrigMassCut->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_mumuTrigMassCut->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_mumuTrigMassCut->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_mumuTrigMassCut->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_mumuTrigMassCut->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_mumuTrigMassCut->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_mumuTrigMassCut->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_mumuTrigMassCut->Divide(h_subLeadingMuonEta);
    h_delR_truth_mumuTrigMassCut->Divide(h_delR_truth);
    h_delR_mumuTrigMassCut->Divide(h_delR);
    h_diMuonMass_truth_mumuTrigMassCut->Divide(h_diMuonMass_truth);
    h_diMuonMass_mumuTrigMassCut->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_L2muTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_L2muTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_L2muTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_L2muTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_L2muTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_L2muTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_L2muTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_L2muTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_L2muTrig->Divide(h_delR_truth);
    h_delR_L2muTrig->Divide(h_delR);
    h_diMuonMass_truth_L2muTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_L2muTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_muOrMumuTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_muOrMumuTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_muOrMumuTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_muOrMumuTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_muOrMumuTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_muOrMumuTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_muOrMumuTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_muOrMumuTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_muOrMumuTrig->Divide(h_delR_truth);
    h_delR_muOrMumuTrig->Divide(h_delR);
    h_diMuonMass_truth_muOrMumuTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_muOrMumuTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_ORTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_ORTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_ORTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_ORTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_ORTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_ORTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_ORTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_ORTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_ORTrig->Divide(h_delR_truth);
    h_delR_ORTrig->Divide(h_delR);
    h_diMuonMass_truth_ORTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_ORTrig->Divide(h_diMuonMass);
    */
    // Gen Level plots
    h_leadingMuonPt_gen->Write();
    h_subleadingMuonPt_gen->Write(); 
    h_delR_gen->Write();
    h_diMuonMass_gen->Write(); 
    h_numMuon_gen->Write();
    // Matched muon plots
    h_leadingMuonPt_truth->Write();
    h_subLeadingMuonPt_truth->Write();
    h_leadingMuonPt->Write();
    h_subLeadingMuonPt->Write();
    h_leadingMuonEta_truth->Write(); 
    h_subLeadingMuonEta_truth->Write(); 
    h_leadingMuonEta->Write();
    h_subLeadingMuonEta->Write(); 
    h_delR_truth->Write(); 
    h_delR->Write();   
    h_diMuonMass_truth->Write();  
    h_diMuonMass->Write();  
    h_numMuonPF2PAT->Write();
    h_leadingmuonPF2PATD0_truth->Write();
    h_leadingmuonPF2PATDBPV_truth->Write();
    h_subleadingmuonPF2PATD0_truth->Write();
    h_subleadingmuonPF2PATDBPV_truth->Write();
    
    h_leadingMuon_pfIso_truth->Write();
    h_subLeadingMuon_pfIso_truth->Write();
  std::map<std::string,TH1F*> hists_1d_;
    h_leadingMuon_trkIso_truth->Write();
    h_subLeadingMuon_trkIso_truth->Write();
    h_leadingMuon_trkIso_orig_truth->Write();
    h_subLeadingMuon_trkIso_orig_truth->Write();
    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth->Write();
    h_leadingMuon_Pt_subLeadingMuon_Pt_truth->Write();
    h_leadingMuon_Pt_delR_truth->Write();
    h_subLeadingMuon_Pt_delR_truth->Write();
    h_leadingMuon_Pt_leadingMuon_trkIso_truth->Write();
    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth->Write();
    // numerator - single mu
    h_leadingMuonPt_truth_muTrig->Write();
    h_subLeadingMuonPt_truth_muTrig->Write(); 
    h_leadingMuonPt_muTrig->Write();             
    h_subLeadingMuonPt_muTrig->Write();           
    h_leadingMuonEta_truth_muTrig->Write();       
    h_subLeadingMuonEta_truth_muTrig->Write();    
    h_leadingMuonEta_muTrig->Write();    
    h_subLeadingMuonEta_muTrig->Write(); 
    h_delR_truth_muTrig->Write();        
    h_delR_muTrig->Write();               
    h_diMuonMass_truth_muTrig->Write(); 
    h_diMuonMass_muTrig->Write();  
    h_numMuonPF2PAT_muTrig->Write();
    h_leadingmuonPF2PATD0_truth_muTrig->Write();
    h_leadingmuonPF2PATDBPV_truth_muTrig->Write();
    h_subleadingmuonPF2PATD0_truth_muTrig->Write();
    h_subleadingmuonPF2PATDBPV_truth_muTrig->Write();
    
    
    h_leadingMuon_pfIso_truth_muTrig->Write();
    h_subLeadingMuon_pfIso_truth_muTrig->Write();
    h_leadingMuon_trkIso_truth_muTrig->Write();
    h_subLeadingMuon_trkIso_truth_muTrig->Write();
    h_leadingMuon_trkIso_orig_truth_muTrig->Write();
    h_subLeadingMuon_trkIso_truth_muTrig->Write();
    h_subLeadingMuon_trkIso_orig_truth_muTrig->Write();
    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig->Write();
    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig->Write();
    h_leadingMuon_Pt_delR_truth_muTrig->Write();
    h_subLeadingMuon_Pt_delR_truth_muTrig->Write();
    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig->Write();
    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig->Write();
    // Failing single mu trigger
    h_leadingMuonPt_truth_muTrig_fail->Write(); 
    h_subLeadingMuonPt_truth_muTrig_fail->Write();  
    h_leadingMuonEta_truth_muTrig_fail->Write();   
    h_subLeadingMuonEta_truth_muTrig_fail->Write(); 
    h_delR_truth_muTrig_fail->Write();              
    h_diMuonMass_truth_muTrig_fail->Write();  
    h_numMuonPF2PAT_muTrig_fail->Write();  
    h_leadingmuonPF2PATD0_truth_muTrig_fail->Write();
    h_leadingmuonPF2PATDBPV_truth_muTrig_fail->Write();
    h_subleadingmuonPF2PATD0_truth_muTrig_fail->Write();
    h_subleadingmuonPF2PATDBPV_truth_muTrig_fail->Write(); 
     
    
    h_leadingMuon_pfIso_truth_muTrig_fail->Write();
    h_subLeadingMuon_pfIso_truth_muTrig_fail->Write();
    h_leadingMuon_trkIso_truth_muTrig_fail->Write();
    h_subLeadingMuon_trkIso_truth_muTrig_fail->Write();
    h_leadingMuon_trkIso_orig_truth_muTrig_fail->Write();
    h_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Write();
    h_leadingMuon_pfIso_subLeadingMuon_pfIso_truth_muTrig_fail->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_truth_muTrig_fail->Write();
    h_leadingMuon_trkIso_subLeadingMuon_trkIso_orig_truth_muTrig_fail->Write();
    h_leadingMuon_Pt_subLeadingMuon_Pt_truth_muTrig_fail->Write();
    h_leadingMuon_Pt_delR_truth_muTrig_fail->Write();
    h_subLeadingMuon_Pt_delR_truth_muTrig_fail->Write();
    h_leadingMuon_Pt_leadingMuon_trkIso_truth_muTrig_fail->Write();
    h_subLeadingMuon_Pt_subLeadingMuon_trkIso_truth_muTrig_fail->Write();
    outFile->Close();

//    cout << "Max nGenPar: " << maxGenPars << endl;    
    auto timerStop = chrono::high_resolution_clock::now(); 
    auto duration  = chrono::duration_cast<chrono::seconds>(timerStop - timerStart);

    cout << "\nFinished. Took " << duration.count() << " seconds" <<endl;
}

/*//____________________________________________________________________________
void quickKinematics::bookHists(edm::Service<TFileService>& fs, const std::string& suffix) {

  std::string suf(suffix);
  if (suffix.size()) suf = "_"+suffix;

  hists_1d_["h_pt"+suf] = fs->make<TH1F>(Form("h_pt%s",suf.c_str()) , "; p_{T} [GeV]" , 100 , 0. , 100. );
  hists_1d_["h_eta"+suf] = fs->make<TH1F>(Form("h_eta%s",suf.c_str()) , "; #eta" , 100 , -3. , 3. );
  hists_1d_["h_phi"+suf] = fs->make<TH1F>(Form("h_phi%s",suf.c_str()) , "; #phi" , 100 , -3.14 , 3.14 );

  return;
}

//____________________________________________________________________________
void quickKinematics::fillHists(const LorentzVector& lv, const std::string& suffix) {

  std::string suf(suffix);
  if (suffix.size()) suf = "_"+suffix;

  hists_1d_["h_pt"+suf]->Fill(lv.pt());
  hists_1d_["h_eta"+suf]->Fill(lv.eta());
  hists_1d_["h_phi"+suf]->Fill(lv.phi());

  return;
}
*/
pair<int,int> FindGen(const AnalysisEvent& event, int numGenMuons, vector<size_t>& idx) { //causes seg fault, don't use
    int charge_mu1, charge_mu2;
    // Sort by Pt and store sorted index
    // Choose top 2 muons, if charge same, go next until different charges
    int mu1_ind, mu2_ind;
    mu1_ind=idx[0];
    charge_mu1 = event.genMuonPF2PATCharge[mu1_ind];
    for (int k=1;k<idx.size();k++) {
        charge_mu2 = event.genMuonPF2PATCharge[idx[k]];
        if (charge_mu1!=charge_mu2) {
            mu2_ind=idx[k];
            break;
        }
    }
    return make_pair(mu1_ind, mu2_ind);
}

int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max) {
    double minDR = 100;
    unsigned index = 0;
    double tmpDR;
    for (int j=0; j<event.numMuonPF2PAT; j++) {
        //TLorentzVector gen_mu{event.genMuonPF2PATPX[j], event.genMuonPF2PATPY[j], event.genMuonPF2PATPZ[j], event.genMuonPF2PATE[j]};
        //TLorentzVector reco_mu{event.muonPF2PATPX[j], event.muonPF2PATPY[j], event.muonPF2PATPZ[j], event.muonPF2PATE[j]};
        //tmpDR=gen_mu.DeltaR(reco_mu);
        tmpDR=deltaR(event.genParEta[gen_ind],event.genParPhi[gen_ind],event.muonPF2PATEta[j],event.muonPF2PATPhi[j]);
        if (event.genParCharge[gen_ind]!= event.muonPF2PATCharge[j]) continue;
        if (minDR < tmpDR) continue;
        
        minDR = tmpDR;
        index = j;    
    }
    if (minDR < dr_max) 
        return index;
    else
        return -99;
}

vector<int> getLooseMuons(const AnalysisEvent& event) {
    vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {   
       if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] /*&& event.muonPF2PATPfIsoLoose[i]*/ && abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
           if (event.muonPF2PATPt[i] >= (muons.empty() ? looseMuonPtLeading_ : looseMuonPt_)) muons.emplace_back(i);
        }
    }
    return muons;
}

vector<int> getPromptMuons(const AnalysisEvent& event, const vector<int>& muonIndex, const bool getPrompt ) {
    vector<int> muons;
    for ( auto it = muonIndex.begin(); it!= muonIndex.end(); it++ ) {
        if ( event.genMuonPF2PATHardProcess[*it] == getPrompt ) muons.push_back(*it);
    }
    return muons;
}

bool getDileptonCand(AnalysisEvent& event, const vector<int>& muons, bool mcTruth) {
    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

            if (event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
            if (event.muonPF2PATPt[j] <= looseMuonPt_) continue;

            if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;
            if ( mcTruth && event.genMuonPF2PATMotherId[muons[i]] == 9000006 && event.genMuonPF2PATMotherId[muons[j]] == 9000006) continue;

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

                const float iso1 = ch_iso1 + max( float(0.0), neutral_iso1 - float(0.5*pu_iso1) );
                const float iso2 = ch_iso2 + max( float(0.0), neutral_iso2 - float(0.5*pu_iso2) );
                const float iso  = ch_iso  + max( float(0.0), neutral_iso  - float(0.5*pu_iso)  );

                const float trkiso1 = ch_trkiso1 + max( float(0.0), neutral_trkiso1 - float(0.5*pu_trkiso1) );
                const float trkiso2 = ch_trkiso2 + max( float(0.0), neutral_trkiso2 - float(0.5*pu_trkiso2) );
                const float trkiso  = ch_trkiso  + max( float(0.0), neutral_trkiso  - float(0.5*pu_trkiso)  );

                event.zPairNewRelIso.first  = iso1/(event.zPairLeptons.first.Pt() + 1.0e-06);
                event.zPairNewRelIso.second = iso2/(event.zPairLeptons.second.Pt() + 1.0e-06);
                event.zRelIso = iso/((event.zPairLeptons.first+event.zPairLeptons.second).Pt() + 1.0e-06);

                event.zPairNewTrkIso.first  = trkiso1/(event.zPairLeptons.first.Pt() + 1.0e-06);
                event.zPairNewTrkIso.second = trkiso2/(event.zPairLeptons.second.Pt() + 1.0e-06);
                event.zTrkIso = trkiso/((event.zPairLeptons.first+event.zPairLeptons.second).Pt() + 1.0e-06);

//                if ( event.zRelIso > 0.2 ) continue;

                event.mumuTrkIndex = getMuonTrackPairIndex(event);

//                if ( (event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxNdof[event.mumuTrkIndex]+1.0e-06) > 10. ) continue;

                event.zPairLeptonsRefitted.first  = TLorentzVector{event.muonTkPairPF2PATTk1Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Pz[event.mumuTrkIndex], sqrt(event.muonTkPairPF2PATTk1P2[event.mumuTrkIndex]+pow(0.1057,2))};
                event.zPairLeptonsRefitted.second = TLorentzVector{event.muonTkPairPF2PATTk2Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Pz[event.mumuTrkIndex], sqrt(event.muonTkPairPF2PATTk2P2[event.mumuTrkIndex]+pow(0.1057,2))};

                return true;
            }
        }
    }
    return false;
}

int getMuonTrackPairIndex(const AnalysisEvent& event) { 
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] != event.zPairIndex.first) continue;
        if (event.muonTkPairPF2PATIndex2[i] != event.zPairIndex.second) continue;
        return i;
    }
    return -1;
}

float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << sqrt((dEta*dEta)+(dPhi*dPhi)) << endl;
  return sqrt((dEta*dEta)+(dPhi*dPhi));
}

void SetStyle(Bool_t graypalette) {
    //cout << "Setting style!" << endl;
    gStyle->Reset("Plain");
    //gStyle->SetOptTitle(0);
    //gStyle->SetOptStat(0);
    if(graypalette) gStyle->SetPalette(8,0);
    else gStyle->SetPalette(1);
    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(2);
    gStyle->SetFrameLineWidth(1);
    gStyle->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadGridY(true);
    gStyle->SetLabelFont(42,"xy");
    gStyle->SetTitleFont(42,"xy");
    gStyle->SetLabelSize(10,"x");
    gStyle->SetTitleSize(10,"x");
    gStyle->SetTitleOffset(1.3,"xy");
    gStyle->SetLabelSize(0.035,"y");
    gStyle->SetTitleSize(0.035,"y");
    gStyle->SetNdivisions(510,"xy");
    gStyle->SetMarkerSize(1);
    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendTextSize(0.03);
    gStyle->SetLegendFillColor(0);
    /*
     gStyle->SetHistLineWidth(1);
    gStyle->SetHistLineColor(kRed);
    gStyle->SetFuncWidth(2);
    gStyle->SetFuncColor(kBlack);
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.045,"xyz");
    gStyle->SetLabelOffset(0.015,"y");
    gStyle->SetLabelOffset(0.01,"x");
    gStyle->SetLabelColor(kBlack,"xyz");
    gStyle->SetTitleSize(0.05,"xyz");
    gStyle->SetTitleOffset(1.81,"y");
    gStyle->SetTitleOffset(1.25,"x");
    gStyle->SetTitleFillColor(kWhite);
    gStyle->SetTextSizePixels(26);
    gStyle->SetTextFont(42);
    gStyle->SetTickLength(0.03,"X");
    gStyle->SetTickLength(0.03,"Y"); 
    gStyle->SetLegendFillColor(kWhite);
    gStyle->SetLegendFont(42);*/
}
