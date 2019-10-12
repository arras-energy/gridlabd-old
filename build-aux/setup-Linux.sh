#!/bin/bash
if [ -f /etc/os-release ]; then
    source /etc/os-release
    SETUP="${0/.sh/-$ID-$(echo $VERSION_ID | cut -f1 -d.).sh}"
    if [ -f "$SETUP" ]; then
        source $SETUP
    else
    	error "not installer available for $(basename $SETUP)"
    fi
else
    error "unknown linux release (/etc/os-release not found)"
fi
