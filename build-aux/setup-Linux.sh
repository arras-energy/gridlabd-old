#!/bin/bash

EXE="$0"
function error()
{
    echo "$EXE: $*" >/dev/stderr
    exit 1
}

if [ -f "/etc/os-release" ]; then
    source "/etc/os-release"
    SETUP="${0/.sh/-$ID-$(echo $VERSION_ID | cut -f1 -d.).sh}"
    if [ -f "$SETUP" ]; then
        $SETUP $*
    else
    	build-aux/setup-manual.sh
    fi
else
    error "unknown linux release (/etc/os-release not found)"
fi
