#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
brew doctor

# build tools
brew install autoconf automake libtool gnu-sed
[ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
[ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed

# docs generators
brew install mono
brew install naturaldocs

# python3
brew install python3 mdbtools
pip3 install matplotlib pandas numpy networkx Pillow

# influxdb
brew install influxdb
brew services start influxdb

brew install pandoc
brew cask install basictex

