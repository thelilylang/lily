#!/usr/bin/env bash

set -e
set -x

# The version of Emscripten the wasm target is built with.
EMSDK_VERSION=latest
EMSDK_DIR="$PWD/emsdk"

# Download & Install Emscripten
#
# The emsdk is installed inside the workspace, so its cache is writable and
# `emcc` does not have to rebuild its sysroot outside of it.
git clone --depth 1 https://github.com/emscripten-core/emsdk.git "$EMSDK_DIR"
"$EMSDK_DIR/emsdk" install "$EMSDK_VERSION"
"$EMSDK_DIR/emsdk" activate "$EMSDK_VERSION"

EMSCRIPTEN_DIR="$EMSDK_DIR/upstream/emscripten"
export PATH="$EMSCRIPTEN_DIR:$PATH"

emcc --version

# Download & Install Ninja
sudo apt-get update
sudo apt-get install -y ninja-build

# Load & Setup submodules
#
# NOTE: LLVM is not one of them here: the wasm target is `cic_wasm.js`, which
# does not link LLVM, so only libyaml is needed.
make submodules_without_llvm

# Configure CMake
#
# The superbuild driver always spawns the native build next to the wasm one, so
# `INNER_BUILD=1` is what leaves it out: the wasm toolchain is configured on its
# own, and it is the Emscripten toolchain file that turns `LILY_WASM` on and
# limits the tree to what `cic` needs.
make debug INNER_BUILD=1 BUILD_WASM=1

# Build
make build

# The wasm build produces no native binary and registers no test, so what it
# has to show for itself is the module: read it back rather than trusting the
# build to have written it.
ls -l ./bin/Debug/wasm/cic_wasm.js ./bin/Debug/wasm/cic_wasm.wasm ./bin/Debug/wasm/cic_wasm.data
