#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
#include <TLegend.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include <onnxruntime_cxx_api.h>

#include "../Topoclusters/TopoclusterHist.C"

void CheckProtonCluster()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    const Float_t ProtonMass = 0.93827208816;

    TString File   = "../Data/data23_2trk_moreTCvars.root";
    std::string name = "CheckProtonCluster";


    const int PROTON_CLUSTER_ID = 1;


    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "CheckProtonCluster");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    const char* onnx_path = "../ONNX/UnsupervisedClusterModel.onnx";
    Ort::Session session(env, onnx_path, session_options);
    const char* input_name        = "input";
    const char* output_name_label = "label";


    Ort::Session session2(env, "../ONNX/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session2.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session2.GetOutputNameAllocated(0, allocator);
    const char* input_name2     = input_name_alloc.get();
    const char* output_names2[] = {"label", "probabilities"};


    const int64_t n_features = 8;
    std::array<int64_t, 2> input_shape{1, n_features};

    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    auto ClassifyTrack = [&](std::array<float, 8>& feats) -> int64_t {
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            memory_info, feats.data(), feats.size(),
            input_shape.data(), input_shape.size());

        std::vector<const char*> input_names{input_name};
        std::vector<const char*> output_names{output_name_label};

        auto output_tensors = session.Run(
            Ort::RunOptions{nullptr},
            input_names.data(), &input_tensor, 1,
            output_names.data(), 1);

        int64_t* label_ptr = output_tensors.front().GetTensorMutableData<int64_t>();
        return label_ptr[0];
    };


    TH1D *InvMass_All           = new TH1D("InvMass_All",
        "Masa niezmiennicza pary - wszystkie;M_{ll} [GeV];Events", 100, 3, 3.2);
    TH1D *InvMass_ClusterProton = new TH1D("InvMass_ClusterProton",
        "Masa niezmiennicza pary - >=1 tor sklasyfikowany jako proton;M_{ll} [GeV];Events", 100, 3, 3.2);
    TH1D *ClusterLabelHist      = new TH1D("ClusterLabelHist",
        "Rozklad predykcji klastra na torach;Klaster;Tory", 5, -0.5, 4.5);


    TChain *mychain = new TChain("G2TauTree");
    mychain->Add(File);
    TTreeReader tree_reader(mychain);

    TTreeReaderArray<unsigned int> TrackNum(tree_reader, "track_n");
    TTreeReaderArray<float> TrackEta(tree_reader, "track_eta");
    TTreeReaderArray<float> TrackPhi(tree_reader, "track_phi");
    TTreeReaderArray<float> TrackPt(tree_reader, "track_pt");
    TTreeReaderArray<int> TrackCharge(tree_reader, "track_charge");

    TTreeReaderArray<float> TrackPixeldEdX(tree_reader, "track_PixeldEdX");
    TTreeReaderArray<float> TrackPixelHits(tree_reader, "track_PixelHits");
    TTreeReaderArray<float> TrackSCTHits(tree_reader, "track_SCTHits");
    TTreeReaderArray<float> TrackTRTHits(tree_reader, "track_TRTHits");

    TTreeReaderArray<unsigned int> TopoCluNum(tree_reader, "topoclus_n");
    TTreeReaderArray<float> TopoCluEta(tree_reader, "topo_cluster_eta");
    TTreeReaderArray<float> TopoCluPhi(tree_reader, "topo_cluster_phi");
    TTreeReaderArray<float> TopoCluPt(tree_reader, "topo_cluster_pt");
    TTreeReaderArray<float> TopoCluLamda(tree_reader, "topo_cluster_lambda");
    TTreeReaderArray<float> TopoCluLamda2(tree_reader, "topo_cluster_lambda2");
    TTreeReaderArray<float> TopoCluR2(tree_reader, "topo_cluster_r2");
    TTreeReaderArray<float> TopoCluEMProb(tree_reader, "topo_cluster_EM_prob");
    TTreeReaderArray<bool> TopoCluPass(tree_reader, "topo_cluster_pass_sig_cut");
    TTreeReaderArray<int> TopoCellSig(tree_reader, "topo_cluster_cell_sig_sampling");

    TH1D *CountPrec   = new TH1D("CountPrec","CountPrec",7,-0.5,6.5);
    TH1D *PerpPrec    = new TH1D("PerpPrec","PerpPrec",100,0,3.5);
    TH1D *FPrec       = new TH1D("FPrec","FPrec",100,0,4);
    TH1D *EMPrec      = new TH1D("EMPrec","EMPrec",40,-0.1,1.1);
    TH1D *LambdaPrec  = new TH1D("LambdaPrec","LambdaPrec",40,0,4);
    TH1D *Lambda2Prec = new TH1D("Lambda2Prec","Lambda2Prec",40,0,600);
    TH1D *RadiusPrec  = new TH1D("RadiusPrec","RadiusPrec",40,0,100);

    int eventID           = 0;
    int nTracksClassified = 0;
    int nPairsWithProton  = 0;

    while (tree_reader.Next()) {
        eventID++;
        std::vector<TLorentzVector> t(2);
        int count = 0;

        if (int(TrackNum[0]) != 2) continue;

        for (int track = 0; track < int(TrackNum[0]); track++) {
            if (abs(TrackEta[track]) < 2.5 && TrackPt[track] > 1) {
                t[track].SetPtEtaPhiM(TrackPt[track], TrackEta[track], TrackPhi[track], ProtonMass);
                count++;
            } else break;
        }

        if (count == 2 && TrackCharge[0] != TrackCharge[1]) {
            TLorentzVector dipartic = t[0] + t[1];

            if (dipartic.Perp() < 0.2 && dipartic.M() < 3.16 && dipartic.M() > 3.04) {

                InvMass_All->Fill(dipartic.M());

                bool anyTrackIsProton = false;
                int protonTracksCount = 0;
                for (int i = 0; i < 2; i++) {
                    float FVariable, EMprop, Lambda, Lambda2, Radius;

                    Topocluster(t[i], TopoCluNum, TopoCluEta, TopoCluPhi, TopoCluPt,
                        TopoCluLamda, TopoCluLamda2, TopoCluR2, TopoCluEMProb, TopoCluPass,
                        CountPrec, PerpPrec, FPrec, EMPrec, LambdaPrec, Lambda2Prec, RadiusPrec,
                        FVariable, EMprop, Lambda2, Lambda, Radius);

                    if (FVariable == -1) continue; 


                    float PixelHits=TrackPixelHits[1];
                    float PixelTRTHits=TrackTRTHits[1];
                    float PixeldEdX=TrackPixeldEdX[1];
                    float PixelSCTHits=TrackSCTHits[1];

                    std::array<float, 8> feats{
                        PixelHits,
                        PixelTRTHits,
                        PixelSCTHits,
                        FVariable,
                        EMprop,
                        Lambda,
                        Lambda2,
                        Radius
                    };

                    std::vector<float>   input2 = {PixelHits, PixelTRTHits, PixelSCTHits, PixeldEdX, FVariable, EMprop, Lambda, Lambda2, Radius};
                    std::vector<int64_t> input_shape2 = {1, 9};
                    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                        allocator.GetInfo(), input2.data(), input2.size(), input_shape2.data(), input_shape2.size());
                    auto output_tensors = session2.Run(Ort::RunOptions{nullptr}, &input_name2, &input_tensor, 1, output_names2, 2);
                    float* prob_ptr = output_tensors[1].GetTensorMutableData<float>();
                    if(prob_ptr[1]>0.5)  continue;


                    int64_t predictedCluster = ClassifyTrack(feats);
                    ClusterLabelHist->Fill(predictedCluster);
                    nTracksClassified++;

                    if (predictedCluster == PROTON_CLUSTER_ID) {
                        protonTracksCount++;
                    }
                }

                if (protonTracksCount >= 1) {
                    InvMass_ClusterProton->Fill(dipartic.M());
                    nPairsWithProton++;
                }
            }
        }
    }

    std::cout << "===========================" << std::endl;
    std::cout << "Liczba eventow: " << eventID << std::endl;
    std::cout << "Liczba sklasyfikowanych torow: " << nTracksClassified << std::endl;
    std::cout << "Liczba par z >=1 torem = proton (klaster " << PROTON_CLUSTER_ID << "): "
               << nPairsWithProton << std::endl;
    std::cout << "===========================" << std::endl;

    TFile *out = new TFile(Form("Plots/%s.root", name.c_str()), "RECREATE");
    InvMass_All->Write();
    InvMass_ClusterProton->Write();
    ClusterLabelHist->Write();
    out->Close();

    TCanvas c1;
    c1.SaveAs(Form("Plots/Control/%s.pdf[", name.c_str()));

    c1.Clear();
    InvMass_All->SetLineColor(kBlack);
    InvMass_All->Draw("HIST");
    InvMass_ClusterProton->SetLineColor(kRed);
    InvMass_ClusterProton->Draw("HIST SAME");
    TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88);
    leg->AddEntry(InvMass_All, "Wszystkie pary", "l");
    leg->AddEntry(InvMass_ClusterProton, "#geq1 tor = proton (klaster 2)", "l");
    leg->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    ClusterLabelHist->Draw("HIST");
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.SaveAs(Form("Plots/Control/%s.pdf]", name.c_str()));
}