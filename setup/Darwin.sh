export INSTALL_VERSION=$(uname -r | cut -f1 -d.)
(curl -H 'Cache-Control: no-cache' -fsL $GRIDLABD_ORIGIN/setup/darwin_$INSTALL_VERSION.sh || (echo "ERROR: unable to find $GRIDLABD_SOURCE/setup/Darwin-$INSTALL_VERSION.sh. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
