#!/bin/bash
#
# Removes directories created by the build and validation process

for i in `find . -type d -name autotest -print`; do 
	for f in `find $i -type d -name 'test_*' -print`; do
		m -rf $f
	done
done

for f in `find . -type d -name '.deps' -print`; do
	rm -rf $f
done

for f in `find . -type d -name '.libs' -print`; do
	rm -rf $f
done