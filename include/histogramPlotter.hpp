#ifndef _histogramPlotter_hpp_
#define _histogramPlotter_hpp_

#include "TPaveText.h"
#include "plots.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

class TH1D;
class TPad;

typedef struct datasetInfo datasetInfo;

class HistogramPlotter
{
    private:
    // A few things that govern the appearance of the plots.
    std::string lumiStr_;
    std::string outputFolder_; // Where the plots will be saved. Make sure to
                               // inclue the final /...
    std::string histogramDirectory_; // Where the histos will be saved. Make
                                     // sure to inclue the final /...
    std::string postfix_; // Will be appended to name of saved files. Defaults
                          // to aPostfix. Set with setter.
    const bool is2016_; // Era
    const bool is2018_; // Era
    bool loadHistos_;

    // Orders of various things and information regarding plotting.
    std::vector<std::string> plotOrder_;
    std::vector<std::string> legOrder_;
    std::map<std::string, datasetInfo> dsetMap_;

    // plots data/MC bit?
    const bool blindPlots_;

    std::vector<std::string>
        extensions_; // Will default to saving root and png files.
    // Labels for the plot.
    TPaveText* labelOne_;
    TPaveText* labelTwo_;
    TPaveText* labelThree_;

    TH1D* ratioHisto;
    TPad* canvy_1;
    TPad* canvy_2;

    // writes the lumi information and the CMS "logo" in the FigGuidelines style
    void CMS_lumi(TPad*, int = 10);
    void setTDRStyle();

    public:
    // Constructor
    HistogramPlotter(std::vector<std::string>,
                     std::vector<std::string>,
                     std::map<std::string, datasetInfo>,
                     const bool,
                     const bool,
                     const bool = true);
    ~HistogramPlotter();
    // methods to set various bits of information in the class. This is so that
    // it doesn't have to set in the constructor. Defaults to current stuff, but
    // can be changed outside.
    void setLabelTextSize(float size);
    void setLabelOne(std::string label)
    {
        labelOne_->SetLabel(label.c_str());
    }
    void setLabelTwo(std::string label)
    {
        labelTwo_->SetLabel(label.c_str());
    }
    void setLabelThree(std::string label)
    {
        labelThree_->SetLabel(label.c_str());
    }
    void setLumiStr(std::string lumiStr)
    {
        lumiStr_ = lumiStr;
    }
    void setPostfix(std::string postfix)
    {
        postfix_ = postfix;
    }
    void setOutputFolder(std::string output);
    void setHistogramFolder(std::string histoDir);
    void changeExtensions(std::vector<std::string> extentions)
    {
        extensions_ = extentions;
    }
    // Actual plotting commands
    void plotHistos(
        std::map<std::string, std::map<std::string, std::shared_ptr<Plots>>>);
    void loadHistos()
    {
        loadHistos_ = true;
    }
    std::map<std::string, TH1D*> loadCutFlowMap(std::string, std::string);
    void saveHistos(std::map<std::string, TH1D*>, std::string, std::string);
    void saveHistos(
        std::map<std::string, std::map<std::string, std::shared_ptr<Plots>>>);
    void plotCutFlows(std::map<std::string, TH1D*>);
    void makePlot(std::map<std::string, TH1D*>, std::string, std::string);
    void makePlot(std::map<std::string, TH1D*>,
                  std::string,
                  std::string,
                  std::string); // Adds a subLabel to the plot.
    void makePlot(
        std::map<std::string, TH1D*>,
        std::string,
        std::string,
        std::vector<std::string>); // Adds x-axis bin labels to the plot.
    void makePlot(
        std::map<std::string, TH1D*>,
        std::string,
        std::string,
        std::string,
        std::vector<std::string>); // Adds a subLabel AND bin labels to the
                                   // plot. Might get confusing later. May come
                                   // up with another name.
};

struct datasetInfo
{
    int colour;
    std::string legLabel;
    std::string legType;
};

#endif
