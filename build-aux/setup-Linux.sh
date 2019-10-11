#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

if [ -f /etc/os-release ]; then
    . /etc/os-release
    METHOD="$0-$ID-$(echo $VERSION | cut -f -d.)"
    if [ -f $METHOD ]; then
        $METHOD
    fi
else
    echo "ERROR: unknown linux type"
    exit 1
fi