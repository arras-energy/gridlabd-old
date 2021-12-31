FROM centos:8
RUN yum install git -y

WORKDIR /usr/local/src

RUN git clone -b develop-fix-centos-8-setup https://github.com/catzzz/gridlabd.git

WORKDIR /usr/local/src/gridlabd