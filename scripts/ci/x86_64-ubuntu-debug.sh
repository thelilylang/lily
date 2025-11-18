#!/usr/bin/env bash

set -e
set -x

# Download & Install LLVM
sudo apt-get update
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 18 all
sudo apt-get install liblld-18-dev

# Download & Install Ninja
sudo apt-get install ninja-build

# Load & Setup submodules
make submodules_without_llvm

# Setup for Local
./scripts/patches/enable_local.sh

# Configure CMake
cmake \
	-B ./build/Debug \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Debug \
	-DLILY_DEBUG=ON \
	-DCMAKE_C_COMPILER=clang-18 \
	-DCMAKE_CXX_COMPILER=clang++-18

# Build
ninja -C build/Debug

# Test
ninja test -C build/Debug
