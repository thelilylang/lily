#!/bin/sh

cmake --build build/Debug > /dev/null

./build/Debug/test_core_scanner

echo
./build/Debug/test_core_preparser

echo
./build/Debug/test_core_precompiler

echo
./build/Debug/test_core_parser

echo
python ./scripts/test_samples.py

let test_samples_status=$?

echo
./scripts/run_bins.sh

exit $test_samples_status
