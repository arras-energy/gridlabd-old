#!/bin/bash
#
# Install setup for Darwin 19 instance
#

[ -z "$SOURCE" ] && SOURCE=http://install.gridlabd.us

# Install needed system tools
echo "Updating system packages..."
brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
brew install -q autoconf automake libtool gnu-sed gawk
[ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
[ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed
[ ! -e /usr/local/bin/libtoolize ] && ln -s /usr/local/bin/glibtoolize /usr/local/bin/libtoolize
brew install -q xz openssl gdbm sqlite ossp-uuid zlib libffi bzip2 curl

# Install the proper version of python3
echo "Updating python3..."
PREFIX=/usr/local/opt/python3
mkdir -p ${PREFIX%/*}   
if [ ! -d $PREFIX ]; then
    cd ${PREFIX%/*} 
    curl -sL http://install.gridlabd.us/python39-darwin20.tarz | tar xz
fi
if [ ! -d $PREFIX ]; then
    mkdir -p /usr/local/src
    cd /usr/local/src
    curl -sL https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
    cd Python-3.9.6
    ./configure --silent --prefix=$PREFIX --enable-optimizations --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
    export MAKEFLAGS=-j$(($(sysctl -n hw.ncpu)*3))
    export PYTHONSETUPFLAGS="-j $(($(sysctl -n hw.ncpu)*3))"
    make --silent
    make altinstall --silent
fi
echo "Linking python3..."
ln -sf $PREFIX/bin/python3.9 /usr/local/bin/python3
ln -sf $PREFIX/bin/python3.9 /usr/local/bin/python3.9
ln -sf $PREFIX/bin/python3.9-config /usr/local/bin/python3-config
ln -sf $PREFIX/bin/pydoc3.9 /usr/local/bin/pydoc
ln -sf $PREFIX/bin/idle3.9 /usr/local/bin/idle
ln -sf $PREFIX/bin/pip3.9 /usr/local/bin/pip3
ln -sf $PREFIX/include/python3.9 /usr/local/include/python3.9
ln -sf $PREFIX/lib/python3.9 /usr/local/lib/python3.9
$PREFIX/bin/python3.9 -m pip -q install --upgrade pip

# Install the required python3 packages
echo "Updating python3 requirements..."
curl -sL $SOURCE/requirements.txt | /usr/local/bin/pip3 -q install -r /dev/stdin

echo "darwin20 system ready for gridlabd install"
