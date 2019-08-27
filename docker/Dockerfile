FROM slacgismo/circleci_gridlabd_base
ENV container docker
ENV REPO=https://github.com/dchassin/gridlabd
ENV BRANCH=master
ENV ENABLE=gismo
ENV GET_WEATHER=no
ENV REMOVE_SOURCE=yes
ENV ENABLE_GDB=no
ENV LD_LIBRARY_PATH /usr/local/lib

RUN (cd /lib/systemd/system/sysinit.target.wants/; for i in *; do [ $i == \
systemd-tmpfiles-setup.service ] || rm -f $i; done); \
rm -f /lib/systemd/system/multi-user.target.wants/*;\
rm -f /etc/systemd/system/*.wants/*;\
rm -f /lib/systemd/system/local-fs.target.wants/*; \
rm -f /lib/systemd/system/sockets.target.wants/*udev*; \
rm -f /lib/systemd/system/sockets.target.wants/*initctl*; \
rm -f /lib/systemd/system/basic.target.wants/*;\
rm -f /lib/systemd/system/anaconda.target.wants/*;
VOLUME [ "/sys/fs/cgroup" ]
WORKDIR /tmp
COPY utilities/docker/centos/*.sh /tmp/
RUN chmod +wx *.sh
RUN bash system.sh
RUN bash gridlabd.sh
EXPOSE 6266-6299/tcp
