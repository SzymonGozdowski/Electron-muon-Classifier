// C++ script to demonstrate the K-Nearest Neighbors (kNN) classifier
// using the TMVA (Toolkit for Multivariate Analysis) package within ROOT.

#include <iostream>
#include <string>
#include <vector>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TCut.h" // Needed for selection cuts

// --- Main function to run TMVA ---
void testTrain() {
    // 1. Initialization and Setup
    TMVA::Tools::Instance();

    // Define the name of your input data file
    const char* inputFileName = "Classified_data_e_mu_file.root"; 
    
    // Attempt to open the input data file
    TFile* inputDataFile = TFile::Open(inputFileName);
    if (!inputDataFile || inputDataFile->IsZombie()) {
        std::cerr << "Error: Could not open input file: " << inputFileName << std::endl;
        std::cerr << "Please replace 'my_analysis_data.root' with the name of your file." << std::endl;
        return;
    }
    
    // Get the Signal and Background TTrees
    // IMPORTANT: Replace "Signal" and "Background" with your actual TTree names if they are different.
    TTree* signalTree = (TTree*)inputDataFile->Get("classified_data_e_mu");
    TTree* backgroundTree = (TTree*)inputDataFile->Get("classified_data_e_mu");

    if (!signalTree || !backgroundTree) {
        std::cerr << "Error: Could not load Signal or Background trees from " << inputFileName << "!" << std::endl;
        inputDataFile->Close();
        return;
    }
    
    std::cout << "--- Successfully loaded Signal Tree (" << signalTree->GetEntries() 
              << " entries) and Background Tree (" << backgroundTree->GetEntries() << " entries) ---" << std::endl;


    // Create a ROOT output file for the TMVA analysis results
    TFile* outputFile = TFile::Open("TMVA_KNN_Results.root", "RECREATE");

    // Create the TMVA Factory
    // AnalysisType=Classification is crucial for kNN
    TMVA::Factory* factory = new TMVA::Factory("TMVAKNNClassification", outputFile,
                                              "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");
    TMVA::DataLoader* dataloader = new TMVA::DataLoader("mydataset");
    
    // 2. Variable Definition
    // that you want to use for classification.
    dataloader->AddVariable("track_TRTHits", "track_TRTHits", "", 'F');
    dataloader->AddVariable("track_PixeldEdX", "track_PixeldEdX", "", 'F');
    dataloader->AddVariable("track_SCTHits", "track_SCTHits", "", 'F');
    dataloader->AddVariable("track_PixelHits", "track_PixelHits", "", 'F');
    

    // 3. Register Trees
    // Register the trees with the dataloader, specifying their weight (1.0 for equal importance)
    // You can apply a global weight if your samples need normalization.
    TCut signalSelection = "areElec == 1";
    TCut backgroundSelection = "areElec == 0"; 
    
    dataloader->AddSignalTree(signalTree, 3.0);
    dataloader->AddBackgroundTree(backgroundTree, 1.0);

    // 4. Data Splitting and Preparation
    // Define optional selection cuts (empty means use all events)
    TCut mycuts = "";
    TCut mycutb = "";

    // Apply the data preparation for training, testing, and validation
    // SplitMode=Random: randomly splits data for training and testing.
    // NormMode=NumEvents: Normalizes weights such that the total weight equals the number of events.
    // The default split is 50% for training and 50% for testing.
    dataloader->PrepareTrainingAndTestTree(signalSelection, backgroundSelection,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");

    // 5. Book the kNN Method
    // Options: 
    // K=20: Use 20 nearest neighbors. This is the most important parameter.
    // ScaleFrac=0.8: Use 80% of the training sample to define the kNN reference set (optional optimization).
    // EffMethod=None: Use simple counting for kNN score. Other options available (e.g., EffMethod=SampleSize).
    // Distance=MetricEuclid: Sets the distance metric. Other options include Manhattan, Chebyshev, etc.

    factory->BookMethod(dataloader, TMVA::Types::kKNN, "kNN", "nkNN=20");
    // 6. Training, Testing, and Evaluation
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    // 7. Cleanup
    outputFile->Close();
    inputDataFile->Close();
    delete factory;

    std::cout << "\n--- TMVA analysis finished. The results, weights, and plots can be found in TMVA_KNN_Results.root ---" << std::endl;
    std::cout << "--- To view the results, run in ROOT: TBrowser browser(\"TMVA_KNN_Results.root\") ---" << std::endl;
}
