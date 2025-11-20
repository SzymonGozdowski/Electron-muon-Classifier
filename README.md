Electron–Muon Classifier (UPC Data – ATLAS CERN)
This repository contains a machine-learning project aimed at separating electrons from muons using Ultra-Peripheral Collision (UPC) data from the ATLAS experiment.
All development is done within the CERN software ecosystem, using ROOT (C++ version) for data handling, feature extraction, training, and evaluation.
The project explores multiple ML algorithms through ROOT’s built-in tools (TMVA) and custom C++ implementations. Performance will be compared under a unified framework to determine the most effective approach for UPC-based lepton identification.
Overview
The project provides:
C++/ROOT modules for data loading and feature preparation
TMVA-based configuration for training multiple ML models
Evaluation utilities for comparing classifiers
Scripts for plotting ROC curves, efficiencies, confusion matrices, and variable importance
A consistent workflow compatible with ATLAS analysis environments
Planned Models
All models are implemented and tested via TMVA and additional C++ code where needed.
TMVA / ROOT Models
Likelihood & Fisher discriminants
Boosted Decision Trees
Support Vector Machines
k-NN
Multilayer Perceptrons (TMVA NN)
Additional Models (C++ Implementations)
Logistic Regression
Custom lightweight neural networks
Optional: integration with ONNX models if needed
Repository Structure
root/
│
├── data/                     # Input ROOT files (not tracked)
├── include/                  # Headers for preprocessing, models, utils
├── src/
│   ├── preprocessing/        # Data reading, variable selection, transforms
│   ├── tmva/                 # TMVA configuration and training classes
│   ├── models/               # Custom C++ ML models
│   ├── evaluation/           # ROC, metrics, comparison tools
│   └── utils/                # Shared helpers
├── macros/                   # ROOT macros for quick checks/plots
├── results/                  # Output weights, XML configs, plots
└── README.md
Requirements
CERN ROOT (C++ version)
CMake
A compiler supported by ROOT (GCC/Clang)
Optional: ONNXRuntime for external model loading
Build Instructions
mkdir build
cd build
cmake ..
make -j
Usage
1. Prepare data
Run preprocessing to convert raw UPC trees into analysis-ready ROOT files:
./preprocess_upc
2. Train models
Run all TMVA trainings:
./train_tmva
Or train a single model:
./train_tmva --method BDT
3. Compare performance
./compare_models
This generates summary plots in results/ showing metrics for every tested algorithm.
Goals
Provide a fully reproducible C++/ROOT pipeline for UPC-based lepton classification
Benchmark a variety of ML approaches under identical conditions
Support future additions such as hyperparameter scans or deep-learning imports
Serve as a reference for ATLAS analyses using ROOT-native ML tools
License
MIT License.
