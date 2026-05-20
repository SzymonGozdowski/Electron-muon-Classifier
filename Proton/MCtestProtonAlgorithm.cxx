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

std::vector<float> load_line(const std::string& path, int line_no)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw std::runtime_error("No file scalars.txt");

    std::string line;
    for (int i = 0; i <= line_no; i++)
        std::getline(in, line);

    std::vector<float> vals;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, ',')) {
        vals.push_back(std::stof(item));
    }

    return vals;
}

void MCtestProtonAlgorithm()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    const Float_t ProtonMass    = 0.93827208816; 
    const Float_t MuonMass    = 0.1056583755;
    const Float_t ElectronMass = 0.00051099895;
    Double_t pi = TMath::Pi();
    double DEG  = 180 / TMath::Pi();

    vector<TString> files(5);
    files[0]="../Data/starlight_jpsiall_pptruth.root";
    files[1]="../Data/mc_ee_background.root";
    files[2]="../Data/mc_jpsi_ee.root";
    files[3]="../Data/mc_mumu_background.root";
    files[4]="../Data/mc_jpsi_mumu.root";


    vector<string> names(5);
    names[0]="MCProtonTestProtonAlgorithm";
    names[1]="Background_yy_EE_TestProtonAlgorithm";
    names[2]="Background_Jpsi_EE_TestProtonAlgorithm";
    names[3]="Background_yy_MuMu_TestProtonAlgorithm";
    names[4]="Background_Jpsi_MuMu_TestProtonAlgorithm";

    for(int f=0;f<5;f++)
    {
        TString File=files[f];
        string name=names[f];
        //========================
        //Creating output data file
        //========================
        TFile *outputFile = new TFile(Form("Plots/%s_hist.root", name.c_str()), "RECREATE");
        float PixelHits, PixelTRTHits, PixeldEdX, PixelSCTHits;
        float FVariable, EMprop, Lambda, Lambda2, Radius, EtaRange;
        bool  IsProton;

        //========================
        // ML setup
        //========================
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "XgbAndRf");
        Ort::SessionOptions session_options;
        session_options.SetIntraOpNumThreads(1);
        Ort::Session session1(env, "../ONNX/XgbProtonBackground.onnx", session_options);
        Ort::Session session2(env, "../ONNX/RFMuonElectron.onnx", session_options);
        Ort::AllocatorWithDefaultOptions allocator;

        auto input_name_alloc1  = session1.GetInputNameAllocated(0, allocator);
        auto output_name_alloc1 = session1.GetOutputNameAllocated(0, allocator);
        const char* input_name1     = input_name_alloc1.get();
        const char* output_names1[] = {"label", "probabilities"};

        auto input_name_alloc2  = session2.GetInputNameAllocated(0, allocator);
        auto output_name_alloc2 = session2.GetOutputNameAllocated(0, allocator);
        const char* input_name2     = input_name_alloc2.get();
        const char* output_names2[] = {"label", "probabilities"};


        auto scaler_mean  = load_line("../ONNX/scalars.txt", 0);
        auto scaler_scale = load_line("../ONNX/scalars.txt", 1);


        //========================
        // Histogramy
        //========================

        TH1D *DiMassProton  = new TH1D("DiMassProton_MCtestProtonAlgorithm", "Proton pair mass",100, 2.5, 3.5);
        TH1D *DiMassProtonAfterAlgorithm  = new TH1D("DiMassProtonAfterAlgorithm_MCtestProtonAlgorithm", "Proton pair mass",100, 2.5, 3.5);

        TH1D *EtaProton  = new TH1D("EtaProton_MCtestProtonAlgorithm","Eta Proton - ", 30, -3,  3);
        TH1D *EnergyProton= new TH1D("ProtonEnergy_MCtestProtonAlgorithm", "Proton Energy - ", 30,  0,  6);
        TH1D *EtaProtonAfterAlgorithm  = new TH1D("EtaProtonAfterAlgorithm_MCtestProtonAlgorithm","Eta Proton After Algorithm - ", 30, -3,  3);
        TH1D *EnergyProtonAfterAlgorithm= new TH1D("ProtonEnergyAfterAlgorithm_MCtestProtonAlgorithm", "Proton Energy After Algorithm - ", 30,  0,  6);
        TH1D *ResponseElectronAlgorithmHist= new TH1D("ResponseElectronAlgorithm_MCtestProtonAlgorithm", "ML Response Electron Algorithm - ", 30,  0,  1);
        TH1D *ResponseProtonAlogirthmHist= new TH1D("ResponseProtonAlgorithm_MCtestProtonAlgorithm", "ML Response Proton Algorithm - ", 30,  0,  1);




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

        TTreeReaderArray<float> TruthPrEta(tree_reader, "truth_monopole_eta");
        TTreeReaderArray<float> TruthPrPhi(tree_reader, "truth_monopole_phi");
        TTreeReaderArray<float> TruthPrPt(tree_reader, "truth_monopole_pt");
        TTreeReaderArray<float> TruthPrMass(tree_reader, "truth_monopole_m");
        int   eventID = 0;
        float protoncount = 0, diffrentparticle = 0, noTopocluster=0;
        float zeroparticle = 0, oneparticle = 0, twoparticle = 0, allparticles = 0;

        while (tree_reader.Next()) {

            if (eventID % 400000 == 0) cout << "Processing " << eventID << " event..." << endl;
            eventID++;
            //if (eventID > 20) break;
            vector<TLorentzVector> Proton(2);
            int count = 0;
            if(f==0){
            if(TruthPrEta.GetSize()!=2) continue;
            if(TruthPrPt[0]<0.9 || TruthPrPt[1]<0.9) continue;
            }
            if (int(TrackNum[0]) != 2) continue;
            for (int track = 0; track < int(TrackNum[0]); track++) {
                if (abs(TrackEta[track]) < 2.5 && TrackPt[track] > 1) {
                    Proton[track].SetPtEtaPhiM(TrackPt[track], TrackEta[track], TrackPhi[track], ProtonMass);
                    count++;
                } else break;
            }

            if (count == 2 && TrackCharge[0] != TrackCharge[1]) {
                TLorentzVector dipartic = Proton[0] + Proton[1];

                if (dipartic.Perp() < 0.2 && dipartic.M() < 3.36 && dipartic.M() > 2.92) {
                    DiMassProton->Fill(dipartic.M());
                    EtaProton->Fill(Proton[0].Eta());
                    EtaProton->Fill(Proton[1].Eta());
                    EnergyProton->Fill(Proton[0].E());
                    EnergyProton->Fill(Proton[1].E());

                    float probcutProton   = 0.5;
                    float responseProton[2] = {probcutProton, probcutProton};

                    float probcutElectron  = 0.65;
                    float responseElectron[2] = {probcutElectron, probcutElectron};


                    int Found=0;
                    for (int i = 0; i < 2; i++) {
                        allparticles++;
                        Topocluster(Proton[i], TopoCluNum, TopoCluEta, TopoCluPhi, TopoCluPt,
                            TopoCluLamda, TopoCluLamda2, TopoCluR2, TopoCluEMProb, TopoCluPass,
                            FVariable, EMprop, Lambda2, Lambda, Radius);

                        PixelHits    = TrackPixelHits[i];
                        PixelTRTHits = TrackTRTHits[i];
                        PixeldEdX    = TrackPixeldEdX[i];
                        PixelSCTHits = TrackSCTHits[i];
                        if (FVariable == -1) noTopocluster++;
                        else{
                            std::vector<float>   input_el = {PixelHits, PixelTRTHits, PixelSCTHits,PixeldEdX,  FVariable, EMprop, Lambda, Lambda2, Radius};
                            std::vector<int64_t> input_shape_el = {1, 9};
                            Ort::Value input_tensor_el = Ort::Value::CreateTensor<float>(
                                allocator.GetInfo(), input_el.data(), input_el.size(), input_shape_el.data(), input_shape_el.size());
                            auto output_tensors2 = session2.Run(Ort::RunOptions{nullptr}, &input_name2, &input_tensor_el, 1, output_names2, 2);
                            float* prob_ptr2 = output_tensors2[1].GetTensorMutableData<float>();
                            responseElectron[i] = prob_ptr2[1];
                            ResponseElectronAlgorithmHist->Fill(responseElectron[i]);


                            std::vector<float>   input = {PixelHits, PixelTRTHits, PixelSCTHits,  FVariable, EMprop, Lambda, Lambda2, Radius};
                            for (size_t i = 0; i < input.size(); i++)
                                input[i] = (input[i] - scaler_mean[i]) / scaler_scale[i];
                            std::vector<int64_t> input_shape = {1, 8};
                            Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                                allocator.GetInfo(), input.data(), input.size(), input_shape.data(), input_shape.size());
                            auto output_tensors = session1.Run(Ort::RunOptions{nullptr}, &input_name1, &input_tensor, 1, output_names1, 2);
                            float* prob_ptr = output_tensors[1].GetTensorMutableData<float>();
                            responseProton[i] = prob_ptr[1];
                            ResponseProtonAlogirthmHist->Fill(responseProton[i]);
                            Found++;
                        }
                    }
                    IsProton = 0;
                    int rightlepton=2;
                    if(Found==0) zeroparticle+=2;
                    else if(Found==1)
                    {
                        if(responseProton[0]==probcutProton)
                        {
                            if(responseProton[1]>probcutProton && responseElectron[1]<probcutElectron) IsProton=1;
                            else Found=0;
                        } 
                        else if(responseProton[1]==probcutProton)
                        {
                            if(responseProton[0]>probcutProton && responseElectron[0]<probcutElectron) IsProton=1;
                            else Found=0;
                        } 
                        oneparticle+=2;
                    } 
                    else if(Found==2) 
                    {
                        twoparticle+=2;
                        if((responseProton[0]+responseProton[1])/2>probcutProton && (responseElectron[0]+responseElectron[1])/2<probcutElectron) IsProton=1;
                    }

                    //========================
                    // Protons
                    //========================
                    if (IsProton == 1 && Found >= 1) {
                        DiMassProtonAfterAlgorithm->Fill(dipartic.M());
                        for (int i = 0; i < 2; i++) {    
                                
                            EnergyProtonAfterAlgorithm->Fill(Proton[i].E());
                            EtaProtonAfterAlgorithm->Fill(Proton[i].Eta());
                            
                            protoncount++;   
                        }
                    }

                }
            }
        }
        cout << "===========================" << endl;
        cout << "File: " << name << endl;
        cout << "Number of events: "   << eventID     << endl;
        cout << "Number of protons: "    << protoncount    << " Ratio "<< protoncount / allparticles << endl;
        cout << "Number of particles without topocluster: "<< noTopocluster <<" Ratio "<< noTopocluster / allparticles << endl;
        cout << "===========================" << endl;
        cout << "Found count:" << endl;
        cout << "  Two particles:      " << twoparticle      << "  Ratio " << twoparticle / allparticles  << endl;
        cout << "  One particle:       " << oneparticle       << "  Ratio " << oneparticle / allparticles   << endl;
        cout << "  Zero particles:     " << zeroparticle     << "  Ratio " << zeroparticle / allparticles << endl;
        cout << "  Diffrent particles: " << diffrentparticle << "  Ratio " << diffrentparticle / allparticles << endl;
        cout << "===========================" << endl;



        //========================
        // Saving histograms
        //========================
        outputFile->cd();
        DiMassProton->Write(); 
        DiMassProtonAfterAlgorithm->Write();  
        EtaProton->Write();
        EtaProtonAfterAlgorithm->Write();
        EnergyProton->Write();
        EnergyProtonAfterAlgorithm->Write();
        ResponseElectronAlgorithmHist->Write();
        ResponseProtonAlogirthmHist->Write();
        outputFile->Close();

    }
}