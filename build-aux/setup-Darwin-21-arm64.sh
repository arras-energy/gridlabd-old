#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
REQ_DIR=$(pwd)
# check for Rosetta Homebrew

    if test -e /usr/local/Cellar; then 
        echo "ERROR: Use of Rosetta Homebrew creates package conflicts with arm64-native gridlabd installation in Darwin systems."
        echo "Please install in a system without Rosetta Homebrew, or you can attempt manual installation."
        exit 1
    fi

# install homebrew instance for gridlabd
    brew update
    if test ! -e /opt/homebrew; then 
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
    brew update-reset
    brew doctor

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
    $ brew install pkg-config openssl@1.1 xz gdbm tcl-tk
    xcode-select --install

    # Update symlinks in /usr/local/bin
    [ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
    [ ! -e /usr/local/bin/sed ] && ln -sf /opt/homebrew/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && ln -sf /opt/homebrew/bin/glibtoolize /usr/local/bin/libtoolize
    [ ! -e /usr/local/bin/libtool ] && ln -sf /opt/homebrew/bin/glibtool /usr/local/bin/libtool


# Install python 3.9.13
# python3 support needed as of 4.2
if [ ! -x /usr/local/opt/gridlabd/bin/python3 -o "$(/usr/local/opt/gridlabd/bin/python3 --version | cut -f2 -d.)" != "Python 3.9" ]; then
	echo "install python 3.9.13"
	cd /usr/local/opt/gridlabd/src

	curl https://www.python.org/ftp/python/3.9.13/Python-3.9.13.tgz | tar xz
	# tar xzf Python-3.9.13.tgz 
	cd Python-3.9.13

    export PKG_CONFIG_PATH="$(brew --prefix tcl-tk)/lib/pkgconfig"
	./configure --prefix=/usr/local/opt/gridlabd \
    --with-universal-archs=arm64 \
    --enable-optimizations \
    --with-system-ffi \
    --with-openssl=$(brew --prefix openssl) \
    --with-pydebug \
    --with-tcltk-libs="$(pkg-config --libs tcl tk)" \
    --with-tcltk-includes="$(pkg-config --cflags tcl tk)"
    --with-computed-gotos CFLAGS="-fPIC" \
    LDFLAGS="-L$(brew --prefix)/lib" CPPFLAGS="-I$(brew --prefix)/include" \

	make -s -j2
	make install
	ln -sf /usr/local/opt/gridlabd/bin/python3.9 /usr/local/opt/gridlabd/bin/python3
	ln -sf /usr/local/opt/gridlabd/bin/python3.9-config /usr/local/opt/gridlabd/bin/python3-config
	ln -sf /usr/local/opt/gridlabd/bin/pydoc3.9 /usr/local/opt/gridlabd/bin/pydoc
	ln -sf /usr/local/opt/gridlabd/bin/idle3.9 /usr/local/opt/gridlabd/bin/idle
	ln -sf /usr/local/opt/gridlabd/bin/pip3.9 /usr/local/opt/gridlabd/bin/pip3

	/usr/local/opt/gridlabd/bin/python3 -m pip install --upgrade pip
	/usr/local/opt/gridlabd/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	/usr/local/opt/gridlabd/bin/python3 -m pip install IPython censusdata


	# if ! grep -q "/usr/local/opt/gridlabd/lib" "/etc/ld.so.conf"; then
	#	sudo touch /etc/ld.so.conf.d/gridlabd.conf
	#	sudo bash -c 'echo "/usr/local/opt/gridlabd/lib" >> /etc/ld.so.conf'
	#	sudo ldconfig
	# fi
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
    ln -sf /opt/homebrew/bin/naturaldocs /usr/local/bin/natural_docs

    brew install doxygen

# influxdb
    brew install influxdb
    brew services start influxdb

# subversion cli
    brew install svn

# mysql connector
#    brew install mysql
#    brew install mysql-client
