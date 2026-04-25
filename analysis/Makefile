# ---------- Project configuration ----------
PROGRAM      := analysis
SRC_DIR      := src
INC_DIR      := include
BUILD_DIR    := build
BIN_DIR      := bin

# wszystkie pliki źródłowe .cpp i .C
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.C)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
OBJS := $(patsubst $(SRC_DIR)/%.C,$(BUILD_DIR)/%.o,$(OBJS))

# ---------- ROOT configuration ----------
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)

CXX      := g++
CXXFLAGS := -O2 -Wall -I$(INC_DIR) $(ROOTCFLAGS)
LDFLAGS  := $(ROOTLIBS)

# ---------- Main targets ----------
all: dirs $(BIN_DIR)/$(PROGRAM)

$(BIN_DIR)/$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# kompilacja plików .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# kompilacja plików .C (ROOT MakeClass)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- Utilities ----------
dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/$(PROGRAM)

cleanall:
	rm -rf $(BUILD_DIR) $(BIN_DIR)




#----- Analysis -----------

run_specified:
ifndef INPUT
	$(error INPUT variable is not set. Usage: make run INPUT=... OUTPUT=... OUTPUT_ML=...)
endif
ifndef OUTPUT
	$(error OUTPUT variable is not set. Usage: make run INPUT=... OUTPUT=... OUTPUT_ML=...)
endif
ifndef OUTPUT_ML
	$(error OUTPUT_ML variable is not set. Usage: make run INPUT=... OUTPUT=... OUTPUT_ML=...)
endif
	$(BIN_DIR)/$(PROGRAM) $(INPUT) $(OUTPUT) $(OUTPUT_ML)

#INPUT  := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/jpsi_psi2s_extra_variables.root"
#INPUT  := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/data23_2trk_moreTCvars.root"
INPUT  := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/jpsi_cell_sig_sampling.root"
OUTPUT := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/control_histograms/out_hists_data_with_sig_sampling.root"
OUTPUT_ML := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data_for_ml/ml_data_with_sig_sampling.root"
run:
	$(BIN_DIR)/$(PROGRAM) $(INPUT) $(OUTPUT) $(OUTPUT_ML)


PLOT_INPUT  := /Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/control_histograms/out_hists_data_with_sig_sampling.root
PLOT_OUTPUT := /Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/plots/out_hists_data_with_sig_sampling.pdf
plot:
	root -l -b -q "src/plot.C(\"$(PLOT_INPUT)\", \"$(PLOT_OUTPUT)\")"

plot_specified:
	@if [ -z "$(output)" ]; then \
		echo "ERROR: Please specify output=electrons or output=muons"; \
		exit 1; \
	fi
	root -l -b -q "src/plot.C(\"$(PLOT_INPUT)\", \"$(dir $(PLOT_OUTPUT))$(basename $(notdir $(PLOT_OUTPUT)))_$(output).pdf\")"


run_MC:
	root -l -b -q '../analysis_MC/MC_analysis.C+'

clean_MC:
	rm -f ../analysis_MC/*.so ../analysis_MC/*_ACLiC_* ../analysis_MC/*.pcm


.PHONY: all clean cleanall dirs run_specified run plot plot_specified run_MC clean_MC
