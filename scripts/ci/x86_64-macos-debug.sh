#!/usr/bin/env bash

set -e
set -x

# Download & Install LLVM
brew install llvm@16

# Download & Install ZLib
brew install zlib

# Download & Install autoconf, automake, libtool
brew install autoconf automake libtool

# Download & Install python requirements
pip install -r requirements.txt

# Load & Setup submodules
make submodules

# Setup for Local
brew install gpatch
./scripts/patches/enable_local.sh

# Configure CMake
export LLVM_DIR=$(brew --prefix llvm@16)
cmake \
	-B ./build/Debug \
	-DCMAKE_BUILD_TYPE=Debug \
	-DLILY_DEBUG=ON \
	-DLILY_BUILD_LLVM=ON \
	-DCMAKE_C_COMPILER=$(brew --prefix llvm@16)/bin/clang \
	-DCMAKE_CXX_COMPILER=$(brew --prefix llvm@16)/bin/clang++

# Build
cmake --build build/Debug

# Test
./scripts/full_test.sh
