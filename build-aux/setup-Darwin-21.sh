#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin/


brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)" -y

brew update && brew upgrade && brew cleanup && brew doctor

# brew doctor -y
# echo 'eval $(/opt/homebrew/bin/brew shellenv)' >> /Users/668/.zprofile
eval $(/opt/homebrew/bin/brew shellenv)
# build tools
brew install autoconf automake libtool gnu-sed gawk openssl



[ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
[ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed
[ ! -e /usr/local/bin/libtoolize ] && ln -s /usr/local/bin/glibtoolize /usr/local/bin/libtoolize

if [ ! -x /usr/local/bin/python3 -o "$(/usr/local/bin/python3 --version)" != "Python 3.9.6" ]; then
	echo "install python 3.9.6"	
	
    # if [ ! -x /usr/local/src ]; then
    #     sudo mkdir /usr/local/src
    # fi
	# cd /usr/local/src
	# brew install gcc openssl bzip2 libffi zlib  xz
    # if [ ! -x /usr/local/src/Python-3.9.6 ]; then
    #      sudo mkdir /usr/local/src/Python-3.9.6
    # fi
	# sudo curl -o Python-3.9.6.tgz https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz 
    sudo tar -xvzf Python-3.9.6.tgz
	# cd /usr/local/src/Python-3.9.6
	sudo ./configure --prefix=/usr/local --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
	# make -j $(nproc)
	# make altinstall
	
    # find current python
    # pyPath=$(which python)
    # basePath=$(dirname ${pyPath})
    # echo ${pyPath}
    # ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
	# ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
	# ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
	# ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
	# ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3
	ln -sf /opt/homebrew/bin/python3.9 /usr/local/bin/python3
	

	ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
	ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
	ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
	ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
	ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3
	# install python packages
	/usr/local/bin/python3 -m pip install --upgrade pip
	/usr/local/bin/python3 -m pip install mysql-connector matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd 
	/usr/local/bin/python3 -m pip install IPython censusdata

	ln -sf /opt/homebrew/bin/python3 /usr/local/bin/python3
	ln -sf /opt/homebrew/bin/python3.9-config /usr/local/bin/python3-config
	ln -sf /opt/homebrew/bin/pydoc3.9 /usr/local/bin/pydoc
	ln -sf /opt/homebrew/bin/idle3.9 /usr/local/bin/idle
	ln -sf /opt/homebrew/bin/pip3.9 /usr/local/bin/pip3

	ln -sf /Users/jimmyleu/Development/gismo/gridlabd/test-env/bin/pip3.8 /usr/local/bin/pip3
fi

brew install openblas
 python3 -m venv test-env
 source test-env/bin/activate
# brew install python@3.9
# pip install --upgrade pip
# pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
# pip install IPython censusdata

# pythonPath=$(which python)

# ln -sf /usr/local/bin/python3.9 ${pythonPath}

# echo "check docker .."
# #check docker installation
# if [[ $(which docker) && $(docker --version) ]]; then
#     echo "docker is installed "
#     # command
# else
#     error "docker is not installed"
#     # command
# fi

# # check docker service 
# if ! docker info > /dev/null 2>&1; then
#     error "This script uses docker, and it isn't running - please start docker and try again!"
# fi

# echo "End of check docker"

# build tools
# brew install autoconf automake libtool gnu-sed gawk
# [ ! -L /usr/local/bin/sed -o ! "$(readlink /usr/local/bin/sed)" == "/usr/local/bin/gsed" ] && mv /usr/local/bin/sed /usr/local/bin/sed-old
# [ ! -e /usr/local/bin/sed ] && ln -s /usr/local/bin/gsed /usr/local/bin/sed
# [ ! -e /usr/local/bin/libtoolize ] && ln -s /usr/local/bin/glibtoolize /usr/local/bin/libtoolize

# install python3
# brew install python@3.9

sudo -H autoreconf -isf && ./configure
sudo -H make -j6 system 

OPENBLAS="$(brew --prefix openblas)" pip3 install numpy pandas


~/Library/Caches/pip

 make reconfigure  