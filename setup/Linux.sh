. /etc/os-release
(curl -fsL $GRIDLABD_SOURCE/setup/Linux-$ID-${VERSION_ID%%.*}.sh || (echo "ERROR: unable to find $GRIDLABD_SOURCE/setup/Linux-$ID-${VERSION_ID%%.*}.sh. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
