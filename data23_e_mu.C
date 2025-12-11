#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include<iostream>
#include<vector>
#include<memory>
#include<cmath>
#include<fstream>

void data23_e_mu()
{
	std::unique_ptr<TFile> tree_File(TFile::Open("/home/feliks//ROOT_pliki/data23_2trk_moreTCvars.root"));
	TTree* G2TauTree = (TTree*)tree_File->Get("G2TauTree");

	UInt_t track_n;
	std::vector<int>* track_charge=nullptr;
	std::vector<float>* track_pt=nullptr;
	std::vector<float>* track_eta=nullptr;
	std::vector<float>* track_phi=nullptr;
	UInt_t eg_cluster_n;
	std::vector<float>* eg_cluster_eta=nullptr;
	std::vector<float>* eg_cluster_phi=nullptr;
	std::vector<float>* track_TRTHits=nullptr;
	std::vector<float>* track_PixelHits=nullptr;
	std::vector<float>* track_SCTHits=nullptr;
	std::vector<int>* track_PixeldEdX=nullptr;

	G2TauTree->SetBranchAddress("track_n", &track_n);
	G2TauTree->SetBranchAddress("track_charge", &track_charge);
	G2TauTree->SetBranchAddress("track_pt", &track_pt);
	G2TauTree->SetBranchAddress("track_eta", &track_eta);
	G2TauTree->SetBranchAddress("track_phi", &track_phi);
	G2TauTree->SetBranchAddress("eg_cluster_n", &eg_cluster_n);
	G2TauTree->SetBranchAddress("eg_cluster_eta", &eg_cluster_eta);
	G2TauTree->SetBranchAddress("eg_cluster_phi", &eg_cluster_phi);
	G2TauTree->SetBranchAddress("track_TRTHits", &track_TRTHits);
	G2TauTree->SetBranchAddress("track_PixelHits", &track_PixelHits);
	G2TauTree->SetBranchAddress("track_SCTHits", &track_SCTHits);
	G2TauTree->SetBranchAddress("track_PixeldEdX", &track_PixeldEdX);

	TH1F* eEta = new TH1F("eEta", "eEta", 100, -3, 3);
	TH1F* ePhi = new TH1F("ePhi", "ePhi", 100, -3, 3);
	TH1F* ePt = new TH1F("ePt", "ePt", 100, 0, 3);
	TH1F* eRapidity = new TH1F("eRapidity", "eRapidity", 100, -3, 3);
	TH1F* eInvarMass = new TH1F("eInvarMass", "eInvarMass", 100, 0, 5);

	TH1F* muEta = new TH1F("muEta", "muEta", 100, -3, 3);
	TH1F* muPhi = new TH1F("muPhi", "muPhi", 100, -3, 3);
	TH1F* muPt = new TH1F("muPt", "muPt", 100, 0, 3);
	TH1F* muRapidity = new TH1F("muRapidity", "muRapidity", 100, -3, 3);
	TH1F* muInvarMass = new TH1F("muInvarMass", "muInvarMass", 100, 0, 5);

	TH1F* bothEta = new TH1F("bothEta", "bothEta", 100, -3, 3);
	TH1F* bothPhi = new TH1F("bothPhi", "bothPhi", 100, -3, 3);
	TH1F* bothPt = new TH1F("bothPt", "bothPt", 100, 0, 3);
	TH1F* bothRapidity = new TH1F("bothRapidity", "bothRapidity", 100, -3, 3);
	TH1F* bothInvarMass = new TH1F("bothInvarMass", "bothInvarMass", 100, 0, 5);

	//helpful stuff
	TLorentzVector Lv1, Lv2, LvSum;
	const float muonMass=0.10566;
	const float elecMass=0.000511;
	int numElec=0;
	int numMuon=0;
	Long64_t num_Entries = G2TauTree->GetEntries();
	ofstream myFile("TRT_Pixel_SCT_dEdX_particle.csv");

	//Brother, May i have some L00ps?
	for(Long64_t i=0; i<num_Entries; i++)
	{
		G2TauTree->GetEntry(i); //progress report
		float minDeltaR=100;

		if (i % (num_Entries/100) == 0) 
		{
        		double progress = 100.0 * i / num_Entries;
        		std::cout << "\rProgress: " << (int)progress << "%" <<std::flush;
    	}
		
		if(track_n!=2 || (*track_charge)[0]==(*track_charge)[1]) continue; //first batch of filtering
		if(std::abs((*track_eta)[1])>=2.5 || std::abs((*track_eta)[0])>=2.5) continue;
		if((*track_pt)[1]<=0.1 || (*track_pt)[0]<=0.1) continue;

		if(eg_cluster_n>=1) //logic for electrons
		{
			for(UInt_t j=0; j<eg_cluster_n; j++)
			{
				float deltaR=sqrt(pow(((*track_eta)[0]-(*eg_cluster_eta)[j]), 2)+pow(((*track_phi)[0]-(*eg_cluster_phi)[j]), 2));

				if(minDeltaR>deltaR)
				{
					minDeltaR=deltaR;
				}
			}
			if(minDeltaR<0.5)
			{
			Lv1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0], (*track_phi)[0], elecMass);
			Lv2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1], (*track_phi)[1], elecMass);
			LvSum=Lv1+Lv2;
			if(LvSum.Pt()>0.2 || LvSum.M()>3) continue;
			numElec++;

			myFile<<(*track_TRTHits)[1]<<';'<<(*track_PixelHits)[1]<<';'<<(*track_SCTHits)[1]<<';'<<(*track_PixeldEdX)[1]<<';'<<"electron"<<'\n';

			eEta->Fill(LvSum.Eta());
			ePhi->Fill(LvSum.Phi());
			ePt->Fill(LvSum.Pt());
			eInvarMass->Fill(LvSum.M());
			eRapidity->Fill(LvSum.Rapidity());
			}
		}
		else //logic for muons
		{
			Lv1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0], (*track_phi)[0], muonMass);
			Lv2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1], (*track_phi)[1], muonMass);
			LvSum=Lv1+Lv2;
			if(LvSum.M()>3.2 || LvSum.M()<2.9 || LvSum.Pt()>0.2) continue;
			numMuon++;

			myFile<<(*track_TRTHits)[0]<<';'<<(*track_PixelHits)[0]<<';'<<(*track_SCTHits)[0]<<';'<<(*track_PixeldEdX)[0]<<';'<<"muon"<<'\n';
			myFile<<(*track_TRTHits)[1]<<';'<<(*track_PixelHits)[1]<<';'<<(*track_SCTHits)[1]<<';'<<(*track_PixeldEdX)[1]<<';'<<"muon"<<'\n';

			muEta->Fill(LvSum.Eta());
			muPhi->Fill(LvSum.Phi());
			muPt->Fill(LvSum.Pt());
			muInvarMass->Fill(LvSum.M());
			muRapidity->Fill(LvSum.Rapidity());
		}
		bothEta->Fill(LvSum.Eta());
		bothPhi->Fill(LvSum.Phi());
		bothPt->Fill(LvSum.Pt());
		bothInvarMass->Fill(LvSum.M());
		bothRapidity->Fill(LvSum.Rapidity());
	}
	myFile.close();
	std::cout << "Number of electrons:" << numElec << '\n';
	std::cout << "Number of muons:" << numMuon * 2 << '\n';

	TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);

	eEta->Draw();
	c1->SaveAs("ElectronEtaHistogram.pdf");
	c1->Clear();

	ePhi->Draw();
	c1->SaveAs("ElectronPhiHistogram.pdf");
	c1->Clear();

	ePt->Draw();
	c1->SaveAs("ElectronPtHistogram.pdf");
	c1->Clear();

	eInvarMass->Draw();
	c1->SaveAs("ElectronInvariantMassHistogram.pdf");
	c1->Clear();

	eRapidity->Draw();
	c1->SaveAs("ElectronRapidityHistogram.pdf");
	c1->Clear();

	muEta->Draw();
	c1->SaveAs("muonEtaHistogram.pdf");
	c1->Clear();

	muPhi->Draw();
	c1->SaveAs("muonPhiHistogram.pdf");
	c1->Clear();

	muPt->Draw();
	c1->SaveAs("muonPtHistogram.pdf");
	c1->Clear();

	muInvarMass->Draw();
	c1->SaveAs("muonInvariantMassHistogram.pdf");
	c1->Clear();

	muRapidity->Draw();
	c1->SaveAs("muonRapidityHistogram.pdf");
	c1->Clear();

	bothEta->Draw();
	c1->SaveAs("bothEtaHistogram.pdf");
	c1->Clear();

	bothPhi->Draw();
	c1->SaveAs("bothPhiHistogram.pdf");
	c1->Clear();

	bothPt->Draw();
	c1->SaveAs("bothPtHistogram.pdf");
	c1->Clear();
	
	bothInvarMass->Draw();
	c1->SaveAs("bothInvariantMassHistogram.pdf");
	c1->Clear();

	bothRapidity->Draw();
	c1->SaveAs("bothRapidityHistogram.pdf");
	c1->Clear();
}