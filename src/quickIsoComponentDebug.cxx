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

float deltaR(float eta1, float phi1, float eta2, float phi2);

bool is2016_;
bool is2018_;

namespace fs = boost::filesystem;

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

    TH1F* h_iso                {new TH1F("h_iso",     "; rel iso;", 500, 0., 5.0)};

    TH1F* h_iso_CHS            {new TH1F("h_iso_CHS", "; #sum_{p_{T}}^{#mu} charged hadrons; ", 100, 0., 50.)};
    TH1F* h_iso_NHS            {new TH1F("h_iso_NHS", "; #sum_{E_{T}}^{#mu} neutral hadrons; ", 100, 0., 50.)};
    TH1F* h_iso_Pho            {new TH1F("h_iso_Pho", "; #sum_{E_{T}}^{#mu} photons; ", 100, 0., 50.)};
    TH1F* h_iso_PU             {new TH1F("h_iso_PU",  "; #frac{1}{2}#times#sum_{p_{T}}^{#mu} PU; ", 100, 0., 50.)};

    TH1F* h_zeroIso_CHS        {new TH1F("h_zeroIso_CHS", "; #sum_{p_{T}}^{#mu} charged hadrons; ", 100, 0., 50.)};
    TH1F* h_zeroIso_NHS        {new TH1F("h_zeroIso_NHS", "; #sum_{E_{T}}^{#mu} neutral hadrons; ", 100, 0., 50.)};
    TH1F* h_zeroIso_Pho        {new TH1F("h_zeroIso_Pho", "; #sum_{E_{T}}^{#mu} photons; ", 100, 0., 50.)};
    TH1F* h_zeroIso_PU         {new TH1F("h_zeroIso_PU",  "; #frac{1}{2}#times#sum_{p_{T}}^{#mu} PU; ", 100, 0., 50.)};

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

            for (int k = 0; k < event.numMuonPF2PAT; k++) {

                if ( event.muonPF2PATPt[k] < 5.0 ) continue;
                if ( !event.muonPF2PATLooseCutId[k] ) continue;

                h_iso->Fill( event.muonPF2PATComRelIsodBeta[k], eventWeight);

                h_iso_CHS->Fill( event.muonPF2PATChHadIso[k], eventWeight);
                h_iso_NHS->Fill( event.muonPF2PATNtHadIso[k], eventWeight);
                h_iso_Pho->Fill( event.muonPF2PATGammaIso[k], eventWeight);
                h_iso_PU->Fill( 0.5*event.muonPF2PATPuIso[k], eventWeight);

                if ( event.muonPF2PATComRelIsodBeta[k] == 0.0 ) {
                    h_zeroIso_CHS->Fill( event.muonPF2PATChHadIso[k], eventWeight);
                    h_zeroIso_NHS->Fill( event.muonPF2PATNtHadIso[k], eventWeight);
                    h_zeroIso_Pho->Fill( event.muonPF2PATGammaIso[k], eventWeight);
                    h_zeroIso_PU->Fill( 0.5*event.muonPF2PATPuIso[k], eventWeight);
                }
            }
        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Write out histos

    h_iso->Write();

    h_iso_CHS->Write();
    h_iso_NHS->Write();
    h_iso_Pho->Write();
    h_iso_PU->Write();

    h_zeroIso_CHS->Write();
    h_zeroIso_NHS->Write();
    h_zeroIso_Pho->Write();
    h_zeroIso_PU->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

