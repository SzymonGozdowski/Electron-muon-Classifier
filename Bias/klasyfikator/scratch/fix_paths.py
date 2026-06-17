import os

file_path = r'C:\Users\qmiko\Desktop\Studia\cern\Bias\Podzial\Plots_filtered\wykresy.ipynb'
with open(file_path, 'r', encoding='utf-8') as f:
    text = f.read()

text = text.replace('SCALER_PATH = \\"../../Bias/klasyfikator/trained/scaler_mlp1.pkl\\"', 'SCALER_PATH = \\"../../../Bias/klasyfikator/trained/scaler_mlp1.pkl\\"')

with open(file_path, 'w', encoding='utf-8') as f:
    f.write(text)

print("Done")
