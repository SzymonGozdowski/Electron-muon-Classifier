#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Przetwarza plik MC.root do CSV.
// Zapisuje 11 cech ML + dodatkowe do wykresow:
//   inv_mass      – masa niezmiennicza pary [GeV]
//   track_pt_sel  – pt wybranego tracka [GeV]
//   track_eta_sel – eta wybranego tracka

const float mass_mu    = 0.1057f;
const float mass_elect = 0.00051f;
const float pt_min     = 0.1f;
const float eta_max    = 2.5f;

float delta_phi(float phi1, float phi2)
{
    float dphi = phi1 - phi2;
    while (dphi >  M_PI) dphi -= 2*M_PI;
    while (dphi < -M_PI) dphi += 2*M_PI;
    return dphi;
}

// is_electron = true  → plik ee,  użyj masy elektronu
// is_electron = false → plik mumu, użyj masy mionu
void process_to_csv(const char* input_path,
                    const char* output_path,
                    bool        is_electron,
                    long long   max_entries = -1)
{
    TFile* file = TFile::Open(input_path);
    if (!file || file->IsZombie()) {
        std::cerr << "Blad otwarcia: " << input_path << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("G2TauTree");
    if (!tree) {
        std::cerr << "Nie znaleziono drzewa G2TauTree w: " << input_path << std::endl;
        file->Close();
        return;
    }

    // ── gałęzie wejściowe ─────────────────────────────────────────────────
    std::vector<int>*   track_TRTHits    = nullptr;
    std::vector<int>*   track_PixelHits  = nullptr;
    std::vector<int>*   track_SCTHits    = nullptr;
    std::vector<float>* track_PixeldEdX  = nullptr;
    std::vector<float>* track_pt         = nullptr;
    std::vector<float>* track_eta        = nullptr;
    std::vector<float>* track_phi        = nullptr;
    std::vector<int>*   track_charge     = nullptr;
    UInt_t track_n = 0;

    std::vector<float>* topo_cluster_eta      = nullptr;
    std::vector<float>* topo_cluster_phi      = nullptr;
    std::vector<float>* topo_cluster_EM_prob  = nullptr;
    std::vector<float>* topo_cluster_pt       = nullptr;
    std::vector<float>* topo_cluster_lambda   = nullptr;
    std::vector<float>* topo_cluster_lambda2  = nullptr;
    UInt_t topoclus_n = 0;

    tree->SetBranchAddress("track_TRTHits",   &track_TRTHits);
    tree->SetBranchAddress("track_PixelHits", &track_PixelHits);
    tree->SetBranchAddress("track_SCTHits",   &track_SCTHits);
    tree->SetBranchAddress("track_PixeldEdX", &track_PixeldEdX);
    tree->SetBranchAddress("track_pt",        &track_pt);
    tree->SetBranchAddress("track_eta",       &track_eta);
    tree->SetBranchAddress("track_phi",       &track_phi);
    tree->SetBranchAddress("track_charge",    &track_charge);
    tree->SetBranchAddress("track_n",         &track_n);

    tree->SetBranchAddress("topo_cluster_eta",      &topo_cluster_eta);
    tree->SetBranchAddress("topo_cluster_phi",      &topo_cluster_phi);
    tree->SetBranchAddress("topo_cluster_EM_prob",  &topo_cluster_EM_prob);
    tree->SetBranchAddress("topo_cluster_pt",       &topo_cluster_pt);
    tree->SetBranchAddress("topo_cluster_lambda",   &topo_cluster_lambda);
    tree->SetBranchAddress("topo_cluster_lambda2",  &topo_cluster_lambda2);
    tree->SetBranchAddress("topoclus_n",            &topoclus_n);

    // ── nagłówek CSV ──────────────────────────────────────────────────────
    std::ofstream csv(output_path);
    csv << "track_TRTHits,track_PixelHits,track_SCTHits,track_PixeldEdX,"
        << "topo_cluster_eta,topo_cluster_phi,topo_cluster_EM_prob,topo_cluster_pt,"
        << "pt_ratio,topo_cluster_lambda,topo_cluster_lambda2,"
        << "inv_mass,track_pt_sel,track_eta_sel\n";

    float mass = is_electron ? mass_elect : mass_mu;

    long long n_total   = tree->GetEntries();
    long long n_limit   = (max_entries > 0) ? std::min(max_entries, n_total) : n_total;
    long long n_saved   = 0;
    long long n_skipped = 0;

    std::cout << "Przetwarzam: " << input_path
              << "  (" << n_total << " eventow)" << std::endl;

    for (long long i = 0; i < n_limit; ++i)
    {
        tree->GetEntry(i);

        // ── podstawowe cięcia ─────────────────────────────────────────────
        if (track_n != 2)                                          { ++n_skipped; continue; }
        if ((*track_charge)[0] == (*track_charge)[1])             { ++n_skipped; continue; }
        if (track_pt->empty() || topo_cluster_pt->empty())        { ++n_skipped; continue; }
        if ((*track_pt)[0] < pt_min  || (*track_pt)[1] < pt_min) { ++n_skipped; continue; }
        if (fabs((*track_eta)[0]) > eta_max ||
            fabs((*track_eta)[1]) > eta_max)                      { ++n_skipped; continue; }

        // ── TLorentzVector i masa niezmiennicza ───────────────────────────
        TLorentzVector v1, v2, vsum;
        v1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0], (*track_phi)[0], mass);
        v2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1], (*track_phi)[1], mass);
        vsum = v1 + v2;

        if (vsum.Pt() > 0.2) { ++n_skipped; continue; }

        int sel = 0;  // zawsze track 0, tak jak w analiza_basic

        // ── topocluster matching ──────────────────────────────────────────
        int   best_topo    = -1;
        float best_topo_pt = 0.0f;
        int   topo_count   = 0;

        for (UInt_t j = 0; j < topoclus_n; ++j)
        {
            float dr = sqrt(
                pow((*track_eta)[sel] - (*topo_cluster_eta)[j], 2) +
                pow(delta_phi((*track_phi)[sel], (*topo_cluster_phi)[j]), 2));

            if (dr <= 0.5f)
            {
                ++topo_count;
                if ((*topo_cluster_pt)[j] > best_topo_pt)
                {
                    best_topo_pt = (*topo_cluster_pt)[j];
                    best_topo    = j;
                }
            }
        }

        if (best_topo == -1) { ++n_skipped; continue; }

        float pt_ratio_val = best_topo_pt / (*track_pt)[sel];

        // ── zapis ─────────────────────────────────────────────────────────
        csv << (*track_TRTHits)[sel]              << ","
            << (*track_PixelHits)[sel]            << ","
            << (*track_SCTHits)[sel]              << ","
            << (*track_PixeldEdX)[sel]            << ","
            << (*topo_cluster_eta)[best_topo]     << ","
            << (*topo_cluster_phi)[best_topo]     << ","
            << (*topo_cluster_EM_prob)[best_topo] << ","
            << (*topo_cluster_pt)[best_topo]      << ","
            << pt_ratio_val                       << ","
            << (*topo_cluster_lambda)[best_topo]  << ","
            << (*topo_cluster_lambda2)[best_topo] << ","
            << vsum.M()                           << ","
            << (*track_pt)[sel]                   << ","
            << (*track_eta)[sel]                  << "\n";

        ++n_saved;

        if (i % 100000 == 0 && i > 0)
            std::cout << "  " << i << " / " << n_limit << std::endl;
    }

    csv.close();
    file->Close();

    std::cout << "Gotowe! Zapisano " << n_saved << " wierszy"
              << "  (pominieto: " << n_skipped << ")" << std::endl << std::endl;
}

void converter()
{
    // ELEKTRONY – plik ee, masa elektronu
    process_to_csv(
        "../../Data_Raw/mc_jpsi_ee.root",
        "../../Data_Separated/MC/electron.csv",
        true    // is_electron = true → masa 0.00051 GeV
    );

    // MIONY – plik mumu, masa mionu
    process_to_csv(
        "../../Data_Raw/mc_jpsi_mumu.root",
        "../../Data_Separated/MC/muon.csv",
        false   // is_electron = false → masa 0.1057 GeV
    );

    std::cout << "=== Konwersja zakonczona ===" << std::endl;
}