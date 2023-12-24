#!/bin/sh

ENABLE_PATCH=./scripts/patches/enable_*.sh
DISABLE_PATCH=./scripts/patches/disable_*.sh

# $1 command
function check_patch() {
	./$1 > /dev/null

	if [ $? -ne 0 ]
	then
		echo "error($e): the patch is out of date"
		exit 1
	fi
}

for e in $ENABLE_PATCH
do
	check_patch $e
done

for d in $DISABLE_PATCH
do
	check_patch $d
done

echo "All patches are up to date."
