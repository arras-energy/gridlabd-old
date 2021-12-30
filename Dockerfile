FROM centos:8
RUN yum install git -y

WORKDIR /usr/local/src

RUN git clone https://github.com/slacgismo/gridlabd

WORKDIR /usr/local/src/gridlabd