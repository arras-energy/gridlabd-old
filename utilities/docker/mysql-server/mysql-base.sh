#!/bin/bash
#
# Building the image
#
#   host% docker build -f Dockerfile-mysql -t gridlabd/mysql-server .
#
# Running the image
#
#   host% docker run --name gridlabd_1 -it gridlabd/mysql-server
#
# Accessing a shell in the image
#
#   host% docker exec -it gridlabd_1
#  

# configuration
GITREPO=https://github.com/dchassin/gridlabd 
GITBRANCH=build_docker_mysql_base
XERCES=xerces-c-src_2_8_0
MYSQL=mysql-connector-c-6.1.11-linux-glibc2.12-x86_64
ARMA=armadillo-7.800.1

# Install needed tools0
echo "###"
echo "### INSTALLING LINUX BUILD TOOLS ###"
echo "###"
cd /tmp
apt-get update -y
apt-get install autoconf -y
apt-get install libtool -y
apt-get install g++ -y
apt-get install cmake -y 
apt-get install git -y

# clone gridlabd
echo "###"
echo "### CLONING ${GITREPO:-https://github.com/dchassin/gridlabd} -b ${GITBRANCH:-master} ###"
echo "###"
cd /usr/local/src
git clone ${GITREPO:-https://github.com/dchassin/gridlabd} -b ${GITBRANCH:-master} gridlabd

# install xercesc
echo "###"
echo "### INSTALLING ${XERCES} ###"
echo "###"
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
echo "###"
echo "### INSTALLING ${MYSQL} ###"
echo "###"
cd /usr/local/src/gridlabd/third_party
gunzip ${MYSQL}.tar.gz
tar xf ${MYSQL}.tar
cp -u ${MYSQL}/bin/* /usr/local/bin
cp -Ru ${MYSQL}/include/* /usr/local/include
cp -Ru ${MYSQL}/lib/* /usr/local/lib

# install armadillo
echo "###"
echo "### INSTALLING ${ARMA} ###"
echo "###"
cd /usr/local/src/gridlabd/third_party
gunzip ${ARMA}.tar.gz
tar xf ${ARMA}.tar
cd ${ARMA}
cmake .
make -j20 install

# install needed support libraries
echo "###"
echo "### INSTALLING SUPPORT LIBRARIES ###"
echo "###"
cd /tmp
apt-get install libcurl4-openssl-dev -y
apt-get install libncurses5-dev -y

# install python3 and python libraries
echo "###"
echo "### INSTALLING PYTHON AND MODULES ###"
echo "###"
cd /tmp
apt-get install python3 -y
apt-get install python3-pip -y
pip3 install matplotlib
pip3 install pandas

# install gridlabd
echo "###"
echo "### INSTALLING GRIDLABD ###"
echo "###"
cd /usr/local/src/gridlabd
autoreconf -isf
./customize configure
make -j20 install

# Validate GridLAB-D
echo "###"
echo "### VALIDATING GRIDLABD ###"
echo "###"
cd /usr/local/src/gridlabd
export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
gridlabd -T 0 --validate

echo "### MYSQL BASE BUILD DONE ###"
