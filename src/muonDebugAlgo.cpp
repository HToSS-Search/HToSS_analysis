#include "MiniMuonAnalysisEvent.hpp"
#include "Compression.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TMVA/Config.h"
#include "TMVA/Timer.h"
#include "TPad.h"
#include "TTree.h"
#include "muonDebugAlgo.hpp"
#include "config_parser.hpp"

#include <LHAPDF/LHAPDF.h>
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/program_options.hpp>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

MuonDebugAlgo::MuonDebugAlgo()
    : plots{false}
    , makeHistos{false}
    , useHistos{false}
    , channel{}
    , cutConfName{}
    , plotConfName{}
    , customJetRegion{false}
    , skipTrig{false}
    , skipScalarCut{false}
    , is2016_{false}
    , is2016APV_{false}
    , is2018_{false}
    , usingBparking_{false}
    , doABCD_{false}
    , doZplusCR_{false}
    , noData_ {true}
    , unblind_ {false}
{}

MuonDebugAlgo::~MuonDebugAlgo() {}

void MuonDebugAlgo::parseCommandLineArguements(int argc, char* argv[]){
    std::stringstream events;
    std::stringstream jetRegion;

    gErrorIgnoreLevel = kInfo;
    // Set up environment a little.
    std::cerr << std::setprecision(6) << std::fixed;
    std::cout << std::setprecision(6) << std::fixed;

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this message.")(
        "config,c",
        po::value<std::string>(&config)->required(),
        "The configuration file to be used.")(
        "2016",
        po::bool_switch(&is2016_),
        "Use 2016 conditions (SFs, et al.).")(
        "2016APV",
        po::bool_switch(&is2016APV_),
        "Use 2016 APV conditions (SFs, et al.).")(
        "2018",
        po::bool_switch(&is2018_),
        "Use 2018 conditions (SFs, et al.).")(
        ",n",
        po::value<long>(&nEvents)->default_value(0),
        "The number of events to be run over. All if set to 0.")(
        "allPlots,p", po::bool_switch(&plots), "Make all plots")(
        "makeHistos",
        po::bool_switch(&makeHistos),
        "Make histos to be used in future plots")(
        "useHistos",
        po::bool_switch(&useHistos),
        "Use saved histos to make plots")(
        "histoDir",
        po::value<std::string>(&histoDir)->default_value("histos/"),
        "The output directory for the histos used to make the plots.")(
        "outFolder,o",
        po::value<std::string>(&outFolder)->default_value("plots/"),
        "The output directory for the plots. Overrides the config file.")(
        "postfix,s",
        po::value<std::string>(&postfix)->default_value("default"),
        "Set postfix for plots. Overrides the config file.")(
        "lumi,l",
        po::value<double>(&usePreLumi)->default_value(0.),
        "Lumi to scale MC plots to.")(
        "cutConf,x",
        po::value<std::string>(&cutConfName),
        "Override the cut configuration given in the config file.")(
        "plotConf",
        po::value<std::string>(&plotConfName),
        "Override the plot configuration given in the config file. Sets --allPlots.")(
        "invertCharge,i",
        po::bool_switch(&invertLepCharge),
        "Inverts the charge requirement for leptons.")(
        "invertIso,j",
        po::bool_switch(&invertLepIso),
        "Inverts the iso requirement for leptons.")(
        "MC,m", po::bool_switch(&skipData), "Monte Carlo only mode. Ignores all data in the config file.")(
        "data,d", po::bool_switch(&skipMC), "Data only mode. Ignores all data in the config file.")(
        "ABCD", po::bool_switch(&doABCD_), "Make or use ABCD background estimation shapes")(
        "zPlus",po::bool_switch(&doZplusCR_), "Use Z+jets CR for dilepton channel. Region mwCut and metCut set by "
        "--mwCut and --metCut.")(
        "events,e",
        po::value<std::vector<int>>(&eventNumbers)->multitoken(),
        "Specify a space-separated list of events to run over.")(
        "channels,k",
        po::value<int>(&channelsToRun)->default_value(2),
        "Mask describing the channels to be run over. The mask "
        "is the sum of each channel's mask, which are:\n"
        "    ee        - 1\n"
        "    mumu      - 2\n"
        "    ee ss     - 4\n"
        "    mumu ss   - 8\n"
        "    emu       - 16\n"
        "    emu ss    - 32\n"
        "0 runs the channels specified in the config file")(
        "skipTrig", po::bool_switch(&skipTrig), "Skip running triggers.")(
        "skipScalarCut", po::bool_switch(&skipScalarCut), "Skip scalar mass cuts.")(
        "jetRegion",
        po::value<std::vector<unsigned>>(&jetRegVars),
        "Set a sustom jet region in the format NJETS NBJETS MAXJETS MAXBJETS.")(
        "metCut",
        po::value<float>(&metCut)->default_value(0),
        "Apply an MET cut. Dilepton Z+jets CR.")(
        "msCut",
        po::value<float>(&msCut)->default_value(4.),
        "Apply an mScalar cut.")(
        "dileptonDelR",
        po::value<float>(&dileptonDelR)->default_value(0.4),
        "Apply a max delR requirement on the dimuon pair.")(
        "chsMass",
        po::value<float>(&chsMass)->default_value(139.57018),
        "Set charged hadron candidate assumed mass (MeV).")(
        "mhCut",
        po::value<float>(&mhCut)->default_value(20.),
        "Apply an mHiggs cut.")(
        "unblind",
        po::bool_switch(&unblind_),
        "Remove blinding criteria! DO NOT USE UNLESS EXPRESS PERMISSION GRANTED")(
        "mwCut",
        po::value<float>(&mwCut)->default_value(20.),
        "Apply an mW cut. Dilepton only.");
    po::variables_map vm;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc;
            std::exit(0);
        }

        po::notify(vm);

        if ( is2016_ && !is2016APV_ && !is2018_) std::cout << "Running in 2016 mode" << std::endl; 
        if ( is2016APV_ && !is2016_ && !is2018_) std::cout << "Running in 2016 APV mode" << std::endl; 
        if ( !is2016_ && !is2018_ && !is2016APV_ ) std::cout << "Running in 2017 mode" << std::endl; 
        if ( is2018_ && !is2016_ && !is2016APV_) std::cout << "Running in 2018 mode" << std::endl; 

        if ( invertLepCharge ) std::cout << "Inverting lepton charge requirement" << std::endl;
        if ( invertLepIso ) std::cout << "Inverting lepton isolation requirement" << std::endl;

        if ( (is2016_ || is2016APV_) && is2018_ ) {
            throw std::logic_error(
                "Default condition is to use 2017. One cannot set "
                "condition to be BOTH 2016 AND 2018! Chose only "
                " one or none!");
        }
        if ( usingBparking_ && !is2018_ ) {
            throw std::logic_error(
                "Bparking is only avaliable in 2018 for Run 2!");
        }

        if (vm.count("channels") && !vm.count("config")) {
            throw std::logic_error(
                "--channels requires --config to be specified");
        }
        if (vm.count("jetRegion")) {
            if (jetRegVars.size() != 4) {
                throw std::logic_error(
                    "--jetRegion takes exactly four arguments.");
            }
            customJetRegion = true;
            std::cout << "CAUTION! Using a custom jet region of "
                      << jetRegVars[0] << "-" << jetRegVars[2] << " jets, and "
                      << jetRegVars[1] << "-" << jetRegVars[3] << " b-jets"
                      << std::endl;
        }
    }
    catch (const std::logic_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << "Use -h or --help for help." << std::endl;
        std::exit(1);
    }

    if (vm.count("plotConf")) {
        plots = true;
    }

    // Some vectors that will be filled in the parsing.
    totalLumi = 0;

    try {
        Parser::parse_config(config, datasets, totalLumi, plotTitles, plotNames, xMin, xMax, nBins, fillExp, xAxisLabels, cutStage, cutConfName, plotConfName, outFolder, postfix, channel, false,  doABCD_);
    }
    catch (const std::exception)  {
        std::cerr << "ERROR Problem with a confugration file, see previous "
                     "errors for more details. If this is the only error, the "
                     "problem is with the main configuration file."
                  << std::endl;
        throw;
    }

    if (channelsToRun)  {
        std::cout << "Running over the channels: " << std::endl;
        for (unsigned channelInd = 1; channelInd != 32; channelInd = channelInd << 1) {
            if (!(channelInd & channelsToRun) && channelsToRun)
                continue;
            if (channelInd & 5)// ee channel
                std::cout << "ee ";
            if (channelInd & 10) // mumu channel
                std::cout << "mumu ";
            if (channelInd & 3) // nominal samples
                std::cout << "nominal" << std::endl;
            if (channelInd & 12) // same sign samples
                std::cout << "same lepton charge" << std::endl;
            if (channelInd & 16) // nominal samples and emu
                std::cout << "emu - used only for ttbar control region " << std::endl;
            if (channelInd & 32) // nominal samples and emu
                std::cout << "emu - used only for ttbar same sign control region "  << std::endl;
        }
    }
}

void MuonDebugAlgo::setup() {
    if (is2016_ || is2016APV_) { // If 2016 mode, get 2016 PU
        // Make pileupReweighting stuff here
        dataPileupFile = new TFile{"pileup/2016/truePileupTest.root", "READ"};
        dataPU = dynamic_cast<TH1D*>(dataPileupFile->Get("pileup")->Clone());
        mcPileupFile = new TFile{"pileup/2016/pileupMC.root", "READ"};
        mcPU = dynamic_cast<TH1D*>(mcPileupFile->Get("pileup")->Clone());
    }
    else if (is2018_) {
        // Make pileupReweighting stuff here
        dataPileupFile = new TFile{"pileup/2018/truePileupTest.root", "READ"};
        dataPU = dynamic_cast<TH1D*>(dataPileupFile->Get("pileup")->Clone());
        mcPileupFile = new TFile{"pileup/2018/pileupMC.root", "READ"};
        mcPU = dynamic_cast<TH1D*>(mcPileupFile->Get("pileup")->Clone());
    }
    else { // If 2017 mode, get 2017 PU
        // Make pileupReweighting stuff here
        dataPileupFile = new TFile{"pileup/2017/truePileupTest.root", "READ"};
        dataPU = dynamic_cast<TH1D*>(dataPileupFile->Get("pileup")->Clone());
        mcPileupFile = new TFile{"pileup/2017/pileupMC.root", "READ"};
        mcPU = dynamic_cast<TH1D*>(mcPileupFile->Get("pileup")->Clone());
    }

    puReweight = dynamic_cast<TH1D*>(dataPU->Clone());
    puReweight->Scale(1.0 / puReweight->Integral());
    mcPU->Scale(1.0 / mcPU->Integral());
    puReweight->Divide(mcPU);
    puReweight->SetDirectory(nullptr);

    dataPileupFile->Close();
    mcPileupFile->Close();

    // Make cuts object. The methods in it should perhaps just be i nthe
    // AnalysisEvent class....
    cutObj = new Cuts{plots, plots, invertLepCharge, invertLepIso, is2016_, is2016APV_, is2018_};

    try {
        cutObj->parse_config(cutConfName);
    }
    catch (const std::exception) {
        std::cerr << "ERROR parsing cut configuration file" << std::endl;
        throw;
    }

    // For studying some trigger things. Default is false.
    cutObj->setSkipTrig(skipTrig);
    cutObj->setSkipScalarCut(skipScalarCut);
    if (customJetRegion) {
        cutObj->setJetRegion(
            jetRegVars[0], jetRegVars[1], jetRegVars[2], jetRegVars[3]);
    }
    cutObj->setMetCut(metCut);
    cutObj->setMWCut(mwCut);
    cutObj->setScalarCut(msCut);
    cutObj->setMaxDileptonDeltaR(dileptonDelR);
    cutObj->setHiggsCut(mhCut);
    cutObj->setChsCandidateMass(chsMass);
    cutObj->setBlindingFlag(unblind_);
    if (doZplusCR_) cutObj->setZplusControlRegionFlag(true);
    if (usingBparking_) cutObj->setBparking(true);
    cutObj->setMiniMuonDebugFlag(true);

    // Do a little initialisation for the plots here. Will later on be done in a
    // config file. Initialise plot stage names.
    stageNames.emplace_back(std::make_pair("noSel", "Pre-cuts"));
    stageNames.emplace_back(std::make_pair("trigSel", "HLT cuts"));
    stageNames.emplace_back(std::make_pair("lepSel", "Muon cuts"));
    stageNames.emplace_back(std::make_pair("zCand", "m_{#mu#mu} cut"));
}

void MuonDebugAlgo::runMainAnalysis() {

    TMVA::gConfig().SetDrawProgressBar(true);

    if (totalLumi == 0.) {
        if ( usePreLumi > 0.0 ) totalLumi = usePreLumi;
        else {
            if (is2016_) totalLumi = 16393.380531441;
            else if (is2016APV_) totalLumi = 19936.295040581;
            else if (is2018_ && !usingBparking_) totalLumi = 59819.714473511;
            else if (is2018_ && usingBparking_) totalLumi = 34790.3414;
            else totalLumi = 41477.877399292;
        }
    }

    std::cout << "Using lumi: " << totalLumi << std::endl;

    bool datasetFilled{false};

    std::string era {""};
    if (is2016_) era = "2016";
    else if (is2016APV_) era = "2016APV";
    else if (is2018_) era = "2018";
    else era = "2017";

    // Begin to loop over all datasets
    for (auto dataset = datasets.begin(); dataset != datasets.end(); ++dataset) {
        datasetFilled = false;
        TChain* datasetChain{new TChain{dataset->treeName().c_str()}};
        datasetChain->SetAutoSave(0);
        unsigned channelIndMax{256};

        const std::hash<std::string> hasher;
        srand(hasher(dataset->name()));

        channelIndMax = 8;
        for (unsigned channelInd{1}; channelInd != channelIndMax; channelInd = channelInd << 1) {
            if (!(channelInd & channelsToRun) && channelsToRun) {
                continue;
            }

            std::string chanName = channelSetup(channelInd);

            if (!dataset->isMC()) noData_ = false;

            if (dataset->isMC() && skipMC) {
                continue;
            }
            if (!dataset->isMC() && skipData) {
                continue;
            }

            if (plots) { // Initialise a load of stuff that's required by the plotting macro.

                // Gather all variables for plotting to make it easier to follow
                std::string histoName{dataset->getFillHisto()}, plotLabel{dataset->getPlotLabel()}, plotType{dataset->getPlotType()};
                int plotColour{dataset->getColour()};

                if (cutFlowMap.find(histoName) == cutFlowMap.end())
                {
                    const size_t numCutFlowBins{stageNames.size()};
                    cutFlowMap[histoName] = new TH1D{(histoName + "cutFlow").c_str(), (histoName + "cutFlow").c_str(), boost::numeric_cast<int>(numCutFlowBins), 0, boost::numeric_cast<double>(numCutFlowBins)};
                    if (datasetInfos.find(histoName) == datasetInfos.end()) {
                        legOrder.emplace_back(histoName);
                        plotOrder.emplace_back(histoName);
                        datasetInfos[histoName] = datasetInfo();
                        datasetInfos[histoName].colour = plotColour;
                        datasetInfos[histoName].legLabel = plotLabel;
                        datasetInfos[histoName].legType = plotType;
                    }
                     if (plots)  { // Only make all the plots if it's entirely necessary.
                         std::cout << "Made plots under " << histoName << " : " << channel << std::endl;
                         if (plotsMap.find(channel) == plotsMap.end())  {
                             plotsVec.emplace_back(channel);
                         }
                         plotsMap[channel][(histoName)] = {};
                         for (unsigned j{0}; j < stageNames.size(); j++) {
                             plotsMap[channel] [histoName][stageNames[j].first] = std::make_shared<Plots>(plotTitles, plotNames, xMin, xMax, nBins, fillExp, xAxisLabels, cutStage, j, histoName + "_" + stageNames[j].first +  "_" + channel);
                         }
                     }
                } // end cutFlow find loop
            } // end plots if statement

            // If making plots and using saved histos, skip running over the
            // datasets ...
            if (plots && useHistos)
                continue;

            // If making either plots, make cut flow object.
            std::cerr << "Processing dataset " << dataset->name() << std::endl;
            if (!datasetFilled) {
                if (!dataset->fillChain(datasetChain)) {
                    std::cerr << "There was a problem constructing the chain for " << dataset->name() << ". Continuing with next dataset.\n";
                    continue;
                }
                datasetFilled = true;
            }

            cutObj->setMC(dataset->isMC());
            cutObj->setTriggerFlag(dataset->getTriggerFlag());
            std::cout << "Trigger flag: " << dataset->getTriggerFlag() << std::endl;

            // Here we will initialise the generator level weight histograms
            TH1I* generatorWeightPlot{nullptr};
            if (dataset->isMC()) {
                generatorWeightPlot = dynamic_cast<TH1I*>(dataset->getGeneratorWeightHistogram()->Clone());
                generatorWeightPlot->SetDirectory(nullptr);
            }

            // extract the dataset weight. MC =
            // (lumi*crossSection)/(totalEvents), data = 1.0
            float datasetWeight{dataset->getDatasetWeight(totalLumi)};
//            float datasetWeight = 1.;
            std::cout << datasetChain->GetEntries() << " number of items in tree. Dataset weight: " << datasetWeight << std::endl;
            if (datasetChain->GetEntries() == 0) {
                std::cout << "No entries in tree, skipping..." << std::endl;
                continue;
            }
            
            AnalysisEvent event{dataset->isMC(), datasetChain, (is2016_ || is2016APV_), is2018_};

            // Adding in some stuff here to make a skim file out of post lep sel
            // stuff
            TFile* outFile1{nullptr};
            TTree* cloneTree{nullptr};

            double eventWeight{0.};

            long long numberOfEvents{datasetChain->GetEntries()};
            if (nEvents && nEvents < numberOfEvents) {
                numberOfEvents = nEvents;
            }
            //    datasetChain->Draw("numElePF2PAT","numMuonPF2PAT > 2");
            //    TH1F * htemp = (TH1F*)gPad->GetPrimitive("htemp");
            //    htemp->SaveAs("tempCanvas.png");
            int foundEvents{0};
            double foundEventsNorm{0.0};

            // If event is amc@nlo, need to sum number of positive and negative weights first.
            if (dataset->isMC()) {
                // Load in plots
                sumPositiveWeights_          = generatorWeightPlot->GetBinContent(1)  + generatorWeightPlot->GetBinContent(2);
                sumNegativeWeights_          = generatorWeightPlot->GetBinContent(1)  - generatorWeightPlot->GetBinContent(2);
                if (sumNegativeWeights_ > sumPositiveWeights_) {
                    std::cout << "Something SERIOUSLY went wrong here - the number of postitive weights minus negative ones is greater than their sum?!" << std::endl;
                    std::cout << "number of postitive weights: " << sumPositiveWeights_ << std::endl;
                    std::cout
                        << "number of negative weights: " << sumNegativeWeights_
                        << std::endl;
                    std::exit(999);
                }
            }

            // Some samples lack LHE info - the LHE flag needs to be set so that the samples are correctly treated
            bool hasLHE {true};
            if ( dataset->name() == "ZG_lnug"
              || dataset->name() == "tHq"
              || dataset->name() == "tWZ" 
              || dataset->name() == "QCD_Pt-20toInf_MuEnrichedPt15"
              || dataset->name() == "QCD_Pt-1000toInf_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-120to170_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-120to170_MuEnrichedPt5_raw"
              || dataset->name() == "QCD_Pt-15to20_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-170to300_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-20to30_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-300to470_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-30to50_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-470to600_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-50to80_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-600to800_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-800to1000_MuEnrichedPt5"
              || dataset->name() == "QCD_Pt-80to120_MuEnrichedPt5_raw"
              || dataset->name() == "QCD_Pt-80to120_MuEnrichedPt5") {
               hasLHE = false;
            }
            TMVA::Timer* lEventTimer{new TMVA::Timer{boost::numeric_cast<int>(numberOfEvents), "Running over dataset ...", false}};
            lEventTimer->DrawProgressBar(0, "");
            std::cout << "Number of events: " << numberOfEvents << std::endl;
            for (int i{0}; i < numberOfEvents; i++) {
                std::stringstream lSStrFoundEvents;
                lSStrFoundEvents << foundEvents;
                lEventTimer->DrawProgressBar(i, ("Found " + lSStrFoundEvents.str() + " events."));
                event.GetEntry(i);
                // Do the systematics indicated by the systematic flag, oooor
                // just do data if that's your thing. Whatevs.
                int systMask{1};
                eventWeight = 1.;

                // apply generator weights here.
                double generatorWeight{1.0};
                if (dataset->isMC() && hasLHE) {
                    generatorWeight = (sumPositiveWeights_) / (sumNegativeWeights_) * (event.origWeightForNorm / std::abs(event.origWeightForNorm));
                }

                eventWeight *= generatorWeight;

                // apply pileup weights here.
                if (dataset->isMC()) { // no weights applied for synchronisation
                   double pileupWeight{puReweight->GetBinContent(puReweight->GetXaxis()->FindBin(event.numVert))};
                   eventWeight *= pileupWeight;
                   // std::cout << "pileupWeight: " <<  pileupWeight <<
                   // std::endl;
                }
 
                // Scale according to lumi
                eventWeight *= datasetWeight;
                // apply negative weighting for SameSign MC lepton samples
                // so that further downstream
//                if (dataset->isMC() && invertLepCharge && !plots) {
//                    eventWeight *= -1.0; // Should NOT be done when plotting non-prompts - separate code for that
//                }

                if (dataset->name() == "ttbarInclusivePowerheg"
                        || dataset->name()
                               == "ttbarInclusivePowerheg_colourFlip"
                        || dataset->name() == "ttbarInclusivePowerheg_hdampUP"
                        || dataset->name() == "ttbarInclusivePowerheg_hdampDown"
                        || dataset->name() == "ttbarInclusivePowerheg_fsrup"
                        || dataset->name() == "ttbarInclusivePowerheg_fsrdown"
                        || dataset->name() == "ttbarInclusivePowerheg_isrup"
                        || dataset->name() == "ttbarInclusivePowerheg_isrdown"
                        || dataset->name() == "ttbar_2l2v"
                        || dataset->name() == "ttbar_hadronic"
                        || dataset->name() == "ttbar_semileptonic")
                {
                   eventWeight *= event.topPtReweight;
                }

                std::string histoName{dataset->getFillHisto()};


                cutObj->makeCuts(event, eventWeight, plotsMap[channel][histoName], *cutFlowMap[histoName], 0);

                foundEvents++;
                foundEventsNorm += eventWeight;

            } // end event loop

            std::cerr << "\nFound " << foundEvents << " in " << dataset->name() << std::endl;
            std::cerr << "Found " << foundEventsNorm << " after normalisation in " << dataset->name() << std::endl;
            std::cerr << "\n\n";
            // Delete generator level plot. Avoid memory leaks, kids.
            delete generatorWeightPlot;
            generatorWeightPlot = nullptr;

            // datasetChain->MakeClass("AnalysisEvent");
        } // end channel loop.
        delete datasetChain;
    } // end dataset loop
}

void MuonDebugAlgo::savePlots() {
    // Save all plot objects. For testing purposes.

    // Now test out the histogram plotter class I just wrote.
    // Make the plotting object.
    if (plots) {
        HistogramPlotter plotObj = HistogramPlotter(legOrder, plotOrder, datasetInfos, (is2016_ || is2016APV_), is2018_, noData_);

        // If either making or reading in histos, then set the correct read in
        // directory
        if ( makeHistos || (useHistos && plots) ) {
            plotObj.setHistogramFolder(histoDir);
        }

        // If making histos, save the output!
        if (makeHistos) {
            std::cout << "Saving histograms for later use ..." << std::endl;
            for (unsigned i{0}; i < plotsVec.size(); i++) {
                plotObj.saveHistos(plotsMap[plotsVec[i]]);
            }
            plotObj.saveHistos(cutFlowMap, "cutFlow", channel); // Don't forget to save the cutflow too!
        }

        if (!makeHistos) {
            if (useHistos) {
                plotObj.loadHistos(); // If using saved histos, read them in ...
            }
            plotObj.setLabelOne("CMS Preliminary");
            plotObj.setLabelTwo("Some amount of lumi");
            plotObj.setPostfix("");
            plotObj.setOutputFolder(outFolder);

            for (unsigned i{0}; i < plotsVec.size(); i++) {
                std::cout << plotsVec[i] << std::endl;
                if (plots)
                {
                    plotObj.plotHistos(plotsMap[plotsVec[i]]);
                }
            }

            // cut flow x axis labels
            std::vector<std::string> cutFlowLabels;
            for (std::vector<std::pair<std::string, std::string>>::const_iterator lIt = stageNames.begin(); lIt != stageNames.end(); ++lIt) {
                cutFlowLabels.emplace_back((*lIt).second);
            }
            if (useHistos) {
                cutFlowMap = plotObj.loadCutFlowMap("cutFlow", channel);
            }
            plotObj.makePlot(
                cutFlowMap, "data/MC Yield", "cutFlow", cutFlowLabels);
        }
    }

    // Delete all the plot objects.

    std::cerr << "Gets to the delete bit" << std::endl;
    std::cerr << "But not past it" << std::endl;
}

std::string MuonDebugAlgo::channelSetup(unsigned channelInd) {
    std::string chanName{};

    if (channelsToRun) {
        if (channelInd & 1) { // ee channels
            cutObj->setNumLeps(0, 0, 2, 2);
            cutObj->setCutConfTrigLabel("e");
            channel = "ee";
            postfix = "ee";
            chanName += "ee";
        }
        if (channelInd & 2) { // mumu channels
            cutObj->setNumLeps(2, 2, 0, 0);
            cutObj->setCutConfTrigLabel("m");
            channel = "mumu";
            postfix = "mumu";
            chanName += "mumu";
        }
        if (channelInd & 4) { // emu channel for ttbar background estimation
            cutObj->setNumLeps(1, 1, 1, 1);
            cutObj->setCutConfTrigLabel("d");
            channel = "emu";
            postfix = "emu";
            chanName += "emu";
        }
    }
    return chanName;
}
