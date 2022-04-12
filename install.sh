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

# local folder
VAR="/usr/local/var/gridlabd"
if [ ! -d "$VAR" ]; then
	mkdir -p $VAR || ( sudo mkdir -p $VAR && sudo chown ${USER:-root} $VAR )
fi

# setup logging
LOG="$VAR/install.log"
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
	echo "DOCKER=$DOCKER" 
	echo "IMAGENAME=$IMAGENAME"
	echo "BRANCH=$BRANCH"
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
	  -i   --no-index        Do not index data archives (Compatible with --docker-build option)
	  -l   --no-link         Do not link new install to activate it
	  -t   --no-test         Do not run validation tests (Compatible with --docker-build option)
	  -u   --no-update       Do not update system to meet requirements
	  -p   --parallel        Enable parallelism when possible (Compatible with --docker-build option)
	       --prefix <path>   Set install prefix
	       --save            Save the current configuration as default
	       --no-setup        Perform system setup
	       --reset           Reset the configuration to default
	  -q   --quick           Run only updates instead of a clean install
	  -s   --silent          Run without showing commands
	  -v   --verbose         Run showing log output
	       --validate        Run validation tests (Compatible with --docker-build option)
	       --version <name>  Override the default version name
		   --docker-build <imagename> Build docker image. default imagename=slacgismo/gridlabd:latest.
		   --branch			 Define the branch of the gridlabd repsitory. Default branch is "master". (Compatible with --docker-build option)
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
	(--docker-build)
		DOCKER="yes"
		IMAGENAME="$2"
		if [ -z "$2" ]; then
			IMAGENAME="slacgismo/gridlabd:latest"
		fi
		shift 1
		;;
	(--branch)
		BRANCH="$2"
		if [ -z "$2" ]; then
			error "--branch input does not exist"
		fi
		shift 1
		;;
	(*)
		error "'$1' is not a valid option"
		exit 1
		;;
	esac
	shift 1
done

NPROC=1
SYSTEM=$(uname -s)
SETUP_FILE="build-aux/setup-debian-11.sh"

if [ "$SYSTEM" == "Linux" ]; then
	source "/etc/os-release"
	RELEASE=$(echo $VERSION_ID | cut -f1 -d.)
	SETUP_FILE="build-aux/setup-$SYSTEM-$ID-$RELEASE.sh"
elif [ "$SYSTEM" == "Darwin" ]; then
	RELEASE=$(uname -r | cut -f1 -d.)
	SETUP_FILE="build-aux/setup-$SYSTEM-$RELEASE.sh"
fi


echo "******** SYSTEM=$SYSTEM , RELEASE = $RELEASE SETUP_FILE=$SETUP_FILE"
if [ "$PARALLEL" == "yes" ]; then
	if [ "$SYSTEM" == "Linux" ]; then
		NPROC=$(lscpu | grep '^CPU(s):' | cut -f2- -d' ')
	elif [ "$SYSTEM" == "Darwin" ]; then
		NPROC=$(sysctl -n machdep.cpu.thread_count)
	fi
fi


# start logging
log clear
log "START: $(date)"
log "COMMAND: $0 $*"
log "CONTEXT: ${USER:-unknown}@${HOSTNAME:-localhost}:$PWD"
log "SYSTEM: $SYSTEM"
log "SYSTEM ID: $SYSTEM_ID"
log "SYSTEM VERSION: $RELEASE"
log "NPROC: $NPROC"
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


# define docker-build 
function docker-build ()
{
	
	echo "check docker .."
	#check docker installation
	if [[ $(which docker) && $(docker --version) ]]; then
		echo "docker is installed "
		# command
	else
		error "docker is not installed"
		# command
	fi

	# check docker service 
	if ! docker info > /dev/null 2>&1; then
	error "This script uses docker, and it isn't running - please start docker and try again!"
	fi

	# docker variables
	RUN_VALIDATION=no
	if [ "$TEST" == "yes" ]; then
		RUN_VALIDATION=$TEST
	fi

	GET_WEATHER=no
	if [ "$INDEX" == "yes" ]; then
		GET_WEATHER=$INDEX
	fi
	
	echo "--build-arg RUN_VALIDATION=$RUN_VALIDATION, NPROC=$NPROC, BRANCH=$BRANCH"	
	docker build -f ./docker/docker-build/Dockerfile -t $IMAGENAME \
			--build-arg GET_WEATHER=$GET_WEATHER \
			--build-arg	RUN_VALIDATION=$RUN_VALIDATION \
			--build-arg	NPROC=$NPROC \
			--build-arg BRANCH=$BRANCH .
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


	SOK="$VAR/setup.ok"
    if [ ! -f "$SOK" -o "$FORCE" == "yes" ]; then
		log "********* SETUP_FILE=$SETUP_FILE ******* "
		if [ ! -f "${SETUP_FILE}" ] ; then 
			log "no $SETUP_FILE run docker build command instead"
			DOCKER="yes"
			
		else
			run ${SETUP_FILE}
		fi
		# run build-aux/setup.sh
		date > "$SOK"
	elif [ -f "$SOK" ]; then
		log "SETUP: already completed at $(cat $SOK)"
	else
		log "SETUP: skipping"
	fi

fi


if [ "$DOCKER" == "yes" ]; then
	docker-build
	exit 0
fi

# dynamic variables
require git
VERSION=${VERSION:-`build-aux/version.sh --name`}
INSTALL=${INSTALL:-$PREFIX/opt/gridlabd/$VERSION}

# run checks
if [ "$LINK" == "yes" -a -f "$PREFIX/bin/gridlabd" -a ! -L "$PREFIX/bin/gridlabd" ]; then
	OLDVER="$(gridlabd --version | cut -f2 -d' ')-saved_$(date '+%y%m%d')"
	VDIR=$OLDVER
	while [ -e $PREFIX/opt/gridlabd/$VDIR ]; do
		VDIR=${OLDVER}_${VNUM:-0}
		VNUM=$((${VNUM:-0}+1))
	done
	log "BACKUP: saving $OLDVER to $PREFIX/opt/gridlabd/$VDIR and linking it back to current version"
	run mkdir -p $PREFIX/opt/gridlabd/$VDIR || error "unable to create $VDIR to save current version"
	run ln -sf $PREFIX/opt/gridlabd/$VDIR $PREFIX/opt/gridlabd/current 
	for item in bin include lib share; do
		[ ! -d $PREFIX/opt/gridlabd/$VDIR/$item ] && run mkdir -p $PREFIX/opt/gridlabd/$VDIR/$item
		[ ! -d $PREFIX/opt/gridlabd/$VDIR/$item ] && run mv $PREFIX/$item/gridlabd* $PREFIX/opt/gridlabd/$VDIR/$item/
		run ln -sf $PREFIX/opt/gridlabd/$VDIR/$item/gridlabd $PREFIX/opt/gridlabd/current/$item/gridlabd
		run ln -sf $PREFIX/opt/gridlabd/current/$item /$PREFIX/$item/gridlabd
	done
	run ln -s $PREFIX/opt/gridlabd/current/bin/gridlabd.bin $PREFIX/bin/gridlabd.bin
	error "stopping here for debugging reasons -- this error message should be deleted"
fi
if [ "$CHECK" == "yes" ]; then
	if [ "$LINK" == "yes" -a -d "$PREFIX/gridlabd" -a ! -L "$PREFIX/gridlabd" ]; then
	    error "$PREFIX/gridlabd exists but it is not a symbolic link"
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
if [ -e "$INSTALL" -a "$FORCE" == "no" ]; then
	error "$INSTALL already exists, please delete it first"
fi
mkdir -p "$INSTALL" || ( run sudo mkdir -p "$INSTALL" && run sudo chown -R "${USER:-root}" "$INSTALL" )
if [ ! -f "configure" -o "$QUICK" == "no" ]; then
    run ./configure --prefix="$INSTALL" $*
fi



# build everything
export PATH=/usr/local/bin:/usr/bin:/bin
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
	[ ! -L "$PREFIX/opt/gridlabd/current" ] && run sudo rm -f "$PREFIX/opt/gridlabd/current"
	run sudo ln -sf "$INSTALL" "$PREFIX/opt/gridlabd/current"
	for dir in bin lib include share; do
		run sudo ln -sf $PREFIX/opt/gridlabd/current/$dir/gridlabd $PREFIX/$dir/gridlabd
	done
	run sudo ln -sf $PREFIX/opt/gridlabd/current/bin/gridlabd.bin $PREFIX/bin/gridlabd.bin
fi

# all done :-)
exit 0