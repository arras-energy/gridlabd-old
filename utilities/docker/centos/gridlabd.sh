REPO=${REPO:-https://github.com/dchassin/gridlabd}
BRANCH=${BRANCH:-master}
WEATHER='CA WA'
echo "
#####################################
# DOCKER BUILD
#   gridlabd <- $REPO/$BRANCH
#####################################
"

# gridlabd source
cd /usr/local/src
git clone $REPO gridlabd -b $BRANCH

# install xercesc
cd /usr/local/src/gridlabd/third_party
XERCES=xerces-c-src_2_8_0
if [ -f ${XERCES}.tar.gz ]; then
	# use older version of xerces if still available
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
else # use newer version
	XERCES=xerces-c-3.2.0
	if [ ! -f ${XERCES}.tar.gz ]; then
		curl -L https://archive.apache.org/dist/xerces/c/3/sources/${XERCES}.tar.gz > ${XERCES}.tar.gz
	fi
	tar xvfz ${XERCES}.tar.gz
	cd ${XERCES}
	./configure --disable-static 'CFLAGS=-O2' 'CXXFLAGS=-O2' --with-mysql
	make install
	echo "${XERCES} installed ok"
fi
	
# install mysql 
cd /usr/local/src/gridlabd/third_party
MYSQL=mysql-connector-c-6.1.11-linux-glibc2.12-x86_64
if [ ! -f ${MYSQL}.tar.gz ]; then
	curl -L https://downloads.mysql.com/archives/get/file/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar.gz -o ${MYSQL}.tar.gz
fi
if [ -f ${MYSQL}.tar.gz ]; then
	gunzip ${MYSQL}.tar.gz
	tar xf ${MYSQL}.tar
	cp -u ${MYSQL}/bin/* /usr/local/bin
	cp -Ru ${MYSQL}/include/* /usr/local/include
	cp -Ru ${MYSQL}/lib/* /usr/local/lib
	MYSQLOPT=/usr/local
	echo "${MYSQL} installed into ${MYSQLOPT}"
else
	echo "WARNING: ${MYSQL} not found -- mysql will not be included in this build" > /dev/stderr
	MYSQLOPT=no
fi

# install armadillo
cd /usr/local/src/gridlabd/third_party
ARMA=armadillo-7.800.1
if [ -f ${ARMA}.tar.gz ]; then
	gunzip ${ARMA}.tar.gz
	tar xf ${ARMA}.tar
	cd ${ARMA}
	cmake .
	make install
else
	echo "WARNING: ${ARMA} not found -- armadillo will not be included in this build" > /dev/stderr
fi

# cleanup third-party sources
echo "Cleaning up third-party sources"
rm -rf ._armadillo-7.800.1 armadillo-7.800.1.tar armadillo-7.800.1/ mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/ xerces-c-src_2_8_0.tar
git checkout .

# debugger support
if [ "${ENABLE_GDB}" == "yes" ]; then
	echo "Added support for gdb"
	debuginfo-install glibc-2.17-222.el7.x86_64 libgcc-4.8.5-28.el7_5.1.x86_64 libstdc++-4.8.5-28.el7_5.1.x86_64
	CFLAGS='-w -O0 -g'
	CXXFLAGS='-w -O0 -g'
else
	CFLAGS='-w -O3'
	CXXFLAGS='-w -O3'
fi

# install gridlabd
cd /usr/local/src/gridlabd
if [ -f customize -a ! -z "${ENABLE}" ]; then
	./customize enable ${ENABLE} || true
fi
autoreconf -isf
./configure --enable-silent-rules --prefix=/usr/local --with-mysql=$MYSQLOPT
make install
git checkout "$BRANCH"
if [ "$BRANCH" = "master" ]
then 
	make validate 
fi

# download weather data
if [ "${GET_WEATHER:-no}" == "yes" ]; then
	if [ -d /usr/local/share/gridlabd ]; then
		git clone https://github.com/dchassin/weather /usr/local/src/weather
		for state in ${WEATHER}; do
			ln /usr/local/src/weather/US/${state}*.tmy3  /usr/local/share/gridlabd
		done
	else
		echo "WARNING: /usr/local/share/gridlabd not found -- no weather data downloaded" >/dev/stderr
	fi
fi

# daemon setup
echo '# gridlabd daemon configuration for docker 
workdir=/usr/local/var/gridlabd
log=/usr/local/var/gridlabd/gridlabd-log
pid=/usr/local/var/gridlabd/gridlabd-pid
user=gridlabd
listen=0.0.0.0
' > /usr/local/etc/gridlabd.cnf
mkdir -p /usr/local/var
adduser -d /usr/local/var/gridlabd gridlabd

# clean up
if [ "${REMOVE_SOURCE:-yes}" == "yes" ]; then
	echo "Cleaning up source code"
	rm -rf /usr/local/src/*
fi

# done
echo "Build of $REPO/$BRANCH done"

