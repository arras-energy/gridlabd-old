export INSTALL_VERSION=$(uname -r | cut -f1 -d.)
(curl -fsL $GRIDLABD_SOURCE/setup/Linux-$INSTALL_VERSION.sh || (echo "ERROR: unable to find $GRIDLABD_SOURCE/setup/Linux-$INSTALL_VERSION.sh. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
