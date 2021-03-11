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
bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons);
bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs);
int getMuonTrackPairIndex(const AnalysisEvent& event);
int getChsTrackPairIndex(const AnalysisEvent& event);
bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);
float deltaR(float eta1, float phi1, float eta2, float phi2);

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {5.}, looseMuonRelIso_ {100.};
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

    // muons

    TH1F* h_leadingMuonPFIso                         {new TH1F ("h_leadingMuonPFIso",                      "Leading muon PF iso; iso", 500, 0., 15.)};
    TH1F* h_subleadingMuonPFIso                      {new TH1F ("h_subleadingMuonPFIso",                   "Subleading muon PF iso; iso", 500, 0., 15.)};
    TH1F* h_leadingMuonPFRelIso                      {new TH1F ("h_leadingMuonPFRelIso",                   "Leading muon PF rel iso; rel iso", 500, 0., 5.)};
    TH1F* h_subleadingMuonPFRelIso                   {new TH1F ("h_subleadingMuonPFRelIso",                "Subleading muon PF rel iso; rel iso", 500, 0., 5.)};

    TH1F* h_leadingMuonTrkIso_0p3                    {new TH1F ("h_leadingMuonTrkIso_0p3",                 "Leading muon trk iso (0.3 cone); iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonTrkIso_0p4                    {new TH1F ("h_leadingMuonTrkIso_0p4",                 "Leading muon trk iso (0.4 cone); iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandIsoNoPU_0p3             {new TH1F ("h_leadingMuonPfCandIsoNoPU_0p3",          "Leading muon pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandIsoNoPU_0p4             {new TH1F ("h_leadingMuonPfCandIsoNoPU_0p4",          "Leading muon pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandPtIsoNoPU_0p3           {new TH1F ("h_leadingMuonPfCandPtIsoNoPU_0p3",        "Leading muon pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandPtIsoNoPU_0p4           {new TH1F ("h_leadingMuonPfCandPtIsoNoPU_0p4",        "Leading muon pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandIso_0p3                 {new TH1F ("h_leadingMuonPfCandIso_0p3",              "Leading muon pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandIso_0p4                 {new TH1F ("h_leadingMuonPfCandIso_0p4",              "Leading muon pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandPtIso_0p3               {new TH1F ("h_leadingMuonPfCandPtIso_0p3",            "Leading muon pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingMuonPfCandPtIso_0p4               {new TH1F ("h_leadingMuonPfCandPtIso_0p4",            "Leading muon pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
 
    TH1F* h_subleadingMuonTrkIso_0p3                 {new TH1F ("h_subleadingMuonTrkIso_0p3",              "Subleading muon trk iso (0.3 cone); iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonTrkIso_0p4                 {new TH1F ("h_subleadingMuonTrkIso_0p4",              "Subleading muon trk iso (0.4 cone); iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandIsoNoPU_0p3          {new TH1F ("h_subleadingMuonPfCandIsoNoPU_0p3",       "Subleading muon pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandIsoNoPU_0p4          {new TH1F ("h_subleadingMuonPfCandIsoNoPU_0p4",       "Subleading muon pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandPtIsoNoPU_0p3        {new TH1F ("h_subleadingMuonPfCandPtIsoNoPU_0p3",     "Subleading muon pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandPtIsoNoPU_0p4        {new TH1F ("h_subleadingMuonPfCandPtIsoNoPU_0p4",     "Subleading muon pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandIso_0p3              {new TH1F ("h_subleadingMuonPfCandIso_0p3",           "Subleading muon pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandIso_0p4              {new TH1F ("h_subleadingMuonPfCandIso_0p4",           "Subleading muon pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandPtIso_0p3            {new TH1F ("h_subleadingMuonPfCandPtIso_0p3",         "Subleading muon pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingMuonPfCandPtIso_0p4            {new TH1F ("h_subleadingMuonPfCandPtIso_0p4",         "Subleading muon pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};

    TH1F* h_leadingMuonTrkRelIso_0p3                 {new TH1F ("h_leadingMuonTrkRelIso_0p3",              "Leading muon trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonTrkRelIso_0p4                 {new TH1F ("h_leadingMuonTrkRelIso_0p4",              "Leading muon trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandRelIsoNoPU_0p3          {new TH1F ("h_leadingMuonPfCandRelIsoNoPU_0p3",       "Leading muon pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandRelIsoNoPU_0p4          {new TH1F ("h_leadingMuonPfCandRelIsoNoPU_0p4",       "Leading muon pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandPtRelIsoNoPU_0p3        {new TH1F ("h_leadingMuonPfCandPtRelIsoNoPU_0p3",     "Leading muon pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandPtRelIsoNoPU_0p4        {new TH1F ("h_leadingMuonPfCandPtRelIsoNoPU_0p4",     "Leading muon pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandRelIso_0p3              {new TH1F ("h_leadingMuonPfCandRelIso_0p3",           "Leading muon pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandRelIso_0p4              {new TH1F ("h_leadingMuonPfCandRelIso_0p4",           "Leading muon pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandPtRelIso_0p3            {new TH1F ("h_leadingMuonPfCandPtRelIso_0p3",         "Leading muon pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingMuonPfCandPtRelIso_0p4            {new TH1F ("h_leadingMuonPfCandPtRelIso_0p4",         "Leading muon pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};


    TH1F* h_subleadingMuonTrkRelIso_0p3              {new TH1F ("h_subleadingMuonTrkRelIso_0p3",           "Subleading muon trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonTrkRelIso_0p4              {new TH1F ("h_subleadingMuonTrkRelIso_0p4",           "Subleading muon trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandRelIsoNoPU_0p3       {new TH1F ("h_subleadingMuonPfCandRelIsoNoPU_0p3",    "Subleading muon pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandRelIsoNoPU_0p4       {new TH1F ("h_subleadingMuonPfCandRelIsoNoPU_0p4",    "Subleading muon pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandPtRelIsoNoPU_0p3     {new TH1F ("h_subleadingMuonPfCandPtRelIsoNoPU_0p3",  "Subleading muon pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandPtRelIsoNoPU_0p4     {new TH1F ("h_subleadingMuonPfCandPtRelIsoNoPU_0p4",  "Subleading muon pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandRelIso_0p3           {new TH1F ("h_subleadingMuonPfCandRelIso_0p3",        "Subleading muon pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandRelIso_0p4           {new TH1F ("h_subleadingMuonPfCandRelIso_0p4",        "Subleading muon pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandPtRelIso_0p3         {new TH1F ("h_subleadingMuonPfCandPtRelIso_0p3",      "Subleading muon pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingMuonPfCandPtRelIso_0p4         {new TH1F ("h_subleadingMuonPfCandPtRelIso_0p4",      "Subleading muon pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};


    TH1F* h_dimuonTrkIso_0p3                         {new TH1F ("h_dimuonTrkIso_0p3",                      "Dimuon trk iso (0.3 cone); iso;", 500, 0., 5.)};
    TH1F* h_dimuonTrkIso_0p4                         {new TH1F ("h_dimuonTrkIso_0p4",                      "Dimuon trk iso (0.4 cone); iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandIsoNoPU_0p3                  {new TH1F ("h_dimuonPfCandIsoNoPU_0p3",               "Dimuon pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandIsoNoPU_0p4                  {new TH1F ("h_dimuonPfCandIsoNoPU_0p4",               "Dimuon pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtIsoNoPU_0p3                {new TH1F ("h_dimuonPfCandPtIsoNoPU_0p3",             "Dimuon pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtIsoNoPU_0p4                {new TH1F ("h_dimuonPfCandPtIsoNoPU_0p4",             "Dimuon pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandIso_0p3                      {new TH1F ("h_dimuonPfCandIso_0p3",                   "Dimuon pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandIso_0p4                      {new TH1F ("h_dimuonPfCandIso_0p4",                   "Dimuon pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtIso_0p3                    {new TH1F ("h_dimuonPfCandPtIso_0p3",                 "Dimuon pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtIso_0p4                    {new TH1F ("h_dimuonPfCandPtIso_0p4",                 "Dimuon pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 5.)};

    TH1F* h_dimuonTrkRelIso_0p3                      {new TH1F ("h_dimuonTrkRelIso_0p3",                   "Dimuon trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonTrkRelIso_0p4                      {new TH1F ("h_dimuonTrkRelIso_0p4",                   "Dimuon trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandRelIsoNoPU_0p3               {new TH1F ("h_dimuonPfCandRelIsoNoPU_0p3",            "Dimuon pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandRelIsoNoPU_0p4               {new TH1F ("h_dimuonPfCandRelIsoNoPU_0p4",            "Dimuon pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtRelIsoNoPU_0p3             {new TH1F ("h_dimuonPfCandPtRelIsoNoPU_0p3",          "Dimuon pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtRelIsoNoPU_0p4             {new TH1F ("h_dimuonPfCandPtRelIsoNoPU_0p4",          "Dimuon pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandRelIso_0p3                   {new TH1F ("h_dimuonPfCandRelIso_0p3",                "Dimuon pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandRelIso_0p4                   {new TH1F ("h_dimuonPfCandRelIso_0p4",                "Dimuon pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtRelIso_0p3                 {new TH1F ("h_dimuonPfCandPtRelIso_0p3",              "Dimuon pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dimuonPfCandPtRelIso_0p4                 {new TH1F ("h_dimuonPfCandPtRelIso_0p4",              "Dimuon pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};

    // hadrons

    TH1F* h_leadingChsTrkIso_0p3                     {new TH1F ("h_leadingChsTrkIso_0p3",                 "Leading ch trk iso (0.3 cone); iso;", 500, 0., 15.)};
    TH1F* h_leadingChsTrkIso_0p4                     {new TH1F ("h_leadingChsTrkIso_0p4",                 "Leading ch trk iso (0.4 cone); iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandIsoNoPU_0p3              {new TH1F ("h_leadingChsPfCandIsoNoPU_0p3",          "Leading ch pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandIsoNoPU_0p4              {new TH1F ("h_leadingChsPfCandIsoNoPU_0p4",          "Leading ch pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandPtIsoNoPU_0p3            {new TH1F ("h_leadingChsPfCandPtIsoNoPU_0p3",        "Leading ch pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandPtIsoNoPU_0p4            {new TH1F ("h_leadingChsPfCandPtIsoNoPU_0p4",        "Leading ch pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandIso_0p3                  {new TH1F ("h_leadingChsPfCandIso_0p3",              "Leading ch pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandIso_0p4                  {new TH1F ("h_leadingChsPfCandIso_0p4",              "Leading ch pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandPtIso_0p3                {new TH1F ("h_leadingChsPfCandPtIso_0p3",            "Leading ch pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_leadingChsPfCandPtIso_0p4                {new TH1F ("h_leadingChsPfCandPtIso_0p4",            "Leading ch pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
 
    TH1F* h_subleadingChsTrkIso_0p3                 {new TH1F ("h_subleadingChsTrkIso_0p3",               "Subleading ch trk iso (0.3 cone); iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsTrkIso_0p4                 {new TH1F ("h_subleadingChsTrkIso_0p4",               "Subleading ch trk iso (0.4 cone); iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandIsoNoPU_0p3          {new TH1F ("h_subleadingChsPfCandIsoNoPU_0p3",        "Subleading ch pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandIsoNoPU_0p4          {new TH1F ("h_subleadingChsPfCandIsoNoPU_0p4",        "Subleading ch pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandPtIsoNoPU_0p3        {new TH1F ("h_subleadingChsPfCandPtIsoNoPU_0p3",      "Subleading ch pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandPtIsoNoPU_0p4        {new TH1F ("h_subleadingChsPfCandPtIsoNoPU_0p4",      "Subleading ch pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandIso_0p3              {new TH1F ("h_subleadingChsPfCandIso_0p3",            "Subleading ch pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandIso_0p4              {new TH1F ("h_subleadingChsPfCandIso_0p4",            "Subleading ch pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandPtIso_0p3            {new TH1F ("h_subleadingChsPfCandPtIso_0p3",          "Subleading ch pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 15.)};
    TH1F* h_subleadingChsPfCandPtIso_0p4            {new TH1F ("h_subleadingChsPfCandPtIso_0p4",          "Subleading ch pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 15.)};

    TH1F* h_leadingChsTrkRelIso_0p3                 {new TH1F ("h_leadingChsTrkRelIso_0p3",               "Leading ch trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsTrkRelIso_0p4                 {new TH1F ("h_leadingChsTrkRelIso_0p4",               "Leading ch trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandRelIsoNoPU_0p3          {new TH1F ("h_leadingChsPfCandRelIsoNoPU_0p3",        "Leading ch pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandRelIsoNoPU_0p4          {new TH1F ("h_leadingChsPfCandRelIsoNoPU_0p4",        "Leading ch pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandPtRelIsoNoPU_0p3        {new TH1F ("h_leadingChsPfCandPtRelIsoNoPU_0p3",      "Leading ch pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandPtRelIsoNoPU_0p4        {new TH1F ("h_leadingChsPfCandPtRelIsoNoPU_0p4",      "Leading ch pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandRelIso_0p3              {new TH1F ("h_leadingChsPfCandRelIso_0p3",            "Leading ch pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandRelIso_0p4              {new TH1F ("h_leadingChsPfCandRelIso_0p4",            "Leading ch pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandPtRelIso_0p3            {new TH1F ("h_leadingChsPfCandPtRelIso_0p3",          "Leading ch pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_leadingChsPfCandPtRelIso_0p4            {new TH1F ("h_leadingChsPfCandPtRelIso_0p4",          "Leading ch pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};


    TH1F* h_subleadingChsTrkRelIso_0p3              {new TH1F ("h_subleadingChsTrkRelIso_0p3",            "Subleading ch trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsTrkRelIso_0p4              {new TH1F ("h_subleadingChsTrkRelIso_0p4",            "Subleading ch trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandRelIsoNoPU_0p3       {new TH1F ("h_subleadingChsPfCandRelIsoNoPU_0p3",     "Subleading ch pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandRelIsoNoPU_0p4       {new TH1F ("h_subleadingChsPfCandRelIsoNoPU_0p4",     "Subleading ch pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandPtRelIsoNoPU_0p3     {new TH1F ("h_subleadingChsPfCandPtRelIsoNoPU_0p3",   "Subleading ch pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandPtRelIsoNoPU_0p4     {new TH1F ("h_subleadingChsPfCandPtRelIsoNoPU_0p4",   "Subleading ch pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandRelIso_0p3           {new TH1F ("h_subleadingChsPfCandRelIso_0p3",         "Subleading ch pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandRelIso_0p4           {new TH1F ("h_subleadingChsPfCandRelIso_0p4",         "Subleading ch pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandPtRelIso_0p3         {new TH1F ("h_subleadingChsPfCandPtRelIso_0p3",       "Subleading ch pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_subleadingChsPfCandPtRelIso_0p4         {new TH1F ("h_subleadingChsPfCandPtRelIso_0p4",       "Subleading ch pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};


    TH1F* h_dichsTrkIso_0p3                         {new TH1F ("h_dichsTrkIso_0p3",                       "Dihadron trk iso (0.3 cone); iso;", 500, 0., 5.)};
    TH1F* h_dichsTrkIso_0p4                         {new TH1F ("h_dichsTrkIso_0p4",                       "Dihadron trk iso (0.4 cone); iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandIsoNoPU_0p3                  {new TH1F ("h_dichsPfCandIsoNoPU_0p3",                "Dihadron pf cand iso (0.3 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandIsoNoPU_0p4                  {new TH1F ("h_dichsPfCandIsoNoPU_0p4",                "Dihadron pf cand iso (0.4 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtIsoNoPU_0p3                {new TH1F ("h_dichsPfCandPtIsoNoPU_0p3",              "Dihadron pf cand iso (p_{T} sum) (0.3 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtIsoNoPU_0p4                {new TH1F ("h_dichsPfCandPtIsoNoPU_0p4",              "Dihadron pf cand iso (p_{T} sum) (0.4 cone) without PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandIso_0p3                      {new TH1F ("h_dichsPfCandIso_0p3",                    "Dihadron pf cand iso (0.3 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandIso_0p4                      {new TH1F ("h_dichsPfCandIso_0p4",                    "Dihadron pf cand iso (0.4 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtIso_0p3                    {new TH1F ("h_dichsPfCandPtIso_0p3",                  "Dihadron pf cand iso (p_{T} sum) (0.3 cone) with PU corrections; iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtIso_0p4                    {new TH1F ("h_dichsPfCandPtIso_0p4",                  "Dihadron pf cand iso (p_{T} sum) (0.4 cone) with PU corrections; iso;", 500, 0., 5.)};

    TH1F* h_dichsTrkRelIso_0p3                      {new TH1F ("h_dichsTrkRelIso_0p3",                    "Dihadron trk rel iso (0.3 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_dichsTrkRelIso_0p4                      {new TH1F ("h_dichsTrkRelIso_0p4",                    "Dihadron trk rel iso (0.4 cone); rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandRelIsoNoPU_0p3               {new TH1F ("h_dichsPfCandRelIsoNoPU_0p3",             "Dihadron pf cand rel iso (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandRelIsoNoPU_0p4               {new TH1F ("h_dichsPfCandRelIsoNoPU_0p4",             "Dihadron pf cand rel iso (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtRelIsoNoPU_0p3             {new TH1F ("h_dichsPfCandPtRelIsoNoPU_0p3",           "Dihadron pf cand rel iso (p_{T} sum) (0.3 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtRelIsoNoPU_0p4             {new TH1F ("h_dichsPfCandPtRelIsoNoPU_0p4",           "Dihadron pf cand rel iso (p_{T} sum) (0.4 cone) without PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandRelIso_0p3                   {new TH1F ("h_dichsPfCandRelIso_0p3",                 "Dihadron pf cand rel iso (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandRelIso_0p4                   {new TH1F ("h_dichsPfCandRelIso_0p4",                 "Dihadron pf cand rel iso (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtRelIso_0p3                 {new TH1F ("h_dichsPfCandPtRelIso_0p3",               "Dihadron pf cand rel iso (p_{T} sum) (0.3 cone) with PU corrections; rel iso;", 500, 0., 5.)};
    TH1F* h_dichsPfCandPtRelIso_0p4                 {new TH1F ("h_dichsPfCandPtRelIso_0p4",               "Dihadron pf cand rel iso (p_{T} sum) (0.4 cone) with PU corrections; rel iso;", 500, 0., 5.)};


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

	const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
	const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

//        if (! ( passDimuonTrigger || passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger ) ) continue;
	if ( !passSingleMuonTrigger ) continue;
	if (!event.metFilters()) continue;
 
	event.muonIndexLoose = getLooseMuons(event);

	if ( event.muonIndexLoose.size() < 2 ) continue;

	if ( !getDileptonCand( event, event.muonIndexLoose ) ) continue;

	// calculate isolation values for selected muons
	// trk iso
	double mu_trkiso_0p3 {0.}, mu_trkiso1_0p3 {0.}, mu_trkiso2_0p3 {0.};
	double mu_trkiso_0p4 {0.}, mu_trkiso1_0p4 {0.}, mu_trkiso2_0p4 {0.};
	// pf cand iso
	//// separate components
	double mu_ch_iso_0p3 {0.}, mu_ch_iso1_0p33 {0.}, mu_ch_iso2_0p33 {0.};
	double mu_ch_iso_0p4 {0.}, mu_ch_iso1_0p34 {0.}, mu_ch_iso2_0p34 {0.};
        double mu_pu_iso_0p3 {0.}, mu_pu_iso1_0p3 {0.}, mu_pu_iso2_0p3 {0.};
        double mu_pu_iso_0p4 {0.}, mu_pu_iso1_0p4 {0.}, mu_pu_iso2_0p4 {0.};
	//// ET
	double mu_nh_iso_0p3 {0.}, mu_nh_iso1_0p3 {0.}, mu_nh_iso2_0p3 {0.};
	double mu_nh_iso_0p4 {0.}, mu_nh_iso1_0p4 {0.}, mu_nh_iso2_0p4 {0.};
	double mu_gamma_iso_0p3 {0.}, mu_gamma_iso1_0p3 {0.}, mu_gamma_iso2_0p3 {0.};
	double mu_gamma_iso_0p4 {0.}, mu_gamma_iso1_0p4 {0.}, mu_gamma_iso2_0p4 {0.};
	//// PT
	double mu_nh_iso_pT_0p3 {0.}, mu_nh_iso1_pT_0p3 {0.}, mu_nh_iso2_pT_0p3 {0.};
	double mu_nh_iso_pT_0p4 {0.}, mu_nh_iso1_pT_0p4 {0.}, mu_nh_iso2_pT_0p4 {0.};
	double mu_gamma_iso_pT_0p3 {0.}, mu_gamma_iso1_pT_0p3 {0.}, mu_gamma_iso2_pT_0p3 {0.};
	double mu_gamma_iso_pT_0p4 {0.}, mu_gamma_iso1_pT_0p4 {0.}, mu_gamma_iso2_pT_0p4 {0.};

	for (int k = 0; k < event.numPackedCands; k++) {
	  // Skip packed cand if it is related to either selected muon
	  if ( k == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] || k == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
	  // Skip low momentum PF cands
	  TLorentzVector packedCandVec;
	  packedCandVec.SetPxPyPzE(event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]);                
	  if ( packedCandVec.Pt() < 0.5 ) continue;
	  
	  // Track iso
	  if ( event.packedCandsHasTrackDetails[k] > 0 ) {
	    TLorentzVector packedTrkVec;
	    packedTrkVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);
	    if ( event.zPairLeptons.first.DeltaR(packedTrkVec)   < 0.3 )                             mu_trkiso1_0p3 += packedTrkVec.Pt();
	    if ( event.zPairLeptons.second.DeltaR(packedTrkVec)  < 0.3 )                             mu_trkiso2_0p3 += packedTrkVec.Pt();
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedTrkVec)  < 0.3 )  mu_trkiso_0p3  += packedTrkVec.Pt();
	    if ( event.zPairLeptons.first.DeltaR(packedTrkVec)   < 0.4 )                             mu_trkiso1_0p4 += packedTrkVec.Pt();
	    if ( event.zPairLeptons.second.DeltaR(packedTrkVec)  < 0.4 )                             mu_trkiso2_0p4 += packedTrkVec.Pt();
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedTrkVec)  < 0.4 )  mu_trkiso_0p4  += packedTrkVec.Pt();
	  }
	  // PF cands a la PF iso
	  int candId (std::abs(event.packedCandsPdgId[k]));
	  if ( candId == 211 ) { // If charged hadron
            if ( event.packedCandsFromPV[k] >= 2 ) { // If from PV
              if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 )                            mu_ch_iso1_0p33 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 )                            mu_ch_iso2_0p33 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) mu_ch_iso_0p3  += packedCandVec.Pt();
	      if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 )                            mu_ch_iso1_0p34 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 )                            mu_ch_iso2_0p34 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) mu_ch_iso_0p4  += packedCandVec.Pt();
	    }
            else {
              if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 )                            mu_pu_iso1_0p3 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 )                            mu_pu_iso2_0p3 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) mu_pu_iso_0p3  += packedCandVec.Pt();
	      if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 )                            mu_pu_iso1_0p4 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 )                            mu_pu_iso2_0p4 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) mu_pu_iso_0p4  += packedCandVec.Pt();
            }
	  }
	  
	  else if ( candId == 130 ) { // If neutral hadron
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 ){
	      mu_nh_iso1_0p3    += packedCandVec.Et();
	      mu_nh_iso1_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 ) {
	      mu_nh_iso2_0p3    += packedCandVec.Et();
	      mu_nh_iso2_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) {
	      mu_nh_iso_0p3     += packedCandVec.Et();
	      mu_nh_iso_pT_0p3  += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 ) {
	      mu_nh_iso1_0p4    += packedCandVec.Et();
	      mu_nh_iso1_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 ) {
	      mu_nh_iso2_0p4    += packedCandVec.Et();
	      mu_nh_iso2_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) {
	      mu_nh_iso_0p4     += packedCandVec.Et();
	      mu_nh_iso_pT_0p4  += packedCandVec.Pt();
	    }
	  }
	  else if ( candId == 22 ) { // If gamma
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 ) {
	      mu_gamma_iso1_0p3 += packedCandVec.Et();
	      mu_gamma_iso1_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 ) {
	      mu_gamma_iso2_0p3 += packedCandVec.Et();
	      mu_gamma_iso2_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) {
	      mu_gamma_iso_0p3  += packedCandVec.Et();
	      mu_gamma_iso_pT_0p3  += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 ) {
	      mu_gamma_iso1_0p4 += packedCandVec.Et();
	      mu_gamma_iso1_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 ) {
	      mu_gamma_iso2_0p4 += packedCandVec.Et();
	      mu_gamma_iso2_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) {
	      mu_gamma_iso_0p4  += packedCandVec.Et();
	      mu_gamma_iso_pT_0p4  += packedCandVec.Pt();
	    }
	  }	  
	}

        /////// 
/*
	// calculate isolation values for selected charged hadrons
	// trk iso
	double mu_trkiso_0p3 {0.}, mu_trkiso1_0p3 {0.}, mu_trkiso2_0p3 {0.};
	double mu_trkiso_0p4 {0.}, mu_trkiso1_0p4 {0.}, mu_trkiso2_0p4 {0.};
	// pf cand iso
	//// separate components
	double mu_ch_iso_0p3 {0.}, mu_ch_iso1_0p33 {0.}, mu_ch_iso2_0p33 {0.};
	double mu_ch_iso_0p4 {0.}, mu_ch_iso1_0p34 {0.}, mu_ch_iso2_0p34 {0.};
        double mu_pu_iso_0p3 {0.}, mu_pu_iso1_0p3 {0.}, mu_pu_iso2_0p3 {0.};
        double mu_pu_iso_0p4 {0.}, mu_pu_iso1_0p4 {0.}, mu_pu_iso2_0p4 {0.};
	//// ET
	double mu_nh_iso_0p3 {0.}, mu_nh_iso1_0p3 {0.}, mu_nh_iso2_0p3 {0.};
	double mu_nh_iso_0p4 {0.}, mu_nh_iso1_0p4 {0.}, mu_nh_iso2_0p4 {0.};
	double mu_gamma_iso_0p3 {0.}, mu_gamma_iso1_0p3 {0.}, mu_gamma_iso2_0p3 {0.};
	double mu_gamma_iso_0p4 {0.}, mu_gamma_iso1_0p4 {0.}, mu_gamma_iso2_0p4 {0.};
	//// PT
	double mu_nh_iso_pT_0p3 {0.}, mu_nh_iso1_pT_0p3 {0.}, mu_nh_iso2_pT_0p3 {0.};
	double mu_nh_iso_pT_0p4 {0.}, mu_nh_iso1_pT_0p4 {0.}, mu_nh_iso2_pT_0p4 {0.};
	double mu_gamma_iso_pT_0p3 {0.}, mu_gamma_iso1_pT_0p3 {0.}, mu_gamma_iso2_pT_0p3 {0.};
	double mu_gamma_iso_pT_0p4 {0.}, mu_gamma_iso1_pT_0p4 {0.}, mu_gamma_iso2_pT_0p4 {0.};

	for (int k = 0; k < event.numPackedCands; k++) {
	  // Skip packed cand if it is related to either selected muon
	  if ( k == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] || k == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
	  // Skip low momentum PF cands
	  TLorentzVector packedCandVec;
	  packedCandVec.SetPxPyPzE(event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]);                
	  if ( packedCandVec.Pt() < 0.5 ) continue;
	  
	  // Track iso
	  if ( event.packedCandsHasTrackDetails[k] > 0 ) {
	    TLorentzVector packedTrkVec;
	    packedTrkVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);
	    if ( event.zPairLeptons.first.DeltaR(packedTrkVec)   < 0.3 )                             mu_trkiso1_0p3 += packedTrkVec.Pt();
	    if ( event.zPairLeptons.second.DeltaR(packedTrkVec)  < 0.3 )                             mu_trkiso2_0p3 += packedTrkVec.Pt();
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedTrkVec)  < 0.3 )  mu_trkiso_0p3  += packedTrkVec.Pt();
	    if ( event.zPairLeptons.first.DeltaR(packedTrkVec)   < 0.4 )                             mu_trkiso1_0p4 += packedTrkVec.Pt();
	    if ( event.zPairLeptons.second.DeltaR(packedTrkVec)  < 0.4 )                             mu_trkiso2_0p4 += packedTrkVec.Pt();
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedTrkVec)  < 0.4 )  mu_trkiso_0p4  += packedTrkVec.Pt();
	  }
	  // PF cands a la PF iso
	  int candId (std::abs(event.packedCandsPdgId[k]));
	  if ( candId == 211 ) { // If charged hadron
            if ( event.packedCandsFromPV[k] >= 2 ) { // If from PV
              if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 )                            mu_ch_iso1_0p33 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 )                            mu_ch_iso2_0p33 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) mu_ch_iso_0p3  += packedCandVec.Pt();
	      if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 )                            mu_ch_iso1_0p34 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 )                            mu_ch_iso2_0p34 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) mu_ch_iso_0p4  += packedCandVec.Pt();
	    }
            else {
              if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 )                            mu_pu_iso1_0p3 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 )                            mu_pu_iso2_0p3 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) mu_pu_iso_0p3  += packedCandVec.Pt();
	      if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 )                            mu_pu_iso1_0p4 += packedCandVec.Pt();
	      if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 )                            mu_pu_iso2_0p4 += packedCandVec.Pt();
	      if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) mu_pu_iso_0p4  += packedCandVec.Pt();
            }
	  }
	  
	  else if ( candId == 130 ) { // If neutral hadron
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 ){
	      mu_nh_iso1_0p3    += packedCandVec.Et();
	      mu_nh_iso1_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 ) {
	      mu_nh_iso2_0p3    += packedCandVec.Et();
	      mu_nh_iso2_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) {
	      mu_nh_iso_0p3     += packedCandVec.Et();
	      mu_nh_iso_pT_0p3  += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 ) {
	      mu_nh_iso1_0p4    += packedCandVec.Et();
	      mu_nh_iso1_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 ) {
	      mu_nh_iso2_0p4    += packedCandVec.Et();
	      mu_nh_iso2_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) {
	      mu_nh_iso_0p4     += packedCandVec.Et();
	      mu_nh_iso_pT_0p4  += packedCandVec.Pt();
	    }
	  }
	  else if ( candId == 22 ) { // If gamma
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.3 ) {
	      mu_gamma_iso1_0p3 += packedCandVec.Et();
	      mu_gamma_iso1_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.3 ) {
	      mu_gamma_iso2_0p3 += packedCandVec.Et();
	      mu_gamma_iso2_pT_0p3 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.3 ) {
	      mu_gamma_iso_0p3  += packedCandVec.Et();
	      mu_gamma_iso_pT_0p3  += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.first.DeltaR(packedCandVec)  < 0.4 ) {
	      mu_gamma_iso1_0p4 += packedCandVec.Et();
	      mu_gamma_iso1_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( event.zPairLeptons.second.DeltaR(packedCandVec) < 0.4 ) {
	      mu_gamma_iso2_0p4 += packedCandVec.Et();
	      mu_gamma_iso2_pT_0p4 += packedCandVec.Pt();
	    }
	    if ( (event.zPairLeptons.first+event.zPairLeptons.second).DeltaR(packedCandVec) < 0.4 ) {
	      mu_gamma_iso_0p4  += packedCandVec.Et();
	      mu_gamma_iso_pT_0p4  += packedCandVec.Pt();
	    }
	  }	  
	}	

*/
        //// Calculate Iso's
        // Calculate Muon iso
	// rel iso for tracks
	const float trkreliso1_0p3 { mu_trkiso1_0p3 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, trkreliso2_0p3 { mu_trkiso2_0p3 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, trkreliso_0p3 { mu_trkiso_0p3 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };
	const float trkreliso1_0p4 { mu_trkiso1_0p4 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, trkreliso2_0p4 { mu_trkiso2_0p4 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, trkreliso_0p4 { mu_trkiso_0p4 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };

	// iso for PF (pT + ET sum) - NO PU!
	const float pf_iso1_noPU_0p3 { mu_ch_iso1_0p33 + std::max( 0.0, mu_nh_iso1_0p3 + mu_gamma_iso1_0p3 )}, pf_iso2_noPU_0p3 {mu_ch_iso2_0p33 + std::max( 0.0, mu_nh_iso2_0p3 + mu_gamma_iso2_0p3 )}, pf_iso_noPU_0p3 {mu_ch_iso_0p3 + std::max( 0.0, mu_nh_iso_0p3 + mu_gamma_iso_0p3 )};
	const float pf_iso1_noPU_0p4 { mu_ch_iso1_0p34 + std::max( 0.0, mu_nh_iso1_0p4 + mu_gamma_iso1_0p4 )}, pf_iso2_noPU_0p4 {mu_ch_iso2_0p34 + std::max( 0.0, mu_nh_iso2_0p4 + mu_gamma_iso2_0p4 )}, pf_iso_noPU_0p4 {mu_ch_iso_0p4 + std::max( 0.0, mu_nh_iso_0p4 + mu_gamma_iso_0p4 )};
	// rel iso for PF (pT + ET sum) - NO PU!
	const float pf_reliso1_noPU_0p3 { pf_iso1_noPU_0p3 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_noPU_0p3 { pf_iso2_noPU_0p3 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_noPU_0p3 { pf_iso_noPU_0p3 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };
	const float pf_reliso1_noPU_0p4 { pf_iso1_noPU_0p4 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_noPU_0p4 { pf_iso2_noPU_0p4 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_noPU_0p4 { pf_iso_noPU_0p4 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };

	// iso for PF (pT + ET sum)
	const float pf_iso1_0p3 { mu_ch_iso1_0p33 + std::max( 0.0, mu_nh_iso1_0p3 + mu_gamma_iso1_0p3 - 0.5*mu_pu_iso1_0p3 )}, pf_iso2_0p3 {mu_ch_iso2_0p33 + std::max( 0.0, mu_nh_iso2_0p3 + mu_gamma_iso2_0p3 - 0.5*mu_pu_iso2_0p3 )}, pf_iso_0p3 {mu_ch_iso_0p3 + std::max( 0.0, mu_nh_iso_0p3 + mu_gamma_iso_0p3 - 0.5*mu_pu_iso_0p3 )};
	const float pf_iso1_0p4 { mu_ch_iso1_0p34 + std::max( 0.0, mu_nh_iso1_0p4 + mu_gamma_iso1_0p4 - 0.5*mu_pu_iso1_0p4 )}, pf_iso2_0p4 {mu_ch_iso2_0p34 + std::max( 0.0, mu_nh_iso2_0p4 + mu_gamma_iso2_0p4 - 0.5*mu_pu_iso2_0p4 )}, pf_iso_0p4 {mu_ch_iso_0p4 + std::max( 0.0, mu_nh_iso_0p4 + mu_gamma_iso_0p4 - 0.5*mu_pu_iso_0p4 )};
	// rel iso for PF (pT + ET sum)
	const float pf_reliso1_0p3 { pf_iso1_0p3 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_0p3 { pf_iso2_0p3 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_0p3 { pf_iso_0p3 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };
	const float pf_reliso1_0p4 { pf_iso1_0p4 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_0p4 { pf_iso2_0p4 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_0p4 { pf_iso_0p4 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };

	// iso for PF (pT only sum) - NO PU!
	const float pf_iso1_pT_noPU_0p3 { mu_ch_iso1_0p33 + mu_nh_iso1_pT_0p3 + mu_gamma_iso1_pT_0p3 }, pf_iso2_pT_noPU_0p3 { mu_ch_iso2_0p33 + mu_nh_iso2_pT_0p4 + mu_gamma_iso2_pT_0p3 }, pf_iso_pT_noPU_0p3 { mu_ch_iso_0p3 + mu_nh_iso_pT_0p3 + mu_gamma_iso_pT_0p3 };
	const float pf_iso1_pT_noPU_0p4 { mu_ch_iso1_0p34 + mu_nh_iso1_pT_0p4 + mu_gamma_iso1_pT_0p4 }, pf_iso2_pT_noPU_0p4 { mu_ch_iso2_0p34 + mu_nh_iso2_pT_0p4 + mu_gamma_iso2_pT_0p4 }, pf_iso_pT_noPU_0p4 { mu_ch_iso_0p4 + mu_nh_iso_pT_0p4 + mu_gamma_iso_pT_0p4 };
	// rel iso for PF (pT only sum) - NO PU!
	const float pf_reliso1_pT_noPU_0p3 { pf_iso1_pT_noPU_0p3 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_pT_noPU_0p3 { pf_iso2_pT_noPU_0p3 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_pT_noPU_0p3 { pf_iso_pT_noPU_0p3 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };
	const float pf_reliso1_pT_noPU_0p4 { pf_iso1_pT_noPU_0p4 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_pT_noPU_0p4 { pf_iso2_pT_noPU_0p4 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_pT_noPU_0p4 { pf_iso_pT_noPU_0p4 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };

	// iso for PF (pT only sum)
	const float pf_iso1_pT_0p3 { mu_ch_iso1_0p33 + mu_nh_iso1_pT_0p3 + mu_gamma_iso1_pT_0p3 - 0.5*mu_pu_iso1_0p3 }, pf_iso2_pT_0p3 { mu_ch_iso2_0p33 + mu_nh_iso2_pT_0p4 + mu_gamma_iso2_pT_0p3  - 0.5*mu_pu_iso2_0p3 }, pf_iso_pT_0p3 { mu_ch_iso_0p3 + mu_nh_iso_pT_0p3 + mu_gamma_iso_pT_0p3 - 0.5*mu_pu_iso_0p3};
	const float pf_iso1_pT_0p4 { mu_ch_iso1_0p34 + mu_nh_iso1_pT_0p4 + mu_gamma_iso1_pT_0p4 - 0.5*mu_pu_iso1_0p4 }, pf_iso2_pT_0p4 { mu_ch_iso2_0p34 + mu_nh_iso2_pT_0p4 + mu_gamma_iso2_pT_0p4  - 0.5*mu_pu_iso2_0p4 }, pf_iso_pT_0p4 { mu_ch_iso_0p4 + mu_nh_iso_pT_0p4 + mu_gamma_iso_pT_0p4 - 0.5*mu_pu_iso_0p4 };
	// rel iso for PF (pT only sum)
	const float pf_reliso1_pT_0p3 { pf_iso1_pT_0p3 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_pT_0p3 { pf_iso2_pT_0p3 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_pT_0p3 { pf_iso_pT_0p3 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };
	const float pf_reliso1_pT_0p4 { pf_iso1_pT_0p4 / ( event.zPairLeptons.first.Pt()+1.0e-06) }, pf_reliso2_pT_0p4 { pf_iso2_pT_0p4 / ( event.zPairLeptons.second.Pt()+1.0e-06) }, pf_reliso_pT_0p4 { pf_iso_pT_0p4 / ((event.zPairLeptons.first+event.zPairLeptons.second).Pt()+1.0e-06) };


	// Fill muon histograms

            h_leadingMuonPFIso->Fill(event.zPairRelIso.first*event.zPairLeptons.first.Pt(), eventWeight);
            h_subleadingMuonPFIso->Fill(event.zPairRelIso.second*event.zPairLeptons.second.Pt(), eventWeight);
            h_leadingMuonPFRelIso->Fill(event.zPairRelIso.first, eventWeight);
            h_subleadingMuonPFRelIso->Fill(event.zPairRelIso.second, eventWeight);

            h_leadingMuonTrkIso_0p3->Fill(mu_trkiso1_0p3, eventWeight);
            h_leadingMuonTrkIso_0p4->Fill(mu_trkiso1_0p4, eventWeight);
            h_leadingMuonPfCandIsoNoPU_0p3->Fill(pf_iso1_noPU_0p3, eventWeight);
            h_leadingMuonPfCandIsoNoPU_0p4->Fill(pf_iso1_noPU_0p4, eventWeight);
            h_leadingMuonPfCandPtIsoNoPU_0p3->Fill(pf_iso1_pT_noPU_0p3, eventWeight);
            h_leadingMuonPfCandPtIsoNoPU_0p4->Fill(pf_iso1_pT_noPU_0p4, eventWeight);
            h_leadingMuonPfCandIso_0p3->Fill(pf_iso1_0p3, eventWeight);
            h_leadingMuonPfCandIso_0p4->Fill(pf_iso1_0p4, eventWeight);
            h_leadingMuonPfCandPtIso_0p3->Fill(pf_iso1_pT_0p3, eventWeight);
            h_leadingMuonPfCandPtIso_0p4->Fill(pf_iso1_pT_0p4, eventWeight);

            h_subleadingMuonTrkIso_0p3->Fill(mu_trkiso2_0p3, eventWeight);
            h_subleadingMuonTrkIso_0p4->Fill(mu_trkiso2_0p4, eventWeight);

            h_subleadingMuonPfCandIsoNoPU_0p3->Fill(pf_reliso2_noPU_0p3, eventWeight);
            h_subleadingMuonPfCandIsoNoPU_0p4->Fill(pf_reliso2_noPU_0p4, eventWeight);
            h_subleadingMuonPfCandPtIsoNoPU_0p3->Fill(pf_reliso2_pT_noPU_0p3, eventWeight);
            h_subleadingMuonPfCandPtIsoNoPU_0p4->Fill(pf_reliso2_pT_noPU_0p4, eventWeight);
            h_subleadingMuonPfCandIso_0p3->Fill(pf_reliso2_0p3, eventWeight);
            h_subleadingMuonPfCandIso_0p4->Fill(pf_reliso2_0p4, eventWeight);
            h_subleadingMuonPfCandPtIso_0p3->Fill(pf_reliso2_pT_0p3, eventWeight);
            h_subleadingMuonPfCandPtIso_0p4->Fill(pf_reliso2_pT_0p4, eventWeight);

            h_leadingMuonTrkRelIso_0p3->Fill(trkreliso1_0p3, eventWeight);
            h_leadingMuonTrkRelIso_0p4->Fill(trkreliso1_0p4, eventWeight);

            h_leadingMuonPfCandRelIsoNoPU_0p3->Fill(pf_reliso1_noPU_0p3, eventWeight);
            h_leadingMuonPfCandRelIsoNoPU_0p4->Fill(pf_reliso1_noPU_0p4, eventWeight);
            h_leadingMuonPfCandPtRelIsoNoPU_0p3->Fill(pf_reliso1_pT_noPU_0p3, eventWeight);
            h_leadingMuonPfCandPtRelIsoNoPU_0p4->Fill(pf_reliso1_pT_noPU_0p4, eventWeight);
            h_leadingMuonPfCandRelIso_0p3->Fill(pf_reliso1_0p3, eventWeight);
            h_leadingMuonPfCandRelIso_0p4->Fill(pf_reliso1_0p4, eventWeight);
            h_leadingMuonPfCandPtRelIso_0p3->Fill(pf_reliso1_pT_0p3, eventWeight);
            h_leadingMuonPfCandPtRelIso_0p4->Fill(pf_reliso1_pT_0p4, eventWeight);

            h_subleadingMuonTrkRelIso_0p3->Fill(trkreliso2_0p3, eventWeight);
            h_subleadingMuonTrkRelIso_0p4->Fill(trkreliso2_0p4, eventWeight);

            h_subleadingMuonPfCandRelIsoNoPU_0p3->Fill(pf_reliso2_noPU_0p3, eventWeight);
            h_subleadingMuonPfCandRelIsoNoPU_0p4->Fill(pf_reliso2_noPU_0p4, eventWeight);
            h_subleadingMuonPfCandPtRelIsoNoPU_0p3->Fill(pf_reliso2_pT_noPU_0p3, eventWeight);
            h_subleadingMuonPfCandPtRelIsoNoPU_0p4->Fill(pf_reliso2_pT_noPU_0p4, eventWeight);
            h_subleadingMuonPfCandRelIso_0p3->Fill(pf_reliso2_0p3, eventWeight);
            h_subleadingMuonPfCandRelIso_0p4->Fill(pf_reliso2_0p4, eventWeight);
            h_subleadingMuonPfCandPtRelIso_0p3->Fill(pf_reliso2_pT_0p3, eventWeight);
            h_subleadingMuonPfCandPtRelIso_0p4->Fill(pf_reliso2_pT_0p4, eventWeight);

            h_dimuonTrkIso_0p3->Fill(mu_trkiso_0p3, eventWeight);
            h_dimuonTrkIso_0p4->Fill(mu_trkiso_0p4, eventWeight);
 
            h_dimuonPfCandIsoNoPU_0p3->Fill(pf_iso_noPU_0p3, eventWeight);
            h_dimuonPfCandIsoNoPU_0p4->Fill(pf_iso_noPU_0p4, eventWeight);
            h_dimuonPfCandPtIsoNoPU_0p3->Fill(pf_iso_pT_noPU_0p3, eventWeight);
            h_dimuonPfCandPtIsoNoPU_0p4->Fill(pf_iso_pT_noPU_0p4, eventWeight);
            h_dimuonPfCandIso_0p3->Fill(pf_iso_0p3, eventWeight);
            h_dimuonPfCandIso_0p4->Fill(pf_iso_0p4, eventWeight);
            h_dimuonPfCandPtIso_0p3->Fill(pf_iso_pT_0p3, eventWeight);
            h_dimuonPfCandPtIso_0p4->Fill(pf_iso_pT_0p4, eventWeight);

            h_dimuonTrkRelIso_0p3->Fill(trkreliso_0p3, eventWeight);
            h_dimuonTrkRelIso_0p4->Fill(trkreliso_0p4, eventWeight);

            h_dimuonPfCandRelIsoNoPU_0p3->Fill(pf_reliso_noPU_0p3, eventWeight);
            h_dimuonPfCandRelIsoNoPU_0p4->Fill(pf_reliso_noPU_0p4, eventWeight);
            h_dimuonPfCandPtRelIsoNoPU_0p3->Fill(pf_reliso_pT_noPU_0p3, eventWeight);
            h_dimuonPfCandPtRelIsoNoPU_0p4->Fill(pf_reliso_pT_noPU_0p4, eventWeight);
            h_dimuonPfCandRelIso_0p3->Fill(pf_reliso_0p3, eventWeight);
            h_dimuonPfCandRelIso_0p4->Fill(pf_reliso_0p4, eventWeight);
            h_dimuonPfCandPtRelIso_0p3->Fill(pf_reliso_pT_0p3, eventWeight);
            h_dimuonPfCandPtRelIso_0p4->Fill(pf_reliso_pT_0p4, eventWeight);


      } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

// Write Histos

// Write muon histos

//    h_leadingMuonPFIso->Write();
//    h_subleadingMuonPFIso->Write();
    h_leadingMuonPFRelIso->Write();
    h_subleadingMuonPFRelIso->Write();
/*
    h_leadingMuonTrkIso_0p3->Write();
    h_leadingMuonTrkIso_0p4->Write();
    h_leadingMuonPfCandIsoNoPU_0p3->Write();
    h_leadingMuonPfCandIsoNoPU_0p4->Write();
    h_leadingMuonPfCandPtIsoNoPU_0p3->Write();
    h_leadingMuonPfCandPtIsoNoPU_0p4->Write();
    h_leadingMuonPfCandIso_0p3->Write();
    h_leadingMuonPfCandIso_0p4->Write();
    h_leadingMuonPfCandPtIso_0p3->Write();
    h_leadingMuonPfCandPtIso_0p4->Write();
    h_subleadingMuonTrkIso_0p3->Write();
    h_subleadingMuonTrkIso_0p4->Write();
    h_subleadingMuonPfCandIsoNoPU_0p3->Write();
    h_subleadingMuonPfCandIsoNoPU_0p4->Write();
    h_subleadingMuonPfCandPtIsoNoPU_0p3->Write();
    h_subleadingMuonPfCandPtIsoNoPU_0p4->Write();
    h_subleadingMuonPfCandIso_0p3->Write();
    h_subleadingMuonPfCandIso_0p4->Write();
    h_subleadingMuonPfCandPtIso_0p3->Write();
    h_subleadingMuonPfCandPtIso_0p4->Write();
*/
    h_leadingMuonTrkRelIso_0p3->Write();
    h_leadingMuonTrkRelIso_0p4->Write();
    h_leadingMuonPfCandRelIsoNoPU_0p3->Write();
    h_leadingMuonPfCandRelIsoNoPU_0p4->Write();
    h_leadingMuonPfCandPtRelIsoNoPU_0p3->Write();
    h_leadingMuonPfCandPtRelIsoNoPU_0p4->Write();
    h_leadingMuonPfCandRelIso_0p3->Write();
    h_leadingMuonPfCandRelIso_0p4->Write();
    h_leadingMuonPfCandPtRelIso_0p3->Write();
    h_leadingMuonPfCandPtRelIso_0p4->Write();

    h_subleadingMuonTrkRelIso_0p3->Write();
    h_subleadingMuonTrkRelIso_0p4->Write();
    h_subleadingMuonPfCandRelIsoNoPU_0p3->Write();
    h_subleadingMuonPfCandRelIsoNoPU_0p4->Write();
    h_subleadingMuonPfCandPtRelIsoNoPU_0p3->Write();
    h_subleadingMuonPfCandPtRelIsoNoPU_0p4->Write();
    h_subleadingMuonPfCandRelIso_0p3->Write();
    h_subleadingMuonPfCandRelIso_0p4->Write();
    h_subleadingMuonPfCandPtRelIso_0p3->Write();
    h_subleadingMuonPfCandPtRelIso_0p4->Write();
/*
    h_dimuonTrkIso_0p3->Write();
    h_dimuonTrkIso_0p4->Write();
    h_dimuonPfCandIsoNoPU_0p3->Write();
    h_dimuonPfCandIsoNoPU_0p4->Write();
    h_dimuonPfCandPtIsoNoPU_0p3->Write();
    h_dimuonPfCandPtIsoNoPU_0p4->Write();
    h_dimuonPfCandIso_0p3->Write();
    h_dimuonPfCandIso_0p4->Write();
    h_dimuonPfCandPtIso_0p3->Write();
    h_dimuonPfCandPtIso_0p4->Write();
*/
    h_dimuonTrkRelIso_0p3->Write();
    h_dimuonTrkRelIso_0p4->Write();
    h_dimuonPfCandRelIsoNoPU_0p3->Write();
    h_dimuonPfCandRelIsoNoPU_0p4->Write();
    h_dimuonPfCandPtRelIsoNoPU_0p3->Write();
    h_dimuonPfCandPtRelIsoNoPU_0p4->Write();
    h_dimuonPfCandRelIso_0p3->Write();
    h_dimuonPfCandRelIso_0p4->Write();
    h_dimuonPfCandPtRelIso_0p3->Write();
    h_dimuonPfCandPtRelIso_0p4->Write();

// Write hadron histos
/*
    h_leadingChsTrkIso_0p3->Write();
    h_leadingChsTrkIso_0p4->Write();
    h_leadingChsPfCandIsoNoPU_0p3->Write();
    h_leadingChsPfCandIsoNoPU_0p4->Write();
    h_leadingChsPfCandPtIsoNoPU_0p3->Write();
    h_leadingChsPfCandPtIsoNoPU_0p4->Write();
    h_leadingChsPfCandIso_0p3->Write();
    h_leadingChsPfCandIso_0p4->Write();
    h_leadingChsPfCandPtIso_0p3->Write();
    h_leadingChsPfCandPtIso_0p4->Write();
    h_subleadingChsTrkIso_0p3->Write();
    h_subleadingChsTrkIso_0p4->Write();
    h_subleadingChsPfCandIsoNoPU_0p3->Write();
    h_subleadingChsPfCandIsoNoPU_0p4->Write();
    h_subleadingChsPfCandPtIsoNoPU_0p3->Write();
    h_subleadingChsPfCandPtIsoNoPU_0p4->Write();
    h_subleadingChsPfCandIso_0p3->Write();
    h_subleadingChsPfCandIso_0p4->Write();
    h_subleadingChsPfCandPtIso_0p3->Write();
    h_subleadingChsPfCandPtIso_0p4->Write();
*/
    h_leadingChsTrkRelIso_0p3->Write();
    h_leadingChsTrkRelIso_0p4->Write();
    h_leadingChsPfCandRelIsoNoPU_0p3->Write();
    h_leadingChsPfCandRelIsoNoPU_0p4->Write();
    h_leadingChsPfCandPtRelIsoNoPU_0p3->Write();
    h_leadingChsPfCandPtRelIsoNoPU_0p4->Write();
    h_leadingChsPfCandRelIso_0p3->Write();
    h_leadingChsPfCandRelIso_0p4->Write();
    h_leadingChsPfCandPtRelIso_0p3->Write();
    h_leadingChsPfCandPtRelIso_0p4->Write();

    h_subleadingChsTrkRelIso_0p3->Write();
    h_subleadingChsTrkRelIso_0p4->Write();
    h_subleadingChsPfCandRelIsoNoPU_0p3->Write();
    h_subleadingChsPfCandRelIsoNoPU_0p4->Write();
    h_subleadingChsPfCandPtRelIsoNoPU_0p3->Write();
    h_subleadingChsPfCandPtRelIsoNoPU_0p4->Write();
    h_subleadingChsPfCandRelIso_0p3->Write();
    h_subleadingChsPfCandRelIso_0p4->Write();
    h_subleadingChsPfCandPtRelIso_0p3->Write();
    h_subleadingChsPfCandPtRelIso_0p4->Write();
/*
    h_dichsTrkIso_0p3->Write();
    h_dichsTrkIso_0p4->Write();
    h_dichsPfCandIsoNoPU_0p3->Write();
    h_dichsPfCandIsoNoPU_0p4->Write();
    h_dichsPfCandPtIsoNoPU_0p3->Write();
    h_dichsPfCandPtIsoNoPU_0p4->Write();
    h_dichsPfCandIso_0p3->Write();
    h_dichsPfCandIso_0p4->Write();
    h_dichsPfCandPtIso_0p3->Write();
    h_dichsPfCandPtIso_0p4->Write();
*/
    h_dichsTrkRelIso_0p3->Write();
    h_dichsTrkRelIso_0p4->Write();
    h_dichsPfCandRelIsoNoPU_0p3->Write();
    h_dichsPfCandRelIsoNoPU_0p4->Write();
    h_dichsPfCandPtRelIsoNoPU_0p3->Write();
    h_dichsPfCandPtRelIsoNoPU_0p4->Write();
    h_dichsPfCandRelIso_0p3->Write();
    h_dichsPfCandRelIso_0p4->Write();
    h_dichsPfCandPtRelIso_0p3->Write();
    h_dichsPfCandPtRelIso_0p4->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

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

        chs.emplace_back(k);
    }
    return chs;
}


bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons) {
    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

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

bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs) {

    for ( unsigned int i{0}; i < chs.size(); i++ ) {

        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

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

                event.chsTrkPairVec.first  = chsTrk1;
                event.chsTrkPairVec.second = chsTrk2;

                event.chsPairTrkIndex = getChsTrackPairIndex(event);

//                if ( (event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06) > 20. ) continue;

       	       	// If refit fails then reject event - all signal events	pass refit, but	QCD does not
                if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;

                TLorentzVector chsTrk1Refitted, chsTrk2Refitted;
                chsTrk1Refitted.SetPtEtaPhiE(event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                chsTrk2Refitted.SetPtEtaPhiE(event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                event.chsTrkPairVecRefitted.first  = chsTrk1Refitted;
                event.chsTrkPairVecRefitted.second = chsTrk2Refitted;

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

