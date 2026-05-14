#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
#include <string>
#include <TLegend.h>
#include <vector>
#include <TH1.h> 

void Topocluster(TLorentzVector particle,TTreeReaderArray<unsigned int>& TopoCluNum, TTreeReaderArray<float>& TopoCluEta,TTreeReaderArray<float>& TopoCluPhi, TTreeReaderArray<float>& TopoCluPt,
       TTreeReaderArray<float>& TopoCluLamda, TTreeReaderArray<float>& TopoCluLamda2, TTreeReaderArray<float>& TopoCluR2, TTreeReaderArray<float>& TopoCluEMProb, TTreeReaderArray<bool>& TopoCluPass, 
       TTreeReaderArray<int>& TopoCellSig, TH1D* Count, TH1D* Perp, TH1D* FVariableHist, TH1D* EMCal, TH1D* LambdaHist, TH1D* Lambda2Hist,   TH1D* RadiusHist, TH1D* CellSig, float &FVariable, float &EMprop,
       float &Lambda2, float &Lambda, float &Radius, bool &muoncheck)
{
   //========================
   //Getting Topocluster informtions and searching for matches 
   //========================
    
    

   int topo_count=0;
   double pt=0;
   int id=-1;
   
   for(int topo=0;topo<int(TopoCluNum[0]);topo++)
   {

      //if(!topo_cluster_pass_sig_cut) continue;

      double deltaPhi=acos(cos(particle.Phi()-TopoCluPhi[topo]));      
      double RadEtaPhi=sqrt(pow(particle.Eta()-TopoCluEta[topo],2)+pow(deltaPhi,2));
      
      if(RadEtaPhi<0.5 && TopoCluPass[topo])
      {
         topo_count++;   
         double temp_pt=TopoCluPt[topo];
         
         if(pt<temp_pt)
         {
            pt=temp_pt;
            id=topo;

         }
      }          
   }
   Count->Fill(topo_count);

   //========================
   //Saving topocluster data
   //========================

   if(id!=-1)
   {
      Perp->Fill(pt);
      FVariable=TopoCluPt[id]/particle.Perp();
      EMprop=TopoCluEMProb[id];
      Lambda=TopoCluLamda[id];
      Lambda2=TopoCluLamda2[id];
      Radius=TopoCluR2[id];
      if(TopoCellSig[id]>7) muoncheck=1;
      CellSig->Fill(TopoCellSig[id]);
      FVariableHist->Fill(FVariable);
      EMCal->Fill(EMprop);
      LambdaHist->Fill(Lambda/1000);
      Lambda2Hist->Fill(Lambda2/1000);
      RadiusHist->Fill(Radius/1000);
   }
   else 
   {
      FVariable=-1;
      EMprop=-1;
      Lambda=-1;
      Lambda2=-1;
      Radius=-1;
   }
}