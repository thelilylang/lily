#!/usr/bin/env bash

set -e
set -x

# The version of LLVM the project is built and linked against.
LLVM_VERSION=18

# Download & Install LLVM
sudo apt-get update
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh "$LLVM_VERSION" all
sudo apt-get install -y "liblld-$LLVM_VERSION-dev"

# Download & Install the latest Clang
#
# The C the transpiler generates is compiled by whichever `clang` is on the
# PATH, so the tests only reach as far into C23 as that one does. The version
# the project is built against is held back to the LLVM it links against, so
# the latest is installed next to it and is the one the PATH is pointed at.
sudo ./llvm.sh
CLANG_VERSION=$(
	for dir in /usr/lib/llvm-*; do
		basename "$dir" | cut -d- -f2
	done | sort -V | tail -n 1
)
sudo update-alternatives \
	--install /usr/bin/clang clang "/usr/bin/clang-$CLANG_VERSION" 100
sudo update-alternatives \
	--install /usr/bin/clang++ clang++ "/usr/bin/clang++-$CLANG_VERSION" 100
sudo update-alternatives --set clang "/usr/bin/clang-$CLANG_VERSION"
sudo update-alternatives --set clang++ "/usr/bin/clang++-$CLANG_VERSION"

# The version the tests are compiled with is worth reading back, as it is what
# tells a test that fails apart from one the compiler cannot yet read.
clang --version

# Download & Install Ninja
sudo apt-get install -y ninja-build

# Load & Setup submodules
make submodules_without_llvm

# Setup for Local
./scripts/patches/enable_local.sh

# Enable assertions (ASSERT, ASSERT_EQ, etc).
# Please look at `include/base/assert.h`.
./scripts/patches/enable_assert.sh

# Configure CMake
#
# NOTE: The package directories are named, so that the LLVM the project is
# built against is the one it asks for, rather than whichever one the newer
# install of Clang brought along.
make debug \
	CC="clang-$LLVM_VERSION" \
	CXX="clang++-$LLVM_VERSION" \
	LLVM_DIR="/usr/lib/llvm-$LLVM_VERSION/lib/cmake/llvm" \
	LLD_DIR="/usr/lib/llvm-$LLVM_VERSION/lib/cmake/lld" \
	CLANG_DIR="/usr/lib/llvm-$LLVM_VERSION/lib/cmake/clang"

# Build & Test
#
# NOTE: `test` builds first, and runs `ctest` from the native inner build of
# the superbuild, which is what registers the tests.
make test
