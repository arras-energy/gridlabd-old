FROM hipas/gridlabd_base:230119


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
COPY gridlabd.sh /tmp/
RUN chmod +wx *.sh
RUN bash gridlabd.sh
EXPOSE 6266-6299/tcp
