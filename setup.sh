curl --version 1>/dev/null 2>&1 || (echo "ERROR: you must install curl first"; exit 1)
export INSTALL_SYSTEM=$(uname -s)
export GRIDLABD_SOURCE=https://raw.githubusercontent.com/${GRIDLABD_ORG:-slacgismo}/${GRIDLABD_REPO:-gridlabd}/${GRIDLABD_BRANCH:-master}
(curl -fsL $GRIDLABD_SOURCE/setup/$INSTALL_SYSTEM.sh || (echo "ERROR: unable to find $GRIDLABD_SOURCE/setup/$INSTALL_SYSTEM.sh. Manual install required." >/dev/stderr; exit 1)) | /bin/sh
