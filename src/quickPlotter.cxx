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
std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt );
bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const bool exactlyTwo);

bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);

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
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Gen Histos

    TProfile* p_kaonAncestry           {new TProfile("p_kaonAncestry", "Intermediate ancestry of charged kaons from scalar decays", 5, 0.5, 5.5)};

    p_kaonAncestry->GetXaxis()->SetBinLabel(1, "No D meson");
    p_kaonAncestry->GetXaxis()->SetBinLabel(2, "D inc");
    p_kaonAncestry->GetXaxis()->SetBinLabel(3, "D#pm");
    p_kaonAncestry->GetXaxis()->SetBinLabel(4, "D0");
    p_kaonAncestry->GetXaxis()->SetBinLabel(5, "D^{#pm}_{S}");

    // Reco Histos
    TH1F* h_numMuons                   {new TH1F("h_numMuons",                 "number of reco muons in the event", 21, -0.5, 20.5)};
    TH1F* h_numMuonsFromScalar         {new TH1F("h_numMuonsFromScalar",       "number of reco muons with motherId = scalarId", 21, -0.5, 20.5)};
    TH1F* h_numMuonsPromptFinalState   {new TH1F("h_numMuonsPromptFinalState", "number of reco muons with PromptFinalState flag", 21, -0.5, 20.5)};
    TH1F* h_numMuonsHardProcess        {new TH1F("h_numMuonsHardProcess",      "number of reco muons with HardProcess flag", 21, -0.5, 20.5)};

    TProfile* p_hardMuonsIdProfile         {new TProfile("p_hardMuonsIdProfile", "ID decisions for muons from hard process", 9, 0.5, 9.5)};
    TProfile* p_softMuonsIdProfile         {new TProfile("p_softMuonsIdProfile", "ID decisions for muons not from hard process", 9, 0.5, 9.5)};

    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(1, "Pass PF ID");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(2, "Pass Loose Cut ID");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(3, "Pass Medium Cut ID");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(4, "Pass Tight Cut ID");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(5, "Pass Very Loose Iso Cut");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(6, "Pass Loose Iso Cut");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(7, "Pass Medium Iso Cut");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(8, "Pass Tight Iso Cut");
    p_hardMuonsIdProfile->GetXaxis()->SetBinLabel(9, "Pass Very Tight Iso Cut");

    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(1, "Pass PF ID");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(2, "Pass Loose Cut ID");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(3, "Pass Medium Cut ID");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(4, "Pass Tight Cut ID");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(5, "Pass Very Loose Iso Cut");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(6, "Pass Loose Iso Cut");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(7, "Pass Medium Iso Cut");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(8, "Pass Tight Iso Cut");
    p_softMuonsIdProfile->GetXaxis()->SetBinLabel(9, "Pass Very Tight Iso Cut");

    TH1F* h_promptMuonPt               {new TH1F("h_promptMuonPt",       "prompt muon p_{T}; p_{T} (GeV);", 400, 0., 200.)};
    TH1F* h_promptMuonEta              {new TH1F("h_promptMuonEta",      "prompt muon #eta; #eta;", 120, 0., 3.2)};
    TH1F* h_promptMuonPhi              {new TH1F("h_promptMuonPhi",      "prompt muon #phi; #phi;", 300, -3.2, 3.2)};
    TH1F* h_promptMuonRelIso           {new TH1F("h_promptMuonRelIso",   "prompt muon rel iso; rel iso;", 500, 0., 1.)};
    TH1F* h_promptMuonDz               {new TH1F("h_promptMuonDz",       "prompt muon d_{z}; d_{z} (cm);", 500, 0., 1.)};
    TH1F* h_promptMuonDxy              {new TH1F("h_promptMuonDxy",      "prompt muon d_{xy}; d_{xy} (cm);", 500, 0., 1.)};
    TH1F* h_promptMuonPdgId            {new TH1F("h_promptMuonPdgId",    "prompt muon matched genPar Id", 501, -0.5, 500.5)};
    TH1F* h_promptMuonMotherId         {new TH1F("h_promptMuonMotherId", "prompt muon matched genPar mother Id", 502, -1.5, 500.5)};

    TH1F* h_nonpromptMuonPt            {new TH1F("h_nonpromptMuonPt",       "nonprompt muon p_{T}; p_{T} (GeV);", 400, 0., 200.)};
    TH1F* h_nonpromptMuonEta           {new TH1F("h_nonpromptMuonEta",      "nonprompt muon #eta; #eta;", 120, 0., 3.2)};
    TH1F* h_nonpromptMuonPhi           {new TH1F("h_nonpromptMuonPhi",      "nonprompt muon #phi; #phi;", 300, -3.2, 3.2)};
    TH1F* h_nonpromptMuonRelIso        {new TH1F("h_nonpromptMuonRelIso",   "nonprompt muon rel iso; rel iso;", 500, 0., 1.)};
    TH1F* h_nonpromptMuonDz            {new TH1F("h_nonpromptMuonDz",       "nonprompt muon d_{z}; d_{z} (cm);", 500, 0., 1.)};
    TH1F* h_nonpromptMuonDxy           {new TH1F("h_nonpromptMuonDxy",      "nonprompt muon d_{xy}; d_{xy} (cm);", 500, 0., 1.)};
    TH1F* h_nonpromptMuonPdgId         {new TH1F("h_nonpromptMuonPdgId",    "nonprompt muon matched genPar Id", 501, -0.5, 500.5)};
    TH1F* h_nonpromptMuonMotherId      {new TH1F("h_nonpromptMuonMotherId", "nonprompt muon matched genPar mother Id", 502, -1.5, 500.5)};

    TH1F* h_recoDimuonMass      {new TH1F("h_recoDimuonMass", "Dimuon reco mass", 30, 0., 11.)};
    TH1F* h_recoDimuonPt        {new TH1F("h_recoDimuonPt",   "Dimuon reco Pt",  200, 0., 200)}; 
    TH1F* h_recoDimuonEta       {new TH1F("h_recoDimuonEta",  "Dimuon reco Eta", 60, 0., 5.)};

    TH2F* h_relIsoVsEta1        {new TH2F("h_relIsoVsEta1", "rel iso vs leading lepton #eta; #eta; rel iso" , 300, -3., 3., 100, 0., 1.0)};
    TH2F* h_relIsoVsEta2        {new TH2F("h_relIsoVsEta2", "rel iso vs subleading lepton #eta; #eta; rel iso" , 300, -3., 3., 100, 0., 1.0)};
    TH2F* h_dxyVsDz1            {new TH2F("h_dxyVsDz1", "dxy vs dz leading lepton; d_{xy} (cm); d_{z} (cm)", 500, 0., 1., 500, 0., 1.)};
    TH2F* h_dxyVsDz2            {new TH2F("h_dxyVsDz2", "dxy vs dz leading lepton; d_{xy} (cm); d_{z} (cm)", 500, 0., 1., 500, 0., 1.)};

    TProfile* p_leadingMuonsProfile     {new TProfile("p_leadingMuonsProfile", "", 3, 0.5, 3.5)};
    TProfile* p_subleadingMuonsProfile  {new TProfile("p_subleadingMuonsProfile", "", 3, 0.5, 3.5)};

    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(1, "Scalar parentage");
    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(2, "PromptFinalState");
    p_leadingMuonsProfile->GetXaxis()->SetBinLabel(3, "HardProcess");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(1, "Scalar parentage");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(2, "PromptFinalState");
    p_subleadingMuonsProfile->GetXaxis()->SetBinLabel(3, "HardProcess");

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
            if (!dataset->fillChain(datasetChain)) {
                std::cerr << "There was a problem constructing the chain for " << dataset->name() << ". Continuing with next dataset.\n";
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

            // gen particle loop
            for ( Int_t k{0}; k < event.nGenPar; k++ ) {
                const int pid { std::abs(event.genParId[k]) };
                if (pid != 321) continue;

                const bool isScalarGrandparent {scalarGrandparent(event,k,9000006)}; 
                if ( !isScalarGrandparent ) continue;

                const bool isChargedDmesonGrandparent {scalarGrandparent(event,k, 411)};
                const bool isNeutralDmesonGrandparent {scalarGrandparent(event,k, 421)};
                const bool isChargedDshortGrandparent {scalarGrandparent(event,k, 431)};

                p_kaonAncestry->Fill(1.0, (!isChargedDmesonGrandparent && !isNeutralDmesonGrandparent && !isChargedDshortGrandparent) );
                p_kaonAncestry->Fill(2.0, (isChargedDmesonGrandparent || isNeutralDmesonGrandparent || isChargedDshortGrandparent) );
                p_kaonAncestry->Fill(3.0,isChargedDmesonGrandparent);
                p_kaonAncestry->Fill(4.0,isNeutralDmesonGrandparent);
                p_kaonAncestry->Fill(5.0,isChargedDshortGrandparent);
          
            }

            float eventWeight = 1.;

            if (!event.metFilters()) continue;
            if (! ( event.mumuTrig() || event.muTrig() ) ) continue;

            std::vector<int> looseMuonIndex = getLooseMuons(event);

            h_numMuons->Fill(looseMuonIndex.size());

            std::vector<int> promptLooseMuonIndex     = getPromptMuons(event, looseMuonIndex, true);
            std::vector<int> nonpromptLooseMuonIndex  = getPromptMuons(event, looseMuonIndex, false);

            int numMuonsFromScalar{0}, numMuonsPromptFinalState{0}, numMuonsHardProcess{0};

            // reco muon loop
            for ( auto it = looseMuonIndex.begin(); it != looseMuonIndex.end(); it++ ) {
      	       	if (event.genMuonPF2PATMotherId[*it] == 9000006) numMuonsFromScalar++;
                if (event.genMuonPF2PATPromptFinalState[*it]) numMuonsPromptFinalState++;

                int motherId = std::abs(event.genMuonPF2PATMotherId[*it]);
                if (motherId == 9000006) motherId = -1.;

      	       	if (event.genMuonPF2PATHardProcess[*it]) {
                    numMuonsHardProcess++;

                    p_hardMuonsIdProfile->Fill(1.0, event.muonPF2PATIsPFMuon[*it]);
                    p_hardMuonsIdProfile->Fill(2.0, event.muonPF2PATLooseCutId[*it]);
                    p_hardMuonsIdProfile->Fill(3.0, event.muonPF2PATMediumCutId[*it]);
                    p_hardMuonsIdProfile->Fill(4.0, event.muonPF2PATTightCutId[*it]);
                    p_hardMuonsIdProfile->Fill(5.0, event.muonPF2PATPfIsoVeryLoose[*it]);
                    p_hardMuonsIdProfile->Fill(6.0, event.muonPF2PATPfIsoLoose[*it]);
                    p_hardMuonsIdProfile->Fill(7.0, event.muonPF2PATPfIsoMedium[*it]);
                    p_hardMuonsIdProfile->Fill(8.0, event.muonPF2PATPfIsoTight[*it]);
                    p_hardMuonsIdProfile->Fill(9.0, event.muonPF2PATPfIsoVeryTight[*it]);

                    h_promptMuonPt->Fill(event.muonPF2PATPt[*it]);
                    h_promptMuonEta->Fill(std::abs(event.muonPF2PATEta[*it]));
                    h_promptMuonPhi->Fill(event.muonPF2PATPhi[*it]);
                    h_promptMuonRelIso->Fill(event.muonPF2PATComRelIsodBeta[*it]);
                    h_promptMuonDz->Fill(event.muonPF2PATDZPV[*it]);
                    h_promptMuonDxy->Fill(event.muonPF2PATDBPV[*it]);
                    h_promptMuonPdgId->Fill(std::abs(event.genMuonPF2PATPdgId[*it]));
                    h_promptMuonMotherId->Fill(motherId);
                }
                else {
                    p_softMuonsIdProfile->Fill(1.0, event.muonPF2PATIsPFMuon[*it]);
                    p_softMuonsIdProfile->Fill(2.0, event.muonPF2PATLooseCutId[*it]);
                    p_softMuonsIdProfile->Fill(3.0, event.muonPF2PATMediumCutId[*it]);
                    p_softMuonsIdProfile->Fill(4.0, event.muonPF2PATTightCutId[*it]);
                    p_softMuonsIdProfile->Fill(5.0, event.muonPF2PATPfIsoVeryLoose[*it]);
                    p_softMuonsIdProfile->Fill(6.0, event.muonPF2PATPfIsoLoose[*it]);
                    p_softMuonsIdProfile->Fill(7.0, event.muonPF2PATPfIsoMedium[*it]);
                    p_softMuonsIdProfile->Fill(8.0, event.muonPF2PATPfIsoTight[*it]);
                    p_softMuonsIdProfile->Fill(9.0, event.muonPF2PATPfIsoVeryTight[*it]);

                    h_nonpromptMuonPt->Fill(event.muonPF2PATPt[*it]);
                    h_nonpromptMuonEta->Fill(std::abs(event.muonPF2PATEta[*it]));
                    h_nonpromptMuonPhi->Fill(event.muonPF2PATPhi[*it]);
                    h_nonpromptMuonRelIso->Fill(event.muonPF2PATComRelIsodBeta[*it]);
                    h_nonpromptMuonDz->Fill(event.muonPF2PATDZPV[*it]);
                    h_nonpromptMuonDxy->Fill(event.muonPF2PATDBPV[*it]);
                    h_nonpromptMuonPdgId->Fill(std::abs(event.genMuonPF2PATPdgId[*it]));
                    h_nonpromptMuonMotherId->Fill(motherId);
                }
            } // end reco muon loop

            h_numMuonsFromScalar->Fill(numMuonsFromScalar);
            h_numMuonsPromptFinalState->Fill(numMuonsPromptFinalState);
            h_numMuonsHardProcess->Fill(numMuonsHardProcess);
            

            if ( looseMuonIndex.size() != 2 ) continue;

            bool exactlyTwo = false;

            if ( !getDileptonCand( event, looseMuonIndex, exactlyTwo) ) continue;

            if ( (event.zPairLeptons.first + event.zPairLeptons.second).M() > invZMassCut_ ) continue;

            eventWeight *= datasetWeight;

            const int index1 {event.zPairIndex.first}, index2 {event.zPairIndex.second};

            h_recoDimuonMass->Fill( (event.zPairLeptons.first+event.zPairLeptons.second).M() );
            h_recoDimuonPt->Fill( (event.zPairLeptons.first+event.zPairLeptons.second).Pt() );
            h_recoDimuonEta->Fill( (event.zPairLeptons.first+event.zPairLeptons.second).Eta() );

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

    p_kaonAncestry->Write();

    h_numMuons->Write();
    h_numMuonsFromScalar->Write();
    h_numMuonsPromptFinalState->Write();
    h_numMuonsHardProcess->Write();

    p_hardMuonsIdProfile->Write();
    p_softMuonsIdProfile->Write();

    h_promptMuonPt->Write();
    h_promptMuonEta->Write();
    h_promptMuonPhi->Write();
    h_promptMuonRelIso->Write();
    h_promptMuonDz->Write();
    h_promptMuonDxy->Write();
    h_promptMuonPdgId->Write();
    h_promptMuonMotherId->Write();
    h_nonpromptMuonPt->Write();
    h_nonpromptMuonEta->Write();
    h_nonpromptMuonPhi->Write();
    h_nonpromptMuonRelIso->Write();
    h_nonpromptMuonDz->Write();
    h_nonpromptMuonDxy->Write();
    h_nonpromptMuonPdgId->Write();
    h_nonpromptMuonMotherId->Write();


    h_recoDimuonMass->Write();
    h_recoDimuonPt->Write();
    h_recoDimuonEta->Write();

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

std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt ) {
    std::vector<int> muons;
    for ( auto it = muonIndex.begin(); it!= muonIndex.end(); it++ ) {
        if ( event.genMuonPF2PATHardProcess[*it] == getPrompt ) muons.push_back(*it);
    }
    return muons;
}

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const bool exactlyTwo) {
    if (exactlyTwo && muons.size() == 2) {

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
    else if (!exactlyTwo && muons.size() > 1) {
        double closestMass {9999.};
        for ( unsigned int i{0}; i < muons.size(); i++ ) {
            for ( unsigned int j{i+1}; j < muons.size(); j++ ) {
                if (event.muonPF2PATCharge[i] * event.muonPF2PATCharge[j] >= 0) continue;
                TLorentzVector lepton1{event.muonPF2PATPX[i],
                                       event.muonPF2PATPY[i],
                                       event.muonPF2PATPZ[i],
                                       event.muonPF2PATE[i]};
                TLorentzVector lepton2{event.muonPF2PATPX[j],
                                       event.muonPF2PATPY[j],
                                       event.muonPF2PATPZ[j],
                                       event.muonPF2PATE[j]};
                double invMass { (lepton1+lepton2).M() };
                if ( std::abs(( invMass - 2.0 )) < std::abs(closestMass) ) {
                    event.zPairLeptons.first = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                    event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[0] : muons[1];
                    event.zPairRelIso.first = event.muonPF2PATComRelIsodBeta[muons[0]];
                    event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                    event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[1]];
                    event.zPairIndex.second = lepton1.Pt() > lepton2.Pt() ? muons[1] : muons[0];
                    closestMass = ( invMass - 2.0 );
                }
            }
        }
        if ( closestMass < 9999. ) return true;
    }
    return false;
}


bool scalarGrandparent (const AnalysisEvent& event, const Int_t& k, const Int_t& grandparentId) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };


    if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
    else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
    else if (motherIndex > event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
    else {
//        std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
        return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
    }
}

