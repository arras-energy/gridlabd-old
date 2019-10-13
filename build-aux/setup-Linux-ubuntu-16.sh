#!/bin/bash

# Install needed system tools
# update first
apt-get update -y

# install python 3.7
apt-get install software-properties-common -y
add-apt-repository ppa:deadsnakes/ppa
apt update
apt install python3.7 -y
apt install python3-pip -y
apt install python3.7-dev -y

# change default python3 to python3.7 for validation
update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.7 1

# install python libraries by validation
pip3 install --upgrade pip
pip install pandas
pip install matplotlib
pip install mysql-client

# install system build tools needed by gridlabd
apt-get install git -y
apt-get install unzip -y
apt-get install autoconf -y
apt-get install libtool -y
apt-get install g++ -y
apt-get install cmake -y 

# # documentation tools
# yum -q install gawk -y
# if [ ! -x /usr/bin/doxygen ]; then
# 	git clone https://github.com/doxygen/doxygen.git doxygen
# 	mkdir build
# 	cd build
# 	cmake -G "Unix Makefiles" ..
# 	make
# 	make install
# fi
# if [ ! -f /usr/bin/mono ]; then
# 	rpmkeys --import "http://pool.sks-keyservers.net/pks/lookup?op=get&search=0x3fa7e0328081bff6a14da29aa6a19b38d3d831ef"
# 	curl https://download.mono-project.com/repo/centos7-stable.repo | tee /etc/yum.repos.d/mono-centos7-stable.repo
# 	yum -q install mono-devel -y
# fi

# # TODO: natural_docs
# if [ ! -x /usr/local/bin/natural_docs ]; then
# 	cd /usr/local
# 	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > /tmp/download-$$.zip
# 	unzip -qq /tmp/download-$$
# 	rm /tmp/download-$$.zip
# 	mv Natural\ Docs natural_docs
# 	echo "#!/bin/bash
# mono /usr/local/natural_docs/NaturalDocs.exe \$*" > /usr/local/bin/natural_docs
# 	chmod a+x /usr/local/bin/natural_docs
# fi

