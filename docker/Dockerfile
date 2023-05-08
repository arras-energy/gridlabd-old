FROM ubuntu:22.04

ENV container docker
ARG GRIDLABD_ORIGIN
ENV github_actions yes

VOLUME [ "/sys/fs/cgroup" ]
WORKDIR /tmp
COPY gridlabd.sh /tmp/
RUN chmod +wx *.sh
RUN sh gridlabd.sh
EXPOSE 6266-6299/tcp
