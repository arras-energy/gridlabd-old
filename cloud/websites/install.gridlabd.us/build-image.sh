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

if [ $SYSTEM == "Darwin" ]; then
    cp /opt/homebrew/lib/lib* /usr/local/opt/gridlabd/lib
    cp /opt/homebrew/opt/*/lib/lib*.* /usr/local/opt/gridlabd/lib
fi

if [ ! -e $HOME/temp ]; then
    echo "Home temp folder not found. Generating home temp folder."
    mkdir $HOME/temp
    cd $HOME/temp
    echo "Saving image in your home temp folder."
    tar -cjf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2 -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
else
    cd $HOME/temp
    echo "Saving image in your home temp folder."
    tar -cjf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2 -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
fi
