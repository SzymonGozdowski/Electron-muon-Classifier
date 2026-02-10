import uproot
#import ROOT
import pandas as pd
import numpy as np

# Open ROOT file and TTree
"""
file = uproot.open("../Classified_data_e_mu_file.root")
tree = file["classified_data_e_mu"]
"""
file = uproot.open("/Users/szymon/Physics/Particles/MLData.root")
tree = file["MLDataTree"]

# Convert to pandas DataFrame
df = tree.arrays(library="pd")
print(df.head())