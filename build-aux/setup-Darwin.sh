#!/bin/bash
RELEASE=$(uname -r | cut -f1 -d.)
SETUP="${0/.sh/-$RELEASE}"
if [ -f "$SETUP" ]; then
    $SETUP &*
else
	${0/.sh/-manual.sh}
fi
