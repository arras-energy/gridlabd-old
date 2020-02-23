#!/bin/bash

# Install needed system tools
yum -q update -y ; 
yum -q clean all
yum -q groupinstall "Development Tools" -y
yum -q install cmake -y 
yum -q install ncurses-devel -y
yum -q install epel-release -y
yum -q install curl-devel -y
yum -q install which -y
yum -q install svn -y

# python3 support needed as of 4.2
yum -q install python3 python36-devel python3-pip python3-tkinter -y
pip3 --quiet install --upgrade pip
echo '#/bin/bash' > /usr/local/bin/python3-config
echo '/usr/bin/python3-config $*' >> /usr/local/bin/python3-config
chmod +x /usr/local/bin/python3-config
[ -x /usr/local/bin/python3 ] || ln -s /usr/bin/python3 /usr/local/bin/python3
/usr/local/bin/python3 -m pip --quiet install matplotlib
/usr/local/bin/python3 -m pip --quiet install pandas
/usr/local/bin/python3 -m pip --quiet install mysql-connector
/usr/local/bin/python3 -m pip --quiet install Pillow

# doxygen
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
	rpmkeys --import "http://pool.sks-keyservers.net/pks/lookup?op=get&search=0x3fa7e0328081bff6a14da29aa6a19b38d3d831ef"
	curl -s https://download.mono-project.com/repo/centos8-stable.repo | tee /etc/yum.repos.d/mono-centos8-stable.repo
	yum -q install mono-devel -y
fi

# natural_docs
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

