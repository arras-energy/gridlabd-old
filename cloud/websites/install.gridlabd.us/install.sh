#!/bin/bash

LATEST=$(curl -sL http://install.gridlabd.us/latest.txt)

SYSTEM=$(uname -s)

case $SYSTEM in

    Darwin)
        
        
        VERSION_ID=$(uname -r | cut -f1 -d.)

        ;;

    Linux)

        source "/etc/os-release"

    *)

        echo Install not available for $SYSTEM.  Try manual installation instead.

esac

TARGET=/usr/local/opt/gridlabd
mkdir -p $TARGET
cd $TARGET
curl -sL "http://install.gridlabd.us/gridlabd-$SYSTEM-$VERSION_ID-$LATEST-master.tarz" | tar xz
if [ -f $TARGET/share/gridlabd/setup-$SYSTEM-$VERSION_ID.sh ]; then
    . $TARGET/share/gridlabd/setup-$SYSTEM-$VERSION_ID.sh
fi
/usr/local/opt/gridlabd/$LATEST-master/bin/gridlabd version set
