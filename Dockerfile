
FROM centos:8
RUN yum install git -y
RUN yum install nano -y
WORKDIR /usr/local/src

#RUN git clone -b develop https://github.com/slacgismo/gridlabd.git
RUN git clone https://github.com/slacgismo/gridlabd.git

WORKDIR /usr/local/src/gridlabd
RUN rm -rf ./build-aux/setup-Linux-centos-8.sh
COPY ./build-aux/setup-Linux-centos-8.sh ./build-aux/setup-Linux-centos-8.sh
COPY ./requirements.txt ./requirements.txt
RUN chmod +rxw ./requirements.txt
RUN chmod +rxw ./build-aux/setup-Linux-centos-8.sh
# RUN ./install.sh -v -t 