FROM ubuntu:22.04

ARG GRIDLABD_ORIGIN

ENV container docker
ENV github_actions yes

EXPOSE 6266-6299/tcp

VOLUME [ "/sys/fs/cgroup" ]
WORKDIR /tmp

COPY gridlabd.sh /tmp/
RUN chmod +wx *.sh
RUN sh gridlabd.sh
