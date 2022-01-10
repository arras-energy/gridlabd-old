#!/bin/bash

REPO=${REPO:-https://github.com/slacgismo/gridlabd}
BRANCH=${BRANCH:-develop}
IMAGE_NAME=${IMAGE_NAME:-gismo/gridlabd}
NPROC=${NPROC:-1}
RUN_VALIDATION=${RUN_VALIDATION:-yes}


echo "
#####################################
#   gridlabd <- $REPO/$BRANCH
#   NPROC <- $NPROC
#   IMAGE_NAME <- $IMAGE_NAME
#   RUN_VALIDATION <- $RUN_VALIDATION
#####################################
"

echo "check docker .."
#check docker installation
if [[ $(which docker) && $(docker --version) ]]; then
    echo "docker is installed "
else
    error "docker is not installed, please install docker"
fi
# check docker service 
if ! docker info > /dev/null 2>&1; then
error "This script uses docker, and it isn't running - please start docker and try again!"
fi

# copy and overwrite Dockerfile to work directory 
cp  -fr ./docker/docker-build/Dockerfile ./Dockerfile
chmod +rxw ./Dockerfile
# run docker build
# docker build -f Dockerfile -t $IMAGE_NAME --build-arg BRANCH=$SOURCE_BRANCH --build-arg RUN_VALIDATION=$RUN_VALIDATION --build-arg NPROC=$NPROC .
