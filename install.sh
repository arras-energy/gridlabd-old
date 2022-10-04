#!/bin/bash

# set the path to use during installation
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# check sudo
if [ "$(whoami)" == "root" ]; then
	function sudo () 
	{
		$*
	}
else
	sudo --version >/dev/null 2>&1 || (echo "$0: sudo is required"; exit 1)
fi

VERSION=${VERSION:-`build-aux/version.sh --name`}

# local folder
VAR="/usr/local/opt/gridlabd"
if [ ! -d "$VAR" ]; then
	mkdir -p $VAR || ( sudo mkdir -p $VAR && sudo chown ${USER:-root} $VAR )
	mkdir -p $VAR/bin
	mkdir -p $VAR/include
	mkdir -p $VAR/lib
	mkdir -p $VAR/man
	mkdir -p $VAR/share
	mkdir -p $VAR/src
	mkdir -p $VAR/var
	mkdir -p $VAR/opt
	mkdir -p $VAR/etc
fi

# create a temp working directory if it does not already exist
if [ ! -d "$HOME/temp" ]; then
	mkdir -p $HOME/temp || ( sudo mkdir -p $HOME/temp && sudo chown ${USER:-root} $HOME/temp )
fi

cp ./build-aux/python3.sh $VAR/bin/python3.sh

if [ ! -d "$VAR/gridlabd/$VERSION" ]; then
    mkdir -p $VAR/gridlabd/$VERSION/bin
	mkdir -p $VAR/gridlabd/$VERSION/include
	mkdir -p $VAR/gridlabd/$VERSION/lib
	mkdir -p $VAR/gridlabd/$VERSION/man
	mkdir -p $VAR/gridlabd/$VERSION/share
	mkdir -p $VAR/gridlabd/$VERSION/src
	mkdir -p $VAR/gridlabd/$VERSION/var
	mkdir -p $VAR/gridlabd/$VERSION/opt
	mkdir -p $VAR/gridlabd/$VERSION/etc
fi

if ! grep -q "$VAR/bin" "$HOME/.bashrc"; then
    touch "$HOME/.bashrc"
    echo "export PATH=$VAR/bin:\$PATH" >> $HOME/.bashrc
fi
export PATH=$VAR/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# setup logging
LOG="$VAR/var/install.log"
function log()
{
	case "$*" in 
	("clear")
		rm -f $LOG
		shift 1
		;;
	(*)
		echo "$*" >> $LOG
		return
		;;
	esac
}

# setup exit handling
function on_exit()
{
	log "STOP: $(date)"
	log "STATUS: ${STATUS:-ok}"
	sleep 1
	if [ "$VERSION" == "yes" ]; then
		kill %1
	fi
}
trap on_exit EXIT

# setup error handling
function error()
{
	[ "${SILENT:-no}" == "no" -a "${VERBOSE:-no}" == "no" ] && echo "ERROR: $*" > /dev/stderr
	echo "ERROR: $*" >> $LOG 
	STATUS="error"
	exit 1
}

# check for commands that absolutely necessary to proceed
function require()
{
	$1 ${2:---version} > /dev/null 2>&1 || error "$1 is required"
}

# check workdir
require dirname -
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
    echo "PARALLEL=$PARALLEL"
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
	cat <<-END 
	Syntax: $0 [<install-options>] [<configure-options>]
	Install options:
	  -h   --help            Print this helpful output
	       --info            Print information about this install
	  -c   --no-check        Do not check system for requirements
	  -d   --no-docs         Do not install documentation
	  -f   --force           Force install into existing target folder
	  -i   --no-index        Do not index data archives
	  -l   --no-link         Do not link new install to activate it
	  -t   --no-test         Do not run validation tests
	  -u   --no-update       Do not update system to meet requirements
	  -p   --parallel        Enable parallelism when possible
	       --prefix <path>   Set install prefix
	       --save            Save the current configuration as default
	       --no-setup        Perform system setup
	       --reset           Reset the configuration to default
	  -q   --quick           Run only updates instead of a clean install
	  -s   --silent          Run without showing commands
	  -v   --verbose         Run showing log output
	       --validate        Run validation tests
	       --version <name>  Override the default version name
	END
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
log clear
log "START: $(date)"
log "COMMAND: $0 $*"
log "CONTEXT: ${USER:-unknown}@${HOSTNAME:-localhost}:$PWD"
log "SYSTEM: $(uname -a)"
log "$(info | sed -e '1,$s/=/: /')"

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
	T0=$(date '+%s')
	$* >> $LOG 2>&1 || error "$0 failed -- see $LOG for details "
	T1=$(date '+%s')
	log "OK: done in $((T1-T0)) seconds"
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
	SOK="$VAR/gridlabd/$VERSION/setup.ok"
    if [ ! -f "$SOK" -o "$FORCE" == "yes" ]; then
		run build-aux/setup.sh
		date > "$SOK"
		# update permissions for site-packages to be writable by user post-setup, unless root
		if [ ! -z $USER ] ; then
			sudo chown -R "${USER:-root}" /usr/local/opt/gridlabd/gridlabd/$VERSION/lib/python3*/site-packages
			echo "Setting site-packages to ${USER:-root}"
		else
			echo "Running as root, not updating site package ownership."
		fi
	elif [ -f "$SOK" ]; then
		log "SETUP: already completed at $(cat $SOK)"
	else
		log "SETUP: skipping"
	fi
fi

require git
# dynamic variables
INSTALL=${INSTALL:-$PREFIX/gridlabd/$VERSION}

# run checks
if [ "$LINK" == "yes" -a -f "$PREFIX/bin/gridlabd" -a ! -L "$PREFIX/bin/gridlabd" ]; then
	OLDVER="$(gridlabd --version | cut -f2 -d' ')-saved_$(date '+%y%m%d')"
	VDIR=$OLDVER
	while [ -e $PREFIX/gridlabd/$VDIR ]; do
		VDIR=${OLDVER}_${VNUM:-0}
		VNUM=$((${VNUM:-0}+1))
	done
	log "BACKUP: saving $OLDVER to $PREFIX/gridlabd/$VDIR and linking it back to current version"
	run mkdir -p $PREFIX/gridlabd/$VDIR || error "unable to create $VDIR to save current version"
	run ln -sf $PREFIX/gridlabd/$VDIR $PREFIX/gridlabd/current 
	for item in bin include lib share; do
		[ ! -d $PREFIX/gridlabd/$VDIR/$item ] && run mkdir -p $PREFIX/gridlabd/$VDIR/$item
		[ ! -d $PREFIX/gridlabd/$VDIR/$item ] && run mv $PREFIX/$item/gridlabd* $PREFIX/gridlabd/$VDIR/$item/
		run ln -sf $PREFIX/gridlabd/$VDIR/$item/gridlabd $PREFIX/gridlabd/current/$item/gridlabd
		run ln -sf $PREFIX/gridlabd/current/$item /$PREFIX/$item/gridlabd
	done
	run ln -s $PREFIX/gridlabd/current/bin/gridlabd.bin $PREFIX/bin/gridlabd.bin
	run ln -s /usr/local/opt/gridlabd/bin/gridlabd /usr/local/bin
	error "stopping here for debugging reasons -- this error message should be deleted"
fi
if [ "$CHECK" == "yes" ]; then
	if [ "$LINK" == "yes" -a -d "$PREFIX/gridlabd/current" -a ! -L "$PREFIX/gridlabd/current" ]; then
	    error "$PREFIX/gridlabd/current exists but it is not a symbolic link"
	fi
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
log "VERSION: $VERSION"
log "INSTALL: $INSTALL"
if [ -e "$INSTALL/bin/gridlabd" -a "$FORCE" == "no" ]; then
	error "$INSTALL has already been installed, please delete it first"
fi
mkdir -p "$INSTALL" || ( run sudo mkdir -p "$INSTALL" && run sudo chown -R "${USER:-root}" "$INSTALL" )
if [ ! -f "configure" -o "$QUICK" == "no" ]; then
    run ./configure --prefix="$INSTALL" $*
fi

NPROC=1
SYSTEM=$(uname -s)
if [ "$PARALLEL" == "yes" ]; then
	if [ "$SYSTEM" == "Linux" ]; then
		NPROC=$(lscpu | grep '^CPU(s):' | cut -f2- -d' ')
	elif [ "$SYSTEM" == "Darwin" ]; then
		NPROC=$(sysctl -n machdep.cpu.thread_count)
	fi
fi

# build everything
export PATH=$VAR/bin:/usr/local/bin:/usr/bin:/bin
run make -j$((3*$NPROC)) system

if [ "$DOCS" == "yes" ]; then
	run make html 
	run cp -r "documents/html" "$INSTALL"
	run cp "documents/index.html" "$INSTALL/html"
fi
if [ "$INDEX" == "yes" ]; then
	run make index
fi
if [ "$TEST" == "yes" ]; then
	export PATH=$INSTALL/bin:$PATH
	export GLPATH=$INSTALL/share/gridlabd/weather/US
	export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-.}
	run $INSTALL/bin/gridlabd -T $NPROC --validate
fi

# activate this version
if [ -x "$INSTALL/bin/gridlabd-version" -a "$TEST" == "yes" ]; then
	log "ACTIVATE: automatic"
	run $INSTALL/bin/gridlabd version set "$VERSION"
elif [ "$LINK" == "yes" ]; then
	log "ACTIVATE: manual"
	[ ! -L "$PREFIX/gridlabd/current" ] && run sudo rm -f "$PREFIX/gridlabd/current"
	run sudo ln -sf "$INSTALL" "$PREFIX/gridlabd/current"
	for dir in bin lib include share; do
		run sudo ln -sf $PREFIX/gridlabd/current/$dir/gridlabd $PREFIX/$dir/gridlabd
	done
	run sudo ln -sf $PREFIX/gridlabd/current/bin/gridlabd.bin $PREFIX/bin/gridlabd.bin
fi

cd /usr/local/opt/gridlabd
sudo rm -rf src
# all done :-)
exit 0