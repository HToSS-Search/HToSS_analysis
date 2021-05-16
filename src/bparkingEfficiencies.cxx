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
    // numerator - bParking
    TH1F* h_leadingMuonPt_truth_bParkingTrig           {new TH1F("h_leadingMuonPt_truth_bParkingTrig",      "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_truth_bParkingTrig        {new TH1F("h_subLeadingMuonPt_truth_bParkingTrig",   "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonPt_bParkingTrig                 {new TH1F("h_leadingMuonPt_bParkingTrig",            "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_subLeadingMuonPt_bParkingTrig              {new TH1F("h_subLeadingMuonPt_bParkingTrig",         "Trigger turn-on for signal; p_{T} (GeV); trigger #epsilon", 200, 0., 100.)};
    TH1F* h_leadingMuonEta_truth_bParkingTrig          {new TH1F("h_leadingMuonEta_truth_bParkingTrig",     "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_truth_bParkingTrig       {new TH1F("h_subLeadingMuonEta_truth_bParkingTrig",  "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_leadingMuonEta_bParkingTrig                {new TH1F("h_leadingMuonEta_bParkingTrig",           "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_subLeadingMuonEta_bParkingTrig             {new TH1F("h_subLeadingMuonEta_bParkingTrig",        "Trigger turn-on for signal; #eta; trigger #epsilon", 300, -3., 3.)};
    TH1F* h_delR_truth_bParkingTrig                    {new TH1F("h_delR_truth_bParkingTrig",               "Trigger turn-on for signal; #Delta R; trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_delR_bParkingTrig                          {new TH1F("h_delR_bParkingTrig",                     "Trigger turn-on for signal; #Delta R; trigger #epsilon", 100, 0., 1.0)};
    TH1F* h_diMuonMass_truth_bParkingTrig              {new TH1F("h_diMuonMass_truth_bParkingTrig",         "Trigger turn-on for signal; m_{#mu#mu}; trigger #epsilon", 200, 0., 100.)};
    TH1F* h_diMuonMass_bParkingTrig                    {new TH1F("h_diMuonMass_bParkingTrig",               "Trigger turn-on for signal; m_{#mu#mu}; trigger #epsilon", 200, 0., 100.)};

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

        AnalysisEvent event{dataset->isMC(), datasetChain, false, true};

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
                if (event.bParkingTrig()) {
                    h_leadingMuonPt_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu1]);
                    h_subLeadingMuonPt_truth_bParkingTrig->Fill(event.muonPF2PATPt[mu2]);
                    h_leadingMuonPt_bParkingTrig->Fill(event.muonPF2PATPt[0]);
                    h_subLeadingMuonPt_bParkingTrig->Fill(event.muonPF2PATPt[1]);
                    h_leadingMuonEta_truth_bParkingTrig->Fill(event.muonPF2PATEta[mu1]);
                    h_subLeadingMuonEta_truth_bParkingTrig->Fill(event.muonPF2PATEta[mu2]);
                    h_leadingMuonEta_bParkingTrig->Fill(event.muonPF2PATEta[0]);
                    h_subLeadingMuonEta_bParkingTrig->Fill(event.muonPF2PATEta[1]);
                    h_delR_truth_bParkingTrig->Fill(delR_truth);
                    h_delR_bParkingTrig->Fill(delR);
                    h_diMuonMass_truth_bParkingTrig->Fill(mass_truth);
                    h_diMuonMass_bParkingTrig->Fill(mass);
                }
            }

        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_leadingMuonPt_truth_bParkingTrig->Divide(h_leadingMuonPt_truth);
    h_subLeadingMuonPt_truth_bParkingTrig->Divide(h_subLeadingMuonPt_truth);
    h_leadingMuonPt_bParkingTrig->Divide(h_leadingMuonPt);
    h_subLeadingMuonPt_bParkingTrig->Divide(h_subLeadingMuonPt);
    h_leadingMuonEta_truth_bParkingTrig->Divide(h_leadingMuonEta_truth);
    h_subLeadingMuonEta_truth_bParkingTrig->Divide(h_subLeadingMuonEta_truth);
    h_leadingMuonEta_bParkingTrig->Divide(h_leadingMuonEta);
    h_subLeadingMuonEta_bParkingTrig->Divide(h_subLeadingMuonEta);
    h_delR_truth_bParkingTrig->Divide(h_delR_truth);
    h_delR_bParkingTrig->Divide(h_delR);
    h_diMuonMass_truth_bParkingTrig->Divide(h_diMuonMass_truth);
    h_diMuonMass_bParkingTrig->Divide(h_diMuonMass);

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
