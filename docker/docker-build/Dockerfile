FROM debian:11 AS base
# variable pass from install.sh

ENV REPO=https://github.com/slacgismo/gridlabd
# ARG NPROC=${NPROC}
# ARG RUN_VALIDATION=${RUN_VALIDATION}
ARG BRANCH=${BRANCH}
ARG NPROC=${NPROC}
ARG RUN_VALIDATION=${RUN_VALIDATION}
ARG GET_WEATHER=${GET_WEATHER}

RUN echo "NPROC=$NPROC RUN_VALIDATION=$RUN_VALIDATION GET_WEATHER=$GET_WEATHER" 

RUN apt-get -q update -y && apt-get install procps -y
RUN apt-get install git -y
WORKDIR /usr/local/src
RUN git clone $REPO -b $BRANCH
FROM base AS dependencies
WORKDIR /usr/local/src/gridlabd
RUN  ./build-aux/setup-Linux-debian-11.sh 

# FROM dependencies AS installation
RUN autoreconf -isf && ./configure 
RUN  make -j${NPROC} system

# RUN export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-.}

# # get weather
RUN if [ "${GET_WEATHER:-yes}" == "yes" ]; then \ make index ; fi 

# run validation
RUN if [ "${RUN_VALIDATION:-no}" == "yes" ]; then \ gridlabd -T 0 --validate ; fi 

EXPOSE 6266-6299/tcp