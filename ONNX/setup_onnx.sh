#!/bin/bash
export ORT_DIR=$(pwd)/onnxruntime
export LD_LIBRARY_PATH=$ORT_DIR/lib:$LD_LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$ORT_DIR/include:$CPLUS_INCLUDE_PATH