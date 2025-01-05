#!/usr/bin/env bash

echo Starting:
echo

make main || exit 1

for i in *.input; do
	echo Testing: $i
    cat $i | ./main
   exit
done