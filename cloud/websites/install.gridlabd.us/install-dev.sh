#!/bin/bash

############################################################
# Help                                                     #
############################################################
Help()
{
   # Display Help
   echo "Set fast install options"
   echo
   echo "Syntax: ./install.sh [-b|h]"
   echo "options:"
   echo "b     Set the image branch (default master)"
   echo "h     Print this Help."
   echo "v     Manually select the version using #.#.#"
   echo
}

############################################################
############################################################
# Main program                                             #
############################################################
############################################################

# check if gridlabd already exists on the system
if test -e /usr/local/opt/gridlabd; then
    echo "A conflicting gridlabd install already exists on the system in /usr/local/opt/gridlabd"
    echo "Aborting fast install."
    exit 1
fi

# check if necessary tools already installed

DLTOOL=

if which curl >/dev/null ; then
    echo "Downloading via curl."
    DLTOOL=curl
elif which wget >/dev/null ; then
    echo "Downloading via wget."
    DLTOOL=wget
else
    echo "Cannot download, neither wget nor curl is available."
    echo "Please install curl, or wget, to continue."
    exit 1
fi

# Gather system information for correct install

SYSTEM=$(uname -s)
RELEASE=$(uname -r | cut -f1 -d.)
KERNEL=
if test $SYSTEM == "Linux"; then
source "/etc/os-release"
KERNEL="-$ID-$(echo $VERSION_ID | cut -f1 -d.)"
L_DISTRO="$ID"
fi
D_ARCH=$(uname -m)
BRANCH=master
MAJ=
MIN=
PAT=

# check if user has necessary permissions

if [ -f /.docker* ] ; then 
    if [ ! -w /usr/local ]; then
        echo "User is not root. Please run as root user."
        exit 1
    fi

    if test $SYSTEM == "Linux"; then
        apt-get update && apt-get -y install sudo
    fi

fi

############################################################
# Process the input options. Add options as needed.        #
############################################################
# Get the options
while getopts ":hb:v:" option; do
   case $option in
      h) # display Help
         Help
         exit;;
      b) # set branch
         BRANCH=$OPTARG;;
      v) # set version
         if [[ $OPTARG =~ [0-9].[0-9].[0-9] ]] ; then
            MAJ=$(echo $OPTARG | cut -d. -f1)
            MIN=$(echo $OPTARG | cut -d. -f2)
            PAT=$(echo $OPTARG | cut -d. -f3)
            echo "Submitted $MAJ.$MIN.$PAT"
         else 
            echo "Error: Invalid version format submitted"
            exit 1
         fi;;
      \?) # Invalid flag
         echo "Error: Invalid flag set"
         exit;;
   esac
done

# create directories used for install if they don't exist
if test ! -e /usr/local/bin; then
    cd /usr/local
    sudo mkdir bin
fi

if [ ! -e $HOME/temp ]; then
    echo "Home temp folder not found. Generating home temp folder."
    cd $HOME
    mkdir temp
    cd temp
else
    cd $HOME/temp
fi 

# get the version file from the branch to locate correct image version
if [ "$DLTOOL" = "curl" ]; then
    curl -O -J https://raw.githubusercontent.com/slacgismo/gridlabd/$BRANCH/source/version.h
elif [ "$DLTOOL" = "wget" ]; then
    wget https://raw.githubusercontent.com/slacgismo/gridlabd/$BRANCH/source/version.h
fi

if [ ! -f "$HOME/temp/version.h" ]; then
    echo "Invalid branch selected, please submit a valid branch or run as default to install the master branch."
    rm -rf version.h
    exit 1
elif grep -q "404: Not Found" "$HOME/temp/version.h"; then
    echo "Invalid branch selected, please submit a valid branch or run as default to install the master branch."
    rm -rf version.h
    exit 1
fi

# process remaining variables to select correct image
FIL="$HOME/temp/version.h"
if [[ -z "$MAJ" ]]; then
    MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
    MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
    PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
fi
BRA=${BRANCH//-/_}

# Standard universal image install for non-arm systems

if test $D_ARCH != "arm64"; then

    echo "Downloading image to your home temp folder."
    echo "Searching for https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz"
    if [ "$DLTOOL" = "curl" ]; then
        curl -O -J https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz
    elif [ "$DLTOOL" = "wget" ]; then
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz
    fi

    if [ ! -e $HOME/temp/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz ]; then
        echo "A fast install image was not located for your operating system."
        echo "You will need to build Gridlabd from source."
        cd $HOME/temp
        rm -rf version.h
        exit 1
    fi
    tar -xzf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz


    if [ ! -e /usr/local/opt ]; then
        cd /usr/local
        sudo mkdir opt
    fi

    cd $HOME/temp
     sudo mv gridlabd /usr/local/opt
    echo "Gridlabd installed. Adding to path."

    if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.bashrc"; then
        touch "$HOME/.bashrc"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bashrc
        sudo cp /usr/local/opt/gridlabd/bin/gridlabd /usr/local/bin
        echo "Path saved to user's .bashrc file. If you use a different terminal than bash,"
        echo "then you will need to manually add gridlabd to that terminal's startup file."
    fi

    # Linux specific installation customization
    if [ $SYSTEM == "Linux" ]; then
        if [ ! -e /etc/ld.so.conf.d/gridlabd.conf ]; then
            sudo touch /etc/ld.so.conf.d/gridlabd.conf
            sudo bash -c 'echo "/usr/local/opt/gridlabd/gridlabd/current/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
            sudo ldconfig
            echo "Added gridlabd lib to the dynamic loader library."
        fi

        # Add additional system requirements that cannot be saved in the image

        echo "Ensuring additional system dependencies installed."
        sudo apt-get install curl -y
        sudo apt-get install libgdal-dev -y

        if [ $L_DISTRO == "Debian" ]; then
            sudo apt-get install g++ -y
        fi

        if [ $L_DISTRO == "Ubuntu" ]; then
            sudo apt-get install libgeos-dev -y
        fi

    fi

    # give user permissions for writing to site-packages
    sudo chown ${USER} /usr/local/opt/gridlabd/gridlabd/current/lib/python3.9/site-packages

    # Add symlink for binary to /usr/local/bin
    sudo ln -sf /usr/local/opt/gridlabd/bin/gridlabd* /usr/local/bin

# Code for arm64 installations
else

    echo "Downloading image to your home temp folder."
    if [ "$DLTOOL" = "curl" ]; then
        curl -O -J https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz
    elif [ "$DLTOOL" = "wget" ]; then
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz
    fi

    if [ ! -e $HOME/temp/gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz ]; then
        echo "A fast install image was not located for your operating system."
        echo "You will need to build Gridlabd from source."
        cd $HOME/temp
        rm -rf version.h
        exit 1
    fi
    tar -xzf gridlabd-$MAJ\_$MIN\_$PAT-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-$BRA.tarz


    if [ ! -e /usr/local/opt ]; then
        cd /usr/local
        sudo mkdir opt
    fi

    cd $HOME/temp
    sudo mv gridlabd /usr/local/opt
    echo "Gridlabd installed. Adding to path."

    if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.bashrc"; then
        touch "$HOME/.bashrc"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bashrc
        sudo cp /usr/local/opt/gridlabd/bin/gridlabd /usr/local/bin
        echo "Path saved to user's .bashrc file. If you use a different terminal than bash,"
        echo "then you will need to manually add gridlabd to that terminal's startup file."
    fi

    # Linux specific installation customization
    if [ $SYSTEM == "Linux" ]; then
        if [ ! -e /etc/ld.so.conf.d/gridlabd.conf ]; then
            sudo touch /etc/ld.so.conf.d/gridlabd.conf
            sudo bash -c 'echo "/usr/local/opt/gridlabd/gridlabd/current/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
            sudo ldconfig
            echo "Added gridlabd lib to the dynamic loader library."
        fi

        # Add additional system requirements that cannot be saved in the image

        echo "Ensuring additional system dependencies installed."
        sudo apt-get install curl -y
        sudo apt-get install libgdal-dev -y

        if [ $L_DISTRO == "Debian" ]; then
            sudo apt-get install g++ -y
        fi
        
        if [ $L_DISTRO == "Ubuntu" ]; then
            sudo apt-get install libgeos-dev -y
        fi
    fi

    # give user permissions for writing to site-packages
    sudo chown ${USER} /usr/local/opt/gridlabd/gridlabd/current/lib/python3.9/site-packages

    # Add symlink for binary to /usr/local/bin
    sudo ln -sf /usr/local/opt/gridlabd/bin/gridlabd* /usr/local/bin

fi

# Darwin can vary a lot in how their shells add to path. So just hit them all. 
if [ $SYSTEM == "Darwin" ]; then

    if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.zshrc"; then
        touch "$HOME/.zshrc"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.zshrc
    fi

    if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.zsh_profile"; then
        touch "$HOME/.zsh_profile"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.zsh_profile
    fi

    if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.bash_profile"; then
        touch "$HOME/.bash_profile"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bash_profile
    fi

    if ! grep -q "/usr/local/opt/gridlabd/lib" "$HOME/.bashrc"; then
        touch "$HOME/.bashrc"
        echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bashrc
    fi

fi

# link all libraries from package to /usr/local/lib, necessary for darwin and specific packages are hardcoded.
# makes life easier this way.
# for linux, some packages need to be linked to usr/lib and /lib, as necessary.  
if test ! -e /usr/local/lib; then
    cd /usr/local
    sudo mkdir lib
fi

if [ -f /.docker* ] ; then 

    if test $SYSTEM == "Linux"; then
        sudo ln -s /usr/local/opt/gridlabd/lib/* /usr/local/lib
        sudo ln -s /usr/local/opt/gridlabd/lib/x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu
        sudo ln -s /usr/local/opt/gridlabd/lib/r_x86_64-linux-gnu/* /lib/x86_64-linux-gnu

        sudo apt-get install g++ -y
    else
        sudo ln -s /usr/local/opt/gridlabd/lib/* /usr/local/lib
    fi

fi

echo "Cleaning up temporary files"

cd $HOME/temp
sudo rm -rf gridlabd*
sudo rm -rf version.h

if [ ! -e /usr/local/opt/gridlabd/bin/gridlabd ]; then
            echo "A fast install image was not located for your operating system."
            echo "You will need to build Gridlabd from source."
            exit 1
fi

echo "Refresh your terminal or open a new terminal window to begin using gridlabd!"
