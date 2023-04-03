#!/bin/bash

# check for Rosetta Homebrew

    if test -e /usr/local/Cellar; then 
        echo "ERROR: Use of Rosetta Homebrew creates package conflicts with arm64-native gridlabd installation in Darwin systems."
        echo "Please install in a system without Rosetta Homebrew, or you can attempt manual installation."
        exit 1
    fi

#!/bin/bash

# VAR is defined in install.sh and exported, to be available here
# Set version and paths, using these vars will make future maintenance much better. #Automation
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VERSION_DIR=$VAR/$VERSION
	PKG_PYTHON_DIR=/usr/local/bin
	VENV_PYTHON_DIR=$VERSION_DIR/bin/pkgenv/bin
    PYTHON_VER=3.10

export PATH=$VERSION_DIR/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Check M1 Homebrew ownership
    if [ -e /opt/homebrew ] ; then
        sudo chown -R ${USER:-root} /opt/homebrew
    fi

# Check if python version currently in Applications and update owner
    if [ -e /Applications/"Python $PYTHON_VER" ] ; then
        sudo chown -R ${USER:-root} /Applications/"Python $PYTHON_VER"
    fi

/opt/homebrew/bin/brew update || /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

/opt/homebrew/bin/brew doctor

# checking for necessary directories

    if test ! -e /usr/local/bin; then 
        cd /usr/local
        sudo mkdir bin
    fi

    if test ! -e /usr/local/lib; then 
        cd /usr/local
        sudo mkdir lib
    fi

    if test ! -e /usr/local/etc; then 
        cd /usr/local
        sudo mkdir etc
    fi


# install homebrew instance for gridlabd
    if test ! -e /opt/homebrew; then 
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
    /opt/homebrew/bin/brew doctor

# build tools

    /opt/homebrew/bin/brew install autoconf automake libtool gnu-sed gawk git
    /opt/homebrew/bin/brew install libffi zlib
    /opt/homebrew/bin/brew install pkg-config xz gdbm tcl-tk
    xcode-select --install

# Update symlinks in /usr/local/bin
    [ ! -e /usr/local/bin/sed ] && sudo ln -sf /opt/homebrew/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && sudo ln -sf /opt/homebrew/bin/glibtoolize /usr/local/bin/libtoolize
    [ ! -e /usr/local/bin/libtool ] && sudo ln -sf /opt/homebrew/bin/glibtool /usr/local/bin/libtool

export DYLD_LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$DYLD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$LD_LIBRARY_PATH
export LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$LIBRARY_PATH

# Install python $PYTHON_VER
# python3 support needed as of 4.2
if [ ! -x /usr/local/bin/python3 ] || [ "$(/usr/local/bin/python3 --version | cut -d' ' -f2 | cut -d. -f1-2)" != "$PYTHON_VER" ]; then
	echo "installing python $PYTHON_VER"
    /opt/homebrew/bin/brew install python@$PYTHON_VER

fi

# check for successful python build
if [ ! -x /usr/local/bin/python3 ]; then
    echo "Could not locate python executable in"
    echo "PYTHON LOCATION: /usr/local/bin/python3"
    echo "Exiting build."
    exit 1
fi

# mdbtools
    /opt/homebrew/bin/brew install mdbtools

# The original scikit-learn at 0.24.2 CANNOT install on the m1 mac. Period. Use 1.1.1 now. Make sure requirements.txt has the change.
# Reason being, is that it requires a version of NumPy that is incompatible with the m1 mac.
# updated in requirements.txt. Same goes for scipy 1.6.2. Use 1.8.1.
    /opt/homebrew/bin/brew install gdal

# docs generators
    /opt/homebrew/bin/brew install mono
    /opt/homebrew/bin/brew install naturaldocs
    sudo ln -sf /opt/homebrew/bin/naturaldocs /usr/local/bin/natural_docs

    /opt/homebrew/bin/brew install doxygen

# influxdb
    /opt/homebrew/bin/brew install influxdb
    /opt/homebrew/bin/brew services start influxdb

# subversion cli
    /opt/homebrew/bin/brew install svn

# libgeos
    /opt/homebrew/bin/brew install geos

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi

sudo ln -sf /opt/homebrew/bin/* /usr/local/bin
sudo ln -sf /opt/homebrew/lib/* /usr/local/lib

cd /usr/local/bin
sudo rm -rf brew
