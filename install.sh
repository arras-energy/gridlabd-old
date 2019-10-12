#!/bin/bash

# set the path to use during installation
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# setup logging
LOG="install.log"
rm -f $LOG
function log()
{
	echo "$*" >> $LOG
}

# setup exit handling
function on_exit()
{
	pkill tail 1>/dev/null 2>&1
	log "STOP: $(date)"
	log "STATUS: ${STATUS:-ok}"
}
trap on_exit EXIT

# start logging
log "START: $(date)"
log "COMMAND: $0 $*"
log "CONTEXT: ${USER:-unknown}@${HOSTNAME:-localhost}:$PWD"
log "SYSTEM: $(uname -a)"

# setup error handling
function error()
{
	echo "ERROR: $*" | tee -a $LOG > /dev/stderr
	STATUS="error"
	exit 1
}

# check for commands that absolutely necessary to proceed
function require()
{
	$1 ${2:---version} > /dev/null 2>&1 || error "$1 is required"
}
require git
if [ "$(whoami)" == "root" ]; then
	function sudo () 
	{
		$*
	}
else
	require sudo
fi

if [ ! -f install.conf-default ]; then
	error "ERROR: missing install.conf-default"
fi

# load the configuration
source install.conf-default
if [ -f "install.conf" ]; then
	source "install.conf"
fi

# define commands that used by command line options
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
    echo "SETUP=$SETUP"
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
	echo "  --force      Force install into target folder"
	echo "  --no-check   Do not check system for requirements"
	echo "  --no-index   Do not index data archives"
	echo "  --no-link    Do not link new install to activate it"
	echo "  --no-test    Do not run validation tests"
	echo "  --no-update  Do not update system to meet requirements"
	echo "  --parallel   Enable parallelism when possible"
	echo "  --save       Save the current configuration as default"
    echo "  --setup      Perform system setup"
	echo "  --reset      Reset the configuration to default"
	echo "  --quick      Run only updates instead of a clean install"
	echo "  --quiet      Run without showing commands"
	echo "  --verbose    Run showing log output"
}

# process command line options
while [ $# -gt 0 ]; do
	if [ "$1" == "--quick" ]; then
		QUICK="yes"
	elif [ "$1" == "--validate" ]; then
		TEST="yes"
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
	elif [ "$1" == "--parallel" ]; then
		PARALLEL="yes"
	elif [ "$1" == "--prefix" ]; then
		PREFIX="$2"
		if [ ! -d "$PREFIX" ]; then
			error "$PREFIX does not exist"
		fi
		INSTALL="$PREFIX/$VERSION"
		shift 1
    elif [ "$1" == "--setup" ]; then
        SETUP="yes"
    elif [ "$1" == "--force" ]; then
    	FORCE="yes"
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

log "CONFIGURATION: {"
if [ "$VERBOSE" == "yes" ]; then
	tail -f -n 100 $LOG 2>/dev/null &
fi
info | sed -e '1,$s/\(.*\)=\(.*\)/\t\1: \2/' >> $LOG
log "}"

# define functions used during install processing
function quiet ()
{
	if [ "$QUIET" == "no" -a "$VERBOSE" == "no" ]; then
		echo $*
	fi
}
function run ()
{
	quiet "$*"
	NAME=${USER:-nobody}
	CMD=($(ps -ocommand $$ | tail -n 1))
	CMD=$(basename ${CMD[0]})
	if [ $(whoami) == "root" ]; then
		NAME="root"
	fi
	echo "RUN: [$CMD://$NAME@$HOSTNAME$PWD] $*" >> $LOG
	$* >> $LOG 2>&1 || error "$0 failed -- see $LOG for details "
}

# run setup
SYSTEM=$(uname -s)
if [ "$SETUP" == "yes" ]; then
    if [ ! -f "build-aux/setup-$SYSTEM.sh" ]; then
        error "unable to setup $SYSTEM, build-aux/setup-$SYSTEM.sh not found"
    fi
	run sudo build-aux/setup-$SYSTEM.sh
	exit 0
fi

# run checks
if [ "$LINK" == "yes" -a -f "$PREFIX/bin/gridlabd" -a ! -L "$PREFIX/bin/gridlabd" ]; then
	error "the existing gridlabd version cannot be managed and must be uninstalled first"
	# TODO: automate this someday
fi
if [ "$CHECK" == "yes" ]; then
	if [ "$LINK" == "yes" -a -d "$PREFIX/gridlabd" -a ! -L "$PREFIX/gridlabd" ]; then
	    error "$PREFIX/gridlabd exists but it is not a symbolic link"
	fi
	if [ $(whoami) == "root" -a "$FORCE" == "no" ]; then
	    error "running $0 as root is not recommended, use --force to override this restriction"
	fi
	if [ "$DOCS" == "yes" ]; then
		require mono
		require doxygen
		require natural_docs
	fi
fi

# run autoconf
if [ "$QUICK" == "no" ]; then
	if [ ! -f "build-aux/ltmain.sh" ]; then
		require libtoolize
		run libtoolize
	fi
	require autoscan
	run autoscan
fi
if [ ! -d "autom4te.cache" -o "$QUICK" == "no" ]; then
	require autoreconf
    run autoreconf -isf
fi

# prep install dir
if [ -e "$INSTALL" -a "$FORCE" == "no" ]; then
	error "$INSTALL already exists, please delete it first"
fi
run sudo mkdir -p "$INSTALL"
if [ ! "${USER:-root}" == "root" ]; then
	run sudo chown -R "$USER" "$INSTALL"
fi
if [ ! -f "configure" -o "$QUICK" == "no" ]; then
    run ./configure --prefix="$INSTALL" $*
fi

NPROC=1
if [ "$PARALLEL" == "yes" ]; then
	if [ "$SYSTEM" == "Linux" ]; then
		NPROC=$(lscpu | grep '^CPU(s):' | cut -f2- -d' ')
	elif [ "$SYSTEM" == "Darwin" ]; then
		NPROC=$(sysctl -n machdep.cpu.thread_count)
	fi
fi

# build everything
export PATH=$INSTALL/bin:/usr/local/bin:/usr/bin:/bin
run make -j$((3*$NPROC))
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
	export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-.}
	run gridlabd -T $NPROC --validate
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

# all done :-)
exit 0