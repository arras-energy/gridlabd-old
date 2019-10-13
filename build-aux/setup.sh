#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

EXE="$0"
function error()
{
    echo "$EXE: $*" >/dev/stderr
    exit 1
}

SYSTEM=$(uname -s)
SETUP=${0/.sh/-$SYSTEM.sh}
if [ ! -f "$SETUP" ]; then
	${0/.sh/-manual.sh} $*
else
	$SETUP $*
fi
