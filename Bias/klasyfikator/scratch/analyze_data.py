import pandas as pd
import numpy as np
import joblib

feature_columns = [
    "track_TRTHits", "track_PixelHits", "track_SCTHits", "track_PixeldEdX",
    "topo_cluster_eta", "topo_cluster_phi", "topo_cluster_EM_prob",
    "topo_cluster_pt", "pt_ratio", "topo_cluster_lambda", "topo_cluster_lambda2"
]

def load_and_filter(path):
    df = pd.read_csv(path)
    df_clean = df[
        (df['topo_cluster_eta'] != -10.0) &
        (df['topo_cluster_pt'] != -1.0) &
        (df['topo_cluster_phi'] != -10.0) &
        (df['topo_cluster_EM_prob'] != -1.0) &
        (df['pt_ratio'] != -1.0) &
        (df['topo_cluster_lambda'] != -1.0) &
        (df['topo_cluster_lambda2'] != -1.0)
    ]
    return df_clean

print("Analyzing Bias data (training for model 2)...")
df_ele_bias = load_and_filter("c:/Users/qmiko/Desktop/Studia/cern/Data_Separated/Bias/Filter_By_MLP/electron.csv")
df_muo_bias = load_and_filter("c:/Users/qmiko/Desktop/Studia/cern/Data_Separated/Bias/Filter_By_MLP/muon.csv")

print("Analyzing MC data (test data)...")
df_ele_mc = load_and_filter("c:/Users/qmiko/Desktop/Studia/cern/Data_Separated/MC/electron.csv")
df_muo_mc = load_and_filter("c:/Users/qmiko/Desktop/Studia/cern/Data_Separated/MC/muon.csv")

print("\nFeature means (Bias Electron vs MC Electron):")
for col in feature_columns:
    print(f"{col:<20} | Bias: {df_ele_bias[col].mean():>10.4f} | MC: {df_ele_mc[col].mean():>10.4f}")

print("\nFeature means (Bias Muon vs MC Muon):")
for col in feature_columns:
    print(f"{col:<20} | Bias: {df_muo_bias[col].mean():>10.4f} | MC: {df_muo_mc[col].mean():>10.4f}")

scaler1 = joblib.load("c:/Users/qmiko/Desktop/Studia/cern/Bias/klasyfikator/trained/scaler_mlp1.pkl")
print("\nScaler 1 params (mean):")
print(scaler1.mean_)
print("\nScaler 1 params (scale):")
print(scaler1.scale_)

scaler2 = joblib.load("c:/Users/qmiko/Desktop/Studia/cern/Bias/klasyfikator/trained/scaler_mlp2.pkl")
print("\nScaler 2 params (mean):")
print(scaler2.mean_)
print("\nScaler 2 params (scale):")
print(scaler2.scale_)
