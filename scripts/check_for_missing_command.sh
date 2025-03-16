#!/bin/sh

which $1 > /dev/null 2>&1

if [ $? -ne 0 ]
then
	echo "You need to install $1."
	exit 1
fi
