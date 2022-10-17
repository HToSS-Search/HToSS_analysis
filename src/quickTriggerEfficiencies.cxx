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
std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt );

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth = false);
int getMuonTrackPairIndex(const AnalysisEvent& event);
float deltaR(float eta1, float phi1, float eta2, float phi2);
int MatchReco(int gen_ind, const AnalysisEvent& event, double dr_max);

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.8}, looseMuonPt_ {6.}, looseMuonPtLeading_ {15.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0};
// Diparticle cuts
double maxDileptonDeltaR_ {0.2};

int main(int argc, char* argv[])
{
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

    // Quick and dirty trigger plots
    // denom
    TH1F* h_leadingMuonPt_truth                  {new TH1F("h_leadingMuonPt_truth",      "", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_truth_subPtcut         {new TH1F("h_leadingMuonPt_truth_subPtcut",      "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth               {new TH1F("h_subLeadingMuonPt_truth",   "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_Ptcut         {new TH1F("h_subLeadingMuonPt_truth_Ptcut",   "", 200, 0., 100.)};
    TH1F* h_leadingMuonPt                        {new TH1F("h_leadingMuonPt",            "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt                     {new TH1F("h_subLeadingMuonPt",         "", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth                 {new TH1F("h_leadingMuonEta_truth",     "", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth              {new TH1F("h_subLeadingMuonEta_truth",  "", 300, -3., 3.)};
    TH1F* h_leadingMuonEta                       {new TH1F("h_leadingMuonEta",           "", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta                    {new TH1F("h_subLeadingMuonEta",        "", 300, -3., 3.)};
    TH1F* h_delR_truth                           {new TH1F("h_delR_truth",               "", 100, 0., 1.0)};
    TH1F* h_delR                                 {new TH1F("h_delR",                     "", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth                     {new TH1F("h_diMuonMass_truth",         "", 200, 0., 100.)}; 
    TH1F* h_diMuonMass                           {new TH1F("h_diMuonMass",               "", 200, 0., 100.)};
    TH2F* h_leadingMuonPt_subLeadingMuonPt_truth {new TH2F("h_leadingMuonPt_subLeadingMuonPt_truth", "p_{T} truth reco; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    TH2F* h_leadingMuonPt_subLeadingMuonPt {new TH2F("h_leadingMuonPt_subLeadingMuonPt", "p_{T} truth reco; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
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
    TH1F* h_diMuonMass_truth_muTrig              {new TH1F("h_diMuonMass_truth_muTrig",         "Trigger turn-on for signal; m_{#mu#mu}; #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_muTrig                    {new TH1F("h_diMuonMass_muTrig",               "Trigger turn-on for signal; m_{#mu#mu}; #mu trigger #epsilon", 200, 0., 100.)};
    // numerator - double mu
    TH1F* h_leadingMuonPt_truth_mumuTrig         {new TH1F("h_leadingMuonPt_truth_mumuTrig",      "Trigger turn-on for signal; p_{T} (GeV); #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_mumuTrig      {new TH1F("h_subLeadingMuonPt_truth_mumuTrig",   "Trigger turn-on for signal; p_{T} (GeV); #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_mumuTrig               {new TH1F("h_leadingMuonPt_mumuTrig",            "Trigger turn-on for signal; p_{T} (GeV); #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_mumuTrig            {new TH1F("h_subLeadingMuonPt_mumuTrig",         "Trigger turn-on for signal; p_{T} (GeV); #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_mumuTrig        {new TH1F("h_leadingMuonEta_truth_mumuTrig",     "Trigger turn-on for signal; #eta; #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_mumuTrig     {new TH1F("h_subLeadingMuonEta_truth_mumuTrig",  "Trigger turn-on for signal; #eta; #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_mumuTrig              {new TH1F("h_leadingMuonEta_mumuTrig",           "Trigger turn-on for signal; #eta; #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_mumuTrig           {new TH1F("h_subLeadingMuonEta_mumuTrig",        "Trigger turn-on for signal; #eta; #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_mumuTrig                  {new TH1F("h_delR_truth_mumuTrig",               "Trigger turn-on for signal; #Delta R; #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_mumuTrig                        {new TH1F("h_delR_mumuTrig",                     "Trigger turn-on for signal;  #Delta R; #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_mumuTrig            {new TH1F("h_diMuonMass_truth_mumuTrig",         "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_mumuTrig                  {new TH1F("h_diMuonMass_mumuTrig",               "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu trigger #epsilon", 200, 0., 100.)};    
    // numerator - double mu with mass cuts
    TH1F* h_leadingMuonPt_truth_mumuTrigMassCut      {new TH1F("h_leadingMuonPt_truth_mumuTrigMassCut",    "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_mumuTrigMassCut   {new TH1F("h_subLeadingMuonPt_truth_mumuTrigMassCut", "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_mumuTrigMassCut            {new TH1F("h_leadingMuonPt_mumuTrigMassCut",          "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_mumuTrigMassCut         {new TH1F("h_subLeadingMuonPt_mumuTrigMassCut",       "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_mumuTrigMassCut     {new TH1F("h_leadingMuonEta_truth_mumuTrigMassCut",   "Trigger turn-on for signal; #eta; #mu#mu+mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_mumuTrigMassCut  {new TH1F("h_subLeadingMuonEta_truth_mumuTrigMassCut","Trigger turn-on for signal; #eta; #mu#mu+mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_mumuTrigMassCut           {new TH1F("h_leadingMuonEta_mumuTrigMassCut",         "Trigger turn-on for signal; #eta; #mu#mu+mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_mumuTrigMassCut        {new TH1F("h_subLeadingMuonEta_mumuTrigMassCut",       "Trigger turn-on for signal; #eta; #mu#mu+mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_mumuTrigMassCut               {new TH1F("h_delR_truth_mumuTrigMassCut",             "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_mumuTrigMassCut                     {new TH1F("h_delR_mumuTrigMassCut",                   "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+mass cut trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_mumuTrigMassCut         {new TH1F("h_diMuonMass_truth_mumuTrigMassCut",       "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_mumuTrigMassCut               {new TH1F("h_diMuonMass_mumuTrigMassCut",             "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu+mass cut trigger #epsilon", 200, 0., 100.)};
    // numerator - double mu with mass cuts
    TH1F* h_leadingMuonPt_truth_mumuTrigNoMassCut    {new TH1F("h_leadingMuonPt_truth_mumuTrigNoMassCut",    "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_mumuTrigNoMassCut {new TH1F("h_subLeadingMuonPt_truth_mumuTrigNoMassCut", "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_mumuTrigNoMassCut          {new TH1F("h_leadingMuonPt_mumuTrigNoMassCut",          "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_mumuTrigNoMassCut       {new TH1F("h_subLeadingMuonPt_mumuTrigNoMassCut",       "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_mumuTrigNoMassCut   {new TH1F("h_leadingMuonEta_truth_mumuTrigNoMassCut",   "Trigger turn-on for signal; #eta; #mu#mu+no mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_mumuTrigNoMassCut{new TH1F("h_subLeadingMuonEta_truth_mumuTrigNoMassCut","Trigger turn-on for signal; #eta; #mu#mu+no mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_mumuTrigNoMassCut         {new TH1F("h_leadingMuonEta_mumuTrigNoMassCut",         "Trigger turn-on for signal; #eta; #mu#mu+no mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_mumuTrigNoMassCut      {new TH1F("h_subLeadingMuonEta_mumuTrigNoMassCut",      "Trigger turn-on for signal; #eta; #mu#mu+no mass cut trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_mumuTrigNoMassCut             {new TH1F("h_delR_truth_mumuTrigNoMassCut",             "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_mumuTrigNoMassCut                   {new TH1F("h_delR_mumuTrigNoMassCut",                   "Trigger turn-on for signal; p_{T} (GeV); #mu#mu+no mass cut trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_mumuTrigNoMassCut       {new TH1F("h_diMuonMass_truth_mumuTrigNoMassCut",       "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_mumuTrigNoMassCut             {new TH1F("h_diMuonMass_mumuTrigNoMassCut",             "Trigger turn-on for signal; m_{#mu#mu}; #mu#mu+no mass cut trigger #epsilon", 200, 0., 100.)};
    // numerator - L2 mu
    TH1F* h_leadingMuonPt_truth_L2muTrig         {new TH1F("h_leadingMuonPt_truth_L2muTrig",      "Trigger turn-on for signal; p_{T} (GeV); L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_L2muTrig      {new TH1F("h_subLeadingMuonPt_truth_L2muTrig",   "Trigger turn-on for signal; p_{T} (GeV); L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_L2muTrig               {new TH1F("h_leadingMuonPt_L2muTrig",            "Trigger turn-on for signal; p_{T} (GeV); L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_L2muTrig            {new TH1F("h_subLeadingMuonPt_L2muTrig",         "Trigger turn-on for signal; p_{T} (GeV); L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_L2muTrig        {new TH1F("h_leadingMuonEta_truth_L2muTrig",     "Trigger turn-on for signal; #eta; L2 #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_L2muTrig     {new TH1F("h_subLeadingMuonEta_truth_L2muTrig",  "Trigger turn-on for signal; #eta; L2 #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_L2muTrig              {new TH1F("h_leadingMuonEta_L2muTrig",           "Trigger turn-on for signal; #eta; L2 #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_L2muTrig           {new TH1F("h_subLeadingMuonEta_L2muTrig",        "Trigger turn-on for signal; #eta; L2 #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_L2muTrig                  {new TH1F("h_delR_truth_L2muTrig",               "Trigger turn-on for signal; #Delta R; L2 #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_L2muTrig                        {new TH1F("h_delR_L2muTrig",                     "Trigger turn-on for signal; #Delta R;  L2 #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_L2muTrig            {new TH1F("h_diMuonMass_truth_L2muTrig",         "Trigger turn-on for signal; m_{#mu#mu}; L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_L2muTrig                  {new TH1F("h_diMuonMass_L2muTrig",               "Trigger turn-on for signal; m_{#mu#mu}; L2 #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH2F* h_leadingMuonPt_subLeadingMuonPt_truth_L2muTrig {new TH2F("h_leadingMuonPt_subLeadingMuonPt_truth_L2muTrig", "p_{T} truth reco; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    TH2F* h_leadingMuonPt_subLeadingMuonPt_L2muTrig {new TH2F("h_leadingMuonPt_subLeadingMuonPt_L2muTrig", "p_{T} truth reco; Leading #mu p_{T} (GeV); SubLeading #mu p_{T} (GeV)", 200, 0.0, 100, 200, 0., 100)};
    // numerator single OR double mu
    TH1F* h_leadingMuonPt_truth_muOrL2muTrig     {new TH1F("h_leadingMuonPt_truth_muOrL2muTrig",      "Trigger turn-on for signal; p_{T} (GeV); #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_muOrL2muTrig  {new TH1F("h_subLeadingMuonPt_truth_muOrL2muTrig",   "Trigger turn-on for signal; p_{T} (GeV); #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_muOrL2muTrig           {new TH1F("h_leadingMuonPt_muOrL2muTrig",            "Trigger turn-on for signal; p_{T} (GeV); #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_muOrL2muTrig        {new TH1F("h_subLeadingMuonPt_muOrL2muTrig",         "Trigger turn-on for signal; p_{T} (GeV); #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_muOrL2muTrig    {new TH1F("h_leadingMuonEta_truth_muOrL2muTrig",     "Trigger turn-on for signal; #eta; #mu OR #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_muOrL2muTrig {new TH1F("h_subLeadingMuonEta_truth_muOrL2muTrig",  "Trigger turn-on for signal; #eta; #mu OR #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_muOrL2muTrig          {new TH1F("h_leadingMuonEta_muOrL2muTrig",           "Trigger turn-on for signal; #eta; #mu OR #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_muOrL2muTrig       {new TH1F("h_subLeadingMuonEta_muOrL2muTrig",        "Trigger turn-on for signal; #eta; #mu OR #mu#mu trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_muOrL2muTrig              {new TH1F("h_delR_truth_muOrL2muTrig",               "Trigger turn-on for signal; #Delta R; #mu OR #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_muOrL2muTrig                    {new TH1F("h_delR_muOrL2muTrig",                     "Trigger turn-on for signal; #Delta R; #mu OR #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_muOrL2muTrig        {new TH1F("h_diMuonMass_truth_muOrL2muTrig",         "Trigger turn-on for signal; m_{#mu#mu}; #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_muOrL2muTrig              {new TH1F("h_diMuonMass_muOrL2muTrig",               "Trigger turn-on for signal; m_{#mu#mu}; #mu OR #mu#mu trigger #epsilon", 200, 0., 100.)};
    // numerator single OR double or L2 mu
    TH1F* h_leadingMuonPt_truth_ORTrig           {new TH1F("h_leadingMuonPt_truth_ORTrig",      "Trigger turn-on for signal; p_{T} (GeV); OR all #mu triggers #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_ORTrig        {new TH1F("h_subLeadingMuonPt_truth_ORTrig",   "Trigger turn-on for signal; p_{T} (GeV); OR all #mu triggers #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_ORTrig                 {new TH1F("h_leadingMuonPt_ORTrig",            "Trigger turn-on for signal; p_{T} (GeV); OR all #mu triggers #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_ORTrig              {new TH1F("h_subLeadingMuonPt_ORTrig",         "Trigger turn-on for signal; p_{T} (GeV); OR all #mu triggers #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_ORTrig          {new TH1F("h_leadingMuonEta_truth_ORTrig",     "Trigger turn-on for signal; #eta; OR all #mu triggers #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_ORTrig       {new TH1F("h_subLeadingMuonEta_truth_ORTrig",  "Trigger turn-on for signal; #eta; OR all #mu triggers #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_ORTrig                {new TH1F("h_leadingMuonEta_ORTrig",           "Trigger turn-on for signal; #eta; OR all #mu triggers #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_ORTrig             {new TH1F("h_subLeadingMuonEta_ORTrig",        "Trigger turn-on for signal; #eta; OR all #mu triggers #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_ORTrig                    {new TH1F("h_delR_truth_ORTrig",               "Trigger turn-on for signal; #Delta R; OR all #mu triggers #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_ORTrig                          {new TH1F("h_delR_ORTrig",                     "Trigger turn-on for signal; #Delta R; OR all #mu triggers #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_ORTrig              {new TH1F("h_diMuonMass_truth_ORTrig",         "Trigger turn-on for signal; m_{#mu#mu}; OR all #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_ORTrig                    {new TH1F("h_diMuonMass_ORTrig",               "Trigger turn-on for signal; m_{#mu#mu}; OR all #mu trigger #epsilon", 200, 0., 100.)};

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

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
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
    catch (po::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
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
    catch (const std::exception)
    {
        std::cerr << "ERROR Problem with a confugration file, see previous "
                     "errors for more details. If this is the only error, the "
                     "problem is with the main configuration file."
                  << std::endl;
        throw;
    }

    if (totalLumi == 0.)
    {
        totalLumi = usePreLumi;
    }
    std::cout << "Using lumi: " << totalLumi << std::endl;

    bool datasetFilled{false};
    std::string era {""};
    if (is2016_) era = "2016";
    else if (is2018_) era = "2018";
    else era = "2017";
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};
//    const std::string postLepSelSkimInputDir{std::string{"/user/almorton/HToSS_analysis/postLepSkims"} + era + "/"};

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset)
    {
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

        int foundEvents{0};
        double foundEventsNorm{0.0};

        TH1I* generatorWeightPlot{nullptr};
        if (dataset->isMC()) {
            if (usePostLepTree) {
                std::string inputPostfix{"mumu"};
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

            float eventWeight = 1.;

            if (!event.metFilters()) continue;

            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};
            const bool passDiMuonNoMassCutTrigger {event.mumuTrig_noMassCut()}, passDiMuonMassCutTrigger {event.mumuTrig_massCut()};

            if ( event.numMuonPF2PAT >= 0 ) {
                // fill muon pT plots pre-triggers
                //// ID requirements PF muon? no pT cut
                //// reco pT 
                

                int numGenMuons=0;
                std::vector<int> gen_mu_ind;
                for (int k=0;k<event.nGenPar;k++) {
                    if (event.genParStatus[k]!=1) continue; //final state
                    if (abs(event.genParId[k])!=13) continue; //Muon PDG ID
                    if ((abs(event.genParMotherId[k])==9000006)) {
                        numGenMuons++;
                        //std::cout<<"Particle #"<<k<<", "<<event.genParCharge[k]<<", "<<event.genParMotherId[k]<<", "<<event.genParDaughterId1[k]<<", "<<event.genParDaughterId2[k]<<", "<<event.genParPt[k]<<std::endl;
                        gen_mu_ind.push_back(k);
                    }
                }
                //std::cout<<"number of GenMuons:"<<numGenMuons<<" and "<<gen_mu_ind.size()<<std::endl;
                //if (numGenMuons!=2) std::cout<<"ERROR: number of GenMuons:"<<numGenMuons<<" and "<<gen_mu_ind.size()<<std::endl;                 
								if (numGenMuons!=2) continue; //strict condition to only select events with 2 genMuons
                
                int tmp1;
                if (event.genParPt[gen_mu_ind[1]]>event.genParPt[gen_mu_ind[0]]) {
                    tmp1=gen_mu_ind[0];
                    gen_mu_ind[0]=gen_mu_ind[1];
                    gen_mu_ind[1]=tmp1;
                }
                
                TLorentzVector gen_mu1, gen_mu2;
                double mu_mass=0.10565837; //in GeV
                gen_mu1.SetPtEtaPhiM(event.genParPt[gen_mu_ind[0]], event.genParEta[gen_mu_ind[0]], event.genParPhi[gen_mu_ind[0]], mu_mass);
                gen_mu2.SetPtEtaPhiM(event.genParPt[gen_mu_ind[1]], event.genParEta[gen_mu_ind[1]], event.genParPhi[gen_mu_ind[1]], mu_mass);       
                
                //if (numGenMuons>2) cout<<"YO SOMETHING IS UP"<<std::endl;
                /*
                double match_dR = 0.05; //0.03 taken from MuonAnalyzer code used by MuonPOG
                int mu1 = MatchReco(gen_mu_ind[0], event, match_dR);
                int mu2 = MatchReco(gen_mu_ind[1], event, match_dR);
                bool flag = true; 
                if ((mu1==mu2) && (mu1!=-99)) {
                    std::cout<<"SOMETHING IS UP HERE TOO"<<std::endl;
                    std::cout<<"Indices-"<<gen_mu_ind[0]<<", "<<gen_mu_ind[1]<<std::endl;
                    std::cout<<"Pt of gen-"<<event.genParPt[gen_mu_ind[0]]<<", "<<event.genParPt[gen_mu_ind[1]]<<std::endl;
                    std::cout<<"mu1, mu2"<<mu1<<", "<<mu2<<std::endl;
                }
         
                if ((mu1<0)||(mu2<0)) {
                    //std::cout<<"reaches this"<<std::endl;
                    //continue;
                    flag=false;
                }
                flag=true; 
								if (flag) continue;*/
                //const TLorentzVector muon1_truth {event.muonPF2PATPX[mu1], event.muonPF2PATPY[mu1], event.muonPF2PATPZ[mu1], event.muonPF2PATE[mu1]};
                //const TLorentzVector muon2_truth {event.muonPF2PATPX[mu2], event.muonPF2PATPY[mu2], event.muonPF2PATPZ[mu2], event.muonPF2PATE[mu2]}; 
                //const TLorentzVector muon1 {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
                //const TLorentzVector muon2 {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]}; 
								if (abs(gen_mu1.Eta()>2.4) || abs(gen_mu2.Eta()>2.4)) continue;	
                const float delR_truth = deltaR(gen_mu1.Eta(), gen_mu1.Phi(), gen_mu2.Eta(), gen_mu2.Phi());
                const float delR       = deltaR(gen_mu1.Eta(), gen_mu1.Phi(), gen_mu2.Eta(), gen_mu2.Phi());
                const float mass_truth = (gen_mu1 + gen_mu2).M();
                const float mass       = (gen_mu1 + gen_mu2).M();

                // Fill general pT/dR (with and without scalar parentage)
                h_leadingMuonPt_truth->Fill(gen_mu1.Pt());
                h_subLeadingMuonPt_truth->Fill(gen_mu2.Pt());
                if ((gen_mu1.Pt()>=0)) h_leadingMuonPt_truth_subPtcut->Fill(gen_mu1.Pt());
                if ((gen_mu2.Pt()>=0)) h_subLeadingMuonPt_truth_Ptcut->Fill(gen_mu1.Pt());
                h_leadingMuonPt->Fill(gen_mu1.Pt());
                h_subLeadingMuonPt->Fill(gen_mu2.Pt());
                h_leadingMuonPt_subLeadingMuonPt_truth->Fill(gen_mu1.Pt(),gen_mu1.Pt());
                h_leadingMuonPt_subLeadingMuonPt->Fill(gen_mu1.Pt(),gen_mu2.Pt());
                  h_leadingMuonEta_truth->Fill(gen_mu1.Eta());
                  h_subLeadingMuonEta_truth->Fill(gen_mu2.Eta());
                  h_leadingMuonEta->Fill(gen_mu1.Eta());
                  h_subLeadingMuonEta->Fill(gen_mu2.Eta());
                  h_delR_truth->Fill(delR_truth);
                  h_delR->Fill(delR);
                  h_diMuonMass_truth->Fill(mass_truth);
                  h_diMuonMass->Fill(mass);
                // Fill pT post trigger (with and without scalar parentage)
                if (passSingleMuonTrigger) {

                    double eventWeight = 1;

                    eventWeight *= (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / std::abs(event.origWeightForNorm));
                    eventWeight *= datasetWeight;

                    foundEvents++;
                    foundEventsNorm += eventWeight;

                    h_leadingMuonPt_truth_muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_muTrig->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_muTrig->Fill(gen_mu2.Eta());
                    h_delR_truth_muTrig->Fill(delR_truth);
                    h_delR_muTrig->Fill(delR);
                    h_diMuonMass_truth_muTrig->Fill(mass_truth);
                    h_diMuonMass_muTrig->Fill(mass);
                }
                if (passDimuonNoVtxTrigger) {
										h_leadingMuonPt_truth_mumuTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_mumuTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_mumuTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_mumuTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_mumuTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_mumuTrig->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_mumuTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_mumuTrig->Fill(gen_mu2.Eta());
                    h_delR_truth_mumuTrig->Fill(delR_truth);
                    h_delR_mumuTrig->Fill(delR);
                    h_diMuonMass_truth_mumuTrig->Fill(mass_truth);
                    h_diMuonMass_mumuTrig->Fill(mass);

                }
                if (passDiMuonNoMassCutTrigger) {
										h_leadingMuonPt_truth_mumuTrigNoMassCut->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_mumuTrigNoMassCut->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_mumuTrigNoMassCut->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_mumuTrigNoMassCut->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_mumuTrigNoMassCut->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_mumuTrigNoMassCut->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_mumuTrigNoMassCut->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_mumuTrigNoMassCut->Fill(gen_mu2.Eta());
                    h_delR_truth_mumuTrigNoMassCut->Fill(delR_truth);
                    h_delR_mumuTrigNoMassCut->Fill(delR);
                    h_diMuonMass_truth_mumuTrigNoMassCut->Fill(mass_truth);
                    h_diMuonMass_mumuTrigNoMassCut->Fill(mass);


                } 
                if (passDiMuonMassCutTrigger) {
										h_leadingMuonPt_truth_mumuTrigMassCut->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_mumuTrigMassCut->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_mumuTrigMassCut->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_mumuTrigMassCut->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_mumuTrigMassCut->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_mumuTrigMassCut->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_mumuTrigMassCut->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_mumuTrigMassCut->Fill(gen_mu2.Eta());
                    h_delR_truth_mumuTrigMassCut->Fill(delR_truth);
                    h_delR_mumuTrigMassCut->Fill(delR);
                    h_diMuonMass_truth_mumuTrigMassCut->Fill(mass_truth);
                    h_diMuonMass_mumuTrigMassCut->Fill(mass);


                }
                if (passL2MuonTrigger) {
  									h_leadingMuonPt_truth_L2muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_L2muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_L2muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_L2muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_L2muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_L2muTrig->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_L2muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_L2muTrig->Fill(gen_mu2.Eta());
                    h_delR_truth_L2muTrig->Fill(delR_truth);
                    h_delR_L2muTrig->Fill(delR);
                    h_diMuonMass_truth_L2muTrig->Fill(mass_truth);
                    h_diMuonMass_L2muTrig->Fill(mass);

              }
                if (passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger) {
                		h_leadingMuonPt_truth_muOrL2muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_muOrL2muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_muOrL2muTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_muOrL2muTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_muOrL2muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_muOrL2muTrig->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_muOrL2muTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_muOrL2muTrig->Fill(gen_mu2.Eta());
                    h_delR_truth_muOrL2muTrig->Fill(delR_truth);
                    h_delR_muOrL2muTrig->Fill(delR);
                    h_diMuonMass_truth_muOrL2muTrig->Fill(mass_truth);
                    h_diMuonMass_muOrL2muTrig->Fill(mass);


								}
                if (passSingleMuonTrigger || passDimuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger) {
                		h_leadingMuonPt_truth_ORTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_truth_ORTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonPt_ORTrig->Fill(gen_mu1.Pt());
                    h_subLeadingMuonPt_ORTrig->Fill(gen_mu2.Pt());
                    h_leadingMuonEta_truth_ORTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_truth_ORTrig->Fill(gen_mu2.Eta());
                    h_leadingMuonEta_ORTrig->Fill(gen_mu1.Eta());
                    h_subLeadingMuonEta_ORTrig->Fill(gen_mu2.Eta());
                    h_delR_truth_ORTrig->Fill(delR_truth);
                    h_delR_ORTrig->Fill(delR);
                    h_diMuonMass_truth_ORTrig->Fill(mass_truth);
                    h_diMuonMass_ORTrig->Fill(mass);


								}
						}
//            if (! ( passDimuonTrigger || passSingleMuonTrigger ) ) continue;

//            std::vector<int> looseMuonIndex = getLooseMuons(event);
//            std::vector<int> promptLooseMuonIndex     = getPromptMuons(event, looseMuonIndex, true);
//            std::vector<int> nonpromptLooseMuonIndex  = getPromptMuons(event, looseMuonIndex, false);

        
        } // end event loop
        std::cerr << "\nFound " << foundEvents << " that pass single muon trigger in " << dataset->name() << std::endl;
        std::cerr << "Found " << foundEventsNorm << " after normalisation that pass single muon trigger in " << dataset->name() << std::endl;
        std::cerr << "\n\n";
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

		double eff_ORTrig, eff_SingleMu, eff_L2Mu;
		eff_ORTrig = h_leadingMuonPt_truth_ORTrig->GetEntries()/h_leadingMuonPt_truth->GetEntries();
		eff_SingleMu = h_leadingMuonPt_truth_muTrig->GetEntries()/h_leadingMuonPt_truth->GetEntries();
		eff_L2Mu = h_leadingMuonPt_truth_L2muTrig->GetEntries()/h_leadingMuonPt_truth->GetEntries();
		std::cout<<""<<eff_ORTrig<<", "<<eff_SingleMu<<", "<<eff_L2Mu<<std::endl;

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

    h_leadingMuonPt_truth_mumuTrig->Divide(h_leadingMuonPt_truth_subPtcut);
    h_subLeadingMuonPt_truth_mumuTrig->Divide(h_subLeadingMuonPt_truth_Ptcut);
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

    h_leadingMuonPt_truth_L2muTrig->Divide(h_leadingMuonPt_truth_subPtcut);
    h_subLeadingMuonPt_truth_L2muTrig->Divide(h_subLeadingMuonPt_truth_Ptcut);
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
    h_leadingMuonPt_subLeadingMuonPt_truth_L2muTrig->Divide(h_leadingMuonPt_subLeadingMuonPt_truth);
    h_leadingMuonPt_subLeadingMuonPt_L2muTrig->Divide(h_leadingMuonPt_subLeadingMuonPt);
    

    h_leadingMuonPt_truth_muOrL2muTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_muOrL2muTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_muOrL2muTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_muOrL2muTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_muOrL2muTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_muOrL2muTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_muOrL2muTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_muOrL2muTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_muOrL2muTrig->Divide(h_delR_truth);
    h_delR_muOrL2muTrig->Divide(h_delR);
    h_diMuonMass_truth_muOrL2muTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_muOrL2muTrig->Divide(h_diMuonMass);

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
    h_leadingMuonPt_truth_mumuTrig->Write();
    h_subLeadingMuonPt_truth_mumuTrig->Write();
    h_leadingMuonPt_mumuTrig->Write();
    h_subLeadingMuonPt_mumuTrig->Write();
    h_leadingMuonEta_truth_mumuTrig->Write();
    h_subLeadingMuonEta_truth_mumuTrig->Write();
    h_leadingMuonEta_mumuTrig->Write();
    h_subLeadingMuonEta_mumuTrig->Write();
    h_delR_truth_mumuTrig->Write();
    h_delR_mumuTrig->Write();
    h_diMuonMass_truth_mumuTrig->Write();
    h_diMuonMass_mumuTrig->Write();

    h_leadingMuonPt_truth_mumuTrigNoMassCut->Write();
    h_subLeadingMuonPt_truth_mumuTrigNoMassCut->Write();
    h_leadingMuonPt_mumuTrigNoMassCut->Write();
    h_subLeadingMuonPt_mumuTrigNoMassCut->Write();
    h_leadingMuonEta_truth_mumuTrigNoMassCut->Write();
    h_subLeadingMuonEta_truth_mumuTrigNoMassCut->Write();
    h_leadingMuonEta_mumuTrigNoMassCut->Write();
    h_subLeadingMuonEta_mumuTrigNoMassCut->Write();
    h_delR_truth_mumuTrigNoMassCut->Write();
    h_delR_mumuTrigNoMassCut->Write();
    h_diMuonMass_truth_mumuTrigNoMassCut->Write();
    h_diMuonMass_mumuTrigNoMassCut->Write();

    h_leadingMuonPt_truth_mumuTrigMassCut->Write();
    h_subLeadingMuonPt_truth_mumuTrigMassCut->Write();
    h_leadingMuonPt_mumuTrigMassCut->Write();
    h_subLeadingMuonPt_mumuTrigMassCut->Write();
    h_leadingMuonEta_truth_mumuTrigMassCut->Write();
    h_subLeadingMuonEta_truth_mumuTrigMassCut->Write();
    h_leadingMuonEta_mumuTrigMassCut->Write();
    h_subLeadingMuonEta_mumuTrigMassCut->Write();
    h_delR_truth_mumuTrigMassCut->Write();
    h_delR_mumuTrigMassCut->Write();
    h_diMuonMass_truth_mumuTrigMassCut->Write();
    h_diMuonMass_mumuTrigMassCut->Write();

    h_leadingMuonPt_truth_L2muTrig->Write();
    h_subLeadingMuonPt_truth_L2muTrig->Write();
    h_leadingMuonPt_L2muTrig->Write();
    h_subLeadingMuonPt_L2muTrig->Write();
    h_leadingMuonEta_truth_L2muTrig->Write();
    h_subLeadingMuonEta_truth_L2muTrig->Write();
    h_leadingMuonEta_L2muTrig->Write();
    h_subLeadingMuonEta_L2muTrig->Write();
    h_delR_truth_L2muTrig->Write();
    h_delR_L2muTrig->Write();
    h_diMuonMass_truth_L2muTrig->Write();
    h_diMuonMass_L2muTrig->Write();
    h_leadingMuonPt_subLeadingMuonPt_truth_L2muTrig->Write();
    h_leadingMuonPt_subLeadingMuonPt_truth->Write();
    h_leadingMuonPt_subLeadingMuonPt_L2muTrig->Write();
    h_leadingMuonPt_subLeadingMuonPt->Write();
    
    
    h_leadingMuonPt_truth_muOrL2muTrig->Write();
    h_subLeadingMuonPt_truth_muOrL2muTrig->Write();
    h_leadingMuonPt_muOrL2muTrig->Write();
    h_subLeadingMuonPt_muOrL2muTrig->Write();
    h_leadingMuonEta_truth_muOrL2muTrig->Write();
    h_subLeadingMuonEta_truth_muOrL2muTrig->Write();
    h_leadingMuonEta_muOrL2muTrig->Write();
    h_subLeadingMuonEta_muOrL2muTrig->Write();
    h_delR_truth_muOrL2muTrig->Write();
    h_delR_muOrL2muTrig->Write();
    h_diMuonMass_truth_muOrL2muTrig->Write();
    h_diMuonMass_muOrL2muTrig->Write();
    h_leadingMuonPt_truth_ORTrig->Write();
    h_subLeadingMuonPt_truth_ORTrig->Write();
    h_leadingMuonPt_ORTrig->Write();
    h_subLeadingMuonPt_ORTrig->Write();
    h_leadingMuonEta_truth_ORTrig->Write();
    h_subLeadingMuonEta_truth_ORTrig->Write();
    h_leadingMuonEta_ORTrig->Write();
    h_subLeadingMuonEta_ORTrig->Write();
    h_delR_truth_ORTrig->Write();
    h_delR_ORTrig->Write();
    h_diMuonMass_truth_ORTrig->Write();
    h_diMuonMass_ORTrig->Write();

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

std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt ) {
    std::vector<int> muons;
    for ( auto it = muonIndex.begin(); it!= muonIndex.end(); it++ ) {
        if ( event.genMuonPF2PATHardProcess[*it] == getPrompt ) muons.push_back(*it);
    }
    return muons;
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

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth) {
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
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}


