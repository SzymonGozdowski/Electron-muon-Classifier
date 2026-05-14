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

auto doubleCB = [](Double_t *x, Double_t *par) -> Double_t {

        double t = (x[0] - par[1]) / par[2];
        double absAL = fabs(par[3]);
        double absAR = fabs(par[5]);

        if (t < -absAL) {
            double a = pow(par[4] / absAL, par[4]) * exp(-0.5 * absAL * absAL);
            double b = par[4] / absAL - absAL;
            return par[0] * a / pow(b - t, par[4]);
        } else if (t > absAR) {
            double a = pow(par[6] / absAR, par[6]) * exp(-0.5 * absAR * absAR);
            double b = par[6] / absAR - absAR;
            return par[0] * a / pow(b + t, par[6]);
        } else {
            return par[0] * exp(-0.5 * t * t);
        }
    };

void DataProton()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    const Float_t ProtonMass    = 0.93827208816; 
    const Float_t MuonMass    = 0.1056583755;
    const Float_t ElectronMass = 0.00051099895;
    Double_t pi = TMath::Pi();
    double DEG  = 180 / TMath::Pi();
    TString File="../Data/data23_2trk_moreTCvars.root";
    string name="DataProton";

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

    TH1D *DiMassFull = new TH1D("DiMass_DataProton","Full pair mass ", 100, 2.5, 3.5);
    TH1D *DiMassProton  = new TH1D("DiMassProton_DataProton", "Proton pair mass",100, 2.5, 3.5);
    TH1D *EtaFull  = new TH1D("EtaFull_DataProton","Eta Full - ", 30, -3,  3);
    TH1D *EtaProton = new TH1D("ProtonEta_DataProton","Proton #eta - ",  30, -3,  3);
    TH1D *EnergyFull = new TH1D("EnergyFull_DataProton","Energy Full - ", 30,  0,  6);
    TH1D *EnergyProton= new TH1D("ProtonEnergy_DataProton", "Proton Energy - ", 30,  0,  6);



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
        vector<TLorentzVector> Proton(2), t_prim(2);
        int count = 0;

        if (int(TrackNum[0]) != 2) continue;
        for (int track = 0; track < int(TrackNum[0]); track++) {
            if (abs(TrackEta[track]) < 2.5 && TrackPt[track] > 1) {
                t_prim[track].SetPtEtaPhiM(TrackPt[track], TrackEta[track], TrackPhi[track], MuonMass);
                count++;
            } else break;
        }

        if (count == 2 && TrackCharge[0] != TrackCharge[1]) {
            TLorentzVector dipartic_prim = t_prim[0] + t_prim[1];

            if (dipartic_prim .Perp() < 0.2 && dipartic_prim .M() < 2.8 && dipartic_prim .M() > 2) { //Mass range can be changed
                Proton[0].SetPtEtaPhiM(t_prim[0].Perp(), t_prim[0].Eta(), t_prim[0].Phi(), ProtonMass);
                Proton[1].SetPtEtaPhiM(t_prim[1].Perp(), t_prim[1].Eta(), t_prim[1].Phi(), ProtonMass);
                TLorentzVector dipartic = Proton[0] + Proton[1];
                EtaFull->Fill(Proton[0].Eta());
                EtaFull->Fill(Proton[1].Eta());
                EnergyFull->Fill(Proton[0].E());
                EnergyFull->Fill(Proton[1].E());
                DiMassFull->Fill(dipartic.M());



                float probcut   = 0.5;
                float response[2] = {probcut, probcut};
                int   Found     = 0;

                for (int i = 0; i < 2; i++) {
                    allparticles++;
                    Topocluster(Proton[i], TopoCluNum, TopoCluEta, TopoCluPhi, TopoCluPt,
                        TopoCluLamda, TopoCluLamda2, TopoCluR2, TopoCluEMProb, TopoCluPass,
                        FVariable, EMprop, Lambda2, Lambda, Radius);

                    PixelHits    = TrackPixelHits[i];
                    PixelTRTHits = TrackTRTHits[i];
                    PixeldEdX    = TrackPixeldEdX[i];
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
                // Protons
                //========================
                if (lepton == 0 && Found >= 1) {
                    DiMassProton->Fill(dipartic.M());
                    for (int i = 0; i < 2; i++) {        
                        EnergyProton->Fill(Proton[i].E());
                        EtaProton->Fill(Proton[i].Eta());
                        
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
    DiMassProton->Write();   
    EtaFull->Write();
    EtaProton->Write();
    EnergyFull->Write();
    EnergyProton->Write();
    outputFile->Close();

}