#!/bin/bash

# This file is for developer and maintainer use only. 
# Build gridlabd on a clean system for best results in building and saving an image.

if [ ! -e ~/temp]; then
    echo "Home temp folder not found. Generating home temp folder."
    mkdir ~/temp
    cd ~/temp
    echo "Saving image in your home temp folder."
    tar -cjf gridlabd-youros-yourkernel-release-master.tar.bz2 -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
else
    cd ~/temp
    echo "Saving image in your home temp folder."
    tar -cjf gridlabd-youros-yourkernel-release-master.tar.bz2 -C /usr/local/opt .
    echo "Build complete. Don't forget to upload the image to aws!"
fi