#!/bin/bash

OPTIONS=$(gridlabd --help | grep '  --' | sed -r 's/^  --([-a-z_]+).+$/\1/g')

for OPT in $OPTIONS; do
	NAME=$(echo ${OPT:0:1} | tr a-z A-Z)${OPT:1}
	FILE="docs/Command/$NAME.md"
	if [ ! -f "$FILE" ]; then
		HELP=$(gridlabd --help | grep "  --$OPT")
		ALT=$(echo $HELP | cut -f2 -d'|' | cut -c2)
		if [ "$ALT" == "-" ]; then
			ALT=""
		fi
		SYN="bash\$ gridlabd ${HELP:2:56}"
		INFO="${HELP:57}"
		echo """[[/Command/$NAME]] - $INFO

# Synopsis
~~~
$SYN
~~~

# Description

$INFO.
""" > $FILE
	fi
done

