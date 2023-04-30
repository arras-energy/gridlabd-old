#!/bin/bash
#
# Install script for Amazon EC2 instance 

# Set environment variable in EC2.
# In order to set the environment variable permanently, the system needs to be rebooted after the first time run this code.
# The other way around is to run `export PATH=/usr/local/bin:$PATH` before run `./install.sh` in the command line. 
# The temporary `PATH` will be generated and used for installation. 
# The `PATH` will be set permanently because of running `echo "export PATH=/usr/local/bin:$PATH" >> /etc/profile.d/setVars.sh`

echo "export PATH=/usr/local/bin:$PATH" >> /etc/profile.d/setVars.sh && \
source /etc/profile.d/setVars.sh

chmod -R 775 /usr/local
chown -R root:adm /usr/local

# Install needed system tools
yum -q update -y
yum -q clean all
yum -q groupinstall "Development Tools" -y
yum -q install cmake -y 
yum -q install ncurses-devel -y
#yum -q install epel-release -y
yum -q install libcurl-devel -y
yum -q install mdbtools -y
yum -q install openssl-devel bzip2-devel libffi-devel zlib-devel xz-devel

# python3.9.x support needed as of 4.2
if [ ! -x /usr/local/bin/python3 -o "$(/usr/local/bin/python3 --version | cut -f2 -d.)" != "Python 3.9" ]; then
	yum install openssl-devel bzip2-devel libffi-devel zlib-devel xz-devel -q -y
	cd /usr/local/src
	curl https://www.python.org/ftp/python/3.10.11/Python-3.10.11.tgz | tar xz
	cd Python-3.10.11
	./configure --prefix=/usr/local --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
	make -j $(nproc)
	make altinstall
	ln -sf /usr/local/bin/python3.10 /usr/local/bin/python3
	ln -sf /usr/local/bin/python3.10-config /usr/local/bin/python3-config
	ln -sf /usr/local/bin/pydoc3.10 /usr/local/bin/pydoc
	ln -sf /usr/local/bin/idle3.10 /usr/local/bin/idle
	ln -sf /usr/local/bin/pip3.10 /usr/local/bin/pip3
	curl -sSL https://bootstrap.pypa.io/get-pip.py | /usr/local/bin/python3
	#/usr/local/bin/python3 pip -m install mysql-connector mysql-client matplotlib numpy pandas Pillow networkx
	/usr/local/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	/usr/local/bin/python3 -m pip install IPython censusdata
fi


# mono
if [ ! -f /usr/bin/mono ]; then
	echo "Install mono"
	cd ~
	wget https://dl.fedoraproject.org/pub/fedora/linux/development/rawhide/Everything/x86_64/os/Packages/l/libpng15-1.5.30-13.fc35.x86_64.rpm
	yum install -y ~/downloads/mono_dependencies/libpng15-1.5.30-13.fc35.x86_64.rpm
	yum install yum-utils
	rpm --import "http://keyserver.ubuntu.com/pks/lookup?op=get&search=0x3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF"
	yum-config-manager --add-repo http://download.mono-project.com/repo/centos/
	yum clean all
	yum makecache
	yum install mono-complete -y
	cd ~
	rm -rf /tmp/mono_deps
fi

# # natural_docs
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

# # converter support

echo "Install support"
cd ~
amazon-linux-extras install epel -y
yum-config-manager --enable epel
yum -q install mdbtools -y

# # #latex
echo "Install latex"
yum -q install texlive -y

