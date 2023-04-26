export GRIDLABD_ORIGIN=dchassin/gridlabd/develop-fix-install
apt-get update
apt-get upgrade -y
apt-get install curl -y
apt-get install git -y
test -d /gridlabd/.git || git clone https://github.com/${GRIDLABD_ORIGIN%/*} -b ${GRIDLABD_ORIGIN##*/} --depth 1 /gridlabd
cd /gridlabd