#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <vector>
#include <iostream>
#include <cmath>

void Sig_bckg_stack()
{
    // =========================
    // CONSTRUCTING
    // =========================

    // FILES
    TFile* f_data = new TFile("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/control_histograms/out_hists_data_with_sig_sampling.root");

    TFile* f_mc = new TFile("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/control_histograms/hists_mc_prep_ml_combined.root");


    // =========================
    // DATA
    // =========================
    TH1D* h_data_mass = (TH1D*)f_data->Get("hist_dilepton_inv_mass");
    TH1D* h_data_pt = (TH1D*)f_data->Get("hist_dilepton_pt");
    h_data_mass->Sumw2();
    h_data_pt->Sumw2();

    // =========================
    // MC EE
    // =========================
    TH1D* h_ee_mass = (TH1D*)f_mc->Get("hist_dilepton_mass_ee");
    TH1D* h_ee_pt = (TH1D*)f_mc->Get("hist_dilepton_pt_ee");

    // =========================
    // MC MUMU
    // =========================
    TH1D* h_mumu_mass = (TH1D*)f_mc->Get("hist_dilepton_mass_mumu");
    TH1D* h_mumu_pt = (TH1D*)f_mc->Get("hist_dilepton_pt_mumu");

    // =========================
    // MC SUM
    // =========================
    TH1D* h_mc_mass = (TH1D*)h_ee_mass->Clone("h_mc_mass");
    h_mc_mass->Add(h_mumu_mass);

    TH1D* h_mc_pt = (TH1D*)h_ee_pt->Clone("h_mc_pt");
    h_mc_pt->Add(h_mumu_pt);

    // =========================
    // SCALING
    // =========================
    double scale_mass = h_data_mass->Integral() / h_mc_mass->Integral();
    double scale_pt = h_data_pt->Integral() / h_mc_pt->Integral();

    // --- MASS copies ---
    TH1D* h_ee_mass_scaled   = (TH1D*)h_ee_mass->Clone("h_ee_mass_scaled");
    TH1D* h_mumu_mass_scaled = (TH1D*)h_mumu_mass->Clone("h_mumu_mass_scaled");
    TH1D* h_mc_mass_scaled   = (TH1D*)h_mc_mass->Clone("h_mc_mass_scaled");

    // --- PT copies ---
    TH1D* h_ee_pt_scaled   = (TH1D*)h_ee_pt->Clone("h_ee_pt_scaled");
    TH1D* h_mumu_pt_scaled = (TH1D*)h_mumu_pt->Clone("h_mumu_pt_scaled");
    TH1D* h_mc_pt_scaled   = (TH1D*)h_mc_pt->Clone("h_mc_pt_scaled");

    h_mc_mass_scaled->Scale(scale_mass);
    h_ee_mass_scaled->Scale(scale_mass);
    h_mumu_mass_scaled->Scale(scale_mass);

    h_mc_pt_scaled->Scale(scale_pt);
    h_ee_pt_scaled->Scale(scale_pt);
    h_mumu_pt_scaled->Scale(scale_pt);


    // =========================
    // PLOTTING
    // =========================

    TString outPdf = "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/plots/stack_plots.pdf";

    TLine* line = nullptr; // reusable pointer

    // =====================================================
    // 1) MASS RAW
    // =====================================================
    TCanvas* c_mass_raw = new TCanvas("c_mass_raw","mass raw",800,800);

    TPad* p1 = new TPad("p1","",0,0.3,1,1);
    TPad* p2 = new TPad("p2","",0,0,1,0.3);

    p1->SetBottomMargin(0.08);
    p2->SetTopMargin(0.05);
    p2->SetBottomMargin(0.3);

    p1->Draw(); p2->Draw();

    // --- TOP PAD ---
    p1->cd();

    THStack* st_mass = new THStack("st_mass","Dilepton mass (raw)");

    h_ee_mass->SetFillColor(kRed);
    h_mumu_mass->SetFillColor(kBlue);

    st_mass->Add(h_mumu_mass);
    st_mass->Add(h_ee_mass);

    st_mass->Draw("HIST");
    st_mass->GetXaxis()->SetTitle("m_{ll} [GeV]");
    st_mass->GetYaxis()->SetTitle("Events");

    h_mc_mass->SetLineColor(kGray+2);
    h_mc_mass->SetLineWidth(2);
    h_mc_mass->Draw("HIST SAME");

    h_data_mass->SetMarkerStyle(20);
    h_data_mass->Draw("EP SAME");

    TLegend* leg1 = new TLegend(0.6,0.6,0.88,0.88);
    leg1->AddEntry(h_data_mass,"DATA","lep");
    leg1->AddEntry(h_mc_mass,"MC sum","l");
    leg1->AddEntry(h_ee_mass,"MC ee","f");
    leg1->AddEntry(h_mumu_mass,"MC mumu","f");
    leg1->Draw();

    // --- RATIO ---
    p2->cd();

    TH1D* r_mass = (TH1D*)h_data_mass->Clone("r_mass");
    r_mass->Divide(h_mc_mass);

    r_mass->SetMarkerStyle(20);
    r_mass->GetYaxis()->SetTitle("DATA / MC");
    r_mass->GetXaxis()->SetTitle("m_{ll} [GeV]");
    r_mass->Draw("EP");

    // line = 1
    line = new TLine(r_mass->GetXaxis()->GetXmin(),1,
                    r_mass->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(2);
    line->Draw("SAME");

    c_mass_raw->Print(outPdf + "(");

    // =====================================================
    // 2) MASS SCALED
    // =====================================================
    TCanvas* c_mass_scaled = new TCanvas("c_mass_scaled","mass scaled",800,800);

    TPad* p3 = new TPad("p3","",0,0.3,1,1);
    TPad* p4 = new TPad("p4","",0,0,1,0.3);

    p3->SetBottomMargin(0.08);
    p4->SetTopMargin(0.05);
    p4->SetBottomMargin(0.3);

    p3->Draw(); p4->Draw();

    p3->cd();

    THStack* st_mass_s = new THStack("st_mass_s","Dilepton mass (scaled)");

    h_ee_mass_scaled->SetFillColor(kRed);
    h_mumu_mass_scaled->SetFillColor(kBlue);

    st_mass_s->Add(h_mumu_mass_scaled);
    st_mass_s->Add(h_ee_mass_scaled);

    st_mass_s->Draw("HIST");
    st_mass_s->GetXaxis()->SetTitle("m_{ll} [GeV]");
    st_mass_s->GetYaxis()->SetTitle("Events");

    h_mc_mass_scaled->SetLineColor(kGray+2);
    h_mc_mass_scaled->SetLineWidth(2);
    h_mc_mass_scaled->Draw("HIST SAME");

    h_data_mass->Draw("EP SAME");
    leg1->Draw();

    p4->cd();

    TH1D* r_mass_s = (TH1D*)h_data_mass->Clone("r_mass_s");
    r_mass_s->Divide(h_mc_mass_scaled);

    r_mass_s->SetMarkerStyle(20);
    r_mass_s->GetYaxis()->SetTitle("DATA / MC");
    r_mass_s->GetXaxis()->SetTitle("m_{ll} [GeV]");
    r_mass_s->Draw("EP");

    line = new TLine(r_mass_s->GetXaxis()->GetXmin(),1,
                    r_mass_s->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(2);
    line->Draw("SAME");

    c_mass_scaled->Print(outPdf);

    // =====================================================
    // 3) PT RAW
    // =====================================================
    TCanvas* c_pt_raw = new TCanvas("c_pt_raw","pt raw",800,800);

    TPad* p5 = new TPad("p5","",0,0.3,1,1);
    TPad* p6 = new TPad("p6","",0,0,1,0.3);

    p5->SetBottomMargin(0.08);
    p6->SetTopMargin(0.05);
    p6->SetBottomMargin(0.3);

    p5->Draw(); p6->Draw();

    p5->cd();

    THStack* st_pt = new THStack("st_pt","Dilepton pT (raw)");

    h_ee_pt->SetFillColor(kRed);
    h_mumu_pt->SetFillColor(kBlue);

    st_pt->Add(h_mumu_pt);
    st_pt->Add(h_ee_pt);

    st_pt->Draw("HIST");
    st_pt->GetXaxis()->SetTitle("p_{T}^{ll} [GeV]");
    st_pt->GetYaxis()->SetTitle("Events");

    h_mc_pt->SetLineColor(kGray+2);
    h_mc_pt->SetLineWidth(2);
    h_mc_pt->Draw("HIST SAME");

    h_data_pt->Draw("EP SAME");
    leg1->Draw();

    p6->cd();

    TH1D* r_pt = (TH1D*)h_data_pt->Clone("r_pt");
    r_pt->Divide(h_mc_pt);

    r_pt->SetMarkerStyle(20);
    r_pt->GetYaxis()->SetTitle("DATA / MC");
    r_pt->GetXaxis()->SetTitle("p_{T}^{ll} [GeV]");
    r_pt->Draw("EP");

    line = new TLine(r_pt->GetXaxis()->GetXmin(),1,
                    r_pt->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(2);
    line->Draw("SAME");

    c_pt_raw->Print(outPdf);

    // =====================================================
    // 4) PT SCALED
    // =====================================================
    TCanvas* c_pt_scaled = new TCanvas("c_pt_scaled","pt scaled",800,800);

    TPad* p7 = new TPad("p7","",0,0.3,1,1);
    TPad* p8 = new TPad("p8","",0,0,1,0.3);

    p7->SetBottomMargin(0.08);
    p8->SetTopMargin(0.05);
    p8->SetBottomMargin(0.3);

    p7->Draw(); p8->Draw();

    p7->cd();

    THStack* st_pt_s = new THStack("st_pt_s","Dilepton pT (scaled)");

    h_ee_pt_scaled->SetFillColor(kRed);
    h_mumu_pt_scaled->SetFillColor(kBlue);

    st_pt_s->Add(h_mumu_pt_scaled);
    st_pt_s->Add(h_ee_pt_scaled);

    st_pt_s->Draw("HIST");
    st_pt_s->GetXaxis()->SetTitle("p_{T}^{ll} [GeV]");
    st_pt_s->GetYaxis()->SetTitle("Events");

    h_mc_pt_scaled->SetLineColor(kGray+2);
    h_mc_pt_scaled->SetLineWidth(2);
    h_mc_pt_scaled->Draw("HIST SAME");

    h_data_pt->Draw("EP SAME");
    leg1->Draw();

    p8->cd();

    TH1D* r_pt_s = (TH1D*)h_data_pt->Clone("r_pt_s");
    r_pt_s->Divide(h_mc_pt_scaled);

    r_pt_s->SetMarkerStyle(20);
    r_pt_s->GetYaxis()->SetTitle("DATA / MC");
    r_pt_s->GetXaxis()->SetTitle("p_{T}^{ll} [GeV]");
    r_pt_s->Draw("EP");

    line = new TLine(r_pt_s->GetXaxis()->GetXmin(),1,
                    r_pt_s->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(2);
    line->Draw("SAME");

    c_pt_scaled->Print(outPdf + ")");

    std::cout << "[OK] PDF saved: " << outPdf << std::endl;

    c_mass_raw->SaveAs("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/plots/stack_mass_raw.png");
    c_mass_scaled->SaveAs("/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/plots/stack_mass_scaled.png");
    std::cout << "[OK] PNGs saved. " << std::endl;

    
}