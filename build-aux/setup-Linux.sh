#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

if [ -f /etc/os-release ]; then
    source /etc/os-release
    SETUP="${0/.sh/-$ID-$(echo $VERSION_ID | cut -f1 -d.).sh}"
    if [ -f "$SETUP" ]; then
        $SETUP
    fi
else
    echo "ERROR: unknown linux release (/etc/os-release not found)"
    exit 1
fi
