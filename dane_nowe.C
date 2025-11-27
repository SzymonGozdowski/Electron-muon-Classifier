#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TStyle.h"
#include "TLine.h"
#include "TF1.h"

using namespace std;

void dane_nowe()
{
    // 1. Definicja histogramów
    TStopwatch t;
    TStopwatch t_full;

    TFile *input = new TFile("data23_2trk_moreTCvars.root", "read");
    if (!input || input->IsZombie()) {
        cout << "BLAD: Nie mozna otworzyc pliku!" << endl;
        return;
    }
    
    TTree *tree = (TTree*)input->Get("G2TauTree");
    TH1F *muon_mass = new TH1F("muon_mass", "Invariant_mass_muon", 100, 2.3, 4); //histogram dla mionow
    TH1F *elec_mass = new TH1F("elec_mass", "Invariant_mass_elec", 100, 2.3, 4); //elektorny
    
    UInt_t track_n;
    UInt_t eg_cluster_n;
    Int_t nElec;
    Int_t nMuon;
    
    vector<float> *track_charge = nullptr;
    vector<int> *track_PixeldEdX = nullptr;
    vector<float> *track_TRTHits = nullptr;
    vector<int> *track_PixelEdx = nullptr;
    vector<float> *track_pt = nullptr;
    vector<float> *track_eta = nullptr;
    vector<float> *track_phi = nullptr;
    vector <float> *new_track_PixelHits = nullptr;

    int isElec = 0;
    int isMuon = 0;
    const float massMu = 0.1057;
    const float massElec = 0.00051;
    float mass;

    int is_good = 0;
    tree->SetBranchAddress("track_TRTHits", &track_TRTHits);
    tree->SetBranchAddress("track_PixeldEdX", &track_PixeldEdX);
    tree->Branch("track_PixelHits", &new_track_PixelHits);
    tree->SetBranchAddress("nElec", &nElec);
    tree->SetBranchAddress("nMuon", &nMuon);
    tree->SetBranchAddress("track_n", &track_n);
    tree->SetBranchAddress("track_charge", &track_charge);
    tree->SetBranchAddress("track_pt", &track_pt);
    tree->SetBranchAddress("track_eta", &track_eta);
    tree->SetBranchAddress("track_phi", &track_phi);
    tree->SetBranchAddress("eg_cluster_n", &eg_cluster_n);

    float eta_max = 2.5;
    float pt_sum_max = 0.2;
    float pt_min = 0.1;
    double entries = tree->GetEntries();
    double elec = 0;
    double muon = 0;
    cout << "Liczba zdarzen: " << entries << endl;

    for(long i=0; i<entries*0.1 && i<entries; i++)
    {
        isElec = 0;
        isMuon = 0;
        tree->GetEntry(i);
        if (i%1000000 == 0 && i != 0)
        {
            std::cout << "Element numer: " << i << endl;
            std::cout<<"Czas iteracji: ";
            t.Print();
            t.Start();
            std::cout<<"Laczny czas: ";
            t_full.Print();
            t_full.Continue();
            std::cout<<endl;
        }


        if ((track_n==2) && (*track_charge)[0] != (*track_charge)[1])
        {
            if ((*track_pt)[0]>=pt_min && (*track_pt)[1]>=pt_min && abs((*track_eta)[0])<eta_max && abs((*track_eta)[1]) <eta_max)
            {
                // vector <TLorentzVector> t_vec(2);
                TLorentzVector v1, v2, v_sum;
                
                is_good = 0;
                isElec = 0;
                isMuon = 0;
                
                if (nElec == 1)
                {
                    isElec = 1;
                    mass = massElec;
                    is_good = 1;
                }

                else
                {
                    isMuon = 1;
                    mass = massMu;
                    is_good = 1;
                }
                
                if (is_good ==1)
                {
                    v1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0],(*track_phi)[0], mass);
                    v2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1],(*track_phi)[1], mass);

                    v_sum = v1 + v2;

                    if(v1.DeltaR(v2) >= 0.1)
                    {
                        if (v_sum.Pt()<pt_sum_max)
                        {
                            if (isElec == 1)
                            {
                                 elec += 1;
                                 elec_mass->Fill(v_sum.M());
                            }
                            else if (isMuon == 1)
                            {
                                if (v_sum.M() <= 3.2 && v_sum.M()>=2.9)
                                {
                                    muon += 1;
                                    muon_mass->Fill(v_sum.M());
                                }
                            }
                        }
                    }
                }

                
            }
        }
    }
cout<<"Elektronów: "<<elec<<endl<<"Mionow: "<<muon<<endl;

TCanvas *c = new TCanvas("c", "dwa", 800, 800);
c->Divide(2,1);
c->cd(1);
muon_mass->Draw();
c->cd(2);
elec_mass->Draw();


// fstream file("branch_names.txt", std::ios::out);

// TObjArray *branches = tree->GetListOfBranches();
//     for (int i = 0; i < branches->GetEntries(); i++) {
//         TBranch *br = (TBranch*)branches->At(i);
//         file << br->GetName() << std::endl;
//     }

//     file.close();
//     std::cout << "Nazwy gałęzi zapisane do nazwy_galezi.txt" << std::endl;


}