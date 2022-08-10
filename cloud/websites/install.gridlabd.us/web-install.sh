#!/bin/bash

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

if test ! -e /usr/local/bin; then
    cd /usr/local
    sudo mkdir bin
fi

# Standard universal image install for non-arm systems

if test $D_ARCH != "arm64"; then

    # Darwin specific setup for necessary tools
    if [ $SYSTEM == "Darwin" ]; then
        brew update || /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        brew doctor

        brew install wget
    fi

    if [ ! -e $HOME/temp ]; then
        echo "Home temp folder not found. Generating home temp folder."
        cd $HOME
        mkdir temp
        cd temp
        echo "Downloading image to your home temp folder."
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2

        if [ ! -e $HOME/temp/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2 ]; then
            echo "A fast install image was not located for your operating system."
            echo "You will need to build Gridlabd from source."
            exit 1
        fi
        tar -xvf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2


        if [ ! -e /usr/local/opt ]; then
            cd /usr/local
            sudo mkdir opt
        fi

        cd $HOME/temp
        sudo mv gridlabd /usr/local/opt
        echo "Gridlabd installed. Adding to path."
    else
        cd $HOME/temp
        echo "Downloading image to your home temp folder."
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2
        tar -xvf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2

        if [ ! -e /usr/local/opt ]; then
            cd /usr/local
            sudo mkdir opt
        fi

        cd $HOME/temp
        sudo mv gridlabd /usr/local/opt
        echo "Gridlabd installed. Adding to path."
    fi

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
            sudo bash -c 'echo "/usr/local/opt/gridlabd/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
            sudo ldconfig
            echo "Added gridlabd lib to the dynamic loader library."
        fi

        # Add additional system requirements that cannot be saved in the image

        echo "Ensuring additional system dependencies installed."
        sudo apt-get install curl -y

        if [ $L_DISTRO == "Debian" ]; then
            sudo apt-get install g++ -y
        fi

        if [ $L_DISTRO == "Ubuntu" ]; then
            sudo apt-get install libgeos-dev -y
        fi

    fi

    # give user permissions for writing to site-packages
    sudo chown ${USER} /usr/local/opt/gridlabd/lib/python3.9/site-packages

    # Add symlink for binary to /usr/local/bin
    sudo ln -sf /usr/local/opt/gridlabd/bin/gridlabd* /usr/local/bin

# Code for arm64 installations
else

    # Darwin specific setup for necessary tools
    if [ $SYSTEM == "Darwin" ]; then
        brew update
        if test ! -e /opt/homebrew; then 
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        fi
        export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
        brew update-reset
        brew doctor

        brew install wget
        # TODO - update more as needed
    fi

    if [ ! -e $HOME/temp ]; then
        echo "Home temp folder not found. Generating home temp folder."
        cd $HOME
        mkdir temp
        cd temp
        echo "Downloading image to your home temp folder."
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2

        if [ ! -e $HOME/temp/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2 ]; then
            echo "A fast install image was not located for your operating system."
            echo "You will need to build Gridlabd from source."
            exit 1
        fi
        tar -xvf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2


        if [ ! -e /usr/local/opt ]; then
            cd /usr/local
            sudo mkdir opt
        fi

        cd $HOME/temp
        sudo mv gridlabd /usr/local/opt
        echo "Gridlabd installed. Adding to path."
    else
        cd $HOME/temp
        echo "Downloading image to your home temp folder."
        wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2
        tar -xvf gridlabd-$SYSTEM$KERNEL-$RELEASE-$D_ARCH-master.tar.bz2

        if [ ! -e /usr/local/opt ]; then
            cd /usr/local
            sudo mkdir opt
        fi

        cd $HOME/temp
        sudo mv gridlabd /usr/local/opt
        echo "Gridlabd installed. Adding to path."
    fi

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
            sudo bash -c 'echo "/usr/local/opt/gridlabd/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
            sudo ldconfig
            echo "Added gridlabd lib to the dynamic loader library."
        fi

        # Add additional system requirements that cannot be saved in the image

        echo "Ensuring additional system dependencies installed."
        sudo apt-get install curl -y

        if [ $L_DISTRO == "Debian" ]; then
            sudo apt-get install g++ -y
        fi
        
        if [ $L_DISTRO == "Ubuntu" ]; then
            sudo apt-get install libgeos-dev -y
        fi
    fi

    # give user permissions for writing to site-packages
    sudo chown ${USER} /usr/local/opt/gridlabd/lib/python3.9/site-packages

    # Add symlink for binary to /usr/local/bin
    sudo ln -sf /usr/local/opt/gridlabd/bin/gridlabd* /usr/local/bin

fi

if [ $SYSTEM == "Darwin" ]; then
    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi

    sudo ln -sf /usr/local/opt/gridlabd/lib/libgeos* /usr/local/lib 

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

cd $HOME/temp
sudo rm -rf gridlabd*

if [ ! -e /usr/local/opt/gridlabd/bin/gridlabd ]; then
            echo "A fast install image was not located for your operating system."
            echo "You will need to build Gridlabd from source."
            exit 1
fi