#define G2TauTree_data23_analysis_cxx
#include "G2TauTree_data23_analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <cmath>




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
   TH1D hist_track_theta("hist_track_theta",  "Track Theta", 100, -0.58, 3.2);
   TH1D hist_track_phi("hist_track_phi",  "Track Phi", 100, -3.2, 3.2);
   TH1D hist_track_pt("hist_track_pt",  "Track Pt", 100, 0, 15000);
   TH1D hist_track_eta("hist_track_eta",  "Track Eta", 300, -3, 3);
   //Inicjalizacja histogramom cut
   TH1D hist_track_pt_cut("hist_track_pt_cut", "Track Pt cut (>0.1 GeV)", 100, 0, 15000);
   TH1D hist_track_eta_cut("hist_track_eta_cut", "Track Eta cut (<2.5)", 300, -3, 3);
   // Inicjalizacja histogramow dilepton
   TH1D hist_dilepton_pt("hist_dilepton_pt", "Dilepton Pt", 500, 0, 0.5);
   TH1D hist_dilepton_rapidity("hist_dilepton_rapidity","Dilepton Rapidity", 300, -3, 3);
   TH1D hist_dilepton_inv_mass("hist_dilepton_inv_mass","Dilepton Invariant Mass ", 600, 2.8, 3.4);
   TH1D hist_dilepton_DR("hist_dilepton_DR", "Dilepton DR", 500, 0, 0.5);

   // Inicjalizacj histogramow zliczajacych czastki
   TH1D hist_dilepton_number("hist_dilepton_number", "Number of dileptons", 5, 0, 5);
   TH1D hist_muon_number("hist_muon_number", "Number of muons", 5, 0, 5);
   TH1D hist_electron_number("hist_electron_number", "Number of electrons", 5, 0, 5);

   //Inicjalizacja hist elektronow
   TH1D hist_electron_E("hist_electron_E", "Electron Energy", 100, 0, 10);
   TH1D hist_electron_Pt("hist_electron_Pt", "Electron Pt", 200, 0, 2);
   TH1D hist_electron_Eta("hist_electron_Eta", "Electron Eta", 250, -0.5, 3);
   TH1D hist_electron_Phi("hist_electron_Phi", "Electron Phi", 350, -0.5, 4);

   //Inicjalizacja hist mionow
   TH1D hist_muon_E("hist_muon_E", "Muon Energy", 100, 0, 10);
   TH1D hist_muon_Pt("hist_muon_Pt", "Muon Pt", 200, 0, 2);
   TH1D hist_muon_Eta("hist_muon_Eta", "Muon Eta", 250, -0.5, 3);
   TH1D hist_muon_Phi("hist_muon_Phi", "Muon Phi", 350, -0.5, 4);

   //Inicjalizacja hist pixel
   TH1D hist_track_PixeldEdX("hist_track_PixeldEdX", " Track PixeldEdX", 200, -10, 10);
   TH1D hist_track_PixelHits("hist_track_PixelHits", "Track PixelHits", 10, 0, 10);
   TH1D hist_track_SCTHits("hist_track_SCTHits", "Track SCTHits", 16, 0, 16);
   TH1D hist_track_TRTHits("hist_track_TRTHits", "Track TRTHits", 500, 0, 50);

   //Inicjalizacja hist pixel elektronow
   TH1D hist_electron_PixeldEdX("hist_electron_PixeldEdX", " Electron PixeldEdX", 200, -10, 10);
   TH1D hist_electron_PixelHits("hist_electron_PixelHits", "Electron PixelHits", 10, 0, 10);
   TH1D hist_electron_SCTHits("hist_electron_SCTHits", "Electron SCTHits", 16, 0, 16);
   TH1D hist_electron_TRTHits("hist_electron_TRTHits", "Electron TRTHits", 500, 0, 50);

   //Inicjalizacja hist pixel muonow
   TH1D hist_muon_PixeldEdX("hist_muon_PixeldEdX", " Muon PixeldEdX", 200, -10, 10);
   TH1D hist_muon_PixelHits("hist_muon_PixelHits", "Muon PixelHits", 10, 0, 10);
   TH1D hist_muon_SCTHits("hist_muon_SCTHits", "Muon SCTHits", 16, 0, 16);
   TH1D hist_muon_TRTHits("hist_muon_TRTHits", "Muon TRTHits", 500, 0, 50);

   const double MUON_MASS = 0.1056583745;
  

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
         hist_track_theta.Fill(track_theta->at(i));
      }
      // filling track_phi
      for (int i = 0; i < track_phi->size(); i++){
         hist_track_phi.Fill(track_phi->at(i));
      }
      // filling track_eta
      for (int i = 0; i < track_eta->size(); i++){
         hist_track_eta.Fill(track_eta->at(i));
         if(abs(track_eta->at(i)) < 2.5){
            hist_track_eta_cut.Fill(track_eta->at(i));
         }
      }
      // filling track_pt
      for (int i = 0; i < track_pt->size(); i++){
         hist_track_pt.Fill(track_pt->at(i));
         if(track_pt->at(i) > 0.1){
            hist_track_pt_cut.Fill(track_pt->at(i));
         }
      }
      //filling track_PixeldEdX
      for (int i = 0; i < track_PixeldEdX->size(); i++){
         hist_track_PixeldEdX.Fill(track_PixeldEdX->at(i));
      }
      //filling track_PixelHits
      for (int i = 0; i < track_PixelHits->size(); i++){
         hist_track_PixelHits.Fill(track_PixelHits->at(i));
      }
      //filling track_SCTHits
      for (int i = 0; i < track_SCTHits->size(); i++){
         hist_track_SCTHits.Fill(track_SCTHits->at(i));
      }
      //filling track_TRTHits
      for (int i = 0; i < track_TRTHits->size(); i++){
         hist_track_TRTHits.Fill(track_TRTHits->at(i));
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
            if (dileptonPt >= 0.2 || dimass > 3.2 || dimass < 2.9) continue; // continue - przeciwne war.!
            hist_dilepton_inv_mass.Fill(dimass);
            hist_dilepton_rapidity.Fill(dileptonRapidity);
            hist_dilepton_pt.Fill(dileptonPt);
            
            // Wyznaczanie delta R;  eg_cluster (i track) <=> e-
            bool classifiedAsElectron = false;
            for(int i = 0; i < eg_cluster_n; i++){
               auto Deta = p_lepton[0].Eta() - eg_cluster_eta->at(i);
               auto Dphi = p_lepton[0].Phi() - eg_cluster_phi->at(i);
               // auto Dphi TVector2::Phi_mpi_pi(p_lepton[0].Phi() - eg_cluster_phi->at(i));
               auto DR = sqrt(Deta*Deta + Dphi*Dphi);
               hist_dilepton_DR.Fill(DR);
               // Cut na delta R < 0.5
               if (DR >= 0.5) continue; // continue - przeciwne war. !
               classifiedAsElectron = true;
            }

            if(classifiedAsElectron){
               p_electrons.push_back(p_lepton[1]); // 1-sza cząstka p_lepton[0] pominięta, druga uznana za e-
            } else {
               p_muons.push_back(p_lepton[0]);
               p_muons.push_back(p_lepton[1]); // obie cząstki uznawane za miony
            }

            // filling hist dilepton
            hist_dilepton_number.Fill(p_dileptons.size());

            // filling hist e-
            if(!p_electrons.empty()){
               //std::cout <<"[DEBUG] filling electotron hists." <<std::endl;
               hist_electron_number.Fill(p_electrons.size()); // p_electrons zawiera albo 0 albo 1 element
               hist_electron_E.Fill(p_electrons[0].E());
               hist_electron_Pt.Fill(p_electrons[0].Pt());
               hist_electron_Eta.Fill(p_electrons[0].Eta());
               hist_electron_Phi.Fill(p_electrons[0].Phi());

               hist_electron_PixeldEdX.Fill(track_PixeldEdX->at(0));
               hist_electron_PixelHits.Fill(track_PixelHits->at(0));
               hist_electron_SCTHits.Fill(track_SCTHits->at(0));
               hist_electron_TRTHits.Fill(track_TRTHits->at(0));
               
            }

            //filling hist muon
            if(!p_muons.empty()){
               hist_muon_number.Fill(p_muons.size());
               for(int i = 0; i < p_muons.size(); i++){
                  hist_muon_E.Fill(p_muons[i].E());
                  hist_muon_Pt.Fill(p_muons[i].Pt());
                  hist_muon_Eta.Fill(p_muons[i].Eta());
                  hist_muon_Phi.Fill(p_muons[i].Phi());

                  hist_muon_PixeldEdX.Fill(track_PixeldEdX->at(i));
                  hist_muon_PixelHits.Fill(track_PixelHits->at(i));
                  hist_muon_SCTHits.Fill(track_SCTHits->at(i));
                  hist_muon_TRTHits.Fill(track_TRTHits->at(i));

               }
            }
            
         }
      }
   }



   // Saving hists to output file
   //hist_electron_eta.Write();
   hist_track_theta.Write();
   hist_track_phi.Write();
   hist_track_pt.Write();
   hist_track_eta.Write();

   hist_track_eta_cut.Write();
   hist_track_pt_cut.Write();

   hist_dilepton_inv_mass.Write();
   hist_dilepton_pt.Write();
   hist_dilepton_rapidity.Write();

   hist_dilepton_number.Write();

   hist_dilepton_DR.Write();
   
   hist_electron_number.Write();
   hist_electron_E.Write();
   hist_electron_Pt.Write();
   hist_electron_Eta.Write();
   hist_electron_Phi.Write();

   hist_muon_number.Write();
   hist_muon_E.Write();
   hist_muon_Pt.Write();
   hist_muon_Eta.Write();
   hist_muon_Phi.Write();

   hist_track_PixeldEdX.Write();
   hist_track_PixelHits.Write();
   hist_track_SCTHits.Write();
   hist_track_TRTHits.Write();

   hist_electron_PixeldEdX.Write();
   hist_electron_PixelHits.Write();
   hist_electron_SCTHits.Write();
   hist_electron_TRTHits.Write();

   hist_muon_PixeldEdX.Write();
   hist_muon_PixelHits.Write();
   hist_muon_SCTHits.Write();
   hist_muon_TRTHits.Write();



   std::cout << "[OK] Histograms saved." << std::endl;
   std::cout << "[DONE]" << std::endl;
}

