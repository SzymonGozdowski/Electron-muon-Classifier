//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 26 15:24:44 2025 by ROOT version 6.36.04
// from TTree G2TauTree/yy->tautau ntuple
// found on file: data23_2trk_moreTCvars.root
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TMath.h>
#include "TLorentzVector.h"
#include <TH2.h>



// Header file for the classes stored in the TTree if any.
#include <vector>
using std::vector;
#include <string>
using std::string;


class Analysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   Float_t         MuonMass=0.1056583755;
   Float_t         ElectronMass=0.00051099895;
   Double_t        pi=TMath::Pi();
   // Declaration of leaf types
   UInt_t          run_number;
   UInt_t          lumi_block;
   ULong64_t       event_number;
   UInt_t          mc_channel_number;
   Float_t         actIntPerXing;
   Float_t         avgIntPerXing;
   UInt_t          bcid;
   Bool_t          larTightSat;
   Bool_t          larMiniNoiBurT;
   Bool_t          isCleanEvent;
   Bool_t          passed_Trig;
   Bool_t          passed_HLT_noalg_L1ZDC_A_C_VTE50;
   Bool_t          passed_HLT_noalg_L1ZDC_XOR_TE5_VTE200;
   Bool_t          passed_HLT_noalg_L1ZDC_XOR;
   Bool_t          passed_L1_VZDC_A_C_TE5_VTE200;
   Bool_t          passed_L1_VZDC_A_C_TE20_VTE200;
   Bool_t          passed_L1_TAU1_TE4_VTE200;
   Bool_t          passed_L1_2TAU1_VTE50;
   Bool_t          passed_L1_2TAU2_VTE200;
   Bool_t          passed_L1_MU4_VTE50;
   Bool_t          passed_L1_MU0_VTE50;
   Bool_t          passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU1_VTE50;
   Bool_t          passed_HLT_mb_sptrk_vetombts2in_exclusiveloose2_L12TAU1_VTE50;
   Bool_t          passed_HLT_mb_sp_L1VTE50;
   Bool_t          passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU2_VTE200;
   Bool_t          passed_HLT_mu4_hi_upc_FgapAC3_L1MU4_VTE50;
   Bool_t          passed_HLT_mb_sptrk_vetombts2in_L1MU0_VTE50;
   Bool_t          passed_HLT_hi_upc_FgapAC3_hi_gg_upc_noiseSup_L1TE4_VTE200_EMPTY;
   Bool_t          passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L1TAU1_TE4_VTE200;
   Bool_t          passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L12TAU1_VTE50;
   Bool_t          passed_HLT_noalg_L1VTE5;
   Bool_t          passed_HLT_noalg_L1MBTS_1_VTE5;
   Bool_t          passed_HLT_noalg_L1TRT_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE20;
   Bool_t          passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE50;
   Bool_t          passed_HLT_noalg_L1TAU1_TE3_VTE200;
   Bool_t          passed_HLT_noalg_L1TAU1_TE5_VTE200;
   Bool_t          passed_HLT_noalg_L1TAU1_VTE200;
   Bool_t          passed_HLT_noalg_L1TAU1_VTE200_EMPTY;
   Bool_t          passed_HLT_noalg_L1MU3V_EMPTY;
   Bool_t          passed_HLT_noalg_L12TAU1_VTE50;
   Bool_t          passed_HLT_noalg_L12TAU1_VTE200;
   Bool_t          passed_HLT_noalg_L1TE5_VTE20;
   Bool_t          passed_HLT_noalg_L1TE20_VTE200;
   Bool_t          passed_HLT_noalg_bkg_L1MU3V_EMPTY;
   Bool_t          passed_HLT_noalg_L1TE5;
   Bool_t          passed_HLT_noalg_L1TE7p0ETA49;
   Bool_t          passed_HLT_noalg_L1TE20;
   Bool_t          passed_HLT_mb_sptrk_L1VTE5;
   Bool_t          passed_HLT_mb_sptrk_L1MBTS_1_VTE5;
   Bool_t          passed_HLT_mb_sp_pix100_L1RD0_FILLED;
   Bool_t          passed_HLT_mb_sp_pix200_L1RD0_FILLED;
   Bool_t          passed_HLT_mb_sp_pix500_L1RD0_FILLED;
   Bool_t          passed_HLT_mb_sp_pix1000_L1RD0_FILLED;
   Bool_t          passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10;
   Bool_t          passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10;
   Bool_t          passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10;
   Bool_t          passed_HLT_mb_sp_nototpix100_L1ZDC_XOR_VTE10;
   Bool_t          passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10;
   Bool_t          passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10;
   Bool_t          passed_HLT_mb_sp_nototpix70_L1ZDC_A_C_VTE10;
   Bool_t          passed_HLT_mb_sp_nototpix70_L1ZDC_XOR_VTE10;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20;
   Bool_t          passed_HLT_noalg_L1TRT_VTE20;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE50;
   Bool_t          passed_HLT_mb_sptrk_hi_FgapAC5_L1VTE50;
   Bool_t          passed_HLT_mb_sptrk_L1ZDC_XOR_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_VZDC_A_VZDC_C_VTE100;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_VZDC_A_VZDC_C_VTE100;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1eEM1_VZDC_A_VZDC_C_VTE100;
   Bool_t          passed_HLT_mb_sptrk_hi_FgapA5_L11ZDC_A_1ZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_L11ZDC_A_1ZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_L1VZDC_A_VZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1VZDC_A_VZDC_C_VTE200;
   Bool_t          passed_HLT_mu3_hi_FgapAC5_L1MU3V_VTE50;
   Bool_t          passed_HLT_mu3_hi_FgapAC5_L1MU3V_VjTE50;
   Bool_t          passed_HLT_noalg_L1TRT_VTE20_L1_isPassedBeforePrescale;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20_L1_isPassedBeforePrescale;
   Bool_t          passed_L11ZDC_A_1ZDC_C_VTE200_AV;
   Bool_t          passed_L1VZDC_A_VZDC_C_VTE200_AV;
   Bool_t          passed_L1VZDC_A_VZDC_C_VTE200;
   Bool_t          passed_L1ZDC_XOR_VTE200;
   Bool_t          passed_HLT_noalg_L1TRT_FILLED;
   Bool_t          passed_HLT_noalg_L1RD0_FILLED;
   Bool_t          passed_HLT_noalg_L1TE50;
   Bool_t          passed_L1TRT_FILLED_L1_isPassedBeforePrescale;
   Bool_t          passed_L1TAU1_VZDC_A_VZDC_C_VTE100_tbp;
   Bool_t          passed_L1TRT_VZDC_A_VZDC_C_VTE50_tbp;
   Bool_t          passed_HLT_mb_sptrk_L12eEM1_VTE200;
   Bool_t          passed_HLT_mb_sptrk_L1ZDC_1XOR5_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_L1eEM1_VTE200;
   Bool_t          passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VTE200;
   Bool_t          passed_HLT_noalg_L1MBTS_1_VTE50;
   Bool_t          passed_HLT_mb_sptrk_L1VTE50;
   Bool_t          passed_HLT_mb_sptrk_L1TRT_VTE20;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE20;
   Bool_t          passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10_UNPAIRED_NONISO;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L12TAU1_VTE200;
   Bool_t          passed_HLT_mb_sp_vpix30_hi_FgapAC5_L1TAU1_TE4_VTE200;
   Bool_t          passed_HLT_mb_sp_vpix30_hi_FgapAC5_L12TAU1_VTE200;
   Bool_t          passed_HLT_mb_sp_vpix30_hi_FgapAC5_2g0_etcut_L12TAU1_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_TE4_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_L1TAU2_TE4_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE50;
   Bool_t          passed_L1TAU1_TE4_VTE200_tbp;
   Bool_t          passed_L12TAU1_VTE200_tbpsP;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L11ZDC_A_1ZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1ZDC_XOR4_VTE200;
   Bool_t          passed_HLT_noalg_L1ZDC_XOR_VTE200;
   Bool_t          passed_HLT_mb_sp50_trk15_hmt_hi_FgapA5_L1MBTS_1_VZDC_A_ZDC_C_VTE200;
   Bool_t          passed_HLT_mb_sp50_trk15_hmt_hi_FgapC5_L1MBTS_1_ZDC_A_VZDC_C_VTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VTE50;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VZDC_A_VZDC_C_VTE100;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_ZDC_XOR4_VTE100;
   Bool_t          passed_L1TRT_FILLED_L1_isPassedAterVeto;
   Bool_t          passed_L1TRT_VTE20_L1_isPassedAterVeto;
   Float_t         prescale_L1TRT_VTE20;
   Bool_t          passed_HLT_mb_sptrk_hi_FgapA5_L1VZDC_A_ZDC_C_VTE200;
   Bool_t          passed_HLT_mb_sptrk_hi_FgapC5_L1ZDC_A_VZDC_C_VTE200;
   Bool_t          passed_HLT_mb_sptrk_L1ZDC_1XOR5_VjTE200;
   Bool_t          passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VjTE200;
   Bool_t          passed_HLT_mb_sptrk_L1ZDC_XOR_VjTE200;
   Bool_t          passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VjTE20;
   Bool_t          passed_HLT_mb_excl_1trk5_pt0p5_hi_FgapAC5_L1TRT_VjTE20;
   Bool_t          passed_HLT_mb_sp_nototpix50_q2_L1ZDC_A_C_VjTE10;
   Bool_t          passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VjTE10;
   Bool_t          passed_L1TRT_VjTE20_L1_isPassedBeforePrescale;
   Bool_t          passed_L1TRT_VjTE200_L1_isPassedBeforePrescale;
   Bool_t          passed_L1TRT_F_L1_isPassedBeforePrescale;
   Bool_t          passed_L1eTAU1_jTE4_VjTE200;
   Bool_t          passed_L1eEM1_jTE4_VjTE200;
   Bool_t          passed_L12eTAU1_VjTE200;
   Bool_t          passed_L1DPHI_2eEM1_VjTE200;
   Bool_t          passed_HLT_mb_sptrk_L1jTAU1_TRT_ZDC_XOR_VjTE200;
   Bool_t          passed_HLT_mb_sptrk_L1TRT_ZDC_XOR_VjTE200;
   Bool_t          passed_L1jTAU1_TRT_ZDC_XOR_VjTE200;
   Bool_t          passed_HLT_noalg_L1TRT_VjTE50;
   Float_t         L1MU4_sf;
   Float_t         L1MU4_sf_stat_up;
   Float_t         L1MU4_sf_stat_down;
   Float_t         L1MU4_sf_syst_up;
   Float_t         L1MU4_sf_syst_down;
   Float_t         HLT_mu4_sf;
   Float_t         HLT_mu4_sf_stat_up;
   Float_t         HLT_mu4_sf_stat_down;
   Float_t         HLT_mu4_sf_syst_up;
   Float_t         HLT_mu4_sf_syst_down;
   Bool_t          passed_GRL;
   Float_t         zdc_ene_a;
   Float_t         zdc_ene_c;
   UInt_t          zdc_status_a;
   UInt_t          zdc_status_c;
   Float_t         mbts_time_a;
   Float_t         mbts_time_c;
   Float_t         mbts_time_diff;
   UInt_t          mbts_count_a;
   UInt_t          mbts_count_c;
   vector<float>   *truth_electron_eta;
   vector<float>   *truth_electron_phi;
   vector<float>   *truth_electron_pt;
   vector<int>     *truth_electron_charge;
   vector<float>   *truth_muon_eta;
   vector<float>   *truth_muon_phi;
   vector<float>   *truth_muon_pt;
   vector<int>     *truth_muon_charge;
   vector<float>   *truth_photon_eta;
   vector<float>   *truth_photon_phi;
   vector<float>   *truth_photon_pt;
   vector<float>   *truth_hadron_eta;
   vector<float>   *truth_hadron_phi;
   vector<float>   *truth_hadron_pt;
   vector<int>     *truth_hadron_charge;
   vector<float>   *truth_monopole_eta;
   vector<float>   *truth_monopole_phi;
   vector<float>   *truth_monopole_pt;
   vector<float>   *truth_monopole_m;
   vector<float>   *truth_tau_eta;
   vector<float>   *truth_tau_phi;
   vector<float>   *truth_tau_pt;
   vector<int>     *truth_tau_charge;
   vector<float>   *truth_tau_vis_eta;
   vector<float>   *truth_tau_vis_phi;
   vector<float>   *truth_tau_vis_pt;
   vector<float>   *truth_tau_charged_eta;
   vector<float>   *truth_tau_charged_phi;
   vector<float>   *truth_tau_charged_pt;
   vector<float>   *truth_tau_neutralpions_eta;
   vector<float>   *truth_tau_neutralpions_phi;
   vector<float>   *truth_tau_neutralpions_pt;
   vector<int>     *truth_tau_decaymode;
   vector<int>     *truth_tau_hadronicdecaymode;
   vector<int>     *truth_tau_is_electronic;
   vector<int>     *truth_tau_is_muonic;
   vector<int>     *truth_tau_is_hadronic;
   vector<float>   *truth_ditau_y;
   vector<float>   *truth_ditau_eta;
   vector<float>   *truth_ditau_deta;
   vector<float>   *truth_ditau_phi;
   vector<float>   *truth_ditau_aco;
   vector<float>   *truth_ditau_m;
   vector<float>   *truth_ditau_pt;
   vector<float>   *truth_ditau_vis_y;
   vector<float>   *truth_ditau_vis_phi;
   vector<float>   *truth_ditau_vis_aco;
   vector<float>   *truth_ditau_vis_m;
   vector<float>   *truth_ditau_vis_pt;
   vector<float>   *truth_ditau_charged_y;
   vector<float>   *truth_ditau_charged_phi;
   vector<float>   *truth_ditau_charged_aco;
   vector<float>   *truth_ditau_charged_m;
   vector<float>   *truth_ditau_charged_pt;
   Float_t         mg5_to_sl_flux_weight;
   Bool_t          pass_mg5_sl_overlapremoval;
   Float_t         sl_to_sc3_flux_weight;
   Float_t         data_0n0n_flux_weight;
   Float_t         data_0n0n_flux_weight_syst;
   UInt_t          PV_n;
   Float_t         perf_PV_x;
   Float_t         perf_PV_y;
   Float_t         perf_PV_z;
   UInt_t          track_n;
   UInt_t          track_n_LoosePrimary;
   UInt_t          track_n_TightPrimary;
   UInt_t          track_n_HITight;
   UInt_t          track_n_HILoose;
   UInt_t          track_n_200mev;
   UInt_t          track_n_1gev;
   UInt_t          track_n_2gev;
   UInt_t          track_n_eta2;
   vector<float>   *track_eta;
   vector<float>   *track_phi;
   vector<float>   *track_pt;
   vector<float>   *track_theta;
   vector<int>     *track_charge;
   vector<bool>    *track_is_LoosePrimary;
   vector<bool>    *track_is_TightPrimary;
   vector<bool>    *track_is_HITight;
   vector<bool>    *track_is_HILoose;
   vector<bool>    *track_is_matched_to_electron;
   vector<bool>    *track_is_matched_to_muon;
   vector<float>   *track_d0;
   vector<float>   *track_d0sig;
   vector<float>   *track_z0;
   vector<float>   *track_z0Abs;
   vector<float>   *track_z0sinTheta;
   vector<float>   *track_PixeldEdX;
   vector<float>   *track_TRTHits;
   vector<float>   *track_TRTHighThresholdHits;
   vector<float>   *track_TRTHighThresholdHitsTotal;
   vector<float>   *track_truthMatchProbability;
   vector<bool>    *track_fail_syst_OverallIDmat;
   vector<bool>    *track_fail_syst_PP0mat;
   vector<bool>    *track_fail_syst_IBLmat;
   vector<bool>    *track_fail_syst_QGSP_BIC;
   vector<float>   *track_PixelHits;
   vector<float>   *track_SCTHits;
   UInt_t          pix_track_n;
   vector<float>   *pix_track_eta;
   vector<float>   *pix_track_phi;
   vector<float>   *pix_track_pt;
   vector<float>   *pix_track_theta;
   vector<int>     *pix_track_charge;
   vector<int>     *pix_track_hits;
   vector<float>   *pix_track_d0;
   vector<float>   *pix_track_d0sig;
   vector<float>   *pix_track_z0;
   vector<float>   *pix_track_z0Abs;
   vector<float>   *pix_track_z0sinTheta;
   vector<float>   *pix_track_PixeldEdX;
   vector<float>   *perf_pix_track_dr_to_stable;
   vector<float>   *perf_pix_track_matched_pt;
   vector<float>   *perf_pix_track_matched_eta;
   vector<float>   *perf_pix_track_matched_phi;
   vector<int>     *perf_pix_track_matched_pdg;
   Int_t           pix_sp_count_endcap_A;
   Int_t           pix_sp_count_Barrel;
   Int_t           pix_sp_count_endcap_C;
   Int_t           pix_sp_count_noToT;
   vector<float>   *siSpacePoint_x;
   vector<float>   *siSpacePoint_y;
   vector<float>   *siSpacePoint_z;
   vector<float>   *siSpacePoint_tot;
   vector<short>   *siSpacePoint_csize;
   vector<unsigned int> *siSpacePoint_module1;
   vector<unsigned int> *siSpacePoint_module2;
   Int_t           siSpacePoint_n;
   Int_t           pixSpacePoint_n;
   vector<float>   *l1_emtau_eta;
   vector<float>   *l1_emtau_phi;
   vector<float>   *l1_emtau_et;
   vector<unsigned int> *l1_emtau_threshold_pattern;
   vector<float>   *l1_muon_eta;
   vector<float>   *l1_muon_phi;
   vector<float>   *l1_muon_threshold;
   Float_t         l1_te;
   vector<float>   *HLT_muon_eta;
   vector<float>   *HLT_muon_phi;
   vector<float>   *HLT_muon_pt;
   UInt_t          topoclus_n;
   Float_t         topoclus_ht;
   vector<float>   *topo_cluster_eta;
   vector<float>   *topo_cluster_phi;
   vector<float>   *topo_cluster_pt;
   vector<float>   *topo_cluster_lambda;
   vector<float>   *topo_cluster_lambda2;
   vector<float>   *topo_cluster_r2;
   vector<bool>    *topo_cluster_pass_sig_cut;
   vector<bool>    *topo_cluster_pass_hotspot_cleaning;
   vector<float>   *topo_cluster_cell_significance;
   vector<int>     *topo_cluster_cell_sig_sampling;
   vector<float>   *topo_cluster_EM_prob;
   vector<float>   *topo_cluster_pt_syst_CALIB__1up;
   vector<bool>    *topo_cluster_fail_syst_eff_sampling;
   vector<float>   *topo_cluster_time;
   vector<float>   *topo_cluster_ENG_FRAC_EM;
   vector<float>   *eg_cluster_eta;
   vector<float>   *eg_cluster_phi;
   vector<float>   *eg_cluster_et;
   UInt_t          eg_cluster_n;
   vector<float>   *photon_eta;
   vector<float>   *photon_phi;
   vector<float>   *photon_pt;
   vector<float>   *photon_f1;
   vector<float>   *photon_f3;
   vector<float>   *photon_f1core;
   vector<float>   *photon_f3core;
   vector<float>   *photon_weta1;
   vector<float>   *photon_weta2;
   vector<float>   *photon_fracs1;
   vector<float>   *photon_wtots1;
   vector<float>   *photon_e277;
   vector<float>   *photon_Reta;
   vector<float>   *photon_Rphi;
   vector<float>   *photon_Eratio;
   vector<float>   *photon_Rhad;
   vector<float>   *photon_Rhad1;
   vector<float>   *photon_DeltaE;
   vector<bool>    *photon_pass_NN_PID;
   vector<bool>    *photon_fail_syst_eff_sampling;
   Int_t           nPhot;
   vector<float>   *electron_eta;
   vector<float>   *electron_phi;
   vector<float>   *electron_pt;
   vector<float>   *electron_theta;
   vector<int>     *electron_charge;
   vector<bool>    *electron_is_LHVeryLoose;
   vector<bool>    *electron_is_LHLoose;
   vector<bool>    *electron_is_LHMedium;
   vector<bool>    *electron_is_LHTight;
   vector<char>    *electron_trigger_thresholds_passed;
   vector<float>   *electron_d0;
   vector<float>   *electron_d0sig;
   vector<float>   *electron_z0;
   vector<float>   *electron_z0Abs;
   vector<float>   *electron_z0sinTheta;
   vector<float>   *electron_recoid_sf;
   vector<float>   *electron_recoid_sf_stat_up;
   vector<float>   *electron_recoid_sf_stat_down;
   vector<float>   *electron_recoid_sf_syst_up;
   vector<float>   *electron_recoid_sf_syst_down;
   Int_t           nElec;
   Float_t         acolt_et;
   Float_t         Ptlt_et;
   Float_t         mlt_et;
   Float_t         ylt_et;
   Float_t         dRlt_et;
   Float_t         dPhilt_et;
   Float_t         dEtalt_et;
   Float_t         acolts_ets;
   Float_t         Ptlts_ets;
   Float_t         mlts_ets;
   Float_t         ylts_ets;
   Float_t         dRlts_ets;
   Float_t         dPhilts_ets;
   Float_t         dEtalts_ets;
   Float_t         acoll_ee;
   Float_t         Ptll_ee;
   Float_t         mll_ee;
   Float_t         yll_ee;
   Float_t         dRll_ee;
   Float_t         dPhill_ee;
   Float_t         dEtall_ee;
   vector<float>   *muon_eta;
   vector<float>   *muon_phi;
   vector<float>   *muon_pt;
   vector<float>   *muon_theta;
   vector<int>     *muon_charge;
   vector<bool>    *muon_is_Loose;
   vector<bool>    *muon_is_Medium;
   vector<bool>    *muon_is_Tight;
   vector<bool>    *muon_is_LowPt;
   vector<bool>    *muon_is_MVALowPt;
   vector<bool>    *muon_trigger_matched_L1;
   vector<bool>    *muon_trigger_matched_HLT;
   vector<float>   *muon_d0;
   vector<float>   *muon_d0sig;
   vector<float>   *muon_z0;
   vector<float>   *muon_z0Abs;
   vector<float>   *muon_z0sinTheta;
   vector<float>   *muon_p_ID;
   vector<float>   *muon_p_MS;
   vector<float>   *muon_energy_loss;
   vector<float>   *muon_recoid_sf;
   vector<float>   *muon_recoid_sf_stat_up;
   vector<float>   *muon_recoid_sf_stat_down;
   vector<float>   *muon_recoid_sf_syst_up;
   vector<float>   *muon_recoid_sf_syst_down;
   Int_t           nMuon;
   vector<float>   *MSmuon_eta;
   vector<float>   *MSmuon_phi;
   vector<float>   *MSmuon_pt;
   vector<float>   *MSmuon_d0;
   Float_t         acolt_mt;
   Float_t         Ptlt_mt;
   Float_t         mlt_mt;
   Float_t         ylt_mt;
   Float_t         dRlt_mt;
   Float_t         dPhilt_mt;
   Float_t         dEtalt_mt;
   Float_t         acolts_mts;
   Float_t         Ptlts_mts;
   Float_t         mlts_mts;
   Float_t         ylts_mts;
   Float_t         dRlts_mts;
   Float_t         dPhilts_mts;
   Float_t         dEtalts_mts;
   Float_t         acoll_mm;
   Float_t         Ptll_mm;
   Float_t         mll_mm;
   Float_t         yll_mm;
   Float_t         dRll_mm;
   Float_t         dPhill_mm;
   Float_t         dEtall_mm;
   Float_t         acoll_em;
   Float_t         Ptll_em;
   Float_t         mll_em;
   Float_t         yll_em;
   Float_t         dRll_em;
   Float_t         dPhill_em;
   Float_t         dEtall_em;
   Float_t         mts;
   Float_t         Ptts;
   Float_t         yts;
   Float_t         Phits;
   Float_t         Etats;
   Float_t         Pttt_tt;
   Float_t         Pttts_tts;
   Float_t         bsm_sig_weight_atau_p010;
   Float_t         bsm_sig_weight_atau_p001;
   Float_t         bsm_sig_weight_atau_p002;
   Float_t         bsm_sig_weight_atau_p003;
   Float_t         bsm_sig_weight_atau_p004;
   Float_t         bsm_sig_weight_atau_p005;
   Float_t         bsm_sig_weight_atau_p006;
   Float_t         bsm_sig_weight_atau_m010;
   Float_t         bsm_sig_weight_atau_m001;
   Float_t         bsm_sig_weight_atau_m002;
   Float_t         bsm_sig_weight_atau_m003;
   Float_t         bsm_sig_weight_atau_m004;
   Float_t         bsm_sig_weight_atau_m005;
   Float_t         bsm_sig_weight_atau_m006;
   vector<float>   *electron_pt_syst_EG_RESOLUTION_ALL__1down;
   vector<float>   *photon_pt_syst_EG_RESOLUTION_ALL__1down;
   vector<float>   *electron_pt_syst_EG_RESOLUTION_ALL__1up;
   vector<float>   *photon_pt_syst_EG_RESOLUTION_ALL__1up;
   vector<float>   *electron_pt_syst_EG_SCALE_ALL__1down;
   vector<float>   *photon_pt_syst_EG_SCALE_ALL__1down;
   vector<float>   *electron_pt_syst_EG_SCALE_ALL__1up;
   vector<float>   *photon_pt_syst_EG_SCALE_ALL__1up;
   vector<float>   *muon_pt_syst_MUON_ID__1down;
   vector<float>   *muon_pt_syst_MUON_ID__1up;
   vector<float>   *muon_pt_syst_MUON_MS__1down;
   vector<float>   *muon_pt_syst_MUON_MS__1up;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_DATASTAT__1down;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_DATASTAT__1up;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_GLOBAL__1down;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_GLOBAL__1up;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_RESBIAS__1down;
   vector<float>   *muon_pt_syst_MUON_SAGITTA_RESBIAS__1up;
   vector<float>   *muon_pt_syst_MUON_SCALE__1down;
   vector<float>   *muon_pt_syst_MUON_SCALE__1up;

   // List of branches
   TBranch        *b_run_number;   //!
   TBranch        *b_lumi_block;   //!
   TBranch        *b_event_number;   //!
   TBranch        *b_mc_channel_number;   //!
   TBranch        *b_actIntPerXing;   //!
   TBranch        *b_avgIntPerXing;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_larTightSat;   //!
   TBranch        *b_larMiniNoiBurT;   //!
   TBranch        *b_isCleanEvent;   //!
   TBranch        *b_passed_Trig;   //!
   TBranch        *b_passed_HLT_noalg_L1ZDC_A_C_VTE50;   //!
   TBranch        *b_passed_HLT_noalg_L1ZDC_XOR_TE5_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1ZDC_XOR;   //!
   TBranch        *b_passed_L1_VZDC_A_C_TE5_VTE200;   //!
   TBranch        *b_passed_L1_VZDC_A_C_TE20_VTE200;   //!
   TBranch        *b_passed_L1_TAU1_TE4_VTE200;   //!
   TBranch        *b_passed_L1_2TAU1_VTE50;   //!
   TBranch        *b_passed_L1_2TAU2_VTE200;   //!
   TBranch        *b_passed_L1_MU4_VTE50;   //!
   TBranch        *b_passed_L1_MU0_VTE50;   //!
   TBranch        *b_passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU1_VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_vetombts2in_exclusiveloose2_L12TAU1_VTE50;   //!
   TBranch        *b_passed_HLT_mb_sp_L1VTE50;   //!
   TBranch        *b_passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU2_VTE200;   //!
   TBranch        *b_passed_HLT_mu4_hi_upc_FgapAC3_L1MU4_VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_vetombts2in_L1MU0_VTE50;   //!
   TBranch        *b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_noiseSup_L1TE4_VTE200_EMPTY;   //!
   TBranch        *b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L1TAU1_TE4_VTE200;   //!
   TBranch        *b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L12TAU1_VTE50;   //!
   TBranch        *b_passed_HLT_noalg_L1VTE5;   //!
   TBranch        *b_passed_HLT_noalg_L1MBTS_1_VTE5;   //!
   TBranch        *b_passed_HLT_noalg_L1TRT_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE20;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE50;   //!
   TBranch        *b_passed_HLT_noalg_L1TAU1_TE3_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TAU1_TE5_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TAU1_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TAU1_VTE200_EMPTY;   //!
   TBranch        *b_passed_HLT_noalg_L1MU3V_EMPTY;   //!
   TBranch        *b_passed_HLT_noalg_L12TAU1_VTE50;   //!
   TBranch        *b_passed_HLT_noalg_L12TAU1_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TE5_VTE20;   //!
   TBranch        *b_passed_HLT_noalg_L1TE20_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_bkg_L1MU3V_EMPTY;   //!
   TBranch        *b_passed_HLT_noalg_L1TE5;   //!
   TBranch        *b_passed_HLT_noalg_L1TE7p0ETA49;   //!
   TBranch        *b_passed_HLT_noalg_L1TE20;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1VTE5;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1MBTS_1_VTE5;   //!
   TBranch        *b_passed_HLT_mb_sp_pix100_L1RD0_FILLED;   //!
   TBranch        *b_passed_HLT_mb_sp_pix200_L1RD0_FILLED;   //!
   TBranch        *b_passed_HLT_mb_sp_pix500_L1RD0_FILLED;   //!
   TBranch        *b_passed_HLT_mb_sp_pix1000_L1RD0_FILLED;   //!
   TBranch        *b_passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10;   //!
   TBranch        *b_passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix100_L1ZDC_XOR_VTE10;   //!
   TBranch        *b_passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10;   //!
   TBranch        *b_passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix70_L1ZDC_A_C_VTE10;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix70_L1ZDC_XOR_VTE10;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20;   //!
   TBranch        *b_passed_HLT_noalg_L1TRT_VTE20;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_hi_FgapAC5_L1VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1ZDC_XOR_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_VZDC_A_VZDC_C_VTE100;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_VZDC_A_VZDC_C_VTE100;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1eEM1_VZDC_A_VZDC_C_VTE100;   //!
   TBranch        *b_passed_HLT_mb_sptrk_hi_FgapA5_L11ZDC_A_1ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_L11ZDC_A_1ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_L1VZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1VZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mu3_hi_FgapAC5_L1MU3V_VTE50;   //!
   TBranch        *b_passed_HLT_mu3_hi_FgapAC5_L1MU3V_VjTE50;   //!
   TBranch        *b_passed_HLT_noalg_L1TRT_VTE20_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_L11ZDC_A_1ZDC_C_VTE200_AV;   //!
   TBranch        *b_passed_L1VZDC_A_VZDC_C_VTE200_AV;   //!
   TBranch        *b_passed_L1VZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_L1ZDC_XOR_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TRT_FILLED;   //!
   TBranch        *b_passed_HLT_noalg_L1RD0_FILLED;   //!
   TBranch        *b_passed_HLT_noalg_L1TE50;   //!
   TBranch        *b_passed_L1TRT_FILLED_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_L1TAU1_VZDC_A_VZDC_C_VTE100_tbp;   //!
   TBranch        *b_passed_L1TRT_VZDC_A_VZDC_C_VTE50_tbp;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L12eEM1_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1ZDC_1XOR5_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_L1eEM1_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1MBTS_1_VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1VTE50;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1TRT_VTE20;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE20;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10_UNPAIRED_NONISO;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L12TAU1_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_L1TAU1_TE4_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_L12TAU1_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_2g0_etcut_L12TAU1_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_TE4_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_L1TAU2_TE4_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE50;   //!
   TBranch        *b_passed_L1TAU1_TE4_VTE200_tbp;   //!
   TBranch        *b_passed_L12TAU1_VTE200_tbp ;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L11ZDC_A_1ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1ZDC_XOR4_VTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1ZDC_XOR_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sp50_trk15_hmt_hi_FgapA5_L1MBTS_1_VZDC_A_ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sp50_trk15_hmt_hi_FgapC5_L1MBTS_1_ZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VTE50;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VZDC_A_VZDC_C_VTE100;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_ZDC_XOR4_VTE100;   //!
   TBranch        *b_passed_L1TRT_FILLED_L1_isPassedAterVeto;   //!
   TBranch        *b_passed_L1TRT_VTE20_L1_isPassedAterVeto;   //!
   TBranch        *b_prescale_L1TRT_VTE20;   //!
   TBranch        *b_passed_HLT_mb_sptrk_hi_FgapA5_L1VZDC_A_ZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_hi_FgapC5_L1ZDC_A_VZDC_C_VTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1ZDC_1XOR5_VjTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VjTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1ZDC_XOR_VjTE200;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VjTE20;   //!
   TBranch        *b_passed_HLT_mb_excl_1trk5_pt0p5_hi_FgapAC5_L1TRT_VjTE20;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix50_q2_L1ZDC_A_C_VjTE10;   //!
   TBranch        *b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VjTE10;   //!
   TBranch        *b_passed_L1TRT_VjTE20_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_L1TRT_VjTE200_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_L1TRT_F_L1_isPassedBeforePrescale;   //!
   TBranch        *b_passed_L1eTAU1_jTE4_VjTE200;   //!
   TBranch        *b_passed_L1eEM1_jTE4_VjTE200;   //!
   TBranch        *b_passed_L12eTAU1_VjTE200;   //!
   TBranch        *b_passed_L1DPHI_2eEM1_VjTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1jTAU1_TRT_ZDC_XOR_VjTE200;   //!
   TBranch        *b_passed_HLT_mb_sptrk_L1TRT_ZDC_XOR_VjTE200;   //!
   TBranch        *b_passed_L1jTAU1_TRT_ZDC_XOR_VjTE200;   //!
   TBranch        *b_passed_HLT_noalg_L1TRT_VjTE50;   //!
   TBranch        *b_L1MU4_sf;   //!
   TBranch        *b_L1MU4_sf_stat_up;   //!
   TBranch        *b_L1MU4_sf_stat_down;   //!
   TBranch        *b_L1MU4_sf_syst_up;   //!
   TBranch        *b_L1MU4_sf_syst_down;   //!
   TBranch        *b_HLT_mu4_sf;   //!
   TBranch        *b_HLT_mu4_sf_stat_up;   //!
   TBranch        *b_HLT_mu4_sf_stat_down;   //!
   TBranch        *b_HLT_mu4_sf_syst_up;   //!
   TBranch        *b_HLT_mu4_sf_syst_down;   //!
   TBranch        *b_passed_GRL;   //!
   TBranch        *b_zdc_ene_a;   //!
   TBranch        *b_zdc_ene_c;   //!
   TBranch        *b_zdc_status_a;   //!
   TBranch        *b_zdc_status_c;   //!
   TBranch        *b_mbts_time_a;   //!
   TBranch        *b_mbts_time_c;   //!
   TBranch        *b_mbts_time_diff;   //!
   TBranch        *b_mbts_count_a;   //!
   TBranch        *b_mbts_count_c;   //!
   TBranch        *b_truth_electron_eta;   //!
   TBranch        *b_truth_electron_phi;   //!
   TBranch        *b_truth_electron_pt;   //!
   TBranch        *b_truth_electron_charge;   //!
   TBranch        *b_truth_muon_eta;   //!
   TBranch        *b_truth_muon_phi;   //!
   TBranch        *b_truth_muon_pt;   //!
   TBranch        *b_truth_muon_charge;   //!
   TBranch        *b_truth_photon_eta;   //!
   TBranch        *b_truth_photon_phi;   //!
   TBranch        *b_truth_photon_pt;   //!
   TBranch        *b_truth_hadron_eta;   //!
   TBranch        *b_truth_hadron_phi;   //!
   TBranch        *b_truth_hadron_pt;   //!
   TBranch        *b_truth_hadron_charge;   //!
   TBranch        *b_truth_monopole_eta;   //!
   TBranch        *b_truth_monopole_phi;   //!
   TBranch        *b_truth_monopole_pt;   //!
   TBranch        *b_truth_monopole_m;   //!
   TBranch        *b_truth_tau_eta;   //!
   TBranch        *b_truth_tau_phi;   //!
   TBranch        *b_truth_tau_pt;   //!
   TBranch        *b_truth_tau_charge;   //!
   TBranch        *b_truth_tau_vis_eta;   //!
   TBranch        *b_truth_tau_vis_phi;   //!
   TBranch        *b_truth_tau_vis_pt;   //!
   TBranch        *b_truth_tau_charged_eta;   //!
   TBranch        *b_truth_tau_charged_phi;   //!
   TBranch        *b_truth_tau_charged_pt;   //!
   TBranch        *b_truth_tau_neutralpions_eta;   //!
   TBranch        *b_truth_tau_neutralpions_phi;   //!
   TBranch        *b_truth_tau_neutralpions_pt;   //!
   TBranch        *b_truth_tau_decaymode;   //!
   TBranch        *b_truth_tau_hadronicdecaymode;   //!
   TBranch        *b_truth_tau_is_electronic;   //!
   TBranch        *b_truth_tau_is_muonic;   //!
   TBranch        *b_truth_tau_is_hadronic;   //!
   TBranch        *b_truth_ditau_y;   //!
   TBranch        *b_truth_ditau_eta;   //!
   TBranch        *b_truth_ditau_deta;   //!
   TBranch        *b_truth_ditau_phi;   //!
   TBranch        *b_truth_ditau_aco;   //!
   TBranch        *b_truth_ditau_m;   //!
   TBranch        *b_truth_ditau_pt;   //!
   TBranch        *b_truth_ditau_vis_y;   //!
   TBranch        *b_truth_ditau_vis_phi;   //!
   TBranch        *b_truth_ditau_vis_aco;   //!
   TBranch        *b_truth_ditau_vis_m;   //!
   TBranch        *b_truth_ditau_vis_pt;   //!
   TBranch        *b_truth_ditau_charged_y;   //!
   TBranch        *b_truth_ditau_charged_phi;   //!
   TBranch        *b_truth_ditau_charged_aco;   //!
   TBranch        *b_truth_ditau_charged_m;   //!
   TBranch        *b_truth_ditau_charged_pt;   //!
   TBranch        *b_mg5_to_sl_flux_weight;   //!
   TBranch        *b_pass_mg5_sl_overlapremoval;   //!
   TBranch        *b_sl_to_sc3_flux_weight;   //!
   TBranch        *b_data_0n0n_flux_weight;   //!
   TBranch        *b_data_0n0n_flux_weight_syst;   //!
   TBranch        *b_PV_n;   //!
   TBranch        *b_perf_PV_x;   //!
   TBranch        *b_perf_PV_y;   //!
   TBranch        *b_perf_PV_z;   //!
   TBranch        *b_track_n;   //!
   TBranch        *b_track_n_LoosePrimary;   //!
   TBranch        *b_track_n_TightPrimary;   //!
   TBranch        *b_track_n_HITight;   //!
   TBranch        *b_track_n_HILoose;   //!
   TBranch        *b_track_n_200mev;   //!
   TBranch        *b_track_n_1gev;   //!
   TBranch        *b_track_n_2gev;   //!
   TBranch        *b_track_n_eta2;   //!
   TBranch        *b_track_eta;   //!
   TBranch        *b_track_phi;   //!
   TBranch        *b_track_pt;   //!
   TBranch        *b_track_theta;   //!
   TBranch        *b_track_charge;   //!
   TBranch        *b_track_is_LoosePrimary;   //!
   TBranch        *b_track_is_TightPrimary;   //!
   TBranch        *b_track_is_HITight;   //!
   TBranch        *b_track_is_HILoose;   //!
   TBranch        *b_track_is_matched_to_electron;   //!
   TBranch        *b_track_is_matched_to_muon;   //!
   TBranch        *b_track_d0;   //!
   TBranch        *b_track_d0sig;   //!
   TBranch        *b_track_z0;   //!
   TBranch        *b_track_z0Abs;   //!
   TBranch        *b_track_z0sinTheta;   //!
   TBranch        *b_track_PixeldEdX;   //!
   TBranch        *b_track_TRTHits;   //!
   TBranch        *b_track_TRTHighThresholdHits;   //!
   TBranch        *b_track_TRTHighThresholdHitsTotal;   //!
   TBranch        *b_track_truthMatchProbability;   //!
   TBranch        *b_track_fail_syst_OverallIDmat;   //!
   TBranch        *b_track_fail_syst_PP0mat;   //!
   TBranch        *b_track_fail_syst_IBLmat;   //!
   TBranch        *b_track_fail_syst_QGSP_BIC;   //!
   TBranch        *b_track_PixelHits;   //!
   TBranch        *b_track_SCTHits;   //!
   TBranch        *b_pix_track_n;   //!
   TBranch        *b_pix_track_eta;   //!
   TBranch        *b_pix_track_phi;   //!
   TBranch        *b_pix_track_pt;   //!
   TBranch        *b_pix_track_theta;   //!
   TBranch        *b_pix_track_charge;   //!
   TBranch        *b_pix_track_hits;   //!
   TBranch        *b_pix_track_d0;   //!
   TBranch        *b_pix_track_d0sig;   //!
   TBranch        *b_pix_track_z0;   //!
   TBranch        *b_pix_track_z0Abs;   //!
   TBranch        *b_pix_track_z0sinTheta;   //!
   TBranch        *b_pix_track_PixeldEdX;   //!
   TBranch        *b_perf_pix_track_dr_to_stable;   //!
   TBranch        *b_perf_pix_track_matched_pt;   //!
   TBranch        *b_perf_pix_track_matched_eta;   //!
   TBranch        *b_perf_pix_track_matched_phi;   //!
   TBranch        *b_perf_pix_track_matched_pdg;   //!
   TBranch        *b_pix_sp_count_endcap_A;   //!
   TBranch        *b_pix_sp_count_Barrel;   //!
   TBranch        *b_pix_sp_count_endcap_C;   //!
   TBranch        *b_pix_sp_count_noToT;   //!
   TBranch        *b_siSpacePoint_x;   //!
   TBranch        *b_siSpacePoint_y;   //!
   TBranch        *b_siSpacePoint_z;   //!
   TBranch        *b_siSpacePoint_tot;   //!
   TBranch        *b_siSpacePoint_csize;   //!
   TBranch        *b_siSpacePoint_module1;   //!
   TBranch        *b_siSpacePoint_module2;   //!
   TBranch        *b_siSpacePoint_n;   //!
   TBranch        *b_pixSpacePoint_n;   //!
   TBranch        *b_l1_emtau_eta;   //!
   TBranch        *b_l1_emtau_phi;   //!
   TBranch        *b_l1_emtau_et;   //!
   TBranch        *b_l1_emtau_threshold_pattern;   //!
   TBranch        *b_l1_muon_eta;   //!
   TBranch        *b_l1_muon_phi;   //!
   TBranch        *b_l1_muon_threshold;   //!
   TBranch        *b_l1_te;   //!
   TBranch        *b_HLT_muon_eta;   //!
   TBranch        *b_HLT_muon_phi;   //!
   TBranch        *b_HLT_muon_pt;   //!
   TBranch        *b_topoclus_n;   //!
   TBranch        *b_topoclus_ht;   //!
   TBranch        *b_topo_cluster_eta;   //!
   TBranch        *b_topo_cluster_phi;   //!
   TBranch        *b_topo_cluster_pt;   //!
   TBranch        *b_topo_cluster_lambda;   //!
   TBranch        *b_topo_cluster_lambda2;   //!
   TBranch        *b_topo_cluster_r2;   //!
   TBranch        *b_topo_cluster_pass_sig_cut;   //!
   TBranch        *b_topo_cluster_pass_hotspot_cleaning;   //!
   TBranch        *b_topo_cluster_cell_significance;   //!
   TBranch        *b_topo_cluster_cell_sig_sampling;   //!
   TBranch        *b_topo_cluster_EM_prob;   //!
   TBranch        *b_topo_cluster_pt_syst_CALIB__1up;   //!
   TBranch        *b_topo_cluster_fail_syst_eff_sampling;   //!
   TBranch        *b_topo_cluster_time;   //!
   TBranch        *b_topo_cluster_ENG_FRAC_EM;   //!
   TBranch        *b_eg_cluster_eta;   //!
   TBranch        *b_eg_cluster_phi;   //!
   TBranch        *b_eg_cluster_et;   //!
   TBranch        *b_eg_cluster_n;   //!
   TBranch        *b_photon_eta;   //!
   TBranch        *b_photon_phi;   //!
   TBranch        *b_photon_pt;   //!
   TBranch        *b_photon_f1;   //!
   TBranch        *b_photon_f3;   //!
   TBranch        *b_photon_f1core;   //!
   TBranch        *b_photon_f3core;   //!
   TBranch        *b_photon_weta1;   //!
   TBranch        *b_photon_weta2;   //!
   TBranch        *b_photon_fracs1;   //!
   TBranch        *b_photon_wtots1;   //!
   TBranch        *b_photon_e277;   //!
   TBranch        *b_photon_Reta;   //!
   TBranch        *b_photon_Rphi;   //!
   TBranch        *b_photon_Eratio;   //!
   TBranch        *b_photon_Rhad;   //!
   TBranch        *b_photon_Rhad1;   //!
   TBranch        *b_photon_DeltaE;   //!
   TBranch        *b_photon_pass_NN_PID;   //!
   TBranch        *b_photon_fail_syst_eff_sampling;   //!
   TBranch        *b_nPhot;   //!
   TBranch        *b_electron_eta;   //!
   TBranch        *b_electron_phi;   //!
   TBranch        *b_electron_pt;   //!
   TBranch        *b_electron_theta;   //!
   TBranch        *b_electron_charge;   //!
   TBranch        *b_electron_is_LHVeryLoose;   //!
   TBranch        *b_electron_is_LHLoose;   //!
   TBranch        *b_electron_is_LHMedium;   //!
   TBranch        *b_electron_is_LHTight;   //!
   TBranch        *b_electron_trigger_thresholds_passed;   //!
   TBranch        *b_electron_d0;   //!
   TBranch        *b_electron_d0sig;   //!
   TBranch        *b_electron_z0;   //!
   TBranch        *b_electron_z0Abs;   //!
   TBranch        *b_electron_z0sinTheta;   //!
   TBranch        *b_electron_recoid_sf;   //!
   TBranch        *b_electron_recoid_sf_stat_up;   //!
   TBranch        *b_electron_recoid_sf_stat_down;   //!
   TBranch        *b_electron_recoid_sf_syst_up;   //!
   TBranch        *b_electron_recoid_sf_syst_down;   //!
   TBranch        *b_nElec;   //!
   TBranch        *b_acolt_et;   //!
   TBranch        *b_Ptlt_et;   //!
   TBranch        *b_mlt_et;   //!
   TBranch        *b_ylt_et;   //!
   TBranch        *b_dRlt_et;   //!
   TBranch        *b_dPhilt_et;   //!
   TBranch        *b_dEtalt_et;   //!
   TBranch        *b_acolts_ets;   //!
   TBranch        *b_Ptlts_ets;   //!
   TBranch        *b_mlts_ets;   //!
   TBranch        *b_ylts_ets;   //!
   TBranch        *b_dRlts_ets;   //!
   TBranch        *b_dPhilts_ets;   //!
   TBranch        *b_dEtalts_ets;   //!
   TBranch        *b_acoll_ee;   //!
   TBranch        *b_Ptll_ee;   //!
   TBranch        *b_mll_ee;   //!
   TBranch        *b_yll_ee;   //!
   TBranch        *b_dRll_ee;   //!
   TBranch        *b_dPhill_ee;   //!
   TBranch        *b_dEtall_ee;   //!
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_phi;   //!
   TBranch        *b_muon_pt;   //!
   TBranch        *b_muon_theta;   //!
   TBranch        *b_muon_charge;   //!
   TBranch        *b_muon_is_Loose;   //!
   TBranch        *b_muon_is_Medium;   //!
   TBranch        *b_muon_is_Tight;   //!
   TBranch        *b_muon_is_LowPt;   //!
   TBranch        *b_muon_is_MVALowPt;   //!
   TBranch        *b_muon_trigger_matched_L1;   //!
   TBranch        *b_muon_trigger_matched_HLT;   //!
   TBranch        *b_muon_d0;   //!
   TBranch        *b_muon_d0sig;   //!
   TBranch        *b_muon_z0;   //!
   TBranch        *b_muon_z0Abs;   //!
   TBranch        *b_muon_z0sinTheta;   //!
   TBranch        *b_muon_p_ID;   //!
   TBranch        *b_muon_p_MS;   //!
   TBranch        *b_muon_energy_loss;   //!
   TBranch        *b_muon_recoid_sf;   //!
   TBranch        *b_muon_recoid_sf_stat_up;   //!
   TBranch        *b_muon_recoid_sf_stat_down;   //!
   TBranch        *b_muon_recoid_sf_syst_up;   //!
   TBranch        *b_muon_recoid_sf_syst_down;   //!
   TBranch        *b_nMuon;   //!
   TBranch        *b_MSmuon_eta;   //!
   TBranch        *b_MSmuon_phi;   //!
   TBranch        *b_MSmuon_pt;   //!
   TBranch        *b_MSmuon_d0;   //!
   TBranch        *b_acolt_mt;   //!
   TBranch        *b_Ptlt_mt;   //!
   TBranch        *b_mlt_mt;   //!
   TBranch        *b_ylt_mt;   //!
   TBranch        *b_dRlt_mt;   //!
   TBranch        *b_dPhilt_mt;   //!
   TBranch        *b_dEtalt_mt;   //!
   TBranch        *b_acolts_mts;   //!
   TBranch        *b_Ptlts_mts;   //!
   TBranch        *b_mlts_mts;   //!
   TBranch        *b_ylts_mts;   //!
   TBranch        *b_dRlts_mts;   //!
   TBranch        *b_dPhilts_mts;   //!
   TBranch        *b_dEtalts_mts;   //!
   TBranch        *b_acoll_mm;   //!
   TBranch        *b_Ptll_mm;   //!
   TBranch        *b_mll_mm;   //!
   TBranch        *b_yll_mm;   //!
   TBranch        *b_dRll_mm;   //!
   TBranch        *b_dPhill_mm;   //!
   TBranch        *b_dEtall_mm;   //!
   TBranch        *b_acoll_em;   //!
   TBranch        *b_Ptll_em;   //!
   TBranch        *b_mll_em;   //!
   TBranch        *b_yll_em;   //!
   TBranch        *b_dRll_em;   //!
   TBranch        *b_dPhill_em;   //!
   TBranch        *b_dEtall_em;   //!
   TBranch        *b_mts;   //!
   TBranch        *b_Ptts;   //!
   TBranch        *b_yts;   //!
   TBranch        *b_Phits;   //!
   TBranch        *b_Etats;   //!
   TBranch        *b_Pttt_tt;   //!
   TBranch        *b_Pttts_tts;   //!
   TBranch        *b_bsm_sig_weight_atau_p010;   //!
   TBranch        *b_bsm_sig_weight_atau_p001;   //!
   TBranch        *b_bsm_sig_weight_atau_p002;   //!
   TBranch        *b_bsm_sig_weight_atau_p003;   //!
   TBranch        *b_bsm_sig_weight_atau_p004;   //!
   TBranch        *b_bsm_sig_weight_atau_p005;   //!
   TBranch        *b_bsm_sig_weight_atau_p006;   //!
   TBranch        *b_bsm_sig_weight_atau_m010;   //!
   TBranch        *b_bsm_sig_weight_atau_m001;   //!
   TBranch        *b_bsm_sig_weight_atau_m002;   //!
   TBranch        *b_bsm_sig_weight_atau_m003;   //!
   TBranch        *b_bsm_sig_weight_atau_m004;   //!
   TBranch        *b_bsm_sig_weight_atau_m005;   //!
   TBranch        *b_bsm_sig_weight_atau_m006;   //!
   TBranch        *b_electron_pt_syst_EG_RESOLUTION_ALL__1down;   //!
   TBranch        *b_photon_pt_syst_EG_RESOLUTION_ALL__1down;   //!
   TBranch        *b_electron_pt_syst_EG_RESOLUTION_ALL__1up;   //!
   TBranch        *b_photon_pt_syst_EG_RESOLUTION_ALL__1up;   //!
   TBranch        *b_electron_pt_syst_EG_SCALE_ALL__1down;   //!
   TBranch        *b_photon_pt_syst_EG_SCALE_ALL__1down;   //!
   TBranch        *b_electron_pt_syst_EG_SCALE_ALL__1up;   //!
   TBranch        *b_photon_pt_syst_EG_SCALE_ALL__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_ID__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_ID__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_MS__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_MS__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_DATASTAT__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_DATASTAT__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_GLOBAL__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_GLOBAL__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_RESBIAS__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_SAGITTA_RESBIAS__1up;   //!
   TBranch        *b_muon_pt_syst_MUON_SCALE__1down;   //!
   TBranch        *b_muon_pt_syst_MUON_SCALE__1up;   //!

   Analysis(TTree *tree=0);
   virtual ~Analysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual void     Topocluster(TLorentzVector particle, TH1D* Count, TH1D* Perp,TH1D* FVariable, TH1D* EMCal);
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Analysis_cxx
Analysis::Analysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data23_2trk_moreTCvars.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data23_2trk_moreTCvars.root");
      }
      f->GetObject("G2TauTree",tree);

   }
   Init(tree);
}

Analysis::~Analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Analysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   truth_electron_eta = 0;
   truth_electron_phi = 0;
   truth_electron_pt = 0;
   truth_electron_charge = 0;
   truth_muon_eta = 0;
   truth_muon_phi = 0;
   truth_muon_pt = 0;
   truth_muon_charge = 0;
   truth_photon_eta = 0;
   truth_photon_phi = 0;
   truth_photon_pt = 0;
   truth_hadron_eta = 0;
   truth_hadron_phi = 0;
   truth_hadron_pt = 0;
   truth_hadron_charge = 0;
   truth_monopole_eta = 0;
   truth_monopole_phi = 0;
   truth_monopole_pt = 0;
   truth_monopole_m = 0;
   truth_tau_eta = 0;
   truth_tau_phi = 0;
   truth_tau_pt = 0;
   truth_tau_charge = 0;
   truth_tau_vis_eta = 0;
   truth_tau_vis_phi = 0;
   truth_tau_vis_pt = 0;
   truth_tau_charged_eta = 0;
   truth_tau_charged_phi = 0;
   truth_tau_charged_pt = 0;
   truth_tau_neutralpions_eta = 0;
   truth_tau_neutralpions_phi = 0;
   truth_tau_neutralpions_pt = 0;
   truth_tau_decaymode = 0;
   truth_tau_hadronicdecaymode = 0;
   truth_tau_is_electronic = 0;
   truth_tau_is_muonic = 0;
   truth_tau_is_hadronic = 0;
   truth_ditau_y = 0;
   truth_ditau_eta = 0;
   truth_ditau_deta = 0;
   truth_ditau_phi = 0;
   truth_ditau_aco = 0;
   truth_ditau_m = 0;
   truth_ditau_pt = 0;
   truth_ditau_vis_y = 0;
   truth_ditau_vis_phi = 0;
   truth_ditau_vis_aco = 0;
   truth_ditau_vis_m = 0;
   truth_ditau_vis_pt = 0;
   truth_ditau_charged_y = 0;
   truth_ditau_charged_phi = 0;
   truth_ditau_charged_aco = 0;
   truth_ditau_charged_m = 0;
   truth_ditau_charged_pt = 0;
   track_eta = 0;
   track_phi = 0;
   track_pt = 0;
   track_theta = 0;
   track_charge = 0;
   track_is_LoosePrimary = 0;
   track_is_TightPrimary = 0;
   track_is_HITight = 0;
   track_is_HILoose = 0;
   track_is_matched_to_electron = 0;
   track_is_matched_to_muon = 0;
   track_d0 = 0;
   track_d0sig = 0;
   track_z0 = 0;
   track_z0Abs = 0;
   track_z0sinTheta = 0;
   track_PixeldEdX = 0;
   track_TRTHits = 0;
   track_TRTHighThresholdHits = 0;
   track_TRTHighThresholdHitsTotal = 0;
   track_truthMatchProbability = 0;
   track_fail_syst_OverallIDmat = 0;
   track_fail_syst_PP0mat = 0;
   track_fail_syst_IBLmat = 0;
   track_fail_syst_QGSP_BIC = 0;
   track_PixelHits = 0;
   track_SCTHits = 0;
   pix_track_eta = 0;
   pix_track_phi = 0;
   pix_track_pt = 0;
   pix_track_theta = 0;
   pix_track_charge = 0;
   pix_track_hits = 0;
   pix_track_d0 = 0;
   pix_track_d0sig = 0;
   pix_track_z0 = 0;
   pix_track_z0Abs = 0;
   pix_track_z0sinTheta = 0;
   pix_track_PixeldEdX = 0;
   perf_pix_track_dr_to_stable = 0;
   perf_pix_track_matched_pt = 0;
   perf_pix_track_matched_eta = 0;
   perf_pix_track_matched_phi = 0;
   perf_pix_track_matched_pdg = 0;
   siSpacePoint_x = 0;
   siSpacePoint_y = 0;
   siSpacePoint_z = 0;
   siSpacePoint_tot = 0;
   siSpacePoint_csize = 0;
   siSpacePoint_module1 = 0;
   siSpacePoint_module2 = 0;
   l1_emtau_eta = 0;
   l1_emtau_phi = 0;
   l1_emtau_et = 0;
   l1_emtau_threshold_pattern = 0;
   l1_muon_eta = 0;
   l1_muon_phi = 0;
   l1_muon_threshold = 0;
   HLT_muon_eta = 0;
   HLT_muon_phi = 0;
   HLT_muon_pt = 0;
   topo_cluster_eta = 0;
   topo_cluster_phi = 0;
   topo_cluster_pt = 0;
   topo_cluster_lambda = 0;
   topo_cluster_lambda2 = 0;
   topo_cluster_r2 = 0;
   topo_cluster_pass_sig_cut = 0;
   topo_cluster_pass_hotspot_cleaning = 0;
   topo_cluster_cell_significance = 0;
   topo_cluster_cell_sig_sampling = 0;
   topo_cluster_EM_prob = 0;
   topo_cluster_pt_syst_CALIB__1up = 0;
   topo_cluster_fail_syst_eff_sampling = 0;
   topo_cluster_time = 0;
   topo_cluster_ENG_FRAC_EM = 0;
   eg_cluster_eta = 0;
   eg_cluster_phi = 0;
   eg_cluster_et = 0;
   photon_eta = 0;
   photon_phi = 0;
   photon_pt = 0;
   photon_f1 = 0;
   photon_f3 = 0;
   photon_f1core = 0;
   photon_f3core = 0;
   photon_weta1 = 0;
   photon_weta2 = 0;
   photon_fracs1 = 0;
   photon_wtots1 = 0;
   photon_e277 = 0;
   photon_Reta = 0;
   photon_Rphi = 0;
   photon_Eratio = 0;
   photon_Rhad = 0;
   photon_Rhad1 = 0;
   photon_DeltaE = 0;
   photon_pass_NN_PID = 0;
   photon_fail_syst_eff_sampling = 0;
   electron_eta = 0;
   electron_phi = 0;
   electron_pt = 0;
   electron_theta = 0;
   electron_charge = 0;
   electron_is_LHVeryLoose = 0;
   electron_is_LHLoose = 0;
   electron_is_LHMedium = 0;
   electron_is_LHTight = 0;
   electron_trigger_thresholds_passed = 0;
   electron_d0 = 0;
   electron_d0sig = 0;
   electron_z0 = 0;
   electron_z0Abs = 0;
   electron_z0sinTheta = 0;
   electron_recoid_sf = 0;
   electron_recoid_sf_stat_up = 0;
   electron_recoid_sf_stat_down = 0;
   electron_recoid_sf_syst_up = 0;
   electron_recoid_sf_syst_down = 0;
   muon_eta = 0;
   muon_phi = 0;
   muon_pt = 0;
   muon_theta = 0;
   muon_charge = 0;
   muon_is_Loose = 0;
   muon_is_Medium = 0;
   muon_is_Tight = 0;
   muon_is_LowPt = 0;
   muon_is_MVALowPt = 0;
   muon_trigger_matched_L1 = 0;
   muon_trigger_matched_HLT = 0;
   muon_d0 = 0;
   muon_d0sig = 0;
   muon_z0 = 0;
   muon_z0Abs = 0;
   muon_z0sinTheta = 0;
   muon_p_ID = 0;
   muon_p_MS = 0;
   muon_energy_loss = 0;
   muon_recoid_sf = 0;
   muon_recoid_sf_stat_up = 0;
   muon_recoid_sf_stat_down = 0;
   muon_recoid_sf_syst_up = 0;
   muon_recoid_sf_syst_down = 0;
   MSmuon_eta = 0;
   MSmuon_phi = 0;
   MSmuon_pt = 0;
   MSmuon_d0 = 0;
   electron_pt_syst_EG_RESOLUTION_ALL__1down = 0;
   photon_pt_syst_EG_RESOLUTION_ALL__1down = 0;
   electron_pt_syst_EG_RESOLUTION_ALL__1up = 0;
   photon_pt_syst_EG_RESOLUTION_ALL__1up = 0;
   electron_pt_syst_EG_SCALE_ALL__1down = 0;
   photon_pt_syst_EG_SCALE_ALL__1down = 0;
   electron_pt_syst_EG_SCALE_ALL__1up = 0;
   photon_pt_syst_EG_SCALE_ALL__1up = 0;
   muon_pt_syst_MUON_ID__1down = 0;
   muon_pt_syst_MUON_ID__1up = 0;
   muon_pt_syst_MUON_MS__1down = 0;
   muon_pt_syst_MUON_MS__1up = 0;
   muon_pt_syst_MUON_SAGITTA_DATASTAT__1down = 0;
   muon_pt_syst_MUON_SAGITTA_DATASTAT__1up = 0;
   muon_pt_syst_MUON_SAGITTA_GLOBAL__1down = 0;
   muon_pt_syst_MUON_SAGITTA_GLOBAL__1up = 0;
   muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down = 0;
   muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up = 0;
   muon_pt_syst_MUON_SAGITTA_RESBIAS__1down = 0;
   muon_pt_syst_MUON_SAGITTA_RESBIAS__1up = 0;
   muon_pt_syst_MUON_SCALE__1down = 0;
   muon_pt_syst_MUON_SCALE__1up = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run_number", &run_number, &b_run_number);
   fChain->SetBranchAddress("lumi_block", &lumi_block, &b_lumi_block);
   fChain->SetBranchAddress("event_number", &event_number, &b_event_number);
   fChain->SetBranchAddress("mc_channel_number", &mc_channel_number, &b_mc_channel_number);
   fChain->SetBranchAddress("actIntPerXing", &actIntPerXing, &b_actIntPerXing);
   fChain->SetBranchAddress("avgIntPerXing", &avgIntPerXing, &b_avgIntPerXing);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("larTightSat", &larTightSat, &b_larTightSat);
   fChain->SetBranchAddress("larMiniNoiBurT", &larMiniNoiBurT, &b_larMiniNoiBurT);
   fChain->SetBranchAddress("isCleanEvent", &isCleanEvent, &b_isCleanEvent);
   fChain->SetBranchAddress("passed_Trig", &passed_Trig, &b_passed_Trig);
   fChain->SetBranchAddress("passed_HLT_noalg_L1ZDC_A_C_VTE50", &passed_HLT_noalg_L1ZDC_A_C_VTE50, &b_passed_HLT_noalg_L1ZDC_A_C_VTE50);
   fChain->SetBranchAddress("passed_HLT_noalg_L1ZDC_XOR_TE5_VTE200", &passed_HLT_noalg_L1ZDC_XOR_TE5_VTE200, &b_passed_HLT_noalg_L1ZDC_XOR_TE5_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1ZDC_XOR", &passed_HLT_noalg_L1ZDC_XOR, &b_passed_HLT_noalg_L1ZDC_XOR);
   fChain->SetBranchAddress("passed_L1_VZDC_A_C_TE5_VTE200", &passed_L1_VZDC_A_C_TE5_VTE200, &b_passed_L1_VZDC_A_C_TE5_VTE200);
   fChain->SetBranchAddress("passed_L1_VZDC_A_C_TE20_VTE200", &passed_L1_VZDC_A_C_TE20_VTE200, &b_passed_L1_VZDC_A_C_TE20_VTE200);
   fChain->SetBranchAddress("passed_L1_TAU1_TE4_VTE200", &passed_L1_TAU1_TE4_VTE200, &b_passed_L1_TAU1_TE4_VTE200);
   fChain->SetBranchAddress("passed_L1_2TAU1_VTE50", &passed_L1_2TAU1_VTE50, &b_passed_L1_2TAU1_VTE50);
   fChain->SetBranchAddress("passed_L1_2TAU2_VTE200", &passed_L1_2TAU2_VTE200, &b_passed_L1_2TAU2_VTE200);
   fChain->SetBranchAddress("passed_L1_MU4_VTE50", &passed_L1_MU4_VTE50, &b_passed_L1_MU4_VTE50);
   fChain->SetBranchAddress("passed_L1_MU0_VTE50", &passed_L1_MU0_VTE50, &b_passed_L1_MU0_VTE50);
   fChain->SetBranchAddress("passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU1_VTE50", &passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU1_VTE50, &b_passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU1_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_vetombts2in_exclusiveloose2_L12TAU1_VTE50", &passed_HLT_mb_sptrk_vetombts2in_exclusiveloose2_L12TAU1_VTE50, &b_passed_HLT_mb_sptrk_vetombts2in_exclusiveloose2_L12TAU1_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sp_L1VTE50", &passed_HLT_mb_sp_L1VTE50, &b_passed_HLT_mb_sp_L1VTE50);
   fChain->SetBranchAddress("passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU2_VTE200", &passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU2_VTE200, &b_passed_HLT_hi_upc_FgapAC3_mb_sptrk_exclusiveloose2_L12TAU2_VTE200);
   fChain->SetBranchAddress("passed_HLT_mu4_hi_upc_FgapAC3_L1MU4_VTE50", &passed_HLT_mu4_hi_upc_FgapAC3_L1MU4_VTE50, &b_passed_HLT_mu4_hi_upc_FgapAC3_L1MU4_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_vetombts2in_L1MU0_VTE50", &passed_HLT_mb_sptrk_vetombts2in_L1MU0_VTE50, &b_passed_HLT_mb_sptrk_vetombts2in_L1MU0_VTE50);
   fChain->SetBranchAddress("passed_HLT_hi_upc_FgapAC3_hi_gg_upc_noiseSup_L1TE4_VTE200_EMPTY", &passed_HLT_hi_upc_FgapAC3_hi_gg_upc_noiseSup_L1TE4_VTE200_EMPTY, &b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_noiseSup_L1TE4_VTE200_EMPTY);
   fChain->SetBranchAddress("passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L1TAU1_TE4_VTE200", &passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L1TAU1_TE4_VTE200, &b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L1TAU1_TE4_VTE200);
   fChain->SetBranchAddress("passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L12TAU1_VTE50", &passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L12TAU1_VTE50, &b_passed_HLT_hi_upc_FgapAC3_hi_gg_upc_L12TAU1_VTE50);
   fChain->SetBranchAddress("passed_HLT_noalg_L1VTE5", &passed_HLT_noalg_L1VTE5, &b_passed_HLT_noalg_L1VTE5);
   fChain->SetBranchAddress("passed_HLT_noalg_L1MBTS_1_VTE5", &passed_HLT_noalg_L1MBTS_1_VTE5, &b_passed_HLT_noalg_L1MBTS_1_VTE5);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TRT_VTE200", &passed_HLT_noalg_L1TRT_VTE200, &b_passed_HLT_noalg_L1TRT_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE20", &passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE20, &b_passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE20);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE50", &passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE50, &b_passed_HLT_mb_excl_1trk2_pt1_L1TRT_VTE50);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TAU1_TE3_VTE200", &passed_HLT_noalg_L1TAU1_TE3_VTE200, &b_passed_HLT_noalg_L1TAU1_TE3_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TAU1_TE5_VTE200", &passed_HLT_noalg_L1TAU1_TE5_VTE200, &b_passed_HLT_noalg_L1TAU1_TE5_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TAU1_VTE200", &passed_HLT_noalg_L1TAU1_VTE200, &b_passed_HLT_noalg_L1TAU1_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TAU1_VTE200_EMPTY", &passed_HLT_noalg_L1TAU1_VTE200_EMPTY, &b_passed_HLT_noalg_L1TAU1_VTE200_EMPTY);
   fChain->SetBranchAddress("passed_HLT_noalg_L1MU3V_EMPTY", &passed_HLT_noalg_L1MU3V_EMPTY, &b_passed_HLT_noalg_L1MU3V_EMPTY);
   fChain->SetBranchAddress("passed_HLT_noalg_L12TAU1_VTE50", &passed_HLT_noalg_L12TAU1_VTE50, &b_passed_HLT_noalg_L12TAU1_VTE50);
   fChain->SetBranchAddress("passed_HLT_noalg_L12TAU1_VTE200", &passed_HLT_noalg_L12TAU1_VTE200, &b_passed_HLT_noalg_L12TAU1_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE5_VTE20", &passed_HLT_noalg_L1TE5_VTE20, &b_passed_HLT_noalg_L1TE5_VTE20);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE20_VTE200", &passed_HLT_noalg_L1TE20_VTE200, &b_passed_HLT_noalg_L1TE20_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_bkg_L1MU3V_EMPTY", &passed_HLT_noalg_bkg_L1MU3V_EMPTY, &b_passed_HLT_noalg_bkg_L1MU3V_EMPTY);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE5", &passed_HLT_noalg_L1TE5, &b_passed_HLT_noalg_L1TE5);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE7p0ETA49", &passed_HLT_noalg_L1TE7p0ETA49, &b_passed_HLT_noalg_L1TE7p0ETA49);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE20", &passed_HLT_noalg_L1TE20, &b_passed_HLT_noalg_L1TE20);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1VTE5", &passed_HLT_mb_sptrk_L1VTE5, &b_passed_HLT_mb_sptrk_L1VTE5);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1MBTS_1_VTE5", &passed_HLT_mb_sptrk_L1MBTS_1_VTE5, &b_passed_HLT_mb_sptrk_L1MBTS_1_VTE5);
   fChain->SetBranchAddress("passed_HLT_mb_sp_pix100_L1RD0_FILLED", &passed_HLT_mb_sp_pix100_L1RD0_FILLED, &b_passed_HLT_mb_sp_pix100_L1RD0_FILLED);
   fChain->SetBranchAddress("passed_HLT_mb_sp_pix200_L1RD0_FILLED", &passed_HLT_mb_sp_pix200_L1RD0_FILLED, &b_passed_HLT_mb_sp_pix200_L1RD0_FILLED);
   fChain->SetBranchAddress("passed_HLT_mb_sp_pix500_L1RD0_FILLED", &passed_HLT_mb_sp_pix500_L1RD0_FILLED, &b_passed_HLT_mb_sp_pix500_L1RD0_FILLED);
   fChain->SetBranchAddress("passed_HLT_mb_sp_pix1000_L1RD0_FILLED", &passed_HLT_mb_sp_pix1000_L1RD0_FILLED, &b_passed_HLT_mb_sp_pix1000_L1RD0_FILLED);
   fChain->SetBranchAddress("passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10", &passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10, &b_passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10", &passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10, &b_passed_HLT_mb_nototpix100_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10", &passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10, &b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix100_L1ZDC_XOR_VTE10", &passed_HLT_mb_sp_nototpix100_L1ZDC_XOR_VTE10, &b_passed_HLT_mb_sp_nototpix100_L1ZDC_XOR_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10", &passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10, &b_passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_A_C_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10", &passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10, &b_passed_HLT_mb_nototpix70_excl_0trk2_pt0p2_L1ZDC_XOR_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix70_L1ZDC_A_C_VTE10", &passed_HLT_mb_sp_nototpix70_L1ZDC_A_C_VTE10, &b_passed_HLT_mb_sp_nototpix70_L1ZDC_A_C_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix70_L1ZDC_XOR_VTE10", &passed_HLT_mb_sp_nototpix70_L1ZDC_XOR_VTE10, &b_passed_HLT_mb_sp_nototpix70_L1ZDC_XOR_VTE10);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TRT_VTE20", &passed_HLT_noalg_L1TRT_VTE20, &b_passed_HLT_noalg_L1TRT_VTE20);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE50", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE50, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_hi_FgapAC5_L1VTE50", &passed_HLT_mb_sptrk_hi_FgapAC5_L1VTE50, &b_passed_HLT_mb_sptrk_hi_FgapAC5_L1VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1ZDC_XOR_VTE200", &passed_HLT_mb_sptrk_L1ZDC_XOR_VTE200, &b_passed_HLT_mb_sptrk_L1ZDC_XOR_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_VZDC_A_VZDC_C_VTE100", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_VZDC_A_VZDC_C_VTE100, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_VZDC_A_VZDC_C_VTE100);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_VZDC_A_VZDC_C_VTE100", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_VZDC_A_VZDC_C_VTE100, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_VZDC_A_VZDC_C_VTE100);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1eEM1_VZDC_A_VZDC_C_VTE100", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1eEM1_VZDC_A_VZDC_C_VTE100, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1eEM1_VZDC_A_VZDC_C_VTE100);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_hi_FgapA5_L11ZDC_A_1ZDC_C_VTE200", &passed_HLT_mb_sptrk_hi_FgapA5_L11ZDC_A_1ZDC_C_VTE200, &b_passed_HLT_mb_sptrk_hi_FgapA5_L11ZDC_A_1ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_L11ZDC_A_1ZDC_C_VTE200", &passed_HLT_mb_excl_1trk5_pt1_L11ZDC_A_1ZDC_C_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_L11ZDC_A_1ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_L1VZDC_A_VZDC_C_VTE200", &passed_HLT_mb_excl_1trk5_pt1_L1VZDC_A_VZDC_C_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_L1VZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1VZDC_A_VZDC_C_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1VZDC_A_VZDC_C_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1VZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mu3_hi_FgapAC5_L1MU3V_VTE50", &passed_HLT_mu3_hi_FgapAC5_L1MU3V_VTE50, &b_passed_HLT_mu3_hi_FgapAC5_L1MU3V_VTE50);
   fChain->SetBranchAddress("passed_HLT_mu3_hi_FgapAC5_L1MU3V_VjTE50", &passed_HLT_mu3_hi_FgapAC5_L1MU3V_VjTE50, &b_passed_HLT_mu3_hi_FgapAC5_L1MU3V_VjTE50);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TRT_VTE20_L1_isPassedBeforePrescale", &passed_HLT_noalg_L1TRT_VTE20_L1_isPassedBeforePrescale, &b_passed_HLT_noalg_L1TRT_VTE20_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20_L1_isPassedBeforePrescale", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20_L1_isPassedBeforePrescale, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE20_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_L11ZDC_A_1ZDC_C_VTE200_AV", &passed_L11ZDC_A_1ZDC_C_VTE200_AV, &b_passed_L11ZDC_A_1ZDC_C_VTE200_AV);
   fChain->SetBranchAddress("passed_L1VZDC_A_VZDC_C_VTE200_AV", &passed_L1VZDC_A_VZDC_C_VTE200_AV, &b_passed_L1VZDC_A_VZDC_C_VTE200_AV);
   fChain->SetBranchAddress("passed_L1VZDC_A_VZDC_C_VTE200", &passed_L1VZDC_A_VZDC_C_VTE200, &b_passed_L1VZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_L1ZDC_XOR_VTE200", &passed_L1ZDC_XOR_VTE200, &b_passed_L1ZDC_XOR_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TRT_FILLED", &passed_HLT_noalg_L1TRT_FILLED, &b_passed_HLT_noalg_L1TRT_FILLED);
   fChain->SetBranchAddress("passed_HLT_noalg_L1RD0_FILLED", &passed_HLT_noalg_L1RD0_FILLED, &b_passed_HLT_noalg_L1RD0_FILLED);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TE50", &passed_HLT_noalg_L1TE50, &b_passed_HLT_noalg_L1TE50);
   fChain->SetBranchAddress("passed_L1TRT_FILLED_L1_isPassedBeforePrescale", &passed_L1TRT_FILLED_L1_isPassedBeforePrescale, &b_passed_L1TRT_FILLED_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_L1TAU1_VZDC_A_VZDC_C_VTE100_tbp", &passed_L1TAU1_VZDC_A_VZDC_C_VTE100_tbp, &b_passed_L1TAU1_VZDC_A_VZDC_C_VTE100_tbp);
   fChain->SetBranchAddress("passed_L1TRT_VZDC_A_VZDC_C_VTE50_tbp", &passed_L1TRT_VZDC_A_VZDC_C_VTE50_tbp, &b_passed_L1TRT_VZDC_A_VZDC_C_VTE50_tbp);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L12eEM1_VTE200", &passed_HLT_mb_sptrk_L12eEM1_VTE200, &b_passed_HLT_mb_sptrk_L12eEM1_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1ZDC_1XOR5_VTE200", &passed_HLT_mb_sptrk_L1ZDC_1XOR5_VTE200, &b_passed_HLT_mb_sptrk_L1ZDC_1XOR5_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_L1eEM1_VTE200", &passed_HLT_mb_excl_1trk5_pt1_L1eEM1_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_L1eEM1_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VTE200", &passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VTE200, &b_passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1MBTS_1_VTE50", &passed_HLT_noalg_L1MBTS_1_VTE50, &b_passed_HLT_noalg_L1MBTS_1_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1VTE50", &passed_HLT_mb_sptrk_L1VTE50, &b_passed_HLT_mb_sptrk_L1VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1TRT_VTE20", &passed_HLT_mb_sptrk_L1TRT_VTE20, &b_passed_HLT_mb_sptrk_L1TRT_VTE20);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE20", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE20, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE20);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10_UNPAIRED_NONISO", &passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10_UNPAIRED_NONISO, &b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VTE10_UNPAIRED_NONISO);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L12TAU1_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L12TAU1_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L12TAU1_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sp_vpix30_hi_FgapAC5_L1TAU1_TE4_VTE200", &passed_HLT_mb_sp_vpix30_hi_FgapAC5_L1TAU1_TE4_VTE200, &b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_L1TAU1_TE4_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sp_vpix30_hi_FgapAC5_L12TAU1_VTE200", &passed_HLT_mb_sp_vpix30_hi_FgapAC5_L12TAU1_VTE200, &b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_L12TAU1_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sp_vpix30_hi_FgapAC5_2g0_etcut_L12TAU1_VTE200", &passed_HLT_mb_sp_vpix30_hi_FgapAC5_2g0_etcut_L12TAU1_VTE200, &b_passed_HLT_mb_sp_vpix30_hi_FgapAC5_2g0_etcut_L12TAU1_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_TE4_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_TE4_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU2_TE4_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_L1TAU2_TE4_VTE200", &passed_HLT_mb_excl_1trk5_pt1_L1TAU2_TE4_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_L1TAU2_TE4_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE50", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE50, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VZDC_A_VZDC_C_VTE50);
   fChain->SetBranchAddress("passed_L1TAU1_TE4_VTE200_tbp", &passed_L1TAU1_TE4_VTE200_tbp, &b_passed_L1TAU1_TE4_VTE200_tbp);
   fChain->SetBranchAddress("passed_L12TAU1_VTE200_tbp ", &passed_L12TAU1_VTE200_tbpsP, &b_passed_L12TAU1_VTE200_tbp );
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L11ZDC_A_1ZDC_C_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L11ZDC_A_1ZDC_C_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L11ZDC_A_1ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1ZDC_XOR4_VTE200", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1ZDC_XOR4_VTE200, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1ZDC_XOR4_VTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1ZDC_XOR_VTE200", &passed_HLT_noalg_L1ZDC_XOR_VTE200, &b_passed_HLT_noalg_L1ZDC_XOR_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sp50_trk15_hmt_hi_FgapA5_L1MBTS_1_VZDC_A_ZDC_C_VTE200", &passed_HLT_mb_sp50_trk15_hmt_hi_FgapA5_L1MBTS_1_VZDC_A_ZDC_C_VTE200, &b_passed_HLT_mb_sp50_trk15_hmt_hi_FgapA5_L1MBTS_1_VZDC_A_ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sp50_trk15_hmt_hi_FgapC5_L1MBTS_1_ZDC_A_VZDC_C_VTE200", &passed_HLT_mb_sp50_trk15_hmt_hi_FgapC5_L1MBTS_1_ZDC_A_VZDC_C_VTE200, &b_passed_HLT_mb_sp50_trk15_hmt_hi_FgapC5_L1MBTS_1_ZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VTE50", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VTE50, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VTE50);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VZDC_A_VZDC_C_VTE100", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VZDC_A_VZDC_C_VTE100, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_VZDC_A_VZDC_C_VTE100);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_ZDC_XOR4_VTE100", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_ZDC_XOR4_VTE100, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TAU1_TRT_ZDC_XOR4_VTE100);
   fChain->SetBranchAddress("passed_L1TRT_FILLED_L1_isPassedAterVeto", &passed_L1TRT_FILLED_L1_isPassedAterVeto, &b_passed_L1TRT_FILLED_L1_isPassedAterVeto);
   fChain->SetBranchAddress("passed_L1TRT_VTE20_L1_isPassedAterVeto", &passed_L1TRT_VTE20_L1_isPassedAterVeto, &b_passed_L1TRT_VTE20_L1_isPassedAterVeto);
   fChain->SetBranchAddress("prescale_L1TRT_VTE20", &prescale_L1TRT_VTE20, &b_prescale_L1TRT_VTE20);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_hi_FgapA5_L1VZDC_A_ZDC_C_VTE200", &passed_HLT_mb_sptrk_hi_FgapA5_L1VZDC_A_ZDC_C_VTE200, &b_passed_HLT_mb_sptrk_hi_FgapA5_L1VZDC_A_ZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_hi_FgapC5_L1ZDC_A_VZDC_C_VTE200", &passed_HLT_mb_sptrk_hi_FgapC5_L1ZDC_A_VZDC_C_VTE200, &b_passed_HLT_mb_sptrk_hi_FgapC5_L1ZDC_A_VZDC_C_VTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1ZDC_1XOR5_VjTE200", &passed_HLT_mb_sptrk_L1ZDC_1XOR5_VjTE200, &b_passed_HLT_mb_sptrk_L1ZDC_1XOR5_VjTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VjTE200", &passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VjTE200, &b_passed_HLT_mb_sptrk_L11ZDC_A_1ZDC_C_VjTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1ZDC_XOR_VjTE200", &passed_HLT_mb_sptrk_L1ZDC_XOR_VjTE200, &b_passed_HLT_mb_sptrk_L1ZDC_XOR_VjTE200);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VjTE20", &passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VjTE20, &b_passed_HLT_mb_excl_1trk5_pt1_hi_FgapAC5_L1TRT_VjTE20);
   fChain->SetBranchAddress("passed_HLT_mb_excl_1trk5_pt0p5_hi_FgapAC5_L1TRT_VjTE20", &passed_HLT_mb_excl_1trk5_pt0p5_hi_FgapAC5_L1TRT_VjTE20, &b_passed_HLT_mb_excl_1trk5_pt0p5_hi_FgapAC5_L1TRT_VjTE20);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix50_q2_L1ZDC_A_C_VjTE10", &passed_HLT_mb_sp_nototpix50_q2_L1ZDC_A_C_VjTE10, &b_passed_HLT_mb_sp_nototpix50_q2_L1ZDC_A_C_VjTE10);
   fChain->SetBranchAddress("passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VjTE10", &passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VjTE10, &b_passed_HLT_mb_sp_nototpix100_L1ZDC_A_C_VjTE10);
   fChain->SetBranchAddress("passed_L1TRT_VjTE20_L1_isPassedBeforePrescale", &passed_L1TRT_VjTE20_L1_isPassedBeforePrescale, &b_passed_L1TRT_VjTE20_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_L1TRT_VjTE200_L1_isPassedBeforePrescale", &passed_L1TRT_VjTE200_L1_isPassedBeforePrescale, &b_passed_L1TRT_VjTE200_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_L1TRT_F_L1_isPassedBeforePrescale", &passed_L1TRT_F_L1_isPassedBeforePrescale, &b_passed_L1TRT_F_L1_isPassedBeforePrescale);
   fChain->SetBranchAddress("passed_L1eTAU1_jTE4_VjTE200", &passed_L1eTAU1_jTE4_VjTE200, &b_passed_L1eTAU1_jTE4_VjTE200);
   fChain->SetBranchAddress("passed_L1eEM1_jTE4_VjTE200", &passed_L1eEM1_jTE4_VjTE200, &b_passed_L1eEM1_jTE4_VjTE200);
   fChain->SetBranchAddress("passed_L12eTAU1_VjTE200", &passed_L12eTAU1_VjTE200, &b_passed_L12eTAU1_VjTE200);
   fChain->SetBranchAddress("passed_L1DPHI_2eEM1_VjTE200", &passed_L1DPHI_2eEM1_VjTE200, &b_passed_L1DPHI_2eEM1_VjTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1jTAU1_TRT_ZDC_XOR_VjTE200", &passed_HLT_mb_sptrk_L1jTAU1_TRT_ZDC_XOR_VjTE200, &b_passed_HLT_mb_sptrk_L1jTAU1_TRT_ZDC_XOR_VjTE200);
   fChain->SetBranchAddress("passed_HLT_mb_sptrk_L1TRT_ZDC_XOR_VjTE200", &passed_HLT_mb_sptrk_L1TRT_ZDC_XOR_VjTE200, &b_passed_HLT_mb_sptrk_L1TRT_ZDC_XOR_VjTE200);
   fChain->SetBranchAddress("passed_L1jTAU1_TRT_ZDC_XOR_VjTE200", &passed_L1jTAU1_TRT_ZDC_XOR_VjTE200, &b_passed_L1jTAU1_TRT_ZDC_XOR_VjTE200);
   fChain->SetBranchAddress("passed_HLT_noalg_L1TRT_VjTE50", &passed_HLT_noalg_L1TRT_VjTE50, &b_passed_HLT_noalg_L1TRT_VjTE50);
   fChain->SetBranchAddress("L1MU4_sf", &L1MU4_sf, &b_L1MU4_sf);
   fChain->SetBranchAddress("L1MU4_sf_stat_up", &L1MU4_sf_stat_up, &b_L1MU4_sf_stat_up);
   fChain->SetBranchAddress("L1MU4_sf_stat_down", &L1MU4_sf_stat_down, &b_L1MU4_sf_stat_down);
   fChain->SetBranchAddress("L1MU4_sf_syst_up", &L1MU4_sf_syst_up, &b_L1MU4_sf_syst_up);
   fChain->SetBranchAddress("L1MU4_sf_syst_down", &L1MU4_sf_syst_down, &b_L1MU4_sf_syst_down);
   fChain->SetBranchAddress("HLT_mu4_sf", &HLT_mu4_sf, &b_HLT_mu4_sf);
   fChain->SetBranchAddress("HLT_mu4_sf_stat_up", &HLT_mu4_sf_stat_up, &b_HLT_mu4_sf_stat_up);
   fChain->SetBranchAddress("HLT_mu4_sf_stat_down", &HLT_mu4_sf_stat_down, &b_HLT_mu4_sf_stat_down);
   fChain->SetBranchAddress("HLT_mu4_sf_syst_up", &HLT_mu4_sf_syst_up, &b_HLT_mu4_sf_syst_up);
   fChain->SetBranchAddress("HLT_mu4_sf_syst_down", &HLT_mu4_sf_syst_down, &b_HLT_mu4_sf_syst_down);
   fChain->SetBranchAddress("passed_GRL", &passed_GRL, &b_passed_GRL);
   fChain->SetBranchAddress("zdc_ene_a", &zdc_ene_a, &b_zdc_ene_a);
   fChain->SetBranchAddress("zdc_ene_c", &zdc_ene_c, &b_zdc_ene_c);
   fChain->SetBranchAddress("zdc_status_a", &zdc_status_a, &b_zdc_status_a);
   fChain->SetBranchAddress("zdc_status_c", &zdc_status_c, &b_zdc_status_c);
   fChain->SetBranchAddress("mbts_time_a", &mbts_time_a, &b_mbts_time_a);
   fChain->SetBranchAddress("mbts_time_c", &mbts_time_c, &b_mbts_time_c);
   fChain->SetBranchAddress("mbts_time_diff", &mbts_time_diff, &b_mbts_time_diff);
   fChain->SetBranchAddress("mbts_count_a", &mbts_count_a, &b_mbts_count_a);
   fChain->SetBranchAddress("mbts_count_c", &mbts_count_c, &b_mbts_count_c);
   fChain->SetBranchAddress("truth_electron_eta", &truth_electron_eta, &b_truth_electron_eta);
   fChain->SetBranchAddress("truth_electron_phi", &truth_electron_phi, &b_truth_electron_phi);
   fChain->SetBranchAddress("truth_electron_pt", &truth_electron_pt, &b_truth_electron_pt);
   fChain->SetBranchAddress("truth_electron_charge", &truth_electron_charge, &b_truth_electron_charge);
   fChain->SetBranchAddress("truth_muon_eta", &truth_muon_eta, &b_truth_muon_eta);
   fChain->SetBranchAddress("truth_muon_phi", &truth_muon_phi, &b_truth_muon_phi);
   fChain->SetBranchAddress("truth_muon_pt", &truth_muon_pt, &b_truth_muon_pt);
   fChain->SetBranchAddress("truth_muon_charge", &truth_muon_charge, &b_truth_muon_charge);
   fChain->SetBranchAddress("truth_photon_eta", &truth_photon_eta, &b_truth_photon_eta);
   fChain->SetBranchAddress("truth_photon_phi", &truth_photon_phi, &b_truth_photon_phi);
   fChain->SetBranchAddress("truth_photon_pt", &truth_photon_pt, &b_truth_photon_pt);
   fChain->SetBranchAddress("truth_hadron_eta", &truth_hadron_eta, &b_truth_hadron_eta);
   fChain->SetBranchAddress("truth_hadron_phi", &truth_hadron_phi, &b_truth_hadron_phi);
   fChain->SetBranchAddress("truth_hadron_pt", &truth_hadron_pt, &b_truth_hadron_pt);
   fChain->SetBranchAddress("truth_hadron_charge", &truth_hadron_charge, &b_truth_hadron_charge);
   fChain->SetBranchAddress("truth_monopole_eta", &truth_monopole_eta, &b_truth_monopole_eta);
   fChain->SetBranchAddress("truth_monopole_phi", &truth_monopole_phi, &b_truth_monopole_phi);
   fChain->SetBranchAddress("truth_monopole_pt", &truth_monopole_pt, &b_truth_monopole_pt);
   fChain->SetBranchAddress("truth_monopole_m", &truth_monopole_m, &b_truth_monopole_m);
   fChain->SetBranchAddress("truth_tau_eta", &truth_tau_eta, &b_truth_tau_eta);
   fChain->SetBranchAddress("truth_tau_phi", &truth_tau_phi, &b_truth_tau_phi);
   fChain->SetBranchAddress("truth_tau_pt", &truth_tau_pt, &b_truth_tau_pt);
   fChain->SetBranchAddress("truth_tau_charge", &truth_tau_charge, &b_truth_tau_charge);
   fChain->SetBranchAddress("truth_tau_vis_eta", &truth_tau_vis_eta, &b_truth_tau_vis_eta);
   fChain->SetBranchAddress("truth_tau_vis_phi", &truth_tau_vis_phi, &b_truth_tau_vis_phi);
   fChain->SetBranchAddress("truth_tau_vis_pt", &truth_tau_vis_pt, &b_truth_tau_vis_pt);
   fChain->SetBranchAddress("truth_tau_charged_eta", &truth_tau_charged_eta, &b_truth_tau_charged_eta);
   fChain->SetBranchAddress("truth_tau_charged_phi", &truth_tau_charged_phi, &b_truth_tau_charged_phi);
   fChain->SetBranchAddress("truth_tau_charged_pt", &truth_tau_charged_pt, &b_truth_tau_charged_pt);
   fChain->SetBranchAddress("truth_tau_neutralpions_eta", &truth_tau_neutralpions_eta, &b_truth_tau_neutralpions_eta);
   fChain->SetBranchAddress("truth_tau_neutralpions_phi", &truth_tau_neutralpions_phi, &b_truth_tau_neutralpions_phi);
   fChain->SetBranchAddress("truth_tau_neutralpions_pt", &truth_tau_neutralpions_pt, &b_truth_tau_neutralpions_pt);
   fChain->SetBranchAddress("truth_tau_decaymode", &truth_tau_decaymode, &b_truth_tau_decaymode);
   fChain->SetBranchAddress("truth_tau_hadronicdecaymode", &truth_tau_hadronicdecaymode, &b_truth_tau_hadronicdecaymode);
   fChain->SetBranchAddress("truth_tau_is_electronic", &truth_tau_is_electronic, &b_truth_tau_is_electronic);
   fChain->SetBranchAddress("truth_tau_is_muonic", &truth_tau_is_muonic, &b_truth_tau_is_muonic);
   fChain->SetBranchAddress("truth_tau_is_hadronic", &truth_tau_is_hadronic, &b_truth_tau_is_hadronic);
   fChain->SetBranchAddress("truth_ditau_y", &truth_ditau_y, &b_truth_ditau_y);
   fChain->SetBranchAddress("truth_ditau_eta", &truth_ditau_eta, &b_truth_ditau_eta);
   fChain->SetBranchAddress("truth_ditau_deta", &truth_ditau_deta, &b_truth_ditau_deta);
   fChain->SetBranchAddress("truth_ditau_phi", &truth_ditau_phi, &b_truth_ditau_phi);
   fChain->SetBranchAddress("truth_ditau_aco", &truth_ditau_aco, &b_truth_ditau_aco);
   fChain->SetBranchAddress("truth_ditau_m", &truth_ditau_m, &b_truth_ditau_m);
   fChain->SetBranchAddress("truth_ditau_pt", &truth_ditau_pt, &b_truth_ditau_pt);
   fChain->SetBranchAddress("truth_ditau_vis_y", &truth_ditau_vis_y, &b_truth_ditau_vis_y);
   fChain->SetBranchAddress("truth_ditau_vis_phi", &truth_ditau_vis_phi, &b_truth_ditau_vis_phi);
   fChain->SetBranchAddress("truth_ditau_vis_aco", &truth_ditau_vis_aco, &b_truth_ditau_vis_aco);
   fChain->SetBranchAddress("truth_ditau_vis_m", &truth_ditau_vis_m, &b_truth_ditau_vis_m);
   fChain->SetBranchAddress("truth_ditau_vis_pt", &truth_ditau_vis_pt, &b_truth_ditau_vis_pt);
   fChain->SetBranchAddress("truth_ditau_charged_y", &truth_ditau_charged_y, &b_truth_ditau_charged_y);
   fChain->SetBranchAddress("truth_ditau_charged_phi", &truth_ditau_charged_phi, &b_truth_ditau_charged_phi);
   fChain->SetBranchAddress("truth_ditau_charged_aco", &truth_ditau_charged_aco, &b_truth_ditau_charged_aco);
   fChain->SetBranchAddress("truth_ditau_charged_m", &truth_ditau_charged_m, &b_truth_ditau_charged_m);
   fChain->SetBranchAddress("truth_ditau_charged_pt", &truth_ditau_charged_pt, &b_truth_ditau_charged_pt);
   fChain->SetBranchAddress("mg5_to_sl_flux_weight", &mg5_to_sl_flux_weight, &b_mg5_to_sl_flux_weight);
   fChain->SetBranchAddress("pass_mg5_sl_overlapremoval", &pass_mg5_sl_overlapremoval, &b_pass_mg5_sl_overlapremoval);
   fChain->SetBranchAddress("sl_to_sc3_flux_weight", &sl_to_sc3_flux_weight, &b_sl_to_sc3_flux_weight);
   fChain->SetBranchAddress("data_0n0n_flux_weight", &data_0n0n_flux_weight, &b_data_0n0n_flux_weight);
   fChain->SetBranchAddress("data_0n0n_flux_weight_syst", &data_0n0n_flux_weight_syst, &b_data_0n0n_flux_weight_syst);
   fChain->SetBranchAddress("PV_n", &PV_n, &b_PV_n);
   fChain->SetBranchAddress("perf_PV_x", &perf_PV_x, &b_perf_PV_x);
   fChain->SetBranchAddress("perf_PV_y", &perf_PV_y, &b_perf_PV_y);
   fChain->SetBranchAddress("perf_PV_z", &perf_PV_z, &b_perf_PV_z);
   fChain->SetBranchAddress("track_n", &track_n, &b_track_n);
   fChain->SetBranchAddress("track_n_LoosePrimary", &track_n_LoosePrimary, &b_track_n_LoosePrimary);
   fChain->SetBranchAddress("track_n_TightPrimary", &track_n_TightPrimary, &b_track_n_TightPrimary);
   fChain->SetBranchAddress("track_n_HITight", &track_n_HITight, &b_track_n_HITight);
   fChain->SetBranchAddress("track_n_HILoose", &track_n_HILoose, &b_track_n_HILoose);
   fChain->SetBranchAddress("track_n_200mev", &track_n_200mev, &b_track_n_200mev);
   fChain->SetBranchAddress("track_n_1gev", &track_n_1gev, &b_track_n_1gev);
   fChain->SetBranchAddress("track_n_2gev", &track_n_2gev, &b_track_n_2gev);
   fChain->SetBranchAddress("track_n_eta2", &track_n_eta2, &b_track_n_eta2);
   fChain->SetBranchAddress("track_eta", &track_eta, &b_track_eta);
   fChain->SetBranchAddress("track_phi", &track_phi, &b_track_phi);
   fChain->SetBranchAddress("track_pt", &track_pt, &b_track_pt);
   fChain->SetBranchAddress("track_theta", &track_theta, &b_track_theta);
   fChain->SetBranchAddress("track_charge", &track_charge, &b_track_charge);
   fChain->SetBranchAddress("track_is_LoosePrimary", &track_is_LoosePrimary, &b_track_is_LoosePrimary);
   fChain->SetBranchAddress("track_is_TightPrimary", &track_is_TightPrimary, &b_track_is_TightPrimary);
   fChain->SetBranchAddress("track_is_HITight", &track_is_HITight, &b_track_is_HITight);
   fChain->SetBranchAddress("track_is_HILoose", &track_is_HILoose, &b_track_is_HILoose);
   fChain->SetBranchAddress("track_is_matched_to_electron", &track_is_matched_to_electron, &b_track_is_matched_to_electron);
   fChain->SetBranchAddress("track_is_matched_to_muon", &track_is_matched_to_muon, &b_track_is_matched_to_muon);
   fChain->SetBranchAddress("track_d0", &track_d0, &b_track_d0);
   fChain->SetBranchAddress("track_d0sig", &track_d0sig, &b_track_d0sig);
   fChain->SetBranchAddress("track_z0", &track_z0, &b_track_z0);
   fChain->SetBranchAddress("track_z0Abs", &track_z0Abs, &b_track_z0Abs);
   fChain->SetBranchAddress("track_z0sinTheta", &track_z0sinTheta, &b_track_z0sinTheta);
   fChain->SetBranchAddress("track_PixeldEdX", &track_PixeldEdX, &b_track_PixeldEdX);
   fChain->SetBranchAddress("track_TRTHits", &track_TRTHits, &b_track_TRTHits);
   fChain->SetBranchAddress("track_TRTHighThresholdHits", &track_TRTHighThresholdHits, &b_track_TRTHighThresholdHits);
   fChain->SetBranchAddress("track_TRTHighThresholdHitsTotal", &track_TRTHighThresholdHitsTotal, &b_track_TRTHighThresholdHitsTotal);
   fChain->SetBranchAddress("track_truthMatchProbability", &track_truthMatchProbability, &b_track_truthMatchProbability);
   fChain->SetBranchAddress("track_fail_syst_OverallIDmat", &track_fail_syst_OverallIDmat, &b_track_fail_syst_OverallIDmat);
   fChain->SetBranchAddress("track_fail_syst_PP0mat", &track_fail_syst_PP0mat, &b_track_fail_syst_PP0mat);
   fChain->SetBranchAddress("track_fail_syst_IBLmat", &track_fail_syst_IBLmat, &b_track_fail_syst_IBLmat);
   fChain->SetBranchAddress("track_fail_syst_QGSP_BIC", &track_fail_syst_QGSP_BIC, &b_track_fail_syst_QGSP_BIC);
   fChain->SetBranchAddress("track_PixelHits", &track_PixelHits, &b_track_PixelHits);
   fChain->SetBranchAddress("track_SCTHits", &track_SCTHits, &b_track_SCTHits);
   fChain->SetBranchAddress("pix_track_n", &pix_track_n, &b_pix_track_n);
   fChain->SetBranchAddress("pix_track_eta", &pix_track_eta, &b_pix_track_eta);
   fChain->SetBranchAddress("pix_track_phi", &pix_track_phi, &b_pix_track_phi);
   fChain->SetBranchAddress("pix_track_pt", &pix_track_pt, &b_pix_track_pt);
   fChain->SetBranchAddress("pix_track_theta", &pix_track_theta, &b_pix_track_theta);
   fChain->SetBranchAddress("pix_track_charge", &pix_track_charge, &b_pix_track_charge);
   fChain->SetBranchAddress("pix_track_hits", &pix_track_hits, &b_pix_track_hits);
   fChain->SetBranchAddress("pix_track_d0", &pix_track_d0, &b_pix_track_d0);
   fChain->SetBranchAddress("pix_track_d0sig", &pix_track_d0sig, &b_pix_track_d0sig);
   fChain->SetBranchAddress("pix_track_z0", &pix_track_z0, &b_pix_track_z0);
   fChain->SetBranchAddress("pix_track_z0Abs", &pix_track_z0Abs, &b_pix_track_z0Abs);
   fChain->SetBranchAddress("pix_track_z0sinTheta", &pix_track_z0sinTheta, &b_pix_track_z0sinTheta);
   fChain->SetBranchAddress("pix_track_PixeldEdX", &pix_track_PixeldEdX, &b_pix_track_PixeldEdX);
   fChain->SetBranchAddress("perf_pix_track_dr_to_stable", &perf_pix_track_dr_to_stable, &b_perf_pix_track_dr_to_stable);
   fChain->SetBranchAddress("perf_pix_track_matched_pt", &perf_pix_track_matched_pt, &b_perf_pix_track_matched_pt);
   fChain->SetBranchAddress("perf_pix_track_matched_eta", &perf_pix_track_matched_eta, &b_perf_pix_track_matched_eta);
   fChain->SetBranchAddress("perf_pix_track_matched_phi", &perf_pix_track_matched_phi, &b_perf_pix_track_matched_phi);
   fChain->SetBranchAddress("perf_pix_track_matched_pdg", &perf_pix_track_matched_pdg, &b_perf_pix_track_matched_pdg);
   fChain->SetBranchAddress("pix_sp_count_endcap_A", &pix_sp_count_endcap_A, &b_pix_sp_count_endcap_A);
   fChain->SetBranchAddress("pix_sp_count_Barrel", &pix_sp_count_Barrel, &b_pix_sp_count_Barrel);
   fChain->SetBranchAddress("pix_sp_count_endcap_C", &pix_sp_count_endcap_C, &b_pix_sp_count_endcap_C);
   fChain->SetBranchAddress("pix_sp_count_noToT", &pix_sp_count_noToT, &b_pix_sp_count_noToT);
   fChain->SetBranchAddress("siSpacePoint_x", &siSpacePoint_x, &b_siSpacePoint_x);
   fChain->SetBranchAddress("siSpacePoint_y", &siSpacePoint_y, &b_siSpacePoint_y);
   fChain->SetBranchAddress("siSpacePoint_z", &siSpacePoint_z, &b_siSpacePoint_z);
   fChain->SetBranchAddress("siSpacePoint_tot", &siSpacePoint_tot, &b_siSpacePoint_tot);
   fChain->SetBranchAddress("siSpacePoint_csize", &siSpacePoint_csize, &b_siSpacePoint_csize);
   fChain->SetBranchAddress("siSpacePoint_module1", &siSpacePoint_module1, &b_siSpacePoint_module1);
   fChain->SetBranchAddress("siSpacePoint_module2", &siSpacePoint_module2, &b_siSpacePoint_module2);
   fChain->SetBranchAddress("siSpacePoint_n", &siSpacePoint_n, &b_siSpacePoint_n);
   fChain->SetBranchAddress("pixSpacePoint_n", &pixSpacePoint_n, &b_pixSpacePoint_n);
   fChain->SetBranchAddress("l1_emtau_eta", &l1_emtau_eta, &b_l1_emtau_eta);
   fChain->SetBranchAddress("l1_emtau_phi", &l1_emtau_phi, &b_l1_emtau_phi);
   fChain->SetBranchAddress("l1_emtau_et", &l1_emtau_et, &b_l1_emtau_et);
   fChain->SetBranchAddress("l1_emtau_threshold_pattern", &l1_emtau_threshold_pattern, &b_l1_emtau_threshold_pattern);
   fChain->SetBranchAddress("l1_muon_eta", &l1_muon_eta, &b_l1_muon_eta);
   fChain->SetBranchAddress("l1_muon_phi", &l1_muon_phi, &b_l1_muon_phi);
   fChain->SetBranchAddress("l1_muon_threshold", &l1_muon_threshold, &b_l1_muon_threshold);
   fChain->SetBranchAddress("l1_te", &l1_te, &b_l1_te);
   fChain->SetBranchAddress("HLT_muon_eta", &HLT_muon_eta, &b_HLT_muon_eta);
   fChain->SetBranchAddress("HLT_muon_phi", &HLT_muon_phi, &b_HLT_muon_phi);
   fChain->SetBranchAddress("HLT_muon_pt", &HLT_muon_pt, &b_HLT_muon_pt);
   fChain->SetBranchAddress("topoclus_n", &topoclus_n, &b_topoclus_n);
   fChain->SetBranchAddress("topoclus_ht", &topoclus_ht, &b_topoclus_ht);
   fChain->SetBranchAddress("topo_cluster_eta", &topo_cluster_eta, &b_topo_cluster_eta);
   fChain->SetBranchAddress("topo_cluster_phi", &topo_cluster_phi, &b_topo_cluster_phi);
   fChain->SetBranchAddress("topo_cluster_pt", &topo_cluster_pt, &b_topo_cluster_pt);
   fChain->SetBranchAddress("topo_cluster_lambda", &topo_cluster_lambda, &b_topo_cluster_lambda);
   fChain->SetBranchAddress("topo_cluster_lambda2", &topo_cluster_lambda2, &b_topo_cluster_lambda2);
   fChain->SetBranchAddress("topo_cluster_r2", &topo_cluster_r2, &b_topo_cluster_r2);
   fChain->SetBranchAddress("topo_cluster_pass_sig_cut", &topo_cluster_pass_sig_cut, &b_topo_cluster_pass_sig_cut);
   fChain->SetBranchAddress("topo_cluster_pass_hotspot_cleaning", &topo_cluster_pass_hotspot_cleaning, &b_topo_cluster_pass_hotspot_cleaning);
   fChain->SetBranchAddress("topo_cluster_cell_significance", &topo_cluster_cell_significance, &b_topo_cluster_cell_significance);
   fChain->SetBranchAddress("topo_cluster_cell_sig_sampling", &topo_cluster_cell_sig_sampling, &b_topo_cluster_cell_sig_sampling);
   fChain->SetBranchAddress("topo_cluster_EM_prob", &topo_cluster_EM_prob, &b_topo_cluster_EM_prob);
   fChain->SetBranchAddress("topo_cluster_pt_syst_CALIB__1up", &topo_cluster_pt_syst_CALIB__1up, &b_topo_cluster_pt_syst_CALIB__1up);
   fChain->SetBranchAddress("topo_cluster_fail_syst_eff_sampling", &topo_cluster_fail_syst_eff_sampling, &b_topo_cluster_fail_syst_eff_sampling);
   fChain->SetBranchAddress("topo_cluster_time", &topo_cluster_time, &b_topo_cluster_time);
   fChain->SetBranchAddress("topo_cluster_ENG_FRAC_EM", &topo_cluster_ENG_FRAC_EM, &b_topo_cluster_ENG_FRAC_EM);
   fChain->SetBranchAddress("eg_cluster_eta", &eg_cluster_eta, &b_eg_cluster_eta);
   fChain->SetBranchAddress("eg_cluster_phi", &eg_cluster_phi, &b_eg_cluster_phi);
   fChain->SetBranchAddress("eg_cluster_et", &eg_cluster_et, &b_eg_cluster_et);
   fChain->SetBranchAddress("eg_cluster_n", &eg_cluster_n, &b_eg_cluster_n);
   fChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
   fChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
   fChain->SetBranchAddress("photon_pt", &photon_pt, &b_photon_pt);
   fChain->SetBranchAddress("photon_f1", &photon_f1, &b_photon_f1);
   fChain->SetBranchAddress("photon_f3", &photon_f3, &b_photon_f3);
   fChain->SetBranchAddress("photon_f1core", &photon_f1core, &b_photon_f1core);
   fChain->SetBranchAddress("photon_f3core", &photon_f3core, &b_photon_f3core);
   fChain->SetBranchAddress("photon_weta1", &photon_weta1, &b_photon_weta1);
   fChain->SetBranchAddress("photon_weta2", &photon_weta2, &b_photon_weta2);
   fChain->SetBranchAddress("photon_fracs1", &photon_fracs1, &b_photon_fracs1);
   fChain->SetBranchAddress("photon_wtots1", &photon_wtots1, &b_photon_wtots1);
   fChain->SetBranchAddress("photon_e277", &photon_e277, &b_photon_e277);
   fChain->SetBranchAddress("photon_Reta", &photon_Reta, &b_photon_Reta);
   fChain->SetBranchAddress("photon_Rphi", &photon_Rphi, &b_photon_Rphi);
   fChain->SetBranchAddress("photon_Eratio", &photon_Eratio, &b_photon_Eratio);
   fChain->SetBranchAddress("photon_Rhad", &photon_Rhad, &b_photon_Rhad);
   fChain->SetBranchAddress("photon_Rhad1", &photon_Rhad1, &b_photon_Rhad1);
   fChain->SetBranchAddress("photon_DeltaE", &photon_DeltaE, &b_photon_DeltaE);
   fChain->SetBranchAddress("photon_pass_NN_PID", &photon_pass_NN_PID, &b_photon_pass_NN_PID);
   fChain->SetBranchAddress("photon_fail_syst_eff_sampling", &photon_fail_syst_eff_sampling, &b_photon_fail_syst_eff_sampling);
   fChain->SetBranchAddress("nPhot", &nPhot, &b_nPhot);
   fChain->SetBranchAddress("electron_eta", &electron_eta, &b_electron_eta);
   fChain->SetBranchAddress("electron_phi", &electron_phi, &b_electron_phi);
   fChain->SetBranchAddress("electron_pt", &electron_pt, &b_electron_pt);
   fChain->SetBranchAddress("electron_theta", &electron_theta, &b_electron_theta);
   fChain->SetBranchAddress("electron_charge", &electron_charge, &b_electron_charge);
   fChain->SetBranchAddress("electron_is_LHVeryLoose", &electron_is_LHVeryLoose, &b_electron_is_LHVeryLoose);
   fChain->SetBranchAddress("electron_is_LHLoose", &electron_is_LHLoose, &b_electron_is_LHLoose);
   fChain->SetBranchAddress("electron_is_LHMedium", &electron_is_LHMedium, &b_electron_is_LHMedium);
   fChain->SetBranchAddress("electron_is_LHTight", &electron_is_LHTight, &b_electron_is_LHTight);
   fChain->SetBranchAddress("electron_trigger_thresholds_passed", &electron_trigger_thresholds_passed, &b_electron_trigger_thresholds_passed);
   fChain->SetBranchAddress("electron_d0", &electron_d0, &b_electron_d0);
   fChain->SetBranchAddress("electron_d0sig", &electron_d0sig, &b_electron_d0sig);
   fChain->SetBranchAddress("electron_z0", &electron_z0, &b_electron_z0);
   fChain->SetBranchAddress("electron_z0Abs", &electron_z0Abs, &b_electron_z0Abs);
   fChain->SetBranchAddress("electron_z0sinTheta", &electron_z0sinTheta, &b_electron_z0sinTheta);
   fChain->SetBranchAddress("electron_recoid_sf", &electron_recoid_sf, &b_electron_recoid_sf);
   fChain->SetBranchAddress("electron_recoid_sf_stat_up", &electron_recoid_sf_stat_up, &b_electron_recoid_sf_stat_up);
   fChain->SetBranchAddress("electron_recoid_sf_stat_down", &electron_recoid_sf_stat_down, &b_electron_recoid_sf_stat_down);
   fChain->SetBranchAddress("electron_recoid_sf_syst_up", &electron_recoid_sf_syst_up, &b_electron_recoid_sf_syst_up);
   fChain->SetBranchAddress("electron_recoid_sf_syst_down", &electron_recoid_sf_syst_down, &b_electron_recoid_sf_syst_down);
   fChain->SetBranchAddress("nElec", &nElec, &b_nElec);
   fChain->SetBranchAddress("acolt_et", &acolt_et, &b_acolt_et);
   fChain->SetBranchAddress("Ptlt_et", &Ptlt_et, &b_Ptlt_et);
   fChain->SetBranchAddress("mlt_et", &mlt_et, &b_mlt_et);
   fChain->SetBranchAddress("ylt_et", &ylt_et, &b_ylt_et);
   fChain->SetBranchAddress("dRlt_et", &dRlt_et, &b_dRlt_et);
   fChain->SetBranchAddress("dPhilt_et", &dPhilt_et, &b_dPhilt_et);
   fChain->SetBranchAddress("dEtalt_et", &dEtalt_et, &b_dEtalt_et);
   fChain->SetBranchAddress("acolts_ets", &acolts_ets, &b_acolts_ets);
   fChain->SetBranchAddress("Ptlts_ets", &Ptlts_ets, &b_Ptlts_ets);
   fChain->SetBranchAddress("mlts_ets", &mlts_ets, &b_mlts_ets);
   fChain->SetBranchAddress("ylts_ets", &ylts_ets, &b_ylts_ets);
   fChain->SetBranchAddress("dRlts_ets", &dRlts_ets, &b_dRlts_ets);
   fChain->SetBranchAddress("dPhilts_ets", &dPhilts_ets, &b_dPhilts_ets);
   fChain->SetBranchAddress("dEtalts_ets", &dEtalts_ets, &b_dEtalts_ets);
   fChain->SetBranchAddress("acoll_ee", &acoll_ee, &b_acoll_ee);
   fChain->SetBranchAddress("Ptll_ee", &Ptll_ee, &b_Ptll_ee);
   fChain->SetBranchAddress("mll_ee", &mll_ee, &b_mll_ee);
   fChain->SetBranchAddress("yll_ee", &yll_ee, &b_yll_ee);
   fChain->SetBranchAddress("dRll_ee", &dRll_ee, &b_dRll_ee);
   fChain->SetBranchAddress("dPhill_ee", &dPhill_ee, &b_dPhill_ee);
   fChain->SetBranchAddress("dEtall_ee", &dEtall_ee, &b_dEtall_ee);
   fChain->SetBranchAddress("muon_eta", &muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon_phi", &muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_pt", &muon_pt, &b_muon_pt);
   fChain->SetBranchAddress("muon_theta", &muon_theta, &b_muon_theta);
   fChain->SetBranchAddress("muon_charge", &muon_charge, &b_muon_charge);
   fChain->SetBranchAddress("muon_is_Loose", &muon_is_Loose, &b_muon_is_Loose);
   fChain->SetBranchAddress("muon_is_Medium", &muon_is_Medium, &b_muon_is_Medium);
   fChain->SetBranchAddress("muon_is_Tight", &muon_is_Tight, &b_muon_is_Tight);
   fChain->SetBranchAddress("muon_is_LowPt", &muon_is_LowPt, &b_muon_is_LowPt);
   fChain->SetBranchAddress("muon_is_MVALowPt", &muon_is_MVALowPt, &b_muon_is_MVALowPt);
   fChain->SetBranchAddress("muon_trigger_matched_L1", &muon_trigger_matched_L1, &b_muon_trigger_matched_L1);
   fChain->SetBranchAddress("muon_trigger_matched_HLT", &muon_trigger_matched_HLT, &b_muon_trigger_matched_HLT);
   fChain->SetBranchAddress("muon_d0", &muon_d0, &b_muon_d0);
   fChain->SetBranchAddress("muon_d0sig", &muon_d0sig, &b_muon_d0sig);
   fChain->SetBranchAddress("muon_z0", &muon_z0, &b_muon_z0);
   fChain->SetBranchAddress("muon_z0Abs", &muon_z0Abs, &b_muon_z0Abs);
   fChain->SetBranchAddress("muon_z0sinTheta", &muon_z0sinTheta, &b_muon_z0sinTheta);
   fChain->SetBranchAddress("muon_p_ID", &muon_p_ID, &b_muon_p_ID);
   fChain->SetBranchAddress("muon_p_MS", &muon_p_MS, &b_muon_p_MS);
   fChain->SetBranchAddress("muon_energy_loss", &muon_energy_loss, &b_muon_energy_loss);
   fChain->SetBranchAddress("muon_recoid_sf", &muon_recoid_sf, &b_muon_recoid_sf);
   fChain->SetBranchAddress("muon_recoid_sf_stat_up", &muon_recoid_sf_stat_up, &b_muon_recoid_sf_stat_up);
   fChain->SetBranchAddress("muon_recoid_sf_stat_down", &muon_recoid_sf_stat_down, &b_muon_recoid_sf_stat_down);
   fChain->SetBranchAddress("muon_recoid_sf_syst_up", &muon_recoid_sf_syst_up, &b_muon_recoid_sf_syst_up);
   fChain->SetBranchAddress("muon_recoid_sf_syst_down", &muon_recoid_sf_syst_down, &b_muon_recoid_sf_syst_down);
   fChain->SetBranchAddress("nMuon", &nMuon, &b_nMuon);
   fChain->SetBranchAddress("MSmuon_eta", &MSmuon_eta, &b_MSmuon_eta);
   fChain->SetBranchAddress("MSmuon_phi", &MSmuon_phi, &b_MSmuon_phi);
   fChain->SetBranchAddress("MSmuon_pt", &MSmuon_pt, &b_MSmuon_pt);
   fChain->SetBranchAddress("MSmuon_d0", &MSmuon_d0, &b_MSmuon_d0);
   fChain->SetBranchAddress("acolt_mt", &acolt_mt, &b_acolt_mt);
   fChain->SetBranchAddress("Ptlt_mt", &Ptlt_mt, &b_Ptlt_mt);
   fChain->SetBranchAddress("mlt_mt", &mlt_mt, &b_mlt_mt);
   fChain->SetBranchAddress("ylt_mt", &ylt_mt, &b_ylt_mt);
   fChain->SetBranchAddress("dRlt_mt", &dRlt_mt, &b_dRlt_mt);
   fChain->SetBranchAddress("dPhilt_mt", &dPhilt_mt, &b_dPhilt_mt);
   fChain->SetBranchAddress("dEtalt_mt", &dEtalt_mt, &b_dEtalt_mt);
   fChain->SetBranchAddress("acolts_mts", &acolts_mts, &b_acolts_mts);
   fChain->SetBranchAddress("Ptlts_mts", &Ptlts_mts, &b_Ptlts_mts);
   fChain->SetBranchAddress("mlts_mts", &mlts_mts, &b_mlts_mts);
   fChain->SetBranchAddress("ylts_mts", &ylts_mts, &b_ylts_mts);
   fChain->SetBranchAddress("dRlts_mts", &dRlts_mts, &b_dRlts_mts);
   fChain->SetBranchAddress("dPhilts_mts", &dPhilts_mts, &b_dPhilts_mts);
   fChain->SetBranchAddress("dEtalts_mts", &dEtalts_mts, &b_dEtalts_mts);
   fChain->SetBranchAddress("acoll_mm", &acoll_mm, &b_acoll_mm);
   fChain->SetBranchAddress("Ptll_mm", &Ptll_mm, &b_Ptll_mm);
   fChain->SetBranchAddress("mll_mm", &mll_mm, &b_mll_mm);
   fChain->SetBranchAddress("yll_mm", &yll_mm, &b_yll_mm);
   fChain->SetBranchAddress("dRll_mm", &dRll_mm, &b_dRll_mm);
   fChain->SetBranchAddress("dPhill_mm", &dPhill_mm, &b_dPhill_mm);
   fChain->SetBranchAddress("dEtall_mm", &dEtall_mm, &b_dEtall_mm);
   fChain->SetBranchAddress("acoll_em", &acoll_em, &b_acoll_em);
   fChain->SetBranchAddress("Ptll_em", &Ptll_em, &b_Ptll_em);
   fChain->SetBranchAddress("mll_em", &mll_em, &b_mll_em);
   fChain->SetBranchAddress("yll_em", &yll_em, &b_yll_em);
   fChain->SetBranchAddress("dRll_em", &dRll_em, &b_dRll_em);
   fChain->SetBranchAddress("dPhill_em", &dPhill_em, &b_dPhill_em);
   fChain->SetBranchAddress("dEtall_em", &dEtall_em, &b_dEtall_em);
   fChain->SetBranchAddress("mts", &mts, &b_mts);
   fChain->SetBranchAddress("Ptts", &Ptts, &b_Ptts);
   fChain->SetBranchAddress("yts", &yts, &b_yts);
   fChain->SetBranchAddress("Phits", &Phits, &b_Phits);
   fChain->SetBranchAddress("Etats", &Etats, &b_Etats);
   fChain->SetBranchAddress("Pttt_tt", &Pttt_tt, &b_Pttt_tt);
   fChain->SetBranchAddress("Pttts_tts", &Pttts_tts, &b_Pttts_tts);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p010", &bsm_sig_weight_atau_p010, &b_bsm_sig_weight_atau_p010);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p001", &bsm_sig_weight_atau_p001, &b_bsm_sig_weight_atau_p001);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p002", &bsm_sig_weight_atau_p002, &b_bsm_sig_weight_atau_p002);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p003", &bsm_sig_weight_atau_p003, &b_bsm_sig_weight_atau_p003);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p004", &bsm_sig_weight_atau_p004, &b_bsm_sig_weight_atau_p004);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p005", &bsm_sig_weight_atau_p005, &b_bsm_sig_weight_atau_p005);
   fChain->SetBranchAddress("bsm_sig_weight_atau_p006", &bsm_sig_weight_atau_p006, &b_bsm_sig_weight_atau_p006);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m010", &bsm_sig_weight_atau_m010, &b_bsm_sig_weight_atau_m010);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m001", &bsm_sig_weight_atau_m001, &b_bsm_sig_weight_atau_m001);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m002", &bsm_sig_weight_atau_m002, &b_bsm_sig_weight_atau_m002);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m003", &bsm_sig_weight_atau_m003, &b_bsm_sig_weight_atau_m003);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m004", &bsm_sig_weight_atau_m004, &b_bsm_sig_weight_atau_m004);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m005", &bsm_sig_weight_atau_m005, &b_bsm_sig_weight_atau_m005);
   fChain->SetBranchAddress("bsm_sig_weight_atau_m006", &bsm_sig_weight_atau_m006, &b_bsm_sig_weight_atau_m006);
   fChain->SetBranchAddress("electron_pt_syst_EG_RESOLUTION_ALL__1down", &electron_pt_syst_EG_RESOLUTION_ALL__1down, &b_electron_pt_syst_EG_RESOLUTION_ALL__1down);
   fChain->SetBranchAddress("photon_pt_syst_EG_RESOLUTION_ALL__1down", &photon_pt_syst_EG_RESOLUTION_ALL__1down, &b_photon_pt_syst_EG_RESOLUTION_ALL__1down);
   fChain->SetBranchAddress("electron_pt_syst_EG_RESOLUTION_ALL__1up", &electron_pt_syst_EG_RESOLUTION_ALL__1up, &b_electron_pt_syst_EG_RESOLUTION_ALL__1up);
   fChain->SetBranchAddress("photon_pt_syst_EG_RESOLUTION_ALL__1up", &photon_pt_syst_EG_RESOLUTION_ALL__1up, &b_photon_pt_syst_EG_RESOLUTION_ALL__1up);
   fChain->SetBranchAddress("electron_pt_syst_EG_SCALE_ALL__1down", &electron_pt_syst_EG_SCALE_ALL__1down, &b_electron_pt_syst_EG_SCALE_ALL__1down);
   fChain->SetBranchAddress("photon_pt_syst_EG_SCALE_ALL__1down", &photon_pt_syst_EG_SCALE_ALL__1down, &b_photon_pt_syst_EG_SCALE_ALL__1down);
   fChain->SetBranchAddress("electron_pt_syst_EG_SCALE_ALL__1up", &electron_pt_syst_EG_SCALE_ALL__1up, &b_electron_pt_syst_EG_SCALE_ALL__1up);
   fChain->SetBranchAddress("photon_pt_syst_EG_SCALE_ALL__1up", &photon_pt_syst_EG_SCALE_ALL__1up, &b_photon_pt_syst_EG_SCALE_ALL__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_ID__1down", &muon_pt_syst_MUON_ID__1down, &b_muon_pt_syst_MUON_ID__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_ID__1up", &muon_pt_syst_MUON_ID__1up, &b_muon_pt_syst_MUON_ID__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_MS__1down", &muon_pt_syst_MUON_MS__1down, &b_muon_pt_syst_MUON_MS__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_MS__1up", &muon_pt_syst_MUON_MS__1up, &b_muon_pt_syst_MUON_MS__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_DATASTAT__1down", &muon_pt_syst_MUON_SAGITTA_DATASTAT__1down, &b_muon_pt_syst_MUON_SAGITTA_DATASTAT__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_DATASTAT__1up", &muon_pt_syst_MUON_SAGITTA_DATASTAT__1up, &b_muon_pt_syst_MUON_SAGITTA_DATASTAT__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_GLOBAL__1down", &muon_pt_syst_MUON_SAGITTA_GLOBAL__1down, &b_muon_pt_syst_MUON_SAGITTA_GLOBAL__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_GLOBAL__1up", &muon_pt_syst_MUON_SAGITTA_GLOBAL__1up, &b_muon_pt_syst_MUON_SAGITTA_GLOBAL__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down", &muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down, &b_muon_pt_syst_MUON_SAGITTA_PTEXTRA__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up", &muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up, &b_muon_pt_syst_MUON_SAGITTA_PTEXTRA__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_RESBIAS__1down", &muon_pt_syst_MUON_SAGITTA_RESBIAS__1down, &b_muon_pt_syst_MUON_SAGITTA_RESBIAS__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SAGITTA_RESBIAS__1up", &muon_pt_syst_MUON_SAGITTA_RESBIAS__1up, &b_muon_pt_syst_MUON_SAGITTA_RESBIAS__1up);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SCALE__1down", &muon_pt_syst_MUON_SCALE__1down, &b_muon_pt_syst_MUON_SCALE__1down);
   fChain->SetBranchAddress("muon_pt_syst_MUON_SCALE__1up", &muon_pt_syst_MUON_SCALE__1up, &b_muon_pt_syst_MUON_SCALE__1up);
   Notify();
}

bool Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void Analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Analysis_cxx
