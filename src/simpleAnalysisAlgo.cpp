#include "AnalysisEvent.hpp"
#include "simpleAnalysisAlgo.hpp"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TASImage.h"
#include "TLatex.h"
#include "TMVA/Timer.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TString.h"

#include "config_parser.hpp"

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/program_options.hpp>
#include <boost/range/iterator_range.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>


SimpleAnalysis::SimpleAnalysis()
: is2016_{false}             // Flag not to be changed by user
, is2018_{false}             // Flag not to be changed by user
, muonPtLeading_{30.0}       // As only single muon trigger should be used normally, this is set to 30., if double muon trigger is used, this needs changing.
, muonPt_{5.0}               // As above
, muonEta_{2.40}             // Max muon eta, should not be changed by user
, packedCandMuonDeltaR_{0.4} // DeltaR window used for muons
, packedCandPionDeltaR_{0.4} // DeltaR window used for pions
, packedCandKaonDeltaR_{0.4} // DeltaR window used for kaons
, isoConeSize_{0.3}          // Isolation cone size of 0.3 is used repeatedly. Define once here for efficiency.
, higgsMass_{125.35}         // Standard Higgs mass - define here rather than multiple locations
, higgsMassWindow_{3.0}      // Define Standard Higgs mass +/ window here rather than in multiple locations
, higgsMassWideWindow_{20.0} // Define wider Standard Higgs mass +/ window here rather than in multiple locations
, statWindow_{0.15} 
, mass_range_{2.0}              // 1 or 2 GeV sample run
, debug_{false}              // Set to true to enable debug couts
{}
SimpleAnalysis::~SimpleAnalysis() {}

void SimpleAnalysis::parseCommandLineArguements(int argc, char* argv[]){

  namespace po = boost::program_options;

  // command line configuration parsing magic!
  po::options_description desc("Options");
  desc.add_options()("help,h", "Print this message.")(
  "config,c",
  po::value<std::string>(&config)->required(),
  "The configuration file to be used.")(
  "2016",
  po::bool_switch(&is2016_),
  "Use 2016 conditions (SFs, et al.). NOT TO BE USED IN NORMAL CIRCUMSTANCES")(
  "lumi,l",
  po::value<double>(&usePreLumi)->default_value(4247.682053046), //4.25 inverse femtobarn
  //po::value<double>(&usePreLumi)->default_value(41528.0),
  "Lumi to scale MC plots to.")(
  "outfile,o",
  po::value<std::string>(&outFileString)->default_value("plots/distributions/output.root"),
  "Output file for plots.")(
  ",u",
  po::bool_switch(&usePostLepTree),
  "Use post lepton selection trees.")(
  ",n",
  po::value<Long64_t>(&nEvents)->default_value(0),
  "The number of events to be run over. All if set to 0.");

  po::variables_map vm;

  try
    {
      po::store(po::parse_command_line(argc, argv, desc), vm);

      if(vm.count("help"))
	{
	  std::cout << desc;
	  std::exit(0);
	}

      po::notify(vm);
    }
  catch(po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::exit(1);
  }

  // Some vectors that will be filled in the parsing
  totalLumi = 0;

  try {
    Parser::parse_config(config, datasets, totalLumi, usePostLepTree);
  }
  catch(const std::exception) {
    std::cerr << "ERROR Problem with a confugration file, see previous "
      "errors for more details. If this is the only error, the "
      "problem is with the main configuration file."
	      << std::endl;
    throw;
  }
}

namespace fs = boost::filesystem;

void SimpleAnalysis::runMainAnalysis() {

  if(totalLumi == 0.)
    totalLumi = usePreLumi;

  std::cout << "Using lumi: " << totalLumi << std::endl;

  bool datasetFilled{false};
	
  Float_t Nbg1=0; //sideband 105-122
  Float_t Nbg2=0; //sideband 128-145
  Float_t Obs=0; //Observation
  Float_t rate_signal=0; 
  Float_t stat_signal=0; 

  Float_t QCD_scalar=0; Float_t QCD_Pantiscalar=0; Float_t QCD_Kantiscalar=0;
  Float_t QCD_20scalar=0; Float_t QCD_20Pantiscalar=0; Float_t QCD_20Kantiscalar=0;
	
  std::string era {""};
  if (is2016_) era = {"2016"};
  else if (is2018_) era = {"2018"};
  else era = {"2017"};

  const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "_legacy/"};

  // Begin to loop over all datasets
  for(auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset){ 
    datasetFilled = false;

    // Initialise TChain containing all files/events in dataset
    TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
    datasetChain->SetAutoSave(0);

    std::cerr << "Processing dataset " << dataset->name() << std::endl;
    if(!usePostLepTree) {
      if(!datasetFilled) {
	if(!dataset->fillChain(datasetChain)) {
	  std::cerr << "There was a problem constructing the chain for " << dataset->name() << ". Continuing with next dataset.\n";
	  continue;
	}
	datasetFilled=true;
      }
    }
    else{
      std::string inputPostfix{"mumu"};
      std::cout << postLepSelSkimInputDir + dataset->name() + inputPostfix + "SmallSkim.root" << std::endl;
      datasetChain->Add((postLepSelSkimInputDir + dataset->name() + inputPostfix + "SmallSkim.root").c_str());
    }


    // extract the dataset weight. MC = (lumi*crossSection)/(totalEvents), data = 1.0
    float datasetWeight{dataset->getDatasetWeight(totalLumi)};
    std::cout << datasetChain->GetEntries() << " number of items in tree. Dataset weight: " << datasetWeight << std::endl;
    if(datasetChain->GetEntries() == 0) {
      std::cout << "No entries in tree, skipping..." << std::endl;
      continue;
    }

    // Load event into nTuple tree structure class in order to access its contents
    // 2016/2018 flags can be ignored here
    AnalysisEvent event{dataset->isMC(), datasetChain, is2016_, is2018_};

    // Get total number of events saved in this dataset for number of events to loop over
    Long64_t numberOfEvents{datasetChain->GetEntries()};

    // If user wants to run over a smaller number of events, set max event counter to new max
    if(nEvents && nEvents < numberOfEvents) numberOfEvents = nEvents;

    // Progress bar drawing for event processing progress
    TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}};
    lEventTimer->DrawProgressBar(0, "");
    
    // Loop over all events in this dataset
    for(Long64_t i{0}; i < numberOfEvents; i++) {

      lEventTimer->DrawProgressBar(i,""); // increment progress bar

      // Grab entry i, and thus the corresponding entry in each branch
      event.GetEntry(i);

      // Initial event weight = unity
      double eventWeight {1.};

      // Normalise event weight to cross section, nEvents, and lumi
      eventWeight *= datasetWeight;

      // Do functions that do not require met filters or triggers
      //fillGeneratorPlots(event); // Commented out currently by CB in main branch

      // Do functions that have met filters applied
      if( !event.metFilters() ) continue;

      // Get physics objects
      std::pair<int, int> patMuons = getPatMuonPair(event); // Get PAT muons indices
      // This function needs a pair of selected muons, but also looks at all muons and different triggers within this, so no blanket trigger application and no skip event if no incompatible muon pair is found
      //fillMuonReconstructionPlots(event, eventWeight, patMuons.first, patMuons.second);

      if ( !event.muTrig() ) continue; // If single muon trigger does not fire
      if (patMuons.first == -1 || patMuons.second == -1) continue; // If there is no valid muon pair in this event (i.e. no muon index returned), skip event as all following code requires this

      std::pair<int, int> packedCandMuons = getPackedCandMuonPair(event, patMuons.first, patMuons.second); // Get packedCands associated with PAT muons
      std::pair<int, int> packedCandHadrons = getDihadronPair(event, patMuons.first, patMuons.second); // Setup dihadron candidate, patMuons indices are passed to exclude their associated packedCands from consideration

      // Check that selected packed cand muons and charged hadrons are valid, if any are not, skip event
      if (packedCandMuons.first == -1 || packedCandMuons.second == -1 || packedCandHadrons.first == -1 || packedCandHadrons.second == -1) continue;

      // Fill other plots now!
      // All of	these plots use	packed PF muons, so the ones corresponding to the PAT muons are provided
      fillPackedCandidatePlots(event, eventWeight, Nbg1, Nbg2, Obs, rate_signal, stat_signal, QCD_Kantiscalar, QCD_Pantiscalar, QCD_20Kantiscalar, QCD_20Pantiscalar, QCD_scalar, QCD_20scalar, patMuons.first, patMuons.second, packedCandMuons.first, packedCandMuons.second, packedCandHadrons.first, packedCandHadrons.second);
      //fillMuonMomentumComparisonPlots(event, eventWeight, patMuons.first, patMuons.second, packedCandMuons.first, packedCandMuons.second, packedCandHadrons.first, packedCandHadrons.second);

    } // End loop over all events

    Float_t Nbg;
    Nbg=(Nbg1+Nbg2)*6/54;
    std::cout<<"Background estimate: "<<Nbg<<std::endl;
    std::cout<<"Sideband left: "<<Nbg1<<std::endl; std::cout<<"Sideband right: "<<Nbg2<<std::endl;
    std::cout<<"Observation: "<<Obs<<std::endl;
    std::cout<<"   "<<std::endl;
    std::cout<<"Rate: "<<rate_signal<<std::endl;
    std::cout<<"Statistical uncertainty: "<<stat_signal<<std::endl;
	  
  } // End loop over all datatsets

}    

std::pair<int, int> SimpleAnalysis::getPatMuonPair(const AnalysisEvent& event) const {

  // ADM - as CB's muon selection code is embedded in plot making bits in SimpleAnalysis::fillMuonReconstructionPlots,
  // I have rewritten it in a simpler standalone form for now

  int muon1 {-1}, muon2{-1}; // Selected muon indices

  // First identify muons that pass baseline criteria
  std::vector<int> muons {}; // empty vector of muon indices
    
  for (int i{0}; i < event.numMuonPF2PAT; i++) { // loop over all PAT muons
    // If the PAT muon passes Loose ID and is a PF muon and has an absolute eta greater than the maximum, check pT
    if (event.muonPF2PATIsPFMuon[i] && event.muonPF2PATLooseCutId[i] && std::abs(event.muonPF2PATEta[i]) < muonEta_) {
      // If the muon index vector is empty, check leading pT cut first, otherwise, lower pT cut
      if (event.muonPF2PATPt[i] >= (muons.empty() ? muonPtLeading_ : muonPt_)) muons.emplace_back(i);
    }
  }

  // Now construct our dimuon candidate
  // Loop over all possible pairs of muons. As PAT muons are pT ordered, the highest pT pairings will be considered first
  for ( unsigned int i{0}; i < muons.size(); i++ ) {
    for ( unsigned int j{i+1}; j < muons.size(); j++ ) {
      if (event.muonPF2PATCharge[muons[i]] * event.muonPF2PATCharge[muons[j]] >= 0) continue; // Check opposite charges

      // Connstruct TLorentzVectors to make accessing quantities easier
      TLorentzVector lepton1{event.muonPF2PATPX[muons[i]], event.muonPF2PATPY[muons[i]], event.muonPF2PATPZ[muons[i]], event.muonPF2PATE[muons[i]]};
      TLorentzVector lepton2{event.muonPF2PATPX[muons[j]], event.muonPF2PATPY[muons[j]], event.muonPF2PATPZ[muons[j]], event.muonPF2PATE[muons[j]]};
      // const double delR { lepton1.DeltaR(lepton2) }; // Define deltaR if used when constructing pair
      // if ( delR < packedCandMuonDeltaR_ ) { // optional delR requirement. CB does not use, so commented out. Use same delR cut for PAT and packed PF cand Muons - hence reused variable
      // While muon[i] should be the leading muon, check explicitly, and set leading muon's indice == muon1, and vice versa for subleading muon
      muon1 = lepton1.Pt() > lepton2.Pt() ? muons[i] : muons[j];
      muon2 = lepton1.Pt() > lepton2.Pt() ? muons[j] : muons[i];
      // } // End delR requirement

    }
  }

  return std::make_pair(muon1, muon2); // Return indices

}

std::pair<int, int> SimpleAnalysis::getPackedCandMuonPair(const AnalysisEvent& event, const int& patMuon1, const int& patMuon2) const {

  int muon1 {-1}, muon2 {-2};

  // This should NEVER happen as all selected muon pairs should be built from PF muons, but in case something has gone horribly wrong ...
  // ... and there is not PF cand building block associated with the PAT muons, return a failsafe value that will be caught
  if (event.muonPF2PATNumSourceCandidates[patMuon1] < 1 || event.muonPF2PATNumSourceCandidates[patMuon2] < 1 ) return std::make_pair(muon1, muon2);
  // Otherwise
  else return std::make_pair(event.muonPF2PATPackedCandIndex[patMuon1], event.muonPF2PATPackedCandIndex[patMuon2]);
}

std::pair<int, int> SimpleAnalysis::getDihadronPair(const AnalysisEvent& event, const int& patMuon1, const int& patMuon2) const {

  Int_t chsIndex1{-1}; Int_t chsIndex2{-1};
 // Int_t muIndex1{-1}; Int_t muIndex2{-1}; // ADM - commented out as no longer required

  Float_t chspt1{-1}; Float_t chspt2{-1};
 // Float_t mupt1{-1}; Float_t mupt2{-1}; // ADM - commented out as no longer required

  for(Int_t k{0};k<event.numPackedCands;k++) {
    // ADM - add check to not consider any packed cand that was associated with our selected muons
    if ( k == event.muonPF2PATPackedCandIndex[patMuon1] || k == event.muonPF2PATPackedCandIndex[patMuon2] ) continue;

    // ADM - add charge and track details check - we only want muons of hadrons with charges/tracks
    if (event.packedCandsCharge[k] == 0 ) continue;
    if (event.packedCandsHasTrackDetails[k] != 1 ) continue;

    // ADM - rest is CB's code but copied and pasted into this new struture
    const int packedId {event.packedCandsPdgId[k]};

    //Find the hadrons (pions)

    if(std::abs(packedId)==211 && event.packedCandsPseudoTrkPt[k]>1){//Selection of pions (charged hadrons)
      if(event.packedCandsPseudoTrkPt[k]>chspt1){
	chspt2=chspt1;
	chspt1=event.packedCandsPseudoTrkPt[k];
	chsIndex2=chsIndex1;
	chsIndex1=k;
      }
      else if(event.packedCandsPseudoTrkPt[k]>chspt2){
	chspt2=event.packedCandsPseudoTrkPt[k];
	chsIndex2=k;
      }
    }
  }
  return std::make_pair(chsIndex1, chsIndex2); // ADM - return the indices of the selected two pions
}

int SimpleAnalysis::getMuonTrackPairIndex(const AnalysisEvent& event, const int& muon1, const int& muon2) const {
  // Loop over all possible refitted muon track pairs
  for (int i{0}; i < event.numMuonTrackPairsPF2PAT; i++) {
    // If the two tracks used to make the refitted track pair are the same as the selected PAT muons ...
    if (event.muonTkPairPF2PATIndex1[i] == muon1 && event.muonTkPairPF2PATIndex2[i] == muon2) return i;
  }
  return -1;
}

int SimpleAnalysis::getChsTrackPairIndex(const AnalysisEvent& event, const int& chs1, const int& chs2) const {
  // Loop over all possible refitted chatged hadron track pairs
  for (int i{0}; i < event.numChsTrackPairs; i++) {
    // If the two tracks used to make the refitted track pair are the same as the selected charged hadrons ...
    if (event.chsTkPairIndex1[i] == chs1 && event.chsTkPairIndex2[i] == chs2) return i;
  }
  return -1;
}

void SimpleAnalysis::fillGeneratorPlots(const AnalysisEvent& event) const {
 
  //GENERATOR PARTICLE STUFF
  std::vector<int> nrofHiggs;
  std::vector<int> nrofScalar; //Number of scalars
  std::vector<int> nrofMuon;
  std::vector<int> nrofKaon;
  std::vector<int> nrofKShort;
  std::vector<int> nrofPion;
    
  Float_t genpt1=0; Float_t genpt2=0;
  Float_t gen1=0; Float_t gen2=0;
  //std::cout << "idx\t | ID\t stat\t | Mo\t Da1\t Da2\t | pt\t eta\t phi\t m" << std::endl;
         
  for(Int_t k{0}; k < event.nGenPar; k++) {
     
    //Print out event record

    //Invariant mass
    TLorentzVector m;
    m.SetPtEtaPhiE(event.genParPt[k],event.genParEta[k],event.genParPhi[k],event.genParE[k]);
    TLorentzVector mass {m.Px(),m.Py(),m.Pz(),event.genParE[k]};
    if (debug_) {
      std::cout << k << "\t | "
		<< event.genParId[k] << "\t "
		<< event.genParStatus[k] << "\t | "
		<< event.genParMotherIndex[k] << "\t "
		<< event.genParDaughter1Index[k] << "\t "
		<< event.genParDaughter2Index[k] << "\t | "
		<< event.genParPt[k] << "\t "
		<< event.genParEta[k] << "\t "
		<< event.genParPhi[k] << "\t "
		<< mass.M() << std::endl;
    }

    // get variables for this event that have been stored in ROOT nTuple tree
    const int pdgId { std::abs(event.genParId[k]) };
    const Int_t motherId { std::abs(event.genParMotherId[k]) };
    const Int_t motherIndex { std::abs(event.genParMotherIndex[k]) };
    const Float_t genParVx {event.genParVx[k]};
    const Float_t genParVy {event.genParVy[k]};
    const Float_t genParVz {event.genParVz[k]};
            
    const Float_t genParEta { event.genParEta[k] };
    const Float_t genParPhi { event.genParPhi[k] };
    const Float_t genParE { event.genParE[k] };
     
    const bool ownParent {pdgId == motherId ? true : false};

    h_genParPt->Fill(event.genParPt[k]);
    h_genParEta->Fill(genParEta);
    h_genParPhi->Fill(genParPhi);
    h_genParE->Fill(genParE);
        
    //Higgs boson
    if(pdgId==25 && !ownParent){ //First entry Higgs - to obtain mass correctly
      nrofHiggs.emplace_back(k);
      h_genParHiggsPt->Fill(event.genParPt[k]);
      h_genParHiggsEta->Fill(genParEta);
      h_genParHiggsPhi->Fill(genParPhi);
      h_genParHiggsE->Fill(genParE);
    }
            
    //Scalar decay
    if(pdgId==9000006){
      nrofScalar.emplace_back(k); //Store the scalar index k in nrofScalar
      h_genParScalarPt->Fill(event.genParPt[k]);
      h_genParScalarEta->Fill(genParEta);
      h_genParScalarPhi->Fill(genParPhi);
      h_genParScalarE->Fill(genParE);
    }
            
    //Particles from scalar decay
    const bool isScalarGrandparent{scalarGrandparent(event,k,9000006)};
    std::vector<Int_t> mu; std::vector<Int_t> mumu;
         
    if(isScalarGrandparent==true){
          
      //Muon from scalar decay
      if(pdgId==13 && !ownParent){
             
	nrofMuon.emplace_back(k);
            
	h_genParScalarMuonEta->Fill(genParEta);
	h_genParScalarMuonPhi->Fill(genParPhi);
	h_genParScalarMuonE->Fill(genParE);
	h_VertexPosXY->Fill(genParVx,genParVy);
	h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
	h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
       
	//Sort leading and subleading
	if(event.genParPt[k]>gen1){
	  gen2=gen1;
	  gen1=event.genParPt[k];
	}
	else if(event.genParPt[k]>gen2){
	  gen2=event.genParPt[k];
	}
    
      }
	    
      //Charged kaon from scalar decay
      if(pdgId==321){
	nrofKaon.emplace_back(k);
	h_genParScalarCKaonPt->Fill(event.genParPt[k]);
	h_genParScalarCKaonEta->Fill(genParEta);
	h_genParScalarCKaonPhi->Fill(genParPhi);
	h_genParScalarCKaonE->Fill(genParE);
	h_VertexPosXY->Fill(genParVx,genParVy);
	h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
	h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
      }
      //K short from scalar decay
      if(pdgId==310){
	nrofKShort.emplace_back(k); //Together with kaon in angular differences
	h_genParScalarKShortPt->Fill(event.genParPt[k]);
	h_genParScalarKShortEta->Fill(genParEta);
	h_genParScalarKShortPhi->Fill(genParPhi);
	h_genParScalarKShortE->Fill(genParE);
	//h_VertexPosXY->Fill(genParVx,genParVy);
	//h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
	//h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
      }
      //Charged pion from scalar decay
      if(pdgId==211){
	nrofPion.emplace_back(k);
	h_genParScalarCPionPt->Fill(event.genParPt[k]);
	h_genParScalarCPionEta->Fill(genParEta);
	h_genParScalarCPionPhi->Fill(genParPhi);
	h_genParScalarCPionE->Fill(genParE);
	h_VertexPosXY->Fill(genParVx,genParVy);
	h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
	h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
      }
      //Neutral pion from scalar decay
      if(pdgId==111){
	nrofPion.emplace_back(k);
	h_genParScalarNPionPt->Fill(event.genParPt[k]);
	h_genParScalarNPionEta->Fill(genParEta);
	h_genParScalarNPionPhi->Fill(genParPhi);
	h_genParScalarNPionE->Fill(genParE);
	//h_VertexPosXY->Fill(genParVx,genParVy);
	//h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
	//h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
      }
    }
            
  }//end for-loop
        
  if(gen1!=0 && gen2!=0){
    h_genParScalarMuonPtL->Fill(gen1);
    h_genParScalarMuonPtS->Fill(gen2);
	  
    if(event.muTrig()){
      h_genParScalarMuonCutPtSL->Fill(gen1); //leading momenta for the event
    }
    if(event.mumuTrig()){
      h_genParScalarMuonCutPtDL->Fill(gen1);
      h_genParScalarMuonCutPtDS->Fill(gen2);
    }
  } 
           
          
  if(nrofScalar.size()==2){ //Two-particle (scalar) correlations
    const Int_t Nr1 {nrofScalar[0]}; //Give the scalar index value k
    const Int_t Nr2 {nrofScalar[1]};
            
    //DeltaR, DeltaPhi
    TLorentzVector nr1;
    TLorentzVector nr2;
            
    nr1.SetPtEtaPhiE(event.genParPt[Nr1],event.genParEta[Nr1],event.genParPhi[Nr1],event.genParE[Nr1]);
    nr2.SetPtEtaPhiE(event.genParPt[Nr2],event.genParEta[Nr2],event.genParPhi[Nr2],event.genParE[Nr2]);
            
    h_ScalarDeltaR->Fill(nr1.DeltaR(nr2));//Get DeltaR between nr1scalar and nr2scalar
    h_ScalarDeltaPhi->Fill(nr1.DeltaPhi(nr2));
        
    //Invariant mass
    TLorentzVector mass1 {nr1.Px(),nr1.Py(),nr1.Pz(),event.genParE[Nr1]};
    TLorentzVector mass2 {nr2.Px(),nr2.Py(),nr2.Pz(),event.genParE[Nr2]};
      
    h_ScalarInvMass->Fill((mass1+mass2).M());
            
    //3D angle
    TVector3 angle1 (event.genParVx[Nr1],event.genParVy[Nr1],event.genParVz[Nr1]); //No actual angle
    TVector3 angle2 (event.genParVx[Nr2],event.genParVy[Nr2],event.genParVz[Nr2]);
        
    h_Scalar3DAngle->Fill(angle1.Angle(angle2));
  }
        
        
  //Now same procedure for kaons,pions,muons,Kshort
        
  if(nrofMuon.size()==2){
    const Int_t Nr1 {nrofMuon[0]};
    const Int_t Nr2 {nrofMuon[1]};
            
    TLorentzVector nr1;
    TLorentzVector nr2;
            
    nr1.SetPtEtaPhiE(event.genParPt[Nr1],event.genParEta[Nr1],event.genParPhi[Nr1],event.genParE[Nr1]);
    nr2.SetPtEtaPhiE(event.genParPt[Nr2],event.genParEta[Nr2],event.genParPhi[Nr2],event.genParE[Nr2]);
            
    h_MuonDeltaR->Fill(nr1.DeltaR(nr2));
    h_MuonDeltaPhi->Fill(nr1.DeltaPhi(nr2));
      
    //Invariant mass
    TLorentzVector mass1 {nr1.Px(),nr1.Py(),nr1.Pz(),event.genParE[Nr1]};
    TLorentzVector mass2 {nr2.Px(),nr2.Py(),nr2.Pz(),event.genParE[Nr2]};
      
    h_MuonInvMass->Fill((mass1+mass2).M());
      
    //3D angle
    TVector3 angle1 (event.genParVx[Nr1],event.genParVy[Nr1],event.genParVz[Nr1]);
    TVector3 angle2 (event.genParVx[Nr2],event.genParVy[Nr2],event.genParVz[Nr2]);

    h_Muon3DAngle->Fill(angle1.Angle(angle2));
  }
        
  if(nrofKaon.size()==2){
    const Int_t Nr1 {nrofKaon[0]};
    const Int_t Nr2 {nrofKaon[1]};
    
    TLorentzVector nr1;
    TLorentzVector nr2;
            
    nr1.SetPtEtaPhiE(event.genParPt[Nr1],event.genParEta[Nr1],event.genParPhi[Nr1],event.genParE[Nr1]);
    nr2.SetPtEtaPhiE(event.genParPt[Nr2],event.genParEta[Nr2],event.genParPhi[Nr2],event.genParE[Nr2]);
            
    h_KaonDeltaR->Fill(nr1.DeltaR(nr2));
    h_KaonDeltaPhi->Fill(nr1.DeltaPhi(nr2));
        
    //Invariant mass
    TLorentzVector mass1 {nr1.Px(),nr1.Py(),nr1.Pz(),event.genParE[Nr1]};
    TLorentzVector mass2 {nr2.Px(),nr2.Py(),nr2.Pz(),event.genParE[Nr2]};
      
    h_KaonInvMass->Fill((mass1+mass2).M());
        
    //3D angle
    TVector3 angle1;
    TVector3 angle2;
        
    angle1.SetXYZ(event.genParVx[Nr1],event.genParVy[Nr1],event.genParVz[Nr1]);
    angle2.SetXYZ(event.genParVx[Nr2],event.genParVy[Nr2],event.genParVz[Nr2]);
          
    h_Kaon3DAngle->Fill(angle1.Angle(angle2));
        
  }
        
  if(nrofKShort.size()==2){
    const Int_t Nr1 {nrofKShort[0]};
    const Int_t Nr2 {nrofKShort[1]};
            
    TLorentzVector nr1;
    TLorentzVector nr2;
            
    nr1.SetPtEtaPhiE(event.genParPt[Nr1],event.genParEta[Nr1],event.genParPhi[Nr1],event.genParE[Nr1]);
    nr2.SetPtEtaPhiE(event.genParPt[Nr2],event.genParEta[Nr2],event.genParPhi[Nr2],event.genParE[Nr2]);
            
    h_KShortDeltaR->Fill(nr1.DeltaR(nr2));
    h_KShortDeltaPhi->Fill(nr1.DeltaPhi(nr2));
        
    //Invariant mass
    TLorentzVector mass1 {nr1.Px(),nr1.Py(),nr1.Pz(),event.genParE[Nr1]};
    TLorentzVector mass2 {nr2.Px(),nr2.Py(),nr2.Pz(),event.genParE[Nr2]};
      
    h_KShortInvMass->Fill((mass1+mass2).M());
          
    //3D angle
    TVector3 angle1;
    TVector3 angle2;
        
    angle1.SetXYZ(event.genParVx[Nr1],event.genParVy[Nr1],event.genParVz[Nr1]);
    angle2.SetXYZ(event.genParVx[Nr2],event.genParVy[Nr2],event.genParVz[Nr2]);
    
    h_KShort3DAngle->Fill(angle1.Angle(angle2));
  }
        
        
  if(nrofPion.size()==2){
    const Int_t Nr1 {nrofPion[0]};
    const Int_t Nr2 {nrofPion[1]};
    
    TLorentzVector nr1;
    TLorentzVector nr2;
            
    nr1.SetPtEtaPhiE(event.genParPt[Nr1],event.genParEta[Nr1],event.genParPhi[Nr1],event.genParE[Nr1]);
    nr2.SetPtEtaPhiE(event.genParPt[Nr2],event.genParEta[Nr2],event.genParPhi[Nr2],event.genParE[Nr2]);
            
    h_PionDeltaR->Fill(nr1.DeltaR(nr2));
    h_PionDeltaPhi->Fill(nr1.DeltaPhi(nr2));
        
    //Invariant mass
    TLorentzVector mass1 {nr1.Px(),nr1.Py(),nr1.Pz(),event.genParE[Nr1]};
    TLorentzVector mass2 {nr2.Px(),nr2.Py(),nr2.Pz(),event.genParE[Nr2]};
      
    h_PionInvMass->Fill((mass1+mass2).M());
           
    //3D angle
    TVector3 angle1;
    TVector3 angle2;
        
    angle1.SetXYZ(event.genParVx[Nr1],event.genParVy[Nr1],event.genParVz[Nr1]);
    angle2.SetXYZ(event.genParVx[Nr2],event.genParVy[Nr2],event.genParVz[Nr2]);
      
    h_Pion3DAngle->Fill(angle1.Angle(angle2));
  }
        
  /// END GENERATOR PARTICLE STUFF
}

void SimpleAnalysis::fillMuonReconstructionPlots(const AnalysisEvent& event, double& eventWeight, const int& muIndex1, const int& muIndex2) const {
  /// BEGIN Muon Reconstruction

  std::vector<Int_t> passedMuons; // vector of PF+PAT muons that pass selection criteria
 
  // ADM - Start loop over PAT muons for quantities without triggers applied
  for(Int_t k{0}; k < event.numMuonPF2PAT; k++) {
             
    const Float_t muonRecPt { event.muonPF2PATPt[k] };
    const Float_t muonRecEta { event.muonPF2PATEta[k] };
    const Float_t muonRecPhi { event.muonPF2PATPhi[k] };
    const Float_t muonRecE { event.muonPF2PATE[k] };
           
    // Fill kinematic plots for PAT muons pre-trigger
    h_muonRecPt->Fill(muonRecPt, eventWeight);
    h_muonRecEta->Fill(muonRecEta, eventWeight);
    h_muonRecPhi->Fill(muonRecPhi, eventWeight);
    h_muonRecE->Fill(muonRecE, eventWeight);
  }   
  // Finish loop over PAT muons

  // consider selected muons
  if(event.muTrig()){

    TLorentzVector muonRec1 {event.muonPF2PATPX[muIndex1], event.muonPF2PATPY[muIndex1], event.muonPF2PATPZ[muIndex1], event.muonPF2PATE[muIndex1]};
    TLorentzVector muonRec2 {event.muonPF2PATPX[muIndex2], event.muonPF2PATPY[muIndex2], event.muonPF2PATPZ[muIndex2], event.muonPF2PATE[muIndex2]};
                          
    h_muonRecDeltaR->Fill(muonRec1.DeltaR(muonRec2), eventWeight);
    h_muonRecDeltaPhi->Fill(muonRec1.DeltaPhi(muonRec2), eventWeight);
            
    h_muonRecInvMass->Fill( (muonRec1+muonRec2).M(), eventWeight );
    //h_muonRecInvMass->Fit(Gaussian1);
  }
        
  h_muonRecPtL->Fill(event.muonPF2PATPt[muIndex1], eventWeight); // Plot selected leading muon pT (no trigger applied) as done in CB's original code
  h_muonRecPtS->Fill(event.muonPF2PATPt[muIndex2], eventWeight); // Ditto for subleading
        
  //To show seperate turn-on curve for single or double muon trigger
  if(event.muTrig() ){
    h_muonCutSingleL->Fill(event.muonPF2PATPt[muIndex1], eventWeight);
  }   
	       
  if(event.mumuTrig()){
    h_muonCutDoubleL->Fill(event.muonPF2PATPt[muIndex1], eventWeight);
    h_muonCutDoubleS->Fill(event.muonPF2PATPt[muIndex2], eventWeight);
  }
    
  //END Muon Reconstruction function
}

void SimpleAnalysis::fillPackedCandidatePlots(const AnalysisEvent& event, double& eventWeight, Float_t& Nbg1, Float_t& Nbg2, Float_t& Obs, Float_t& rate_signal, Float_t& stat_signal, Float_t& QCD_Kantiscalar, Float_t& QCD_Pantiscalar, Float_t& QCD_20Kantiscalar, Float_t& QCD_20Pantiscalar, Float_t& QCD_scalar, Float_t& QCD_20scalar, const int& patMuIndex1, const int& patMuIndex2, const int& muIndex1, const int& muIndex2, const int& chsIndex1, const int& chsIndex2) const {

  //BEGIN Packed candidates

  Float_t mupt1{event.packedCandsPseudoTrkPt[muIndex1]}, mupt2{event.packedCandsPseudoTrkPt[muIndex2]};
  Float_t chspt1{event.packedCandsPseudoTrkPt[chsIndex1]}, chspt2{event.packedCandsPseudoTrkPt[chsIndex2]};

  // Loop over all packed cands to plot displacement info
  /*for(Int_t k{0};k<event.numPackedCands;k++) {
     
    // plot Dxy and Dz of all packed cands
    h_packedCDxy->Fill(event.packedCandsDxy[k], eventWeight);
    h_packedCDz->Fill(event.packedCandsDz[k], eventWeight);

    // Plot Vx, Vy, Vz of all charged tracks
    if(event.packedCandsHasTrackDetails[k]==1){
            
      const Int_t packedCandsPseudoTrkCharge {event.packedCandsPseudoTrkCharge[k]};
      const Int_t packedCandsCharge {event.packedCandsCharge[k]};
            
      if(packedCandsCharge!=0 && packedCandsCharge==packedCandsPseudoTrkCharge){ //No neutral particles as they don't leave tracks, only charged
               
	h_packedCVx->Fill(event.packedCandsPseudoTrkVx[k], eventWeight);
	h_packedCVy->Fill(event.packedCandsPseudoTrkVy[k], eventWeight);
	h_packedCVz->Fill(event.packedCandsPseudoTrkVz[k], eventWeight);
              
	//Displacement from interaction point
	// h_displacedXY->Fill(event.packedCandsPseudoTrkVx[k],event.packedCandsPseudoTrkVy[k], eventWeight);
	// h_displacedRZ->Fill(std::abs(event.packedCandsPseudoTrkVz[k]),std::sqrt(event.packedCandsPseudoTrkVx[k]*event.packedCandsPseudoTrkVx[k]+event.packedCandsPseudoTrkVy[k]*event.packedCandsPseudoTrkVy[k]), eventWeight);
			
      }
    }
	      
  }//end plot displacement info plot*/

  /////

  // Initialise variables used to calculate isolation and invariant masses
    
  Float_t KIsoSum1 {0}, KIsoSum2 {0}; // Isolation for leading/subleading kaons
  Float_t PIsoSum1 {0}, PIsoSum2 {0}; // Isolation for leading/subleading pions
  Float_t MuonIsoSum1 {0}, MuonIsoSum2 {0}; // Isolation for leading/subleading muons

  Float_t RKIsoSum1 {0}, RKIsoSum2 {0}; // Isolation for leading/subleading kaons
  Float_t RPIsoSum1 {0}, RPIsoSum2 {0}; // Isolation for leading/subleading pions
  Float_t RMuonIsoSum1 {0}, RMuonIsoSum2 {0}; // Isolation for leading/subleading muons
	
  Float_t Khadroninv, Phadroninv, muoninv; // invariant masses

  Float_t Kpx{0}, Kpy{0}, Kpz{0}, KE{0};
  Float_t KMpx{0}, KMpy{0}, KMpz{0}, KME{0};

  Float_t Ppx {0}, Ppy {0}, Ppz {0}, PE{0};
  Float_t PMpx {0}, PMpy {0}, PMpz {0}, PME{0};
	      
  TLorentzVector mm1, mm2; // muon LVecs
  TLorentzVector kaonLVec1; TLorentzVector kaonLVec2;
  TLorentzVector pionLVec1; TLorentzVector pionLVec2;
  TLorentzVector muonLVec1; TLorentzVector muonLVec2;
  TLorentzVector packedCandLVec;
  
  TLorentzVector Rmm1, Rmm2;
  TLorentzVector RkaonLVec1; TLorentzVector RkaonLVec2;
  TLorentzVector RpionLVec1; TLorentzVector RpionLVec2;
  TLorentzVector RmuonLVec1; TLorentzVector RmuonLVec2;
  TLorentzVector R0packedCandLVec; TLorentzVector R1packedCandLVec; TLorentzVector R2packedCandLVec; TLorentzVector R3packedCandLVec; TLorentzVector R4packedCandLVec; TLorentzVector R5packedCandLVec;
	
  TLorentzVector scalarLVec, Kantiscalar, Pantiscalar;
  TLorentzVector RscalarLVec, RKantiscalar, RPantiscalar;
	
  Double_t knentries1; Double_t knentries2; Double_t knentries3; Double_t knentries4;
  Double_t pnentries1; Double_t pnentries2; Double_t pnentries3; Double_t pnentries4;
  Double_t mnentries1; Double_t mnentries2; Double_t mnentries3; Double_t mnentries4;
  
  Float_t Khiggs{0}, Phiggs{0};     	

  // Do charged hadrons bit here
  // Skip if null charged hadron indicies or 0 momentum charged hadrons

  // Setup LVecs for the charged hadrons for delR plotting
  kaonLVec1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[chsIndex1],event.packedCandsPseudoTrkEta[chsIndex1],event.packedCandsPseudoTrkPhi[chsIndex1],std::sqrt(event.packedCandsE[chsIndex1]*event.packedCandsE[chsIndex1]-std::pow(0.1396,2)+std::pow(0.494,2)));
  kaonLVec2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[chsIndex2],event.packedCandsPseudoTrkEta[chsIndex2],event.packedCandsPseudoTrkPhi[chsIndex2],std::sqrt(event.packedCandsE[chsIndex2]*event.packedCandsE[chsIndex2]-std::pow(0.1396,2)+std::pow(0.494,2)));

 // RkaonLVec1.SetPtEtaPhiE(event.chsTkPairTk1Pt[chsIndex1],event.chsTkPairTk1Eta[chsIndex1],event.chsTkPairTk1Phi[chsIndex1],std::sqrt(event.chsTkPairTk1P2[chsIndex1]+std::pow(0.494,2)*std::pow(0.494,2)));
 // RkaonLVec2.SetPtEtaPhiE(event.chsTkPairTk2Pt[chsIndex2],event.chsTkPairTk2Eta[chsIndex2],event.chsTkPairTk2Phi[chsIndex2],std::sqrt(event.chsTkPairTk2P2[chsIndex2]+std::pow(0.494,2)*std::pow(0.494,2)));

  h_KhadronDeltaR->Fill(kaonLVec1.DeltaR(kaonLVec2), eventWeight);

  // Setup LVecs for the charged pions for delR plotting
  pionLVec1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[chsIndex1],event.packedCandsPseudoTrkEta[chsIndex1],event.packedCandsPseudoTrkPhi[chsIndex1],event.packedCandsE[chsIndex1]);
  pionLVec2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[chsIndex2],event.packedCandsPseudoTrkEta[chsIndex2],event.packedCandsPseudoTrkPhi[chsIndex2],event.packedCandsE[chsIndex2]);
  
 // RpionLVec1.SetPtEtaPhiE(event.chsTkPairTk1Pt[chsIndex1],event.chsTkPairTk1Eta[chsIndex1],event.chsTkPairTk1Phi[chsIndex1],std::sqrt(event.chsTkPairTk1P2[chsIndex1]+std::pow(0.1396,2)*std::pow(0.1396,2)));
 // RpionLVec2.SetPtEtaPhiE(event.chsTkPairTk2Pt[chsIndex2],event.chsTkPairTk2Eta[chsIndex2],event.chsTkPairTk2Phi[chsIndex2],std::sqrt(event.chsTkPairTk2P2[chsIndex2]+std::pow(0.1396,2)*std::pow(0.1396,2)));

  h_PhadronDeltaR->Fill(pionLVec1.DeltaR(pionLVec2), eventWeight);

  // Setup LVecs for the muons for delR plotting
  mm1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex1],event.packedCandsPseudoTrkEta[muIndex1],event.packedCandsPseudoTrkPhi[muIndex1],event.packedCandsE[muIndex1]);
  mm2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex2],event.packedCandsPseudoTrkEta[muIndex2],event.packedCandsPseudoTrkPhi[muIndex2],event.packedCandsE[muIndex2]);
  
  Int_t muonTrkPairIndex = getMuonTrackPairIndex(event, patMuIndex1, patMuIndex2);
	
 // Rmm1.SetPtEtaPhiE(event.muonTkPairPF2PATTk1Pt[muonTrkPairIndex],event.muonTkPairPF2PATTk1Eta[muonTrkPairIndex],event.muonTkPairPF2PATTk1Phi[muonTrkPairIndex],std::sqrt(event.chsTkPairTk1P2[muonTrkPairIndex]+std::pow(0.106,2)*std::pow(0.106,2)));
 // Rmm2.SetPtEtaPhiE(event.muonTkPairPF2PATTk2Pt[muonTrkPairIndex],event.muonTkPairPF2PATTk2Eta[muonTrkPairIndex],event.muonTkPairPF2PATTk2Phi[muonTrkPairIndex],std::sqrt(event.chsTkPairTk2P2[muonTrkPairIndex]+std::pow(0.106,2)*std::pow(0.106,2)));

  h_KmuonsDeltaR->Fill(mm1.DeltaR(mm2), eventWeight);
  h_PmuonsDeltaR->Fill(mm1.DeltaR(mm2), eventWeight); // Identical, but preserved

  for(Int_t k{0};k<event.numPackedCands;k++){
     packedCandLVec.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
  
     if(k!=chsIndex1 && k!=chsIndex2){      
       if(kaonLVec1.DeltaR(packedCandLVec)<isoConeSize_) KIsoSum1+=event.packedCandsPseudoTrkPt[k];
       if(kaonLVec2.DeltaR(packedCandLVec)<isoConeSize_) KIsoSum2+=event.packedCandsPseudoTrkPt[k];
	     

       if(pionLVec1.DeltaR(packedCandLVec)<isoConeSize_) PIsoSum1+=event.packedCandsPseudoTrkPt[k];
       if(pionLVec2.DeltaR(packedCandLVec)<isoConeSize_) PIsoSum2+=event.packedCandsPseudoTrkPt[k];
     }  
     if(k!=muIndex1 && k!=muIndex2){      
       if(mm1.DeltaR(packedCandLVec)<isoConeSize_) MuonIsoSum1+=event.packedCandsPseudoTrkPt[k];
       if(mm2.DeltaR(packedCandLVec)<isoConeSize_) MuonIsoSum2+=event.packedCandsPseudoTrkPt[k];  
     }
  }
	
  /*for(Int_t k{0};k<event.numChsTrackPairs;k++){
     R0packedCandLVec.SetPtEtaPhiE(event.chsTkPairTk1Pt[k],event.chsTkPairTk1Eta[k],event.chsTkPairTk1Phi[k],std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.494,2)*std::pow(0.494,2)));
     R1packedCandLVec.SetPtEtaPhiE(event.chsTkPairTk2Pt[k],event.chsTkPairTk2Eta[k],event.chsTkPairTk2Phi[k],std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.494,2)*std::pow(0.494,2)));
    
     R4packedCandLVec.SetPtEtaPhiE(event.chsTkPairTk1Pt[k],event.chsTkPairTk1Eta[k],event.chsTkPairTk1Phi[k],std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.1396,2)*std::pow(0.1396,2)));
     R5packedCandLVec.SetPtEtaPhiE(event.chsTkPairTk2Pt[k],event.chsTkPairTk2Eta[k],event.chsTkPairTk2Phi[k],std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.1396,2)*std::pow(0.1396,2)));
  
     if(k!=chsIndex1 && k!=chsIndex2){      
       if(RkaonLVec1.DeltaR(R0packedCandLVec)<isoConeSize_) RKIsoSum1+=event.packedCandsPseudoTrkPt[k];
       if(RkaonLVec2.DeltaR(R1packedCandLVec)<isoConeSize_) RKIsoSum2+=event.packedCandsPseudoTrkPt[k];
	     
       if(RpionLVec1.DeltaR(R4packedCandLVec)<isoConeSize_) RPIsoSum1+=event.packedCandsPseudoTrkPt[k];
       if(RpionLVec2.DeltaR(R5packedCandLVec)<isoConeSize_) RPIsoSum2+=event.packedCandsPseudoTrkPt[k];
     }  
  }
  for(Int_t k{0};k<event.numMuonTrackPairsPF2PAT;k++){
     R2packedCandLVec.SetPtEtaPhiE(event.muonTkPairPF2PATTk1Pt[k],event.muonTkPairPF2PATTk1Eta[k],event.muonTkPairPF2PATTk1Phi[k],std::sqrt(event.muonTkPairPF2PATTk1P2[k]+std::pow(0.106,2)*std::pow(0.106,2)));
     R3packedCandLVec.SetPtEtaPhiE(event.muonTkPairPF2PATTk2Pt[k],event.muonTkPairPF2PATTk2Eta[k],event.muonTkPairPF2PATTk2Phi[k],std::sqrt(event.muonTkPairPF2PATTk2P2[k]+std::pow(0.106,2)*std::pow(0.106,2)));

     if(k!=muonTrkPairIndex){      
       if(Rmm1.DeltaR(R2packedCandLVec)<isoConeSize_) RMuonIsoSum1+=event.muonTkPairPF2PATTk1Pt[k];
       if(Rmm2.DeltaR(R3packedCandLVec)<isoConeSize_) RMuonIsoSum2+=event.muonTkPairPF2PATTk2Pt[k];  
     }  
  }*/
	
	
  // Kaon bit
  if(kaonLVec1.DeltaR(kaonLVec2)<packedCandKaonDeltaR_){
    // Repeat LVecs for the charged hadrons for delR plotting - why?
    TLorentzVector lhadron1 {event.packedCandsPseudoTrkPx[chsIndex1], event.packedCandsPseudoTrkPy[chsIndex1], event.packedCandsPseudoTrkPz[chsIndex1], std::sqrt(event.packedCandsE[chsIndex1]*event.packedCandsE[chsIndex1]-std::pow(0.1396,2)+std::pow(0.494,2))};
    TLorentzVector lhadron2 {event.packedCandsPseudoTrkPx[chsIndex2], event.packedCandsPseudoTrkPy[chsIndex2], event.packedCandsPseudoTrkPz[chsIndex2], std::sqrt(event.packedCandsE[chsIndex2]*event.packedCandsE[chsIndex2]-std::pow(0.1396,2)+std::pow(0.494,2))};

    //Invariant mass for two hadrons
    Khadroninv=(lhadron1+lhadron2).M();

    // Plot kaon invariant mass twice, but in different plots?
    h_KhadronInvMass->Fill( (lhadron1+lhadron2).M(), eventWeight);
    h_KhadronInvMass2->Fill( (lhadron1+lhadron2).M(), eventWeight); // This one appears to have finer binning than the above. Was not obvious before looking at constructor
		
    // Vector addition - sum together the two kaons explicitly using each component
    if(event.packedCandsPseudoTrkCharge[chsIndex1]==-(event.packedCandsPseudoTrkCharge[chsIndex2])){
      Kpx=event.packedCandsPseudoTrkPx[chsIndex1]+event.packedCandsPseudoTrkPx[chsIndex2];
      Kpy=event.packedCandsPseudoTrkPy[chsIndex1]+event.packedCandsPseudoTrkPy[chsIndex2];
      Kpz=event.packedCandsPseudoTrkPz[chsIndex1]+event.packedCandsPseudoTrkPz[chsIndex2];
      KE=std::sqrt(event.packedCandsE[chsIndex1]*event.packedCandsE[chsIndex1]-std::pow(0.1396,2)+std::pow(0.494,2))+std::sqrt(event.packedCandsE[chsIndex2]*event.packedCandsE[chsIndex2]-std::pow(0.1396,2)+std::pow(0.494,2));
		
      Kantiscalar.SetPxPyPzE(Kpx,Kpy,Kpz,KE);
      h_TestKantiscalarInvMass->Fill(Kantiscalar.M(), eventWeight);
    }
   
  } // End Kaon delR if
	

  // pion bit
  if(pionLVec1.DeltaR(pionLVec2)<packedCandPionDeltaR_){
    //Invariant mass for two pions
    // Repeat LVecs for the charged hadrons for delR plotting - why?
    TLorentzVector lhadron1 {event.packedCandsPseudoTrkPx[chsIndex1], event.packedCandsPseudoTrkPy[chsIndex1], event.packedCandsPseudoTrkPz[chsIndex1],event.packedCandsE[chsIndex1]};
    TLorentzVector lhadron2 {event.packedCandsPseudoTrkPx[chsIndex2], event.packedCandsPseudoTrkPy[chsIndex2], event.packedCandsPseudoTrkPz[chsIndex2],event.packedCandsE[chsIndex2]};
    Phadroninv=(lhadron1+lhadron2).M();
    h_PhadronInvMass->Fill((lhadron1+lhadron2).M(), eventWeight);
    h_PhadronInvMass2->Fill((lhadron1+lhadron2).M(), eventWeight);
		
    if(event.packedCandsPseudoTrkCharge[chsIndex1]==-(event.packedCandsPseudoTrkCharge[chsIndex2])){
      //Vector addition
      Ppx=event.packedCandsPseudoTrkPx[chsIndex1]+event.packedCandsPseudoTrkPx[chsIndex2];
      Ppy=event.packedCandsPseudoTrkPy[chsIndex1]+event.packedCandsPseudoTrkPy[chsIndex2];
      Ppz=event.packedCandsPseudoTrkPz[chsIndex1]+event.packedCandsPseudoTrkPz[chsIndex2];
      PE=event.packedCandsE[chsIndex1]+event.packedCandsE[chsIndex2];//std::sqrt(event.packedCandsE[chsIndex1]*event.packedCandsE[chsIndex1]-std::pow(0.1396,2)+std::pow(0.494,2))+std::sqrt(event.packedCandsE[chsIndex2]*event.packedCandsE[chsIndex2]-std::pow(0.1396,2)+std::pow(0.494,2));

      Pantiscalar.SetPxPyPzE(Ppx,Ppy,Ppz,PE);
      h_TestPantiscalarInvMass->Fill(Pantiscalar.M(), eventWeight);
    }
   
  } // End pion delR if

  // muon bit
  if(mm1.DeltaR(mm2)<packedCandMuonDeltaR_){
		   
    h_PATmuonEta->Fill(event.muonPF2PATEta[patMuIndex1]); h_PATmuonEta->Fill(event.muonPF2PATEta[patMuIndex2]);  
	  
    TLorentzVector lmuon1 {event.packedCandsPseudoTrkPx[muIndex1], event.packedCandsPseudoTrkPy[muIndex1], event.packedCandsPseudoTrkPz[muIndex1], event.packedCandsE[muIndex1]};
    TLorentzVector lmuon2 {event.packedCandsPseudoTrkPx[muIndex2], event.packedCandsPseudoTrkPy[muIndex2], event.packedCandsPseudoTrkPz[muIndex2], event.packedCandsE[muIndex2]};
                        
    muoninv=(lmuon1+lmuon2).M();
    h_KmuonsInvMass->Fill((lmuon1+lmuon2).M(), eventWeight);
	
    if(event.packedCandsPseudoTrkCharge[muIndex1]==-(event.packedCandsPseudoTrkCharge[muIndex2])){
      KMpx=event.packedCandsPseudoTrkPx[muIndex1]+event.packedCandsPseudoTrkPx[muIndex2];
      KMpy=event.packedCandsPseudoTrkPy[muIndex1]+event.packedCandsPseudoTrkPy[muIndex2];
      KMpz=event.packedCandsPseudoTrkPz[muIndex1]+event.packedCandsPseudoTrkPz[muIndex2];
      KME=event.packedCandsE[muIndex1]+event.packedCandsE[muIndex2];
	      
      scalarLVec.SetPxPyPzE(KMpx,KMpy,KMpz,KME);
      h_TestscalarInvMass->Fill(scalarLVec.M(), eventWeight);
    }
		   
  }
 
  h_TestPhiggsInvMass->Fill((Pantiscalar+scalarLVec).M(), eventWeight);
  h_TestKhiggsInvMass->Fill((Kantiscalar+scalarLVec).M(), eventWeight);
  h_higgsassump->Fill((Pantiscalar+scalarLVec).M(),(Kantiscalar+scalarLVec).M(), eventWeight);
	
  //Refitted objects
  TLorentzVector refkaon; TLorentzVector refpion; TLorentzVector refitmuon;	   
  for(Int_t k{0}; k<event.numChsTrackPairs;k++){
     if(event.chsTkPairIndex1[k]==chsIndex1 && event.chsTkPairIndex2[k]==chsIndex2){
       if(event.chsTkPairTk1Charge[k]==-(event.chsTkPairTk2Charge[k])){
	 TLorentzVector ka1 {event.chsTkPairTk1Px[k], event.chsTkPairTk1Py[k], event.chsTkPairTk1Pz[k], std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.494,2))};
         TLorentzVector ka2 {event.chsTkPairTk2Px[k], event.chsTkPairTk2Py[k], event.chsTkPairTk2Pz[k], std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.494,2))};
	 
	 refkaon=ka1+ka2;
	       
	 TLorentzVector pi1 {event.chsTkPairTk1Px[k], event.chsTkPairTk1Py[k], event.chsTkPairTk1Pz[k], std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.1396,2))};
         TLorentzVector pi2 {event.chsTkPairTk2Px[k], event.chsTkPairTk2Py[k], event.chsTkPairTk2Pz[k], std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.1396,2))};
	 
	 refpion=pi1+pi2;
       }
     }
  }
  if(event.muonTkPairPF2PATTk1Charge[muonTrkPairIndex]==-(event.muonTkPairPF2PATTk2Charge[muonTrkPairIndex])){
    TLorentzVector Mu1 {event.muonTkPairPF2PATTk1Px[muonTrkPairIndex], event.muonTkPairPF2PATTk1Py[muonTrkPairIndex], event.muonTkPairPF2PATTk1Pz[muonTrkPairIndex], std::sqrt(event.muonTkPairPF2PATTk1P2[muonTrkPairIndex]+std::pow(0.106,2))};
    TLorentzVector Mu2 {event.muonTkPairPF2PATTk2Px[muonTrkPairIndex], event.muonTkPairPF2PATTk2Py[muonTrkPairIndex], event.muonTkPairPF2PATTk2Pz[muonTrkPairIndex], std::sqrt(event.muonTkPairPF2PATTk2P2[muonTrkPairIndex]+std::pow(0.106,2))};
	  
    refitmuon=Mu1+Mu2;	
  }
	      
	      
	      
  //Kaon 
  h_kNentries->SetBinContent(1,eventWeight); 
	
  if(kaonLVec1.DeltaR(kaonLVec2)<packedCandKaonDeltaR_){
    h_KIsoSum1->Fill(KIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1], eventWeight);
    h_KIsoSum2->Fill(KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2], eventWeight);
  }
  h_KIso2->Fill(KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2],event.packedCandsPseudoTrkPt[chsIndex2], eventWeight);
  
  if(KIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1){
     h_kNentries->SetBinContent(2,eventWeight);
	   
     if(std::abs((Kantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWindow_){
       h_kNentries->SetBinContent(3,eventWeight);

       //if(std::abs((Kantiscalar).M()-(scalarLVec).M())<statWindow_){
       if(std::abs((Kantiscalar).M()-mass_range_)<statWindow_){
         h_KantiscalarInvMass->Fill(Kantiscalar.M(), eventWeight);
	 QCD_Kantiscalar+=eventWeight;    

         h_kNentries->SetBinContent(4,eventWeight);
       }
     }
	  
     if(std::abs((Kantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWideWindow_){
      //if(std::abs((Kantiscalar).M()-(scalarLVec).M())<statWindow_){
      if(std::abs((Kantiscalar).M()-mass_range_)<statWindow_){
          h_K20antiscalarInvMass->Fill(Kantiscalar.M(), eventWeight);
	  QCD_20Kantiscalar+=eventWeight;
      }
    }
  } 
	
	
  //kaon higgs
  if(KIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){
    if(std::abs((Kantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((Kantiscalar).M()-(scalarLVec).M())<statWindow_){
          Khiggs=(Kantiscalar+scalarLVec).M();
          h_KhiggsInvMass->Fill(Khiggs, eventWeight);
          //h_KhiggsDeltaR->Fill(Kantiscalar.DeltaR(scalarLVec), eventWeight);
          //h_Kinvmass->Fill(Khadroninv,muoninv, eventWeight);
      } 
    }
    if(std::abs((Kantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((Kantiscalar).M()-(scalarLVec).M())<statWindow_){
        Khiggs=(Kantiscalar+scalarLVec).M();
        h_K20higgsInvMass->Fill(Khiggs, eventWeight);
      } 
    } 
  }

  //Kaon refitted and wider window	
  if(KIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1){	    
    if(std::abs((refkaon+refitmuon).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((refkaon).M()-mass_range_)<statWindow_){   
	h_Rkaonre12InvMass->Fill((refkaon).M(), eventWeight);
      }
    }
    if(std::abs((refkaon+refitmuon).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((refkaon).M()-mass_range_)<statWindow_){
	h_widekaonRInvMass->Fill((refkaon).M(), eventWeight);
      }
    }    
  }
      

  //Pion,muon 
	
  if(pionLVec1.DeltaR(pionLVec2)<packedCandPionDeltaR_){	
  h_PIsoSum1->Fill(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1], eventWeight);
  h_PIsoSum2->Fill(PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2], eventWeight);
  }
	
  h_PIso2->Fill(PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2],event.packedCandsPseudoTrkPt[chsIndex2], eventWeight);

  if(mm1.DeltaR(mm2)<packedCandMuonDeltaR_){
  h_MuonIsoSum1->Fill(MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1], eventWeight);
  h_MuonIsoSum2->Fill(MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2], eventWeight);
  }
	
  h_pNentries->SetBinContent(1,eventWeight);
  h_mNentries->SetBinContent(1,eventWeight);
	
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1){
    h_pNentries->SetBinContent(2,eventWeight);
   
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWindow_){
      h_pNentries->SetBinContent(3,eventWeight);
	    
      //if(std::abs((Pantiscalar).M()-(scalarLVec).M())<statWindow_){
      if(std::abs((Pantiscalar).M()-mass_range_)<statWindow_){
        h_PantiscalarInvMass->Fill(Pantiscalar.M(), eventWeight);
	QCD_Pantiscalar+=eventWeight;

        h_pNentries->SetBinContent(4,eventWeight);
	      
        //Revealing the inner structure of the tracker layer?
        /*h_HVertexPosXY->Fill(event.packedCandsVx[chsIndex1],event.packedCandsVy[chsIndex1], eventWeight);
        h_HVertexPosXY->Fill(event.packedCandsVx[chsIndex2],event.packedCandsVy[chsIndex2], eventWeight);
	  	     
        h_HVertexPosRZ->Fill(event.packedCandsVz[chsIndex1],std::sqrt(event.packedCandsVx[chsIndex1]*event.packedCandsVx[chsIndex1]+event.packedCandsVy[chsIndex1]*event.packedCandsVy[chsIndex1]), eventWeight);
        h_HVertexPosRZ->Fill(event.packedCandsVz[chsIndex2],std::sqrt(event.packedCandsVx[chsIndex2]*event.packedCandsVx[chsIndex2]+event.packedCandsVy[chsIndex2]*event.packedCandsVy[chsIndex2]), eventWeight);
	*/
      }
    }
  }
  if(MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){ 
    h_mNentries->SetBinContent(2,eventWeight);
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWindow_){  
      h_mNentries->SetBinContent(3,eventWeight);
	    
      if(std::abs((scalarLVec).M()-mass_range_)<statWindow_){
        h_PscalarInvMass->Fill(scalarLVec.M(), eventWeight);
	QCD_scalar+=eventWeight;
	      
        h_mNentries->SetBinContent(4,eventWeight);
      }
    }    	  
  }
	

  //Muon,pion wider	
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1){
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((Pantiscalar).M()-mass_range_)<statWindow_){
        h_P20antiscalarInvMass->Fill(Pantiscalar.M(), eventWeight);
	QCD_20Pantiscalar+=eventWeight;
      } 
    }
  }
  if(MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((scalarLVec).M()-mass_range_)<statWindow_){
        h_P20scalarInvMass->Fill(scalarLVec.M(), eventWeight);
	QCD_20scalar+=eventWeight;
      }	  
    }
  }

  //Higgs mass and wider window
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){ 
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((Pantiscalar).M()-(scalarLVec).M())<statWindow_){
         // h_PhiggsDeltaR->Fill(Pantiscalar.DeltaR(scalarLVec), eventWeight);
         // h_Pinvmass->Fill(Phadroninv,muoninv, eventWeight);
	h_PhiggsInvMass->Fill((Pantiscalar+scalarLVec).M(), eventWeight);
	      
	Obs+=1;
	rate_signal+=eventWeight;
	stat_signal+=1;
      }
    }
    if(std::abs((Pantiscalar+scalarLVec).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((Pantiscalar).M()-(scalarLVec).M())<statWindow_){
        h_P20higgsInvMass->Fill((Pantiscalar+scalarLVec).M(), eventWeight);
      }    
    }
  }
  
	
  //Background estimate
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){ 
    if((Pantiscalar+scalarLVec).M()>105 && (Pantiscalar+scalarLVec).M()<122){
      if(std::abs((Pantiscalar).M()-(scalarLVec).M())<statWindow_){
        Nbg1+=1;//eventWeight;
      }
    }
  }
 
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){ 
    if((Pantiscalar+scalarLVec).M()>128 && (Pantiscalar+scalarLVec).M()<145){
      if(std::abs((Pantiscalar).M()-(scalarLVec).M())<statWindow_){
        Nbg2+=1;//eventWeight;
      }
    }
  }	
 
  
  //Pion refitted and wider window
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1){	    
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((refpion).M()-mass_range_)<statWindow_){   
	h_Rpionre12InvMass->Fill((refpion).M(), eventWeight);
      }
    }
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((refpion).M()-mass_range_)<statWindow_){
	h_widepionRInvMass->Fill((refpion).M(), eventWeight);
      }
    }    
  }
	
  //Muon refitted and wider window	
  if(MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((refitmuon).M()-mass_range_)<statWindow_){
	  h_Rrefit12InvMass->Fill(refitmuon.M(), eventWeight);
      }
    }
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((refitmuon).M()-mass_range_)<statWindow_){
        h_widemuonRInvMass->Fill((refitmuon).M(), eventWeight);
	}
      }
  }
  
 
	
  //Higgs refitted (pion assumption) and wider window	
  if(PIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){	
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((refpion).M()-(refitmuon).M())<statWindow_){
        h_PhiggsRInvMass->Fill((refpion+refitmuon).M(), eventWeight); 
      }
    }
    if(std::abs((refpion+refitmuon).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((refpion).M()-(refitmuon).M())<statWindow_){
        h_PhiggsR20InvMass->Fill((refpion+refitmuon).M(), eventWeight);
      }
    }
  } 

  //Higgs refitted (kaon assumption) and wider window	
  if(KIsoSum1/event.packedCandsPseudoTrkPt[chsIndex1]<0.4 && KIsoSum2/event.packedCandsPseudoTrkPt[chsIndex2]<1 && MuonIsoSum1/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && MuonIsoSum2/event.packedCandsPseudoTrkPt[muIndex2]<1){	
    if(std::abs((refkaon+refitmuon).M()-higgsMass_)<higgsMassWindow_){
      if(std::abs((refkaon).M()-(refitmuon).M())<statWindow_){
        h_KhiggsRInvMass->Fill((refkaon+refitmuon).M(), eventWeight); 
      }
    }
    if(std::abs((refkaon+refitmuon).M()-higgsMass_)<higgsMassWideWindow_){
      if(std::abs((refkaon).M()-(refitmuon).M())<statWindow_){
        h_KhiggsR20InvMass->Fill((refkaon+refitmuon).M(), eventWeight);
      }
    }
  } 
        
}//close this function



  void SimpleAnalysis::fillMuonMomentumComparisonPlots(const AnalysisEvent& event, double& eventWeight, const int& patMuIndex1, const int& patMuIndex2, const int& packedMuIndex1, const int& packedMuIndex2, const int& chsIndex1, const int& chsIndex2) const {
    //Muon momentum comparison

    //Packed candidates
    if (packedMuIndex1!=-1 && packedMuIndex2!=-1 && event.packedCandsPseudoTrkPt[packedMuIndex1]!=0 && event.packedCandsPseudoTrkPt[packedMuIndex2]!=0) {
    
      if (event.packedCandsHasTrackDetails[packedMuIndex1]==1) h_muon1packedPtTrk->Fill(event.packedCandsPseudoTrkPt[packedMuIndex1], eventWeight);
      if (event.packedCandsHasTrackDetails[packedMuIndex2]==1) h_muon2packedPtTrk->Fill(event.packedCandsPseudoTrkPt[packedMuIndex2], eventWeight);
	   
      TLorentzVector muonpackedPt1 {event.packedCandsPx[packedMuIndex1],event.packedCandsPy[packedMuIndex1],event.packedCandsPz[packedMuIndex1],event.packedCandsE[packedMuIndex1]};
      TLorentzVector muonpackedPt2 {event.packedCandsPx[packedMuIndex2],event.packedCandsPy[packedMuIndex2],event.packedCandsPz[packedMuIndex2],event.packedCandsE[packedMuIndex2]};
      h_muon1packedPt->Fill(muonpackedPt1.Pt(), eventWeight);
      h_muon2packedPt->Fill(muonpackedPt2.Pt(), eventWeight);
		
      h_muon1packedInvMass->Fill(muonpackedPt1.M(), eventWeight);
      h_muon2packedInvMass->Fill(muonpackedPt2.M(), eventWeight);
      h_muon12packedInvMass->Fill((muonpackedPt1+muonpackedPt2).M(), eventWeight);
    }

    for(Int_t k{0}; k<event.numPackedCands;k++){
      if(event.packedCandsHasTrackDetails[k]==1){
	h_muonipackedPtTrk->Fill(event.packedCandsPseudoTrkPt[k], eventWeight);
      }
      TLorentzVector muonpackedPti {event.packedCandsPx[k],event.packedCandsPy[k],event.packedCandsPz[k],event.packedCandsE[k]};
      h_muonipackedPt->Fill(muonpackedPti.Pt(), eventWeight);
      h_muonipackedInvMass->Fill(muonpackedPti.M(), eventWeight);
    }
 
	      
    //PAT muons
    for(Int_t k{0}; k<event.numMuonPF2PAT;k++){
      h_muoniRecPtTrk->Fill(event.muonPF2PATInnerTkPt[k], eventWeight);
      h_muoniRecPt->Fill(event.muonPF2PATPt[k], eventWeight);
    }  
	
	      
    //Tracks associated
    if (event.muonPF2PATInnerTkPt[patMuIndex1]!=0 && event.muonPF2PATInnerTkPt[patMuIndex2]!=0) {
      h_muon1RecPtTrk->Fill(event.muonPF2PATInnerTkPt[patMuIndex1], eventWeight);
    h_muon2RecPtTrk->Fill(event.muonPF2PATInnerTkPt[patMuIndex2], eventWeight);
  } 
  //No tracks associated
  h_muon1RecPt->Fill(event.muonPF2PATPt[patMuIndex1], eventWeight);
  h_muon2RecPt->Fill(event.muonPF2PATPt[patMuIndex2], eventWeight);
	      
  TLorentzVector VecMu1 {event.muonPF2PATPX[patMuIndex1], event.muonPF2PATPY[patMuIndex1], event.muonPF2PATPZ[patMuIndex1], event.muonPF2PATE[patMuIndex1]};
  TLorentzVector VecMu2 {event.muonPF2PATPX[patMuIndex2], event.muonPF2PATPY[patMuIndex2], event.muonPF2PATPZ[patMuIndex2], event.muonPF2PATE[patMuIndex2]};

  h_muon1RecInvMass->Fill(VecMu1.M(), eventWeight);
  h_muon2RecInvMass->Fill(VecMu2.M(), eventWeight);
  h_muon12RecInvMass->Fill((VecMu1+VecMu2).M(), eventWeight);
	     
 
	     
  //Refitted tracks muons
  int muonTrkPairIndex = getMuonTrackPairIndex(event, patMuIndex1, patMuIndex2);
  h_muon1PairsPt->Fill(event.muonTkPairPF2PATTk1Pt[muonTrkPairIndex], eventWeight);
  h_muon2PairsPt->Fill(event.muonTkPairPF2PATTk2Pt[muonTrkPairIndex], eventWeight);
		   
  TLorentzVector VecRefitMu1 {event.muonTkPairPF2PATTk1Px[muonTrkPairIndex], event.muonTkPairPF2PATTk1Py[muonTrkPairIndex], event.muonTkPairPF2PATTk1Pz[muonTrkPairIndex], event.muonPF2PATE[patMuIndex1]};
  TLorentzVector VecRefitMu2 {event.muonTkPairPF2PATTk2Px[muonTrkPairIndex], event.muonTkPairPF2PATTk2Py[muonTrkPairIndex], event.muonTkPairPF2PATTk2Pz[muonTrkPairIndex], event.muonPF2PATE[patMuIndex2]};
	 
  h_muon1refitInvMass->Fill(VecRefitMu1.M(), eventWeight);
  h_muon2refitInvMass->Fill(VecRefitMu2.M(), eventWeight);
  h_muon12refitInvMass->Fill((VecRefitMu1+VecRefitMu2).M(), eventWeight);
		  
  TLorentzVector MuAlt1 {event.muonTkPairPF2PATTk1Px[muonTrkPairIndex], event.muonTkPairPF2PATTk1Py[muonTrkPairIndex], event.muonTkPairPF2PATTk1Pz[muonTrkPairIndex], std::sqrt(event.muonTkPairPF2PATTk1P2[muonTrkPairIndex]+std::pow(0.106,2))};
  TLorentzVector MuAlt2 {event.muonTkPairPF2PATTk2Px[muonTrkPairIndex], event.muonTkPairPF2PATTk2Py[muonTrkPairIndex], event.muonTkPairPF2PATTk2Pz[muonTrkPairIndex], std::sqrt(event.muonTkPairPF2PATTk2P2[muonTrkPairIndex]+std::pow(0.106,2))};
	 
  h_refit1InvMass->Fill(MuAlt1.M(), eventWeight);
  h_refit2InvMass->Fill(MuAlt2.M(), eventWeight);
	  
  h_refit12InvMass->Fill((MuAlt1+MuAlt2).M(), eventWeight);
  
			   
  //h_muonPairsXY->Fill(event.muonTkPairPF2PATTkVx[muonTrkPairIndex],event.muonTkPairPF2PATTkVy[muonTrkPairIndex], eventWeight);
  //h_muonPairsRZ->Fill(std::abs(event.muonTkPairPF2PATTkVz[muonTrkPairIndex]),std::sqrt(event.muonTkPairPF2PATTkVx[muonTrkPairIndex]*event.muonTkPairPF2PATTkVx[muonTrkPairIndex]+event.muonTkPairPF2PATTkVy[muonTrkPairIndex]*event.muonTkPairPF2PATTkVy[muonTrkPairIndex]), )
	
      
  //Refitted tracks pions
  int chargedHadronTrkPairIndex = getChsTrackPairIndex(event, chsIndex1, chsIndex2);
		   
  TLorentzVector pi1 {event.chsTkPairTk1Px[chargedHadronTrkPairIndex], event.chsTkPairTk1Py[chargedHadronTrkPairIndex], event.chsTkPairTk1Pz[chargedHadronTrkPairIndex], std::sqrt(event.chsTkPairTk1P2[chargedHadronTrkPairIndex]+std::pow(0.1396,2))};
  TLorentzVector pi2 {event.chsTkPairTk2Px[chargedHadronTrkPairIndex], event.chsTkPairTk2Py[chargedHadronTrkPairIndex], event.chsTkPairTk2Pz[chargedHadronTrkPairIndex], std::sqrt(event.chsTkPairTk2P2[chargedHadronTrkPairIndex]+std::pow(0.1396,2))};
	 
  h_pionre12InvMass->Fill((pi1+pi2).M(), eventWeight);
}

void SimpleAnalysis::setupPlots() {

  std::cout << "START PLOTS SETUP" << std::endl;

  h_genParPt = new TH1F("h_genParPt", "genPar p_{T}", 1000, 0., 1000.);
  h_genParEta = new TH1F("h_genParEta", "genPar #eta", 200, -7., 7.);
  h_genParPhi = new TH1F("h_genParPhi", "genPar #phi", 100, -3.5, 3.5);
  h_genParE = new TH1F("h_genParE", "genPar energy", 1000, 0., 1000.);
  h_VertexPosR = new TH1F("h_VertexPosR", "Vertex position R",100,0.,250.);
    
  //Higgs boson
  h_genParHiggsPt = new TH1F("h_genParHiggsPt", "genPar h_0 p_{T}", 1000, 0., 1000.);
  h_genParHiggsEta = new TH1F("h_genParHiggsEta", "genPar h_0 #eta", 200, -7., 7.);
  h_genParHiggsPhi = new TH1F("h_genParHiggsPhi", "genPar h_0 #phi", 100, -3.5, 3.5);
  h_genParHiggsE = new TH1F("h_genParHiggsE", "genPar h_0 energy", 1000, 0., 1000.);
  h_HiggsInvMass = new TH1F("h_HiggsInvMass", "h_0 Invariant mass", 1000, 0., 1000.);
    
  //Scalar decay
  h_genParScalarPt = new TH1F("h_genParScalarPt", "Scalar p_{T}", 1000, 0., 1000.);
  h_genParScalarEta = new TH1F("h_genParScalarEta", "Scalar #eta", 200, -7., 7.);
  h_genParScalarPhi = new TH1F("h_genParScalarPhi", "Scalar #phi", 100, -3.5, 3.5);
  h_genParScalarE = new TH1F("h_genParScalarE", "Scalar energy", 1000, 0., 1000.);
  h_ScalarDeltaR = new TH1F("h_ScalarDeltaR", "Scalar #DeltaR",1500,-10., 10.);
  h_ScalarDeltaPhi = new TH1F("h_ScalarDeltaPhi", "Scalar #Delta#phi",1500, -3.5, 3.5);
  h_ScalarInvMass = new TH1F("h_ScalarInvMass", "Scalar Invariant mass",1000, 0., 7.);
  h_Scalar3DAngle = new TH1F("h_Scalar3DAngle", "Scalar 3D Angle",1000,-10., 10.);
    
  //Muon from scalar decay
  h_genParScalarMuonPtL = new TH1F("h_genParScalarMuonPtL", "#mu^{#pm} from scalar decay p_{T}, leading", 1000, 0., 1000.);
  h_genParScalarMuonPtS = new TH1F("h_genParScalarMuonPtS", "#mu^{#pm} from scalar decay p_{T}, subleading", 1000, 0., 1000.);
  h_genParScalarMuonCutPtSL = new TH1F("h_genParScalarMuonCutPtSL", "Single #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.);
  h_genParScalarMuonDivPtSL = new TH1F("h_genParScalarMuonDivPtSL", "Turn-on Single #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.);
  h_genParScalarMuonCutPtDL = new TH1F("h_genParScalarMuonCutPtDL", "Double #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.);
  h_genParScalarMuonDivPtDL = new TH1F("h_genParScalarMuonDivPtDL", "Turn-on Double #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.);
  h_genParScalarMuonCutPtDS = new TH1F("h_genParScalarMuonCutPtDS", "Double #mu^{#pm} trigger subleading p_{T}", 1000, 0., 1000.);
  h_genParScalarMuonDivPtDS = new TH1F("h_genParScalarMuonDivPtDS", "Turn-on Double #mu^{#pm} trigger subleading p_{T}", 300, 0., 1000.);
    
  h_genParScalarMuonEta = new TH1F("h_genParScalarMuonEta", "#mu^{#pm} from scalar decay #eta", 200, -7., 7.);
  h_genParScalarMuonPhi = new TH1F("h_genParScalarMuonPhi", "#mu^{#pm} from scalar decay #phi", 100, -3.5, 3.5);
  h_genParScalarMuonE = new TH1F("h_genParScalarMuonE", "#mu^{#pm} from scalar decay energy", 1000, 0., 1000.);
  h_MuonDeltaR = new TH1F("h_MuonDeltaR", "Muon #DeltaR",2500, -10., 10.);
  h_MuonDeltaPhi = new TH1F("h_MuonDeltaPhi", "Muon #Delta#phi",2500, -3.5, 3.5);
  h_MuonInvMass = new TH1F("h_MuonInvMass", "Muon Invariant mass",1000, 0., 7.);
  h_Muon3DAngle = new TH1F("h_Muon3DAngle", "Muon 3D Angle",1000,-10., 10.);
		
  //Kaon from scalar decay
  h_genParScalarCKaonPt = new TH1F("h_genParScalarCKaonPt", "K^{#pm} from scalar decay p_{T}", 1000, 0., 1000.);
  h_genParScalarCKaonEta = new TH1F("h_genParScalarCKaonEta", "K^{#pm} from scalar decay #eta", 200, -7., 7.);
  h_genParScalarCKaonPhi = new TH1F("h_genParScalarCKaonPhi", "K^{#pm} from scalar decay #phi", 100, -3.5, 3.5);
  h_genParScalarCKaonE = new TH1F("h_genParScalarCKaonE", "K^{#pm} from scalar decay energy", 1000, 0., 1000.);
  h_KaonDeltaR = new TH1F("h_KaonDeltaR", "Kaon #DeltaR",2500, -10., 10.);
  h_KaonDeltaPhi = new TH1F("h_KaonDeltaPhi", "Kaon #Delta#phi",2500, -3.5, 3.5);
  h_KaonInvMass = new TH1F("h_KaonInvMass", "Kaon Invariant mass",1000, 0., 7.);
  h_Kaon3DAngle = new TH1F("h_Kaon3DAngle", "Kaon 3D Angle",1000,-10., 10.);
    
  //K short from scalar decay
  h_genParScalarKShortPt = new TH1F("h_genParScalarKShortPt", "K^{0}_S from scalar decay p_{T}", 1000, 0., 1000.);
  h_genParScalarKShortEta = new TH1F("h_genParScalarKShortEta", "K^{0}_S from scalar decay #eta", 200, -7., 7.);
  h_genParScalarKShortPhi = new TH1F("h_genParScalarKShortPhi", "K^{0}_S from scalar decay #phi", 100, -3.5, 3.5);
  h_genParScalarKShortE = new TH1F("h_genParScalarKShortE", "K^{0}_S from scalar decay energy", 1000, 0., 1000.);
  h_KShortDeltaR = new TH1F("h_KShortDeltaR", "K^{0}_S #DeltaR",2500, -10., 10.);
  h_KShortDeltaPhi = new TH1F("h_KShortDeltaPhi", "K^{0}_S #Delta#phi",2500, -3.5, 3.5);
  h_KShortInvMass = new TH1F("h_KShortInvMass", "K^{0}_S Invariant mass",1000, 0., 7.);
  h_KShort3DAngle = new TH1F("h_KShort3DAngle", "K^{0}_S 3D Angle",1000,-10., 10.);
    
  //Pion from scalar decay
  h_genParScalarCPionPt = new TH1F("h_genParScalarCPionPt", "#pi^{#pm} from scalar decay p_{T}", 1000, 0., 1000.);
  h_genParScalarCPionEta = new TH1F("h_genParScalarCPionEta", "#pi^{#pm} from scalar decay #eta", 200, -7., 7.);
  h_genParScalarCPionPhi = new TH1F("h_genParScalarCPionPhi", "#pi^{#pm} from scalar decay #phi", 100, -3.5, 3.5);
  h_genParScalarCPionE = new TH1F("h_genParScalarCPionE", "#pi^{#pm} from scalar decay energy", 1000, 0., 1000.);
  h_genParScalarNPionPt = new TH1F("h_genParScalarNPionPt", "#pi^{0} from scalar decay p_{T}", 1000, 0., 1000.);
  h_genParScalarNPionEta = new TH1F("h_genParScalarNPionEta", "#pi^{0} from scalar decay #eta", 200, -7., 7.);
  h_genParScalarNPionPhi = new TH1F("h_genParScalarNPionPhi", "#pi^{0} from scalar decay #phi", 100, -3.5, 3.5);
  h_genParScalarNPionE = new TH1F("h_genParScalarNPionE", "#pi^{0} from scalar decay energy", 1000, 0., 1000.);
  h_PionDeltaR = new TH1F("h_PionDeltaR", "Pion #DeltaR",2500, -10., 10.);
  h_PionDeltaPhi = new TH1F("h_PionDeltaPhi", "Pion #Delta#phi",2500, -3.5, 3.5);
  h_PionInvMass = new TH1F("h_PionInvMass", "Pion Invariant mass",1000, 0., 7.);
  h_Pion3DAngle =new TH1F("h_Pion3DAngle", "Pion 3D Angle",1000,-10., 10.);
  
   
  //Vertex position: muons, kaons, kshort, pions
  h_VertexPosXY = new TH2F("h_VertexPosXY", "Vertex Position XY", 100, -150,150,100,-150,150);
  h_VertexPosRZ = new TH2F("h_VertexPosRZ", "Vertex Position RZ", 100, 0,20,100,0,250);

  //PAT Muon reconstruction
  h_muonRecPt = new TH1F("h_muonRecPt", "#mu^{#pm} reconstruction p_{T}", 1000, 0., 200.);
  h_muonRecPtL = new TH1F("h_muonRecPtL", "#mu^{#pm} reconstruction p_{T}, leading", 1000, 0., 200.);
  h_muonRecPtS = new TH1F("h_muonRecPtS", "#mu^{#pm} reconstruction p_{T}, subleading", 1000, 0., 200.);
  h_muonRecEta = new TH1F("h_muonRecEta", "#mu^{#pm} reconstruction #eta", 200, -7., 7.);
  h_muonRecPhi = new TH1F("h_muonRecPhi", "#mu^{#pm} reconstruction #phi", 100, -3.5, 3.5);
  h_muonRecE = new TH1F("h_muonRecE", "#mu^{#pm} reconstruction energy", 1000, 0., 1000.);
  h_muonRecDeltaR = new TH1F("h_muonRecDeltaR", "Muon reconstruction #DeltaR",2500, -10., 10.);
  h_muonRecDeltaPhi = new TH1F("h_muonRecDeltaPhi", "Muon reconstruction #Delta#phi",2500, -3.5, 3.5);
  h_muonRecInvMass = new TH1F("h_muonRecInvMass", "Muon reconstruction invariant mass",500, 0, 5);
  Gaussian1 = new TF1("Gaussian1","gaus",1.,3.);
  h_muonCutSingleL = new TH1F("h_muonCutSingleL", "Single #mu^{#pm} trigger leading p_{T}", 1000, 0., 200.);
  h_muonCutDoubleL = new TH1F("h_muonCutDoubleL", "Double #mu^{#pm} trigger leading p_{T}", 1000, 0., 200.);
  h_muonDivSingleL = new TH1F("h_muonDivSingleL", "Turn-on Single #mu^{#pm} trigger leading p_{T}", 300, 0., 200.);
  h_muonDivDoubleL = new TH1F("h_muonDivDoubleL", "Turn-on Double #mu^{#pm} trigger leading p_{T}", 300, 0., 200.);
  h_muonCutDoubleS = new TH1F("h_muonCutDoubleS", "Double #mu^{#pm} trigger subleading p_{T}", 1000, 0., 200.);
  h_muonDivDoubleS = new TH1F("h_muonDivDoubleS", "Turn-on Double #mu^{#pm} trigger subleading p_{T}", 300, 0., 200.);
  h_PATmuonEta = new TH1F("h_PATmuonEta", "#mu^{#pm} reconstruction #eta", 200, -2.5, 2.5);
	
  //Packed candidates
  h_packedCDxy = new TH1F("h_packedCDxy", "Packed Candidate Dxy", 500, -200., 200.);
  h_packedCDz = new TH1F("h_packedCDz", "Packed Candidate Dz", 1500, -500., 500.);
  h_packedCVx = new TH1F("h_packedCVx", "Packed Candidate track vx", 500, -150., 150.);
  h_packedCVy = new TH1F("h_packedCVy", "Packed Candidate track vy", 500, -150., 150.);
  h_packedCVz = new TH1F("h_packedCVz", "Packed Candidate track vz", 1500, -500., 500.);
  h_displacedXY = new TH2I("h_displacedXY", "Displacement XY", 100, -150,150,100,-150,150);
  h_displacedRZ = new TH2I("h_displacedRZ", "Displacement RZ", 100, 0,20,100,0,250);
	
  h_HVertexPosXY = new TH2F("h_HVertexPosXY", "Pion track vertex XY", 500, -50,50,500,-50,50);
  h_HVertexPosRZ = new TH2F("h_HVertexPosRZ", "Pion track vertex RZ", 500, -50,50,500,-50,50);
	
  //Kaon mass assumption
  h_KhadronDeltaR = new TH1F("h_KhadronDeltaR", "Two hadrons (kaons) #DeltaR",2500, -10., 10.);
  h_KmuonsDeltaR = new TH1F("h_KmuonsDeltaR", "Two muons #DeltaR",2500, -10., 10.);
  h_KIsoSum1 = new TH1F("h_KIsoSum1", "0.3 p_{T} Cone construction kaon 1", 1000, 0., 5.);
  h_KIsoSum2 = new TH1F("h_KIsoSum2", "0.3 p_{T} Cone construction kaon 2", 1000, 0., 5.);
  h_KIso2 = new TH2F("h_KIso2", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.);
  h_MuonIsoSum1 = new TH1F("h_MuonIsoSum1", "0.3 p_{T} Cone construction muon 1", 1000, 0., 5.);
  h_MuonIsoSum2 = new TH1F("h_MuonIsoSum2", "0.3 p_{T} Cone construction muon 2", 1000, 0., 5.);
  h_KhadronInvMass = new TH1F("h_KhadronInvMass", "Two hadrons (kaons) - Invariant mass",200, 0., 7.);
  h_KhadronInvMass2 = new TH1F("h_KhadronInvMass2", "Two hadrons (kaons) - Invariant mass, smaller binning",100, 0., 7.);
  h_KmuonsInvMass = new TH1F("h_KmuonsInvMass", "Two muons - Invariant mass",1000, 0., 7.);
  h_Kinvmass = new TH2F("h_Kinvmass", "Invariant mass: charged hadrons (kaons) vs muons", 1000, 0.,7.,1000,0.,7.);
  
  h_KantiscalarInvMass = new TH1F("h_KantiscalarInvMass", "(Kaon) Antiscalar Invariant mass", 200, 0.,5.);
  h_KscalarInvMass = new TH1F("h_KscalarInvMass", "Scalar Invariant mass", 200, 0.,5.);
  h_KhiggsInvMass = new TH1F("h_KhiggsInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_K20higgsInvMass = new TH1F("h_K20higgsInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_KhiggsRInvMass = new TH1F("h_KhiggsRInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_KhiggsR20InvMass = new TH1F("h_KhiggsR20InvMass", "Higgs invariant mass", 500, 0., 200.);
  h_KhiggsDeltaR = new TH1F("h_KhiggsDeltaR", "Scalar-Antiscalar #DeltaR",2500, 0., 15.);
	
  h_kNentries = new TH1F("h_kNentries", "Number of kaon events", 4, 0.,4.);
  h_pNentries = new TH1F("h_pNentries", "Number of pion events", 4, 0.,4.);	
  h_mNentries = new TH1F("h_mNentries", "Number of muon events", 4, 0.,4.);
	
  //Pion mass assumption
  h_PhadronDeltaR = new TH1F("h_PhadronDeltaR", "Two hadrons (pions) #DeltaR",2500, -10., 10.);
  h_PmuonsDeltaR = new TH1F("h_PmuonsDeltaR", "Two muons #DeltaR",2500, -10., 10.);
  h_PIsoSum1 = new TH1F("h_PIsoSum1", "Leading pion relative isolation", 1000, 0., 5.);
  h_PIsoSum2 = new TH1F("h_PIsoSum2", "Subleading pion relative isolation", 1000, 0., 5.);
  h_PIso2 = new TH2F("h_PIso2", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.);
  h_PIsoSum3 = new TH1F("h_PIsoSum3", "Leading muon relative isolation", 1000, 0., 50.);
  h_PIsoSum4 = new TH1F("h_PIsoSum4", "Subleading muon relative isolation", 1000, 0., 50.);
  h_PIso4 = new TH2F("h_PIso4", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.);
  h_PhadronInvMass = new TH1F("h_PhadronInvMass", "Dihadron (pion) invariant mass",1000, 0., 7.);
  h_PhadronInvMass2 = new TH1F("h_PhadronInvMass2", "Two hadrons (pions) - Invariant mass, smaller binning",500, 0., 7.);
  h_PmuonsInvMass = new TH1F("h_PmuonsInvMass", "Dimuon invariant mass",1000, 0., 7.);
  Gaussian2 = new TF1("Gaussian2","gaus",1.,3.);
  h_Pinvmass = new TH2F("h_Pinvmass", "Invariant mass: charged hadrons (pions) vs muons", 1000, 0.,7.,1000,0.,7.);
  
  h_PantiscalarInvMass = new TH1F("h_PantiscalarInvMass", "Dihadron (pion) invariant mass", 200, 0.,5.);
  h_PscalarInvMass = new TH1F("h_PscalarInvMass", "Dimuon invariant mass", 200, 0.,5.);
  h_TestPantiscalarInvMass = new TH1F("h_TestPantiscalarInvMass", "Dihadron (pion) invariant mass", 200, 0.,5.);
  h_TestKantiscalarInvMass = new TH1F("h_TestKantiscalarInvMass", "Dihadron (kaon) invariant mass", 200, 0.,5.);
  h_TestscalarInvMass = new TH1F("h_TestscalarInvMass", "Dimuon invariant mass", 200, 0.,5.);
  h_TestPhiggsInvMass = new TH1F("h_TestPhiggsInvMass", "Higgs invariant mass (pion)", 500, 0.,200.);
  h_TestKhiggsInvMass = new TH1F("h_TestKhiggsInvMass", "Higgs invariant mass (kaon)", 500, 0.,200.);
  Gaussian3 = new TF1("Gaussian3","gaus",1.,3.);
  h_PhiggsInvMass = new TH1F("h_PhiggsInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_PhiggsRInvMass = new TH1F("h_PhiggsRInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_PhiggsR20InvMass = new TH1F("h_PhiggsR20InvMass", "Higgs invariant mass", 500, 0., 200.);
  h_PhiggsDeltaR = new TH1F("h_PhiggsDeltaR", "Discalar #DeltaR",2500, 0., 15.);
	
  h_Rrefit12InvMass = new TH1F("h_Rrefit12InvMass", "Dimuon refitted invariant mass with requirements", 200, 0.,5.);
  h_Rpionre12InvMass = new TH1F("h_Rpionre12InvMass", "Dihadron (pion) refit invariant mass", 200, 0.,5.);
  h_Rkaonre12InvMass = new TH1F("h_Rkaonre12InvMass", "Dihadron (kaon) refit invariant mass", 200, 0.,5.);

  h_widemuonRInvMass = new TH1F("h_widemuonRInvMass", "Dimuon refitted invariant mass with requirements", 200, 0.,5.);
  h_widekaonRInvMass = new TH1F("h_widekaonRInvMass", "Dihadron (kaon) refitted invariant mass with requirements", 200, 0.,5.);
  h_widepionRInvMass = new TH1F("h_widepionRInvMass", "Dihadron (pion) refitted invariant mass with requirements", 200, 0.,5.);
	
  h_P20antiscalarInvMass = new TH1F("h_P20antiscalarInvMass", "Dihadron (pion) invariant mass", 200, 0.,5.);
  h_P20scalarInvMass = new TH1F("h_P20scalarInvMass", "Dimuon invariant mass", 200, 0.,5.);
  h_P20higgsInvMass = new TH1F("h_P20higgsInvMass", "Higgs invariant mass", 500, 0., 200.);
  h_K20antiscalarInvMass = new TH1F("h_K20antiscalarInvMass", "Dihadron (kaon) invariant mass", 500, 0.,5.);	
	
  h_massassump = new TH2F("h_massassump", "Invariant mass: charged hadrons (pions) vs charged hadrons (kaons)", 500, 0.,7.,1000,0.,7.);
  h_pmassassump = new TH2F("h_pmassassump", "Invariant mass: charged hadrons (pions) vs charged hadrons (kaons)", 500, 0.,7.,1000,0.,7.);
  h_higgsassump = new TH2F("h_higgsassump", "Higgs invariant mass: pion vs. kaon assumption", 500, 0.,200.,1000,0.,200.);
	
	
  //Comparison muon momenta
	
  //PAT muons
  h_muoniRecPtTrk = new TH1F("h_muoniRecPtTrk", "#mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.);
  h_muon1RecPtTrk = new TH1F("h_muon1RecPtTrk", "Leading #mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.);
  h_muon2RecPtTrk = new TH1F("h_muon2RecPtTrk", "Subleading #mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.);
  h_muoniRecPt = new TH1F("h_muoniRecPt", "#mu^{#pm} reconstruction p_{T}", 1000, 0., 200.);
  h_muon1RecPt = new TH1F("h_muon1RecPt", "Leading #mu^{#pm} reconstruction p_{T}", 1000, 0., 200.);
  h_muon2RecPt = new TH1F("h_muon2RecPt", "Subleading #mu^{#pm} reconstruction p_{T}", 1000, 0., 200.);
  h_muon1RecInvMass = new TH1F("h_muon1RecInvMass", "Leading reconstruction invariant mass",500, 0, 100);
  h_muon2RecInvMass = new TH1F("h_muon2RecInvMass", "Subleading reconstruction invariant mass",500, 0, 100);
  h_muon12RecInvMass = new TH1F("h_muon12RecInvMass", "Scalar reconstruction invariant mass",500, 0, 100);
	
  //Packed muons
  h_muonipackedPt = new TH1F("h_muonipackedPt", "#mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.);
  h_muonipackedInvMass = new TH1F("h_muonipackedInvMass", "#mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.);
  h_muonipackedPtTrk = new TH1F("h_muonipackedPtTrk", "#mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.);
  h_muon1packedPt = new TH1F("h_muon1packedPt", "Leading #mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.);
  h_muon1packedInvMass = new TH1F("h_muon1packedInvMass", "Leading #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.);
  h_muon1packedPtTrk = new TH1F("h_muon1packedPtTrk", "Leading #mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.);
  h_muon2packedPt = new TH1F("h_muon2packedPt", "Subleading #mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.);
  h_muon2packedInvMass = new TH1F("h_muon2packedInvMass", "Subleading #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.);
  h_muon2packedPtTrk = new TH1F("h_muon2packedPtTrk", "Subleading #mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.);
  h_muon12packedInvMass = new TH1F("h_muon12packedInvMass", "Scalar #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.);
	
  //Refitted tracks muons
  h_muon1PairsPt = new TH1F("h_muon1PairsPt", "#mu^{#pm} 1 Refitted tracks p_{T} m", 1000, 0., 1000.);
  h_muon2PairsPt = new TH1F("h_muon2PairsPt", "#mu^{#pm} 2 Refitted tracks p_{T}", 1000, 0., 1000.);
  h_muon1refitInvMass = new TH1F("h_muon1refitInvMass", "Leading #mu^{#pm} Refit Invariant mass", 500, 0.,5.);
  h_muon2refitInvMass = new TH1F("h_muon2refitInvMass", "Subleading #mu^{#pm} Refit Invariant mass", 500, 0.,5.);
  h_muon12refitInvMass = new TH1F("h_muon12refitInvMass", "Scalar Refit Invariant mass", 500, 0.,5.);
  h_refit1InvMass = new TH1F("h_refit1InvMass", "Leading #mu^{#pm} Refit Invariant mass", 500, 0.,5.);
  h_refit2InvMass = new TH1F("h_refit2InvMass", "Subleading #mu^{#pm} Refit Invariant mass", 500, 0.,5.);
  h_refit12InvMass = new TH1F("h_refit12InvMass", "Dimuon refitted invariant mass", 500, 0.,5.);
  
  //Refitted tracks pions
  h_pionre12InvMass = new TH1F("h_pionre12InvMass", "Dihadron (pion) refit invariant mass", 500, 0.,5.);
  h_muonPairsXY = new TH2F("h_muonPairsXY", "Refitted tracks vertex XY", 100, -150,150,100,-150,150);
  h_muonPairsRZ = new TH2F("h_muonPairsRZ", "Refitted tracks vertex RZ", 100, 0,20,100,0,250);

  std::cout << "END PLOTS SETUP" << std::endl;
}
    
void SimpleAnalysis::savePlots() {

  std::cout << "START SAVING PLOTS" << std::endl;

  std::cout << "Open output file: " << outFileString.c_str() << std::endl;

  // Create output ROOT file
  TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
  // change current ROOT directory to that of outFile
  outFile->cd();

  std::cout << "WRITING PLOTS TO OUTPUT FILE" << std::endl;

  // Write histograms to file
  h_genParPt->GetXaxis()->SetTitle("GeV");
  h_genParPt->Write();
  h_genParEta->Write();
  h_genParPhi->Write();
  h_genParE->Write();
  h_VertexPosR->Write();
	
  h_genParHiggsPt->GetXaxis()->SetTitle("p_{T} (GeV)");
  h_genParHiggsPt->GetYaxis()->SetTitle("Events");
  h_genParHiggsPt->Write();
  h_genParHiggsEta->Write();
  h_genParHiggsPhi->Write();
  h_genParHiggsE->GetXaxis()->SetTitle("GeV");
  h_genParHiggsE->Write();
  h_HiggsInvMass->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^2)");
  h_HiggsInvMass->GetYaxis()->SetTitle("Events");
  h_HiggsInvMass->Write();

  h_genParScalarPt->GetXaxis()->SetTitle("p_{T} (GeV)");
  h_genParScalarPt->GetYaxis()->SetTitle("Events");
  h_genParScalarPt->Write();
  h_genParScalarEta->Write();
  h_genParScalarPhi->Write();
  h_genParScalarE->GetXaxis()->SetTitle("GeV");
  h_genParScalarE->Write();
  h_ScalarDeltaR->GetXaxis()->SetTitle("Radians");
  h_ScalarDeltaR->GetYaxis()->SetTitle("Events");
  h_ScalarDeltaR->Write();
  h_ScalarDeltaPhi->Write();
  h_ScalarInvMass->GetXaxis()->SetTitle("m_{scalar} (GeV/c^{2})");
  h_ScalarInvMass->GetYaxis()->SetTitle("Events");
  h_ScalarInvMass->Write();
  h_Scalar3DAngle->Write();

  h_genParScalarMuonDivPtSL=(TH1F*)h_genParScalarMuonCutPtSL->Clone();
  h_genParScalarMuonDivPtSL->Divide(h_genParScalarMuonPtL);
  //h_genParScalarMuonDivPtSL->SetTitle("Turn-on Single trigger leading");
    
  h_genParScalarMuonDivPtDL=(TH1F*)h_genParScalarMuonCutPtDL->Clone();
  h_genParScalarMuonDivPtDL->Divide(h_genParScalarMuonPtL);
  //h_genParScalarMuonDivPtDL->SetTitle("Turn-on Double trigger leading");
   
  h_genParScalarMuonDivPtDS=(TH1F*)h_genParScalarMuonCutPtDS->Clone();
  h_genParScalarMuonDivPtDS->Divide(h_genParScalarMuonPtS);
  //h_genParScalarMuonDivPtDS->SetTitle("Turn-on Double trigger subleading");

  h_genParScalarMuonPtL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_genParScalarMuonPtL->GetYaxis()->SetTitle("Events");
  h_genParScalarMuonPtL->Write();
  h_genParScalarMuonPtS->GetXaxis()->SetTitle("Subleading muon p_{T} (GeV)");
  h_genParScalarMuonPtS->GetYaxis()->SetTitle("Events");
  h_genParScalarMuonPtS->Write();
  h_genParScalarMuonCutPtSL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_genParScalarMuonCutPtSL->GetYaxis()->SetTitle("Events");
  h_genParScalarMuonCutPtSL->Write();
  h_genParScalarMuonDivPtSL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_genParScalarMuonDivPtSL->GetYaxis()->SetTitle("Trigger efficiency");
  h_genParScalarMuonDivPtSL->Write();
  h_genParScalarMuonCutPtDL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_genParScalarMuonCutPtDL->Write();
  h_genParScalarMuonDivPtDL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_genParScalarMuonDivPtDL->GetYaxis()->SetTitle("Trigger efficiency");
  h_genParScalarMuonDivPtDL->Write();
  h_genParScalarMuonCutPtDS->GetXaxis()->SetTitle("Subleading muon p_{T} (GeV)");
  h_genParScalarMuonCutPtDS->Write();
  h_genParScalarMuonDivPtDS->GetXaxis()->SetTitle("Subleading muon p_{T} (GeV)");
  h_genParScalarMuonDivPtDS->GetYaxis()->SetTitle("Trigger efficiency");
  h_genParScalarMuonDivPtDS->Write();
 
  h_genParScalarMuonEta->Write();
  h_genParScalarMuonPhi->Write();
  h_genParScalarMuonE->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonE->Write();
  h_MuonDeltaR->GetXaxis()->SetTitle("Radians");
  h_MuonDeltaR->GetYaxis()->SetTitle("Events");
  h_MuonDeltaR->Write();
  h_MuonDeltaPhi->Write();
  h_MuonInvMass->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
  h_MuonInvMass->GetYaxis()->SetTitle("Events");
  h_MuonInvMass->Write();
  h_Muon3DAngle->Write();
 
  h_genParScalarCKaonPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarCKaonPt->Write();
  h_genParScalarCKaonEta->Write();
  h_genParScalarCKaonPhi->Write();
  h_genParScalarCKaonE->GetXaxis()->SetTitle("GeV");
  h_genParScalarCKaonE->Write();
  h_KaonDeltaR->GetXaxis()->SetTitle("Radians");
  h_KaonDeltaR->GetYaxis()->SetTitle("Events");
  h_KaonDeltaR->Write();
  h_KaonDeltaPhi->Write();
  h_KaonInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_KaonInvMass->GetYaxis()->SetTitle("Events");
  h_KaonInvMass->Write();
  h_Kaon3DAngle->Write();

  h_genParScalarKShortPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarKShortPt->Write();
  h_genParScalarKShortEta->Write();
  h_genParScalarKShortPhi->Write();
  h_genParScalarKShortE->GetXaxis()->SetTitle("GeV");
  h_genParScalarKShortE->Write();
  h_KShortDeltaR->GetXaxis()->SetTitle("Radians");
  h_KShortDeltaR->Write();
  h_KShortDeltaPhi->Write();
  h_KShortInvMass->GetXaxis()->SetTitle("GeV");
  h_KShortInvMass->Write();
  h_KShort3DAngle->Write();
 
  h_genParScalarCPionPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarCPionPt->Write();
  h_genParScalarCPionEta->Write();
  h_genParScalarCPionPhi->Write();
  h_genParScalarCPionE->GetXaxis()->SetTitle("GeV");
  h_genParScalarCPionE->Write();
  h_genParScalarNPionPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarNPionPt->Write();
  h_genParScalarNPionEta->Write();
  h_genParScalarNPionPhi->Write();
  h_genParScalarNPionE->GetXaxis()->SetTitle("GeV");
  h_genParScalarNPionE->Write();
  h_PionDeltaR->GetXaxis()->SetTitle("Radians");
  h_PionDeltaR->GetYaxis()->SetTitle("Events");
  h_PionDeltaR->Write();
  h_PionDeltaPhi->Write();
  h_PionInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_PionInvMass->GetYaxis()->SetTitle("Events");
  h_PionInvMass->Write();
  h_Pion3DAngle->Write();

  h_VertexPosXY->GetXaxis()->SetTitle("Vertex position x (mm)"); // set a title for the x-axis
  h_VertexPosXY->GetXaxis()->SetRangeUser(-150., 150.);
  h_VertexPosXY->GetYaxis()->SetTitle("Vertex position y (mm)"); // set a title for the y-axis
  h_VertexPosXY->GetYaxis()->SetRangeUser(-150., 150.);
  h_VertexPosXY->Write();
  h_VertexPosRZ->GetXaxis()->SetTitle("Vertex position z (mm)");
  h_VertexPosRZ->GetYaxis()->SetTitle("R");
  h_VertexPosRZ->Write();
    

	
  //Reconstruction
  h_muonRecPt->GetXaxis()->SetTitle("p_{T} (GeV)");
  h_muonRecPt->GetYaxis()->SetTitle("Events");
  h_muonRecPt->Write();
  h_muonRecPtL->GetXaxis()->SetTitle("Leading p_{T} (GeV)");
  h_muonRecPtL->GetYaxis()->SetTitle("Events");
  h_muonRecPtL->Write();
  h_muonRecPtS->GetXaxis()->SetTitle("Subleading p_{T} (GeV)");
  h_muonRecPtS->GetYaxis()->SetTitle("Events");
  h_muonRecPtS->Write();
  h_muonRecEta->Write();
  h_muonRecPhi->Write();
  h_muonRecE->GetXaxis()->SetTitle("GeV");
  h_muonRecE->Write();
  h_muonRecDeltaR->GetXaxis()->SetTitle("Radians");
  h_muonRecDeltaR->GetYaxis()->SetTitle("Events");
  h_muonRecDeltaR->Write();
  h_muonRecDeltaPhi->Write();
  h_muonRecInvMass->GetXaxis()->SetTitle("m_{dimuon} (GeV/c^{2})");
  h_muonRecInvMass->GetYaxis()->SetTitle("Events");
  h_muonRecInvMass->Write();
  h_muonCutSingleL->GetXaxis()->SetTitle("GeV");
  h_muonCutSingleL->Write();

  h_muonDivSingleL=(TH1F*)h_muonCutSingleL->Clone();
  h_muonDivSingleL->Divide(h_muonRecPtL);
 // h_muonDivSingleL->SetTitle("Turn-on Single trigger, leading");
          
  h_muonDivDoubleL=(TH1F*)h_muonCutDoubleL->Clone();
  h_muonDivDoubleL->Divide(h_muonRecPtL);
//  h_muonDivDoubleL->SetTitle("Turn-on Double trigger, leading");
    
  h_muonDivDoubleS=(TH1F*)h_muonCutDoubleS->Clone();
  h_muonDivDoubleS->Divide(h_muonRecPtS);
//  h_muonDivDoubleS->SetTitle("Turn-on Double trigger, subleading");

  h_muonDivSingleL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_muonDivSingleL->GetYaxis()->SetTitle("Trigger efficiency");
  h_muonDivSingleL->Write();
  h_muonCutDoubleL->GetXaxis()->SetTitle("GeV");
  h_muonCutDoubleL->Write();
  h_muonDivDoubleL->GetXaxis()->SetTitle("Leading muon p_{T} (GeV)");
  h_muonDivDoubleL->GetYaxis()->SetTitle("Trigger efficiency");
  h_muonDivDoubleL->Write();
  h_muonCutDoubleS->GetXaxis()->SetTitle("GeV");
  h_muonCutDoubleS->Write();
  h_muonDivDoubleS->GetXaxis()->SetTitle("Subleading muon p_{T} (GeV)");
  h_muonDivDoubleS->GetYaxis()->SetTitle("Trigger efficiency");
  h_muonDivDoubleS->Write();

  h_PATmuonEta->Write();
	
  /*
  //Packed Candidates
  h_packedCDxy->Write();
  h_packedCDz->Write();
  h_packedCVx->Write();
  h_packedCVy->Write();
  h_packedCVz->Write();
  h_displacedXY->GetXaxis()->SetTitle("Vertex position x");
  h_displacedXY->GetYaxis()->SetTitle("Vertex position y");
  h_displacedXY->Write();
  h_displacedRZ->GetXaxis()->SetTitle("Vertex position z");
  h_displacedRZ->GetYaxis()->SetTitle("R");
  h_displacedRZ->Write();
 
  h_HVertexPosXY->GetXaxis()->SetTitle("V_{x} (cm)");
  h_HVertexPosXY->GetYaxis()->SetTitle("V_{y} (cm)");
  h_HVertexPosXY->Write();
  h_HVertexPosRZ->GetXaxis()->SetTitle("V_{z} (cm)");
  h_HVertexPosRZ->GetYaxis()->SetTitle("R (cm)");
  h_HVertexPosRZ->Write();
  */


  //Kaon mass assumption
  h_KhadronDeltaR->GetXaxis()->SetTitle("Radians");
  h_KhadronDeltaR->GetYaxis()->SetTitle("Events");
  h_KhadronDeltaR->Write();
  h_KmuonsDeltaR->GetXaxis()->SetTitle("Radians");
  h_KmuonsDeltaR->Write();
  h_KIsoSum1->Write();
  h_KIsoSum2->Write();
  h_KIso2->GetXaxis()->SetTitle("Relative isolation sum");
  h_KIso2->GetYaxis()->SetTitle("Particle momentum");
  h_KIso2->Write();
  h_MuonIsoSum1->Write();
  h_MuonIsoSum2->Write();
  h_KmuonsInvMass->GetXaxis()->SetTitle("GeV");
  h_KmuonsInvMass->Write();
  h_KhadronInvMass->GetXaxis()->SetTitle("GeV");
  h_KhadronInvMass->Write();
  h_KhadronInvMass2->GetXaxis()->SetTitle("GeV");
  h_KhadronInvMass2->Write();
  h_Kinvmass->GetXaxis()->SetTitle("Hadron (kaon) invariant mass (GeV)");
  h_Kinvmass->GetYaxis()->SetTitle("Muon invariant mass (GeV)");
  h_Kinvmass->Write();


  h_KantiscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_KantiscalarInvMass->Write();
  h_TestKantiscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_TestKantiscalarInvMass->Write();
  h_TestKhiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_TestKhiggsInvMass->Write();
  h_KscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_KscalarInvMass->Write();
  h_KhiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_KhiggsInvMass->GetYaxis()->SetTitle("Events");
  h_KhiggsInvMass->Write();
  h_K20higgsInvMass->Write();
  h_KhiggsDeltaR->GetXaxis()->SetTitle("Radians");
  h_KhiggsDeltaR->Write();
  h_KhiggsRInvMass->GetXaxis()->SetTitle("GeV");
  h_KhiggsRInvMass->GetYaxis()->SetTitle("Events");
  h_KhiggsRInvMass->Write();
  h_KhiggsR20InvMass->GetXaxis()->SetTitle("GeV");
  h_KhiggsR20InvMass->GetYaxis()->SetTitle("Events");
  h_KhiggsR20InvMass->Write();
  
  h_kNentries->GetXaxis()->SetBinLabel(1,"Number of dihadrons"); h_kNentries->GetXaxis()->SetBinLabel(2,"Relative isolation"); h_kNentries->GetXaxis()->SetBinLabel(3,"Higgs mass window: #pm 3 GeV"); h_kNentries->GetXaxis()->SetBinLabel(4,"Scalar mass window: #pm 150 MeV");
  h_kNentries->Write();	
	
  //Pion mass assumption
  h_PhadronDeltaR->GetXaxis()->SetTitle("Radians");
  h_PhadronDeltaR->Write();
  h_PmuonsDeltaR->GetXaxis()->SetTitle("Radians");
  h_PmuonsDeltaR->Write();
  //h_PIsoSum1->SetTitle("Leading pion relative isolation");
  h_PIsoSum1->GetYaxis()->SetTitle("Events");
  h_PIsoSum1->GetXaxis()->SetTitle("Rel. iso");
  h_PIsoSum1->Write();

  //h_PIsoSum2->SetTitle("Subleading pion relative isolation");
  h_PIsoSum2->GetYaxis()->SetTitle("Events");
  h_PIsoSum2->GetXaxis()->SetTitle("Rel. iso");
  h_PIsoSum2->Write();
  h_PIso2->GetXaxis()->SetTitle("Relative isolation sum");
  h_PIso2->GetYaxis()->SetTitle("Particle momentum");
  h_PIso2->Write();
  //h_PIsoSum3->SetTitle("Leading muon relative isolation");
 
  h_PIsoSum3->GetXaxis()->SetTitle("Rel. iso");
  h_PIsoSum3->GetYaxis()->SetTitle("Events");
  h_PIsoSum3->Write();
  //h_PIsoSum4->SetTitle("Subleading muon relative isolation");
  h_PIsoSum4->GetXaxis()->SetTitle("Rel. iso");
  h_PIsoSum4->GetYaxis()->SetTitle("Events");
  h_PIsoSum4->Write();
  h_PIso4->GetXaxis()->SetTitle("Relative isolation sum");
  h_PIso4->GetYaxis()->SetTitle("Particle momentum");
  h_PIso4->Write();
  h_PmuonsInvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_PmuonsInvMass->GetYaxis()->SetTitle("Events");
  h_PmuonsInvMass->Write();
  h_PhadronInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_PhadronInvMass->GetYaxis()->SetTitle("Events");
  h_PhadronInvMass->Write();
  h_PhadronInvMass2->GetXaxis()->SetTitle("GeV");
  h_PhadronInvMass2->Write();
  h_Pinvmass->GetXaxis()->SetTitle("Hadron (pion) invariant mass (GeV)");
  h_Pinvmass->GetYaxis()->SetTitle("Muon invariant mass (GeV)");
  h_Pinvmass->Write();

  h_pNentries->GetXaxis()->SetBinLabel(1,"Number of dihadrons"); h_pNentries->GetXaxis()->SetBinLabel(2,"Relative isolation"); h_pNentries->GetXaxis()->SetBinLabel(3,"Higgs mass window: #pm 3 GeV"); h_pNentries->GetXaxis()->SetBinLabel(4,"Scalar mass window: #pm 150 MeV");
  h_pNentries->Write();	
  
  h_TestPantiscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_TestPantiscalarInvMass->Write();
  h_TestPhiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_TestPhiggsInvMass->Write();
  h_PantiscalarInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_PantiscalarInvMass->GetYaxis()->SetTitle("Events");
  h_PantiscalarInvMass->Write();
  h_TestscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_TestscalarInvMass->Write();
  h_PscalarInvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_PscalarInvMass->GetYaxis()->SetTitle("Events");
  h_PscalarInvMass->Write();

  h_mNentries->GetXaxis()->SetBinLabel(1,"Number of dimuons"); h_mNentries->GetXaxis()->SetBinLabel(2,"Relative isolation"); h_mNentries->GetXaxis()->SetBinLabel(3,"Higgs mass window: #pm 3 GeV"); h_mNentries->GetXaxis()->SetBinLabel(4,"Scalar mass window: #pm 150 MeV");
  h_mNentries->Write();		
	
  h_PhiggsInvMass->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
  h_PhiggsInvMass->GetYaxis()->SetTitle("Events");
  h_PhiggsInvMass->Write();
  h_PhiggsRInvMass->GetYaxis()->SetTitle("Events");
  h_PhiggsRInvMass->Write();
  h_PhiggsR20InvMass->GetYaxis()->SetTitle("Events");
  h_PhiggsR20InvMass->Write();
  h_PhiggsDeltaR->GetYaxis()->SetTitle("Events");
  h_PhiggsDeltaR->GetXaxis()->SetTitle("Radians");
  h_PhiggsDeltaR->Write();
  h_Rrefit12InvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_Rrefit12InvMass->GetYaxis()->SetTitle("Events");
  h_Rrefit12InvMass->Write();
  h_Rpionre12InvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_Rpionre12InvMass->GetYaxis()->SetTitle("Events");
  h_Rpionre12InvMass->Write();
  h_Rkaonre12InvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_Rkaonre12InvMass->GetYaxis()->SetTitle("Events");
  h_Rkaonre12InvMass->Write();

  h_widemuonRInvMass->Write();
  h_widekaonRInvMass->Write();
  h_widepionRInvMass->Write();
	  
  h_P20antiscalarInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_P20antiscalarInvMass->GetYaxis()->SetTitle("Events");
  h_P20antiscalarInvMass->Write();
  h_P20scalarInvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_P20scalarInvMass->GetYaxis()->SetTitle("Events");
  h_P20scalarInvMass->Write();
  h_K20antiscalarInvMass->Write();
  h_P20higgsInvMass->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
  h_P20higgsInvMass->GetYaxis()->SetTitle("Events");
  h_P20higgsInvMass->Write();

  h_massassump->GetXaxis()->SetTitle("Hadron (pion) invariant mass (GeV)");
  h_massassump->GetYaxis()->SetTitle("Hadron (kaon) invariant mass (GeV)");
  h_massassump->Write();
  h_pmassassump->GetXaxis()->SetTitle("Hadron (pion) invariant mass (GeV)");
  h_pmassassump->GetYaxis()->SetTitle("Hadron (kaon) invariant mass (GeV)");
  h_pmassassump->Write();
  h_higgsassump->GetXaxis()->SetTitle("Higgs (pion-muon) invariant mass (GeV)");
  h_higgsassump->GetYaxis()->SetTitle("Higgs (kaon-muon) invariant mass (GeV)");
  h_higgsassump->Write();


  //Muon comparison
	
  //PAT muons
  h_muoniRecPt->GetXaxis()->SetTitle("GeV");
  h_muoniRecPt->Write();
  h_muon1RecPt->GetXaxis()->SetTitle("GeV");
  h_muon1RecPt->Write();
  h_muon2RecPt->GetXaxis()->SetTitle("GeV");
  h_muon2RecPt->Write();
  h_muoniRecPtTrk->GetXaxis()->SetTitle("GeV");
  h_muoniRecPtTrk->Write();
  h_muon1RecPtTrk->GetXaxis()->SetTitle("GeV");
  h_muon1RecPtTrk->Write();
  h_muon2RecPtTrk->GetXaxis()->SetTitle("GeV");
  h_muon2RecPtTrk->Write();
  h_muon1RecInvMass->GetXaxis()->SetTitle("GeV");
  h_muon1RecInvMass->Write();
  h_muon2RecInvMass->GetXaxis()->SetTitle("GeV");
  h_muon2RecInvMass->Write();
  h_muon12RecInvMass->GetXaxis()->SetTitle("GeV");
  h_muon12RecInvMass->Write();
	
  //Packed muons
  h_muonipackedPt->GetXaxis()->SetTitle("GeV");
  h_muonipackedPt->Write();
  h_muonipackedInvMass->GetXaxis()->SetTitle("GeV");
  h_muonipackedInvMass->Write();
  h_muonipackedPtTrk->GetXaxis()->SetTitle("GeV");
  h_muonipackedPtTrk->Write();
  h_muon1packedPt->GetXaxis()->SetTitle("GeV");
  h_muon1packedPt->Write();
  h_muon1packedInvMass->GetXaxis()->SetTitle("GeV");
  h_muon1packedInvMass->Write();
  h_muon1packedPtTrk->GetXaxis()->SetTitle("GeV");
  h_muon1packedPtTrk->Write();
  h_muon2packedPt->GetXaxis()->SetTitle("GeV");
  h_muon2packedPt->Write();
  h_muon2packedInvMass->GetXaxis()->SetTitle("GeV");
  h_muon2packedInvMass->Write();
  h_muon2packedPtTrk->GetXaxis()->SetTitle("GeV");
  h_muon2packedPtTrk->Write();
  h_muon12packedInvMass->GetXaxis()->SetTitle("GeV");
  h_muon12packedInvMass->Write();
	
  //Refitted tracks muons
  h_muon1PairsPt->GetXaxis()->SetTitle("GeV");
  h_muon1PairsPt->Write();
  h_muon2PairsPt->GetXaxis()->SetTitle("GeV");
  h_muon2PairsPt->Write();
  h_muon1refitInvMass->GetXaxis()->SetTitle("GeV");
  h_muon1refitInvMass->Write();
  h_muon2refitInvMass->GetXaxis()->SetTitle("GeV");
  h_muon2refitInvMass->Write();
  h_muon12refitInvMass->GetXaxis()->SetTitle("GeV");
  h_muon12refitInvMass->Write();
  h_refit1InvMass->GetXaxis()->SetTitle("GeV");
  h_refit1InvMass->Write();
  h_refit2InvMass->GetXaxis()->SetTitle("GeV");
  h_refit2InvMass->Write();
  h_refit12InvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_refit12InvMass->GetYaxis()->SetTitle("Events");
  h_refit12InvMass->Write();

  //Refitted tracks pions
  h_pionre12InvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_pionre12InvMass->GetYaxis()->SetTitle("Events");
  h_pionre12InvMass->Write();
	
  h_muonPairsXY->SetTitle("Vertex position x");
  h_muonPairsXY->SetTitle("Vertex position y");
  h_muonPairsXY->Write();
  h_muonPairsRZ->SetTitle("Vertex position z");
  h_muonPairsRZ->SetTitle("Vertex position R");
  h_muonPairsRZ->Write();
	
  std::cout << "FINISHED WRITING PLOTS TO OUTPUT FILE" << std::endl;
	
  // Safely close file
  outFile->Close();

  std::cout << "CLOSE OUTPUT FILE" << std::endl;

}


// Function to convert pdgId numerical code into a Latex or unicode string! 
std::string SimpleAnalysis::pdgIdCode (const Int_t parId, const bool unicode) const {

  std::string particle;
  int id = std::abs(parId);

  switch (id) {
  case 0 : particle += "-"; break;
  case 1 : particle += "d"; break;
  case 2 : particle += "u"; break;
  case 3 : particle += "s"; break;
  case 4 : particle += "c"; break;
  case 5 : particle += "b"; break;
  case 6 : particle += "t"; break;
  case 9 : particle += "g"; break;

  case 11 : particle += "e"; break;
  case 12 : particle += unicode ? "\u03BD_e" : "#nu_{e}" ; break;
  case 13 : particle += unicode ? "\u03BC" : "#mu"; break;
  case 14 : particle += unicode ? "\u03BD_\u03BC" : "#nu_{#mu}"; break;
  case 15 : particle += unicode ? "\u03C4" : "#tau"; break;
  case 16 : particle += unicode ? "\u03BD_\u03C4" : "#nu_{#tau}"; break;

  case 21 : particle += "g"; break;
  case 22 : particle += unicode ? "Y" : "#gamma"; break;
  case 23 : particle += "Z_{0}"; break;
  case 24 : particle += "W"; break;
  case 25 : particle += "h_{0}"; break;

  case 111 : particle += unicode ? "\u03C00" : "#pi^{0}"; break;
  case 211 : particle += unicode ? "\u03C0" : "#pi^{+}"; break;
  case 113 : particle += unicode ? "\u03C1" : "#rho(770)^{0}"; break;
  case 213 : particle += unicode ? "\u03C1" : "#rho(770)^{+}"; break;
  case 225 : particle += unicode ? "f2(1270)" : "f_{2}(1270)"; break;

  case 130 : particle += unicode ? "K0_L" : "K_{L}^{0}"; break;
  case 310 : particle += unicode ? "K0_S" : "K_{S}^{0}"; break;
  case 311 : particle += unicode ? "K0" : "K^{0}"; break;
  case 321 : particle += unicode ? "K" : "K^{+}"; break;

  case 313 : particle += unicode ? "K*0" : "K^{*}(892)^{0}"; break;
  case 315 : particle += unicode ? "K*0_2" : "K^{*}_{2}(1430)"; break;
  case 323 : particle += unicode ? "K*" : "K^{*}(892)^{+}"; break;

  case 10311 : particle += unicode ? "K0*_0(1430)" : "K^{*}_{0}(1430)"; break;

  case 221 : particle += unicode ? "\u03B7" : "#eta"; break;
  case 331 : particle += unicode ? "\u03B7\'" : "#eta'"; break;
  case 223 : particle += unicode ? "\u03C9" : "#omega"; break;
  case 333 : particle += unicode ? "\u03C6" : "#phi"; break;

  case 411 : particle += unicode ? "D" : "D^{+}"; break;
  case 421 : particle += unicode ? "D0" : "D^{0}"; break;
  case 413 : particle += unicode ? "D*" : "D^{*} 2010)^{+}"; break;
  case 423 : particle += unicode ? "D*0" : "D^{*}(2007)^{0}"; break;
  case 431 : particle += unicode ? "D_S" : "D_{s}^{+}"; break;
  case 433 : particle += unicode ? "D*_S" : "D_{s}^{*+}"; break;
  case 443 : particle += unicode ? "J/\u03C8" : "J/#psi"; break;
  case 445 : particle += unicode ? "\u03C7_c2(1P)" : "#chi_{c2}(1P)"; break;
 
  case 511 : particle += unicode ? "B0" : "B^{0}"; break;
  case 521 : particle += unicode ? "B" : "B^{+}"; break;
  case 513 : particle += unicode ? "B*0" : "B^{*0}"; break;
  case 523 : particle += unicode ? "B*" : "B^{*+}"; break;
  case 531 : particle += unicode ? "B0_S" : "B^{0}_{s}"; break;
  case 533 : particle += unicode ? "B*0_S" : "B^{*0}_{s}"; break;

  case 1101 : particle += unicode ? "(dd)0" : "(dd)_{0}"; break;
  case 2101 : particle += unicode ? "(ud)0" : "(ud)_{0}"; break;
  case 2103 : particle += unicode ? "(ud)1" : "(ud)_{1}"; break;
  case 2203 : particle += unicode ? "(uu)1" : "(uu)_{1}"; break;

  case 2212 : particle += "p"; break;
  case 2112 : particle += "n"; break;
  case 2224 : particle += unicode ? "\u0394++" : "#Delta^{++}"; break;
  case 2214 : particle += unicode ? "\u0394+" : "#Delta^{+}"; break;
  case 2114 : particle += unicode ? "\u03940" : "#Delta^{0}"; break;
  case 1114 : particle += unicode ? "\u0394-" : "#Delta^{-}"; break;

  case 3122 : particle += unicode ? "\u0394" : "#Lambda"; break;
  case 3222 : particle += unicode ? "\u03A3" : "#Sigma^{+}"; break;
  case 3224 : particle += unicode ? "\u03A3*" : "#Sigma^{*+}"; break;
  case 3212 : particle += unicode ? "\u03A30" : "#Sigma^{0}"; break;
  case 3322 : particle += unicode ? "\u03A30" : "#Xi^{0}"; break;
  case 3312 : particle += unicode ? "\u03A3-" : "#Xi^{-}"; break;
  case 3112 : particle += unicode ? "\u03A3-" : "#Sigma_{-}"; break;

  case 3324 : particle += unicode ? "\u039E*0" : "#Xi^{*0}"; break;
  case 3334 : particle += unicode ? "\u03A9-" : "#Omega^{-}"; break;

  case 4214 : particle += unicode ? "\u03A3*_C" : "#Sigma_{c}^{*+}"; break;
  case 4222 : particle += unicode ? "\u03A3_C" : "#Sigma_{c}^{++}"; break;
  case 4122 : particle += unicode ? "\u039BC" : "#Lambda_{c}^{+}"; break;
  case 4114 : particle += unicode ? "\u03A3*0_C" : "#Sigma_{c}^{*0}"; break;
  case 4224 : particle += unicode ? "\u03A3+C" : "#Sigma_{c}^{*++}"; break;

  case 5122 : particle += unicode ? "\u039B0_b" : "#Lambda^{0}_{b}"; break;
  case 5212 : particle += unicode ? "\u03A30_b" : "#Sigma_{b}^{0}"; break;
  case 5232 : particle += unicode ? "\u039E0_b" : "#Xi_{b}^{0}"; break;

  case 10313: particle += unicode ? "K0_1 (1270)" : "K_{1} (1270)^{0}"; break;
  case 10441: particle += unicode ? "\u03C7_c0(1P)" : "#chi_{c0}(1P)"; break;
  case 10551: particle += unicode ? "\u03C7_b0(1P)" : "#chi_{b0}(1P)"; break;
  case 20443: particle += unicode ? "\u03C7_c1(1P)" : "#chi_{c1}(1P)"; break;
  case 20313: particle += unicode ? "K0_1 (1400)" : "K_{1}(1400)^0}"; break;
  case 20213: particle += unicode ? "a1" : "a_{1} (1260)^{+}"; break;

  case 9000006 : particle += unicode ? "Scalar" : "SCALAR"; break;
  case 9010221 : particle += unicode ? "f0(980)" : "f_{0}(980)"; break;

  default : {particle += std::to_string(std::abs(parId)); std::cout << "UNKNOWN PID: " << parId << std::endl;}
  }

  if ( unicode && (parId == 211 || parId == 213 || parId == 321 || parId == 323 || parId == 411 || parId == 431 || parId == 433 || parId == 521 || parId == 4122 || parId == 20213 || parId == 4214 || parId == 523
		   || parId == 3224 || parId == 3222
		   )) particle += "+";

  if ( unicode && parId < 0) particle += "-";
  if ( unicode && (parId == -4222 || parId == -4224) ) particle += "-";
  if ( unicode && (parId == 4222 || parId == 4222) ) particle += "+";

  return particle;
}


bool SimpleAnalysis::scalarGrandparent (const AnalysisEvent& event, const Int_t& k, const Int_t& grandparentId) const {

  const Int_t pdgId { std::abs(event.genParId[k]) };
  const Int_t numDaughters { event.genParNumDaughters[k] };
  const Int_t motherId { std::abs(event.genParMotherId[k]) };
  const Int_t motherIndex { std::abs(event.genParMotherIndex[k]) };


  if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
  else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
  else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
  else {
    // std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
    // debugCounter++;
    // std::cout << "debugCounter: " << debugCounter << std::endl;
    return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
  }
}
