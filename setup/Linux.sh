. /etc/os-release
(curl -fsL $GRIDLABD_ORIGIN/setup/Linux-$ID-${VERSION_ID%%.*}.sh || (echo "ERROR: unable to find $GRIDLABD_ORIGIN/setup/Linux-$ID-${VERSION_ID%%.*}.sh. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
