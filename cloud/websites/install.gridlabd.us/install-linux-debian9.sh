#!/bin/bash
#
# Install setup for Debian 9 instance (including wls hosts)
#

# Install needed system tools
echo "Updating system packages..."
apt -q -y update
apt -q -y install tzdata
apt -q -y install software-properties-common
apt -q -y install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libssl-dev libreadline-dev libffi-dev
apt -q -y install libz-dev libbz2-dev liblzma-dev tk-dev uuid-dev libsqlite3-dev
apt -q -y install libcurl4-gnutls.dev libcurl4-gnutls-dev libcurl4-openssl-dev
apt -q -y install autoconf automake libtool

# Install the proper version of python3
echo "Updating python3..."
apt -q -y remove python3
PREFIX=/usr/local/opt/python3
mkdir -p ${PREFIX%/*}   
if [ ! -d $PREFIX ]; then
    cd ${PREFIX%/*} 
    curl -sL http://install.gridlabd.us/python39-amzn2.tarz | tar xz
fi
if [ ! -d $PREFIX ]; then
    cd /usr/local/src
    curl -sL https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
    cd Python-3.9.6
    ./configure --silent --prefix=$PREFIX --enable-optimizations --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
    export MAKEFLAGS=-j$(($(nproc)*3))
    export PYTHONSETUPFLAGS="-j $(($(nproc)*3))"
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
# if [ ! -f $PREFIX/bin/pip3.9 ]; then
#   curl -sSL https://bootstrap.pypa.io/get-pip.py | /usr/local/bin/python3
# fi
$PREFIX/bin/python3.9 -m pip -q install --upgrade pip

# Install the required python3 packages
echo "Updating python3 requirements..."
curl -sL http://install.gridlabd.us/requirements.txt | /usr/local/bin/pip3 -q install -r /dev/stdin

echo '
**************************************************
Welcome to the HiPAS GridLAB-D host.

Use 'gridlabd --help' for help on using GridLAB-D

Online documentation is available at

  https://docs.gridlabd.us/

**************************************************
' > /etc/motd
echo 'export PATH=/usr/local/bin:$PATH' >> /etc/bashrc
