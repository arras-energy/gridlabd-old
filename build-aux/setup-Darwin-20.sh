#!/bin/bash

# VAR is defined in install.sh and exported, to be available here
# Set version and paths, using these vars will make future maintenance much better. #Automation
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VERSION_DIR=$VAR/$VERSION
	PKG_PYTHON_DIR=/usr/local/bin
	VENV_PYTHON_DIR=$VERSION_DIR/bin/pkgenv/bin
    PYTHON_VER=3.10

export PATH=$VERSION_DIR/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Check if python version currently in Applications and update owner
    if [ -e /Applications/"Python $PYTHON_VER" ] ; then
        sudo chown -R ${USER:-root} /Applications/"Python $PYTHON_VER"
    fi

brew update || /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew doctor

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

export DYLD_LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$DYLD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$LD_LIBRARY_PATH
export LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:$LIBRARY_PATH


# build tools

    brew install autoconf automake libtool gnu-sed gawk git
    brew install libffi zlib
    brew install pkg-config xz gdbm tcl-tk
    xcode-select --install

# Update symlinks in /usr/local/bin
    [ ! -e /usr/local/bin/sed ] && sudo ln -sf /usr/local/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && sudo ln -sf /usr/local/bin/glibtoolize /usr/local/bin/libtoolize
    [ ! -e /usr/local/bin/libtool ] && sudo ln -sf /usr/local/bin/glibtool /usr/local/bin/libtool

# Install python $PYTHON_VER
# python3 support needed as of 4.2
if [ ! -x /usr/local/bin/python3 ] || [ "$(/usr/local/bin/python3 --version | cut -d' ' -f2 | cut -d. -f1-2)" != "$PYTHON_VER" ]; then
	echo "installing python $PYTHON_VER and ssl module dependencies"

# needed for SSL module to make proper connections, as openssl does not actually provide the certificates.
    brew reinstall openssl@1.1
    brew install ca-certificates
    CERT_DIR="$(echo /usr/local/Cellar/ca-certificates)"
    unset -v latest
    for file in "$CERT_DIR"/* ; do
        [[ $file -nt $latest ]] && latest=$file
    done
    CERT_DIR=$latest
    cp $CERT_DIR/share/ca-certificates/* /usr/local/etc/openssl@1.1/certs/cert.pem

    echo "installing python $PYTHON_VER"
    brew install python@$PYTHON_VER

fi

# check for successful python build
if [ ! -x /usr/local/bin/python$PYTHON_VER ]; then
    echo "Could not locate python executable in"
    echo "PYTHON LOCATION: /usr/local/bin/python$PYTHON_VER"
    echo "Exiting build."
    exit 1
fi

brew install gdal

# docs generators
    brew install mono
    brew install naturaldocs
    sudo ln -sf /usr/local/bin/naturaldocs /usr/local/bin/natural_docs

    brew install doxygen

# influxdb
    brew install influxdb
    brew services start influxdb

# subversion cli
    brew install svn

# libgeos
    brew install geos

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi
