FROM centos:8
# variable pass from install.sh
ARG NPROC=${NPROC}
ARG RUN_VALIDATION=${RUN_VALIDATION}
RUN echo "NPROC=$NPROC RUN_VALIDATION=$RUN_VALIDATION"

RUN yum install git -y
WORKDIR /usr/local/src
RUN git clone $REPO -b $BRANCH
WORKDIR /usr/local/src/gridlabd
RUN ./build-aux/setup-Linux-centos-8.sh
RUN autoreconf -isf && ./configure
RUN export MAKEFLAGS=-j$(($(nproc)*3))
RUN export PYTHONSETUPFLAGS="-j $(($(nproc)*3))"
RUN make -j$(($(nproc)*3)) system
RUN export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-.}
# get weather
RUN if [ "${GET_WEATHER:-yes}" == "yes" ]; then \ make index \ fi

# run validation
RUN if [ "${RUN_VALIDATION:-no}" == "yes" ]; then\ gridlabd -T 0 --validate \ fi
