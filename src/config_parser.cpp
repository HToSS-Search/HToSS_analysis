// config_parser.cpp
#include "config_parser.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <typeinfo>


void Parser::parse_config(const std::string conf, std::vector<Dataset>& datasets, double& lumi, const bool usePostLepTree, bool hack) {
    std::vector<std::string> datasetConfs;
    if (hack) {
        datasetConfs.push_back(conf);
    }
    else {
        std::cout<<"Going the traditional way"<<std::endl;
        const YAML::Node root{YAML::LoadFile(conf)};
        datasetConfs = root["datasets"].as<std::vector<std::string>>();
    }

    // std::cout<<"See config here (inside parser) -"<<conf<<","<<typeid(datasetConfs).name()<<std::endl;
    try {
        parse_files(datasetConfs, datasets, lumi, usePostLepTree);
    }
    catch (const std::exception) {
        std::cerr << "ERROR while parsing dataset file" << std::endl;
        throw;
    }
}
//testing below
// void Parser::parse_config(const std::string conf, const std::string cut, std::vector<Dataset>& datasets, double& lumi, const bool usePostLepTree, bool hack) {
//     std::vector<std::string> datasetConfs;
//     if (hack) {
//         datasetConfs.push_back(conf);
//     }
//     else {
//         std::cout<<"Going the traditional way"<<std::endl;
//         const YAML::Node root{YAML::LoadFile(conf)};
//         datasetConfs = root["datasets"].as<std::vector<std::string>>();
//     }

//     // std::cout<<"See config here (inside parser) -"<<conf<<","<<typeid(datasetConfs).name()<<std::endl;
//     try {
//         parse_files(datasetConfs, datasets, lumi, usePostLepTree);
//     }
//     catch (const std::exception) {
//         std::cerr << "ERROR while parsing dataset file" << std::endl;
//         throw;
//     }
// }

void Parser::parse_config(const std::string conf,
                          std::vector<Dataset>& datasets,
                          double& lumi,
                          std::vector<std::string>& plotTitles,
                          std::vector<std::string>& plotNames,
                          std::vector<float>& xMin,
                          std::vector<float>& xMax,
                          std::vector<int>& nBins,
                          std::vector<std::string>& fillExp,
                          std::vector<std::string>& xAxisLabels,
                          std::vector<int>& cutStage,
                          std::string& cutsConfName,
                          std::string& plotConfName,
                          std::string& outFolder,
                          std::string& postfix,
                          std::string& channel,
                          const bool usePostLepTree, 
                          const bool NPL)
{
    const YAML::Node root{YAML::LoadFile(conf)};

    auto datasetConfs{root["datasets"].as<std::vector<std::string>>()};

    try {
        parse_files(datasetConfs, datasets, lumi, usePostLepTree, NPL);
    }
    catch (const std::exception) {
        std::cerr << "ERROR while parsing dataset file" << std::endl;
        throw;
    }

    if (cutsConfName == "")
    {
        cutsConfName = root["cuts"].as<std::string>();
    }
    if (plotConfName == "") // If you haven't already chosen the plots to use,
                            // use the ones here.
    {
        plotConfName = root["plots"].as<std::string>();
    }

    try
    {
        parse_plots(plotConfName,
                    plotTitles,
                    plotNames,
                    xMin,
                    xMax,
                    nBins,
                    fillExp,
                    xAxisLabels,
                    cutStage);
    }
    catch (const std::exception)
    {
        std::cerr << "ERROR while parsing plot configuration" << std::endl;
        throw;
    }

    if (outFolder == "plots/" && root["outputFolder"])
    {
        outFolder = root["outputFolder"].as<std::string>();
    }
    if (postfix == "default" && root["outputPostfix"])
    {
        postfix = root["outputPostfix"].as<std::string>();
    }
    if (root["channelName"])
    {
        channel = root["channelName"].as<std::string>();
    }
}

// For reading the file config.
void Parser::parse_files(const std::vector<std::string> files,
                         std::vector<Dataset>& datasets,
                         double& totalLumi,
                         const bool usePostLepTree,
                         const bool NPL)
{
    std::cerr << "Adding datasets:" << std::endl;

    std::string treeName {"makeTopologyNtupleMiniAOD/tree"};
    if (usePostLepTree) treeName = "tree";
    //add handle here to check if old ntuple by checking if almorton in name
    for (const auto& file : files) {
        const YAML::Node root{YAML::LoadFile(file)};
        const bool isMC{root["mc"].as<bool>()};
        auto ntuple_loc{root["locations"].as<std::vector<std::string>>()[0]};
        std::size_t found = ntuple_loc.find("almorton"); //hack for old ntuples right now, modify yaml for a longtime solution
        bool isOldNtuple;
        if (found != std::string::npos) isOldNtuple=true;
        else isOldNtuple=false;
        // std::cout<<isOldNtuple<<" - isOldNtuple"<<std::endl;
        datasets.emplace_back(root["name"].as<std::string>(),
                              isMC ? 0 : root["luminosity"].as<double>(),
                              isMC,
                              isOldNtuple,
                              isMC ? root["cross_section"].as<double>() : 0,
                              root["locations"].as<std::vector<std::string>>(),
                              root["histogram"].as<std::string>(),
                              treeName,
                              root["colour"].as<std::string>(),
                              root["label"].as<std::string>(),
                              root["plot_type"].as<std::string>(),
                              isMC ? "" : root["trigger_flag"].as<std::string>());
        //This new element is constructed in place using args as the arguments for its constructor

        // If we are doing NPLs, add the NPL version of this dataset
        // if (NPL) {
        //     datasets.emplace_back(
        //         root["name"].as<std::string>(),
        //         0,
        //         isMC,
        //         isMC ? root["cross_section"].as<double>() : 0,
        //         root["locations"].as<std::vector<std::string>>(),
        //         "NPL",
        //         treeName,
        //         "#003300",
        //         "NPL",
        //         "f",
        //         isMC ? "" : root["trigger_flag"].as<std::string>());
        // }

        if (root["luminosity"])
            totalLumi += root["luminosity"].as<double>();

        std::cerr << datasets.back().name() << "\t(" << (isMC ? "MC" : "Data")
                  << ')' << std::endl;
    }
}

void Parser::parse_plots(const std::string plotConf,
                         std::vector<std::string>& plotTitles,
                         std::vector<std::string>& plotNames,
                         std::vector<float>& xMin,
                         std::vector<float>& xMax,
                         std::vector<int>& nBins,
                         std::vector<std::string>& fillExp,
                         std::vector<std::string>& xAxisLabels,
                         std::vector<int>& cutStage)
{
    const YAML::Node root{YAML::LoadFile(plotConf)};
    const YAML::Node plots{root["plots"]};

    for (YAML::const_iterator it = plots.begin(); it != plots.end(); ++it)
    {
        plotTitles.emplace_back((*it)["title"].as<std::string>());
        plotNames.emplace_back((*it)["name"].as<std::string>());
        xMin.emplace_back((*it)["xMin"].as<float>());
        xMax.emplace_back((*it)["xMax"].as<float>());
        nBins.emplace_back((*it)["nBins"].as<int>());
        fillExp.emplace_back((*it)["fillExp"].as<std::string>());
        xAxisLabels.emplace_back((*it)["xAxisLabel"].as<std::string>());
        cutStage.emplace_back((*it)["cutStage"].as<int>());
    }
}
