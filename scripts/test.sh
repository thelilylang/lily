#!/usr/bin/env bash

make clean > /dev/null
make debug > /dev/null
ninja -C build/Debug > /dev/null
ctest --test-dir build/Debug
