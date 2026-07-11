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
#include <onnxruntime_cxx_api.h>



void CreatingData()
{

    gROOT->SetBatch(kTRUE);
    gROOT->ProcessLine("gErrorIgnoreLevel = 3000;");
    //gStyle->SetOptStat(0);

    const Float_t ProtonMass    = 0.93827208816; 
    Double_t pi=TMath::Pi();
    double DEG=180/TMath::Pi();
    
    TString File;
    string name;  
                
    File="../Data/data23_2trk_moreTCvars.root";
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

    TFile *file = new TFile(Form("Data/MLData%s.root",name.c_str()), "RECREATE");
    TTree *MLDataTree = new TTree("MLDataTree", "MLDataTree");

    MLDataTree->Branch("track_PixelHits", &PixelHits, "track_PixelHits/F");
    MLDataTree->Branch("track_TRTHits", &PixelTRTHits, "track_PixelTRTHits/F");
    MLDataTree->Branch("track_SCTHits", &PixelSCTHits, "track_PixelSCTHits/F");
    MLDataTree->Branch("Cal_FVariable", &FVariable, "Cal_FVariable/F");
    MLDataTree->Branch("Cal_EMprop", &EMprop, "Cal_EMprop/F");
    MLDataTree->Branch("Cal_Lambda", &Lambda, "Cal_Lambda/F");
    MLDataTree->Branch("Cal_Lambda2", &Lambda2, "Cal_Lambda2/F");
    MLDataTree->Branch("Cal_Radius", &Radius, "Cal_Radius/F");
    MLDataTree->Branch("EtaRange", &EtaRange, "EtaRange/F");

    
    //TFile *histFile = new TFile(Form("Plots/Histograms_%s.root", name.c_str()), "RECREATE");
    //MLDataTree->SetDirectory(file);
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
    TTreeReaderArray<int> TopoCellSig(tree_reader, "topo_cluster_cell_sig_sampling");

    //========================
    // ML setup
    //========================
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "rf");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    Ort::Session session(env, "../ONNX/RFMuonElectron.onnx", session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    auto input_name_alloc  = session.GetInputNameAllocated(0, allocator);
    auto output_name_alloc = session.GetOutputNameAllocated(0, allocator);
    const char* input_name     = input_name_alloc.get();
    const char* output_names[] = {"label", "probabilities"};



    //========================
    //Creating histograms
    //========================
    TH1D *Number = new TH1D("Number","Number",6,-0.5,5.5);
    TH1D *DeltaPhi = new TH1D("DeltaPhi","DeltaPhi",100,0,3.5);
    TH1D *DeltaEta = new TH1D("DeltaEta","DeltaEta",100,-3,3);
    TH1D *HRadEtaPhi = new TH1D("RadEtaPhi","RadEtaPhi",100,0,0.8);
    TH1D *EtaRangeHist = new TH1D("EtaRangeHist","EtaRangeHist",3,0.5,3.5);
    TH1D *CheckSigPrec = new TH1D("CheckSigPrec","CheckSigPrec",21,-0.5,20.5);


    TH1D *DiMass = new TH1D("dimass","Lepton pair mass",50,2.9,3.2);
    TH1D *Pt = new TH1D("Pt","Transverse momentum of lepton pair",50,0,0.2);
    TH1D *Rapidity = new TH1D("Rapidity ","Rapidity of lepton pair",50,-3,3);

    TH1D *ProtonEta =new TH1D("ProtonEta","ProtonEta",50,-3,3);
    TH1D *ProtonEnergy =new TH1D("ProtonEnergy","ProtonEnergy",50,0,6);
    TH1D *ProtonPt =new TH1D("ProtonPt","ProtonPt",50,0,1.8);
    TH1D *ProtonPhi =new TH1D("ProtonPhi","ProtonPhi",30,-pi,pi);

    
    TH1D *H_PrPixelHits =new TH1D("PrPixelHits","PixelHits",11,-0.5,10.5);
    TH1D *H_PrPixelTRTHits =new TH1D("PrPixelTRTHits","PixelTRTHits",51,-1,50);
    TH1D *H_PrPixeldEdX =new TH1D("PrPixeldEdX","PixeldEdX",100,0,2);
    TH1D *H_PrPixelSCTHits =new TH1D("PrPixelSCTHits","PixelSCTHits",12,2.5,14.5);
    
    
    TH1D *CountPrec = new TH1D("CountPrec","CountPrec",7,-0.5,6.5);
    TH1D *PerpPrec = new TH1D("PerpPrec","PerpPrec",100,0,3.5);
    TH1D *FPrec = new TH1D("FPrec","FPrec",100,0,4);
    TH1D *EMPrec = new TH1D("EMPrec","EMPrec",40,-0.1,1.1);
    
    TH1D *LambdaPrec = new TH1D("LambdaPrec","LambdaPrec",40,0,4);
    TH1D *Lambda2Prec = new TH1D("Lambda2Prec","Lambda2Prec",40,0,600);
    TH1D *RadiusPrec = new TH1D("RadiusPrec","RadiusPrec",40,0,100);


    int eventID=0;
    int diparticcount=0;
    int protoncount=0;


    while(tree_reader.Next()){
        //if(eventID>491858) break;
        eventID++;
        //if(eventID%300000==0) cout<<"Processing "<<eventID<<" event..."<<endl;
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
                t[track].SetPtEtaPhiM(TrackPt[track],TrackEta[track],TrackPhi[track],ProtonMass);
                count++;
            }
            else break;
        }
        //========================
        //Creating pair of pariticles || dif charge, M<3.2GeV, Pt<0.2 GeV 
        //========================
        if(count==2 && TrackCharge[0]!=TrackCharge[1])
        {
            diparticcount++;
            TLorentzVector dipartic;
            dipartic=t[0]+t[1];
            
            if(dipartic.Perp()<0.2 && dipartic.M()<3.16 && dipartic.M()>3.04)
            {
                for(int i=0;i<2;i++)
                {
                    H_PrPixeldEdX->Fill(TrackPixeldEdX[1]);
                    H_PrPixelHits->Fill(TrackPixelHits[1]);
                    H_PrPixelSCTHits->Fill(TrackSCTHits[1]);
                    H_PrPixelTRTHits->Fill(TrackTRTHits[1]);

                    ProtonEnergy->Fill(t[i].E());
                    ProtonEta->Fill(t[i].Eta());
                    ProtonPt->Fill(t[i].Pt());
                    ProtonPhi->Fill(t[i].Phi());

                    DiMass->Fill(dipartic.M());

                    Topocluster(t[i],TopoCluNum,TopoCluEta,TopoCluPhi,TopoCluPt,TopoCluLamda,TopoCluLamda2,TopoCluR2,TopoCluEMProb,TopoCluPass,
                        CountPrec,PerpPrec,FPrec,EMPrec,LambdaPrec,Lambda2Prec,RadiusPrec, FVariable,EMprop,Lambda2,Lambda,Radius);
                    
                    if(FVariable==-1) continue; 

                    PixelHits=TrackPixelHits[1];
                    PixelTRTHits=TrackTRTHits[1];
                    PixeldEdX=TrackPixeldEdX[1];
                    PixelSCTHits=TrackSCTHits[1];
                    std::vector<float>   input = {PixelHits, PixelTRTHits, PixelSCTHits, PixeldEdX, FVariable, EMprop, Lambda, Lambda2, Radius};
                    std::vector<int64_t> input_shape = {1, 9};
                    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                        allocator.GetInfo(), input.data(), input.size(), input_shape.data(), input_shape.size());
                    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, &input_name, &input_tensor, 1, output_names, 2);
                    float* prob_ptr = output_tensors[1].GetTensorMutableData<float>();
                    if(prob_ptr[1]>0.5)  continue;

                    if(abs(t[i].Eta())<1) EtaRange=1;
                    else if(abs(t[i].Eta())>1 && abs(t[i].Eta())<1.5) EtaRange=2;
                    else if(abs(t[i].Eta())>1.5 && abs(t[i].Eta())<2.5) EtaRange=3;
                    MLDataTree->Fill();
                    EtaRangeHist->Fill(EtaRange);
                    protoncount++;
                }
               

            }
    
            
        
        }
        
    }
    
    

    cout<<"==========================="<<endl;
    cout<<"End of file: "<<name<<endl;
    cout<<"Number of events: "<<eventID<<endl;
    cout<<"Number of particles aft cuts: "<<diparticcount*2<<endl;
    cout<<"Number of Protons: "<<protoncount<<endl;
    cout<<"==========================="<<endl;

    //gStyle->SetOptStat(000000);

    //========================
    //Drawing histograms  
    //========================
       
    TCanvas c1;
    
    c1.SaveAs(Form("Plots/Control/%s.pdf[", name.c_str()));

    c1.Clear();
    DiMass->GetXaxis()->SetTitle("M_{ll} [GeV]");
    DiMass->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    Pt->GetXaxis()->SetTitle("P_{t}^{ll} [GeV]");
    Pt->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    Rapidity->GetXaxis()->SetTitle("\\eta_{ll} [GeV]");
    Rapidity->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));


    c1.Clear();
    c1.Divide(2,2);
    c1.cd(1);
    ProtonEnergy->Draw();
    c1.cd(2);
    ProtonEta->Draw();
    c1.cd(3);
    ProtonPt->Draw();
    c1.cd(4);
    ProtonPhi->SetMinimum(0);
    ProtonPhi->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    
    c1.Clear();
    c1.Divide(2,2);
    c1.cd(1);
    H_PrPixelHits->SetTitle("PixelHits");
    H_PrPixelHits->Scale(1./H_PrPixelHits->Integral());
    H_PrPixelHits->SetLineColor(kBlue);
    H_PrPixelHits->Draw("HIST");

    c1.cd(2);
    H_PrPixelHits->SetTitle("PixelTRTHits");
    H_PrPixelTRTHits->Scale(1./H_PrPixelTRTHits->Integral());
    H_PrPixelTRTHits->SetLineColor(kBlue);
    H_PrPixelTRTHits->Draw("HIST");

    c1.cd(3);
    H_PrPixelHits->SetTitle("PixelSCTHits");

    H_PrPixelSCTHits->Scale(1./H_PrPixelSCTHits->Integral());
    H_PrPixelSCTHits->SetLineColor(kBlue);
    H_PrPixelSCTHits->Draw("HIST");

    c1.cd(4);
    H_PrPixelHits->SetTitle("PixeldEdX");
    H_PrPixeldEdX->Scale(1./H_PrPixeldEdX->Integral());
    H_PrPixeldEdX->SetLineColor(kBlue);
    H_PrPixeldEdX->Draw("HIST");

    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    c1.Divide(2,2);
    c1.cd(1);
    CountPrec->Scale(1./CountPrec->Integral());
    CountPrec->SetLineColor(kBlue);
    CountPrec->Draw("HIST");

    c1.cd(2);
    PerpPrec->SetTitle("Travsersal momentum");
    PerpPrec->Scale(1./PerpPrec->Integral());
    PerpPrec->SetLineColor(kBlue);
    PerpPrec->Draw("HIST");

    c1.cd(3);
    FPrec->SetTitle("F variable");
    FPrec->Scale(1./FPrec->Integral());
    FPrec->SetLineColor(kBlue);
    FPrec->Draw("HIST SAME");

    c1.cd(4);
    EMPrec->SetTitle("EMCalorimetr probality");
    EMPrec->Scale(1./EMPrec->Integral());
    EMPrec->SetLineColor(kBlue);
    EMPrec->Draw("HIST SAME");
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    c1.Divide(2,2);
    c1.cd(1);
    LambdaPrec->SetTitle("Cluster's lambda");
    LambdaPrec->Scale(1./LambdaPrec->Integral());
    LambdaPrec->SetLineColor(kBlue);
    LambdaPrec->Draw("HIST");

    c1.cd(2);
    Lambda2Prec->SetTitle("Cluster's lambda2");
    Lambda2Prec->Scale(1./Lambda2Prec->Integral());
    Lambda2Prec->SetLineColor(kBlue);
    Lambda2Prec->Draw("HIST");

    c1.cd(3);
    RadiusPrec->SetTitle("Cluster's radius");
    RadiusPrec->Scale(1./RadiusPrec->Integral());
    RadiusPrec->SetLineColor(kBlue);
    RadiusPrec->Draw("HIST");
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));


    c1.Clear();
    CheckSigPrec->SetLineColor(kRed);
    CheckSigPrec->Draw();

    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));
    

    c1.Clear();
    Number->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    c1.Divide(1,2);
    c1.cd(1);
    DeltaEta->Draw();
    c1.cd(2);
    DeltaPhi->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    HRadEtaPhi->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.Clear();
    EtaRangeHist->Draw();
    c1.SaveAs(Form("Plots/Control/%s.pdf", name.c_str()));

    c1.SaveAs(Form("Plots/Control/%s.pdf]", name.c_str()));
    MLDataTree->Write();
    file->Close();
    
    
}

