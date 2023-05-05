if ! autoconf --version 1>/dev/null 2>&1 ; then
	echo "ERROR: autoconf not installed. Did you run setup.sh?" > /dev/stderr
elif [ "$(autoconf --version 2>/dev/null | head -n 1 | grep -o '[^ ]*$')" != "2.71" ]; then
	echo "ERROR: autoconf version 2.71 required. Did you run setup.sh?" > /dev/stderr
elif ! git --version 1>/dev/null 2>&1 ; then
	echo "ERROR: you must install git to build GridLAB-D" > /dev/stderr
elif [ ! -f configure.ac ] ; then
	echo "ERROR: you must build from the source directory where configure.ac is located" > /dev/stderr
elif [ ! -f $HOME/.gridlabd/bin/activate ]; then
	echo "ERROR: $HOME/.gridlabd is not found. Have you run setup.sh yet?" > /dev/stderr
else
	. $HOME/.gridlabd/bin/activate
	autoreconf -isf
	./configure
	make -j$(($(nproc)*3)) system
	gridlabd -T 0 --validate
fi
