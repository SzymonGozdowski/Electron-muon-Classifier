#define Analysis_cxx
#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
using namespace std;

void Analysis::Loop()
{

   if (fChain == 0) return;

   float PixelHits;
   float PixelTRTHits;
   float PixeldEdX;
   bool  IsMuon;

   TFile *file = new TFile("MLData.root", "RECREATE");
   TTree *MLDataTree = new TTree("MLDataTree", "MLDataTree");

   MLDataTree->Branch("track_PixelHits", &PixelHits, "track_PixelHits/F");
   MLDataTree->Branch("track_TRTHits", &PixelTRTHits, "track_PixelTRTHits/F");
   MLDataTree->Branch("track_PixeldEdX", &PixeldEdX, "track_PixeldEdX/F");
   MLDataTree->Branch("IsMuon", &IsMuon, "IsMuon/B");


   Long64_t nentries = fChain->GetEntriesFast();
   TH1D *Number = new TH1D("Number","Number",15,-0.5,14.5);
   TH1D *HRadEtaPhi = new TH1D("RadEtaPhi","RadEtaPhi",100,-0.01,0.1);


   TH1D *DiMass = new TH1D("dimass","dimass",50,2.9,3.2);
   TH1D *Pt = new TH1D("Pt","Pt",50,0,0.2);
   TH1D *Rapidity = new TH1D("Rapidity ","Rapidity ",50,-3,3);

   TH1D *ElectronEta =new TH1D("ElectronEta","ElectronEta",50,-3,3);
   TH1D *ElectronEnergy =new TH1D("ElectronEnergy","ElectronEnergy",50,1,6);
   TH1D *ElectronPt =new TH1D("ElectronPt","ElectronPt",50,0.5,1.8);
   TH1D *ElectronPhi =new TH1D("ElectronPhi","ElectronPhi",50,-pi-0.1,pi+0.1);

   TH1D *MuonEta =new TH1D("MuonEta","MuonEta",50,-3,3);
   TH1D *MuonEnergy =new TH1D("MuonEnergy","MuonEnergy",50,1,6);
   TH1D *MuonPt =new TH1D("MuonPt","MuonPt",50,0.5,1.8);
   TH1D *MuonPhi =new TH1D("MuonPhi","MuonPhi",50,-pi-0.1,pi+0.1);
   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   //for (Long64_t jentry=0; jentry<1000;jentry++) {
      if(jentry%10000==0) cout<<"Processing "<<jentry<<" event..."<<endl;
      int count=0;
      float ElEta=0,ElPhi=0;
      TLorentzVector Electron;
      vector<TLorentzVector> Muon(2);
      vector<TLorentzVector> t(2);
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      Number->Fill(nElec);
      if(nElec==2)
      {
         ElEta=electron_eta->at(0);
         ElPhi=electron_phi->at(0);
      }
      
      
      if(track_n==2)
      {
         for(int track=0;track<int(track_n);track++)
         {
            
            if(abs(track_eta->at(track))<2.5 && track_pt->at(track)>0.1)
            {
               t[track].SetPtEtaPhiM(track_pt->at(track),track_eta->at(track),track_phi->at(track),MuonMass);
               count++;
               
            }
            else break;
         }
         
         if(count==2 && track_charge->at(0)!=track_charge->at(1))
         {
            
            TLorentzVector dipartic;
            dipartic=t[0]+t[1];
            
            if((dipartic).Perp()<0.2 && (dipartic).M()<3.2 && (dipartic).M()>2.9)
            {
               Pt->Fill((dipartic).Perp());
               Rapidity->Fill(dipartic.Rapidity());
               DiMass->Fill((dipartic).M());

               vector<double> RadEtaPhi(2);

               if((dipartic).M()>3.05)
               {
                  for(int i=0;i<2;i++)
                  {
                     Muon[i]=t[i];
                     PixelHits=track_PixelHits->at(i);
                     PixelTRTHits=track_TRTHits->at(i);
                     PixeldEdX=track_PixeldEdX->at(i);
                     IsMuon=1;
                     MLDataTree->Fill();
                     MuonEnergy->Fill(Muon[i].E());
                     MuonEta->Fill(Muon[i].Eta());
                     MuonPt->Fill(Muon[i].Pt());
                     MuonPhi->Fill(Muon[i].Phi());
                  }
               }
               else
               {
                  for(int i=0;i<2;i++) RadEtaPhi[i]=sqrt(pow(t[i].Eta()-ElEta,2)+pow(t[i].Phi()-ElPhi,2));
                  int index;
                  if(RadEtaPhi[0]<RadEtaPhi[1])
                  {
                     if(RadEtaPhi[0]<0.1) Electron=t[1];
                     HRadEtaPhi->Fill(RadEtaPhi[0]);
                     index=1;
                  }
                  else
                  {
                     if(RadEtaPhi[1]<0.1) Electron=t[0];
                     HRadEtaPhi->Fill(RadEtaPhi[1]);
                     index=0;
                  } 
                  if(Electron.E()!=0)
                  {
                     PixelHits=track_PixelHits->at(index);
                     PixelTRTHits=track_TRTHits->at(index);
                     PixeldEdX=track_PixeldEdX->at(index);
                     IsMuon=0;
                     MLDataTree->Fill();

                     ElectronEnergy->Fill(Electron.E());
                     ElectronEta->Fill(Electron.Eta());
                     ElectronPt->Fill(Electron.Pt());
                     ElectronPhi->Fill(Electron.Phi());
                  }
               }
               
            }

         }
      
      }     
   }

   TCanvas c1;
   
   c1.SaveAs("Plots/test.pdf[");

   Number->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   HRadEtaPhi->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   DiMass->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   Pt->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   Rapidity->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   c1.Divide(2,2);
   c1.cd(1);
   ElectronEnergy->Draw();
   c1.cd(2);
   ElectronEta->Draw();
   c1.cd(3);
   ElectronPt->Draw();
   c1.cd(4);
   ElectronPhi->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
   c1.Divide(2,2);
   c1.cd(1);
   MuonEnergy->Draw();
   c1.cd(2);
   MuonEta->Draw();
   c1.cd(3);
   MuonPt->Draw();
   c1.cd(4);
   MuonPhi->Draw();
   c1.SaveAs("Plots/test.pdf");

   c1.SaveAs("Plots/test.pdf]");
   
   MLDataTree->Write();
   file->Close();



}
int main()
{
   Analysis t;
   t.Loop();
}
