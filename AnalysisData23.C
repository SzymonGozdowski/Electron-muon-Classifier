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
//   In a ROOT session, you can do:
//      root> .L AnalysisData23.C
//      root> AnalysisData23 t
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
      
      // if (Cut(ientry) < 0) continue;
      // Conditions to choose entries
      if(track_n ==  2 && track_charge->at(0) != track_charge->at(1)){

        for(int i=0; i<track_eta->size(); ++i){
          histEta->Fill(track_eta->at(i));
	  histPhi->Fill(track_phi->at(i));

          vector<TLorentzVector> vec(2);

	  for(int track=0; track<2; ++track){
		if(track_eta->at(track) < 2.5 && track_pt->at(track) ){
			vec[track].SetPtEtaPhiM(
track_pt->at(track), 
track_eta->at(track), 
track_phi->at(track), 
muMass);
		}	
	  }
	  TLorentzVector vec3 = vec[0] + vec[1];
	  histPt->Fill(vec3.Pt());
	  histMass->Fill(vec3.M());
	  if(vec3.M() > 2.9 && vec3.M() <3.2){massCount++;}	

} 

      }
   }

  cout<< "Counts of mass between (2.9; 3.2)GeV: " << massCount <<endl; 
   TCanvas *c1 = new TCanvas("c1", "Histograms", 1200, 400);
   //c1->SaveAs("Plots/test.pdf[");

   c1->Divide(3,1);
   c1->cd(1); histEta->Draw();

   c1->cd(2); histPhi->Draw(); 

   c1->cd(3); histPt->Draw();

   c1->SaveAs("Plots/test.pdf");

// I want to have mass histogram in another file
   TCanvas *c2 = new TCanvas("c2", "Mass histograms" ,100, 100);
   histMass->Draw();
   c2->SaveAs("Plots/histMass.pdf");
}

int main(){
AnalysisData23 t;
t.Loop();
return 0;
}
