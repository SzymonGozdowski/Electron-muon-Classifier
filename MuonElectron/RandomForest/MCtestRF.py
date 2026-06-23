import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import precision_recall_curve, auc, average_precision_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import (
    accuracy_score,
    classification_report,
    confusion_matrix,
    roc_curve,
    auc
)

import seaborn as sns
import os


os.makedirs("Plots", exist_ok=True)

#========================
# Importing training data
#========================
# Ustawia bazowy rozmiar czcionki na 14 (domyślnie jest ok. 10)
plt.rcParams.update({'font.size': 14})

print("Importing training data...")
with uproot.open("Data/MLDataTrueData.root") as f:
    df_test = f["MLDataTree"].arrays(library="pd")

    
#========================
# Importing testing data
#========================
print("Importing testing data...")
with uproot.open("Data/MLDataMCElectronFull.root") as fe:
    df_Electron = fe["MLDataTree"].arrays(library="pd")
    
with uproot.open("Data/MLDataMCMuonFull.root") as fm:
    df_Muon = fm["MLDataTree"].arrays(library="pd")
df_train = pd.concat([df_Electron, df_Muon], ignore_index=True)
    
features_list=['track_PixelHits', 'track_TRTHits', 'track_SCTHits',
                        'track_PixeldEdX','Cal_EMprop','Cal_FVariable',
                        'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius' ]

X_train_full = df_train[features_list]
y_train_full = df_train['IsMuon']



print("Traing model on data...")
rf = RandomForestClassifier(n_estimators=200, max_depth=10, min_samples_split=10, random_state=42, n_jobs=-1,class_weight='balanced')
rf.fit(X_train_full, y_train_full)






# ========================
# Generating outputs
# ========================
eta_values = {
    0: 'FullRange',
    1: '<1.0',
    2: '1.0-1.5',
    3: '1.5-2.5'
}
reports = {}


with PdfPages("Plots/RF_test_output.pdf") as pdf:
    
    for eta in range(4):
        eta_name = eta_values[eta]
        print(f"\n=== Testing for EtaRange = {eta_name} (MC test set) ===")
        
        if eta == 0:
            df_test_eta = df_test.copy()
        else:
            df_test_eta = df_test[df_test['EtaRange'] == eta].copy()
        
        print(f"Test leptons (MC): {len(df_test_eta)}")
        
        X_test = df_test_eta[features_list]
        y_test = df_test_eta['IsMuon']
        
        y_pred = rf.predict(X_test)
        y_proba = rf.predict_proba(X_test)
        
        acc = accuracy_score(y_test, y_pred)
        print(f"Accuracy on MC Sample: {acc:.3f}")
        
        label_names = ["Electron", "Muon"]
        report_title = f"EtaRange = {eta_name}"
        reports[report_title] = classification_report(
            y_test, y_pred, target_names=label_names
        )
        
        fig, axes = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle(
            f'EtaRange = {eta_name} - Accuracy on MC: {acc:.3f}\n(trained on full TrueData)',
            fontsize=16,
            fontweight='bold'
        )
        
        # 1. Confusion Matrix
        cm = confusion_matrix(y_test, y_pred)
        sns.heatmap(
            cm, annot=True, fmt='.0f', ax=axes[0, 0], cmap='Blues',
            xticklabels=label_names, yticklabels=label_names
        )
        axes[0, 0].set_title('Confusion Matrix')
        axes[0, 0].set_ylabel('True Label')
        axes[0, 0].set_xlabel('Predicted Label')
        
        # 2. ROC Curve
        if len(np.unique(y_test)) == 2:
            fpr, tpr, _ = roc_curve(y_test, y_proba[:, 1])
            auc_score = auc(fpr, tpr)
            axes[0, 1].plot(
                fpr, tpr, label=f'ROC (AUC = {auc_score:.3f})', linewidth=2
            )
            axes[0, 1].plot([0, 1], [0, 1], 'k--', label='Random')
            axes[0, 1].set_xlabel('False Positive Rate')
            axes[0, 1].set_ylabel('True Positive Rate')
            axes[0, 1].set_title('ROC Curve')
            axes[0, 1].legend()
            axes[0, 1].grid(True, alpha=0.3)

        # 3. RF response (Score distribution)
        scores = y_proba[:, 1]
        signal_scores = scores[y_test == 1]
        bkg_scores = scores[y_test == 0]
        bins = np.linspace(0, 1, 25)
        
        axes[1, 0].hist(
            bkg_scores, bins=bins, density=True, histtype='stepfilled',
            alpha=0.6, color='blue', label='Electron (MC)'
        )
        axes[1, 0].hist(
            signal_scores, bins=bins, density=True, histtype='stepfilled',
            alpha=0.4, color='red', label='Muon (MC)', hatch='//')
        axes[1, 0].set_xlabel('RF response')
        axes[1, 0].set_ylabel('(1/N) dN/dx')
        axes[1, 0].set_title('Classifier Response (MC)')
        axes[1, 0].legend(loc='upper center')
        axes[1, 0].grid(True, alpha=0.3)
        
        # 4. Feature Importance 
        importances = rf.feature_importances_
        features = ['PixelHits', 'TRTHits', 'SCTHits', 'PixeldEdX',
                    'Cal_FVar', 'Cal_EMprop', 'Cal_Lambda',
                    'Cal_Lambda2', 'Cal_Radius']
        axes[1, 1].bar(features, importances)
        axes[1, 1].set_title('Feature Importance (full TrueData)')
        axes[1, 1].tick_params(axis='x', rotation=45)
        axes[1, 1].grid(True, alpha=0.3, axis='y')
        
        
        plt.tight_layout()
        pdf.savefig(fig, bbox_inches='tight')
        plt.close(fig)
        
        print("\nReport (MC Electron/Muon):")
        print(reports[report_title])
    
    # Last page
    fig = plt.figure(figsize=(11, 14))
    fig.text(
        0.35, 0.98,
        "Classification Reports Summary\nTrained: Double TrueData.root | Test: MC Samples",
        ha='center', fontsize=18, fontweight='bold'
    )
    
    y_position = 0.95
    for title, report in reports.items():
        fig.text(
            0.05, y_position, f"\n{title}",
            fontsize=12, fontweight='bold', family='monospace'
        )
        y_position -= 0.04
        
        report_lines = report.split('\n')
        for line in report_lines:
            if line.strip():
                fig.text(
                    0.05, y_position, line,
                    fontsize=9, family='monospace'
                )
                y_position -= 0.015
        
        y_position -= 0.03
    
    pdf.savefig(fig, bbox_inches='tight')
    plt.close(fig)


print("\n" + "="*70)
print("PDF : Plots/RF_test_output.pdf")
print("Model trained on: MLFinalDataTrueData.root")
print("Tested on: MLDataMCElectron.root + MLDataMCMuon.root")
print("="*70)


# ========================================================
# EKSPORT OSOBNYCH PLIKÓW PNG (Dla FullRange / eta=0)
# ========================================================
print("\nGenerowanie osobnych plików PNG dla FullRange...")

# Przygotowanie danych dla całego zakresu
X_full_test = df_test[features_list]
y_full_test = df_test['IsMuon']
y_full_proba = rf.predict_proba(X_full_test)[:, 1]

# 1. Wykres ROC Curve
plt.figure(figsize=(8, 6))
fpr, tpr, _ = roc_curve(y_full_test, y_full_proba)
auc_score = auc(fpr, tpr)
plt.plot(fpr, tpr, color='darkorange', lw=3, label=f'ROC curve (area = {auc_score:.4f})')
plt.plot([0, 1], [0, 1], color='navy', lw=3, linestyle='--')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('ROC Curve - tagged')
plt.legend(loc="lower right")
plt.grid(alpha=0.3)
plt.savefig("Plots/ROC_FullRange_tagged.png", dpi=300)
plt.close()

# 2. Wykres Response (Score Distribution)
plt.figure(figsize=(8, 6))
plt.hist(y_full_proba[y_full_test == 0], bins=50, density=True, alpha=0.6, label='Electron', color='blue')
plt.hist(y_full_proba[y_full_test == 1], bins=50, density=True, alpha=0.4, label='Muon', color='red', hatch='//')
plt.xlabel('Classifier Score (Probability of being Muon)')
plt.ylabel('Normalized counts')
plt.title('Classifier Response - tagged')
plt.legend(loc='upper center')
plt.grid(alpha=0.3)
plt.savefig("Plots/Response_FullRange_tagged.png", dpi=300)
plt.close()

# 3. Wykres Feature Importance (Parametry)
plt.figure(figsize=(10, 6))
importances = rf.feature_importances_
indices = np.argsort(importances)
plt.title('Feature Importances - Random Forest - tagged')
plt.barh(range(len(indices)), importances[indices], color='skyblue', align='center')
plt.yticks(range(len(indices)), [features_list[i] for i in indices])
plt.xlabel('Relative Importance')
plt.tight_layout()
plt.savefig("Plots/Features_Importance_tagged.png", dpi=300)
plt.close()

print("Pliki PNG zostały zapisane w folderze Plots/.")