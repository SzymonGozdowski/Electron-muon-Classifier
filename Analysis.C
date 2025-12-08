#define Analysis_cxx
#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
using namespace std;

void Analysis::Loop()
{
   //g++ -Wall -std=c++17 -I `root-config --incdir` -o analysis Analysis.C `root-config --libs` && ./analysis
   if (fChain == 0) return;

   float PixelHits;
   float PixelTRTHits;
   float PixeldEdX; 
   float PixelSCTHits; 
   bool  IsMuon;

   TFile *file = new TFile("MLData.root", "RECREATE");
   TTree *MLDataTree = new TTree("MLDataTree", "MLDataTree");

   MLDataTree->Branch("track_PixelHits", &PixelHits, "track_PixelHits/F");
   MLDataTree->Branch("track_TRTHits", &PixelTRTHits, "track_PixelTRTHits/F");
   MLDataTree->Branch("track_PixeldEdX", &PixeldEdX, "track_PixeldEdX/F");
   MLDataTree->Branch("track_SCTHits", &PixelSCTHits, "track_PixelSCTHits/F");
   MLDataTree->Branch("IsMuon", &IsMuon, "IsMuon/B");

   Long64_t nentries = fChain->GetEntriesFast();
   TH1D *Number = new TH1D("Number","Number",15,-0.5,14.5);
   TH1D *DeltaPhi = new TH1D("DeltaPhi","DeltaPhi",100,-3,3);
   TH1D *DeltaEta = new TH1D("DeltaEta","DeltaEta",100,-3,3);
   TH1D *HRadEtaPhi = new TH1D("RadEtaPhi","RadEtaPhi",100,-0.01,10);


   TH1D *DiMass = new TH1D("dimass","dimass",50,2.9,3.2);
   TH1D *Pt = new TH1D("Pt","Pt",50,0,0.2);
   TH1D *Rapidity = new TH1D("Rapidity ","Rapidity ",50,-3,3);

   TH1D *ElectronEta =new TH1D("ElectronEta","ElectronEta",50,-3,3);
   TH1D *ElectronEnergy =new TH1D("ElectronEnergy","ElectronEnergy",50,1,6);
   TH1D *ElectronPt =new TH1D("ElectronPt","ElectronPt",50,0.9,1.8);
   TH1D *ElectronPhi =new TH1D("ElectronPhi","ElectronPhi",30,-pi,pi);

   TH1D *MuonEta =new TH1D("MuonEta","MuonEta",50,-3,3);
   TH1D *MuonEnergy =new TH1D("MuonEnergy","MuonEnergy",50,1,6);
   TH1D *MuonPt =new TH1D("MuonPt","MuonPt",50,0.9,1.8);
   TH1D *MuonPhi =new TH1D("MuonPhi","MuonPhi",30,-pi,pi);


   TH1D *H_ElPixelHits =new TH1D("ElPixelHits","PixelHits",11,-0.5,10.5);
   TH1D *H_ElPixelTRTHits =new TH1D("ElPixelTRTHits","PixelTRTHits",51,-1,50);
   TH1D *H_ElPixeldEdX =new TH1D("ElPixeldEdX","PixeldEdX",100,0,2);
   TH1D *H_ElPixelSCTHits =new TH1D("ElPixelSCTHits","PixelSCTHits",12,2.5,14.5);

   TH1D *H_MuPixelHits =new TH1D("MuPixelHits","PixelHits",11,-0.5,10.5);
   TH1D *H_MuPixelTRTHits =new TH1D("MuPixelTRTHits","PixelTRTHits",51,-1,50);
   TH1D *H_MuPixeldEdX =new TH1D("MuPixeldEdX","PixeldEdX",100,0,2);
   TH1D *H_MuPixelSCTHits =new TH1D("MuPixelSCTHits","PixelSCTHits",12,2.5,14.5);

   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   //for (Long64_t jentry=0; jentry<300;jentry++) {
      if(jentry%10000==0) cout<<"Processing "<<jentry<<" event..."<<endl;
      int count=0;
      TLorentzVector Electron;
      vector<TLorentzVector> Muon(2);
      vector<TLorentzVector> t(2);
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      
      
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

               double RadEtaPhi;

               int c=0;
               bool elctroncheck=0;
               for(int i=0;i<eg_cluster_n;i++)
               {
                  //cout<<"========================="<<endl;
                  double deltaPhi=t[0].Phi()-eg_cluster_phi->at(i);
                  //cout<<"deltaPhi1   "<<deltaPhi<<" "<<jentry<<endl;
                  if(deltaPhi>3) deltaPhi=2*pi-deltaPhi;
                  if(deltaPhi<-3) deltaPhi=2*pi+deltaPhi;

                  //cout<<"deltaPhi2   "<<deltaPhi<<" "<<jentry<<endl;

                  DeltaEta->Fill(t[0].Eta()-eg_cluster_eta->at(i));
                  DeltaPhi->Fill(deltaPhi);
                  RadEtaPhi=sqrt(pow(t[0].Eta()-eg_cluster_eta->at(i),2)+pow(deltaPhi,2));
                  
                  double temp;
                  HRadEtaPhi->Fill(RadEtaPhi);
                  if(RadEtaPhi<0.5)
                  {
                     //cout<<"roznica  "<<t[0].Phi()-t[1].Phi()<<" "<<jentry<<endl;
                     if(c==0)
                     {
                        Electron=t[1];
                        temp=RadEtaPhi;
                     } 
                     if(c>0 && temp>RadEtaPhi)
                     {
                        Electron=t[1];
                        
                        temp=RadEtaPhi;
                     }
                     elctroncheck=1;
                     c++;
                  } 
               }
               
               Number->Fill(c);
               //if(c>1) cout<<"Error "<<c<<"  "<<jentry<<endl;
               
               if(elctroncheck)
               {
                  PixelHits=track_PixelHits->at(1);
                  PixelTRTHits=track_TRTHits->at(1);
                  PixeldEdX=track_PixeldEdX->at(1);
                  PixelSCTHits=track_SCTHits->at(1);
                  IsMuon=0;
                  MLDataTree->Fill();

                  H_ElPixeldEdX->Fill(track_PixeldEdX->at(1));
                  H_ElPixelHits->Fill(track_PixelHits->at(1));
                  H_ElPixelSCTHits->Fill(track_SCTHits->at(1));
                  H_ElPixelTRTHits->Fill(track_TRTHits->at(1));

                  ElectronEnergy->Fill(Electron.E());
                  ElectronEta->Fill(Electron.Eta());
                  ElectronPt->Fill(Electron.Pt());
                  ElectronPhi->Fill(Electron.Phi());
               }
               if((dipartic).M()>3 && !elctroncheck)
               {
                  for(int i=0;i<2;i++)
                  {
                     Muon[i]=t[i];
                     PixelHits=track_PixelHits->at(i);
                     PixelTRTHits=track_TRTHits->at(i);
                     PixeldEdX=track_PixeldEdX->at(i);
                     PixelSCTHits=track_SCTHits->at(i);
                     IsMuon=1;
                     MLDataTree->Fill();

                     H_MuPixeldEdX->Fill(track_PixeldEdX->at(i));
                     H_MuPixelHits->Fill(track_PixelHits->at(i));
                     H_MuPixelSCTHits->Fill(track_SCTHits->at(i));
                     H_MuPixelTRTHits->Fill(track_TRTHits->at(i));

                     MuonEnergy->Fill(Muon[i].E());
                     MuonEta->Fill(Muon[i].Eta());
                     MuonPt->Fill(Muon[i].Pt());
                     MuonPhi->Fill(Muon[i].Phi());
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
   c1.Divide(1,2);
   c1.cd(1);
   DeltaEta->Draw();
   c1.cd(2);
   DeltaPhi->Draw();
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
   ElectronPhi->SetMinimum(0);
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
   MuonPhi->SetMinimum(0);
   MuonPhi->Draw();
   c1.SaveAs("Plots/test.pdf");

   
   c1.Clear();
   c1.Divide(2,2);
   c1.cd(1);
   H_ElPixelHits->Scale(1./H_ElPixelHits->Integral());
   H_MuPixelHits->Scale(1./H_MuPixelHits->Integral());
   H_ElPixelHits->SetLineColor(kRed);
   H_MuPixelHits->SetLineColor(kBlue);
   H_ElPixelHits->Draw("HIST");
   H_MuPixelHits->Draw("HIST SAME");

   c1.cd(2);
   H_ElPixelTRTHits->Scale(1./H_ElPixelTRTHits->Integral());
   H_MuPixelTRTHits->Scale(1./H_MuPixelTRTHits->Integral());
   H_ElPixelTRTHits->SetLineColor(kRed);
   H_MuPixelTRTHits->SetLineColor(kBlue);
   H_ElPixelTRTHits->Draw("HIST");
   H_MuPixelTRTHits->Draw("HIST SAME");

   c1.cd(3);
   H_ElPixelSCTHits->Scale(1./H_ElPixelSCTHits->Integral());
   H_MuPixelSCTHits->Scale(1./H_MuPixelSCTHits->Integral());
   H_ElPixelSCTHits->SetLineColor(kRed);
   H_MuPixelSCTHits->SetLineColor(kBlue);
   H_ElPixelSCTHits->Draw("HIST");
   H_MuPixelSCTHits->Draw("HIST SAME");

   c1.cd(4);
   H_ElPixeldEdX->Scale(1./H_ElPixeldEdX->Integral());
   H_MuPixeldEdX->Scale(1./H_MuPixeldEdX->Integral());
   H_ElPixeldEdX->SetLineColor(kRed);
   H_MuPixeldEdX->SetLineColor(kBlue);
   H_ElPixeldEdX->Draw("HIST");
   H_MuPixeldEdX->Draw("HIST SAME");

   c1.SaveAs("Plots/test.pdf");


   c1.SaveAs("Plots/test.pdf]");
   
   MLDataTree->Write();
   file->Close();

}
int main()
{
   Analysis t;
   t.Loop();
   return 0;
}
