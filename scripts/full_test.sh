#!/bin/sh

ninja -C build/Debug > /dev/null

./build/Debug/test_core_scanner

echo
./build/Debug/test_core_preparser

echo
./build/Debug/test_core_precompile

echo
./build/Debug/test_core_parser

echo 
./scripts/test_samples.py