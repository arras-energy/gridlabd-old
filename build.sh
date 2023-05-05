error () { echo "ERROR [build.sh]: $*" > /dev/stderr ; exit 1; }
autoconf --version 1>/dev/null 2>&1 || error "autoconf not installed. Did you run setup.sh?"
test "$(autoconf --version 2>/dev/null | head -n 1 | grep -o '[^ ]*$')" = "2.71" || error "autoconf version 2.71 required. Did you run setup.sh?"
git --version 1>/dev/null 2>&1 || error "you must install git to build GridLAB-D"
test -f configure.ac || error "you must build from the source directory where configure.ac is located"
test -f $HOME/.gridlabd/bin/activate || error "$HOME/.gridlabd is not found. Run setup.sh again."
test ! -z "$VIRTUAL_ENV" || . $HOME/.gridlabd/bin/activate
test ! -z "$VIRTUAL_ENV" || error "unable to activate gridlabd venv"
test -f ./configure || autoreconf -isf || error "autoconf failed"
test -f Makefile || ./configure || error "./configure failed"
make && gridlabd -T 0 --validate && echo "Use 'make install' or 'make system' to install this build"
