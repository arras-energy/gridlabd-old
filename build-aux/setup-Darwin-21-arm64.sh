#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# install homebrew instance for gridlabd
    brew update || sudo mkdir /opt/homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C /opt/homebrew
    export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
    brew update-reset
    brew doctor

echo "$1"
if ! grep -q "$1/bin" "$HOME/.zshrc"; then
    touch "$HOME/.zshrc"
    echo "export PATH=$1/bin:\$PATH" >> $HOME/.zshrc
fi

# build tools

    brew install autoconf automake libtool gnu-sed gawk

    # Update symlinks in the gridlabd bin
    [ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
    [ ! -e /usr/local/bin/sed ] && ln -s /opt/homebrew/bin/gsed /usr/local/bin/sed
    [ ! -e /usr/local/bin/libtoolize ] && ln -s /opt/homebrew/bin/glibtoolize /usr/local/bin/libtoolize

# install python3
    brew install python3

# mdbtools
echo "THIS IS ARM!!!!!!!!!!!!!!!!!"
exit 125
    brew install mdbtools
    pip3 install matplotlib pandas numpy networkx Pillow
echo "THIS IS ARM!!!!!!!!!!!!!!!!!"
exit 125

# The original scikit-learn at 0.24.2 CANNOT install on the m1 mac. Period. Use 1.1.1 now.
# Reason being, is that it requires a version of NumPy that is incompatible with the m1 mac.
# updated in requirements.txt. Same goes for scipy 1.6.2.
    brew install gdal

# Install pyproj manually due to error building final wheel
    brew install proj
    pip3 install pyproj

# docs generators
    brew install --build-from-source mono
    brew install naturaldocs
    ln -s /opt/homebrew/bin/naturaldocs /usr/local/bin/natural_docs

    brew install doxygen

# influxdb
    brew install influxdb
    brew services start influxdb

# subversion cli
    brew install svn

# mysql connector
    brew install mysql
    brew install mysql-client

ln -s /opt/homebrew/bin/* /usr/local/bin || sudo ln -s /opt/homebrew/bin/* /usr/local/bin
ln -s /opt/homebrew/lib/* /usr/local/lib || sudo ln -s /opt/homebrew/lib/* /usr/local/lib
ln -s /opt/homebrew/include/* /usr/local/include || sudo ln -s /opt/homebrew/include/* /usr/local/include
ln -s /opt/homebrew/var/* /usr/local/var || sudo ln -s /opt/homebrew/var/* /usr/local/var
ln -s /opt/homebrew/etc/* /usr/local/etc || sudo ln -s /opt/homebrew/etc/* /usr/local/etc
ln -s /opt/homebrew/share/* /usr/local/share || sudo ln -s /opt/homebrew/share/* /usr/local/share