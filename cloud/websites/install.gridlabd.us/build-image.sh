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
EXE=$(pwd)

if [ "$(basename $EXE)" != "install.gridlabd.us" ] ; then
    echo "Please run this script from within the install.gridlabd.us directory."
    exit 1
else
    reqdir="$(dirname $EXE)"
    reqdir="$(dirname $reqdir)"
    reqdir="$(dirname $reqdir)"
fi

cd ../../..
TOP=$(pwd)
FIL="${TOP}/source/version.h"
MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
BRA=`git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$//;s/^_//'`

VERSION_LOC=$($TOP/build-aux/version.sh --name)

# local folder
VAR="/usr/local/opt/gridlabd"
VERSION_DIR=$VAR/$VERSION_LOC
if [ ! -e $VERSION_DIR/src ] ; then
    mkdir $VERSION_DIR/src
fi
cp $reqdir/requirements.txt $VERSION_DIR/src/

if [ ! -e $VERSION_DIR/lib/locallib ] ; then
    mkdir $VERSION_DIR/lib/locallib
fi

if [ ! -e $VERSION_DIR/lib/usrlib ] ; then
    mkdir $VERSION_DIR/lib/usrlib
fi

if [ ! -e $VERSION_DIR/lib/rootlib ] ; then
    mkdir $VERSION_DIR/lib/rootlib
fi

# Darwin apparently expects a capital -R and not lowercase. Apple always has to be special.
if [ $SYSTEM == "Darwin" ]; then
    if [ D_ARCH = "arm64" ]; then
        cp -R /opt/homebrew/lib/lib* $VERSION_DIR/lib/locallib
        cp -R /opt/homebrew/opt/*/lib/lib*.* $VERSION_DIR/lib/locallib
    else
        cp -R /usr/local/lib/lib* $VERSION_DIR/lib/locallib
    fi
fi

if [ $SYSTEM == "Linux" ]; then
    if [ -f /.docker* ]; then 
        cp -r /usr/lib/* $VERSION_DIR/lib/usrlib
        cd $VERSION_DIR/lib/usrlib 
        rm -rf apt  dpkg  locale  mime  os-release  ssl  sudo	tmpfiles.d  udev
        cp -r /lib/x86_64-linux-gnu $VERSION_DIR/lib/rootlib
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
    tar -czf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C $VAR .
    echo "Build complete. Don't forget to upload the image to aws!"
else
    cd $HOME/temp
    echo "Saving image in your home temp folder."
    tar -czf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz -C $VAR .
    echo "Build complete. Don't forget to upload the image to aws!"
fi
