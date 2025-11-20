void histAngles(){

TFile *f = TFile::Open("/Users/szymon/Downloads/jpsi_psi2s_extra_variables.root");

    if (!f || f->IsZombie()) {
        printf("Error: cannot open file!\n");
        return;
    }

    // Access the tree
    TTree *tree = (TTree*)f->Get("G2TauTree");
    if (!tree) {
        printf("Error: cannot find tree!\n");
        f->Close();
        return;
    }

TCanvas *c1 = new TCanvas();
TH1F *hist = new TH1F("hist", "Histogram Eta Angle;Eta Angle;Number of Tracks", 100, -4, 4);


std::vector<double> *eta = nullptr;
tree->SetBranchAddress("track_eta", &eta);

for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
  tree->GetEntry(i);
  if (eta) {
    for (double val: *eta){
     hist->Fill(val); 
    }
  }
}

hist->Draw();
f->Close();
}
