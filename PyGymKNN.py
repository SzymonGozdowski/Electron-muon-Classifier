import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
import seaborn as sns


chunks = []
with uproot.open("MLData.root") as f:
    df = f["MLDataTree"].arrays(library="pd")  

X = df[['track_PixelHits', 'track_TRTHits', 'track_SCTHits', 'track_PixeldEdX', 'Cal_FVariable', 'Cal_EMprop' ,'Cal_Lambda','Cal_Lambda2','Cal_Radius','Cal_Time']]
y = df['IsMuon'] 

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)



knn = KNeighborsClassifier(n_neighbors=13, weights='distance')
knn.fit(X_train, y_train)
y_pred = knn.predict(X_test)
print(f"Accuracy: {accuracy_score(y_test, y_pred):.3f}")


fig, axes = plt.subplots(2, 2, figsize=(15, 12))


cm = confusion_matrix(y_test, y_pred)
sns.heatmap(cm, annot=True, fmt='d', ax=axes[0,0], cmap='Blues')
axes[0,0].set_title('Confusion matrix')


from sklearn.metrics import roc_curve, auc
if len(np.unique(y)) == 2:
    fpr, tpr, _ = roc_curve(y_test, knn.predict_proba(X_test)[:,1])
    auc_score = auc(fpr, tpr)
    axes[0,1].plot(fpr, tpr, label=f'ROC (AUC = {auc_score:.3f})')
    axes[0,1].plot([0,1],[0,1], 'k--')
    axes[0,1].set_xlabel('FPR'); axes[0,1].set_ylabel('TPR')
    axes[0,1].legend()


axes[1,0].scatter(range(100), y_test[:100], c='blue', alpha=0.6, label='True')
axes[1,0].scatter(range(100), y_pred[:100], c='red', alpha=0.6, label='Predicted')
axes[1,0].legend(); axes[1,0].set_title('Predicted vs True')


k_range = range(5, 6)
scores = []
for k in k_range:
    knn_k = KNeighborsClassifier(n_neighbors=k)
    knn_k.fit(X_train, y_train)
    scores.append(accuracy_score(y_test, knn_k.predict(X_test)))
axes[1,1].plot(k_range, scores, 'o-')
axes[1,1].set_xlabel('k'); axes[1,1].set_ylabel('Accuracy')
axes[1,1].set_title('Optimal k'); axes[1,1].grid(True)
plt.tight_layout()
plt.savefig("Plots/knn_info.png", dpi=300)
plt.show()


scores = knn.predict_proba(X_test)[:, 1]


signal_scores = scores[y_test == 1]
bkg_scores    = scores[y_test == 0]


bins = np.linspace(0, 1, 25)  

plt.figure(figsize=(8,6))
plt.hist(bkg_scores, bins=bins, density=True,
         histtype='stepfilled', alpha=0.6,
         color='red', label='Electron', hatch='//')
plt.hist(signal_scores, bins=bins, density=True,
         histtype='stepfilled', alpha=0.4,
         color='blue', label='Muon')

plt.xlabel('kNN response')
plt.ylabel('(1/N) dN/dx')
plt.title('kNN response distribution')
plt.legend()
plt.tight_layout()
plt.savefig("Plots/knn_score.png", dpi=300)
plt.show()

print("\nReport:")
print(classification_report(y_test, y_pred))

