#!/bin/bash

DIR=${0/unittest.sh/..}
N=0
for F in $DIR/*.py; do
	echo '*** '"$(basename $F) unittest"' ***' > /tmp/$$_$N.err
	( gridlabd python $F -q 2>>/tmp/$$_$N.err && rm /tmp/$$_$N.err ) &
	N=$(($N+1))
done
wait
rm -rf /tmp/geodata
touch /tmp/$$_ok.err
cat /tmp/$$_*.err > /tmp/$$.err
rm -f /tmp/$$_*.err
if [ -s /tmp/$$.err ]; then
	cat /tmp/$$.err > /dev/stderr
	rm -f /tmp/$$.err
	exit 1
else
	rm -f /tmp/$$.err
	exit 0
fi
