#!/bin/bash
#
# Install setup for Darwin 19 instance
#

[ -z "$SOURCE" ] && SOURCE=http://install.gridlabd.us

# Install needed system tools
echo "Updating system packages..."
brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
brew install autoconf automake libtool gnu-sed gawk
[ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
[ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed
[ ! -e /usr/local/bin/libtoolize ] && ln -s /usr/local/bin/glibtoolize /usr/local/bin/libtoolize

# Install the proper version of python3
if [ "$(python3 --version | cut -f-2 -d.)" != "3.9" ]; then
    echo "Installing python 3.9"
    brew install python@3.9
fi

PREFIX=$(dirname $(which python3.9))
if [ "$PREFIX" != "/usr/local/bin" ]; then
    echo "Linking python3..."
    ln -sf $PREFIX/bin/python3.9 /usr/local/bin/python3
    ln -sf $PREFIX/bin/python3.9 /usr/local/bin/python3.9
    ln -sf $PREFIX/bin/python3.9-config /usr/local/bin/python3-config
    ln -sf $PREFIX/bin/pydoc3.9 /usr/local/bin/pydoc
    ln -sf $PREFIX/bin/idle3.9 /usr/local/bin/idle
    ln -sf $PREFIX/bin/pip3.9 /usr/local/bin/pip3
    ln -sf $PREFIX/include/python3.9 /usr/local/include/python3.9
    ln -sf $PREFIX/lib/python3.9 /usr/local/lib/python3.9
fi
$PREFIX/bin/python3.9 -m pip -q install --upgrade pip

# Install the required python3 packages
echo "Updating python3 requirements..."
curl -sL $SOURCE/requirements.txt | /usr/local/bin/pip3 -q install -r /dev/stdin

echo "darwin19 system ready for gridlabd install"
