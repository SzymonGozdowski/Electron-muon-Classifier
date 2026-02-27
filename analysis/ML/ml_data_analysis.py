# ML data preparation 
import uproot
import pandas as pd
import numpy as np
import os
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
print("sklearn imported")
import tensorflow as tf
from tensorflow import keras
print("tensorflow imported")
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt
print("sklearn metrics imported")
print("All libraries imported successfully")

file = uproot.open("../../out/out_ml_data23.root")
tree  = file["mlDataTree"]
df = tree.arrays(library="pd")

print("_______________________________")
print("DEBUG INFO:")
print(df.head())
print(df.dtypes)
print(df.shape)
print("_______________________________")


# 0 = electron, 1 = muon
df["label"] = (df["truth_particle_tag"] == 1).astype(int)


y = df["label"]
X = df.drop(columns=["label", "truth_particle_tag"])  
X = X.fillna(0)  



X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)



model = keras.Sequential([
    keras.layers.Dense(64, activation='relu', input_shape=(X_train_scaled.shape[1],)),
    keras.layers.Dense(32, activation='relu'),
    keras.layers.Dense(1, activation='sigmoid')
])
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
history = model.fit(X_train_scaled, y_train, epochs=20, batch_size=32, validation_split=0.2, verbose=1)

loss, accuracy = model.evaluate(X_test_scaled, y_test)
print(f"Test Loss: {loss:.4f}, Test Accuracy: {accuracy:.4f}")



y_pred = model.predict(X_test_scaled).ravel()

print("_______________________________")
print("DEBUG INFO:")
print("Overall label counts:\n", df["label"].value_counts())
print("Train label counts:\n", y_train.value_counts())
print("Test label counts:\n", y_test.value_counts())
print("Label counts:\n", df["label"].value_counts())
print("Unique y_test:", np.unique(y_test))
print("Min pred:", y_pred.min())
print("Max pred:", y_pred.max())
print("_______________________________")

fpr, tpr, _ = roc_curve(y_test, y_pred)
roc_auc = auc(fpr, tpr)

plt.plot(fpr, tpr)
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.title(f"ROC AUC = {roc_auc:.3f}")
plt.show()

plt.hist(y_pred, bins=50)
plt.title("Predicted probabilities")
plt.show()


## Saving results to CSV for later analysis and plotting
print("Saving NN results to CSV...")
threshold = 0.5 
df_test = X_test.copy()
df_test['truth_particle_tag'] = y_test.values  
df_test['predicted_prob']  = y_pred
df_test['predicted_label'] = (y_pred > threshold).astype(int)

output_filename = "nn_results.csv"
output_path = os.path.join(os.getcwd(), output_filename)
df_test.to_csv(output_path, index=False)

print(f"NN results saved to: {output_path}")
