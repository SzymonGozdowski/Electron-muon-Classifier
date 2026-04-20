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

void McTrueEfficiency()
{
    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    Float_t MuonMass    = 0.1056583755;
    Float_t ElectronMass = 0.00051099895;
    Double_t pi = TMath::Pi();
    double DEG  = 180 / TMath::Pi();

    vector<TString> files(3);
    files[0] = "Data/mc_jpsi_ee.root";
    files[1] = "Data/mc_jpsi_mumu.root";
    files[2] = "Data/data23_2trk_moreTCvars.root";


    TString fileLabels[3] = { "MC ee", "MC #mu#mu","Data"};
    int     fileColors[3] = { kRed, kBlue, kBlack};

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
    TH1D *DiMass[3], *DiMassEl[3], *DiMassMu[3];
    TH1D *EtaFull[3],  *EnergyFull[3], *EtaCalo[3], *EnergyCalo[3];
    TH1D *ElectronEta[3], *ElectronEnergy[3];
    TH1D *MuonEta[3],     *MuonEnergy[3];

    // [etaRange][file]  ->  5 zakresow: |eta|<0.5, 1.0, 1.5, 2.0, 2.5
    TH1D *DiMassEta[5][3];
    TH1D *DiMassElEta[5][3];
    TH1D *DiMassMuEta[5][3];

    // Histogramy pomocnicze (niezalezne od pliku)
    TH1D *H_Count   = new TH1D("H_Count",   "Count",   7,  -0.5, 6.5);
    TH1D *H_Perp    = new TH1D("H_Perp",    "Perp",    100, 0,   3.5);
    TH1D *H_F       = new TH1D("H_F",       "F",       100, 0,   4);
    TH1D *H_EM      = new TH1D("H_EM",      "EM",      40, -0.1, 1.1);
    TH1D *H_Lambda  = new TH1D("H_Lambda",  "Lambda",  40,  0,   4);
    TH1D *H_Lambda2 = new TH1D("H_Lambda2", "Lambda2", 40,  0,   600);
    TH1D *H_Radius  = new TH1D("H_Radius",  "Radius",  40,  0,   100);

    for (int f = 0; f < 3; f++) {
        DiMass[f]        = new TH1D(Form("DiMass_%d",f),       Form("Lepton pair mass - %s",       fileLabels[f].Data()), 50, 2.3, 3.5);
        DiMassEl[f]      = new TH1D(Form("DiMassEl_%d",f),     Form("Electron pair mass - %s",     fileLabels[f].Data()), 50, 2.3, 3.5);
        DiMassMu[f]      = new TH1D(Form("DiMassMu_%d",f),     Form("Muon pair mass - %s",         fileLabels[f].Data()), 50, 2.3, 3.5);
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
            Form("l^{+}l^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.3, 3.5);
        DiMassElEta[4][f] = new TH1D(Form("DiMassElEta_%d_%d", 4, f),
            Form("e^{+}e^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.3, 3.5);
        DiMassMuEta[4][f] = new TH1D(Form("DiMassMuEta_%d_%d", 4, f),
            Form("#mu^{+}#mu^{-} mass |#eta|<%.1f - %s", 2.5, fileLabels[f].Data()), 25, 2.3, 3.5);
    }

    //========================
    // Petla po plikach i eventach
    //========================
    for (int File = 0; File < 3; File++) {
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
            //if (eventID > 400000) break;
            vector<TLorentzVector> Electron(2), Muon(2), t(2);
            int count = 0;

            if (int(TrackNum[0]) != 2) continue;
            for (int track = 0; track < int(TrackNum[0]); track++) {
                if (abs(TrackEta[track]) < 2.5 && TrackPt[track] > 1) {
                    t[track].SetPtEtaPhiM(TrackPt[track], TrackEta[track], TrackPhi[track], MuonMass);
                    count++;
                } else break;
            }

            if (count == 2 && TrackCharge[0] != TrackCharge[1]) {
                TLorentzVector dipartic = t[0] + t[1];
                if (dipartic.Perp() < 0.2 && dipartic.M() < 3.5) {

                    EtaFull[File]->Fill(t[0].Eta());
                    EtaFull[File]->Fill(t[1].Eta());
                    EnergyFull[File]->Fill(t[0].E());
                    EnergyFull[File]->Fill(t[1].E());
                    DiMass[File]->Fill(dipartic.M());
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
                            if      (response[1] < probcut - 0.15) lepton = 0;
                            else if (response[1] > probcut + 0.15) lepton = 1;
                            EtaCalo[File]->Fill(t[1].Eta());
                            EnergyCalo[File]->Fill(t[1].E());
                            foundlepton=1;

                        } 
                        else if (response[1] == probcut) {
                            if      (response[0] < probcut - 0.15) lepton = 0;
                            else if (response[0] > probcut + 0.15) lepton = 1;
                            EtaCalo[File]->Fill(t[0].Eta());
                            EnergyCalo[File]->Fill(t[0].E());
                            foundlepton=0;


                        }
                        oneparticle += 2;
                    } else if (Found == 2) {
                        twoparticle += 2;
                        if      ((response[0] + response[1]) / 2 < probcut - 0.15) lepton = 0;
                        else if ((response[0] + response[1]) / 2 > probcut + 0.15) lepton = 1;
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
    gStyle->SetOptStat(000000);
    
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

    

    TF1 *MCmassElCb = new TF1("MCmassElCb", doubleCB, 2.5, 3.3, 7);

    MCmassElCb->SetParNames("Norm", "Mean", "Sigma", "Alpha_L", "N_L", "Alpha_R", "N_R");
    MCmassElCb->SetParameters(
        DiMass[0]->GetMaximum(),  // Norm
        3.097,                       // Mean
        0.02,                        // Sigma
        1.5,                         // Alpha_L
        5.0,                         // N_L
        1.5,                         // Alpha_R
        5.0                          // N_R
    );

    MCmassElCb->SetParLimits(3, 0.1,   10.0);   // Alpha_L
    MCmassElCb->SetParLimits(4, 1.0,   50.0);   // N_L
    MCmassElCb->SetParLimits(5, 0.1,   10.0);   // Alpha_R
    MCmassElCb->SetParLimits(6, 1.0,   50.0);   // N_R

    DiMass[0]->Fit("MCmassElCb", "RLQ"); 

    double elmean_val  = MCmassElCb->GetParameter(1);
    double elmean_err  = MCmassElCb->GetParError(1);
    double elchi2_ndf  = MCmassElCb->GetChisquare() / MCmassElCb->GetNDF();

    

    TF1 *MCmassMuCb = new TF1("MCmassMuCb", doubleCB, 2.85, 3.35, 7);

    MCmassMuCb->SetParNames("Norm", "Mean", "Sigma", "Alpha_L", "N_L", "Alpha_R", "N_R");
    MCmassMuCb->SetParameters(
        DiMass[1]->GetMaximum(),  // Norm
        3.097,                       // Mean
        0.02,                        // Sigma
        1.5,                         // Alpha_L
        5.0,                         // N_L
        1.5,                         // Alpha_R
        5.0                          // N_R
    );


    MCmassMuCb->SetParLimits(3, 0.1,   10.0);   // Alpha_L
    MCmassMuCb->SetParLimits(4, 1.0,   50.0);   // N_L
    MCmassMuCb->SetParLimits(5, 0.1,   10.0);   // Alpha_R
    MCmassMuCb->SetParLimits(6, 1.0,   50.0);   // N_R


    DiMass[1]->Fit("MCmassMuCb", "RLQ");  // "R" = użyj zakresu z TF1

    double mumean_val  = MCmassMuCb->GetParameter(1);
    double mumean_err  = MCmassMuCb->GetParError(1);
    double muchi2_ndf  = MCmassMuCb->GetChisquare() / MCmassMuCb->GetNDF();
    
    auto StyleRatio = [](TH1F* h, const char* yTitle) {
        h->SetTitle("");
        
        // Ustawienia osi Y
        h->GetYaxis()->SetTitle(yTitle);
        h->GetYaxis()->SetTitleSize(0.12);
        h->GetYaxis()->SetTitleOffset(0.35);
        h->GetYaxis()->SetLabelSize(0.07);
        h->GetYaxis()->CenterTitle(true);
        
        // KLUCZOWE: Sztywne limity, żeby wykres się nie "psuł"
        h->SetMinimum(0.5); // Ratio nie powinno być ujemne
        h->SetMaximum(1.5); // Pokazuj odchylenia do 100% w górę
        
        // Ustawienia osi X
        h->GetXaxis()->SetTitleSize(0.14);
        h->GetXaxis()->SetLabelSize(0.12);
        h->GetXaxis()->SetTitle("Mass [GeV]");
        
        h->SetMarkerStyle(20);
        h->SetMarkerSize(0.7);
    };
    TLegend *legEFF = new TLegend(0.75, 0.7, 0.9, 0.85);
    legEFF->SetBorderSize(0);
    legEFF->SetTextFont(42);
    legEFF->SetTextSize(0.04);
    legEFF->AddEntry(DiMassMu[1],"Data","L");
    legEFF->AddEntry(MCmassMuCb,"Fit","L");
    legEFF->Draw();

   

    // --- KANWA I PADY DLA MIONÓW ---
    TCanvas *cPullMu = new TCanvas("cPullMu", "Fit mumu", 800, 800);
    TPad *pad11 = new TPad("pad11", "pad11", 0, 0.3, 1, 1.0);
    TPad *pad22 = new TPad("pad22", "pad22", 0, 0.0, 1, 0.3);
    pad11->SetBottomMargin(0);
    pad22->SetTopMargin(0);
    pad22->SetBottomMargin(0.35);
    pad11->Draw(); 
    pad22->Draw();

    pad11->cd();
    DiMass[1]->SetLineColor(kBlack);
    DiMass[1]->SetTitle("J/#psi #rightarrow #mu#mu : MC and Fit");
    DiMass[1]->GetYaxis()->SetTitle("Events");
    DiMass[1]->GetXaxis()->SetRangeUser(2.8, 3.4);
    DiMass[1]->Draw("E");
    MCmassMuCb->Draw("same");
    legEFF->Draw("same");

    pad22->cd(); // TERAZ pad22 JEST ZADEKLAROWANY WYŻEJ
    TH1F *hRatioMu = (TH1F*)DiMass[1]->Clone("hRatioMu");
    hRatioMu->Reset();
    for (int i = 1; i <= DiMass[1]->GetNbinsX(); i++) {
        double data_val = DiMass[1]->GetBinContent(i);
        double data_err = DiMass[1]->GetBinError(i);
        double model_val = MCmassMuCb->Eval(DiMass[1]->GetBinCenter(i));
        if (model_val > 0) {
            hRatioMu->SetBinContent(i, data_val / model_val);
            hRatioMu->SetBinError(i, data_err / model_val);
        }
    }
    StyleRatio(hRatioMu, "Data / Fit");

    hRatioMu->Draw("E0");

    TLine *l1m = new TLine(2.8, 1.0, 3.4, 1.0);
    l1m->SetLineColor(kRed); l1m->SetLineStyle(2); l1m->Draw();

    cPullMu->SaveAs("Plots/CrystalBallMu_Ratio.png");

     // --- KANWA I PADY DLA ELEKTRONÓW ---
    TCanvas *cPullEl = new TCanvas("cPullEl", "Fit ee", 800, 800);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3); 
    pad1->SetBottomMargin(0);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.35); 
    pad1->Draw(); 
    pad2->Draw();

    pad1->cd();
    DiMass[0]->SetTitle("J/#psi #rightarrow ee : MC and Fit");
    DiMass[0]->GetYaxis()->SetTitle("Events");
    DiMass[0]->SetLineColor(kBlack);
    DiMass[0]->Draw("E");
    MCmassElCb->Draw("same");
    legEFF->Draw("same");

    pad2->cd(); 
    TH1F *hRatioEl = (TH1F*)DiMass[0]->Clone("hRatioEl");
    hRatioEl->Reset();

    for (int i = 1; i <= DiMass[0]->GetNbinsX(); i++) {
        double data_val = DiMass[0]->GetBinContent(i);
        double data_err = DiMass[0]->GetBinError(i);
        double model_val = MCmassElCb->Eval(DiMass[0]->GetBinCenter(i));
        if (model_val > 0) {
            hRatioEl->SetBinContent(i, data_val / model_val);
            hRatioEl->SetBinError(i, data_err / model_val);
        }
    }
    StyleRatio(hRatioEl, "Data / Fit");

    hRatioEl->Draw("E0");

    TLine *l1 = new TLine(hRatioEl->GetXaxis()->GetXmin(), 1.0, hRatioEl->GetXaxis()->GetXmax(), 1.0);
    l1->SetLineColor(kRed); l1->SetLineStyle(2); l1->Draw();
    cPullEl->SaveAs("Plots/CrystalBallEl_Ratio.png");

    


    TF1 *MCmassMuCbExp = new TF1("MCmassMuCbExp", [&](double *x, double *p) -> double { return doubleCB(x, p) + p[7] * TMath::Exp(p[8] * x[0]);}, 2.5, 3.5, 9);
    for (int i = 1; i < 7; i++) {
        MCmassMuCbExp->FixParameter(i, MCmassMuCb->GetParameter(i));
    }

    // Parametry tła
    MCmassMuCbExp->SetParameter(0, 100);  
    MCmassMuCbExp->SetParameter(7,  DiMassMu[2]->GetMaximum() * 0.35);  
    MCmassMuCbExp->SetParameter(8, -0.01);                                
    MCmassMuCbExp->SetParLimits(7, 0, 1e9);   
    MCmassMuCbExp->SetParLimits(8, -10, 0); 
    DiMassMu[2]->Fit("MCmassMuCbExp", "RLQ"); 

    double muTruechi2_ndf  = MCmassMuCbExp->GetChisquare() / MCmassMuCbExp->GetNDF();

    TF1 *MCmassMuBackExp = new TF1("MCmassMuBackExp", "[0]*exp([1]*x)", 2.5, 3.5);
    MCmassMuBackExp->SetParameter(0, MCmassMuCbExp->GetParameter(7));  
    MCmassMuBackExp->SetParameter(1,  MCmassMuCbExp->GetParameter(8));

    TLegend *legEFFData = new TLegend(0.65, 0.7, 0.8, 0.85);
    legEFFData->SetBorderSize(0);
    legEFFData->SetTextFont(42);
    legEFFData->SetTextSize(0.04);
    legEFFData->AddEntry(DiMassMu[2],"Data","L");
    legEFFData->AddEntry(MCmassMuCbExp,"Fit","L");
    legEFFData->AddEntry(MCmassMuBackExp,"Background","L");
    legEFFData->Draw();

    // Kanwa i Ratio dla elektronów
    TCanvas *cDataMu = new TCanvas("cDataMu", "Fit Subtracted Mu", 800, 800);
    TPad *p1dm = new TPad("p1e","p1e",0,0.3,1,1);
    TPad *p2dm = new TPad("p2e","p2e",0,0,1,0.3);
    p1dm->SetBottomMargin(0); p2dm->SetTopMargin(0); p2dm->SetBottomMargin(0.35);
    p1dm->Draw(); p2dm->Draw();

    p1dm->cd();
    DiMassMu[2]->SetTitle("J/#psi #rightarrow #mu#mu : Data vs MC Fit + Exp;Mass [GeV];Events");
    DiMassMu[2]->SetLineColor(kBlack);
    DiMassMu[2]->Draw("E");
    MCmassMuCbExp->Draw("same");
    MCmassMuBackExp->SetLineColor(kGreen);
    MCmassMuBackExp->Draw("same");
    legEFFData->Draw("same");


    p2dm->cd();
    TH1F *hRatioDataMu = (TH1F*)DiMassMu[2]->Clone("hRatioDataMu");
    hRatioDataMu->Reset();
    for (int i = 1; i <= DiMassMu[2]->GetNbinsX(); i++) {
        double model = MCmassMuCbExp->Eval(DiMassMu[2]->GetBinCenter(i));
        if (model > 0) {
            hRatioDataMu->SetBinContent(i, DiMassMu[2]->GetBinContent(i) / model);
            hRatioDataMu->SetBinError(i, DiMassMu[2]->GetBinError(i) / model);
        }
    }
    StyleRatio(hRatioDataMu, "Data / Fit");
    hRatioDataMu->Draw("E0");
    TLine *l1dm = new TLine(hRatioDataMu->GetXaxis()->GetXmin(), 1.0, hRatioDataMu->GetXaxis()->GetXmax(), 1.0);
    l1dm->SetLineColor(kRed); l1dm->SetLineStyle(2); l1dm->Draw();

    cDataMu->SaveAs("Plots/DataMu_Fit.png");

    TF1 *MCmassElCbExp = new TF1("MCmassElCbExp", [&](double *x, double *p) -> double { return doubleCB(x, p) + p[7] * TMath::Exp(p[8] * x[0]);}, 2.5, 3.5, 9);
    for (int i = 1; i < 7; i++) {
        MCmassElCbExp->FixParameter(i, MCmassElCb->GetParameter(i));
    }

    // Parametry tła
    MCmassElCbExp->SetParameter(0, 100);  
    MCmassElCbExp->SetParameter(7,  DiMassEl[2]->GetMaximum() * 0.05);  
    MCmassElCbExp->SetParameter(8, -1.0);                                
    MCmassElCbExp->SetParLimits(7, 0, 1e9);   
    MCmassElCbExp->SetParLimits(8, -10, 0); 
    DiMassEl[2]->Fit("MCmassElCbExp", "RLQ"); 

    double elTruechi2_ndf  = MCmassElCbExp->GetChisquare() / MCmassElCbExp->GetNDF();

    TF1 *MCmassElBackExp = new TF1("MCmassElBackExp",  "[0]*exp([1]*x)", 2.5, 3.5);
    MCmassElBackExp->SetParameter(0, MCmassElCbExp->GetParameter(7));  
    MCmassElBackExp->SetParameter(1,  MCmassElCbExp->GetParameter(8)); 

    

    // Kanwa i Ratio dla elektronów
    TCanvas *cDataEl = new TCanvas("cDataEl", "Fit Subtracted El", 800, 800);
    TPad *p1de = new TPad("p1e","p1e",0,0.3,1,1);
    TPad *p2de = new TPad("p2e","p2e",0,0,1,0.3);
    p1de->SetBottomMargin(0); p2de->SetTopMargin(0); p2de->SetBottomMargin(0.35);
    p1de->Draw(); p2de->Draw();

    p1de->cd();
    DiMassEl[2]->SetTitle("J/#psi #rightarrow ee : Data vs MC Fit + Exp;Mass [GeV];Events");
    DiMassEl[2]->SetLineColor(kBlack);
    DiMassEl[2]->Draw("E");
    MCmassElCbExp->Draw("same");
    MCmassElBackExp->SetLineColor(kGreen);
    MCmassElBackExp->Draw("same");
    legEFFData->Draw("same");


    p2de->cd();
    TH1F *hRatioDataEl = (TH1F*)DiMassEl[2]->Clone("hRatioDataEl");
    hRatioDataEl->Reset();
    for (int i = 1; i <= DiMassEl[2]->GetNbinsX(); i++) {
        double model = MCmassElCbExp->Eval(DiMassEl[2]->GetBinCenter(i));
        if (model > 0) {
            hRatioDataEl->SetBinContent(i, DiMassEl[2]->GetBinContent(i) / model);
            hRatioDataEl->SetBinError(i, DiMassEl[2]->GetBinError(i) / model);
        }
    }
    StyleRatio(hRatioDataEl, "Data / Fit");
    hRatioDataEl->Draw("E0");
    TLine *l1de = new TLine(hRatioDataEl->GetXaxis()->GetXmin(), 1.0, hRatioDataEl->GetXaxis()->GetXmax(), 1.0);
    l1de->SetLineColor(kRed); l1de->SetLineStyle(2); l1de->Draw();

    cDataEl->SaveAs("Plots/DataEl_Fit.png");
    
    cout<<"MC Electron Chi2/NDf: "<<elchi2_ndf<<endl;
    cout<<"MC Muon Chi2/NDf: "<<muchi2_ndf<<endl;

    cout<<"True Electron Chi2/NDf: "<<elTruechi2_ndf<<endl;
    cout<<"True Muon Chi2/NDf: "<<muTruechi2_ndf<<endl;


    for (int eta = 0; eta < 5; eta++) {
    cout<<"=============================="<<endl;
    cout<<"Eta: "<<eta<<endl;
    cout<<"=============================="<<endl;

        
        TF1 *MCmassElCbEta = new TF1("MCmassElCbEta", doubleCB, 2.3, 3.3, 7);

        MCmassElCbEta->SetParNames("Norm", "Mean", "Sigma", "Alpha_L", "N_L", "Alpha_R", "N_R");
        MCmassElCbEta->SetParameters(
            DiMassEta[eta][0]->GetMaximum(),  // Norm
            3.097,                       // Mean
            0.02,                        // Sigma
            1.5,                         // Alpha_L
            5.0,                         // N_L
            1.5,                         // Alpha_R
            5.0                          // N_R
        );

        MCmassElCbEta->SetParLimits(3, 0.1,   10.0);   // Alpha_L
        MCmassElCbEta->SetParLimits(4, 1.0,   50.0);   // N_L
        MCmassElCbEta->SetParLimits(5, 0.1,   10.0);   // Alpha_R
        MCmassElCbEta->SetParLimits(6, 1.0,   50.0);   // N_R

        DiMassEta[eta][0]->Fit("MCmassElCbEta", "RLQ"); 

        double elmean_val  = MCmassElCbEta->GetParameter(1);
        double elmean_err  = MCmassElCbEta->GetParError(1);
        double elchi2_ndf  = MCmassElCbEta->GetChisquare() / MCmassElCbEta->GetNDF();

        

        TF1 *MCmassMuCbEta = new TF1("MCmassMuCbEta", doubleCB, 2.85, 3.35, 7);

        MCmassMuCbEta->SetParNames("Norm", "Mean", "Sigma", "Alpha_L", "N_L", "Alpha_R", "N_R");
        MCmassMuCbEta->SetParameters(
            DiMassEta[eta][1]->GetMaximum(),  // Norm
            3.097,                       // Mean
            0.02,                        // Sigma
            1.5,                         // Alpha_L
            5.0,                         // N_L
            1.5,                         // Alpha_R
            5.0                          // N_R
        );


        MCmassMuCbEta->SetParLimits(3, 0.1,   10.0);   // Alpha_L
        MCmassMuCbEta->SetParLimits(4, 1.0,   50.0);   // N_L
        MCmassMuCbEta->SetParLimits(5, 0.1,   10.0);   // Alpha_R
        MCmassMuCbEta->SetParLimits(6, 1.0,   50.0);   // N_R


        DiMassEta[eta][1]->Fit("MCmassMuCbEta", "RLQ");  // "R" = użyj zakresu z TF1

        double mumean_val  = MCmassMuCbEta->GetParameter(1);
        double mumean_err  = MCmassMuCbEta->GetParError(1);
        double muchi2_ndf  = MCmassMuCbEta->GetChisquare() / MCmassMuCbEta->GetNDF();
        



        TF1 *MCmassMuCbExpEta = new TF1("MCmassMuCbExpEta", [&](double *x, double *p) -> double { return doubleCB(x, p) + p[7] * TMath::Exp(p[8] * x[0]);}, 2.5, 3.5, 9);
        for (int i = 1; i < 7; i++) {
            MCmassMuCbExpEta->FixParameter(i, MCmassMuCbEta->GetParameter(i));
        }

        // Parametry tła
        MCmassMuCbExpEta->SetParameter(0, 100);  
        MCmassMuCbExpEta->SetParameter(7,  DiMassMuEta[eta][2]->GetMaximum() * 0.35);  
        MCmassMuCbExpEta->SetParameter(8, -1);                                
        MCmassMuCbExpEta->SetParLimits(7, 0, 1e9);   
        MCmassMuCbExpEta->SetParLimits(8, -10, -0.2); 
        DiMassMuEta[eta][2]->Fit("MCmassMuCbExpEta", "RLQ"); 

        double muTruechi2_ndf  = MCmassMuCbExpEta->GetChisquare() / MCmassMuCbExpEta->GetNDF();

        TF1 *MCmassMuBackExpEta = new TF1("MCmassMuBackExpEta", "[0]*exp([1]*x)", 2.5, 3.5);
        MCmassMuBackExpEta->SetParameter(0, MCmassMuCbExpEta->GetParameter(7));  
        MCmassMuBackExpEta->SetParameter(1,  MCmassMuCbExpEta->GetParameter(8));

        TF1 *MCmassElCbExpEta = new TF1("MCmassElCbExpEta", [&](double *x, double *p) -> double { return doubleCB(x, p) + p[7] * TMath::Exp(p[8] * x[0]);}, 2.3, 3.5, 9);
    for (int i = 1; i < 7; i++) {
        MCmassElCbExpEta->FixParameter(i, MCmassElCbEta->GetParameter(i));
    }

        // Parametry tła
        MCmassElCbExpEta->SetParameter(0, 100);  
        MCmassElCbExpEta->SetParameter(7,  DiMassElEta[eta][2]->GetMaximum() * 0.05);  
        MCmassElCbExpEta->SetParameter(8, -1.0);                                
        MCmassElCbExpEta->SetParLimits(7, 0, 1e9);   
        MCmassElCbExpEta->SetParLimits(8, -10, -0.3);
        DiMassElEta[eta][2]->Fit("MCmassElCbExpEta", "RLQ"); 

        double elTruechi2_ndf  = MCmassElCbExpEta->GetChisquare() / MCmassElCbExpEta->GetNDF();

        TF1 *MCmassElBackExpEta = new TF1("MCmassElBackExpEta",  "[0]*exp([1]*x)", 2.3, 3.5);
        MCmassElBackExpEta->SetParameter(0, MCmassElCbExpEta->GetParameter(7));  
        MCmassElBackExpEta->SetParameter(1,  MCmassElCbExpEta->GetParameter(8)); 

        
        TCanvas *cMassEta = new TCanvas("cMassEta", "Mass Eta", 800, 800);
        cMassEta->Divide(2,2);
        cMassEta->cd(1);
        DiMassEta[eta][1]->SetLineColor(kBlack);
        DiMassEta[eta][1]->SetTitle("J/#psi #rightarrow #mu#mu : MC and Fit");
        DiMassEta[eta][1]->GetYaxis()->SetTitle("Events");
        DiMassEta[eta][1]->Draw("E");
        MCmassMuCbEta->Draw("same");
        legEFF->Draw("same");

        cMassEta->cd(2);
        DiMassEta[eta][0]->SetTitle("J/#psi #rightarrow ee : MC and Fit");
        DiMassEta[eta][0]->GetYaxis()->SetTitle("Events");
        DiMassEta[eta][0]->SetLineColor(kBlack);
        DiMassEta[eta][0]->Draw("E");
        MCmassElCbEta->Draw("same");
        legEFF->Draw("same");

        cMassEta->cd(3);
        DiMassMuEta[eta][2]->SetTitle("J/#psi #rightarrow #mu#mu : Data vs MC Fit + Exp;Mass [GeV];Events");
        DiMassMuEta[eta][2]->SetLineColor(kBlack);
        DiMassMuEta[eta][2]->Draw("E");
        MCmassMuCbExpEta->Draw("same");
        MCmassMuBackExpEta->SetLineColor(kGreen);
        MCmassMuBackExpEta->Draw("same");
        legEFFData->Draw("same");


        cMassEta->cd(4);
        DiMassElEta[eta][2]->SetTitle("J/#psi #rightarrow ee : Data vs MC Fit + Exp;Mass [GeV];Events");
        DiMassElEta[eta][2]->SetLineColor(kBlack);
        DiMassElEta[eta][2]->Draw("E");
        MCmassElCbExpEta->Draw("same");
        MCmassElBackExpEta->SetLineColor(kGreen);
        MCmassElBackExpEta->Draw("same");
        legEFFData->Draw("same");


        cMassEta->SaveAs(Form("Plots/MCvsData/PlotsEta%d.png",eta));
        
        cout<<"MC Electron Chi2/NDf: "<<elchi2_ndf<<endl;
        cout<<"MC Muon Chi2/NDf: "<<muchi2_ndf<<endl;

        cout<<"True Electron Chi2/NDf: "<<elTruechi2_ndf<<endl;
        cout<<"True Muon Chi2/NDf: "<<muTruechi2_ndf<<endl;

    
    }

}