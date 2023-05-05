#/bin/sh
error () { echo "ERROR [build.sh]: $*" > /dev/stderr ; exit 1; }
VERIFY="--version=all"
TARGET=
CONFIGURE=
UPLOAD=
while [ $# -gt 0 ]; do
	case "$1" in
		-h|--help|help)
			echo "Syntax: build.sh [--clean] [--cache-config] [--prefix PREFIX] [--install|--system] [--validate] [--upload|--release]"
			exit 0
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
		--cache-config)
			CONFIGURE="$CONFIGURE -C"
			;;
		--prefix)
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
test ! -z "$VIRTUAL_ENV" || . $HOME/.gridlabd/bin/activate
test ! -z "$VIRTUAL_ENV" || error "unable to activate gridlabd venv"
test -f ./configure || autoreconf -isf || error "autoconf failed"
test -f Makefile || ./configure $CONFIGURE || error "./configure failed"
make $TARGET && $(build-aux/version.sh --install)/bin/gridlabd $VERIFY || error "unable to make and verify build"
test -z "$UPLOAD" || make $UPLOAD