#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"

void Gym(const char* infile = "MLData.root")
{
    TMVA::Tools::Instance();
    
    TFile *f = TFile::Open(infile);
    TTree *tree = (TTree*)f->Get("MLDataTree");

    TFile *fout = TFile::Open("TMVA_KNN_output.root","RECREATE");

    TMVA::Factory    *factory    =
        new TMVA::Factory("KNN_test", fout,
                          "!V:!Silent:Transformations=I;:AnalysisType=Classification");

    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    dataloader->AddVariable("track_PixelHits", 'F');
    dataloader->AddVariable("track_TRTHits",  'F');
    dataloader->AddVariable("track_SCTHits",  'F');
    dataloader->AddVariable("track_PixeldEdX",  'F');


    TCut muonCut     = "IsMuon==1";
    TCut electronCut = "IsMuon==0";

    dataloader->AddSignalTree    (tree, 1.0);   
    dataloader->AddBackgroundTree(tree, 1.0);   

    dataloader->SetSignalWeightExpression    ("(IsMuon==1)");
    dataloader->SetBackgroundWeightExpression("(IsMuon==0)");

    dataloader->PrepareTrainingAndTestTree("IsMuon==1", "IsMuon==0",
        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");

    factory->BookMethod(dataloader, TMVA::Types::kKNN, "KNN_kernel",
        "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:!Trim");

    // trenowanie, test, ewaluacja
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fout->Close();
    delete factory;
    delete dataloader;
}
