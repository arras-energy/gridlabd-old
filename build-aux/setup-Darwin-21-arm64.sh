#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# check for Rosetta Homebrew

    if test -e /usr/local/Cellar; then 
        echo "ERROR: Use of Rosetta Homebrew creates package conflicts with arm64-native gridlabd installation in Darwin systems."
        echo "Please install in a system without Rosetta Homebrew, or you can attempt manual installation."
        exit 1
    fi

# install homebrew instance for gridlabd
    brew update
    if test ! -e /opt/homebrew; then 
        sudo mkdir /opt/homebrew 
        sudo chmod 777 /opt/homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C /opt/homebrew
    fi
    export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
    brew update-reset
    brew doctor

# linking bins with arm homebrew versions
sudo ln -sf /opt/homebrew/bin /usr/local/bin
sudo ln -sf /opt/homebrew/lib /usr/local/lib
sudo ln -sf /opt/homebrew/include /usr/local/include
sudo ln -sf /opt/homebrew/var /usr/local/var
sudo ln -sf /opt/homebrew/etc /usr/local/etc
sudo ln -sf /opt/homebrew/share /usr/local/share

echo "$1"
if ! grep -q "$1/bin" "$HOME/.zshrc"; then
    touch "$HOME/.zshrc"
    echo "export PATH=$1/bin:\$PATH" >> $HOME/.zshrc
    echo "export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:\$PATH" >> $HOME/.zshrc
fi

if ! grep -q "$1/bin" "$HOME/.bashrc"; then
    touch "$HOME/.bashrc"
    echo "export PATH=$1/bin:\$PATH" >> $HOME/.bashrc
    echo "export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:\$PATH" >> $HOME/.bashrc
fi

# build tools

    brew install autoconf automake libtool gnu-sed gawk git

    # Update symlinks in the gridlabd bin
    [ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
    [ ! -e /usr/local/bin/sed ] && ln -sf /opt/homebrew/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && ln -sf /opt/homebrew/bin/glibtoolize /usr/local/bin/libtoolize

# install python3
    brew install python3

# mdbtools
    brew install mdbtools
    pip3 install matplotlib pandas numpy networkx Pillow

# The original scikit-learn at 0.24.2 CANNOT install on the m1 mac. Period. Use 1.1.1 now. Make sure requirements.txt has the change.
# Reason being, is that it requires a version of NumPy that is incompatible with the m1 mac.
# updated in requirements.txt. Same goes for scipy 1.6.2.
    brew install gdal

# Install pyproj manually due to error building final wheel
    brew install proj
    pip3 install pyproj

# docs generators
    brew install mono
    brew install naturaldocs
    ln -sf /opt/homebrew/bin/naturaldocs /usr/local/bin/natural_docs

    brew install doxygen

# influxdb
    brew install influxdb
    brew services start influxdb

# subversion cli
    brew install svn

# mysql connector
    brew install mysql
    brew install mysql-client

sudo ln -sf /opt/homebrew/bin /usr/local/bin
sudo ln -sf /opt/homebrew/lib /usr/local/lib
sudo ln -sf /opt/homebrew/include /usr/local/include
sudo ln -sf /opt/homebrew/var /usr/local/var
sudo ln -sf /opt/homebrew/etc /usr/local/etc
sudo ln -sf /opt/homebrew/share /usr/local/share