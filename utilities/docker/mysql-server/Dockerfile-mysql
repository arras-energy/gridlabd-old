FROM mysql:5.7
ENV container docker
EXPOSE 3306/tcp

COPY *.sh /tmp/
COPY *.sql /tmp/

WORKDIR /tmp
RUN bash mysql-base.sh

ENV MYSQL_RANDOM_ROOT_PASSWORD=true
ENV MYSQL_USER=gridlabd_a
ENV MYSQL_PASSWORD=gridlabd
ENV MYSQL_DATABASE=gridlabd
