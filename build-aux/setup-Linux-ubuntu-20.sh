#!/bin/bash

# Set version and paths, using these vars will make future maintenance much better. #Automation
    VERSION=${VERSION:-`build-aux/version.sh --name`}
    VAR="/usr/local/opt/gridlabd"
    VERSION_DIR=$VAR/gridlabd/$VERSION
    PYTHON_VER=3.9.6
    PY_EXE=3.9
	REQ_DIR=$(pwd)

# Install needed system tools
# update first
sudo apt-get -q update

sudo apt-get install tzdata -y
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
sudo apt install libgdal-dev -y

# Update Autoconf to 2.71 manually as apt-get does not track the latest version
	if [ ! -e $HOME/temp ]; then
		mkdir $HOME/temp
	fi
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

# Install python $PYTHON_VER
# python3 support needed as of 4.2
if [ ! -x $VERSION_DIR/bin/python3 -o "$($VERSION_DIR/bin/python3 --version | cut -f3 -d.)" != "Python $PY_EXE" ]; then
	echo "installing python $PYTHON_VER and ssl module dependencies"
	cd $VERSION_DIR/src

	curl https://www.python.org/ftp/python/$PYTHON_VER/Python-$PYTHON_VER.tgz | tar xz

	# tar xzf Python-$PYTHON_VER.tgz
	cd $VERSION_DIR/src/Python-$PYTHON_VER

	./configure --prefix=$VERSION_DIR --enable-shared --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"

	make -j $(nproc)
	make install
	/sbin/ldconfig $VERSION_DIR/lib
	ln -sf $VERSION_DIR/bin/python3.9 $VERSION_DIR/bin/python3
	ln -sf $VERSION_DIR/bin/python3.9-config $VERSION_DIR/bin/python3-config
	ln -sf $VERSION_DIR/bin/pydoc3.9 $VERSION_DIR/bin/pydoc
	ln -sf $VERSION_DIR/bin/idle3.9 $VERSION_DIR/bin/idle
	ln -sf $VERSION_DIR/bin/pip3.9 $VERSION_DIR/bin/pip3

	if [ ! -e /etc/ld.so.conf.d/gridlabd.conf ]; then
		sudo touch /etc/ld.so.conf.d/gridlabd.conf
		sudo bash -c 'echo "$VERSION_DIR/lib" >> /etc/ld.so.conf.d/gridlabd.conf'
		sudo ldconfig
	fi

	$VERSION_DIR/bin/python3 -m pip install --upgrade pip
	$VERSION_DIR/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
	$VERSION_DIR/bin/python3 -m pip install IPython censusdata
	
	if ! gdal-config --version &> /dev/null ; then
		cd $HOME/temp
		sudo wget download.osgeo.org/gdal/3.0.4/gdal304.zip
		unzip gdal304.zip
		cd gdal-3.0.4
		./configure
		sudo make clean && sudo make && sudo make install
	fi

	# manually set install due to pip not adjusting automatically for ubuntu's limitations
	sudo add-apt-repository ppa:ubuntugis/ppa -y
	sudo apt-get update -y
	sudo apt-get install gdal-bin -y
	$VERSION_DIR/bin/python3 -m pip install GDAL==3.0.4
	$VERSION_DIR/bin/python3 -m pip install rasterio==1.2.10

	cd $REQ_DIR
	$VERSION_DIR/bin/python3 -m pip install -r requirements.txt

fi

# check for successful python build
if [ ! -x $VERSION_DIR/bin/python${PY_EXE} ]; then
    echo "Could not locate python executable in"
    echo "PYTHON LOCATION: $VERSION_DIR/bin/python${PY_EXE}"
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
if [ ! -x $VERSION_DIR/bin/natural_docs ]; then
	cd $VERSION_DIR
	curl https://www.naturaldocs.org/download/natural_docs/2.0.2/Natural_Docs_2.0.2.zip > natural_docs.zip
	unzip -qq natural_docs
	rm -f natural_docs.zip
	mv Natural\ Docs natural_docs
	echo '#!/bin/bash
mono $VERSION_DIR/natural_docs/NaturalDocs.exe \$*' > $VERSION_DIR/bin/natural_docs
	chmod a+x $VERSION_DIR/bin/natural_docs
fi
