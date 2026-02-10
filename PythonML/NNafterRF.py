import torch
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset
import numpy as np
#from loadData import df
from RFTrain import df
#from RFTrain import X_train, X_test
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import (
    roc_curve, auc,
    confusion_matrix,
    ConfusionMatrixDisplay,
    classification_report
)




# ---------------- Model ----------------
class SimpleNN(nn.Module):
    def __init__(self, input_size, num_classes=2):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_size, 32),
            nn.ReLU(),
            nn.Linear(32, 16),
            nn.ReLU(),
            nn.Linear(16, num_classes)
        )

    def forward(self, x):
        return self.net(x)


# ---------------- Main ----------------
if __name__ == "__main__":
    # Optional divide eta range
    #df_eta_smaller_137 = df[abs(df['Eta']) < 1.37]
    # I assume eta is already filtered to be <2.5
    #df_eta_bigger_137 = df[abs(df['Eta']) > 1.37]

    # ----- Configuration -----
    target_branch = "IsMuonRF"
    feature_branches = [c for c in df.columns if c != target_branch and c != 'Eta' and c != 'IsMuon']

    batch_size = 64
    num_epochs = 10
    lr = 1e-3

    # ----- Prepare data -----
    X = df[feature_branches].values.astype(np.float32)
    y = df[target_branch].values.astype(np.int64)

    X_train, X_test, y_train, y_test = train_test_split(
        X, y,
        test_size=0.3,
        random_state=42,
        stratify=y
    )

    scaler = StandardScaler()
    X_train = scaler.fit_transform(X_train)
    X_test = scaler.transform(X_test)

    train_ds = TensorDataset(
        torch.tensor(X_train),
        torch.tensor(y_train)
    )
    test_ds = TensorDataset(
        torch.tensor(X_test),
        torch.tensor(y_test)
    )

    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True)

    # ----- Model -----
    model = SimpleNN(len(feature_branches))
    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=lr)

    # ----- Training -----
    print("Starting training\n")

    for epoch in range(num_epochs):
        model.train()
        total_loss = 0.0

        for Xb, yb in train_loader:
            optimizer.zero_grad()
            out = model(Xb)
            loss = criterion(out, yb)
            loss.backward()
            optimizer.step()
            total_loss += loss.item()

        print(f"Epoch [{epoch+1}/{num_epochs}]  "
              f"Loss: {total_loss/len(train_loader):.4f}")

    # ----- Evaluation -----
    model.eval()
    with torch.no_grad():
        logits = model(test_ds.tensors[0])
        probs = torch.softmax(logits, dim=1)[:, 1].numpy()
        y_true = test_ds.tensors[1].numpy()
        y_pred = logits.argmax(dim=1).numpy()

    # ----- Metrics -----
    fpr, tpr, thresholds = roc_curve(y_true, probs)
    roc_auc = auc(fpr, tpr)

    print("\nNeural Network Classification report (with RF feedback):\n")
    print(classification_report(
        y_true, y_pred,
        target_names=["electron", "muon"]
    ))

    # ----- Plots -----
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
    cm = confusion_matrix(y_true, y_pred, normalize="true")
    disp = ConfusionMatrixDisplay(cm, display_labels=["e", "μ"])
    disp.plot(cmap="Blues", ax=plt.gca())
    plt.title("Normalized Confusion Matrix")

    # Score distributions
    plt.subplot(2, 2, 3)
    plt.hist(probs[y_true == 0], bins=50, alpha=0.5,
             label="electron", density=True)
    plt.hist(probs[y_true == 1], bins=50, alpha=0.5,
             label="muon", density=True)
    plt.xlabel("Muon probability")
    plt.ylabel("Density")
    plt.title("Classifier Output")
    plt.legend()
    plt.grid(True)
    #plt.yscale("log")

    # Efficiency vs fake rate
    plt.subplot(2, 2, 4)
    plt.subplot(2, 2, 4)

    plt.plot(thresholds[1:], tpr[1:], label="Muon")
    plt.plot(thresholds[1:], fpr[1:], label="Electron")

    plt.xlabel("Probability threshold")
    plt.ylabel("Rate")
    plt.legend()
    plt.grid(True)

    plt.suptitle("Neural Network Classifier")

    plt.tight_layout()
    plt.show()

    # ----- Example working point -----
    cut = 0.9
    y_cut = (probs > cut).astype(int)
    cm_cut = confusion_matrix(y_true, y_cut, normalize="true")

    print(f"\nWorking point @ P(mu) > {cut}")
    print(f"Muon efficiency : {cm_cut[1,1]:.4f}")
    print(f"Electron fake rate : {cm_cut[0,1]:.4e}")
