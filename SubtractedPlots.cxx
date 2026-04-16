    // --- PRZYGOTOWANIE FUNKCJI DO DANYCH (tylko Norm jest wolna) ---
    // Pobieramy parametry z dopasowania MC, które już wykonałeś
    for(int i=1; i<7; i++) {
        MCmassMuCb->FixParameter(i, MCmassMuCb->GetParameter(i));
    }

    // Opcjonalnie: ustawiamy nową startową normalizację na maksimum nowego histogramu
    MCmassMuCb->SetParameter(0, DiMassMuSub->GetMaximum());

    // Fitowanie - teraz ROOT zmieni tylko parametr [0] (Norm)
    DiMassMuSub->Fit("MCmassMuCb", "RL");
    //double muTruechi2_ndf  = MCmassMuCb->GetChisquare() / MCmassMuCb->GetNDF();

    // --- RYSOWANIE Z RATIO PLOTEM ---
    TCanvas *cSubMu = new TCanvas("cSubMu", "Fit Subtracted Mu", 800, 800);
    TPad *p1m = new TPad("p1m","p1m",0,0.3,1,1);
    TPad *p2m = new TPad("p2m","p2m",0,0,1,0.3);
    p1m->SetBottomMargin(0); p2m->SetTopMargin(0); p2m->SetBottomMargin(0.35);
    p1m->Draw(); p2m->Draw();

    p1m->cd();
    DiMassMuSub->SetTitle("J/#psi #rightarrow #mu#mu : Data vs MC Fit;Mass [GeV];Events");
    DiMassMuSub->GetXaxis()->SetRangeUser(2.8, 3.4);
    DiMassMuSub->SetLineColor(kBlack);

    DiMassMuSub->Draw("E");
    MCmassMuCb->Draw("same");
    legEFF->Draw("same");


    p2m->cd();
    TH1F *hRatioSubMu = (TH1F*)DiMassMuSub->Clone("hRatioSubMu");
    hRatioSubMu->Reset();
    for (int i = 1; i <= DiMassMuSub->GetNbinsX(); i++) {
        double model = MCmassMuCb->Eval(DiMassMuSub->GetBinCenter(i));
        if (model > 0) {
            hRatioSubMu->SetBinContent(i, DiMassMuSub->GetBinContent(i) / model);
            hRatioSubMu->SetBinError(i, DiMassMuSub->GetBinError(i) / model);
        }
    }
    StyleRatio(hRatioSubMu, "Data / Fit");
    hRatioSubMu->Draw("E0");
    TLine *l2m = new TLine(2.8, 1.0, 3.4, 1.0);
    l2m->SetLineColor(kRed); l2m->SetLineStyle(2); l2m->Draw();

    cSubMu->SaveAs("Plots/SubtractedMu_Fit.png");

    // Zamrażamy parametry kształtu dla elektronów
    for(int i=1; i<7; i++) {
        MCmassElCb->FixParameter(i, MCmassElCb->GetParameter(i));
    }
    MCmassElCb->SetParameter(0, DiMassElSub->GetMaximum());

    DiMassElSub->Fit("MCmassElCb", "RL");
    
     


    // Kanwa i Ratio dla elektronów
    TCanvas *cSubEl = new TCanvas("cSubEl", "Fit Subtracted El", 800, 800);
    TPad *p1e = new TPad("p1e","p1e",0,0.3,1,1);
    TPad *p2e = new TPad("p2e","p2e",0,0,1,0.3);
    p1e->SetBottomMargin(0); p2e->SetTopMargin(0); p2e->SetBottomMargin(0.35);
    p1e->Draw(); p2e->Draw();

    p1e->cd();
    DiMassElSub->SetTitle("J/#psi #rightarrow ee : Data vs MC Fit;Mass [GeV];Events");
    DiMassElSub->SetLineColor(kBlack);
    DiMassElSub->Draw("E");
    MCmassElCb->Draw("same");
    legEFF->Draw("same");


    p2e->cd();
    TH1F *hRatioSubEl = (TH1F*)DiMassElSub->Clone("hRatioSubEl");
    hRatioSubEl->Reset();
    for (int i = 1; i <= DiMassElSub->GetNbinsX(); i++) {
        double model = MCmassElCb->Eval(DiMassElSub->GetBinCenter(i));
        if (model > 0) {
            hRatioSubEl->SetBinContent(i, DiMassElSub->GetBinContent(i) / model);
            hRatioSubEl->SetBinError(i, DiMassElSub->GetBinError(i) / model);
        }
    }
    StyleRatio(hRatioSubEl, "Data / Fit");
    hRatioSubEl->Draw("E0");
    TLine *l1e = new TLine(hRatioSubEl->GetXaxis()->GetXmin(), 1.0, hRatioSubEl->GetXaxis()->GetXmax(), 1.0);
    l1e->SetLineColor(kRed); l1e->SetLineStyle(2); l1e->Draw();

    cSubEl->SaveAs("Plots/SubtractedEl_Fit.png");