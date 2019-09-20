#!/bin/bash
#
# docker centos-gridlabd setup script
#
# Starting docker on the host
#
#   host% docker run -it -v $(pwd):/tmp ubuntu bash
#.  host%
#  

# choose versions
GITURL=https://github.com/dchassin/gridlabd
XERCES=xerces-c-src_2_8_0
MYSQL=mysql-connector-c-6.1.11-linux-glibc2.12-x86_64
ARMA=armadillo-7.800.1

# Install needed tools
cd /tmp
apt-get update -y
apt-get install autoconf -y
apt-get install libtool -y
apt-get install g++ -y
apt-get install cmake -y 
apt-get install git -y

# clone gridlabd
cd /usr/local/src
git clone ${GITURL} gridlabd

# install xercesc
cd /usr/local/src/gridlabd/third_party
gunzip ${XERCES}.tar.gz
tar xf ${XERCES}.tar
cd ${XERCES}
export XERCESCROOT=`pwd`
cd src/xercesc
./runConfigure -plinux -cgcc -xg++ -minmem -nsocket -tnative -rpthread
make
cd ${XERCESCROOT}
cp -r include/xercesc /usr/include
chmod -R a+r /usr/include/xercesc
ln lib/* /usr/lib 
/sbin/ldconfig

# install mysql 
cd /usr/local/src/gridlabd/third_party

gunzip ${MYSQL}.tar.gz
tar xf ${MYSQL}.tar
cp -u ${MYSQL}/bin/* /usr/local/bin
cp -Ru ${MYSQL}/include/* /usr/local/include
cp -Ru ${MYSQL}/lib/* /usr/local/lib

# install armadillo
cd /usr/local/src/gridlabd/third_party

gunzip ${ARMA}.tar.gz
tar xf ${ARMA}.tar
cd ${ARMA}
cmake .
make install

# install needed support libraries
cd /tmp
apt-get install libcurl4-openssl-dev -y
apt-get install libncurses5-dev -y

# install python3 and python libraries
cd /tmp
apt-get install python3 -y
apt-get install python3-pip -y
pip3 install matplotlib
pip3 install pandas

# install gridlabd
cd /usr/local/src/gridlabd
autoreconf -isf
./customize configure
make install

# Validate GridLAB-D
cd /usr/local/src/gridlabd
export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
gridlabd -T 0 --validate
