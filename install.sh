# export INSTALL_SOURCE=https://install-dev.gridlabd.us
# export INSTALL_TARGET=FOLDERNAME
# export INSTALL_STDOUT=FILENAME
# export INSTALL_STDERR=FILENAME
# export GRIDLABD_IMAGE=OS_VERSION-MACHINE
DEFAULT_SOURCE="https://install.gridlabd.us"
DEFAULT_TARGET="/usr/local/opt"
DEFAULT_STDOUT="/dev/stdout"
DEFAULT_STDERR="/dev/stderr"
test "$(whoami)" = "root"  && echo "WARNING [install.sh]: installing as root can cause permission problems with python" > $DEFAULT_STDERR
if [ $# -gt 1 ]; then
	echo "ERROR [install.sh]: install.sh can only be run as a script" > $DEFAULT_STDERR
elif [ -z "$GRIDLABD_IMAGE" ]; then
	case $(uname -s) in
		Darwin)
			GRIDLABD_IMAGE="darwin_$(uname -r | cut -f1 -d.)-$(uname -m)"
			;;
		Linux)
			. /etc/os-release
			GRIDLABD_IMAGE="${ID}_${VERSION_ID%.*}-$(uname -m)"
			;;
		*)
			echo "ERROR [install.sh]: $(uname -s) is not available for fast install. Use build.sh instead." >${INSTALL_STDERR:-$DEFAULT_STDERR}
			;;
	esac
fi
if [ -z "$GRIDLABD_IMAGE" ]; then
	echo "ERROR [install.sh]: GRIDLABD_IMAGE name not specified" 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
elif ! mkdir -p "${INSTALL_TARGET:-$DEFAULT_TARGET}/gridlabd" 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR} ; then
	echo "ERROR [install.sh]: unable to create ${INSTALL_TARGET:-$DEFAULT_TARGET}/gridlabd" > $DEFAULT_STDERR
else
	cd "${INSTALL_TARGET:-$DEFAULT_TARGET}/gridlabd" 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
	GRIDLABD_SOURCE="${INSTALL_SOURCE:-$DEFAULT_SOURCE}/$GRIDLABD_IMAGE.tarz"
	echo "Downloading $GRIDLABD_SOURCE..." 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT}
	if ! curl -sL -I $GRIDLABD_SOURCE 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} ; then
		echo "ERROR [install.sh]: image $GRIDLABD_SOURCE not found. Use build.sh instead." 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
	else
		case $(uname -s) in
			Darwin)
				GRIDLABD_FOLDER=$(curl -sL -H 'Cache-Control: no-cache' "$GRIDLABD_SOURCE" | tar xvz 2>&1 | tail -n 1  | cut -c3- | cut -f1 -d/ ) 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
				;;
			*)
				export DEBIAN_FRONTEND=noninteractive
				GRIDLABD_FOLDER=$(curl -sL -H 'Cache-Control: no-cache' "$GRIDLABD_SOURCE" | tar xvz | tail -n 1  | cut -f1 -d/ ) 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
				;;
		esac
		if [ -z "$GRIDLABD_FOLDER" -o ! -d "$GRIDLABD_FOLDER" ]  ; then
			echo "ERROR [install.sh]: unable to download install image $GRIDLABD_SOURCE to $GRIDLABD_FOLDER" >${INSTALL_STDERR:-$DEFAULT_STDERR}
		elif ! sh "$GRIDLABD_FOLDER/share/gridlabd/setup.sh" ; then
			echo "ERROR [install.sh]: setup script not found for $GRIDLABD_FOLDER" >${INSTALL_STDERR:-$DEFAULT_STDERR}
		else
			ln -sf "$GRIDLABD_FOLDER" "current" 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
			ln -sf "${INSTALL_TARGET:-$DEFAULT_TARGET}/gridlabd/current/bin/gridlabd" "/usr/local/bin/gridlabd" 1>${INSTALL_STDOUT:-$DEFAULT_STDOUT} 2>${INSTALL_STDERR:-$DEFAULT_STDERR}
			if [ ! "$(/usr/local/bin/gridlabd --version=name)" = "$GRIDLABD_FOLDER" ]; then
				echo "ERROR [install.sh]: /usr/local/bin/gridlabd not linked to $GRIDLABD_FOLDER" >${INSTALL_STDERR:-$DEFAULT_STDERR}
			fi
			if [ ! "$(gridlabd --version=install)" = "${INSTALL_TARGET:-$DEFAULT_TARGET}/gridlabd/$GRIDLABD_FOLDER" ]; then
				echo "WARNING [install.sh]: gridlabd is not in the current shell PATH" >${INSTALL_STDERR:-$DEFAULT_STDERR}
			fi
		fi
	fi
fi
