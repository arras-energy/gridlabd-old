#!/bin/bash

DIR=${0/unittest.sh/..}
for F in $DIR/*.py; do
	echo '*** '"$(basename $F) unittest"' ***' > /tmp/$$
	/usr/local/bin/python3 $F -q 2>>/tmp/$$ || cat /tmp/$$ >>/tmp/$$.err
done
rm -f /tmp/$$
if [ -f /tmp/$$.err -a ! -s /tmp/$$.err ]; then
	cat /tmp/$$.err >/dev/stderr
	rm -f /tmp/$$.err
	exit 1
else
	exit 0
fi
