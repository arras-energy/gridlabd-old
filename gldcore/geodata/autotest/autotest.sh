#!/bin/bash

DIR=${0/autotest.sh/..}
for F in $DIR/*.py; do
	echo "$(basename $F) unittest:" > /tmp/$$
	/usr/local/bin/python3 $F -q 2>>/tmp/$$ || cat /tmp/$$ >/dev/stderr
done
rm -f /tmp/$$