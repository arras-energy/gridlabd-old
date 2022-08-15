#!/bin/bash
export PATH=/usr/local/opt/gridlabd/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

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
    brew install pkg-config openssl@1.1 xz gdbm tcl-tk
    xcode-select --install

    cd /usr/local
    sudo mkdir ssl
    sudo ln -sf /usr/local/opt/openssl /usr/local/ssl
    # Update symlinks in /usr/local/bin
    #if [ ! -L "/usr/local/bin" ] && [ -d "/usr/local/bin" ]; then
        #mv /usr/local/bin/* /opt/homebrew/bin
        #sudo ln -sf /opt/homebrew/bin /usr/local/bin
    #fi
    [ ! -e /usr/local/bin/sed ] && sudo ln -sf /usr/local/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && sudo ln -sf /usr/local/bin/glibtoolize /usr/local/bin/libtoolize
    [ ! -e /usr/local/bin/libtool ] && sudo ln -sf /usr/local/bin/glibtool /usr/local/bin/libtool

# Install python 3.9.13
# python3 support needed as of 4.2
if [ ! -x /usr/local/opt/gridlabd/bin/python3 -o "$(/usr/local/opt/gridlabd/bin/python3 --version | cut -f3 -d.)" != "Python 3.9" ]; then
	echo "install python 3.9.6"
	cd /usr/local/opt/gridlabd/src

	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
	# tar xzf Python-3.9.6.tgz 
	cd Python-3.9.6

	./configure --prefix=/usr/local/opt/gridlabd --enable-shared --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"

	make -s -j2
	make install 

	sudo ln -sf /usr/local/opt/gridlabd/bin/python3.9 /usr/local/opt/gridlabd/bin/python3
	sudo ln -sf /usr/local/opt/gridlabd/bin/python3.9-config /usr/local/opt/gridlabd/bin/python3-config
	sudo ln -sf /usr/local/opt/gridlabd/bin/pydoc3.9 /usr/local/opt/gridlabd/bin/pydoc
	sudo ln -sf /usr/local/opt/gridlabd/bin/idle3.9 /usr/local/opt/gridlabd/bin/idle
	sudo ln -sf /usr/local/opt/gridlabd/bin/pip3.9 /usr/local/opt/gridlabd/bin/pip3
    # macos refuses to let me set my all-important library paths. I have to link to /usr/local/lib otherwise the libraries cannot be found.
    # I also need to now add an os-specific script section to the cloud install script. Thanks, apple.
    #if ! test -e /usr/local/lib; then 
    #    sudo mkdir /usr/local/lib
    #    sudo ln -sf /usr/local/opt/gridlabd/lib/* /usr/local/lib
    #else
    #sudo ln -s /usr/local/opt/gridlabd/lib/* /usr/local/lib
    #fix
	/usr/local/opt/gridlabd/bin/python3 -m pip install --upgrade pip
	/usr/local/opt/gridlabd/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
    /usr/local/opt/gridlabd/bin/python3 -m pip install build
    /usr/local/opt/gridlabd/bin/python3 -m pip install pyproj

    # sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/bin/* /usr/local/opt/gridlabd/bin
    # sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/include/* /usr/local/opt/gridlabd/include
    # sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/lib/* /usr/local/opt/gridlabd/lib
    # sudo ln -s /usr/local/opt/gridlabd/Python.framework/Versions/Current/share/* /usr/local/opt/gridlabd/share
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
    cp /usr/local/lib/libgeos* /usr/local/opt/gridlabd/lib

    if test ! -e /usr/local/lib; then
        cd /usr/local
        sudo mkdir lib
    fi

    ln -sf /usr/local/opt/gridlabd/lib/libgeos* /usr/local/lib 

sudo ln -sf /usr/local/opt/gridlabd/bin/* /usr/local/bin
