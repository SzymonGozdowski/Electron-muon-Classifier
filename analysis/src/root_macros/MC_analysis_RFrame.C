#include <ROOT/RDataFrame.hxx>
#include <TFile.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <cmath>
#include <vector>
#include <iostream>

const float ELEC_MASS = 0.000511; // GeV/c^2
const float MUON_MASS = 0.10566; // GeV/c^2

struct TopoFeatures{
    float FVariable;
    float EMprop;
    float lambda;
    float lambda2;
    float time;
    float radius;
};

TopoFeatures ComputeTopoFeatures(
    const TLorentzVector& lepton,
    const std::vector<float>& topo_pt,
    const std::vector<float>& topo_eta,
    const std::vector<float>& topo_phi,
    const std::vector<float>& topo_EM_prob,
    const std::vector<float>& topo_lambda,
    const std::vector<float>& topo_lambda2,
    const std::vector<float>& topo_time,
    const std::vector<float>& topo_r2,
    const std::vector<bool>& topo_pass_sig_cut
){
    TopoFeatures f{};
    f.FVariable = -1;
    f.EMprop = -1;
    f.lambda = -1;
    f.lambda2 = -1;
    f.time = -1;
    f.radius = -1;

    float max_pt = -1;
    int id = -1;

    for(size_t i=0;i<topo_pt.size();i++){
        float Dphi = std::acos(std::cos(lepton.Phi() - topo_phi[i]));
        float dR2 = (lepton.Eta()-topo_eta[i])*(lepton.Eta()-topo_eta[i]) + Dphi*Dphi;

        if(dR2 < 0.5*0.5 && topo_pass_sig_cut[i]){
            if(topo_pt[i] > max_pt){
                max_pt = topo_pt[i];
                id = i;
            }
        }
    }

    if(id!=-1){
        f.FVariable = topo_pt[id]/lepton.Pt();
        f.EMprop = topo_EM_prob[id];
        f.lambda = topo_lambda[id];
        f.lambda2 = topo_lambda2[id];
        f.time = topo_time[id];
        f.radius = topo_r2[id];
    }

    return f;
}

ROOT::RDF::RNode SampleAnalysis(
    ROOT::RDF::RNode df,
    int label,
    std::vector<ROOT::RDF::RResultPtr<TH1D>>& histograms
){

    auto df2 = df
    .Filter("track_pt.size()>=2")
    .Define("lep0", [label](const std::vector<float>& pt,
                            const std::vector<float>& eta,
                            const std::vector<float>& phi){
        TLorentzVector v;
        v.SetPtEtaPhiM(pt[0],eta[0],phi[0], label==0 ? ELEC_MASS : MUON_MASS);
        return v;
    }, {"track_pt","track_eta","track_phi"})

    .Define("lep1", [label](const std::vector<float>& pt,
                            const std::vector<float>& eta,
                            const std::vector<float>& phi){
        TLorentzVector v;
        v.SetPtEtaPhiM(pt[1],eta[1],phi[1], label==0 ? ELEC_MASS : MUON_MASS);
        return v;
    }, {"track_pt","track_eta","track_phi"})

    .Filter([](TLorentzVector l0, TLorentzVector l1){
        if(l0.Pt()<0.2 || l1.Pt()<0.2) return false;
        if(std::abs(l0.Eta())>2.5 || std::abs(l1.Eta())>2.5) return false;
        float dileptonPt = (l0+l1).Pt();
        float dimass = (l0+l1).M();
        if(dileptonPt>=0.2 || dimass>3.2 || dimass<2.9) return false;
        return true;
    }, {"lep0","lep1"})

    .Define("lepton_classified", [label](TLorentzVector l0, TLorentzVector l1){
        if(label==0) return l1;
        return l0;
    },  {"lep0","lep1"})

    .Filter(
        [label](const ROOT::VecOps::RVec<int>& topo){
        if(label == 0) return true;   // electron
        return topo[0] > 7;           // muon
    },  {"topo_cluster_cell_sig_sampling"})

    .Define("TopoFeatures",[](TLorentzVector lep,
                              const std::vector<float>& topo_pt,
                              const std::vector<float>& topo_eta,
                              const std::vector<float>& topo_phi,
                              const std::vector<float>& topo_EM_prob,
                              const std::vector<float>& topo_lambda,
                              const std::vector<float>& topo_lambda2,
                              const std::vector<float>& topo_time,
                              const std::vector<float>& topo_r2,
                              const std::vector<bool>& topo_pass_sig_cut){
        return ComputeTopoFeatures(lep,topo_pt,topo_eta,topo_phi,
                                   topo_EM_prob,topo_lambda,topo_lambda2,
                                   topo_time,topo_r2,topo_pass_sig_cut);
    },
    {"lepton_classified",
     "topo_cluster_pt","topo_cluster_eta","topo_cluster_phi",
     "topo_cluster_EM_prob","topo_cluster_lambda","topo_cluster_lambda2",
     "topo_cluster_time","topo_cluster_r2","topo_cluster_pass_sig_cut"})

    .Define("lepton_FVariable","TopoFeatures.FVariable")
    .Define("lepton_EMCal","TopoFeatures.EMprop")
    .Define("lepton_lambda","TopoFeatures.lambda")
    .Define("lepton_lambda2","TopoFeatures.lambda2")
    .Define("lepton_time","TopoFeatures.time")
    .Define("lepton_radius","TopoFeatures.radius")
    .Define("truth_particle_tag",[label](){return label;});

    histograms.push_back(df2.Histo1D({"hist_FVariable","",100,0,1},"lepton_FVariable"));
    histograms.push_back(df2.Histo1D({"hist_EMCal","",100,0,1},"lepton_EMCal"));
    histograms.push_back(df2.Histo1D({"hist_lambda","",600,0,6000},"lepton_lambda"));
    histograms.push_back(df2.Histo1D({"hist_lambda2","",600,0,600},"lepton_lambda2"));
    histograms.push_back(df2.Histo1D({"hist_time","",70,-30,40},"lepton_time"));
    histograms.push_back(df2.Histo1D({"hist_radius","",100,0,2.1e5},"lepton_radius"));

    histograms.push_back(df2.Histo1D({"hist_pixel_dEdX","",100,0,100},"track_PixeldEdX"));
    histograms.push_back(df2.Histo1D({"hist_pixel_hits","",10,0,10},"track_PixelHits"));
    histograms.push_back(df2.Histo1D({"hist_sct_hits","",16,0,16},"track_SCTHits"));
    histograms.push_back(df2.Histo1D({"hist_trt_hits","",500,0,50},"track_TRTHits"));

    return df2;
}

void MC_analysis(){

    ROOT::EnableImplicitMT();

    std::vector<ROOT::RDF::RResultPtr<TH1D>> histogramsA;
    std::vector<ROOT::RDF::RResultPtr<TH1D>> histogramsB;


    std::cout<<"Creating dataframes..."<<std::endl;
    ROOT::RDataFrame dfA("G2TauTree","../data/mc_jpsi_ee_sig_sampling.root");
    ROOT::RDataFrame dfB("G2TauTree","../data/mc_jpsi_mumu_sig_sampling.root");
    std::cout<<"Done."<<std::endl;

    std::cout<<"Processing samples and filling histograms..."<<std::endl;
    auto dfA_nn = SampleAnalysis(dfA,0,histogramsA);
    auto dfB_nn = SampleAnalysis(dfB,1,histogramsB);
    std::cout<<"Done."<<std::endl;

    std::vector<std::string> nn_cols = {
        "track_PixelHits",
        "track_SCTHits",
        "track_PixeldEdX",
        "track_TRTHits",
        "lepton_FVariable",
        "lepton_EMCal",
        "lepton_lambda",
        "lepton_lambda2",
        "lepton_time",
        "lepton_radius",
        "truth_particle_tag"
    };

    std::cout<<"Saving ML tree..."<<std::endl;
    dfA_nn.Snapshot("ml_tree","../out/out_ml_MC_ee_with_sig_sampling.root", nn_cols);
    dfB_nn.Snapshot("ml_tree","../out/out_ml_MC_mumu_with_sig_sampling.root", nn_cols);
    std::cout<<"Done."<<std::endl;

    auto nA = dfA_nn.Count(); // RResultPtr<ULong64_t>
    auto nB = dfB_nn.Count();

    std::cout << "[DEBUG] Sample A entries: " << nA.GetValue() << std::endl;
    std::cout << "[DEBUG] Sample B entries: " << nB.GetValue() << std::endl;    

    ROOT::RDataFrame df_all(
        "ml_tree",
        {"../out/out_ml_MC_ee_with_sig_sampling.root","../out/out_ml_MC_mumu_with_sig_sampling.root"}
    );

    df_all.Snapshot("mlMCTree","../out/out_ml_MC_with_sig_sampling.root", nn_cols);
    auto n_all = df_all.Count();
    std::cout << "[DEBUG] All samples entries: " << n_all.GetValue() << std::endl;

    std::cout<<"Saving histograms..."<<std::endl;
    TFile foutA("../out/out_hists_MC_ee_with_sig_sampling.root","RECREATE");
    TFile foutB("../out/out_hists_MC_mumu_with_sig_sampling.root","RECREATE");
    for(auto& h : histogramsA) h->Write();
    for(auto& h : histogramsB) h->Write();
    std::cout<<"Done."<<std::endl;

}