#include "AnalysisEvent.hpp"
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
  bool usePostLepTree {false};

  std::map<int, int> pdgIdMap; // declare map of int, int - first int corresponds to pdgId, second will note how many times a particle with that pdgId has been found
  std::string outFileString{"plots/distributions/output.root"}; //
  const bool is2016_ {false}; // analysis framework is setup to run over multiple years - as we are considering 2017 conditions currently, this is set to false for safety.
  const bool is2018_ {false}; // analysis framework is setup to run over multiple years - as we are considering 2017 conditions currently, this is set to false for safety.
  Long64_t nEvents; // Max number of events to consider per dataset. Default is set in config file, but can be overriden with command line arguements
  Long64_t totalEvents {0}; // Counter for total number of events
  std::string era {""};
  if (is2016_) era = {"2016"};
  else if (is2018_) era = {"2018"};
  else era = {"2017"};
  const std::string postLepSelSkimInputDir{std::string{"/pnfs/iihe/cms/store/user/almorton/MC/postLepSkims/postLepSkims"} + era + "/"};

	
	
	
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
  TH1F* h_genParScalarMuonPtL         {new TH1F("h_genParScalarMuonPtL",  "#mu^{#pm} from scalar decay p_{T}, leading", 1000, 0., 1000.)};
  TH1F* h_genParScalarMuonPtS         {new TH1F("h_genParScalarMuonPtS",  "#mu^{#pm} from scalar decay p_{T}, subleading", 1000, 0., 1000.)};
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
  TH2F* h_VertexPosXY  {new TH2F("h_VertexPosXY", "Vertex Position XY", 100, -150,150,100,-150,150)};
  TH2F* h_VertexPosRZ  {new TH2F("h_VertexPosRZ", "Vertex Position RZ", 100, 0,20,100,0,250)};
    
  
	
  //PAT Muon reconstruction
  TH1F* h_muonRecPt            {new TH1F("h_muonRecPt",  "#mu^{#pm} reconstruction p_{T}", 1000, 0., 200.)};
  TH1F* h_muonRecPtL           {new TH1F("h_muonRecPtL",  "#mu^{#pm} reconstruction p_{T}, leading", 1000, 0., 200.)};
  TH1F* h_muonRecPtS           {new TH1F("h_muonRecPtS",  "#mu^{#pm} reconstruction p_{T}, subleading", 1000, 0., 200.)};
  TH1F* h_muonRecEta           {new TH1F("h_muonRecEta", "#mu^{#pm} reconstruction #eta",  200, -7., 7.)};
  TH1F* h_muonRecPhi           {new TH1F("h_muonRecPhi", "#mu^{#pm} reconstruction #phi",  100, -3.5, 3.5)};
  TH1F* h_muonRecE             {new TH1F("h_muonRecE",   "#mu^{#pm} reconstruction energy",     1000, 0., 1000.)};
  TH1F* h_muonRecDeltaR        {new TH1F("h_muonRecDeltaR", "Muon reconstruction #DeltaR",2500, -10., 10.)};
  TH1F* h_muonRecDeltaPhi      {new TH1F("h_muonRecDeltaPhi", "Muon reconstruction #Delta#phi",2500, -3.5, 3.5)};
  TH1F* h_muonRecInvMass       {new TH1F("h_muonRecInvMass", "Muon reconstruction invariant mass",500, 0, 5)};
  TF1* Gaussian1 	       {new TF1("Gaussian1","gaus",1.,3.)};
  TH1F* h_muonCutSingleL       {new TH1F("h_muonCutSingleL",  "Single #mu^{#pm} trigger leading p_{T}", 1000, 0., 200.)};
  TH1F* h_muonCutDoubleL       {new TH1F("h_muonCutDoubleL",  "Double #mu^{#pm} trigger leading p_{T}", 1000, 0., 200.)};
  TH1F* h_muonDivSingleL       {new TH1F("h_muonDivSingleL",  "Turn-on Single #mu^{#pm} trigger leading p_{T}", 300, 0., 200.)};
  TH1F* h_muonDivDoubleL       {new TH1F("h_muonDivDoubleL",  "Turn-on Double #mu^{#pm} trigger leading p_{T}", 300, 0., 200.)};
  TH1F* h_muonCutDoubleS       {new TH1F("h_muonCutDoubleS",  "Double #mu^{#pm} trigger subleading p_{T}", 1000, 0., 200.)};
  TH1F* h_muonDivDoubleS       {new TH1F("h_muonDivDoubleS",  "Turn-on Double #mu^{#pm} trigger subleading p_{T}", 300, 0., 200.)};
   	
	
  //Packed candidates
  TH1F* h_packedCDxy    {new TH1F("h_packedCDxy", "Packed Candidate Dxy", 500,  -200., 200.)};
  TH1F* h_packedCDz     {new TH1F("h_packedCDz",  "Packed Candidate Dz", 1500, -500., 500.)};
  TH1F* h_packedCVx     {new TH1F("h_packedCVx",  "Packed Candidate track vx", 500,  -150., 150.)};
  TH1F* h_packedCVy     {new TH1F("h_packedCVy",  "Packed Candidate track vy", 500,  -150., 150.)};
  TH1F* h_packedCVz     {new TH1F("h_packedCVz",  "Packed Candidate track vz", 1500, -500., 500.)};
  TH2I* h_displacedXY   {new TH2I("h_displacedXY", "Displacement XY", 100, -150,150,100,-150,150)};
  TH2I* h_displacedRZ   {new TH2I("h_displacedRZ", "Displacement RZ", 100, 0,20,100,0,250)};
	
  TH2F* h_HVertexPosXY         {new TH2F("h_HVertexPosXY", "Pion track vertex XY", 500, -50,50,500,-50,50)};
  TH2F* h_HVertexPosRZ         {new TH2F("h_HVertexPosRZ", "Pion track vertex RZ", 500, -50,50,500,-50,50)};	
	
  //Kaon mass assumption	
  TH1F* h_KhadronDeltaR  {new TH1F("h_KhadronDeltaR", "Two hadrons (kaons) #DeltaR",2500, -10., 10.)};
  TH1F* h_KmuonsDeltaR   {new TH1F("h_KmuonsDeltaR", "Two muons #DeltaR",2500, -10., 10.)};
  TH1F* h_KIsoSum1       {new TH1F("h_KIsoSum1",  "0.3 p_{T} Cone construction kaon 1", 1000, 0., 50.)};
  TH1F* h_KIsoSum2       {new TH1F("h_KIsoSum2",  "0.3 p_{T} Cone construction kaon 2", 1000, 0., 50.)};
  TH2F* h_KIso2          {new TH2F("h_KIso2", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.)};
  TH1F* h_KIsoSum3       {new TH1F("h_KIsoSum3",  "0.3 p_{T} Cone construction muon 1", 1000, 0., 50.)};
  TH1F* h_KIsoSum4       {new TH1F("h_KIsoSum4",  "0.3 p_{T} Cone construction muon 2", 1000, 0., 50.)};
  TH1F* h_KhadronInvMass {new TH1F("h_KhadronInvMass", "Two hadrons (kaons) - Invariant mass",1000, 0., 7.)};
  TH1F* h_KhadronInvMass2 {new TH1F("h_KhadronInvMass2", "Two hadrons (kaons) - Invariant mass, smaller binning",500, 0., 7.)};
  TH1F* h_KmuonsInvMass  {new TH1F("h_KmuonsInvMass", "Two muons - Invariant mass",1000, 0., 7.)};
  TH2F* h_Kinvmass       {new TH2F("h_Kinvmass", "Invariant mass: charged hadrons (kaons) vs muons", 1000, 0.,7.,1000,0.,7.)};
  
  TH1F* h_KantiscalarInvMass        {new TH1F("h_KantiscalarInvMass", "(Kaon) Antiscalar Invariant mass", 1000, 0.,15.)};
  TH1F* h_KscalarInvMass            {new TH1F("h_KscalarInvMass", "Scalar Invariant mass", 1000, 0.,15.)};
  TH1F* h_KhiggsInvMass             {new TH1F("h_KhiggsInvMass",  "h_0 Invariant mass", 1000, 0., 200.)};   
  TH1F* h_KhiggsDeltaR              {new TH1F("h_KhiggsDeltaR", "Scalar-Antiscalar #DeltaR",2500, 0., 15.)};  

	
	
  //Pion mass assumption
  TH1F* h_PhadronDeltaR  {new TH1F("h_PhadronDeltaR", "Two hadrons (pions) #DeltaR",2500, -10., 10.)};
  TH1F* h_PmuonsDeltaR   {new TH1F("h_PmuonsDeltaR", "Two muons #DeltaR",2500, -10., 10.)};
  TH1F* h_PIsoSum1       {new TH1F("h_PIsoSum1",  "Leading pion relative isolation", 1000, 0., 50.)};
  TH1F* h_PIsoSum2       {new TH1F("h_PIsoSum2",  "Subleading pion relative isolation", 1000, 0., 50.)};
  TH2F* h_PIso2          {new TH2F("h_PIso2", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.)};
  TH1F* h_PIsoSum3       {new TH1F("h_PIsoSum3",  "Leading muon relative isolation", 1000, 0., 50.)};
  TH1F* h_PIsoSum4       {new TH1F("h_PIsoSum4",  "Subleading muon relative isolation", 1000, 0., 50.)};
  TH2F* h_PIso4          {new TH2F("h_PIso4", "Relative isolatium sum vs. particle momentum", 1000, 0.,50.,1000,0.,500.)};
  TH1F* h_PhadronInvMass {new TH1F("h_PhadronInvMass", "Dihadron (pion) invariant mass",1000, 0., 7.)};
  TH1F* h_PhadronInvMass2 {new TH1F("h_PhadronInvMass2", "Two hadrons (pions) - Invariant mass, smaller binning",500, 0., 7.)};
  TH1F* h_PmuonsInvMass  {new TH1F("h_PmuonsInvMass", "Dimuon invariant mass",1000, 0., 7.)};
  TF1* Gaussian2 	 {new TF1("Gaussian2","gaus",1.,3.)};
  TH2F* h_Pinvmass       {new TH2F("h_Pinvmass", "Invariant mass: charged hadrons (pions) vs muons", 1000, 0.,7.,1000,0.,7.)};
  
  TH1F* h_PantiscalarInvMass        {new TH1F("h_PantiscalarInvMass", "Dihadron (pion) invariant mass", 1000, 0.,15.)};
  TH1F* h_PscalarInvMass            {new TH1F("h_PscalarInvMass", "Dimuon invariant mass", 1000, 0.,15.)};
  TF1* Gaussian3 	 	    {new TF1("Gaussian3","gaus",1.,3.)};
  TH1F* h_PhiggsInvMass             {new TH1F("h_PhiggsInvMass",  "Higgs invariant mass", 1000, 0., 200.)};   
  TH1F* h_PhiggsDeltaR              {new TH1F("h_PhiggsDeltaR", "Discalar #DeltaR",2500, 0., 15.)}; 	
  TH1F* h_Rrefit12InvMass           {new TH1F("h_Rrefit12InvMass", "Dimuon refitted invariant mass with requirements", 500, 0.,5.)};
  TH1F* h_Rpionre12InvMass          {new TH1F("h_Rpionre12InvMass", "Dihadron (pion) refit invariant mass", 500, 0.,5.)};

  TH1F* h_P20antiscalarInvMass        {new TH1F("h_P20antiscalarInvMass", "Dihadron (pion) invariant mass", 1000, 0.,15.)};
  TH1F* h_P20scalarInvMass            {new TH1F("h_P20scalarInvMass", "Dimuon invariant mass", 1000, 0.,15.)};
  TH1F* h_P20higgsInvMass             {new TH1F("h_P20higgsInvMass",  "Higgs invariant mass", 1000, 0., 200.)};   
	
  TH2F* h_massassump       {new TH2F("h_massassump", "Invariant mass: charged hadrons (pions) vs charged hadrons (kaons)", 1000, 0.,7.,1000,0.,7.)};
  TH2F* h_pmassassump       {new TH2F("h_pmassassump", "Invariant mass: charged hadrons (pions) vs charged hadrons (kaons)", 1000, 0.,7.,1000,0.,7.)};
  TH2F* h_higgsassump      {new TH2F("h_higgsassump", "Invariant mass: h_0 (pions-muons) vs h_0 (kaons-muons)", 1000, 0.,200.,1000,0.,200.)};
	
	
  //Comparison muon momenta
	
  //PAT muons
  TH1F* h_muoniRecPtTrk                {new TH1F("h_muoniRecPtTrk",  "#mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.)};
  TH1F* h_muon1RecPtTrk                {new TH1F("h_muon1RecPtTrk",  "Leading #mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.)};
  TH1F* h_muon2RecPtTrk                {new TH1F("h_muon2RecPtTrk",  "Subleading #mu^{#pm} reconstruction p_{T} track", 1000, 0., 200.)};
  TH1F* h_muoniRecPt                   {new TH1F("h_muoniRecPt",  "#mu^{#pm} reconstruction p_{T}", 1000, 0., 200.)};
  TH1F* h_muon1RecPt                   {new TH1F("h_muon1RecPt",  "Leading #mu^{#pm} reconstruction p_{T}", 1000, 0., 200.)};
  TH1F* h_muon2RecPt                   {new TH1F("h_muon2RecPt",  "Subleading #mu^{#pm} reconstruction p_{T}", 1000, 0., 200.)};
  TH1F* h_muon1RecInvMass              {new TH1F("h_muon1RecInvMass", "Leading reconstruction invariant mass",500, 0, 100)};
  TH1F* h_muon2RecInvMass              {new TH1F("h_muon2RecInvMass", "Subleading reconstruction invariant mass",500, 0, 100)};
  TH1F* h_muon12RecInvMass             {new TH1F("h_muon12RecInvMass", "Scalar reconstruction invariant mass",500, 0, 100)};
	
  //Packed muons
  TH1F* h_muonipackedPt                {new TH1F("h_muonipackedPt",  "#mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.)};
  TH1F* h_muonipackedInvMass           {new TH1F("h_muonipackedInvMass", "#mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.)};
  TH1F* h_muonipackedPtTrk             {new TH1F("h_muonipackedPtTrk",  "#mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.)};
  TH1F* h_muon1packedPt                {new TH1F("h_muon1packedPt",  "Leading #mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.)};
  TH1F* h_muon1packedInvMass           {new TH1F("h_muon1packedInvMass", "Leading #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.)};
  TH1F* h_muon1packedPtTrk             {new TH1F("h_muon1packedPtTrk",  "Leading #mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.)};
  TH1F* h_muon2packedPt                {new TH1F("h_muon2packedPt",  "Subleading #mu^{#pm} Packed candidate p_{T}", 1000, 0., 1000.)};
  TH1F* h_muon2packedInvMass           {new TH1F("h_muon2packedInvMass", "Subleading #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.)};
  TH1F* h_muon2packedPtTrk             {new TH1F("h_muon2packedPtTrk",  "Subleading #mu^{#pm} Packed candidate p_{T} track", 1000, 0., 1000.)};	
  TH1F* h_muon12packedInvMass          {new TH1F("h_muon12packedInvMass", "Scalar #mu^{#pm} Packed candidate Invariant mass", 500, 0.,5.)};
	
  //Refitted tracks muons
  TH1F*	h_muon1PairsPt		      {new TH1F("h_muon1PairsPt",  "#mu^{#pm} 1 Refitted tracks p_{T} m", 1000, 0., 1000.)};
  TH1F*	h_muon2PairsPt 		      {new TH1F("h_muon2PairsPt",  "#mu^{#pm} 2 Refitted tracks p_{T}", 1000, 0., 1000.)};
  TH1F* h_muon1refitInvMass           {new TH1F("h_muon1refitInvMass", "Leading #mu^{#pm} Refit Invariant mass", 500, 0.,5.)};
  TH1F* h_muon2refitInvMass           {new TH1F("h_muon2refitInvMass", "Subleading #mu^{#pm} Refit Invariant mass", 500, 0.,5.)};
  TH1F* h_muon12refitInvMass          {new TH1F("h_muon12refitInvMass", "Scalar Refit Invariant mass", 500, 0.,5.)};
  TH1F* h_refit1InvMass               {new TH1F("h_refit1InvMass", "Leading #mu^{#pm} Refit Invariant mass", 500, 0.,5.)};
  TH1F* h_refit2InvMass               {new TH1F("h_refit2InvMass", "Subleading #mu^{#pm} Refit Invariant mass", 500, 0.,5.)};
  TH1F* h_refit12InvMass              {new TH1F("h_refit12InvMass", "Dimuon refitted invariant mass", 500, 0.,5.)};
  
  //Refitted tracks pions
  TH1F* h_pionre12InvMass              {new TH1F("h_pionre12InvMass", "Dihadron (pion) refit invariant mass", 500, 0.,5.)};
  /*TH2F* h_muonPairsXY                 {new TH2F("h_muonPairsXY", "Refitted tracks vertex XY", 100, -150,150,100,-150,150)};
  TH2F* h_muonPairsRZ                 {new TH2F("h_muonPairsRZ", "Refitted tracks vertex RZ", 100, 0,20,100,0,250)};		
  */	  



	
	
	
	
	  
  namespace po = boost::program_options;


  // command line configuration parsing magic!
  po::options_description desc("Options");
  desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "lumi,l",
        //po::value<double>(&usePreLumi)->default_value(4247.682053046),
	po::value<double>(&usePreLumi)->default_value(41528.0),
        "Lumi to scale MC plots to.")(
        "outfile,o",
        po::value<std::string>(&outFileString)->default_value(outFileString),
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


  // Begin to loop over all datasets
  for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
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
          std::string inputPostfix{"mumu"};
          std::cout << postLepSelSkimInputDir + dataset->name() + inputPostfix + "SmallSkim.root"  << std::endl;
          datasetChain->Add((postLepSelSkimInputDir + dataset->name() + inputPostfix + "SmallSkim.root").c_str());
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
    Float_t gen1=0; Float_t gen2=0;
    //std::cout << "idx\t | ID\t stat\t | Mo\t Da1\t Da2\t | pt\t eta\t phi\t m" << std::endl;
         
       for (Int_t k{0}; k < event.nGenPar; k++) {
     
          //Print out event record

          //Invariant mass
          /*TLorentzVector m;
          m.SetPtEtaPhiE(event.genParPt[k],event.genParEta[k],event.genParPhi[k],event.genParE[k]);
          TLorentzVector mass {m.Px(),m.Py(),m.Pz(),event.genParE[k]};

          std::cout << k << "\t | "
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
      const Float_t genParVx     {event.genParVx[k]};
      const Float_t genParVy     {event.genParVy[k]};
      const Float_t genParVz     {event.genParVz[k]};
            
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
            h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
            h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
       
            if(event.genParPt[k]>gen1){
                gen2=gen1;
                gen1=event.genParPt[k];
              }
            else if(event.genParPt[k]>gen2){
                     gen2=event.genParPt[k];
            }
           
            if(event.muTrig() || event.mumuTrig()){
              if(event.genParPt[k]>genpt1){
                genpt2=genpt1;
                genpt1=event.genParPt[k];
              }
              else if(event.genParPt[k]>genpt2){
                     genpt2=event.genParPt[k];
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
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
          h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
        }
        //K short from scalar decay
        if (pdgId==310){
          nrofKShort.emplace_back(k); //Together with kaon in angular differences
          h_genParScalarKShortPt->Fill(event.genParPt[k]);
          h_genParScalarKShortEta->Fill(genParEta);
          h_genParScalarKShortPhi->Fill(genParPhi);
          h_genParScalarKShortE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
              h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
        }
        //Charged pion from scalar decay
        if (pdgId==211){
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
        if (pdgId==111){
          nrofPion.emplace_back(k);
          h_genParScalarNPionPt->Fill(event.genParPt[k]);
          h_genParScalarNPionEta->Fill(genParEta);
          h_genParScalarNPionPhi->Fill(genParPhi);
          h_genParScalarNPionE->Fill(genParE);
          h_VertexPosXY->Fill(genParVx,genParVy);
          h_VertexPosRZ->Fill(std::abs(genParVz),std::sqrt(genParVx*genParVx+genParVy*genParVy));
          h_VertexPosR->Fill(std::sqrt(genParVx*genParVx+genParVy*genParVy));
        }
      }

      // Increment counter for pdgId found
      pdgIdMap[pdgId]++;
            
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
    if(gen1!=0 && gen2!=0){
       h_genParScalarMuonPtL->Fill(gen1); 
       h_genParScalarMuonPtS->Fill(gen2);
    }  
 
    
    h_genParScalarMuonDivPtSL=(TH1F*)h_genParScalarMuonCutPtSL->Clone();
    h_genParScalarMuonDivPtSL->Divide(h_genParScalarMuonPtL);
    h_genParScalarMuonDivPtSL->SetTitle("Turn-on Single trigger leading");
    
    h_genParScalarMuonDivPtDL=(TH1F*)h_genParScalarMuonCutPtDL->Clone();
    h_genParScalarMuonDivPtDL->Divide(h_genParScalarMuonPtL);
    h_genParScalarMuonDivPtDL->SetTitle("Turn-on Double trigger leading");
    
    h_genParScalarMuonDivPtDS=(TH1F*)h_genParScalarMuonCutPtDS->Clone();
    h_genParScalarMuonDivPtDS->Divide(h_genParScalarMuonPtS);
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
          
    uint leadingFlag{0}; std::vector<Int_t> leadingIndex{}; 
      
    if(event.metFilters()){
      
       for (Int_t k{0}; k < event.numMuonPF2PAT; k++) {
             
           const Float_t muonRecPt   { event.muonPF2PATPt[k] };
           const Float_t muonRecEta  { event.muonPF2PATEta[k] };
           const Float_t muonRecPhi  { event.muonPF2PATPhi[k] };
           const Float_t muonRecE    { event.muonPF2PATE[k] };
           
           h_muonRecPt->Fill(muonRecPt, datasetWeight);
	   
	   
	   if(event.muonPF2PATLooseCutId[k]==1 && std::abs(event.muonPF2PATEta[k])<2.4){ 
             leadingFlag++; leadingIndex.emplace_back(k);
	   }
	      
           h_muonRecEta->Fill(muonRecEta, datasetWeight);
           h_muonRecPhi->Fill(muonRecPhi, datasetWeight);
           h_muonRecE->Fill(muonRecE, datasetWeight);
           
           if(event.muTrig() || event.mumuTrig()){
               
             if(event.muonPF2PATCharge[0]==-(event.muonPF2PATCharge[1])){ //Electric charge control
        
               TLorentzVector muonRec1;
               TLorentzVector muonRec2;
          
               muonRec1.SetPtEtaPhiE(event.muonPF2PATPt[0],event.muonPF2PATEta[0],event.muonPF2PATPhi[0],event.muonPF2PATE[0]);
               muonRec2.SetPtEtaPhiE(event.muonPF2PATPt[1],event.muonPF2PATEta[1],event.muonPF2PATPhi[1],event.muonPF2PATE[1]);
            
               h_muonRecDeltaR->Fill(muonRec1.DeltaR(muonRec2), datasetWeight);
               h_muonRecDeltaPhi->Fill(muonRec1.DeltaPhi(muonRec2), datasetWeight);
            
               //Invariant mass for two highest p_T
               TLorentzVector lVecMu1  {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
               TLorentzVector lVecMu2  {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]};

               h_muonRecInvMass->Fill( (lVecMu1+lVecMu2).M(), datasetWeight );
               //h_muonRecInvMass->Fit(Gaussian1);
		
		      
		     
               if(event.muonPF2PATPt[0]>30 && event.muonPF2PATPt[1]>12){//combined (single+double, mix) p_T cut applied
             
                 if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4){//Loose ID cut and |eta| < 2.4
                   passedMuons.emplace_back(k);//Take its index
                 }
               }
             }
           }
        
        
           //To show seperate turn-on curve for single or double muon trigger
           if(event.muTrig() ){
             if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4){ //Loose ID cut, |eta| < 2.4 
               singleFlag++; singleIndex.emplace_back(k);
             }
           }
           
	   
	       
           if(event.mumuTrig()){
             if(event.muonPF2PATLooseCutId[k]==1 && std::abs(muonRecEta)<2.4){//Loose ID cut, |eta| < 2.4 
               doubleFlag++; doubleIndex.emplace_back(k);
             } 
           }
           
	   
               
       }
       if(leadingFlag>0){
	 h_muonRecPtL->Fill(event.muonPF2PATPt[leadingIndex[0]], datasetWeight);      
       }
	       
       if(leadingFlag>1){ 
	 h_muonRecPtS->Fill(event.muonPF2PATPt[leadingIndex[1]], datasetWeight); 
       }
       if(singleFlag>0){
         h_muonCutSingleL->Fill(event.muonPF2PATPt[singleIndex[0]], datasetWeight); 
       }
       if(doubleFlag>1){
         h_muonCutDoubleL->Fill(event.muonPF2PATPt[doubleIndex[0]], datasetWeight);
         h_muonCutDoubleS->Fill(event.muonPF2PATPt[doubleIndex[1]], datasetWeight);
       }
           
    }//MET filter
    
    h_muonDivSingleL=(TH1F*)h_muonCutSingleL->Clone();
    h_muonDivSingleL->Divide(h_muonRecPtL);
    h_muonDivSingleL->SetTitle("Turn-on Single trigger, leading");
          
    h_muonDivDoubleL=(TH1F*)h_muonCutDoubleL->Clone();
    h_muonDivDoubleL->Divide(h_muonRecPtL);
    h_muonDivDoubleL->SetTitle("Turn-on Double trigger, leading");
          
    h_muonDivDoubleS=(TH1F*)h_muonCutDoubleS->Clone();
    h_muonDivDoubleS->Divide(h_muonRecPtS);
    h_muonDivDoubleS->SetTitle("Turn-on Double trigger, subleading");

    //END Muon Reconstruction
          
    
          
          
    //BEGIN Packed candidates
    Int_t pionIndex1{-1}; Int_t pionIndex2{-1};
    Int_t muIndex1{-1}; Int_t muIndex2{-1};
     
    Float_t pionpt1{-1}; Float_t pionpt2{-1};
    Float_t mupt1{-1}; Float_t mupt2{-1};
       
    if(event.metFilters()){
        
      for (Int_t k{0};k<event.numPackedCands;k++) {
          
          if(event.muTrig() || event.mumuTrig()){
            
            const Int_t packedId {event.packedCandsPdgId[k]};
        
            h_packedCDxy->Fill(event.packedCandsDxy[k], datasetWeight);
            h_packedCDz->Fill(event.packedCandsDz[k], datasetWeight);
          
            if(event.packedCandsHasTrackDetails[k]==1){
            
              const Int_t packedCandsPseudoTrkCharge {event.packedCandsPseudoTrkCharge[k]};
              const Int_t packedCandsCharge {event.packedCandsCharge[k]};
            
              if(packedCandsCharge!=0 && packedCandsCharge==packedCandsPseudoTrkCharge){ //No neutral particles as they don't leave tracks, only charged
               
                h_packedCVx->Fill(event.packedCandsPseudoTrkVx[k], datasetWeight);
                h_packedCVy->Fill(event.packedCandsPseudoTrkVy[k], datasetWeight);
                h_packedCVz->Fill(event.packedCandsPseudoTrkVz[k], datasetWeight);
              
                //Displacement from interaction point
                h_displacedXY->Fill(event.packedCandsPseudoTrkVx[k],event.packedCandsPseudoTrkVy[k], datasetWeight);
                h_displacedRZ->Fill(std::abs(event.packedCandsPseudoTrkVz[k]),std::sqrt(event.packedCandsPseudoTrkVx[k]*event.packedCandsPseudoTrkVx[k]+event.packedCandsPseudoTrkVy[k]*event.packedCandsPseudoTrkVy[k]), datasetWeight);
              
		      
		//Find the hadrons (pions)
                if(std::abs(packedId)==211 && event.packedCandsPseudoTrkPt[k]>5){//Selection of pions (charged hadrons) 
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
                }
     
                if(std::abs(packedId)==13 && event.packedCandsPseudoTrkPt[k]>5){//Selection of muons
                 if(event.packedCandsPseudoTrkPt[k]>mupt1){
                    mupt2=mupt1;
                    mupt1=event.packedCandsPseudoTrkPt[k];
                    muIndex2=muIndex1;
                    muIndex1=k;
                  }
                  else if(event.packedCandsPseudoTrkPt[k]>mupt2){
                         mupt2=event.packedCandsPseudoTrkPt[k];
                         muIndex2=k;
                  }
                }
              }
            }
          }
        
	      
      }
    }
    
/*
    Float_t KIsoSum1=0;  Float_t KIsoSum2=0;
    Float_t KIsoSum3=0;  Float_t KIsoSum4=0;
   
    Float_t Khadroninv; Float_t Kmuoninv;

    Float_t Kpx=0; Float_t Kpy=0; Float_t Kpz=0; Float_t KE=0;
    Float_t KMpx=0; Float_t KMpy=0; Float_t KMpz=0; Float_t KME=0;
	      
    TLorentzVector mm1; TLorentzVector mm2;
    TLorentzVector packed1; TLorentzVector packed2; 
	      
    TLorentzVector Kscalar; TLorentzVector Kantiscalar;
	      
    Float_t Khiggs=0;
	
    //Kaon mass assumption
    if(event.metFilters()){
      if(event.muTrig()||event.mumuTrig()){ 
	 
	 if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsCharge[pionIndex1]==-(event.packedCandsCharge[pionIndex2])){
           
	   packed1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex1],event.packedCandsPseudoTrkEta[pionIndex1],event.packedCandsPseudoTrkPhi[pionIndex1],std::sqrt(event.packedCandsE[pionIndex1]*event.packedCandsE[pionIndex1]-std::pow(0.1396,2)+std::pow(0.494,2)));
           packed2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkEta[pionIndex2],event.packedCandsPseudoTrkPhi[pionIndex2],std::sqrt(event.packedCandsE[pionIndex2]*event.packedCandsE[pionIndex2]-std::pow(0.1396,2)+std::pow(0.494,2)));
            
           h_KhadronDeltaR->Fill(packed1.DeltaR(packed2), datasetWeight);
   
	   
		 
	   if(packed1.DeltaR(packed2)<0.2){
             //Invariant mass for two hadrons
             TLorentzVector lhadron1  {event.packedCandsPseudoTrkPx[pionIndex1], event.packedCandsPseudoTrkPy[pionIndex1], event.packedCandsPseudoTrkPz[pionIndex1], std::sqrt(event.packedCandsE[pionIndex1]*event.packedCandsE[pionIndex1]-std::pow(0.1396,2)+std::pow(0.494,2))};
             TLorentzVector lhadron2  {event.packedCandsPseudoTrkPx[pionIndex2], event.packedCandsPseudoTrkPy[pionIndex2], event.packedCandsPseudoTrkPz[pionIndex2], std::sqrt(event.packedCandsE[pionIndex2]*event.packedCandsE[pionIndex2]-std::pow(0.1396,2)+std::pow(0.494,2))};

             Khadroninv=(lhadron1+lhadron2).M();
             h_KhadronInvMass->Fill((lhadron1+lhadron2).M(), datasetWeight);
             h_KhadronInvMass2->Fill((lhadron1+lhadron2).M(), datasetWeight);
		
	     //Vector addition
	     Kpx=event.packedCandsPseudoTrkPx[pionIndex1]+event.packedCandsPseudoTrkPx[pionIndex2];
             Kpy=event.packedCandsPseudoTrkPy[pionIndex1]+event.packedCandsPseudoTrkPy[pionIndex2];
             Kpz=event.packedCandsPseudoTrkPz[pionIndex1]+event.packedCandsPseudoTrkPz[pionIndex2];
             KE=std::sqrt(event.packedCandsE[pionIndex1]*event.packedCandsE[pionIndex1]-std::pow(0.1396,2)+std::pow(0.494,2))+std::sqrt(event.packedCandsE[pionIndex2]*event.packedCandsE[pionIndex2]-std::pow(0.1396,2)+std::pow(0.494,2));
		
	     Kantiscalar.SetPxPyPzE(Kpx,Kpy,Kpz,KE);
	     h_KantiscalarInvMass->Fill(Kantiscalar.M(), datasetWeight);
	   }
		 
	   for(Int_t k{0};k<event.numPackedCands;k++) {
              const Int_t packedId {event.packedCandsPdgId[k]};
              if(k!=pionIndex1 && k!=pionIndex2){
                
                TLorentzVector cone1;//The pion
                TLorentzVector cone2;//Packed candidate
                    
                cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex1],event.packedCandsPseudoTrkEta[pionIndex1],event.packedCandsPseudoTrkPhi[pionIndex1],std::sqrt(event.packedCandsE[pionIndex1]*event.packedCandsE[pionIndex1]-std::pow(0.1396,2)+std::pow(0.494,2)));
                cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
		 
		       
                if(cone1.DeltaR(cone2)<0.3){
                  KIsoSum1+=event.packedCandsPseudoTrkPt[k];
		}
            
                TLorentzVector cone3;//The other pion
                TLorentzVector cone4;//Packed candidate
                    
                cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkEta[pionIndex2],event.packedCandsPseudoTrkPhi[pionIndex2],std::sqrt(event.packedCandsE[pionIndex2]*event.packedCandsE[pionIndex2]-std::pow(0.1396,2)+std::pow(0.494,2)));
                cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

		       
                if(cone3.DeltaR(cone4)<0.3){
                  KIsoSum2+=event.packedCandsPseudoTrkPt[k];
		}
	      }
		   
	   }//end of for-loop
	   
	 }
	      
	 if(muIndex1!=-1 && muIndex2!=-1 && event.packedCandsPseudoTrkPt[muIndex1]!=0 && event.packedCandsPseudoTrkPt[muIndex2]!=0 && event.packedCandsCharge[muIndex1]==-(event.packedCandsCharge[muIndex2])){
	   
	   mm1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex1],event.packedCandsPseudoTrkEta[muIndex1],event.packedCandsPseudoTrkPhi[muIndex1],event.packedCandsE[muIndex1]);
           mm2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex2],event.packedCandsPseudoTrkEta[muIndex2],event.packedCandsPseudoTrkPhi[muIndex2],event.packedCandsE[muIndex2]);
            
           h_KmuonsDeltaR->Fill(mm1.DeltaR(mm2), datasetWeight);
            
           if(mm1.DeltaR(mm2)<0.2){
		   
	     TLorentzVector lmuon1  {event.packedCandsPseudoTrkPx[muIndex1], event.packedCandsPseudoTrkPy[muIndex1], event.packedCandsPseudoTrkPz[muIndex1], event.packedCandsE[muIndex1]};
             TLorentzVector lmuon2  {event.packedCandsPseudoTrkPx[muIndex2], event.packedCandsPseudoTrkPy[muIndex2], event.packedCandsPseudoTrkPz[muIndex2], event.packedCandsE[muIndex2]};
                        
             Kmuoninv=(lmuon1+lmuon2).M();
             h_KmuonsInvMass->Fill((lmuon1+lmuon2).M(), datasetWeight);
		   
	     KMpx=event.packedCandsPseudoTrkPx[muIndex1]+event.packedCandsPseudoTrkPx[muIndex2];
             KMpy=event.packedCandsPseudoTrkPy[muIndex1]+event.packedCandsPseudoTrkPy[muIndex2];
             KMpz=event.packedCandsPseudoTrkPz[muIndex1]+event.packedCandsPseudoTrkPz[muIndex2];
	     KME=event.packedCandsE[muIndex1]+event.packedCandsE[muIndex2];
	      
	     Kscalar.SetPxPyPzE(KMpx,KMpy,KMpz,KME);
	     h_KscalarInvMass->Fill(Kscalar.M(), datasetWeight);   
		   
	   }
		 
	   for(Int_t k{0};k<event.numPackedCands;k++) {
              const Int_t packedId {event.packedCandsPdgId[k]};
              if(k!=muIndex1 && k!=muIndex2){
                  
                TLorentzVector cone1;//The muon
                TLorentzVector cone2;//Packed candidate

                cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex1],event.packedCandsPseudoTrkEta[muIndex1],event.packedCandsPseudoTrkPhi[muIndex1],event.packedCandsE[muIndex1]);
                cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
          
                if(cone1.DeltaR(cone2)<0.3){
                  KIsoSum3+=event.packedCandsPseudoTrkPt[k];
		}
              
		TLorentzVector cone3;//The other muon
                TLorentzVector cone4;//Packed candidate

                cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex2],event.packedCandsPseudoTrkEta[muIndex2],event.packedCandsPseudoTrkPhi[muIndex2],event.packedCandsE[muIndex2]);
                cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

                if(cone3.DeltaR(cone4)<0.3){
                  KIsoSum4+=event.packedCandsPseudoTrkPt[k];
		}
	      }
	   }
	   if(std::abs((Kantiscalar+Kscalar).M()-125)<3){
	     if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsPseudoTrkCharge[pionIndex1]==-(event.packedCandsPseudoTrkCharge[pionIndex2])){
	       h_KIsoSum1->Fill(KIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1], datasetWeight);    
	       h_KIsoSum2->Fill(KIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2], datasetWeight);
	       h_KIso2->Fill(KIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkPt[pionIndex2], datasetWeight);
	     }
	     h_KIsoSum3->Fill(KIsoSum3/event.packedCandsPseudoTrkPt[muIndex1], datasetWeight);
             h_KIsoSum4->Fill(KIsoSum4/event.packedCandsPseudoTrkPt[muIndex2], datasetWeight);
	   }
	 }
	      
	 if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsPseudoTrkCharge[pionIndex1]==-(event.packedCandsPseudoTrkCharge[pionIndex2]) && muIndex1!=-1 && muIndex2!=-1 && event.packedCandsPseudoTrkPt[muIndex1]!=0 && event.packedCandsPseudoTrkPt[muIndex2]!=0 && event.packedCandsPseudoTrkCharge[muIndex1]==-(event.packedCandsPseudoTrkCharge[muIndex2])){
    
	   if(mm1.DeltaR(mm2)<0.2 && packed1.DeltaR(packed2)<0.2){  
	      Khiggs=(Kantiscalar+Kscalar).M();
	      h_KhiggsInvMass->Fill(Khiggs, datasetWeight);
	      h_KhiggsDeltaR->Fill(Kantiscalar.DeltaR(Kscalar), datasetWeight);
	      h_Kinvmass->Fill(Khadroninv,Kmuoninv, datasetWeight);
	   }
	    	 
	 }
	       
	      
      }
    }//end of met filter   
	      
	      
	*/	 
	
      
	     	      
	      
    Float_t PIsoSum1=0;  Float_t PIsoSum2=0;
    Float_t PIsoSum3=0;  Float_t PIsoSum4=0;
   
    Float_t Phadroninv; Float_t Pmuoninv;

    Float_t Ppx=0; Float_t Ppy=0; Float_t Ppz=0; Float_t PE=0;
    Float_t PMpx=0; Float_t PMpy=0; Float_t PMpz=0; Float_t PME=0;
	      
    TLorentzVector mm3; TLorentzVector mm4;
    TLorentzVector packed3; TLorentzVector packed4;  
	      
    TLorentzVector Pscalar; TLorentzVector Pantiscalar;
	      
    Float_t Phiggs=0;
	
    Int_t muonIndex1{-1}; Int_t muonIndex2{-1};
    Float_t muonpt1{-1}; Float_t muonpt2{-1};
	      
    //Pion mass assumption
    if(event.metFilters()){
      if(event.muTrig()||event.mumuTrig()){ 
	      
	 for(Int_t k{0}; k<event.numMuonPF2PAT;k++){ 
           if(event.muonPF2PATInnerTkPt[k]>muonpt1){
             muonpt2=muonpt1;
             muonpt1=event.muonPF2PATInnerTkPt[k];
             muonIndex2=muonIndex1;
             muonIndex1=k;
	   }
           else if(event.muonPF2PATInnerTkPt[k]>muonpt2){
                  muonpt2=event.muonPF2PATInnerTkPt[k];
                  muonIndex2=k;
	   }
	   h_muoniRecPtTrk->Fill(event.muonPF2PATInnerTkPt[k], datasetWeight);
	   h_muoniRecPt->Fill(event.muonPF2PATPt[k], datasetWeight);
	 }  
	      
	 if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsCharge[pionIndex1]==-(event.packedCandsCharge[pionIndex2])){
           
	   packed3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex1],event.packedCandsPseudoTrkEta[pionIndex1],event.packedCandsPseudoTrkPhi[pionIndex1],event.packedCandsE[pionIndex1]);
           packed4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkEta[pionIndex2],event.packedCandsPseudoTrkPhi[pionIndex2],event.packedCandsE[pionIndex2]);
            
           h_PhadronDeltaR->Fill(packed3.DeltaR(packed4), datasetWeight);
            
	   if(packed3.DeltaR(packed4)<0.2){
             //Invariant mass for two hadrons
             TLorentzVector lhadron1  {event.packedCandsPseudoTrkPx[pionIndex1], event.packedCandsPseudoTrkPy[pionIndex1], event.packedCandsPseudoTrkPz[pionIndex1],event.packedCandsE[pionIndex1]};
             TLorentzVector lhadron2  {event.packedCandsPseudoTrkPx[pionIndex2], event.packedCandsPseudoTrkPy[pionIndex2], event.packedCandsPseudoTrkPz[pionIndex2],event.packedCandsE[pionIndex2]};

             Phadroninv=(lhadron1+lhadron2).M();
             h_PhadronInvMass->Fill((lhadron1+lhadron2).M(), datasetWeight);
             h_PhadronInvMass2->Fill((lhadron1+lhadron2).M(), datasetWeight);
		
	     //Vector addition
	     Ppx=event.packedCandsPseudoTrkPx[pionIndex1]+event.packedCandsPseudoTrkPx[pionIndex2];
             Ppy=event.packedCandsPseudoTrkPy[pionIndex1]+event.packedCandsPseudoTrkPy[pionIndex2];
             Ppz=event.packedCandsPseudoTrkPz[pionIndex1]+event.packedCandsPseudoTrkPz[pionIndex2];
             PE=event.packedCandsE[pionIndex1]+event.packedCandsE[pionIndex2];
		
	     Pantiscalar.SetPxPyPzE(Ppx,Ppy,Ppz,PE);
	     
	   }
		
	   for(Int_t k{0};k<event.numPackedCands;k++) {
              const Int_t packedId {event.packedCandsPdgId[k]};
              if(k!=pionIndex1 && k!=pionIndex2){
                
                TLorentzVector cone1;//The pion
                TLorentzVector cone2;//Packed candidate
                    
                cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex1],event.packedCandsPseudoTrkEta[pionIndex1],event.packedCandsPseudoTrkPhi[pionIndex1],event.packedCandsE[pionIndex1]);
                cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
		 
		       
                if(cone1.DeltaR(cone2)<0.3){
                  PIsoSum1+=event.packedCandsPseudoTrkPt[k];
		}
            
                TLorentzVector cone3;//The other pion
                TLorentzVector cone4;//Packed candidate
                    
                cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkEta[pionIndex2],event.packedCandsPseudoTrkPhi[pionIndex2],event.packedCandsE[pionIndex2]);
                cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

		       
                if(cone3.DeltaR(cone4)<0.3){
                  PIsoSum2+=event.packedCandsPseudoTrkPt[k];
		}
	      }
		   
	   }//end of for-loop
	
	 }
	 
	 if(muonIndex1!=-1 && muonIndex2!=-1 && event.muonPF2PATInnerTkPt[muonIndex1]!=0 && event.muonPF2PATInnerTkPt[muonIndex2]!=0 && event.muonPF2PATCharge[muonIndex1]==-(event.muonPF2PATCharge[muonIndex2])){
          
	
	 if(muIndex1!=-1 && muIndex2!=-1 && event.packedCandsPseudoTrkPt[muIndex1]!=0 && event.packedCandsPseudoTrkPt[muIndex2]!=0 && event.packedCandsPseudoTrkCharge[muIndex1]==-(event.packedCandsPseudoTrkCharge[muIndex2])){
	   
	   if(muIndex1==event.muonPF2PATPackedCandIndex[muonIndex1] && muIndex2==event.muonPF2PATPackedCandIndex[muonIndex2]){
		   
	   mm3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex1],event.packedCandsPseudoTrkEta[muIndex1],event.packedCandsPseudoTrkPhi[muIndex1],event.packedCandsE[muIndex1]);
           mm4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex2],event.packedCandsPseudoTrkEta[muIndex2],event.packedCandsPseudoTrkPhi[muIndex2],event.packedCandsE[muIndex2]);
            
           h_PmuonsDeltaR->Fill(mm3.DeltaR(mm4), datasetWeight);
            
           if(mm3.DeltaR(mm4)<0.2){
		   
	     TLorentzVector lmuon1  {event.packedCandsPseudoTrkPx[muIndex1], event.packedCandsPseudoTrkPy[muIndex1], event.packedCandsPseudoTrkPz[muIndex1], event.packedCandsE[muIndex1]};
             TLorentzVector lmuon2  {event.packedCandsPseudoTrkPx[muIndex2], event.packedCandsPseudoTrkPy[muIndex2], event.packedCandsPseudoTrkPz[muIndex2], event.packedCandsE[muIndex2]};
                        
             Pmuoninv=(lmuon1+lmuon2).M();
             h_PmuonsInvMass->Fill((lmuon1+lmuon2).M(), datasetWeight);
	     //h_PmuonsInvMass->Fit(Gaussian2);
		   
	     PMpx=event.packedCandsPseudoTrkPx[muIndex1]+event.packedCandsPseudoTrkPx[muIndex2];
             PMpy=event.packedCandsPseudoTrkPy[muIndex1]+event.packedCandsPseudoTrkPy[muIndex2];
             PMpz=event.packedCandsPseudoTrkPz[muIndex1]+event.packedCandsPseudoTrkPz[muIndex2];
	     PME=event.packedCandsE[muIndex1]+event.packedCandsE[muIndex2];
	      
	     Pscalar.SetPxPyPzE(PMpx,PMpy,PMpz,PME);
	     
	     //h_PscalarInvMass->Fit(Gaussian3);
		   
	   }
		 
	   for(Int_t k{0};k<event.numPackedCands;k++) {
              const Int_t packedId {event.packedCandsPdgId[k]};
              if(k!=muIndex1 && k!=muIndex2){
                  
                TLorentzVector cone1;//The muon
                TLorentzVector cone2;//Packed candidate

                cone1.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex1],event.packedCandsPseudoTrkEta[muIndex1],event.packedCandsPseudoTrkPhi[muIndex1],event.packedCandsE[muIndex1]);
                cone2.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);
          
                if(cone1.DeltaR(cone2)<0.3){
                  PIsoSum3+=event.packedCandsPseudoTrkPt[k];
		}
              
		TLorentzVector cone3;//The other muon
                TLorentzVector cone4;//Packed candidate

                cone3.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[muIndex2],event.packedCandsPseudoTrkEta[muIndex2],event.packedCandsPseudoTrkPhi[muIndex2],event.packedCandsE[muIndex2]);
                cone4.SetPtEtaPhiE(event.packedCandsPseudoTrkPt[k],event.packedCandsPseudoTrkEta[k],event.packedCandsPseudoTrkPhi[k],event.packedCandsE[k]);

                if(cone3.DeltaR(cone4)<0.3){
                  PIsoSum4+=event.packedCandsPseudoTrkPt[k];
		}
	      }
		
	   }
	   if(std::abs((Pantiscalar+Pscalar).M()-125)<3){
	     if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsPseudoTrkCharge[pionIndex1]==-(event.packedCandsPseudoTrkCharge[pionIndex2])){
	       h_PIsoSum1->Fill(PIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1], datasetWeight);    
	       h_PIsoSum2->Fill(PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2], datasetWeight);
	       h_PIso2->Fill(PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2],event.packedCandsPseudoTrkPt[pionIndex2], datasetWeight);
		     
	       if(PIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2]<1){	   
	         h_PantiscalarInvMass->Fill(Pantiscalar.M(), datasetWeight);
		       
		 //Revealing the inner structure of the tracker layer?     
	         h_HVertexPosXY->Fill(event.packedCandsVx[pionIndex1],event.packedCandsVy[pionIndex1], datasetWeight);
	         h_HVertexPosXY->Fill(event.packedCandsVx[pionIndex2],event.packedCandsVy[pionIndex2], datasetWeight);
	  	     
	         h_HVertexPosRZ->Fill(event.packedCandsVz[pionIndex1],std::sqrt(event.packedCandsVx[pionIndex1]*event.packedCandsVx[pionIndex1]+event.packedCandsVy[pionIndex1]*event.packedCandsVy[pionIndex1]), datasetWeight);
	         h_HVertexPosRZ->Fill(event.packedCandsVz[pionIndex2],std::sqrt(event.packedCandsVx[pionIndex2]*event.packedCandsVx[pionIndex2]+event.packedCandsVy[pionIndex2]*event.packedCandsVy[pionIndex2]), datasetWeight);

	       }
		     
	       if(mm3.DeltaR(mm4)<0.2 && packed3.DeltaR(packed4)<0.2){  
	         if(PIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2]<1 && PIsoSum3/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && PIsoSum4/event.packedCandsPseudoTrkPt[muIndex2]<1){
	           h_PhiggsDeltaR->Fill(Pantiscalar.DeltaR(Pscalar), datasetWeight);
	           h_Pinvmass->Fill(Phadroninv,Pmuoninv, datasetWeight); 
		 }
		 if(PIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2]<1){
      		   for(Int_t k{0}; k<event.numChsTrackPairs;k++){
	   	      if(event.chsTkPairIndex1[k]==pionIndex1 && event.chsTkPairIndex2[k]==pionIndex2){	
	                TLorentzVector pi1  {event.chsTkPairTk1Px[k], event.chsTkPairTk1Py[k], event.chsTkPairTk1Pz[k], std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.1396,2))};
                        TLorentzVector pi2  {event.chsTkPairTk2Px[k], event.chsTkPairTk2Py[k], event.chsTkPairTk2Pz[k], std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.1396,2))};
	 
                        h_Rpionre12InvMass->Fill((pi1+pi2).M(), datasetWeight);
		      }
		   }
		 }
		 if(PIsoSum3/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && PIsoSum4/event.packedCandsPseudoTrkPt[muIndex2]<1){           
		   for(Int_t k{0}; k<event.numMuonTrackPairsPF2PAT;k++){
	   	      if(event.muonTkPairPF2PATIndex1[k]==muIndex1 && event.muonTkPairPF2PATIndex2[k]==muIndex2){
	                TLorentzVector Mu1  {event.muonTkPairPF2PATTk1Px[k], event.muonTkPairPF2PATTk1Py[k], event.muonTkPairPF2PATTk1Pz[k], std::sqrt(event.muonTkPairPF2PATTk1P2[k]+std::pow(0.106,2))};
                        TLorentzVector Mu2  {event.muonTkPairPF2PATTk2Px[k], event.muonTkPairPF2PATTk2Py[k], event.muonTkPairPF2PATTk2Pz[k], std::sqrt(event.muonTkPairPF2PATTk2P2[k]+std::pow(0.106,2))};
	                h_Rrefit12InvMass->Fill((Mu1+Mu2).M(), datasetWeight);
		      }   
		   }
		 }	 
	        
		   
		 
	       }
	     } //close pion!=-1 
		
	     
	     h_PIsoSum3->Fill(PIsoSum3/event.packedCandsPseudoTrkPt[muIndex1], datasetWeight);
             h_PIsoSum4->Fill(PIsoSum4/event.packedCandsPseudoTrkPt[muIndex2], datasetWeight);
	     
	     
	     if(PIsoSum3/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && PIsoSum4/event.packedCandsPseudoTrkPt[muIndex2]<1){	   
	       h_PscalarInvMass->Fill(Pscalar.M(), datasetWeight);
	     }
	   }//close Higgs mass window
		   
	   //wider higgs window	   
	   if(std::abs((Pantiscalar+Pscalar).M()-125)<20){//wider higgs mass window \pm20GeV
	     if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsPseudoTrkCharge[pionIndex1]==-(event.packedCandsPseudoTrkCharge[pionIndex2])){
               if(PIsoSum1/event.packedCandsPseudoTrkPt[pionIndex1]<0.4 && PIsoSum2/event.packedCandsPseudoTrkPt[pionIndex2]<1){	   
	         h_P20antiscalarInvMass->Fill(Pantiscalar.M(), datasetWeight);
	       } 
	       Phiggs=(Pantiscalar+Pscalar).M();
	       h_P20higgsInvMass->Fill(Phiggs, datasetWeight);     
	     }
		   
	     if(PIsoSum3/event.packedCandsPseudoTrkPt[muIndex1]<0.4 && PIsoSum4/event.packedCandsPseudoTrkPt[muIndex2]<1){	   
	       h_P20scalarInvMass->Fill(Pscalar.M(), datasetWeight);
	     }    
	   }//close wider window
	  
	   }
	 }//close muon!=-1
	 }    
	     
           
	      
      }//end of for loop packed candidates
  }//end of met filter   
	      
    
	      
	      
	      
    //Muon momentum comparison    

    if(event.metFilters()){
      if(event.muTrig()||event.mumuTrig()){ 
	      
	/*      
	//Packed candidates     
	if(muIndex1!=-1 && muIndex2!=-1 && event.packedCandsPseudoTrkPt[muIndex1]!=0 && event.packedCandsPseudoTrkPt[muIndex2]!=0 && event.packedCandsCharge[muIndex1]==-(event.packedCandsCharge[muIndex2])){
    
	  if(event.packedCandsHasTrackDetails[muIndex1]==1){
            h_muon1packedPtTrk->Fill(event.packedCandsPseudoTrkPt[muIndex1], datasetWeight);
	  }
	  if(event.packedCandsHasTrackDetails[muIndex2]==1){
            h_muon2packedPtTrk->Fill(event.packedCandsPseudoTrkPt[muIndex2], datasetWeight);
	  }
	   
	  TLorentzVector muonpackedPt1 {event.packedCandsPx[muIndex1],event.packedCandsPy[muIndex1],event.packedCandsPz[muIndex1],event.packedCandsE[muIndex1]}; 
	  TLorentzVector muonpackedPt2 {event.packedCandsPx[muIndex2],event.packedCandsPy[muIndex2],event.packedCandsPz[muIndex2],event.packedCandsE[muIndex2]}; 
          h_muon1packedPt->Fill(muonpackedPt1.Pt(), datasetWeight); 
	  h_muon2packedPt->Fill(muonpackedPt2.Pt(), datasetWeight); 
		
          h_muon1packedInvMass->Fill(muonpackedPt1.M(), datasetWeight); 
	  h_muon2packedInvMass->Fill(muonpackedPt2.M(), datasetWeight);
	  h_muon12packedInvMass->Fill((muonpackedPt1+muonpackedPt2).M(), datasetWeight); 
	}
	for(Int_t k{0}; k<event.numPackedCands;k++){
	   if(event.packedCandsHasTrackDetails[k]==1){
            h_muonipackedPtTrk->Fill(event.packedCandsPseudoTrkPt[k], datasetWeight);
	   }	
	  TLorentzVector muonpackedPti {event.packedCandsPx[k],event.packedCandsPy[k],event.packedCandsPz[k],event.packedCandsE[k]}; 
          h_muonipackedPt->Fill(muonpackedPti.Pt(), datasetWeight);  
          h_muonipackedInvMass->Fill(muonpackedPti.M(), datasetWeight); 
	}
	 */
	      
	      
	//PAT muons      
	for(Int_t k{0}; k<event.numMuonPF2PAT;k++){ 
           if(event.muonPF2PATInnerTkPt[k]>muonpt1){
             muonpt2=muonpt1;
             muonpt1=event.muonPF2PATInnerTkPt[k];
             muonIndex2=muonIndex1;
             muonIndex1=k;
	   }
           else if(event.muonPF2PATInnerTkPt[k]>muonpt2){
                  muonpt2=event.muonPF2PATInnerTkPt[k];
                  muonIndex2=k;
	   }
	   h_muoniRecPtTrk->Fill(event.muonPF2PATInnerTkPt[k], datasetWeight);
	   h_muoniRecPt->Fill(event.muonPF2PATPt[k], datasetWeight);
	}  
	
	if(muonIndex1!=-1 && muonIndex2!=-1 && event.muonPF2PATInnerTkPt[muonIndex1]!=0 && event.muonPF2PATInnerTkPt[muonIndex2]!=0 && event.muonPF2PATCharge[muonIndex1]==-(event.muonPF2PATCharge[muonIndex2])){
          
		  
		  
	  
	}     
	      
	      
	/*
	      
	//Tracks associated
	if(muonIndex1!=-1 && muonIndex2!=-1 && event.muonPF2PATInnerTkPt[muonIndex1]!=0 && event.muonPF2PATInnerTkPt[muonIndex2]!=0 && event.muonPF2PATCharge[muonIndex1]==-(event.muonPF2PATCharge[muonIndex2])){
	  h_muon1RecPtTrk->Fill(event.muonPF2PATInnerTkPt[muonIndex1], datasetWeight);
	  h_muon2RecPtTrk->Fill(event.muonPF2PATInnerTkPt[muonIndex2], datasetWeight);  
	} 
	//No tracks associated      
	h_muon1RecPt->Fill(event.muonPF2PATPt[0], datasetWeight);
	h_muon2RecPt->Fill(event.muonPF2PATPt[1], datasetWeight);
	      
	TLorentzVector VecMu1  {event.muonPF2PATPX[0], event.muonPF2PATPY[0], event.muonPF2PATPZ[0], event.muonPF2PATE[0]};
        TLorentzVector VecMu2  {event.muonPF2PATPX[1], event.muonPF2PATPY[1], event.muonPF2PATPZ[1], event.muonPF2PATE[1]};
	 
	h_muon1RecInvMass->Fill(VecMu1.M(), datasetWeight);
	h_muon2RecInvMass->Fill(VecMu2.M(), datasetWeight);
        h_muon12RecInvMass->Fill((VecMu1+VecMu2).M(), datasetWeight);
	     
	 */
	      
	//Refitted tracks muons   
	for(Int_t k{0}; k<event.numMuonTrackPairsPF2PAT;k++){
	   
	   if(event.muonTkPairPF2PATIndex1[k]==muonIndex1 && event.muonTkPairPF2PATIndex2[k]==muonIndex2){	
	   
	     h_muon1PairsPt->Fill(event.muonTkPairPF2PATTk1Pt[k], datasetWeight);
	     h_muon2PairsPt->Fill(event.muonTkPairPF2PATTk2Pt[k], datasetWeight); 
		   
	    /* if(muIndex1!=-1 && muIndex2!=-1 && event.packedCandsPseudoTrkPt[muIndex1]!=0 && event.packedCandsPseudoTrkPt[muIndex2]!=0 && event.packedCandsCharge[muIndex1]==-(event.packedCandsCharge[muIndex2])){

	       TLorentzVector VecMu1  {event.muonTkPairPF2PATTk1Px[k], event.muonTkPairPF2PATTk1Py[k], event.muonTkPairPF2PATTk1Pz[k], event.packedCandsE[muIndex1]};
               TLorentzVector VecMu2  {event.muonTkPairPF2PATTk2Px[k], event.muonTkPairPF2PATTk2Py[k], event.muonTkPairPF2PATTk2Pz[k], event.packedCandsE[muIndex2]};
	 
	       h_muon1refitInvMass->Fill(VecMu1.M(), datasetWeight);
	       h_muon2refitInvMass->Fill(VecMu2.M(), datasetWeight);
               h_muon12refitInvMass->Fill((VecMu1+VecMu2).M(), datasetWeight);
	     }*/
		  
	     TLorentzVector Mu1  {event.muonTkPairPF2PATTk1Px[k], event.muonTkPairPF2PATTk1Py[k], event.muonTkPairPF2PATTk1Pz[k], std::sqrt(event.muonTkPairPF2PATTk1P2[k]+std::pow(0.106,2))};
             TLorentzVector Mu2  {event.muonTkPairPF2PATTk2Px[k], event.muonTkPairPF2PATTk2Py[k], event.muonTkPairPF2PATTk2Pz[k], std::sqrt(event.muonTkPairPF2PATTk2P2[k]+std::pow(0.106,2))};
	 
	     h_refit1InvMass->Fill(Mu1.M(), datasetWeight);
	     h_refit2InvMass->Fill(Mu2.M(), datasetWeight);
             h_refit12InvMass->Fill((Mu1+Mu2).M(), datasetWeight);
	
		   
	     //h_muonPairsXY->Fill(event.muonTkPairPF2PATTkVx[k],event.muonTkPairPF2PATTkVy[k], datasetWeight);
             //h_muonPairsRZ->Fill(std::abs(event.muonTkPairPF2PATTkVz[k]),std::sqrt(event.muonTkPairPF2PATTkVx[k]*event.muonTkPairPF2PATTkVx[k]+event.muonTkPairPF2PATTkVy[k]*event.muonTkPairPF2PATTkVy[k]), );
             
	   }
	     
	}  
	
      
	//Refitted tracks pions   
	for(Int_t k{0}; k<event.numChsTrackPairs;k++){
	   if(pionIndex1!=-1 && pionIndex2!=-1 && event.packedCandsPseudoTrkPt[pionIndex1]!=0 && event.packedCandsPseudoTrkPt[pionIndex2]!=0 && event.packedCandsCharge[pionIndex1]==-(event.packedCandsCharge[pionIndex2])){
 
	     if(event.chsTkPairIndex1[k]==pionIndex1 && event.chsTkPairIndex2[k]==pionIndex2){	
		   
	       TLorentzVector pi1  {event.chsTkPairTk1Px[k], event.chsTkPairTk1Py[k], event.chsTkPairTk1Pz[k], std::sqrt(event.chsTkPairTk1P2[k]+std::pow(0.1396,2))};
               TLorentzVector pi2  {event.chsTkPairTk2Px[k], event.chsTkPairTk2Py[k], event.chsTkPairTk2Pz[k], std::sqrt(event.chsTkPairTk2P2[k]+std::pow(0.1396,2))};
	 
               h_pionre12InvMass->Fill((pi1+pi2).M(), datasetWeight);
	     }
	   }
	}
	   
	 
	
	      
      }//end of single/double muon trigger
    }//end of met filter
	      
	  
	      
	      
	      
          
           
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
  h_genParHiggsE->GetXaxis()->SetTitle("GeV");
  h_genParHiggsE->Write();
  h_HiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_HiggsInvMass->Write();

  h_genParScalarPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarPt->Write();
  h_genParScalarEta->Write();
  h_genParScalarPhi->Write();
  h_genParScalarE->GetXaxis()->SetTitle("GeV");
  h_genParScalarE->Write();
  h_ScalarDeltaR->GetXaxis()->SetTitle("Radians");
  h_ScalarDeltaR->Write();
  h_ScalarDeltaPhi->Write();
  h_ScalarInvMass->GetXaxis()->SetTitle("GeV");
  h_ScalarInvMass->Write();
  h_Scalar3DAngle->Write();
    
 
  h_genParScalarMuonPtL->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonPtL->Write();
  h_genParScalarMuonPtS->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonPtS->Write();
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
  h_genParScalarMuonE->GetXaxis()->SetTitle("GeV");
  h_genParScalarMuonE->Write();
  h_MuonDeltaR->GetXaxis()->SetTitle("Radians");
  h_MuonDeltaR->Write();
  h_MuonDeltaPhi->Write();
  h_MuonInvMass->GetXaxis()->SetTitle("GeV");
  h_MuonInvMass->Write();
  h_Muon3DAngle->Write();
    
  h_genParScalarCKaonPt->GetXaxis()->SetTitle("GeV");
  h_genParScalarCKaonPt->Write();
  h_genParScalarCKaonEta->Write();
  h_genParScalarCKaonPhi->Write();
  h_genParScalarCKaonE->GetXaxis()->SetTitle("GeV");
  h_genParScalarCKaonE->Write();
  h_KaonDeltaR->GetXaxis()->SetTitle("Radians");
  h_KaonDeltaR->Write();
  h_KaonDeltaPhi->Write();
  h_KaonInvMass->GetXaxis()->SetTitle("GeV");
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
  h_muonRecPtL->GetXaxis()->SetTitle("GeV");
  h_muonRecPtL->Write();
  h_muonRecPtS->GetXaxis()->SetTitle("GeV");
  h_muonRecPtS->Write();
  h_muonRecEta->Write();
  h_muonRecPhi->Write();
  h_muonRecE->GetXaxis()->SetTitle("GeV");
  h_muonRecE->Write();
  h_muonRecDeltaR->GetXaxis()->SetTitle("Radians");
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


 
  h_HVertexPosXY->GetXaxis()->SetTitle("V_{x} (cm)");
  h_HVertexPosXY->GetYaxis()->SetTitle("V_{y} (cm)");
  h_HVertexPosXY->Write();
  h_HVertexPosRZ->GetXaxis()->SetTitle("V_{z} (cm)");
  h_HVertexPosRZ->GetYaxis()->SetTitle("R (cm)");
  h_HVertexPosRZ->Write();
	



  //Kaon mass assumption	
  h_KhadronDeltaR->GetXaxis()->SetTitle("Radians");
  h_KhadronDeltaR->Write();
  h_KmuonsDeltaR->GetXaxis()->SetTitle("Radians");
  h_KmuonsDeltaR->Write();
  h_KIsoSum1->Write();
  h_KIsoSum2->Write();
  h_KIso2->GetXaxis()->SetTitle("Relative isolation sum");
  h_KIso2->GetYaxis()->SetTitle("Particle momentum");
  h_KIso2->Write();
  h_KIsoSum3->Write();
  h_KIsoSum4->Write();
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
  h_KscalarInvMass->GetXaxis()->SetTitle("GeV");
  h_KscalarInvMass->Write();
  h_KhiggsInvMass->GetXaxis()->SetTitle("GeV");
  h_KhiggsInvMass->Write();
  h_KhiggsDeltaR->GetXaxis()->SetTitle("Radians");
  h_KhiggsDeltaR->Write();  
	
	
	
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

  h_PantiscalarInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_PantiscalarInvMass->GetYaxis()->SetTitle("Events");
  h_PantiscalarInvMass->Write();
  h_PscalarInvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_PscalarInvMass->GetYaxis()->SetTitle("Events");
  h_PscalarInvMass->Write();
  h_PhiggsInvMass->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2})");
  h_PhiggsInvMass->GetYaxis()->SetTitle("Events");
  h_PhiggsInvMass->Write();
  h_PhiggsDeltaR->GetYaxis()->SetTitle("Events");
  h_PhiggsDeltaR->GetXaxis()->SetTitle("Radians");
  h_PhiggsDeltaR->Write();  
  h_Rrefit12InvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_Rrefit12InvMass->GetYaxis()->SetTitle("Events");
  h_Rrefit12InvMass->Write();
  h_Rpionre12InvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_Rpionre12InvMass->GetYaxis()->SetTitle("Events");
  h_Rpionre12InvMass->Write();

  h_P20antiscalarInvMass->GetXaxis()->SetTitle("m_{dihadron} (GeV/c^{2})");
  h_P20antiscalarInvMass->GetYaxis()->SetTitle("Events");
  h_P20antiscalarInvMass->Write();
  h_P20scalarInvMass->GetXaxis()->SetTitle("m_{#mu#mu} (GeV/c^{2})");
  h_P20scalarInvMass->GetYaxis()->SetTitle("Events");
  h_P20scalarInvMass->Write();
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
	
  /*h_muonPairsXY->SetTitle("Vertex position x")
  h_muonPairsXY->SetTitle("Vertex position y")
  h_muonPairsXY->Write();
  h_muonPairsRZ->SetTitle("Vertex position z")
  h_muonPairsRZ->SetTitle("Vertex position R")
  h_muonPairsRZ->Write();*/
	
	
	
	
	
	
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
