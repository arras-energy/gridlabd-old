#!/bin/bash

# Install needed system tools
yum -q update -y ; yum -q clean all
yum -q install systemd -y
yum -q clean all
yum -q groupinstall "Development Tools" -y
yum -q install cmake -y 
yum -q install ncurses-devel -y
yum -q install epel-release -y
yum -q install libcurl-devel -y
yum -q install which -y

# upgrade git to version 2.x
yum -q remove git* -y
yum -q install https://centos7.iuscommunity.org/ius-release.rpm -y
yum -q install git2u-all -y

# python3 support needed as of 4.2
yum -q --disablerepo="*" --enablerepo="epel" install python36 -y
yum -q install python36 python36-devel python36-pip python36-tkinter -y
[ -f /usr/bin/python3 ] || ln -s /usr/bin/python3.6 /usr/bin/python3
pip3 --quiet install --upgrade pip
ln -s /usr/local/bin/pip3 /usr/bin/pip3
pip3 --quiet install matplotlib
pip3 --quiet install pandas
pip3 --quiet install mysql-connector

# documentation tools
yum -q install gawk -y
if [ ! -x /usr/bin/doxygen ]; then
	git clone https://github.com/doxygen/doxygen.git doxygen
	mkdir build
	cd build
	cmake -G "Unix Makefiles" ..
	make
	make install
fi
if [ ! -f /usr/bin/mono ]; then
	rpmkeys --import "http://pool.sks-keyservers.net/pks/lookup?op=get&search=0x3fa7e0328081bff6a14da29aa6a19b38d3d831ef"
	curl https://download.mono-project.com/repo/centos7-stable.repo | tee /etc/yum.repos.d/mono-centos7-stable.repo
	yum -q install mono-devel -y
fi

# TODO: natural_docs
if [ ! -x /usr/local/bin/natural_docs ]; then
	cd /usr/local
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > /tmp/download-$$.zip
	unzip -qq /tmp/download-$$
	rm /tmp/download-$$.zip
	mv Natural\ Docs natural_docs
	echo "#!/bin/bash
mono /usr/local/natural_docs/NaturalDocs.exe \$*" > /usr/local/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi

