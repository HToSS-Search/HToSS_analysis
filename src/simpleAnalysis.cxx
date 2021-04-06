#include "simpleAnalysisAlgo.hpp"


#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {

    auto timerStart = std::chrono::high_resolution_clock::now();  // timer used to figure out how long program takes to run

    SimpleAnalysis simpleAnalysis;
    simpleAnalysis.parseCommandLineArguements(argc, argv);
    simpleAnalysis.setupPlots();
    simpleAnalysis.runMainAnalysis();
    simpleAnalysis.savePlots();

    auto timerStop = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::seconds>(timerStop - timerStart);

    std::cout << "\nFinished. Took " << duration.count() << " seconds" <<std::endl;

}

