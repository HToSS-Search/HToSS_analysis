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

float deltaR(float eta1, float phi1, float eta2, float phi2);

namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
   
    std::string outFileString{"plots/distributions/output.root"};
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Quick and dirty trigger plots
    // denom
    TH1F* h_leadingMuonPt_truth                  {new TH1F("h_leadingMuonPt_truth",                "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth               {new TH1F("h_subLeadingMuonPt_truth",             "", 200, 0., 100.)};
    TH1F* h_leadingMuonPt                        {new TH1F("h_leadingMuonPt",                      "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt                     {new TH1F("h_subLeadingMuonPt",                   "", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth                 {new TH1F("h_leadingMuonEta_truth",               "", 300, -3., 3.)};	
    TH1F* h_subLeadingMuonEta_truth              {new TH1F("h_subLeadingMuonEta_truth",            "", 300, -3., 3.)};
    TH1F* h_leadingMuonEta                       {new TH1F("h_leadingMuonEta",                     "", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta                    {new TH1F("h_subLeadingMuonEta",                  "", 300, -3., 3.)};

    TH1F* h_leadingMuonD0_truth                  {new TH1F("h_leadingMuonD0_truth",                "", 200, -100., 100)};
    TH1F* h_subLeadingMuonD0_truth               {new TH1F("h_subLeadingMuonD0_truth",             "", 200, -100., 100)};
    TH1F* h_leadingMuonD0                        {new TH1F("h_leadingMuonD0",                      "", 200, -100., 100)};
    TH1F* h_subLeadingMuonD0                     {new TH1F("h_subLeadingMuonD0",                   "", 200, -100., 100)};
    TH1F* h_leadingMuonDz_truth                  {new TH1F("h_leadingMuonDz_truth",                "", 200, -100., 100)};
    TH1F* h_subLeadingMuonDz_truth               {new TH1F("h_subLeadingMuonDz_truth",             "", 200, -100., 100)};
    TH1F* h_leadingMuonDz                        {new TH1F("h_leadingMuonDz",                      "", 200, -100., 100)};
    TH1F* h_subLeadingMuonDz                     {new TH1F("h_subLeadingMuonDz",                   "", 200, -100., 100)};

    TH1F* h_delR_truth                           {new TH1F("h_delR_truth",                         "", 100, 0., 1.0)};
    TH1F* h_delR                                 {new TH1F("h_delR",                               "", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth                     {new TH1F("h_diMuonMass_truth",                   "", 200, 0., 100.)}; 
    TH1F* h_diMuonMass                           {new TH1F("h_diMuonMass",                         "", 200, 0., 100.)};

    TH1F* h_leadingMuonImpactTransSig_truth      {new TH1F("h_leadingMuonImpactTransSig_truth",    "", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpactTransSig_truth   {new TH1F("h_subLeadingMuonImpactTransSig_truth", "", 400, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig            {new TH1F("h_leadingMuonImpactTransSig",          "", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpactTransSig         {new TH1F("h_subLeadingMuonImpactTransSig",       "", 400, 0., 100.)};
    TH1F* h_leadingMuonImpact3DSig_truth         {new TH1F("h_leadingMuonImpact3DSig_truth",       "", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpact3DSig_truth      {new TH1F("h_subLeadingMuonImpact3DSig_truth",    "", 400, 0., 100.)};
    TH1F* h_leadingMuonImpact3DSig               {new TH1F("h_leadingMuonImpact3DSig",             "", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpact3DSig            {new TH1F("h_subLeadingMuonImpact3DSig",          "", 400, 0., 100.)};

    TH1F* h_leadingMuonImpactTransSig_0to2_truth     {new TH1F("h_leadingMuonImpactTransSig_0to2_truth",   "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to2           {new TH1F("h_leadingMuonImpactTransSig_0to2",         "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_2to4_truth     {new TH1F("h_leadingMuonImpactTransSig_2to4_truth",   "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_2to4           {new TH1F("h_leadingMuonImpactTransSig_2to4",         "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_4to6_truth     {new TH1F("h_leadingMuonImpactTransSig_4to6_truth",   "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_4to6           {new TH1F("h_leadingMuonImpactTransSig_4to6",         "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_6to8_truth     {new TH1F("h_leadingMuonImpactTransSig_6to8_truth",   "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_6to8           {new TH1F("h_leadingMuonImpactTransSig_6to8",         "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_8to10_truth    {new TH1F("h_leadingMuonImpactTransSig_8to10_truth",  "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_8to10          {new TH1F("h_leadingMuonImpactTransSig_8to10",        "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to10_truth    {new TH1F("h_leadingMuonImpactTransSig_0to10_truth",  "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to10          {new TH1F("h_leadingMuonImpactTransSig_0to10",        "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_10to20_truth   {new TH1F("h_leadingMuonImpactTransSig_10to20_truth", "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_10to20         {new TH1F("h_leadingMuonImpactTransSig_10to20",       "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_20to30_truth   {new TH1F("h_leadingMuonImpactTransSig_20to30_truth", "", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_20to30         {new TH1F("h_leadingMuonImpactTransSig_20to30",       "", 100, 0., 100.)};

    // numerator - bParking
    TH1F* h_leadingMuonPt_truth_bParkingTrig                  {new TH1F("h_leadingMuonPt_truth_bParkingTrig",                 "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_bParkingTrig               {new TH1F("h_subLeadingMuonPt_truth_bParkingTrig",              "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_bParkingTrig                        {new TH1F("h_leadingMuonPt_bParkingTrig",                       "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_bParkingTrig                     {new TH1F("h_subLeadingMuonPt_bParkingTrig",                    "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_bParkingTrig                 {new TH1F("h_leadingMuonEta_truth_bParkingTrig",                "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_bParkingTrig              {new TH1F("h_subLeadingMuonEta_truth_bParkingTrig",             "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_bParkingTrig                       {new TH1F("h_leadingMuonEta_bParkingTrig",                      "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_bParkingTrig                    {new TH1F("h_subLeadingMuonEta_bParkingTrig",                   "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};

    TH1F* h_leadingMuonD0_truth_bParkingTrig                  {new TH1F("h_leadingMuonD0_truth_bParkingTrig",                 "Trigger turn-on for signal; d_{0} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_subLeadingMuonD0_truth_bParkingTrig               {new TH1F("h_subLeadingMuonD0_truth_bParkingTrig",              "Trigger turn-on for signal; d_{0} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_leadingMuonD0_bParkingTrig                        {new TH1F("h_leadingMuonD0_bParkingTrig",                       "Trigger turn-on for signal; d_{0} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_subLeadingMuonD0_bParkingTrig                     {new TH1F("h_subLeadingMuonD0_bParkingTrig",                    "Trigger turn-on for signal; d_{0} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_leadingMuonDz_truth_bParkingTrig                  {new TH1F("h_leadingMuonDz_truth_bParkingTrig",                 "Trigger turn-on for signal; d_{z} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_subLeadingMuonDz_truth_bParkingTrig               {new TH1F("h_subLeadingMuonDz_truth_bParkingTrig",              "Trigger turn-on for signal; d_{z} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_leadingMuonDz_bParkingTrig                        {new TH1F("h_leadingMuonDz_bParkingTrig",                       "Trigger turn-on for signal; d_{z} (cm); trigger #epsilon", 200, -100., 100)};
    TH1F* h_subLeadingMuonDz_bParkingTrig                     {new TH1F("h_subLeadingMuonDz_bParkingTrig",                    "Trigger turn-on for signal; d_{z} (cm); trigger #epsilon", 200, -100., 100)};

    TH1F* h_delR_truth_bParkingTrig                           {new TH1F("h_delR_truth_bParkingTrig",                          "Trigger turn-on for signal; #Delta R; trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_bParkingTrig                                 {new TH1F("h_delR_bParkingTrig",                                "Trigger turn-on for signal; #Delta R; trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_bParkingTrig                     {new TH1F("h_diMuonMass_truth_bParkingTrig",                    "Trigger turn-on for signal; m_{#mu#mu}; trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_bParkingTrig                           {new TH1F("h_diMuonMass_bParkingTrig",                          "Trigger turn-on for signal; m_{#mu#mu}; trigger #epsilon", 200, 0., 100.)};

    TH1F* h_leadingMuonImpactTransSig_truth_bParkingTrig      {new TH1F("h_leadingMuonImpactTransSig_truth_bParkingTrig",     "Trigger turn-on for signal; IP_{trans}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpactTransSig_truth_bParkingTrig   {new TH1F("h_subLeadingMuonImpactTransSig_truth_bParkingTrig",  "Trigger turn-on for signal; IP_{trans}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_bParkingTrig  	      {new TH1F("h_leadingMuonImpactTransSig_bParkingTrig",           "Trigger turn-on for signal; IP_{trans}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpactTransSig_bParkingTrig  	      {new TH1F("h_subLeadingMuonImpactTransSig_bParkingTrig",        "Trigger turn-on for signal; IP_{trans}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_leadingMuonImpact3DSig_truth_bParkingTrig         {new TH1F("h_leadingMuonImpact3DSig_truth_bParkingTrig",        "Trigger turn-on for signal; IP_{3D}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpact3DSig_truth_bParkingTrig      {new TH1F("h_subLeadingMuonImpact3DSig_truth_bParkingTrig",     "Trigger turn-on for signal; IP_{3D}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_leadingMuonImpact3DSig_bParkingTrig     	      {new TH1F("h_leadingMuonImpact3DSig_bParkingTrig",              "Trigger turn-on for signal; IP_{3D}; trigger #epsilon", 400, 0., 100.)};
    TH1F* h_subLeadingMuonImpact3DSig_bParkingTrig     	      {new TH1F("h_subLeadingMuonImpact3DSig_bParkingTrig",           "Trigger turn-on for signal; IP_{3D}; trigger #epsilon", 400, 0., 100.)};

    TH1F* h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig     {new TH1F("h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig",   "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to2_bParkingTrig           {new TH1F("h_leadingMuonImpactTransSig_0to2_bParkingTrig",         "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig     {new TH1F("h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig",   "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_2to4_bParkingTrig           {new TH1F("h_leadingMuonImpactTransSig_2to4_bParkingTrig",         "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig     {new TH1F("h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig",   "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_4to6_bParkingTrig           {new TH1F("h_leadingMuonImpactTransSig_4to6_bParkingTrig",         "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig     {new TH1F("h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig",   "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_6to8_bParkingTrig           {new TH1F("h_leadingMuonImpactTransSig_6to8_bParkingTrig",         "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig    {new TH1F("h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig",  "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_8to10_bParkingTrig          {new TH1F("h_leadingMuonImpactTransSig_8to10_bParkingTrig",        "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig    {new TH1F("h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig",  "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_0to10_bParkingTrig          {new TH1F("h_leadingMuonImpactTransSig_0to10_bParkingTrig",        "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig   {new TH1F("h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig", "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_10to20_bParkingTrig         {new TH1F("h_leadingMuonImpactTransSig_10to20_bParkingTrig",       "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig   {new TH1F("h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig", "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};
    TH1F* h_leadingMuonImpactTransSig_20to30_bParkingTrig         {new TH1F("h_leadingMuonImpactTransSig_20to30_bParkingTrig",       "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 100, 0., 100.)};

    // ratio 
    TH1F* h_leadingMuonPt_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonPt_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonPt_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonPt_bParkingTrig_ratio;
    TH1F* h_leadingMuonEta_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonEta_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonEta_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonEta_bParkingTrig_ratio;

    TH1F* h_leadingMuonD0_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonD0_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonD0_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonD0_bParkingTrig_ratio;
    TH1F* h_leadingMuonDz_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonDz_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonDz_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonDz_bParkingTrig_ratio;

    TH1F* h_delR_truth_bParkingTrig_ratio;
    TH1F* h_delR_bParkingTrig_ratio;
    TH1F* h_diMuonMass_truth_bParkingTrig_ratio;
    TH1F* h_diMuonMass_bParkingTrig_ratio;

    TH1F* h_leadingMuonImpactTransSig_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonImpactTransSig_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonImpactTransSig_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpact3DSig_truth_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonImpact3DSig_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpact3DSig_bParkingTrig_ratio;
    TH1F* h_subLeadingMuonImpact3DSig_bParkingTrig_ratio;

    TH1F* h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig_ratio;
    TH1F* h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio;

    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(34790.3414),
        "Lumi to scale MC plots to.")(
        "outfile,o",
        po::value<std::string>(&outFileString)->default_value(outFileString),
        "Output file for plots.")(
        ",n",
        po::value<Long64_t>(&nEvents)->default_value(0),
        "The number of events to be run over. All if set to 0.")(
        ",u",
        po::bool_switch(&usePostLepTree),
        "Use post lepton selection trees.");
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
    const std::string era {"2018"};
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

        AnalysisEvent event{dataset->isMC(), datasetChain, false, true};

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

            if ( event.numMuonPF2PAT > 1 ) {
                // fill muon pT plots pre-triggers
                //// ID requirements PF muon? no pT cut
                //// reco pT 
                int mu1 {-1}, mu2{-1};
                for ( Int_t k{0}; k < event.numMuonPF2PAT; k++ ) {
                    if ( event.genMuonPF2PATScalarAncestor[k] && mu1 < 0 ) mu1 = k;
                    else if ( event.genMuonPF2PATScalarAncestor[k] && mu2 < 0 ) mu2 = k;
                    else if (mu1 >= 0 && mu2 > 0) break;
                }

                const TLorentzVector muon1_truth {event.muonPF2PATPX[mu1], event.muonPF2PATPY[mu1], event.muonPF2PATPZ[mu1], event.muonPF2PATE[mu1]};
                const TLorentzVector muon2_truth {event.muonPF2PATPX[mu2], event.muonPF2PATPY[mu2], event.muonPF2PATPZ[mu2], event.muonPF2PATE[mu2]}; 
                const TLorentzVector muon1 {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
                const TLorentzVector muon2 {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]}; 
	
                const float delR_truth = deltaR(event.muonPF2PATEta[mu1], event.muonPF2PATPhi[mu1], event.muonPF2PATEta[mu2], event.muonPF2PATPhi[mu2]);
                const float delR       = deltaR(event.muonPF2PATEta[0], event.muonPF2PATPhi[0], event.muonPF2PATEta[1], event.muonPF2PATPhi[1]);
                const float mass_truth = (muon1_truth + muon2_truth).M();
                const float mass       = (muon1 + muon2).M();

                // Fill general pT/dR (with and without scalar parentage)
                h_leadingMuonPt_truth->Fill(event.muonPF2PATPt[mu1]);
                h_subLeadingMuonPt_truth->Fill(event.muonPF2PATPt[mu2]);
                h_leadingMuonPt->Fill(event.muonPF2PATPt[0]);
                h_subLeadingMuonPt->Fill(event.muonPF2PATPt[1]);
                h_leadingMuonEta_truth->Fill(event.muonPF2PATEta[mu1]);
                h_subLeadingMuonEta_truth->Fill(event.muonPF2PATEta[mu2]);
                h_leadingMuonEta->Fill(event.muonPF2PATEta[0]);
                h_subLeadingMuonEta->Fill(event.muonPF2PATEta[1]);

                h_leadingMuonD0_truth->Fill(event.muonPF2PATDBPV[mu1]);
                h_subLeadingMuonD0_truth->Fill(event.muonPF2PATDBPV[mu2]);
                h_leadingMuonDz_truth->Fill(event.muonPF2PATDBPV[0]);
                h_subLeadingMuonDz_truth->Fill(event.muonPF2PATDBPV[1]);
                h_leadingMuonD0->Fill(event.muonPF2PATDZPV[mu1]);
                h_subLeadingMuonD0->Fill(event.muonPF2PATDZPV[mu2]);
                h_leadingMuonDz->Fill(event.muonPF2PATDZPV[0]);
                h_subLeadingMuonDz->Fill(event.muonPF2PATDZPV[1]);

                h_delR_truth->Fill(delR_truth);
                h_delR->Fill(delR);
                h_diMuonMass_truth->Fill(mass_truth);
                h_diMuonMass->Fill(mass);

                h_leadingMuonImpactTransSig_truth->Fill(event.muonPF2PATImpactTransSignificance[mu1]);
                h_subLeadingMuonImpactTransSig_truth->Fill(event.muonPF2PATImpactTransSignificance[mu2]);
                h_leadingMuonImpactTransSig->Fill(event.muonPF2PATImpactTransSignificance[0]);
                h_subLeadingMuonImpactTransSig->Fill(event.muonPF2PATImpactTransSignificance[1]);
                h_leadingMuonImpact3DSig_truth->Fill(event.muonPF2PATImpact3DSignificance[mu1]);
                h_subLeadingMuonImpact3DSig_truth->Fill(event.muonPF2PATImpact3DSignificance[mu2]);
                h_leadingMuonImpact3DSig->Fill(event.muonPF2PATImpact3DSignificance[0]);
                h_subLeadingMuonImpact3DSig->Fill(event.muonPF2PATImpact3DSignificance[1]);

                if (event.muonPF2PATImpactTransSignificance[mu1] >= 0. && event.muonPF2PATImpactTransSignificance[mu1] < 2.0  ) h_leadingMuonImpactTransSig_0to2_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 0. && event.muonPF2PATImpactTransSignificance[0]   < 2.0  ) h_leadingMuonImpactTransSig_0to2->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 2. && event.muonPF2PATImpactTransSignificance[mu1] < 4.0  ) h_leadingMuonImpactTransSig_2to4_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 2. && event.muonPF2PATImpactTransSignificance[0]   < 4.0  ) h_leadingMuonImpactTransSig_2to4->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 4. && event.muonPF2PATImpactTransSignificance[mu1] < 6.0  ) h_leadingMuonImpactTransSig_4to6_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 4. && event.muonPF2PATImpactTransSignificance[0]   < 6.0  ) h_leadingMuonImpactTransSig_4to6->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 6. && event.muonPF2PATImpactTransSignificance[mu1] < 8.0  ) h_leadingMuonImpactTransSig_6to8_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 6. && event.muonPF2PATImpactTransSignificance[0]   < 8.0  ) h_leadingMuonImpactTransSig_6to8->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 8. && event.muonPF2PATImpactTransSignificance[mu1] < 10.0 ) h_leadingMuonImpactTransSig_8to10_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 8. && event.muonPF2PATImpactTransSignificance[0]   < 10.0 ) h_leadingMuonImpactTransSig_8to10->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 0. && event.muonPF2PATImpactTransSignificance[mu1] < 10.0 ) h_leadingMuonImpactTransSig_0to10_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 0. && event.muonPF2PATImpactTransSignificance[0]   < 10.0 ) h_leadingMuonImpactTransSig_0to10->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 10. && event.muonPF2PATImpactTransSignificance[mu1] < 20.0) h_leadingMuonImpactTransSig_10to20_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 10. && event.muonPF2PATImpactTransSignificance[0]   < 20.0) h_leadingMuonImpactTransSig_10to20->Fill(event.muonPF2PATPt[0]);
                if (event.muonPF2PATImpactTransSignificance[mu1] >= 20. && event.muonPF2PATImpactTransSignificance[mu1] < 30.0) h_leadingMuonImpactTransSig_20to30_truth->Fill(event.muonPF2PATPt[mu1]);
                if (event.muonPF2PATImpactTransSignificance[0]   >= 20. && event.muonPF2PATImpactTransSignificance[0]   < 30.0) h_leadingMuonImpactTransSig_20to30->Fill(event.muonPF2PATPt[0]);

                // Fill pT post trigger (with and without scalar parentage)
                if (event.bParkingTrigInclusive()) {
//                if (event.bParkingTrig_Mu12_IP6()) {
//                if (event.bParkingTrig_Mu9_IP6()) {
//                if (event.bParkingTrig_Mu9_IP5()) {

                    double eventWeight = 1.;

                    eventWeight *= (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / std::abs(event.origWeightForNorm));
                    eventWeight *= datasetWeight;

                    foundEvents++;
                    foundEventsNorm += eventWeight;

                    h_leadingMuonPt_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_bParkingTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_bParkingTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_bParkingTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_bParkingTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_bParkingTrig->Fill(event.muonPF2PATEta[1]);

                    h_leadingMuonD0_truth_bParkingTrig->Fill(event.muonPF2PATDBPV[mu1]);
                    h_subLeadingMuonD0_truth_bParkingTrig->Fill(event.muonPF2PATDBPV[mu2]);
                    h_leadingMuonDz_truth_bParkingTrig->Fill(event.muonPF2PATDBPV[0]);
                    h_subLeadingMuonDz_truth_bParkingTrig->Fill(event.muonPF2PATDBPV[1]);
                    h_leadingMuonD0_bParkingTrig->Fill(event.muonPF2PATDZPV[mu1]);
                    h_subLeadingMuonD0_bParkingTrig->Fill(event.muonPF2PATDZPV[mu2]);
                    h_leadingMuonDz_bParkingTrig->Fill(event.muonPF2PATDZPV[0]);
                    h_subLeadingMuonDz_bParkingTrig->Fill(event.muonPF2PATDZPV[1]);

                    h_delR_truth_bParkingTrig->Fill(delR_truth);
                    h_delR_bParkingTrig->Fill(delR);
                    h_diMuonMass_truth_bParkingTrig->Fill(mass_truth);
                    h_diMuonMass_bParkingTrig->Fill(mass);

                    h_leadingMuonImpactTransSig_truth_bParkingTrig->Fill(event.muonPF2PATImpactTransSignificance[mu1]);
                    h_subLeadingMuonImpactTransSig_truth_bParkingTrig->Fill(event.muonPF2PATImpactTransSignificance[mu2]);
                    h_leadingMuonImpactTransSig_bParkingTrig->Fill(event.muonPF2PATImpactTransSignificance[0]);
                    h_subLeadingMuonImpactTransSig_bParkingTrig->Fill(event.muonPF2PATImpactTransSignificance[1]);
                    h_leadingMuonImpact3DSig_truth_bParkingTrig->Fill(event.muonPF2PATImpact3DSignificance[mu1]);
                    h_subLeadingMuonImpact3DSig_truth_bParkingTrig->Fill(event.muonPF2PATImpact3DSignificance[mu2]);
                    h_leadingMuonImpact3DSig_bParkingTrig->Fill(event.muonPF2PATImpact3DSignificance[0]);
                    h_subLeadingMuonImpact3DSig_bParkingTrig->Fill(event.muonPF2PATImpact3DSignificance[1]);

                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 0. && event.muonPF2PATImpactTransSignificance[mu1] < 2.0  ) h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 0. && event.muonPF2PATImpactTransSignificance[0]   < 2.0  ) h_leadingMuonImpactTransSig_0to2_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 2. && event.muonPF2PATImpactTransSignificance[mu1] < 4.0  ) h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 2. && event.muonPF2PATImpactTransSignificance[0]   < 4.0  ) h_leadingMuonImpactTransSig_2to4_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 4. && event.muonPF2PATImpactTransSignificance[mu1] < 6.0  ) h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 4. && event.muonPF2PATImpactTransSignificance[0]   < 6.0  ) h_leadingMuonImpactTransSig_4to6_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 6. && event.muonPF2PATImpactTransSignificance[mu1] < 8.0  ) h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 6. && event.muonPF2PATImpactTransSignificance[0]   < 8.0  ) h_leadingMuonImpactTransSig_6to8_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 8. && event.muonPF2PATImpactTransSignificance[mu1] < 10.0 ) h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 8. && event.muonPF2PATImpactTransSignificance[0]   < 10.0 ) h_leadingMuonImpactTransSig_8to10_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 0. && event.muonPF2PATImpactTransSignificance[mu1] < 10.0 ) h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 0. && event.muonPF2PATImpactTransSignificance[0]   < 10.0 ) h_leadingMuonImpactTransSig_0to10_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 10. && event.muonPF2PATImpactTransSignificance[mu1] < 20.0) h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 10. && event.muonPF2PATImpactTransSignificance[0]   < 20.0) h_leadingMuonImpactTransSig_10to20_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    if (event.muonPF2PATImpactTransSignificance[mu1] >= 20. && event.muonPF2PATImpactTransSignificance[mu1] < 30.0) h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    if (event.muonPF2PATImpactTransSignificance[0]   >= 20. && event.muonPF2PATImpactTransSignificance[0]   < 30.0) h_leadingMuonImpactTransSig_20to30_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                }
            }

        } // end event loop

        std::cerr << "\nFound " << foundEvents << " in " << dataset->name() << std::endl;
        std::cerr << "Found " << foundEventsNorm << " after normalisation in " << dataset->name() << std::endl;
        std::cerr << "\n\n";

    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Denominators
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

    h_leadingMuonImpactTransSig_truth->Write();
    h_subLeadingMuonImpactTransSig_truth->Write();
    h_leadingMuonImpactTransSig->Write();
    h_subLeadingMuonImpactTransSig->Write();
    h_leadingMuonImpact3DSig_truth->Write();
    h_subLeadingMuonImpact3DSig_truth->Write();
    h_leadingMuonImpact3DSig->Write();
    h_subLeadingMuonImpact3DSig->Write();   

    // Numerators
    h_leadingMuonPt_truth_bParkingTrig->Write();
    h_subLeadingMuonPt_truth_bParkingTrig->Write();
    h_leadingMuonPt_bParkingTrig->Write();
    h_subLeadingMuonPt_bParkingTrig->Write();
    h_leadingMuonEta_truth_bParkingTrig->Write();
    h_subLeadingMuonEta_truth_bParkingTrig->Write();
    h_leadingMuonEta_bParkingTrig->Write();
    h_subLeadingMuonEta_bParkingTrig->Write();
    h_delR_truth_bParkingTrig->Write();
    h_delR_bParkingTrig->Write();
    h_diMuonMass_truth_bParkingTrig->Write();
    h_diMuonMass_bParkingTrig->Write();

    h_leadingMuonImpactTransSig_truth_bParkingTrig->Write();
    h_subLeadingMuonImpactTransSig_truth_bParkingTrig->Write();
    h_leadingMuonImpactTransSig_bParkingTrig->Write();
    h_subLeadingMuonImpactTransSig_bParkingTrig->Write();
    h_leadingMuonImpact3DSig_truth_bParkingTrig->Write();
    h_subLeadingMuonImpact3DSig_truth_bParkingTrig->Write();
    h_leadingMuonImpact3DSig_bParkingTrig->Write();
    h_subLeadingMuonImpact3DSig_bParkingTrig->Write();

    // Ratio
    h_leadingMuonPt_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonPt_truth_bParkingTrig->Clone("h_leadingMuonPt_truth_bParkingTrig_ratio"));
    h_subLeadingMuonPt_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonPt_truth_bParkingTrig->Clone("h_subLeadingMuonPt_truth_bParkingTrig_ratio"));
    h_leadingMuonPt_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonPt_bParkingTrig->Clone("h_leadingMuonPt_bParkingTrig_ratio"));
    h_subLeadingMuonPt_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonPt_bParkingTrig->Clone("h_subLeadingMuonPt_bParkingTrig_ratio"));
    h_leadingMuonEta_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonEta_truth_bParkingTrig->Clone("h_leadingMuonEta_truth_bParkingTrig_ratio"));
    h_subLeadingMuonEta_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonEta_truth_bParkingTrig->Clone("h_subLeadingMuonEta_truth_bParkingTrig_ratio"));
    h_leadingMuonEta_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonEta_bParkingTrig->Clone("h_leadingMuonEta_bParkingTrig_ratio"));
    h_subLeadingMuonEta_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonEta_bParkingTrig->Clone("h_subLeadingMuonEta_bParkingTrig_ratio"));

    h_leadingMuonD0_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonD0_truth_bParkingTrig->Clone("h_leadingMuonD0_truth_bParkingTrig_ratio"));
    h_subLeadingMuonD0_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonD0_truth_bParkingTrig->Clone("h_subLeadingMuonD0_truth_bParkingTrig_ratio"));
    h_leadingMuonD0_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonD0_bParkingTrig->Clone("h_leadingMuonD0_bParkingTrig_ratio"));
    h_subLeadingMuonD0_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonD0_bParkingTrig->Clone("h_subLeadingMuonD0_bParkingTrig_ratio"));
    h_leadingMuonDz_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonDz_truth_bParkingTrig->Clone("h_leadingMuonDz_truth_bParkingTrig_ratio"));
    h_subLeadingMuonDz_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonDz_truth_bParkingTrig->Clone("h_subLeadingMuonDz_truth_bParkingTrig_ratio"));
    h_leadingMuonDz_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonDz_bParkingTrig->Clone("h_leadingMuonDz_bParkingTrig_ratio"));
    h_subLeadingMuonDz_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonDz_bParkingTrig->Clone("h_subLeadingMuonDz_bParkingTrig_ratio"));

    h_delR_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_delR_truth_bParkingTrig->Clone("h_delR_truth_bParkingTrig_ratio"));
    h_delR_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_delR_bParkingTrig->Clone("h_delR_bParkingTrig_ratio"));
    h_diMuonMass_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_diMuonMass_truth_bParkingTrig->Clone("h_diMuonMass_truth_bParkingTrig_ratio"));
    h_diMuonMass_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_diMuonMass_bParkingTrig->Clone("h_diMuonMass_bParkingTrig_ratio"));

    h_leadingMuonImpactTransSig_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_truth_bParkingTrig_ratio"));
    h_subLeadingMuonImpactTransSig_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonImpactTransSig_truth_bParkingTrig->Clone("h_subLeadingMuonImpactTransSig_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_bParkingTrig->Clone("h_leadingMuonImpactTransSig_bParkingTrig_ratio"));
    h_subLeadingMuonImpactTransSig_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonImpactTransSig_bParkingTrig->Clone("h_subLeadingMuonImpactTransSig_bParkingTrig_ratio"));
    h_leadingMuonImpact3DSig_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpact3DSig_truth_bParkingTrig->Clone("h_leadingMuonImpact3DSig_truth_bParkingTrig_ratio"));
    h_subLeadingMuonImpact3DSig_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonImpact3DSig_truth_bParkingTrig->Clone("h_subLeadingMuonImpact3DSig_truth_bParkingTrig_ratio"));
    h_leadingMuonImpact3DSig_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpact3DSig_bParkingTrig->Clone("h_leadingMuonImpact3DSig_bParkingTrig_ratio"));
    h_subLeadingMuonImpact3DSig_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_subLeadingMuonImpact3DSig_bParkingTrig->Clone("h_subLeadingMuonImpact3DSig_bParkingTrig_ratio"));

    h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_0to2_bParkingTrig->Clone("h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig_ratio  = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_2to4_bParkingTrig->Clone("h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig_ratio  = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_4to6_bParkingTrig->Clone("h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_6to8_bParkingTrig->Clone("h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_8to10_bParkingTrig->Clone("h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_0to10_bParkingTrig->Clone("h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_10to20_bParkingTrig->Clone("h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig->Clone("h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig_ratio"));
    h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio = dynamic_cast<TH1F*>(h_leadingMuonImpactTransSig_20to30_bParkingTrig->Clone("h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio"));

    h_leadingMuonPt_truth_bParkingTrig_ratio->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_bParkingTrig_ratio->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_bParkingTrig_ratio->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_bParkingTrig_ratio->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_bParkingTrig_ratio->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_bParkingTrig_ratio->Divide(h_subLeadingMuonEta);

    h_leadingMuonD0_truth_bParkingTrig_ratio->Divide(h_leadingMuonD0_truth);
    h_subLeadingMuonD0_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonD0_truth);
    h_leadingMuonD0_bParkingTrig_ratio->Divide(h_leadingMuonD0);
    h_subLeadingMuonD0_bParkingTrig_ratio->Divide(h_subLeadingMuonD0);
    h_leadingMuonDz_truth_bParkingTrig_ratio->Divide(h_leadingMuonDz_truth);
    h_subLeadingMuonDz_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonDz_truth);
    h_leadingMuonDz_bParkingTrig_ratio->Divide(h_leadingMuonDz);
    h_subLeadingMuonDz_bParkingTrig_ratio->Divide(h_subLeadingMuonDz);

    h_delR_truth_bParkingTrig_ratio->Divide(h_delR_truth);
    h_delR_bParkingTrig_ratio->Divide(h_delR);
    h_diMuonMass_truth_bParkingTrig_ratio->Divide(h_diMuonMass_truth);
    h_diMuonMass_bParkingTrig_ratio->Divide(h_diMuonMass);

    h_leadingMuonImpactTransSig_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_truth);
    h_subLeadingMuonImpactTransSig_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonImpactTransSig_truth);
    h_leadingMuonImpactTransSig_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig);
    h_subLeadingMuonImpactTransSig_bParkingTrig_ratio->Divide(h_subLeadingMuonImpactTransSig);
    h_leadingMuonImpact3DSig_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpact3DSig_truth);
    h_subLeadingMuonImpact3DSig_truth_bParkingTrig_ratio->Divide(h_subLeadingMuonImpact3DSig_truth);
    h_leadingMuonImpact3DSig_bParkingTrig_ratio->Divide(h_leadingMuonImpact3DSig);
    h_subLeadingMuonImpact3DSig_bParkingTrig_ratio->Divide(h_subLeadingMuonImpact3DSig);

    h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_0to2_truth);
    h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_0to2);
    h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_2to4_truth);
    h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_2to4);
    h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_4to6_truth);
    h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_4to6);
    h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_6to8_truth);
    h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_6to8);
    h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_8to10_truth);
    h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_8to10);
    h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_0to10_truth);
    h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_0to10);
    h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_10to20_truth);
    h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_10to20);
    h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_20to30_truth);
    h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio->Divide(h_leadingMuonImpactTransSig_20to30);

    h_leadingMuonPt_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonPt_truth_bParkingTrig_ratio->Write();
    h_leadingMuonPt_bParkingTrig_ratio->Write();
    h_subLeadingMuonPt_bParkingTrig_ratio->Write();
    h_leadingMuonEta_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonEta_truth_bParkingTrig_ratio->Write();
    h_leadingMuonEta_bParkingTrig_ratio->Write();
    h_subLeadingMuonEta_bParkingTrig_ratio->Write();

    h_leadingMuonD0_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonD0_truth_bParkingTrig_ratio->Write();
    h_leadingMuonD0_bParkingTrig_ratio->Write();
    h_subLeadingMuonD0_bParkingTrig_ratio->Write();
    h_leadingMuonDz_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonDz_truth_bParkingTrig_ratio->Write();
    h_leadingMuonDz_bParkingTrig_ratio->Write();
    h_subLeadingMuonDz_bParkingTrig_ratio->Write();

    h_delR_truth_bParkingTrig_ratio->Write();
    h_delR_bParkingTrig_ratio->Write();
    h_diMuonMass_truth_bParkingTrig_ratio->Write();
    h_diMuonMass_bParkingTrig_ratio->Write();

    h_leadingMuonImpactTransSig_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonImpactTransSig_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_bParkingTrig_ratio->Write();
    h_subLeadingMuonImpactTransSig_bParkingTrig_ratio->Write();
    h_leadingMuonImpact3DSig_truth_bParkingTrig_ratio->Write();
    h_subLeadingMuonImpact3DSig_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpact3DSig_bParkingTrig_ratio->Write();
    h_subLeadingMuonImpact3DSig_bParkingTrig_ratio->Write();

    h_leadingMuonImpactTransSig_0to2_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_0to2_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_2to4_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_2to4_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_4to6_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_4to6_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_6to8_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_6to8_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_8to10_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_8to10_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_0to10_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_0to10_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_10to20_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_10to20_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_20to30_truth_bParkingTrig_ratio->Write();
    h_leadingMuonImpactTransSig_20to30_bParkingTrig_ratio->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
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
