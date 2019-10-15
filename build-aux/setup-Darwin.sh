#!/bin/bash
RELEASE=$(uname -r | cut -f1 -d.)
SETUP="${0/.sh/-$RELEASE.sh}"
if [ ! -f "$SETUP" ]; then
	${0/.sh/-manual.sh}
    echo "manual install required"
    exit 1
fi
$SETUP $*
