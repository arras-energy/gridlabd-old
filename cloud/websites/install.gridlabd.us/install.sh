#!/bin/bash

LATEST=$(curl -sL http://install.gridlabd.us/latest.txt)

SYSTEM=$(uname -s)

case $SYSTEM in

    Darwin)
        
        RELEASE=$(uname -r | cut -f1 -d.)

        ;;

    *)

        echo Install not available for $SYSTEM.  Try manual installation instead.

esac

TARGET=/usr/local/opt/gridlabd
mkdir -p $TARGET
cd $TARGET
curl -sL "http://install.gridlabd.us/gridlabd-$SYSTEM-$RELEASE-$LATEST-master.tarz" | tar xz
if [ -f $TARGET/share/gridlabd/setup-$SYSTEM-$RELEASE.sh ]; then
    . $TARGET/share/gridlabd/setup-$SYSTEM-$RELEASE.sh
fi
/usr/local/opt/gridlabd/$LATEST-master/bin/gridlabd version set
