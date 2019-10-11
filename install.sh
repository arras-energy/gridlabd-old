#!/bin/bash

# check
if [ -d /usr/local/gridlabd -a ! -L /usr/local/gridlabd ]; then
    echo "ERROR: /usr/local/gridlabd is not a symbolic"
    exit 1
fi
if [ $(whoami) == "root" ]; then
    echo "ERROR: do not run $0 as root"
fi

# autoconf
if [ ! "$1" == "quick" ]; then
    autoscan
fi
if [ ! -d autom4te.cache -o ! "$1" == "quick" ]; then
    autoreconf -isf
fi

# prep install dir
VERSION=`build-aux/version.sh --install`
INSTALL=/usr/local/$VERSION
sudo rm -rf $INSTALL || exit 1
sudo mkdir -p $INSTALL
sudo /usr/sbin/chown -R $USER $INSTALL
if [ ! -f configure -o ! "$1" == "quick" ]; then
    ./configure --prefix=$INSTALL $*
fi

# build everything
export PATH=$INSTALL/bin:/usr/local/bin:/usr/bin:/bin
make -j30
make install
make html 
cp -r documents/html $INSTALL
cp documents/index.html $INSTALL/html
#make index
#gridlabd --validate

# activate this version
if [ -x $INSTALL/bin/gridlabd-version ]; then
	gridlabd version set $VERSION
else
	sudo rm -f /usr/local/gridlabd
	sudo ln -s $INSTALL /usr/local/gridlabd
	if [ ! -f /usr/local/bin/gridlabd ]; then
		sudo ln -s /usr/local/gridlabd/bin/gridlabd /usr/local/bin/gridlabd
	fi
fi