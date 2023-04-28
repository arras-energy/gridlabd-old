set -x

OPT=/usr/local/opt

if $# -eq 1; then
	case $(uname -s) in
		Darwin)
			TARGET="darwin_$(uname -r)-$(uname -m)"
			;;
		Linux)
			. /etc/os-release
			TARGET=${ID}_${VERSION_ID%.*}-$(uname -m)
			;;
		*)
			echo "ERROR: $(uname -s) is not available for fast install. Use build.sh instead." > /dev/stderr
			;;
	esac
else
	TARGET=$1
fi
if [ ! -z "${TARGET}" ]; then
	mkdir -p $OPT/gridlabd
	cd $OPT/gridlabd
	if ! (curl -sL https://install.gridlabd.us/$TARGET.tarz | tar xz) ; then
		echo "ERROR: unable to find install image for $TARGET" > /dev/stderr
	elif ! sh $TARGET/share/gridlabd/setup.sh ; then
		echo "ERROR: setup script not found for $TARGET" > /dev/stderr
	else
		ln -sf $TARGET current
		ln -sF $OPT/current/bin/gridlabd /usr/local/bin/gridlabd
		if [ ! "$(/usr/local/bin/gridlabd --version=name)" == "$TARGET" ] ; then
			echo "ERROR: /usr/local/bin/gridlabd not linked to $TARGET > /dev/stderr
		fi
	fi
fi