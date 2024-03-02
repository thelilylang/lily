#!/usr/bin/env bash

# ./scripts/exe.sh <exe> [args...]
#
# Brief: This script lets you run the executable locally when shared
# libraries are not installed.

# Set a library path because libraries like `liblily_sys` and
# `liblily_builtin` are shared, so in local mode you need
# to set `LD_LIBRARY_PATH`.
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:build:build/Debug"

./$@
