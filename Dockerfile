FROM debian:11
RUN apt update 
RUN apt install git -y
WORKDIR /usr/local/src
RUN git clone -b develop-fix-debian11-setup https://github.com/slacgismo/gridlabd.git
WORKDIR /usr/local/src/gridlabd
RUN export DEBIAN_FRONTEND=noninteractive
RUN ln -fs /usr/share/zoneinfo/Etc/GMT+8 /etc/localtime
RUN apt-get install -y tzdata
RUN dpkg-reconfigure --frontend noninteractive tzdata
COPY ./build-aux/setup-Linux-debian-11.sh ./build-aux/setup-Linux-debian-11.sh 
# RUN ./install.sh -v -t --parallel
