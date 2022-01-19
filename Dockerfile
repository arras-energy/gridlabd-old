FROM debian:11
RUN apt update
RUN apt-get -q install git -y
WORKDIR /usr/local/src
RUN git clone -b develop https://github.com/slacgismo/gridlabd.git
WORKDIR /usr/local/src/gridlabd
COPY ./build-aux/setup-Linux-debian-11.sh ./build-aux/setup-Linux-debian-11.sh
RUN chmod +rxw ./build-aux/setup-Linux-debian-11.sh
RUN ./install.sh -v -t --parallel