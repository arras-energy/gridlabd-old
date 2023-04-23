curl --version 1>/dev/null 2>&1 || (echo "ERROR: you must install curl first"; exit 1)
export INSTALL_SYSTEM=$(uname -s)
(curl -fsL https://raw.githubusercontent.com/dchassin/gridlabd/develop-fix-install/setup/$INSTALL_SYSTEM.sh || (echo "ERROR: unable to find installer for $INSTALL_SYSTEM. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
