#include "TFile.h"
#include "TTree.h"
#include "G2TauTree_data23_analysis.h"
#include <iostream>

int main(int argc, char** argv) {
    if(argc < 4) {
        std::cout << "Usage: ./analysis input.root output.root MLdata_output.root" << std::endl;
        return 1;
    }

    std::cout << "[INFO] Initializing G2TauTree_analysis." << std::endl;
    G2TauTree_data23_analysis analyzer(argv[1], argv[2], argv[3]);
    std::cout << "[INFO] Starting Loop." << std::endl;
    analyzer.Loop();
    return 0;
}

