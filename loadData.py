import uproot
#import ROOT
import pandas as pd
import numpy as np


# Open the ROOT file and create an RDataFrame
# "TTreeName" should be replaced with the name of your TTree
# "file.root" should be replaced with the path to your file
"""
df_root = ROOT.RDataFrame("classified_data_e_mu", "Classified_data_e_mu_file.root")

# Convert the RDataFrame columns to a dictionary of NumPy arrays
# You can specify a list of columns to retrieve, or leave it empty to get all
column_names = ["track_PixelHits", "trackTRTHits", "PixelEdx"] # Replace with your branch names
numpy_dict = df_root.AsNumpy(columns=column_names)

# Create the pandas DataFrame
df_pandas = pd.DataFrame(numpy_dict)
"""

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