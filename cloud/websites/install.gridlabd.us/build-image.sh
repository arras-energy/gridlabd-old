#!/bin/bash

# This file is for developer and maintainer use only. 
# Build gridlabd on a clean system for best results in building and saving an image.
# this file is currently built to be run from within the same folder.
# It could certainly be cleaned up a little, but works just fine as is. 

SYSTEM=$(uname -s)
RELEASE=$(uname -r | cut -f1 -d.)
KERNEL=
if test $SYSTEM == "Linux"; then
source "/etc/os-release"
KERNEL="-$ID-$(echo $VERSION_ID | cut -f1 -d.)"
fi
D_ARCH=$(uname -m)
EXE=$0

if [ "$(basename $EXE)" != "install.gridlabd.us" ] ; then
    echo "$EXE"
    echo "Please run this script from within the install.gridlabd.us directory."
    exit 1
else
    echo "$EXE else"
    reqdir="$(dirname $EXE)"
    reqdir="$(dirname $reqdir)"
    reqdir="$(dirname $reqdir)"
    cp $reqdir/requirements.txt /usr/local/opt/gridlabd/src
fi

cd ../../..
TOP=$(pwd)
FIL="${TOP}/source/version.h"
MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
BRA=`git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$//;s/^_//'`


if [ $SYSTEM == "Darwin" ]; then
    if [ D_ARCH = "arm64" ]; then
        cp /opt/homebrew/lib/lib* /usr/local/opt/gridlabd/lib
        cp /opt/homebrew/opt/*/lib/lib*.* /usr/local/opt/gridlabd/lib
    else
        cp /usr/local/lib/lib* /usr/local/opt/gridlabd/lib
    fi
fi

if [ $SYSTEM == "Linux" ]; then
    if [ -f /.docker* ]; then 
        cp -r /usr/lib/* /usr/local/opt/gridlabd/lib
        cd /usr/local/opt/gridlabd/lib 
        rm -rf apt  dpkg  locale  mime  os-release  ssl  sudo	tmpfiles.d  udev
        cp -r /lib/x86_64-linux-gnu /usr/local/opt/gridlabd/lib/r_x86_64-linux-gnu
    else
        echo "Linux images should only be built from clean docker containers for maximum portability!"
        exit 1
    fi
fi

#bz2 may be a better compression, but tarz is more universal. Confirmed bz2 unpacking issues when used in a fresh docker debian container.

if [ ! -e $HOME/temp ]; then
    echo "Home temp folder not found. Generating home temp folder."
    mkdir $HOME/temp
    cd $HOME/temp
    echo "Saving image in your home temp folder."
    tar -czf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
else
    cd $HOME/temp
    echo "Saving image in your home temp folder."
    tar -czf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
fi