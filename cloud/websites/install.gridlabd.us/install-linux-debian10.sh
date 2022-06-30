#!/bin/bash

# This file is for developer and maintainer use only. 
# Build gridlabd on a clean system for best results in building and saving an image.

if [ ! -e ~/temp]; then
    echo "Home temp folder not found. Generating home temp folder."
    mkdir ~/temp
    cd ~/temp
    echo "Downloading image to your home temp folder."
    wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/debian-10-image.tar.bz2
    tar -xvf debian-10-image.tar.bz2
    sudo mv gridlabd /usr/local/opt
    echo "Gridlabd installed. Adding to path."
else
    cd ~/temp
    echo "Downloading image to your home temp folder."
    wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/debian-10-image.tar.bz2
    tar -xvf debian-10-image.tar.bz2
    sudo mv gridlabd /usr/local/opt
    echo "Gridlabd installed. Adding to path."
fi

if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.bashrc"; then
    touch "$HOME/.bashrc"
    echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bashrc
    echo "Path saved to user's .bashrc file. If you use a different terminal than bash,"
    echo "then you will need to manually add gridlabd to that terminal's startup file."
fi

if [ ! -e /etc/ld.so.conf.d/gridlabd.conf ]; then
	sudo touch /etc/ld.so.conf.d/gridlabd.conf
	sudo bash -c 'echo "/usr/local/opt/gridlabd/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
	sudo ldconfig
    echo "Added gridlabd lib to the dynamic loader library."
fi

# Add additional system requirements that cannot be saved in the image

echo "Ensuring additional system dependencies installed."
apt-get install curl -y

# give user permissions for writing to site-packages
# sudo chown ${USER:-root} /usr/local/opt/gridlabd/lib/python3.9/site-packages