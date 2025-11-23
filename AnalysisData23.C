#define AnalysisData23_cxx
#include "AnalysisData23.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <iostream>

using namespace std;

void AnalysisData23::Loop()
{

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // Initialise histograms   
   TH1F *histEta = new TH1F("histEta", "histEta", 100, -3, 3); 
   TH1F *histPhi = new TH1F("histPhi", "histPhi", 100, -3, 3); 
   TH1F *histPt = new TH1F("histPt", "histPt", 100, 0.1, 5); // na razie gorna granica z dupy 
   TH1F *histMass = new TH1F("histMass", "histMass", 100, 0, 10);
   TH1F *histRapidity = new TH1F("histRapidity", "histRapidity", 100, -3, 3);
 
   // Variable to store muon mass
   const float muMass = 0.1057; // GeV
   
   // variable to check how many counts of mass between 2.9 and 3.2 GeV
   int massCount = 0;

   // Create newfile and TTree to store classified data
   TFile* newfile = new TFile("Classified_data_e_mu_file.root", "RECREATE");
   TTree* newtree = new TTree("classified_data_e_mu", "classified_data_e_mu"); 
   
   // Variables to new Branches
   int new_track_PixelHits;
   int new_track_TRTHits;
   float new_track_PixelEdX;
   int areElec;

   // Create new Branches
   newtree->Branch("track_PixelHits", &new_track_PixelHits, "track_PixelHits/I");
   newtree->Branch("track_TRTHits", &new_track_TRTHits, "track_TRTHits/I");
   newtree->Branch("track_PixelEdX", &new_track_PixelEdX, "track_PixelEdX/F");
   newtree->Branch("areElec", &areElec, "areElec/I");   


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if(jentry%100000==0) cout<<"Processing "<<jentry<<" event..."<<endl;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
    
 
      // Selection logic 
      if(track_n ==  2 && track_charge->at(0) != track_charge->at(1)){

	    // 2. Define the two vectors
            TLorentzVector v0, v1, vSystem;

            // Check Kinematic cuts (e.g. Pt > 0.1, Eta < 2.5) for BOTH tracks
            bool passKinematics = true;
            if (track_pt->at(0) <= 0.1 || abs(track_eta->at(0)) >= 2.5) passKinematics = false;
            if (track_pt->at(1) <= 0.1 || abs(track_eta->at(1)) >= 2.5) passKinematics = false;

            if (passKinematics) {
                // Set Vectors
                v0.SetPtEtaPhiM(track_pt->at(0), track_eta->at(0), track_phi->at(0), muMass);
                v1.SetPtEtaPhiM(track_pt->at(1), track_eta->at(1), track_phi->at(1), muMass);

                // Reconstruct the system
                vSystem = v0 + v1;

   // Logic to check if electrons  
     if (nElec >= 1) {
	areElec = 1;
     } else {
	areElec = 0;
     }
                if(vSystem.Perp() < 0.2 && vSystem.M() > 2.9 && vSystem.M() < 3.2){
		  new_track_PixelHits = track_PixelHits->at(0);
		  new_track_TRTHits = track_TRTHits->at(0);
		  new_track_PixelEdX = track_PixeldEdX->at(0);
		  newtree->Fill();

		// Fill Histograms
                histEta->Fill(vSystem.Eta());
                
                histPhi->Fill(vSystem.Phi());

                histPt->Fill(vSystem.Pt());
                histMass->Fill(vSystem.M());
		histRapidity->Fill(vSystem.Rapidity());

                // Count specific mass range instances
                    massCount++;
                }
            }
        }
}   
newtree->Write();
delete newfile;

   cout<< "Counts of mass between (2.9; 3.2)GeV: " << massCount <<endl; 
   TCanvas *c1 = new TCanvas("c1", "Histograms", 1200, 400);
   c1->SaveAs("Plots/histAngles.pdf[");

histEta->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

histPhi->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

histPt->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

histMass->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

histRapidity->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->SaveAs("Plots/histAngles.pdf]");
c1->Clear();
c1->Close();
}

int main(){
AnalysisData23 t;
t.Loop();
return 0;
}
