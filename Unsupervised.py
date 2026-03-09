import pandas as pd
import uproot
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from sklearn.metrics import confusion_matrix, roc_curve, auc


# 1. Lista cech (features)
features_list = [
    'track_PixelHits', 'track_TRTHits', 'track_SCTHits',
    'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius'
]

# 2. Wczytywanie danych
# Dane do "nauki" (Unsupervised - bez zaglądania do etykiet)
with uproot.open("Data/MLFinalDataTrueData.root") as f:
    df = f["MLDataTree"].arrays(library="pd")

# Dane z etykietami do weryfikacji (Testowe)
with uproot.open("Data/MLDataMCElectronFull.root") as f:
    df_electron = f["MLDataTree"].arrays(library="pd")
with uproot.open("Data/MLDataMCMuonFull.root") as f:
    df_muon = f["MLDataTree"].arrays(library="pd")

# Łączymy dane testowe w jeden zbiór
df_test = pd.concat([df_electron, df_muon], ignore_index=True)

# 3. CZYSZCZENIE: Usuwamy wiersze z NaN (Kluczowe dla stabilności)
df = df.dropna(subset=features_list).copy()
df_test = df_test.dropna(subset=features_list).copy()

# 4. SKALOWANIE
scaler = StandardScaler()
# Uczymy skalera na danych treningowych
X_train_scaled = scaler.fit_transform(df[features_list])
# Dane testowe skalujemy TYMI SAMYMI parametrami (używamy .transform)
X_test_scaled = scaler.transform(df_test[features_list])

# 5. KLASTRYZACJA (K-Means)
kmeans = KMeans(n_clusters=2, random_state=42, n_init=10)
# Trenujemy na danych głównych
clusters_train = kmeans.fit_predict(X_train_scaled)
# Przypisujemy klastry do danych testowych (bez douczania modelu)
clusters_test = kmeans.predict(X_test_scaled)

# 6. AUTOMATYCZNE NAZEWNICTWO (Interpretacja fizyczna)
# Sprawdzamy średnią Cal_EMprop - elektrony mają ją zazwyczaj wyższą
df['cluster'] = clusters_train
mean_em = df.groupby('cluster')['Cal_EMprop'].mean()

electron_cluster_id = mean_em.idxmax()
muon_cluster_id = mean_em.idxmin()

mapping = {electron_cluster_id: 'Electron', muon_cluster_id: 'Muon'}

# 7. WERYFIKACJA NA DANYCH TESTOWYCH
df_test['predicted_cluster'] = clusters_test
df_test['predicted_name'] = df_test['predicted_cluster'].map(mapping)

# Tabela porównawcza (Confusion Matrix)
# IsMuon == True to Muon, IsMuon == False to Electron
comparison = pd.crosstab(df_test['predicted_name'], df_test['IsMuon'], 
                         rownames=['Predykcja'], colnames=['Prawdziwy Mion'])

print("--- Wyniki klastryzacji vs Prawda (IsMuon) ---")
print(comparison)

# 8. PCA - Wizualizacja wyników
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_train_scaled)

plt.figure(figsize=(10, 7))
scatter = plt.scatter(X_pca[:, 0], X_pca[:, 1], c=clusters_train, cmap='viridis', alpha=0.4)
plt.colorbar(scatter, label='Cluster ID')
plt.title('Wizualizacja PCA - Podział na klastry (Unsupervised)')
plt.xlabel('Główna składowa 1')
plt.ylabel('Główna składowa 2')
plt.grid(True, linestyle='--', alpha=0.6)
plt.savefig("Plots/Final_Unsupervised_PCA.png")
print("\nWykres został zapisany w Plots/Final_Unsupervised_PCA.png")

# 9. Statystyki fizyczne klastrów
print("\n--- Średnie wartości cech dla rozpoznanych cząstek ---")
analysis = df_test.groupby('predicted_name')[features_list].mean()
print(analysis)
# --- 1. Macierz Pomyłek (Heatmap) ---
plt.figure(figsize=(8, 6))
sns.heatmap(comparison, annot=True, fmt='d', cmap='Blues')
plt.title('Macierz Pomyłek: K-Means vs Prawda (IsMuon)')
plt.ylabel('Predykcja Algorytmu')
plt.xlabel('Prawda (IsMuon)')
plt.savefig("Plots/Confusion_Matrix.png")
plt.show()

# --- 2. Krzywa ROC ---
# Aby narysować ROC dla K-Means, musimy obliczyć odległość każdego punktu od centroidów.
# Im bliżej centroidu "Electron", tym mniejsza szansa, że to mion.
distances = kmeans.transform(X_test_scaled) 
# Wybieramy odległość od klastra mionowego jako nasz "score"
# (odległość od klastra elektronowego / suma odległości) daje nam przybliżone prawdopodobieństwo
muon_scores = distances[:, electron_cluster_id] / distances.sum(axis=1)

fpr, tpr, thresholds = roc_curve(df_test['IsMuon'], muon_scores)
roc_auc = auc(fpr, tpr)



plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, color='darkorange', lw=2, label=f'Krzywa ROC (area = {roc_auc:.2f})')
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate (Typ II błąd)')
plt.ylabel('True Positive Rate (Czułość)')
plt.title('Krzywa ROC dla klastryzacji Unsupervised')
plt.legend(loc="lower right")
plt.grid(alpha=0.3)
plt.savefig("Plots/ROC_Curve.png")
plt.show()

print(f"Pole pod krzywą (AUC): {roc_auc:.4f}")