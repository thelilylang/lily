#!/usr/bin/env bash

set -e
set -x

# Download & Install LLVM
brew install llvm@18

# Download & Install ZLib
brew install zlib

# Download & Install Ninja
brew install ninja

# Download & Install autoconf, automake, libtool
brew install autoconf automake libtool

# Download & Install python requirements
pip install -r requirements.txt

# Load & Setup submodules
make submodules_without_llvm

# Setup for Local
brew install gpatch
./scripts/patches/enable_local.sh

# Configure CMake
export LLVM_DIR=$(brew --prefix llvm@18)
cmake \
	-B ./build/Debug \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Debug \
	-DLILY_DEBUG=ON \
	-DCMAKE_C_COMPILER=$(brew --prefix llvm@18)/bin/clang \
	-DCMAKE_CXX_COMPILER=$(brew --prefix llvm@18)/bin/clang++

# Build
cmake --build build/Debug

# Test
./scripts/full_test.sh
