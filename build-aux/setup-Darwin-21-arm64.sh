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
    PYTHON_DIR=Python.framework/Versions/Current
    PYTHON_VER=3.9.13
    PY_EXE=3.9

export PATH=$VERSION_DIR/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Check M1 Homebrew ownership
    if [ -e /opt/homebrew ] ; then
        sudo chown -R ${USER:-root} /opt/homebrew
    fi

# Check if python version currently in Applications and update owner
    if [ -e /Applications/"Python $PY_EXE" ] ; then
        sudo chown -R ${USER:-root} /Applications/"Python $PY_EXE"
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

export DYLD_LIBRARY_PATH=$VERSION_DIR/lib:$DYLD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VERSION_DIR/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$VERSION_DIR/lib:$LIBRARY_PATH

# install homebrew instance for gridlabd
    brew update
    if test ! -e /opt/homebrew; then 
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
    brew update-reset
    brew doctor

# build tools

    brew install autoconf automake libtool gnu-sed gawk git
    brew install libffi zlib
    brew install pkg-config xz gdbm tcl-tk
    xcode-select --install

# Update symlinks in /usr/local/bin
    [ ! -e /usr/local/bin/sed ] && sudo ln -sf /opt/homebrew/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && sudo ln -sf /opt/homebrew/bin/glibtoolize /usr/local/bin/libtoolize
    [ ! -e /usr/local/bin/libtool ] && sudo ln -sf /opt/homebrew/bin/glibtool /usr/local/bin/libtool


# Install python $PYTHON_VER
# python3 support needed as of 4.2
if [ ! -x $VERSION_DIR/bin/python3 -o "$($VERSION_DIR/bin/python3 --version | cut -f3 -d.)" != "Python $PY_EXE" ]; then
	echo "installing python $PYTHON_VER and ssl module dependencies"
	cd $VERSION_DIR/src

	curl https://www.python.org/ftp/python/$PYTHON_VER/Python-$PYTHON_VER.tgz | tar xz

# include python ssl module and dependencies, uses ./Configure instead of ./configure due to custom implementation    
    curl -L http://xrl.us/installperlosx | bash
    curl https://www.openssl.org/source/old/1.1.1/openssl-1.1.1n.tar.gz | tar xz
    cd openssl-1.1.1n

# Needed to build python's ssl module
    ./Configure --prefix=$VERSION_DIR/openssl --openssldir=$VERSION_DIR/ssl --libdir=lib darwin64-arm64-cc
    make
    make install

# needed for SSL module to make proper connections, as openssl does not actually provide the certificates.
    brew install ca-certificates
    CERT_DIR="$(echo /opt/homebrew/Cellar/ca-certificates)"
    unset -v latest
    for file in "$CERT_DIR"/*; do
        [[ $file -nt $latest ]] && latest=$file
    done
    CERT_DIR=$latest
    cp $CERT_DIR/share/ca-certificates/* $VERSION_DIR/ssl/cert.pem

    if [ ! -f $VERSION_DIR/ssl/cert.pem ] ; then
        echo "The installer was unable to locate certificates for the python build"
        echo "Please make sure homebrew can install ca-certificates before retrying build."
        exit 1
    fi

	# tar xzf Python-$PYTHON_VER.tgz
	cd $VERSION_DIR/src/Python-$PYTHON_VER

    export MACOSX_DEPLOYMENT_TARGET=$( sw_vers -productVersion | sed 's/..$//' )
    # export PKG_CONFIG_PATH="/opt/homebrew/opt/tcl-tk/lib/pkgconfig"
    export PKG_CONFIG_PATH="$VERSION_DIR/lib/pkgconfig:/opt/homebrew/opt/tcl-tk/lib/pkgconfig:$pythonLocation/lib/pkgconfig"
	./configure --prefix=$VERSION_DIR \
    --enable-framework=$VERSION_DIR \
    --with-openssl=$VERSION_DIR/openssl \
    --with-pydebug \
    --with-computed-gotos \
    --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    --with-tcltk-includes="$(pkg-config --cflags tcl tk)" \
    CFLAGS="-I/opt/homebrew/include " \
    LDFLAGS="-L/opt/homebrew/lib -L/opt/homebrew/opt/zlib/lib" \
    CPPFLAGS="-I/opt/homebrew/include -I/opt/homebrew/opt/zlib/include"
    # --enable-shared \ 
    # --with-universal-archs=arm64 \
    # --with-gxx-include-dir=/Library/Developer/CommandLineTools/SDKs/MacOSX12.0.sdk/usr/include/c++/v1
    # --with-system-ffi \
    

    # --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    # --with-tcltk-includes="$(pkg-config --cflags tcl tk)"
    
    

    # MAKEFLAGS need to be set to an empty string to prevent a bug with framework builds. Multiprocessor python framework builds fail when a -j value is set.
    export MAKEFLAGS=""
	make
	make install

	sudo ln -s $VERSION_DIR/$PYTHON_DIR/bin/python${PY_EXE}d $VERSION_DIR/$PYTHON_DIR/bin/python3
    $VERSION_DIR/$PYTHON_DIR/bin/python${PY_EXE}d -m ensurepip --upgrade

	sudo ln -sf $VERSION_DIR/$PYTHON_DIR/bin/python${PY_EXE}d-config $VERSION_DIR/$PYTHON_DIR/bin/python3-config
	sudo ln -sf $VERSION_DIR/$PYTHON_DIR/bin/pydoc${PY_EXE} $VERSION_DIR/$PYTHON_DIR/bin/pydoc
	sudo ln -sf $VERSION_DIR/$PYTHON_DIR/bin/idle${PY_EXE} $VERSION_DIR/$PYTHON_DIR/bin/idle
    sudo ln -sf $VERSION_DIR/$PYTHON_DIR/bin/* $VERSION_DIR/bin

    $VERSION_DIR/bin/python3 -m ensurepip --upgrade
	$VERSION_DIR/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
    $VERSION_DIR/bin/python3 -m pip install build
    $VERSION_DIR/bin/python3 -m pip install pyproj

    sudo ln -sf $VERSION_DIR/$PYTHON_DIR/bin/* $VERSION_DIR/bin
    sudo ln -sf $VERSION_DIR/$PYTHON_DIR/include/* $VERSION_DIR/include
    sudo ln -sf $VERSION_DIR/$PYTHON_DIR/lib/* $VERSION_DIR/lib
    sudo ln -sf $VERSION_DIR/$PYTHON_DIR/share/* $VERSION_DIR/share
fi

# check for successful python build
if [ ! -x $VERSION_DIR/$PYTHON_DIR/bin/python${PY_EXE}d ]; then
    echo "Could not locate python executable in"
    echo "PYTHON LOCATION: $VERSION_DIR/$PYTHON_DIR/bin/python${PY_EXE}d"
    echo "Exiting build."
    exit 1
fi

# mdbtools
    brew install mdbtools

# The original scikit-learn at 0.24.2 CANNOT install on the m1 mac. Period. Use 1.1.1 now. Make sure requirements.txt has the change.
# Reason being, is that it requires a version of NumPy that is incompatible with the m1 mac.
# updated in requirements.txt. Same goes for scipy 1.6.2. Use 1.8.1.
    brew install gdal

# docs generators
    brew install mono
    brew install naturaldocs
    sudo ln -sf /opt/homebrew/bin/naturaldocs /usr/local/bin/natural_docs

    brew install doxygen

# influxdb
    brew install influxdb
    brew services start influxdb

# subversion cli
    brew install svn

# libgeos
    brew install geos
    cp /opt/homebrew/lib/libgeos* $VERSION_DIR/lib

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi

    ln -sf $VERSION_DIR/gridlabd/lib/libgeos* /usr/local/lib 

# mysql connector
#    brew install mysql
#    brew install mysql-client

sudo ln -s /opt/homebrew/bin/* /usr/local/bin
sudo ln -s /opt/homebrew/etc/* /usr/local/etc
sudo ln -s /opt/homebrew/lib/* /usr/local/lib

cd /usr/local/bin
sudo rm -rf brew
