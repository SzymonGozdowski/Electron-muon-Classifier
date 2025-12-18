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
	std::vector<bool>* topo_cluster_pass_sig_cut = nullptr;
	std::vector<float>* topo_cluster_eta = nullptr;
	std::vector<float>* topo_cluster_phi = nullptr;
	std::vector<float>* topo_cluster_EM_prob = nullptr;
	std::vector<float>* topo_cluster_pt = nullptr;
	UInt_t topoclus_n;

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
	G2TauTree->SetBranchAddress("topo_cluster_pass_sig_cut", &topo_cluster_pass_sig_cut);
	G2TauTree->SetBranchAddress("topo_cluster_phi", &topo_cluster_phi);
	G2TauTree->SetBranchAddress("topo_cluster_eta", &topo_cluster_eta);
	G2TauTree->SetBranchAddress("topo_cluster_EM_prob", &topo_cluster_EM_prob);
	G2TauTree->SetBranchAddress("topoclus_n", &topoclus_n);
	G2TauTree->SetBranchAddress("topo_cluster_pt", &topo_cluster_pt);

	TH1F* eEta = new TH1F("eEta", "eEta", 100, -3, 3);
	TH1F* ePhi = new TH1F("ePhi", "ePhi", 100, -3, 3);
	TH1F* ePt = new TH1F("ePt", "ePt", 100, 0, 3);
	TH1F* eRapidity = new TH1F("eRapidity", "eRapidity", 100, -3, 3);
	TH1F* eInvarMass = new TH1F("eInvarMass", "eInvarMass", 100, 0, 5);
	TH1F* ef = new TH1F("eEMProb", "eEMProb", 100, 0, 5);
	TH1F* eTopoClusterN = new TH1F("eTopoClusterN", "eTopoClusterN", 100, 0, 40);

	TH1F* muEta = new TH1F("muEta", "muEta", 100, -3, 3);
	TH1F* muPhi = new TH1F("muPhi", "muPhi", 100, -3, 3);
	TH1F* muPt = new TH1F("muPt", "muPt", 100, 0, 3);
	TH1F* muRapidity = new TH1F("muRapidity", "muRapidity", 100, -3, 3);
	TH1F* muInvarMass = new TH1F("muInvarMass", "muInvarMass", 100, 0, 5);
	TH1F* muf = new TH1F("muf", "muf", 100, 0, 5);
	TH1F* muTopoClusterN = new TH1F("muTopoClusterN", "muTopoClusterN", 100, 0, 40);

	//helpful stuff
	TLorentzVector Lv1, Lv2, LvSum;
	const float muonMass=0.10566;
	const float elecMass=0.000511;
	int numElec=0;
	int numMuon=0;
	Long64_t num_Entries = G2TauTree->GetEntries();
	ofstream myFile("TRT_Pixel_SCT_dEdX_f_EmProb_particle.csv");
	float f=0;

	//Brother, May i have some L00ps?
	for(Long64_t i=0; i<num_Entries; i++)
	{
		G2TauTree->GetEntry(i);

		float minDeltaR=100;
		if (topoclus_n < 1) continue;
		float topoClusterPtMax = (*topo_cluster_pt)[0];
		float LeadingTopoClusterEMProb = (*topo_cluster_EM_prob)[0];
		
		if (i % (num_Entries/100) == 0) 
		{
        		double progress = 100.0 * i / num_Entries;
        		std::cout << "\rProgress: " << (int)progress << "%" <<std::flush;
    	}
		
		if(track_n!=2 || (*track_charge)[0]==(*track_charge)[1]) continue; //first batch of filtering
		if(std::abs((*track_eta)[1])>=2.5 || std::abs((*track_eta)[0])>=2.5) continue;
		if((*track_pt)[1]<=0.1 || (*track_pt)[0]<=0.1) continue;
		if (!(*topo_cluster_pass_sig_cut)[0] || !(*topo_cluster_pass_sig_cut)[1]) continue;

		if(eg_cluster_n>=1) //logic for electrons
		{
			for(UInt_t j=0; j<topoclus_n; j++)
			{
				float deltaR = sqrt(pow(((*track_eta)[0] - (*topo_cluster_eta)[j]), 2) + pow((acos(cos((*track_phi)[0])) - acos(cos((*topo_cluster_phi)[j]))), 2));
				if(minDeltaR>deltaR)
				{
					minDeltaR=deltaR;
				}
			}

			for (UInt_t k = 0; k < topoclus_n; k++)
			{
				if ((*topo_cluster_pt)[k] > topoClusterPtMax)
				{
					topoClusterPtMax = (*topo_cluster_pt)[k];
					LeadingTopoClusterEMProb = (*topo_cluster_EM_prob)[k];
				}
			}

			if (minDeltaR < 0.5)
			{
				Lv1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0], (*track_phi)[0], elecMass);
				Lv2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1], (*track_phi)[1], elecMass);
				LvSum = Lv1 + Lv2;
				if (LvSum.Pt() > 0.2 || LvSum.M() > 3) continue;
				numElec++;

				f = topoClusterPtMax / (*track_pt)[1];

				ef->Fill(f);

				myFile << (*track_TRTHits)[1] << ';' << (*track_PixelHits)[1] << ';' << (*track_SCTHits)[1] << ';' << (*track_PixeldEdX)[1] <<';'<< f << ';'<< LeadingTopoClusterEMProb <<';' << "electron" << '\n';

				eEta->Fill(LvSum.Eta());
				ePhi->Fill(LvSum.Phi());
				ePt->Fill(LvSum.Pt());
				eInvarMass->Fill(LvSum.M());
				eRapidity->Fill(LvSum.Rapidity());
				eTopoClusterN->Fill(topoclus_n);
			}
		}
		else //logic for muons
		{
			Lv1.SetPtEtaPhiM((*track_pt)[0], (*track_eta)[0], (*track_phi)[0], muonMass);
			Lv2.SetPtEtaPhiM((*track_pt)[1], (*track_eta)[1], (*track_phi)[1], muonMass);
			LvSum=Lv1+Lv2;
			if(LvSum.M()>3.2 || LvSum.M()<2.9 || LvSum.Pt()>0.2) continue;
			numMuon++;

			f = topoClusterPtMax / (*track_pt)[0];
			muf->Fill(f);
			myFile<<(*track_TRTHits)[0]<<';'<<(*track_PixelHits)[0]<<';'<<(*track_SCTHits)[0]<<';'<<(*track_PixeldEdX)[0]<< ';' << f<<';'<< LeadingTopoClusterEMProb << ';'<<"muon"<<'\n';
			f = topoClusterPtMax / (*track_pt)[1];
			muf->Fill(f);
			myFile<<(*track_TRTHits)[1]<<';'<<(*track_PixelHits)[1]<<';'<<(*track_SCTHits)[1]<<';'<<(*track_PixeldEdX)[1]<< ';' << f<<';'<< LeadingTopoClusterEMProb << ';'<<"muon"<<'\n';

			muEta->Fill(LvSum.Eta());
			muPhi->Fill(LvSum.Phi());
			muPt->Fill(LvSum.Pt());
			muInvarMass->Fill(LvSum.M());
			muRapidity->Fill(LvSum.Rapidity());
			muTopoClusterN->Fill(topoclus_n);
		}
	}
	myFile.close();
	std::cout << '\n' << "Number of electrons:" << numElec << '\n';
	std::cout << "Number of muons:" << numMuon * 2 << '\n';

	TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);

	eEta->Draw();
	c1->SaveAs("Plots/ElectronEtaHistogram.pdf");
	c1->Clear();

	ePhi->Draw();
	c1->SaveAs("Plots/ElectronPhiHistogram.pdf");
	c1->Clear();

	ePt->Draw();
	c1->SaveAs("Plots/ElectronPtHistogram.pdf");
	c1->Clear();

	eInvarMass->Draw();
	c1->SaveAs("Plots/ElectronInvariantMassHistogram.pdf");
	c1->Clear();

	eRapidity->Draw();
	c1->SaveAs("Plots/ElectronRapidityHistogram.pdf");
	c1->Clear();

	ef->Draw();
	c1->SaveAs("Plots/ElectronfHistogram.pdf");
	c1->Clear();

	muEta->Draw();
	c1->SaveAs("Plots/muonEtaHistogram.pdf");
	c1->Clear();

	muPhi->Draw();
	c1->SaveAs("Plots/muonPhiHistogram.pdf");
	c1->Clear();

	muPt->Draw();
	c1->SaveAs("Plots/muonPtHistogram.pdf");
	c1->Clear();

	muInvarMass->Draw();
	c1->SaveAs("Plots/muonInvariantMassHistogram.pdf");
	c1->Clear();

	muRapidity->Draw();
	c1->SaveAs("Plots/muonRapidityHistogram.pdf");
	c1->Clear();

	muf->Draw();
	c1->SaveAs("Plots/muonfHistogram.pdf");
	c1->Clear();

	eTopoClusterN->SetLineColor(kRed);
	muTopoClusterN->SetLineColor(kBlue);
	eTopoClusterN->Draw();
	muTopoClusterN->Draw("SAME");
	c1->SaveAs("Plots/electronMuonTopoClusterNHistogram.pdf");
	c1->Clear();
	c1->Close();
}