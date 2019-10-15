#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
brew doctor
brew install autoconf automake libtool gnu-sed
[ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
[ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed
brew install python3
pip3 install matplotlib pandas numpy
