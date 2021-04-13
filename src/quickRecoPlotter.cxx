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
std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt );

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, bool mcTruth = false);
bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chsIndex, bool mcTruth = false);
int getMuonTrackPairIndex(const AnalysisEvent& event);
int getChsTrackPairIndex(const AnalysisEvent& event);
float deltaR(float eta1, float phi1, float eta2, float phi2);

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0}, chsMass_{0.13957018};

// Diparticle cuts
double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4};
double higgsTolerence_ {999.};

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
    bool mcTruth_ {false};
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    TH1F* h_muonPtGenuine           { new TH1F("h_muonPtGenuine",          ";p_{T}", 200, 0., 100.)};
    TH1F* h_muon1PtGenuine          { new TH1F("h_muon1PtGenuine",         ";p_{T}", 200, 0., 100.)};
    TH1F* h_muon2PtGenuine          { new TH1F("h_muon2PtGenuine",         ";p_{T}", 200, 0., 100.)};
    TH1F* h_muonDeltaRGenuine       { new TH1F("h_muonDeltaRGenuine",       "#DeltaR between muons from scalar decay; #DeltaR", 500, 0., 1.)};

    TH1F* h_genMuonPtGenuine        { new TH1F("h_genMuonPtGenuine",          ";p_{T}", 200, 0., 100.)};
    TH1F* h_genMuon1PtGenuine       { new TH1F("h_genMuon1PtGenuine",         ";p_{T}", 200, 0., 100.)};
    TH1F* h_genMuon2PtGenuine       { new TH1F("h_genMuon2PtGenuine",         ";p_{T}", 200, 0., 100.)};
    TH1F* h_genMuonDeltaRGenuine    { new TH1F("h_genMuonDeltaRGenuine",      "#DeltaR between muons from scalar decay; #DeltaR", 500, 0., 1.)};

    // Quick and dirty plots
    TH1F* h_leadingMuonPt        { new TH1F("h_leadingMuonPt",          ";p_{T}", 200, 0., 100.)};
    TH1F* h_subleadingMuonPt     { new TH1F("h_subleadingMuonPt",       ";p_{T}", 200, 0., 100.)};
    TH1F* h_muonDeltaR           { new TH1F("h_muonDeltaR",             ";#Delta R", 500, 0., 1.)}; 
    TH1F* h_dimuonPt             { new TH1F("h_dimuonPt",               ";p_{T}", 400, 0., 200.)};
    TH1F* h_muonPtOverDeltaR     { new TH1F("h_muonPtOverDeltaR",       "p_{T}/#Delta R", 4000, 0., 10.)};
    TH2F* h_muonPtOverDeltaR2D   { new TH2F("h_muonPtOverDeltaR2D",     "", 200, 0., 100., 500, 0., 5.)};
    TH1F* h_diMuonMass           { new TH1F("h_diMuonMass",             ";Mass", 100, 0., 4.0)};
    TH1F* h_diMuonRefittedMass   { new TH1F("h_diMuonRefittedMass",     ";Mass", 100, 0., 4.0)};

    TH1F* h_leadingChsPt         { new TH1F("h_leadingChsPt", 	        ";p_{T}", 200, 0., 100.)};
    TH1F* h_subleadingChsPt      { new TH1F("h_subleadingChsPt",        ";p_{T}", 200, 0., 100.)};
    TH1F* h_chsDeltaR            { new TH1F("h_chsDeltaR",              ";#Delta R", 500, 0., 4.)};
    TH1F* h_diChsPt              { new TH1F("h_diChsPt",                ";p_{T}", 400, 0., 200.)};
    TH1F* h_diChsPtOverDeltaR    { new TH1F("h_diChsPtOverDeltaR",      "p_{T}/#Delta R", 4000, 0., 10.)};
    TH2F* h_diChsPtOverDeltaR2D  { new TH2F("h_diChsPtOverDeltaR2D",    "; p_{T}; #Delta R", 200, 0., 100., 500, 0., 10.)};
    TH1F* h_diChsMass            { new TH1F("h_diChsMass",              ";Mass", 100, 0., 4.0)};
    TH1F* h_diChsRefittedMass    { new TH1F("h_diChsRefittedMass",      ";Mass", 100, 0., 4.0)};

    TH1F* h_scalarDeltaR         { new TH1F("h_scalarDeltaR",           ";#Delta R", 500, 0., 10.)};
    TH2F* h_scalarMasses         { new TH2F("h_scalarMasses",           ";#mu#mu Mass; #pi#pi Mass", 400, 0., 4.0, 400, 0., 4.0)};
    TH2F* h_scalarMassesNew      { new TH2F("h_scalarMassesNew",        ";#mu#mu Mass; #pi#pi Mass", 400, 0., 4.0, 400, 0., 4.0)};
    TH2F* h_scalarRefittedMasses { new TH2F("h_scalarRefittedMasses",   ";#mu#mu Mass; #pi#pi Mass", 400, 0., 4.0, 400, 0., 4.0)};
    TH1F* h_scalarMass           { new TH1F("h_scalarMass",             ";Higgs Mass", 200, 75., 175.)};
    TH1F* h_scalarRefittedMass   { new TH1F("h_scalarRefittedMass",     ";Higgs Mass", 200, 75., 175.)};
    TH2F* h_scalarRelIso         { new TH2F("h_scalarRelIso",           "RelIso;I_{#mu#mu};I_{#pi#pi}", 500, 0., 0.5, 500, 0., 0.5)};
    TH2F* h_scalarIso            { new TH2F("h_scalarIso",              "Non-rel Iso;I_{#mu#mu};I_{#pi#pi}", 300, 0., 15, 300, 0., 15)};

    TH1F* h_leadingChsJetPt         { new TH1F("h_leadingChsJetPt",          ";p_{T}", 200, 0., 100.)};
    TH1F* h_subleadingChsJetPt      { new TH1F("h_subleadingChsJetPt",        ";p_{T}", 200, 0., 100.)};
    TH1F* h_chsJetDeltaR            { new TH1F("h_ChsJetDeltaR",              ";#Delta R", 500, 0., 4.)};
    TH1F* h_diChsJetPt              { new TH1F("h_diChsJetPt",                ";p_{T}", 400, 0., 200.)};
    TH1F* h_diChsJEtPtOverDeltaR    { new TH1F("h_diChsJetPtOverDeltaR",      "p_{T}/#Delta R", 5000, 0., 10.)};
    TH2F* h_diChsJEtPtOverDeltaR2D  { new TH2F("h_diChsJetPtOverDeltaR2D",    "; p_{T}; #Delta R", 200, 0., 100., 500, 0., 10.)};
    TH1F* h_diChsJetMass            { new TH1F("h_diChsJetMass",              ";Mass", 100, 0., 4.0)};

    TH1F* h_scalarJetDeltaR         { new TH1F("h_scalarJetDeltaR",           ";#Delta R", 500, 0., 10.)};
    TH2F* h_scalarJetMasses         { new TH2F("h_scalarJetMasses",           ";#mu#mu Mass; a#bar{a} Mass", 100, 0., 4.0, 100, 0., 4.0)};
    TH1F* h_scalarJetMass           { new TH1F("h_scalarJetMass",             ";Higgs Mass", 200, 75., 175.)};

    // post trigger

    TH1F* ht_leadingMuonPt       { new TH1F("ht_leadingMuonPt",         ";p_{T}", 200, 0., 100.)};
    TH1F* ht_subleadingMuonPt    { new TH1F("ht_subleadingMuonPt",      ";p_{T}", 200, 0., 100.)};
    TH1F* ht_muonDeltaR          { new TH1F("ht_muonDeltaR",            ";#Delta R", 500, 0., 1.)}; 
    TH1F* ht_dimuonPt            { new TH1F("ht_dimuonPt",              ";p_{T}", 400, 0., 200.)};
    TH1F* ht_muonPtOverDeltaR    { new TH1F("ht_muonPtOverDeltaR",      "p_{T}/#Delta R", 5000, 0., 10.)};
    TH2F* ht_muonPtOverDeltaR2D  { new TH2F("ht_muonPtOverDeltaR2D",    "; p_{T}; #Delta R", 200, 0., 100., 500, 0., 5.)};
    TH1F* ht_diMuonMass          { new TH1F("ht_diMuonMass",            "Mass", 100, 0., 4.0)};
    TH1F* ht_diMuonRefittedMass  { new TH1F("ht_diMuonRefittedMass",    ";Mass", 100, 0., 4.0)};

    TH1F* ht_leadingChsPt        { new TH1F("ht_leadingChsPt",          ";p_{T}", 200, 0., 100.)};
    TH1F* ht_subleadingChsPt     { new TH1F("ht_subleadingChsPt",       ";p_{T}", 200, 0., 100.)};
    TH1F* ht_chsDeltaR           { new TH1F("ht_chsDeltaR",             ";#Delta R", 500, 0., 4.)};
    TH1F* ht_diChsPt             { new TH1F("ht_diChsPt",               ";p_{T}", 400, 0., 200.)};
    TH1F* ht_diChsPtOverDeltaR   { new TH1F("ht_diChsPtOverDeltaR",     "p_{T}/#Delta R", 5000, 0., 10.)};
    TH2F* ht_diChsPtOverDeltaR2D { new TH2F("ht_diChsPtOverDeltaR2D",   "; p_{T}; #Delta R", 200, 0., 100., 500, 0., 10.)};
    TH1F* ht_diChsMass           { new TH1F("ht_diChsMass",             "'Mass", 100, 0., 4.0)};
    TH1F* ht_diChsRefittedMass   { new TH1F("ht_diChsRefittedMass",     ";Mass", 100, 0., 4.0)};

    TH1F* ht_scalarDeltaR          { new TH1F("ht_scalarDeltaR",           ";#Delta R", 500, 0., 10.)};
    TH2F* ht_scalarMasses          { new TH2F("ht_scalarMasses",           ";#mu#mu Mass; #pi#pi mass", 400, 0., 4.0, 400, 0., 4.0)};
    TH2F* ht_scalarMassesNew       { new TH2F("ht_scalarMassesNew",        ";#mu#mu Mass; #pi#pi mass", 400, 0., 4.0, 400, 0., 4.0)};
    TH2F* ht_scalarRefittedMasses  { new TH2F("ht_scalarRefittedMasses",   ";#mu#mu Mass; #pi#pi mass", 400, 0., 4.0,  400, 0., 4.0)};
    TH1F* ht_scalarMass            { new TH1F("ht_scalarMass",             "Higgs Mass", 200, 75., 175.)};
    TH1F* ht_scalarRefittedMass    { new TH1F("ht_scalarRefittedMass",     "Higgs Mass", 200, 75., 175.)};
    TH2F* ht_scalarRelIso          { new TH2F("ht_scalarRelIso",           "RelIso;I_{#mu#mu};I_{#pi#pi}", 500, 0., 0.5, 500, 0., 0.5)};
    TH2F* ht_scalarIso             { new TH2F("ht_scalarIso",              "Non-rel Iso;I_{#mu#mu};I_{#pi#pi}", 300, 0., 15, 300, 0., 15)};

    TH1F* ht_leadingChsJetPt         { new TH1F("ht_leadingChsJetPt", 	        ";p_{T}", 200, 0., 100.)};
    TH1F* ht_subleadingChsJetPt      { new TH1F("ht_subleadingChsJetPt",        ";p_{T}", 200, 0., 100.)};
    TH1F* ht_chsJetDeltaR            { new TH1F("ht_ChsJetDeltaR",              ";#Delta R", 500, 0., 4.)};
    TH1F* ht_diChsJetPt              { new TH1F("ht_diChsJetPt",                ";p_{T}", 400, 0., 200.)};
    TH1F* ht_diChsJEtPtOverDeltaR    { new TH1F("ht_diChsJetPtOverDeltaR",      "p_{T}/#Delta R", 5000, 0., 10.)};
    TH2F* ht_diChsJEtPtOverDeltaR2D  { new TH2F("ht_diChsJetPtOverDeltaR2D",    "; p_{T}; #Delta R", 200, 0., 100., 500, 0., 10.)};
    TH1F* ht_diChsJetMass            { new TH1F("ht_diChsJetMass",              ";Mass", 100, 0., 4.0)};

    TH1F* ht_scalarJetDeltaR         { new TH1F("ht_scalarJetDeltaR",           ";#Delta R", 500, 0., 10.)};
    TH2F* ht_scalarJetMasses         { new TH2F("ht_scalarJetMasses",           ";#mu#mu Mass; a#bar{a} Mass", 100, 0., 4.0, 100, 0., 4.0)};
    TH1F* ht_scalarJetMass           { new TH1F("ht_scalarJetMass",             ";Higgs Mass", 200, 75., 175.)};


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

            for (Int_t k = 0; k < event.numMuonPF2PAT; k++ ){
                if ( event.genMuonPF2PATMotherId[k] == 9000006 )  {
                    h_muonPtGenuine->Fill(event.muonPF2PATPt[k]);
                    h_genMuonPtGenuine->Fill(event.genMuonPF2PATPT[k]);
                }
            }

            // Get muons
            std::vector<int> looseMuonIndex = getLooseMuons(event);

            if ( looseMuonIndex.size() < 2 ) continue;

            getDileptonCand( event, looseMuonIndex, false);

            if ( event.genMuonPF2PATMotherId[event.zPairIndex.first] == 9000006 )  {
                h_muon1PtGenuine->Fill(event.muonPF2PATPt[event.zPairIndex.first],datasetWeight);
                h_genMuon1PtGenuine->Fill(event.genMuonPF2PATPT[event.zPairIndex.first],datasetWeight);
            }
            if ( event.genMuonPF2PATMotherId[event.zPairIndex.second] == 9000006 ) {
                h_muon2PtGenuine->Fill(event.muonPF2PATPt[event.zPairIndex.second],datasetWeight);
                h_genMuon2PtGenuine->Fill(event.genMuonPF2PATPT[event.zPairIndex.second],datasetWeight);
            }

            if ( event.genMuonPF2PATMotherId[event.zPairIndex.first] == 9000006 && event.genMuonPF2PATMotherId[event.zPairIndex.second] == 9000006 ) {
                h_muonDeltaRGenuine->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
                h_genMuonDeltaRGenuine->Fill( deltaR(event.genMuonPF2PATEta[event.zPairIndex.first], event.genMuonPF2PATPhi[event.zPairIndex.first], event.genMuonPF2PATEta[event.zPairIndex.second], event.genMuonPF2PATPhi[event.zPairIndex.second]) );
            }
	
            if (!event.metFilters()) continue;

            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

            const bool passTriggers ( event.muTrig() );

//            const int index1 {event.zPairIndex.first}, index2 {event.zPairIndex.second};
            const TLorentzVector muon1Vec {event.zPairLeptons.first}, muon2Vec {event.zPairLeptons.second};

             int idx1 {event.muonPF2PATPackedCandIndex[event.zPairIndex.first]};
             int idx2 {event.muonPF2PATPackedCandIndex[event.zPairIndex.second]};

            TLorentzVector muon1VecNew{event.packedCandsPseudoTrkPx[idx1], event.packedCandsPseudoTrkPy[idx1], event.packedCandsPseudoTrkPz[idx1], event.packedCandsE[idx1]};
            TLorentzVector muon2VecNew{event.packedCandsPseudoTrkPx[idx2], event.packedCandsPseudoTrkPy[idx2], event.packedCandsPseudoTrkPz[idx2], event.packedCandsE[idx2]};

            // Get CHS

            std::vector<int> chsIndex = getChargedHadronTracks(event);
            if ( chsIndex.size() < 2 ) continue;
            if (!getDihadronCand(event, chsIndex)) continue;

//            float muScalarMass ( (event.zPairLeptons.first + event.zPairLeptons.second).M() ), hadScalarMass ( (event.chsTrkPairVec.first + event.chsTrkPairVec.second).M() );
//            if ( muScalarMass  < hadScalarMass*1.25 && muScalarMass  >= hadScalarMass*.75 ) return false;
 //           if ( hadScalarMass < muScalarMass*1.25  && hadScalarMass >= muScalarMass*.75  ) return false;

            const TLorentzVector chs1Vec{event.chsPairVec.first}, chs2Vec{event.chsPairVec.second};

            TLorentzVector jet1Vec, jet2Vec;
            const int jetIndex1 {event.packedCandsJetIndex[event.chsPairIndex.first]}, jetIndex2 {event.packedCandsJetIndex[event.chsPairIndex.second]};
            jet1Vec.SetPtEtaPhiE(event.jetPF2PATPt[jetIndex1], event.jetPF2PATEta[jetIndex1], event.jetPF2PATPhi[jetIndex1], event.jetPF2PATE[jetIndex1]);
            jet2Vec.SetPtEtaPhiE(event.jetPF2PATPt[jetIndex2], event.jetPF2PATEta[jetIndex2], event.jetPF2PATPhi[jetIndex2], event.jetPF2PATE[jetIndex2]);

            h_leadingMuonPt->Fill(muon1Vec.Pt(),datasetWeight);
            h_subleadingMuonPt->Fill(muon2Vec.Pt(),datasetWeight);
            h_muonDeltaR->Fill(muon1Vec.DeltaR(muon2Vec),datasetWeight);
            h_dimuonPt->Fill( (muon1Vec + muon2Vec).Pt(),datasetWeight );
            h_muonPtOverDeltaR->Fill( ((muon1Vec + muon2Vec).Pt())/(muon1Vec.DeltaR(muon2Vec) + 1.0e-06),datasetWeight );
            h_muonPtOverDeltaR2D->Fill( (muon1Vec + muon2Vec).Pt(), muon1Vec.DeltaR(muon2Vec),datasetWeight );
            h_diMuonMass->Fill((muon1Vec + muon2Vec).M(),datasetWeight );
            h_diMuonRefittedMass->Fill((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).M(),datasetWeight );

            h_leadingChsPt->Fill(chs1Vec.Pt(),datasetWeight);
      	    h_subleadingChsPt->Fill(chs2Vec.Pt(),datasetWeight);
      	    if ( (chs1Vec+chs2Vec).M() < 5.) h_chsDeltaR->Fill(chs1Vec.DeltaR(chs2Vec),datasetWeight);
      	    h_diChsPt->Fill( (chs1Vec+chs2Vec).Pt(),datasetWeight );
      	    h_diChsPtOverDeltaR->Fill( ((chs1Vec+chs2Vec).Pt())/ (chs1Vec.DeltaR(chs2Vec) + 1.0e-06),datasetWeight );
      	    h_diChsPtOverDeltaR2D->Fill( (chs1Vec+chs2Vec).Pt(), chs1Vec.DeltaR(chs2Vec),datasetWeight );
            h_diChsMass->Fill( (chs1Vec+chs2Vec).M(),datasetWeight );
            h_diChsRefittedMass->Fill( (event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );

///

            h_scalarDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR( (chs1Vec+chs2Vec) ), datasetWeight );
            h_scalarMasses->Fill( (muon1Vec+muon2Vec).M(), (chs1Vec+chs2Vec).M(), datasetWeight );
            h_scalarMassesNew->Fill( (muon1VecNew+muon2VecNew).M(), (chs1Vec+chs2Vec).M(),datasetWeight );
            h_scalarRefittedMasses->Fill( (event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).M(), (event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );
            h_scalarMass->Fill( (muon1Vec+muon2Vec+chs1Vec+chs2Vec).M(),datasetWeight );
            h_scalarRefittedMass->Fill( (event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second+event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );
            h_scalarRelIso->Fill(event.zRelIso, event.chsTrkIso, datasetWeight);
            h_scalarIso->Fill(event.zRelIso*(muon1Vec+muon2Vec).Pt(), event.chsTrkIso*(chs1Vec+chs2Vec).Pt(), datasetWeight);

            h_leadingChsJetPt->Fill( jet1Vec.Pt(),datasetWeight );
            h_subleadingChsJetPt->Fill( jet2Vec.Pt(),datasetWeight );
            h_chsJetDeltaR->Fill( jet1Vec.DeltaR(jet2Vec),datasetWeight );
            h_diChsJetPt->Fill( (jet1Vec+jet2Vec).Pt(),datasetWeight );
            h_diChsJEtPtOverDeltaR->Fill( ((jet1Vec+jet2Vec).Pt())/ (jet1Vec.DeltaR(jet2Vec) + 1.0e-06),datasetWeight );
            h_diChsJEtPtOverDeltaR2D->Fill( (jet1Vec+jet2Vec).Pt(), jet1Vec.DeltaR(jet2Vec),datasetWeight );
            h_diChsJetMass->Fill( (jet1Vec+jet2Vec).M(),datasetWeight );

            h_scalarJetDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR(jet1Vec+jet2Vec),datasetWeight );
            h_scalarJetMasses->Fill( (muon1Vec+muon2Vec).M(), (jet1Vec+jet2Vec).M(),datasetWeight );
            h_scalarJetMass->Fill( (muon1Vec+muon2Vec+jet1Vec+jet2Vec).M(),datasetWeight );

            if ( passTriggers ) {
                ht_leadingMuonPt->Fill(muon1Vec.Pt(),datasetWeight);
                ht_subleadingMuonPt->Fill(muon2Vec.Pt(),datasetWeight);
                ht_muonDeltaR->Fill(muon1Vec.DeltaR(muon2Vec),datasetWeight);
                ht_dimuonPt->Fill( (muon1Vec + muon2Vec).Pt(),datasetWeight );
                ht_muonPtOverDeltaR->Fill( ((muon1Vec + muon2Vec).Pt())/(muon1Vec.DeltaR(muon2Vec) + 1.0e-06),datasetWeight );
                ht_muonPtOverDeltaR2D->Fill( (muon1Vec + muon2Vec).Pt(), muon1Vec.DeltaR(muon2Vec),datasetWeight );
                ht_diMuonMass->Fill((muon1Vec + muon2Vec).M(),datasetWeight );
                ht_diMuonRefittedMass->Fill((event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).M(),datasetWeight );

                ht_leadingChsPt->Fill(chs1Vec.Pt(),datasetWeight);
                ht_subleadingChsPt->Fill(chs2Vec.Pt(),datasetWeight);
      	        ht_chsDeltaR->Fill(chs1Vec.DeltaR(chs2Vec),datasetWeight);
                ht_diChsPt->Fill( (chs1Vec+chs2Vec).Pt(),datasetWeight );
      	        ht_diChsPtOverDeltaR->Fill( ((chs1Vec+chs2Vec).Pt())/ (chs1Vec.DeltaR(chs2Vec) + 1.0e-06),datasetWeight );
                ht_diChsPtOverDeltaR2D->Fill( (chs1Vec+chs2Vec).Pt(), chs1Vec.DeltaR(chs2Vec),datasetWeight );
                ht_diChsMass->Fill( (chs1Vec+chs2Vec).M(),datasetWeight );
                ht_diChsRefittedMass->Fill( (event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );

                ht_scalarDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR( (chs1Vec+chs2Vec) ),datasetWeight );
                ht_scalarMasses->Fill( (muon1Vec+muon2Vec).M(), (chs1Vec+chs2Vec).M(),datasetWeight );
                ht_scalarRefittedMasses->Fill( (event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second).M(), (event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );
                ht_scalarMass->Fill( (muon1Vec+muon2Vec+chs1Vec+chs2Vec).M(),datasetWeight );
                ht_scalarRefittedMass->Fill( (event.zPairLeptonsRefitted.first+event.zPairLeptonsRefitted.second+event.chsTrkPairVecRefitted.first+event.chsTrkPairVecRefitted.second).M(),datasetWeight );
                ht_scalarRelIso->Fill(event.zRelIso, event.chsTrkIso, datasetWeight);
                ht_scalarIso->Fill(event.zRelIso*(muon1Vec+muon2Vec).Pt(), event.chsTrkIso*(chs1Vec+chs2Vec).Pt(), datasetWeight);


                h_leadingChsJetPt->Fill( jet1Vec.Pt(),datasetWeight );
                h_subleadingChsJetPt->Fill( jet2Vec.Pt(),datasetWeight );
                h_chsJetDeltaR->Fill( jet1Vec.DeltaR(jet2Vec),datasetWeight );
                h_diChsJetPt->Fill( (jet1Vec+jet2Vec).Pt(),datasetWeight );
                h_diChsJEtPtOverDeltaR->Fill( ((jet1Vec+jet2Vec).Pt())/ (jet1Vec.DeltaR(jet2Vec) + 1.0e-06),datasetWeight );
                h_diChsJEtPtOverDeltaR2D->Fill( (jet1Vec+jet2Vec).Pt(), jet1Vec.DeltaR(jet2Vec),datasetWeight );
                h_diChsJetMass->Fill( (jet1Vec+jet2Vec).M(),datasetWeight );

                h_scalarJetDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR(jet1Vec+jet2Vec),datasetWeight );
                h_scalarJetMasses->Fill( (muon1Vec+muon2Vec).M(), (jet1Vec+jet2Vec).M(),datasetWeight );
                h_scalarJetMass->Fill( (muon1Vec+muon2Vec+jet1Vec+jet2Vec).M(),datasetWeight );
            }
            
        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_muonPtGenuine->Write();
    h_muon1PtGenuine->Write();
    h_muon2PtGenuine->Write();
    h_muonDeltaRGenuine->Write();

    h_genMuonPtGenuine->Write();
    h_genMuon1PtGenuine->Write();
    h_genMuon2PtGenuine->Write();
    h_genMuonDeltaRGenuine->Write();

    h_leadingMuonPt->Write();
    h_subleadingMuonPt->Write();
    h_muonDeltaR->Write();    
    h_dimuonPt->Write();
    h_muonPtOverDeltaR->Write();
    h_muonPtOverDeltaR2D->Write();   
    h_diMuonMass->Write();
    h_diMuonRefittedMass->Write();

    h_leadingChsPt->Write();
    h_subleadingChsPt->Write();
    h_chsDeltaR->Write();
    h_diChsPt->Write();
    h_diChsPtOverDeltaR->Write();
    h_diChsPtOverDeltaR2D->Write();
    h_diChsMass->Write();
    h_diChsRefittedMass->Write();

    h_scalarDeltaR->Write();
    h_scalarMasses->Write();
    h_scalarMassesNew->Write();
    h_scalarRefittedMasses->Write();
    h_scalarMass->Write();
    h_scalarRefittedMass->Write();
    h_scalarRelIso->Write();
    h_scalarIso->Write();

    h_leadingChsJetPt->Write();
    h_subleadingChsJetPt->Write();
    h_chsJetDeltaR->Write();
    h_diChsJetPt->Write();
    h_diChsJEtPtOverDeltaR->Write();
    h_diChsJEtPtOverDeltaR2D->Write();
    h_diChsJetMass->Write();

    h_scalarJetDeltaR->Write();
    h_scalarJetMasses->Write();
    h_scalarJetMass->Write();

    // post trigger plots
    ht_leadingMuonPt->Write();
    ht_subleadingMuonPt->Write();
    ht_muonDeltaR->Write();    
    ht_dimuonPt->Write();
    ht_muonPtOverDeltaR->Write();
    ht_muonPtOverDeltaR2D->Write();   
    ht_diMuonMass->Write();
    ht_diMuonRefittedMass->Write();

    ht_leadingChsPt->Write();
    ht_subleadingChsPt->Write();
    ht_chsDeltaR->Write();
    ht_diChsPt->Write();
    ht_diChsPtOverDeltaR->Write();
    ht_diChsPtOverDeltaR2D->Write();
    ht_diChsMass->Write();
    ht_diChsRefittedMass->Write();

    ht_scalarDeltaR->Write();
    ht_scalarMasses->Write();
    ht_scalarRefittedMasses->Write();
    ht_scalarMass->Write();
    ht_scalarRefittedMass->Write();
    ht_scalarRelIso->Write();
    ht_scalarIso->Write();

    ht_leadingChsJetPt->Write();
    ht_subleadingChsJetPt->Write();
    ht_chsJetDeltaR->Write();
    ht_diChsJetPt->Write();
    ht_diChsJEtPtOverDeltaR->Write();
    ht_diChsJEtPtOverDeltaR2D->Write();
    ht_diChsJetMass->Write();

    ht_scalarJetDeltaR->Write();
    ht_scalarJetMasses->Write();
    ht_scalarJetMass->Write();

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
        TLorentzVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
        if (lVec.Pt() < 1.0) continue;

        chs.emplace_back(k);
    }
    return chs;
}

std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt ) {
    std::vector<int> muons;
    for ( auto it = muonIndex.begin(); it!= muonIndex.end(); it++ ) {
        if ( event.genMuonPF2PATHardProcess[*it] == getPrompt ) muons.push_back(*it);
    }
    return muons;
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

            if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ ) {
                event.chsPairVec.first  = chs1.Pt() > chs2.Pt() ? chs1 : chs2;
                event.chsPairVec.second = chs1.Pt() > chs2.Pt() ? chs2 : chs1;
                event.chsPairIndex.first = chs1.Pt() > chs2.Pt() ? chs[i] : chs[j];
                event.chsPairIndex.second = chs1.Pt() > chs2.Pt() ? chs[j] : chs[i];

                TLorentzVector chsTrk1, chsTrk2;
                chsTrk1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.first], event.packedCandsPseudoTrkEta[event.chsPairIndex.first], event.packedCandsPseudoTrkPhi[event.chsPairIndex.first], event.packedCandsE[event.chsPairIndex.first]);
                chsTrk2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.second], event.packedCandsPseudoTrkEta[event.chsPairIndex.second], event.packedCandsPseudoTrkPhi[event.chsPairIndex.second], event.packedCandsE[event.chsPairIndex.second]);

                event.chsTrkPairVec.first  = chsTrk1;
                event.chsTrkPairVec.second = chsTrk2;

                float neutral_iso {0.0}, neutral_iso1 {0.0}, neutral_iso2 {0.0};
                float ch_iso {0.0}, ch_iso1 {0.0}, ch_iso2 {0.0};
                float pu_iso {0.0}, pu_iso1 {0.0}, pu_iso2 {0.0};

                for (int k = 0; k < event.numPackedCands; k++) {
                    if ( k == event.chsPairIndex.first || k == event.chsPairIndex.second ) continue;

                    TLorentzVector packedCandVec;
                    packedCandVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k], event.packedCandsPseudoTrkEta[k], event.packedCandsPseudoTrkPhi[k], event.packedCandsE[k]);

                    if ( event.packedCandsCharge[k] == 0 ) {
                        if ( packedCandVec.Pt() < 0.5 ) continue;
                        if ( event.chsTrkPairVec.first.DeltaR(packedCandVec)   < 0.3 )  neutral_iso1 += packedCandVec.Et();
                        if ( event.chsTrkPairVec.second.DeltaR(packedCandVec)  < 0.3 )  neutral_iso2 += packedCandVec.Et();
                        if ( (event.chsTrkPairVec.first+event.chsTrkPairVec.second).DeltaR(packedCandVec)  < 0.3 ) neutral_iso += packedCandVec.Et();
                    }
                    else {
                        if ( event.packedCandsFromPV[k] >= 2 ) {
                            if ( event.chsTrkPairVec.first.DeltaR(packedCandVec)   < 0.3 )  ch_iso1 += packedCandVec.Pt();
                            if ( event.chsTrkPairVec.second.DeltaR(packedCandVec)  < 0.3 )  ch_iso2 += packedCandVec.Pt();
                            if ( (event.chsTrkPairVec.first+event.chsTrkPairVec.second).DeltaR(packedCandVec)  < 0.3 ) ch_iso += packedCandVec.Pt();
                        }
                        else {
                            if ( packedCandVec.Pt() < 0.5 ) continue;
                                if ( event.chsTrkPairVec.first.DeltaR(packedCandVec)   < 0.3 )  pu_iso1 += packedCandVec.Pt();
                                if ( event.chsTrkPairVec.second.DeltaR(packedCandVec)  < 0.3 )  pu_iso2 += packedCandVec.Pt();
                                if ( (event.chsTrkPairVec.first+event.chsTrkPairVec.second).DeltaR(packedCandVec)  < 0.3 ) pu_iso += packedCandVec.Pt();
                        }
                    }
                }

                const float iso1 = ch_iso1 + std::max( float(0.0), neutral_iso1 - float(0.5*pu_iso1) );
                const float iso2 = ch_iso2 + std::max( float(0.0), neutral_iso2 - float(0.5*pu_iso2) );
                const float iso  = ch_iso  + std::max( float(0.0), neutral_iso  - float(0.5*pu_iso)  );

                event.chsPairTrkIso.first = iso1/(event.chsPairVec.first.Pt() + 1.0e-06);
                event.chsPairTrkIso.second = iso2/(event.chsPairVec.second.Pt() + 1.0e-06);
                event.chsTrkIso = iso/((event.chsPairVec.first+event.chsPairVec.second).Pt() + 1.0e-06);

//                if ( event.chsTrkIso > 0.4 ) continue;

                event.chsPairTrkIndex = getChsTrackPairIndex(event);
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

float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}

