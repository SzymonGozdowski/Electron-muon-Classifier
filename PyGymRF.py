import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import (
    accuracy_score,
    classification_report,
    confusion_matrix,
    roc_curve,
    auc
)
import seaborn as sns

# Wczytaj dane
with uproot.open("MLDataMCElectron.root") as f:
    df = f["MLDataTree"].arrays(library="pd")

# Mapowanie eta -> etykieta/nazwa
eta_values = {
    0: 'FullRange',
    1: '<1.0',
    2: '1.0-1.5',
    3: '1.5-2.5'
}

# Słownik na przechowywanie raportów
reports = {}

# Przygotuj PDF
with PdfPages("Plots/RF_eta_output.pdf") as pdf:

    for eta in range(4):
        # Ustal nazwę eta
        eta_name = eta_values[eta]

        print(f"\n=== Analysis for EtaRange = {eta_name} ===")

        # Filtrowanie po EtaRange (dla eta==0 bierzemy całe df, jak w Twoim kodzie)
        if eta == 0:
            df_eta = df.copy()
        else:
            df_eta = df[df['EtaRange'] == eta].copy()

        print(f"Number of Leptons: {len(df_eta)}")

        # Dane wejściowe i etykiety
        X = df_eta[['track_PixelHits', 'track_TRTHits', 'track_SCTHits',
                    'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop',
                    'Cal_Lambda', 'Cal_Lambda2', 'Cal_Radius']]
        y = df_eta['IsMuon']

        # Podział na train/test
        X_train, X_test, y_train, y_test = train_test_split(
            X, y,
            test_size=0.2,
            random_state=42,
            stratify=y
        )

        # Skalowanie
        scaler = StandardScaler()
        X_train = scaler.fit_transform(X_train)
        X_test = scaler.transform(X_test)

        # Random Forest
        rf = RandomForestClassifier(
            n_estimators=200,
            max_depth=10,
            min_samples_split=10,
            random_state=42,
            n_jobs=-1
        )
        rf.fit(X_train, y_train)
        y_pred = rf.predict(X_test)

        acc = accuracy_score(y_test, y_pred)
        print(f"Accuracy: {acc:.3f}")

        # Raport klasyfikacji (tu: 0 = Electron, 1 = Muon)
        label_names = ["Electron", "Muon"]
        report_title = f"EtaRange = {eta_name}"
        reports[report_title] = classification_report(
            y_test, y_pred, target_names=label_names
        )

        # Figura 2x2 jak w drugim kodzie
        fig, axes = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle(
            f'EtaRange = {eta_name} - Accuracy: {acc:.3f}',
            fontsize=16,
            fontweight='bold'
        )

        # 1. Confusion Matrix
        cm = confusion_matrix(y_test, y_pred)
        sns.heatmap(
            cm,
            annot=True,
            fmt='d',
            ax=axes[0, 0],
            cmap='Blues',
            xticklabels=label_names,
            yticklabels=label_names
        )
        axes[0, 0].set_title('Confusion Matrix')
        axes[0, 0].set_ylabel('True Label')
        axes[0, 0].set_xlabel('Predicted Label')

        # 2. ROC Curve (jeśli binarna)
        if len(np.unique(y)) == 2:
            fpr, tpr, _ = roc_curve(y_test, rf.predict_proba(X_test)[:, 1])
            auc_score = auc(fpr, tpr)
            axes[0, 1].plot(
                fpr,
                tpr,
                label=f'ROC (AUC = {auc_score:.3f})',
                linewidth=2
            )
            axes[0, 1].plot([0, 1], [0, 1], 'k--', label='Random')
            axes[0, 1].set_xlabel('False Positive Rate')
            axes[0, 1].set_ylabel('True Positive Rate')
            axes[0, 1].set_title('ROC Curve')
            axes[0, 1].legend()
            axes[0, 1].grid(True, alpha=0.3)
        else:
            axes[0, 1].text(
                0.5, 0.5,
                "ROC not available\n(non-binary labels)",
                ha='center',
                va='center',
                fontsize=12
            )
            axes[0, 1].set_axis_off()

        # 3. RF response (Score distribution)
        scores = rf.predict_proba(X_test)[:, 1]
        signal_scores = scores[y_test == 1]
        bkg_scores = scores[y_test == 0]
        bins = np.linspace(0, 1, 25)

        axes[1, 0].hist(
            bkg_scores,
            bins=bins,
            density=True,
            histtype='stepfilled',
            alpha=0.6,
            color='blue',
            label='Electron'
        )
        axes[1, 0].hist(
            signal_scores,
            bins=bins,
            density=True,
            histtype='stepfilled',
            alpha=0.4,
            color='orange',
            label='Muon',
            hatch='//'
        )
        axes[1, 0].set_xlabel('RF response')
        axes[1, 0].set_ylabel('(1/N) dN/dx')
        axes[1, 0].set_title('Classifier Response')
        axes[1, 0].legend()
        axes[1, 0].grid(True, alpha=0.3)

        # 4. Feature Importance
        importances = rf.feature_importances_
        features = ['PixelHits', 'TRTHits', 'SCTHits', 'PixeldEdX',
                    'Cal_FVar', 'Cal_EMprop', 'Cal_Lambda',
                    'Cal_Lambda2', 'Cal_Radius']
        axes[1, 1].bar(features, importances)
        axes[1, 1].set_title('Feature Importance')
        axes[1, 1].tick_params(axis='x', rotation=45)
        axes[1, 1].grid(True, alpha=0.3, axis='y')

        plt.tight_layout()
        pdf.savefig(fig, bbox_inches='tight')
        plt.close(fig)

        print("\nReport:")
        print(reports[report_title])

    # Ostatnia strona: podsumowanie wszystkich raportów
    fig = plt.figure(figsize=(11, 14))
    fig.text(
        0.5, 0.98,
        "Classification Reports Summary (Eta ranges)",
        ha='center',
        fontsize=18,
        fontweight='bold'
    )

    y_position = 0.95
    for title, report in reports.items():
        fig.text(
            0.05,
            y_position,
            f"\n{title}",
            fontsize=12,
            fontweight='bold',
            family='monospace'
        )
        y_position -= 0.04

        report_lines = report.split('\n')
        for line in report_lines:
            if line.strip():
                fig.text(
                    0.05,
                    y_position,
                    line,
                    fontsize=9,
                    family='monospace'
                )
                y_position -= 0.015

        y_position -= 0.03

    pdf.savefig(fig, bbox_inches='tight')
    plt.close(fig)

print("\n" + "="*70)
print("PDF zapisany: Plots/RF_eta_output.pdf")
print("="*70)
