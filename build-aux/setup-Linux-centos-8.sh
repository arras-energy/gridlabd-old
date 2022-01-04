#!/bin/bash

#Install needed system tools
yum -q update -y ; 
yum -q clean all
yum -q groupinstall "Development Tools" -y
yum -q install cmake -y 
yum -q install ncurses-devel -y
yum -q install epel-release -y
yum -q install curl-devel -y
yum -q install which -y
yum -q install svn -y
yum -q install openssl-devel bzip2-devel libffi-devel zlib-devel -y

# # python3 support needed as of 4.2
if [ ! -x /usr/local/bin/python3 -o "$(/usr/local/bin/python3 --version)" != "Python 3.9.6" ]; then
	echo "install python 3.9.6"	
	
	cd /usr/local/src
	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
	cd Python-3.9.6
	./configure --prefix=/usr/local --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
	make -j $(nproc)
	make altinstall
	ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
	ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
	ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
	ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
	ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3
	# install python packages
	/usr/local/bin/python3 -m pip install --upgrade pip
	/usr/local/bin/python3 -m pip install mysql-connector 
	/usr/local/bin/python3 -m pip install matplotlib 
	/usr/local/bin/python3 -m pip install numpy 
	/usr/local/bin/python3 -m pip install pandas
	/usr/local/bin/python3 -m pip install Pillow
	/usr/local/bin/python3 -m pip install IPython 
	/usr/local/bin/python3 -m pip install wheel 
	/usr/local/bin/python3 -m pip install censusdata
	# update numpy
	/usr/local/bin/python3 -m pip install --upgrade numpy
	# remove tgz
	rm -f /usr/local/src/Python-3.9.6.tgz
fi

# # latex
if [ ! -x /usr/bin/tex ]; then
	yum -q install tex -y
fi

# # doxygen

if [ ! -x /usr/bin/doxygen ]; then
	echo "install doxygen"
	if [ ! -d /usr/local/src/doxygen ]; then
		git clone https://github.com/doxygen/doxygen.git /usr/local/src/doxygen --depth 1
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

	rpmkeys --import "http://keyserver.ubuntu.com/pks/lookup?op=get&search=0x3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF"
	su -c 'curl https://download.mono-project.com/repo/centos8-stable.repo | tee /etc/yum.repos.d/mono-centos8-stable.repo'
	yum -q install mono-devel -y
fi

# # # natural_docs
if [ ! -x /usr/local/bin/natural_docs ]; then
	
	cd /usr/local
	curl -s https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > natural_docs.zip
	unzip -qq natural_docs
	rm -f natural_docs.zip
	mv Natural\ Docs natural_docs
	echo '#!/bin/bash
mono /usr/local/natural_docs/NaturalDocs.exe \$*' > /usr/local/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi

