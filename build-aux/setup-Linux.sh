#!/bin/bash

if [ -f "/etc/os-release" ]; then
    source "/etc/os-release"
    SETUP="${0/.sh/-$ID-$(echo $VERSION_ID | cut -f1 -d.).sh}"
    if [ -f "$SETUP" ]; then
        $SETUP $*
    else
    	${0/.sh/-manual.sh}
    fi
else
    error "unknown linux release (/etc/os-release not found)"
fi
