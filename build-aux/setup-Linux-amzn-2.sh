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

# python3.9.x support needed as of 4.2
if [ ! -x /usr/local/bin/python3 -o "$(/usr/local/bin/python3 --version | cut -f2 -d.)" != "Python 3.9" ]; then
	yum install openssl-devel bzip2-devel libffi-devel zlib-devel xz-devel -q -y
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
	curl -sSL https://bootstrap.pypa.io/get-pip.py | /usr/local/bin/python3
	#/usr/local/bin/python3 pip -m install mysql-connector mysql-client matplotlib numpy pandas Pillow networkx
	/usr/local/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	/usr/local/bin/python3 -m pip install IPython censusdata
fi

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
# cd /tmp
# curl http://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/m/mdbtools-0.7.1-3.el7.x86_64.rpm > mdbtools-0.7.1-3.el7.x86_64.rpm
# rpm -Uvh mdbtools-0.7.1-3.el7.x86_64.rpm
yum -q install mdbtools -y
