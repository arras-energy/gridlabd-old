#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

EXE="$0"
function error()
{
    echo "$EXE: $*" >/dev/stderr
    exit 1
}

SYSTEM=$(uname -s)
if [ ! -f "build-aux/setup-$SYSTEM.sh" ]; then
	build-aux/setup-manual.sh $*
else
	build-aux/setup-$SYSTEM.sh $*
fi
