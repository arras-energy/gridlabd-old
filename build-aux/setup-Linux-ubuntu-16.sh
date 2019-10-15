#!/bin/bash

# Install needed system tools
# update first
apt -q update
apt-get -q tzdata -y

# install python 3.7
apt-get -q install software-properties-common -y
add-apt-repository ppa:deadsnakes/ppa -y
apt-get -q update
apt-get -q install python3.7 -y
apt-get -q install python3-pip -y
apt-get -q install python3.7-dev -y

# change default python3 to python3.7 for validation
update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.7 1

# install python libraries by validation
pip3 -q install --upgrade pip
pip -q install pandas
pip -q install matplotlib
pip -q install mysql-client

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
apt-get -q install libncurses5-dev -y
apt-get -q install subversion -y

# doxgygen
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
apt-get -q install curl -y
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
	rm natural_docs.zip
	mv Natural\ Docs natural_docs
	echo "#!/bin/bash
mono /usr/local/natural_docs/NaturalDocs.exe \$*" > /usr/local/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi

