#!/usr/bin/env bash

set -e
set -o pipefail

COMMAND="./bin/Debug/lilyc"
FILES_EXCLUDED=("./tests/samples/package/a.lily" "./tests/samples/package/b.lily")
SAMPLES_DIRS=./tests/samples/**
SUCCESS=0
FAILED=0

# Option config
NO_EXIT=false

# $1: success
# $2: failed
# $3: total
function print_summary {
	echo "failed: $2, success: $1, total: $3"
}

# parse options
for arg in $@
do
	case $arg in
		"--no-exit")
			NO_EXIT=true
			;;
		"-h" | "--help")
			echo  "Usage: ./scripts/test_samples.sh [options]

Options:

  --no-exit        Force to not exit(1), even if there are failed tests
  -h, --help       Print help"
  				exit 0
				;;
		*)
			echo "error: wrong argument: $arg"
			exit 1
			;;
	esac
done

# Run all samples
for dir in $SAMPLES_DIRS
do
	if [[ ! -d $dir ]]
	then
		continue
	fi

	for samples_file in $dir/*.lily
	do
		is_excluded=false

		for file_excluded in ${FILES_EXCLUDED[@]}
		do	
			if [ $file_excluded == $samples_file ]
			then
				is_excluded=true
				break
			fi
		done

		if ! $is_excluded
		then
			set +e

			$COMMAND $samples_file > /dev/null

			command_status=$?

			if [ $command_status == 0 ]
			then
				let "SUCCESS+=1"
			else
				echo "Failed on $samples_file"
				let "FAILED+=1"
			fi

			set -e
		fi
	done
done

print_summary $SUCCESS $FAILED $(($SUCCESS + $FAILED))

if ! $NO_EXIT
then
	if [ $FAILED -gt 0 ]
	then
		exit 1
	fi
fi
