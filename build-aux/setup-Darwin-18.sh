#!/bin/bash
export PATH=/usr/local/opt/gridlabd/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Set version and paths, using these vars will make future maintenance much better. #Automation
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VAR="/usr/local/opt/gridlabd"
    VERSION_DIR=$VAR/gridlabd/$VERSION
    PYTHON_DIR=Python.framework/Versions/Current
    PYTHON_VER=3.9.6
    PY_EXE=3.9

brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
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

# adding necessary paths to user bash and zsh terminals
# apparently, which profile or rc file is used varies wildly across Macs. RIP me. Add to all. =')
if ! grep -q "$VAR/bin" "$HOME/.zshrc"; then
    touch "$HOME/.zshrc"
    echo "export PATH=$VAR/bin:\$PATH" >> $HOME/.zshrc
    echo "export DYLD_LIBRARY_PATH=$VAR/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.zshrc
fi

if ! grep -q "$VAR/bin" "$HOME/.zsh_profile"; then
    touch "$HOME/.zsh_profile"
    echo "export PATH=$VAR/bin:\$PATH" >> $HOME/.zsh_profile
    echo "export DYLD_LIBRARY_PATH=$VAR/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.zsh_profile
fi

if ! grep -q "$VAR/bin" "$HOME/.bash_profile"; then
    touch "$HOME/.bash_profile"
    echo "export PATH=$VAR/bin:\$PATH" >> $HOME/.bash_profile
    echo "export DYLD_LIBRARY_PATH=$VAR/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.bash_profile
    echo "export LD_LIBRARY_PATH=$VAR/lib:\$LD_LIBRARY_PATH" >> $HOME/.bash_profile
    echo "export LIBRARY_PATH=$VAR/lib:\$LIBRARY_PATH" >> $HOME/.bash_profile
fi

if ! grep -q "$VAR/lib" "$HOME/.bashrc"; then
    touch "$HOME/.bashrc"
    echo "export PATH=$VAR/bin:\$PATH" >> $HOME/.bashrc
    echo "export DYLD_LIBRARY_PATH=$VAR/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.bashrc
    echo "export LD_LIBRARY_PATH=$VAR/lib:\$LD_LIBRARY_PATH" >> $HOME/.bashrc
    echo "export LIBRARY_PATH=$VAR/lib:\$LIBRARY_PATH" >> $HOME/.bashrc
fi

export DYLD_LIBRARY_PATH=$VAR/lib:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$VAR/lib:$DYLD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VAR/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VAR/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$VAR/lib:$LIBRARY_PATH
export LIBRARY_PATH=$VAR/lib:$LIBRARY_PATH

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
if [ ! -x $VERSION_DIR/bin/python3 -o "$($VERSION_DIR/bin/python3 --version | cut -f3 -d.)" != "Python $PY_EXE" ]; then
	echo "installing python $PYTHON_VER and ssl module dependencies"
	cd $VERSION_DIR/src

	curl https://www.python.org/ftp/python/$PYTHON_VER/Python-$PYTHON_VER.tgz | tar xz

# include python ssl module and dependencies, uses ./Configure instead of ./configure due to custom implementation    
    curl -L http://xrl.us/installperlosx | bash
    curl https://www.openssl.org/source/old/1.1.1/openssl-1.1.1n.tar.gz | tar xz
    cd openssl-1.1.1n

# Needed to build python's ssl module
    ./Configure --prefix=$VERSION_DIR/openssl --openssldir=$VERSION_DIR/ssl --libdir=lib darwin64-x86_64-cc
    make
    make install

# needed for SSL module to make proper connections, as openssl does not actually provide the certificates.
    brew install ca-certificates
    cp /usr/local/Cellar/ca-certificates/2022-07-19_1/share/ca-certificates/* $VERSION_DIR/ssl/cert.pem

	# tar xzf Python-$PYTHON_VER.tgz
	cd $VERSION_DIR/src/Python-$PYTHON_VER

    export MACOSX_DEPLOYMENT_TARGET=$( sw_vers -productVersion | sed 's/..$//' )
     export PKG_CONFIG_PATH="$VERSION_DIR/lib/pkgconfig:/usr/local/opt/tcl-tk/lib/pkgconfig:$pythonLocation/lib/pkgconfig"
	./configure --prefix=$VERSION_DIR \
    --enable-framework=$VERSION_DIR \
    --with-openssl=$VERSION_DIR/openssl \
    --with-pydebug \
    --with-computed-gotos \
    --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    --with-tcltk-includes="$(pkg-config --cflags tcl tk)" \
    CFLAGS="-I/usr/local/include " \
    LDFLAGS="-L/usr/local/lib -L/usr/local/opt/zlib/lib" \
    CPPFLAGS="-I/usr/local/include -I/usr/local/opt/zlib/include"

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
    cp /usr/local/opt/geos/lib/libgeos* $VAR/lib

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi
