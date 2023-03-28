#include "SharedFunctions.hpp"
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
#include "TMVA/Config.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "config_parser.hpp"
#include <Math/Vector4D.h>

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

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {4.0}, chsMass_{0.13957018};

// Diparticle cuts
const double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4};
const double scalarMassCut_{4.0}, higgsTolerence_ {10.};

// debug flag
const bool useMCTruth_{false};

//interesting events
const int ninteresting=2;
int run_nb[ninteresting]={1,1}; 
int lumi_nb[ninteresting]={8,9};
int num_nb[ninteresting]={9362,10548}; //has to be in ascending order though

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
    int flow, fhigh;
    bool verbose_{false};
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    bool ishack_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    TH1F* weightHisto {new TH1F{"weightHisto","Generator Weights",2,0.5,2.5}};
    

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
        "2018", po::bool_switch(&is2018_), "Use 2018 conditions (SFs, et al.).")(
		"flow", po::value<int>(&flow)->default_value(0), "Starting file no. - ")(
		"fhigh", po::value<int>(&fhigh)->default_value(10000), "Ending file no. -")(
		"verbose,v", po::bool_switch(&verbose_), "Print extra info")(
        "hack", po::value<bool>(&ishack_)->default_value(false), "Pass directly to dataset conf processing"); //BEWARE: bool_switch, switches the option from default when arg is present 
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
                             usePostLepTree,
                             ishack_); //last par is for hack, currently true
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
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};
//    const std::string postLepSelSkimInputDir{std::string{"/user/almorton/HToSS_analysis/postLepSkims"} + era + "/"};

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);

        std::cerr << "Processing dataset " << dataset->name() << std::endl;
        //int flow = 1;
        //int fhigh = 10000; //to be changed to take input from condor

        if (!usePostLepTree) {
            if (!datasetFilled) {
                if (!dataset->fillChain(datasetChain,  flow, fhigh)) {
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
     	
     	TMVA::gConfig().SetDrawProgressBar(true);	 
        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        
        totalEvents += numberOfEvents;
        Double_t sumWeights = 0.;

        // sumWeights = 835599; //from terminal from before -> But check later on
        
        // array of interesting events (PFIso leading > 2)
        int ctr=0;
        bool orig_verbose = verbose_;
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");
            event.GetEntry(i);
            sumWeights += event.processMCWeight;
            event.processMCWeight >= 0.0 ? weightHisto->AddBinContent(1,event.processMCWeight) : weightHisto->AddBinContent(2,std::abs(event.processMCWeight));
            // if (i%10000==0) std::cout<<"Measuring weights"<<std::endl;
        } // end event loop
        std::cout<<"SumWeights:"<<sumWeights<<std::endl;
        std::cout<<"Crosscheck:"<<weightHisto->GetBinContent(1)-weightHisto->GetBinContent(2)<<std::endl;
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();
    weightHisto->Write();
    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

