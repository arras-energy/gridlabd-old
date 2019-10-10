#!/bin/bash
#
# Removes all artifacts produced by the autotest validation process

for i in `find . -type d -name autotest`; 
do 
	for j in `find $i -type d -name test_*`; 
	do 
		rm -rf $j; 
	done; 
done