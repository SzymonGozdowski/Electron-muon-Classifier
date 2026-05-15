auto doubleCB = [](Double_t *x, Double_t *par) -> Double_t {

        double t = (x[0] - par[1]) / par[2];
        double absAL = fabs(par[3]);
        double absAR = fabs(par[5]);

        if (t < -absAL) {
            double a = pow(par[4] / absAL, par[4]) * exp(-0.5 * absAL * absAL);
            double b = par[4] / absAL - absAL;
            return par[0] * a / pow(b - t, par[4]);
        } else if (t > absAR) {
            double a = pow(par[6] / absAR, par[6]) * exp(-0.5 * absAR * absAR);
            double b = par[6] / absAR - absAR;
            return par[0] * a / pow(b + t, par[6]);
        } else {
            return par[0] * exp(-0.5 * t * t);
        }
};

void Plotting()
{
    gROOT->SetBatch(kTRUE);
    gStyle->SetOptStat(000000);

    //=========================
    //Importing data files
    //=========================
    
    TFile *DataFile = new TFile("Plots/DataProton_hist.root","READ");
    TFile *MCProtonFile = new TFile("Plots/MCProton_hist.root","READ");
    TFile *BackgroundJpsiFile = new TFile("Plots/Background_Jpsi_EE_hist.root","READ");
    TFile *BackgroundyyFile = new TFile("Plots/Background_yy_EE_hist.root","READ");


    
    //Plotting J/Psi mass Contol Plots

        TH1D *DiMassDataProton = (TH1D*)DataFile->Get("DiMassProton_DataProton");
        TH1D *DiMassMCProton = (TH1D*)MCProtonFile->Get("Dimass_MCProton");
        TH1D *DiMassBackgroundJpsi = (TH1D*)BackgroundJpsiFile->Get("Dimass_Background_Jpsi_EE");
        TH1D *DiMassBackgroundyy = (TH1D*)BackgroundyyFile->Get("Dimass_Background_yy_EE");

        DiMassDataProton->Scale( 1.0 / DiMassDataProton->GetBinWidth(1) );
        DiMassMCProton->Scale( 1.0 / DiMassMCProton->GetBinWidth(1) );
        DiMassBackgroundJpsi->Scale( 1.0 / DiMassBackgroundJpsi->GetBinWidth(1) );
        DiMassBackgroundyy->Scale( 1.0 / DiMassBackgroundyy->GetBinWidth(1) );

        //=========================
        //Plotting Double-sided crystal ball from J/Psi Electron Monte Carlo
        //=========================
        TF1 *FunPrCb = new TF1("FunPrCb", doubleCB, 2.92, 3.36, 7);
        FunPrCb->SetParameters(
            DiMassMCProton->GetMaximum(),  // Norm
            3.097,                       // Mean
            0.02,                        // Sigma
            1.5,                         // Alpha_L
            5.0,                         // N_L
            1.5,                         // Alpha_R
            5.0                          // N_R
        );
        FunPrCb->SetParLimits(3, 0.1,   10.0);   // Alpha_L
        FunPrCb->SetParLimits(4, 1.0,   50.0);   // N_L
        FunPrCb->SetParLimits(5, 0.1,   10.0);   // Alpha_R
        FunPrCb->SetParLimits(6, 1.0,   50.0);   // N_R
        DiMassMCProton->Fit("FunPrCb", "RLQ"); 


        TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
        DiMassMCProton->SetLineColor(kBlack);
        DiMassMCProton->SetTitle("J/#psi #rightarrow ee : MC Proton;Mass [GeV];Events");
        DiMassMCProton->Draw("E");
        FunPrCb->Draw("same");
        c1->SaveAs("Plots/MassPlots/MCProton_Fit.png");

        //=========================
        //Plotting Exp from J/Psi Electron Monte Carlo
        //=========================

        TF1 *FunBkgJPsiExp = new TF1("FunBkgJPsiExp", "[0]*exp([1]*x*x)", 2.92, 3.36);
        FunBkgJPsiExp->SetParameters(DiMassBackgroundJpsi->GetMaximum(),0.5);
        DiMassBackgroundJpsi->Fit("FunBkgJPsiExp", "RLQ"); 

        TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
        DiMassBackgroundJpsi->SetLineColor(kBlack);
        DiMassBackgroundJpsi->SetTitle("J/#psi #rightarrow ee : Background J/Psi;Mass [GeV];Events");
        DiMassBackgroundJpsi->Draw("E");
        FunBkgJPsiExp->Draw("same");
        c2->SaveAs("Plots/MassPlots/Background_Jpsi_Fit.png");

        //=========================
        //Plotting pol2 from yy Electron Monte Carlo
        //=========================

        TF1 *FunBkgYYPoly = new TF1("FunBkgYYPoly", "pol2", 2.92, 3.36);
        FunBkgYYPoly->SetParameters(
            DiMassBackgroundyy->GetMaximum(),   // p0
            0.0,                                // p1
            0.0                                 // p2
        );

        DiMassBackgroundyy->Fit("FunBkgYYPoly", "RLQ");

        TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
        DiMassBackgroundyy->SetLineColor(kBlack);
        DiMassBackgroundyy->SetTitle("J/#psi #rightarrow ee : Background J/Psi;Mass [GeV];Events");
        DiMassBackgroundyy->Draw("E");
        FunBkgYYPoly->Draw("same");
        c3->SaveAs("Plots/MassPlots/Background_yy_Fit.png");

        //=========================
        //Plotting J/Psi data
        //=========================

        TF1 *FunData = new TF1("FunData",
        [&](double *x, double *p) -> double {

            double cb = doubleCB(x, p);

            double expo = p[7] * TMath::Exp(p[8] * x[0]*x[0]);

            double poly = p[9] * ( p[10] + p[11]*x[0] + p[12]*x[0]*x[0] );

            return cb + expo + poly;
        },
        2.92, 3.36, 13);
        for (int i = 1; i < 7; i++) {
            FunData->FixParameter(i, FunPrCb->GetParameter(i));
        }
        FunData->FixParameter(8, FunBkgJPsiExp->GetParameter(1));
        FunData->FixParameter(10, FunBkgYYPoly->GetParameter(0));
        FunData->FixParameter(11, FunBkgYYPoly->GetParameter(1));
        FunData->FixParameter(12, FunBkgYYPoly->GetParameter(2));

        FunData->SetParameter(0, 200);
        FunData->SetParameter(7, 200);
        FunData->SetParameter(9, 200);

        FunData->SetParLimits(0, 0, 10000);
        FunData->SetParLimits(7, 0, 1000); 
        FunData->SetParLimits(9, 0, 1000); 
        
        DiMassDataProton->Fit("FunData", "RL");

        TF1 *FunDataBkg = new TF1("FunDataBkg",
        [&](double *x, double *p) -> double {

            double expo = p[0] * TMath::Exp(p[1] * x[0]*x[0]);

            double poly = p[2] * ( p[3] + p[4]*x[0] + p[5]*x[0]*x[0] );

            return  expo + poly;
        },
        2.92, 3.36, 6);
        for (int i = 0; i < 6; i++) {
            FunDataBkg->FixParameter(i, FunData->GetParameter(i+7));
        }

        TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);
        DiMassDataProton->SetLineColor(kBlack);
        DiMassDataProton->SetTitle("J/#psi #rightarrow ee : Data Proton;Mass [GeV];Events/GeV");
        DiMassDataProton->Draw("E");
        DiMassDataProton->GetXaxis()->SetRangeUser(2.8, 3.4);
        FunData->SetLineColor(kRed);
        FunData->Draw("same");
        FunDataBkg->SetLineColor(kGreen);
        FunDataBkg->Draw("same");
        TLegend *leg = new TLegend(0.6, 0.2, 0.88, 0.4);
        leg->SetBorderSize(0);
        leg->SetTextFont(42);
        leg->SetTextSize(0.04);
        leg->AddEntry(DiMassDataProton,"Data","L");
        leg->AddEntry(FunData,"Fit","L");
        leg->AddEntry(FunDataBkg,"Background","L");
        leg->Draw("same");

        c4->SaveAs("Plots/MassPlots/DataProton_Fit.png");
        cout<<"========================="<<endl;
        cout<<"Background Area: "<<FunDataBkg->Integral(2.92, 3.36)<<endl;
        cout<<"Signal Area: "<<FunData->Integral(2.92, 3.36)-FunDataBkg->Integral(2.92, 3.36)<<endl;
        cout<<"Area Ratio (Background/Signal): "<<FunDataBkg->Integral(2.92, 3.36)/(FunData->Integral(2.92, 3.36)-FunDataBkg->Integral(2.92, 3.36))<<endl;
        cout<<"========================="<<endl;



    //Plotting Efficiency Control Plots
        
        TH1D *EnergyElectronLike = (TH1D*)MCProtonFile->Get("EnergyElectronLikeProton_MCProton");
        TH1D *EtaElectronLike = (TH1D*)MCProtonFile->Get("EtaElectronLikeProton_MCProton");
        TH1D *ProtonEnergy = (TH1D*)MCProtonFile->Get("ProtonEnergy_MCProton");
        TH1D *ProtonEta = (TH1D*)MCProtonFile->Get("ProtonEta_MCProton");

        //==========================
        //Plotting Efficiency in MonteCarlo
        //==========================
        EnergyElectronLike = (TH1D*)EnergyElectronLike->Rebin(2, "EnergyElectronLike_rebinned");
        ProtonEnergy       = (TH1D*)ProtonEnergy->Rebin(2, "ProtonEnergy_rebinned");

        EtaElectronLike = (TH1D*)EtaElectronLike->Rebin(2, "EtaElectronLike_rebinned");
        ProtonEta       = (TH1D*)ProtonEta->Rebin(2, "ProtonEta_rebinned");
        TEfficiency *EnergyEfficiency = new TEfficiency(*EnergyElectronLike, *ProtonEnergy);
        TEfficiency *EtaEfficiency = new TEfficiency(*EtaElectronLike, *ProtonEta);

        TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
        EnergyEfficiency->SetTitle("Efficiency of Electron Energy Cut;Energy [GeV];Efficiency");
        EnergyEfficiency->Draw();
        c5->SaveAs("Plots/EfficiencyPlots/EnergyEfficiency.png");

        TCanvas *c6 = new TCanvas("c6", "c6", 800, 600);
        EtaEfficiency->SetTitle("Efficiency of Electron Eta Cut;Eta;Efficiency");
        EtaEfficiency->Draw();
        c6->SaveAs("Plots/EfficiencyPlots/EtaEfficiency.png");

    }

