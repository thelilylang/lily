#!/usr/bin/env bash

STATUS_CODE=0

function update_status_code {
	local current_status_code=$?

	if [ $STATUS_CODE == 0 ]
	then
		STATUS_CODE=$current_status_code
	fi
}

function run_test {
	$@
	update_status_code
	echo
}

cmake --build build/Debug > /dev/null

run_test ./bin/Debug/test_base
run_test ./bin/Debug/test_core_scanner
run_test ./bin/Debug/test_core_preparser
run_test ./bin/Debug/test_core_precompiler
run_test ./bin/Debug/test_core_parser
run_test python ./scripts/test_samples.py
run_test ./scripts/run_bins.sh

exit $STATUS_CODE
