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


int nTrksInCone(TLorentzVector& particle, const AnalysisEvent& event, double dr_max);
void bookHists(const std::string &prefix);
void fillMuonHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight);
void fillChHadHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight);
void writeHists(const std::string &prefix);

namespace fs = boost::filesystem;
std::map<std::string, TH1F*> hists_1d_;
std::map<std::string, TH2F*> hists_2d_;	

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.}; //leading muon is 30, previously 5
const float invZMassCut_ {10.0}, chsMass_{0.13957018};

// Diparticle cuts
double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4}; // previously 0.2 for dilepton (unsure why)
double higgsTolerence_ {1.};

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
		int flow, fhigh;
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};

		// muons
    
    bookHists("leadingMuon");
    bookHists("subleadingMuon");
    bookHists("leadingChHad");
    bookHists("subleadingChHad");
    hists_1d_["h_DiMuonDeltaR"]                  = new TH1F("h_DiMuonDeltaR", "", 1000, 0., 10.);
    hists_1d_["h_DiMuonPt"]                      = new TH1F("h_DiMuonPt", "", 1000, 0., 150.);
    hists_1d_["h_DiMuonMass"]                    = new TH1F("h_DiMuonMass", "", 1000, 0., 10.);
    hists_1d_["h_DiMuonDeltaRAtZeroIso"]         = new TH1F("h_DiMuonDeltaRAtZeroIso", "", 100, 0., 1.);
    hists_1d_["h_DiChHadDeltaR"]                 = new TH1F("h_DiChHadDeltaR", "", 1000, 0., 10.);
    hists_1d_["h_DiChHadPt"]                     = new TH1F("h_DiChHadPt", "", 1000, 0., 150.);
    hists_1d_["h_DiChHadMass"]                   = new TH1F("h_DiChHadMass", "", 1000, 0., 10.);
    hists_1d_["h_DiChHadDeltaRAtZeroIso"]        = new TH1F("h_DiChHadDeltaRAtZeroIso", "", 100, 0., 1.);
    hists_1d_["Cutflow"]                         = new TH1F ("Cutflow", "Cutflow distribution", 7, 0.5, 7.5);
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(1, "GeneratedEvts");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(2, "HLT_IsoMu27");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(3, "MET Filters");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(4, "#mu selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(5, "dimuon candidate");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(6, "chs Trk selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(7, "dichs candidate");
		const char *PDGs[8] = {"ele","mu","photon","charged hadron from PV","charged hadron from PU","neutral hadron","HF hadron","HF EM"};
		//std::map<char,std::map<int,int>> PDG_map = {{"ele",{11,1}},{"mu",{13,2}},{"photon",{22,3}},{"charged hadron from PV",{211,4}},{"charged hadron from PU",{211,5}},{"neutral hadron",{130,6}},{"HF hadron",{1,7}},{"HF EM",{2,8}}};
	  std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
		
    // hadrons
    
    
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
				"fhigh", po::value<int>(&fhigh)->default_value(10000), "Ending file no. -");
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
    std::cout<<"usePostLepTree :"<<usePostLepTree<<std::endl;
    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
      datasetFilled = false;
      TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
      datasetChain->SetAutoSave(0);
      
      std::cerr << "Processing dataset " << dataset->name() << std::endl;
      if (!usePostLepTree) {
	      if (!datasetFilled) {
	        if (!dataset->fillChain(datasetChain,flow,fhigh)) {
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
      TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ..."}}; 
      lEventTimer->DrawProgressBar(0, "");
      
      totalEvents += numberOfEvents;
      for (Long64_t i{0}; i < numberOfEvents; i++) {
	
	      lEventTimer->DrawProgressBar(i,"");
	     	if (i%1000==0) std::cout<<"Events processed - "<<i<<std::endl; 
	      event.GetEntry(i);
	      
	      float eventWeight = 1.;
	      //eventWeight *= datasetWeight;
        hists_1d_["Cutflow"]->Fill(1, eventWeight);

	      const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
	      const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};

      //        if (! ( passDimuonTrigger || passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger ) ) continue;
	      if ( !passSingleMuonTrigger ) continue;
        hists_1d_["Cutflow"]->Fill(2, eventWeight);
	      if (!event.metFilters()) continue;
        hists_1d_["Cutflow"]->Fill(3, eventWeight);
	      
	      SharedFunctions shf{false};
       
        // Below for muons 
       /*event selection criteria for leptons*/
	      event.muonIndexLoose = shf.getLooseMuons(event);
	      if ( event.muonIndexLoose.size() < 2 ) continue;
        hists_1d_["Cutflow"]->Fill(4, eventWeight);

	      if ( !shf.getDileptonCand( event, event.muonIndexLoose ) ) continue;
	      
        hists_1d_["Cutflow"]->Fill(5, eventWeight);
        /*Now analyse*/
        fillMuonHists(event, "leadingMuon", event.zPairIndex.first, eventWeight);
        fillMuonHists(event, "subleadingMuon", event.zPairIndex.second, eventWeight);
	      hists_1d_["h_DiMuonDeltaR"]->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second), eventWeight);
	      hists_1d_["h_DiMuonMass"]->Fill((event.zPairLeptons.first + event.zPairLeptons.second).M(), eventWeight);
	      hists_1d_["h_DiMuonPt"]->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Pt(), eventWeight);
	      //std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;    
	      if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.first] == 0.) {
          hists_1d_["h_DiMuonDeltaRAtZeroIso"]->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second), eventWeight);        
        }
        
	      //std::cout << "Enters event, processes muons" << std::endl;
        //Below for charged hadrons
        /*event selection criteria for hadrons*/
	      event.chsIndex = shf.getChargedHadronTracks(event);
	      if ( event.chsIndex.size() < 2 ) continue;
        hists_1d_["Cutflow"]->Fill(6, eventWeight);
	      //std::cout << "Enters event, found 2 hadrons!" << std::endl;
	      if ( !shf.getDihadronCand( event, event.chsIndex ) ) {
	        std::cout << "Eh, no dihadron found :("<<std::endl;      
	        continue; //fine for now
	      }
	      
        hists_1d_["Cutflow"]->Fill(1, eventWeight);
	      //std::cout << "Enters event, found dihadron object!" << std::endl;
	      /*Now analyse*/
        fillChHadHists(event, "leadingChHad", event.chsPairIndex.first, eventWeight);
        fillChHadHists(event, "subleadingChHad", event.chsPairIndex.second, eventWeight);
	      //std::cout << "Enters event, Filled the histos for hadrons!" << std::endl;
	      hists_1d_["h_DiChHadDeltaR"]->Fill(event.chsPairVec.first.DeltaR(event.chsPairVec.second), eventWeight);
	      hists_1d_["h_DiChHadMass"]->Fill((event.chsPairVec.first + event.chsPairVec.second).M(), eventWeight);
	      hists_1d_["h_DiChHadPt"]->Fill((event.chsPairVec.first + event.chsPairVec.second).Pt(), eventWeight);
	      //std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;    
	      if (event.chsPairRelIso.first == 0.) {
          hists_1d_["h_DiChHadDeltaRAtZeroIso"]->Fill(event.chsPairVec.first.DeltaR(event.chsPairVec.second), eventWeight);        
        }
        
	      //std::cout << "Enters event, processes hadrons" << std::endl;
	
      } // end event loop
    } // end dataset loop

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Write Histos

    // Write muon histos
    writeHists("leadingMuon");
    writeHists("subleadingMuon");
    writeHists("leadingChHad");
    writeHists("subleadingChHad");
    hists_1d_["h_DiMuonDeltaR"]->Write();
		hists_1d_["h_DiMuonPt"]->Write();             
    hists_1d_["h_DiMuonMass"]->Write();
    hists_1d_["h_DiMuonDeltaRAtZeroIso"]->Write();  
    hists_1d_["h_DiChHadDeltaR"]->Write();
		hists_1d_["h_DiChHadPt"]->Write(); 
    hists_1d_["h_DiChHadMass"]->Write();
    hists_1d_["h_DiChHadDeltaRAtZeroIso"]->Write();      
    outFile->Close();

//  std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}
/*
std::vector<int> getLooseMuons(const AnalysisEvent& event) {
    std::vector<int> muons;
    for (int i{0}; i < event.numMuonPF2PAT; i++)  {
       if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] && std::abs(event.muonPF2PATEta[i]) < looseMuonEta_) {
           if ( event.muonPF2PATPt[i] >= (muons.empty() ? looseMuonPtLeading_ : looseMuonPt_)) muons.emplace_back(i); //storing indices
        }
    }
    return muons; //safe to assume pT ordered if PF PAT collection is pT ordered -> pls check
}

std::vector<int> getChargedHadronTracks(const AnalysisEvent& event) {
    std::vector<int> chs;
    for (Int_t k = 0; k < event.numPackedCands; k++) {
        if (std::abs(event.packedCandsPdgId[k]) != 211) continue; //charged hadron
        if (event.packedCandsCharge[k] == 0 ) continue;
        if (event.packedCandsHasTrackDetails[k] != 1 ) continue; //need those tracks
        TLorentzVector lVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};
        if (lVec.Pt() < 1.0) continue; //Pt > 1 GeV

        chs.emplace_back(k);
    }
    return chs;
}*/
/*

bool getDileptonCand(AnalysisEvent& event, const std::vector<int>& muons) {
    //should iterate over muons collection indices
    //for ( unsigned int i{0}; i < muons.size(); i++ ) {
    //    for ( unsigned int j{i+1}; j < muons.size(); j++ ) {
    for (int i : muons) {
        for (int j : muons) {
            //if (j <= i) continue; 
            if (event.muonPF2PATPt[i] <= looseMuonPtLeading_) continue; //since pT ordered, i is highest pT
            if (event.muonPF2PATPt[j] <= looseMuonPt_) continue;

            if (event.muonPF2PATCharge[i] * event.muonPF2PATCharge[j] >= 0) continue;

            TLorentzVector lepton1{event.muonPF2PATPX[i], event.muonPF2PATPY[i], event.muonPF2PATPZ[i], event.muonPF2PATE[i]};
            TLorentzVector lepton2{event.muonPF2PATPX[j], event.muonPF2PATPY[j], event.muonPF2PATPZ[j], event.muonPF2PATE[j]};
            double delR { lepton1.DeltaR(lepton2) };
            if ( delR < maxDileptonDeltaR_  ) {
                if (lepton1.Pt() < lepton2.Pt()) std::cout << "Something fishy with muons" << std::endl; 
                event.zPairLeptons.first  = lepton1.Pt() > lepton2.Pt() ? lepton1 : lepton2;
                event.zPairLeptons.second = lepton1.Pt() > lepton2.Pt() ? lepton2 : lepton1;
                event.zPairIndex.first = lepton1.Pt() > lepton2.Pt() ? i : j;
                event.zPairIndex.second  = lepton1.Pt() > lepton2.Pt() ? j : i;
                event.zPairRelIso.first  = lepton1.Pt() > lepton2.Pt() ? event.muonPF2PATComRelIsodBeta[i] : event.muonPF2PATComRelIsodBeta[j];
                event.zPairRelIso.second = lepton1.Pt() > lepton2.Pt() ? event.muonPF2PATComRelIsodBeta[j] : event.muonPF2PATComRelIsodBeta[i];

                event.mumuTrkIndex = getMuonTrackPairIndex(event);
                //if (event.mumuTrkIndex < 0) std::cout << "Something fishy here too" << std::endl; //some events do not have muon track pairs

//                if ( (event.muonTkPairPF2PATTkVtxChi2[event.mumuTrkIndex])/(event.muonTkPairPF2PATTkVtxNdof[event.mumuTrkIndex]+1.0e-06) > 10. ) continue;

                event.zPairLeptonsRefitted.first  = TLorentzVector{event.muonTkPairPF2PATTk1Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk1Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk1P2[event.mumuTrkIndex]+std::pow(0.1057,2))};
                event.zPairLeptonsRefitted.second = TLorentzVector{event.muonTkPairPF2PATTk2Px[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Py[event.mumuTrkIndex], event.muonTkPairPF2PATTk2Pz[event.mumuTrkIndex], std::sqrt(event.muonTkPairPF2PATTk2P2[event.mumuTrkIndex]+std::pow(0.1057,2))};

                return true;
            }
        }
    }
    return false;
}

bool getDihadronCand(AnalysisEvent& event, std::vector<int>& chs) {
    //std::cout<<"Looking for hadrons from collection of "<<chs.size()<<" hadrons"<<std::endl;
    //std::cout<<"Last hadron at "<<chs[chs.size()-1]<<" hadrons"<<std::endl;
    for (int i : chs) {
		//for ( unsigned int i0{0}; i0 < chs.size(); i0++ ) {
				//int i = chs[i0]; 
        //ensuring that track is hadron and not muon? Removing for now
        //if ( event.packedCandsMuonIndex[i] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue; 
        //if ( event.packedCandsMuonIndex[i] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

        for (int j : chs) {
				//for ( unsigned int j0{i0+1}; j0 < chs.size(); j0++ ) {
						//int j = chs[j0]; 
            //ensuring that track is hadron and not muon? Removing for now
            //if ( event.packedCandsMuonIndex[j] == event.muonPF2PATPackedCandIndex[event.zPairIndex.first] ) continue;
            //if ( event.packedCandsMuonIndex[j] == event.muonPF2PATPackedCandIndex[event.zPairIndex.second] ) continue;

            //if (j <= i) continue; 
            if (event.packedCandsCharge[i] * event.packedCandsCharge[j] >= 0) continue; //OS charges allowed
            //std::cout << "Jet index - "<<i<<", "<<j<< std::endl; 
            TLorentzVector chs1 {event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
            TLorentzVector chs2 {event.packedCandsPx[j], event.packedCandsPy[j], event.packedCandsPz[j], event.packedCandsE[j]};
            //std::cout << "No problem in forming Vectors: "<<i<<", "<<j<< std::endl; 
            double pT { (chs1+chs2).Pt() };
            double delR { chs1.DeltaR(chs2) };
            double higgsMass { (chs1+chs2+event.zPairLeptons.first+event.zPairLeptons.second).M() };
            //if (chs1.Pt() < chs2.Pt()) std::cout << "Something fishy with hadrons" << std::endl; 
            //hadrons are not pt ordered according to above
            //if ( delR < maxChsDeltaR_ && (higgsMass - 125.) < higgsTolerence_ && pT >= 0. ) { //higgs window was applied, removing for now
            
            //std::cout << "dihadron candidate? Check: dR-" <<delR<<",pT-"<< pT  << std::endl;
            if ( delR < maxChsDeltaR_ && pT >= 0. ) {
                event.chsPairVec.first  = chs1.Pt() > chs2.Pt() ? chs1 : chs2;
                event.chsPairVec.second = chs1.Pt() > chs2.Pt() ? chs2 : chs1;
                event.chsPairIndex.first = chs1.Pt() > chs2.Pt() ? i : j;
                event.chsPairIndex.second = chs1.Pt() > chs2.Pt() ? j : i;

                TLorentzVector chsTrk1, chsTrk2; //PseudoTrack is the original reco::Track of candidate
                chsTrk1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.first], event.packedCandsPseudoTrkEta[event.chsPairIndex.first], event.packedCandsPseudoTrkPhi[event.chsPairIndex.first], event.packedCandsE[event.chsPairIndex.first]);
                chsTrk2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[event.chsPairIndex.second], event.packedCandsPseudoTrkEta[event.chsPairIndex.second], event.packedCandsPseudoTrkPhi[event.chsPairIndex.second], event.packedCandsE[event.chsPairIndex.second]);

                event.chsPairTrkVec.first  = chsTrk1;
                event.chsPairTrkVec.second = chsTrk2;

                event.chsPairTrkIndex = getChsTrackPairIndex(event);
                
                //std::cout << "Returns from getChsTrackPairIndex(event)!" << std::endl; 

//                if ( (event.chsTkPairTkVtxChi2[event.chsPairTrkIndex])/(event.chsTkPairTkVtxNdof[event.chsPairTrkIndex]+1.0e-06) > 20. ) continue;

       	       	// If refit fails then reject event - all signal events	pass refit, but	QCD does not
                if ( std::isnan(event.chsTkPairTk1Pt[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2Pt[event.chsPairTrkIndex]) ) {
                  //std::cout << "Fails to find refit tracks!" << std::endl; 
                  return false;
                }
                if ( std::isnan(event.chsTkPairTk1P2[event.chsPairTrkIndex])  || std::isnan(event.chsTkPairTk2P2[event.chsPairTrkIndex]) ) return false;
                if ( std::isnan(event.chsTkPairTk1Phi[event.chsPairTrkIndex]) || std::isnan(event.chsTkPairTk2Phi[event.chsPairTrkIndex]) ) return false;
                
                
                //std::cout << "Finds refit tracks!" << std::endl; 

                TLorentzVector chsTrk1Refitted, chsTrk2Refitted;
                chsTrk1Refitted.SetPtEtaPhiE(event.chsTkPairTk1Pt[event.chsPairTrkIndex], event.chsTkPairTk1Eta[event.chsPairTrkIndex], event.chsTkPairTk1Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk1P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                chsTrk2Refitted.SetPtEtaPhiE(event.chsTkPairTk2Pt[event.chsPairTrkIndex], event.chsTkPairTk2Eta[event.chsPairTrkIndex], event.chsTkPairTk2Phi[event.chsPairTrkIndex], std::sqrt(event.chsTkPairTk2P2[event.chsPairTrkIndex]+std::pow(chsMass_,2)));
                event.chsPairTrkVecRefitted.first  = chsTrk1Refitted;
                event.chsPairTrkVecRefitted.second = chsTrk2Refitted;

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
                            if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.4 )  neutral_iso1 += packedCandVec.Et();
                            if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.4 )  neutral_iso2 += packedCandVec.Et();
                            if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.4 ) neutral_iso += packedCandVec.Et();
                        }
                        if ( packedCandTrkVec.Pt() >= 0.5 ) {
                            if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)  < 0.4 ) neutral_trkiso1 += packedCandTrkVec.Et();
                            if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec) < 0.4 ) neutral_trkiso2 += packedCandTrkVec.Et();
                            if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec) ) neutral_trkiso += packedCandVec.Et();
                        }
                    }
                    else {
                        if ( event.packedCandsFromPV[k] >= 2 ) {
                            if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.4 )  ch_iso1 += packedCandVec.Pt();
                            if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.4 )  ch_iso2 += packedCandVec.Pt();
                            if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.4 ) ch_iso += packedCandVec.Pt();

                            if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)   < 0.4 )  ch_trkiso1 += packedCandTrkVec.Pt();
                            if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec)  < 0.4 )  ch_trkiso2 += packedCandTrkVec.Pt();
                            if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec)  < 0.4 ) ch_trkiso += packedCandTrkVec.Pt();
                        }
                        else {
                            if ( packedCandVec.Pt() >= 0.5 ) {
                                if ( event.chsPairVec.first.DeltaR(packedCandVec)   < 0.4 )  pu_iso1 += packedCandVec.Pt();
                                if ( event.chsPairVec.second.DeltaR(packedCandVec)  < 0.4 )  pu_iso2 += packedCandVec.Pt();
                                if ( (event.chsPairVec.first+event.chsPairVec.second).DeltaR(packedCandVec)  < 0.4 ) pu_iso += packedCandVec.Pt();
                            }
                            if ( packedCandTrkVec.Pt() >= 0.5 ) {
                                if ( event.chsPairTrkVec.first.DeltaR(packedCandTrkVec)   < 0.4 )  pu_trkiso1 += packedCandTrkVec.Pt();
                                if ( event.chsPairTrkVec.second.DeltaR(packedCandTrkVec)  < 0.4 )  pu_trkiso2 += packedCandTrkVec.Pt();
                                if ( (event.chsPairTrkVec.first+event.chsPairTrkVec.second).DeltaR(packedCandTrkVec)  < 0.4 ) pu_trkiso += packedCandTrkVec.Pt();
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
                
                event.chsPairChHadIso.first = ch_iso1;
                event.chsPairNtIso.first = neutral_iso1;
                event.chsPairPuIso.first = pu_iso1;
                
                event.chsPairChHadIso.second = ch_iso2;
                event.chsPairNtIso.second = neutral_iso2;
                event.chsPairPuIso.second = pu_iso2;
                
                //std::cout << "Returns true" << std::endl; 
                return true;
            }
            else continue;
        }
    }
    //std::cout << "Returns false" << std::endl; 
    return false;
}
*/
/*int getMuonTrackPairIndex(const AnalysisEvent& event) { 
// track pairs with opp. charge and has "valid" RecoVtx (implemented via KalmanVertexFitter)
    for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
        if (event.muonTkPairPF2PATIndex1[i] == event.zPairIndex.first && event.muonTkPairPF2PATIndex2[i] == event.zPairIndex.second) return i;
    }
    return -1;
}*/

/*int getChsTrackPairIndex(const AnalysisEvent& event) {
    for (int i{0}; i < event.numChsTrackPairs; i++) {
        if (event.chsTkPairIndex1[i] == event.chsPairIndex.first && event.chsTkPairIndex2[i] == event.chsPairIndex.second) return i;
    } //iterating over track pairs to look for the selected pair of tracks?
    return -1;
}*/

int nTrksInCone(TLorentzVector& particle, const AnalysisEvent& event, double dr_max) {
    int count = 0;
    double tmpdR=999.;
    for (int i=0;i < event.numPackedCands; i++) {
        TLorentzVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        if ((event.packedCandsPdgId[i] != 211) || (event.packedCandsFromPV[i] < 2)) continue;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = particle.DeltaR(trk);
        if ((tmpdR < dr_max) && (tmpdR > 0.03)) count++;// 0.03 cut to ensure not the same track as candidate
    }
    return count;
}
/*
bool scalarGrandparent (const AnalysisEvent& event, const Int_t& k, const Int_t& grandparentId) {

    const Int_t pdgId        { std::abs(event.genParId[k]) };
    const Int_t numDaughters { event.genParNumDaughters[k] };
    const Int_t motherId     { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };


    if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
    else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
    else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
    else {
//        std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
        return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
    }
}
*/
/*
float deltaR(float eta1, float phi1, float eta2, float phi2){
  float dEta = eta1-eta2;
  float dPhi = phi1-phi2;
  while (fabs(dPhi) > 3.14159265359){
    dPhi += (dPhi > 0.? -2*3.14159265359:2*3.14159265359);
  }
  //  if(singleEventInfoDump_)  std::cout << eta1 << " " << eta2 << " phi " << phi1 << " " << phi2 << " ds: " << eta1-eta2 << " " << phi1-phi2 << " dR: " << std::sqrt((dEta*dEta)+(dPhi*dPhi)) << std::endl;
  return std::sqrt((dEta*dEta)+(dPhi*dPhi));
}
*/
void bookHists(const std::string &prefix) {
	//Some kinematics
  hists_1d_["h_"+prefix+"Pt"]                       = new TH1F(Form("h_%sPt",prefix.c_str()), "", 5000, 0., 150.);
  hists_1d_["h_"+prefix+"Eta"]                       = new TH1F(Form("h_%sEta",prefix.c_str()), "", 5000, -3., 3.);
  hists_1d_["h_"+prefix+"Phi"]                       = new TH1F(Form("h_%sPhi",prefix.c_str()), "", 5000, -4,4);
	//Histograms to study isolation
  hists_1d_["h_"+prefix+"RelIso"]                       = new TH1F(Form("h_%sRelIso",prefix.c_str()), "", 5000, 0., 10.);
  hists_1d_["h_"+prefix+"TrackIso"]                     = new TH1F(Form("h_%sTrackIso",prefix.c_str()), "", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtChHad"]                   = new TH1F(Form("h_%sSumPtChHad",prefix.c_str()),"", 5000, 0., 100.);
  if (prefix.find("Muon") != std::string::npos) {
    hists_1d_["h_"+prefix+"SumPtNtHad"]                   = new TH1F(Form("h_%sSumPtNtHad",prefix.c_str()),"", 5000, 0., 100.);
    hists_1d_["h_"+prefix+"SumPtPhoton"]                  = new TH1F(Form("h_%sSumPtPhoton",prefix.c_str()),"", 5000, 0., 100.);
  }
  else hists_1d_["h_"+prefix+"SumPtNt"]                   = new TH1F(Form("h_%sSumPtNt",prefix.c_str()),"", 5000, 0., 100.);  
  
  hists_1d_["h_"+prefix+"SumPtPU"]                      = new TH1F(Form("h_%sSumPtPU",prefix.c_str()),"", 5000, 0., 100.);
  //TH1F* h_DiMuonDeltaR                {new TH1F("h_DiMuonDeltaR", "", 100, 0., 1.)};
  hists_1d_["h_"+prefix+"TrksInCone"]                   = new TH1F(Form("h_%sTrksInCone",prefix.c_str()), "", 100, 0., 100.);
	hists_1d_["h_"+prefix+"ChHadFromPVdR"]                = new TH1F(Form("h_%sChHadFromPVdR",prefix.c_str()), "", 1000, 0., 5.);
	//Investigation at Zero Iso
	hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]            = new TH1F(Form("h_%sTrackIsoAtZeroIso",prefix.c_str()), "", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtChHadAtZeroIso"]          = new TH1F(Form("h_%sSumPtChHadAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
  if (prefix.find("Muon") != std::string::npos) {
    hists_1d_["h_"+prefix+"SumPtNtHadAtZeroIso"]          = new TH1F(Form("h_%sSumPtNtHadAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
    hists_1d_["h_"+prefix+"SumPtPhotonAtZeroIso"]         = new TH1F(Form("h_%sSumPtPhotonAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
  }
  else hists_1d_["h_"+prefix+"SumPtNtAtZeroIso"]                   = new TH1F(Form("h_%sSumPtNtAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);  
  hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]             = new TH1F(Form("h_%sSumPtPUAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"PtInConeAtZeroIso"]            = new TH1F(Form("h_%sPtInConeAtZeroIso",prefix.c_str()), "", 500, 0., 150.);
  hists_1d_["h_"+prefix+"EtaInConeAtZeroIso"]           = new TH1F(Form("h_%sEtaInConeAtZeroIso",prefix.c_str()), "", 100, -5., 5.);
  hists_1d_["h_"+prefix+"PhiInConeAtZeroIso"]           = new TH1F(Form("h_%sPhiInConeAtZeroIso",prefix.c_str()), "", 100, -4., 4.);
  //  TH1F* h_DiMuonDeltaRAtZeroIso               = new TH1F("h_DiMuonDeltaRAtZeroIso", "", 100, 0., 1.)};
  hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]          = new TH1F(Form("h_%sTrksInConeAtZeroIso",prefix.c_str()), "", 100, 0., 100.);
	
	const char *PDGs[8] = {"ele","mu","photon","charged hadron from PV","charged hadron from PU","neutral hadron","HF hadron","HF EM"};
		//std::map<char,std::map<int,int>> PDG_map = {{"ele",{11,1}},{"mu",{13,2}},{"photon",{22,3}},{"charged hadron from PV",{211,4}},{"charged hadron from PU",{211,5}},{"neutral hadron",{130,6}},{"HF hadron",{1,7}},{"HF EM",{2,8}}};
  hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"]         = new TH1F(Form("h_%sPdgIdInConeAtZeroIso",prefix.c_str()), "", 8, 0, 8);
	for (int i=1;i<=8;i++) hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"]->GetXaxis()->SetBinLabel(i,PDGs[i-1]);
       
  hists_2d_["h_"+prefix+"RelIso_dR"]                    = new TH2F(Form("h_%sRelIso_dR",prefix.c_str()),"",5000,0.,10.,100,0.,1.);
	hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]  = new TH2F(Form("h_%sRelIso_%sPt",prefix.c_str()), "", 1000,0.,1.,500, 0., 150.);
  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]  = new TH2F(Form("h_%sRelIso_%sTrksInCone",prefix.c_str(),prefix.c_str()), "", 1000,0.,1.,100, 0., 100.);
	hists_2d_["h_"+prefix+"ChHadFromPVdR_ChHadPt"]        = new TH2F(Form("h_%sChHadFromPVdR_ChHadPt",prefix.c_str(),prefix.c_str()), "", 1000,0.,5., 500, 0., 150.);
}

void fillMuonHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight){
	hists_1d_["h_"+prefix+"RelIso"]->Fill(event.muonPF2PATComRelIsodBeta[idx],eventweight);        
	hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.muonPF2PATTrackIso[idx],eventweight);
  hists_1d_["h_"+prefix+"SumPtChHad"]->Fill(event.muonPF2PATChHadIso[idx],eventweight);                              
  hists_1d_["h_"+prefix+"SumPtNtHad"]->Fill(event.muonPF2PATNtHadIso[idx],eventweight);   
  hists_1d_["h_"+prefix+"SumPtPhoton"]->Fill(event.muonPF2PATGammaIso[idx],eventweight);   
  hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.muonPF2PATPuIso[idx],eventweight); 
  
  TLorentzVector particle{event.muonPF2PATPX[idx], event.muonPF2PATPY[idx], event.muonPF2PATPZ[idx], event.muonPF2PATE[idx]};
	hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
	hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
	hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
  hists_1d_["h_"+prefix+"TrksInCone"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.muonPF2PATComRelIsodBeta[idx], particle.Pt(),eventweight);
	hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.muonPF2PATComRelIsodBeta[idx], event.zPairLeptons.first.DeltaR(event.zPairLeptons.second),eventweight);
	hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.muonPF2PATComRelIsodBeta[idx], nTrksInCone(particle,event,0.4),eventweight);
	//std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;
	
	std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
	double tmpdR = 999.;
	//easier to just loop over tracks here and fill hists directly
	for (int i=0;i < event.numPackedCands; i++) {
        TLorentzVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        //if ((event.packedCandsPdgId[i] == 211) || (event.packedCandsPdgId[i] == 130)) std::cout<<"Found hadrons"<<std::endl;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = particle.DeltaR(trk);
				if ((std::abs(event.packedCandsPdgId[i]) == 211) && (event.packedCandsFromPV[i] >= 2)) {
					hists_1d_["h_"+prefix+"ChHadFromPVdR"]->Fill(tmpdR,eventweight);
					hists_2d_["h_"+prefix+"ChHadFromPVdR_ChHadPt"]->Fill(tmpdR,trk.Pt(),eventweight);
				}
				if (event.muonPF2PATComRelIsodBeta[idx] != 0.) continue; // At Zero Iso
				if ((tmpdR > 0.4) || (tmpdR < 0.03)) continue; // Only if diff. trk within dR0p4
				//if ((event.packedCandsPdgId[i] == 211)
				//if (event.packedCandsPdgId[i]!=211) h_leadingMuonPdgIdInConeAtZeroIso->Fill(PDG_map[event.packedCandsPdgId[i]]);
				if (std::abs(event.packedCandsPdgId[i]) == 211){
					if (event.packedCandsFromPV[i] >= 2) hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(4-0.5,eventweight);
				 	else hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(5-0.5,eventweight); 	
				}  
				else hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(PDG_map[std::abs(event.packedCandsPdgId[i])]-0.5,eventweight);
				hists_1d_["h_"+prefix+"PtInConeAtZeroIso"]->Fill(trk.Pt(),eventweight);
				hists_1d_["h_"+prefix+"EtaInConeAtZeroIso"]->Fill(trk.Eta(),eventweight);
				hists_1d_["h_"+prefix+"PhiInConeAtZeroIso"]->Fill(trk.Phi(),eventweight);
	}

	if (event.muonPF2PATComRelIsodBeta[idx] == 0.) {
    hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.muonPF2PATTrackIso[idx],eventweight);
    hists_1d_["h_"+prefix+"SumPtChHadAtZeroIso"]->Fill(event.muonPF2PATChHadIso[idx],eventweight);
    hists_1d_["h_"+prefix+"SumPtNtHadAtZeroIso"]->Fill(event.muonPF2PATNtHadIso[idx],eventweight);
    hists_1d_["h_"+prefix+"SumPtPhotonAtZeroIso"]->Fill(event.muonPF2PATGammaIso[idx],eventweight);
    hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.muonPF2PATPuIso[idx],eventweight); 
    //h_DiMuonDeltaRAtZeroIso->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
    hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  
  }
} 

void fillChHadHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight){
  int pair_idx = prefix.find("sub") == std::string::npos ? 0 : 1; // npos is "not found" and 0 is leading
  TLorentzVector particle{event.packedCandsPx[idx], event.packedCandsPy[idx], event.packedCandsPz[idx], event.packedCandsE[idx]};
  if (prefix.find("sub") == std::string::npos) {
	  hists_1d_["h_"+prefix+"RelIso"]->Fill(event.chsPairRelIso.first,eventweight);        
	  hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.chsPairTrkIso.first,eventweight);
    hists_1d_["h_"+prefix+"SumPtChHad"]->Fill(event.chsPairChHadIso.first,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNt"]->Fill(event.chsPairNtIso.first,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.first,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.first, particle.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.first, event.chsPairVec.first.DeltaR(event.chsPairVec.second),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.first, nTrksInCone(particle,event,0.4),eventweight);
	  if (event.chsPairRelIso.first == 0.) {
      hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.chsPairTrkIso.first,eventweight);
      hists_1d_["h_"+prefix+"SumPtChHadAtZeroIso"]->Fill(event.chsPairChHadIso.first,eventweight);
      hists_1d_["h_"+prefix+"SumPtNtAtZeroIso"]->Fill(event.chsPairNtIso.first,eventweight);
      hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.chsPairPuIso.first,eventweight); 
    }
  }
  else {
	  hists_1d_["h_"+prefix+"RelIso"]->Fill(event.chsPairRelIso.second,eventweight);        
	  hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.chsPairTrkIso.second,eventweight);
    hists_1d_["h_"+prefix+"SumPtChHad"]->Fill(event.chsPairChHadIso.second,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNt"]->Fill(event.chsPairNtIso.second,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.second,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.second, particle.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.second, event.chsPairVec.first.DeltaR(event.chsPairVec.second),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.second, nTrksInCone(particle,event,0.4),eventweight);
	  if (event.chsPairRelIso.second == 0.) {
      hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.chsPairTrkIso.second,eventweight);
      hists_1d_["h_"+prefix+"SumPtChHadAtZeroIso"]->Fill(event.chsPairChHadIso.second,eventweight);
      hists_1d_["h_"+prefix+"SumPtNtAtZeroIso"]->Fill(event.chsPairNtIso.second,eventweight);
      hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.chsPairPuIso.second,eventweight); 
    }
  }
  hists_1d_["h_"+prefix+"TrksInCone"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  
	//std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;
	
	std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
	double tmpdR = 999.;
	//easier to just loop over tracks here and fill hists directly
	for (int i=0;i < event.numPackedCands; i++) {
        TLorentzVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        //if ((event.packedCandsPdgId[i] == 211) || (event.packedCandsPdgId[i] == 130)) std::cout<<"Found hadrons"<<std::endl;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = particle.DeltaR(trk);
				if ((std::abs(event.packedCandsPdgId[i]) == 211) && (event.packedCandsFromPV[i] >= 2)) {
					hists_1d_["h_"+prefix+"ChHadFromPVdR"]->Fill(tmpdR,eventweight);
					hists_2d_["h_"+prefix+"ChHadFromPVdR_ChHadPt"]->Fill(tmpdR,trk.Pt(),eventweight);
				}
				if (prefix.find("sub") == std::string::npos){
				  if (event.chsPairRelIso.first != 0.) continue; // At Zero Iso
				}
				else {
				  if (event.chsPairRelIso.second != 0.) continue; // At Zero Iso
				}
				if ((tmpdR > 0.4) || (tmpdR < 0.03)) continue; // Only if diff. trk within dR0p4
				// another condition here to exclude the other hadron leg from contributing
				if ( i == event.chsPairIndex.first || i == event.chsPairIndex.second ) continue;
				//if ((event.packedCandsPdgId[i] == 211)
				//if (event.packedCandsPdgId[i]!=211) h_leadingMuonPdgIdInConeAtZeroIso->Fill(PDG_map[event.packedCandsPdgId[i]]);
				if (std::abs(event.packedCandsPdgId[i]) == 211){
					if (event.packedCandsFromPV[i] >= 2) hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(4-0.5,eventweight);
				 	else hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(5-0.5,eventweight); 	
				}  
				else hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"] ->Fill(PDG_map[std::abs(event.packedCandsPdgId[i])]-0.5,eventweight);
				hists_1d_["h_"+prefix+"PtInConeAtZeroIso"]->Fill(trk.Pt(),eventweight);
				hists_1d_["h_"+prefix+"EtaInConeAtZeroIso"]->Fill(trk.Eta(),eventweight);
				hists_1d_["h_"+prefix+"PhiInConeAtZeroIso"]->Fill(trk.Phi(),eventweight);
	}
}
void writeHists(const std::string &prefix) {
	hists_1d_["h_"+prefix+"Pt"]->Write();
	hists_1d_["h_"+prefix+"Eta"]->Write();
	hists_1d_["h_"+prefix+"Phi"]->Write();
	hists_1d_["h_"+prefix+"RelIso"]->Write();                       
  hists_1d_["h_"+prefix+"TrackIso"]->Write();                   
  hists_1d_["h_"+prefix+"SumPtChHad"]->Write();   
  if (prefix.find("Muon") != std::string::npos) {
    hists_1d_["h_"+prefix+"SumPtNtHad"]->Write();
    hists_1d_["h_"+prefix+"SumPtPhoton"]->Write();
  }
  else hists_1d_["h_"+prefix+"SumPtNt"]->Write();
  hists_1d_["h_"+prefix+"SumPtPU"]->Write();                    
  hists_1d_["h_"+prefix+"TrksInCone"]->Write();                 
  hists_1d_["h_"+prefix+"ChHadFromPVdR"]->Write();              
  //Investigation at Zero Iso
  hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Write();          
  hists_1d_["h_"+prefix+"SumPtChHadAtZeroIso"]->Write();  
  if (prefix.find("Muon") != std::string::npos) {      
    hists_1d_["h_"+prefix+"SumPtNtHadAtZeroIso"]->Write();        
    hists_1d_["h_"+prefix+"SumPtPhotonAtZeroIso"]->Write();     
  }
  else hists_1d_["h_"+prefix+"SumPtNtAtZeroIso"]->Write();  
  hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Write();           
  hists_1d_["h_"+prefix+"PtInConeAtZeroIso"]->Write();          
  hists_1d_["h_"+prefix+"EtaInConeAtZeroIso"]->Write();         
  hists_1d_["h_"+prefix+"PhiInConeAtZeroIso"]->Write();         
  hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]->Write();        
  
  hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"]->Write();            
  hists_2d_["h_"+prefix+"RelIso_dR"]->Write();                  
  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Write();
  hists_2d_["h_"+prefix+"ChHadFromPVdR_ChHadPt"]->Write();      
}

