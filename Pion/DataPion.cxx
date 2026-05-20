#include <onnxruntime_cxx_api.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
#include <string>
#include <TLegend.h>
#include <TEfficiency.h>
#include <vector>
#include "../Topoclusters/Topocluster.C"


void DataPion()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    const Float_t ProtonMass    = 0.93827208816; 
    const Float_t MuonMass    = 0.1056583755;
    const Float_t ElectronMass = 0.00051099895;
    const Float_t PionMass = 0.13957039;

    Double_t pi = TMath::Pi();
    double DEG  = 180 / TMath::Pi();
    TString File="../Data/data23_2trk_moreTCvars.root";
    string name="DataPion";

    TFile *outputFile = new TFile(Form("Plots/%s_hist.root", name.c_str()), "RECREATE");
    float PixelHits, PixelTRTHits, PixeldEdX, PixelSCTHits;
    float FVariable, EMprop, Lambda, Lambda2, Radius, EtaRange;
    bool  IsMuon;

    //========================
    // ML setup
    //========================
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "rf");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    Ort::Session session(env, "../ONNX/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session.GetOutputNameAllocated(0, allocator);
    const char* input_name     = input_name_alloc.get();
    const char* output_names[] = {"label", "probabilities"};

    //========================
    // Histogramy
    //========================

    TH1D *DiMassFull = new TH1D("DiMass_DataPion","Full pair mass ", 100, 2.5, 3.5);
    TH1D *DiMassPion  = new TH1D("DiMassPion_DataPion  ", "Pion pair mass",100, 2.5, 3.5);
    TH1D *EtaFull  = new TH1D("EtaFull_DataPion","Eta Full - ", 30, -3,  3);
    TH1D *EtaPion = new TH1D("PionEta_DataPion","Pion #eta - ",  30, -3,  3);
    TH1D *EnergyFull = new TH1D("EnergyFull_DataPion","Energy Full - ", 30,  0,  6);
    TH1D *EnergyPion= new TH1D("PionEnergy_DataPion", "Pion Energy - ", 30,  0,  6);
    TH1D *Pixel_dEdx_Hist= new TH1D("Pixel_dEdx_DataPion", "Pixel dE/dx - ", 30,  0,  3);


    //========================
    // Petla po plikach i eventach
    //========================
    TChain *mychain = new TChain("G2TauTree");
    mychain->Add(File);
    TTreeReader tree_reader(mychain);

    TTreeReaderArray<unsigned int> TrackNum(tree_reader, "track_n");
    TTreeReaderArray<float>        TrackEta(tree_reader, "track_eta");
    TTreeReaderArray<float>        TrackPhi(tree_reader, "track_phi");
    TTreeReaderArray<float>        TrackPt(tree_reader, "track_pt");
    TTreeReaderArray<int>          TrackCharge(tree_reader, "track_charge");

    TTreeReaderArray<unsigned int> EgCluNum(tree_reader, "eg_cluster_n");
    TTreeReaderArray<float>        EgCluEta(tree_reader, "eg_cluster_eta");
    TTreeReaderArray<float>        EgCluPhi(tree_reader, "eg_cluster_phi");

    TTreeReaderArray<float> TrackPixeldEdX(tree_reader, "track_PixeldEdX");
    TTreeReaderArray<float> TrackPixelHits(tree_reader, "track_PixelHits");
    TTreeReaderArray<float> TrackSCTHits(tree_reader,  "track_SCTHits");
    TTreeReaderArray<float> TrackTRTHits(tree_reader,  "track_TRTHits");

    TTreeReaderArray<unsigned int> TopoCluNum(tree_reader, "topoclus_n");
    TTreeReaderArray<float>        TopoCluEta(tree_reader, "topo_cluster_eta");
    TTreeReaderArray<float>        TopoCluPhi(tree_reader, "topo_cluster_phi");
    TTreeReaderArray<float>        TopoCluPt(tree_reader,  "topo_cluster_pt");
    TTreeReaderArray<float>        TopoCluLamda(tree_reader,  "topo_cluster_lambda");
    TTreeReaderArray<float>        TopoCluLamda2(tree_reader, "topo_cluster_lambda2");
    TTreeReaderArray<float>        TopoCluR2(tree_reader,     "topo_cluster_r2");
    TTreeReaderArray<float>        TopoCluEMProb(tree_reader, "topo_cluster_EM_prob");
    TTreeReaderArray<bool>         TopoCluPass(tree_reader,   "topo_cluster_pass_sig_cut");

    int   eventID = 0;
    float protoncount = 0, diffrentparticle = 0;
    float zeroparticle = 0, oneparticle = 0, twoparticle = 0, allparticles = 0;
    while (tree_reader.Next()) {
        if (eventID % 400000 == 0) cout << "Processing " << eventID << " event..." << endl;
        eventID++;
        //if (eventID > 20) break;
        vector<TLorentzVector> Pion(2);
        int count=0;

        if (int(TrackNum[0]) != 2) continue;
        for (int track = 0; track < int(TrackNum[0]); track++) {
            if (abs(TrackEta[track]) < 2.5 && TrackPt[track] > 1) {
                Pion[track].SetPtEtaPhiM(TrackPt[track], TrackEta[track], TrackPhi[track], PionMass);
                count++;
            } else break;
        }

        if (count == 2 && TrackCharge[0] == TrackCharge[1]) {
                TLorentzVector dipartic = Pion[0] + Pion[1];

            if (dipartic.Perp() < 0.2) { 
 
                EtaFull->Fill(Pion[0].Eta());
                EtaFull->Fill(Pion[1].Eta());
                EnergyFull->Fill(Pion[0].E());
                EnergyFull->Fill(Pion[1].E());
                DiMassFull->Fill(dipartic.M());



                float probcut   = 0.5;
                float response[2] = {probcut, probcut};
                int   Found     = 0;

                for (int i = 0; i < 2; i++) {
                    allparticles++;
                    Topocluster(Pion[i], TopoCluNum, TopoCluEta, TopoCluPhi, TopoCluPt,
                        TopoCluLamda, TopoCluLamda2, TopoCluR2, TopoCluEMProb, TopoCluPass,
                        FVariable, EMprop, Lambda2, Lambda, Radius);

                    PixelHits    = TrackPixelHits[i];
                    PixelTRTHits = TrackTRTHits[i];
                    PixeldEdX    = TrackPixeldEdX[i];
                    Pixel_dEdx_Hist->Fill(PixeldEdX);
                    PixelSCTHits = TrackSCTHits[i];
                    if (FVariable == -1) continue;

                    std::vector<float>   input = {PixelHits, PixelTRTHits, PixelSCTHits, PixeldEdX, FVariable, EMprop, Lambda, Lambda2, Radius};
                    std::vector<int64_t> input_shape = {1, 9};
                    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                        allocator.GetInfo(), input.data(), input.size(), input_shape.data(), input_shape.size());
                    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, &input_name, &input_tensor, 1, output_names, 2);
                    float* prob_ptr = output_tensors[1].GetTensorMutableData<float>();
                    response[i] = prob_ptr[1];
                    Found++;
                }

                int lepton=2;
                int rightlepton=2;
                if(Found==0) zeroparticle+=2;
                else if(Found==1)
                {
                    if(response[0]==probcut)
                    {
                        if(response[1]<probcut-0.15) lepton=0;
                        else if(response[1]>probcut+0.15) lepton=1;
                        else Found=0;
                    } 
                    else if(response[1]==probcut)
                    {
                        if(response[0]<probcut-0.15) lepton=0;
                        else if(response[0]>probcut+0.15) lepton=1;
                        else Found=0;
                    } 
                    oneparticle+=2;
                } 
                else if(Found==2) 
                {
                    twoparticle+=2;
                    if((response[0]+response[1])/2<probcut-0.15) lepton=0;
                    else if((response[0]+response[1])/2>probcut+0.15) lepton=1;
                    else
                    {
                        diffrentparticle+=2;
                        Found=0;
                    } 
                }

                //========================
                // Pions
                //========================
                if (lepton == 0 && Found >= 1) {
                    DiMassPion->Fill(dipartic.M());
                    for (int i = 0; i < 2; i++) {        
                        EnergyPion->Fill(Pion[i].E());
                        EtaPion->Fill(Pion[i].Eta());
                        
                        protoncount++;   
                    }
                }

            }
        }
    }
    cout << "===========================" << endl;
    cout << "File: " << name << endl;
    cout << "Number of events: "   << eventID     << endl;
    cout << "Number of protons: "    << protoncount    << endl;
    cout << "===========================" << endl;
    cout << "Found count:" << endl;
    cout << "  Two particles:      " << twoparticle      << "  Ratio " << twoparticle / allparticles  << endl;
    cout << "  One particle:       " << oneparticle       << "  Ratio " << oneparticle / allparticles   << endl;
    cout << "  Zero particles:     " << zeroparticle     << "  Ratio " << zeroparticle / allparticles << endl;
    cout << "  Diffrent particles: " << diffrentparticle << endl;
    cout << "===========================" << endl;



    //========================
    // Saving histograms
    //========================
    outputFile->cd();
    DiMassFull->Write();
    DiMassPion->Write();   
    EtaFull->Write();
    EtaPion->Write();
    EnergyFull->Write();
    EnergyPion->Write();
    Pixel_dEdx_Hist->Write();
    outputFile->Close();

}