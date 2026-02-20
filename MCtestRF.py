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
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType
import seaborn as sns
import os


os.makedirs("Plots", exist_ok=True)

#========================
# Importing training data
#========================
print("Importing training data...")
with uproot.open("Data/MLFinalDataTrueData.root") as f:
    df_train = f["MLDataTree"].arrays(library="pd")

    
#========================
# Importing testing data
#========================
print("Importing testing data...")
with uproot.open("Data/MLDataMCElectron.root") as fe:
    df_Electron = fe["MLDataTree"].arrays(library="pd")
    
with uproot.open("Data/MLDataMCMuon.root") as fm:
    df_Muon = fm["MLDataTree"].arrays(library="pd")
df_test = pd.concat([df_Electron, df_Muon], ignore_index=True)
    
features_list=['track_PixelHits', 'track_TRTHits', 'track_SCTHits',
                        'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
                        'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius']

X_train_full = df_train[features_list]
y_train_full = df_train['IsMuon']

scaler = StandardScaler()
X_train_full_scaled = scaler.fit_transform(X_train_full)

print("Traing model on data...")
rf = RandomForestClassifier(n_estimators=200, max_depth=10, min_samples_split=10, random_state=42, n_jobs=-1,class_weight='balanced')
rf.fit(X_train_full_scaled, y_train_full)

#========================
# Exporting model and parameters
#========================
params = np.concatenate([scaler.mean_, scaler.scale_])
np.savetxt("Data/scaler.txt", params.reshape(1, -1), fmt='%.8f')

initial_type = [('float_input', FloatTensorType([None, 9]))]
onnx_model = convert_sklearn(rf, initial_types=initial_type, target_opset=17, options={type(rf): {"zipmap": False}})
onnx_model.ir_version = 9
with open("Data/RFMuonElectron.onnx", "wb") as fo:
    fo.write(onnx_model.SerializeToString())




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


with PdfPages("Plots/doubleTrainSimulation-testMC.pdf") as pdf:
    
    for eta in range(4):
        eta_name = eta_values[eta]
        print(f"\n=== Testing for EtaRange = {eta_name} (MC test set) ===")
        
        if eta == 0:
            df_test_eta = df_test.copy()
        else:
            df_test_eta = df_test[df_test['EtaRange'] == eta].copy()
        
        print(f"Test leptons (MC): {len(df_test_eta)}")
        
        X_test = df_test_eta[features_list]
        X_test_scaled = scaler.transform(X_test)
        y_test = df_test_eta['IsMuon']
        
        y_pred = rf.predict(X_test_scaled)
        y_proba = rf.predict_proba(X_test_scaled)
        
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
        cm = confusion_matrix(y_test, y_pred, normalize='true')
        sns.heatmap(
            cm, annot=True, fmt='.2%', ax=axes[0, 0], cmap='Blues',
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
            alpha=0.4, color='orange', label='Muon (MC)', hatch='//')
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
