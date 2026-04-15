#define Analysis_cxx
#include <iostream>
#include <vector>
#include <cmath>

#include "TFile.h"
#include "TTree.h"

using namespace std;

struct old_tree_data{
    vector<float> *track_pt = nullptr;
    vector<float> *track_eta = nullptr;
    vector<int>   *track_charge = nullptr;
    vector<float> *track_phi = nullptr;

    UInt_t track_n = 0;

    vector<float> *track_TRTHits   = nullptr;
    vector<float> *track_SCTHits   = nullptr;
    vector<float> *track_PixelHits = nullptr;
    vector<int>   *track_PixeldEdX = nullptr;

    UInt_t topoclus_n = 0;
    vector<float> *topo_cluster_eta     = nullptr;
    vector<float> *topo_cluster_phi     = nullptr;
    vector<float> *topo_cluster_pt      = nullptr;
    vector<float> *topo_cluster_EM_prob = nullptr;
    vector<int>   *topo_cluster_cell_significance = nullptr;
    vector<float> *topo_cluster_lambda = nullptr;
    vector<float> *topo_cluster_lambda2 = nullptr;
};

struct new_tree_data{
    float track_TRTHits;
    float track_SCTHits;
    float track_PixelHits;
    int   track_PixeldEdX;

    float topo_cluster_eta;
    float topo_cluster_phi;
    float topo_cluster_pt;
    float topo_cluster_EM_prob;
    int   topo_cluster_cell_significance;
    float topo_cluster_lambda;
    float topo_cluster_lambda2;

    float pt_ratio;
    int   topocount;
};

float delta_phi(float phi1, float phi2)
{
    float dphi = phi1 - phi2;
    while (dphi > M_PI) dphi -= 2*M_PI;
    while (dphi < -M_PI) dphi += 2*M_PI;
    return dphi;
}

void old_tree_read(TTree *tree, old_tree_data &d)
{
    tree->SetBranchAddress("track_pt", &d.track_pt);
    tree->SetBranchAddress("track_eta", &d.track_eta);
    tree->SetBranchAddress("track_charge", &d.track_charge);
    tree->SetBranchAddress("track_phi", &d.track_phi);
    tree->SetBranchAddress("track_n", &d.track_n);

    tree->SetBranchAddress("track_TRTHits", &d.track_TRTHits);
    tree->SetBranchAddress("track_SCTHits", &d.track_SCTHits);
    tree->SetBranchAddress("track_PixelHits", &d.track_PixelHits);
    tree->SetBranchAddress("track_PixeldEdX", &d.track_PixeldEdX);

    tree->SetBranchAddress("topoclus_n", &d.topoclus_n);
    tree->SetBranchAddress("topo_cluster_eta", &d.topo_cluster_eta);
    tree->SetBranchAddress("topo_cluster_phi", &d.topo_cluster_phi);
    tree->SetBranchAddress("topo_cluster_pt", &d.topo_cluster_pt);
    tree->SetBranchAddress("topo_cluster_EM_prob", &d.topo_cluster_EM_prob);
    tree->SetBranchAddress("topo_cluster_cell_significance", &d.topo_cluster_cell_significance);
    tree->SetBranchAddress("topo_cluster_lambda", &d.topo_cluster_lambda);
    tree->SetBranchAddress("topo_cluster_lambda2", &d.topo_cluster_lambda2);
}

TTree* create_tree(new_tree_data &dn)
{
    TTree *tree = new TTree("MLData", "MLData");

    tree->Branch("track_TRTHits", &dn.track_TRTHits);
    tree->Branch("track_SCTHits", &dn.track_SCTHits);
    tree->Branch("track_PixelHits", &dn.track_PixelHits);
    tree->Branch("track_PixeldEdX", &dn.track_PixeldEdX);

    tree->Branch("topo_cluster_eta", &dn.topo_cluster_eta);
    tree->Branch("topo_cluster_phi", &dn.topo_cluster_phi);
    tree->Branch("topo_cluster_pt", &dn.topo_cluster_pt);
    tree->Branch("topo_cluster_EM_prob", &dn.topo_cluster_EM_prob);
    tree->Branch("topo_cluster_cell_significance", &dn.topo_cluster_cell_significance);
    tree->Branch("topo_cluster_lambda", &dn.topo_cluster_lambda);
    tree->Branch("topo_cluster_lambda2", &dn.topo_cluster_lambda2);

    tree->Branch("pt_ratio", &dn.pt_ratio);
    tree->Branch("topocount", &dn.topocount);

    return tree;
}

void analiza_basic()
{
    TFile *input = TFile::Open("../../Data_Raw/jpsi_cell_sig_sampling.root");
    TTree *old_tree = (TTree*)input->Get("G2TauTree");

    TFile *output = new TFile("../../Data_Raw/Data_Basic_filter.root", "RECREATE");

    old_tree_data d;
    old_tree_read(old_tree, d);

    new_tree_data dn;
    TTree *tree = create_tree(dn);

    double entries = old_tree->GetEntries();
    double data_points = 0;

    for (int i = 0; i < entries; i++)
    {
        old_tree->GetEntry(i);

        if (d.track_n != 2) continue;
        if ((*d.track_charge)[0] == (*d.track_charge)[1]) continue;

        int sel = 0; // wybierasz jeden track

        int best_topo = -1;
        float best_pt = 0;
        int count = 0;

        for (int j = 0; j < d.topoclus_n; j++)
        {
            float dr = sqrt(
                pow((*d.track_eta)[sel] - (*d.topo_cluster_eta)[j], 2) +
                pow(delta_phi((*d.track_phi)[sel], (*d.topo_cluster_phi)[j]), 2)
            );

            if (dr < 0.5)
            {
                count++;
                if ((*d.topo_cluster_pt)[j] > best_pt)
                {
                    best_pt = (*d.topo_cluster_pt)[j];
                    best_topo = j;
                }
            }
        }

        // TRACK
        dn.track_TRTHits = (*d.track_TRTHits)[sel];
        dn.track_SCTHits = (*d.track_SCTHits)[sel];
        dn.track_PixelHits = (*d.track_PixelHits)[sel];
        dn.track_PixeldEdX = (*d.track_PixeldEdX)[sel];

        // TOPO
        if (best_topo != -1)
        {
            dn.topo_cluster_eta = (*d.topo_cluster_eta)[best_topo];
            dn.topo_cluster_phi = (*d.topo_cluster_phi)[best_topo];
            dn.topo_cluster_pt  = (*d.topo_cluster_pt)[best_topo];
            dn.topo_cluster_EM_prob = (*d.topo_cluster_EM_prob)[best_topo];
            dn.topo_cluster_cell_significance = (*d.topo_cluster_cell_significance)[best_topo];
            dn.topo_cluster_lambda = (*d.topo_cluster_lambda)[best_topo];
            dn.topo_cluster_lambda2 = (*d.topo_cluster_lambda2)[best_topo];

            dn.pt_ratio = best_pt / (*d.track_pt)[sel];
            dn.topocount = count;
        }
        else
        {
            dn.topo_cluster_eta = -999;
            dn.topo_cluster_phi = -999;
            dn.topo_cluster_pt  = -999;
            dn.topo_cluster_EM_prob = -1;
            dn.topo_cluster_cell_significance = -1;
            dn.topo_cluster_lambda = -1;
            dn.topo_cluster_lambda2 = -1;

            dn.pt_ratio = -1;
            dn.topocount = 0;
        }

        tree->Fill();
        data_points++;
    }

    tree->Write();
    output->Close();

    cout << "Danych: " << data_points << endl;
}