#include "AnalysisEvent.hpp"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
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

std::string pythiaStatus (const Int_t status);
std::string pdgIdCode (const Int_t status, const bool unicode = false);
bool scalarGrandparent(const AnalysisEvent& event, const Int_t& k, const Int_t& pdgId_);

uint debugCounter;

namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
    auto timerStart = std::chrono::high_resolution_clock::now(); 

    std::string config;
    std::vector<Dataset> datasets;
    double totalLumi;
    double usePreLumi;

    std::map<int, int> pdgIdMap;
    std::map<int, int> pdgIdMapStatus1;
    std::map<int, int> pdgIdMapStatus2;
    std::map<int, int> pdgIdMapStatus23;
    std::map<int, int> pdgIdMapStatus33;
    std::map<int, int> pdgIdMapStatus6X;
    std::map<int, int> pdgIdMapStatus7X;

    std::map<int, int> pdgIdMapScalarMother;

    std::map<int, int> pdgIdMapFromScalar;
    std::map<int, int> pdgIdMapFromScalarStatus1;
    std::map<int, int> pdgIdMapFromScalarStatus23;
    std::map<int, int> pdgIdMapFromScalarStatus33;
    std::map<int, int> pdgIdMapFromScalarStatus71;

    std::map<int, int> pdgIdMapScalarDecayProducts;

    std::vector <int>  nJetsFromScalar;
    std::vector <int>  nJetsPerEvent;
    std::vector <int>  nOutgoingStatus;
    std::vector <int>  nOutgoingStatus23;
    std::vector <int>  nOutgoingStatus33;

    std::map<int, int> pdgIdMapJets;
    std::map<int, int> pdgIdMapGenJets;
    std::map<int, int> pdgIdMapJetsFromScalar;
    std::map<int, int> pdgIdMapGenJetsFromScalar;
    std::map<int, int> pdgIdMapJetsNotFromScalar;
    std::map<int, int> pdgIdMapGenJetsNotFromScalar;

    std::map<int, int> pdgIdMap1GenJetFromScalar;
    std::map<int, int> pdgIdMap2GenJetsFromScalar;
    std::map<int, int> pdgIdMap3GenJetsFromScalar;
    std::map<int, int> pdgIdMap4GenJetsFromScalar;
    std::map<int, int> pdgIdMap5GenJetsFromScalar;
    std::map<int, int> pdgIdMap6GenJetsFromScalar;

   
    std::string outFileString{"plots/distributions/output.root"};
    bool is2016_;
    bool is2018_;
    Long64_t nEvents;
    Long64_t totalEvents {0};
    const std::regex mask{".*\\.root"};


    TH1I* h_pidsFromScalarDecays  {new TH1I("h_pidsFromScalarDecays",  "pids of scalar decays"    , 6, 0, 6)};
    TH1I* h_kaonsFromScalarDecays {new TH1I("h_kaonsFromScalarDecays", "kaons from scalar decays" , 6, 0, 6)};
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(1, "K K");
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(2, "K #pi^{#pm}");
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(3, "K #gamma");
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(4, "#pi^{#pm} #pi^{#pm}");
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(5, "#pi^{#pm} #gamma");
    h_pidsFromScalarDecays->GetXaxis()->SetBinLabel(6, "#gamma #gamma");

    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(1, "K^{#pm} K^{#pm}");
    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(2, "K^{#pm} K_{S}^{0}");
    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(3, "K^{#pm} K_{L}^{0}");
    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(4, "K_{S}^{0} K_{S}^{0}");
    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(5, "K_{S}^{0} K_{L}^{0}");
    h_kaonsFromScalarDecays->GetXaxis()->SetBinLabel(6, "K_{L}^{0} K_{L}^{0}");

    // GenPar scalar decay product kinematics
    TH1F* h_genParMuonPt  {new TH1F("h_genParMuonPt",  "genPar #mu from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParMuonEta {new TH1F("h_genParMuonEta", "genPar #mu from scalar decay #eta",  200, -7., 7.)}; 
    TH1F* h_genParMuonPhi {new TH1F("h_genParMuonPhi", "genPar #mu from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParMuonE   {new TH1F("h_genParMuonE",   "genPar #mu from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParMuonMass   {new TH1F("h_genParMuonMass",   "genPar #mu from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParKaonPt  {new TH1F("h_genParKaonPt",  "genPar K from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParKaonEta {new TH1F("h_genParKaonEta", "genPar K from scalar decay #eta",  200, -7., 7.)}; 
    TH1F* h_genParKaonPhi {new TH1F("h_genParKaonPhi", "genPar K from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParKaonE   {new TH1F("h_genParKaonE",   "genPar K from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParKaonMass   {new TH1F("h_genParKaonMass",   "genPar K from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParChargedKaonPt  {new TH1F("h_genParChargedKaonPt",  "genPar K^{#pm} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParChargedKaonEta {new TH1F("h_genParChargedKaonEta", "genPar K^{#pm} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParChargedKaonPhi {new TH1F("h_genParChargedKaonPhi", "genPar K^{#pm} from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParChargedKaonE   {new TH1F("h_genParChargedKaonE",   "genPar K^{#pm} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParChargedKaonMass   {new TH1F("h_genParChargedKaonMass",   "genPar K^{#pm} from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParNeutralKaonPt  {new TH1F("h_genParNeutralKaonPt",  "genPar K^{0} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParNeutralKaonEta {new TH1F("h_genParNeutralKaonEta", "genPar K^{0} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParNeutralKaonPhi {new TH1F("h_genParNeutralKaonPhi", "genPar K^{0} from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParNeutralKaonE   {new TH1F("h_genParNeutralKaonE",   "genPar K^{0} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParNeutralKaonMass   {new TH1F("h_genParNeutralKaonMass",   "genPar K^{0} from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParKshortPt  {new TH1F("h_genParKshortPt",  "genPar K^{0}_{S} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParKshortEta {new TH1F("h_genParKshortEta", "genPar K^{0}_{S} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParKshortPhi {new TH1F("h_genParKshortPhi", "genPar K^{0}_{S} from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParKshortE   {new TH1F("h_genParKshortE",   "genPar K^{0}_{S} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParKshortMass   {new TH1F("h_genParKshortMass",   "genPar K^{0}_{S} from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParKlongPt  {new TH1F("h_genParKlongPt",  "genPar K^{0}_{L} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParKlongEta {new TH1F("h_genParKlongEta", "genPar K^{0}_{L} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParKlongPhi {new TH1F("h_genParKlongPhi", "genPar K^{0}_{L} from scalar decay #phi",  100, -3.5, 3.5)}; 
    TH1F* h_genParKlongE   {new TH1F("h_genParKlongE",   "genPar K^{0}_{L} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParKlongMass   {new TH1F("h_genParKlongMass",   "genPar K^{0}_{L} from scalar decay mass",     200, 0., 1.)};

    TH1F* h_genParPionPt  {new TH1F("h_genParPionPt",  "genPar #pi from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParPionEta {new TH1F("h_genParPionEta", "genPar #pi from scalar decay #eta",  200, -7., 7.)}; 
    TH1F* h_genParPionPhi {new TH1F("h_genParPionPhi", "genPar #pi from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParPionE   {new TH1F("h_genParPionE",   "genPar #pi from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParPionMass {new TH1F("h_genParPionMass",   "genPar #pi from scalar decay Mass",     200, 0., 1.)};

    TH1F* h_genParChargedPionPt  {new TH1F("h_genParChargedPionPt",  "genPar #pi^{#pm} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParChargedPionEta {new TH1F("h_genParChargedPionEta", "genPar #pi^{#pm} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParChargedPionPhi {new TH1F("h_genParChargedPionPhi", "genPar #pi^{#pm} from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParChargedPionE   {new TH1F("h_genParChargedPionE",   "genPar #pi^{#pm} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParChargedPionMass {new TH1F("h_genParChargedPionMass",   "genPar #pi^{#pm} from scalar decay Mass",     200, 0., 1.)};

    TH1F* h_genParNeutralPionPt  {new TH1F("h_genParNeutralPionPt",  "genPar #pi^{0} from scalar decay p_{T}", 600, 0., 300.)};
    TH1F* h_genParNeutralPionEta {new TH1F("h_genParNeutralPionEta", "genPar #pi^{0} from scalar decay #eta",  200, -7., 7.)};
    TH1F* h_genParNeutralPionPhi {new TH1F("h_genParNeutralPionPhi", "genPar #pi^{0} from scalar decay #phi",  100, -3.5, 3.5)};
    TH1F* h_genParNeutralPionE   {new TH1F("h_genParNeutralPionE",   "genPar #pi^{0} from scalar decay E",     1000, 0., 1000.)};
    TH1F* h_genParNeutralPionMass {new TH1F("h_genParNeutralPionMass",   "genPar #pi^{0} from scalar decay Mass",     200, 0., 1.)};

    //

    TH1F* h_packedCandDxy   {new TH1F("h_packedCandDxy", "Packed Candidate dxy",       300,  -150., 150.)};
    TH1F* h_packedCandDz    {new TH1F("h_packedCandDz",  "Packed Candidate dz",        1200, -600., 600.)};
    TH1F* h_packedCandVx    {new TH1F("h_packedCandVx",  "Packed Candidate track vx",  300,  -150., 150.)};
    TH1F* h_packedCandVy    {new TH1F("h_packedCandVy",  "Packed Candidate track vy",  300,  -150., 150.)};
    TH1F* h_packedCandVxy   {new TH1F("h_packedCandVxy", "Packed Candidate track vxy", 300,  -150., 150.)};
    TH1F* h_packedCandVz    {new TH1F("h_packedCandVz",  "Packed Candidate track vz",  1200, -600., 600.)};

    TH1F* h_muonDR          {new TH1F("h_muonDR",         "#DeltaR between generator level muons", 100, 0., 10.)};
    TH1F* h_pionDR          {new TH1F("h_pionDR",         "#DeltaR between generator level pions", 100, 0., 10.)};

    TH1F* h_scalarDR        {new TH1F("h_scalarDR",       "#DeltaR between generator level scalars" ,    100, 0., 10.)};
    TH1F* h_scalarDPhi      {new TH1F("h_scalarDPhi",     "#Delta#phi between generator level scalars",  100, -3.5, 3.5)};
    TH1F* h_scalarDrEtaPhi  {new TH1F("h_scalarDrEtaPhi", "#Deltar#eta#phi between generator level scalars", 100, 0., 7.)};

    TH1F* h_scalarPt      {new TH1F("h_scalarPt",     "gen scalar p_{T}", 600, 0., 300.)};
    TH1F* h_scalarEta      {new TH1F("h_scalarEta",     "gen scalar #eta", 200, -7., 7.)};
    TH1F* h_scalarPhi      {new TH1F("h_scalarPhi",     "gen scalar #eta", 100,-3.5, 3.5)};
    TH1F* h_scalarMass      {new TH1F("h_scalarMass",     "gen scalar mass", 200, 0., 4.)};

    TH1F* h_higgsPt      {new TH1F("h_higgsPt",     "gen higgs p_{T}", 600, 0., 300.)};
    TH1F* h_higgsEta      {new TH1F("h_higgsEta",     "gen higgs #eta", 200, -7., 7.)};
    TH1F* h_higgsPhi      {new TH1F("h_higgsPhi",     "gen higgs #eta", 100,-3.5, 3.5)};
    TH1F* h_higgsMass      {new TH1F("h_higgsMass",     "gen higgs mass", 400, 0., 200.)};

    TH1F* h_genParKaonDR        {new TH1F("h_genParKaonDR",       "#DeltaR between genParticle kaons from scalar decays" ,    100, 0., 10.)};
    TH1F* h_genParKaonDPhi      {new TH1F("h_genParKaonDPhi",     "#Delta#phi between genParticle kaons from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_genParKaonDrEtaPhi  {new TH1F("h_genParKaonDrEtaPhi", "#Deltar#eta#phi between genParticle kaons from scalar decays", 100, 0., 7.)};
    TH1F* h_genJetKaonDR        {new TH1F("h_genJetKaonDR",       "#DeltaR between genJet kaons from scalar decays" ,    100, 0., 10.)};
    TH1F* h_genJetKaonDPhi      {new TH1F("h_genJetKaonDPhi",     "#Delta#phi between genJet kaons from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_genJetKaonDrEtaPhi  {new TH1F("h_genJetKaonDrEtaPhi", "#Deltar#eta#phi between genJet kaons from scalar decays", 100, 0., 7.)};
    TH1F* h_recoJetKaonDR        {new TH1F("h_recoJetKaonDR",       "#DeltaR between recoJet kaons from scalar decays" ,    100, 0., 10.)};
    TH1F* h_recoJetKaonDPhi      {new TH1F("h_recoJetKaonDPhi",     "#Delta#phi between recoJet kaons from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_recoJetKaonDrEtaPhi  {new TH1F("h_recoJetKaonDrEtaPhi", "#Deltar#eta#phi between recoJet kaons from scalar decays", 100, 0., 7.)};

    TH1F* h_genParKshortDR        {new TH1F("h_genParKshortDR",       "#DeltaR between genParticle K_{S}^{0} from scalar decays" ,    100, 0., 10.)};
    TH1F* h_genParKshortDPhi      {new TH1F("h_genParKshortDPhi",     "#Delta#phi between genParticle K_{S}^{0} from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_genParKshortDrEtaPhi  {new TH1F("h_genParKshortDrEtaPhi", "#Deltar#eta#phi between genParticle K_{S}^{0} from scalar decays", 100, 0., 7.)};
    TH1F* h_genJetKshortDR        {new TH1F("h_genJetKshortDR",       "#DeltaR between genJet K_{S}^{0} from scalar decays" ,    100, 0., 10.)};
    TH1F* h_genJetKshortDPhi      {new TH1F("h_genJetKshortDPhi",     "#Delta#phi between genJet K_{S}^{0} from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_genJetKshortDrEtaPhi  {new TH1F("h_genJetKshortDrEtaPhi", "#Deltar#eta#phi between genJet K_{S}^{0} from scalar decays", 100, 0., 7.)};
    TH1F* h_recoJetKshortDR        {new TH1F("h_recoJetKshortDR",       "#DeltaR between recoJet K_{S}^{0} from scalar decays" ,    100, 0., 10.)};
    TH1F* h_recoJetKshortDPhi      {new TH1F("h_recoJetKshortDPhi",     "#Delta#phi between recoJet K_{S}^{0} from scalar decays",  100, -3.5, 3.5)};
    TH1F* h_recoJetKshortDrEtaPhi  {new TH1F("h_recoJetKshortDrEtaPhi", "#Deltar#eta#phi between recoJet K_{S}^{0} from scalar decays", 100, 0., 7.)};

    TH1F* h_recoJetSumInvMass    {new TH1F("h_recoJetSumInvMass", "Sum of invariant masses of all reco jets",1000, 0.0, 200.)};
    TH1F* h_recoJetInvMass       {new TH1F("h_recoJetInvMass",    "Invariant mass of each reco jet",1000, 0.0, 200.)};
    TH1F* h_recoJetPt            {new TH1F("h_recoJetPt",         "p_{T} mass of all reco jets",200, 0.0, 200.)};
    TH1F* h_recoJetEta           {new TH1F("h_recoJetEta",        "#eta of all reco jets", 200, -7., 7.)};

    TH1F* h_recoJetScalarSumInvMass  {new TH1F("h_recoJetScalarSumInvMass", "Sum of invariant masses of reco jets descended from scalar particles",1000, 0.0, 1000.)};
    TH1F* h_reco2JetScalarSumInvMass {new TH1F("h_reco2JetScalarSumInvMass", "Sum of invariant masses of two reco jets descended from scalar particles",1000, 0.0, 1000.)};
    TH1F* h_reco2JetMinScalarSumInvMass {new TH1F("h_reco2JetMinScalarSumInvMass", "Sum of invariant masses of at least two reco jets descended from scalar particles",1000, 0.0, 1000.)};
    TH1F* h_recoJetScalarInvMass    {new TH1F("h_recoJetScalarInvMass",    "Invariant mass of each reco jet descended from scalar particles",1000, 0.0, 1000.)};
    TH1F* h_recoJetScalarPt         {new TH1F("h_recoJetScalarPt",         "p_{T} of all reco jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetScalarEta        {new TH1F("h_recoJetScalarEta",        "#eta of all reco jets descended from scalar particles",200, -7., 7.)};
    TH1F* h_genJetScalarSumInvMass  {new TH1F("h_genJetScalarSumInvMass",  "Sum of the invariant masses of gen jets descended from scalar particles",1000, 0.0, 200.)};
    TH1F* h_gen2JetScalarSumInvMass  {new TH1F("h_gen2JetScalarSumInvMass","Sum of the invariant masses of two gen jets descended from scalar particles",1000, 0.0, 200.)};
    TH1F* h_gen2JetMinScalarSumInvMass  {new TH1F("h_gen2JetMinScalarSumInvMass","Sum of the invariant masses of at least two gen jets descended from scalar particles",1000, 0.0, 200.)};
    TH1F* h_genJetScalarInvMass     {new TH1F("h_genJetScalarInvMass",     "Invariant mass of each gen jet descended from scalar particles",1000, 0.0, 200.)};
    TH1F* h_genJetScalarPt          {new TH1F("h_genJetScalarPt",          "p_{T} of all gen jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetScalarEta         {new TH1F("h_genJetScalarEta",         "#eta of all gen jets descended from scalar particles",200, -7., 7.)};

    TH1F* h_recoJetPionInvMass         {new TH1F("h_recoJetPionInvMass",         "Reco invariant mass of #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetPionScalarInvMass   {new TH1F("h_recoJetPionScalarInvMass",   "Reco invariant mass of all #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetPionPt              {new TH1F("h_recoJetPionPt",              "Reco p_{T} of #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetPionEta             {new TH1F("h_recoJetPionEta",             "Reco #eta of #pi jets descended from scalar particles",200, -7., 7.)};
    TH1F* h_genJetPionInvMass          {new TH1F("h_genJetPionInvMass"      ,    "Gen invariant mass of #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetPionScalarInvMass    {new TH1F("h_genJetPionScalarInvMass",    "Gen invariant mass of all #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetPionPt               {new TH1F("h_genJetPionPt",               "Gen p_{T} of #pi jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetPionEta              {new TH1F("h_genJetPionEta",              "Gen #eta of #pi jets descended from scalar particles",200, -7., 7.)};

    TH1F* h_recoJetKaonInvMass         {new TH1F("h_recoJetKaonInvMass",         "Reco invariant mass of Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetKaonScalarInvMass   {new TH1F("h_recoJetKaonScalarInvMass",   "Reco invariant mass of all Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetKaonPt              {new TH1F("h_recoJetKaonPt",              "Reco p_{T} of Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJetKaonEta             {new TH1F("h_recoJetKaonEta",             "Reco #eta of Kaon descended from scalar particles",200, -7., 7.)};
    TH1F* h_genJetKaonInvMass          {new TH1F("h_genJetKaonInvMass",          "Gen invariant mass of Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetKaonScalarInvMass    {new TH1F("h_genJetKaonScalarInvMass",    "Gen invariant mass of all Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetKaonPt               {new TH1F("h_genJetKaonPt",               "Gen p_{T} of Kaon jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJetKaonEta              {new TH1F("h_genJetKaonEta",              "Gen #eta of Kaon descended from scalar particles",200, -7., 7.)};

    TH1F* h_genJetMass           {new TH1F("h_genJetMass",            "GenJet MC truth mass of all gen jets descended from scalar particles",200, 0.0, 200.)};

    TH1F* h_recoJet1InvMass {new TH1F("h_recoJet1InvMass", "Invariant mass of 1 reco jet descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJet1InvMass  {new TH1F("h_genJet1InvMass",  "Invariant mass of 1 gen jet descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJet1Mass     {new TH1F("h_genJet1Mass",      "Reco::jet mass of 1 gen jet descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_recoJet2InvMass {new TH1F("h_recoJet2InvMass", "Invariant mass of 2 reco jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJet2InvMass  {new TH1F("h_genJet2InvMass",  "Invariant mass of 2 gen jets descended from scalar particles",200, 0.0, 200.)};
    TH1F* h_genJet2Mass     {new TH1F("h_genJet2Mass",      "Reco::jet mass of 2 gen jets descended from scalar particles",200, 0.0, 200.)};

////

//    int maxGenPars {0};

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
        "2016", po::bool_switch(&is2016_), "Use 2016 conditions (SFs, et al.).")(
        "2018", po::bool_switch(&is2018_), "Use 2018 conditions (SFs, et al.).");
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

        AnalysisEvent event{dataset->isMC(), datasetChain, is2016_, is2018_};

        Long64_t numberOfEvents{datasetChain->GetEntries()};
        if (nEvents && nEvents < numberOfEvents) numberOfEvents = nEvents;

        TMVA::Timer* lEventTimer{ new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}}; 
        lEventTimer->DrawProgressBar(0, "");
    
        totalEvents += numberOfEvents;
        for (Long64_t i{0}; i < numberOfEvents; i++) {

            lEventTimer->DrawProgressBar(i,"");

            event.GetEntry(i);      

            int nJetsFromScalarCounter {0};
            int nJetsPerEventCounter   {0};

            int nOutgoingStatusCounter {0};
            int nOutgoingStatus23Counter {0};
            int nOutgoingStatus33Counter {0};


//            std::cout << "eventNum: " << event.eventNum << std::endl;

            //////// PACKED CAND STUFF

//            std::cout << "PackedCands COLLECTION" << std::endl;
//            std::cout << "event.numPackedCands: " << event.numPackedCands << std::endl;

            for (Int_t k{0}; k < event.numPackedCands; k++) {
                const TLorentzVector packedCandsLVec {event.packedCandsPx[k], event.packedCandsPy[k], event.packedCandsPz[k], event.packedCandsE[k]};

                const Int_t packedCandsPid                {event.packedCandsPdgId[k]};
                const Float_t packedCandsPt               {float (packedCandsLVec.Pt()) };
                
//                std::cout << "PID = " << packedCandsPid << " ; pT= " << packedCandsPt << " ; LVec mass = " << packedCandsLVec.M() << std::endl;
//                std::cout << "Vx = " << event.packedCandsPseudoTrkVx[k] << "; Vy = " << event.packedCandsPseudoTrkVy[k] << "; Vz = " << event.packedCandsPseudoTrkVz[k] << std::endl;
//                std::cout << "dz = " << event.packedCandsDz[k] << "; dxy = " << event.packedCandsDz[k] << "; highQuality[k] = " << event.packedCandsHighPurityTrack[k] << std::endl;

                  h_packedCandDxy->Fill(event.packedCandsDxy[k], datasetWeight);
                  h_packedCandDz->Fill(event.packedCandsDz[k], datasetWeight);
                  if ( event.packedCandsHasTrackDetails[k] ) {
                      h_packedCandVx->Fill(event.packedCandsPseudoTrkVx[k], datasetWeight);
                      h_packedCandVy->Fill(event.packedCandsPseudoTrkVy[k], datasetWeight);
                      float Vxy = std::sqrt(event.packedCandsPseudoTrkVx[k]*event.packedCandsPseudoTrkVx[k] + event.packedCandsPseudoTrkVy[k]*event.packedCandsPseudoTrkVy[k]);
                      h_packedCandVxy->Fill(Vxy, datasetWeight);
                      h_packedCandVz->Fill(event.packedCandsPseudoTrkVz[k], datasetWeight);
                  }
            }

//            std::cout << "Number of general pions: " << pionTrackCounter << std::endl;
//            std::cout << "Number of general pions with tracks: " << pionAndTrackCounter << std::endl;
//            std::cout << "Number of general pions with jets: " << pionAndJetCounter << std::endl;

            //////// ISO TRACK STUFF
/*
            uint pionTrackCounter = 0;
//            std::cout << "ISO TRACKS COLLECTION" << std::endl;

            for (Int_t k{0}; k < event.numIsolatedTracks; k++) {
                const Int_t isoTrackPid                {event.isoTracksPdgId[k]};
                const Float_t isoTrackCaloJetEmEnergy  {event.isoTracksMatchedCaloJetEmEnergy[k]};
                const Float_t isoTrackCaloJetHadEnergy {event.isoTracksMatchedCaloJetHadEnergy[k]};
		const Float_t isoTrackPt               {event.isoTracksPt[k]};
                const bool isPion {std::abs(isoTrackPid) == 211};
                if (isPion) pionTrackCounter++;

//		const TLorentzVector isoTrackLVec {event.isoTracksPx[k], event.isoTracksPy[k], event.isoTracksPz[k], event.isoTracksE[k]};
//                std::cout << isoTrackPid << " : " << isoTrackPt << " : " << isoTrackCaloJetEmEnergy << " : " << isoTrackCaloJetHadEnergy << " : LVec mass = " << isoTrackLVec.M() << std::endl;

            }
//            std::cout << "Number of isolated pion tracks: " << pionTrackCounter << std::endl;
*/

            //////// JET STUFF


            std::vector< std::pair<TLorentzVector, int> > recoJetVec;
            std::vector< std::pair<TLorentzVector, int> > recoJetVecFromScalar;
            std::vector< std::pair<TLorentzVector, int> > genJetVecFromScalar;

            int jetDaughtersCounter {0};

            for (Int_t k{0}; k < event.numJetPF2PAT; k++) {
                const Int_t jetPid       {event.jetPF2PATPID[k]};
                const Int_t genJetPid    {event.genJetPF2PATPID[k]};
                const Int_t fromScalar   {event.genJetPF2PATScalarAncestor[k]};

                const TLorentzVector recoJet {event.jetPF2PATPx[k], event.jetPF2PATPy[k], event.jetPF2PATPz[k], event.jetPF2PATE[k]};
                const TLorentzVector genJet  {event.genJetPF2PATPX[k], event.genJetPF2PATPY[k], event.genJetPF2PATPZ[k], event.genJetPF2PATE[k]};

                recoJetVec.emplace_back(std::make_pair(recoJet,genJetPid));

                pdgIdMapJets[std::abs(jetPid)]++;
                pdgIdMapGenJets[std::abs(genJetPid)]++;

                nJetsPerEventCounter++;

                if ( fromScalar ) { 
                    pdgIdMapJetsFromScalar[std::abs(jetPid)]++;
                    pdgIdMapGenJetsFromScalar[std::abs(genJetPid)]++;
                    recoJetVecFromScalar.emplace_back(std::make_pair(recoJet,genJetPid));
                    genJetVecFromScalar.emplace_back(std::make_pair(genJet,genJetPid));
                    nJetsFromScalarCounter++;
                }
                else {
                    pdgIdMapJetsNotFromScalar[std::abs(jetPid)]++;
                    pdgIdMapGenJetsNotFromScalar[std::abs(genJetPid)]++;
                }
            }

            ///////// jet multiplicity dependant jet stuff

            if ( nJetsFromScalarCounter > 0 ) {

                float genJetMassFromScalar {0.0}, genJet1MassFromScalar{0.0}, genJet2MassFromScalar{0.0};

                uint pionFlag {0}, kaonFlag {0}, photonFlag{0}, kChargedFlag{0}, kShortFlag{0}, kLongFlag{0};

                std::vector<int> kaonIndex{}, kShortIndex{};

                for (Int_t k{0}; k < event.numJetPF2PAT; k++) {
                    const Int_t genJetPid    {event.genJetPF2PATPID[k]};
                    const Int_t fromScalar   {event.genJetPF2PATScalarAncestor[k]};
                    if ( nJetsFromScalarCounter == 1 && fromScalar ) pdgIdMap1GenJetFromScalar[std::abs(genJetPid)]++;
                    else if ( nJetsFromScalarCounter == 2 && fromScalar ) pdgIdMap2GenJetsFromScalar[std::abs(genJetPid)]++;
                    else if ( nJetsFromScalarCounter == 3 && fromScalar ) pdgIdMap3GenJetsFromScalar[std::abs(genJetPid)]++;
                    else if ( nJetsFromScalarCounter == 4 && fromScalar ) pdgIdMap4GenJetsFromScalar[std::abs(genJetPid)]++;
                    else if ( nJetsFromScalarCounter == 5 && fromScalar ) pdgIdMap5GenJetsFromScalar[std::abs(genJetPid)]++;
                    else if ( nJetsFromScalarCounter == 6 && fromScalar ) pdgIdMap6GenJetsFromScalar[std::abs(genJetPid)]++;

                    if (fromScalar) {
                        genJetMassFromScalar += event.genJetPF2PATMass[k];
                        if ( nJetsFromScalarCounter == 1 ) genJet1MassFromScalar += event.genJetPF2PATMass[k];
                        if ( nJetsFromScalarCounter == 2 ) genJet2MassFromScalar += event.genJetPF2PATMass[k];
                    }

                    if ( nJetsFromScalarCounter == 2 && fromScalar ) {
                        if ( std::abs(genJetPid) == 321 ) { kaonFlag++; kChargedFlag++; kaonIndex.emplace_back(k);}
                        if ( std::abs(genJetPid) == 130 ) { kaonFlag++; kShortFlag++; kaonIndex.emplace_back(k);  kShortIndex.emplace_back(k);}
                        if ( std::abs(genJetPid) == 310 ) { kaonFlag++; kLongFlag++; kaonIndex.emplace_back(k);}
                        if ( std::abs(genJetPid) == 22  ) photonFlag++;
                        if ( std::abs(genJetPid) == 211 ) pionFlag++;

                    }
                }
                if ( kaonFlag == 2 ) {

                    const int k1 {kaonIndex[0]}, k2 {kaonIndex[1]};
                    const TLorentzVector genJetK1  {event.genJetPF2PATPX[k1], event.genJetPF2PATPY[k1], event.genJetPF2PATPZ[k1], event.genJetPF2PATE[k1]};
                    const TLorentzVector genJetK2  {event.genJetPF2PATPX[k2], event.genJetPF2PATPY[k2], event.genJetPF2PATPZ[k2], event.genJetPF2PATE[k2]};
                    const TLorentzVector recoJetK1 {event.jetPF2PATPx[k1], event.jetPF2PATPy[k1], event.jetPF2PATPz[k1], event.jetPF2PATE[k1]};
                    const TLorentzVector recoJetK2 {event.jetPF2PATPx[k2], event.jetPF2PATPy[k2], event.jetPF2PATPz[k2], event.jetPF2PATE[k2]};

                    h_genJetKaonDR->Fill(genJetK1.DeltaR(genJetK2), datasetWeight);
                    h_genJetKaonDPhi->Fill(genJetK1.DeltaPhi(genJetK2), datasetWeight);
                    h_genJetKaonDrEtaPhi->Fill(genJetK1.DrEtaPhi(genJetK2), datasetWeight);
                    h_recoJetKaonDR->Fill(recoJetK1.DeltaR(recoJetK2), datasetWeight);
                    h_recoJetKaonDPhi->Fill(recoJetK1.DeltaPhi(recoJetK2), datasetWeight);
                    h_recoJetKaonDrEtaPhi->Fill(recoJetK1.DrEtaPhi(recoJetK2), datasetWeight);

                    h_pidsFromScalarDecays->AddBinContent(1);
                    if ( kChargedFlag == 2 ) h_kaonsFromScalarDecays->AddBinContent(1);
                    if ( kChargedFlag == 1 && kShortFlag == 1 ) h_kaonsFromScalarDecays->AddBinContent(2);
                    if ( kChargedFlag == 1 && kLongFlag == 1 ) h_kaonsFromScalarDecays->AddBinContent(3);
                    if ( kShortFlag == 2 ) {
                        h_kaonsFromScalarDecays->AddBinContent(4);                
                        const int ks1 {kShortIndex[0]}, ks2 {kShortIndex[1]};
                        const TLorentzVector genJetKS1  {event.genJetPF2PATPX[ks1], event.genJetPF2PATPY[ks1], event.genJetPF2PATPZ[ks1], event.genJetPF2PATE[ks1]};
                        const TLorentzVector genJetKS2  {event.genJetPF2PATPX[ks2], event.genJetPF2PATPY[ks2], event.genJetPF2PATPZ[ks2], event.genJetPF2PATE[ks2]};
                        const TLorentzVector recoJetKS1 {event.jetPF2PATPx[ks1], event.jetPF2PATPy[ks1], event.jetPF2PATPz[ks1], event.jetPF2PATE[ks1]};
                        const TLorentzVector recoJetKS2 {event.jetPF2PATPx[ks2], event.jetPF2PATPy[ks2], event.jetPF2PATPz[ks2], event.jetPF2PATE[ks2]};
                        h_genJetKshortDR->Fill(genJetKS1.DeltaR(genJetKS2), datasetWeight);
                        h_genJetKshortDPhi->Fill(genJetKS1.DeltaPhi(genJetKS2), datasetWeight);
                        h_genJetKshortDrEtaPhi->Fill(genJetKS1.DrEtaPhi(genJetKS2), datasetWeight);
                        h_recoJetKshortDR->Fill(recoJetKS1.DeltaR(recoJetKS2), datasetWeight);
                        h_recoJetKshortDPhi->Fill(recoJetKS1.DeltaPhi(recoJetKS2), datasetWeight);
                        h_recoJetKshortDrEtaPhi->Fill(recoJetKS1.DrEtaPhi(recoJetKS2), datasetWeight);
                    }
                    if ( kShortFlag == 1 && kLongFlag == 1 ) h_kaonsFromScalarDecays->AddBinContent(5);
                    if ( kLongFlag == 2 ) h_kaonsFromScalarDecays->AddBinContent(6);
                }
                if ( kaonFlag == 1 && pionFlag == 1 ) h_pidsFromScalarDecays->AddBinContent(2);
                if ( kaonFlag == 1 && photonFlag == 1 ) h_pidsFromScalarDecays->AddBinContent(3);
                if ( pionFlag == 2 )  h_pidsFromScalarDecays->AddBinContent(4);
                if ( pionFlag == 1 && photonFlag == 1 ) h_pidsFromScalarDecays->AddBinContent(5);
                if ( photonFlag == 2 ) h_pidsFromScalarDecays->AddBinContent(6);
//                std::cout << "Scalar jet flags: kaonFlag = " << kaonFlag << "; pionFlag = " << pionFlag << "; photonFlag = " << photonFlag <<std::endl;
//                if ( pionFlag == 2 ) std::cout << "genJetPt: " << event.genJetPF2PATPT << std::endl;

                h_genJetMass->Fill(genJetMassFromScalar, datasetWeight);
                h_genJet1Mass->Fill(genJet1MassFromScalar, datasetWeight);
                h_genJet2Mass->Fill(genJet2MassFromScalar, datasetWeight);

                // All reco  jets
                TLorentzVector recoJetSumInvMass;
                for (auto it : recoJetVec ) {
                    recoJetSumInvMass += it.first;
                    h_recoJetInvMass->Fill(it.first.M(), datasetWeight);
                    h_recoJetPt->Fill(it.first.Pt(), datasetWeight);
                    h_recoJetEta->Fill(it.first.Eta(), datasetWeight);
                }
                h_recoJetSumInvMass->Fill(recoJetSumInvMass.M(), datasetWeight);

                // All reco jets matched to a gen jet descended from a sclaar
                TLorentzVector recoJetScalarSumInvMass, reco2JetScalarSumInvMass, reco2JetMinScalarSumInvMass;
                TLorentzVector pionRecoJetScalarSumInvMass, kaonRecoJetScalarSumInvMass;
                for (auto it : recoJetVecFromScalar ) {
                    recoJetScalarSumInvMass += it.first;
                    if ( nJetsFromScalarCounter == 2 ) reco2JetScalarSumInvMass += it.first;
                    if ( nJetsFromScalarCounter > 1 ) reco2JetMinScalarSumInvMass += it.first;
                    h_recoJetScalarInvMass->Fill(it.first.M(), datasetWeight);
                    h_recoJetScalarPt->Fill(it.first.Pt(), datasetWeight);
                    h_recoJetScalarEta->Fill(it.first.Eta(), datasetWeight);
                    const int pid = std::abs(it.second);
                    if (pid == 22) {
                        pionRecoJetScalarSumInvMass += it.first;
                        h_recoJetPionInvMass->Fill(it.first.M(), datasetWeight);
                        h_recoJetPionPt->Fill(it.first.Pt(), datasetWeight);
                        h_recoJetPionEta->Fill(it.first.Eta(), datasetWeight);
                    }
                    if (pid == 321 || pid == 130 || pid == 310 ) { 
                        kaonRecoJetScalarSumInvMass += it.first;
                        h_recoJetKaonInvMass->Fill(it.first.M(), datasetWeight);
                        h_recoJetKaonPt->Fill(it.first.Pt(), datasetWeight);
                        h_recoJetKaonEta->Fill(it.first.Eta(), datasetWeight);
                    }
                }
                if ( recoJetScalarSumInvMass.M() > 0.1 ) h_recoJetScalarSumInvMass->Fill(recoJetScalarSumInvMass.M(), datasetWeight);
                if ( reco2JetScalarSumInvMass.M() > 0.1 ) h_reco2JetScalarSumInvMass->Fill(reco2JetScalarSumInvMass.M(), datasetWeight);
                if ( reco2JetMinScalarSumInvMass.M() > 0.1 ) h_reco2JetMinScalarSumInvMass->Fill(reco2JetMinScalarSumInvMass.M(), datasetWeight);

                h_recoJetPionScalarInvMass->Fill(pionRecoJetScalarSumInvMass.M(), datasetWeight);
                h_recoJetKaonScalarInvMass->Fill(kaonRecoJetScalarSumInvMass.M(), datasetWeight);


                // All gen jets matched to a gen jet descended from a sclaar
                TLorentzVector genJetScalarSumInvMass, gen2JetScalarSumInvMass, gen2JetMinScalarSumInvMass;
                TLorentzVector genPionJetScalarSumInvMass, genKaonJetScalarSumInvMass;
                for (auto it : genJetVecFromScalar ) {
                    genJetScalarSumInvMass += it.first;
                    if ( nJetsFromScalarCounter == 2 ) gen2JetScalarSumInvMass += it.first;
                    if ( nJetsFromScalarCounter > 1 ) gen2JetMinScalarSumInvMass += it.first;
                    h_genJetScalarInvMass->Fill(it.first.M(), datasetWeight);
                    h_genJetScalarPt->Fill(it.first.Pt(), datasetWeight);
                    h_genJetScalarEta->Fill(it.first.Eta(), datasetWeight);
                    const int pid = std::abs(it.second);
                    if (pid == 22) {
                        genPionJetScalarSumInvMass += it.first;
                        h_genJetPionInvMass->Fill(it.first.M(), datasetWeight);
                        h_genJetPionPt->Fill(it.first.Pt(), datasetWeight);
                        h_genJetPionEta->Fill(it.first.Eta(), datasetWeight);
                    }
                    if (pid == 321 || pid == 130 || pid == 310 ) { 
                        genKaonJetScalarSumInvMass += it.first;
                        h_genJetKaonInvMass->Fill(it.first.M(), datasetWeight);
                        h_genJetKaonPt->Fill(it.first.Pt(), datasetWeight);
                        h_genJetKaonEta->Fill(it.first.Eta(), datasetWeight);
                    }
                }
                if ( genJetScalarSumInvMass.M() ) h_genJetScalarSumInvMass->Fill(genJetScalarSumInvMass.M(), datasetWeight);
                if ( gen2JetScalarSumInvMass.M() ) h_gen2JetScalarSumInvMass->Fill(gen2JetScalarSumInvMass.M(), datasetWeight);
                if ( gen2JetMinScalarSumInvMass.M() ) h_gen2JetMinScalarSumInvMass->Fill(gen2JetMinScalarSumInvMass.M(), datasetWeight);
                h_genJetPionScalarInvMass->Fill(genPionJetScalarSumInvMass.M(), datasetWeight);
                h_genJetKaonScalarInvMass->Fill(genKaonJetScalarSumInvMass.M(), datasetWeight);

                float recoJet1InvMass {0.0}, genJet1InvMass {0.0}, recoJet2InvMass {0.0}, genJet2InvMass {0.0};
                if ( nJetsFromScalarCounter == 1 ) {
                    for (auto it : recoJetVecFromScalar ) recoJet1InvMass += it.first.M();
                    for (auto it : genJetVecFromScalar )  genJet1InvMass += it.first.M();
                    h_recoJet1InvMass->Fill(recoJet1InvMass, datasetWeight);
                    h_genJet1InvMass->Fill(genJet1InvMass, datasetWeight);
                }
                if ( nJetsFromScalarCounter == 2 ) {
                    for (auto it : recoJetVecFromScalar ) recoJet2InvMass += it.first.M();
                    for (auto it : genJetVecFromScalar )  genJet2InvMass += it.first.M();
                    h_recoJet2InvMass->Fill(recoJet2InvMass, datasetWeight);
                    h_genJet2InvMass->Fill(genJet2InvMass, datasetWeight);
                }
            }

            nJetsFromScalar.emplace_back(nJetsFromScalarCounter);
            nJetsPerEvent.emplace_back(nJetsPerEventCounter);



            //////// GENERATOR PARTICLE STUFF

///
            int higgsIndex;
            std::vector< int > scalarIndex;
            std::vector< int > muonIndex;
            std::vector< int > pionIndex;
            std::vector< int > kaonIndex;
            std::vector< int > kShortIndex;

            for (Int_t k{0}; k < event.nGenPar; k++) {
                const Int_t pdgId    { std::abs(event.genParId[k]) };
		const Int_t status   { event.genParStatus[k] };
		const Int_t motherId { std::abs(event.genParMotherId[k]) };
		const Int_t numDaughters { event.genParNumDaughters[k] };
		const bool isOwnParent { pdgId == motherId ? true : false };
                const Int_t motherIndex  { std::abs(event.genParMotherIndex[k]) };

                const Float_t genParPt  { event.genParPt[k] };
                const Float_t genParEta { event.genParEta[k] };
                const Float_t genParPhi { event.genParPhi[k] };
                const Float_t genParE   { event.genParE[k] };
                TLorentzVector genParVec;
                genParVec.SetPtEtaPhiE(genParPt, genParEta, genParPhi, genParE);
                const Float_t genParMass {Float_t( genParVec.M() )};

                if ( pdgId == 9000006 ) scalarIndex.emplace_back(k);
                if ( pdgId == 25 ) higgsIndex = k;

                const bool isScalarGrandparent{ scalarGrandparent(event, k, 9000006) };

                if ( pdgId == 13  && isScalarGrandparent) muonIndex.emplace_back(k);
                if ( pdgId == 211 && isScalarGrandparent) pionIndex.emplace_back(k);

//                if ( motherId == 9000006 ) std::cout << "MOTHER IS SCALAR and has " << daughters << " daughters and status " << status << " and pdgId " << pdgId << std::endl;                
	  
		if ( numDaughters == 0 && (status == 1 || status == 2 || status == 71 || status == 72) ) pdgIdMap[pdgId]++;
		if (status == 1 && numDaughters == 0) pdgIdMapStatus1[pdgId]++;
		if (status == 2 && numDaughters == 0) pdgIdMapStatus2[pdgId]++;
                if (status == 23) {
                   uint localCounter {0};
                   for (Int_t m{0}; m < event.nGenPar; m++) {
                       if (event.genParStatus[m] == 23) localCounter++;
                   }
//                   if (localCounter == 4) pdgIdMapStatus23[pdgId]++;
                    pdgIdMapStatus23[pdgId]++;
                    nOutgoingStatusCounter++;
                    nOutgoingStatus23Counter++;
                }
                if (status == 33) {
                    pdgIdMapStatus33[pdgId]++;
                    nOutgoingStatusCounter++;
                    nOutgoingStatus33Counter++;
                }
		if ((status == 61 && status == 62 || status == 63) && numDaughters == 0) pdgIdMapStatus6X[pdgId]++;
		if ((status == 71 || status == 72 || status == 74) && numDaughters == 0) pdgIdMapStatus7X[pdgId]++;

                // particles directly outgoing from scalar decays
                if (motherId == 9000006 && ( status == 23 || status == 33) && !isOwnParent) pdgIdMapScalarMother[pdgId]++;

                // search ancestry of final state for final state particles from scalars
                if ( numDaughters == 0 ) {
                    if ( isScalarGrandparent == true ) {
                        pdgIdMapFromScalar[pdgId]++;
                        if (status == 1) pdgIdMapFromScalarStatus1[pdgId]++;
                        if (status == 23) pdgIdMapFromScalarStatus23[pdgId]++;
                        if (status == 33) pdgIdMapFromScalarStatus33[pdgId]++;
                        if (status == 71) pdgIdMapFromScalarStatus71[pdgId]++;
                    }
                }

                // search for physical particles from scalar decays before they decay
                if ( isScalarGrandparent == true ) {
                    const bool quarkParent = std::abs(motherId < 7);
                    const bool isQuark = std::abs(pdgId < 7);

                    if ( status == 1 && motherId == 9000006 ) pdgIdMapScalarDecayProducts[pdgId]++; // final state is quarks directly from scalar

                    // grouped colour singlet quarks
                    else if ( status == 23 && motherId == 9000006 && numDaughters == 0 ) pdgIdMapScalarDecayProducts[pdgId]++; // quarks that don't decay/get grouped into colour singlet
                    else if ( status == 71 && isOwnParent && numDaughters == 0 ) pdgIdMapScalarDecayProducts[pdgId]++; // quarks from status 23 quarks that don't decay further

                    // omega, eta, eta', omega, kshort
                    else if ( status == 2 && quarkParent && !isQuark ) pdgIdMapScalarDecayProducts[pdgId]++; // is from quark from scalar (i.e. colour grouping into physical particle)
                    else if ( status == 71 && quarkParent && !isQuark ) pdgIdMapScalarDecayProducts[pdgId]++; // is from quark from scalar (i.e. colour grouping into physical particle)

                    // k-short, pi+
                    else if ( status == 1 && quarkParent && !isQuark ) pdgIdMapScalarDecayProducts[pdgId]++; // is from quark from scalar and is final status

                    // if it is a kaon and is descended from scalar
                    if ( (pdgId == 130 || pdgId == 310 || pdgId == 311 || pdgId == 321) ) kaonIndex.emplace_back(k);
                    // if it is a kShort and is descended from scalar
                    if ( pdgId == 310 ) kShortIndex.emplace_back(k);

                    // kaon and pion distributions in pT, eta, phi
                    if (pdgId == 13) {
                        h_genParMuonPt->Fill(genParPt, datasetWeight);
                        h_genParMuonEta->Fill(genParEta, datasetWeight);
                        h_genParMuonPhi->Fill(genParPhi, datasetWeight);
                        h_genParMuonE->Fill(genParE, datasetWeight);
                        h_genParMuonMass->Fill(genParMass, datasetWeight);
                    }
                    else if ( (pdgId == 130 || pdgId == 310 || pdgId == 311 || pdgId == 321) ) {
                        h_genParKaonPt->Fill(genParPt, datasetWeight);
                        h_genParKaonEta->Fill(genParEta, datasetWeight);
                        h_genParKaonPhi->Fill(genParPhi, datasetWeight);
                        h_genParKaonE->Fill(genParE, datasetWeight);
                        h_genParKaonMass->Fill(genParMass, datasetWeight);
                        // If charged kaon
                        if ( pdgId == 321 ) {
                            h_genParChargedKaonPt->Fill(genParPt, datasetWeight);
                            h_genParChargedKaonEta->Fill(genParEta, datasetWeight);
                            h_genParChargedKaonPhi->Fill(genParPhi, datasetWeight);
                            h_genParChargedKaonE->Fill(genParE, datasetWeight);
                            h_genParChargedKaonMass->Fill(genParMass, datasetWeight);
                        }
                        // If neutral kaon
                        if ( pdgId == 311 ) {
                            h_genParNeutralKaonPt->Fill(genParPt, datasetWeight);
                            h_genParNeutralKaonEta->Fill(genParEta, datasetWeight);
                            h_genParNeutralKaonPhi->Fill(genParPhi, datasetWeight);
                            h_genParNeutralKaonE->Fill(genParE, datasetWeight);
                            h_genParNeutralKaonMass->Fill(genParMass, datasetWeight);
                        }
                        // If kShort
                        if ( pdgId == 310 ) {
                            h_genParKshortPt->Fill(genParPt, datasetWeight);
                            h_genParKshortEta->Fill(genParEta, datasetWeight);
                            h_genParKshortPhi->Fill(genParPhi, datasetWeight);
                            h_genParKshortE->Fill(genParE, datasetWeight);
                            h_genParKshortMass->Fill(genParMass, datasetWeight);
                        }
                        // If kLong
                        if ( pdgId == 130 ) {
                            h_genParKlongPt->Fill(genParPt, datasetWeight);
                            h_genParKlongEta->Fill(genParEta, datasetWeight);
                            h_genParKlongPhi->Fill(genParPhi, datasetWeight);
                            h_genParKlongE->Fill(genParE, datasetWeight);
                            h_genParKlongMass->Fill(genParMass, datasetWeight);
                        }
                    }
                    else if ( pdgId == 111 || pdgId == 211 ) {
                        h_genParPionPt->Fill(genParPt, datasetWeight);
                        h_genParPionEta->Fill(genParEta, datasetWeight);
                        h_genParPionPhi->Fill(genParPhi, datasetWeight);
                        h_genParPionE->Fill(genParE, datasetWeight);
                        h_genParPionMass->Fill(genParMass, datasetWeight);
                        // If charged pion
                        if ( pdgId == 211 ) {
                            h_genParChargedPionPt->Fill(genParPt, datasetWeight);
                            h_genParChargedPionEta->Fill(genParEta, datasetWeight);
                            h_genParChargedPionPhi->Fill(genParPhi, datasetWeight);
                            h_genParChargedPionE->Fill(genParE, datasetWeight);
                            h_genParChargedPionMass->Fill(genParMass, datasetWeight);
                        }
                        // If neutral pion
                        if ( pdgId == 111 ) {
                            h_genParNeutralPionPt->Fill(genParPt, datasetWeight);
                            h_genParNeutralPionEta->Fill(genParEta, datasetWeight);
                            h_genParNeutralPionPhi->Fill(genParPhi, datasetWeight);
                            h_genParNeutralPionE->Fill(genParE, datasetWeight);
                            h_genParNeutralPionMass->Fill(genParMass, datasetWeight);
                        }
                    }
                 }

//		if ( !daughters ) {
//              if ( isScalarGrandparent == true ) {
//		std::cout << "index / pdgId / mother / motherIndex / nDaughers / status: " << std::endl;
//  	        std::cout << k << " / " << pdgIdCode( pdgId, false ) << " / " << pdgIdCode( motherId, false ) << " / " << motherIndex << " / " << numDaughters << " / " << pythiaStatus( status ) << std::endl;
//              }
	    } ///

            if ( higgsIndex > -1 ) {
                TLorentzVector higgsVec;
                higgsVec.SetPtEtaPhiE(event.genParPt[higgsIndex], event.genParEta[higgsIndex], event.genParPhi[higgsIndex], event.genParE[higgsIndex]);
                h_higgsPt->Fill(event.genParPt[higgsIndex], datasetWeight);
                h_higgsEta->Fill(event.genParEta[higgsIndex], datasetWeight);
                h_higgsPhi->Fill(event.genParPhi[higgsIndex], datasetWeight);
                h_higgsMass->Fill(higgsVec.Pt(), datasetWeight);
            }
            if ( muonIndex.size() == 2 ) {
                const int index1 {muonIndex[0]}, index2 {muonIndex[1]};
                TLorentzVector muon1, muon2;
                muon1.SetPtEtaPhiE(event.genParPt[index1], event.genParEta[index1], event.genParPhi[index1], event.genParE[index1]);
                muon2.SetPtEtaPhiE(event.genParPt[index2], event.genParEta[index2], event.genParPhi[index2], event.genParE[index2]);
                h_muonDR->Fill(muon1.DeltaR(muon2));
            }
            if ( pionIndex.size() == 2 ) {
                const int index1 {pionIndex[0]}, index2 {pionIndex[1]};
                TLorentzVector pion1, pion2;
                pion1.SetPtEtaPhiE(event.genParPt[index1], event.genParEta[index1], event.genParPhi[index1], event.genParE[index1]);
                pion2.SetPtEtaPhiE(event.genParPt[index2], event.genParEta[index2], event.genParPhi[index2], event.genParE[index2]);
                h_pionDR->Fill(pion1.DeltaR(pion2));
            }
            if ( scalarIndex.size() == 2 ) {
                const int index1 {scalarIndex[0]}, index2 {scalarIndex[1]};
                TLorentzVector scalar1, scalar2;
                scalar1.SetPtEtaPhiE(event.genParPt[index1], event.genParEta[index1], event.genParPhi[index1], event.genParE[index1]);
                scalar2.SetPtEtaPhiE(event.genParPt[index2], event.genParEta[index2], event.genParPhi[index2], event.genParE[index2]);
                h_scalarDR->Fill(scalar1.DeltaR(scalar2), datasetWeight);
                h_scalarDPhi->Fill(scalar1.DeltaPhi(scalar2), datasetWeight);
                h_scalarDrEtaPhi->Fill(scalar1.DrEtaPhi(scalar2), datasetWeight);
                h_scalarPt->Fill(scalar1.Pt(), datasetWeight);
                h_scalarPt->Fill(scalar2.Pt(), datasetWeight);
                h_scalarEta->Fill(scalar1.Eta(), datasetWeight);
                h_scalarEta->Fill(scalar2.Eta(), datasetWeight);
                h_scalarPhi->Fill(scalar1.Phi(), datasetWeight);
                h_scalarPhi->Fill(scalar2.Phi(), datasetWeight);
                h_scalarMass->Fill(scalar1.M(), datasetWeight);
                h_scalarMass->Fill(scalar2.M(), datasetWeight);
            }

            if ( kaonIndex.size() == 2 ) {
                const int index1 {kaonIndex[0]}, index2 {kaonIndex[1]};
                TLorentzVector scalar1, scalar2;
                scalar1.SetPtEtaPhiE(event.genParPt[index1], event.genParEta[index1], event.genParPhi[index1], event.genParE[index1]);
                scalar2.SetPtEtaPhiE(event.genParPt[index2], event.genParEta[index2], event.genParPhi[index2], event.genParE[index2]);
                h_genParKaonDR->Fill(scalar1.DeltaR(scalar2), datasetWeight);
                h_genParKaonDPhi->Fill(scalar1.DeltaPhi(scalar2), datasetWeight);
                h_genParKaonDrEtaPhi->Fill(scalar1.DrEtaPhi(scalar2), datasetWeight);
            }

            if ( kShortIndex.size() == 2 ) {
                const int index1 {kShortIndex[0]}, index2 {kShortIndex[1]};
                TLorentzVector scalar1, scalar2;
                scalar1.SetPtEtaPhiE(event.genParPt[index1], event.genParEta[index1], event.genParPhi[index1], event.genParE[index1]);
                scalar2.SetPtEtaPhiE(event.genParPt[index2], event.genParEta[index2], event.genParPhi[index2], event.genParE[index2]);
                h_genParKshortDR->Fill(scalar1.DeltaR(scalar2), datasetWeight);
                h_genParKshortDPhi->Fill(scalar1.DeltaPhi(scalar2), datasetWeight);
                h_genParKshortDrEtaPhi->Fill(scalar1.DrEtaPhi(scalar2), datasetWeight);
            }

            nOutgoingStatus.emplace_back(nOutgoingStatusCounter);
            nOutgoingStatus23.emplace_back(nOutgoingStatus23Counter);
            nOutgoingStatus33.emplace_back(nOutgoingStatus33Counter);
//	    std::cout << std::endl;
	} 
    }

    std::cout << std::endl;
    std::cout << "Total no. of events:\t\t\t" << totalEvents << std::endl;
    std::cout << std::endl;

    // Do scalable histograms
    int nPdgIds                   = pdgIdMap.size();                   // number of different final state pdgIds
    int nPdgIdsStatus1            = pdgIdMapStatus1.size();            // number of different final state pdgIds with status 1
    int nPdgIdsStatus2            = pdgIdMapStatus2.size();            // number of different final state pdgIds with status 2
    int nPdgIdsStatus23           = pdgIdMapStatus23.size();           // number of different final state pdgIds with status 23
    int nPdgIdsStatus33           = pdgIdMapStatus33.size();           // number of different final state pdgIds with status 33
    int nPdgIdsStatus6X           = pdgIdMapStatus6X.size();           // number of different final state pdgIds with status 6X
    int nPdgIdsStatus7X           = pdgIdMapStatus7X.size();           // number of different final state pdgIds with status 7X
    int nPdgIdsScalarMother       = pdgIdMapScalarMother.size();       // number of different particles produced from scalar decay
    int nPdgIdsFromScalar         = pdgIdMapFromScalar.size();         // number of different final state pdgIds that are descended from scalar particle
    int nPdgIdsFromScalarStatus1  = pdgIdMapFromScalarStatus1.size();  // number of different final state pdgIds that are descended from scalar particle
    int nPdgIdsFromScalarStatus23 = pdgIdMapFromScalarStatus23.size(); // number of different final state pdgIds that are descended from scalar particle
    int nPdgIdsFromScalarStatus33 = pdgIdMapFromScalarStatus33.size(); // number of different final state pdgIds that are descended from scalar particle
    int nPdgIdsFromScalarStatus71 = pdgIdMapFromScalarStatus71.size(); // number of different final state pdgIds that are descended from scalar particle

    int nPdgIdsScalarDecayProducts = pdgIdMapScalarDecayProducts.size(); // number of physical decay products from scalars (before decaying)

    int nJetsFromScalarMax   = *std::max_element(nJetsFromScalar.begin(),nJetsFromScalar.end());
    int nJetsPerEventMax     = *std::max_element(nJetsPerEvent.begin(),nJetsPerEvent.end());
    int nOutgoingStatusMax   = *std::max_element(nOutgoingStatus.begin(),nOutgoingStatus.end());
    int nOutgoingStatus23Max = *std::max_element(nOutgoingStatus23.begin(),nOutgoingStatus23.end());
    int nOutgoingStatus33Max = *std::max_element(nOutgoingStatus33.begin(),nOutgoingStatus33.end());

    int nPdgIdsFromJets               = pdgIdMapJets.size();
    int nPdgIdsFromGenJets            = pdgIdMapGenJets.size();
    int nPdgIdsFromJetsFromScalar     = pdgIdMapJetsFromScalar.size();
    int nPdgIdsFromGenJetsFromScalar  = pdgIdMapGenJetsFromScalar.size();
    int nPdgIdsFromJetsNotFromScalar     = pdgIdMapJetsNotFromScalar.size();
    int nPdgIdsFromGenJetsNotFromScalar  = pdgIdMapGenJetsNotFromScalar.size();

    int nPdgIds1GenJetFromScalar   = pdgIdMap1GenJetFromScalar.size();
    int nPdgIds2GenJetsFromScalar  = pdgIdMap2GenJetsFromScalar.size();
    int nPdgIds3GenJetsFromScalar  = pdgIdMap3GenJetsFromScalar.size();
    int nPdgIds4GenJetsFromScalar  = pdgIdMap4GenJetsFromScalar.size();
    int nPdgIds5GenJetsFromScalar  = pdgIdMap5GenJetsFromScalar.size();
    int nPdgIds6GenJetsFromScalar  = pdgIdMap6GenJetsFromScalar.size();


    // status == 1 for final state particles
    // status == 2 for a decayed Standard Model hadron or tau or mu lepton, excepting virtual intermediate states thereof (i.e. the particle must undergo a normal decay, not e.g. a shower branching);
    // status == 61-63 for particles produced by beam-remnant treatment
    // status == 71 for partons in preparation of hadronization process and 72+74 (but exclude particles who are their own parent)

    TH1I* h_pdgId            {new TH1I{"h_pdgId",            "Final state content - all final state codes"   , nPdgIds,           0, Double_t(nPdgIds)           }};
    TH1I* h_pdgIdStatus1     {new TH1I{"h_pdgIdStatus1",     "Final state content - status code 1"           , nPdgIdsStatus1,    0, Double_t(nPdgIdsStatus1)    }};
    TH1I* h_pdgIdStatus2     {new TH1I{"h_pdgIdStatus2",     "Final state content - status code 2"           , nPdgIdsStatus2,    0, Double_t(nPdgIdsStatus2)    }};
    TH1I* h_pdgIdStatus23    {new TH1I{"h_pdgIdStatus23",    "Outgoing particles of hardest subprocess"      , nPdgIdsStatus23,   0, Double_t(nPdgIdsStatus23)   }};
    TH1I* h_pdgIdStatus33    {new TH1I{"h_pdgIdStatus33",    "Outgoing particles of subsequent subprocesses" , nPdgIdsStatus33,   0, Double_t(nPdgIdsStatus33)   }};
    TH1I* h_pdgIdStatus6X    {new TH1I{"h_pdgIdStatus6X",    "Final state content - status code 6X"          , nPdgIdsStatus6X,   0, Double_t(nPdgIdsStatus6X)   }};
    TH1I* h_pdgIdStatus7X    {new TH1I{"h_pdgIdStatus7X",    "Final state content - status code 7X"          , nPdgIdsStatus7X,   0, Double_t(nPdgIdsStatus7X)   }};

    TH1I* h_pdgIdScalarMother       {new TH1I{"h_pdgIdScalarMother",         "Direct decays from scalars"                      , nPdgIdsScalarMother, 0, Double_t(nPdgIdsScalarMother) }};

    TH1I* h_pdgIdFromScalar          {new TH1I{"h_pdgIdFromScalar",          "Final state content from scalars"                , nPdgIdsFromScalar, 0, Double_t(nPdgIdsFromScalar) }}; 
    TH1I* h_pdgIdFromScalarStatus1   {new TH1I{"h_pdgIdFromScalarStatus1",   "Final state content from scalars with status 1"  , nPdgIdsFromScalarStatus1, 0, Double_t(nPdgIdsFromScalarStatus1) }}; 
    TH1I* h_pdgIdFromScalarStatus23  {new TH1I{"h_pdgIdFromScalarStatus23",  "Final state content from scalars with status 23" , nPdgIdsFromScalarStatus23, 0, Double_t(nPdgIdsFromScalarStatus23) }}; 
    TH1I* h_pdgIdFromScalarStatus33  {new TH1I{"h_pdgIdFromScalarStatus33",  "Final state content from scalars with status 33" , nPdgIdsFromScalarStatus33, 0, Double_t(nPdgIdsFromScalarStatus33) }}; 
    TH1I* h_pdgIdFromScalarStatus71  {new TH1I{"h_pdgIdFromScalarStatus71",  "Final state content from scalars with status 71" , nPdgIdsFromScalarStatus71, 0, Double_t(nPdgIdsFromScalarStatus71) }}; 

    TH1* h_pdgIdMapScalarDecayProducts {new TH1I{"h_pdgIdMapScalarDecayProducts", "Physical decay products from scalar decays" , nPdgIdsScalarDecayProducts, 0, Double_t(nPdgIdsScalarDecayProducts) }};

    TH1I* h_outgoingStatus   {new TH1I{"h_outgoingStatus"  , "Number of outgoing particles - hardest subprocess"   , nOutgoingStatusMax+1   , -0.5, Double_t(nOutgoingStatusMax+0.5)   }};
    TH1I* h_outgoingStatus23 {new TH1I{"h_outgoingStatus23", "Number of outgoing particles - hardest subprocess"   , nOutgoingStatus23Max+1 , -0.5, Double_t(nOutgoingStatus23Max+0.5) }};
    TH1I* h_outgoingStatus33 {new TH1I{"h_outgoingStatus33", "Number of outgoing particles - hardest subprocess"   , nOutgoingStatus33Max+1 , -0.5, Double_t(nOutgoingStatus33Max+0.5) }};

    TH1I* h_jetsFromScalar   {new TH1I{"h_jetsFromScalar"  , "Number of jets per event from scalar decays"          , nJetsFromScalarMax+1   , -0.5, Double_t(nJetsFromScalarMax+0.5)   }}; 
    TH1I* h_jetsPerEvent     {new TH1I{"h_jetsPerEvent"    , "Number of jets per event"                             , nJetsPerEventMax+1     , -0.5, Double_t(nJetsPerEventMax+0.5)     }}; 

    TH1I* h_pdgIdMapJets              {new TH1I{"h_pdgIdMapJets",              "reco jet pid", nPdgIdsFromJets+1,    -0.5, Double_t(nPdgIdsFromJets+0.5)    }};
    TH1I* h_pdgIdMapGenJets           {new TH1I{"h_pdgIdMapGenJets",           "gen jet pid",  nPdgIdsFromGenJets+1, -0.5, Double_t(nPdgIdsFromGenJets+0.5) }};
    TH1I* h_pdgIdMapJetsFromScalar    {new TH1I{"h_pdgIdMapJetsFromScalar",    "reco jet which is descended from generator scalar", nPdgIdsFromJetsFromScalar+1,    -0.5, Double_t(nPdgIdsFromJetsFromScalar+0.5)    }};
    TH1I* h_pdgIdMapGenJetsFromScalar {new TH1I{"h_pdgIdMapGenJetsFromScalar", "gen jet which is descended from generator scalar",  nPdgIdsFromGenJetsFromScalar+1, -0.5, Double_t(nPdgIdsFromGenJetsFromScalar+0.5) }};
    TH1I* h_pdgIdMapJetsNotFromScalar    {new TH1I{"h_pdgIdMapJetsNotFromScalar",    "reco jet which is not descended from generator scalar", nPdgIdsFromJetsNotFromScalar+1,    -0.5, Double_t(nPdgIdsFromJetsNotFromScalar+0.5)    }};
    TH1I* h_pdgIdMapGenJetsNotFromScalar {new TH1I{"h_pdgIdMapGenJetsNotFromScalar", "gen jet which is not descended from generator scalar",  nPdgIdsFromGenJetsNotFromScalar+1, -0.5, Double_t(nPdgIdsFromGenJetsNotFromScalar+0.5) }};

    TH1I* h_pdgIdMap1GenJetFromScalar  {new TH1I{"h_pdgIdMap1GenJetFromScalar",  "1 gen jet pid descended from generator scalar",   nPdgIds1GenJetFromScalar+1,  -0.5, Double_t(nPdgIds1GenJetFromScalar+0.5) }};
    TH1I* h_pdgIdMap2GenJetsFromScalar {new TH1I{"h_pdgIdMap2GenJetsFromScalar", "2 gen jets pid descended from generator scalar",  nPdgIds2GenJetsFromScalar+1, -0.5, Double_t(nPdgIds2GenJetsFromScalar+0.5) }};
    TH1I* h_pdgIdMap3GenJetsFromScalar {new TH1I{"h_pdgIdMap3GenJetsFromScalar", "3 gen jets pid descended from generator scalar",  nPdgIds3GenJetsFromScalar+1, -0.5, Double_t(nPdgIds3GenJetsFromScalar+0.5) }};
    TH1I* h_pdgIdMap4GenJetsFromScalar {new TH1I{"h_pdgIdMap4GenJetsFromScalar", "4 gen jets pid descended from generator scalar",  nPdgIds4GenJetsFromScalar+1, -0.5, Double_t(nPdgIds4GenJetsFromScalar+0.5) }};
    TH1I* h_pdgIdMap5GenJetsFromScalar {new TH1I{"h_pdgIdMap5GenJetsFromScalar", "5 gen jets pid descended from generator scalar",  nPdgIds5GenJetsFromScalar+1, -0.5, Double_t(nPdgIds5GenJetsFromScalar+0.5) }};
    TH1I* h_pdgIdMap6GenJetsFromScalar {new TH1I{"h_pdgIdMap6GenJetsFromScalar", "6 gen jets pid descended from generator scalar",  nPdgIds6GenJetsFromScalar+1, -0.5, Double_t(nPdgIds6GenJetsFromScalar+0.5) }};


    uint binCounter {1};
    for (auto it = pdgIdMap.begin(); it != pdgIdMap.end(); ++it) {
//        std::cout << "Add " << it->second << " to bin " << binCounter << " for pdgId " << it->first << std::endl;
        h_pdgId->SetBinContent(binCounter, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
//        const char *label2 = ( pdgIdCode(it->first, true) ).c_str();
//        std::cout << "label : " << label2 << std::endl;
        h_pdgId->GetXaxis()->SetBinLabel(binCounter, label);
        binCounter++;
    }

    uint binCounterStatus1 {1};
    for (auto it = pdgIdMapStatus1.begin(); it != pdgIdMapStatus1.end(); ++it) {
        h_pdgIdStatus1->SetBinContent(binCounterStatus1, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus1->GetXaxis()->SetBinLabel(binCounterStatus1, label);
        binCounterStatus1++;
    }
    uint binCounterStatus2 {1};
    for (auto it = pdgIdMapStatus2.begin(); it != pdgIdMapStatus2.end(); ++it) {
        h_pdgIdStatus2->SetBinContent(binCounterStatus2, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus2->GetXaxis()->SetBinLabel(binCounterStatus2, label);
        binCounterStatus2++;
    }
    uint binCounterStatus23 {1};
    for (auto it = pdgIdMapStatus23.begin(); it != pdgIdMapStatus23.end(); ++it) {
        h_pdgIdStatus23->SetBinContent(binCounterStatus23, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus23->GetXaxis()->SetBinLabel(binCounterStatus23, label);
        binCounterStatus23++;
    }
    uint binCounterStatus33 {1};
    for (auto it = pdgIdMapStatus33.begin(); it != pdgIdMapStatus33.end(); ++it) {
        h_pdgIdStatus33->SetBinContent(binCounterStatus33, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus33->GetXaxis()->SetBinLabel(binCounterStatus33, label);
        binCounterStatus33++;
    }
    uint binCounterStatus6X {1};
    for (auto it = pdgIdMapStatus6X.begin(); it != pdgIdMapStatus6X.end(); ++it) {
        h_pdgIdStatus6X->SetBinContent(binCounterStatus6X, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus6X->GetXaxis()->SetBinLabel(binCounterStatus6X, label);
        binCounterStatus6X++;
    }
    uint binCounterStatus7X {1};
    for (auto it = pdgIdMapStatus7X.begin(); it != pdgIdMapStatus7X.end(); ++it) {
        h_pdgIdStatus7X->SetBinContent(binCounterStatus7X, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdStatus7X->GetXaxis()->SetBinLabel(binCounterStatus7X, label);
        binCounterStatus7X++;
    }

    uint binCounterScalarMother {1};
    for (auto it = pdgIdMapScalarMother.begin(); it != pdgIdMapScalarMother.end(); ++it) {
        h_pdgIdScalarMother->SetBinContent(binCounterScalarMother, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdScalarMother->GetXaxis()->SetBinLabel(binCounterScalarMother, label);
        binCounterScalarMother++;
    }

    uint binCounterFromScalar {1};
    for (auto it = pdgIdMapFromScalar.begin(); it != pdgIdMapFromScalar.end(); ++it) {
        h_pdgIdFromScalar->SetBinContent(binCounterFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdFromScalar->GetXaxis()->SetBinLabel(binCounterFromScalar, label);
        binCounterFromScalar++;
    }

    uint binCounterFromScalarStatus1 {1};
    for (auto it = pdgIdMapFromScalarStatus1.begin(); it != pdgIdMapFromScalarStatus1.end(); ++it) {
        h_pdgIdFromScalarStatus1->SetBinContent(binCounterFromScalarStatus1, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdFromScalarStatus1->GetXaxis()->SetBinLabel(binCounterFromScalarStatus1, label);
        binCounterFromScalarStatus1++;
    }

    uint binCounterFromScalarStatus23 {1};
    for (auto it = pdgIdMapFromScalarStatus23.begin(); it != pdgIdMapFromScalarStatus23.end(); ++it) {
        h_pdgIdFromScalarStatus23->SetBinContent(binCounterFromScalarStatus23, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdFromScalarStatus23->GetXaxis()->SetBinLabel(binCounterFromScalarStatus23, label);
        binCounterFromScalarStatus23++;
    }

    uint binCounterFromScalarStatus33 {1};
    for (auto it = pdgIdMapFromScalarStatus33.begin(); it != pdgIdMapFromScalarStatus33.end(); ++it) {
        h_pdgIdFromScalarStatus33->SetBinContent(binCounterFromScalarStatus33, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdFromScalarStatus33->GetXaxis()->SetBinLabel(binCounterFromScalarStatus33, label);
        binCounterFromScalarStatus33++;
    }

    uint binCounterFromScalarStatus71 {1};
    for (auto it = pdgIdMapFromScalarStatus71.begin(); it != pdgIdMapFromScalarStatus71.end(); ++it) {
        h_pdgIdFromScalarStatus71->SetBinContent(binCounterFromScalarStatus71, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdFromScalarStatus71->GetXaxis()->SetBinLabel(binCounterFromScalarStatus71, label);
        binCounterFromScalarStatus71++;
    }

    uint binCounterScalarDecayProducts {1};
    for (auto it = pdgIdMapScalarDecayProducts.begin(); it != pdgIdMapScalarDecayProducts.end(); ++it) {
        h_pdgIdMapScalarDecayProducts->SetBinContent(binCounterScalarDecayProducts, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapScalarDecayProducts->GetXaxis()->SetBinLabel(binCounterScalarDecayProducts, label);
        binCounterScalarDecayProducts++;        
    }

    for (auto it = nOutgoingStatus.begin(); it != nOutgoingStatus.end(); ++it) {
        h_outgoingStatus->Fill(*it);
    }
    for (auto it = nOutgoingStatus23.begin(); it != nOutgoingStatus23.end(); ++it) {
        h_outgoingStatus23->Fill(*it);
    }
    for (auto it = nOutgoingStatus33.begin(); it != nOutgoingStatus33.end(); ++it) {
        h_outgoingStatus33->Fill(*it);
    }

    for (auto it = nJetsFromScalar.begin(); it != nJetsFromScalar.end(); ++it) {
        h_jetsFromScalar->Fill(*it);
    }

    for (auto it = nJetsPerEvent.begin(); it != nJetsPerEvent.end(); ++it) {
        h_jetsPerEvent->Fill(*it);
    }

    uint binCounterJets {1};
    for (auto it = pdgIdMapJets.begin(); it != pdgIdMapJets.end(); ++it) {
        h_pdgIdMapJets->SetBinContent(binCounterJets, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapJets->GetXaxis()->SetBinLabel(binCounterJets, label);
        binCounterJets++;
    }
    uint binCounterGenJets {1};
    for (auto it = pdgIdMapGenJets.begin(); it != pdgIdMapGenJets.end(); ++it) {
        h_pdgIdMapGenJets->SetBinContent(binCounterGenJets, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapGenJets->GetXaxis()->SetBinLabel(binCounterGenJets, label);
        binCounterGenJets++;
    }

    uint binCounterJetsFromScalar {1};
    for (auto it = pdgIdMapJetsFromScalar.begin(); it != pdgIdMapJetsFromScalar.end(); ++it) {
        h_pdgIdMapJetsFromScalar->SetBinContent(binCounterJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapJetsFromScalar->GetXaxis()->SetBinLabel(binCounterJetsFromScalar, label);
        binCounterJetsFromScalar++;
    }
    uint binCounterGenJetsFromScalar {1};
    for (auto it = pdgIdMapGenJetsFromScalar.begin(); it != pdgIdMapGenJetsFromScalar.end(); ++it) {
        h_pdgIdMapGenJetsFromScalar->SetBinContent(binCounterGenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapGenJetsFromScalar->GetXaxis()->SetBinLabel(binCounterGenJetsFromScalar, label);
        binCounterGenJetsFromScalar++;
    }

    uint binCounterJetsNotFromScalar {1};
    for (auto it = pdgIdMapJetsNotFromScalar.begin(); it != pdgIdMapJetsNotFromScalar.end(); ++it) {
        h_pdgIdMapJetsNotFromScalar->SetBinContent(binCounterJetsNotFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapJetsNotFromScalar->GetXaxis()->SetBinLabel(binCounterJetsNotFromScalar, label);
        binCounterJetsNotFromScalar++;
    }
    uint binCounterGenJetsNotFromScalar {1};
    for (auto it = pdgIdMapGenJetsNotFromScalar.begin(); it != pdgIdMapGenJetsNotFromScalar.end(); ++it) {
        h_pdgIdMapGenJetsNotFromScalar->SetBinContent(binCounterGenJetsNotFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMapGenJetsNotFromScalar->GetXaxis()->SetBinLabel(binCounterGenJetsNotFromScalar, label);
        binCounterGenJetsNotFromScalar++;
    }

    uint binCounter1GenJetFromScalar {1};
    for (auto it = pdgIdMap1GenJetFromScalar.begin(); it != pdgIdMap1GenJetFromScalar.end(); ++it) {
        h_pdgIdMap1GenJetFromScalar->SetBinContent(binCounter1GenJetFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap1GenJetFromScalar->GetXaxis()->SetBinLabel(binCounter1GenJetFromScalar, label);
        binCounter1GenJetFromScalar++;
    }

    uint binCounter2GenJetsFromScalar {1};
    for (auto it = pdgIdMap2GenJetsFromScalar.begin(); it != pdgIdMap2GenJetsFromScalar.end(); ++it) {
        h_pdgIdMap2GenJetsFromScalar->SetBinContent(binCounter2GenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap2GenJetsFromScalar->GetXaxis()->SetBinLabel(binCounter2GenJetsFromScalar, label);
        binCounter2GenJetsFromScalar++;
    }

    uint binCounter3GenJetsFromScalar {1};
    for (auto it = pdgIdMap3GenJetsFromScalar.begin(); it != pdgIdMap3GenJetsFromScalar.end(); ++it) {
        h_pdgIdMap3GenJetsFromScalar->SetBinContent(binCounter3GenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap3GenJetsFromScalar->GetXaxis()->SetBinLabel(binCounter3GenJetsFromScalar, label);
        binCounter3GenJetsFromScalar++;
    }

    uint binCounter4GenJetsFromScalar {1};
    for (auto it = pdgIdMap4GenJetsFromScalar.begin(); it != pdgIdMap4GenJetsFromScalar.end(); ++it) {
        h_pdgIdMap4GenJetsFromScalar->SetBinContent(binCounter4GenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap4GenJetsFromScalar->GetXaxis()->SetBinLabel(binCounter4GenJetsFromScalar, label);
        binCounter4GenJetsFromScalar++;
    }

    uint binCounter5GenJetsFromScalar {1};
    for (auto it = pdgIdMap5GenJetsFromScalar.begin(); it != pdgIdMap5GenJetsFromScalar.end(); ++it) {
        h_pdgIdMap5GenJetsFromScalar->SetBinContent(binCounter5GenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap5GenJetsFromScalar->GetXaxis()->SetBinLabel(binCounter5GenJetsFromScalar, label);
        binCounter5GenJetsFromScalar++;
    }

    uint binCounter6GenJetsFromScalar {1};
    for (auto it = pdgIdMap6GenJetsFromScalar.begin(); it != pdgIdMap6GenJetsFromScalar.end(); ++it) {
        h_pdgIdMap6GenJetsFromScalar->SetBinContent(binCounter6GenJetsFromScalar, it->second);
        const char *label = ( pdgIdCode(it->first, false) ).c_str();
        h_pdgIdMap6GenJetsFromScalar->GetXaxis()->SetBinLabel(binCounter6GenJetsFromScalar, label);
        binCounter6GenJetsFromScalar++;
    }

    TFile* outFile{new TFile{outFileString.c_str(), "RECREATE"}};
    outFile->cd();

    h_pdgId->Write();

    h_pdgIdStatus1->Write();
    h_pdgIdStatus2->Write();
    h_pdgIdStatus23->Write();
    h_pdgIdStatus33->Write();
    h_pdgIdStatus6X->Write();
    h_pdgIdStatus7X->Write();
    h_pdgIdScalarMother->Write();
    h_pdgIdFromScalar->Write();
    h_pdgIdFromScalarStatus1->Write();
    h_pdgIdFromScalarStatus23->Write();
    h_pdgIdFromScalarStatus33->Write();
    h_pdgIdFromScalarStatus71->Write();
    h_pdgIdMapScalarDecayProducts->Write();
    h_outgoingStatus->Write();
    h_outgoingStatus23->Write();
    h_outgoingStatus33->Write();

    h_jetsFromScalar->Write();
    h_jetsPerEvent->Write();
    h_pdgIdMapJets->Write();
    h_pdgIdMapGenJets->Write();
    h_pdgIdMapJetsFromScalar->Write();
    h_pdgIdMapGenJetsFromScalar->Write();
    h_pdgIdMapJetsNotFromScalar->Write();
    h_pdgIdMapGenJetsNotFromScalar->Write();

    if ( nJetsFromScalarMax > 0 ) h_pdgIdMap1GenJetFromScalar->Write();
    if ( nJetsFromScalarMax > 1 ) h_pdgIdMap2GenJetsFromScalar->Write();
    if ( nJetsFromScalarMax > 2 ) h_pdgIdMap3GenJetsFromScalar->Write();
    if ( nJetsFromScalarMax > 3 ) h_pdgIdMap4GenJetsFromScalar->Write();
    if ( nJetsFromScalarMax > 4 ) h_pdgIdMap5GenJetsFromScalar->Write();
    if ( nJetsFromScalarMax > 5 ) h_pdgIdMap6GenJetsFromScalar->Write();

    h_pidsFromScalarDecays->Write();
    h_kaonsFromScalarDecays->Write();

    h_genParMuonPt->Write();
    h_genParMuonEta->Write();
    h_genParMuonPhi->Write();
    h_genParMuonE->Write();
    h_genParMuonMass->Write();
    h_genParKaonPt->Write();
    h_genParKaonEta->Write();
    h_genParKaonPhi->Write();
    h_genParKaonE->Write();
    h_genParKaonMass->Write();
    h_genParChargedKaonPt->Write();
    h_genParChargedKaonEta->Write();
    h_genParChargedKaonPhi->Write();
    h_genParChargedKaonE->Write();
    h_genParChargedKaonMass->Write();
    h_genParNeutralKaonPt->Write();
    h_genParNeutralKaonEta->Write();
    h_genParNeutralKaonPhi->Write();
    h_genParNeutralKaonE->Write();
    h_genParNeutralKaonMass->Write();
    h_genParKshortPt->Write();
    h_genParKshortEta->Write();
    h_genParKshortPhi->Write();
    h_genParKshortE->Write();
    h_genParKshortMass->Write();
    h_genParKlongPt->Write();
    h_genParKlongEta->Write();
    h_genParKlongPhi->Write();
    h_genParKlongE->Write();
    h_genParKlongMass->Write();
    h_genParPionPt->Write();
    h_genParPionEta->Write();
    h_genParPionPhi->Write();
    h_genParPionE->Write();
    h_genParPionMass->Write();
    h_genParChargedPionPt->Write();
    h_genParChargedPionEta->Write();
    h_genParChargedPionPhi->Write();
    h_genParChargedPionE->Write();
    h_genParChargedPionMass->Write();
    h_genParNeutralPionPt->Write();
    h_genParNeutralPionEta->Write();
    h_genParNeutralPionPhi->Write();
    h_genParNeutralPionE->Write();
    h_genParNeutralPionMass->Write();

    h_packedCandDxy->Write();
    h_packedCandDz->Write();
    h_packedCandVx->Write();
    h_packedCandVy->Write();
    h_packedCandVxy->Write();
    h_packedCandVz->Write();

    h_muonDR->Write();
    h_pionDR->Write();

    h_scalarDR->Write();
    h_scalarDPhi->Write();
    h_scalarDrEtaPhi->Write();
    h_scalarPt->Write();
    h_scalarEta->Write();
    h_scalarPhi->Write();
    h_scalarMass->Write();

    h_higgsPt->Write();
    h_higgsEta->Write();
    h_higgsPhi->Write();
    h_higgsMass->Write();

    h_genParKaonDR->Write();
    h_genParKaonDPhi->Write();
    h_genParKaonDrEtaPhi->Write();
    h_genJetKaonDR->Write();
    h_genJetKaonDPhi->Write();
    h_genJetKaonDrEtaPhi->Write();
    h_recoJetKaonDR->Write();
    h_recoJetKaonDPhi->Write();
    h_recoJetKaonDrEtaPhi->Write();

    h_genParKshortDR->Write();
    h_genParKshortDPhi->Write();
    h_genParKshortDrEtaPhi->Write();
    h_genJetKshortDR->Write();
    h_genJetKshortDPhi->Write();
    h_genJetKshortDrEtaPhi->Write();
    h_recoJetKshortDR->Write();
    h_recoJetKshortDPhi->Write();
    h_recoJetKshortDrEtaPhi->Write();

    h_recoJetSumInvMass->Write();
    h_recoJetInvMass->Write();
    h_recoJetPt->Write();
    h_recoJetEta->Write();

    h_recoJetScalarSumInvMass->Write();
    h_reco2JetScalarSumInvMass->Write();
    h_reco2JetMinScalarSumInvMass->Write();
    h_recoJetScalarInvMass->Write();
    h_recoJetScalarPt->Write();
    h_recoJetScalarEta->Write();

    h_genJetScalarSumInvMass->Write();
    h_gen2JetScalarSumInvMass->Write();
    h_gen2JetMinScalarSumInvMass->Write();
    h_genJetScalarInvMass->Write();
    h_genJetScalarPt->Write();
    h_genJetScalarEta->Write();

    h_recoJetPionInvMass->Write();
    h_recoJetPionScalarInvMass->Write();
    h_recoJetPionPt->Write();
    h_recoJetPionEta->Write();
    h_genJetPionInvMass->Write();
    h_genJetPionScalarInvMass->Write();
    h_genJetPionPt->Write();
    h_genJetPionEta->Write();
    h_recoJetKaonInvMass->Write();
    h_recoJetKaonScalarInvMass->Write();
    h_recoJetKaonPt->Write();
    h_recoJetKaonEta->Write();
    h_genJetKaonInvMass->Write();
    h_genJetKaonScalarInvMass->Write();
    h_genJetKaonPt->Write();
    h_genJetKaonEta->Write();

    h_genJetMass->Write();
    h_recoJet1InvMass->Write();
    h_genJet1InvMass->Write();
    h_genJet1Mass->Write();
    h_recoJet2InvMass->Write();
    h_genJet2InvMass->Write();
    h_genJet2Mass->Write();

    outFile->Close();

//    std::cout << "Max nGenPar: " << maxGenPars << std::endl;    
    auto timerStop = std::chrono::high_resolution_clock::now(); 
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;
}

std::string pythiaStatus (const Int_t status) {

   std::string message;
   message += std::to_string(status);

   switch (status) {

       case 0 : message += " : an empty entry, with no meaningful information"; break;
       case 1 : message += " : a final-state particle"; break;
       case 2 : message += " : a decayed Standard Model hadron or tau or mu lepton, excepting virtual intermediate states thereof"; break;
       case 3 : message += " : a documentation entry - not used in PYTHIA"; break;
       case 4 : message += " : an incoming beam particle"; break;

       case 11 : message += " : beam particles - the event as a whole";
           break;
       case 12 : message += " : beam particles - incoming beam";
           break;
       case 13 : message += " : beam particles - incoming beam-inside-beam (e.g. gamma inside e)";
           break;
       case 14 : message += " : beam particles - outgoing elastically scattered";
           break;
       case 15 : message += " : beam particles - outgoing diffractively scattered";
           break;
       case 21 : message += " : particles of the hardest subprocess - incoming";
           break;
       case 22 : message += " : particles of the hardest subprocess - intermediate (intended to have preserved mass)";
           break;
       case 23 : message += " : particles of the hardest subprocess - outgoing";
           break;
       case 24 : message += " : particles of the hardest subprocess - outgoing, nonperturbatively kicked out in diffraction";
           break;
       case 31 : message += " : particles of subsequent subprocesses - incoming";
           break;
       case 32 : message += " : particles of subsequent subprocesses - intermediate (intended to have preserved mass)";
           break;
       case 33 : message += " : particles of subsequent subprocesses - outgoing";
           break;
       case 34 : message += " : particles of subsequent subprocesses - incoming that has already scattered";
           break;
       case 41 : message += " : particles produced by initial-state-showers - incoming on spacelike main branch";
           break;
       case 42 : message += " : particles produced by initial-state-showers - incoming copy of recoiler";
           break;
       case 43 : message += " : particles produced by initial-state-showers - outgoing produced by a branching";
           break;
       case 44 : message += " : particles produced by initial-state-showers - outgoing shifted by a branching";
           break;
       case 45 : message += " : particles produced by initial-state-showers - incoming rescattered parton, with changed kinematics owing to ISR in the mother system (cf. status 34)";
           break;
       case 46 : message += " : particles produced by initial-state-showers - incoming copy of recoiler when this is a rescattered parton (cf. status 42)";
           break;
       case 47 : message += " : particles produced by initial-state-showers - a W or Z gauge boson produced in the shower evolution";
           break;
       case 51 : message += " : particles produced by final-state-showers - outgoing produced by parton branching";
           break;
       case 52 : message += " : particles produced by final-state-showers - outgoing copy of recoiler, with changed momentum";
           break;
       case 53 : message += " : particles produced by final-state-showers - copy of recoiler when this is incoming parton, with changed momentum";
           break;
       case 54 : message += " : particles produced by final-state-showers - copy of a recoiler, when in the initial state of a different system from the radiator";
           break;
       case 55 : message += " : particles produced by final-state-showers - copy of a recoiler, when in the final state of a different system from the radiator";
           break;
       case 56 : message += " : particles produced by final-state-showers - a W or Z gauge boson produced in a shower branching (special case of 51)";
           break;
       case 61 : message += " : particles produced by beam-remnant treatment - incoming subprocess particle with primordial kT included";
           break;
       case 62 : message += " : particles produced by beam-remnant treatment - outgoing subprocess particle with primordial kT included";
           break;
       case 63 : message += " : particles produced by beam-remnant treatment - outgoing beam remnant";
           break;
       case 71 : message += " : partons in preparation of hadronization process - copied partons to collect into contiguous colour singlet";
           break;
       case 72 : message += " : partons in preparation of hadronization process - copied recoiling singlet when ministring collapses to one hadron and momentum has to be reshuffled";
           break;
       case 73 : message += " : partons in preparation of hadronization process - combination of very nearby partons into one";
           break;
       case 74 : message += " : partons in preparation of hadronization process - combination of two junction quarks (+ nearby gluons) to a diquark";
           break;
       case 75 : message += " : partons in preparation of hadronization process - gluons split to decouple a junction-antijunction pair";
           break;
       case 76 : message += " : partons in preparation of hadronization process - partons with momentum shuffled to decouple a junction-antijunction pair";
           break;
       case 77 : message += " : partons in preparation of hadronization process - temporary opposing parton when fragmenting first two strings in to junction (should disappear again)";
           break;
       case 78 : message += " : partons in preparation of hadronization process - temporary combined diquark end when fragmenting last string in to junction (should disappear again)";
           break;
       case 81 : message += " : primary hadrons produced by hadronization process - from ministring into one hadron";
           break;
       case 82 : message += " : primary hadrons produced by hadronization process - from ministring into two hadron";
           break;
       case 83 : message += " : primary hadrons produced by hadronization process - from normal string (the difference between the two is technical, whether fragmented off from the top of the string system or from the bottom, useful for debug only)";
           break;
       case 84 : message += " : primary hadrons produced by hadronization process - from normal string (the difference between the two is technical, whether fragmented off from the top of the string system or from the bottom, useful for debug only)";
           break;
       case 85 : message += " : primary hadrons produced by hadronization process - primary produced hadrons in junction fragmentation of the first two string legs in to the junction, in order of treatment";
           break;
       case 86 : message += " : primary hadrons produced by hadronization process - primary produced hadrons in junction fragmentation of the first two string legs in to the junction, in order of treatment";
           break;
       case 91 : message += " : particles produced in decay process, or by Bose-Einstein effects - normal decay products";
           break;
       case 92 : message += " : particles produced in decay process, or by Bose-Einstein effects - decay products after oscillation";
           break;
       case 93 : message += " : particles produced in decay process, or by Bose-Einstein effects - decay handled by external program, normally or with oscillation";
           break;
       case 94 : message += " : particles produced in decay process, or by Bose-Einstein effects - decay handled by external program, normally or with oscillation";
           break;
       case 99 : message += " : particles produced in decay process, or by Bose-Einstein effects - particles with momenta shifted by Bose-Einstein effects";
           break;
       case 101 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - when a string system contains two such long-lived particles, the system is split up by the production of a new q-qbar pair";
           break;
       case 102 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - partons rearranged from the long-lived particle end to prepare for fragmentation from this end";
           break;
       case 103 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - intermediate half-R-hadron ormed when a colour octet particle (like the gluino) has been fragmented on one side, but not yet on the other";
           break;
       case 104 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - an R-hadron";
           break;
       case 105 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - partons or particles formed together with the R-hadron during the fragmentation treatment";
           break;
       case 106 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - subdivision of an R-hadron into its flavour content, with momentum split accordingly, in preparation of the decay of the heavy new particle, if it is unstable";
           break;
       case 107 : message += " : particles in the handling of R-hadron production and decay, i.e. long-lived (or stable) particles with very heavy flavour - two temporary leftover gluons joined into one in the formation of a gluino-gluon R-hadron";
           break;
//       default: message += " : unknown status code ";
   }

   return message;
}

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
//        std::cout << "Going up the ladder ... pdgId = " << pdgId << " : motherIndex = " << motherIndex << " : motherId = " << motherId << std::endl;
//        debugCounter++;
//        std::cout << "debugCounter: " << debugCounter << std::endl;
        return scalarGrandparent(event, motherIndex, grandparentId); // otherwise check mother's mother ...
    }
}

