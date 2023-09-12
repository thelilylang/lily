#!/bin/sh

FAIL_139=0
FAIL_1=0
N=100

ninja -C build/Debug

for _ in $( seq 0 $N )
do
	./build/Debug/lilyc --verbose ./mypkg/main.lily 2> /dev/null

	let code_status=$?
	
	if [ $code_status == 139 ]
	then
		let "FAIL_139+=1"
	elif [ $code_status == 1 ]
	then
		let "FAIL_1+=1"
	fi
done

echo "Exit with 139 code: $FAIL_139"
echo "Exit with 1 code: $FAIL_1"
echo "Total: $(($N - ($FAIL_139 + $FAIL_1)))/100 passed"
