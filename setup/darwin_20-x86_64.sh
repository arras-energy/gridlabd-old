set -x
alias INSTALL=''

INSTALL error () { echo "ERROR [darwin_20-x86_64.sh]: $*" > /dev/stderr ; exit 1 ; }
INSTALL PYTHON_VERSION=3.10
INSTALL PYTHON_VENV=${HOME:-/tmp}/.gridlabd
INSTALL PYTHON_EXEC=$PYTHON_VENV/bin/python$PYTHON_VERSION
INSTALL PYTHON_CONFIG=$PYTHON_VENV/bin/python${PYTHON_VERSION}-config

# check for root access
INSTALL test "$(whoami)" != "root" || error "you should not run setup.sh as root or use sudo"

# prepare brew for installations
INSTALL brew -h 1>/dev/null 2>&1 || error "you must install brew first. See https://brew.sh for details."

# setup requires python version if not already installed
if ! python$PYTHON_VERSION --version 1>/dev/null 2>&1 ; then
    INSTALL brew install python@$PYTHON_VERSION
    python$PYTHON_VERSION --version || error "python$PYTHON_VERSION installation failed"
fi
if ! python$PYTHON_VERSION -m venv -h 1>/dev/null 2>&1 ; then
    printf "installing... "
    INSTALL brew install python$PYTHON_VERSION-venv
    python$PYTHON_VERSION -m venv -h >/dev/null || error "unable to install python$PYTHON_VERSION-venv"
fi
if ! python$PYTHON_VERSION -m distutils.core 1>/dev/null 2>&1 ; then
    INSTALL brew install python$PYTHON_VERSION-distutils
    python$PYTHON_VERSION -m distutils.core >/dev/null || error "unable to install python$PYTHON_VERSION-distutils"
fi

# create python venv for setup if not already done
if [ ! -x "$PYTHON_EXEC" ] ; then
    INSTALL python$PYTHON_VERSION -m venv --symlinks $PYTHON_VENV
    test -x "$PYTHON_EXEC" || error "python venv creation failed"
fi

# activate the build environment for python
INSTALL . $PYTHON_VENV/bin/activate
test ! -z "$VIRTUAL_ENV" || error "python venv activation failed"

# upgrade pip if needed
if ! "$PYTHON_EXEC" -m pip --version 1>/dev/null 2>&1 ; then
    INSTALL curl -fsL https://bootstrap.pypa.io/get-pip.py | python$PYTHON_VERSION
    INSTALL "$PYTHON_EXEC" -m pip --version || error "pip installation failed"
fi

# install python-config
if ! "python$PYTHON_VERSION-config" --prefix 1>/dev/null 2>&1 ; then
    INSTALL brew install python$PYTHON_VERSION-dev
    python$PYTHON_VERSION-config --prefix || error "python$PYTHON_VERSION-config installation failed"
fi
INSTALL "$PYTHON_EXEC" -m pip install --upgrade pip || error "pip update failed"

# install required libraries
INSTALL brew install autoconf libffi zlib pkg-config xz gdbm tcl-tk

# install required tools
INSTALL brew install automake libtool gnu-sed gawk

clang -v >/dev/null || error "you have not installed clang. Use 'xcode-select --install' to install command line build tools."

# # update library paths
# INSTALL ldconfig

# install autoconf 2.71 as required
if [ "$(autoconf --version | head -n 1 | cut -f4 -d' ')" != "2.71" ] ; then
    (cd /tmp ; curl -sL https://ftpmirror.gnu.org/autoconf/autoconf-2.71.tar.gz | tar xz )
    (cd /tmp/autoconf-2.71 ; ./configure ; make ; make install)
    test "$(autoconf --version | head -n 1 | cut -f4 -d' ')" = "2.71" || error "autoconf installation failed"
fi
