#include <onnxruntime_cxx_api.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
#include <string>
#include <TLegend.h>
#include <vector>
#include "OldTopocluster.C"


void SecondAnalysis()
{

    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");

    Float_t MuonMass=0.1056583755;
    Float_t ElectronMass=0.00051099895;
    Double_t pi=TMath::Pi();
    double DEG=180/TMath::Pi();
    
    TString File;
    string name;
     
    File="Data/data23_2trk_moreTCvars.root";
    //File="Data/mc_jpsi_ee.root";
    //File="Data/mc_jpsi_mumu.root";
    name="TrueData";
        
    
    //========================
    //Creating output data file
    //========================
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
    bool  IsMuon;

    TFile *file = new TFile(Form("Data/MLFinalData%s.root",name.c_str()), "RECREATE");
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
    MLDataTree->Branch("IsMuon", &IsMuon, "IsMuon/B");

    //========================
    //Importing first ML data 
    //One must install onnxruntime
    //========================
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "rf");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    Ort::Session session(env, "Data/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session.GetOutputNameAllocated(0, allocator);

    const char* input_name  = input_name_alloc.get();
    const char* output_names[] = {"label", "probabilities"};
    /*
    float mean[9];
    float scale[9];
    std::ifstream file_scaler("Data/scaler.txt");
    for(int i = 0; i < 9; ++i) file_scaler >> mean[i];
    for(int i = 0; i < 9; ++i) file_scaler >> scale[i];
    */   
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


    // Get eg_cluster Information
    TTreeReaderArray<unsigned int> EgCluNum(tree_reader, "eg_cluster_n");
    TTreeReaderArray<float> EgCluEta(tree_reader, "eg_cluster_eta");
    TTreeReaderArray<float> EgCluPhi(tree_reader, "eg_cluster_phi");

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
    TH1D *Number = new TH1D("Number","Number",6,-0.5,5.5);
    TH1D *Response = new TH1D("Response","MCRF Response to Experiment data",50,0,1);
    TH1D *ResponseDiffrend = new TH1D("ResponseDiffrend","ResponseDiffrend",100,0,1);
    TH1D *ResponseOneFound = new TH1D("ResponseOneFound","ResponseOneFound",100,0,1);

    TH1D *DeltaPhi = new TH1D("DeltaPhi","DeltaPhi",100,0,3.5);
    TH1D *DeltaEta = new TH1D("DeltaEta","DeltaEta",100,-3,3);
    TH1D *HRadEtaPhi = new TH1D("RadEtaPhi","RadEtaPhi",100,0,8);
    TH1D *EtaRangeHist = new TH1D("EtaRangeHist","EtaRangeHist",3,0.5,3.5);

    TH1D *DiMass = new TH1D("dimass","Lepton pair mass",50,2.5,3.5);
    TH1D *DiMassEl = new TH1D("dimassEl","Lepton pair mass",50,2.5,3.5);
    TH1D *DiMassMu = new TH1D("dimassMu","Lepton pair mass",50,2.5,3.5);
    TH1D *DiMassMuEta[5],*DiMassElEta[5];//*DiMassEta[5],
    for(int etarange=0;etarange<5;etarange++)
    {   
        double min=etarange*0.5;
        double max=min+0.5;
       //DiMassEta[etarange] = TH1D(Form("dimassEta%d",etarange),Form("Lepton pair mass: Eta: %d - %d",min,max),50,2.9,3.2);
        DiMassElEta[etarange] = new TH1D(Form("dimassElEta%d",etarange),Form("Lepton pair mass: Eta: %g - %g",min,max),40,2.5,3.5);
        DiMassMuEta[etarange] = new TH1D(Form("dimassMuEta%d",etarange),Form("Lepton pair mass: Eta: %g - %g",min,max),40,2.5,3.5);
    }

    TH1D *Pt = new TH1D("Pt","Transverse momentum of lepton pair",50,0,0.2);
    TH1D *Rapidity = new TH1D("Rapidity ","Rapidity of lepton pair",50,-3,3);

    TH1D *ElectronEta =new TH1D("ElectronEta","ElectronEta",50,-3,3);
    TH1D *ElectronEnergy =new TH1D("ElectronEnergy","ElectronEnergy",50,0,6);
    TH1D *ElectronPt =new TH1D("ElectronPt","ElectronPt",50,0,1.8);
    TH1D *ElectronPhi =new TH1D("ElectronPhi","ElectronPhi",30,-pi,pi);

    TH1D *MuonEta =new TH1D("MuonEta","MuonEta",50,-3,3);
    TH1D *MuonEnergy =new TH1D("MuonEnergy","MuonEnergy",50,0,6);
    TH1D *MuonPt =new TH1D("MuonPt","MuonPt",50,0,1.8);
    TH1D *MuonPhi =new TH1D("MuonPhi","MuonPhi",30,-pi,pi);
    
    TH1D *H_ElPixelHits =new TH1D("ElPixelHits","PixelHits",11,-0.5,10.5);
    TH1D *H_ElPixelTRTHits =new TH1D("ElPixelTRTHits","PixelTRTHits",51,-1,50);
    TH1D *H_ElPixeldEdX =new TH1D("ElPixeldEdX","PixeldEdX",100,0,2);
    TH1D *H_ElPixelSCTHits =new TH1D("ElPixelSCTHits","PixelSCTHits",12,2.5,14.5);
    TH1D *H_MuPixelHits =new TH1D("MuPixelHits","PixelHits",11,-0.5,10.5);
    TH1D *H_MuPixelTRTHits =new TH1D("MuPixelTRTHits","PixelTRTHits",51,-1,50);
    TH1D *H_MuPixeldEdX =new TH1D("MuPixeldEdX","PixeldEdX",100,0,2);
    TH1D *H_MuPixelSCTHits =new TH1D("MuPixelSCTHits","PixelSCTHits",12,2.5,14.5);
    
    TH1D *CountMuon = new TH1D("CountMuon","CountMuon",7,-0.5,6.5);
    TH1D *CountElec = new TH1D("CountElec","CountElec",7,-0.5,6.5);
    TH1D *PerpMuon = new TH1D("PerpMuon","PerpMuon",100,0,3.5);
    TH1D *PerpElec = new TH1D("PerpElec","PerpElec",100,0,3.5);
    TH1D *FMuon = new TH1D("FMuon","FMuon",100,0,4);
    TH1D *FElec = new TH1D("FElec","FElec",100,0,4);
    TH1D *EMMuon = new TH1D("EMMuon","EMMuon",40,-0.1,1.1);
    TH1D *EMElec = new TH1D("EMElec","EMElec",40,-0.1,1.1);
    TH1D *LambdaMuon = new TH1D("LambdaMuon","LambdaMuon",40,0,4);
    TH1D *LambdaElec = new TH1D("LambdaElec","LambdaElec",40,0,4);
    TH1D *Lambda2Muon = new TH1D("Lambda2Muon","Lambda2Muon",40,0,600);
    TH1D *Lambda2Elec = new TH1D("Lambda2Elec","Lambda2Elec",40,0,600);
    TH1D *RadiusMuon = new TH1D("RadiusMuon","RadiusMuon",40,0,100);
    TH1D *RadiusElec = new TH1D("RadiusElec","RadiusElec",40,0,100);

    TH1D *H_Count = new TH1D("H_Count","Count",7,-0.5,6.5);
    TH1D *H_Perp = new TH1D("H_Perp","Perp",100,0,3.5);
    TH1D *H_F = new TH1D("H_F","F",100,0,4);
    TH1D *H_EM = new TH1D("H_EM","EM",40,-0.1,1.1);
    TH1D *H_Lambda = new TH1D("H_Lambda","Lambda",40,0,4);
    TH1D *H_Lambda2 = new TH1D("H_Lambda2","Lambda2",40,0,600);
    TH1D *H_Radius = new TH1D("H_Radius","Radius",40,0,100);

    TH1D *PhiDiffHist = new TH1D("PhiDiffHist","PhiDiffHist",40,160,180);
    
    


    int eventID=0;
    float muoncount=0;
    float electroncount=0;
    float diffrentparticle=0;
    float zeroparticle=0;
    float oneparticle=0;
    float twoparticle=0;
    float allparticles=0;


    while(tree_reader.Next())
    {
        if(eventID%200000==0) cout<<"Processing "<<eventID<<" event..."<<endl;
        eventID++;
        vector<TLorentzVector> Electron(2);
        vector<TLorentzVector> Muon(2);
        vector<TLorentzVector> t(2);
        int count=0;

        //========================
        //Finding tracks of particles || |Eta|<2.5, pt>1GeV 
        //========================
        if(int(TrackNum[0])!=2) continue;
        for(int track=0;track<int(TrackNum[0]);track++)
        {
            
            if(abs(TrackEta[track])<2.5 && TrackPt[track]>1)
            {
                t[track].SetPtEtaPhiM(TrackPt[track],TrackEta[track],TrackPhi[track],MuonMass);
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
            dipartic=t[0]+t[1];
            if(dipartic.Perp()<0.2 && dipartic.M()<3.5)// && dipartic.M()>2.7)
            {
            
                Pt->Fill(dipartic.Perp());
                Rapidity->Fill(dipartic.Rapidity());
                DiMass->Fill(dipartic.M());
                //========================
                //Cut on Bethe-Heitler
                //========================
                TVector3 b = dipartic.BoostVector();
                TLorentzVector t_prim0, t_prim1;
                t_prim0=t[0];// t_prim0.Boost(b);
                t_prim1=t[1];// t_prim1.Boost(b);
                PhiDiffHist->Fill(acos(cos(t_prim0.Phi()-t_prim1.Phi()))*DEG);



                
                //========================
                //Identifying particle by ML data
                //========================
                //float probcut=0.72;
                float probcut=0.5;
                float response[2]={probcut,probcut};
                int Found=0;
                for(int i=0;i<2;i++)
                {
                    allparticles++;
                    Topocluster(t[i],TopoCluNum,TopoCluEta,TopoCluPhi,TopoCluPt,TopoCluLamda,TopoCluLamda2,TopoCluR2,TopoCluEMProb,TopoCluPass,
                        H_Count,H_Perp,H_F,H_EM,H_Lambda,H_Lambda2,H_Radius,FVariable,EMprop,Lambda2,Lambda,Radius);
                    
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
                        ResponseOneFound->Fill(response[1]);

                    } 
                    else if(response[1]==probcut)
                    {
                        if(response[0]<probcut-0.15) lepton=0;
                        else if(response[0]>probcut+0.15) lepton=1;
                        ResponseOneFound->Fill(response[0]);
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
                        ResponseDiffrend->Fill(response[0]);

                    } 

                }
                

                //========================
                //Saving electron data  
                //========================
                if(lepton==0 && Found>=1) // && dipartic.M()<3 
                {
                    for(int i=0;i<2;i++)
                    {
                       
                        Electron[i]=t[i];
                        H_ElPixeldEdX->Fill(TrackPixeldEdX[i]);
                        H_ElPixelHits->Fill(TrackPixelHits[i]);
                        H_ElPixelSCTHits->Fill(TrackSCTHits[i]);
                        H_ElPixelTRTHits->Fill(TrackTRTHits[i]);

                        ElectronEnergy->Fill(Electron[i].E());
                        ElectronEta->Fill(Electron[i].Eta());
                        ElectronPt->Fill(Electron[i].Pt());
                        ElectronPhi->Fill(Electron[i].Phi());
                        DiMassEl->Fill(dipartic.M());
                        
                        Topocluster(Electron[i],TopoCluNum,TopoCluEta,TopoCluPhi,TopoCluPt,TopoCluLamda,TopoCluLamda2,TopoCluR2,TopoCluEMProb,TopoCluPass,
                            CountElec,PerpElec,FElec,EMElec,LambdaElec,Lambda2Elec,RadiusElec,FVariable,EMprop,Lambda2,Lambda,Radius);
                        
                        if(FVariable==-1) continue;          
                        PixelHits=TrackPixelHits[i];
                        PixelTRTHits=TrackTRTHits[i];
                        PixeldEdX=TrackPixeldEdX[i];
                        PixelSCTHits=TrackSCTHits[i];
                        if(abs(Electron[i].Eta())<1) EtaRange=1;
                        else if(abs(Electron[i].Eta())>1 && abs(Electron[i].Eta())<1.5) EtaRange=2;
                        else if(abs(Electron[i].Eta())>1.5 && abs(Electron[i].Eta())<2.5) EtaRange=3;
                        IsMuon=0;
                        MLDataTree->Fill();
                        EtaRangeHist->Fill(EtaRange);
                        for(int etarange=0;etarange<5;etarange++)
                        {   
                            double max=etarange*0.5+0.5;
                            if(abs(Electron[i].Eta())< max)
                            {
                                DiMassElEta[etarange]->Fill(dipartic.M());
                                break;
                            }  
                        }
                        DiMassEl->Fill(dipartic.M());
                        electroncount++;
                    } 

                }
                //========================
                //Saving muon data  
                //========================
                if(lepton==1  && Found>=1)//&& dipartic.M()>2.9
                {
                    for(int i=0;i<2;i++)
                    {
                      

                        Muon[i]=t[i];

                        H_MuPixeldEdX->Fill(TrackPixeldEdX[i]);
                        H_MuPixelHits->Fill(TrackPixelHits[i]);
                        H_MuPixelSCTHits->Fill(TrackSCTHits[i]);
                        H_MuPixelTRTHits->Fill(TrackTRTHits[i]);

                        MuonEnergy->Fill(Muon[i].E());
                        MuonEta->Fill(Muon[i].Eta());
                        MuonPt->Fill(Muon[i].Pt());
                        MuonPhi->Fill(Muon[i].Phi());
                        Topocluster(Muon[i],TopoCluNum,TopoCluEta,TopoCluPhi,TopoCluPt,TopoCluLamda,TopoCluLamda2,TopoCluR2,TopoCluEMProb,TopoCluPass,
                            CountMuon,PerpMuon,FMuon,EMMuon,LambdaMuon,Lambda2Muon,RadiusMuon, FVariable, EMprop,Lambda2, Lambda, Radius);

                        if(FVariable==-1) continue;
                        PixelHits=TrackPixelHits[i];
                        PixelTRTHits=TrackTRTHits[i];
                        PixeldEdX=TrackPixeldEdX[i];
                        PixelSCTHits=TrackSCTHits[i];
                        IsMuon=1;
                        if(abs(Muon[i].Eta())<1) EtaRange=1;
                        else if(abs(Muon[i].Eta())>1 && abs(Muon[i].Eta())<1.5) EtaRange=2;
                        else if(abs(Muon[i].Eta())>1.5 && abs(Muon[i].Eta())<2.5) EtaRange=3;
                        MLDataTree->Fill();
                        EtaRangeHist->Fill(EtaRange);
                        for(int etarange=0;etarange<=5;etarange++)
                        {   
                            double max=etarange*0.5+0.5;
                            if(abs(Muon[i].Eta())< max)
                            {
                                DiMassMuEta[etarange]->Fill(dipartic.M());
                                break;
                            }  
                        }
                        DiMassMu->Fill(dipartic.M());
                        muoncount++;
                    }  
                }
            }
        }
    }

    cout<<"==========================="<<endl;
    cout<<"End of file"<<endl;
    cout<<"Number of events: "<<eventID<<endl;
    cout<<"Number of electrons: "<<electroncount<<endl;
    cout<<"Number of muons: "<<muoncount<<endl;
    cout<<"==========================="<<endl;
    cout<<"Found count:"<<endl;
    cout<<"             Two particles: "<<twoparticle<<" Ratio "<<twoparticle/allparticles<<endl;
    cout<<"             One particle: "<<oneparticle<<" Ratio "<<oneparticle/allparticles<<endl;
    cout<<"             Zero particles: "<<zeroparticle<<" Ratio "<<zeroparticle/allparticles<<endl;
    cout<<"             Diffrent particles: "<<diffrentparticle<<endl;
    cout<<"==========================="<<endl;

    //gStyle->SetOptStat(000000);
    
    //========================
    //Drawing histograms  
    //========================
    TCanvas c1;
    TLegend* leg = new TLegend(0.58, 0.6, 0.85, 0.85);
        leg->SetBorderSize(0);
        leg->SetNColumns(1);
        leg->SetColumnSeparation(0.1);
        leg->SetEntrySeparation(0.1);
        leg->SetMargin(0.15);
        leg->SetTextFont(42);
        leg->SetTextSize(0.05);
        leg->AddEntry(H_ElPixelHits,"Electron","l");
        leg->AddEntry(H_MuPixelHits,"Muon","l");
    TLegend* leg2 = new TLegend(0.2, 0.6, 0.45, 0.85);
        leg2->SetBorderSize(0);
        leg2->SetNColumns(1);
        leg2->SetColumnSeparation(0.1);
        leg2->SetEntrySeparation(0.1);
        leg2->SetMargin(0.15);
        leg2->SetTextFont(42);
        leg2->SetTextSize(0.05);
        leg2->AddEntry(DiMassEl,"Electron","l");
        leg2->AddEntry(DiMassMu,"Muon","l");
    
    c1.SaveAs(Form("Plots/2%s.pdf[", name.c_str()));
    c1.Clear();
    Response->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));
    c1.Clear();
    ResponseDiffrend->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));
    c1.Clear();
    ResponseOneFound->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    c1.Clear();
    DiMass->GetXaxis()->SetTitle("M_{ll} [GeV]");
    DiMass->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    c1.Clear();
    Pt->GetXaxis()->SetTitle("P_{t}^{ll} [GeV]");
    Pt->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    c1.Clear();
    Rapidity->GetXaxis()->SetTitle("\\eta_{ll} [GeV]");
    Rapidity->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    c1.Clear();
    PhiDiffHist->GetXaxis()->SetTitle("\\Delta \\Phi");
    PhiDiffHist->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    
    c1.Clear();
    DiMassMu->GetXaxis()->SetTitle("M_{ll} [GeV]");
    DiMassEl->SetLineColor(kRed);
    DiMassMu->SetLineColor(kBlue);
    DiMassMu->Draw();
    DiMassEl->Draw("same");
    leg2->Draw();
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

    for(int etarange=0;etarange<5;etarange++)
    {   
       
        c1.Clear();
        double maxMu = DiMassMuEta[etarange]->GetMaximum();
        double maxEl = DiMassElEta[etarange]->GetMaximum();
        double totalMax = (maxMu > maxEl) ? maxMu : maxEl;

        DiMassMuEta[etarange]->GetYaxis()->SetRangeUser(0, totalMax * 1.1);
        DiMassMuEta[etarange]->GetXaxis()->SetTitle("M_{ll} [GeV]");
        DiMassElEta[etarange]->SetLineColor(kRed);
        DiMassMuEta[etarange]->SetLineColor(kBlue);
        DiMassMuEta[etarange]->Draw();
        DiMassElEta[etarange]->Draw("same");
        leg2->Draw();
        c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));
    }
    
    
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
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));

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
    c1.SaveAs(Form("Plots/2%s.pdf", name.c_str()));
    c1.SaveAs(Form("Plots/2%s.pdf]", name.c_str()));
    gStyle->SetOptStat(000000);

    c1.Clear();
    c1.Divide(3,2);
    for(int etarange=0;etarange<5;etarange++)
    {   
        c1.cd(etarange+1);
        
        double maxMu = DiMassMuEta[etarange]->GetMaximum();
        double maxEl = DiMassElEta[etarange]->GetMaximum();
        double totalMax = (maxMu > maxEl) ? maxMu : maxEl;
        DiMassMuEta[etarange]->GetYaxis()->SetRangeUser(0, totalMax * 1.05);
        DiMassMuEta[etarange]->GetXaxis()->SetTitle("M_{ll} [GeV]");
        DiMassElEta[etarange]->SetLineColor(kRed);
        DiMassMuEta[etarange]->SetLineColor(kBlue);
        DiMassElEta[etarange]->SetLineWidth(2);
        DiMassMuEta[etarange]->SetLineWidth(2);
        DiMassMuEta[etarange]->Draw();
        DiMassElEta[etarange]->Draw("same");
        leg2->Draw();
    }
    c1.SaveAs("Plots/PaperHist.pdf");

    c1.Clear();
    Response->SetLineWidth(2);
    Response->Draw();
    c1.SaveAs("Plots/ResponseHist.pdf");


    

    MLDataTree->Write();
    file->Close();
    
}

