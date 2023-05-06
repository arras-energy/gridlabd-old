#/bin/sh
# Syntax: build.sh [OPTIONS]
# 
# Build gridlabd on this local system. You must have run `setup.sh` first.
# 
# Options:
# 
#   -v|--verbose)   Enable output of commands as they are executed
# 
#   -s|--silent)    Disable output to stderr
# 
#   -q|--quiet)     Disable output to stdout
# 
#   --validate)     Enable validation of build
# 
#   --install)      Install build to target folder
# 
#   --system)       Make target folder the system default installation
# 
#   --target)       Specify the target folder (default is output of /usr/local/opt)
# 
#   --clean)        Delete all new uncommitted source files before starting build
# 
#   --upload)       Upload installed image to AWS when build is done
# 
#   --release)      Make AWS upload the default image for system/machine type
# 
#   --parallel)     Run the build in parallel if possible
# 
# Environment variables:
# 
#   MAKEFLAGS   Specify additional make options
# 
#   CFLAGS      Specify additional C compiler flags
# 
#   CPPFLAGS    Specify additional C++ compiler flags
# 
#   LDFLAGS     Specify additional linker flags
# 
# Example:
# 
#   A typical first-time build/install command is usually
#
#   $ ./build.sh --parallel --system
#
STDOUT=/dev/stdout
STDERR=/dev/stderr
error () { echo "ERROR [build.sh]: $*" > $STDERR ; exit 1; }
VERIFY="--version=all"
TARGET=
if [ -f "config.cache" ]; then
	CONFIGURE="-C"
else
	CONFIGURE=
fi
UPLOAD=
while [ $# -gt 0 ]; do
	case "$1" in
		-h|--help|help)
			cat $0 | grep '^# ' | cut -c3-
			exit 0
			;;
		-v|--verbose)
			set -x
			MAKEFLAGS="$MAKEFLAGS --debug V=1"
			;;
		-s|--silent)
			STDERR=/dev/null
			;;
		-q|--quiet)
			STDOUT=/dev/null
			;;
		--validate)
			VERIFY="-T 0 --validate"
			;;
		--install)
			TARGET="$TARGET install"
			;;
		--system)
			TARGET="$TARGET system"
			;;
		--target)
			CONFIGURE="$CONFIGURE --prefix=$2"
			shift 1
			;;
		--clean)
			utilities/cleanwc
			;;
		--upload)
			UPLOAD=aws-image
			;;
		--release)
			UPLOAD=aws-image-default
			;;
		--parallel)
			if nproc --help 1>/dev/null 2>&1 ; then
				MAKEFLAGS=-j$(($(nproc)*3))
			elif sysctl -a 1>/dev/null 2>&1 ; then
				MAKEFLAGS=-j$(($(sysctl -n hw.ncpu)*3))
			else
				error "unable to determine the number of available CPUs"
			fi
			;;
		*)
			error "option '$1' is not valid"
			;;
	esac
	shift 1
done
autoconf --version 1>/dev/null 2>&1 || error "autoconf not installed. Did you run setup.sh?"
test "$(autoconf --version 2>/dev/null | head -n 1 | grep -o '[^ ]*$')" = "2.71" || error "autoconf version 2.71 required. Did you run setup.sh?"
git --version 1>/dev/null 2>&1 || error "you must install git to build GridLAB-D"
test -f configure.ac || error "you must build from the source directory where configure.ac is located"
test -f $HOME/.gridlabd/bin/activate || error "$HOME/.gridlabd is not found. Run setup.sh again."
test ! -z "$VIRTUAL_ENV" || . $HOME/.gridlabd/bin/activate 1>$STDOUT 2>$STDERR
test ! -z "$VIRTUAL_ENV" || error "unable to activate gridlabd venv"
test -f ./configure || autoreconf -isf 1>$STDOUT 2>$STDERR || error "autoconf failed"
test -f Makefile || ./configure $CONFIGURE 1>$STDOUT 2>$STDERR || error "./configure failed"
make $MAKEFLAGS $TARGET 1>$STDOUT 2>$STDERR && $(build-aux/version.sh --install)/bin/gridlabd $VERIFY 1>$STDOUT 2>$STDERR || error "unable to make and verify build"
test -z "$UPLOAD" || make $UPLOAD 1>$STDOUT 2>$STDERR
