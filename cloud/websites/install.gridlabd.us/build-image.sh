#!/bin/bash

# This file is for developer and maintainer use only. 
# Build gridlabd on a clean system for best results in building and saving an image.

SYSTEM=$(uname -s)
RELEASE=$(uname -r | cut -f1 -d.)
KERNEL=
if test $SYSTEM == "Linux"; then
source "/etc/os-release"
KERNEL="-$ID-$(echo $VERSION_ID | cut -f1 -d.)"
fi
D_ARCH=$(uname -m)
EXE=$0
TOP=$(cd ${EXE/version.sh/..}; pwd)
cd $TOP
cd ../../..
TOP=$(cd ${EXE/version.sh/..}; pwd)
FIL="${TOP}/source/version.h"
MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
BRA=`git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 - | sed 's/-+/-/g;s/-$//;s/^-//'`


if [ $SYSTEM == "Darwin" ]; then
    cp /opt/homebrew/lib/lib* /usr/local/opt/gridlabd/lib
    cp /opt/homebrew/opt/*/lib/lib*.* /usr/local/opt/gridlabd/lib
fi

# MAJ MIN PAT, how would a user get these values from an install script?

if [ ! -e $HOME/tmp ]; then
    echo "Home tmp folder not found. Generating home tmp folder."
    mkdir $HOME/tmp
    cd $HOME/tmp
    echo "Saving image in your home tmp folder."
    tar -czf gridlabd-$MAJ\-$MIN\-$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
else
    cd $HOME/tmp
    echo "Saving image in your home tmp folder."
    tar -czf gridlabd-$MAJ\-$MIN\-$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
fi
