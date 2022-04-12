FROM debian:11 AS base
ENV REPO=https://github.com/slacgismo/gridlabd
# ARG NPROC=${NPROC}
# ARG RUN_VALIDATION=${RUN_VALIDATION}
ARG BRANCH=develop-add-docker-build-setup


RUN apt-get -q update -y && apt-get install procps -y
RUN apt-get install git -y
WORKDIR /usr/local/src
RUN git clone $REPO -b $BRANCH


WORKDIR /usr/local/src/gridlabd
COPY ./build-aux/setup-Linux-debian-11.sh ./build-aux/setup-Linux-debian-11.sh 
COPY ./install.sh ./install.sh
COPY ./docker/docker-build/Dockerfile ./docker/docker-build/Dockerfile