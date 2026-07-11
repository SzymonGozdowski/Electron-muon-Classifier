import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import seaborn as sns

from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans, AgglomerativeClustering
from sklearn.mixture import GaussianMixture, BayesianGaussianMixture
from sklearn.metrics import silhouette_score, calinski_harabasz_score
from sklearn.pipeline import Pipeline

from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType


plt.rcParams.update({'font.size': 14})

with uproot.open("Data/MLDataTrueData.root") as f:
    df = f["MLDataTree"].arrays(library="pd")

features_list = [
    'track_PixelHits', 'track_TRTHits', 'track_SCTHits',
    'Cal_FVariable', 'Cal_EMprop',
    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius'
]

X = df[features_list].copy()


mask_finite = np.isfinite(X).all(axis=1)
X = X[mask_finite]
df = df[mask_finite].reset_index(drop=True)

print(f"\n{'='*55}")
print(f"  Number of particles: {len(X)}")
print(f"{'='*55}")


scaler = StandardScaler()
X_sc   = scaler.fit_transform(X)



N_CLUSTERS = 2  

kmeans_final = KMeans(n_clusters=N_CLUSTERS, n_init=10, random_state=42)
kmeans_labels = kmeans_final.fit_predict(X_sc)

gmm = GaussianMixture(
    n_components    = N_CLUSTERS,
    covariance_type = 'full',
    n_init          = 10,
    random_state    = 42
)
gmm_labels = gmm.fit_predict(X_sc)
gmm_probs  = gmm.predict_proba(X_sc)

bgm = BayesianGaussianMixture(
    n_components    = N_CLUSTERS,
    covariance_type = 'full',
    n_init          = 10,
    max_iter        = 500,
    random_state    = 42
)
bgm_labels = bgm.fit_predict(X_sc)

agg = AgglomerativeClustering(n_clusters=N_CLUSTERS, linkage='ward')
agg_labels = agg.fit_predict(X_sc)

sil_km  = silhouette_score(X_sc, kmeans_labels)
sil_gmm = silhouette_score(X_sc, gmm_labels)
sil_bgm = silhouette_score(X_sc, bgm_labels)
sil_agg = silhouette_score(X_sc, agg_labels)

cal_km  = calinski_harabasz_score(X_sc, kmeans_labels)
cal_gmm = calinski_harabasz_score(X_sc, gmm_labels)
cal_bgm = calinski_harabasz_score(X_sc, bgm_labels)
cal_agg = calinski_harabasz_score(X_sc, agg_labels)

print(f"\n  Different models (N={N_CLUSTERS}):")
print(f"    {'Model':<22}{'Silhouette':>12}{'Calinski-Harabasz':>20}")
print(f"    {'KMeans':<22}{sil_km:>12.4f}{cal_km:>20.1f}")
print(f"    {'GaussianMixture':<22}{sil_gmm:>12.4f}{cal_gmm:>20.1f}")
print(f"    {'BayesianGMM':<22}{sil_bgm:>12.4f}{cal_bgm:>20.1f}")
print(f"    {'Agglomerative(ward)':<22}{sil_agg:>12.4f}{cal_agg:>20.1f}")


exportable = {
    'kmeans':      (kmeans_final, kmeans_labels, sil_km),
    'gmm':         (gmm,          gmm_labels,    sil_gmm),
    'bayesian_gmm':(bgm,          bgm_labels,    sil_bgm),
}
best_name, (best_model, best_labels, best_sil) = max(exportable.items(), key=lambda kv: kv[1][2])
print(f"\n  Best model by silhouette: '{best_name}' (silhouette={best_sil:.4f})")

df['cluster']         = best_labels
df['cluster_kmeans']  = kmeans_labels 

has_proba = hasattr(best_model, 'predict_proba')
if has_proba:
    df['cluster_prob_max'] = best_model.predict_proba(X_sc).max(axis=1)


profile = df.groupby('cluster')[features_list].mean()
profile['n_particles'] = df['cluster'].value_counts().sort_index()
print(f"\n  Different clusters (average values, model='{best_name}'):")
print(profile.to_string(float_format=lambda x: f"{x:.4f}"))


pca = PCA(n_components=2, random_state=42)
X_pca = pca.fit_transform(X_sc)

with PdfPages("Plots/UnsupervisedClustering.pdf") as pdf:


    fig, ax = plt.subplots(figsize=(7, 6))
    scatter = ax.scatter(X_pca[:, 0], X_pca[:, 1], c=best_labels, cmap='tab10', s=5, alpha=0.5)
    ax.set_xlabel(f"PC1 ({pca.explained_variance_ratio_[0]*100:.1f}%) variance")
    ax.set_ylabel(f"PC2 ({pca.explained_variance_ratio_[1]*100:.1f}% variance)")
    ax.set_title(f"PCA projection - clusters '{best_name}' (N={N_CLUSTERS})")
    legend1 = ax.legend(*scatter.legend_elements(), title="Cluster")
    ax.add_artist(legend1)
    pdf.savefig(fig)
    plt.close(fig)


    for feat in features_list:
        fig, ax = plt.subplots(figsize=(7, 5))
        for c in range(N_CLUSTERS):
            sns.kdeplot(df.loc[df['cluster'] == c, feat], label=f"Cluster {c}", ax=ax, fill=True, alpha=0.3)
        ax.set_xlabel(feat)
        ax.set_ylabel("Density")
        ax.set_title(f"Distribution of {feat} in clusters ('{best_name}')")
        ax.legend()
        pdf.savefig(fig)
        plt.close(fig)


    fig, ax = plt.subplots(figsize=(6, 5))
    crosstab = pd.crosstab(df['cluster'], df['cluster_kmeans'])
    sns.heatmap(crosstab, annot=True, fmt='d', cmap='Blues', ax=ax)
    ax.set_xlabel("KMeans cluster")
    ax.set_ylabel(f"Cluster '{best_name}'")
    ax.set_title(f"Agreement '{best_name}' vs KMeans")
    pdf.savefig(fig)
    plt.close(fig)

print("\n  Saved plots to UnsupervisedClustering.pdf")


cluster_pipeline = Pipeline(steps=[
    ('scaler', scaler),
    ('model',  best_model),
])

n_features = X.shape[1]
convert_options = {}
if has_proba and isinstance(best_model, GaussianMixture):
    convert_options = {id(best_model): {'zipmap': False}}

onnx_model = convert_sklearn(
    cluster_pipeline,
    initial_types=[('input', FloatTensorType([None, n_features]))],
    options=convert_options,
    target_opset=17,
)
onnx_model.ir_version = 9

onnx_path = "../ONNX/UnsupervisedClusterModel.onnx"
with open(onnx_path, "wb") as f_out:
    f_out.write(onnx_model.SerializeToString())

print(f"  ONNX model ('{best_name}', scaler+model) saved: {onnx_path}")
if has_proba:
    print("    Model outputs: [hard label, probabilities for each cluster]")
else:
    print("    Model outputs: [hard label]")