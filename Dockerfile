FROM debian:11
RUN apt-get -q update
RUN apt-get install git -q -y 
WORKDIR /usr/local/src
RUN git clone https://github.com/slacgismo/gridlabd -b develop-fix-debian11-setup gridlabd --depth 1 
WORKDIR /usr/local/src/gridlabd
COPY ./build-aux/setup-Linux-debian-11.sh ./build-aux/setup-Linux-debian-11.sh
# RUN ./install.sh -v -t --parallel