import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import (
    accuracy_score,
    classification_report,
    confusion_matrix,
    roc_curve,
    auc
)
import seaborn as sns

#========================
# Importing data
#========================
#with uproot.open("Data/MLFinalDataTrueData.root") as f:
 #  df = f["MLDataTree"].arrays(library="pd")

with uproot.open("Data/MLDataMCElectronFullCalo.root") as f:
    df_Electron = f["MLDataTree"].arrays(library="pd")
with uproot.open("Data/MLDataMCMuonFullCalo.root") as f:
    df_Muon = f["MLDataTree"].arrays(library="pd")
df = pd.concat([df_Electron, df_Muon], ignore_index=True)

eta_values = {
    0: 'FullRange',
    1: '<1.0',
    2: '1.0-1.5',
    3: '1.5-2.5'
}
                        
features_list = [
    'track_PixelHits', 'track_TRTHits', 'track_SCTHits',
    'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius', 'HasCalo'
]

# ========================
# Splitting dataset to train and test 
# ========================
df_train, df_test = train_test_split(
    df, 
    test_size=0.20, 
    random_state=42, 
    stratify=df['IsMuon']
)

X_train = df_train[features_list]
y_train = df_train['IsMuon']

# ========================
# Scaling and training
# ========================


print("Traing model on data...")
rf = RandomForestClassifier(n_estimators=200, max_depth=10, min_samples_split=10, random_state=42, n_jobs=-1, class_weight='balanced')
rf.fit(X_train, y_train)

#========================
# Exporting model and parameters
#========================


initial_type = [('float_input', FloatTensorType([None, 10]))]
onnx_model = convert_sklearn(rf, initial_types=initial_type, target_opset=17, options={type(rf): {"zipmap": False}})
onnx_model.ir_version = 9
with open("Data/RFMuonElectron.onnx", "wb") as fo:
    fo.write(onnx_model.SerializeToString())

# ========================
# Generating outputs
# ========================

reports = {}
with PdfPages("Plots/MCRFCalo.pdf") as pdf:
    
    for eta in range(4):
        eta_name = eta_values[eta]
        print(f"\n=== Testing for EtaRange = {eta_name} ===")
        
        # Filtrowanie ze zbioru testowego (model nigdy go nie widział)
        if eta == 0:
            df_eta = df_test.copy()
        else:
            df_eta = df_test[df_test['EtaRange'] == eta].copy()
        
        if len(df_eta) == 0:
            continue

        print(f"Number of Leptons: {len(df_eta)}")
        

        X_test = df_eta[features_list]
        y_test = df_eta['IsMuon']
        
        
        y_pred = rf.predict(X_test)
        y_proba = rf.predict_proba(X_test)
        acc = accuracy_score(y_test, y_pred) 
        print(f"Accuracy Test:    {acc:.4f}")

        label_names = ["Electron", "Muon"]
        report_title = f"EtaRange = {eta_name}"
        reports[report_title] = classification_report(
            y_test, y_pred, target_names=label_names
        )
        
        fig, axes = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle(
            f'EtaRange = {eta_name} (TEST DATA) - Accuracy: {acc:.3f}',
            fontsize=16,
            fontweight='bold'
        )
        
        # 1. Confusion Matrix
        cm = confusion_matrix(y_test, y_pred, normalize='true')
        sns.heatmap(cm, annot=True, fmt='.2%', ax=axes[0, 0], cmap='Blues',
                    xticklabels=label_names, yticklabels=label_names)
        axes[0, 0].set_title('Confusion Matrix')
        
        # 2. ROC Curve
        if len(np.unique(y_test)) == 2:
            fpr, tpr, _ = roc_curve(y_test, y_proba[:, 1])
            auc_score = auc(fpr, tpr)
            axes[0, 1].plot(fpr, tpr, label=f'ROC (AUC = {auc_score:.3f})', linewidth=2)
            axes[0, 1].plot([0, 1], [0, 1], 'k--')
            axes[0, 1].set_title('ROC Curve')
            axes[0, 1].legend()
        
        # 3. Score distribution
        scores = y_proba[:, 1]
        axes[1, 0].hist(scores[y_test == 0], bins=25, density=True, alpha=0.6, label='Electron', color='blue')
        axes[1, 0].hist(scores[y_test == 1], bins=25, density=True, alpha=0.4, label='Muon', color='orange', hatch='//')
        axes[1, 0].set_title('Classifier Response')
        axes[1, 0].legend(loc='upper center')
        
        # 4. Feature Importance
        importances = rf.feature_importances_
        axes[1, 1].bar(features_list, importances)
        axes[1, 1].set_title('Feature Importance')
        axes[1, 1].tick_params(axis='x', rotation=45)
        
        plt.tight_layout()
        pdf.savefig(fig, bbox_inches='tight')
        plt.close(fig)

    # Last page
    
    fig2=plt.figure(figsize=(10,8))
    sns.heatmap(df[features_list + ['IsMuon']].corr(), annot=True, cmap='coolwarm')
    pdf.savefig(fig2, bbox_inches='tight')
    
    fig = plt.figure(figsize=(11, 14))
    fig.text(
        0.35, 0.98,
        "Classification Reports Summary\nTrained:  MCData.root | Test: MCData.root",
        ha='center', fontsize=18, fontweight='bold'
    )
    y_pos = 0.90
    for title, report in reports.items():
        fig.text(0.05, y_pos, f"\n{title}", fontsize=12, fontweight='bold', family='monospace')
        y_pos -= 0.03
        for line in report.split('\n'):
            if line.strip():
                fig.text(0.05, y_pos, line, fontsize=9, family='monospace')
                y_pos -= 0.015
        y_pos -= 0.03
    
    pdf.savefig(fig, bbox_inches='tight')
    plt.close(fig)

print("\n" + "="*70)
print(f"End. Testing data: {len(df_test)} leptons.")
print("PDF: Plots/MCRF.pdf")
print("="*70)

