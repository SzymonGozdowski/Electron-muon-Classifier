import numpy as np
import matplotlib.pyplot as plt

from loadData import df

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import (
    roc_curve, auc,
    confusion_matrix,
    ConfusionMatrixDisplay,
    classification_report
)

# ---------------- Configuration ----------------
target_branch = "IsMuon"
feature_branches = [c for c in df.columns if c != target_branch]

# ---------------- Data ----------------
X = df[feature_branches].values
y = df[target_branch].values.astype(int)

X_train, X_test, y_train, y_test = train_test_split(
    X, y,
    test_size=0.2,
    random_state=42,
    stratify=y
)

# (optional but safe)
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# ---------------- Model ----------------
model = RandomForestClassifier(
    n_estimators=100,
    random_state=42,
    n_jobs=-1
)

model.fit(X_train, y_train)

# ---------------- Evaluation ----------------
y_pred = model.predict(X_test)
probs = model.predict_proba(X_test)[:, 1]   # P(muon)

print("Random Forest")
print("\nClassification report:\n")
print(classification_report(
    y_test, y_pred,
    target_names=["electron", "muon"]
))

# ---------------- ROC ----------------
fpr, tpr, thresholds = roc_curve(y_test, probs)
roc_auc = auc(fpr, tpr)

# ---------------- Plots ----------------
plt.figure(figsize=(12, 10))

# ROC
plt.subplot(2, 2, 1)
plt.plot(fpr, tpr, label=f"AUC = {roc_auc:.3f}")
plt.plot([0, 1], [0, 1], linestyle="--")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.title("ROC Curve")
plt.legend()
plt.grid(True)

# Confusion matrix (normalized)
plt.subplot(2, 2, 2)
cm = confusion_matrix(y_test, y_pred, normalize="true")
disp = ConfusionMatrixDisplay(cm, display_labels=["e", "μ"])
disp.plot(cmap="Blues", ax=plt.gca())
plt.title("Normalized Confusion Matrix")

# Score distributions
plt.subplot(2, 2, 3)
plt.hist(probs[y_test == 0], bins=50, alpha=0.5,
         label="electron", density=True)
plt.hist(probs[y_test == 1], bins=50, alpha=0.5,
         label="muon", density=True)
plt.xlabel("Muon probability")
plt.ylabel("Density")
plt.title("Classifier Output")
plt.legend()
plt.grid(True)
#plt.yscale("log")

# Efficiency vs fake rate
plt.subplot(2, 2, 4)
plt.plot(thresholds[1:], tpr[1:], label="Muon efficiency")
plt.plot(thresholds[1:], fpr[1:], label="Electron fake rate")
plt.xlabel("Probability threshold")
plt.ylabel("Rate")
plt.legend()
plt.grid(True)

plt.suptitle("Random Forest Classifier")
plt.tight_layout()
plt.show()

plt.tight_layout()
plt.show()

# ---------------- Example working point ----------------
cut = 0.9
y_cut = (probs > cut).astype(int)
cm_cut = confusion_matrix(y_test, y_cut, normalize="true")

print(f"\nWorking point @ P(mu) > {cut}")
print(f"Muon efficiency : {cm_cut[1,1]:.4f}")
print(f"Electron fake rate : {cm_cut[0,1]:.4e}")


