#include <onnxruntime_cxx_api.h>
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
#include "../Topoclusters/TopoclusterHist.C"


void MCProton()
{

    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");
    //gStyle->SetOptStat(0);

    const Float_t ProtonMass    = 0.93827208816; 
    const Float_t MuonMass    = 0.1056583755;
    const Float_t ElectronMass = 0.00051099895;
    Double_t pi=TMath::Pi();
    double DEG=180/TMath::Pi();

    
    TString File="../Data/starlight_jpsiall_pptruth.root";
    string name="MCProton";

    //========================
    //Creating output data file
    //========================
    TFile *outputFile = new TFile(Form("Plots/%s_hist.root", name.c_str()), "RECREATE");

    float PixelHits;
    float PixelTRTHits;
    float PixeldEdX; 
    float PixelSCTHits; 
    float FVariable;
    float EMprop;
    float Lambda;
    float Lambda2;
    float Radius;
    float EtaRange;
    bool  IsProton;

    TFile *dataFile = new TFile(Form("../Data/MLData%s.root",name.c_str()), "RECREATE");
    TTree *MLDataTree = new TTree("MLDataTree", "MLDataTree");

    MLDataTree->Branch("track_PixelHits", &PixelHits, "track_PixelHits/F");
    MLDataTree->Branch("track_TRTHits", &PixelTRTHits, "track_PixelTRTHits/F");
    MLDataTree->Branch("track_PixeldEdX", &PixeldEdX, "track_PixeldEdX/F");
    MLDataTree->Branch("track_SCTHits", &PixelSCTHits, "track_PixelSCTHits/F");
    MLDataTree->Branch("Cal_FVariable", &FVariable, "Cal_FVariable/F");
    MLDataTree->Branch("Cal_EMprop", &EMprop, "Cal_EMprop/F");
    MLDataTree->Branch("Cal_Lambda", &Lambda, "Cal_Lambda/F");
    MLDataTree->Branch("Cal_Lambda2", &Lambda2, "Cal_Lambda2/F");
    MLDataTree->Branch("Cal_Radius", &Radius, "Cal_Radius/F");
    MLDataTree->Branch("EtaRange", &EtaRange, "EtaRange/F");

    MLDataTree->Branch("IsProton", &IsProton, "IsProton/B");


    //========================
    //Importing first ML data 
    //One must install onnxruntime
    //========================
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "rf");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    Ort::Session session(env, "../ONNX/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session.GetOutputNameAllocated(0, allocator);

    const char* input_name  = input_name_alloc.get();
    const char* output_names[] = {"label", "probabilities"};

    //========================
    // Set up input file chain
    //========================
    TChain *mychain = new TChain("G2TauTree");
    mychain->Add(File);

    // Initialize reader
    TTreeReader tree_reader(mychain);

    // Get Track Information
    TTreeReaderArray<unsigned int> TrackNum(tree_reader, "track_n");
    TTreeReaderArray<float> TrackEta(tree_reader, "track_eta");
    TTreeReaderArray<float> TrackPhi(tree_reader, "track_phi");
    TTreeReaderArray<float> TrackPt(tree_reader, "track_pt");
    TTreeReaderArray<int> TrackCharge(tree_reader, "track_charge");

    TTreeReaderArray<float> TruthPrEta(tree_reader, "truth_monopole_eta");
    TTreeReaderArray<float> TruthPrPhi(tree_reader, "truth_monopole_phi");
    TTreeReaderArray<float> TruthPrPt(tree_reader, "truth_monopole_pt");
    TTreeReaderArray<float> TruthPrMass(tree_reader, "truth_monopole_m");




    // Get Information from Trackers
    TTreeReaderArray<float> TrackPixeldEdX(tree_reader, "track_PixeldEdX");
    TTreeReaderArray<float> TrackPixelHits(tree_reader, "track_PixelHits");
    TTreeReaderArray<float> TrackSCTHits(tree_reader, "track_SCTHits");
    TTreeReaderArray<float> TrackTRTHits(tree_reader, "track_TRTHits");

    // Get topo_cluster Information
    TTreeReaderArray<unsigned int> TopoCluNum(tree_reader, "topoclus_n");
    TTreeReaderArray<float> TopoCluEta(tree_reader, "topo_cluster_eta");
    TTreeReaderArray<float> TopoCluPhi(tree_reader, "topo_cluster_phi");
    TTreeReaderArray<float> TopoCluPt(tree_reader, "topo_cluster_pt");
    TTreeReaderArray<float> TopoCluLamda(tree_reader, "topo_cluster_lambda");
    TTreeReaderArray<float> TopoCluLamda2(tree_reader, "topo_cluster_lambda2");
    TTreeReaderArray<float> TopoCluR2(tree_reader, "topo_cluster_r2");
    TTreeReaderArray<float> TopoCluEMProb(tree_reader, "topo_cluster_EM_prob");
    TTreeReaderArray<bool> TopoCluPass(tree_reader, "topo_cluster_pass_sig_cut");




    //========================
    //Creating histograms
    //========================


    TH1D *DiMass = new TH1D("Dimass_MCProton","Proton pair mass",100,2.5,3.5);
    TH1D *DiMassMuonLikeProton = new TH1D("Dimass_MuonLike_MCProton","Muon-like proton pair mass",100,2.5,3.5);
    TH1D *DiMassElectronLikeProton = new TH1D("Dimass_ElectronLike_MCProton","Electron-like proton pair mass",100,2.5,3.5);

    TH1D *Pt = new TH1D("Pt_MCProton","Transverse momentum of proton pair",50,0,0.2);
    TH1D *Rapidity = new TH1D("Rapidity_MCProton","Rapidity of proton pair",50,-3,3);

    TH1D *ProtonEta =new TH1D("ProtonEta_MCProton","ProtonEta",50,-3,3);
    TH1D *ProtonEnergy =new TH1D("ProtonEnergy_MCProton","ProtonEnergy",50,0,6);

    TH1D *EtaMuonLikeProton =new TH1D("EtaMuonLikeProton_MCProton","MuonLikeProtonEta",50,-3,3);
    TH1D *EnergyMuonLikeProton =new TH1D("EnergyMuonLikeProton_MCProton","MuonLikeProtonEnergy",50,0,6);

    TH1D *EtaElectronLikeProton =new TH1D("EtaElectronLikeProton_MCProton","ElectronLikeProtonEta",50,-3,3);
    TH1D *EnergyElectronLikeProton =new TH1D("EnergyElectronLikeProton_MCProton","ElectronLikeProtonEnergy",50,0,6);

    TH1D *ProtonPt =new TH1D("ProtonPt_MCProton","ProtonPt",50,0,1.8);
    TH1D *ProtonPhi =new TH1D("ProtonPhi_MCProton","ProtonPhi",30,-pi,pi);
    
    TH1D *H_PrPixelHits =new TH1D("PrPixelHits_MCProton","PixelHits",11,-0.5,10.5);
    TH1D *H_PrPixelTRTHits =new TH1D("PrPixelTRTHits_MCProton","PixelTRTHits",51,-1,50);
    TH1D *H_PrPixeldEdX =new TH1D("PrPixeldEdX_MCProton","PixeldEdX",100,0,2);
    TH1D *H_PrPixelSCTHits =new TH1D("PrPixelSCTHits_MCProton","PixelSCTHits",12,2.5,14.5);
    
    TH1D *CountProt = new TH1D("CountProt_MCProton","CountProt",7,-0.5,6.5);
    TH1D *PerpProt = new TH1D("PerpProt_MCProton","PerpProt",100,0,3.5);
    TH1D *FProt = new TH1D("FProt_MCProton","FProt",100,0,4);
    TH1D *EMProt = new TH1D("EMProt_MCProton","EMProt",40,-0.1,1.1);
    
    TH1D *LambdaProt = new TH1D("LambdaProt_MCProton","LambdaProt",40,0,4);
    TH1D *Lambda2Prot = new TH1D("Lambda2Prot_MCProton","Lambda2Prot",40,0,600);
    TH1D *RadiusProt = new TH1D("RadiusProt_MCProton","RadiusProt",40,0,100);

    TH1D *EtaRangeHist = new TH1D("EtaRangeHist_MCProton","EtaRangeHist",3,0.5,3.5);

    TH1D *Response = new TH1D("Response_MCProton","Response",3,0.5,3.5);




    int eventID=0;
    int protoncount=0;
    int muonlikecount=0, electronlikecount=0;
    double zeroparticle=0, oneparticle=0, twoparticle=0, allparticles=0,diffrentparticle=0;


    while(tree_reader.Next()){
        //if(eventID>491858) break;
        eventID++;
        if(eventID%300000==0) cout<<"Processing "<<eventID<<" event..."<<endl;

        vector<TLorentzVector> Proton(2);
        int count=0;
        if(TruthPrEta.GetSize()!=2) continue;
        if(TruthPrPt[0]<0.9 || TruthPrPt[1]<0.9) continue;

        //========================
        //Finding tracks of particles || |Eta|<2.5, pt>1GeV 
        //========================
        if(int(TrackNum[0])!=2) continue;
        for(int track=0;track<int(TrackNum[0]);track++)
        {
            
            if(abs(TrackEta[track])<2.5 && TrackPt[track]>1)
            {
                Proton[track].SetPtEtaPhiM(TrackPt[track],TrackEta[track],TrackPhi[track],ProtonMass);
                count++;
            }
            else break;
        }
        //========================
        //Creating pair of pariticles || dif charge, M<3.2GeV, Pt<0.2 GeV 
        //========================
        if(count==2 && TrackCharge[0]!=TrackCharge[1])
        {
            
            TLorentzVector dipartic;
            dipartic=Proton[0]+Proton[1];
            if(dipartic.Perp()<0.2)
            {
                
                Pt->Fill(dipartic.Perp());
                Rapidity->Fill(dipartic.Rapidity());
                DiMass->Fill(dipartic.M());

                //========================
                //Saving proton data  
                //========================

                float probcut   = 0.5;
                float response[2] = {probcut, probcut};
                int   Found     = 0;

                for(int i=0;i<2;i++)
                {
                    allparticles++;

                    H_PrPixeldEdX->Fill(TrackPixeldEdX[i]);
                    H_PrPixelHits->Fill(TrackPixelHits[i]);
                    H_PrPixelSCTHits->Fill(TrackSCTHits[i]);
                    H_PrPixelTRTHits->Fill(TrackTRTHits[i]);

                    ProtonEnergy->Fill(Proton[i].E());
                    ProtonEta->Fill(Proton[i].Eta());
                    ProtonPt->Fill(Proton[i].Pt());
                    ProtonPhi->Fill(Proton[i].Phi());
            
                    
                    Topocluster(Proton[i],TopoCluNum,TopoCluEta,TopoCluPhi,TopoCluPt,TopoCluLamda,TopoCluLamda2,TopoCluR2,TopoCluEMProb,TopoCluPass,
                        CountProt,PerpProt,FProt,EMProt,LambdaProt,Lambda2Prot,RadiusProt,FVariable,EMprop,Lambda2,Lambda,Radius);
                    
                    PixelHits=TrackPixelHits[i];
                    PixelTRTHits=TrackTRTHits[i];
                    PixeldEdX=TrackPixeldEdX[i];
                    PixelSCTHits=TrackSCTHits[i];
                    if(FVariable==-1) continue;              

                    std::vector<float> input = {PixelHits, PixelTRTHits, PixelSCTHits, PixeldEdX, FVariable, EMprop, Lambda, Lambda2, Radius};
                    std::vector<int64_t> input_shape = {1, 9};
                    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(allocator.GetInfo(),input.data(),input.size(),input_shape.data(),input_shape.size());
                    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, &input_name, &input_tensor, 1, output_names, 2);
                    float* prob_ptr = output_tensors[1].GetTensorMutableData<float>();
                    response[i] = prob_ptr[1];
                    Response->Fill(prob_ptr[1]);
                    Found++;

                    if(abs(Proton[i].Eta())<1) EtaRange=1;
                    else if(abs(Proton[i].Eta())>1 && abs(Proton[i].Eta())<1.5) EtaRange=2;
                    else if(abs(Proton[i].Eta())>1.5 && abs(Proton[i].Eta())<2.5) EtaRange=3;
                    EtaRangeHist->Fill(EtaRange);
                    protoncount+=2;
                    IsProton=1;
                    MLDataTree->Fill();

                }


                int lepton=2;
                int rightlepton=2;
                if(Found==0) zeroparticle+=2;
                else if(Found==1)
                {
                    if(response[0]==probcut)
                    {
                        if(response[1]<probcut-0.15) lepton=0;
                        else if(response[1]>probcut+0.15) lepton=1;
                        else Found=0;
                    } 
                    else if(response[1]==probcut)
                    {
                        if(response[0]<probcut-0.15) lepton=0;
                        else if(response[0]>probcut+0.15) lepton=1;
                        else Found=0;
                    } 
                    oneparticle+=2;
                } 
                else if(Found==2) 
                {
                    twoparticle+=2;
                    if((response[0]+response[1])/2<probcut-0.15) lepton=0;
                    else if((response[0]+response[1])/2>probcut+0.15) lepton=1;
                    else
                    {
                        diffrentparticle+=2;
                        Found=0;
                    } 
                }
                

                //========================
                //Saving electron data  
                //========================
                if(lepton==0 && Found>=1) 
                {   
                    DiMassElectronLikeProton->Fill(dipartic.M());
                    for (int i = 0; i < 2; i++) {        
                        EnergyElectronLikeProton->Fill(Proton[i].E());
                        EtaElectronLikeProton->Fill(Proton[i].Eta());
                        
                        electronlikecount++;   
                    }  
                }
                //========================
                //Saving muon data  
                //========================
                if(lepton==1  && Found>=1)
                {   
                    DiMassMuonLikeProton->Fill(dipartic.M());
                    for (int i = 0; i < 2; i++) {        
                        EnergyMuonLikeProton->Fill(Proton[i].E());
                        EtaMuonLikeProton->Fill(Proton[i].Eta());
                        
                        muonlikecount++;   
                    }    
                }
                

                

            }              
        }
    
            
        
    }

    
    dataFile->cd();
    MLDataTree->Write();

    cout<<"==========================="<<endl;
    cout<<"End of file: "<<name<<endl;
    cout<<"Number of events: "<<eventID<<endl;
    cout<<"Number of protons: "<<protoncount<<endl;
    cout<<"Number of muon-like leptons: "<<muonlikecount<<" Ratio: "<<(float)muonlikecount/protoncount<<endl;
    cout<<"Number of electron-like leptons: "<<electronlikecount<<" Ratio: "<<(float)electronlikecount/protoncount<<endl;
    cout<<"Ratio of electron-like/muon-like leptons: "<<(float)electronlikecount/muonlikecount<<endl;
    cout<<"==========================="<<endl;
    cout << "Found count:" << endl;
    cout << "  Two particles:      " << twoparticle      << "  Ratio " << twoparticle / allparticles  << endl;
    cout << "  One particle:       " << oneparticle       << "  Ratio " << oneparticle / allparticles   << endl;
    cout << "  Zero particles:     " << zeroparticle     << "  Ratio " << zeroparticle / allparticles << endl;
    cout << "===========================" << endl;

    outputFile->cd();
    EtaRangeHist->Write();
    DiMass->Write();
    DiMassMuonLikeProton->Write();
    DiMassElectronLikeProton->Write();
    Pt->Write();
    Rapidity->Write();
    EnergyMuonLikeProton->Write();
    EtaMuonLikeProton->Write();
    EnergyElectronLikeProton->Write();
    EtaElectronLikeProton->Write();
    ProtonEnergy->Write();
    ProtonEta->Write();
    ProtonPt->Write();
    ProtonPhi->Write();
    H_PrPixelHits->Write();
    H_PrPixelTRTHits->Write();
    H_PrPixeldEdX->Write();
    H_PrPixelSCTHits->Write();
    CountProt->Write();
    PerpProt->Write();
    FProt->Write();
    EMProt->Write();
    LambdaProt->Write();
    Lambda2Prot->Write();
    RadiusProt->Write();
    Response->Write();

    outputFile->Close();
    dataFile->Close();
  

    
    
}

