set -x

OPT=/usr/local/opt

if [ $# -eq 0 ]; then
	case $(uname -s) in
		Darwin)
			GRIDLABD_IMAGE="darwin_$(uname -r)-$(uname -m)"
			;;
		Linux)
			. /etc/os-release
			GRIDLABD_IMAGE=${ID}_${VERSION_ID%.*}-$(uname -m)
			;;
		*)
			echo "ERROR: $(uname -s) is not available for fast install. Use build.sh instead." > /dev/stderr
			;;
	esac
fi
if [ -z "${GRIDLABD_IMAGE}" ]; then
	echo "ERROR: GRIDLABD_IMAGE name not specified" > /dev/stderr
else
	mkdir -p $OPT/gridlabd
	cd $OPT/gridlabd
	if ! (curl -sL https://install.gridlabd.us/$GRIDLABD_IMAGE.tarz | tar xz) ; then
		echo "ERROR: unable to find install image for $TARGET" > /dev/stderr
	elif ! sh $GRIDLABD_IMAGE/share/gridlabd/setup.sh ; then
		echo "ERROR: setup script not found for $GRIDLABD_IMAGE" > /dev/stderr
	else
		ln -sf $GRIDLABD_IMAGE current
		ln -sF $OPT/current/bin/gridlabd /usr/local/bin/gridlabd
		if [ ! "$(/usr/local/bin/gridlabd --version=name)" == "$GRIDLABD_IMAGE" ] ; then
			echo "ERROR: /usr/local/bin/gridlabd not linked to $GRIDLABD_IMAGE" > /dev/stderr
		fi
	fi
fi