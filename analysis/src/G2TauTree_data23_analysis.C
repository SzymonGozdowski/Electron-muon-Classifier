#define G2TauTree_data23_analysis_cxx
#include "G2TauTree_data23_analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <cmath>
#include "TTree.h"

// Analiza topoklastra
void G2TauTree_data23_analysis::TopoCluster(TLorentzVector& lepton_classified, TH1D* hist_lepton_count, TH1D* hist_lepton_pt,
    TH1D* hist_lepton_eta, TH1D* hist_lepton_phi, TH1D* hist_FVariable, TH1D* hist_EMCal, TH1D* hist_lambda, TH1D* hist_lambda2, TH1D* hist_time, TH1D* hist_radius,
   float& FVariable, float& EMprop, float& lambda, float& lambda2, float& time, float& radius){

      int topo_count = 0;
      int id = -1;
      auto cluster_max_pt = 0.0; // war. maks. pt topoklastra
      for(int i_topo = 0; i_topo < topoclus_n; i_topo++){
         auto Dphi = acos(cos(lepton_classified.Phi() - acos(cos(topo_cluster_phi->at(i_topo)))));
         auto RadEtaPhi = sqrt((lepton_classified.Eta() - topo_cluster_eta->at(i_topo))*(lepton_classified.Eta() - topo_cluster_eta->at(i_topo)) + Dphi*Dphi);

         if(RadEtaPhi < 0.5 && topo_cluster_pass_sig_cut){
            topo_count++;
            auto topo_pT = topo_cluster_pt->at(i_topo);

            if(topo_pT > cluster_max_pt){
               cluster_max_pt = topo_pT;
               id = i_topo;
            }
         }
      }
      hist_lepton_count->Fill(topo_count);
      if(id != -1){
         hist_lepton_pt->Fill(cluster_max_pt);
         FVariable = topo_cluster_pt->at(id)/lepton_classified.Pt();
         EMprop = topo_cluster_EM_prob->at(id);
         lambda = topo_cluster_lambda->at(id);
         lambda2 = topo_cluster_lambda2->at(id);
         time = topo_cluster_time->at(id);
         radius = topo_cluster_r2->at(id);

         hist_FVariable->Fill(FVariable);
         hist_EMCal->Fill(EMprop);
         hist_lambda->Fill(lambda);
         hist_lambda2->Fill(lambda2);
         hist_time->Fill(time);
         hist_radius->Fill(radius);
         hist_lepton_eta->Fill(topo_cluster_eta->at(id));
         hist_lepton_phi->Fill(topo_cluster_phi->at(id));
      } else {
         FVariable = -1;
         EMprop = -1;
         lambda = -1;
         lambda2 = -1;
         time = -1;
         radius = -1;
      }
 

};



void G2TauTree_data23_analysis::mlDataTreeInit()
{
    mlDataTree = new TTree("mlDataTree", "ML Data Tree");
    

    mlDataTree->Branch("track_PixelHits", &track_PixelHits, "track_PixelHits/O");
    mlDataTree->Branch("track_SCTHits", &track_SCTHits, "track_SCTHits/O");
    mlDataTree->Branch("track_PixeldEdX", &track_PixeldEdX, "track_PixeldEdX/F");
    mlDataTree->Branch("track_TRTHits", &track_TRTHits, "track_TRTHits/O");
    mlDataTree->Branch("lepton_FVariable", &FVariable, "lepton_FVariable/F");
    mlDataTree->Branch("lepton_EMCal", &EMprop, "lepton_EMCal/F");
    mlDataTree->Branch("lepton_lambda", &lambda, "lepton_lambda/F");
    mlDataTree->Branch("lepton_lambda2", &lambda2, "lepton_lambda2/F");
    mlDataTree->Branch("lepton_time", &time, "lepton_time/F");
    mlDataTree->Branch("lepton_radius", &radius, "lepton_radius/F");
    mlDataTree->Branch("truth_particle_tag", &truth_particle_tag, "truth_particle_tag/I"); // 0 - electron, 1 - muon
    //mlDataTree->Branch("electron_number", &hist_electron_number, "electron_number/I");
    //mlDataTree->Branch("muon_number", &hist_muon_number, "muon_number/I");
};



void G2TauTree_data23_analysis::Loop(){
   //   In a ROOT session, you can do:
   //      root> .L G2TauTree_analisys.C
   //      root> G2TauTree_analisys t
   //      root> t.GetEntry(12); // Fill t data members with entry number 12
   //      root> t.Show();       // Show values of entry 12
   //      root> t.Show(16);     // Read and show values of entry 16
   //      root> t.Loop();       // Loop on all entries
   //

   //     This is the loop skeleton where:
   //    jentry is the global entry number in the chain
   //    ientry is the entry number in the current Tree
   //  Note that the argument to GetEntry must be:
   //    jentry for TChain::GetEntry
   //    ientry for TTree::GetEntry and TBranch::GetEntry
   //
   //       To read only selected branches, Insert statements like:
   // METHOD1:
   //    fChain->SetBranchStatus("*",0);  // disable all branches
   //    fChain->SetBranchStatus("branchname",1);  // activate branchname
   // METHOD2: replace line
   //    fChain->GetEntry(jentry);       //read all branches
   //by  b_branchname->GetEntry(ientry); //read only this branch


   // Wszystkie deklaracje w  G2TauTree_analisys.h

   // Inicjalizacja histogramów
   //TH1D hist_electron_eta("hist_electron_eta",  "Electron Eta", 100, 0, 200);
   TH1D* hist_track_theta = new TH1D("hist_track_theta",  "Track Theta", 100, -0.58, 3.2);
   TH1D* hist_track_phi = new TH1D("hist_track_phi",  "Track Phi", 100, -3.2, 3.2);
   TH1D* hist_track_pt = new TH1D("hist_track_pt",  "Track Pt", 100, 0, 15000);
   TH1D* hist_track_eta = new TH1D("hist_track_eta",  "Track Eta", 300, -3, 3);
   //Inicjalizacja histogramom cut
   TH1D* hist_track_pt_cut = new TH1D ("hist_track_pt_cut", "Track Pt cut (>0.1 GeV)", 100, 0, 15000);
   TH1D* hist_track_eta_cut = new TH1D("hist_track_eta_cut", "Track Eta cut (<2.5)", 300, -3, 3);
   // Inicjalizacja histogramow dilepton
   TH1D* hist_dilepton_pt = new TH1D("hist_dilepton_pt", "Dilepton Pt", 300, 0, 0.3);
   TH1D* hist_dilepton_rapidity = new TH1D("hist_dilepton_rapidity","Dilepton Rapidity", 300, -3, 3);
   TH1D* hist_dilepton_inv_mass = new TH1D("hist_dilepton_inv_mass","Dilepton Invariant Mass ", 600, 2.8, 3.4);
   TH1D* hist_dilepton_DR = new TH1D("hist_dilepton_DR", "Dilepton DR", 500, 0, 0.5);

   // Inicjalizacj histogramow zliczajacych czastki
   TH1D* hist_dilepton_number = new TH1D("hist_dilepton_number", "Number of dileptons", 5, 0, 5);
   TH1D* hist_muon_number = new TH1D("hist_muon_number", "Number of muons", 4, -1, 3);
   TH1D* hist_electron_number = new TH1D("hist_electron_number", "Number of electrons", 3, -1, 2);

   //Inicjalizacja hist elektronow
   TH1D* hist_electron_E = new TH1D("hist_electron_E", "Electron Energy", 100, 0, 10);
   TH1D* hist_electron_Pt = new TH1D("hist_electron_Pt", "Electron Pt", 200, 0, 2);
   TH1D* hist_electron_Eta = new TH1D("hist_electron_Eta", "Electron Eta", 250, -0.5, 3);
   TH1D* hist_electron_Phi = new TH1D("hist_electron_Phi", "Electron Phi", 350, -0.5, 4);

   //Inicjalizacja hist mionow
   TH1D* hist_muon_E = new TH1D("hist_muon_E", "Muon Energy", 100, 0, 10);
   TH1D* hist_muon_Pt = new TH1D("hist_muon_Pt", "Muon Pt", 200, 0, 2);
   TH1D* hist_muon_Eta = new TH1D("hist_muon_Eta", "Muon Eta", 250, -0.5, 3);
   TH1D* hist_muon_Phi = new TH1D("hist_muon_Phi", "Muon Phi", 350, -0.5, 4);

   //Inicjalizacja hist pixel
   TH1D* hist_track_PixeldEdX = new TH1D("hist_track_PixeldEdX", " Track PixeldEdX", 200, -10, 10);
   TH1D* hist_track_PixelHits = new TH1D("hist_track_PixelHits", "Track PixelHits", 10, 0, 10);
   TH1D* hist_track_SCTHits = new TH1D("hist_track_SCTHits", "Track SCTHits", 16, 0, 16);
   TH1D* hist_track_TRTHits = new TH1D("hist_track_TRTHits", "Track TRTHits", 500, 0, 50);

   //Inicjalizacja hist pixel elektronow
   TH1D* hist_electron_PixeldEdX = new TH1D("hist_electron_PixeldEdX", " Electron PixeldEdX", 600, -2, 4);
   TH1D* hist_electron_PixelHits = new TH1D("hist_electron_PixelHits", "Electron PixelHits", 10, 0, 10);
   TH1D* hist_electron_SCTHits = new TH1D("hist_electron_SCTHits", "Electron SCTHits", 16, 0, 16);
   TH1D* hist_electron_TRTHits = new TH1D("hist_electron_TRTHits", "Electron TRTHits", 500, 0, 50);

   //Inicjalizacja hist pixel muonow
   TH1D* hist_muon_PixeldEdX = new TH1D("hist_muon_PixeldEdX", " Muon PixeldEdX", 600, -2, 4);
   TH1D* hist_muon_PixelHits = new TH1D("hist_muon_PixelHits", "Muon PixelHits", 10, 0, 10);
   TH1D* hist_muon_SCTHits = new TH1D("hist_muon_SCTHits", "Muon SCTHits", 16, 0, 16);
   TH1D* hist_muon_TRTHits = new TH1D("hist_muon_TRTHits", "Muon TRTHits", 500, 0, 50);

   //Inicjalizacja hist topocluster
   //TH1D hist_FVariable("hist_FVariable", "TopoCluster FVariable", 100, 0, 1);
   //TH1D hist_EMCal("hist_EMCal", "TopoCluster EMCal", 100, 0, 1);
   TH1D* hist_topo_eta = new TH1D("hist_topo_eta", "TopoCluster Eta", 100, -5, 5);
   TH1D* hist_topo_phi = new TH1D("hist_topo_phi", "TopoCluster Phi", 800, -4, 4);
   TH1D* hist_topo_pt = new TH1D("hist_topo_pt", "TopoCluster Pt", 800, -2, 6);
   TH1D* hist_topo_lambda = new TH1D("hist_topo_lambda", "TopoCluster Lambda", 600, 0, 6000);
   TH1D* hist_topo_lambda2 = new TH1D("hist_topo_lambda2", "TopoCluster Lambda2", 60, 0, 600);
   TH1D* hist_topo_time = new TH1D("hist_topo_time", "TopoCluster Time", 70, -30, 40);
   TH1D* hist_topo_radius = new TH1D("hist_topo_radius", "TopoCluster Radius", 100, 0, 2.1e5);

   //Inicjalizacja hist topocluster elektronow
   TH1D* hist_electron_FVariable = new TH1D("hist_electron_FVariable", "Electron TopoCluster FVariable", 100, 0, 1);
   TH1D* hist_electron_EMCal = new TH1D("hist_electron_EMCal", "Electron TopoCluster EMCal", 100, 0, 1);
   TH1D* hist_electron_topo_eta = new TH1D("hist_electron_topo_eta", "Electron TopoCluster Eta", 100, -5, 5);
   TH1D* hist_electron_topo_phi = new TH1D("hist_electron_topo_phi", "Electron TopoCluster Phi", 800, -4, 4);
   TH1D* hist_electron_topo_pt = new TH1D("hist_electron_topo_pt", "Electron TopoCluster Pt", 800, -2, 6);
   TH1D* hist_electron_topo_lambda = new TH1D("hist_electron_topo_lambda", "Electron TopoCluster Lambda", 600, 0, 6000);
   TH1D* hist_electron_topo_lambda2 = new TH1D("hist_electron_topo_lambda2", "Electron TopoCluster Lambda2", 600, 0, 600);
   TH1D* hist_electron_topo_time = new TH1D("hist_electron_topo_time", "Electron TopoCluster Time", 70, -30, 40);
   TH1D* hist_electron_topo_radius = new TH1D("hist_electron_topo_radius", "Electron TopoCluster Radius", 100, 0, 2.1e5);

   //Inicjalizacja hist topocluster muonow
   TH1D* hist_muon_FVariable = new TH1D("hist_muon_FVariable", "Muon TopoCluster FVariable", 100, 0, 1);
   TH1D* hist_muon_EMCal = new TH1D("hist_muon_EMCal", "Muon TopoCluster EMCal", 100, 0, 1);
   TH1D* hist_muon_topo_eta = new TH1D("hist_muon_topo_eta", "Muon TopoCluster Eta", 100, -5, 5);
   TH1D* hist_muon_topo_phi = new TH1D("hist_muon_topo_phi", "Muon TopoCluster Phi", 800, -4, 4);
   TH1D* hist_muon_topo_pt = new TH1D("hist_muon_topo_pt", "Muon TopoCluster Pt", 800, -2, 6);
   TH1D* hist_muon_topo_lambda = new TH1D("hist_muon_topo_lambda", "Muon TopoCluster Lambda", 600, 0, 6000);
   TH1D* hist_muon_topo_lambda2 = new TH1D("hist_muon_topo_lambda2", "Muon TopoCluster Lambda2", 600, 0, 600);
   TH1D* hist_muon_topo_time = new TH1D("hist_muon_topo_time", "Muon TopoCluster Time", 70, -30, 40);
   TH1D* hist_muon_topo_radius = new TH1D("hist_muon_topo_radius", "Muon TopoCluster Radius", 100, 0, 2.1e5);

   // Inicjalizaca hist Cell Significance Sampling Layer
   TH1D* hist_topo_cell_sig_sampling = new TH1D("hist_topo_cell_sig_sampling", "TopoCluster Cell Significance Sampling Layer", 20, -0.5, 19.5);

   std::vector<TH1*> histograms;
   histograms.push_back(hist_track_theta);
   histograms.push_back(hist_track_phi);
   histograms.push_back(hist_track_pt);
   histograms.push_back(hist_track_eta);

   histograms.push_back(hist_track_pt_cut);
   histograms.push_back(hist_track_eta_cut);

   histograms.push_back(hist_dilepton_pt);
   histograms.push_back(hist_dilepton_rapidity);
   histograms.push_back(hist_dilepton_inv_mass);
   histograms.push_back(hist_dilepton_DR);
   histograms.push_back(hist_dilepton_number);

   histograms.push_back(hist_muon_number);
   histograms.push_back(hist_electron_number);

   histograms.push_back(hist_electron_E);
   histograms.push_back(hist_electron_Pt);
   histograms.push_back(hist_electron_Eta);
   histograms.push_back(hist_electron_Phi);

   histograms.push_back(hist_muon_E);
   histograms.push_back(hist_muon_Pt);
   histograms.push_back(hist_muon_Eta);
   histograms.push_back(hist_muon_Phi);

   histograms.push_back(hist_track_PixeldEdX);
   histograms.push_back(hist_track_PixelHits);
   histograms.push_back(hist_track_SCTHits);
   histograms.push_back(hist_track_TRTHits);

   histograms.push_back(hist_electron_PixeldEdX);
   histograms.push_back(hist_electron_PixelHits);
   histograms.push_back(hist_electron_SCTHits);
   histograms.push_back(hist_electron_TRTHits);

   histograms.push_back(hist_muon_PixeldEdX);
   histograms.push_back(hist_muon_PixelHits);
   histograms.push_back(hist_muon_SCTHits);
   histograms.push_back(hist_muon_TRTHits);

   histograms.push_back(hist_topo_eta);
   histograms.push_back(hist_topo_phi);
   histograms.push_back(hist_topo_pt);
   histograms.push_back(hist_topo_lambda);
   histograms.push_back(hist_topo_lambda2);
   histograms.push_back(hist_topo_time);
   histograms.push_back(hist_topo_radius);

   histograms.push_back(hist_electron_FVariable);
   histograms.push_back(hist_electron_EMCal);
   histograms.push_back(hist_electron_topo_eta);
   histograms.push_back(hist_electron_topo_phi);
   histograms.push_back(hist_electron_topo_pt);
   histograms.push_back(hist_electron_topo_lambda);
   histograms.push_back(hist_electron_topo_lambda2);
   histograms.push_back(hist_electron_topo_time);
   histograms.push_back(hist_electron_topo_radius);
   
   histograms.push_back(hist_muon_FVariable);
   histograms.push_back(hist_muon_EMCal);
   histograms.push_back(hist_muon_topo_eta);
   histograms.push_back(hist_muon_topo_phi);
   histograms.push_back(hist_muon_topo_pt);
   histograms.push_back(hist_muon_topo_lambda);
   histograms.push_back(hist_muon_topo_lambda2);
   histograms.push_back(hist_muon_topo_time);
   histograms.push_back(hist_muon_topo_radius);

   histograms.push_back(hist_topo_cell_sig_sampling);

   //int electron_number = 0;
   //int muon_number = 0;


   truth_particle_tag = -1; // 0 - electron, 1 - muon
   const double MUON_MASS = 0.1056583745; // GeV/c^2

   mlDataTreeInit();


   

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   //Long64_t nentries = 10000; for debug

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // update
      if (jentry % 10000 == 0) {
    	std::cout << "Progress: " << jentry << " / " << nentries << " entries\r" << std::flush;
    	}

      //filling track_theta
      for (int i = 0; i < track_theta->size(); i++){
         hist_track_theta->Fill(track_theta->at(i));
      }
      // filling track_phi
      for (int i = 0; i < track_phi->size(); i++){
         hist_track_phi->Fill(track_phi->at(i));
      }
      // filling track_eta
      for (int i = 0; i < track_eta->size(); i++){
         hist_track_eta->Fill(track_eta->at(i));
         //if(abs(track_eta->at(i)) < 2.5){
            hist_track_eta_cut->Fill(track_eta->at(i));
         //}
      }
      // filling track_pt
      for (int i = 0; i < track_pt->size(); i++){
         hist_track_pt->Fill(track_pt->at(i));
         //if(track_pt->at(i) > 0.1){
            hist_track_pt_cut->Fill(track_pt->at(i));
         //}
      }
      //filling track_PixeldEdX
      for (int i = 0; i < track_PixeldEdX->size(); i++){
         hist_track_PixeldEdX->Fill(track_PixeldEdX->at(i));
      }
      //filling track_PixelHits
      for (int i = 0; i < track_PixelHits->size(); i++){
         hist_track_PixelHits->Fill(track_PixelHits->at(i));
      }
      //filling track_SCTHits
      for (int i = 0; i < track_SCTHits->size(); i++){
         hist_track_SCTHits->Fill(track_SCTHits->at(i));
      }
      //filling track_TRTHits
      for (int i = 0; i < track_TRTHits->size(); i++){
         hist_track_TRTHits->Fill(track_TRTHits->at(i));
      }

      // filling topocluster
      for (int i = 0; i < topo_cluster_eta->size(); i++){
         hist_topo_eta->Fill(topo_cluster_eta->at(i));
      }
      for (int i = 0; i < topo_cluster_phi->size(); i++){
         hist_topo_phi->Fill(topo_cluster_phi->at(i));
      }
      for (int i = 0; i < topo_cluster_pt->size(); i++){
         hist_topo_pt->Fill(topo_cluster_pt->at(i));
      }
      for (int i = 0; i < topo_cluster_lambda->size(); i++){
         hist_topo_lambda->Fill(topo_cluster_lambda->at(i));
      }
      for (int i = 0; i < topo_cluster_lambda2->size(); i++){
         hist_topo_lambda2->Fill(topo_cluster_lambda2->at(i));
      }
      for (int i = 0; i < topo_cluster_time->size(); i++){
         hist_topo_time->Fill(topo_cluster_time->at(i));
      }
      for (int i = 0; i < topo_cluster_r2->size(); i++){
         hist_topo_radius->Fill(topo_cluster_r2->at(i));
      }
      // filling topo_cluster_cell_sig_sampling
      for (int i = 0; i < topo_cluster_cell_sig_sampling->size(); i++){
         hist_topo_cell_sig_sampling->Fill(topo_cluster_cell_sig_sampling->at(i));
      }
      
      // dileptons
      if(track_n == 2){
         vector<TLorentzVector> p_lepton;
         vector<TLorentzVector> p_dileptons; // electrons + muons (wszystkie pary)
         vector<TLorentzVector> p_electrons; // zaklasyfikowane e-
         vector<TLorentzVector> p_muons; // zaklasyfikowane mu

         for(int i = 0; i < track_n; i++){
            if(track_pt->at(i) <= 0.1 || abs(track_eta->at(i)) >= 2.5) continue; // continue - przeciwne war.!
            TLorentzVector p_temp;
            p_temp.SetPtEtaPhiM(track_pt->at(i), track_eta->at(i), track_phi->at(i), MUON_MASS);
            p_lepton.push_back(p_temp);
         }

         if (p_lepton.size() == 2 && track_charge->at(0) * track_charge->at(1) < 0){
            TLorentzVector p_dilepton;
            p_dilepton = p_lepton[0] + p_lepton[1];
            p_dileptons.push_back(p_dilepton);

            auto dimass = p_dilepton.M();
            auto dileptonPt = p_dilepton.Pt();
            auto dileptonRapidity = p_dilepton.Rapidity();
            

             // filling dilepton M, Pt, Rapidity; cut Pt < 0.2Gev; cut dimass < 3.2 and dimass > 2.9
            //if(dimass > 3.2 || dimass < 2.9) continue;
            if (dileptonPt >= 0.2) continue; // continue - przeciwne war.!
            hist_dilepton_inv_mass->Fill(dimass);
            hist_dilepton_rapidity->Fill(dileptonRapidity);
            hist_dilepton_pt->Fill(dileptonPt);
            
            // Wyznaczanie delta R;  eg_cluster (i track) <=> e-
            bool classifiedAsElectron = false;
            for(int i = 0; i < eg_cluster_n; i++){
               auto Deta = p_lepton[0].Eta() - eg_cluster_eta->at(i);
               auto Dphi = TVector2::Phi_mpi_pi(p_lepton[0].Phi() - eg_cluster_phi->at(i));
               auto DR = sqrt(Deta*Deta + Dphi*Dphi);
               hist_dilepton_DR->Fill(DR);
               // Cut na delta R < 0.5
               if (DR >= 0.5) continue; // continue - przeciwne war. !
               classifiedAsElectron = true;
            }

            //Sprawdzanie Cell Significance Sampling Layer; war. > 7
            bool HasHighCellSignificance = false;

            for(int i = 0; i < topoclus_n; i++){
               auto Deta = p_lepton[1].Eta() - topo_cluster_eta->at(i);
               auto Dphi = TVector2::Phi_mpi_pi(p_lepton[1].Phi() - topo_cluster_phi->at(i));
               auto DR = sqrt(Deta*Deta + Dphi*Dphi);
               if(DR >= 0.5) continue;
               if(topo_cluster_cell_sig_sampling->at(i) <= 7) continue;
               HasHighCellSignificance = true;
            }

            if(classifiedAsElectron){
               p_electrons.push_back(p_lepton[1]); // 1-sza cząstka p_lepton[0] pominięta, druga uznana za e-
            } else {
               if(HasHighCellSignificance){
                  //p_muons.push_back(p_lepton[0]); // obie cząstki uznawane za miony
                  p_muons.push_back(p_lepton[1]); // 1-sza cząstka p_lepton[0] pominięta, druga uznana za mion (po war. Cell Significance Sampling Layer)
               }
            }

            // filling hist dilepton
            hist_dilepton_number->Fill(p_dileptons.size());
            // filling hist e-
            if(!p_electrons.empty()){
               //std::cout <<"[DEBUG] filling electotron hists." <<std::endl;
               //electron_number = p_electrons.size();
               hist_electron_number->Fill(p_electrons.size()); // p_electrons zawiera albo 0 albo 1 element
               hist_electron_E->Fill(p_electrons[0].E());
               hist_electron_Pt->Fill(p_electrons[0].Pt());
               hist_electron_Eta->Fill(p_electrons[0].Eta());
               hist_electron_Phi->Fill(p_electrons[0].Phi());

               hist_electron_PixeldEdX->Fill(track_PixeldEdX->at(0));
               hist_electron_PixelHits->Fill(track_PixelHits->at(0));
               hist_electron_SCTHits->Fill(track_SCTHits->at(0));
               hist_electron_TRTHits->Fill(track_TRTHits->at(0));
               
               //std::cout <<"[DEBUG] calling TopoCluster for electron." <<std::endl;
               TopoCluster(p_electrons[0], hist_electron_number, hist_electron_topo_pt, hist_electron_topo_eta, hist_electron_topo_phi, hist_electron_FVariable, hist_electron_EMCal, hist_electron_topo_lambda,
                  hist_electron_topo_lambda2, hist_electron_topo_time, hist_electron_topo_radius, FVariable, EMprop, lambda, lambda2, time, radius);

               truth_particle_tag = 0; // 0 - electron, 1 - muon
                  
               
               mlDataTree->Fill(); // Zapis danych do drzewa ML 
               
            }

            //filling hist muon
            if(!p_muons.empty()){
               //muon_number = p_muons.size();
               hist_muon_number->Fill(p_muons.size()); // p_muons zawiera albo 0 albo 2 elementy
               //for(int i = 0; i < p_muons.size(); i++){
                  hist_muon_E->Fill(p_muons[0].E());
                  hist_muon_Pt->Fill(p_muons[0].Pt());
                  hist_muon_Eta->Fill(p_muons[0].Eta());
                  hist_muon_Phi->Fill(p_muons[0].Phi());

                  hist_muon_PixeldEdX->Fill(track_PixeldEdX->at(0));
                  hist_muon_PixelHits->Fill(track_PixelHits->at(0));
                  hist_muon_SCTHits->Fill(track_SCTHits->at(0));
                  hist_muon_TRTHits->Fill(track_TRTHits->at(0));

                  //std::cout <<"[DEBUG] calling TopoCluster for muon." <<std::endl;
                  TopoCluster(p_muons[0], hist_muon_number, hist_muon_topo_pt, hist_muon_topo_eta, hist_muon_topo_phi, hist_muon_FVariable, hist_muon_EMCal, hist_muon_topo_lambda,
                  hist_muon_topo_lambda2, hist_muon_topo_time, hist_muon_topo_radius, FVariable, EMprop, lambda, lambda2, time, radius);

                  truth_particle_tag = 1; // 0 - electron, 1 - muon
                  
                    
                  mlDataTree->Fill(); // Zapis danych do drzewa ML

               //}
            }
            
         }
      }
   }






   // Saving hists to output file
   //hist_electron_eta.Write();




   std::cout << "[DONE] Loop finished." << std::endl;

   // Histograms
   fOut->cd();
   for(auto h : histograms){ 
      h->SetDirectory(fOut); 
      h->Write();
   }
   fOut->Close();

   // ML Tree
   fOutMLdata->cd();
   mlDataTree->SetDirectory(fOutMLdata);
   mlDataTree->Write();  
   fOutMLdata->Close();



   std::cout << "[OK] Histograms and ML data saved." << std::endl;
}

