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

bool pass_DST_HT250_Calo     ( AnalysisEvent& event );
bool pass_DST_HT250_CaloBtag ( AnalysisEvent& event );
bool pass_DST_HT410_PF       ( AnalysisEvent& event );
bool pass_DST_HT410_Btag     ( AnalysisEvent& event );
bool pass_DST_DoubleMu_noVtx ( AnalysisEvent& event );

namespace fs = boost::filesystem;

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

    // Quick and dirty trigger plots
    // denom
    TH1F* h_leadingMuonPt_truth                  {new TH1F("h_leadingMuonPt_truth",      "", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth               {new TH1F("h_subLeadingMuonPt_truth",   "", 200, 0., 100.)};
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
    // numerator - HT250 Calo
    TH1F* h_leadingMuonPt_truth_HT250_CaloTrig           {new TH1F("h_leadingMuonPt_truth_HT250_CaloTrig",      "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_HT250_CaloTrig        {new TH1F("h_subLeadingMuonPt_truth_HT250_CaloTrig",   "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_HT250_CaloTrig                 {new TH1F("h_leadingMuonPt_HT250_CaloTrig",            "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_HT250_CaloTrig              {new TH1F("h_subLeadingMuonPt_HT250_CaloTrig",         "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_HT250_CaloTrig          {new TH1F("h_leadingMuonEta_truth_HT250_CaloTrig",     "Trigger turn-on for signal; #eta; H_{T} > 250 Calo trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_HT250_CaloTrig       {new TH1F("h_subLeadingMuonEta_truth_HT250_CaloTrig",  "Trigger turn-on for signal; #eta; H_{T} > 250 Calo trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_HT250_CaloTrig                {new TH1F("h_leadingMuonEta_HT250_CaloTrig",           "Trigger turn-on for signal; #eta; H_{T} > 250 Calo trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_HT250_CaloTrig             {new TH1F("h_subLeadingMuonEta_HT250_CaloTrig",        "Trigger turn-on for signal; #eta; H_{T} > 250 Calo trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_HT250_CaloTrig                    {new TH1F("h_delR_truth_HT250_CaloTrig",               "Trigger turn-on for signal; #Delta R; H_{T} > 250 Calo trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_HT250_CaloTrig                          {new TH1F("h_delR_HT250_CaloTrig",                     "Trigger turn-on for signal; #Delta R; H_{T} > 250 Calo trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_HT250_CaloTrig              {new TH1F("h_diMuonMass_truth_HT250_CaloTrig",         "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_HT250_CaloTrig                    {new TH1F("h_diMuonMass_HT250_CaloTrig",               "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 250 Calo trigger #epsilon", 200, 0., 100.)};
    // numerator - HT250 Calo Btag
    TH1F* h_leadingMuonPt_truth_HT250_CaloBtagTrig         {new TH1F("h_leadingMuonPt_truth_HT250_CaloBtagTrig",      "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_HT250_CaloBtagTrig      {new TH1F("h_subLeadingMuonPt_truth_HT250_CaloBtagTrig",   "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_HT250_CaloBtagTrig               {new TH1F("h_leadingMuonPt_HT250_CaloBtagTrig",            "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_HT250_CaloBtagTrig            {new TH1F("h_subLeadingMuonPt_HT250_CaloBtagTrig",         "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_HT250_CaloBtagTrig        {new TH1F("h_leadingMuonEta_truth_HT250_CaloBtagTrig",     "Trigger turn-on for signal; #eta; H_{T} > 250 Calo Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_HT250_CaloBtagTrig     {new TH1F("h_subLeadingMuonEta_truth_HT250_CaloBtagTrig",  "Trigger turn-on for signal; #eta; H_{T} > 250 Calo Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_HT250_CaloBtagTrig              {new TH1F("h_leadingMuonEta_HT250_CaloBtagTrig",           "Trigger turn-on for signal; #eta; H_{T} > 250 Calo Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_HT250_CaloBtagTrig           {new TH1F("h_subLeadingMuonEta_HT250_CaloBtagTrig",        "Trigger turn-on for signal; #eta; H_{T} > 250 Calo Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_HT250_CaloBtagTrig                  {new TH1F("h_delR_truth_HT250_CaloBtagTrig",               "Trigger turn-on for signal; #Delta R; H_{T} > 250 Calo Btag trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_HT250_CaloBtagTrig                        {new TH1F("h_delR_HT250_CaloBtagTrig",                     "Trigger turn-on for signal;  #Delta R; H_{T} > 250 Calo Btag trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_HT250_CaloBtagTrig            {new TH1F("h_diMuonMass_truth_HT250_CaloBtagTrig",         "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_HT250_CaloBtagTrig                  {new TH1F("h_diMuonMass_HT250_CaloBtagTrig",               "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 250 Calo Btag trigger #epsilon", 200, 0., 100.)};    
    // numerator - HT410 PF
    TH1F* h_leadingMuonPt_truth_HT410_PF         {new TH1F("h_leadingMuonPt_truth_HT410_PF",      "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_HT410_PF      {new TH1F("h_subLeadingMuonPt_truth_HT410_PF",   "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_HT410_PF               {new TH1F("h_leadingMuonPt_HT410_PF",            "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_HT410_PF            {new TH1F("h_subLeadingMuonPt_HT410_PF",         "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_HT410_PF        {new TH1F("h_leadingMuonEta_truth_HT410_PF",     "Trigger turn-on for signal; #eta; H_{T} > 410 PF trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_HT410_PF     {new TH1F("h_subLeadingMuonEta_truth_HT410_PF",  "Trigger turn-on for signal; #eta; H_{T} > 410 PF trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_HT410_PF              {new TH1F("h_leadingMuonEta_HT410_PF",           "Trigger turn-on for signal; #eta; H_{T} > 410 PF trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_HT410_PF           {new TH1F("h_subLeadingMuonEta_HT410_PF",        "Trigger turn-on for signal; #eta; H_{T} > 410 PF trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_HT410_PF                  {new TH1F("h_delR_truth_HT410_PF",               "Trigger turn-on for signal; #Delta R; H_{T} > 410 PF trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_HT410_PF                        {new TH1F("h_delR_HT410_PF",                     "Trigger turn-on for signal; #Delta R; L2 #mu#mu trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_HT410_PF            {new TH1F("h_diMuonMass_truth_HT410_PF",         "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_HT410_PF                  {new TH1F("h_diMuonMass_HT410_PF",               "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 410 PF trigger #epsilon", 200, 0., 100.)};
    // numerator HT410 Btag
    TH1F* h_leadingMuonPt_truth_HT410_Btag     {new TH1F("h_leadingMuonPt_truth_HT410_Btag",      "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_HT410_Btag  {new TH1F("h_subLeadingMuonPt_truth_HT410_Btag",   "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_HT410_Btag           {new TH1F("h_leadingMuonPt_HT410_Btag",            "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_HT410_Btag        {new TH1F("h_subLeadingMuonPt_HT410_Btag",         "Trigger turn-on for signal; p_{T} (GeV); H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_HT410_Btag    {new TH1F("h_leadingMuonEta_truth_HT410_Btag",     "Trigger turn-on for signal; #eta; H_{T} > 410 Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_HT410_Btag {new TH1F("h_subLeadingMuonEta_truth_HT410_Btag",  "Trigger turn-on for signal; #eta; H_{T} > 410 Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_HT410_Btag          {new TH1F("h_leadingMuonEta_HT410_Btag",           "Trigger turn-on for signal; #eta; H_{T} > 410 Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_HT410_Btag       {new TH1F("h_subLeadingMuonEta_HT410_Btag",        "Trigger turn-on for signal; #eta; H_{T} > 410 Btag trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_HT410_Btag              {new TH1F("h_delR_truth_HT410_Btag",               "Trigger turn-on for signal; #Delta R; H_{T} > 410 Btag trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_HT410_Btag                    {new TH1F("h_delR_HT410_Btag",                     "Trigger turn-on for signal; #Delta R; H_{T} > 410 Btag trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_HT410_Btag        {new TH1F("h_diMuonMass_truth_HT410_Btag",         "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_HT410_Btag              {new TH1F("h_diMuonMass_HT410_Btag",               "Trigger turn-on for signal; m_{#mu#mu}; H_{T} > 410 Btag trigger #epsilon", 200, 0., 100.)};
    // numerator DoubleMu + noVtx
    TH1F* h_leadingMuonPt_truth_DoubleMuNoVtxTrig           {new TH1F("h_leadingMuonPt_truth_DoubleMuNoVtxTrig",      "Trigger turn-on for signal; p_{T} (GeV); #mu#mu + no ntx triggers #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_DoubleMuNoVtxTrig        {new TH1F("h_subLeadingMuonPt_truth_DoubleMuNoVtxTrig",   "Trigger turn-on for signal; p_{T} (GeV); #mu#mu + no ntx triggers #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_DoubleMuNoVtxTrig                 {new TH1F("h_leadingMuonPt_DoubleMuNoVtxTrig",            "Trigger turn-on for signal; p_{T} (GeV); #mu#mu + no ntx triggers #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_DoubleMuNoVtxTrig              {new TH1F("h_subLeadingMuonPt_DoubleMuNoVtxTrig",         "Trigger turn-on for signal; p_{T} (GeV); #mu#mu + no ntx triggers #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_DoubleMuNoVtxTrig          {new TH1F("h_leadingMuonEta_truth_DoubleMuNoVtxTrig",     "Trigger turn-on for signal; #eta; #mu#mu + no ntx triggers #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_DoubleMuNoVtxTrig       {new TH1F("h_subLeadingMuonEta_truth_DoubleMuNoVtxTrig",  "Trigger turn-on for signal; #eta; #mu#mu + no ntx triggers #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_DoubleMuNoVtxTrig                {new TH1F("h_leadingMuonEta_DoubleMuNoVtxTrig",           "Trigger turn-on for signal; #eta; #mu#mu + no ntx triggers #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_DoubleMuNoVtxTrig             {new TH1F("h_subLeadingMuonEta_DoubleMuNoVtxTrig",        "Trigger turn-on for signal; #eta; #mu#mu + no ntx triggers #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_DoubleMuNoVtxTrig                    {new TH1F("h_delR_truth_DoubleMuNoVtxTrig",               "Trigger turn-on for signal; #Delta R; #mu#mu + no ntx triggers #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_DoubleMuNoVtxTrig                          {new TH1F("h_delR_DoubleMuNoVtxTrig",                     "Trigger turn-on for signal; #Delta R; #mu#mu + no ntx triggers #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_DoubleMuNoVtxTrig              {new TH1F("h_diMuonMass_truth_DoubleMuNoVtxTrig",         "Trigger turn-on for signal; m_{#mu#mu}; OR all #mu trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_DoubleMuNoVtxTrig                    {new TH1F("h_diMuonMass_DoubleMuNoVtxTrig",               "Trigger turn-on for signal; m_{#mu#mu}; OR all #mu trigger #epsilon", 200, 0., 100.)};

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
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "/"};
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

        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        totalEvents += numberOfEvents;
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");

            event.GetEntry(i);

            float eventWeight = 1.;

            if (!event.metFilters()) continue;

            const bool pass_HT250_Calo (pass_DST_HT250_Calo(event));
            const bool pass_HT250_CaloBTag (pass_DST_HT250_CaloBtag(event));
            const bool pass_HT410_PF (pass_DST_HT410_PF(event));
            const bool pass_HT410_Btag (pass_DST_HT410_Btag(event));
            const bool pass_DoubleMu_noNtx (pass_DST_DoubleMu_noVtx(event));

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
                h_delR_truth->Fill(delR_truth);
                h_delR->Fill(delR);
                h_diMuonMass_truth->Fill(mass_truth);
                h_diMuonMass->Fill(mass);

                // Fill pT post trigger (with and without scalar parentage)
                if (pass_HT250_Calo) {
                    h_leadingMuonPt_truth_HT250_CaloTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_HT250_CaloTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_HT250_CaloTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_HT250_CaloTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_HT250_CaloTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_HT250_CaloTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_HT250_CaloTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_HT250_CaloTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_HT250_CaloTrig->Fill(delR_truth);
                    h_delR_HT250_CaloTrig->Fill(delR);
                    h_diMuonMass_truth_HT250_CaloTrig->Fill(mass_truth);
                    h_diMuonMass_HT250_CaloTrig->Fill(mass);
                }
                if (pass_HT250_CaloBTag) {
                    h_leadingMuonPt_truth_HT250_CaloBtagTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_HT250_CaloBtagTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_HT250_CaloBtagTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_HT250_CaloBtagTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_HT250_CaloBtagTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_HT250_CaloBtagTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_HT250_CaloBtagTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_HT250_CaloBtagTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_HT250_CaloBtagTrig->Fill(delR_truth);
                    h_delR_HT250_CaloBtagTrig->Fill(delR);
                    h_diMuonMass_truth_HT250_CaloBtagTrig->Fill(mass_truth);
                    h_diMuonMass_HT250_CaloBtagTrig->Fill(mass);
                }
                if (pass_HT410_PF) {
                    h_leadingMuonPt_truth_HT410_PF->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_HT410_PF->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_HT410_PF->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_HT410_PF->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_HT410_PF->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_HT410_PF->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_HT410_PF->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_HT410_PF->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_HT410_PF->Fill(delR_truth);
                    h_delR_HT410_PF->Fill(delR);
                    h_diMuonMass_truth_HT410_PF->Fill(mass_truth);
                    h_diMuonMass_HT410_PF->Fill(mass);
                }
                if (pass_HT410_Btag) {
                    h_leadingMuonPt_truth_HT410_Btag->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_HT410_Btag->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_HT410_Btag->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_HT410_Btag->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_HT410_Btag->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_HT410_Btag->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_HT410_Btag->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_HT410_Btag->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_HT410_Btag->Fill(delR_truth);
                    h_delR_HT410_Btag->Fill(delR);
                    h_diMuonMass_truth_HT410_Btag->Fill(mass_truth);
                    h_diMuonMass_HT410_Btag->Fill(mass);
                }
                if (pass_DoubleMu_noNtx) {
                    h_leadingMuonPt_truth_DoubleMuNoVtxTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_DoubleMuNoVtxTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_DoubleMuNoVtxTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_DoubleMuNoVtxTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_DoubleMuNoVtxTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_DoubleMuNoVtxTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_DoubleMuNoVtxTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_DoubleMuNoVtxTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_DoubleMuNoVtxTrig->Fill(delR_truth);
                    h_delR_DoubleMuNoVtxTrig->Fill(delR);
                    h_diMuonMass_truth_DoubleMuNoVtxTrig->Fill(mass_truth);
                    h_diMuonMass_DoubleMuNoVtxTrig->Fill(mass);
                }
            }

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_leadingMuonPt_truth_HT250_CaloTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_HT250_CaloTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_HT250_CaloTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_HT250_CaloTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_HT250_CaloTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_HT250_CaloTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_HT250_CaloTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_HT250_CaloTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_HT250_CaloTrig->Divide(h_delR_truth);
    h_delR_HT250_CaloTrig->Divide(h_delR);
    h_diMuonMass_truth_HT250_CaloTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_HT250_CaloTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_HT250_CaloBtagTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_HT250_CaloBtagTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_HT250_CaloBtagTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_HT250_CaloBtagTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_HT250_CaloBtagTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_HT250_CaloBtagTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_HT250_CaloBtagTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_HT250_CaloBtagTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_HT250_CaloBtagTrig->Divide(h_delR_truth);
    h_delR_HT250_CaloBtagTrig->Divide(h_delR);
    h_diMuonMass_truth_HT250_CaloBtagTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_HT250_CaloBtagTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_HT410_PF->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_HT410_PF->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_HT410_PF->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_HT410_PF->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_HT410_PF->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_HT410_PF->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_HT410_PF->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_HT410_PF->Divide(h_subLeadingMuonEta);
    h_delR_truth_HT410_PF->Divide(h_delR_truth);
    h_delR_HT410_PF->Divide(h_delR);
    h_diMuonMass_truth_HT410_PF->Divide(h_diMuonMass_truth);
    h_diMuonMass_HT410_PF->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_HT410_Btag->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_HT410_Btag->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_HT410_Btag->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_HT410_Btag->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_HT410_Btag->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_HT410_Btag->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_HT410_Btag->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_HT410_Btag->Divide(h_subLeadingMuonEta);
    h_delR_truth_HT410_Btag->Divide(h_delR_truth);
    h_delR_HT410_Btag->Divide(h_delR);
    h_diMuonMass_truth_HT410_Btag->Divide(h_diMuonMass_truth);
    h_diMuonMass_HT410_Btag->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_DoubleMuNoVtxTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_DoubleMuNoVtxTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_DoubleMuNoVtxTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_DoubleMuNoVtxTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_DoubleMuNoVtxTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_DoubleMuNoVtxTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_DoubleMuNoVtxTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_DoubleMuNoVtxTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_DoubleMuNoVtxTrig->Divide(h_delR_truth);
    h_delR_DoubleMuNoVtxTrig->Divide(h_delR);
    h_diMuonMass_truth_DoubleMuNoVtxTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_DoubleMuNoVtxTrig->Divide(h_diMuonMass);

    h_leadingMuonPt_truth_HT250_CaloTrig->Write();
    h_subLeadingMuonPt_truth_HT250_CaloTrig->Write();
    h_leadingMuonPt_HT250_CaloTrig->Write();
    h_subLeadingMuonPt_HT250_CaloTrig->Write();
    h_leadingMuonEta_truth_HT250_CaloTrig->Write();
    h_subLeadingMuonEta_truth_HT250_CaloTrig->Write();
    h_leadingMuonEta_HT250_CaloTrig->Write();
    h_subLeadingMuonEta_HT250_CaloTrig->Write();
    h_delR_truth_HT250_CaloTrig->Write();
    h_delR_HT250_CaloTrig->Write();
    h_diMuonMass_truth_HT250_CaloTrig->Write();
    h_diMuonMass_HT250_CaloTrig->Write();
    h_leadingMuonPt_truth_HT250_CaloBtagTrig->Write();
    h_subLeadingMuonPt_truth_HT250_CaloBtagTrig->Write();
    h_leadingMuonPt_HT250_CaloBtagTrig->Write();
    h_subLeadingMuonPt_HT250_CaloBtagTrig->Write();
    h_leadingMuonEta_truth_HT250_CaloBtagTrig->Write();
    h_subLeadingMuonEta_truth_HT250_CaloBtagTrig->Write();
    h_leadingMuonEta_HT250_CaloBtagTrig->Write();
    h_subLeadingMuonEta_HT250_CaloBtagTrig->Write();
    h_delR_truth_HT250_CaloBtagTrig->Write();
    h_delR_HT250_CaloBtagTrig->Write();
    h_diMuonMass_truth_HT250_CaloBtagTrig->Write();
    h_diMuonMass_HT250_CaloBtagTrig->Write();

    h_leadingMuonPt_truth_HT410_PF->Write();
    h_subLeadingMuonPt_truth_HT410_PF->Write();
    h_leadingMuonPt_HT410_PF->Write();
    h_subLeadingMuonPt_HT410_PF->Write();
    h_leadingMuonEta_truth_HT410_PF->Write();
    h_subLeadingMuonEta_truth_HT410_PF->Write();
    h_leadingMuonEta_HT410_PF->Write();
    h_subLeadingMuonEta_HT410_PF->Write();
    h_delR_truth_HT410_PF->Write();
    h_delR_HT410_PF->Write();
    h_diMuonMass_truth_HT410_PF->Write();
    h_diMuonMass_HT410_PF->Write();
    h_leadingMuonPt_truth_HT410_Btag->Write();
    h_subLeadingMuonPt_truth_HT410_Btag->Write();
    h_leadingMuonPt_HT410_Btag->Write();
    h_subLeadingMuonPt_HT410_Btag->Write();
    h_leadingMuonEta_truth_HT410_Btag->Write();
    h_subLeadingMuonEta_truth_HT410_Btag->Write();
    h_leadingMuonEta_HT410_Btag->Write();
    h_subLeadingMuonEta_HT410_Btag->Write();
    h_delR_truth_HT410_Btag->Write();
    h_delR_HT410_Btag->Write();
    h_diMuonMass_truth_HT410_Btag->Write();
    h_diMuonMass_HT410_Btag->Write();
    h_leadingMuonPt_truth_DoubleMuNoVtxTrig->Write();
    h_subLeadingMuonPt_truth_DoubleMuNoVtxTrig->Write();
    h_leadingMuonPt_DoubleMuNoVtxTrig->Write();
    h_subLeadingMuonPt_DoubleMuNoVtxTrig->Write();
    h_leadingMuonEta_truth_DoubleMuNoVtxTrig->Write();
    h_subLeadingMuonEta_truth_DoubleMuNoVtxTrig->Write();
    h_leadingMuonEta_DoubleMuNoVtxTrig->Write();
    h_subLeadingMuonEta_DoubleMuNoVtxTrig->Write();
    h_delR_truth_DoubleMuNoVtxTrig->Write();
    h_delR_DoubleMuNoVtxTrig->Write();
    h_diMuonMass_truth_DoubleMuNoVtxTrig->Write();
    h_diMuonMass_DoubleMuNoVtxTrig->Write();

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

bool pass_DST_HT250_Calo ( AnalysisEvent& event ) {
    if ( event.DST_HT250_CaloScouting_v6 > 0 
      || event.DST_HT250_CaloScouting_v7 > 0
      || event.DST_HT250_CaloScouting_v8 > 0
      || event.DST_HT250_CaloScouting_v9 > 0 ) return true;
    else return false;
}
bool pass_DST_HT250_CaloBtag ( AnalysisEvent& event ) {
    if ( event.DST_HT250_CaloBTagScouting_v5 > 0
      || event.DST_HT250_CaloBTagScouting_v6 > 0
      || event.DST_HT250_CaloBTagScouting_v7 > 0
      || event.DST_HT250_CaloBTagScouting_v8 > 0 ) return true;
    else return false;
}
bool pass_DST_HT410_PF ( AnalysisEvent& event ) {
    if ( event.DST_HT410_PFScouting_v9 > 0 
      || event.DST_HT410_PFScouting_v10 > 0
      || event.DST_HT410_PFScouting_v11 > 0
      || event.DST_HT410_PFScouting_v12 > 0
      || event.DST_HT410_PFScouting_v13 > 0
      || event.DST_HT410_PFScouting_v14 > 0 ) return true;
    else return false;
}
bool pass_DST_HT410_Btag ( AnalysisEvent& event ) {
    if ( event.DST_HT410_BTagScouting_v10 > 0
      || event.DST_HT410_BTagScouting_v11 > 0 
      || event.DST_HT410_BTagScouting_v12 > 0 
      || event.DST_HT410_BTagScouting_v13 > 0 
      || event.DST_HT410_BTagScouting_v14 > 0 ) return true;
    else return false;
}
bool pass_DST_DoubleMu_noVtx ( AnalysisEvent& event ) {
    if ( event.DST_DoubleMu3_noVtx_CaloScouting_v2 > 0 
      || event.DST_DoubleMu3_noVtx_CaloScouting_v3 > 0
      || event.DST_DoubleMu3_noVtx_CaloScouting_v4 > 0
      || event.DST_DoubleMu3_noVtx_CaloScouting_v5 > 0 ) return true;
    else return false;
}

