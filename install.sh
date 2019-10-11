#!/bin/bash

QUICK="no"
INDEX="yes"
CHECK="yes"
FORCE="no"
INDEX="yes"
TEST="no"
LINK="yes"
DOCS="yes"
UPDATE="yes"
PREFIX="/usr/local"
VERSION=`build-aux/version.sh --install`
QUIET="no"
INSTALL="$PREFIX/$VERSION"
VERBOSE="no"

function on_exit()
{
	pkill tail 1>/dev/null 2>&1
}
trap on_exit EXIT

export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH

if [ -f "install.conf" ]; then
	source "install.conf"
fi

LOG="install.log"
rm -f $LOG

function log()
{
	if [ ! -f $LOG ]; then
		echo "Install log started $date by $USER on $(uname -a)" > $LOG
	fi
	echo "$*" >> $LOG
}

function error()
{
	echo "ERROR: $*" | tee -a $LOG > /dev/stderr
	exit 1
}

function info()
{
	echo "CHECK=$CHECK"
	echo "DOCS=$DOCS"
	echo "FORCE=$FORCE"
	echo "INSTALL=$INSTALL"	
	echo "INDEX=$INDEX"
	echo "LINK=$LINK"
	echo "PREFIX=$PREFIX"
	echo "QUIET=$QUIET"
	echo "QUICK=$QUICK"
	echo "TEST=$TEST"
	echo "UPDATE=$UPDATE"
	echo "VERBOSE=$VERBOSE"
	echo "VERSION=$VERSION"
}

function help()
{
	echo "Syntax: $0 <options>"
	echo "Options"
	echo "  --help       Print this helpful output"
	echo "  --info       Print information about this install"
	echo "  --no-check   Do not check system for requirements"
	echo "  --no-index   Do not index data archives"
	echo "  --no-link    Do not link new install to activate it"
	echo "  --no-test    Do not run validation tests"
	echo "  --no-update  Do not update system to meet requirements"
	echo "  --save       Save the current configuration as default"
	echo "  --reset      Reset the configuration to default"
	echo "  --quick      Run only updates instead of a clean install"
	echo "  --quiet      Run without showing commands"
	echo "  --verbose    Run showing log output"
}

while [ $# -gt 0 ]; do
	if [ "$1" == "--quick" ]; then
		QUICK="yes"
	elif [ "$1" == "--no-index" ]; then
		INDEX="no"
	elif [ "$1" == "--no-check" ]; then
		CHECK="no"
	elif [ "$1" == "--no-test" ]; then
		TEST="no"
	elif [ "$1" == "--no-link" ]; then
		LINK="no"
	elif [ "$1" == "--no-docs" ]; then
		DOCS="no"
	elif [ "$1" == "--no-update" ]; then
		UPDATE="no"
	elif [ "$1" == "--quiet" ]; then
		QUIET="yes"
	elif [ "$1" == "--verbose" ]; then
		VERBOSE="yes"
	elif [ "$1" == "--prefix" ]; then
		PREFIX="$2"
		if [ ! -d "$PREFIX" ]; then
			error "$PREFIX does not exist"
		fi
		INSTALL="$PREFIX/$VERSION"
		shift 1
	elif [ "$1" == "--save" ]; then
		info > "install.conf"
		exit 0
	elif [ "$1" == "--reset" ]; then
		rm -f "install.conf"
		exit 0
	elif [ "$1" == "--help" -o "$1" == "-h" ]; then
		help
		exit 0
	elif [ "$1" == "--info" ]; then
		info
		exit 0
	else
		error "'$1' is not a valid option"
		exit 1
	fi
	shift 1
done

log "Install options:"
if [ "$VERBOSE" == "yes" ]; then
	tail -f $LOG &
fi
info >> $LOG
log "Starting install:"

function quiet ()
{
	if [ "$QUIET" == "no" ]; then
		echo $*
	fi
}
function run ()
{
	quiet "$*"
	echo "$PWD% $*  " >> $LOG
	$* >> $LOG 2>&1 || error "$0 failed -- see $LOG for details "
}

# check
SYSTEM=$(uname -s)
if [ -f "$PREFIX/bin/gridlabd" -a ! -L "$PREFIX/bin/gridlabd" ]; then
	error "the existing gridlabd version cannot be managed and must be uninstalled first"
	# TODO: automate this someday
fi
if [ "$CHECK" == "yes" -a -f "requirements-$SYSTEM.sh" ]; then
	run ./requirements-$SYSTEM.sh
fi
if [ "$LINK" == "yes" -a -d "$PREFIX/gridlabd" -a ! -L "$PREFIX/gridlabd" ]; then
    error "$PREFIX/gridlabd exists but it is not a symbolic link"
fi
if [ $(whoami) == "root" -a "$FORCE" == "no" ]; then
    error "do not run $0 as root"
fi
if [ -z "$(which doxygen)" -a "$CHECK" == "yes" ]; then
	error "doxygen is not installed"
fi
if [ -z "$(which mono)" -a "$CHECK" == "yes" ]; then
	error "mono is not installed"
fi
if [ -z "$(which natural_docs)" -a "$CHECK" == "yes" ]; then
	error "natural_docs is not installed"
fi

# autoconf
if [ "$QUICK" == "no" ]; then
	run autoscan
fi
if [ ! -d autom4te.cache -o "$QUICK" == "no" ]; then
    run autoreconf -isf
fi

# prep install dir
run sudo rm -rf "$INSTALL"
run sudo mkdir -p "$INSTALL"
run sudo chown -R "$USER" "$INSTALL"
if [ ! -f "configure" -o "$QUICK" == "no" ]; then
    run ./configure --prefix="$INSTALL" $*
fi

# build everything
export PATH=$INSTALL/bin:/usr/local/bin:/usr/bin:/bin
run make -j30
run make install
if [ "$DOCS" == "yes" ]; then
	run make html 
	run cp -r "documents/html" "$INSTALL"
	run cp "documents/index.html" "$INSTALL/html"
fi
if [ "$INDEX" == "yes" ]; then
	run make index
fi
if [ "$TEST" == "yes" ]; then
	run gridlabd --validate
fi

# activate this version
if [ -x "$INSTALL/bin/gridlabd-version" -a "$TEST" == "yes" ]; then
	run gridlabd version set "$VERSION"
elif [ "$LINK" == "yes" ]; then
	run sudo rm -f "$PREFIX/gridlabd"
	run sudo ln -s "$INSTALL" "$PREFIX/gridlabd"
	if [ ! -f "$PREFIX/bin/gridlabd" ]; then
		run sudo ln -s "$PREFIX/gridlabd/bin/gridlabd" "$PREFIX/bin/gridlabd"
	fi
fi