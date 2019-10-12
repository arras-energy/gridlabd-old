#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Install needed system tools
yum update -y ; yum clean all
yum install systemd -y
yum clean all
yum groupinstall "Development Tools" -y
yum install cmake -y 
yum install ncurses-devel -y
yum install epel-release -y
yum install libcurl-devel -y
yum install which -y

# upgrade git to version 2.x
yum remove git* -y
yum install https://centos7.iuscommunity.org/ius-release.rpm -y
yum install git2u-all -y

# python3 support needed as of 4.2
yum --disablerepo="*" --enablerepo="epel" install python36 -y
yum install python36 python36-devel python36-pip python36-tkinter -y
[ -f /usr/bin/python3 ] || ln -s /usr/bin/python3.6 /usr/bin/python3
pip3 install --upgrade pip
ln -s /usr/local/bin/pip3 /usr/bin/pip3
pip3 install matplotlib
pip3 install pandas
pip3 install mysql-connector

# documentation tools
yum install gawk -y
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
	yum install mono-devel -y
fi

# TODO: natural_docs
if [ ! -x /usr/local/bin/natural_docs ]; then
	cd /usr/local
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > /tmp/download-$$.zip
	unzip /tmp/download-$$
	rm /tmp/download-$$.zip
	mv Natural\ Docs natural_docs
	echo "#!/bin/bash
mono /usr/local/natural_docs/NaturalDocs.exe \$*" > /usr/local/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi

