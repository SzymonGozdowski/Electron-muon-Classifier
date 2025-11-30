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


void division()
{
    /*clustering based on:
    - energy deposited in first calorymetr - rg_cluster class
    - space direction reconstructed in calorymetr vs in tracker
    - all criteria which characterized coherent particle collision (pt, eta, invariant mass)
    */

    const float mass_mu = 0.1057;
    const float mass_elect = 0.00051;

    //CONDITIONS
    int n_track = 2; 
    float pt_min = 0.1;  //0.1 gev for each track
    float pt_max_sum = 0.2; //for coherent
    float eta_max = 2.5; // for detector efficency

    std::string path = "../data_new.root";


    //data import
    TFile *input = TFile::Open(path.c_str(), "READ");
    TTree *old_tree = (TTree*)input->Get("G2TauTree");

   /*
track_pt, type: vector<float>
track_eta, type: vector<float>
track_charge, type: vector<int>
track_phi, type: vector<float>
track_n, type: UInt_t
track_TRTHits, type: vector<float>
track_SCTHits type: vector<float>
track_PixelHits type: vector<float>
track_PixelEdx type: vector<int>
eg_cluster_n, type: UInt_t
eg_cluster_eta, type: vector<float>
eg_cluster_phi, type: vector<float>
   */

   vector <float> *track_pt = nullptr;
   vector <float> *track_eta = nullptr;
   vector <int>   *track_charge = nullptr;
   vector <float> *track_phi = nullptr;
   UInt_t track_n = 0;
   UInt_t eg_cluster_n  = 0;
   vector <float> *eg_cluster_eta = nullptr;
   vector <float> *eg_cluster_phi = nullptr;
   vector <float> *eg_cluster_pt = nullptr;

   vector <float> *track_TRTHits = nullptr;
   vector <float> *track_SCTHits = nullptr;
   vector <float> *track_PixelHits = nullptr;
   vector <int> *track_PixeldEdX = nullptr;

   old_tree->SetBranchAddress("track_pt", &track_pt);
   old_tree->SetBranchAddress("track_eta", &track_eta);
   old_tree->SetBranchAddress("track_charge", &track_charge);
   old_tree->SetBranchAddress("track_phi", &track_phi);
   old_tree->SetBranchAddress("track_n", &track_n);
   old_tree->SetBranchAddress("eg_cluster_n", &eg_cluster_n);
   old_tree->SetBranchAddress("eg_cluster_eta", &eg_cluster_eta);
   old_tree->SetBranchAddress("eg_cluster_phi", &eg_cluster_phi);
   old_tree->SetBranchAddress("track_TRTHits", &track_TRTHits);
   old_tree->SetBranchAddress("track_SCTHits", &track_SCTHits);
   old_tree->SetBranchAddress("track_PixelHits", &track_PixelHits);
   old_tree->SetBranchAddress("track_PixeldEdX", &track_PixeldEdX);

   double entries = old_tree->GetEntriesFast();
   Long64_t num_elec = 0;
   Long64_t num_muon = 0;

   cout<<std::scientific<<entries<<endl;
   TStopwatch t_full, t_iter;


   TFile *file = new TFile("data.root", "recreate");
   //Hists for sum 
   TH1F *muon_pt = new TH1F("muon_pt", "muon_pt", 100, 0, 0.25);
   TH1F *muon_eta = new TH1F("muon_eta", "muon_eta", 100, -2.55, 2.55);
   TH1F *muon_phi = new TH1F("muon_phi", "muon_phi", 100, -3.15, 3.15);
   TH1F *muon_m = new TH1F("muon_m", "muon_m", 100, 2.9, 3.3);
   
   TH1F *elec_pt = new TH1F("elec_pt", "elec_pt", 100, 0, 0.25);
   TH1F *elec_eta = new TH1F("elec_eta", "elec_eta", 100, -2.55, 2.55);
   TH1F *elec_phi = new TH1F("elec_phi", "elec_phi", 100, -3.15, 3.15);
   TH1F *elec_m = new TH1F("elec_m", "elec_m", 100, 2.9, 3.3);

   //only potential electron
   TH1F *single_elec_pt = new TH1F("single_elec_pt", "elec_pt", 100, 0, 5);
   TH1F *single_elec_eta = new TH1F("single_elec_eta", "elec_eta", 100, -2.55, 2.55);
   TH1F *single_elec_phi = new TH1F("single_elec_phi", "elec_phi", 100, -3.15, 3.15);

   TH1F *single_muon_pt = new TH1F("single_muon_pt", "muon_pt", 100, 0.5, 2);
   TH1F *single_muon_eta = new TH1F("single_muon_eta", "muon_eta", 100, -2.55, 2.55);
   TH1F *single_muon_phi = new TH1F("single_muon_phi", "muon_phi", 100, -3.15, 3.15);


   //MAIN LOOP
   for (int i=0; i<entries; i++)
   {
    int elec_track;

        if (i% 1000000 ==0 && i!= 0)
        {
            cout<<"Iteration: "<<i<<"/"<<entries<<endl;
            t_iter.Print();
            t_iter.Start();
        }
        int is_elec = 0;
        int is_muon = 0;

        old_tree->GetEntry(i);

        if (track_n == 2 && (*track_charge)[0] != (*track_charge)[1])
        {
            if (abs((*track_eta)[0]) <= eta_max && abs((*track_eta)[1]) <= eta_max)
            {
                if ((*track_pt)[0] >= pt_min && (*track_pt)[1] >= pt_min)
                {  
                    float mass = mass_mu;
                    is_muon = 1;

                        if(eg_cluster_n >= 1)
                        {
                                int cluster_best, track_best; //Lowest Delta R
                                float low_delta = 10.0;
                                float delta_r =0.0;
                                is_muon = 0; //(?) if there is 2 deposition of energy, there probably was no muon

                                for (int j=0; j< eg_cluster_n; j++)
                                {
                                    for (int idx = 0; idx < track_n; idx++)
                                    {

                                        delta_r =  sqrt(pow( (*track_eta)[idx] - (*eg_cluster_eta)[j], 2) + 
                                                        pow( (*track_phi)[idx] - (*eg_cluster_phi)[j], 2)); //def: delta_r = sqrt(delta eta^2 + delta phi^2)
                                        
                                        if(delta_r <low_delta)
                                        {
                                            low_delta = delta_r;
                                            track_best = idx;
                                            cluster_best = j;
                                        }
                                    }
                                }

                            if (low_delta<= 0.5)
                            { //checking for the other track
                                elec_track = 1-track_best; //if j = 1 then elec_track is 0 else 1
                                delta_r =  sqrt(pow( (*track_eta)[elec_track] - (*eg_cluster_eta)[1-cluster_best], 2) + 
                                                        pow( (*track_phi)[elec_track] - (*eg_cluster_phi)[1-cluster_best], 2));
                                if (delta_r <= 0.5)
                                {
                                    is_elec = 1;
                                    is_muon = 0; //here we assume that first vector is electron so second will also be an electron
                                    mass = mass_elect;
                                }
                            }
                        }

                        TLorentzVector v1, v2, vsum;

                        v1.SetPtEtaPhiM( (*track_pt)[0], (*track_eta)[0], (*track_phi)[0], mass);
                        v2.SetPtEtaPhiM( (*track_pt)[1], (*track_eta)[1], (*track_phi)[1], mass);
                        vsum = v1+v2;

                        if (vsum.Pt() <= 0.2)
                        {
                            if(is_muon == 1)
                            {
                                if(vsum.M() >= 2.9 && vsum.M() <= 3.2)
                                {
                                    num_muon += 1;

                                    muon_pt->Fill(vsum.Pt());
                                    muon_eta->Fill(vsum.Eta());
                                    muon_phi->Fill(vsum.Phi());
                                    muon_m->Fill(vsum.M());
                                    
                                    single_muon_eta->Fill(v1.Eta());
                                    single_muon_eta->Fill(v2.Eta());
                                    single_muon_pt->Fill(v1.Pt());
                                    single_muon_pt->Fill(v2.Pt());
                                    single_muon_phi->Fill(v1.Phi());
                                    single_muon_phi->Fill(v2.Phi());
                                }
                            }

                            else if (is_elec ==1 )
                            {
                                TLorentzVector electron;
                                if (elec_track == 0)  electron = v1;
                                else electron = v2;
                                if (electron.E() != 0)
                                {
                                    num_elec += 1;
                                    elec_pt->Fill(vsum.Pt());
                                    elec_eta->Fill(vsum.Eta());
                                    elec_phi->Fill(vsum.Phi());
                                    elec_m->Fill(vsum.M());
                                    
                                    single_elec_pt->Fill(electron.Pt());
                                    single_elec_eta->Fill(electron.Eta());
                                    single_elec_phi->Fill(electron.Phi());
                                }
                            }
                        }

                        
                    }
                 }
            }
     }
t_full.Print();
input->Close();
cout<<endl<<"Muons: "<<num_muon*2<<endl<<"Single Electrons: "<<num_elec;

// Create canvases and draw plots
TCanvas *c1 = new TCanvas("c1", "Muon Distributions", 1200, 800);
c1->Divide(2, 2);

c1->cd(1);
muon_pt->Draw();

c1->cd(2);
muon_eta->Draw();

c1->cd(3);
muon_phi->Draw();

c1->cd(4);
muon_m->Draw();
c1->SaveAs("muons_pair.png");

TCanvas *c2 = new TCanvas("c2", "Electron Distributions", 1200, 800);
c2->Divide(2, 2);

c2->cd(1);
elec_pt->Draw();

c2->cd(2);
elec_eta->Draw();

c2->cd(3);
elec_phi->Draw();

c2->cd(4);
elec_m->Draw();
c2->SaveAs("electrons_pair.png");

TCanvas *c3 = new TCanvas("c3", "Single Track Distributions", 1200, 800);
c3->Divide(2, 3);

c3->cd(1);
single_muon_pt->Draw();

c3->cd(2);
single_muon_eta->Draw();

c3->cd(3);
single_muon_phi->Draw();

c3->cd(4);
single_elec_pt->Draw();

c3->cd(5);
single_elec_eta->Draw();

c3->cd(6);
single_elec_phi->Draw();
c3->SaveAs("elec_muon_single.png");

}