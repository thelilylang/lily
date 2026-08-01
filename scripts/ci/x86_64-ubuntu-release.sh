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
make configure CC=clang-18 CXX=clang++-18

# Build
make build
