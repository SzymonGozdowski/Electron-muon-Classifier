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
#include "OldTopocluster.C"

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

void ProtonSearch()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    const Float_t ProtonMass    = 0.93827208816; 
    const Float_t MuonMass    = 0.1056583755;
    const Float_t ElectronMass = 0.00051099895;
    Double_t pi = TMath::Pi();
    double DEG  = 180 / TMath::Pi();
    const int NumberOfFiles=1;
    vector<TString> files(NumberOfFiles);
    files[0] = "Data/data23_2trk_moreTCvars.root";


    TString fileLabels[NumberOfFiles] = {"Data"};
    int     fileColors[NumberOfFiles] = {kBlack};

    float PixelHits, PixelTRTHits, PixeldEdX, PixelSCTHits;
    float FVariable, EMprop, Lambda, Lambda2, Radius, EtaRange;
    bool  IsMuon;

    //========================
    // ML setup
    //========================
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "rf");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    Ort::Session session(env, "ONNX/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session.GetOutputNameAllocated(0, allocator);
    const char* input_name     = input_name_alloc.get();
    const char* output_names[] = {"label", "probabilities"};

    //========================
    // Histogramy - tablice [3] po plikach, [5][3] po zakresach eta x plik
    //========================
   
      TH1D *DiMass[NumberOfFiles], *DiMassEl[NumberOfFiles], *DiMassMu[NumberOfFiles];
    TH1D *EtaFull[NumberOfFiles],  *EnergyFull[NumberOfFiles], *EtaCalo[NumberOfFiles], *EnergyCalo[NumberOfFiles];
    TH1D *ElectronEta[NumberOfFiles], *ElectronEnergy[NumberOfFiles];
    TH1D *MuonEta[NumberOfFiles],     *MuonEnergy[NumberOfFiles];

    // [etaRange][file]  ->  5 zakresow: |eta|<0.5, 1.0, 1.5, 2.0, 2.5
    TH1D *DiMassEta[5][NumberOfFiles];
    TH1D *DiMassElEta[5][NumberOfFiles];
    TH1D *DiMassMuEta[5][NumberOfFiles];


    // Histogramy pomocnicze (niezalezne od pliku)
    TH1D *H_Count   = new TH1D("H_Count",   "Count",   7,  -0.5, 6.5);
    TH1D *H_Perp    = new TH1D("H_Perp",    "Perp",    100, 0,   3.5);
    TH1D *H_F       = new TH1D("H_F",       "F",       100, 0,   4);
    TH1D *H_EM      = new TH1D("H_EM",      "EM",      40, -0.1, 1.1);
    TH1D *H_Lambda  = new TH1D("H_Lambda",  "Lambda",  40,  0,   4);
    TH1D *H_Lambda2 = new TH1D("H_Lambda2", "Lambda2", 40,  0,   600);
    TH1D *H_Radius  = new TH1D("H_Radius",  "Radius",  40,  0,   100);

    for (int f = 0; f < 1; f++) {
        DiMass[f]        = new TH1D(Form("DiMass_%d",f),       Form("Lepton pair mass - %s",       fileLabels[f].Data()), 50, 2.5, 3.5);
        DiMassEl[f]      = new TH1D(Form("DiMassEl_%d",f),     Form("Electron pair mass - %s",     fileLabels[f].Data()), 50, 2.5, 3.5);
        DiMassMu[f]      = new TH1D(Form("DiMassMu_%d",f),     Form("Muon pair mass - %s",         fileLabels[f].Data()), 50, 2.5, 3.5);
        EtaFull[f]       = new TH1D(Form("EtaFull_%d",f),      Form("Eta Full - %s",               fileLabels[f].Data()), 30, -3,  3);
        EnergyFull[f]    = new TH1D(Form("EnergyFull_%d",f),   Form("Energy Full - %s",            fileLabels[f].Data()), 30,  0,  6);
        EtaCalo[f]       = new TH1D(Form("EtaCalo_%d",f),      Form("Eta Calo - %s",               fileLabels[f].Data()), 30, -3,  3);
        EnergyCalo[f]    = new TH1D(Form("EnergyCalo_%d",f),   Form("Energy Calo - %s",            fileLabels[f].Data()), 30,  0,  6);
        ElectronEta[f]   = new TH1D(Form("ElEta_%d",f),        Form("Electron #eta - %s",          fileLabels[f].Data()), 30, -3,  3);
        ElectronEnergy[f]= new TH1D(Form("ElEnergy_%d",f),     Form("Electron Energy - %s",        fileLabels[f].Data()), 30,  0,  6);
        MuonEta[f]       = new TH1D(Form("MuEta_%d",f),        Form("Muon #eta - %s",              fileLabels[f].Data()), 30, -3,  3);
        MuonEnergy[f]    = new TH1D(Form("MuEnergy_%d",f),     Form("Muon Energy - %s",            fileLabels[f].Data()), 30,  0,  6);

        for (int eta = 0; eta < 4; eta++) {
            double maxEta = (eta + 1) * 0.5;
            DiMassEta[eta][f] = new TH1D(Form("DiMassEta_%d_%d", eta, f),
                Form("l^{+}l^{-} mass |#eta|<%.1f - %s", maxEta, fileLabels[f].Data()), 100, 2.3, 3.5);
            DiMassElEta[eta][f] = new TH1D(Form("DiMassElEta_%d_%d", eta, f),
                Form("e^{+}e^{-} mass |#eta|<%.1f - %s", maxEta, fileLabels[f].Data()), 100, 2.3, 3.5);
            DiMassMuEta[eta][f] = new TH1D(Form("DiMassMuEta_%d_%d", eta, f),
                Form("#mu^{+}#mu^{-} mass |#eta|<%.1f - %s", maxEta, fileLabels[f].Data()), 100, 2.3, 3.5);
        }
        DiMassEta[4][f] = new TH1D(Form("DiMassEta_%d_%d", 4, f),
            Form("l^{+}l^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.5, 3.5);
        DiMassElEta[4][f] = new TH1D(Form("DiMassElEta_%d_%d", 4, f),
            Form("e^{+}e^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.5, 3.5);
        DiMassMuEta[4][f] = new TH1D(Form("DiMassMuEta_%d_%d", 4, f),
            Form("#mu^{+}#mu^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.5, 3.5);
    }

    //========================
    // Petla po plikach i eventach
    //========================
    for (int File = 0; File < 1; File++) {
        TChain *mychain = new TChain("G2TauTree");
        mychain->Add(files[File]);
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
        float muoncount = 0, electroncount = 0, diffrentparticle = 0;
        float zeroparticle = 0, oneparticle = 0, twoparticle = 0, allparticles = 0;

        while (tree_reader.Next()) {
            if (eventID % 400000 == 0) cout << "Processing " << eventID << " event..." << endl;
            eventID++;
            //if (eventID > 20) break;
            vector<TLorentzVector> Electron(2), Muon(2), t_prim(2), t(2);
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

                if (dipartic_prim .Perp() < 0.2 && dipartic_prim .M() < 2.9 && dipartic_prim .M() > 2) {
                    t[0].SetPtEtaPhiM(t_prim[0].Perp(), t_prim[0].Eta(), t_prim[0].Phi(), ProtonMass);
                    t[1].SetPtEtaPhiM(t_prim[1].Perp(), t_prim[1].Eta(), t_prim[1].Phi(), ProtonMass);
                    TLorentzVector dipartic = t[0] + t[1];
                    EtaFull[File]->Fill(t[0].Eta());
                    EtaFull[File]->Fill(t[1].Eta());
                    EnergyFull[File]->Fill(t[0].E());
                    EnergyFull[File]->Fill(t[1].E());
                    DiMass[File]->Fill(dipartic.M());
                    //cout<<dipartic.M()<<endl;
                    for (int etarange = 0; etarange < 5; etarange++) {
                        double maxEta = (etarange + 1) * 0.5;
                        if (abs(dipartic.Rapidity()) < maxEta) {
                            DiMassEta[etarange][File]->Fill(dipartic.M());
                            break;
                        }
                    }


                    float probcut   = 0.5;
                    float response[2] = {probcut, probcut};
                    int   Found     = 0;

                    for (int i = 0; i < 2; i++) {
                        allparticles++;
                        Topocluster(t[i], TopoCluNum, TopoCluEta, TopoCluPhi, TopoCluPt,
                            TopoCluLamda, TopoCluLamda2, TopoCluR2, TopoCluEMProb, TopoCluPass,
                            H_Count, H_Perp, H_F, H_EM, H_Lambda, H_Lambda2, H_Radius,
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

                    int lepton = 2;
                    int foundlepton = 3;
                    if (Found == 0) {
                        zeroparticle += 2;
                    }
                     else if (Found == 1) {
                        if (response[0] == probcut) {
                            if      (response[1] < probcut ) lepton = 0;
                            else if (response[1] > probcut ) lepton = 1;
                            EtaCalo[File]->Fill(t[1].Eta());
                            EnergyCalo[File]->Fill(t[1].E());
                            foundlepton=1;

                        } 
                        else if (response[1] == probcut) {
                            if      (response[0] < probcut ) lepton = 0;
                            else if (response[0] > probcut ) lepton = 1;
                            EtaCalo[File]->Fill(t[0].Eta());
                            EnergyCalo[File]->Fill(t[0].E());
                            foundlepton=0;


                        }
                        oneparticle += 2;
                    } else if (Found == 2) {
                        twoparticle += 2;
                        if      ((response[0] + response[1]) / 2 < probcut ) lepton = 0;
                        else if ((response[0] + response[1]) / 2 > probcut ) lepton = 1;
                        else { diffrentparticle += 2; Found = 0; }
                        EtaCalo[File]->Fill(t[0].Eta());
                        EtaCalo[File]->Fill(t[1].Eta());
                        EnergyCalo[File]->Fill(t[0].E());
                        EnergyCalo[File]->Fill(t[1].E());
                    }

                    //========================
                    // Elektrony
                    //========================
                    if (lepton == 0 && Found >= 1) {
                        DiMassEl[File]->Fill(dipartic.M());
                        for (int etarange = 0; etarange < 5; etarange++) {
                                double maxEta = (etarange + 1) * 0.5;
                                if (abs(dipartic.Rapidity()) < maxEta) {
                                    DiMassElEta[etarange][File]->Fill(dipartic.M());
                                    break;
                                }
                            }
                        for (int i = 0; i < 2; i++) {
                            Electron[i] = t[i];

                            if(Found==2)
                            {
                                ElectronEnergy[File]->Fill(Electron[i].E());
                                ElectronEta[File]->Fill(Electron[i].Eta());
                            }
                            else if(Found==1)
                            {
                                if(i==foundlepton)
                                {
                                    ElectronEnergy[File]->Fill(Electron[foundlepton].E());
                                    ElectronEta[File]->Fill(Electron[foundlepton].Eta());
                                }
                            }
                            electroncount++;

                            
                        }
                    }

                    //========================
                    // Miony
                    //========================
                    if (lepton == 1 && Found >= 1) {
                        DiMassMu[File]->Fill(dipartic.M());
                        for (int etarange = 0; etarange < 5; etarange++) {
                                double maxEta = (etarange + 1) * 0.5;
                                if (abs(dipartic.Rapidity()) < maxEta) {
                                    DiMassMuEta[etarange][File]->Fill(dipartic.M());
                                    break;
                                }
                            }
                        
                        for (int i = 0; i < 2; i++) {
                            Muon[i] = t[i];
                            if(Found==2)
                            {
                                MuonEnergy[File]->Fill(Muon[i].E());
                                MuonEta[File]->Fill(Muon[i].Eta());
                            }
                            else if(Found==1)
                            {
                                if(i==foundlepton)
                                {
                                MuonEnergy[File]->Fill(Muon[foundlepton].E());
                                MuonEta[File]->Fill(Muon[foundlepton].Eta());
                                }
                            }
                            muoncount++;

                            
                        }
                    }
                }
            }
        }
        cout << "===========================" << endl;
        cout << "File: " << fileLabels[File] << endl;
        cout << "Number of events: "   << eventID     << endl;
        cout << "Number of electrons: " << electroncount << endl;
        cout << "Number of muons: "    << muoncount    << endl;
        cout << "===========================" << endl;
        cout << "Found count:" << endl;
        cout << "  Two particles:      " << twoparticle      << "  Ratio " << twoparticle / allparticles  << endl;
        cout << "  One particle:       " << oneparticle       << "  Ratio " << oneparticle / allparticles   << endl;
        cout << "  Zero particles:     " << zeroparticle     << "  Ratio " << zeroparticle / allparticles << endl;
        cout << "  Diffrent particles: " << diffrentparticle << endl;
        cout << "===========================" << endl;

    }

    //========================
    // Rysowanie
    //========================
    //gStyle->SetOptStat(000000);
    TCanvas c1;
    c1.SaveAs("Plots/ProtonSearch.pdf[");

    // --- 1. DiMass z fitem - osobna strona dla kazdego pliku ---
    for (int f = 0; f < 1; f++) {
        c1.Clear();
        DiMass[f]->SetLineColor(kBlack);
        DiMassEl[f]->SetLineColor(kRed);
        DiMassMu[f]->SetLineColor(kBlue);

        

        DiMass[f]->SetMinimum(0);
        DiMass[f]->GetXaxis()->SetTitle("M_{pp} [GeV]");
        DiMassEl[f]->GetXaxis()->SetTitle("M_{pp} [GeV]");
        DiMassMu[f]->GetXaxis()->SetTitle("M_{pp} [GeV]");

        DiMass[f]->SetTitle("Proton - J/psi");
        DiMassEl[f]->SetTitle("Proton - J/psi");
        DiMass[f]->SetTitle("Proton - J/psi");

        c1.Clear();
        DiMass[f]->Draw();
        c1.SaveAs("Plots/ProtonSearch.pdf"); 
        c1.Clear();

        DiMassMu[f]->Draw();
        c1.SaveAs("Plots/ProtonSearch.pdf");
        c1.Clear();

        TF1 *MCmassPrCbExp = new TF1("MCmassPrCbExp", [&](double *x, double *p) -> double { return doubleCB(x, p) + p[7] * TMath::Exp(p[8] * x[0]);}, 2.95, 3.25, 9);
        


        // Parametry tła
        MCmassPrCbExp->SetParameter(0, 1000);  
        MCmassPrCbExp->SetParameter(1, 3.1);  
        MCmassPrCbExp->SetParameter(2, 0.2);  
        MCmassPrCbExp->SetParameter(3, 2);  
        MCmassPrCbExp->SetParameter(4, 5);  
        MCmassPrCbExp->SetParameter(5, 2);  
        MCmassPrCbExp->SetParameter(6, 5);  
        MCmassPrCbExp->SetParameter(7,  800);  
        MCmassPrCbExp->SetParameter(8, -0.8);  
        MCmassPrCbExp->SetParLimits(1, 3, 3.2);   
        MCmassPrCbExp->SetParLimits(2, 0, 0.07);                               
        MCmassPrCbExp->SetParLimits(7, 0, 1e9);   
        MCmassPrCbExp->SetParLimits(8, -1, 0); 
        DiMassEl[f]->Fit("MCmassPrCbExp", "RL"); 

        double PrTruechi2_ndf  = MCmassPrCbExp->GetChisquare() / MCmassPrCbExp->GetNDF();

        TF1 *MCmassPrBackExp = new TF1("MCmassPrBackExp", "[0]*exp([1]*x)", 2.95, 3.25);
        MCmassPrBackExp->SetParameter(0, MCmassPrCbExp->GetParameter(7));  
        MCmassPrBackExp->SetParameter(1,  MCmassPrCbExp->GetParameter(8));

        TLegend *legEFFData = new TLegend(0.65, 0.7, 0.8, 0.85);
        legEFFData->SetBorderSize(0);
        legEFFData->SetTextFont(42);
        legEFFData->SetTextSize(0.04);
        legEFFData->AddEntry(DiMassEl[f],"Data","L");
        legEFFData->AddEntry(MCmassPrCbExp,"Fit","L");
        legEFFData->AddEntry(MCmassPrBackExp,"Background","L");
        legEFFData->Draw();

        

        DiMassEl[f]->SetTitle("J/#psi #rightarrow #P#P : Data vs MC Fit + Exp;Mass [GeV];Events");
        DiMassEl[f]->SetLineColor(kBlack);
        DiMassEl[f]->Draw("E");
        MCmassPrCbExp->Draw("same");
        MCmassPrBackExp->SetLineColor(kGreen);
        MCmassPrBackExp->Draw("same");
        legEFFData->Draw("same");

        DiMassEl[f]->Draw();
        c1.SaveAs("Plots/ProtonSearch.pdf"); 

    }
    c1.SaveAs("Plots/ProtonSearch.pdf]");

}