#!/usr/bin/env bash

FAIL_139=0
FAIL_1=0
OTHER_FAIL=0
SUCCESS=0

for bin in ./out.lily/bin/*
do
	./$bin

	let code_status=$?

	if [ $code_status == 139 ]
	then
		let "FAIL_139+=1"
	elif [ $code_status == 1 ]
	then
		let "FAIL_1+=1"
	elif [ $code_status == 0 ]
	then
		let "SUCCESS+=1"
	else
		let "OHTER_FAIL+=1"
	fi
done

let total=$(($FAIL_139 + $FAIL_1 + $OTHER_FAIL + $SUCCESS))
let total_fail=$(($FAIL_139 + $FAIL_1 + $OTHER_FAIL))

# Resume
echo "Exit with 139 code: $FAIL_139"
echo "Exit with 1 code: $FAIL_1"
echo "Success code: $SUCCESS"
echo "Total: $(($total - $total_fail))/$total passed"

if [ $total_fail -gt 0 ]
then
	exit 1
fi
