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
#include <ROOT/RDataFrame.hxx>

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
#include <typeinfo>


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

// debug flag
bool verbose = false;

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;
    bool usePostLepTree {false};
		int flow, fhigh;
    std::string fweights = "";
   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    bool ishack_;
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
    hists_1d_["h_AvgMass_mumu_hh"]               = new TH1F("h_AvgMass_mumu_hh","",1000,0.,10.);

    hists_1d_["Cutflow"]                         = new TH1F ("Cutflow", "Cutflow distribution", 7, 0.5, 7.5);
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(1, "GeneratedEvts");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(2, "HLT_IsoMu27");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(3, "MET Filters");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(4, "#mu selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(5, "dimuon candidate");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(6, "chs Trk selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(7, "dichs candidate");

    hists_2d_["h_leadingMuon_RelIso_leadingHadron_RelIso"]               = new TH2F("h_leadingMuon_RelIso_leadingHadron_RelIso", "", 5000, 0., 10., 5000, 0., 10.);
    hists_2d_["h_leadingMuon_SumPtCh_leadingHadron_SumPtCh"]             = new TH2F("h_leadingMuon_SumPtCh_leadingHadron_SumPtCh","", 5000, 0., 100., 5000, 0., 100.);    
    hists_2d_["h_leadingMuon_SumPtNh_leadingHadron_SumPtNh"]             = new TH2F("h_leadingMuon_SumPtNh_leadingHadron_SumPtNh","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_leadingMuon_SumPtPh_leadingHadron_SumPtPh"]             = new TH2F("h_leadingMuon_SumPtPh_leadingHadron_SumPtPh","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_leadingMuon_SumPtPU_leadingHadron_SumPtPU"]             = new TH2F("h_leadingMuon_SumPtPU_leadingHadron_SumPtPU","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_subleadingMuon_RelIso_subleadingHadron_RelIso"]               = new TH2F("h_subleadingMuon_RelIso_subleadingHadron_RelIso", "", 5000, 0., 10., 5000, 0., 10.);
    hists_2d_["h_subleadingMuon_SumPtCh_subleadingHadron_SumPtCh"]             = new TH2F("h_subleadingMuon_SumPtCh_subleadingHadron_SumPtCh","", 5000, 0., 100., 5000, 0., 100.);    
    hists_2d_["h_subleadingMuon_SumPtNh_subleadingHadron_SumPtNh"]             = new TH2F("h_subleadingMuon_SumPtNh_subleadingHadron_SumPtNh","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_subleadingMuon_SumPtPh_subleadingHadron_SumPtPh"]             = new TH2F("h_subleadingMuon_SumPtPh_subleadingHadron_SumPtPh","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_subleadingMuon_SumPtPU_subleadingHadron_SumPtPU"]             = new TH2F("h_subleadingMuon_SumPtPU_subleadingHadron_SumPtPU","", 5000, 0., 100., 5000, 0., 100.);
    hists_2d_["h_DiMuonMass_DiChHadMass"]               = new TH2F("h_DiMuonMass_DiChHadMass", "", 1000, 0., 10., 1000, 0., 10.);


    

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
				"fhigh", po::value<int>(&fhigh)->default_value(10000), "Ending file no. -")(
        "weight,w",
        po::value<std::string>(&fweights)->default_value(fweights),
        "The weights file to be used. Put "" for none")(
        "hack",
        po::value<bool>(&ishack_)->default_value(false), "Pass directly to dataset conf processing");
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
        // std::vector<std::string> tempconf;
        // tempconf.push_back(config);
        // std::cout<<"See config here -"<<config<<","<<typeid(tempconf).name()<<std::endl;
        // Parser::parse_files(tempconf,datasets,totalLumi,usePostLepTree); //temporary hack, find a clean way for everything
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
    std::vector<std::string> filenames;
    TH1D histA;
		// Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
      // auto dataset=datasets.begin();
      filenames.clear();
      for (const auto& location : dataset->locations()) {
        const fs::path dir{location};
        if (fs::is_directory(dir))
            //chain->Add(TString{location + "*.root"});
            {for (int i=flow; i<=fhigh; i++) filenames.push_back(location + Form("output_%d.root",i)); }
        else {
            std::cout << "ERROR: " << location << "is not a valid directory" << std::endl;
        }
      }
      ROOT::RDataFrame d(dataset->treeName().c_str(),filenames);
      histA = d.Histo1D("numMuonPF2PAT").GetValue();
      // auto scalarAncestry = []() {

      // };
      // bool SharedFunctions::scalarAncestry (const AnalysisEvent& event, const Int_t& k, const Int_t& ancestorId, const bool verbose = false) {
      //     const Int_t pdgId        { std::abs(event.genParId[k]) };
      //     const Int_t numDaughters { event.genParNumDaughters[k] };
      //     const Int_t motherId     { std::abs(event.genParMotherId[k]) };
      //     const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };
      //     // if (verbose) std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
      //     if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
      //     else if (motherId == std::abs(ancestorId)) return true; // if parent is ancestor, return true
      //     else if ((pdgId != 130) && (pdgId != 310) && (pdgId != 311) && (motherId != pdgId)) {
      //       return false; // if parent is not an excited state of same particle, return false except in the case of neutral kaons
      //     }
      //     else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
      //     else {
              
      // //        debugCounter++;
      // //        std::cout << "debugCounter: " << debugCounter << std::endl;
              
      //         //if (verbose) std::cout << "Enters right condition ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
      //         return scalarAncestry(event, motherIndex, ancestorId, verbose); // otherwise check mother's mother ...
      //     }
      // }

    } // end dataset loop
		TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    // Write Histos
    histA.Write();

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
    hists_2d_["h_leadingMuon_RelIso_leadingHadron_RelIso"]->Write();
    hists_2d_["h_leadingMuon_SumPtCh_leadingHadron_SumPtCh"]->Write();
    hists_2d_["h_leadingMuon_SumPtNh_leadingHadron_SumPtNh"]->Write();
    hists_2d_["h_leadingMuon_SumPtPh_leadingHadron_SumPtPh"]->Write();
    hists_2d_["h_leadingMuon_SumPtPU_leadingHadron_SumPtPU"]->Write();
    hists_2d_["h_subleadingMuon_RelIso_subleadingHadron_RelIso"]->Write();
    hists_2d_["h_subleadingMuon_SumPtCh_subleadingHadron_SumPtCh"]->Write();
    hists_2d_["h_subleadingMuon_SumPtNh_subleadingHadron_SumPtNh"]->Write();
    hists_2d_["h_subleadingMuon_SumPtPh_subleadingHadron_SumPtPh"]->Write();
    hists_2d_["h_subleadingMuon_SumPtPU_subleadingHadron_SumPtPU"]->Write();
    hists_2d_["h_DiMuonMass_DiChHadMass"]->Write();
    hists_1d_["h_AvgMass_mumu_hh"]->Write();
    
    float norm = hists_1d_["Cutflow"]->GetBinContent(1);
    hists_1d_["Cutflow"]->Scale(1./norm);
    // for(int i=1;i<=hists_1d_["Cutflow"]->GetNbinsX();i++) hists_1d_["Cutflow"]->SetBinContent(i,hists_1d_["Cutflow"]->GetBinContent(i)/norm);
		hists_1d_["Cutflow"]->Write();
    outFile->Close();

//  std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

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
void bookHists(const std::string &prefix) {
	//Some kinematics
  hists_1d_["h_"+prefix+"Pt"]                       = new TH1F(Form("h_%sPt",prefix.c_str()), "", 5000, 0., 150.);
  hists_1d_["h_"+prefix+"Eta"]                       = new TH1F(Form("h_%sEta",prefix.c_str()), "", 5000, -3., 3.);
  hists_1d_["h_"+prefix+"Phi"]                       = new TH1F(Form("h_%sPhi",prefix.c_str()), "", 5000, -4,4);
	//Histograms to study isolation
  hists_1d_["h_"+prefix+"RelIso"]                       = new TH1F(Form("h_%sRelIso",prefix.c_str()), "", 5000, 0., 10.);
  hists_1d_["h_"+prefix+"TrackIso"]                     = new TH1F(Form("h_%sTrackIso",prefix.c_str()), "", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtCh"]                   = new TH1F(Form("h_%sSumPtCh",prefix.c_str()),"", 5000, 0., 100.);    
  hists_1d_["h_"+prefix+"SumPtChRel"]                   = new TH1F(Form("h_%sSumPtChRel",prefix.c_str()),"", 5000, 0., 10.);    
  hists_1d_["h_"+prefix+"SumPtNh"]                   = new TH1F(Form("h_%sSumPtNh",prefix.c_str()),"", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtPh"]                  = new TH1F(Form("h_%sSumPtPh",prefix.c_str()),"", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtPU"]                      = new TH1F(Form("h_%sSumPtPU",prefix.c_str()),"", 5000, 0., 100.);
  //TH1F* h_DiMuonDeltaR                {new TH1F("h_DiMuonDeltaR", "", 100, 0., 1.)};
  hists_1d_["h_"+prefix+"TrksInCone"]                   = new TH1F(Form("h_%sTrksInCone",prefix.c_str()), "", 100, 0., 100.);
	hists_1d_["h_"+prefix+"ChHadFromPVdR"]                = new TH1F(Form("h_%sChHadFromPVdR",prefix.c_str()), "", 1000, 0., 5.);
	//Investigation at Zero Iso
	hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]            = new TH1F(Form("h_%sTrackIsoAtZeroIso",prefix.c_str()), "", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]          = new TH1F(Form("h_%sSumPtChAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]          = new TH1F(Form("h_%sSumPtNhAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
  hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]         = new TH1F(Form("h_%sSumPtPhAtZeroIso",prefix.c_str()),"", 5000, 0., 100.);
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
  TLorentzVector particle{event.muonPF2PATPX[idx], event.muonPF2PATPY[idx], event.muonPF2PATPZ[idx], event.muonPF2PATE[idx]};
	hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.muonPF2PATTrackIso[idx] ,eventweight);
  if (prefix.find("sub") == std::string::npos) {
    hists_1d_["h_"+prefix+"RelIso"]->Fill(event.zPairRelIso.first ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.zPairChIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.zPairChIso.first/particle.Pt()  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.zPairNhIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.zPairPhIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.zPairPuIso.first  ,eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.zPairRelIso.first, particle.Pt(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.zPairRelIso.first, event.zPairLeptons.first.DeltaR(event.zPairLeptons.second),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.zPairRelIso.first, nTrksInCone(particle,event,0.4),eventweight);
	
  }
  else {
    hists_1d_["h_"+prefix+"RelIso"]->Fill(event.zPairRelIso.second ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.zPairChIso.second ,eventweight);
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.zPairChIso.second/particle.Pt()  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.zPairNhIso.second  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.zPairPhIso.second  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.zPairPuIso.second  ,eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.zPairRelIso.second, particle.Pt(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.zPairRelIso.second, event.zPairLeptons.first.DeltaR(event.zPairLeptons.second),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.zPairRelIso.second, nTrksInCone(particle,event,0.4),eventweight);
  }
	hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
	hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
	hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
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
    hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]->Fill(event.muonPF2PATChHadIso[idx] ,eventweight);
    hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]->Fill(event.muonPF2PATNtHadIso[idx] ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]->Fill(event.muonPF2PATGammaIso[idx] ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.muonPF2PATPuIso[idx] ,eventweight); 
    //h_DiMuonDeltaRAtZeroIso->Fill(event.zPairLeptons.first.DeltaR(event.zPairLeptons.second));
    hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  
  }
} 

void fillChHadHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight){
  int pair_idx = prefix.find("sub") == std::string::npos ? 0 : 1; // npos is "not found" and 0 is leading
  TLorentzVector particle{event.packedCandsPx[idx], event.packedCandsPy[idx], event.packedCandsPz[idx], event.packedCandsE[idx]};
  if (prefix.find("sub") == std::string::npos) {
	  hists_1d_["h_"+prefix+"RelIso"]->Fill(event.chsPairRelIso.first,eventweight);        
	  hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.chsPairTrkIso.first ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.chsPairChIso.first ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.chsPairChIso.first/particle.Pt() ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.chsPairNhIso.first ,eventweight); //neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.chsPairPhIso.first ,eventweight); //photons
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.first ,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.first, particle.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.first, event.chsPairVec.first.DeltaR(event.chsPairVec.second),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.first, nTrksInCone(particle,event,0.4),eventweight);
	  if (event.chsPairRelIso.first == 0.) {
      hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.chsPairTrkIso.first ,eventweight);
      hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]->Fill(event.chsPairChIso.first ,eventweight);
      hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]->Fill(event.chsPairNhIso.first ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]->Fill(event.chsPairPhIso.first ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.chsPairPuIso.first ,eventweight); 
    }
  }
  else {
	  hists_1d_["h_"+prefix+"RelIso"]->Fill(event.chsPairRelIso.second,eventweight);        
	  hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.chsPairTrkIso.second,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.chsPairChIso.second ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.chsPairChIso.second/particle.Pt() ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.chsPairNhIso.second ,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.chsPairPhIso.second ,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.second ,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.second, particle.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.second, event.chsPairVec.first.DeltaR(event.chsPairVec.second),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.second, nTrksInCone(particle,event,0.4),eventweight);
	  if (event.chsPairRelIso.second == 0.) {
      hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.chsPairTrkIso.second,eventweight);
      hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]->Fill(event.chsPairChIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]->Fill(event.chsPairNhIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]->Fill(event.chsPairPhIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.chsPairPuIso.second ,eventweight); 
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
  hists_1d_["h_"+prefix+"SumPtCh"]->Write();   
  hists_1d_["h_"+prefix+"SumPtChRel"]->Write();   
  hists_1d_["h_"+prefix+"SumPtNh"]->Write();
  hists_1d_["h_"+prefix+"SumPtPh"]->Write();
  hists_1d_["h_"+prefix+"SumPtPU"]->Write();                    
  hists_1d_["h_"+prefix+"TrksInCone"]->Write();                 
  hists_1d_["h_"+prefix+"ChHadFromPVdR"]->Write();              
  //Investigation at Zero Iso
  // hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Write();          
  // hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]->Write();      
  // hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]->Write();        
  // hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]->Write();     
  // hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Write();           
  // hists_1d_["h_"+prefix+"PtInConeAtZeroIso"]->Write();          
  // hists_1d_["h_"+prefix+"EtaInConeAtZeroIso"]->Write();         
  // hists_1d_["h_"+prefix+"PhiInConeAtZeroIso"]->Write();         
  // hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]->Write();        
  
  hists_1d_["h_"+prefix+"PdgIdInConeAtZeroIso"]->Write();            
  hists_2d_["h_"+prefix+"RelIso_dR"]->Write();                  
  hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Write();
  hists_2d_["h_"+prefix+"ChHadFromPVdR_ChHadPt"]->Write();      
}

