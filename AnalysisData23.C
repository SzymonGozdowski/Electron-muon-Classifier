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
 
   // Variable to store muon mass
   const float muMass = 0.1057; // GeV
   
   // variable to check how many counts of mass between 2.9 and 3.2 GeV
   int massCount = 0;
 
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
            // I added a specific value for Pt (e.g., > 0.5) rather than just checking if it exists
            bool passKinematics = true;
            if (track_pt->at(0) < 0.1 || abs(track_eta->at(0)) > 2.5) passKinematics = false;
            if (track_pt->at(1) < 0.1 || abs(track_eta->at(1)) > 2.5) passKinematics = false;

            if (passKinematics) {
                // Set Vectors
                v0.SetPtEtaPhiM(track_pt->at(0), track_eta->at(0), track_phi->at(0), muMass);
                v1.SetPtEtaPhiM(track_pt->at(1), track_eta->at(1), track_phi->at(1), muMass);

                // Reconstruct the system
                vSystem = v0 + v1;

                // Fill Histograms
                histEta->Fill(vSystem.Eta());
                
                histPhi->Fill(vSystem.Phi());

                histPt->Fill(vSystem.Pt());
                histMass->Fill(vSystem.M());

                // Count specific mass range instances
                if(vSystem.M() > 2.9 && vSystem.M() < 3.2){
                    massCount++;
                }
            }
        }
}   
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
c1->SaveAs("Plots/histAngles.pdf]");
c1->Clear();
}

int main(){
AnalysisData23 t;
t.Loop();
return 0;
}
