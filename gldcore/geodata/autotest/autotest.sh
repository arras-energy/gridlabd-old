#!/bin/bash

DIR=${0/autotest.sh/..}
for F in $DIR/*.py; do
	echo "Processing $F..."
	/usr/local/bin/python3 $F 
done