#!/bin/bash
D_ARCH=$(uname -m)
SETUP="${0/.sh/-$D_ARCH.sh}"
if [ ! -f "$SETUP" ]; then
	${0/Darwin.sh/manual.sh}
    echo "ERROR: $SETUP not found, manual install required"
    exit 1
fi
$SETUP $*