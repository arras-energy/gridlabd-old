#!/bin/bash
RELEASE=$(uname -r | cut -f1 -d.)
SETUP="${0/.sh/-$RELEASE}"
if [ -f "$SETUP" ]; then
    source "$SETUP" &*
else
	echo "$0: no installer available for $(basename $SETUP)"
fi
