FROM slacgismo/circleci_gridlabd_base
ENV container docker
ENV REPO=https://github.com/dchassin/gridlabd
ENV BRANCH=master
ENV ENABLE=gismo
ENV GET_WEATHER=no
ENV REMOVE_SOURCE=yes
ENV ENABLE_GDB=no
ENV LD_LIBRARY_PATH /usr/local/lib

VOLUME [ "/sys/fs/cgroup" ]
WORKDIR /tmp
COPY utilities/docker/centos/*.sh /tmp/
RUN chmod +x *.sh
RUN system.sh
RUN gridlabd.sh
EXPOSE 6266-6299/tcp
