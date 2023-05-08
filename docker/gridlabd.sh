#!/bin/sh

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
export github_actions=yes # same issues with filesync persist on docker build as on github
if [ "$BRANCH" = "master" ]; then
	./build.sh --parallel --system --validate || error "master build failed"
else
	./build.sh --parallel --system --validate || echo "WARNING [docker/gridlabd.sh]: development build/validate failed, image saved for follow-up"
fi

# cleanup source
cd -
test "$BRANCH" = "master" && rm -rf $HOME/gridlabd

exit 0
