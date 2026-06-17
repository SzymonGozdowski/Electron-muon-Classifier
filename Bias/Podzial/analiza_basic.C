#define Analysis_cxx
#include <iostream>
#include <vector>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TMath.h"

using namespace std;

const float mass_mu    = 0.1057;
const float mass_elect = 0.00051;
const float pt_min     = 0.1;
const float eta_max    = 2.5;

// ── stara struktura (wejście) ─────────────────────────────────────────────────
struct old_tree_data {
    vector<float> *track_pt     = nullptr;
    vector<float> *track_eta    = nullptr;
    vector<int>   *track_charge = nullptr;
    vector<float> *track_phi    = nullptr;
    UInt_t track_n = 0;

    UInt_t eg_cluster_n = 0;
    vector<float> *eg_cluster_eta = nullptr;
    vector<float> *eg_cluster_phi = nullptr;
    vector<float> *eg_cluster_pt  = nullptr;

    vector<float> *track_TRTHits   = nullptr;
    vector<float> *track_SCTHits   = nullptr;
    vector<float> *track_PixelHits = nullptr;
    vector<int>   *track_PixeldEdX = nullptr;

    UInt_t topoclus_n = 0;
    vector<float> *topo_cluster_eta              = nullptr;
    vector<float> *topo_cluster_phi              = nullptr;
    vector<float> *topo_cluster_pt               = nullptr;
    vector<float> *topo_cluster_EM_prob          = nullptr;
    vector<int>   *topo_cluster_cell_significance = nullptr;
    vector<float> *topo_cluster_lambda           = nullptr;
    vector<float> *topo_cluster_lambda2          = nullptr;
};

// ── nowa struktura (wyjście) ──────────────────────────────────────────────────
struct new_tree_data {
    // cechy ML (11)
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

    // ── dodatkowe gałęzie do wykresów ────────────────────────────────────────
    float inv_mass;      // masa niezmiennicza pary [GeV]
    float track_pt_sel;  // pt wybranego tracka [GeV]
    float track_eta_sel; // eta wybranego tracka
};

// ── pomocnicze ───────────────────────────────────────────────────────────────
float delta_phi(float phi1, float phi2)
{
    float dphi = phi1 - phi2;
    while (dphi >  M_PI) dphi -= 2*M_PI;
    while (dphi < -M_PI) dphi += 2*M_PI;
    return dphi;
}

void old_tree_read(TTree *tree, old_tree_data &d)
{
    tree->SetBranchAddress("track_pt",     &d.track_pt);
    tree->SetBranchAddress("track_eta",    &d.track_eta);
    tree->SetBranchAddress("track_charge", &d.track_charge);
    tree->SetBranchAddress("track_phi",    &d.track_phi);
    tree->SetBranchAddress("track_n",      &d.track_n);

    tree->SetBranchAddress("eg_cluster_n",   &d.eg_cluster_n);
    tree->SetBranchAddress("eg_cluster_eta", &d.eg_cluster_eta);
    tree->SetBranchAddress("eg_cluster_phi", &d.eg_cluster_phi);
    tree->SetBranchAddress("eg_cluster_pt",  &d.eg_cluster_pt);

    tree->SetBranchAddress("track_TRTHits",   &d.track_TRTHits);
    tree->SetBranchAddress("track_SCTHits",   &d.track_SCTHits);
    tree->SetBranchAddress("track_PixelHits", &d.track_PixelHits);
    tree->SetBranchAddress("track_PixeldEdX", &d.track_PixeldEdX);

    tree->SetBranchAddress("topoclus_n",                      &d.topoclus_n);
    tree->SetBranchAddress("topo_cluster_eta",                &d.topo_cluster_eta);
    tree->SetBranchAddress("topo_cluster_phi",                &d.topo_cluster_phi);
    tree->SetBranchAddress("topo_cluster_pt",                 &d.topo_cluster_pt);
    tree->SetBranchAddress("topo_cluster_EM_prob",            &d.topo_cluster_EM_prob);
    tree->SetBranchAddress("topo_cluster_cell_significance",  &d.topo_cluster_cell_significance);
    tree->SetBranchAddress("topo_cluster_lambda",             &d.topo_cluster_lambda);
    tree->SetBranchAddress("topo_cluster_lambda2",            &d.topo_cluster_lambda2);
}

TTree* create_tree(new_tree_data &dn)
{
    TTree *tree = new TTree("MLData", "MLData");

    // cechy ML
    tree->Branch("track_TRTHits",                 &dn.track_TRTHits);
    tree->Branch("track_SCTHits",                 &dn.track_SCTHits);
    tree->Branch("track_PixelHits",               &dn.track_PixelHits);
    tree->Branch("track_PixeldEdX",               &dn.track_PixeldEdX);
    tree->Branch("topo_cluster_eta",              &dn.topo_cluster_eta);
    tree->Branch("topo_cluster_phi",              &dn.topo_cluster_phi);
    tree->Branch("topo_cluster_pt",               &dn.topo_cluster_pt);
    tree->Branch("topo_cluster_EM_prob",          &dn.topo_cluster_EM_prob);
    tree->Branch("topo_cluster_cell_significance",&dn.topo_cluster_cell_significance);
    tree->Branch("topo_cluster_lambda",           &dn.topo_cluster_lambda);
    tree->Branch("topo_cluster_lambda2",          &dn.topo_cluster_lambda2);
    tree->Branch("pt_ratio",                      &dn.pt_ratio);
    tree->Branch("topocount",                     &dn.topocount);

    // dodatkowe do wykresów
    tree->Branch("inv_mass",      &dn.inv_mass);
    tree->Branch("track_pt_sel",  &dn.track_pt_sel);
    tree->Branch("track_eta_sel", &dn.track_eta_sel);

    return tree;
}

// ── główna funkcja ────────────────────────────────────────────────────────────
void analiza_basic()
{
    TFile *input = TFile::Open("../../Data_Raw/jpsi_cell_sig_sampling.root");
    if (!input || input->IsZombie()) {
        cout << "Blad: nie mozna otworzyc pliku wejsciowego!" << endl;
        return;
    }

    TTree *old_tree = (TTree*)input->Get("G2TauTree");
    if (!old_tree) {
        cout << "Blad: nie znaleziono drzewa G2TauTree!" << endl;
        return;
    }

    TFile *output = new TFile("../../Data_Raw/Data_Basic_filter.root", "RECREATE");

    old_tree_data d;
    old_tree_read(old_tree, d);

    new_tree_data dn;
    TTree *tree = create_tree(dn);

    Long64_t entries     = old_tree->GetEntries();
    Long64_t data_points = 0;

    for (Long64_t i = 0; i < entries; i++)
    {
        if (i % 1000000 == 0 && i != 0)
            cout << "Iteration: " << i << "/" << entries << endl;

        old_tree->GetEntry(i);

        // ── podstawowe cięcia ─────────────────────────────────────────────
        if (d.track_n != 2) continue;
        if ((*d.track_charge)[0] == (*d.track_charge)[1]) continue;
        if (abs((*d.track_eta)[0]) > eta_max || abs((*d.track_eta)[1]) > eta_max) continue;
        if ((*d.track_pt)[0] < pt_min       || (*d.track_pt)[1] < pt_min)        continue;

        // ── identyfikacja e/μ przez eg_cluster ───────────────────────────
        int   best_track  = -1;
        float best_deltaR = 999.0;
        float mass        = mass_mu;

        if (d.eg_cluster_n >= 1)
        {
            for (int t = 0; t < 2; t++)
            {
                float min_dr = 999.0;
                for (UInt_t c = 0; c < d.eg_cluster_n; c++)
                {
                    float dr = sqrt(
                        pow((*d.track_eta)[t] - (*d.eg_cluster_eta)[c], 2) +
                        pow(delta_phi((*d.track_phi)[t], (*d.eg_cluster_phi)[c]), 2));
                    if (dr < min_dr) min_dr = dr;
                }
                if (min_dr < best_deltaR) { best_deltaR = min_dr; best_track = t; }
            }
            if (best_deltaR <= 0.2) mass = mass_elect;
        }
        if (best_track == -1) best_track = 0;

        // ── TLorentzVector i masa niezmiennicza ───────────────────────────
        TLorentzVector v1, v2, vsum;
        v1.SetPtEtaPhiM((*d.track_pt)[0], (*d.track_eta)[0], (*d.track_phi)[0], mass);
        v2.SetPtEtaPhiM((*d.track_pt)[1], (*d.track_eta)[1], (*d.track_phi)[1], mass);
        vsum = v1 + v2;

        if (vsum.Pt() > 0.2) continue;

        int sel = best_track;

        // ── topocluster matching ──────────────────────────────────────────
        int   best_topo    = -1;
        float best_topo_pt = 0.0;
        int   topo_count   = 0;
        float pt_ratio_val = -1.0;

        for (UInt_t j = 0; j < d.topoclus_n; j++)
        {
            float dr = sqrt(
                pow((*d.track_eta)[sel] - (*d.topo_cluster_eta)[j], 2) +
                pow(delta_phi((*d.track_phi)[sel], (*d.topo_cluster_phi)[j]), 2));

            if (dr <= 0.5)
            {
                topo_count++;
                if ((*d.topo_cluster_pt)[j] > best_topo_pt)
                {
                    best_topo_pt = (*d.topo_cluster_pt)[j];
                    best_topo    = j;
                }
            }
        }
        if (best_topo != -1)
            pt_ratio_val = best_topo_pt / (*d.track_pt)[sel];

        // ── wypełnienie struktury ─────────────────────────────────────────
        dn.track_TRTHits   = (*d.track_TRTHits)[sel];
        dn.track_SCTHits   = (*d.track_SCTHits)[sel];
        dn.track_PixelHits = (*d.track_PixelHits)[sel];
        dn.track_PixeldEdX = (*d.track_PixeldEdX)[sel];

        if (best_topo != -1)
        {
            int j = best_topo;
            dn.topo_cluster_eta               = (*d.topo_cluster_eta)[j];
            dn.topo_cluster_phi               = (*d.topo_cluster_phi)[j];
            dn.topo_cluster_pt                = (*d.topo_cluster_pt)[j];
            dn.topo_cluster_EM_prob           = (*d.topo_cluster_EM_prob)[j];
            dn.topo_cluster_cell_significance = (*d.topo_cluster_cell_significance)[j];
            dn.topo_cluster_lambda            = (*d.topo_cluster_lambda)[j];
            dn.topo_cluster_lambda2           = (*d.topo_cluster_lambda2)[j];
            dn.pt_ratio                       = pt_ratio_val;
            dn.topocount                      = topo_count;
        }
        else
        {
            dn.topo_cluster_eta               = -999;
            dn.topo_cluster_phi               = -999;
            dn.topo_cluster_pt                = -999;
            dn.topo_cluster_EM_prob           = -1;
            dn.topo_cluster_cell_significance = -1;
            dn.topo_cluster_lambda            = -1;
            dn.topo_cluster_lambda2           = -1;
            dn.pt_ratio                       = -1;
            dn.topocount                      = 0;
        }

        // dodatkowe do wykresów
        dn.inv_mass      = vsum.M();
        dn.track_pt_sel  = (*d.track_pt)[sel];
        dn.track_eta_sel = (*d.track_eta)[sel];

        tree->Fill();
        data_points++;
    }

    tree->Write();
    output->Close();
    input->Close();

    cout << "Zapisano eventow: " << data_points << endl;
}