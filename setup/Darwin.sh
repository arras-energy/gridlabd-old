export ID="darwin"
export VERSION_ID=$(uname -r | cut -f1 -d.)
SOURCE=$GRIDLABD_ORIGIN/setup/${ID}_${VERSION_ID%%.*}-$(uname -m).sh
(curl -H 'Cache-Control: no-cache' -fsL $SOURCE || (echo "ERROR: unable to find $SOURCE. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
