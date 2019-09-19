#!/bin/bash
#
# docker centos-gridlabd setup script
#
# Starting docker on the host
#
#   host% docker run -it -v $(pwd):/tmp ubuntu bash
#.  host%
#  

### Docker commands to build gridlabd

# Install needed tools
apt-get update -y
apt-get install autoconf -y
apt-get install libtools -y
apt-get install autoconf -y
apt-get install g++ -y
apt-get install cmake -y 

cd /usr/local/src
git clone https://github.com/dchassin/gridlabd gridlabd

# install xercesc
cd /usr/local/src/gridlabd/third_party
XERCES=xerces-c-src_2_8_0
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
MYSQL=mysql-connector-c-6.1.11-linux-glibc2.12-x86_64
gunzip ${MYSQL}.tar.gz
tar xf ${MYSQL}.tar
cp -u ${MYSQL}/bin/* /usr/local/bin
cp -Ru ${MYSQL}/include/* /usr/local/include
cp -Ru ${MYSQL}/lib/* /usr/local/lib

# install armadillo
cd /usr/local/src/gridlabd/third_party
ARMA=armadillo-7.800.1
gunzip ${ARMA}.tar.gz
tar xf ${ARMA}.tar
cd ${ARMA}
cmake .
make install

# install gridlabd
cd /usr/local/src/gridlabd
autoreconf -isf
./customize configure
make install

# Validate GridLAB-D
export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
gridlabd -T 0 --validate



