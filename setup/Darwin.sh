export INSTALL_VERSION=$(uname -r | cut -f1 -d.)
(curl -fsL https://raw.githubusercontent.com/dchassin/gridlabd/develop-fix-install/setup/Darwin-$INSTALL_VERSION.sh || (echo "ERROR: unable to find installer for Darwin $INSTALL_VERSION. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
