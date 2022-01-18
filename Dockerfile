FROM debian:9
RUN apt-get -q update
RUN apt-get -q install tzdata -y

RUN apt-get -q install software-properties-common -y
RUN apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev curl -y

# install system build tools needed by gridlabd

RUN apt-get -q install git -y
WORKDIR /usr/local/src
RUN git clone -b develop https://github.com/slacgismo/gridlabd.git
WORKDIR /usr/local/src/gridlabd
COPY ./build-aux/setup-Linux-debian-9.sh ./build-aux/setup-Linux-debian-9.sh
RUN ./install.sh -v -t --parallel