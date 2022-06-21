#!/bin/bash

# Install needed system tools
# update first
apt-get -q update
apt-get -q install tzdata -y

# install system build tools needed by gridlabd
apt-get -q install git -y
apt-get -q install unzip -y
apt-get -q install autoconf -y
apt-get -q install libtool -y
apt-get -q install g++ -y
apt-get -q install cmake -y 
apt-get -q install flex -y
apt-get -q install bison -y
apt-get -q install libcurl4-gnutls-dev -y
apt-get -q install subversion -y
apt-get -q install util-linux -y
apt-get -q install libncurses5-dev -y
apt-get -q install libncursesw5-dev -y
apt-get -q install liblzma-dev -y
apt-get -q install libssl-dev -y
apt-get -q install libbz2-dev -y
apt-get -q install libffi-dev -y
apt-get -q install zlib1g-dev -y
apt-get -q install mdbtools -y
apt-get -q install curl -y
apt-get -q install build-essential -y
apt-get -q install libgdbm-dev -y
apt-get -q install libnss3-dev -y
apt-get -q install libreadline-dev -y
apt-get install xz-utils -y

# install python 3.9
if [ ! -e /usr/local/bin/python3.9 ]; then
	cd /usr/local/src
	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
	cd Python-3.9.6

	./configure --prefix=/usr/local --enable-shared --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
	
	make -j $(nproc)
	make install
	/sbin/ldconfig /usr/local/lib
	ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
	ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
	ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
	ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
	ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3

	# curl -sSL https://bootstrap.pypa.io/get-pip.py | /usr/local/bin/python3

	# install python libraries by validation
	/usr/local/bin/python3 -m pip install --upgrade pip
	/usr/local/bin/python3 -m pip install mysql-connector mysql-client matplotlib numpy pandas Pillow networkx

	# add to LD_LIBRARY_PATH for Python to compile in Gridlabd
	#if [ ! -e /etc/ld.so.conf.d/gridlabd.conf ]; then
	#	sudo touch /etc/ld.so.conf.d/gridlabd.conf
	#	sudo bash -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
	#	sudo ldconfig
	#fi
fi

# doxygen
apt-get -q install gawk -y
if [ ! -x /usr/bin/doxygen ]; then
	if [ ! -d /usr/local/src/doxygen ]; then
		git clone https://github.com/doxygen/doxygen.git /usr/local/src/doxygen
	fi
	if [ ! -d /usr/local/src/doxygen/build ]; then
		mkdir /usr/local/src/doxygen/build
	fi
	cd /usr/local/src/doxygen/build
	cmake -G "Unix Makefiles" ..
	make
	make install
fi

# mono
if [ ! -f /usr/bin/mono ]; then
	cd /tmp
	apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
	echo "deb http://download.mono-project.com/repo/ubuntu wheezy/snapshots/4.8.0 main" | tee /etc/apt/sources.list.d/mono-official.list
	apt-get -q update -y
	apt-get -q install mono-devel -y
fi

# natural_docs
if [ ! -x /usr/local/bin/natural_docs ]; then
	cd /usr/local
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > natural_docs.zip
	unzip -qq natural_docs
	rm -f natural_docs.zip
	mv Natural\ Docs natural_docs
	echo '#!/bin/bash
mono /usr/local/natural_docs/NaturalDocs.exe \$*' > /usr/local/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi
