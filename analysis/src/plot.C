#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <iostream>


//dopisać funcję DrawHist(TH1D hist, bool set_logy, ...?)
// dopisać funkcję GetHist(const char* inputFile) - get hists if they exist, skip if they don't



TH1D* GetHist(TFile *f, const char* hist_name){ // f - input file provided by plot() function

    if(!hist_name){
        std::cerr << "[ERROR] Invalid hist_name (NULL)!" << std::endl;
        return nullptr;
    }
     TH1D *hist_g = (TH1D*)f->Get(hist_name);
     if(!hist_g){
        std::cerr << "[ERROR] No histogram: "<< hist_name << std::endl;
        return nullptr;
    }
    hist_g->SetDirectory(0);
    return hist_g;
};

void DrawHist(std::string fPDF_name, TCanvas* c, TH1D* hist_d, bool logy){ // mozna dodac inne ustawienia do rysowania

    if(!c){
        std::cerr << "[ERROR] No existing TCanvas. Provide TCanvas (in plot())" << std::endl;
        return;
    }
    if(!hist_d){
        std::cerr << "[ERROR] Invalid histogram!" << std::endl;
        return; 
    }
    if(hist_d->GetEntries() == 0){
        std::cout << "[INFO] Skipping empty histogram: " << hist_d->GetName() <<std::endl;
        return;
    }
    if(fPDF_name.empty())
    {
        std::cerr << "[ERROR] Invalid PDF name (empty). Provide correct pdf name (plot())." << std::endl;
        return;
    }
    c->Clear();
    c->SetLogy(logy);
    hist_d->Draw();
    c->Print(fPDF_name.c_str());
};



//


void plot(const char* inputFile, const char* outputPDF){
    TFile *f = TFile::Open(inputFile);
    if (!f || f->IsZombie()) {
        std::cout << "ERROR: Cannot open input file." << std::endl;
        return;
    }

    std::cout << "[INFO] Starting pdf plot." << std::endl;
    TH1D* hist_track_theta = GetHist(f, "hist_track_theta");
    TH1D* hist_track_phi = GetHist(f, "hist_track_phi");
    TH1D* hist_track_pt = GetHist(f, "hist_track_pt");
    TH1D* hist_track_eta = GetHist(f, "hist_track_eta");
    //
    TH1D* hist_track_pt_cut = GetHist(f, "hist_track_pt_cut");
    TH1D* hist_track_eta_cut = GetHist(f, "hist_track_eta");
    //
    TH1D* hist_dilepton_inv_mass = GetHist(f, "hist_dilepton_inv_mass");
    TH1D* hist_dilepton_pt = GetHist(f, "hist_dilepton_pt");
    TH1D* hist_dilepton_rapidity = GetHist(f, "hist_dilepton_rapidity");
    //
    TH1D* hist_dilepton_number = GetHist(f, "hist_dilepton_number");
    TH1D* hist_dilepton_DR = GetHist(f, "hist_dilepton_DR");
    //
    TH1D* hist_electron_number = GetHist(f, "hist_electron_number");
    TH1D* hist_electron_E = GetHist(f, "hist_electron_E");
    TH1D* hist_electron_Pt = GetHist(f, "hist_electron_Pt");
    TH1D* hist_electron_Eta = GetHist(f, "hist_electron_Eta");
    TH1D* hist_electron_Phi = GetHist(f, "hist_electron_Phi");
    //
    TH1D* hist_muon_number = GetHist(f, "hist_muon_number");
    TH1D* hist_muon_E = GetHist(f, "hist_muon_E");
    TH1D* hist_muon_Pt = GetHist(f, "hist_muon_Pt");
    TH1D* hist_muon_Eta = GetHist(f, "hist_muon_Eta");
    TH1D* hist_muon_Phi = GetHist(f, "hist_muon_Phi");
    //
    TH1D* hist_track_PixeldEdX = GetHist(f, "hist_track_PixeldEdX");
    TH1D* hist_track_PixelHits = GetHist(f, "hist_track_PixelHits");
    TH1D* hist_track_SCTHits = GetHist(f, "hist_track_SCTHits");
    TH1D* hist_track_TRTHits = GetHist(f, "hist_track_TRTHits");
    //
    TH1D* hist_electron_PixeldEdX = GetHist(f, "hist_electron_PixeldEdX");
    TH1D* hist_electron_PixelHits = GetHist(f, "hist_electron_PixelHits");
    TH1D* hist_electron_SCTHits = GetHist(f, "hist_electron_SCTHits");
    TH1D* hist_electron_TRTHits = GetHist(f, "hist_electron_TRTHits");
    //
    TH1D* hist_muon_PixeldEdX = GetHist(f, "hist_muon_PixeldEdX");
    TH1D* hist_muon_PixelHits = GetHist(f, "hist_muon_PixelHits");
    TH1D* hist_muon_SCTHits = GetHist(f, "hist_muon_SCTHits");
    TH1D* hist_muon_TRTHits = GetHist(f, "hist_muon_TRTHits");




    TCanvas *c = new TCanvas("c", "c", 800, 600);

    // otwarcie PDF
    std::string pdfStart = std::string(outputPDF) + "[";
    std::string pdfMiddle = std::string(outputPDF);
    std::string pdfEnd   = std::string(outputPDF) + "]";
    
    c->SaveAs(pdfStart.c_str());
    
    //c->SetLogy();
    //h1->Draw();
    //c->Print(pdfMiddle.c_str());
    
    DrawHist(pdfMiddle, c, hist_track_theta, false);
    DrawHist(pdfMiddle, c, hist_track_phi, false);

    DrawHist(pdfMiddle, c, hist_track_pt, true);
    DrawHist(pdfMiddle, c, hist_track_pt_cut, true);

    DrawHist(pdfMiddle, c, hist_track_eta, false);
    DrawHist(pdfMiddle, c, hist_track_eta_cut, false);

    DrawHist(pdfMiddle, c, hist_track_PixeldEdX, false);
    DrawHist(pdfMiddle, c, hist_track_PixelHits, false);
    DrawHist(pdfMiddle, c, hist_track_SCTHits, false);
    DrawHist(pdfMiddle, c, hist_track_TRTHits, false);

    //
    DrawHist(pdfMiddle, c, hist_dilepton_inv_mass, false);
    DrawHist(pdfMiddle, c, hist_dilepton_pt, true);
    DrawHist(pdfMiddle, c, hist_dilepton_rapidity, false);
    DrawHist(pdfMiddle, c, hist_dilepton_number, false);
    DrawHist(pdfMiddle, c, hist_dilepton_DR, false);
    //
    DrawHist(pdfMiddle, c, hist_electron_number, false);
    c->Divide(2, 2);
    c->cd(1);
    DrawHist(pdfMiddle, c, hist_electron_E, false);
    c->cd(2);
    DrawHist(pdfMiddle, c, hist_electron_Pt, true);
    c->cd(3);
    DrawHist(pdfMiddle, c, hist_electron_Eta, false);
    c->cd(4);
    DrawHist(pdfMiddle, c, hist_electron_Phi, false);

    DrawHist(pdfMiddle, c, hist_electron_PixeldEdX, false);
    DrawHist(pdfMiddle, c, hist_electron_PixelHits, false);
    DrawHist(pdfMiddle, c, hist_electron_SCTHits, false);
    DrawHist(pdfMiddle, c, hist_electron_TRTHits, false);

    
    //
    DrawHist(pdfMiddle, c, hist_muon_number, false);
    c->Divide(2, 2);
    c->cd(1);
    DrawHist(pdfMiddle, c, hist_muon_E, false);
    c->cd(2);
    DrawHist(pdfMiddle, c, hist_muon_Pt, true);
    c->cd(3);
    DrawHist(pdfMiddle, c, hist_muon_Eta, false);
    c->cd(4);
    DrawHist(pdfMiddle, c, hist_muon_Phi, false);

    DrawHist(pdfMiddle, c, hist_muon_PixeldEdX, false);
    DrawHist(pdfMiddle, c, hist_muon_PixelHits, false);
    DrawHist(pdfMiddle, c, hist_muon_SCTHits, false);
    DrawHist(pdfMiddle, c, hist_muon_TRTHits, false);


    // Zamknij PDF
    c->SaveAs(pdfEnd.c_str());
    std::cout << "[OK] PDFs saved." << std::endl;


    delete c;
    f->Close();
    std::cout << "[DONE]" << std::endl;
}
