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

std::vector<int> getLooseMuons(const AnalysisEvent& event, const bool& mcTruth);
std::vector<int> getPromptMuons(const AnalysisEvent& event, const std::vector<int>& muonIndex, const bool getPrompt );
bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const float scalarMass, const bool exactlyTwo);
bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chs, const float scalarMass);
bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);
float deltaR(float eta1, float phi1, float eta2, float phi2);

namespace fs = boost::filesystem;

// Lepton cut variables
//const float looseMuonEta_ {2.8}, looseMuonPt_ {6.}, looseMuonPtLeading_ {15.}, looseMuonRelIso_ {100.};
const float looseMuonEta_ {2.8}, looseMuonPt_ {0.}, looseMuonPtLeading_ {0.}, looseMuonRelIso_ {100.};
const float invZMassCut_ {10.0};

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
    float scalarMass_;
    bool exactlyTwo_ {false};
    bool mcTruth_ {false};
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

    // Quick and dirty plots
    TH1F* h_leadingMuonPt        { new TH1F("h_leadingMuonPt",          "", 200, 0., 100.)};
    TH1F* h_subleadingMuonPt     { new TH1F("h_subleadingMuonPt",       "", 200, 0., 100.)};
    TH1F* h_muonDeltaR           { new TH1F("h_muonDeltaR",             "", 500, 0., 1.)}; 
    TH1F* h_dimuonPt             { new TH1F("h_dimuonPt",               "", 400, 0., 200.)};
    TH1F* h_muonPtOverDeltaR     { new TH1F("h_muonPtOverDeltaR",       "", 5000, 0., 10.)};
    TH2F* h_muonPtOverDeltaR2D   { new TH2F("h_muonPtOverDeltaR2D",     "", 200, 0., 100., 500, 0., 10.)};
    TH1F* h_diMuonMass           { new TH1F("h_diMuonMass",             "", 100, 0., 4.0)};

    TH1F* h_leadingChsPt         { new TH1F("h_leadingChsPt", 	        "", 200, 0., 100.)};
    TH1F* h_subleadingChsPt      { new TH1F("h_subleadingChsPt",         "", 200, 0., 100.)};
    TH1F* h_chsDeltaR            { new TH1F("h_chsDeltaR",              "", 500, 0., 10.)};
    TH1F* h_diChsPt              { new TH1F("h_diChsPt",                "", 400, 0., 200.)};
    TH1F* h_diChsPtOverDeltaR    { new TH1F("h_diChsPtOverDeltaR",      "", 5000, 0., 10.)};
    TH2F* h_diChsPtOverDeltaR2D  { new TH2F("h_diChsPtOverDeltaR2D",    "", 200, 0., 100., 500, 0., 10.)};
    TH1F* h_diChsMass            { new TH1F("h_diChsMass",              "", 100, 0., 4.0)};

    TH1F* h_scalarDeltaR         { new TH1F("h_scalarDeltaR",           "", 500, 0., 10.)};
    TH1F* h_scalarMass           { new TH1F("h_scalarMass",             "", 200, 75., 175.)};

    // post trigger

    TH1F* ht_leadingMuonPt       { new TH1F("ht_leadingMuonPt",         "", 200, 0., 100.)};
    TH1F* ht_subleadingMuonPt    { new TH1F("ht_subleadingMuonPt",      "", 200, 0., 100.)};
    TH1F* ht_muonDeltaR          { new TH1F("ht_muonDeltaR",            "", 500, 0., 1.)}; 
    TH1F* ht_dimuonPt            { new TH1F("ht_dimuonPt",              "", 400, 0., 200.)};
    TH1F* ht_muonPtOverDeltaR    { new TH1F("ht_muonPtOverDeltaR",      "", 5000, 0., 10.)};
    TH2F* ht_muonPtOverDeltaR2D  { new TH2F("ht_muonPtOverDeltaR2D",    "", 200, 0., 100., 500, 0., 10.)};
    TH1F* ht_diMuonMass          { new TH1F("ht_diMuonMass",            "", 100, 0., 4.0)};

    TH1F* ht_leadingChsPt        { new TH1F("ht_leadingChsPt",          "", 200, 0., 100.)};
    TH1F* ht_subleadingChsPt     { new TH1F("ht_subleadingChsPt",        "", 200, 0., 100.)};
    TH1F* ht_chsDeltaR           { new TH1F("ht_chsDeltaR",             "", 500, 0., 10.)};
    TH1F* ht_diChsPt             { new TH1F("ht_diChsPt",               "", 400, 0., 200.)};
    TH1F* ht_diChsPtOverDeltaR   { new TH1F("ht_diChsPtOverDeltaR",     "", 5000, 0., 10.)};
    TH2F* ht_diChsPtOverDeltaR2D { new TH2F("ht_diChsPtOverDeltaR2D",  "", 200, 0., 100., 500, 0., 10.)};
    TH1F* ht_diChsMass           { new TH1F("ht_diChsMass",             "", 100, 0., 4.0)};

    TH1F* ht_scalarDeltaR        { new TH1F("ht_scalarDeltaR",          "", 500, 0., 10.)};
    TH1F* ht_scalarMass          { new TH1F("ht_scalarMass",            "", 200, 75., 175.)};

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
        "scalarMass,s",
        po::value<float>(&scalarMass_)->default_value(2.0),
        "scalar mass being searched for.")(
        ",e", po::bool_switch(&exactlyTwo_), "only use events with exactly two loose muons");
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
    const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + (is2016_ ? "2016" : "2017") + "/"};
//    const std::string postLepSelSkimInputDir{std::string{"/user/almorton/HToSS_analysis/postLepSkims"} + (is2016_ ? "2016" : "2017") + "/"};

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

            const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
            const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

            const bool passTriggers ( event.muTrig() || event.mumuTrig() || event.mumuL2Trig() || event.mumuNoVtxTrig() );

            // Get muons

            std::vector<int> looseMuonIndex = getLooseMuons(event, mcTruth_);

            if ( looseMuonIndex.size() < 2 ) continue;

            getDileptonCand( event, looseMuonIndex, scalarMass_, exactlyTwo_);

//            const int index1 {event.zPairIndex.first}, index2 {event.zPairIndex.second};
            const TLorentzVector muon1Vec {event.zPairLeptons.first}, muon2Vec {event.zPairLeptons.second};

            // Get CHS
            std::vector<int> chsIndex;
            for (Int_t k = 0; k < event.numPackedCands; k++) {
                if (event.packedCandsPdgId[k] != 211) continue;
                if (event.packedCandsCharge[k] == 0 ) continue;
                if (event.packedCandsHasTrackDetails[k] < 1 ) continue;
                if (mcTruth_ && !event.genJetPF2PATScalarAncestor[event.packedCandsJetIndex[k]]) continue;
                chsIndex.emplace_back(k);
            }

            getDihadronCand( event, chsIndex, scalarMass_);

            const TLorentzVector chs1Vec{event.chsPairVec.first}, chs2Vec{event.chsPairVec.second};

            h_leadingMuonPt->Fill(muon1Vec.Pt());
            h_subleadingMuonPt->Fill(muon2Vec.Pt());
            h_muonDeltaR->Fill(muon1Vec.DeltaR(muon2Vec));
            h_dimuonPt->Fill( (muon1Vec + muon2Vec).Pt() );
            h_muonPtOverDeltaR->Fill( ((muon1Vec + muon2Vec).Pt())/(muon1Vec.DeltaR(muon2Vec) + 1.0e-06) );
            h_muonPtOverDeltaR2D->Fill( (muon1Vec + muon2Vec).Pt(), muon1Vec.DeltaR(muon2Vec) );
            h_diMuonMass->Fill((muon1Vec + muon2Vec).M() );

            h_leadingChsPt->Fill(chs1Vec.Pt());
      	    h_subleadingChsPt->Fill(chs2Vec.Pt());
      	    h_chsDeltaR->Fill(chs1Vec.DeltaR(chs2Vec));
      	    h_diChsPt->Fill( (chs1Vec+chs2Vec).Pt() );
      	    h_diChsPtOverDeltaR->Fill( ((chs1Vec+chs2Vec).Pt())/ (chs1Vec.DeltaR(chs2Vec) + 1.0e-06) );
      	    h_diChsPtOverDeltaR2D->Fill( (chs1Vec+chs2Vec).Pt(), chs1Vec.DeltaR(chs2Vec) );
            h_diChsMass->Fill( (chs1Vec+chs2Vec).M() );

            h_scalarDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR( (chs1Vec+chs2Vec) ) );
            h_scalarMass->Fill( (muon1Vec+muon2Vec+chs1Vec+chs2Vec).M() );

            if ( passTriggers ) {
                ht_leadingMuonPt->Fill(muon1Vec.Pt());
                ht_subleadingMuonPt->Fill(muon2Vec.Pt());
                ht_muonDeltaR->Fill(muon1Vec.DeltaR(muon2Vec));
                ht_dimuonPt->Fill( (muon1Vec + muon2Vec).Pt() );
                ht_muonPtOverDeltaR->Fill( ((muon1Vec + muon2Vec).Pt())/(muon1Vec.DeltaR(muon2Vec) + 1.0e-06) );
                ht_muonPtOverDeltaR2D->Fill( (muon1Vec + muon2Vec).Pt(), muon1Vec.DeltaR(muon2Vec) );
                ht_diMuonMass->Fill((muon1Vec + muon2Vec).M() );

                ht_leadingChsPt->Fill(chs1Vec.Pt());
                ht_subleadingChsPt->Fill(chs2Vec.Pt());
      	        ht_chsDeltaR->Fill(chs1Vec.DeltaR(chs2Vec));
                ht_diChsPt->Fill( (chs1Vec+chs2Vec).Pt() );
      	        ht_diChsPtOverDeltaR->Fill( ((chs1Vec+chs2Vec).Pt())/ (chs1Vec.DeltaR(chs2Vec) + 1.0e-06) );
                ht_diChsPtOverDeltaR2D->Fill( (chs1Vec+chs2Vec).Pt(), chs1Vec.DeltaR(chs2Vec) );
                ht_diChsMass->Fill( (chs1Vec+chs2Vec).M() );

                ht_scalarDeltaR->Fill( (muon1Vec+muon2Vec).DeltaR( (chs1Vec+chs2Vec) ) );
                ht_scalarMass->Fill( (muon1Vec+muon2Vec+chs1Vec+chs2Vec).M() );
            }
            
        } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_leadingMuonPt->Write();
    h_subleadingMuonPt->Write();
    h_muonDeltaR->Write();    
    h_dimuonPt->Write();
    h_muonPtOverDeltaR->Write();
    h_muonPtOverDeltaR2D->Write();   
    h_diMuonMass->Write();

    h_leadingChsPt->Write();
    h_subleadingChsPt->Write();
    h_chsDeltaR->Write();
    h_diChsPt->Write();
    h_diChsPtOverDeltaR->Write();
    h_diChsPtOverDeltaR2D->Write();
    h_diChsMass->Write();

    h_scalarDeltaR->Write();
    h_scalarMass->Write();

    // post trigger plots
    ht_leadingMuonPt->Write();
    ht_subleadingMuonPt->Write();
    ht_muonDeltaR->Write();    
    ht_dimuonPt->Write();
    ht_muonPtOverDeltaR->Write();
    ht_muonPtOverDeltaR2D->Write();   
    ht_diMuonMass->Write();

    ht_leadingChsPt->Write();
    ht_subleadingChsPt->Write();
    ht_chsDeltaR->Write();
    ht_diChsPt->Write();
    ht_diChsPtOverDeltaR->Write();
    ht_diChsPtOverDeltaR2D->Write();
    ht_diChsMass->Write();

    ht_scalarDeltaR->Write();
    ht_scalarMass->Write();


    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

std::vector<int> getLooseMuons(const AnalysisEvent& event, const bool& mcTruth) {
    std::vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {
       if ( mcTruth && event.genMuonPF2PATPdgId[i] != 9000006 ) continue;
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

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons, const float skMass, const bool exactlyTwo) {
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
                if ( std::abs(( invMass - skMass )) < std::abs(closestMass) ) {
                    event.zPairLeptons.first = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                    event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
                    event.zPairRelIso.first = event.muonPF2PATComRelIsodBeta[muons[i]];
                    event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                    event.zPairRelIso.second = event.muonPF2PATComRelIsodBeta[muons[j]];
                    event.zPairIndex.second = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
                    closestMass = ( invMass - skMass );
                }
            }
        }
        if ( closestMass < 9999. ) return true;
    }
    return false;
}

bool getDihadronCand(AnalysisEvent& event, const std::vector<int>& chs, const float skMass) {

    if (chs.size() > 1) {
        double closestMass {9999.};
        for ( unsigned int i{0}; i < chs.size(); i++ ) {
            for ( unsigned int j{i+1}; j < chs.size(); j++ ) {
                if (event.packedCandsCharge[i] * event.packedCandsCharge[j] >= 0) continue;
                TLorentzVector lepton1{event.packedCandsPseudoTrkPx[i],
                                       event.packedCandsPseudoTrkPy[i],
                                       event.packedCandsPseudoTrkPz[i],
                                       event.packedCandsE[i]};
                TLorentzVector lepton2{event.packedCandsPseudoTrkPx[j],
                                       event.packedCandsPseudoTrkPy[j],
                                       event.packedCandsPseudoTrkPz[j],
                                       event.packedCandsE[j]};
                double invMass { (lepton1+lepton2).M() };
                if ( std::abs(( invMass - skMass )) < std::abs(closestMass) ) {
                    event.chsPairVec.first = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                    event.chsPairIndex.first = lepton1.Pt() > lepton2.Pt() ? chs[i] : chs[j];
                    event.chsPairVec.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                    event.chsPairIndex.second = lepton1.Pt() > lepton2.Pt() ? chs[i] : chs[j];
                    closestMass = ( invMass - skMass );
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

float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}

