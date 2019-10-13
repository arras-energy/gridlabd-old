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

# check workdir
require dirname
cd $(dirname $0)
if [ ! -f "install.sh" -o ! -d "build-aux" ]; then
	error "$0 must be run from the source root"
fi

# load the configuration
if [ ! -f "install.conf-default" ]; then
	error "missing install.conf-default"
fi
source "install.conf-default"
if [ -f "install.conf" ]; then
	source "install.conf"
fi

# define commands that used by command line options
function info()
{
	echo "CHECK=$CHECK"
	echo "DOCS=$DOCS"
	echo "FORCE=$FORCE"
	echo "INDEX=$INDEX"
	echo "LINK=$LINK"
	echo "PREFIX=$PREFIX"
	echo "QUICK=$QUICK"
    echo "SETUP=$SETUP"
	echo "SILENT=$SILENT"
	echo "TEST=$TEST"
	echo "UPDATE=$UPDATE"
	echo "VERBOSE=$VERBOSE"
}

function help()
{
	echo "Syntax: $0 <options>"
	echo "Options"
	echo "  -h|--help       Print this helpful output"
	echo "  --info          Print information about this install"
	echo "  -c|--no-check   Do not check system for requirements"
	echo "  -d|--no-docs    Do not install documentation"
	echo "  -f|--force      Force install into existing target folder"
	echo "  -i|--no-index   Do not index data archives"
	echo "  -l|--no-link    Do not link new install to activate it"
	echo "  -t|--no-test    Do not run validation tests"
	echo "  -u|--no-update  Do not update system to meet requirements"
	echo "  -p|--parallel   Enable parallelism when possible"
	echo "  --prefix path   Set install prefix"
	echo "  --save          Save the current configuration as default"
    echo "  --setup         Perform system setup"
	echo "  --reset         Reset the configuration to default"
	echo "  -q|--quick      Run only updates instead of a clean install"
	echo "  -s|--silent     Run without showing commands"
	echo "  -v|--verbose    Run showing log output"
	echo "  --validate      Run validation tests"
	echo "  --install path  Override the default installation path"
	echo "  --version name  Override the default version name"
}

# process command line options
while [ $# -gt 0 ]; do
	case "$1" in
	(-q|--quick)
		QUICK="yes"
		;;
	(--validate)
		TEST="yes"
		;;
	(-i|--no-index)
		INDEX="no"
		;;
	(-c|--no-check)
		CHECK="no"
		;;
	(-t|--no-test)
		TEST="no"
		;;
	(-l|--no-link)
		LINK="no"
		;;
	(-d|--no-docs)
		DOCS="no"
		;;
	(-u|--no-update)
		UPDATE="no"
		;;
	(-s|--silent)
		SILENT="yes"
		;;
	(-v|--verbose)
		VERBOSE="yes"
		;;
	(-p|--parallel)
		PARALLEL="yes"
		;;
	(--prefix)
		PREFIX="$2"
		if [ ! -d "$PREFIX" ]; then
			error "$PREFIX does not exist"
		fi
		INSTALL="$PREFIX/$VERSION"
		shift 1
		;;
    (--no-setup)
        SETUP="no"
        ;;
    (-f|--force)
    	FORCE="yes"
    	;;
	(--save)
		info > "install.conf"
		exit 0
		;;
	(--reset)
		rm -f "install.conf"
		exit 0
		;;
	(-h|--help)
		help
		exit 0
		;;
	(--info)
		info
		exit 0
		;;
	(--version)
		VERSION="$2"
		shift 1
		;;
	(--install)
		INSTALL="$2"
		shift 1
		;;
	(*)
		error "'$1' is not a valid option"
		exit 1
		;;
	esac
	shift 1
done

# start logging
log "START: $(date)"
log "COMMAND: $0 $*"
log "CONTEXT: ${USER:-unknown}@${HOSTNAME:-localhost}:$PWD"
log "SYSTEM: $(uname -a)"
log "CONFIGURATION: {"
info | sed -e '1,$s/\(.*\)=\(.*\)/\t\1: \2/' >> $LOG
log "}"

# check sudo
if [ "$(whoami)" == "root" ]; then
	function sudo () 
	{
		$*
	}
else
	require sudo
fi

# define functions used during install processing
function run ()
{
	if [ "$SILENT" == "no" -a "$VERBOSE" == "no" ]; then
		echo $*
	fi
	NAME=${USER:-nobody}
	CMD=($(ps -ocommand $$ | tail -n 1))
	CMD=$(basename ${CMD[0]})
	if [ $(whoami) == "root" ]; then
		NAME="root"
	fi
	echo "RUN: [$CMD://$NAME@$HOSTNAME$PWD] $*" >> $LOG
	$* >> $LOG 2>&1 || error "$0 failed -- see $LOG for details "
}

# enable verbose logging
if [ "$VERBOSE" == "yes" ]; then
	tail -f -n 10000 $LOG 2>/dev/null &
fi

# run setup
if [ "$SETUP" == "yes" ]; then
    if [ ! -f "build-aux/setup.sh" ]; then
        error "build-aux/setup.sh not found"
    fi
	run sudo build-aux/setup.sh
fi

# dynamic variables
require git
VERSION=${VERSION:-`build-aux/version.sh --install`}
INSTALL=${INSTALL:-$PREFIX/$VERSION}

# run checks
if [ "$LINK" == "yes" -a -f "$PREFIX/bin/gridlabd" -a ! -L "$PREFIX/bin/gridlabd" ]; then
	error "the existing gridlabd version cannot be managed and must be uninstalled first"
	# TODO: automate this someday
fi
if [ "$CHECK" == "yes" ]; then
	if [ "$LINK" == "yes" -a -d "$PREFIX/gridlabd" -a ! -L "$PREFIX/gridlabd" ]; then
	    error "$PREFIX/gridlabd exists but it is not a symbolic link"
	fi
	# if [ $(whoami) == "root" -a "$FORCE" == "no" ]; then
	#     error "running $0 as root is not recommended, use --force to override this restriction"
	# fi
	if [ "$DOCS" == "yes" ]; then
		require doxygen
		require mono
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
	run $INSTALL/bin/gridlabd -T $NPROC --validate
fi

# activate this version
if [ -x "$INSTALL/bin/gridlabd-version" -a "$TEST" == "yes" ]; then
	run $INSTALL/bin/gridlabd version set "$VERSION"
elif [ "$LINK" == "yes" ]; then
	run sudo rm -f "$PREFIX/gridlabd"
	run sudo ln -s "$INSTALL" "$PREFIX/gridlabd"
	if [ ! -f "$PREFIX/bin/gridlabd" ]; then
		run sudo ln -s "$PREFIX/gridlabd/bin/gridlabd" "$PREFIX/bin/gridlabd"
	fi
fi

# all done :-)
exit 0