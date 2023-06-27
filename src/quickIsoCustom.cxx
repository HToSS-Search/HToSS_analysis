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
#include "Math/SMatrix.h"
#include "TF1.h"


#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/program_options.hpp>
#include <boost/range/iterator_range.hpp>
#include <yaml-cpp/yaml.h>


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


int nTrksInCone(ROOT::Math::PxPyPzMVector& particle, const AnalysisEvent& event, double dr_max);
void bookHists(const std::string &prefix);
void fillMuonHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight);
void fillChHadHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight);
void writeHists(const std::string &prefix);

namespace fs = boost::filesystem;
std::map<std::string, TH1F*> hists_1d_;
std::map<std::string, TH2F*> hists_2d_;	

// Lepton cut variables
const float looseMuonEta_ {2.4}, looseMuonPt_ {5.}, looseMuonPtLeading_ {30.}, looseMuonRelIso_ {100.}; //leading muon is 30, previously 5
const float invZMassCut_ {10.0};
float chsMass_{0.13957018};

// Diparticle cuts
double maxDileptonDeltaR_ {0.4}, maxChsDeltaR_ {0.4}; // previously 0.2 for dilepton (unsure why)
double higgsTolerence_ {1.};

// debug flag
bool verbose = false;

int main(int argc, char* argv[]) {
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::string cuts_config;
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
    hists_1d_["h_recoHiggsMass"]                 = new TH1F("h_recoHiggsMass", "", 20000, 0., 1000.);
    hists_1d_["h_DiChHadVtxSignificance"]        = new TH1F("h_DiChHadVtxSignificance", "", 2000, 0., 1000.);
    hists_1d_["h_DiMuonVtxSignificance"]         = new TH1F("h_DiMuonVtxSignificance", "", 2000, 0., 1000.);
    hists_1d_["h_DiChHadVtxLxy"]                 = new TH1F("h_DiChHadVtxLxy", "", 5000, 0., 1000.);
    hists_1d_["h_DiMuonVtxLxy"]                  = new TH1F("h_DiMuonVtxLxy", "", 5000, 0., 1000.);
    hists_1d_["h_DiChHadVtxSigma"]               = new TH1F("h_DiChHadVtxSigma", "", 1000, 0., 500.);
    hists_1d_["h_DiMuonVtxSigma"]                = new TH1F("h_DiMuonVtxSigma", "", 1000, 0., 500.);
    //before application of cut
    hists_1d_["h_leadingMuon_RelIso_BC"]         = new TH1F("h_leadingMuon_RelIso", "", 5000, 0., 10.);
    hists_1d_["h_leadingChHad_RelIso_BC"]        = new TH1F("h_leadingChHad_RelIso_BC", "", 5000, 0., 10.);
    hists_1d_["h_subleadingMuon_RelIso_BC"]      = new TH1F("h_subleadingMuon_RelIso_BC", "", 5000, 0., 10.);
    hists_1d_["h_subleadingChHad_RelIso_BC"]     = new TH1F("h_subleadingChHad_RelIso_BC", "", 5000, 0., 10.);
    hists_1d_["h_DiChHadPt_BC"]                  = new TH1F("h_DiChHadPt_BC", "", 1000, 0., 150.);
    hists_1d_["h_recoHiggsMass_BC"]              = new TH1F("h_recoHiggsMass_BC", "", 20000, 0., 1000.);
    



    // Lxy 2d distribution, error distribution, also fix the binning of reco-higgs mass


    hists_1d_["Cutflow"]                         = new TH1F ("Cutflow", "Cutflow distribution", 9, 0.5, 9.5);
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(1, "GeneratedEvts");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(2, "HLT_IsoMu27");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(3, "MET Filters");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(4, "#mu selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(5, "dimuon candidate");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(6, "hadron trk selection");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(7, "dihadron candidate");
    // hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(8, "dichs Pt>20 GeV");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(8, "higgs mass");
    hists_1d_["Cutflow"]->GetXaxis()->SetBinLabel(9, "scalar mass");

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
    hists_2d_["h_DiMuonVtxSignificance_DiChHadVtxSignificance"]         = new TH2F("h_DiMuonVtxSignificance_DiChHadVtxSignificance", "", 2000, 0., 1000., 2000, 0., 1000.);
    hists_2d_["h_DiMuonVtxLxy_DiChHadVtxLxy"]         = new TH2F("h_DiMuonVtxLxy_DiChHadVtxLxy", "", 5000, 0., 1000., 5000, 0., 1000.);
    hists_2d_["h_DiMuonVtxLxy_DiMuonVtxSigma"]               = new TH2F("h_DiMuonVtxLxy_DiMuonVtxSigma", "", 5000, 0., 1000., 1000, 0., 500.);
    hists_2d_["h_DiChHadVtxLxy_DiChHadVtxSigma"]               = new TH2F("h_DiChHadVtxLxy_DiChHadVtxSigma", "", 5000, 0., 1000., 1000, 0., 500.);
    // hists_1d_["h_DiMuonVtxSigma"]                = new TH1F("h_DiMuonVtxSigma", "", 500, 0., 100.);
    //before application of cut
    hists_2d_["h_DiMuonMass_DiChHadMass_BC"]               = new TH2F("h_DiMuonMass_DiChHadMass_BC", "", 1000, 0., 10., 1000, 0., 10.);






    

		const char *PDGs[8] = {"ele","mu","photon","charged hadron from PV","charged hadron from PU","neutral hadron","HF hadron","HF EM"};
		//std::map<char,std::map<int,int>> PDG_map = {{"ele",{11,1}},{"mu",{13,2}},{"photon",{22,3}},{"charged hadron from PV",{211,4}},{"charged hadron from PU",{211,5}},{"neutral hadron",{130,6}},{"HF hadron",{1,7}},{"HF EM",{2,8}}};
	  std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
		
    // hadrons
    
    
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")("cuts",
        po::value<std::string>(&cuts_config)->required(),
        "The cuts file to be used.")(
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
                            //  cuts,
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
		std::ofstream isotail_evts;
		std::ofstream masstail_evts;
		isotail_evts.open ("interesting_events_iso.txt");
		masstail_evts.open ("interesting_events_mass.txt");
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
      // extract the dataset weight. MC = (lumi*crossSection), data = 1.0
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
      TFile* f_wts;
      float sumWeights = 0.;
      if ((dataset->isMC())&&(fweights!="")&&(dataset->isOldNtuple())) {
        f_wts = new TFile(fweights.c_str(),"READ");
        TH1F* h_weights = (TH1F*)f_wts->Get("weightHisto");
        sumWeights += h_weights->GetBinContent(1) - h_weights->GetBinContent(2);
      }
      if ((dataset->isMC())) std::cout<<"Sum of weights (Nevts):"<<sumWeights<<std::endl;
      if (sumWeights<=0) std::cout<<"Something wrong with the sum of weights! Check pls."<<std::endl; //temporary measure until new ntuples

      for (Long64_t i{0}; i < numberOfEvents; i++) {

				verbose = false;
	
	      lEventTimer->DrawProgressBar(i,"");
	     	if (i%1000==0) std::cout<<"Events processed - "<<i<<std::endl; 
	      event.GetEntry(i);
        if (event.eventRun == 1 && event.eventLumiblock == 9 && event.eventNum == 10548) {
          std::cout<<"Enters verbose check"<<std::endl;
          verbose=true;
        }
        // std::cout<<"Pointer smh inside main:"<<&hists_1d_<<std::endl;
        SharedFunctions shf{false, verbose, &hists_1d_, &hists_2d_};
	      //else SharedFunctions shf{false, true};
	      float eventWeight = 1.;
        if (dataset->isMC()) {
          //modification for old ntuple
          if (dataset->isOldNtuple()&&(sumWeights>0)) eventWeight *= (event.processMCWeight/sumWeights); //*evtweight/nevts
          else eventWeight *= event.processMCWeight; //*weight
          eventWeight *= datasetWeight; //multiply cs*L/sumWeights (for not old ntuples) for old, cs*L
        }
        event.eventWeight = eventWeight;
        // std::cout<<"eventWeight, processMCWeight, sumWeights, datasetweight:"<<eventWeight<<","<<event.processMCWeight<<","<<sumWeights<<","<<datasetWeight<<std::endl;
				TString dname(dataset->name());
				bool MCSignal = dname.Contains("HToSS");
				if ((MCSignal)&&(!shf.GenLevelCheck(event,false))) continue;
        hists_1d_["Cutflow"]->Fill(1, eventWeight);

        // For make cuts here in this class itself
        const YAML::Node config{YAML::LoadFile(cuts_config)};
        TString hadronType = config["hadronType"].as<std::string>();
        TString trig = config["trig"].as<std::string>();
        bool metfilters_flag = config["metfilters"].as<bool>();
        const YAML::Node cuts{config["cuts"]};

        // TString hadronType = "kaon";// for now, change manually, but later, modify to read from a parser
        //TString hadronType = "pion";// for now, change manually, but later, modify to read from a parser
        shf.massAssumption(hadronType);
        if (hadronType.Contains("kaon")) chsMass_ = 0.493677;
        // std::cout<<"chsMass: "<<chsMass_<<std::endl;
        // std::cout<<"From sharedfunction, chsMass: "<<shf.chsMass_<<std::endl;

        
	      const bool passSingleMuonTrigger {event.muTrig()}, passDimuonTrigger {event.mumuTrig()};
	      const bool passL2MuonTrigger {event.mumuL2Trig()}, passDimuonNoVtxTrigger {event.mumuNoVtxTrig()};
        
        
        // const YAML::Node tight_eles{cuts["tightElectrons"]};

      //        if (! ( passDimuonTrigger || passSingleMuonTrigger || passL2MuonTrigger || passDimuonNoVtxTrigger ) ) continue;
        //trigger options
        if (trig.Contains("single")) {
	        if ( !passSingleMuonTrigger ) continue;
        }
        else if (trig.Contains("double")) {
	        if ( !passL2MuonTrigger ) continue;
        }
        else if (trig.Contains("mixed")) {
	        if ( !(passL2MuonTrigger || passSingleMuonTrigger) ) continue;
        }
        else {
          std::cout<<"No trig option provided, default to HLT_IsoMu"<<std::endl;
	        if ( !passSingleMuonTrigger ) continue;
        }
        hists_1d_["Cutflow"]->Fill(2, eventWeight);

        if (metfilters_flag) {
	        if (!event.metFilters()) continue;
          hists_1d_["Cutflow"]->Fill(3, eventWeight);
        }

        // Below for muons 
       /*event selection criteria for leptons*/
        const YAML::Node muon_cuts{cuts["muons"]};
        const YAML::Node chs_cuts{cuts["hadrons"]};
        const YAML::Node dimuon_cuts{cuts["dimuon"]};
        const YAML::Node dihadron_cuts{cuts["dihadron"]};
        const YAML::Node recohiggs_cuts{cuts["recohiggs"]};
        const YAML::Node recoscalar_cuts{cuts["recoscalar"]};

        shf.looseMuonEta_ = muon_cuts["eta"].as<double>();
        shf.looseMuonPt_ = muon_cuts["pt"].as<double>();
        shf.looseMuonPtLeading_ = muon_cuts["ptLeading"].as<double>();
        
        shf.looseChsEta_ = chs_cuts["eta"].as<double>();
        shf.looseChsPt_ = chs_cuts["pt"].as<double>();

        shf.looseMuonRelIsoLeading_ = dimuon_cuts["relisoLeading"].as<double>();
        shf.looseMuonRelIso_ = dimuon_cuts["reliso"].as<double>();
        shf.maxDileptonDeltaR_ = dimuon_cuts["dR"].as<double>();
        shf.diMuonPt_ = dimuon_cuts["pt"].as<double>();
        shf.diMuonOppCharge_ = dimuon_cuts["charge"].as<std::string>() == "OS"; //opposite sign or same sign muons

        shf.looseChsSumPtCh_ = dihadron_cuts["sumptch"].as<double>();
        shf.looseChsSumPtChLeading_ = dihadron_cuts["sumptchLeading"].as<double>();
        shf.maxChsDeltaR_ = dihadron_cuts["dR"].as<double>();
        shf.diChsPt_ = dihadron_cuts["pt"].as<double>();
        shf.diChsOppCharge_ = dihadron_cuts["charge"].as<std::string>() == "OS"; //opposite sign or same sign tracks

        shf.higgsPeakLow_ = recohiggs_cuts["massLow"].as<double>(); 
        shf.higgsPeakHigh_ = recohiggs_cuts["massHigh"].as<double>(); 
        shf.higgsPeakSideband_ = recohiggs_cuts["sideband"].as<double>(); 
        shf.SR_ = recohiggs_cuts["unblind"].as<bool>(); 

        std::vector<double> params_lower = recoscalar_cuts["lowerbound"].as<std::vector<double>>();
        std::vector<double> params_higher = recoscalar_cuts["higherbound"].as<std::vector<double>>();
        
        
        
        // if (muon_cuts["flag"].as<bool>()) {
        event.muonIndexLoose = shf.getLooseMuons(event, muon_cuts["flag"].as<bool>());
        if ( event.muonIndexLoose.size() < muon_cuts["number"].as<int>() ) continue;
        hists_1d_["Cutflow"]->Fill(4, eventWeight);
            
        // hists_1d_["h_leadingMuon_RelIso_BC"]         = new TH1F("h_leadingMuon_RelIso", "", 5000, 0., 10.);
        // hists_1d_["h_subleadingMuon_RelIso_BC"]      = new TH1F("h_subleadingMuon_RelIso_BC", "", 5000, 0., 10.);

        if ( !shf.getDileptonCand( event, event.muonIndexLoose , dimuon_cuts["flag"].as<bool>()  )) continue;
        hists_1d_["Cutflow"]->Fill(5, eventWeight);
        // }
	      
        
	      //std::cout << "Enters event, processes muons" << std::endl;
        //Below for charged hadrons
        /*event selection criteria for hadrons*/
        // if (chs_cuts["flag"].as<bool>()) {
                  // hists_1d_["h_leadingChHad_RelIso_BC"]        = new TH1F("h_leadingChHad_RelIso_BC", "", 5000, 0., 10.);
        // hists_1d_["h_subleadingChHad_RelIso_BC"]     = new TH1F("h_subleadingChHad_RelIso_BC", "", 5000, 0., 10.);
        // hists_1d_["h_DiChHadPt_BC"]                  = new TH1F("h_DiChHadPt_BC", "", 1000, 0., 150.);

        event.chsIndex = shf.getChargedHadronTracks(event, chs_cuts["flag"].as<bool>());
        if ( event.chsIndex.size() < chs_cuts["number"].as<int>() ) continue;
        hists_1d_["Cutflow"]->Fill(6, eventWeight);

        //std::cout << "Enters event, found 2 hadrons!" << std::endl;
        if ( !shf.getDihadronCand( event, event.chsIndex, dihadron_cuts["flag"].as<bool>()) ) { //dihadron pt cut included here
          // std::cout << "Eh, no dihadron found :("<<std::endl;      
          continue; //fine for now
        }
        hists_1d_["Cutflow"]->Fill(7, eventWeight);
        // }
        if (recohiggs_cuts["flag"].as<bool>()) {
                  // hists_1d_["h_recoHiggsMass_BC"]              = new TH1F("h_recoHiggsMass_BC", "", 20000, 0., 1000.);
        // hists_2d_["h_DiMuonMass_DiChHadMass_BC"]               = new TH2F("h_DiMuonMass_DiChHadMass_BC", "", 1000, 0., 10., 1000, 0., 10.);

          if ( !shf.HiggsWindow(event) ) continue; 
          hists_1d_["Cutflow"]->Fill(8, eventWeight);
        }

        if (recoscalar_cuts["flag"].as<bool>()) {
          // std::cout<<"enters mass window"<<std::endl;
          TF1 func_lowerbound_("func_lowerbound","[1]*x+[0]",0.,10.);
          TF1 func_higherbound_("func_higherbound","[1]*x+[0]",0.,10.);
          func_lowerbound_.FixParameter(0,params_lower[0]);
          func_lowerbound_.FixParameter(1,params_lower[1]);
          func_higherbound_.FixParameter(0,params_higher[0]);
          func_higherbound_.FixParameter(1,params_higher[1]);

          if ( !shf.MassCompatibility(event,func_lowerbound_,func_higherbound_) ) continue; 
          hists_1d_["Cutflow"]->Fill(9, eventWeight);
        }

        /*Now analyse*/
        fillMuonHists(event, "leadingMuon", event.zPairIndex.first, eventWeight);
        fillMuonHists(event, "subleadingMuon", event.zPairIndex.second, eventWeight);
	      hists_1d_["h_DiMuonDeltaR"]->Fill(ROOT::Math::VectorUtil::DeltaR(event.zPairLeptons.first,event.zPairLeptons.second), eventWeight);
	      hists_1d_["h_DiMuonMass"]->Fill((event.zPairLeptons.first + event.zPairLeptons.second).M(), eventWeight);
	      hists_1d_["h_DiMuonPt"]->Fill((event.zPairLeptons.first + event.zPairLeptons.second).Pt(), eventWeight);
	      //std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;    
	      if (event.muonPF2PATComRelIsodBeta[event.zPairIndex.first] == 0.) {
          hists_1d_["h_DiMuonDeltaRAtZeroIso"]->Fill(ROOT::Math::VectorUtil::DeltaR(event.zPairLeptons.first,event.zPairLeptons.second), eventWeight);        
        }
	      //std::cout << "Enters event, found dihadron object!" << std::endl;
	      /*Now analyse*/
        fillChHadHists(event, "leadingChHad", event.chsPairIndex.first, eventWeight);
        fillChHadHists(event, "subleadingChHad", event.chsPairIndex.second, eventWeight);
	      //std::cout << "Enters event, Filled the histos for hadrons!" << std::endl;
	      hists_1d_["h_DiChHadDeltaR"]->Fill(ROOT::Math::VectorUtil::DeltaR(event.chsPairVec.first,event.chsPairVec.second), eventWeight);
	      hists_1d_["h_DiChHadMass"]->Fill((event.chsPairVec.first + event.chsPairVec.second).M(), eventWeight);
	      hists_1d_["h_DiChHadPt"]->Fill((event.chsPairVec.first + event.chsPairVec.second).Pt(), eventWeight);

        float mass_avg = ((event.chsPairVec.first + event.chsPairVec.second).M() + (event.zPairLeptons.first + event.zPairLeptons.second).M())/2.;
        hists_1d_["h_AvgMass_mumu_hh"]->Fill(mass_avg,eventWeight);
        hists_1d_["h_recoHiggsMass"]->Fill((event.chsPairVec.first + event.chsPairVec.second+event.zPairLeptons.first + event.zPairLeptons.second).M(),eventWeight);
        
        int k;
        for (k=0;k<event.numPVs;k++) {
          if (event.pvChi2[k]!= 0 && event.pvNdof[k]!= 0) break;
        }
        double pvCov_arr[9] = {event.pvCov00[k], event.pvCov01[k], event.pvCov02[k], event.pvCov10[k], event.pvCov11[k], event.pvCov12[k], event.pvCov20[k], event.pvCov21[k], event.pvCov22[k]};
        double muon_svCov_arr[9] = {event.muonTkPairPF2PATTkVtxCov00[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov01[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov02[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov10[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov11[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov12[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov20[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov21[event.mumuTrkIndex], event.muonTkPairPF2PATTkVtxCov22[event.mumuTrkIndex]};
        ROOT::Math::SVector<double, 3>  muon_distVecXY(event.muonTkPairPF2PATTkVx[event.chsPairTrkIndex] - event.pvX[k], event.muonTkPairPF2PATTkVy[event.chsPairTrkIndex] - event.pvY[k], 0.);
        ROOT::Math::SMatrix<double,3> muon_svCov(muon_svCov_arr,9);
        ROOT::Math::SMatrix<double,3> pvCov(pvCov_arr,9);
        auto muon_totalCov = pvCov + muon_svCov;
        double muon_distMagXY = ROOT::Math::Mag(muon_distVecXY);
        double muon_similarity_val = sqrt(ROOT::Math::Similarity(muon_totalCov, muon_distVecXY/muon_distMagXY));
        double muon_sigmaDistMagXY = muon_similarity_val/muon_distMagXY;
        double muon_significance = muon_distMagXY/muon_similarity_val;

        double chs_svCov_arr[9] = {event.chsTkPairTkVtxCov00[event.chsPairTrkIndex], event.chsTkPairTkVtxCov01[event.chsPairTrkIndex], event.chsTkPairTkVtxCov02[event.chsPairTrkIndex], event.chsTkPairTkVtxCov10[event.chsPairTrkIndex], event.chsTkPairTkVtxCov11[event.chsPairTrkIndex], event.chsTkPairTkVtxCov12[event.chsPairTrkIndex], event.chsTkPairTkVtxCov20[event.chsPairTrkIndex], event.chsTkPairTkVtxCov21[event.chsPairTrkIndex], event.chsTkPairTkVtxCov22[event.chsPairTrkIndex]};
        ROOT::Math::SVector<double, 3>  chs_distVecXY(event.chsTkPairTkVx[event.chsPairTrkIndex] - event.pvX[k], event.chsTkPairTkVy[event.chsPairTrkIndex] - event.pvY[k], 0.);
        ROOT::Math::SMatrix<double,3> chs_svCov(chs_svCov_arr,9);
        auto chs_totalCov = pvCov + chs_svCov;
        double chs_distMagXY = ROOT::Math::Mag(chs_distVecXY);
        double chs_similarity_val = sqrt(ROOT::Math::Similarity(chs_totalCov, chs_distVecXY/chs_distMagXY));
        double chs_sigmaDistMagXY = chs_similarity_val/chs_distMagXY;
        double chs_significance = chs_distMagXY/chs_similarity_val;

        hists_1d_["h_DiChHadVtxSignificance"]->Fill(chs_significance,eventWeight);
        hists_1d_["h_DiMuonVtxSignificance"]->Fill(muon_significance,eventWeight);
        hists_2d_["h_DiMuonVtxSignificance_DiChHadVtxSignificance"]->Fill(muon_significance,chs_significance,eventWeight);
        hists_1d_["h_DiChHadVtxLxy"]->Fill(chs_distMagXY,eventWeight);
        hists_1d_["h_DiMuonVtxLxy"]->Fill(muon_distMagXY, eventWeight);
        hists_2d_["h_DiMuonVtxLxy_DiChHadVtxLxy"]->Fill(muon_distMagXY,chs_distMagXY,eventWeight);
        hists_1d_["h_DiChHadVtxSigma"]->Fill(chs_similarity_val,eventWeight);
        hists_1d_["h_DiMuonVtxSigma"]->Fill(muon_similarity_val,eventWeight);
        hists_2d_["h_DiMuonVtxLxy_DiMuonVtxSigma"]->Fill(muon_distMagXY,muon_similarity_val,eventWeight);
        hists_2d_["h_DiChHadVtxLxy_DiChHadVtxSigma"]->Fill(chs_distMagXY,chs_similarity_val,eventWeight);

        hists_2d_["h_leadingMuon_RelIso_leadingHadron_RelIso"]->Fill(event.zPairRelIso.first,event.chsPairRelIso.first,eventWeight);
        hists_2d_["h_leadingMuon_SumPtCh_leadingHadron_SumPtCh"]->Fill(event.zPairChIso.first,event.chsPairChIso.first,eventWeight);
        hists_2d_["h_leadingMuon_SumPtNh_leadingHadron_SumPtNh"]->Fill(event.zPairNhIso.first,event.chsPairNhIso.first,eventWeight);
        hists_2d_["h_leadingMuon_SumPtPh_leadingHadron_SumPtPh"]->Fill(event.zPairPhIso.first,event.chsPairPhIso.first,eventWeight);
        hists_2d_["h_leadingMuon_SumPtPU_leadingHadron_SumPtPU"]->Fill(event.zPairPuIso.first,event.chsPairPuIso.first,eventWeight);
        hists_2d_["h_subleadingMuon_RelIso_subleadingHadron_RelIso"]->Fill(event.zPairRelIso.second,event.chsPairRelIso.second,eventWeight);
        hists_2d_["h_subleadingMuon_SumPtCh_subleadingHadron_SumPtCh"]->Fill(event.zPairChIso.second,event.chsPairChIso.second,eventWeight);
        hists_2d_["h_subleadingMuon_SumPtNh_subleadingHadron_SumPtNh"]->Fill(event.zPairNhIso.second,event.chsPairNhIso.second,eventWeight);
        hists_2d_["h_subleadingMuon_SumPtPh_subleadingHadron_SumPtPh"]->Fill(event.zPairPhIso.second,event.chsPairPhIso.second,eventWeight);
        hists_2d_["h_subleadingMuon_SumPtPU_subleadingHadron_SumPtPU"]->Fill(event.zPairPuIso.second,event.chsPairPuIso.second,eventWeight);

        hists_2d_["h_DiMuonMass_DiChHadMass"]->Fill((event.zPairLeptons.first + event.zPairLeptons.second).M(),(event.chsPairVec.first + event.chsPairVec.second).M(),eventWeight);
	      
	      //std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;    
	      if (event.chsPairRelIso.first == 0.) {
          hists_1d_["h_DiChHadDeltaRAtZeroIso"]->Fill(ROOT::Math::VectorUtil::DeltaR(event.chsPairVec.first,event.chsPairVec.second), eventWeight);        
        }

				if ((event.chsPairRelIso.first > 2)&&(MCSignal)) isotail_evts <<event.eventRun<<":"<<event.eventLumiblock<<":"<<event.eventNum<<"\n";
				if (((event.chsPairVec.first + event.chsPairVec.second).M()<=1.0) && (MCSignal)) masstail_evts <<event.eventRun<<":"<<event.eventLumiblock<<":"<<event.eventNum<<"\n";


	      //std::cout << "Enters event, processes hadrons" << std::endl;
	
      } // end event loop
    } // end dataset loop

		isotail_evts.close();
		masstail_evts.close();
    
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
    hists_1d_["h_recoHiggsMass"]->Write();
    hists_1d_["h_DiChHadVtxSignificance"]->Write();
    hists_1d_["h_DiMuonVtxSignificance"]->Write();
    hists_2d_["h_DiMuonVtxSignificance_DiChHadVtxSignificance"]->Write();
    hists_2d_["h_DiMuonVtxLxy_DiChHadVtxLxy"]->Write();
    hists_1d_["h_DiChHadVtxLxy"]->Write();
    hists_1d_["h_DiMuonVtxLxy"]->Write();
    hists_1d_["h_DiChHadVtxSigma"]->Write();
    hists_1d_["h_DiMuonVtxSigma"]->Write();
    hists_2d_["h_DiMuonVtxLxy_DiMuonVtxSigma"]->Write();
    hists_2d_["h_DiChHadVtxLxy_DiChHadVtxSigma"]->Write();
    
    // float norm = hists_1d_["Cutflow"]->GetBinContent(1);
    // hists_1d_["Cutflow"]->Scale(1./norm);
    // for(int i=1;i<=hists_1d_["Cutflow"]->GetNbinsX();i++) hists_1d_["Cutflow"]->SetBinContent(i,hists_1d_["Cutflow"]->GetBinContent(i)/norm);
		hists_1d_["Cutflow"]->Write();
    hists_1d_["h_leadingMuon_RelIso_BC"]->Write();    
    hists_1d_["h_leadingChHad_RelIso_BC"]->Write();   
    hists_1d_["h_subleadingMuon_RelIso_BC"]->Write(); 
    hists_1d_["h_subleadingChHad_RelIso_BC"]->Write();
    hists_1d_["h_DiChHadPt_BC"]->Write();             
    hists_1d_["h_recoHiggsMass_BC"]->Write();         
    hists_2d_["h_DiMuonMass_DiChHadMass_BC"]->Write();

    outFile->Close();

//  std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

int nTrksInCone( ROOT::Math::PxPyPzMVector& particle, const AnalysisEvent& event, double dr_max) {
    int count = 0;
    double tmpdR=999.;
    for (int i=0;i < event.numPackedCands; i++) {
         ROOT::Math::PxPyPzEVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        if ((event.packedCandsPdgId[i] != 211) || (event.packedCandsFromPV[i] < 2)) continue;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,trk);
        if ((tmpdR < dr_max) && (tmpdR > 0.03)) count++;// 0.03 cut to ensure not the same track as candidate
    }
    return count;
}
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
  ROOT::Math::PxPyPzEVector particle{event.muonPF2PATPX[idx], event.muonPF2PATPY[idx], event.muonPF2PATPZ[idx], event.muonPF2PATE[idx]};
	hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.muonPF2PATTrackIso[idx] ,eventweight);
  if (prefix.find("sub") == std::string::npos) {
    hists_1d_["h_"+prefix+"RelIso"]->Fill(event.zPairRelIso.first ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.zPairChIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.zPairChIso.first/particle.Pt()  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.zPairNhIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.zPairPhIso.first  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.zPairPuIso.first  ,eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.zPairRelIso.first, particle.Pt(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.zPairRelIso.first, ROOT::Math::VectorUtil::DeltaR(event.zPairLeptons.first,event.zPairLeptons.second),eventweight);
    // hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.zPairRelIso.first, nTrksInCone(particle,event,0.4),eventweight);
	
  }
  else {
    hists_1d_["h_"+prefix+"RelIso"]->Fill(event.zPairRelIso.second ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.zPairChIso.second ,eventweight);
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.zPairChIso.second/particle.Pt()  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.zPairNhIso.second  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.zPairPhIso.second  ,eventweight);
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.zPairPuIso.second  ,eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.zPairRelIso.second, particle.Pt(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.zPairRelIso.second, ROOT::Math::VectorUtil::DeltaR(event.zPairLeptons.first,event.zPairLeptons.second),eventweight);
    // hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.zPairRelIso.second, nTrksInCone(particle,event,0.4),eventweight);
  }
	hists_1d_["h_"+prefix+"Pt"]->Fill(particle.Pt(),eventweight); 
	hists_1d_["h_"+prefix+"Eta"]->Fill(particle.Eta(),eventweight);
	hists_1d_["h_"+prefix+"Phi"]->Fill(particle.Phi(),eventweight);
  // hists_1d_["h_"+prefix+"TrksInCone"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  //std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;
	
	std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
	double tmpdR = 999.;
	//easier to just loop over tracks here and fill hists directly
	for (int i=0;i < event.numPackedCands; i++) {
        ROOT::Math::PxPyPzEVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        //if ((event.packedCandsPdgId[i] == 211) || (event.packedCandsPdgId[i] == 130)) std::cout<<"Found hadrons"<<std::endl;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,trk);
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
    // hists_1d_["h_"+prefix+"TrksInConeAtZeroIso"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  
  }
} 

void fillChHadHists(const AnalysisEvent& event, const std::string &prefix, const int idx, const float eventweight){
  int pair_idx = prefix.find("sub") == std::string::npos ? 0 : 1; // npos is "not found" and 0 is leading
  ROOT::Math::PxPyPzMVector particle{event.packedCandsPx[idx], event.packedCandsPy[idx], event.packedCandsPz[idx], chsMass_};
  if (prefix.find("sub") == std::string::npos) {
	  hists_1d_["h_"+prefix+"RelIso"]->Fill(event.chsPairRelIso.first,eventweight);        
	  hists_1d_["h_"+prefix+"TrackIso"]->Fill(event.chsPairTrkIso.first ,eventweight);
    hists_1d_["h_"+prefix+"SumPtCh"]->Fill(event.chsPairChIso.first ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.chsPairChIso.first/event.chsPairVec.first.Pt() ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.chsPairNhIso.first ,eventweight); //neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.chsPairPhIso.first ,eventweight); //photons
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.first ,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(event.chsPairVec.first.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(event.chsPairVec.first.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(event.chsPairVec.first.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.first, event.chsPairVec.first.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.first, ROOT::Math::VectorUtil::DeltaR(event.chsPairVec.first,event.chsPairVec.second),eventweight);
	  // hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.first, nTrksInCone(event.chsPairVec.first,event,0.4),eventweight);
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
    hists_1d_["h_"+prefix+"SumPtChRel"]->Fill(event.chsPairChIso.second/event.chsPairVec.second.Pt() ,eventweight);                              
    hists_1d_["h_"+prefix+"SumPtNh"]->Fill(event.chsPairNhIso.second ,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPh"]->Fill(event.chsPairPhIso.second ,eventweight); //photons + neutral hadrons  
    hists_1d_["h_"+prefix+"SumPtPU"]->Fill(event.chsPairPuIso.second ,eventweight); 
		hists_1d_["h_"+prefix+"Pt"]->Fill(event.chsPairVec.second.Pt(),eventweight); 
		hists_1d_["h_"+prefix+"Eta"]->Fill(event.chsPairVec.second.Eta(),eventweight);
		hists_1d_["h_"+prefix+"Phi"]->Fill(event.chsPairVec.second.Phi(),eventweight);
    hists_2d_["h_"+prefix+"RelIso_"+prefix+"Pt"]->Fill(event.chsPairRelIso.second, event.chsPairVec.second.Pt(),eventweight);
	  hists_2d_["h_"+prefix+"RelIso_"+"dR"]->Fill(event.chsPairRelIso.second, ROOT::Math::VectorUtil::DeltaR(event.chsPairVec.first,event.chsPairVec.second),eventweight);
	  // hists_2d_["h_"+prefix+"RelIso_"+prefix+"TrksInCone"]->Fill(event.chsPairRelIso.second, nTrksInCone(event.chsPairVec.second,event,0.4),eventweight);
	  if (event.chsPairRelIso.second == 0.) {
      hists_1d_["h_"+prefix+"TrackIsoAtZeroIso"]->Fill(event.chsPairTrkIso.second,eventweight);
      hists_1d_["h_"+prefix+"SumPtChAtZeroIso"]->Fill(event.chsPairChIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtNhAtZeroIso"]->Fill(event.chsPairNhIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPhAtZeroIso"]->Fill(event.chsPairPhIso.second ,eventweight);
      hists_1d_["h_"+prefix+"SumPtPUAtZeroIso"]->Fill(event.chsPairPuIso.second ,eventweight); 
    }
  }
  // hists_1d_["h_"+prefix+"TrksInCone"]->Fill(nTrksInCone(particle,event,0.4),eventweight);
  
	//std::cout<<"Number of tracks:"<<nTrksInCone(event.zPairIndex.first,event,0.4)<<std::endl;
	
	std::map<int,int> PDG_map = {{11,1},{13,2},{22,3},{130,6},{1,7},{2,8}};//all maps except 211	
	double tmpdR = 999.;
	//easier to just loop over tracks here and fill hists directly
	for (int i=0;i < event.numPackedCands; i++) {
        ROOT::Math::PxPyPzEVector trk{event.packedCandsPx[i], event.packedCandsPy[i], event.packedCandsPz[i], event.packedCandsE[i]};
        //if (trk.Pt() < 0.5) continue;
        //if ((event.packedCandsPdgId[i] == 211) || (event.packedCandsPdgId[i] == 130)) std::cout<<"Found hadrons"<<std::endl;
        //if (std::abs(trk.Pt() - event.zPairLeptons.first.Pt()) < 0.5) continue; //if pt is within 0.5 GeV, then same track possibly
        tmpdR = ROOT::Math::VectorUtil::DeltaR(particle,trk);
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

