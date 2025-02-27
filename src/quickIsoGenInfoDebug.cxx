#include "AnalysisEvent.hpp"
#include "TChain.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
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

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const bool invertCharge);
bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chsIndex, const bool invertCharge);
int getMuonTrackPairIndex(const AnalysisEvent& event);
int getChsTrackPairIndex(const AnalysisEvent& event);
float deltaR(float eta1, float phi1, float eta2, float phi2);

std::string pdgIdCode (const Int_t status, const bool unicode = false);

bool is2016_;
bool is2018_;

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.};
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
    bool mcTruth_ {false};
    bool normalise_ {true};
   
    std::string outFileString{"plots/distributions/output.root"};
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Quick and dirty plots

    std::map<int, int> pdgIdMap;

    TH1I* h_leadingMatchedGenMuonNgenJets         {new TH1I("h_leadingMatchedGenMuonNgenJets",       "", 10, -0.5, 9.5)};
    TH1I* h_subleadingMatchedGenMuonNgenJets      {new TH1I("h_subleadingMatchedGenMuonNgenJets",    "", 10, -0.5, 9.5)};

    TH1I* h_leadingMatchedGenMuonNoIsoNgenJets    {new TH1I("h_leadingMatchedGenMuonIsoNgenJets",    "", 10, -0.5, 9.5)};
    TH1I* h_subleadingMatchedGenMuonNoIsoNgenJets {new TH1I("h_subleadingMatchedGenMuonIsoNgenJets", "", 10, -0.5, 9.5)};

    TH1F* h_leadingMatchedGenMuonNoIsoJetMuEnergyFraction               {new TH1F("h_leadingMatchedGenMuonNoIsoJetMuEnergyFraction",               "", 100, 0., 1.)};
    TH1F* h_leadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction    {new TH1F("h_leadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction",    "", 100, 0., 1.)};
    TH1F* h_leadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction    {new TH1F("h_leadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction",    "", 100, 0., 1.)};
    TH1F* h_leadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction        {new TH1F("h_leadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction",        "", 100, 0., 1.)};
    TH1F* h_leadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction        {new TH1F("h_leadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction",        "", 100, 0., 1.)};

    TH1F* h_subleadingMatchedGenMuonNoIsoJetMuEnergyFraction            {new TH1F("h_subleadingMatchedGenMuonNoIsoJetMuEnergyFraction",            "", 100, 0., 1.)};
    TH1F* h_subleadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction {new TH1F("h_subleadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction", "", 100, 0., 1.)};
    TH1F* h_subleadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction {new TH1F("h_subleadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction", "", 100, 0., 1.)};
    TH1F* h_subleadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction     {new TH1F("h_subleadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction",     "", 100, 0., 1.)};
    TH1F* h_subleadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction     {new TH1F("h_subleadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction",     "", 100, 0., 1.)};

    TH1F* h_leadingMuonNoIsoLeadingGenJetPid                            {new TH1F("h_leadingMuonNoIsoLeadingGenJetPid",                            "; genJet PID;", 1000, 0., 1000.)};
    TH1F* h_leadingMuonNoIsoLeadingGenJetPt                             {new TH1F("h_leadingMuonNoIsoLeadingGenJetPt",                             "; genJet p_{T};", 500, 0., 250.)};
    TH1F* h_leadingMuonNoIsoLeadingGenJetPfCandChargedContribution      {new TH1F("h_leadingMuonNoIsoLeadingGenJetPfCandChargedContribution",      ";#sum_{p_{T}}^{#mu1} charged contribution;", 200, 0., 100.)};
    TH1F* h_leadingMuonNoIsoLeadingGenJetPfCandNeutralContribution      {new TH1F("h_leadingMuonNoIsoLeadingGenJetPfCandNeutralContribution",      ";#sum_{E_{T}}^{#mu1} neutral contribution;", 200, 0., 100.)};
    TH1F* h_leadingMuonNoIsoLeadingGenJetPfCandPuContribution           {new TH1F("h_leadingMuonNoIsoLeadingGenJetPfCandPuContribution",           ";#frac{1}{2}#times#sum_{p_{T}}^{#mu1} PU;", 200, 0., 100.)};

    TH1F* h_subleadingMuonNoIsoLeadingGenJetPid                         {new TH1F("h_subleadingMuonNoIsoLeadingGenJetPid",                         "; genJet PID;", 1000, 0., 1000.)};
    TH1F* h_subleadingMuonNoIsoLeadingGenJetPt                          {new TH1F("h_subleadingMuonNoIsoLeadingGenJetPt",                          "; genJet p_{T};", 500, 0., 250.)};
    TH1F* h_subleadingMuonNoIsoLeadingGenJetPfCandChargedContribution   {new TH1F("h_subleadingMuonNoIsoLeadingGenJetPfCandChargedContribution",   ";#sum_{p_{T}}^{#mu2} charged contribution;", 200, 0., 100.)};
    TH1F* h_subleadingMuonNoIsoLeadingGenJetPfCandNeutralContribution   {new TH1F("h_subleadingMuonNoIsoLeadingGenJetPfCandNeutralContribution",   ";#sum_{E_{T}}^{#mu2} neutral contribution;", 200, 0., 100.)};
    TH1F* h_subleadingMuonNoIsoLeadingGenJetPfCandPuContribution        {new TH1F("h_subleadingMuonNoIsoLeadingGenJetPfCandPuContribution",        ";#frac{1}{2}#times#sum_{p_{T}}^{#mu2} PU;", 200, 0., 100.)};

    TH1I* h_leadingMuonNoIsoLeadingGenJetPfCandId                       {new TH1I("h_leadingMuonNoIsoLeadingGenJetPfCandId",                       "; pfCand (assoc with genJet) PID;", 8, 0., 8)};
    TH1I* h_subleadingMuonNoIsoLeadingGenJetPfCandId                    {new TH1I("h_subleadingMuonNoIsoLeadingGenJetPfCandId",                    "; pfCand (assoc with genJet) PID;", 8, 0., 8)};

    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(1, "Unknown");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(2, "ele");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(3, "mu");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(4, "gamma");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(5, "charged hadrons");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(6, "neutral hadrons");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(7, "hadronic HF");
    h_leadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(8, "em HF");

    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(1, "Unknown");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(2, "ele");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(3, "mu");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(4, "gamma");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(5, "charged hadrons");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(6, "neutral hadrons");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(7, "hadronic HF");
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->GetXaxis()->SetBinLabel(8, "em HF");


    TProfile* p_leadingMuonIso            {new TProfile ("p_leadingMuonIso", "Leading muon iso cuts", 8, 0.5, 8.5, "ymax = 1.0")};
    p_leadingMuonIso->GetXaxis()->SetBinLabel(1, "PF Iso Very Loose (40%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(2, "PF Iso Loose (25%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(3, "PF Iso Medium (20%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(4, "PF Iso Tight (15%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(5, "PF Iso Very Tight (10%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(6, "PF Iso Very Very Tight (5%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(7, "Tk Iso Loose (10%)");
    p_leadingMuonIso->GetXaxis()->SetBinLabel(8, "Tk Iso Tight (5%)");

    TProfile* p_subleadingMuonIso         {new TProfile ("p_subleadingMuonIso", "Subleading muon iso cuts", 8, 0.5, 8.5, "ymax = 1.0")};
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(1, "PF Iso Very Loose (40%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(2, "PF Iso Loose (25%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(3, "PF Iso Medium (20%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(4, "PF Iso Tight (15%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(5, "PF Iso Very Tight (10%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(6, "PF Iso Very Very Tight (5%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(7, "Tk Iso Loose (10%)");
    p_subleadingMuonIso->GetXaxis()->SetBinLabel(8, "Tk Iso Tight (5%)");

    TProfile* p_numPromptMuons            {new TProfile ("p_numPromptMuons", "Muon flags for selected reco muons", 6, 0.5, 6.5, "ymax = 1.0")};
    p_numPromptMuons->GetXaxis()->SetBinLabel(1, "Prompt Decayed");
    p_numPromptMuons->GetXaxis()->SetBinLabel(2, "Prompt Final State");
    p_numPromptMuons->GetXaxis()->SetBinLabel(3, "Hard Process");
    p_numPromptMuons->GetXaxis()->SetBinLabel(4, "Likely Pythia6 Status 3");
    p_numPromptMuons->GetXaxis()->SetBinLabel(5, "ScalarAncestor");
    p_numPromptMuons->GetXaxis()->SetBinLabel(6, "DirectScalarAncestor");

    TH1F* h_promptMuonMotherId            {new TH1F("h_promptMuonMotherId", "; Prompt Final State Muon Mothers' PID;", 1000, 0., 1000.)};

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(41528.0),
        "Lumi to scale MC plots to.")(
        "norm, k",
        po::bool_switch(&normalise_),
        "Do not normalise wrt. cross section and lumi")(
        "outfile,o",
        po::value<std::string>(&outFileString)->default_value(outFileString),
        "Output file for plots.")(
        "mcTruth,m",
        po::bool_switch(&mcTruth_),
        "Use MC truth to select particles")(
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

    if (totalLumi == 0.) {
        totalLumi = usePreLumi;
    }
    std::cout << "Using lumi: " << totalLumi << std::endl;

    bool datasetFilled{false};
    std::string era {""};
    if (is2016_) era = "2016";
    else if (is2018_) era = "2018";
    else era = "2017";
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};

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

            if ( normalise_) eventWeight *= datasetWeight;

            // Trigger + met filters
            if (!event.muTrig()) continue;
            if (!event.metFilters()) continue;

            // Get muons
            std::vector<int> looseMuonIndex = getLooseMuons(event);
            if ( looseMuonIndex.size() < 2 ) continue;


            if ( getDileptonCand(event, looseMuonIndex, false) ) {

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

                if ( event.genMuonPF2PATPromptFinalState[event.zPairIndex.first] )  h_promptMuonMotherId->Fill( std::abs( event.genMuonPF2PATMotherId[event.zPairIndex.first] ) );
                if ( event.genMuonPF2PATPromptFinalState[event.zPairIndex.second] ) h_promptMuonMotherId->Fill( std::abs( event.genMuonPF2PATMotherId[event.zPairIndex.second] ) );

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


                const int muIndex1 {event.zPairIndex.first}, muIndex2{event.zPairIndex.second};
                TLorentzVector muonVec1 {event.genMuonPF2PATPX[muIndex1], event.genMuonPF2PATPY[muIndex1], event.genMuonPF2PATPZ[muIndex1], event.muonPF2PATE[muIndex1]};
                TLorentzVector muonVec2 {event.genMuonPF2PATPX[muIndex2], event.genMuonPF2PATPY[muIndex2], event.genMuonPF2PATPZ[muIndex2], event.muonPF2PATE[muIndex2]};	

                unsigned int nJets1 {0}, nJets2 {0}, nJetsNoIso1 {0}, nJetsNoIso2 {0};

                for (unsigned int i{0}; i < event.numJetPF2PAT; i++) {
                    TLorentzVector jetLVec {event.genJetPF2PATPX[i], event.genJetPF2PATPY[i], event.genJetPF2PATPZ[i], event.genJetPF2PATE[i]};
                    if ( jetLVec.DeltaR(muonVec1) < 0.4 ) {
                        nJets1++;
                        if ( event.zPairRelIso.first == 0.0 ) {

                            if ( nJetsNoIso1 == 0 ) {
                                h_leadingMuonNoIsoLeadingGenJetPt->Fill(jetLVec.Pt(), eventWeight);

                                h_leadingMuonNoIsoLeadingGenJetPid->Fill(std::abs(event.genJetPF2PATPID[i]), eventWeight);

      	       	       	        for (unsigned int j{0}; j < event.numPackedCands; j++) {
                                    if ( j == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] || j == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
                                    if ( event.packedCandsJetIndex[j] != i ) continue;

                                    const int pfPid {std::abs(event.packedCandsPdgId[j])};
                                    if ( pfPid == 11 )       h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(2, eventWeight);
                                    else if ( pfPid == 13 )  h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(3, eventWeight);
                                    else if ( pfPid == 22 )  h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(4, eventWeight);
                                    else if ( pfPid == 211 ) h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(5, eventWeight);
                                    else if ( pfPid == 130 ) h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(6, eventWeight);
                                    else if ( pfPid == 1 )   h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(7, eventWeight);
                                    else if ( pfPid == 2 )   h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(8, eventWeight);
                                    else h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(1, eventWeight);

                                    TLorentzVector pfCandVec {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
                                    if ( event.packedCandsCharge[j] != 0 && event.packedCandsFromPV[j] >= 2 ) h_leadingMuonNoIsoLeadingGenJetPfCandChargedContribution->Fill(pfCandVec.Pt(), eventWeight);
                                    if ( event.packedCandsCharge[j] == 0 && pfCandVec.Pt() > 0.5 )            h_leadingMuonNoIsoLeadingGenJetPfCandNeutralContribution->Fill(pfCandVec.Pt(), eventWeight);
                                    if ( event.packedCandsCharge[j] != 0 && event.packedCandsFromPV[j] < 2 && pfCandVec.Pt() > 0.5 )  h_leadingMuonNoIsoLeadingGenJetPfCandPuContribution->Fill(0.5*pfCandVec.Pt(), eventWeight);
                                }
                            }

                            nJetsNoIso1++;

                            h_leadingMatchedGenMuonNoIsoJetMuEnergyFraction->Fill(event.jetPF2PATMuonFractionCorr[i], eventWeight);
                            h_leadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction->Fill(event.jetPF2PATNeutralHadronEnergyFractionCorr[i], eventWeight);
                            h_leadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction->Fill(event.jetPF2PATChargedHadronEnergyFractionCorr[i], eventWeight);
                            h_leadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction->Fill(event.jetPF2PATNeutralEmEnergyFractionCorr[i], eventWeight);
                            h_leadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction->Fill(event.jetPF2PATChargedEmEnergyFractionCorr[i], eventWeight);
                    }
                    if ( jetLVec.DeltaR(muonVec2) < 0.4 ) {
                        nJets2++;
                        if ( event.zPairRelIso.second == 0.0 ) {

                            if ( nJetsNoIso2 == 0 ) {
                                h_subleadingMuonNoIsoLeadingGenJetPt->Fill(jetLVec.Pt(), eventWeight);
                                h_subleadingMuonNoIsoLeadingGenJetPid->Fill(std::abs(event.genJetPF2PATPID[i]), eventWeight);
                                for (unsigned int j{0}; j < event.numPackedCands; j++) {
                                    if ( j == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] || j == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;
                                    if ( event.packedCandsJetIndex[j] != i ) continue;
                                    const int pfPid {std::abs(event.packedCandsPdgId[j])};

                                    if ( pfPid == 11 )       h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(2, eventWeight);
                                    else if ( pfPid == 13 )  h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(3, eventWeight);
                                    else if ( pfPid == 22 )  h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(4, eventWeight);
                                    else if ( pfPid == 211 ) h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(5, eventWeight);
                                    else if ( pfPid == 130 ) h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(6, eventWeight);
                                    else if ( pfPid == 1 )   h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(7, eventWeight);
                                    else if ( pfPid == 2 )   h_subleadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(8, eventWeight);
                                    else h_leadingMuonNoIsoLeadingGenJetPfCandId->AddBinContent(1, eventWeight);

       	       	       	       	    TLorentzVector pfCandVec {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
                                    if ( event.packedCandsCharge[j] != 0 && event.packedCandsFromPV[j] >= 2 ) h_subleadingMuonNoIsoLeadingGenJetPfCandChargedContribution->Fill(pfCandVec.Pt(), eventWeight);
                                    if ( event.packedCandsCharge[j] == 0 && pfCandVec.Pt() > 0.5 )            h_subleadingMuonNoIsoLeadingGenJetPfCandNeutralContribution->Fill(pfCandVec.Pt(), eventWeight);
                                    if ( event.packedCandsCharge[j] != 0 && event.packedCandsFromPV[j] < 2 && pfCandVec.Pt() > 0.5 )  h_subleadingMuonNoIsoLeadingGenJetPfCandPuContribution->Fill(0.5*pfCandVec.Pt(), eventWeight);
                                }
                            }

                            nJetsNoIso2++;

                            h_subleadingMatchedGenMuonNoIsoJetMuEnergyFraction->Fill(event.jetPF2PATMuonFractionCorr[i], eventWeight);
                            h_subleadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction->Fill(event.jetPF2PATNeutralHadronEnergyFractionCorr[i], eventWeight);
                            h_subleadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction->Fill(event.jetPF2PATChargedHadronEnergyFractionCorr[i], eventWeight);
                            h_subleadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction->Fill(event.jetPF2PATNeutralEmEnergyFractionCorr[i], eventWeight);
                            h_subleadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction->Fill(event.jetPF2PATChargedEmEnergyFractionCorr[i], eventWeight);
                    }
                        }
                    }
                } // end jet loop

                h_leadingMatchedGenMuonNgenJets->Fill(nJets1, eventWeight);
                h_subleadingMatchedGenMuonNgenJets->Fill(nJets2, eventWeight);
                if ( event.zPairRelIso.first == 0.0 )  h_leadingMatchedGenMuonNoIsoNgenJets->Fill(nJetsNoIso1, eventWeight);
                if ( event.zPairRelIso.second == 0.0 ) h_subleadingMatchedGenMuonNoIsoNgenJets->Fill(nJetsNoIso2, eventWeight);

            } // End dimuon bool

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Write out histos

    p_numPromptMuons->Write();
    h_promptMuonMotherId->Write();

    p_leadingMuonIso->Write();
    p_subleadingMuonIso->Write();

    h_leadingMatchedGenMuonNgenJets->Write();
    h_subleadingMatchedGenMuonNgenJets->Write();

    h_leadingMatchedGenMuonNoIsoNgenJets->Write();
    h_subleadingMatchedGenMuonNoIsoNgenJets->Write();

    h_leadingMatchedGenMuonNoIsoJetMuEnergyFraction->Write();
    h_leadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction->Write();
    h_leadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction->Write();
    h_leadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction->Write();
    h_leadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction->Write();

    h_subleadingMatchedGenMuonNoIsoJetMuEnergyFraction->Write();
    h_subleadingMatchedGenMuonNoIsoJetNeutralHadronEnergyFraction->Write();
    h_subleadingMatchedGenMuonNoIsoJetChargedHadronEnergyFraction->Write();
    h_subleadingMatchedGenMuonNoIsoJetNeutralEmEnergyFraction->Write();
    h_subleadingMatchedGenMuonNoIsoJetChargedEmEnergyFraction->Write();

    h_leadingMuonNoIsoLeadingGenJetPid->Write();
    h_leadingMuonNoIsoLeadingGenJetPfCandId->Write();
    h_leadingMuonNoIsoLeadingGenJetPt->Write();
    h_leadingMuonNoIsoLeadingGenJetPfCandChargedContribution->Write();
    h_leadingMuonNoIsoLeadingGenJetPfCandNeutralContribution->Write();
    h_leadingMuonNoIsoLeadingGenJetPfCandPuContribution->Write();

    h_subleadingMuonNoIsoLeadingGenJetPid->Write();
    h_subleadingMuonNoIsoLeadingGenJetPfCandId->Write();
    h_subleadingMuonNoIsoLeadingGenJetPt->Write();
    h_subleadingMuonNoIsoLeadingGenJetPfCandChargedContribution->Write();
    h_subleadingMuonNoIsoLeadingGenJetPfCandNeutralContribution->Write();
    h_subleadingMuonNoIsoLeadingGenJetPfCandPuContribution->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

std::vector<int> getLooseMuons(const AnalysisEvent& event) {
    std::vector<int> muons;
    if (is2016_) {
        for (int i{0}; i < event.numMuonPF2PAT; i++) {
            if (!event.muonPF2PATIsPFMuon[i]) continue;

            if (muons.size() < 1 && event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
            else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= looseMuonPt_) continue;

            if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;
            else if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;

            if (event.muonPF2PATComRelIsodBeta[i] >= looseMuonRelIso_)  continue;
            if (event.muonPF2PATGlobalID[i] || event.muonPF2PATTrackID[i])  muons.emplace_back(i);
        }
    }
    else {
        for (int i{0}; i < event.numMuonPF2PAT; i++)  {
            if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] ) {
//            if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATTightCutId[i] ) {

                if (muons.size() < 1 && event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
                else if (muons.size() >= 1 && event.muonPF2PATPt[i] <= looseMuonPt_) continue;

                if (muons.size() < 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;
                else if (muons.size() >= 1 && std::abs(event.muonPF2PATEta[i]) >= looseMuonEta_) continue;

                muons.emplace_back(i);
            }
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
        if (lVec.Pt() < 1.0) continue;

        chs.emplace_back(k);
    }
    return chs;
}

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const bool invertCharge = false) {

    for ( unsigned int i{0}; i < muons.size(); i++ ) {
        for ( unsigned int j{i+1}; j < muons.size(); j++ ) {

            if (event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue;
            if (event.muonPF2PATPt[j] <= looseMuonPt_) continue;

            if ( !invertCharge ) {
                if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue;
            }
            else {
                if (! (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) ) continue;
            }

            TLorentzVector lepton1{event.muonPF2PATPX[muons[i]], event.muonPF2PATPY[muons[i]], event.muonPF2PATPZ[muons[i]], event.muonPF2PATE[muons[i]]};
            TLorentzVector lepton2{event.muonPF2PATPX[muons[j]], event.muonPF2PATPY[muons[j]], event.muonPF2PATPZ[muons[j]], event.muonPF2PATE[muons[j]]};
            const double delR { lepton1.DeltaR(lepton2) };
//            if ( delR < maxDileptonDeltaR_  ) {
            if ( delR > -1.0  ) {
                event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
                event.zPairIndex.second  = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
                event.zPairRelIso.first  = event.muonPF2PATComRelIsodBeta[muons[i]];
                event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];

                if (!event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first]) continue;
                if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] > 1.0) continue;

//                if (event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first]) continue;
//                if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] <= 1.0) continue;

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

bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chs, const bool invertCharge = false) {

    for ( unsigned int i{0}; i < chs.size(); i++ ) {

        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
        if ( event.packedCandsMuonIndex[chs[i]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        for ( unsigned int j{i+1}; j < chs.size(); j++ ) {

            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            if ( event.packedCandsMuonIndex[chs[j]] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;


            if ( !invertCharge ) {
                if (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] >= 0) continue;
            }
            else {
                if (! (event.packedCandsCharge[chs[i]] * event.packedCandsCharge[chs[j]] >= 0) ) continue;
            }

            TLorentzVector chs1 {event.packedCandsPx[chs[i]], event.packedCandsPy[chs[i]], event.packedCandsPz[chs[i]], event.packedCandsE[chs[i]]};
            TLorentzVector chs2 {event.packedCandsPx[chs[j]], event.packedCandsPy[chs[j]], event.packedCandsPz[chs[j]], event.packedCandsE[chs[j]]};

            double pT { (chs1+chs2).Pt() };
            double delR { chs1.DeltaR(chs2) };
            double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };

            if ( delR < maxChsDeltaR_ ) {
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

                // If refit fails then reject event - all signal events pass refit, but QCD does not
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

float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}

std::string pdgIdCode (const Int_t parId, const bool unicode) {

   std::string particle;
   int id = std::abs(parId);

   switch (id) {
      case 0 : particle += "-"; break;
      case 1 : particle += "d"; break;
      case 2 : particle += "u"; break;
      case 3 : particle += "s"; break;
      case 4 : particle += "c"; break;
      case 5 : particle += "b"; break;
      case 6 : particle += "t"; break;
      case 9 : particle += "g"; break;

      case 11 : particle += "e"; break;
      case 12 : particle += unicode ? "\u03BD_e" : "#nu_{e}" ; break;
      case 13 : particle += unicode ? "\u03BC" : "#mu"; break;
      case 14 : particle += unicode ? "\u03BD_\u03BC" : "#nu_{#mu}"; break;
      case 15 : particle += unicode ? "\u03C4" : "#tau"; break;
      case 16 : particle += unicode ? "\u03BD_\u03C4" : "#nu_{#tau}"; break;

      case 21 : particle += "g"; break;
      case 22 : particle += unicode ? "Y" : "#gamma"; break;
      case 23 : particle += "Z_{0}"; break;
      case 24 : particle += "W"; break;
      case 25 : particle += "h_{0}"; break;

      case 111 : particle += unicode ? "\u03C00" : "#pi^{0}"; break;
      case 211 : particle += unicode ? "\u03C0" : "#pi^{+}"; break;
      case 113 : particle += unicode ? "\u03C1" : "#rho(770)^{0}"; break;
      case 213 : particle += unicode ? "\u03C1" : "#rho(770)^{+}"; break;
      case 225 : particle += unicode ? "f2(1270)" : "f_{2}(1270)"; break;

      case 130 : particle += unicode ? "K0_L" : "K_{L}^{0}"; break;
      case 310 : particle += unicode ? "K0_S" : "K_{S}^{0}"; break;
      case 311 : particle += unicode ? "K0" : "K^{0}"; break;
      case 321 : particle += unicode ? "K" : "K^{+}"; break;

      case 313 : particle += unicode ? "K*0" : "K^{*}(892)^{0}"; break;
      case 315 : particle += unicode ? "K*0_2" : "K^{*}_{2}(1430)"; break;
      case 323 : particle += unicode ? "K*" : "K^{*}(892)^{+}"; break;

      case 10311 : particle += unicode ? "K0*_0(1430)" : "K^{*}_{0}(1430)"; break;

      case 221 : particle += unicode ? "\u03B7" : "#eta"; break;
      case 331 : particle += unicode ? "\u03B7\'" : "#eta'"; break;
      case 223 : particle += unicode ? "\u03C9" : "#omega"; break;
      case 333 : particle += unicode ? "\u03C6" : "#phi"; break;

      case 411 : particle += unicode ? "D" : "D^{+}"; break;
      case 421 : particle += unicode ? "D0" : "D^{0}"; break;
      case 413 : particle += unicode ? "D*" : "D^{*} 2010)^{+}"; break;
      case 423 : particle += unicode ? "D*0" : "D^{*}(2007)^{0}"; break;
      case 431 : particle += unicode ? "D_S" : "D_{s}^{+}"; break;
      case 433 : particle += unicode ? "D*_S" : "D_{s}^{*+}"; break;
      case 443 : particle += unicode ? "J/\u03C8" : "J/#psi"; break;
      case 445 : particle += unicode ? "\u03C7_c2(1P)" : "#chi_{c2}(1P)"; break;
 
      case 511 : particle += unicode ? "B0" : "B^{0}"; break;
      case 521 : particle += unicode ? "B" : "B^{+}"; break;
      case 513 : particle += unicode ? "B*0" : "B^{*0}"; break;
      case 523 : particle += unicode ? "B*" : "B^{*+}"; break;
      case 531 : particle += unicode ? "B0_S" : "B^{0}_{s}"; break;
      case 533 : particle += unicode ? "B*0_S" : "B^{*0}_{s}"; break;

      case 1101 : particle += unicode ? "(dd)0" : "(dd)_{0}"; break;
      case 2101 : particle += unicode ? "(ud)0" : "(ud)_{0}"; break;
      case 2103 : particle += unicode ? "(ud)1" : "(ud)_{1}"; break;
      case 2203 : particle += unicode ? "(uu)1" : "(uu)_{1}"; break;

      case 2212 : particle += "p"; break;
      case 2112 : particle += "n"; break;
      case 2224 : particle += unicode ? "\u0394++" : "#Delta^{++}"; break;
      case 2214 : particle += unicode ? "\u0394+" : "#Delta^{+}"; break;
      case 2114 : particle += unicode ? "\u03940" : "#Delta^{0}"; break;
      case 1114 : particle += unicode ? "\u0394-" : "#Delta^{-}"; break;

      case 3122 : particle += unicode ? "\u0394" : "#Lambda"; break;
      case 3222 : particle += unicode ? "\u03A3" : "#Sigma^{+}"; break;
      case 3224 : particle += unicode ? "\u03A3*" : "#Sigma^{*+}"; break;
      case 3212 : particle += unicode ? "\u03A30" : "#Sigma^{0}"; break;
      case 3322 : particle += unicode ? "\u03A30" : "#Xi^{0}"; break;
      case 3312 : particle += unicode ? "\u03A3-" : "#Xi^{-}"; break;
      case 3112 : particle += unicode ? "\u03A3-" : "#Sigma_{-}"; break;

      case 3324 : particle += unicode ? "\u039E*0" : "#Xi^{*0}"; break;
      case 3334 : particle += unicode ? "\u03A9-" : "#Omega^{-}"; break;

      case 4214 : particle += unicode ? "\u03A3*_C" : "#Sigma_{c}^{*+}"; break;
      case 4222 : particle += unicode ? "\u03A3_C" : "#Sigma_{c}^{++}"; break;
      case 4122 : particle += unicode ? "\u039BC" : "#Lambda_{c}^{+}"; break;
      case 4114 : particle += unicode ? "\u03A3*0_C" : "#Sigma_{c}^{*0}"; break;
      case 4224 : particle += unicode ? "\u03A3+C" : "#Sigma_{c}^{*++}"; break;

      case 5122 : particle += unicode ? "\u039B0_b" : "#Lambda^{0}_{b}"; break;
      case 5212 : particle += unicode ? "\u03A30_b" : "#Sigma_{b}^{0}"; break;
      case 5232 : particle += unicode ? "\u039E0_b" : "#Xi_{b}^{0}"; break;

      case 10313: particle += unicode ? "K0_1 (1270)" : "K_{1} (1270)^{0}"; break;
      case 10441: particle += unicode ? "\u03C7_c0(1P)" : "#chi_{c0}(1P)"; break;
      case 10551: particle += unicode ? "\u03C7_b0(1P)" : "#chi_{b0}(1P)"; break;
      case 20443: particle += unicode ? "\u03C7_c1(1P)" : "#chi_{c1}(1P)"; break;
      case 20313: particle += unicode ? "K0_1 (1400)" : "K_{1}(1400)^0}"; break;
      case 20213: particle += unicode ? "a1" : "a_{1} (1260)^{+}"; break;

      case 9000006 : particle += unicode ? "Scalar" : "SCALAR"; break;
      case 9010221 : particle += unicode ? "f0(980)" : "f_{0}(980)"; break;

      default : {particle += std::to_string(std::abs(parId)); /*std::cout << "UNKNOWN PID: " << parId << std::endl;*/}
   }

   if ( unicode && (parId == 211 || parId == 213 || parId == 321 || parId == 323 || parId == 411 || parId == 431 || parId == 433 || parId == 521 || parId == 4122 || parId == 20213 || parId == 4214 || parId == 523
   || parId == 3224 || parId == 3222
))  particle += "+";

   if ( unicode && parId < 0) particle += "-";
   if ( unicode && (parId == -4222 || parId == -4224) ) particle += "-";
   if ( unicode && (parId == 4222  || parId == 4222) ) particle += "+";

   return particle;
}

