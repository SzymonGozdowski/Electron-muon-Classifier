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
	$(error INPUT variable is not set. Usage: make run INPUT=... OUTPUT=...)
endif
ifndef OUTPUT
	$(error OUTPUT variable is not set. Usage: make run INPUT=... OUTPUT=...)
endif
	$(BIN_DIR)/$(PROGRAM) $(INPUT) $(OUTPUT)

INPUT  := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/data/jpsi_psi2s_extra_variables.root"
OUTPUT := "/Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/out/out_hists.root"
run:
	$(BIN_DIR)/$(PROGRAM) $(INPUT) $(OUTPUT)


PLOT_INPUT  := /Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/out/out_hists.root
PLOT_OUTPUT := /Volumes/Sandisk_Macbook_Pro_1tb/electron_muon_project/plots/out_hists.pdf
plot:
	root -l -b -q "src/plot.C(\"$(PLOT_INPUT)\", \"$(PLOT_OUTPUT)\")"



.PHONY: all clean cleanall dirs run_specified run plot
