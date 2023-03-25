#!/bin/sh

make clean
make debug
ninja -C build/Debug
ctest --test-dir build/Debug
