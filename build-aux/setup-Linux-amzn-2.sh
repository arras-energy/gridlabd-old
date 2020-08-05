#!/bin/bash
#
# Install script for Amazon EC2 instance 
#

chmod -R 775 /usr/local
chown -R root:adm /usr/local

# Install needed system tools
yum -q update -y ; 
yum -q clean all
yum -q groupinstall "Development Tools" -y
yum -q install cmake -y 
yum -q install ncurses-devel -y
#yum -q install epel-release -y
yum -q install libcurl-devel -y

# python3 support needed as of 4.2
yum -q install python37 python3-devel python3-tkinter -y
[ -f /bin/python3 -a ! -f /usr/local/bin/python3 ] && ln -s /bin/python3 /usr/local/bin/python3
[ -f /bin/python3-config -a ! -f /usr/local/bin/python3-config ] && echo '#!/bin/bash
/bin/python3-config $*' > /usr/local/bin/python3-config
chmod +x /usr/local/bin/python3-config
[ ! -f /usr/local/include/python3.7m ] && ln -sf /usr/include/python3.7m /usr/local/include/python3.7m
/usr/local/bin/python3 -m pip install --quiet --user matplotlib pandas mysql-connector Pillow
chmod -R a+w /usr/local/lib64/python3.7/site-packages

# mono
if [ ! -f /usr/bin/mono ]; then
	rpmkeys --import "http://pool.sks-keyservers.net/pks/lookup?op=get&search=0x3fa7e0328081bff6a14da29aa6a19b38d3d831ef"
	curl https://download.mono-project.com/repo/centos7-stable.repo > /etc/yum.repos.d/mono-centos7-stable.repo
	yum -q install mono-devel -y
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

# converter support
/usr/local/bin/python3 -m pip install --quiet --user networkx 
# cd /tmp
# curl http://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/m/mdbtools-0.7.1-3.el7.x86_64.rpm > mdbtools-0.7.1-3.el7.x86_64.rpm
# rpm -Uvh mdbtools-0.7.1-3.el7.x86_64.rpm
# yum -q install mdbtools -y
