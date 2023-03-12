#include "dataset.hpp"

#include "TChain.h"
#include "TColor.h"
#include "TFile.h"
#include "TH1.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = boost::filesystem;

Dataset::Dataset(std::string name, float lumi, bool isMC, float crossSection, std::vector<std::string> locations, std::string histoName, std::string treeName, std::string colourHex, std::string plotLabel, std::string plotType, std::string triggerFlag) : colour_{TColor::GetColor(colourHex.c_str())} {
    name_ = name;
    lumi_ = lumi;
    isMC_ = isMC;
    crossSection_ = crossSection;
    // TODO: Do something with the fileList here. This will build the TChain.
    fillName_ = histoName;
    treeName_ = treeName;
    locations_ = locations;
    plotType_ = plotType;
    plotLabel_ = plotLabel;
    triggerFlag_ = triggerFlag;
    generatorWeightPlot_ = nullptr;

    std::cout << "For dataset " << name_ << " trigger flag is " << triggerFlag_  << std::endl;

    for (auto& location : locations_) {
        if (location.back() != '/')
            location += '/';
    }

    // Read in generator level plots to determine event weights and total number of events
    if (isMC_) {
        const std::regex mask{R"(\.root$)"};
        bool firstFile{true};
        for (const auto& location : locations_) {
            for (const auto& file : boost::make_iterator_range(fs::directory_iterator{location}, {})) {
                const std::string path{file.path().string()};
                if (!fs::is_regular_file(file.status()) || !std::regex_search(path, mask))
                    continue;

                TFile* tempFile{new TFile{path.c_str(), "READ"}};
                if (firstFile) {
                    generatorWeightPlot_ = dynamic_cast<TH1I*>((TH1I*)tempFile->Get("makeTopologyNtupleMiniAOD/weightHisto")->Clone());
                    firstFile = false;
                }
                else {
                    generatorWeightPlot_->Add((TH1I*)tempFile->Get("makeTopologyNtupleMiniAOD/weightHisto"));
                    tempFile->Close();
                    delete tempFile;
                }
            }
        }
    }

    if (isMC_) totalEvents_ = generatorWeightPlot_->GetBinContent(1)+generatorWeightPlot_->GetBinContent(2);
}

// Method that fills a TChain with the files that will be used for the analysis.
// Returns 1 if succesful, otherwise returns 0. This can probably be largely
// ignored.
int Dataset::fillChain(TChain* chain,int flow, int fhigh) {
    for (const auto& location : locations_) {
        const fs::path dir{location};
        if (fs::is_directory(dir))
            //chain->Add(TString{location + "*.root"});
            {for (int i=flow; i<=fhigh; i++) chain->Add(TString{location + Form("output_%d.root",i)});}
        else {
            std::cout << "ERROR: " << location << "is not a valid directory" << std::endl;
            return 0;
        }
    }
    return 1;
}

// Function that returns the weight of a dataset. This is 1 is the dataset is
// data, but varies by lumi, number of events and cross section otherwise.
float Dataset::getDatasetWeight(double lumi) {
    if (!isMC_)
        return 1.;
    // return (lumi * crossSection_) / (totalEvents_ + 1.0e-06);
    return (lumi * crossSection_); // for now, we only include lumi*cross-section here; weight/Nevts added in main code
}

