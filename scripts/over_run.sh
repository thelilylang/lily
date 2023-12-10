#!/bin/sh

FAIL_139=0
FAIL_1=0
OTHER_FAIL=0
N=100

ninja -C build/Debug

echo -n "filename> "
read filename

for _ in $( seq 0 $N )
do
	./bin/lilyc --verbose $filename 2> /dev/null

	let code_status=$?
	
	if [ $code_status == 139 ]
	then
		let "FAIL_139+=1"
	elif [ $code_status == 1 ]
	then
		let "FAIL_1+=1"
	elif [ $code_status != 0 ]
	then
		let "OTHER_FAIL+=1"
	fi
done

let total_fail=$(($FAIL_139 + $FAIL_1 + $OTHER_FAIL))

echo "Exit with 139 code: $FAIL_139"
echo "Exit with 1 code: $FAIL_1"
echo "Exit with other failing code: $OTHER_FAIL"
echo "Total: $(($N - $total_fail))/100 passed"

if [ $total_fail -gt 0 ]
then
	exit 1
fi
