#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

EXE="$0"
function error()
{
    echo "$EXE: $*" >/dev/stderr
    exit 1
}

if [ ! -f "setup.ok" ]; then
	SYSTEM=$(uname -s)
	if [ ! -f "build-aux/setup-$SYSTEM.sh" ]; then
	    error "unable to setup $SYSTEM, build-aux/setup-$SYSTEM.sh not found"
	else
		build-aux/setup-$SYSTEM.sh $*
	fi
	date > setup.ok
fi