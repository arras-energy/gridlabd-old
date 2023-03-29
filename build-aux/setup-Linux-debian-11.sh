#!/bin/bash

# VAR is defined in install.sh and exported, to be available here
# Set version and paths, using these vars will make future maintenance much better. #Automation
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VERSION_DIR=$VAR/$VERSION
	PKG_PYTHON_DIR=/usr/local/bin
	VENV_PYTHON_DIR=$VERSION_DIR/bin/pkgenv/bin
    PYTHON_VER=3.9.6
    PY_EXE=3.9
	REQ_DIR=$(pwd)

# Install needed system tools
# update first and install libgdal-dev second, as sometimes other package installs break libgdal, but tzdata needs to be first
# to prevent interactive popup breaking automation
sudo apt-get -q update

export DEBIAN_FRONTEND=noninteractive

ln -fs /usr/share/zoneinfo/America/Los_Angeles /etc/localtime
apt-get install -y tzdata
dpkg-reconfigure --frontend noninteractive tzdata
sudo apt install libgdal-dev -y
sudo apt-get install curl -y
sudo apt-get install apt-utils -y

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

sudo apt-get -q install software-properties-common -y
sudo apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev -y

# install system build tools needed by gridlabd

sudo apt-get -q install git -y
sudo apt-get -q install unzip -y
sudo apt-get -q install autoconf -y
sudo apt-get -q install libtool g++ cmake flex bison libcurl4-gnutls-dev subversion util-linux liblzma-dev libbz2-dev libncursesw5-dev xz-utils -y
sudo apt-get -q install g++ -y
sudo apt-get -q install cmake -y 
sudo apt-get -q install flex -y
sudo apt-get -q install bison -y
sudo apt-get -q install libcurl4-gnutls-dev -y
sudo apt-get -q install subversion -y
sudo apt-get -q install util-linux -y
sudo apt-get install liblzma-dev -y
sudo apt-get install libbz2-dev -y
sudo apt-get install libncursesw5-dev -y
sudo apt-get install xz-utils -y
sudo apt-get install wget -y
sudo apt-get install curl -y

# autoconf fills their version with a lot of crud. This is to purify it down to the actual version, and make it comparable. 
	ACV=$(autoconf --version | cut -d ' ' -f4)
	ACV=$(echo $ACV | cut -d ' ' -f1)
	ACV=$(echo "${ACV//./}")

	if [ -z "$ACV" ] || [ "$ACV" -lt "271" ] ; then
		cd $HOME/temp
		wget https://ftpmirror.gnu.org/autoconf/autoconf-2.71.tar.gz
		tar xzvf autoconf-2.71.tar.gz
		cd autoconf-2.71
		./configure
		make
		make install
		cd $HOME/temp
		rm -rf autoconf-2.71
		rm -rf autoconf-2.71.tar.gz
	fi

export LD_LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:/usr/local/lib:/usr/local/include:$LD_LIBRARY_PATH
export LIBRARY_PATH=$VERSION_DIR/lib:$VERSION_DIR/include:/usr/local/lib:/usr/local/include:$LIBRARY_PATH

# Install python $PYTHON_VER
# python3 support needed as of 4.2
if [ ! -x /usr/local/bin/python3 -o "$(/usr/local/bin/python3 --version | cut -f3 -d.)" != "Python $PY_EXE" ]; then
	echo "installing python $PYTHON_VER and ssl module dependencies"
	cd /usr/local/src

	curl https://www.python.org/ftp/python/$PYTHON_VER/Python-$PYTHON_VER.tgz | tar xz

	# tar xzf Python-$PYTHON_VER.tgz
	cd /usr/local/src/Python-$PYTHON_VER

	./configure --prefix=/usr/local --enable-shared --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"

	make -j $(nproc)
	make install
	/sbin/ldconfig $VERSION_DIR/lib
	ln -sf $PKG_PYTHON_DIR/python3.9 $PKG_PYTHON_DIR/python3
	ln -sf $PKG_PYTHON_DIR/python3.9-config $PKG_PYTHON_DIR/python3-config
	ln -sf $PKG_PYTHON_DIR/pydoc3.9 $PKG_PYTHON_DIR/pydoc
	ln -sf $PKG_PYTHON_DIR/idle3.9 $PKG_PYTHON_DIR/idle
	ln -sf $PKG_PYTHON_DIR/pip3.9 $PKG_PYTHON_DIR/pip3

	cd $VERSION_DIR/bin
	$PKG_PYTHON_DIR/python3 -m venv pkgenv

	ln -sf $PKG_PYTHON_DIR/python3.9-config $VENV_PYTHON_DIR/python3-config
	ln -sf /usr/local/lib/*$PY_EXE $VERSION_DIR/bin/pkgenv/lib
	ln -sf /usr/local/include/python$PY_EXE $VERSION_DIR/bin/pkgenv/include

#	if [ ! -e /etc/ld.so.conf.d/gridlabd-$VERSION.conf ]; then
#		cd $HOME/temp
#		sudo touch $HOME/temp/gridlabd-$VERSION.conf
#		echo "$VERSION_DIR/lib" >> $HOME/temp/gridlabd-$VERSION.conf
#		sudo mv $HOME/temp/gridlabd-$VERSION.conf /etc/ld.so.conf.d/gridlabd-$VERSION.conf
#		sudo ldconfig
#	fi

	$VENV_PYTHON_DIR/python3 -m pip install --upgrade pip
	$VENV_PYTHON_DIR/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	$VENV_PYTHON_DIR/python3 -m pip install IPython censusdata
	
	if ! gdal-config --version &> /dev/null ; then
		cd $HOME/temp
		sudo wget download.osgeo.org/gdal/3.0.4/gdal304.zip
		unzip gdal304.zip
		cd gdal-3.0.4
		./configure
		sudo make clean && sudo make && sudo make install
	fi

	# manually set install due to pip not adjusting automatically for debian's limitations
	sudo apt-get update -y
	sudo apt-get install python-numpy gdal-bin libgdal-dev -y
	$VENV_PYTHON_DIR/python3 -m pip install GDAL==3.0.4
	$VENV_PYTHON_DIR/python3 -m pip install rasterio==1.2.10

	cd $REQ_DIR
	$VENV_PYTHON_DIR/python3 -m pip install -r requirements.txt

fi

# check for successful python build
if [ ! -x $VENV_PYTHON_DIR/python3 ]; then
    echo "Could not locate python executable in"
    echo "PYTHON LOCATION: $VENV_PYTHON_DIR"
    echo "Exiting build."
    exit 1
fi

# install latex
apt-get install texlive -y

# doxgygen
apt-get -q install gawk -y
if [ ! -x /usr/bin/doxygen ]; then
	if [ ! -d $VERSION_DIR/src/doxygen ]; then
		git clone https://github.com/doxygen/doxygen.git $VERSION_DIR/src/doxygen
	fi
	if [ ! -d $VERSION_DIR/src/doxygen/build ]; then
		mkdir $VERSION_DIR/src/doxygen/build
	fi
	cd $VERSION_DIR/src/doxygen/build
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
if [ ! -x /usr/local/bin/natural_docs ]; then
	cd /usr/local/bin
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > natural_docs.zip
	unzip -qq natural_docs
	rm -f natural_docs.zip
	mv Natural\ Docs natural_docs
	echo '#!/bin/bash
mono $VERSION_DIR/natural_docs/NaturalDocs.exe \$*' > $VERSION_DIR/bin/natural_docs
	chmod a+x /usr/local/bin/natural_docs
fi
