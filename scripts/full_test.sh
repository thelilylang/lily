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

function print_header {
	printf '=%.0s' {1..50}
	printf "$1"
	printf '=%.0s' {1..50}
	printf '\n'
	sleep 1
}

cmake --build build/Debug > /dev/null

print_header "Lily"

run_test ./bin/Debug/test_base
run_test ./bin/Debug/test_core_scanner
run_test ./bin/Debug/test_core_preparser
run_test ./bin/Debug/test_core_precompiler
run_test ./bin/Debug/test_core_parser
run_test python ./scripts/test_samples.py
run_test ./scripts/run_bins.sh

print_header "CI"

run_test ./scripts/compare_ci.sh

exit $STATUS_CODE
