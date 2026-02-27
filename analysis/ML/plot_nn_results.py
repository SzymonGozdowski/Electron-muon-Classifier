import pandas as pd 
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay, roc_curve, auc
from matplotlib.backends.backend_pdf import PdfPages

input_file = "nn_results.csv"
df = pd.read_csv(input_file)

y_true = df['truth_particle_tag'].values
y_pred_label = df['predicted_label'].values
y_pred_prob  = df['predicted_prob'].values

pdf_filename = "nn_analysis_results.pdf"
pdf_pages = PdfPages(pdf_filename)

# Confusion Matrix
cm = confusion_matrix(y_true, y_pred_label)
disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=["Electron (0)", "Muon (1)"])

fig, ax = plt.subplots(figsize=(5,5))
disp.plot(ax=ax, cmap=plt.cm.Blues, colorbar=True)
plt.title("Confusion Matrix")
pdf_pages.savefig(fig)
plt.close(fig)

# Bar plot of true vs predicted counts
truth_counts = pd.Series(y_true).value_counts().sort_index()       # 0=electron, 1=muon
pred_counts = pd.Series(y_pred_label).value_counts().sort_index()

labels = ["Electron (0)", "Muon (1)"]
x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots(figsize=(6,4))
ax.bar(x - width/2, truth_counts, width, label='Truth', color='skyblue')
ax.bar(x + width/2, pred_counts, width, label='Predicted', color='orange')

ax.set_ylabel("Number of particles")
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.set_title("Predicted vs Truth particles")
ax.legend()
pdf_pages.savefig(fig)
plt.close(fig)

# ROC curve and AUC
fpr, tpr, _ = roc_curve(y_true, y_pred_prob)
roc_auc = auc(fpr, tpr)
fig, ax = plt.subplots(figsize=(6,5))
ax.plot(fpr, tpr, color='darkorange', lw=2, label=f"ROC curve (AUC = {roc_auc:.3f})")
ax.plot([0,1], [0,1], color='navy', lw=1, linestyle='--')
ax.set_xlim([0.0, 1.0])
ax.set_ylim([0.0, 1.05])
ax.set_xlabel("False Positive Rate")
ax.set_ylabel("True Positive Rate")
ax.set_title("ROC Curve")
ax.legend(loc="lower right")
pdf_pages.savefig(fig)
plt.close(fig)

# Histogram of predicted probabilities
fig, ax = plt.subplots(figsize=(6,4))
ax.hist(y_pred_prob, bins=50, color='green', alpha=0.7)
ax.set_xlabel("Predicted probability for muon (1)")
ax.set_ylabel("Number of particles")
ax.set_title("Predicted probabilities")
pdf_pages.savefig(fig)
plt.close(fig)

#save all plots to PDF
pdf_pages.close()
print(f"All plots saved to: {pdf_filename}")


