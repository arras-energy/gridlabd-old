#!/bin/bash
export PATH=/usr/local/opt/gridlabd/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

VERSION=${VERSION:-`./version.sh --name`}

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
if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.zshrc"; then
    touch "$HOME/.zshrc"
    echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.zshrc
    echo "export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.zshrc
fi

if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.zsh_profile"; then
    touch "$HOME/.zsh_profile"
    echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.zsh_profile
    echo "export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.zsh_profile
fi

if ! grep -q "/usr/local/opt/gridlabd/bin" "$HOME/.bash_profile"; then
    touch "$HOME/.bash_profile"
    echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bash_profile
    echo "export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.bash_profile
    echo "export LD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$LD_LIBRARY_PATH" >> $HOME/.bash_profile
    echo "export LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$LIBRARY_PATH" >> $HOME/.bash_profile
fi

if ! grep -q "/usr/local/opt/gridlabd/lib" "$HOME/.bashrc"; then
    touch "$HOME/.bashrc"
    echo "export PATH=/usr/local/opt/gridlabd/bin:\$PATH" >> $HOME/.bashrc
    echo "export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$DYLD_LIBRARY_PATH" >> $HOME/.bashrc
    echo "export LD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$LD_LIBRARY_PATH" >> $HOME/.bashrc
    echo "export LIBRARY_PATH=/usr/local/opt/gridlabd/lib:\$LIBRARY_PATH" >> $HOME/.bashrc
fi

export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$DYLD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$LIBRARY_PATH
export LIBRARY_PATH=/usr/local/opt/gridlabd/lib:$LIBRARY_PATH

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
if [ ! -x /usr/local/opt/gridlabd/bin/python3 -o "$(/usr/local/opt/gridlabd/bin/python3 --version | cut -f3 -d.)" != "Python 3.9" ]; then
	echo "install python 3.9.6"
	cd /usr/local/opt/gridlabd/src

	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz

# include python ssl module and dependencies, uses ./Configure instead of ./configure due to custom implementation    
    curl -L http://xrl.us/installperlosx | bash
    curl https://www.openssl.org/source/openssl-1.1.1q.tar.gz | tar xz
    cd openssl-1.1.1q
# 1.1.1q has an issue where one test is missing a header. Remove no-tests with the next release, as that should have the missing header patched in.
    ./Configure --prefix=/usr/local/opt/gridlabd --openssldir=/usr/local/opt/gridlabd/ssl darwin64-x86_64-cc no-tests
    make
    make install
	# tar xzf Python-3.9.6.tgz 
	cd /usr/local/opt/gridlabd/src/Python-3.9.6

    export MACOSX_DEPLOYMENT_TARGET=10.15
     export PKG_CONFIG_PATH="/usr/local/opt/gridlabd/lib/pkgconfig:/usr/local/opt/tcl-tk/lib/pkgconfig:$pythonLocation/lib/pkgconfig"
	./configure --prefix=/usr/local/opt/gridlabd/gridlabd/$VERSION \
    --enable-framework=/usr/local/opt/gridlabd \
    --with-openssl=/usr/local/opt/gridlabd/opt/openssl@1.1 \
    --with-pydebug \
    --with-computed-gotos \
    --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    --with-tcltk-includes="$(pkg-config --cflags tcl tk)" \
    CFLAGS="-I/usr/local/include " \
    LDFLAGS="-L/usr/local/lib -L/usr/local/opt/zlib/lib" \
    CPPFLAGS="-I/usr/local/include -I/usr/local/opt/zlib/include"

	make -s -j2
	make install 

	sudo ln -sf /usr/local/opt/gridlabd/bin/python3.9 /usr/local/opt/gridlabd/bin/python3
	sudo ln -sf /usr/local/opt/gridlabd/bin/python3.9d-config /usr/local/opt/gridlabd/bin/python3-config
	sudo ln -sf /usr/local/opt/gridlabd/bin/pydoc3.9 /usr/local/opt/gridlabd/bin/pydoc
	sudo ln -sf /usr/local/opt/gridlabd/bin/idle3.9 /usr/local/opt/gridlabd/bin/idle
    sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/3.9/bin/* /usr/local/opt/gridlabd/bin
    # macos refuses to let me set my all-important library paths. I have to link to /usr/local/lib otherwise the libraries cannot be found.
    # I also need to now add an os-specific script section to the cloud install script. Thanks, apple.
    #if ! test -e /usr/local/lib; then 
    #    sudo mkdir /usr/local/lib
    #    sudo ln -sf /usr/local/opt/gridlabd/lib/* /usr/local/lib
    #else
    #sudo ln -s /usr/local/opt/gridlabd/lib/* /usr/local/lib
    #fix
    /usr/local/opt/gridlabd/bin/python3 -m ensurepip --upgrade
	/usr/local/opt/gridlabd/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
    /usr/local/opt/gridlabd/bin/python3 -m pip install build
    /usr/local/opt/gridlabd/bin/python3 -m pip install pyproj

    sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/bin/* /usr/local/opt/gridlabd/bin
    sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/include/* /usr/local/opt/gridlabd/include
    sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/lib/* /usr/local/opt/gridlabd/lib
    sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/share/* /usr/local/opt/gridlabd/share
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
    cp /usr/local/opt/geos/lib/libgeos* /usr/local/opt/gridlabd/lib

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi

    ln -sf /usr/local/opt/gridlabd/lib/libgeos* /usr/local/lib 

sudo ln -sf /usr/local/opt/gridlabd/bin/* /usr/local/bin
