#!/usr/bin/env bash

set -e
set -x

LLVM_DIR='clang+llvm-16.0.4-x86_64-linux-gnu-ubuntu-22.04'
LLVM_ARCHIVE="$LLVM_DIR.tar.xz"
PWD="$(pwd)"

# Download & Uncompress LLVM
wget -q "https://github.com/llvm/llvm-project/releases/download/llvmorg-16.0.4/$LLVM_ARCHIVE"
tar -xf "$LLVM_ARCHIVE"

# Download & Install a C compiler
sudo apt-get update
sudo apt-get install clang libcurl4-openssl-dev

# Download & Install Ninja
sudo apt-get install ninja-build

# Download & Install python requirements
pip install -r requirements.txt

# Load & Setup submodules
make submodules_without_llvm

# Setup for Local
./scripts/patches/enable_local.sh

# Configure CMake
cmake \
	-B ./build/Debug \
	-G Ninja \
	-DLLVM_DIR="$PWD/$LLVM_DIR/lib/cmake/llvm" \
	-DLLD_DIR="$PWD/$LLVM_DIR/lib/cmake/lld" \
	-DCMAKE_BUILD_TYPE=Debug \
	-DLILY_DEBUG=ON \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++

# Build
cmake --build build/Debug

# Test
./scripts/full_test.sh
