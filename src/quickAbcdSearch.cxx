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

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const bool invertCharge);
bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chsIndex, const bool invertCharge);
int getMuonTrackPairIndex(const AnalysisEvent& event);
int getChsTrackPairIndex(const AnalysisEvent& event);
float deltaR(float eta1, float phi1, float eta2, float phi2);

bool is2016_;
bool is2018_;

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0}, chsMass_{0.13957018};

// Diparticle cuts
double maxDileptonDeltaR_ {0.2}, maxChsDeltaR_ {0.4};
double higgsTolerence_ {10.};

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
    bool mcTruth_ {false};
   
    std::string outFileString{"plots/distributions/output.root"};
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Quick and dirty plots

    TH2F* h_qMuonOverQhadrons      {new TH2F("h_qMuonOverQhadrons",       "; Q_{#mu#mu}; Q_{qq}", 2, -1.0, 1.0, 2, -1.0, 1.0)};
    TH2F* h_qMuonOverRelIsoMuMu    {new TH2F("h_qMuonOverRelIsoMuMu",     "; Q_{#mu#mu}; I^{rel}_{#mu#mu}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qMuonOverRelIsoQq      {new TH2F("h_qMuonOverRelIsoQq",       "; Q_{#mu#mu}; I^{rel}_{qq}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qMuonOverRelIsoMu1     {new TH2F("h_qMuonOverRelIsoMu1",      "; Q_{#mu#mu}; I^{rel}_{#mu1}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qMuonOverRelIsoMu2     {new TH2F("h_qMuonOverRelIsoMu2",      "; Q_{#mu#mu}; I^{rel}_{#mu2}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qMuonOverRelIsoQ1      {new TH2F("h_qMuonOverRelIsoQ1",       "; Q_{#mu#mu}; I^{rel}_{q1}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qMuonOverRelIsoQ2      {new TH2F("h_qMuonOverRelIsoQ2",       "; Q_{#mu#mu}; I^{rel}_{q2}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoMuMu {new TH2F("h_qHadronsOverRelIsoMuMu",  "; Q_{qq}; I^{rel}_{#mu#mu}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoQq   {new TH2F("h_qHadronsOverRelIsoQq",    "; Q_{qq}; I^{rel}_{qq}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoMu1  {new TH2F("h_qHadronsOverRelIsoMu1",   "; Q_{qq}; I^{rel}_{#mu1}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoMu2  {new TH2F("h_qHadronsOverRelIsoMu2",   "; Q_{qq}; I^{rel}_{#mu2}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoQ1   {new TH2F("h_qHadronsOverRelIsoQ1",    "; Q_{qq}; I^{rel}_{q1}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_qHadronsOverRelIsoQ2   {new TH2F("h_qHadronsOverRelIsoQ2",    "; Q_{qq}; I^{rel}_{q2}", 2, -1.0, 1.0, 500, 0., 5.)};
    TH2F* h_relIsoMuMuOverRelIsoQq {new TH2F("h_relIsoMuMuOverRelIsoQq",  "; I^{rel}_{#mu#mu}; I^{rel}_{qq}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMuMuOverRelIsoQ1 {new TH2F("h_relIsoMuMuOverRelIsoQ1",  "; I^{rel}_{#mu#mu}; I^{rel}_{q1}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMuMuOverRelIsoQ2 {new TH2F("h_relIsoMuMuOverRelIsoQ2",  "; I^{rel}_{#mu#mu}; I^{rel}_{q2}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoQqOverRelIsoMu1  {new TH2F("h_relIsoQqOverRelIsoMu1",   "; I^{rel}_{qq}; I^{rel}_{#mu1}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoQqOverRelIsoMu2  {new TH2F("h_relIsoQqOverRelIsoMu2",   "; I^{rel}_{qq}; I^{rel}_{#mu2}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMu1OverRelIsoQ1  {new TH2F("h_relIsoMu1OverRelIsoQ1",   "; I^{rel}_{#mu1}; I^{rel}_{q1}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMu1OverRelIsoQ2  {new TH2F("h_relIsoMu1OverRelIsoQ2",   "; I^{rel}_{#mu1}; I^{rel}_{q2}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMu2OverRelIsoQ1  {new TH2F("h_relIsoMu2OverRelIsoQ1",   "; I^{rel}_{#mu2}; I^{rel}_{q1}", 500, 0., 5., 500, 0., 5.)};
    TH2F* h_relIsoMu2OverRelIsoQ2  {new TH2F("h_relIsoMu2OverRelIsoQ2",   "; I^{rel}_{#mu2}; I^{rel}_{q2}", 500, 0., 5., 500, 0., 5.)};

    // Set bin labels for charge

    h_qMuonOverQhadrons->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverQhadrons->GetXaxis()->SetBinLabel(2, "SS #mu#mu");
    h_qMuonOverQhadrons->GetYaxis()->SetBinLabel(1, "OS qq");
    h_qMuonOverQhadrons->GetYaxis()->SetBinLabel(2, "SS qq");

    h_qMuonOverRelIsoMuMu->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverRelIsoMuMu->GetXaxis()->SetBinLabel(2, "SS #mu#mu");
    h_qMuonOverRelIsoMu1->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverRelIsoMu1->GetXaxis()->SetBinLabel(2, "SS #mu#mu");
    h_qMuonOverRelIsoMu2->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverRelIsoMu2->GetXaxis()->SetBinLabel(2, "SS #mu#mu");
    h_qMuonOverRelIsoQ1->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverRelIsoQ1->GetXaxis()->SetBinLabel(2, "SS #mu#mu");
    h_qMuonOverRelIsoQ2->GetXaxis()->SetBinLabel(1, "OS #mu#mu");
    h_qMuonOverRelIsoQ2->GetXaxis()->SetBinLabel(2, "SS #mu#mu");

    h_qHadronsOverRelIsoMuMu->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoMuMu->GetXaxis()->SetBinLabel(2, "SS qq");
    h_qHadronsOverRelIsoQq->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoQq->GetXaxis()->SetBinLabel(2, "SS qq");
    h_qHadronsOverRelIsoMu1->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoMu1->GetXaxis()->SetBinLabel(2, "SS qq");
    h_qHadronsOverRelIsoMu2->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoMu2->GetXaxis()->SetBinLabel(2, "SS qq");
    h_qHadronsOverRelIsoQ1->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoQ1->GetXaxis()->SetBinLabel(2, "SS qq");
    h_qHadronsOverRelIsoQ2->GetXaxis()->SetBinLabel(1, "OS qq");
    h_qHadronsOverRelIsoQ2->GetXaxis()->SetBinLabel(2, "SS qq");

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

            eventWeight *= datasetWeight;

            // Trigger + met filters
            if (!event.muTrig()) continue;
            if (!event.metFilters()) continue;

            // Get muons
            std::vector<int> looseMuonIndex = getLooseMuons(event);
            if ( looseMuonIndex.size() < 2 ) continue;

            // Get CHS
            std::vector<int> chsIndex = getChargedHadronTracks(event);
            if ( chsIndex.size() < 2 ) continue;

            // Nominal muon + hadron charges
            const bool dimuonFound = getDileptonCand( event, looseMuonIndex, false);
            const float muRelIso1 {event.zPairNewRelIso.first}, muRelIso2 {event.zPairNewRelIso.second}, mumuRelIso {event.zRelIso};
            const bool dihadronFound = getDihadronCand(event, chsIndex, false);
            const float chsRelIso1 {event.chsPairRelIso.first}, chsRelIso2 {event.chsPairRelIso.second}, dichsRelIso {event.chsRelIso};

            // Nominal muon + same sign hadron charges
            const bool dihadronFound_ssh = getDihadronCand(event, chsIndex, true);
            const float chsRelIso1_ssh {event.chsPairRelIso.first}, chsRelIso2_ssh {event.chsPairRelIso.second}, dichsRelIso_ssh {event.chsRelIso};

	    // Same charge muons + nominal hadron charges
            const bool dimuonFound_ssm = getDileptonCand( event, looseMuonIndex, true);
            const float muRelIso1_ssm {event.zPairNewRelIso.first}, muRelIso2_ssm {event.zPairNewRelIso.second}, mumuRelIso_ssm {event.zRelIso};
            const bool dihadronFound_ssm = getDihadronCand(event, chsIndex, false);
            const float chsRelIso1_ssm {event.chsPairRelIso.first}, chsRelIso2_ssm {event.chsPairRelIso.second}, dichsRelIso_ssm {event.chsRelIso};

            // Same charge muons + hadron charges
            const bool dihadronFound_ssmh = getDihadronCand(event, chsIndex, true);
            const float chsRelIso1_ssmh {event.chsPairRelIso.first}, chsRelIso2_ssmh {event.chsPairRelIso.second}, dichsRelIso_ssmh {event.chsRelIso};
/*
std::cout << __LINE__ << " : " << __FILE__ << std::endl;
std::cout << "dimuonFound = " << dimuonFound << std::endl;
std::cout << "dimuonFound_ssm = " << dimuonFound_ssm << std::endl;
std::cout << "dihadronFound = " << dihadronFound << std::endl;
std::cout << "dihadronFound_ssh = " << dihadronFound_ssh << std::endl;
std::cout << "dihadronFound_ssm = " << dihadronFound_ssm << std::endl;
std::cout << "dihadronFound_ssmh = " << dihadronFound_ssmh << std::endl;
std::cout << __LINE__ << " : " << __FILE__ << std::endl;
*/
            if ( dimuonFound && dihadronFound )            h_qMuonOverQhadrons->Fill( -0.5, -0.5, eventWeight );
            if ( dimuonFound && dihadronFound_ssh )        h_qMuonOverQhadrons->Fill( -0.5,  0.5, eventWeight );
            if ( dimuonFound_ssm && dihadronFound_ssm )    h_qMuonOverQhadrons->Fill(  0.5, -0.5, eventWeight );
            if ( dimuonFound_ssm &&  dihadronFound_ssmh )  h_qMuonOverQhadrons->Fill(  0.5,  0.5, eventWeight );

            // Muon charge vs iso
            if ( dimuonFound )        h_qMuonOverRelIsoMuMu->Fill( -0.5, mumuRelIso, eventWeight );
            if ( dimuonFound_ssm )    h_qMuonOverRelIsoMuMu->Fill(  0.5, mumuRelIso_ssm, eventWeight );
            if ( dimuonFound )        h_qMuonOverRelIsoMu1->Fill(  -0.5, muRelIso1, eventWeight );
            if ( dimuonFound_ssm )    h_qMuonOverRelIsoMu1->Fill(   0.5, muRelIso1_ssm, eventWeight );
            if ( dimuonFound )        h_qMuonOverRelIsoMu2->Fill(  -0.5, muRelIso2, eventWeight );
            if ( dimuonFound_ssm )    h_qMuonOverRelIsoMu2->Fill(   0.5, muRelIso2_ssm, eventWeight );
            if ( dimuonFound && dihadronFound)             h_qMuonOverRelIsoQq->Fill(   -0.5, dichsRelIso, eventWeight );
            if ( dimuonFound_ssm && dihadronFound_ssm )    h_qMuonOverRelIsoQq->Fill(    0.5, dichsRelIso_ssm, eventWeight );
            if ( dimuonFound && dihadronFound )            h_qMuonOverRelIsoQ1->Fill(   -0.5, chsRelIso1, eventWeight );
            if ( dimuonFound_ssm && dihadronFound_ssm )    h_qMuonOverRelIsoQ1->Fill(    0.5, chsRelIso1_ssm, eventWeight );
            if ( dimuonFound && dihadronFound)             h_qMuonOverRelIsoQ2->Fill(   -0.5, chsRelIso2, eventWeight );
            if ( dimuonFound_ssm && dihadronFound_ssm)     h_qMuonOverRelIsoQ2->Fill(    0.5, chsRelIso2_ssm, eventWeight );

            // Hadron charge vs iso
            if ( dihadronFound )      h_qHadronsOverRelIsoMuMu->Fill( -0.5, mumuRelIso, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoMuMu->Fill(  0.5, mumuRelIso, eventWeight );
            if ( dihadronFound )      h_qHadronsOverRelIsoQq->Fill(   -0.5, dichsRelIso, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoQq->Fill(    0.5, dichsRelIso_ssh, eventWeight );
            if ( dihadronFound )      h_qHadronsOverRelIsoMu1->Fill(  -0.5, muRelIso1, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoMu1->Fill(   0.5, muRelIso1, eventWeight );
            if ( dihadronFound )      h_qHadronsOverRelIsoMu2->Fill(  -0.5, muRelIso2, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoMu2->Fill(   0.5, muRelIso2, eventWeight );
            if ( dihadronFound )      h_qHadronsOverRelIsoQ1->Fill(   -0.5, chsRelIso1, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoQ1->Fill(    0.5, chsRelIso1_ssh, eventWeight );
            if ( dihadronFound )      h_qHadronsOverRelIsoQ2->Fill(   -0.5, chsRelIso2, eventWeight );
            if ( dihadronFound_ssh )  h_qHadronsOverRelIsoQ2->Fill(    0.5, chsRelIso2_ssh, eventWeight );

            // No charge inversion
            if ( dimuonFound && dihadronFound ) {
                h_relIsoMuMuOverRelIsoQq->Fill( mumuRelIso, dichsRelIso, eventWeight );
                h_relIsoMuMuOverRelIsoQ1->Fill( mumuRelIso, chsRelIso1, eventWeight );
                h_relIsoMuMuOverRelIsoQ2->Fill( mumuRelIso, chsRelIso2, eventWeight );
                h_relIsoQqOverRelIsoMu1->Fill(  dichsRelIso, muRelIso1, eventWeight );
                h_relIsoQqOverRelIsoMu2->Fill(  dichsRelIso, muRelIso2, eventWeight );
                h_relIsoMu1OverRelIsoQ1->Fill(  muRelIso1, chsRelIso1, eventWeight );
                h_relIsoMu1OverRelIsoQ2->Fill(  muRelIso1, chsRelIso2, eventWeight );
                h_relIsoMu2OverRelIsoQ1->Fill(  muRelIso2, chsRelIso1, eventWeight );
                h_relIsoMu2OverRelIsoQ2->Fill(  muRelIso2, chsRelIso2, eventWeight );
            }

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Write out histos

    h_qMuonOverQhadrons->Write();
    h_qMuonOverRelIsoMuMu->Write();
    h_qMuonOverRelIsoQq->Write();
    h_qMuonOverRelIsoMu1->Write();
    h_qMuonOverRelIsoMu2->Write();
    h_qMuonOverRelIsoQ1->Write();
    h_qMuonOverRelIsoQ2->Write();
    h_qHadronsOverRelIsoMuMu->Write();
    h_qHadronsOverRelIsoQq->Write();
    h_qHadronsOverRelIsoMu1->Write();
    h_qHadronsOverRelIsoMu2->Write();
    h_qHadronsOverRelIsoQ1->Write();
    h_qHadronsOverRelIsoQ2->Write();
    h_relIsoMuMuOverRelIsoQq->Write();
    h_relIsoMuMuOverRelIsoQ1->Write();
    h_relIsoMuMuOverRelIsoQ2->Write();
    h_relIsoQqOverRelIsoMu1->Write();
    h_relIsoQqOverRelIsoMu2->Write();
    h_relIsoMu1OverRelIsoQ1->Write();
    h_relIsoMu1OverRelIsoQ2->Write();
    h_relIsoMu2OverRelIsoQ1->Write();
    h_relIsoMu2OverRelIsoQ2->Write();

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
            if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] /* && event.muonPF2PATPfIsoVeryLoose[i] */ ) {

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
            if ( delR < maxDileptonDeltaR_  ) {
                event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
                event.zPairIndex.second  = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
                event.zPairRelIso.first  = event.muonPF2PATComRelIsodBeta[muons[i]];
                event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];

//                if (!event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first]) continue;
//                if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] > 1.0) continue;

                if (event.muonPF2PATPfIsoVeryLoose[event.zPairIndex.first]) continue;
                if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.second] <= 1.0) continue;

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

