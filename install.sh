set -x

export SYSTEMNAME=$(uname -s)
case $SYSTEMNAME in
	Darwin)
		VERSION=""
		;;
	Linux)
		. /etc/os-release
		SYSTEMNAME=${ID}-${VERSION_ID%.*}-$(uname -m)
		LATEST=$(curl -fsL ${GRIDLABD_INSTALL:-http://install.gridlabd.us/}latest.csv | grep ^$SYSTEMNAME)
		if [ -z "$LATEST" ]; then
			echo "ERROR: $SYSTEMNAME is not available for fast install. Use build.sh instead."
			exit 1
		fi
		;;
	*)
		;;
esac
