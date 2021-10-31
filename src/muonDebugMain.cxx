#include "TTree.h"
#include "muonDebugAlgo.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

int main(int argc, char* argv[])
{
    TTree::SetMaxTreeSize(std::numeric_limits<long long>::max());

    MuonDebugAlgo muonMain;

    muonMain.parseCommandLineArguements(argc, argv);
    muonMain.setup();
    muonMain.runMainAnalysis();
    muonMain.savePlots();
}
