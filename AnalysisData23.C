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

   // Initialise histograms for whole system 
   TH1F *histEta = new TH1F("histEta", "histEta", 100, -3, 3); 
   TH1F *histPhi = new TH1F("histPhi", "histPhi", 100, -3, 3); 
   TH1F *histPt = new TH1F("histPt", "histPt", 100, 0.1, 2); 
   TH1F *histMass = new TH1F("histMass", "histMass", 100, 1, 5);
   TH1F *histRapidity = new TH1F("histRapidity", "histRapidity", 100, -3, 3);
 
   // Initialise histograms for electrons
   TH1F *eHistEta = new TH1F("eHistEta", "eHistEta", 100, -3, 3); 
   TH1F *eHistPhi = new TH1F("eHistPhi", "eHistPhi", 100, -3, 3); 
   TH1F *eHistPt = new TH1F("eHistPt", "eHistPt", 100, 0.1, 2); 
   TH1F *eHistMass = new TH1F("eHistMass", "eHistMass", 100, 1, 5);
   TH1F *eHistRapidity = new TH1F("eHistRapidity", "eHistRapidity", 100, -3, 3);
   
   // Initialise histograms for muons

   TH1F *muHistEta = new TH1F("muHistEta", "muHistEta", 100, -3, 3); 
   TH1F *muHistPhi = new TH1F("muHistPhi", "muHistPhi", 100, -3, 3); 
   TH1F *muHistPt = new TH1F("muHistPt", "muHistPt", 100, 0.1, 2); 
   TH1F *muHistMass = new TH1F("muHistMass", "muHistMass", 100, 1, 5);
   TH1F *muHistRapidity = new TH1F("muHistRapidity", "muHistRapidity", 100, -3, 3);
   
   // Initialise histograms for whole system, with differentation e/mu 
   TH1F *diffHistPt = new TH1F("diffHistPt", "diffHistPt", 100, 0.1, 2); 
   TH1F *diffHistMass = new TH1F("diffHistMass", "diffHistMass", 100, 1, 5);
   TH1F *diffHistRapidity = new TH1F("diffHistRapidity", "diffHistRapidity", 100, -3, 3);

   // Variable to store muon mass
   const float muMass = 0.1057; // GeV
   const float elecMass = 0.00051;
 
   // variable to check how many valid varticles/pairs
   int signalCount = 0;
   int elecCount = 0;
   int muCount = 0;

   // Create newfile and TTree to store classified data
   TFile* newfile = new TFile("Classified_data_e_mu_file.root", "RECREATE");
   TTree* newtree = new TTree("classified_data_e_mu", "classified_data_e_mu"); 
   
   // Variables to new Branches
   float new_track_PixelHits = 0;
   float new_track_TRTHits = 0;
   float new_track_PixeldEdX = 0;
   float new_track_SCTHits = 0;
   int areElec;
   // isSignal to check if there are e/mu to reconstruct and not a background
   bool isSignal = false;

   // Create new Branches, last changes floats instead of vectors
   newtree->Branch("track_PixelHits", &new_track_PixelHits, "new_track_PixelHits/F");
   newtree->Branch("track_TRTHits", &new_track_TRTHits, "new_track_TRTHits/F");
   newtree->Branch("track_PixeldEdX", &new_track_PixeldEdX, "new_track_PixeldEdX/F");
   newtree->Branch("track_SCTHits", &new_track_SCTHits, "new_track_SCTHits/F");
   newtree->Branch("areElec", &areElec, "areElec/I");   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if(jentry%100000==0) cout<<"Processing "<<jentry<<" event..."<<endl;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      isSignal = false; 
      // Selection logic 
      if(track_n ==  2 && track_charge->at(0) != track_charge->at(1)){
	    // Set electrons and muons to 0, to set background
	    areElec = 0;
	    // 2. Define the two vectors
            TLorentzVector v0, v1, vSystem;
	   
            // Check Kinematic cuts (e.g. Pt > 0.1, Eta < 2.5) for BOTH tracks
            bool passKinematics = true;
            if (track_pt->at(0) <= 0.1 || abs(track_eta->at(0)) >= 2.5) passKinematics = false;
            if (track_pt->at(1) <= 0.1 || abs(track_eta->at(1)) >= 2.5) passKinematics = false;
            
	    if (passKinematics) {
   	    
		    // Logic to check if electrons  
		    if (eg_cluster_n >= 1) {
			// Set Vectors as electrons
			v0.SetPtEtaPhiM(track_pt->at(0), track_eta->at(0), track_phi->at(0), elecMass);
			v1.SetPtEtaPhiM(track_pt->at(1), track_eta->at(1), track_phi->at(1), elecMass);
			vSystem = v0 + v1;
			// DeltaR to check if products are from one collisions
			// I check delta R between possible electron and reconstructed data from electron-gamma cluster
			float deltaR =sqrt(pow( (*track_eta)[0] - (*eg_cluster_eta)[0], 2) + 
                                                        pow( (*track_phi)[0] - (*eg_cluster_phi)[0], 2)); 
			if (deltaR < 0.5){

			  if(vSystem.Pt() < 0.2 && vSystem.M() < 3){
				isSignal = true; 
				areElec = 1;
				elecCount++;
				// Fill Histograms for electrons
				eHistEta->Fill(vSystem.Eta());
				
				eHistPhi->Fill(vSystem.Phi());

				eHistPt->Fill(vSystem.Pt());
				eHistMass->Fill(vSystem.M());
				eHistRapidity->Fill(vSystem.Rapidity());
				// Fill branches in new TTree, but only to unmatched electron(hipotheses)
				// to avoid bias
				new_track_PixelHits = track_PixelHits->at(1);
				new_track_TRTHits = track_TRTHits->at(1);
				new_track_PixeldEdX = track_PixeldEdX->at(1);
				new_track_SCTHits = track_SCTHits->at(1);
				newtree->Fill();
			   }
			}
		    } else {
			// Set Vectors
			v0.SetPtEtaPhiM(track_pt->at(0), track_eta->at(0), track_phi->at(0), muMass);
			v1.SetPtEtaPhiM(track_pt->at(1), track_eta->at(1), track_phi->at(1), muMass);
			vSystem = v0 + v1;
			// DeltaR to check if products are from one collisions
			//if (v0.DeltaR(v1) <= 0.5){

			  if(vSystem.Pt() < 0.2 && vSystem.M() > 2.9 && vSystem.M() < 3.2){
				isSignal = true; 
				areElec = 0;
				muCount++;
				// Fill Histograms for muons
				muHistEta->Fill(vSystem.Eta());
				
				muHistPhi->Fill(vSystem.Phi());

				muHistPt->Fill(vSystem.Pt());
				muHistMass->Fill(vSystem.M());
				muHistRapidity->Fill(vSystem.Rapidity());
				
				for(int i=0; i<2; i++){
					new_track_PixelHits = track_PixelHits->at(i);
					new_track_TRTHits = track_TRTHits->at(i);
					new_track_PixeldEdX = track_PixeldEdX->at(i);
					newtree->Fill();	
				}	
			  }
			//}
		    }
	    }
	    if (isSignal){
		// Fill Histograms for everything before cuts 
		histEta->Fill(vSystem.Eta());
		
		histPhi->Fill(vSystem.Phi());

		histPt->Fill(vSystem.Pt());
		histMass->Fill(vSystem.M());
		histRapidity->Fill(vSystem.Rapidity());
		// Fill Histograms for electrons
                histEta->Fill(vSystem.Eta());
                
                histPhi->Fill(vSystem.Phi());

                histPt->Fill(vSystem.Pt());
                histMass->Fill(vSystem.M());
		histRapidity->Fill(vSystem.Rapidity());

                // Count all electrons and muons
                signalCount++;
	        }

	}
}   

newtree->Write();
delete newfile;


cout<< "Counts of all valid particles pairs after cuts: " << signalCount <<endl;
cout << "Number of electrons to new TTree: " << elecCount << endl; 
cout << "Number of muons: " << 2 * muCount << endl; 
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

//Draw electron histograms
eHistEta->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

eHistPhi->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

eHistPt->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

eHistMass->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

eHistRapidity->Draw();
c1->SaveAs("Plots/histAngles.pdf");

// Draw muon histograms
muHistEta->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

muHistPhi->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

muHistPt->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

muHistMass->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

muHistRapidity->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

// J/Psi Pt difference between e/mu 
eHistPt->Scale(1.0 / eHistPt->GetEntries()); // Normalize 
muHistPt->Scale(1.0 / muHistPt->GetEntries());

eHistPt->SetLineColor(kBlue);
muHistPt->SetLineColor(kMagenta);
muHistPt->Draw("HIST");
eHistPt->Draw("HIST SAME");
// 4. Create and Populate the Legend
TLegend *legend = new TLegend(0.7, 0.7, 0.95, 0.9);
legend->SetBorderSize(0);
legend->SetFillStyle(0);
legend->SetTextSize(0.035);
// The option "p" is sufficient to show the marker style in the legend
legend->AddEntry(eHistPt, "Electrons J/Psi Pt", "l"); 
legend->AddEntry(muHistPt, "Muons J/Psi Pt", "l");
legend->Draw();
c1->SaveAs("Plots/histAngles.pdf");
legend->Clear();
c1->Clear();

// J/Psi Pt difference between e/mu 
eHistPt->Scale(1.0 / eHistPt->GetEntries()); // Normalize 
muHistPt->Scale(1.0 / muHistPt->GetEntries());

eHistMass->SetLineColor(kBlue);
muHistMass->SetLineColor(kMagenta);
muHistMass->Draw("HIST");
eHistMass->Draw("HIST SAME");
// 4. Create and Populate the Legend
/*
TLegend *legend = new TLegend(0.7, 0.7, 0.95, 0.9);
legend->SetBorderSize(0);
legend->SetFillStyle(0);
legend->SetTextSize(0.035);
*/
// The option "p" is sufficient to show the marker style in the legend
legend->AddEntry(eHistMass, "Electrons J/Psi Mass", "l"); 
legend->AddEntry(muHistMass, "Muons J/Psi Mass", "l");
legend->Draw();
c1->SaveAs("Plots/histAngles.pdf");
c1->Clear();

c1->SaveAs("Plots/histAngles.pdf]");
c1->Clear();
c1->Close();
}

int main(){
AnalysisData23 t;
t.Loop();
return 0;
}
