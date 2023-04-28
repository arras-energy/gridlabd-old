. /etc/os-release
SOURCE=$GRIDLABD_ORIGIN/setup/$ID-${VERSION_ID%%.*}-$(uname -m).sh
(curl -H 'Cache-Control: no-cache' -fsL $SOURCE || (echo "ERROR: unable to find $SOURCE. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
