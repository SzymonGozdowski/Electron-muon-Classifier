#define Analysis_cxx
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

struct old_tree_data{
    vector<float> *track_pt = nullptr;
    vector<float> *track_eta = nullptr;
    vector<int>   *track_charge = nullptr;
    vector<float> *track_phi = nullptr;

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
    vector<float> *topo_cluster_eta     = nullptr;
    vector<float> *topo_cluster_phi     = nullptr;
    vector<float> *topo_cluster_pt      = nullptr;
    vector<float> *topo_cluster_EM_prob = nullptr;
    vector<int>   *topo_cluster_cell_significance = nullptr;
    vector<float> *topo_cluster_lambda = nullptr;
    vector<float> *topo_cluster_lambda2 = nullptr;
    UInt_t topocount = 0;
};

struct new_tree_data{
    vector<float>* track_TRTHits   = nullptr;
    vector<float>* track_SCTHits   = nullptr;
    vector<float>* track_PixelHits = nullptr;
    vector<int>*   track_PixeldEdX = nullptr;

    vector<float> *topo_cluster_eta     = nullptr;
    vector<float> *topo_cluster_phi     = nullptr;
    vector<float> *topo_cluster_pt      = nullptr;
    vector<float> *topo_cluster_EM_prob = nullptr;
    vector<int>   *topo_cluster_cell_significance = nullptr;
    vector<float> *pt_ratio = nullptr;
    vector<float> *topo_cluster_lambda = nullptr;
    vector<float> *topo_cluster_lambda2 = nullptr;
    UInt_t topocount = 0;
};

struct Hists{
    TH1F *muon_pt   = nullptr;
    TH1F *muon_eta  = nullptr;
    TH1F *muon_phi  = nullptr;
    TH1F *muon_m    = nullptr;

    TH1F *elec_pt   = nullptr;
    TH1F *elec_eta  = nullptr;
    TH1F *elec_phi  = nullptr;
    TH1F *elec_m    = nullptr;

    TH1F *single_elec_pt   = nullptr;
    TH1F *single_elec_eta  = nullptr;
    TH1F *single_elec_phi  = nullptr;

    TH1F *single_muon_pt   = nullptr;
    TH1F *single_muon_eta  = nullptr;
    TH1F *single_muon_phi  = nullptr;

    TH1F *single_elec_trt    = nullptr;
    TH1F *single_elec_sct    = nullptr;
    TH1F *single_elec_pixel = nullptr;

    TH1F *single_muon_trt    = nullptr;
    TH1F *single_muon_sct    = nullptr;
    TH1F *single_muon_pixel = nullptr;

    TH1F *single_elec_topo_cluster_pt      = nullptr;
    TH1F *single_elec_topo_cluster_eta     = nullptr;
    TH1F *single_elec_topo_cluster_phi     = nullptr;
    TH1F *single_elec_topo_cluster_EM_prob = nullptr;
    TH1F *single_elec_topo_cluster_cell_significance = nullptr;
    TH1F *elec_topocount = nullptr;
    TH1F *single_elec_topo_cluster_pt_ratio = nullptr;

    TH1F *single_muon_topo_cluster_pt      = nullptr;
    TH1F *single_muon_topo_cluster_eta     = nullptr;
    TH1F *single_muon_topo_cluster_phi     = nullptr;
    TH1F *single_muon_topo_cluster_EM_prob = nullptr;
    TH1F *single_muon_topo_cluster_cell_significance = nullptr;
    TH1F *muon_topocount = nullptr;
    TH1F *single_muon_topo_cluster_pt_ratio = nullptr;

    TH1F *single_elec_topo_cluster_lambda = nullptr;
    TH1F *single_elec_topo_cluster_lambda2 = nullptr;
    TH1F *single_muon_topo_cluster_lambda = nullptr;
    TH1F *single_muon_topo_cluster_lambda2 = nullptr;

    TH1F *single_muon_topo_cluster_pt_clean = nullptr;
    TH1F *single_muon_topo_cluster_eta_clean = nullptr;
    TH1F *single_muon_topo_cluster_phi_clean = nullptr;
    TH1F *single_muon_topo_cluster_EM_prob_clean = nullptr;
    TH1F *single_muon_topo_cluster_cell_significance_clean = nullptr;
    TH1F *single_muon_topo_cluster_pt_ratio_clean = nullptr;

    TH1F *single_elec_topo_cluster_pt_clean = nullptr;
    TH1F *single_elec_topo_cluster_eta_clean = nullptr;
    TH1F *single_elec_topo_cluster_phi_clean = nullptr;
    TH1F *single_elec_topo_cluster_EM_prob_clean = nullptr;
    TH1F *single_elec_topo_cluster_cell_significance_clean = nullptr;
    TH1F *single_elec_topo_cluster_pt_ratio_clean = nullptr;
};

void old_tree_read(TTree *old_tree, old_tree_data &d);
void plot_hists(Hists &h);
float delta_phi(float phi1, float phi2);
std::pair<TTree*, TTree*> create_new_trees(new_tree_data &d_muon, new_tree_data &d_elec);
void init_hists(Hists &h);
void init_new_data(new_tree_data &dn);
void clear_new_data(new_tree_data &dn);

void analiza()
{
    const float mass_mu = 0.1057;
    const float mass_elect = 0.00051;

    float pt_min = 0.1;
    float eta_max = 2.5;

    std::string path = "../../Data_Raw/jpsi_cell_sig_sampling.root";
    // std::string path = "C:/Users/qmiko/Desktop/Studia/cern/Bias/Podzial/data_separated.root";
    TFile *input = TFile::Open(path.c_str(), "READ");

    if (!input || input->IsZombie()) {
        cout << "Błąd: Nie można otworzyć pliku wejściowego!" << endl;
        return;
    }

    TTree *old_tree = (TTree*)input->Get("G2TauTree");
    if (!old_tree) {
        cout << "Błąd: Nie znaleziono drzewa G2TauTree w pliku!" << endl;
        return;
    }

    TFile *output = new TFile("../../Data_Separated/Bias/MLDataBias.root", "recreate");
    // TFile *output = new TFile("C:/Users/qmiko/Desktop/Studia/cern/Data_Separated/Bias/MLDataBias.root", "recreate");
    

    old_tree_data d;
    old_tree_read(old_tree, d);

    new_tree_data dn_muon;
    new_tree_data dn_elec;
    init_new_data(dn_muon);
    init_new_data(dn_elec);
    auto [tree_muon, tree_elec] = create_new_trees(dn_muon, dn_elec);
    Hists h;
    init_hists(h);

    double entries = old_tree->GetEntriesFast();
    Long64_t num_elec = 0;
    Long64_t num_muon = 0;

    for (int i = 0; i < entries; i++)
    {
        if (i % 1000000 == 0 && i != 0)
            cout << "Iteration: " << i << "/" << entries << endl;

        old_tree->GetEntry(i);

        if (d.track_n != 2) continue;
        if ((*d.track_charge)[0] == (*d.track_charge)[1]) continue;
        if (abs((*d.track_eta)[0]) > eta_max || abs((*d.track_eta)[1]) > eta_max) continue;
        if ((*d.track_pt)[0] < pt_min || (*d.track_pt)[1] < pt_min) continue;

        int best_track = -1;
        float best_deltaR = 999.0;
        int is_elec = 0;
        int is_muon = 1;
        float mass = mass_mu;

        if (d.eg_cluster_n >= 1)
        {
            is_muon = 0;
            for (int t = 0; t < 2; t++)
            {
                float min_dr = 999.0;
                for (int c = 0; c < d.eg_cluster_n; c++)
                {
                    float dr = sqrt(pow((*d.track_eta)[t] - (*d.eg_cluster_eta)[c], 2) +
                                    pow(delta_phi((*d.track_phi)[t], (*d.eg_cluster_phi)[c]), 2));
                    if (dr < min_dr) min_dr = dr;
                }
                if (min_dr < best_deltaR)
                {
                    best_deltaR = min_dr;
                    best_track = t;
                }
            }

            if (best_deltaR <= 0.2)
            {
                is_elec = 1;
                // is_muon = 0;
                mass = mass_elect;
            }
        }
        else
        {
            best_track = 0; // jeśli brak eg_cluster, bierz pierwszy
        }

        if (best_track == -1) best_track = 0;

        TLorentzVector v1, v2, vsum;
        v1.SetPtEtaPhiM((*d.track_pt)[0], (*d.track_eta)[0], (*d.track_phi)[0], mass);
        v2.SetPtEtaPhiM((*d.track_pt)[1], (*d.track_eta)[1], (*d.track_phi)[1], mass);
        vsum = v1 + v2;

        if (vsum.Pt() > 0.2) continue;

        // Wybór najlepszego tracka
        int sel = best_track;
        TLorentzVector selected = (sel == 0) ? v1 : v2;

        // Topocluster matching dla wybranego tracka
        int best_topo = -1;
        float best_topo_pt = 0.0;
        int topo_count = 0;
        float pt_ratio_val = -1.0;
        float delta_high = 0.5;

        for (int j = 0; j < d.topoclus_n; j++)
        {
            float dr = sqrt(pow((*d.track_eta)[sel] - (*d.topo_cluster_eta)[j], 2) +
                            pow(delta_phi((*d.track_phi)[sel], (*d.topo_cluster_phi)[j]), 2));

            if (dr <= delta_high)
            {
                topo_count++;
                if ((*d.topo_cluster_pt)[j] > best_topo_pt)
                {
                    best_topo_pt = (*d.topo_cluster_pt)[j];
                    best_topo = j;
                }
            }
        }

        if (best_topo != -1)
            pt_ratio_val = best_topo_pt / (*d.track_pt)[sel];
        
        if (is_muon == 1) { if ((*d.topo_cluster_cell_significance)[best_topo]  < 7) continue;}

        // Zapis
        if (is_muon == 1 && vsum.M() >= 2.9 && vsum.M() <= 3.2)
        {
            num_muon++;

            h.muon_pt->Fill(vsum.Pt());
            h.muon_eta->Fill(vsum.Eta());
            h.muon_phi->Fill(vsum.Phi());
            h.muon_m->Fill(vsum.M());

            h.single_muon_eta->Fill(selected.Eta());
            h.single_muon_pt->Fill(selected.Pt());
            h.single_muon_phi->Fill(selected.Phi());
            h.single_muon_trt->Fill((*d.track_TRTHits)[sel]);
            h.single_muon_sct->Fill((*d.track_SCTHits)[sel]);
            h.single_muon_pixel->Fill((*d.track_PixelHits)[sel]);

            clear_new_data(dn_muon);

            dn_muon.track_TRTHits->push_back((*d.track_TRTHits)[sel]);
            dn_muon.track_SCTHits->push_back((*d.track_SCTHits)[sel]);
            dn_muon.track_PixelHits->push_back((*d.track_PixelHits)[sel]);
            dn_muon.track_PixeldEdX->push_back((*d.track_PixeldEdX)[sel]);

            if (best_topo == -1)
            {
                dn_muon.topo_cluster_eta->push_back(-10.0);
                dn_muon.topo_cluster_pt->push_back(-1.0);
                dn_muon.topo_cluster_phi->push_back(-10.0);
                dn_muon.topo_cluster_EM_prob->push_back(-1.0);
                dn_muon.topo_cluster_cell_significance->push_back(-1);
                dn_muon.topo_cluster_lambda->push_back(-1.0);
                dn_muon.topo_cluster_lambda2->push_back(-1.0);
                dn_muon.pt_ratio->push_back(-1.0);
                dn_muon.topocount = 0;
            }
            else
            {
                int j = best_topo;
                dn_muon.topo_cluster_eta->push_back((*d.topo_cluster_eta)[j]);
                dn_muon.topo_cluster_pt->push_back((*d.topo_cluster_pt)[j]);
                dn_muon.topo_cluster_phi->push_back((*d.topo_cluster_phi)[j]);
                dn_muon.topo_cluster_EM_prob->push_back((*d.topo_cluster_EM_prob)[j]);
                dn_muon.topo_cluster_cell_significance->push_back((*d.topo_cluster_cell_significance)[j]);
                dn_muon.topo_cluster_lambda->push_back((*d.topo_cluster_lambda)[j]);
                dn_muon.topo_cluster_lambda2->push_back((*d.topo_cluster_lambda2)[j]);
                dn_muon.pt_ratio->push_back(pt_ratio_val);
                dn_muon.topocount = topo_count;
            }

            tree_muon->Fill();
        }
        else if (is_elec == 1 && vsum.M() <= 3.0)
        {
            num_elec++;

            h.elec_pt->Fill(vsum.Pt());
            h.elec_eta->Fill(vsum.Eta());
            h.elec_phi->Fill(vsum.Phi());
            h.elec_m->Fill(vsum.M());

            h.single_elec_eta->Fill(selected.Eta());
            h.single_elec_pt->Fill(selected.Pt());
            h.single_elec_phi->Fill(selected.Phi());
            h.single_elec_trt->Fill((*d.track_TRTHits)[sel]);
            h.single_elec_sct->Fill((*d.track_SCTHits)[sel]);
            h.single_elec_pixel->Fill((*d.track_PixelHits)[sel]);

            clear_new_data(dn_elec);

            dn_elec.track_TRTHits->push_back((*d.track_TRTHits)[sel]);
            dn_elec.track_SCTHits->push_back((*d.track_SCTHits)[sel]);
            dn_elec.track_PixelHits->push_back((*d.track_PixelHits)[sel]);
            dn_elec.track_PixeldEdX->push_back((*d.track_PixeldEdX)[sel]);

            if (best_topo == -1)
            {
                dn_elec.topo_cluster_eta->push_back(-10.0);
                dn_elec.topo_cluster_pt->push_back(-1.0);
                dn_elec.topo_cluster_phi->push_back(-10.0);
                dn_elec.topo_cluster_EM_prob->push_back(-1.0);
                dn_elec.topo_cluster_cell_significance->push_back(-1);
                dn_elec.topo_cluster_lambda->push_back(-1.0);
                dn_elec.topo_cluster_lambda2->push_back(-1.0);
                dn_elec.pt_ratio->push_back(-1.0);
                dn_elec.topocount = 0;
            }
            else
            {
                int j = best_topo;
                dn_elec.topo_cluster_eta->push_back((*d.topo_cluster_eta)[j]);
                dn_elec.topo_cluster_pt->push_back((*d.topo_cluster_pt)[j]);
                dn_elec.topo_cluster_phi->push_back((*d.topo_cluster_phi)[j]);
                dn_elec.topo_cluster_EM_prob->push_back((*d.topo_cluster_EM_prob)[j]);
                dn_elec.topo_cluster_cell_significance->push_back((*d.topo_cluster_cell_significance)[j]);
                dn_elec.topo_cluster_lambda->push_back((*d.topo_cluster_lambda)[j]);
                dn_elec.topo_cluster_lambda2->push_back((*d.topo_cluster_lambda2)[j]);
                dn_elec.pt_ratio->push_back(pt_ratio_val);
                dn_elec.topocount = topo_count;
            }

            tree_elec->Fill();
        }
    }

    cout << endl << "Pairs Muons: " << num_muon << endl << "Pairs Electrons: " << num_elec << endl;
    plot_hists(h);

    tree_muon->Write();
    tree_elec->Write();
    output->Close();
    delete output;
}

void old_tree_read(TTree *old_tree, old_tree_data &d)
{
   old_tree->SetBranchAddress("track_pt", &d.track_pt);
   old_tree->SetBranchAddress("track_eta", &d.track_eta);
   old_tree->SetBranchAddress("track_charge", &d.track_charge);
   old_tree->SetBranchAddress("track_phi", &d.track_phi);
   old_tree->SetBranchAddress("track_n", &d.track_n);
   old_tree->SetBranchAddress("eg_cluster_n", &d.eg_cluster_n);
   old_tree->SetBranchAddress("eg_cluster_eta", &d.eg_cluster_eta);
   old_tree->SetBranchAddress("eg_cluster_phi", &d.eg_cluster_phi);
   old_tree->SetBranchAddress("track_TRTHits", &d.track_TRTHits);
   old_tree->SetBranchAddress("track_SCTHits", &d.track_SCTHits);
   old_tree->SetBranchAddress("track_PixelHits", &d.track_PixelHits);
   old_tree->SetBranchAddress("track_PixeldEdX", &d.track_PixeldEdX);

   old_tree->SetBranchAddress("topoclus_n", &d.topoclus_n);
   old_tree->SetBranchAddress("topo_cluster_eta", &d.topo_cluster_eta);
   old_tree->SetBranchAddress("topo_cluster_phi", &d.topo_cluster_phi);
   old_tree->SetBranchAddress("topo_cluster_pt", &d.topo_cluster_pt);
   old_tree->SetBranchAddress("topo_cluster_EM_prob", &d.topo_cluster_EM_prob);
   old_tree->SetBranchAddress("topo_cluster_cell_significance", &d.topo_cluster_cell_significance);
   old_tree->SetBranchAddress("topo_cluster_lambda", &d.topo_cluster_lambda);
   old_tree->SetBranchAddress("topo_cluster_lambda2", &d.topo_cluster_lambda2);
};

std::pair<TTree*, TTree*> create_new_trees(new_tree_data &d_muon, new_tree_data &d_elec)
{
   TTree *tree_muon = new TTree("MLDataMuon", "MLDataMuon");
   tree_muon->Branch("track_TRTHits", &d_muon.track_TRTHits);
   tree_muon->Branch("track_PixelHits", &d_muon.track_PixelHits);
   tree_muon->Branch("track_SCTHits", &d_muon.track_SCTHits);
   tree_muon->Branch("track_PixeldEdX", &d_muon.track_PixeldEdX);
   tree_muon->Branch("topo_cluster_eta", &d_muon.topo_cluster_eta);
   tree_muon->Branch("topo_cluster_phi", &d_muon.topo_cluster_phi);
   tree_muon->Branch("topo_cluster_EM_prob", &d_muon.topo_cluster_EM_prob);
   tree_muon->Branch("topo_cluster_pt", &d_muon.topo_cluster_pt);
   tree_muon->Branch("pt_ratio", &d_muon.pt_ratio);
   tree_muon->Branch("topo_cluster_lambda", &d_muon.topo_cluster_lambda);
   tree_muon->Branch("topo_cluster_lambda2", &d_muon.topo_cluster_lambda2);

   TTree *tree_elec = new TTree("MLDataElec", "MLDataElec");
   tree_elec->Branch("track_TRTHits", &d_elec.track_TRTHits);
   tree_elec->Branch("track_PixelHits", &d_elec.track_PixelHits);
   tree_elec->Branch("track_SCTHits", &d_elec.track_SCTHits);
   tree_elec->Branch("track_PixeldEdX", &d_elec.track_PixeldEdX);
   tree_elec->Branch("topo_cluster_eta", &d_elec.topo_cluster_eta);
   tree_elec->Branch("topo_cluster_phi", &d_elec.topo_cluster_phi);
   tree_elec->Branch("topo_cluster_EM_prob", &d_elec.topo_cluster_EM_prob);
   tree_elec->Branch("topo_cluster_pt", &d_elec.topo_cluster_pt);
   tree_elec->Branch("pt_ratio", &d_elec.pt_ratio);
   tree_elec->Branch("topo_cluster_lambda", &d_elec.topo_cluster_lambda);
   tree_elec->Branch("topo_cluster_lambda2", &d_elec.topo_cluster_lambda2);

   return std::make_pair(tree_muon, tree_elec);
};

void init_new_data(new_tree_data &dn)
{
    dn.track_TRTHits   = new vector<float>();
    dn.track_SCTHits   = new vector<float>();
    dn.track_PixelHits = new vector<float>();
    dn.track_PixeldEdX = new vector<int>();
    dn.topo_cluster_eta     = new vector<float>();
    dn.topo_cluster_phi     = new vector<float>();
    dn.topo_cluster_pt      = new vector<float>();
    dn.topo_cluster_EM_prob = new vector<float>();
    dn.topo_cluster_cell_significance = new vector<int>();
    dn.pt_ratio = new vector<float>();
    dn.topo_cluster_lambda = new vector<float>();
    dn.topo_cluster_lambda2 = new vector<float>(); 
}

void clear_new_data(new_tree_data &dn)
{
    if(dn.track_TRTHits)   dn.track_TRTHits->clear();
    if(dn.track_SCTHits)   dn.track_SCTHits->clear();
    if(dn.track_PixelHits) dn.track_PixelHits->clear();
    if(dn.track_PixeldEdX) dn.track_PixeldEdX->clear();

    if(dn.topo_cluster_eta)     dn.topo_cluster_eta->clear();
    if(dn.topo_cluster_phi)     dn.topo_cluster_phi->clear();
    if(dn.topo_cluster_pt)      dn.topo_cluster_pt->clear();
    if(dn.topo_cluster_EM_prob) dn.topo_cluster_EM_prob->clear();
    if(dn.topo_cluster_cell_significance) dn.topo_cluster_cell_significance->clear();
    if(dn.pt_ratio)             dn.pt_ratio->clear();
    if(dn.topo_cluster_lambda)  dn.topo_cluster_lambda->clear();
    if(dn.topo_cluster_lambda2) dn.topo_cluster_lambda2->clear();

    dn.topocount = 0;
}
void plot_hists(Hists &h)
{
    TCanvas *c1 = new TCanvas("c1", "Pair Distributions: Muons vs Electrons", 1200, 800);
    c1->Divide(2, 2);

    c1->cd(1);
    h.muon_pt->SetLineColor(kBlue); h.muon_pt->SetLineWidth(2);
    h.elec_pt->SetLineColor(kRed); h.elec_pt->SetLineWidth(2);
    double muon_pt_integral = h.muon_pt->Integral();
    double elec_pt_integral = h.elec_pt->Integral();
    if (muon_pt_integral > 0) h.muon_pt->Scale(1.0/muon_pt_integral);
    if (elec_pt_integral > 0) h.elec_pt->Scale(1.0/elec_pt_integral);
    h.muon_pt->SetTitle("p_{T} Distributions (Normalized)");
    double max_pt = TMath::Max(h.muon_pt->GetMaximum(), h.elec_pt->GetMaximum());
    h.muon_pt->SetMaximum(max_pt * 1.2);
    h.muon_pt->Draw("HIST");
    h.elec_pt->Draw("HIST SAME");
    TLegend *leg1 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg1->AddEntry(h.muon_pt, "Muons", "l");
    leg1->AddEntry(h.elec_pt, "Electrons", "l");
    leg1->Draw();

    c1->cd(2);
    h.muon_eta->SetLineColor(kBlue); h.muon_eta->SetLineWidth(2);
    h.elec_eta->SetLineColor(kRed); h.elec_eta->SetLineWidth(2);
    double muon_eta_integral = h.muon_eta->Integral();
    double elec_eta_integral = h.elec_eta->Integral();
    if (muon_eta_integral > 0) h.muon_eta->Scale(1.0/muon_eta_integral);
    if (elec_eta_integral > 0) h.elec_eta->Scale(1.0/elec_eta_integral);
    h.muon_eta->SetTitle("#eta Distributions (Normalized)");
    double max_eta = TMath::Max(h.muon_eta->GetMaximum(), h.elec_eta->GetMaximum());
    h.muon_eta->SetMaximum(max_eta * 1.2);
    h.muon_eta->Draw("HIST");
    h.elec_eta->Draw("HIST SAME");
    TLegend *leg2 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg2->AddEntry(h.muon_eta, "Muons", "l");
    leg2->AddEntry(h.elec_eta, "Electrons", "l");
    leg2->Draw();

    c1->cd(3);
    h.muon_phi->SetLineColor(kBlue); h.muon_phi->SetLineWidth(2);
    h.elec_phi->SetLineColor(kRed); h.elec_phi->SetLineWidth(2);
    double muon_phi_integral = h.muon_phi->Integral();
    double elec_phi_integral = h.elec_phi->Integral();
    if (muon_phi_integral > 0) h.muon_phi->Scale(1.0/muon_phi_integral);
    if (elec_phi_integral > 0) h.elec_phi->Scale(1.0/elec_phi_integral);
    h.muon_phi->SetTitle("#phi Distributions (Normalized)");
    double max_phi = TMath::Max(h.muon_phi->GetMaximum(), h.elec_phi->GetMaximum());
    h.muon_phi->SetMaximum(max_phi * 1.2);
    h.muon_phi->Draw("HIST");
    h.elec_phi->Draw("HIST SAME");
    TLegend *leg3 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg3->AddEntry(h.muon_phi, "Muons", "l");
    leg3->AddEntry(h.elec_phi, "Electrons", "l");
    leg3->Draw();

    c1->cd(4);
    h.muon_m->SetLineColor(kBlue); h.muon_m->SetLineWidth(2);
    h.elec_m->SetLineColor(kRed); h.elec_m->SetLineWidth(2);
    double muon_m_integral = h.muon_m->Integral();
    double elec_m_integral = h.elec_m->Integral();
    if (muon_m_integral > 0) h.muon_m->Scale(1.0/muon_m_integral);
    if (elec_m_integral > 0) h.elec_m->Scale(1.0/elec_m_integral);
    h.muon_m->SetTitle("Mass Distributions (Normalized)");
    double max_m = TMath::Max(h.muon_m->GetMaximum(), h.elec_m->GetMaximum());
    h.muon_m->SetMaximum(max_m * 1.2);
    h.muon_m->Draw("HIST");
    h.elec_m->Draw("HIST SAME");
    TLegend *leg4 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg4->AddEntry(h.muon_m, "Muons", "l");
    leg4->AddEntry(h.elec_m, "Electrons", "l");
    leg4->Draw();

    c1->SaveAs("Plots/pair_muon_electron.png");
    delete c1;

    TCanvas *c2 = new TCanvas("c2", "Single Track Distributions: Muons vs Electrons", 1200, 800);
    c2->Divide(2, 2);

    c2->cd(1);
    h.single_muon_pt->SetLineColor(kBlue); h.single_muon_pt->SetLineWidth(2);
    h.single_elec_pt->SetLineColor(kRed); h.single_elec_pt->SetLineWidth(2);
    double s_muon_pt_int = h.single_muon_pt->Integral();
    double s_elec_pt_int = h.single_elec_pt->Integral();
    if (s_muon_pt_int>0) h.single_muon_pt->Scale(1.0/s_muon_pt_int);
    if (s_elec_pt_int>0) h.single_elec_pt->Scale(1.0/s_elec_pt_int);
    h.single_muon_pt->SetTitle("Single Track p_{T} (Normalized)");
    double max_s_pt = TMath::Max(h.single_muon_pt->GetMaximum(), h.single_elec_pt->GetMaximum());
    h.single_muon_pt->SetMaximum(max_s_pt * 1.2);
    h.single_muon_pt->Draw("HIST");
    h.single_elec_pt->Draw("HIST SAME");
    TLegend *leg5 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg5->AddEntry(h.single_muon_pt, "Muons", "l");
    leg5->AddEntry(h.single_elec_pt, "Electrons", "l");
    leg5->Draw();

    c2->cd(2);
    h.single_muon_eta->SetLineColor(kBlue); h.single_muon_eta->SetLineWidth(2);
    h.single_elec_eta->SetLineColor(kRed); h.single_elec_eta->SetLineWidth(2);
    double s_muon_eta_int = h.single_muon_eta->Integral();
    double s_elec_eta_int = h.single_elec_eta->Integral();
    if (s_muon_eta_int>0) h.single_muon_eta->Scale(1.0/s_muon_eta_int);
    if (s_elec_eta_int>0) h.single_elec_eta->Scale(1.0/s_elec_eta_int);
    h.single_muon_eta->SetTitle("Single Track #eta (Normalized)");
    double max_s_eta = TMath::Max(h.single_muon_eta->GetMaximum(), h.single_elec_eta->GetMaximum());
    h.single_muon_eta->SetMaximum(max_s_eta * 1.2);
    h.single_muon_eta->Draw("HIST");
    h.single_elec_eta->Draw("HIST SAME");
    TLegend *leg6 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg6->AddEntry(h.single_muon_eta, "Muons", "l");
    leg6->AddEntry(h.single_elec_eta, "Electrons", "l");
    leg6->Draw();

    c2->cd(3);
    h.single_muon_phi->SetLineColor(kBlue); h.single_muon_phi->SetLineWidth(2);
    h.single_elec_phi->SetLineColor(kRed); h.single_elec_phi->SetLineWidth(2);
    double s_muon_phi_int = h.single_muon_phi->Integral();
    double s_elec_phi_int = h.single_elec_phi->Integral();
    if (s_muon_phi_int>0) h.single_muon_phi->Scale(1.0/s_muon_phi_int);
    if (s_elec_phi_int>0) h.single_elec_phi->Scale(1.0/s_elec_phi_int);
    h.single_muon_phi->SetTitle("Single Track #phi (Normalized)");
    double max_s_phi = TMath::Max(h.single_muon_phi->GetMaximum(), h.single_elec_phi->GetMaximum());
    h.single_muon_phi->SetMaximum(max_s_phi * 1.2);
    h.single_muon_phi->Draw("HIST");
    h.single_elec_phi->Draw("HIST SAME");
    TLegend *leg7 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg7->AddEntry(h.single_muon_phi, "Muons", "l");
    leg7->AddEntry(h.single_elec_phi, "Electrons", "l");
    leg7->Draw();

    c2->SaveAs("Plots/single_tracks_muon_electron.png");
    delete c2;

    TCanvas *c3 = new TCanvas("c3", "Detector Hits: Muons vs Electrons", 1200, 800);
    c3->Divide(2, 2);

    c3->cd(1);
    h.single_muon_trt->SetLineColor(kBlue); h.single_muon_trt->SetLineWidth(2);
    h.single_elec_trt->SetLineColor(kRed); h.single_elec_trt->SetLineWidth(2);
    double m_trt_int = h.single_muon_trt->Integral();
    double e_trt_int = h.single_elec_trt->Integral();
    if (m_trt_int>0) h.single_muon_trt->Scale(1.0/m_trt_int);
    if (e_trt_int>0) h.single_elec_trt->Scale(1.0/e_trt_int);
    h.single_muon_trt->SetTitle("TRT Hits (Normalized)");
    double max_trt = TMath::Max(h.single_muon_trt->GetMaximum(), h.single_elec_trt->GetMaximum());
    h.single_muon_trt->SetMaximum(max_trt*1.2);
    h.single_muon_trt->Draw("HIST");
    h.single_elec_trt->Draw("HIST SAME");
    TLegend *leg8 = new TLegend(0.6,0.7,0.85,0.85);
    leg8->AddEntry(h.single_muon_trt,"Muons","l");
    leg8->AddEntry(h.single_elec_trt,"Electrons","l");
    leg8->Draw();

    c3->cd(2);
    h.single_muon_sct->SetLineColor(kBlue); h.single_muon_sct->SetLineWidth(2);
    h.single_elec_sct->SetLineColor(kRed); h.single_elec_sct->SetLineWidth(2);
    double m_sct_int = h.single_muon_sct->Integral();
    double e_sct_int = h.single_elec_sct->Integral();
    if (m_sct_int>0) h.single_muon_sct->Scale(1.0/m_sct_int);
    if (e_sct_int>0) h.single_elec_sct->Scale(1.0/e_sct_int);
    h.single_muon_sct->SetTitle("SCT Hits (Normalized)");
    double max_sct = TMath::Max(h.single_muon_sct->GetMaximum(), h.single_elec_sct->GetMaximum());
    h.single_muon_sct->SetMaximum(max_sct*1.2);
    h.single_muon_sct->Draw("HIST");
    h.single_elec_sct->Draw("HIST SAME");
    TLegend *leg9 = new TLegend(0.6,0.7,0.85,0.85);
    leg9->AddEntry(h.single_muon_sct,"Muons","l");
    leg9->AddEntry(h.single_elec_sct,"Electrons","l");
    leg9->Draw();

    c3->cd(3);
    h.single_muon_pixel->SetLineColor(kBlue); h.single_muon_pixel->SetLineWidth(2);
    h.single_elec_pixel->SetLineColor(kRed); h.single_elec_pixel->SetLineWidth(2);
    double m_pixel_int = h.single_muon_pixel->Integral();
    double e_pixel_int = h.single_elec_pixel->Integral();
    if (m_pixel_int>0) h.single_muon_pixel->Scale(1.0/m_pixel_int);
    if (e_pixel_int>0) h.single_elec_pixel->Scale(1.0/e_pixel_int);
    h.single_muon_pixel->SetTitle("Pixel Hits (Normalized)");
    double max_pixel = TMath::Max(h.single_muon_pixel->GetMaximum(), h.single_elec_pixel->GetMaximum());
    h.single_muon_pixel->SetMaximum(max_pixel*1.2);
    h.single_muon_pixel->Draw("HIST");
    h.single_elec_pixel->Draw("HIST SAME");
    TLegend *leg10 = new TLegend(0.6,0.7,0.85,0.85);
    leg10->AddEntry(h.single_muon_pixel,"Muons","l");
    leg10->AddEntry(h.single_elec_pixel,"Electrons","l");
    leg10->Draw();

    c3->SaveAs("Plots/hits_muon_electron.png");
    delete c3;

    TCanvas *c4 = new TCanvas("c4", "Topocluster Distributions: Muons vs Electrons (Full)", 1200, 1000);
    c4->Divide(2, 3);

    c4->cd(1);
    h.single_muon_topo_cluster_pt->SetLineColor(kBlue); h.single_muon_topo_cluster_pt->SetLineWidth(2);
    h.single_elec_topo_cluster_pt->SetLineColor(kRed); h.single_elec_topo_cluster_pt->SetLineWidth(2);
    if(h.single_muon_topo_cluster_pt->Integral()>0) h.single_muon_topo_cluster_pt->Scale(1.0/h.single_muon_topo_cluster_pt->Integral());
    if(h.single_elec_topo_cluster_pt->Integral()>0) h.single_elec_topo_cluster_pt->Scale(1.0/h.single_elec_topo_cluster_pt->Integral());
    h.single_muon_topo_cluster_pt->SetTitle("Topocluster p_{T} (Normalized)");
    h.single_muon_topo_cluster_pt->SetMaximum(TMath::Max(h.single_muon_topo_cluster_pt->GetMaximum(), h.single_elec_topo_cluster_pt->GetMaximum())*1.2);
    h.single_muon_topo_cluster_pt->Draw("HIST");
    h.single_elec_topo_cluster_pt->Draw("HIST SAME");
    TLegend *leg_pt = new TLegend(0.6,0.7,0.85,0.85);
    leg_pt->AddEntry(h.single_muon_topo_cluster_pt,"Muons","l");
    leg_pt->AddEntry(h.single_elec_topo_cluster_pt,"Electrons","l");
    leg_pt->Draw();

    c4->cd(2);
    h.single_muon_topo_cluster_eta->SetLineColor(kBlue); h.single_muon_topo_cluster_eta->SetLineWidth(2);
    h.single_elec_topo_cluster_eta->SetLineColor(kRed); h.single_elec_topo_cluster_eta->SetLineWidth(2);
    if(h.single_muon_topo_cluster_eta->Integral()>0) h.single_muon_topo_cluster_eta->Scale(1.0/h.single_muon_topo_cluster_eta->Integral());
    if(h.single_elec_topo_cluster_eta->Integral()>0) h.single_elec_topo_cluster_eta->Scale(1.0/h.single_elec_topo_cluster_eta->Integral());
    h.single_muon_topo_cluster_eta->SetTitle("Topocluster #eta (Normalized)");
    h.single_muon_topo_cluster_eta->SetMaximum(TMath::Max(h.single_muon_topo_cluster_eta->GetMaximum(), h.single_elec_topo_cluster_eta->GetMaximum())*1.2);
    h.single_muon_topo_cluster_eta->Draw("HIST");
    h.single_elec_topo_cluster_eta->Draw("HIST SAME");
    TLegend *leg_eta = new TLegend(0.6,0.7,0.85,0.85);
    leg_eta->AddEntry(h.single_muon_topo_cluster_eta,"Muons","l");
    leg_eta->AddEntry(h.single_elec_topo_cluster_eta,"Electrons","l");
    leg_eta->Draw();

    c4->cd(3);
    h.single_muon_topo_cluster_phi->SetLineColor(kBlue); h.single_muon_topo_cluster_phi->SetLineWidth(2);
    h.single_elec_topo_cluster_phi->SetLineColor(kRed); h.single_elec_topo_cluster_phi->SetLineWidth(2);
    if(h.single_muon_topo_cluster_phi->Integral()>0) h.single_muon_topo_cluster_phi->Scale(1.0/h.single_muon_topo_cluster_phi->Integral());
    if(h.single_elec_topo_cluster_phi->Integral()>0) h.single_elec_topo_cluster_phi->Scale(1.0/h.single_elec_topo_cluster_phi->Integral());
    h.single_muon_topo_cluster_phi->SetTitle("Topocluster #phi (Normalized)");
    h.single_muon_topo_cluster_phi->SetMaximum(TMath::Max(h.single_muon_topo_cluster_phi->GetMaximum(), h.single_elec_topo_cluster_phi->GetMaximum())*1.2);
    h.single_muon_topo_cluster_phi->Draw("HIST");
    h.single_elec_topo_cluster_phi->Draw("HIST SAME");
    TLegend *leg_phi = new TLegend(0.6,0.7,0.85,0.85);
    leg_phi->AddEntry(h.single_muon_topo_cluster_phi,"Muons","l");
    leg_phi->AddEntry(h.single_elec_topo_cluster_phi,"Electrons","l");
    leg_phi->Draw();

    c4->cd(4);
    h.single_muon_topo_cluster_EM_prob->SetLineColor(kBlue); h.single_muon_topo_cluster_EM_prob->SetLineWidth(2);
    h.single_elec_topo_cluster_EM_prob->SetLineColor(kRed); h.single_elec_topo_cluster_EM_prob->SetLineWidth(2);
    if(h.single_muon_topo_cluster_EM_prob->Integral()>0) h.single_muon_topo_cluster_EM_prob->Scale(1.0/h.single_muon_topo_cluster_EM_prob->Integral());
    if(h.single_elec_topo_cluster_EM_prob->Integral()>0) h.single_elec_topo_cluster_EM_prob->Scale(1.0/h.single_elec_topo_cluster_EM_prob->Integral());
    h.single_muon_topo_cluster_EM_prob->SetTitle("Topocluster EM_prob (Normalized)");
    h.single_muon_topo_cluster_EM_prob->SetMaximum(TMath::Max(h.single_muon_topo_cluster_EM_prob->GetMaximum(), h.single_elec_topo_cluster_EM_prob->GetMaximum())*1.2);
    h.single_muon_topo_cluster_EM_prob->Draw("HIST");
    h.single_elec_topo_cluster_EM_prob->Draw("HIST SAME");
    TLegend *leg_em = new TLegend(0.6,0.7,0.85,0.85);
    leg_em->AddEntry(h.single_muon_topo_cluster_EM_prob,"Muons","l");
    leg_em->AddEntry(h.single_elec_topo_cluster_EM_prob,"Electrons","l");
    leg_em->Draw();

    c4->cd(5);
    h.muon_topocount->SetLineColor(kBlue); h.muon_topocount->SetLineWidth(2);
    h.elec_topocount->SetLineColor(kRed); h.elec_topocount->SetLineWidth(2);
    if(h.muon_topocount->Integral()>0) h.muon_topocount->Scale(1.0/h.muon_topocount->Integral());
    if(h.elec_topocount->Integral()>0) h.elec_topocount->Scale(1.0/h.elec_topocount->Integral());
    h.muon_topocount->SetTitle("Topocluster Count (Normalized)");
    h.muon_topocount->SetMaximum(TMath::Max(h.muon_topocount->GetMaximum(), h.elec_topocount->GetMaximum())*1.2);
    h.muon_topocount->Draw("HIST");
    h.elec_topocount->Draw("HIST SAME");
    TLegend *leg_count = new TLegend(0.6,0.7,0.85,0.85);
    leg_count->AddEntry(h.muon_topocount,"Muons","l");
    leg_count->AddEntry(h.elec_topocount,"Electrons","l");
    leg_count->Draw();

    c4->cd(6);
    h.single_muon_topo_cluster_pt_ratio->SetLineColor(kBlue); h.single_muon_topo_cluster_pt_ratio->SetLineWidth(2);
    h.single_elec_topo_cluster_pt_ratio->SetLineColor(kRed); h.single_elec_topo_cluster_pt_ratio->SetLineWidth(2);
    if(h.single_muon_topo_cluster_pt_ratio->Integral()>0) h.single_muon_topo_cluster_pt_ratio->Scale(1.0/h.single_muon_topo_cluster_pt_ratio->Integral());
    if(h.single_elec_topo_cluster_pt_ratio->Integral()>0) h.single_elec_topo_cluster_pt_ratio->Scale(1.0/h.single_elec_topo_cluster_pt_ratio->Integral());
    h.single_muon_topo_cluster_pt_ratio->SetTitle("Topocluster pT ratio (Normalized)");
    h.single_muon_topo_cluster_pt_ratio->SetMaximum(TMath::Max(h.single_muon_topo_cluster_pt_ratio->GetMaximum(), h.single_elec_topo_cluster_pt_ratio->GetMaximum())*1.2);
    h.single_muon_topo_cluster_pt_ratio->Draw("HIST");
    h.single_elec_topo_cluster_pt_ratio->Draw("HIST SAME");
    TLegend *leg_pt_ratio = new TLegend(0.6,0.7,0.85,0.85);
    leg_pt_ratio->AddEntry(h.single_muon_topo_cluster_pt_ratio,"Muons","l");
    leg_pt_ratio->AddEntry(h.single_elec_topo_cluster_pt_ratio,"Electrons","l");
    leg_pt_ratio->Draw();

    c4->SaveAs("Plots/topocluster_muon_electron_full.png");
    delete c4;

    TCanvas *c5 = new TCanvas("c5", "Topocluster Distributions: Muons vs Electrons (Clean)", 1200, 1000);
    c5->Divide(2, 3);

    c5->cd(1);
    h.single_muon_topo_cluster_pt_clean->SetLineColor(kBlue); h.single_muon_topo_cluster_pt_clean->SetLineWidth(2);
    h.single_elec_topo_cluster_pt_clean->SetLineColor(kRed); h.single_elec_topo_cluster_pt_clean->SetLineWidth(2);
    if(h.single_muon_topo_cluster_pt_clean->Integral()>0) h.single_muon_topo_cluster_pt_clean->Scale(1.0/h.single_muon_topo_cluster_pt_clean->Integral());
    if(h.single_elec_topo_cluster_pt_clean->Integral()>0) h.single_elec_topo_cluster_pt_clean->Scale(1.0/h.single_elec_topo_cluster_pt_clean->Integral());
    h.single_muon_topo_cluster_pt_clean->SetTitle("Topocluster p_{T} (Clean, Normalized)");
    h.single_muon_topo_cluster_pt_clean->SetMaximum(TMath::Max(h.single_muon_topo_cluster_pt_clean->GetMaximum(), h.single_elec_topo_cluster_pt_clean->GetMaximum())*1.2);
    h.single_muon_topo_cluster_pt_clean->Draw("HIST");
    h.single_elec_topo_cluster_pt_clean->Draw("HIST SAME");
    TLegend *leg_pt_c = new TLegend(0.6,0.7,0.85,0.85);
    leg_pt_c->AddEntry(h.single_muon_topo_cluster_pt_clean,"Muons","l");
    leg_pt_c->AddEntry(h.single_elec_topo_cluster_pt_clean,"Electrons","l");
    leg_pt_c->Draw();

    c5->cd(2);
    h.single_muon_topo_cluster_eta_clean->SetLineColor(kBlue); h.single_muon_topo_cluster_eta_clean->SetLineWidth(2);
    h.single_elec_topo_cluster_eta_clean->SetLineColor(kRed); h.single_elec_topo_cluster_eta_clean->SetLineWidth(2);
    if(h.single_muon_topo_cluster_eta_clean->Integral()>0) h.single_muon_topo_cluster_eta_clean->Scale(1.0/h.single_muon_topo_cluster_eta_clean->Integral());
    if(h.single_elec_topo_cluster_eta_clean->Integral()>0) h.single_elec_topo_cluster_eta_clean->Scale(1.0/h.single_elec_topo_cluster_eta_clean->Integral());
    h.single_muon_topo_cluster_eta_clean->SetTitle("Topocluster #eta (Clean, Normalized)");
    h.single_muon_topo_cluster_eta_clean->SetMaximum(TMath::Max(h.single_muon_topo_cluster_eta_clean->GetMaximum(), h.single_elec_topo_cluster_eta_clean->GetMaximum())*1.2);
    h.single_muon_topo_cluster_eta_clean->Draw("HIST");
    h.single_elec_topo_cluster_eta_clean->Draw("HIST SAME");
    TLegend *leg_eta_c = new TLegend(0.6,0.7,0.85,0.85);
    leg_eta_c->AddEntry(h.single_muon_topo_cluster_eta_clean,"Muons","l");
    leg_eta_c->AddEntry(h.single_elec_topo_cluster_eta_clean,"Electrons","l");
    leg_eta_c->Draw();

    c5->cd(3);
    h.single_muon_topo_cluster_phi_clean->SetLineColor(kBlue); h.single_muon_topo_cluster_phi_clean->SetLineWidth(2);
    h.single_elec_topo_cluster_phi_clean->SetLineColor(kRed); h.single_elec_topo_cluster_phi_clean->SetLineWidth(2);
    if(h.single_muon_topo_cluster_phi_clean->Integral()>0) h.single_muon_topo_cluster_phi_clean->Scale(1.0/h.single_muon_topo_cluster_phi_clean->Integral());
    if(h.single_elec_topo_cluster_phi_clean->Integral()>0) h.single_elec_topo_cluster_phi_clean->Scale(1.0/h.single_elec_topo_cluster_phi_clean->Integral());
    h.single_muon_topo_cluster_phi_clean->SetTitle("Topocluster #phi (Clean, Normalized)");
    h.single_muon_topo_cluster_phi_clean->SetMaximum(TMath::Max(h.single_muon_topo_cluster_phi_clean->GetMaximum(), h.single_elec_topo_cluster_phi_clean->GetMaximum())*1.2);
    h.single_muon_topo_cluster_phi_clean->Draw("HIST");
    h.single_elec_topo_cluster_phi_clean->Draw("HIST SAME");
    TLegend *leg_phi_c = new TLegend(0.6,0.7,0.85,0.85);
    leg_phi_c->AddEntry(h.single_muon_topo_cluster_phi_clean,"Muons","l");
    leg_phi_c->AddEntry(h.single_elec_topo_cluster_phi_clean,"Electrons","l");
    leg_phi_c->Draw();

    c5->cd(4);
    h.single_muon_topo_cluster_EM_prob_clean->SetLineColor(kBlue); h.single_muon_topo_cluster_EM_prob_clean->SetLineWidth(2);
    h.single_elec_topo_cluster_EM_prob_clean->SetLineColor(kRed); h.single_elec_topo_cluster_EM_prob_clean->SetLineWidth(2);
    if(h.single_muon_topo_cluster_EM_prob_clean->Integral()>0) h.single_muon_topo_cluster_EM_prob_clean->Scale(1.0/h.single_muon_topo_cluster_EM_prob_clean->Integral());
    if(h.single_elec_topo_cluster_EM_prob_clean->Integral()>0) h.single_elec_topo_cluster_EM_prob_clean->Scale(1.0/h.single_elec_topo_cluster_EM_prob_clean->Integral());
    h.single_muon_topo_cluster_EM_prob_clean->SetTitle("Topocluster EM_prob (Clean, Normalized)");
    h.single_muon_topo_cluster_EM_prob_clean->SetMaximum(TMath::Max(h.single_muon_topo_cluster_EM_prob_clean->GetMaximum(), h.single_elec_topo_cluster_EM_prob_clean->GetMaximum())*1.2);
    h.single_muon_topo_cluster_EM_prob_clean->Draw("HIST");
    h.single_elec_topo_cluster_EM_prob_clean->Draw("HIST SAME");
    TLegend *leg_em_c = new TLegend(0.6,0.7,0.85,0.85);
    leg_em_c->AddEntry(h.single_muon_topo_cluster_EM_prob_clean,"Muons","l");
    leg_em_c->AddEntry(h.single_elec_topo_cluster_EM_prob_clean,"Electrons","l");
    leg_em_c->Draw();

    c5->cd(5);
    h.single_muon_topo_cluster_pt_ratio_clean->SetLineColor(kBlue); h.single_muon_topo_cluster_pt_ratio_clean->SetLineWidth(2);
    h.single_elec_topo_cluster_pt_ratio_clean->SetLineColor(kRed); h.single_elec_topo_cluster_pt_ratio_clean->SetLineWidth(2);
    if(h.single_muon_topo_cluster_pt_ratio_clean->Integral()>0) h.single_muon_topo_cluster_pt_ratio_clean->Scale(1.0/h.single_muon_topo_cluster_pt_ratio_clean->Integral());
    if(h.single_elec_topo_cluster_pt_ratio_clean->Integral()>0) h.single_elec_topo_cluster_pt_ratio_clean->Scale(1.0/h.single_elec_topo_cluster_pt_ratio_clean->Integral());
    h.single_muon_topo_cluster_pt_ratio_clean->SetTitle("Topocluster pT ratio (Clean, Normalized)");
    h.single_muon_topo_cluster_pt_ratio_clean->SetMaximum(TMath::Max(h.single_muon_topo_cluster_pt_ratio_clean->GetMaximum(), h.single_elec_topo_cluster_pt_ratio_clean->GetMaximum())*1.2);
    h.single_muon_topo_cluster_pt_ratio_clean->Draw("HIST");
    h.single_elec_topo_cluster_pt_ratio_clean->Draw("HIST SAME");
    TLegend *leg_pt_ratio_c = new TLegend(0.6,0.7,0.85,0.85);
    leg_pt_ratio_c->AddEntry(h.single_muon_topo_cluster_pt_ratio_clean,"Muons","l");
    leg_pt_ratio_c->AddEntry(h.single_elec_topo_cluster_pt_ratio_clean,"Electrons","l");
    leg_pt_ratio_c->Draw();

    c5->SaveAs("Plots/topocluster_muon_electron_clean.png");
    delete c5;

    TCanvas *c6 = new TCanvas("c6", "Topocluster Lambda: Muons vs Electrons", 1200, 800);
    c6->Divide(2, 2);

    c6->cd(1);
    h.single_muon_topo_cluster_lambda->SetLineColor(kBlue); 
    h.single_muon_topo_cluster_lambda->SetLineWidth(2);
    h.single_elec_topo_cluster_lambda->SetLineColor(kRed); 
    h.single_elec_topo_cluster_lambda->SetLineWidth(2);

    if(h.single_muon_topo_cluster_lambda->Integral() > 0) 
        h.single_muon_topo_cluster_lambda->Scale(1.0/h.single_muon_topo_cluster_lambda->Integral());
    if(h.single_elec_topo_cluster_lambda->Integral() > 0) 
        h.single_elec_topo_cluster_lambda->Scale(1.0/h.single_elec_topo_cluster_lambda->Integral());

    h.single_muon_topo_cluster_lambda->SetTitle("Topocluster #lambda (Normalized)");
    h.single_muon_topo_cluster_lambda->GetXaxis()->SetTitle("#lambda [mm]");
    h.single_muon_topo_cluster_lambda->GetYaxis()->SetTitle("Normalized");

    double max_lambda = TMath::Max(h.single_muon_topo_cluster_lambda->GetMaximum(), 
                                h.single_elec_topo_cluster_lambda->GetMaximum());
    h.single_muon_topo_cluster_lambda->SetMaximum(max_lambda * 1.2);

    h.single_muon_topo_cluster_lambda->Draw("HIST");
    h.single_elec_topo_cluster_lambda->Draw("HIST SAME");

    TLegend *leg_lambda = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg_lambda->AddEntry(h.single_muon_topo_cluster_lambda, "Muons", "l");
    leg_lambda->AddEntry(h.single_elec_topo_cluster_lambda, "Electrons", "l");
    leg_lambda->Draw();

    c6->cd(2);
    h.single_muon_topo_cluster_lambda2->SetLineColor(kBlue); 
    h.single_muon_topo_cluster_lambda2->SetLineWidth(2);
    h.single_elec_topo_cluster_lambda2->SetLineColor(kRed); 
    h.single_elec_topo_cluster_lambda2->SetLineWidth(2);

    if(h.single_muon_topo_cluster_lambda2->Integral() > 0) 
        h.single_muon_topo_cluster_lambda2->Scale(1.0/h.single_muon_topo_cluster_lambda2->Integral());
    if(h.single_elec_topo_cluster_lambda2->Integral() > 0) 
        h.single_elec_topo_cluster_lambda2->Scale(1.0/h.single_elec_topo_cluster_lambda2->Integral());

    h.single_muon_topo_cluster_lambda2->SetTitle("Topocluster #lambda_{2} (Normalized)");
    h.single_muon_topo_cluster_lambda2->GetXaxis()->SetTitle("#lambda_{2} [mm^{2}]");
    h.single_muon_topo_cluster_lambda2->GetYaxis()->SetTitle("Normalized");

    double max_lambda2 = TMath::Max(h.single_muon_topo_cluster_lambda2->GetMaximum(), 
                                    h.single_elec_topo_cluster_lambda2->GetMaximum());
    h.single_muon_topo_cluster_lambda2->SetMaximum(max_lambda2 * 1.2);

    h.single_muon_topo_cluster_lambda2->Draw("HIST");
    h.single_elec_topo_cluster_lambda2->Draw("HIST SAME");

    TLegend *leg_lambda2 = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg_lambda2->AddEntry(h.single_muon_topo_cluster_lambda2, "Muons", "l");
    leg_lambda2->AddEntry(h.single_elec_topo_cluster_lambda2, "Electrons", "l");
    leg_lambda2->Draw();

    c6->SaveAs("Plots/topocluster_lambda.png");
    delete c6;
}

void init_hists(Hists &h) {
    h.muon_pt   = new TH1F("muon_pt",   "Muon p_{T}", 100, 0, 0.25);
    h.muon_eta  = new TH1F("muon_eta",  "Muon #eta", 100, -2.55, 2.55);
    h.muon_phi  = new TH1F("muon_phi",  "Muon #phi", 100, -3.15, 3.15);
    h.muon_m    = new TH1F("muon_m",    "Muon Mass", 100, 2.9, 3.3);

    h.elec_pt   = new TH1F("elec_pt",   "Electron p_{T}", 100, 0, 0.25);
    h.elec_eta  = new TH1F("elec_eta",  "Electron #eta", 100, -2.55, 2.55);
    h.elec_phi  = new TH1F("elec_phi",  "Electron #phi", 100, -3.15, 3.15);
    h.elec_m    = new TH1F("elec_m",    "Electron Mass", 100, 2.9, 3.3);

    h.single_elec_pt   = new TH1F("single_elec_pt",  "Single Electron p_{T}", 100, 0.0, 2.0);
    h.single_elec_eta  = new TH1F("single_elec_eta", "Single Electron #eta", 100, -2.55, 2.55);
    h.single_elec_phi  = new TH1F("single_elec_phi", "Single Electron #phi", 100, -3.15, 3.15);

    h.single_muon_pt   = new TH1F("single_muon_pt",  "Single Muon p_{T}", 100, 0.0, 2);
    h.single_muon_eta  = new TH1F("single_muon_eta", "Single Muon #eta", 100, -2.55, 2.55);
    h.single_muon_phi  = new TH1F("single_muon_phi", "Single Muon #phi", 100, -3.15, 3.15);

    h.single_elec_trt    = new TH1F("single_elec_trt",   "Single Electron TRT hits", 50, 0, 60);
    h.single_elec_sct    = new TH1F("single_elec_sct",   "Single Electron SCT hits", 50, 0, 16);
    h.single_elec_pixel  = new TH1F("single_elec_pixel", "Single Electron Pixel hits", 50, 0, 10);

    h.single_muon_trt    = new TH1F("single_muon_trt",   "Single Muon TRT hits", 50, 0, 60);
    h.single_muon_sct    = new TH1F("single_muon_sct",   "Single Muon SCT hits", 50, 0, 16);
    h.single_muon_pixel  = new TH1F("single_muon_pixel", "Single Muon Pixel hits", 50, 0, 10);
    
    h.single_elec_topo_cluster_pt       = new TH1F("single_elec_topo_pt",       "Single Electron Topo p_{T}", 100, -2, 5);  
    h.single_elec_topo_cluster_eta      = new TH1F("single_elec_topo_eta",      "Single Electron Topo #eta", 100, -11, 2.55);
    h.single_elec_topo_cluster_phi      = new TH1F("single_elec_topo_phi",      "Single Electron Topo #phi", 100, -11, 3.15);
    h.single_elec_topo_cluster_EM_prob = new TH1F("single_elec_topo_EM_prob", "Single Electron Topo EM prob", 100, -2, 1);  
    h.single_elec_topo_cluster_cell_significance = new TH1F("single_elec_topo_pass", "Single Electron Topo pass sig cut", 15, -2, 9.5);
    h.elec_topocount  = new TH1F("elec_topocount",     "Elec_topocount", 10, -1, 10);
    h.single_elec_topo_cluster_pt_ratio = new TH1F("single_elec_topo_cluster_pt_ratio","Electron Topocluster pT ratio",50,0,2);
    h.single_elec_topo_cluster_lambda = new TH1F("single_elec_topo_lambda", "Single Electron Topo #lambda", 100, -100, 2000);
    h.single_elec_topo_cluster_lambda2 = new TH1F("single_elec_topo_lambda2", "Single Electron Topo #lambda2", 100, -100, 2000);

    h.single_muon_topo_cluster_pt       = new TH1F("single_muon_topo_pt",       "Single Muon Topo p_{T}", 100, -2, 5);
    h.single_muon_topo_cluster_eta      = new TH1F("single_muon_topo_eta",      "Single Muon Topo #eta", 100, -11, 2.55);
    h.single_muon_topo_cluster_phi      = new TH1F("single_muon_topo_phi",      "Single Muon Topo #phi", 100, -11, 3.15);
    h.single_muon_topo_cluster_EM_prob = new TH1F("single_muon_topo_EM_prob", "Single Muon Topo EM prob", 100, -2, 1);
    h.single_muon_topo_cluster_cell_significance = new TH1F("single_muon_topo_pass", "Single Muon Topo pass sig cut", 15, -2, 9.5);
    h.muon_topocount  = new TH1F("muon_topocount",     "muon_topocount", 10, -1, 10);
    h.single_muon_topo_cluster_pt_ratio = new TH1F("single_muon_topo_cluster_pt_ratio","Muon Topocluster pT ratio",50,0,2);
    h.single_muon_topo_cluster_lambda = new TH1F("single_muon_topo_lambda", "Single Muon Topo #lambda", 100, -100, 2000);
    h.single_muon_topo_cluster_lambda2 = new TH1F("single_muon_topo_lambda2", "Single Muon Topo #lambda2", 100, -100, 2000);

    h.single_muon_topo_cluster_pt_clean = new TH1F("single_muon_topo_cluster_pt_clean", 
        "Single Muon Topo p_{T} (Clean)", 100, 0.01, 5);
    h.single_muon_topo_cluster_eta_clean = new TH1F("single_muon_topo_cluster_eta_clean",
        "Single Muon Topo #eta (Clean)", 100, -2.5, 2.5);
    h.single_muon_topo_cluster_phi_clean = new TH1F("single_muon_topo_cluster_phi_clean",
         "Single Muon Topo #phi (Clean)", 100, -3.15, 3.15);
    h.single_muon_topo_cluster_EM_prob_clean = new TH1F("single_muon_topo_cluster_EM_prob_clean",
        "Single Muon Topo EM prob (Clean)", 100, 0, 1);
    h.single_muon_topo_cluster_cell_significance_clean = new TH1F("single_muon_topo_cluster_cell_significance_clean",
         "Single Muon Topo pass sig cut (Clean)", 10, 0, 2);
    h.single_muon_topo_cluster_pt_ratio_clean = new TH1F("single_muon_topo_cluster_pt_ratio_clean",
        "Muon Topocluster pT ratio (Clean)", 50, 0, 2);

    h.single_elec_topo_cluster_pt_clean = new TH1F("single_elec_topo_cluster_pt_clean",
        "Single Electron Topo p_{T} (Clean)", 100, 0.01, 5);
    h.single_elec_topo_cluster_eta_clean = new TH1F("single_elec_topo_cluster_eta_clean",
        "Single Electron Topo #eta (Clean)", 100, -2.5, 2.5);
    h.single_elec_topo_cluster_phi_clean = new TH1F("single_elec_topo_cluster_phi_clean",
         "Single Electron Topo #phi (Clean)", 100, -3.15, 3.15);
    h.single_elec_topo_cluster_EM_prob_clean = new TH1F("single_elec_topo_cluster_EM_prob_clean",
        "Single Electron Topo EM prob (Clean)", 100, 0, 1);
    h.single_elec_topo_cluster_cell_significance_clean = new TH1F("single_elec_topo_cluster_cell_significance_clean",
         "Single Electron Topo pass sig cut (Clean)", 10, 0, 2);
    h.single_elec_topo_cluster_pt_ratio_clean = new TH1F("single_elec_topo_cluster_pt_ratio_clean",
        "Electron Topocluster pT ratio (Clean)", 50, 0, 2);
}

float delta_phi(float phi1, float phi2){
    float diff = std::abs(phi1 - phi2);
    return std::min(diff, (float)(2*M_PI - diff));
}