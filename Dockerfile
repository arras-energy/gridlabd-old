FROM centos:8
RUN yum install git -y
RUN yum install nano -y
WORKDIR /usr/local/src

RUN git clone https://github.com/slacgismo/gridlabd.git

WORKDIR /usr/local/src/gridlabd