import uproot
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split,RandomizedSearchCV
from sklearn.metrics import roc_auc_score, classification_report
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType

# ========================
# 1. Import i przygotowanie danych
# ========================
with uproot.open("Data/MLDataMCElectronFullCalo.root") as f:
    df_Electron = f["MLDataTree"].arrays(library="pd")
with uproot.open("Data/MLDataMCMuonFullCalo.root") as f:
    df_Muon = f["MLDataTree"].arrays(library="pd")

df = pd.concat([df_Electron, df_Muon], ignore_index=True)

features_list = [
    'track_PixelHits', 'track_TRTHits', 'track_SCTHits',
    'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius', 'HasCalo'
]

X = df[features_list]
y = df['IsMuon']

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.20, random_state=42, stratify=y
)
train_size_sample = min(50000, len(X_train))
X_train_small = X_train.sample(n=train_size_sample, random_state=42)
y_train_small = y_train.loc[X_train_small.index]
# ========================
# 2. Grid Search - Optymalizacja
# ========================
print("Rozpoczynam optymalizację parametrów (Grid Search)...")

param_distributions = {
    'n_estimators': [100, 150, 200, 250,1000],  # Mniej drzew na start szybciej się liczy
    'max_depth': [10,12,13,14,11,9],
    'min_samples_split': [5,10, 20],
    'max_features': ['sqrt']
}

rf_base = RandomForestClassifier(random_state=42, n_jobs=1, class_weight='balanced')

# cv=3 dla szybkości, scoring='roc_auc' dla najlepszej separacji klas
random_search = RandomizedSearchCV(
    estimator=rf_base, 
    param_distributions=param_distributions, 
    n_iter=8, 
    scoring='roc_auc', 
    cv=2,             # CV=2 jest DUŻO szybsze niż 3
    verbose=1, 
    random_state=42,
    n_jobs=-1
)

random_search.fit(X_train_small, y_train_small)

print(f"\nNajlepsze parametry: {random_search.best_params_}")
print("Trenuję finalny model na PEŁNYM zbiorze treningowym...")



