import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.patches import Patch
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import (classification_report, confusion_matrix,
                             roc_curve, auc, precision_recall_curve,
                             average_precision_score)
from xgboost import XGBClassifier
import seaborn as sns
import os
import shap

from sklearn.base import BaseEstimator, TransformerMixin
from skl2onnx import update_registered_converter
from skl2onnx.common.data_types import FloatTensorType
from skl2onnx.common._apply_operation import (
    apply_sub, apply_div, apply_mul, apply_add, apply_sqrt
)

from onnxmltools import convert_xgboost 
from onnxmltools.convert.common.data_types import FloatTensorType as OnnxFloat 
#========================
# Importing data
#========================

plt.rcParams.update({'font.size': 14})

with uproot.open("../Data/MLDataBackground_Jpsi_EE.root") as f:
    df_Background1 = f["MLDataTree"].arrays(library="pd")
with uproot.open("../Data/MLDataBackground_yy_EE.root") as f:
    df_Background2 = f["MLDataTree"].arrays(library="pd")
with uproot.open("../Data/MLDataBackground_Jpsi_MuMu.root") as f:
    df_Background3 = f["MLDataTree"].arrays(library="pd")
with uproot.open("../Data/MLDataBackground_yy_MuMu.root") as f:
    df_Background4 = f["MLDataTree"].arrays(library="pd")
with uproot.open("../Data/MLDataMCProton.root") as f:
    df_Proton = f["MLDataTree"].arrays(library="pd")
    
# ========================
# Weights of Proton samples
# ========================

n_bkg = (
    len(df_Background1)
    + len(df_Background2)
    + len(df_Background3)
    + len(df_Background4)
)

n_proton = len(df_Proton)

ratio = n_bkg / n_proton if n_proton > 0 else 1.0
data_ratio =30
print(f"\nWeights: background/proton = {ratio:.2f}/{data_ratio:.2f}")

df_Background1["weight"] = 1.0
df_Background2["weight"] = 1.0
df_Background3["weight"] = 1.0
df_Background4["weight"] = 1.0

df_Proton["weight"] = ratio/data_ratio

df = pd.concat([df_Background1, df_Background2, df_Background3, df_Background4, df_Proton], ignore_index=True)

eta_values = {
    0: 'FullRange',
    1: '<1.0',
    2: '1.0-1.5',
    3: '1.5-2.5'
}
                        
features_list = [
    'track_PixelHits', 'track_TRTHits', 'track_SCTHits',
    'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius'
]

# ========================
# Splitting dataset to train and test 
# ========================
df_train, df_test = train_test_split(
    df, 
    test_size=0.20, 
    random_state=42, 
    stratify=df['IsProton']
)

X_train = df_train[features_list]
y_train = df_train['IsProton']
X_test  = df_test[features_list]
y_test  = df_test['IsProton']

n_sig_train = int(y_train.sum())
n_bkg_train = int((y_train == 0).sum())
n_sig_test  = int(y_test.sum())
n_bkg_test  = int((y_test == 0).sum())

print(f"\n{'='*55}")
print(f"  TRENING : Sygnał={n_sig_train:>5}  Tło={n_bkg_train:>5}  "
      f"S/B = 1:{n_bkg_train/n_sig_train:.1f}")
print(f"  TEST    : Sygnał={n_sig_test:>5}  Tło={n_bkg_test:>5}  "
      f"S/B = 1:{n_bkg_test/n_sig_test:.1f}  ")
print(f"{'='*55}")

scaler     = StandardScaler()
X_train_sc = scaler.fit_transform(X_train)
X_test_sc  = scaler.transform(X_test)
means = scaler.mean_
scales = scaler.scale_

with open("../ONNX/scalars.txt", "w") as f:
    f.write(",".join([f"{x:.8f}" for x in means]) + "\n")
    f.write(",".join([f"{x:.8f}" for x in scales]) + "\n")

# =============================================================================
# 5. XGBOOST – TRENING
# =============================================================================
scale_pos_weight = n_bkg_train / n_sig_train

xgb = XGBClassifier(
    n_estimators          = 600,
    learning_rate         = 0.05,
    max_depth             = 3,
    min_child_weight      = 10,
    subsample             = 0.6,
    colsample_bytree      = 0.6,
    gamma                 = 2.0,
    reg_alpha             = 0.1,
    reg_lambda            = 1.0,
    scale_pos_weight      = scale_pos_weight,
    eval_metric           = 'auc',
    early_stopping_rounds = 30,
    tree_method           = 'hist',
    random_state          = 42,
    n_jobs                = -1,
    verbosity             = 1,
)


xgb.fit(
    X_train_sc, y_train,
    sample_weight=df_train["weight"],
    eval_set=[(X_train_sc, y_train), (X_test_sc, y_test)],
    verbose=50
)

y_pred  = xgb.predict(X_test_sc)
y_probs = xgb.predict_proba(X_test_sc)[:, 1]

best_iteration = xgb.best_iteration
print(f"\nNajlepszy iteration (early stopping): {best_iteration}")

n_features = X_train_sc.shape[1]
onnx_model = convert_xgboost(
    xgb,
    initial_types=[('input', OnnxFloat([None, n_features]))]
)
onnx_path = "../ONNX/XgbProtonBackground.onnx"
with open(onnx_path, "wb") as f:
    f.write(onnx_model.SerializeToString())
print(f"Model ONNX zapisany: {onnx_path}")

# =============================================================================
# 6. Precision and Recall, Confusion Matrix
# =============================================================================
fpr_arr, tpr_arr, roc_thresholds = roc_curve(y_test, y_probs)
roc_auc = auc(fpr_arr, tpr_arr)

prec_arr, rec_arr, pr_thresholds = precision_recall_curve(y_test, y_probs)
f1_arr         = 2 * prec_arr * rec_arr / (prec_arr + rec_arr + 1e-9)
best_f1_idx    = np.argmax(f1_arr[:-1])
best_f1_thresh = pr_thresholds[best_f1_idx]
ap             = average_precision_score(y_test, y_probs)

idx_95        = np.argmin(np.abs(fpr_arr - 0.05))
thresh_95bkg  = roc_thresholds[idx_95]
sig_eff_at_95 = tpr_arr[idx_95]

idx_99        = np.argmin(np.abs(fpr_arr - 0.01))
thresh_99bkg  = roc_thresholds[idx_99]
sig_eff_at_99 = tpr_arr[idx_99]

print(f"\nROC AUC             : {roc_auc:.4f}")
print(f"Average Precision   : {ap:.4f}")
print(f"Próg max-F1         : {best_f1_thresh:.3f}  (F1={f1_arr[best_f1_idx]:.3f})")
print(f"Próg 95% rej. tła   : {thresh_95bkg:.3f}  (sig. eff.={sig_eff_at_95:.3f})")
print(f"Próg 99% rej. tła   : {thresh_99bkg:.3f}  (sig. eff.={sig_eff_at_99:.3f})")

evals          = xgb.evals_result()
train_auc_hist = evals['validation_0']['auc']
val_auc_hist   = evals['validation_1']['auc']


# =============================================================================
# 8. PDF — PLOTS 
# =============================================================================
plt.rcParams.update({
    'figure.facecolor': 'white',
    'axes.facecolor':   'white',
    'font.family':      'sans-serif',
    'axes.spines.top':  False,
    'axes.spines.right':False,
})

SIG_COLOR = '#1f77b4'
BKG_COLOR = "#ff0e0e"
ACC_COLOR = '#2ca02c'
PUR_COLOR = '#9467bd'

with PdfPages("Plots/XGB_Output.pdf") as pdf:
    # =============================================================================
    # PAGE 1 — METRICS, CURVES, CONFUSION MATRIX 
    # =============================================================================
    fig = plt.figure(figsize=(18, 14))
    fig.suptitle('Proton Identification — XGBoost Analysis\n'
                  f'[Train S/B=1:{n_bkg_train/n_sig_train:.0f}  |  '
                  f'Test S/B=1:{n_bkg_test/n_sig_test:.0f} (realistic, all files)]',
                  fontsize=16, y=0.98, fontweight='bold')
    gs = gridspec.GridSpec(2, 3, figure=fig, hspace=0.4, wspace=0.35)

    ax_cm = fig.add_subplot(gs[0, 0])
    y_pred_f1 = (y_probs >=0.86).astype(int)
    cm_mat = confusion_matrix(y_test, y_pred_f1)
    cm_norm = cm_mat.astype(float) / cm_mat.sum(axis=1)[:, np.newaxis]

    sns.heatmap(cm_norm, annot=True, fmt='.2%', ax=ax_cm,
                cmap='Blues', linewidths=0.5,
                annot_kws={"size": 16}, 
                xticklabels=['Background', 'Proton'],
                yticklabels=['Background', 'Proton'],
                cbar_kws={'label': 'Fraction'})

    ax_cm.set_title(f'Confusion Matrix\n@ Max F1-Score (thr={0.86})',
                    color=SIG_COLOR, pad=10, fontsize=16)
    ax_cm.set_xlabel('Predicted', fontsize=16)
    ax_cm.set_ylabel('True', fontsize=16)

    ax_cm.tick_params(axis='both', labelsize=16)

    for i in range(2):
        for j in range(2):
            ax_cm.text(j+0.5, i+0.72, f'({cm_mat[i,j]})',
                    ha='center', va='center', fontsize=16)

    ax_roc = fig.add_subplot(gs[0, 1])

    ax_roc.plot(fpr_arr, tpr_arr, color=SIG_COLOR, lw=2,
                label=f'AUC = {roc_auc:.4f}')
    ax_roc.fill_between(fpr_arr, tpr_arr, alpha=0.1, color=SIG_COLOR)
    ax_roc.plot([0, 1], [0, 1], linestyle='--', lw=1)

    ax_roc.set_title('ROC Curve', color=SIG_COLOR, pad=10, fontsize=16)
    ax_roc.set_xlabel('False Positive Rate', fontsize=16)
    ax_roc.set_ylabel('True Positive Rate', fontsize=16)

    ax_roc.legend(fontsize=16)

    ax_roc.tick_params(axis='both', labelsize=16)

    ax_roc.grid(True)

    ax_pr = fig.add_subplot(gs[0, 2])
    ax_pr.plot(rec_arr, prec_arr, color=BKG_COLOR, lw=2,
               label=f'AP = {ap:.4f}')
    ax_pr.fill_between(rec_arr, prec_arr, alpha=0.1, color=BKG_COLOR)
    ax_pr.scatter(rec_arr[best_f1_idx], prec_arr[best_f1_idx],
                  color=ACC_COLOR, s=80, zorder=5,
                  label=f'Best F1={f1_arr[best_f1_idx]:.3f}\n(thr={best_f1_thresh:.2f})')
    ax_pr.set_title('Precision-Recall Curve', color=SIG_COLOR, pad=10)
    ax_pr.set_xlabel('Recall (Signal Efficiency)')
    ax_pr.set_ylabel('Precision')
    ax_pr.legend(fontsize=9)
    ax_pr.grid(True)

    ax_resp = fig.add_subplot(gs[1, 0:2])
    bins = np.linspace(0, 1, 50)

    ax_resp.hist(y_probs[y_test==0], bins=bins, alpha=0.6, density=True,
                color=BKG_COLOR, label=f'Background (N={n_bkg_test})',
                hatch='//', edgecolor=BKG_COLOR)
    ax_resp.hist(y_probs[y_test==1], bins=bins, alpha=0.6, density=True,
                color=SIG_COLOR, label=f'Proton (N={n_sig_test})')



    ax_resp.set_title('Classifier Response Distribution [Test: realistic S/B]', 
                    color=SIG_COLOR, pad=10, fontsize=16)
    ax_resp.set_xlabel('P(Proton)', fontsize=16)
    ax_resp.set_ylabel('Normalized counts (log)', fontsize=16)

    ax_resp.tick_params(axis='both', labelsize=16)
    ax_resp.legend(fontsize=16)
    ax_resp.grid(True)
    ax_resp.set_yscale('log')

    ax_eff = fig.add_subplot(gs[1, 2])
    bkg_rej = 1 - fpr_arr
    ax_eff.plot(bkg_rej, tpr_arr, color=PUR_COLOR, lw=2)
    ax_eff.axvline(0.95, color=ACC_COLOR, linestyle=':', lw=1.5,
                   label=f'95% bkg rej.\nSig eff={sig_eff_at_95:.2f}')
    ax_eff.axvline(0.99, linestyle=':', lw=1.5,
                   label=f'99% bkg rej.\nSig eff={sig_eff_at_99:.2f}')
    ax_eff.set_title('Signal Eff. vs Background Rejection', color=SIG_COLOR, pad=10)
    ax_eff.set_xlabel('Background Rejection (1 - FPR)')
    ax_eff.set_ylabel('Signal Efficiency (TPR)')
    ax_eff.legend(fontsize=9)
    ax_eff.grid(True)

    pdf.savefig(fig, bbox_inches='tight')
    plt.close()
    # ============================================================
    # PAGE 2, 3 — SHAP VALUES (global importance)
    # ============================================================

    # Bierzemy próbkę testową (SHAP jest ciężki)
    sample_size = min(5000, len(X_test_sc))
    sample_idx = np.random.choice(len(X_test_sc), size=sample_size, replace=False)
    X_shap = X_test_sc[sample_idx]

    explainer = shap.TreeExplainer(xgb)
    shap_values = explainer.shap_values(X_shap)

    plt.figure(figsize=(12, 10))
    shap.summary_plot(shap_values, X_shap, feature_names=features_list, show=False)
    fig = plt.gcf()
    fig.suptitle("SHAP Summary Plot — Global Feature Impact", fontsize=16)
    pdf.savefig(fig, bbox_inches='tight')
    plt.close()

    plt.figure(figsize=(10, 8))
    shap.summary_plot(shap_values, X_shap, feature_names=features_list,
                      plot_type="bar", show=False)
    fig = plt.gcf()
    fig.suptitle("SHAP Feature Importance (mean |SHAP|)", fontsize=16)
    pdf.savefig(fig, bbox_inches='tight')
    plt.close()


  

print("="*70)
print("End")
print("PDF: Plots/XGB_Output.pdf")
print("="*70)