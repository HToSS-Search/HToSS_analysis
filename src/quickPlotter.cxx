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
bool getDileptonCand(AnalysisEvent& event, const std::vector<int> muons);

namespace fs = boost::filesystem;

// Lepton cut variables
const float looseMuonEta_ {5.0}, looseMuonPt_ {0.}, looseMuonPtLeading_ {0.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0};

int main(int argc, char* argv[])
{
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    int numFiles;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Histos
    TH1F* h_numMuons                   {new TH1F("h_numMuons",                 "number of reco muons in the event", 21, -0.5, 20.5)};
    TH1F* h_numMuonsFromScalar         {new TH1F("h_numMuonsFromScalar",       "number of reco muons with motherId = scalarId", 21, -0.5, 20.5)};
    TH1F* h_numMuonsPromptFinalState   {new TH1F("h_numMuonsPromptFinalState", "number of reco muons with PromptFinalState flag", 21, -0.5, 20.5)};
    TH1F* h_numMuonsHardProcess        {new TH1F("h_numMuonsHardProcess",      "number of reco muons with HardProcess flag", 21, -0.5, 20.5)};

    TH2F* h_relIsoVsEta1        {new TH2F("h_relIsoVsEta1", "rel iso vs leading lepton #eta; #eta; rel iso" , 300, -3., 3., 100, 0., 1.0)};
    TH2F* h_relIsoVsEta2        {new TH2F("h_relIsoVsEta2", "rel iso vs subleading lepton #eta; #eta; rel iso" , 300, -3., 3., 100, 0., 1.0)};
    TH2F* h_dxyVsDz1            {new TH2F("h_dxyVsDz1", "dxy vs dz leading lepton; d_{xy} (cm); d_{z} (cm)", 500, 0., 1., 500, 0., 1.)};
    TH2F* h_dxyVsDz2            {new TH2F("h_dxyVsDz2", "dxy vs dz leading lepton; d_{xy} (cm); d_{z} (cm)", 500, 0., 1., 500, 0., 1.)};

    TProfile* p_leadingMuonsProfile          {new TProfile("p_leadingMuonsProfile", "", 3, 0.5, 3.5)};
    TProfile* p_subleadingMuonsProfile       {new TProfile("p_subleadingMuonsProfile", "", 3, 0.5, 3.5)};

    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(1, "Scalar parentage");
    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(2, "PromptFinalState");
    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(2, "HardProcess");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(1, "Scalar parentage");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(2, "PromptFinalState");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(2, "HardProcess");

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "nFiles,f",
        po::value<int>(&numFiles)->default_value(-1),
        "Number of files to run over. All if set to -1.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(41528.0),
        "Lumi to scale MC plots to.")(
        "outfile,o",
        po::value<std::string>(&outFileString)->default_value(outFileString),
        "Output file for plots.")(
        ",n",
        po::value<Long64_t>(&nEvents)->default_value(0),
        "The number of events to be run over. All if set to 0.")(
        "2016", po::bool_switch(&is2016_), "Use 2016 conditions (SFs, et al.).");
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
                             totalLumi);
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

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset)
    {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);

        if (!datasetFilled) {
            if (!dataset->fillChain(datasetChain, numFiles)) {
                std::cerr << "There was a problem constructing the chain for " << dataset->name() << " made of " << numFiles << " files. Continuing with next dataset.\n";
                continue;
            }
            datasetFilled=true;
        }

        // extract the dataset weight. MC = (lumi*crossSection)/(totalEvents), data = 1.0
        float datasetWeight{dataset->getDatasetWeight(totalLumi)};
        std::cout << datasetChain->GetEntries() << " number of items in tree. Dataset weight: " << datasetWeight << std::endl;
        if (datasetChain->GetEntries() == 0) {
            std::cout << "No entries in tree, skipping..." << std::endl;
            continue;
        }

        AnalysisEvent event{dataset->isMC(), datasetChain, is2016_};

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
            if (! ( event.mumuTrig() || event.muTrig() ) ) continue;

            std::vector<int> looseMuonIndex = getLooseMuons(event);

            h_numMuons->Fill(looseMuonIndex.size());

            int numMuonsFromScalar{0}, numMuonsPromptFinalState{0}, numMuonsHardProcess{0};

            for ( auto it = looseMuonIndex.begin(); it != looseMuonIndex.end(); it++ ) {
      	       	if (event.genMuonPF2PATMotherId[*it] == 9000006) numMuonsFromScalar++;
                if (event.genMuonPF2PATPromptFinalState[*it]) numMuonsPromptFinalState++;
      	       	if (event.genMuonPF2PATHardProcess[*it]) numMuonsHardProcess++;
            }

            h_numMuonsFromScalar->Fill(numMuonsFromScalar);
            h_numMuonsPromptFinalState->Fill(numMuonsPromptFinalState);
            h_numMuonsHardProcess->Fill(numMuonsHardProcess);
            

            if ( looseMuonIndex.size() != 2 ) continue;

            if ( !getDileptonCand( event, looseMuonIndex) ) continue;

            if ( (event.zPairLeptons.first + event.zPairLeptons.second).M() > invZMassCut_ ) continue;

            eventWeight *= datasetWeight;

            const int index1 {event.zPairIndex.first}, index2 {event.zPairIndex.second};

            h_relIsoVsEta1->Fill( event.zPairLeptons.first.Eta(),  event.zPairRelIso.first, eventWeight );
            h_relIsoVsEta2->Fill( event.zPairLeptons.second.Eta(), event.zPairRelIso.second, eventWeight );

            h_dxyVsDz1->Fill ( event.muonPF2PATDZPV[index1], event.muonPF2PATDBPV[index1], eventWeight );
            h_dxyVsDz2->Fill ( event.muonPF2PATDZPV[index2], event.muonPF2PATDBPV[index2], eventWeight );

            const int genMuonFromScalar1 { event.genMuonPF2PATMotherId[index1] == 9000006 ? 1 : 0 };
            const int genMuonFromScalar2 { event.genMuonPF2PATMotherId[index2] == 9000006 ? 1 : 0 };

            p_leadingMuonsProfile->Fill( 1.0, genMuonFromScalar1 );
      	    p_leadingMuonsProfile->Fill( 2.0, event.genMuonPF2PATPromptFinalState[index1] );
      	    p_leadingMuonsProfile->Fill( 3.0, event.genMuonPF2PATHardProcess[index1] );

            p_subleadingMuonsProfile->Fill( 1.0, genMuonFromScalar2 );
      	    p_subleadingMuonsProfile->Fill( 2.0, event.genMuonPF2PATPromptFinalState[index2] );
      	    p_subleadingMuonsProfile->Fill( 3.0, event.genMuonPF2PATHardProcess[index2] );

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_numMuons->Write();
    h_numMuonsFromScalar->Write();
    h_numMuonsPromptFinalState->Write();
    h_numMuonsHardProcess->Write();

    h_relIsoVsEta1->Write();
    h_relIsoVsEta2->Write();
    h_dxyVsDz1->Write();
    h_dxyVsDz2->Write();

    p_leadingMuonsProfile->Write();
    p_subleadingMuonsProfile->Write();

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

bool getDileptonCand(AnalysisEvent& event, const std::vector<int> muons) {
    if (muons.size() == 2) {

        if (event.muonPF2PATCharge[muons[0]] * event.muonPF2PATCharge[muons[1]] >= 0) return false;

        TLorentzVector lepton1{event.muonPF2PATPX[muons[0]],
                               event.muonPF2PATPY[muons[0]],
                               event.muonPF2PATPZ[muons[0]],
                               event.muonPF2PATE[muons[0]]};
        TLorentzVector lepton2{event.muonPF2PATPX[muons[1]],
                               event.muonPF2PATPY[muons[1]],
                               event.muonPF2PATPZ[muons[1]],
                               event.muonPF2PATE[muons[1]]};

        event.zPairLeptons.first = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
        event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[0] : muons[1];
        event.zPairRelIso.first = event.muonPF2PATComRelIsodBeta[muons[0]];
        event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
        event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[1]];
        event.zPairIndex.second = lepton1.Pt() > lepton2.Pt() ? muons[1] : muons[0];
        return true;
    }
    return false;
}
