#include "AnalysisEvent.hpp"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
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
#include <cmath>

std::string pdgIdCode (const Int_t pdgId, const bool unicode = false); // declaring function called below main(); pdgIdCode translate stored numerical pdgId code into a string: unicode for output to string, or latex style for ROOT plotting
bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);



namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{

  auto timerStart = std::chrono::high_resolution_clock::now();  // timer used to figure out how long program takes to run

  // declare some variables used for config and dataset parsing
  std::string config;
  std::vector<Dataset> datasets;

  double totalLumi;
  double usePreLumi;

  std::map<int, int> pdgIdMap; // declare map of int, int - first int corresponds to pdgId, second will note how many times a particle with that pdgId has been found
  std::string outFileString{"plots/distributions/output.root"}; //
  const bool is2016_ {false}; // analysis framework is setup to run over multiple years - as we are considering 2017 conditions currently, this is set to false for safety.
  Long64_t nEvents; // Max number of events to consider per dataset. Default is set in config file, but can be overriden with command line arguements
  Long64_t totalEvents {0}; // Counter for total number of events

  // Declare TH1F GenPar plots
    
  TH1F* h_genParPt      {new TH1F("h_genParPt",  "genPar p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParEta     {new TH1F("h_genParEta", "genPar #eta",  200, -7., 7.)};
  TH1F* h_genParPhi     {new TH1F("h_genParPhi", "genPar #phi",  100, -3.5, 3.5)};
  TH1F* h_genParE       {new TH1F("h_genParE",   "genPar energy",     1000, 0., 1000.)};
  TH1F* h_VertexPosR    {new TH1F("h_VertexPosR", "Vertex position R",100,0.,250.)};
    
  //Higgs boson
  TH1F* h_genParHiggsPt      {new TH1F("h_genParHiggsPt",  "genPar h_0 p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParHiggsEta     {new TH1F("h_genParHiggsEta", "genPar h_0 #eta",  200, -7., 7.)};
  TH1F* h_genParHiggsPhi     {new TH1F("h_genParHiggsPhi", "genPar h_0 #phi",  100, -3.5, 3.5)};
  TH1F* h_genParHiggsE       {new TH1F("h_genParHiggsE",   "genPar h_0 energy",     1000, 0., 1000.)};
  TH1F* h_HiggsInvMass       {new TH1F("h_HiggsInvMass",  "h_0 Invariant mass", 1000, 0., 1000.)};
    
  //Scalar decay
  TH1F* h_genParScalarPt      {new TH1F("h_genParScalarPt",  "Scalar p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarEta     {new TH1F("h_genParScalarEta", "Scalar #eta",  200, -7., 7.)};
  TH1F* h_genParScalarPhi     {new TH1F("h_genParScalarPhi", "Scalar #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarE       {new TH1F("h_genParScalarE",   "Scalar energy",     1000, 0., 1000.)};
  TH1F* h_ScalarDeltaR        {new TH1F("h_ScalarDeltaR", "Scalar #DeltaR",1500,-10., 10.)};
  TH1F* h_ScalarDeltaPhi      {new TH1F("h_ScalarDeltaPhi", "Scalar #Delta#phi",1500, -3.5, 3.5)};
  TH1F* h_ScalarInvMass       {new TH1F("h_ScalarInvMass", "Scalar Invariant mass",1000, 0., 7.)};
  TH1F* h_Scalar3DAngle       {new TH1F("h_Scalar3DAngle", "Scalar 3D Angle",1000,-10., 10.)};
    
  //Muon from scalar decay
  TH1F* h_genParScalarMuonPt          {new TH1F("h_genParScalarMuonPt",  "#mu^{#pm} from scalar decay p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarMuonCutPtSL     {new TH1F("h_genParScalarMuonCutPtSL",  "Single #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarMuonDivPtSL     {new TH1F("h_genParScalarMuonDivPtSL",  "Turn-on Single #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.)};
  TH1F* h_genParScalarMuonCutPtDL     {new TH1F("h_genParScalarMuonCutPtDL",  "Double #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarMuonDivPtDL     {new TH1F("h_genParScalarMuonDivPtDL",  "Turn-on Double #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.)};
  TH1F* h_genParScalarMuonCutPtDS     {new TH1F("h_genParScalarMuonCutPtDS",  "Double #mu^{#pm} trigger subleading p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarMuonDivPtDS     {new TH1F("h_genParScalarMuonDivPtDS",  "Turn-on Double #mu^{#pm} trigger subleading p_{T}", 300, 0., 1000.)};
    
  TH1F* h_genParScalarMuonEta     {new TH1F("h_genParScalarMuonEta", "#mu^{#pm} from scalar decay #eta",  200, -7., 7.)};
  TH1F* h_genParScalarMuonPhi     {new TH1F("h_genParScalarMuonPhi", "#mu^{#pm} from scalar decay #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarMuonE       {new TH1F("h_genParScalarMuonE",   "#mu^{#pm} from scalar decay energy",     1000, 0., 1000.)};
  TH1F* h_MuonDeltaR              {new TH1F("h_MuonDeltaR", "Muon #DeltaR",2500, -10., 10.)};
  TH1F* h_MuonDeltaPhi            {new TH1F("h_MuonDeltaPhi", "Muon #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_MuonInvMass             {new TH1F("h_MuonInvMass", "Muon Invariant mass",1000, 0., 7.)};
  TH1F* h_Muon3DAngle             {new TH1F("h_Muon3DAngle", "Muon 3D Angle",1000,-10., 10.)};
    
  //Kaon from scalar decay
  TH1F* h_genParScalarCKaonPt      {new TH1F("h_genParScalarCKaonPt",  "K^{#pm} from scalar decay p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarCKaonEta     {new TH1F("h_genParScalarCKaonEta", "K^{#pm} from scalar decay #eta",  200, -7., 7.)};
  TH1F* h_genParScalarCKaonPhi     {new TH1F("h_genParScalarCKaonPhi", "K^{#pm} from scalar decay #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarCKaonE       {new TH1F("h_genParScalarCKaonE",   "K^{#pm} from scalar decay energy",     1000, 0., 1000.)};
  TH1F* h_KaonDeltaR               {new TH1F("h_KaonDeltaR", "Kaon #DeltaR",2500, -10., 10.)};
  TH1F* h_KaonDeltaPhi             {new TH1F("h_KaonDeltaPhi", "Kaon #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_KaonInvMass              {new TH1F("h_KaonInvMass", "Kaon Invariant mass",1000, 0., 7.)};
  TH1F* h_Kaon3DAngle              {new TH1F("h_Kaon3DAngle", "Kaon 3D Angle",1000,-10., 10.)};
    
  //K short from scalar decay
  TH1F* h_genParScalarKShortPt      {new TH1F("h_genParScalarKShortPt",  "K^{0}_S from scalar decay p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarKShortEta     {new TH1F("h_genParScalarKShortEta", "K^{0}_S from scalar decay #eta",  200, -7., 7.)};
  TH1F* h_genParScalarKShortPhi     {new TH1F("h_genParScalarKShortPhi", "K^{0}_S from scalar decay #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarKShortE       {new TH1F("h_genParScalarKShortE",   "K^{0}_S from scalar decay energy",     1000, 0., 1000.)};
  TH1F* h_KShortDeltaR              {new TH1F("h_KShortDeltaR", "K^{0}_S #DeltaR",2500, -10., 10.)};
  TH1F* h_KShortDeltaPhi            {new TH1F("h_KShortDeltaPhi", "K^{0}_S #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_KShortInvMass             {new TH1F("h_KShortInvMass", "K^{0}_S Invariant mass",1000, 0., 7.)};
  TH1F* h_KShort3DAngle             {new TH1F("h_KShort3DAngle", "K^{0}_S 3D Angle",1000,-10., 10.)};
    
  //Pion from scalar decay
  TH1F* h_genParScalarCPionPt      {new TH1F("h_genParScalarCPionPt",  "#pi^{#pm} from scalar decay p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarCPionEta     {new TH1F("h_genParScalarCPionEta", "#pi^{#pm} from scalar decay #eta",  200, -7., 7.)};
  TH1F* h_genParScalarCPionPhi     {new TH1F("h_genParScalarCPionPhi", "#pi^{#pm} from scalar decay #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarCPionE       {new TH1F("h_genParScalarCPionE",   "#pi^{#pm} from scalar decay energy",     1000, 0., 1000.)};
  TH1F* h_genParScalarNPionPt      {new TH1F("h_genParScalarNPionPt",  "#pi^{0} from scalar decay p_{T}", 1000, 0., 1000.)};
  TH1F* h_genParScalarNPionEta     {new TH1F("h_genParScalarNPionEta", "#pi^{0} from scalar decay #eta",  200, -7., 7.)};
  TH1F* h_genParScalarNPionPhi     {new TH1F("h_genParScalarNPionPhi", "#pi^{0} from scalar decay #phi",  100, -3.5, 3.5)};
  TH1F* h_genParScalarNPionE       {new TH1F("h_genParScalarNPionE",   "#pi^{0} from scalar decay energy",     1000, 0., 1000.)};
  TH1F* h_PionDeltaR               {new TH1F("h_PionDeltaR", "Pion #DeltaR",2500, -10., 10.)};
  TH1F* h_PionDeltaPhi             {new TH1F("h_PionDeltaPhi", "Pion #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_PionInvMass              {new TH1F("h_PionInvMass", "Pion Invariant mass",1000, 0., 7.)};
  TH1F* h_Pion3DAngle              {new TH1F("h_Pion3DAngle", "Pion 3D Angle",1000,-10., 10.)};
  
    
  //Vertex position: muons, kaons, kshort, pions
  TH2I* h_VertexPosXY  {new TH2I("h_VertexPosXY", "Vertex Position XY", 100, -150,150,100,-150,150)};
  TH2I* h_VertexPosRZ  {new TH2I("h_VertexPosRZ", "Vertex Position RZ", 100, 0,20,100,0,250)};
    
  //RECONSTRUCTION histograms
  //Muon reco
  TH1F* h_muonRecPt            {new TH1F("h_muonRecPt",  "#mu^{#pm} reconstruction p_{T}", 1000, 0., 1000.)};
  TH1F* h_muonRecEta           {new TH1F("h_muonRecEta", "#mu^{#pm} reconstruction #eta",  200, -7., 7.)};
  TH1F* h_muonRecPhi           {new TH1F("h_muonRecPhi", "#mu^{#pm} reconstruction #phi",  100, -3.5, 3.5)};
  TH1F* h_muonRecE             {new TH1F("h_muonRecE",   "#mu^{#pm} reconstruction energy",     1000, 0., 1000.)};
  TH1F* h_muonRecDeltaR        {new TH1F("h_muonRecDeltaR", "Muon reconstruction #DeltaR",2500, -10., 10.)};
  TH1F* h_muonRecDeltaPhi      {new TH1F("h_muonRecDeltaPhi", "Muon reconstruction #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_muonRecInvMass       {new TH1F("h_muonRecInvMass", "Muon reconstruction invariant mass",1000, 0, 500)};
  TH1F* h_muonCutSingleL       {new TH1F("h_muonCutSingleL",  "Single #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.)};
  TH1F* h_muonCutDoubleL       {new TH1F("h_muonCutDoubleL",  "Double #mu^{#pm} trigger leading p_{T}", 1000, 0., 1000.)};
  TH1F* h_muonDivSingleL       {new TH1F("h_muonDivSingleL",  "Turn-on Single #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.)};
  TH1F* h_muonDivDoubleL       {new TH1F("h_muonDivDoubleL",  "Turn-on Double #mu^{#pm} trigger leading p_{T}", 300, 0., 1000.)};
  TH1F* h_muonCutDoubleS       {new TH1F("h_muonCutDoubleS",  "Double #mu^{#pm} trigger subleading p_{T}", 1000, 0., 1000.)};
  TH1F* h_muonDivDoubleS       {new TH1F("h_muonDivDoubleS",  "Turn-on Double #mu^{#pm} trigger subleading p_{T}", 300, 0., 1000.)};
    
  //Packed candidates
  TH1F* h_packedCDxy    {new TH1F("h_packedCDxy", "Packed Candidate Dxy", 500,  -200., 200.)};
  TH1F* h_packedCDz     {new TH1F("h_packedCDz",  "Packed Candidate Dz", 1500, -500., 500.)};
  TH1F* h_packedCVx     {new TH1F("h_packedCVx",  "Packed Candidate track vx", 500,  -150., 150.)};
  TH1F* h_packedCVy     {new TH1F("h_packedCVy",  "Packed Candidate track vy", 500,  -150., 150.)};
  TH1F* h_packedCVz     {new TH1F("h_packedCVz",  "Packed Candidate track vz", 1500, -500., 500.)};
  TH2I* h_displacedXY   {new TH2I("h_displacedXY", "Displacement XY", 100, -150,150,100,-150,150)};
  TH2I* h_displacedRZ   {new TH2I("h_displacedRZ", "Displacement RZ", 100, 0,20,100,0,250)};
  TH1F* h_hadronDeltaR  {new TH1F("h_hadronDeltaR", "Two hadrons #DeltaR",2500, -10., 10.)};
  TH1F* h_muonsDeltaR   {new TH1F("h_muonsDeltaR", "Two muons #DeltaR",2500, -10., 10.)};
  TH1F* h_IsoSum1       {new TH1F("h_IsoSum1",  "0.3 p_{T} Cone construction pion 1", 1000, 0., 1000.)};
  TH1F* h_IsoSum2       {new TH1F("h_IsoSum2",  "0.3 p_{T} Cone construction pion 2", 1000, 0., 1000.)};
  TH1F* h_IsoSum3       {new TH1F("h_IsoSum3",  "0.3 p_{T} Cone construction muon 1", 1000, 0., 1000.)};
  TH1F* h_IsoSum4       {new TH1F("h_IsoSum4",  "0.3 p_{T} Cone construction muon 2", 1000, 0., 1000.)};
  TH1F* h_hadronInvMass {new TH1F("h_hadronInvMass", "Two hadrons - Invariant mass",1000, 0., 7.)};
  TH1F* h_muonsInvMass  {new TH1F("h_muonsInvMass", "Two muons - Invariant mass",1000, 0., 7.)};
  TH2F* h_invmass       {new TH2F("h_invmass", "Invariant mass: pions vs muons", 1000, 0.,7.,1000,0.,7.)};
   
  TH1F* h_testInvMass   {new TH1F("h_testInvMass", "Invariant mass Packed Candidates",1000, 0., 7.)};
    
    
    
     
    
  namespace po = boost::program_options;

  // command line configuration parsing magic!
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
                                                                                                      "The number of events to be run over. All if set to 0.");
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

      // Progress bar drawing for event processing progress
      TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}};
      lEventTimer->DrawProgressBar(0, "");
    
      totalEvents += numberOfEvents;

      // Loop over all events in this dataset
      for (Long64_t i{0}; i < numberOfEvents; i++) {

    lEventTimer->DrawProgressBar(i,""); // increment progress bar

    //
    event.GetEntry(i);


          
          
          
          
          
    //GENERATOR PARTICLE STUFF
    std::vector<int> nrofHiggs;
    std::vector<int> nrofScalar; //Number of scalars
    std::vector<int> nrofMuon;
    std::vector<int> nrofKaon;
    std::vector<int> nrofKShort;
    std::vector<int> nrofPion;
    
    Float_t genpt1=0; Float_t genpt2=0;
    std::cout << "idx\t | ID\t stat\t | Mo\t Da1\t Da2\t | pt\t eta\t phi\t m" << std::endl;
         
       for (Int_t k{0}; k < event.nGenPar; k++) {
     
          //Print out event record

          //Invariant mass
          TLorentzVector m;
          m.SetPtEtaPhiE(event.genParPt[k],event.genParEta[k],event.genParPhi[k],event.genParE[k]);
          TLorentzVector mass {m.Px(),m.Py(),m.Pz(),event.genParE[k]};

          /*std::cout << k << "\t | "
          << event.genParId[k] << "\t "
          << event.genParStatus[k] << "\t | "
          << event.genParMotherIndex[k] << "\t "
          << event.genParDaughter1Index[k] << "\t "
          << event.genParDaughter2Index[k] << "\t | "
          << event.genParPt[k] << "\t "
          << event.genParEta[k] << "\t "
          << event.genParPhi[k] << "\t "
          << mass.M() << std::endl;*/






      // get variables for this event that have been stored in ROOT nTuple tree
      const Int_t pdgId        { std::abs(event.genParId[k]) };
      const Int_t motherId     { std::abs(event.genParMotherId[k]) };
      const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };
      const Int_t genParVx     {event.genParVx[k]};
      const Int_t genParVy     {event.genParVy[k]};
      const Int_t genParVz     {event.genParVz[k]};
            
      const Float_t genParEta  { event.genParEta[k] };
      const Float_t genParPhi  { event.genParPhi[k] };
      const Float_t genParE    { event.genParE[k] };
     
      const bool ownParent {pdgId == motherId ? true : false};

      h_genParPt->Fill(event.genParPt[k]);
      h_genParEta->Fill(genParEta);
      h_genParPhi->Fill(genParPhi);
      h_genParE->Fill(genParE);
        
      //Higgs boson
      if (pdgId==25 && !ownParent){ //First entry Higgs - to obtain mass correctly
        nrofHiggs.emplace_back(k);
        h_genParHiggsPt->Fill(event.genParPt[k]);
        h_genParHiggsEta->Fill(genParEta);
        h_genParHiggsPhi->Fill(genParPhi);
        h_genParHiggsE->Fill(genParE);
      }
            
      //Scalar decay
      if (pdgId==9000006){
        nrofScalar.emplace_back(k); //Store the scalar index k in nrofScalar
        h_genParScalarPt->Fill(event.genParPt[k]);
        h_genParScalarEta->Fill(genParEta);
        h_genParScalarPhi->Fill(genParPhi);
        h_genParScalarE->Fill(genParE);
      }
            
      //Particles from scalar decay
      const bool isScalarGrandparent{scalarGrandparent(event,k,9000006)};
      std::vector<Int_t> mu; std::vector<Int_t> mumu;
         
      if (isScalarGrandparent==true){
          
          //Muon from scalar decay
         if (pdgId==13 && !ownParent){
             
            nrofMuon.emplace_back(k);
            
            h_genParScalarMuonEta->Fill(genParEta);
            h_genParScalarMuonPhi->Fill(genParPhi);
            h_genParScalarMuonE->Fill(genParE);
            h_VertexPosXY->Fill(genParVx,genParVy);
            h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx^2+genParVy^2));
            h_VertexPosR->Fill(std::sqrt(genParVx^2+genParVy^2));
        
            h_genParScalarMuonPt->Fill(event.genParPt[k]);
            if(event.muTrig() || event.mumuTrig()){
              if(event.genParPt[k]>genpt1){
                genpt2=genpt1;
                genpt1=event.genParPt[k];
              }
              else if(event.genParPt[k]>genpt2){
                     genpt2=event.genParPt[k];
              }
            }
            if(genpt1!=0 && genpt2!=0){         
               if(event.muTrig()){
                 h_genParScalarMuonCutPtSL->Fill(genpt1); //leading momenta for the event
               }
               if(event.mumuTrig()){
                  h_genParScalarMuonCutPtDL->Fill(genpt1);
                  h_genParScalarMuonCutPtDS->Fill(genpt2);
               }
            }
             
         }
        //Charged kaon from scalar decay
        if (pdgId==321){
          nrofKaon.emplace_back(k);
          h_genParScalarCKaonPt->Fill(event.genParPt[k]);
          h_genParScalarCKaonEta->Fill(genParEta);
          h_genParScalarCKaonPhi->Fill(genParPhi);
          h_genParScalarCKaonE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx^2+genParVy^2));
          h_VertexPosR->Fill(std::sqrt(genParVx^2+genParVy^2));
        }
        //K short from scalar decay
        if (pdgId==310){
          nrofKShort.emplace_back(k); //Together with kaon in angular differences
          h_genParScalarKShortPt->Fill(event.genParPt[k]);
          h_genParScalarKShortEta->Fill(genParEta);
          h_genParScalarKShortPhi->Fill(genParPhi);
          h_genParScalarKShortE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx^2+genParVy^2));
              h_VertexPosR->Fill(std::sqrt(genParVx^2+genParVy^2));
        }
        //Charged pion from scalar decay
        if (pdgId==211){
          nrofPion.emplace_back(k);
          h_genParScalarCPionPt->Fill(event.genParPt[k]);
          h_genParScalarCPionEta->Fill(genParEta);
          h_genParScalarCPionPhi->Fill(genParPhi);
          h_genParScalarCPionE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx^2+genParVy^2));
          h_VertexPosR->Fill(std::sqrt(genParVx^2+genParVy^2));
        }
        //Neutral pion from scalar decay
        if (pdgId==111){
          nrofPion.emplace_back(k);
          h_genParScalarNPionPt->Fill(event.genParPt[k]);
          h_genParScalarNPionEta->Fill(genParEta);
          h_genParScalarNPionPhi->Fill(genParPhi);
          h_genParScalarNPionE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx^2+genParVy^2));
          h_VertexPosR->Fill(std::sqrt(genParVx^2+genParVy^2));
        }
      }

      // Increment counter for pdgId found
      pdgIdMap[pdgId]++;
            
    }

    h_genParScalarMuonDivPtSL=(TH1F*)h_genParScalarMuonCutPtSL->Clone();
    h_genParScalarMuonDivPtSL->Divide(h_genParScalarMuonPt);
    h_genParScalarMuonDivPtSL->SetTitle("Turn-on Single trigger leading");
    
    h_genParScalarMuonDivPtDL=(TH1F*)h_genParScalarMuonCutPtDL->Clone();
    h_genParScalarMuonDivPtDL->Divide(h_genParScalarMuonPt);
    h_genParScalarMuonDivPtDL->SetTitle("Turn-on Double trigger leading");
    
    h_genParScalarMuonDivPtDS=(TH1F*)h_genParScalarMuonCutPtDS->Clone();
    h_genParScalarMuonDivPtDS->Divide(h_genParScalarMuonPt);
    h_genParScalarMuonDivPtDS->SetTitle("Turn-on Double trigger subleading");
          
          
    if (nrofScalar.size()==2){ //Two-particle (scalar) correlations
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
        
    if (nrofMuon.size()==2){
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
        
    if (nrofKaon.size()==2){
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
        
    if (nrofKShort.size()==2){
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
        
        
    if (nrofPion.size()==2){
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

    
          
          
    
          
          
    /// BEGIN Muon Reconstruction
    std::vector<Int_t> passedMuons;
 
    uint singleFlag{0}; uint doubleFlag{0};
    std::vector<Int_t> singleIndex{}; std::vector<Int_t> doubleIndex{};
          
    if(event.metFilters()){
      
       for (Int_t k{0}; k < event.numMuonPF2PAT; k++) {
                 
           const Float_t muonRecPt   { event.muonPF2PATPt[k] };
           const Float_t muonRecEta  { event.muonPF2PATEta[k] };
           const Float_t muonRecPhi  { event.muonPF2PATPhi[k] };
           const Float_t muonRecE    { event.muonPF2PATE[k] };
           
           h_muonRecPt->Fill(event.muonPF2PATPt[0]);//Two highest momenta
           h_muonRecPt->Fill(event.muonPF2PATPt[1]);
           
           h_muonRecEta->Fill(muonRecEta);
           h_muonRecPhi->Fill(muonRecPhi);
           h_muonRecE->Fill(muonRecE);
           
           if(event.muTrig() || event.mumuTrig()){
               
             if(event.muonPF2PATCharge[0]==-(event.muonPF2PATCharge[1])){ //Electric charge control
        
               TLorentzVector muonRec1;
               TLorentzVector muonRec2;
          
               muonRec1.SetPtEtaPhiE(event.muonPF2PATPt[0],event.muonPF2PATEta[0],event.muonPF2PATPhi[0],event.muonPF2PATE[0]);
               muonRec2.SetPtEtaPhiE(event.muonPF2PATPt[1],event.muonPF2PATEta[1],event.muonPF2PATPhi[1],event.muonPF2PATE[1]);
            
               h_muonRecDeltaR->Fill(muonRec1.DeltaR(muonRec2));
               h_muonRecDeltaPhi->Fill(muonRec1.DeltaPhi(muonRec2));
            
               //Invariant mass for two highest p_T
               TLorentzVector lVecMu1  {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
               TLorentzVector lVecMu2  {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]};

               h_muonRecInvMass->Fill( (lVecMu1+lVecMu2).M() );
           
               if(event.muonPF2PATPt[0]>30 && event.muonPF2PATPt[1]>12){//combined (single+double, mix) p_T cut applied
             
                 if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4){//Loose ID cut and |eta| < 2.4
                   passedMuons.emplace_back(k);//Take its index
                 }
               }
             }
           }
        
        
           //To show seperate turn-on curve for single or double muon trigger
           if(event.muTrig() ){
             if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4 && event.muonPF2PATPt[k]>5){ //Loose ID cut, |eta| < 2.4 and min 5GeV cut
               singleFlag++; singleIndex.emplace_back(k);
             }
           }
         
           if(singleFlag>0){
             const int s1 {singleIndex[0]};
             h_muonCutSingleL->Fill(event.muonPF2PATPt[s1]);
           }
           if(event.mumuTrig()){
             if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4 && event.muonPF2PATPt[k]>5){//Loose ID cut, |eta| < 2.4 and min 5GeV cut
               doubleFlag++; doubleIndex.emplace_back(k);
             } 
           }
           if(doubleFlag>1){
             const int d1 {doubleIndex[0]}; const int d2 {doubleIndex[1]};
             h_muonCutDoubleL->Fill(event.muonPF2PATPt[d1]);
             h_muonCutDoubleS->Fill(event.muonPF2PATPt[d2]);
           }
               
       }
    }//MET filter
    
    h_muonDivSingleL=(TH1F*)h_muonCutSingleL->Clone();
    h_muonDivSingleL->Divide(h_muonRecPt);
    h_muonDivSingleL->SetTitle("Turn-on Single trigger, leading");
          
    h_muonDivDoubleL=(TH1F*)h_muonCutDoubleL->Clone();
    h_muonDivDoubleL->Divide(h_muonRecPt);
    h_muonDivDoubleL->SetTitle("Turn-on Double trigger, leading");
          
    h_muonDivDoubleS=(TH1F*)h_muonCutDoubleS->Clone();
    h_muonDivDoubleS->Divide(h_muonRecPt);
    h_muonDivDoubleS->SetTitle("Turn-on Double trigger, subleading");

    //END Muon Reconstruction
          
          
          
          
          
    //BEGIN Packed candidates
    Int_t pionIndex1{-1}; Int_t pionIndex2{-1};
    std::vector<Int_t> muonIndex{};
     
    Float_t pionpt1{-1}; Float_t pionpt2{-1};
    Float_t mupt1=0; Float_t mupt2=0;
        
    Float_t IsoSum1=0;  Float_t IsoSum2=0;
    Float_t IsoSum3=0;  Float_t IsoSum4=0;
   
    Float_t hadroninv; Float_t muoninv;
          
    if(event.metFilters()){
        
      for (Int_t k{0};k<event.numPackedCands;k++) {
          
          if(event.muTrig() || event.mumuTrig()){
            
            const Int_t packedId {event.packedCandsPdgId[k]};
            //Id of 211 or -211: Charged pions
        
            h_packedCDxy->Fill(event.packedCandsDxy[k]);
            h_packedCDz->Fill(event.packedCandsDz[k]);
          
            if(event.packedCandsHasTrackDetails[k]==1){
          
              //Test invariant masses
              TLorentzVector test  {event.packedCandsPseudoTrkPx[k], event.packedCandsPseudoTrkPy[k], event.packedCandsPseudoTrkPz[k], event.packedCandsE[k]};
              h_testInvMass->Fill(test.M());
              //std::cout<<"invariant mass packed candid "<<test.M()<<std::endl;
            
              const Int_t packedCandsPseudoTrkCharge {event.packedCandsPseudoTrkCharge[k]};
              const Int_t packedCandsCharge {event.packedCandsCharge[k]};
            
              if(packedCandsCharge!=0 && packedCandsCharge==packedCandsPseudoTrkCharge){ //No neutral particles as they don't leave tracks, only charged
               
                h_packedCVx->Fill(event.packedCandsPseudoTrkVx[k]);
                h_packedCVy->Fill(event.packedCandsPseudoTrkVy[k]);
                h_packedCVz->Fill(event.packedCandsPseudoTrkVz[k]);
              
                //Displacement from interaction point
                h_displacedXY->Fill(event.packedCandsPseudoTrkVx[k],event.packedCandsPseudoTrkVy[k]);
                h_displacedRZ->Fill(std::abs(event.packedCandsPseudoTrkVz[k]),std::sqrt(event.packedCandsPseudoTrkVx[k]*event.packedCandsPseudoTrkVx[k]+event.packedCandsPseudoTrkVy[k]*event.packedCandsPseudoTrkVy[k]));
             
          
                //Find the hadrons (pions)
                if(std::abs(packedId)!=13 && event.packedCandsPseudoTrkPt[k]>5){//Selection of pions (charged hadrons)
                  
                  if(event.packedCandsPseudoTrkPt[k]>pionpt1){
                    pionpt2=pionpt1;
                    pionpt1=event.packedCandsPseudoTrkPt[k];
                    pionIndex2=pionIndex1;
                    pionIndex1=k;
                  }
                  else if(event.packedCandsPseudoTrkPt[k]>pionpt2){
                         pionpt2=event.packedCandsPseudoTrkPt[k];
                         pionIndex2=k;
                  }
                  std::cout<<"k "<<k<<"pion mom 1 "<<pionpt1<<"pion mom 2 "<<pionpt2<<std::endl;
                  std::cout<<"pionIndex1 "<<pionIndex1<<"pionIndex2 "<<pionIndex2<<std::endl;
                  std::cout<<"Controle mom 1 "<<event.packedCandsPseudoTrkPt[pionIndex1]<<"Controle mom 2 "<<event.packedCandsPseudoTrkPt[pionIndex2]<<std::endl;
                }
                //std::cout<<"laatste element k "<<k<<"index voor pion "<<pionIndex1.back()<<std::endl;
                /*if(std::abs(packedId)==13 && event.packedCandsPseudoTrkPt[k]>5){//Selection of muons
                  muonIndex.emplace_back(k);
                  if(event.packedCandsPseudoTrkPt[k]>mupt1){
                    mupt2=mupt1;
                    mupt1=event.packedCandsPseudoTrkPt[k];
                    std::cout<<"leading "<<k<<std::endl;
                  }
                  else if(event.packedCandsPseudoTrkPt[k]>mupt2){
                         mupt2=event.packedCandsPseudoTrkPt[k];
                         std::cout<<"subleading "<<k<<std::endl;
                  }
                }*/
              }
              //Safety measure
              /*if(pionIndex1.size()>0 && pionIndex2.size()>0){
                //const Int_t p1 {pionIndex1.end()}; const Int_t p2 {pionIndex2.end()};
                std::cout<<"index1 "<<pionIndex1.end()<<"index2 "<<pionIndex2.end()<<std::endl;
                if(event.packedCandsPseudoTrkCharge[p1]==-(event.packedCandsPseudoTrkCharge[p2])){//Opposite charge
            
                  TLorentzVector packed1;
                  TLorentzVector packed2;
          
                  packed1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[p1],event.packedCandsPseudoTrkEta[p1],event.packedCandsPseudoTrkPhi[p1],event.packedCandsE[p1]);
                  packed2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[p2],event.packedCandsPseudoTrkEta[p2],event.packedCandsPseudoTrkPhi[p2],event.packedCandsE[p2]);
            
                  h_hadronDeltaR->Fill(packed1.DeltaR(packed2));
            
                  //Invariant mass for two hadrons
                  TLorentzVector lhadron1  {event.packedCandsPseudoTrkPx[p1], event.packedCandsPseudoTrkPy[p1], event.packedCandsPseudoTrkPz[p1], event.packedCandsE[p1]};
                  TLorentzVector lhadron2  {event.packedCandsPseudoTrkPx[p2], event.packedCandsPseudoTrkPy[p2], event.packedCandsPseudoTrkPz[p2], event.packedCandsE[p2]};

                  hadroninv=(lhadron1+lhadron2).M();
                  h_hadronInvMass->Fill((lhadron1+lhadron2).M());
                }
            
                if(k!=p1 && k!=p2){
                
                  TLorentzVector cone1;//The pion
                  TLorentzVector cone2;//Packed candidate
                    
                  cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[p1],event.packedCandsPseudoTrkEta[p1],event.packedCandsPseudoTrkPhi[p1],event.packedCandsE[p1]);
                  cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
              
                  if(cone1.DeltaR(cone2)<0.3){
                    IsoSum1+=event.packedCandsPseudoTrkPt[k];
                    h_IsoSum1->Fill(IsoSum1/event.packedCandsPseudoTrkPt[k]);
                  }
            
                  TLorentzVector cone3;//The other pion
                  TLorentzVector cone4;//Packed candidate
                    
                  cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[p2],event.packedCandsPseudoTrkEta[p2],event.packedCandsPseudoTrkPhi[p2],event.packedCandsE[p2]);
                  cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
              
                  if(cone3.DeltaR(cone4)<0.3){
                    IsoSum2+=event.packedCandsPseudoTrkPt[k];
                    h_IsoSum2->Fill(IsoSum2/event.packedCandsPseudoTrkPt[k]);
                  }
                }
              
              }*/ 
              
            /*
                const int m1 {muonIndex[0]}; const int m2 {muonIndex[1]};
                if(event.packedCandsPseudoTrkCharge[m1]==-(event.packedCandsPseudoTrkCharge[m2])){
            
                  //Invariant mass for two muons
                  TLorentzVector lmuon1  {event.packedCandsPseudoTrkPx[m1], event.packedCandsPseudoTrkPy[m1], event.packedCandsPseudoTrkPz[m1], event.packedCandsE[m1]};
                  TLorentzVector lmuon2  {event.packedCandsPseudoTrkPx[m2], event.packedCandsPseudoTrkPy[m2], event.packedCandsPseudoTrkPz[m2], event.packedCandsE[m2]};
                        
                  muoninv=(lmuon1+lmuon2).M();
                  h_muonsInvMass->Fill((lmuon1+lmuon2).M());
                 
                  TLorentzVector mm1;
                  TLorentzVector mm2;
          
                  mm1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[m1],event.packedCandsPseudoTrkEta[m1],event.packedCandsPseudoTrkPhi[m1],event.packedCandsE[m1]);
                  mm2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[m2],event.packedCandsPseudoTrkEta[m2],event.packedCandsPseudoTrkPhi[m2],event.packedCandsE[m2]);
            
                  h_muonsDeltaR->Fill(mm1.DeltaR(mm2));
                }
                if(k!=m1 && k!=m2){
                  
                  TLorentzVector cone1;//The muon
                  TLorentzVector cone2;//Packed candidate

                  cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[m1],event.packedCandsPseudoTrkEta[m1],event.packedCandsPseudoTrkPhi[m1],event.packedCandsE[m1]);
                  cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

                  if(cone1.DeltaR(cone2)<0.3){
                    IsoSum3+=event.packedCandsPseudoTrkPt[k];
                    h_IsoSum3->Fill(IsoSum3/event.packedCandsPseudoTrkPt[k]);
                  }

                  TLorentzVector cone3;//The other muon
                  TLorentzVector cone4;//Packed candidate

                  cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[m2],event.packedCandsPseudoTrkEta[m2],event.packedCandsPseudoTrkPhi[m2],event.packedCandsE[m2]);
                  cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

                  if(cone3.DeltaR(cone4)<0.3){
                    IsoSum4+=event.packedCandsPseudoTrkPt[k];
                    h_IsoSum4->Fill(IsoSum4/event.packedCandsPseudoTrkPt[k]);
                  }
                }
              */
             // h_invmass->Fill(hadroninv,muoninv);
            }
          }
        
          
      }
    }
    //END Packed Candidates
          
      
          
          
          
          
          
          
           
      } //Loop over all events

  } //Loop over all datatsets
        


  
    
    

  std::cout << std::endl;
  std::cout << "Total no. of events:\t\t\t" << totalEvents << std::endl;
  std::cout << std::endl;

  // Do scalable histograms that plot only pdgIds found
  int nPdgIds                   = pdgIdMap.size();                   // number of different final state pdgIds
  // Declare histogram with scalable axes
  TH1I* h_pdgId            {new TH1I{"h_pdgId",            "number of particles in Generator Partice collections"   , nPdgIds,           0, Double_t(nPdgIds)           }};

  // Fill entries for scalable histograms
  uint binCounter {1};
  for (auto it = pdgIdMap.begin(); it != pdgIdMap.end(); ++it) {
    h_pdgId->SetBinContent(binCounter, it->second);
    const char *label = ( pdgIdCode(it->first, false) ).c_str();
    h_pdgId->GetXaxis()->SetBinLabel(binCounter, label);
    binCounter++;
  }

    
    
  // Create output ROOT file
  TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
  // change current ROOT directory to that of outFile
  outFile->cd();

    
    
    
  // Write histograms to file
  h_genParPt->GetXaxis()->SetTitle("GeV");
  h_genParPt->Write();
  h_genParEta->Write();
  h_genParPhi->Write();
  h_genParE->Write();
  h_pdgId->Write();
  h_VertexPosR->Write();

  h_genParHiggsPt->GetXaxis()->SetTitle("GeV");
  h_genParHiggsPt->Write();
  h_genParHiggsEta->Write();
  h_genParHiggsPhi->Write();
  h_genParHiggsE->Write();
  h_HiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_HiggsInvMass->Write();

  h_genParScalarPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarPt->Write();
  h_genParScalarEta->Write();
  h_genParScalarPhi->Write();
  h_genParScalarE->Write();
  h_ScalarDeltaR->Write();
  h_ScalarDeltaPhi->Write();
  h_ScalarInvMass->GetXaxis()->SetTitle("GeV");
  h_ScalarInvMass->Write();
  h_Scalar3DAngle->Write();
    
 
  h_genParScalarMuonPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonPt->Write();
  h_genParScalarMuonCutPtSL->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonCutPtSL->Write();
  h_genParScalarMuonDivPtSL->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonDivPtSL->Write();
  h_genParScalarMuonCutPtDL->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonCutPtDL->Write();
  h_genParScalarMuonDivPtDL->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonDivPtDL->Write();
  h_genParScalarMuonCutPtDS->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonCutPtDS->Write();
  h_genParScalarMuonDivPtDS->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonDivPtDS->Write();
    
  h_genParScalarMuonEta->Write();
  h_genParScalarMuonPhi->Write();
  h_genParScalarMuonE->Write();
  h_MuonDeltaR->Write();
  h_MuonDeltaPhi->Write();
  h_MuonInvMass->GetXaxis()->SetTitle("GeV");
  h_MuonInvMass->Write();
  h_Muon3DAngle->Write();
    
  h_genParScalarCKaonPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarCKaonPt->Write();
  h_genParScalarCKaonEta->Write();
  h_genParScalarCKaonPhi->Write();
  h_genParScalarCKaonE->Write();
  h_KaonDeltaR->Write();
  h_KaonDeltaPhi->Write();
  h_KaonInvMass->GetXaxis()->SetTitle("GeV");
  h_KaonInvMass->Write();
  h_Kaon3DAngle->Write();

  h_genParScalarKShortPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarKShortPt->Write();
  h_genParScalarKShortEta->Write();
  h_genParScalarKShortPhi->Write();
  h_genParScalarKShortE->Write();
  h_KShortDeltaR->Write();
  h_KShortDeltaPhi->Write();
  h_KShortInvMass->GetXaxis()->SetTitle("GeV");
  h_KShortInvMass->Write();
  h_KShort3DAngle->Write();
    
  h_genParScalarCPionPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarCPionPt->Write();
  h_genParScalarCPionEta->Write();
  h_genParScalarCPionPhi->Write();
  h_genParScalarCPionE->Write();
  h_genParScalarNPionPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarNPionPt->Write();
  h_genParScalarNPionEta->Write();
  h_genParScalarNPionPhi->Write();
  h_genParScalarNPionE->Write();
  h_PionDeltaR->Write();
  h_PionDeltaPhi->Write();
  h_PionInvMass->GetXaxis()->SetTitle("GeV");
  h_PionInvMass->Write();
  h_Pion3DAngle->Write();
    
  h_VertexPosXY->GetXaxis()->SetTitle("Vertex position x"); // set a title for the x-axis
  h_VertexPosXY->GetXaxis()->SetRangeUser(-150., 150.);
  h_VertexPosXY->GetYaxis()->SetTitle("Vertex position y"); // set a title for the y-axis
  h_VertexPosXY->GetYaxis()->SetRangeUser(-150., 150.);
  h_VertexPosXY->Write();
  h_VertexPosRZ->GetXaxis()->SetTitle("Vertex position z");
  h_VertexPosRZ->GetYaxis()->SetTitle("R");
  h_VertexPosRZ->Write();
    
  //Reconstruction
  h_muonRecPt->GetXaxis()->SetTitle("GeV");
  h_muonRecPt->Write();
  h_muonRecEta->Write();
  h_muonRecPhi->Write();
  h_muonRecE->Write();
  h_muonRecDeltaR->Write();
  h_muonRecDeltaPhi->Write();
  h_muonRecInvMass->GetXaxis()->SetTitle("GeV");
  h_muonRecInvMass->Write();
  h_muonCutSingleL->GetXaxis()->SetTitle("GeV");
  h_muonCutSingleL->Write();
  h_muonDivSingleL->GetXaxis()->SetTitle("GeV");
  h_muonDivSingleL->Write();
  h_muonCutDoubleL->GetXaxis()->SetTitle("GeV");
  h_muonCutDoubleL->Write();
  h_muonDivDoubleL->GetXaxis()->SetTitle("GeV");
  h_muonDivDoubleL->Write();
  h_muonCutDoubleS->GetXaxis()->SetTitle("GeV");
  h_muonCutDoubleS->Write();
  h_muonDivDoubleS->GetXaxis()->SetTitle("GeV");
  h_muonDivDoubleS->Write();
    
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
  h_hadronDeltaR->Write();
  h_muonsDeltaR->Write();
  h_IsoSum1->Write();
  h_IsoSum1->GetXaxis()->SetTitle("GeV");
  h_IsoSum2->Write();
  h_IsoSum2->GetXaxis()->SetTitle("GeV");
  h_IsoSum3->Write();
  h_IsoSum3->GetXaxis()->SetTitle("GeV");
  h_IsoSum4->Write();
  h_IsoSum4->GetXaxis()->SetTitle("GeV");
  h_muonsInvMass->Write();
  h_muonsInvMass->GetXaxis()->SetTitle("GeV");
  h_hadronInvMass->Write();
  h_hadronInvMass->GetXaxis()->SetTitle("GeV");
  h_invmass->Write();
  h_invmass->GetXaxis()->SetTitle("Hadron invariant mass");
  h_invmass->GetYaxis()->SetTitle("Muon invariant mass");
  
  h_testInvMass->Write();
    
    
    
    
    
  // Safely close file
  outFile->Close();

  auto timerStop = std::chrono::high_resolution_clock::now();
  auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

  std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;

}


// Function to convert pdgId numerical code into a Latex or unicode string!
std::string pdgIdCode (const Int_t parId, const bool unicode) {

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

  default : {particle += std::to_string(std::abs(parId)); /*std::cout << "UNKNOWN PID: " << parId << std::endl;*/}
  }

  if ( unicode && (parId == 211 || parId == 213 || parId == 321 || parId == 323 || parId == 411 || parId == 431 || parId == 433 || parId == 521 || parId == 4122 || parId == 20213 || parId == 4214 || parId == 523
           || parId == 3224 || parId == 3222
           ))  particle += "+";

  if ( unicode && parId < 0) particle += "-";
  if ( unicode && (parId == -4222 || parId == -4224) ) particle += "-";
  if ( unicode && (parId == 4222  || parId == 4222) ) particle += "+";

  return particle;
}


bool scalarGrandparent (const AnalysisEvent& event, const Int_t& k, const Int_t& grandparentId) {

  const Int_t pdgId        { std::abs(event.genParId[k]) };
  const Int_t numDaughters { event.genParNumDaughters[k] };
  const Int_t motherId     { std::abs(event.genParMotherId[k]) };
  const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };


  if (motherId == 0 || motherIndex == -1) return false; // if no parent, then mother Id is null and there's no index, quit search
  else if (motherId == std::abs(grandparentId)) return true; // if mother is granparent being searched for, return true
  else if (motherIndex >= event.NGENPARMAX) return false; // index exceeds stored genParticle range, return false for safety
  else {
    //   std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
    //   debugCounter++;
    //   std::cout << "debugCounter: " << debugCounter << std::endl;
    return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
  }
}
