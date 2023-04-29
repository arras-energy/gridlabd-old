set -x
INSTALL_SOURCE="http://install.gridlabd.us"
OPT="/usr/local/opt"

if [ $# -eq 0 ]; then
	case $(uname -s) in
		Darwin)
			GRIDLABD_IMAGE="darwin_$(uname -r)-$(uname -m)"
			;;
		Linux)
			. /etc/os-release
			GRIDLABD_IMAGE="${ID}_${VERSION_ID%.*}-$(uname -m)"
			;;
		*)
			echo "ERROR: $(uname -s) is not available for fast install. Use build.sh instead." > /dev/stderr
			;;
	esac
fi
if [ -z "${GRIDLABD_IMAGE}" ]; then
	echo "ERROR: GRIDLABD_IMAGE name not specified" > /dev/stderr
else
	mkdir -p "$OPT/gridlabd"
	cd "$OPT/gridlabd"
	GRIDLABD_FOLDER=$(curl -sL -H 'Cache-Control: no-cache' "$INSTALL_SOURCE/$GRIDLABD_IMAGE.tarz" | tar xvz | tail -n 1 | cut -f1 -d/ )
	if [ -z "$GRIDLABD_FOLDER" -o ! -d "$GRIDLABD_FOLDER" ]  ; then
		echo "ERROR: unable to download install image for $GRIDLABD_IMAGE" > /dev/stderr
	elif ! sh "$GRIDLABD_FOLDER/share/gridlabd/setup.sh" ; then
		echo "ERROR: setup script not found for $GRIDLABD_IMAGE" > /dev/stderr
	else
		ln -sf "$GRIDLABD_IMAGE" "current"
		ln -sF "$OPT/current/bin/gridlabd" "/usr/local/bin/gridlabd"
		if [ ! "$(/usr/local/bin/gridlabd --version=name)" == "$GRIDLABD_FOLDER" ] ; then
			echo "ERROR: /usr/local/bin/gridlabd not linked to $GRIDLABD_FOLDER" > /dev/stderr
		fi
	fi
fi