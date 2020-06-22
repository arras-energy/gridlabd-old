#!/bin/bash
dnf install gcc openssl-devel bzip2-devel libffi-devel -y
yum install -y xz-devel -y
cd /usr/local/src
curl https://www.python.org/ftp/python/3.7.7/Python-3.7.7.tgz > Python-3.7.7.tgz
tar xzf Python-3.7.7.tgz
cd Python-3.7.7
./configure --enable-optimizations
make altinstall
ln -sf /usr/local/bin/python3.7 /usr/local/bin/python3
ln -f /usr/local/bin/python3.7m-config /usr/local/python3-config
ln -s /usr/local/bin/pyvenv-3.7 /usr/local/bin/pyvenv
/usr/local/bin/python3 -m pip install matplotlib Pillow pandas numpy
cd /usr/local/src
rm -f Python-3.7.7.tgz
