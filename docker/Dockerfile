FROM hipas/gridlabd_base:220322


ENV container docker
ENV REPO=https://github.com/slacgismo/gridlabd
ARG BRANCH
RUN echo "Building $BRANCH"
ENV ENABLE=gismo
ENV GET_WEATHER=no
ENV REMOVE_SOURCE=yes
ARG RUN_VALIDATION
ENV LD_LIBRARY_PATH /usr/local/opt/gridlabd/lib


VOLUME [ "/sys/fs/cgroup" ]
WORKDIR /tmp
RUN yum clean all 
RUN yum update -y
RUN yum install git -y
COPY gridlabd.sh /tmp/
RUN chmod +wx *.sh
RUN bash gridlabd.sh
EXPOSE 6266-6299/tcp
