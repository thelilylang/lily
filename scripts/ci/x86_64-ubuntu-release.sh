#!/usr/bin/env bash

set -e
set -x

# Download & Install LLVM
sudo apt-get update
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 16 all

# Download & Install python requirements
pip install -r requirements.txt

# Load & Setup submodules
make submodules

# Setup for Local
./scripts/patches/enable_local.sh

# Configure CMake
cmake \
	-B ./build \
	-DCMAKE_BUILD_TYPE=Release \
	-DLILY_DEBUG=ON \
	-DLILY_BUILD_LLVM=ON \
	-DCMAKE_C_COMPILER=clang-16 \
	-DCMAKE_CXX_COMPILER=clang++-16

# Build
cmake --build build
