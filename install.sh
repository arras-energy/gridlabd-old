INSTALL_SOURCE="http://install.gridlabd.us"
INSTALL_TARGET="/usr/local/opt"
INSTALL_STDERR="/dev/stderr"
INSTALL_STDOUT="/dev/stderr"

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
			echo "ERROR: $(uname -s) is not available for fast install. Use build.sh instead." >$INSTALL_STDERR
			;;
	esac
fi
if [ -z "${GRIDLABD_IMAGE}" ]; then
	echo "ERROR: GRIDLABD_IMAGE name not specified" 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
else
	mkdir -p "$INSTALL_TARGET/gridlabd" 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
	cd "$INSTALL_TARGET/gridlabd" 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
	GRIDLABD_SOURCE="$INSTALL_SOURCE/$GRIDLABD_IMAGE.tarz"
	echo "Downloading $GRIDLABD_SOURCEz..." 1>$INSTALL_STDOUT
	if ! curl -sL -I $GRIDLABD_SOURCE 1>$INSTALL_STDOUT ; then
		echo "ERROR: image $GRIDLABD_SOURCE not found" 2>$INSTALL_STDERR
	else
		GRIDLABD_FOLDER=$(curl -sL -H 'Cache-Control: no-cache' "$GRIDLABD_SOURCE" | tar xvz | tail -n 1 | cut -f1 -d/ ) 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
		if [ -z "$GRIDLABD_FOLDER" -o ! -d "$GRIDLABD_FOLDER" ]  ; then
			echo "ERROR: unable to download install image $GRIDLABD_SOURCE" >$INSTALL_STDERR
		elif ! sh "$GRIDLABD_FOLDER/share/gridlabd/setup.sh" ; then
			echo "ERROR: setup script not found for $GRIDLABD_FOLDER" >$INSTALL_STDERR
		else
			ln -sf "$GRIDLABD_FOLDER" "current" 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
			ln -sf "$INSTALL_TARGET/gridlabd/current/bin/gridlabd" "/usr/local/bin/gridlabd" 1>$INSTALL_STDOUT 2>$INSTALL_STDERR
			if [ ! "$(/usr/local/bin/gridlabd --version=name)" = "$GRIDLABD_FOLDER" ]; then
				echo "ERROR: /usr/local/bin/gridlabd not linked to $GRIDLABD_FOLDER" >$INSTALL_STDERR
			fi
			if [ ! "$(gridlabd --version=install)" = "$INSTALL_TARGET/gridlabd/$GRIDLABD_FOLDER" ]; then
				echo "WARNING: gridlabd is not in the current shell PATH" >$INSTALL_STDERR
			fi
		fi
	fi
fi