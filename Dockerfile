FROM centos:8
RUN yum install git -y
WORKDIR /usr/local/src
RUN git clone -b develop-fix-centos8-setup https://github.com/slacgismo/gridlabd.git
WORKDIR /usr/local/src/gridlabd
RUN ./install.sh -t -v --parallel
