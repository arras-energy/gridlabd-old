#!/bin/sh
#
# IMPORTANT: only run this script from the Dockerfile !!!
#

# setup
error () { echo "ERROR [docker/gridlabd.sh]: $*" >/dev/stderr ; exit 1 ; }
set -x # enable echo of commands
test ! -z "$GRIDLABD_ORIGIN" || error "'--build-arg GRIDLABD_ORIGIN=ORG/REPO/BRANCH' not specified"
REPO=https://github.com/${GRIDLABD_ORIGIN%/*}
BRANCH=${GRIDLABD_ORIGIN##*/}
echo "Building docker image from $REPO/$BRANCH"

# download source
apt-get update
apt-get install git curl -y
git clone $REPO $HOME/gridlabd -b $BRANCH --depth 1 || error "git clone of $REPO/$BRANCH failed"
test -d $HOME/gridlabd || error "download from $REPO/$BRANCH failed"

# setup system
cd $HOME/gridlabd 
./setup.sh --local || error "setup failed"

# build and validate gridlabd
. $HOME/.gridlabd/bin/activate
if ./build.sh --parallel --system; then
	if [ gridlabd -W $HOME/gridlabd -T 0 --validate ]; then
		cd -
		rm -rf $HOME/gridlabd
	else
		echo "WARNING [docker/gridlabd.sh]: validate failed, source has been preserved"
	fi
	exit 0
else
	error "build failed"
	exit 1
fi

