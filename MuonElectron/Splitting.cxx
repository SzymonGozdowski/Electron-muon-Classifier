void Splitting() {
    ROOT::RDataFrame df("G2TauTree", "../Data/data23_2trk_moreTCvars.root");

    cout<< "First File!"<<endl;
    auto df_even = df.Filter("rdfentry_ % 2 == 0");
    df_even.Snapshot("G2TauTree", "../Data/DataTestingFile.root");

    cout<< "Second File!"<<endl;
    auto df_odd = df.Filter("rdfentry_ % 2 != 0");
    df_odd.Snapshot("G2TauTree", "../Data/DataTrainingFile.root");

    cout << "Done! Splitted!" << endl;
}