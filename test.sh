#!/bin/bash
TEST_DATA_DIR="data/testing/4"
TEST_NAME=${1:-"test"}
TEST_FILE="$TEST_DATA_DIR/$TEST_NAME" 
if test -f "$TEST_FILE.src" ; then
	if cmake . && make && ./bin/compilerdriver "$TEST_FILE.src" ; then
		./data/handout/4/moon/bin_moon +x -t "$TEST_FILE.moon" +x data/handout/4/moon/samples/lib.m
	fi
else
	echo "$TEST_FILE.src does not exist"
fi
