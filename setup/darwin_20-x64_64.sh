set -x
alias INSTALL=''

INSTALL PYTHON_VERSION=3.10
INSTALL PYTHON_VENV=${HOME:-/tmp}/.gridlabd
INSTALL PYTHON_EXEC=$PYTHON_VENV/bin/python$PYTHON_VERSION
INSTALL PYTHON_CONFIG=$PYTHON_VENV/bin/python${PYTHON_VERSION}-config

# check for root access
if [ "$(whoami)" = "root" ]; then
    echo "ERROR: you should not run setup.sh as root or use sudo" > /dev/stderr
    exit 1
fi

# prepare brew for installations
INSTALL brew update

# setup required python version if not already installed
if ! python$PYTHON_VERSION --version 1>/dev/null 2>&1 ; then
    echo "Installing python${PYTHON_VERSION}..."
    INSTALL brew install python@$PYTHON_VERSION
    python$PYTHON_VERSION --version || ( echo "ERROR: python$PYTHON_VERSION installation failed" > /dev/stderr ; exit 1 )
fi
if ! python$PYTHON_VERSION -m venv -h >/dev/null ; then
    INSTALL brew install python$PYTHON_VERSION-venv -y
    python$PYTHON_VERSION -m venv -h >/dev/null || ( echo "ERROR: unable to install python$PYTHON_VERSION-venv" ; exit 1 )
fi
if ! python$PYTHON_VERSION -m distutils.core >/dev/null ; then

    brew install python$PYTHON_VERSION-distutils -y

# create python venv for setup if not already done
if [ ! -x "$PYTHON_EXEC" ] ; then
    INSTALL python$PYTHON_VERSION -m venv --symlinks $PYTHON_VENV
    test -x "$PYTHON_EXEC" || ( echo "ERROR: python venv creation failed" > /dev/stderr ; exit 1 )
fi

# activate the build environment for python
INSTALL . $PYTHON_VENV/bin/activate

# upgrade pip if needed
if ! "$PYTHON_EXEC" -m pip --version 1>/dev/null 2>&1 ; then
    INSTALL curl -fsL https://bootstrap.pypa.io/get-pip.py | python$PYTHON_VERSION
    INSTALL "$PYTHON_EXEC" -m pip --version || (  echo "ERROR: pip installation failed" > /dev/stderr ; exit 1 )
fi

# check gdal
if ! gdal-config --version 1>/dev/null 2>&1 ; then
    brew install libgdal-dev -y
    gdal-config --version || ( echo "ERROR: libgdal-dev installation failed" > /dev/stderr ; exit 1 )
fi

# install python-config
if ! "python$PYTHON_VERSION-config" --prefix 1>/dev/null 2>&1 ; then
    INSTALL brew install python$PYTHON_VERSION-dev -y
    python$PYTHON_VERSION-config --prefix || ( echo "ERROR: python$PYTHON_VERSION-config installation failed" > /dev/stderr ; exit 1 )
fi
INSTALL "$PYTHON_EXEC" -m pip install --upgrade pip || ( echo "ERROR: pip update failed" > /dev/stderr ; exit 1 )

# install required libraries
INSTALL brew install build-essential zlib1g-dev libcurl4-gnutls-dev libncurses5-dev liblzma-dev libbz2-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev -y

# install required tools
INSTALL brew install git unzip libtool g++ cmake flex bison subversion util-linux xz-utils wget -y

# update library paths
INSTALL ldconfig

# install autoconf 2.71 as required
if [ "$(autoconf --version | head -n 1 | cut -f4 -d' ')" != "2.71" ] ; then
    (cd /tmp ; curl -sL https://ftpmirror.gnu.org/autoconf/autoconf-2.71.tar.gz | tar xz )
    (cd /tmp/autoconf-2.71 ; ./configure ; make ; make install)
    test "$(autoconf --version | head -n 1 | cut -f4 -d' ')" = "2.71" || (echo "ERROR: autoconf installation failed" > /dev/stderr ; exit 1 )
fi
