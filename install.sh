curl --version 1>/dev/null 2>&1 || (echo "ERROR: you must install curl first"; exit 1)
export INSTALL_SYSTEM=$(uname -s)
(curl -sL https://raw.githubusercontent.com/dchassin/gridlabd/develop-fix-install/build-aux/install-$INSTALL_SYSTEM.sh | bash) || (echo "ERROR: unable to install automatically for $INSTALL_SYSTEM. Manual install required.")
