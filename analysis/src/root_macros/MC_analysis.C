/*
    Makro do analizy MC. W przypadku dodania do TCHain plikow MC_bckg i MC_sig tworzy wspolny rozkald do porownania
    z danymi (makro Sig_bckg_stack.C). Dodatkowo na podstawie podanych plikow MC tworzy plik ze zmiennymi do
    trenowania modeli klasyfikatora (mlDataTree).
*/

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <vector>
#include <iostream>
#include <cmath>

// =========================
// PROGRESS BAR
// =========================
void DisplayUpdate(int entry, int nentries)
{
    if (entry % 10000 == 0 || entry == nentries - 1)
        std::cout << "Progress: " << entry << "/" << nentries << "\r" << std::flush;
}

// =========================
// VARIABLES
// =========================
struct Variables {
    UInt_t track_n;
    UInt_t topoclus_n;

    std::vector<float>* track_pt = nullptr;
    std::vector<float>* track_eta = nullptr;
    std::vector<float>* track_phi = nullptr;
    std::vector<int>*   track_charge = nullptr;

    std::vector<float>* topo_cluster_pt = nullptr;
    std::vector<float>* topo_cluster_eta = nullptr;
    std::vector<float>* topo_cluster_phi = nullptr;
    std::vector<bool>*  topo_cluster_pass_sig_cut = nullptr;

    std::vector<float>* topo_cluster_lambda = nullptr;
    std::vector<float>* topo_cluster_lambda2 = nullptr;
    std::vector<float>* topo_cluster_radius = nullptr;
    std::vector<float>* topo_cluster_time = nullptr;

    //std::vector<float>* track_PixelHits = nullptr;
    //std::vector<float>* track_SCTHits = nullptr;
    //std::vector<float>* track_PixeldEdX = nullptr;
    //std::vector<float>* track_TRTHits = nullptr;
};

// =========================
// BRANCH SETUP
// =========================
void SetBranchAddresses(TTree* tree, Variables& v)
{
    tree->SetBranchAddress("track_n", &v.track_n);
    tree->SetBranchAddress("topoclus_n", &v.topoclus_n);

    tree->SetBranchAddress("track_pt", &v.track_pt);
    tree->SetBranchAddress("track_eta", &v.track_eta);
    tree->SetBranchAddress("track_phi", &v.track_phi);
    tree->SetBranchAddress("track_charge", &v.track_charge);

    tree->SetBranchAddress("topo_cluster_pt", &v.topo_cluster_pt);
    tree->SetBranchAddress("topo_cluster_eta", &v.topo_cluster_eta);
    tree->SetBranchAddress("topo_cluster_phi", &v.topo_cluster_phi);
    tree->SetBranchAddress("topo_cluster_pass_sig_cut", &v.topo_cluster_pass_sig_cut);

    tree->SetBranchAddress("topo_cluster_lambda", &v.topo_cluster_lambda);
    tree->SetBranchAddress("topo_cluster_lambda2", &v.topo_cluster_lambda2);
    tree->SetBranchAddress("topo_cluster_r2", &v.topo_cluster_radius);
    tree->SetBranchAddress("topo_cluster_time", &v.topo_cluster_time);

    //tree->SetBranchAddress("track_PixelHits", &v.track_PixelHits);
    //tree->SetBranchAddress("track_SCTHits", &v.track_SCTHits);
    //tree->SetBranchAddress("track_PixeldEdX", &v.track_PixeldEdX);
    //tree->SetBranchAddress("track_TRTHits", &v.track_TRTHits);
}

// =========================
// ANALYSIS
// =========================
void AnalyzeSample(
    TTree* tree,
    TH1D* hist_dilepton_mass,
    TH1D* hist_dilepton_pt,
    TH1D* hist_lepton_topo_count,
    TTree* mlTree,
    int &particle_tag,
    int &isSignal,
    float &FVariable,
    float &lambda,
    float &lambda2,
    float &radius,
    float &time)
{
    Variables v;
    SetBranchAddresses(tree, v);

    const double MUON_MASS = 0.105658;
    int n_events = tree->GetEntries();

    for (int entry = 0; entry < n_events; entry++)
    {
        tree->GetEntry(entry);
        DisplayUpdate(entry, n_events);

        TFile* f = tree->GetCurrentFile();
        std::string fname = f->GetName();

        if (fname.find("_sig_") != std::string::npos){ //<- plik z JPsi->ll musi miec w nazwie "_sig_"
            isSignal = 1;
        }
        else{
            isSignal = 0;
        }

        if (v.track_n != 2) continue;

        std::vector<TLorentzVector> leptons;
        std::vector<int> good_indices;
        int tag = 2;

        // =========================
        // LEPTON SELECTION
        // =========================
        for (int i = 0; i < v.track_n; i++)
        {
            if (v.track_pt->at(i) <= 0.1) continue;
            if (fabs(v.track_eta->at(i)) >= 2.5) continue;

            TLorentzVector p;
            p.SetPtEtaPhiM(
                v.track_pt->at(i),
                v.track_eta->at(i),
                v.track_phi->at(i),
                MUON_MASS
            );

            leptons.push_back(p);
            good_indices.push_back(i);
        }

        // dokładnie 2 dobre leptony
        if (leptons.size() != 2) continue;

        int i1 = good_indices[0];
        int i2 = good_indices[1];

        // =========================
        // CHARGE CUT 
        // =========================
        if (v.track_charge->at(i1) * v.track_charge->at(i2) >= 0)
            continue;

        // =========================
        // DILEPTON
        // =========================
        TLorentzVector dilepton = leptons[0] + leptons[1];

        double mass = dilepton.M();
        double pt   = dilepton.Pt();

        // reference cut (J/psi region logic)
        if (pt >= 0.2) continue;
        if (mass < 2.9 || mass > 3.2) continue; 

        hist_dilepton_mass->Fill(mass);
        hist_dilepton_pt->Fill(pt);

        // =========================
        // TOPO MATCHING
        // =========================
        int topo_count = 0;
        int best_id = -1;
        double max_pt = -1;

        for (int i_topo = 0; i_topo < v.topoclus_n; i_topo++)
        {
            double dphi = TVector2::Phi_mpi_pi(
                leptons[0].Phi() - v.topo_cluster_phi->at(i_topo)
            );

            double dR = sqrt(
                pow(leptons[0].Eta() - v.topo_cluster_eta->at(i_topo), 2)
                + dphi * dphi
            );

            if (dR < 0.5 && v.topo_cluster_pass_sig_cut->at(i_topo))
            {
                topo_count++;

                double pt_topo = v.topo_cluster_pt->at(i_topo);
                if (pt_topo > max_pt)
                {
                    max_pt = pt_topo;
                    best_id = i_topo;
                }
            }
        }

        hist_lepton_topo_count->Fill(topo_count);

        // =========================
        // ML FILL
        // =========================
        tag = particle_tag;

        if (best_id != -1)
        {
            FVariable = v.topo_cluster_pt->at(best_id) / leptons[0].Pt();
            lambda    = v.topo_cluster_lambda->at(best_id);
            lambda2   = v.topo_cluster_lambda2->at(best_id);
            radius    = v.topo_cluster_radius->at(best_id);
            time      = v.topo_cluster_time->at(best_id);

            //track_PixelHits = v.track_PixelHits->at(i1);
            //track_SCTHits   = v.track_SCTHits->at(i1);
            //track_PixeldEdX = v.track_PixeldEdX->at(i1);
            //track_TRTHits   = v.track_TRTHits->at(i1);

            mlTree->Fill();
        }
    }

    std::cout << "\n[INFO] Finished sample (tag = " << particle_tag << ")\n";
}
// =========================
// ML TREE SET UP
// =========================
void TreeSetUpML(TTree*& mlTree,
                  float& FVariable,
                  float& lambda,
                  float& lambda2,
                  float& radius,
                  float& time,
                  int& particle_tag,
                  int& isSignal)
{
     mlTree = new TTree("mlDataTree","ML");

    mlTree->Branch("FVariable", &FVariable, "FVariable/F");
    mlTree->Branch("lambda", &lambda, "lambda/F");
    mlTree->Branch("lambda2", &lambda2, "lambda2/F");
    mlTree->Branch("radius", &radius, "radius/F");
    mlTree->Branch("time", &time, "time/F");

    //mlTree->Branch("track_PixelHits", &track_PixelHits, "track_PixelHits/F");
    //mlTree->Branch("track_SCTHits", &track_SCTHits, "track_SCTHits/F");
    //mlTree->Branch("track_PixeldEdX", &track_PixeldEdX, "track_PixeldEdX/F");
    //mlTree->Branch("track_TRTHits", &track_TRTHits, "track_TRTHits/F");
    mlTree->Branch("particle_tag", &particle_tag, "particle_tag/I");
    mlTree->Branch("isSignal", &isSignal);


}

// =========================
// MAIN
// =========================
void MC_analysis()
{
    // =========================
    // FILES AND TREES (TCHAIN)
    // =========================
    TString file_ee_sig   = "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/mc_jpsi_ee_sig_sampling.root";
    TString file_ee_bckg  = "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/yyee_starlight_sig_sampling.root";

    TString file_mumu_sig  = "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/mc_jpsi_mumu_sig_sampling.root";
    TString file_mumu_bckg = "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/yymumu_starlight_sig_sampling.root";

    TFile* fout_hists = new TFile("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/control_histograms/hists_mc_prep_ml_combined.root","RECREATE");
    TFile* fout_ml = new TFile("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data_for_ml/ml_mc_combined.root","RECREATE");

    TChain* tree_ee = new TChain("G2TauTree");
    tree_ee->Add(file_ee_sig);
    tree_ee->Add(file_ee_bckg);

    TChain* tree_mumu = new TChain("G2TauTree");
    tree_mumu->Add(file_mumu_sig);
    tree_mumu->Add(file_mumu_bckg);



    // =========================
    // HISTOGRAMS 
    // =========================
    TH1D* hist_dilepton_mass_ee  = new TH1D("hist_dilepton_mass_ee","",600,2.8,3.4);
    TH1D* hist_dilepton_pt_ee    = new TH1D("hist_dilepton_pt_ee","",500,0,0.5);
    TH1D* hist_lepton_topo_count_ee = new TH1D("hist_lepton_topo_count_ee","",100,0,10);

    TH1D* hist_dilepton_mass_mumu  = new TH1D("hist_dilepton_mass_mumu","",600,2.8,3.4);
    TH1D* hist_dilepton_pt_mumu    = new TH1D("hist_dilepton_pt_mumu","",500,0,0.5);
    TH1D* hist_lepton_topo_count_mumu = new TH1D("hist_lepton_topo_count_mumu","",100,0,10);

    // =========================
    // ML VARIABLES AND TREE
    // =========================
    // ML TREE
    TTree* mlTree = nullptr;
    // ML variables
    float FVariable, lambda, lambda2, radius, time;
    int particle_tag, isSignal;
    //float track_PixelHits, track_SCTHits, track_PixeldEdX, track_TRTHits;
    TreeSetUpML(  mlTree,
                  FVariable,
                  lambda,
                  lambda2,
                  radius,
                  time,
                  particle_tag,
                  isSignal);

    // =========================
    // RUN
    // =========================

    // ee
    particle_tag = 0;
    AnalyzeSample(tree_ee,
                  hist_dilepton_mass_ee,
                  hist_dilepton_pt_ee,
                  hist_lepton_topo_count_ee,
                  mlTree,
                  particle_tag,
                  isSignal,
                  FVariable,
                  lambda,
                  lambda2,
                  radius,
                  time);


    // mumu              
    particle_tag = 1;
    AnalyzeSample(tree_mumu,
                  hist_dilepton_mass_mumu,
                  hist_dilepton_pt_mumu,
                  hist_lepton_topo_count_mumu,
                  mlTree,
                  particle_tag,
                  isSignal,
                  FVariable,
                  lambda,
                  lambda2,
                  radius,
                  time);

    // =========================
    // SUM ALL (optional)
    // =========================
    TH1D* hist_dilepton_mass_all = (TH1D*)hist_dilepton_mass_ee->Clone("hist_dilepton_mass_all");
    hist_dilepton_mass_all->Add(hist_dilepton_mass_mumu);

    TH1D* hist_dilepton_pt_all = (TH1D*)hist_dilepton_pt_ee->Clone("hist_dilepton_pt_all");
    hist_dilepton_pt_all->Add(hist_dilepton_pt_mumu);

    TH1D* hist_lepton_topo_count_all = (TH1D*)hist_lepton_topo_count_ee->Clone("hist_lepton_topo_count_all");
    hist_lepton_topo_count_all->Add(hist_lepton_topo_count_mumu);

    // =========================
    // SAVE
    // =========================

    fout_hists->cd();


    hist_dilepton_mass_ee->SetDirectory(fout_hists);
    hist_dilepton_pt_ee->SetDirectory(fout_hists);
    hist_lepton_topo_count_ee->SetDirectory(fout_hists);

    hist_dilepton_mass_mumu->SetDirectory(fout_hists);
    hist_dilepton_pt_mumu->SetDirectory(fout_hists);
    hist_lepton_topo_count_mumu->SetDirectory(fout_hists);

    hist_dilepton_mass_all->SetDirectory(fout_hists);
    hist_dilepton_pt_all->SetDirectory(fout_hists);
    hist_lepton_topo_count_all->SetDirectory(fout_hists);


    hist_dilepton_mass_ee->Write();
    hist_dilepton_pt_ee->Write();
    hist_lepton_topo_count_ee->Write();

    hist_dilepton_mass_mumu->Write();
    hist_dilepton_pt_mumu->Write();
    hist_lepton_topo_count_mumu->Write();

    hist_dilepton_mass_all->Write();
    hist_dilepton_pt_all->Write();
    hist_lepton_topo_count_all->Write();

    fout_hists->Close();

    std::cout << "[OK] Histograms saved\n";

    std::cout << "[INFO] Saving ML tree...\n";

    fout_ml->cd();
    mlTree->SetDirectory(fout_ml);
    mlTree->Write();
    fout_ml->Close();

    std::cout << "[OK] ML tree saved\n";
}