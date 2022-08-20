#!/bin/bash
export PATH=/usr/local/opt/gridlabd/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Set version
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VAR="/usr/local/opt/gridlabd"

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

# Install python 3.9.6
# python3 support needed as of 4.2
if [ ! -x $VAR/gridlabd/$VERSION/bin/python3 -o "$($VAR/gridlabd/$VERSION/bin/python3 --version | cut -f3 -d.)" != "Python 3.9" ]; then
	echo "installing python 3.9.6 and ssl module dependencies"
	cd $VAR/src

	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz

# include python ssl module and dependencies, uses ./Configure instead of ./configure due to custom implementation    
    curl -L http://xrl.us/installperlosx | bash
    curl https://www.openssl.org/source/old/1.1.1/openssl-1.1.1n.tar.gz | tar xz
    cd openssl-1.1.1n

# Needed to build python's ssl module
    ./Configure --prefix=$VAR/gridlabd/$VERSION/openssl --openssldir=$VAR/gridlabd/$VERSION/ssl --libdir=lib darwin64-x86_64-cc
    make
    make install

# needed for SSL module to make proper connections, as openssl does not actually provide the certificates.
    brew install ca-certificates
    cp /usr/local/Cellar/ca-certificates/2022-07-19_1/share/ca-certificates/* $VAR/gridlabd/$VERSION/ssl/cert.pem

	# tar xzf Python-3.9.6.tgz
	cd $VAR/src/Python-3.9.6

    export MACOSX_DEPLOYMENT_TARGET=11.6
     export PKG_CONFIG_PATH="$VAR/gridlabd/$VERSION/lib/pkgconfig:/usr/local/opt/tcl-tk/lib/pkgconfig:$pythonLocation/lib/pkgconfig"
	./configure --prefix=$VAR/gridlabd/$VERSION \
    --enable-framework=$VAR/gridlabd/$VERSION \
    --with-openssl=$VAR/gridlabd/$VERSION/openssl \
    --with-pydebug \
    --with-computed-gotos \
    --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    --with-tcltk-includes="$(pkg-config --cflags tcl tk)" \
    CFLAGS="-I/usr/local/include " \
    LDFLAGS="-L/usr/local/lib -L/usr/local/opt/zlib/lib" \
    CPPFLAGS="-I/usr/local/include -I/usr/local/opt/zlib/include"

	make -s -j2
	make install

	sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/python3.9d $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/python3
    $VAR/gridlabd/$VERSION/bin/python3 -m ensurepip --upgrade

	sudo ln -sf $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/python3.9d-config $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/python3-config
	sudo ln -sf $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/pydoc3.9 $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/pydoc
	sudo ln -sf $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/idle3.9 $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/idle
    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/* $VAR/gridlabd/$VERSION/bin
    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/* $VAR/bin

    $VAR/gridlabd/$VERSION/bin/python3 -m ensurepip --upgrade
	$VAR/gridlabd/$VERSION/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
    $VAR/gridlabd/$VERSION/bin/python3 -m pip install build
    $VAR/gridlabd/$VERSION/bin/python3 -m pip install pyproj

    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/bin/* $VAR/bin
    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/include/* $VAR/include
    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/lib/* $VAR/lib
    sudo ln -s $VAR/gridlabd/$VERSION/Python.framework/Versions/Current/share/* $VAR/share
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

    ln -sf $VAR/lib/libgeos* /usr/local/lib 

sudo ln -sf $VAR/bin/* /usr/local/bin
