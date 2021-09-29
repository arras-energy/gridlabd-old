#!/bin/bash
#
# Install setup for Amazon EC2 instance 
#

chmod -R 775 /usr/local
chown -R root:adm /usr/local

# Install needed system tools
yum -q -y install deltarpm
yum -q -y update
yum -q clean all
yum -q -y groupinstall "Development Tools"
yum -q -y install cmake ncurses-devel libcurl-devel
yum -q -y install openssl-devel bzip2-devel libffi-devel zlib-devel xz-devel

cd /usr/local/src
yum -q -y remove python3
curl -sL https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
cd Python-3.9.6
./configure --prefix=/usr/local --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
export MAKEFLAGS=-j$(($(nproc)*3))
export PYTHONSETUPFLAGS="-j $(($(nproc)*3))"
make
make altinstall
ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3
if [ ! -f /usr/local/bin/pip3.9 ]; then
	curl -sSL https://bootstrap.pypa.io/get-pip.py | /usr/local/bin/python3
fi
