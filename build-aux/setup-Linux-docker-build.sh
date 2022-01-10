#!/bin/bash

REPO=${REPO:-https://github.com/slacgismo/gridlabd}
BRANCH=${BRANCH:-master}
echo "
#####################################
#   gridlabd <- $REPO/$BRANCH
#####################################
"


# create Dockerfile 
echo "FROM centos:8" >> Dockerfile
echo "RUN yum install git -y" >> Dockerfile
echo "WORKDIR /usr/local/src" >> Dockerfile
echo "RUN git clone $REPO -b $BRANCH" >> Dockerfile
echo "WORKDIR /usr/local/src/gridlabd" >> Dockerfile
echo "# install system dependencies and python packages" >> Dockerfile
# echo "RUN ./build-aux/setup-Linux-centos-8.sh" >> Dockerfile

chmod +rxw Dockerfile
docker build -t gismo/gridlabd .
docker run -it gismo/gridlabd bash
