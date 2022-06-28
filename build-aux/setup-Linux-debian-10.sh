#!/bin/bash

# Install needed system tools
# update first
apt-get -q update


apt-get install tzdata -y
apt-get install curl -y
apt-get install apt-utils -y

# "Etc" will cause installation error
if [ ! -f /etc/timezone -o "$(cat /etc/timezone | cut -f1 -d'/')" == "Etc" ]; then 
	# get time zone from URL 
	URL="https://ipapi.co/timezone"
	response=$(curl -s -w "%{http_code}" $URL)
	http_code=$(tail -n1 <<< "${response: -3}")  # get the last 3 digits
	if [ $http_code == "200" ]; then 
		time_zone=$(sed 's/.\{3\}$//' <<< "${response}") # remove the last 3 digits
		echo "successful get timezone from  $URL , Set time zone as $time_zone"
		ln -fs /usr/share/zoneinfo/$time_zone /etc/localtime
	else 
		echo "Can not get timezone from $URL , http_code is $http_code "
		echo "Set default time zone as UTC/GMT. "
		ln -fs /usr/share/zoneinfo/UTC/GMT /etc/localtime
	fi

	export DEBIAN_FRONTEND=noninteractive
	apt-get install -y tzdata
	dpkg-reconfigure --frontend noninteractive tzdata
fi

apt-get -q install software-properties-common -y
apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev -y

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
apt-get install liblzma-dev -y
apt-get install libbz2-dev -y
apt-get install libncursesw5-dev -y
apt-get install xz-utils -y

# Install python 3.9.6
# python3 support needed as of 4.2
if [ ! -x /usr/local/opt/gridlabd/bin/python3 -o "$(/usr/local/bin/python3 --version | cut -f2 -d.)" != "Python 3.9" ]; then
	echo "install python 3.9.6"
	cd /usr/local/opt/gridlabd/src

	curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
	# tar xzf Python-3.9.6.tgz 
	cd Python-3.9.6

	./configure --prefix=/usr/local/opt/gridlabd --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"

	make -j $(nproc)
	make install
	/sbin/ldconfig /usr/local/opt/gridlabd/lib
	ln -sf /usr/local/opt/gridlabd/bin/python3.9 /usr/local/opt/gridlabd/bin/python3
	ln -sf /usr/local/opt/gridlabd/bin/python3.9-config /usr/local/opt/gridlabd/bin/python3-config
	ln -sf /usr/local/opt/gridlabd/bin/pydoc3.9 /usr/local/opt/gridlabd/bin/pydoc
	ln -sf /usr/local/opt/gridlabd/bin/idle3.9 /usr/local/opt/gridlabd/bin/idle
	ln -sf /usr/local/opt/gridlabd/bin/pip3.9 /usr/local/opt/gridlabd/bin/pip3
	/usr/local/opt/gridlabd/bin/python3 -m pip install --upgrade pip
	/usr/local/opt/gridlabd/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	/usr/local/opt/gridlabd/bin/python3 -m pip install IPython censusdata
fi

# install latex
apt-get install texlive -y

# doxgygen
apt-get -q install gawk -y
if [ ! -x /usr/bin/doxygen ]; then
	if [ ! -d /usr/local/opt/gridlabd/src/doxygen ]; then
		git clone https://github.com/doxygen/doxygen.git /usr/local/opt/gridlabd/src/doxygen
	fi
	if [ ! -d /usr/local/opt/gridlabd/src/doxygen/build ]; then
		mkdir /usr/local/opt/gridlabd/src/doxygen/build
	fi
	cd /usr/local/opt/gridlabd/src/doxygen/build
	cmake -G "Unix Makefiles" ..
	make
	make install
fi

# # mono

if [ ! -f /usr/bin/mono ]; then
	cd /tmp
	apt install apt-transport-https dirmngr gnupg ca-certificates -y
	apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
	echo "deb https://download.mono-project.com/repo/debian stable-bustergrid main" | sudo tee /etc/apt/sources.list.d/mono-official-stable.list
	apt-get -q update -y
	apt-get -q install mono-devel -y
fi

# natural_docs
if [ ! -x /usr/local/opt/gridlabd/bin/natural_docs ]; then
	cd /usr/local/opt/gridlabd
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > natural_docs.zip
	unzip -qq natural_docs
	rm -f natural_docs.zip
	mv Natural\ Docs natural_docs
	echo '#!/bin/bash
mono /usr/local/opt/gridlabd/natural_docs/NaturalDocs.exe \$*' > /usr/local/opt/gridlabd/bin/natural_docs
	chmod a+x /usr/local/opt/gridlabd/bin/natural_docs
fi

