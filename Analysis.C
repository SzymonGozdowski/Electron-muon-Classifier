#define Analysis_cxx
#include "MyAnalysis.h"
#include <TStyle.h>
#include <TCanvas.h>
using namespace std;
void Analysis::Topocluster(TLorentzVector particle, TH1D* Count, TH1D* Perp, TH1D* FVarible, TH1D* EMCal)
{
   int topo_count=0;
   double pt=5;
   int id=-1;

   for(int topo=0;topo<int(topoclus_n);topo++)
   {

      //if(!topo_cluster_pass_sig_cut) continue;

      double deltaPhi=acos(cos(particle.Phi()))-acos(cos(topo_cluster_phi->at(topo)));      
      double RadEtaPhi=sqrt(pow(particle.Eta()-topo_cluster_eta->at(topo),2)+pow(deltaPhi,2));
      
      if(RadEtaPhi<0.5 && topo_cluster_pass_sig_cut)
      {
         topo_count++;   
         double temp_pt=topo_cluster_pt->at(topo);
         Perp->Fill(temp_pt);
         if(pt>temp_pt)
         {
            pt=temp_pt;
            id=topo;
         }
      }          
   }
   Count->Fill(topo_count);
   
   if(id!=-1)
   {
      FVarible->Fill(topo_cluster_pt->at(id)/particle.Perp());
      EMCal->Fill(topo_cluster_EM_prob->at(id));
   }
}

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
   TH1D *Number = new TH1D("Number","Number",6,-0.5,5.5);
   TH1D *DeltaPhi = new TH1D("DeltaPhi","DeltaPhi",100,0,3.14);
   TH1D *DeltaEta = new TH1D("DeltaEta","DeltaEta",100,-3,3);
   TH1D *HRadEtaPhi = new TH1D("RadEtaPhi","RadEtaPhi",100,0,4);
   
   TH1D *CountMuon = new TH1D("CountMuon","CountMuon",10,-0.5,9.5);
   TH1D *CountElec = new TH1D("CountElec","CountElec",10,-0.5,9.5);
   TH1D *PerpMuon = new TH1D("PerpMuon","PerpMuon",100,0,3.5);
   TH1D *PerpElec = new TH1D("PerpElec","PerpElec",100,0,3.5);
   TH1D *FMuon = new TH1D("FMuon","FMuon",100,0,4);
   TH1D *FElec = new TH1D("FElec","FElec",100,0,4);
   TH1D *EMMuon = new TH1D("EMMuon","EMMuon",40,-0.1,1.1);
   TH1D *EMElec = new TH1D("EMElec","EMElec",40,-0.1,1.1);
   

   TH1D *DiMass = new TH1D("dimass","dimass",50,2.9,3.2);
   TH1D *DiMassEl = new TH1D("dimassEl","dimassEl",50,1.5,3.9);
   TH1D *DiMassMu = new TH1D("dimassMu","dimassMu",50,1.5,3.9);

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
      if(jentry%100000==0) cout<<"Processing "<<jentry<<" event..."<<endl;
      TLorentzVector Electron;
      vector<TLorentzVector> Muon(2);
      vector<TLorentzVector> t(2);
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      int count=0;

      if(track_n!=2) continue;
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
         
         if(dipartic.Perp()<0.2 && dipartic.M()<3.2)
         {
            if(dipartic.M()>2.9)
            {
               Pt->Fill(dipartic.Perp());
               Rapidity->Fill(dipartic.Rapidity());
               DiMass->Fill(dipartic.M());
            }
            int c=0;
            bool elctroncheck=0;
            for(int i=0;i<int(eg_cluster_n);i++)
            {
               double RadEtaPhi;
               double deltaPhi=acos(cos(t[0].Phi()))-acos(cos(eg_cluster_phi->at(i)));
               DeltaEta->Fill(t[0].Eta()-eg_cluster_eta->at(i));
               DeltaPhi->Fill(deltaPhi);
               RadEtaPhi=sqrt(pow(t[0].Eta()-eg_cluster_eta->at(i),2)+pow(deltaPhi,2));
               HRadEtaPhi->Fill(RadEtaPhi);
               
               double temp=0;
               if(RadEtaPhi<0.5)
               {
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
            
            //if(c>1) cout<<"Error "<<c<<"  "<<jentry<<endl;
            
            if(elctroncheck && dipartic.M()<3)
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
               DiMassEl->Fill(dipartic.M());
               Topocluster(Electron,CountElec,PerpElec,FElec,EMElec);

            }
            if(!elctroncheck && dipartic.M()>2.9 )
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
                  Topocluster(Muon[i],CountMuon,PerpMuon,FMuon,EMMuon);
               }
               DiMassMu->Fill(dipartic.M());
            }              
         }
      }
   }

   TCanvas c1;
   
   c1.SaveAs("Plots/test.pdf[");

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
   DiMassEl->SetLineColor(kBlue);
   DiMassMu->SetLineColor(kRed);
   DiMassMu->Draw();
   DiMassEl->Draw("same");
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

   c1.Clear();
   c1.Divide(2,2);
   c1.cd(1);
   CountElec->Scale(1./CountElec->Integral());
   CountMuon->Scale(1./CountMuon->Integral());
   CountElec->SetLineColor(kBlue);
   CountMuon->SetLineColor(kRed);
   CountMuon->Draw("HIST");
   CountElec->Draw("HIST SAME");

   c1.cd(2);
   PerpElec->Scale(1./PerpElec->Integral());
   PerpMuon->Scale(1./PerpMuon->Integral());
   PerpElec->SetLineColor(kBlue);
   PerpMuon->SetLineColor(kRed);
   PerpElec->Draw("HIST");
   PerpMuon->Draw("HIST SAME");

   c1.cd(3);
   FElec->Scale(1./FElec->Integral());
   FMuon->Scale(1./FMuon->Integral());
   FElec->SetLineColor(kBlue);
   FMuon->SetLineColor(kRed);
   FMuon->Draw("HIST");
   FElec->Draw("HIST SAME");

   c1.cd(4);
   EMElec->Scale(1./EMElec->Integral());
   EMMuon->Scale(1./EMMuon->Integral());
   EMElec->SetLineColor(kBlue);
   EMMuon->SetLineColor(kRed);
   EMMuon->Draw("HIST");
   EMElec->Draw("HIST SAME");
   c1.SaveAs("Plots/test.pdf");

   c1.Clear();
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

   c1.SaveAs("Plots/test.pdf]");
   
   MLDataTree->Write();
   file->Close();
   cout<<"Completed analysis of "<<nentries<<" events"<<endl;
}
int main()
{
   gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");
   Analysis t;
   t.Loop();
   return 0;
}
