#include "TFile.h"
#include "TTree.h"
#include "G2TauTree_data23_analysis.h"
#include <iostream>

int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "Usage: ./analysis input.root output.root" << std::endl;
        return 1;
    }

    std::cout << "[INFO] Initializing G2TauTree_analysis." << std::endl;
    G2TauTree_data23_analysis analyzer(argv[1], argv[2]);
    std::cout << "[INFO] Starting Loop." << std::endl;
    analyzer.Loop();
    std::cout << "[INFO] Loop done." << std::endl;
    return 0;
}

