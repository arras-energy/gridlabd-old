FROM mysql/mysql-server:latest
ENV container docker
ENV MYSQL_ROOT_PASSWORD gridlabd
WORKDIR /tmp
COPY mysql-setup.sql /tmp/
RUN mysql -uroot -pgridlabd < /tmp/mysql-setup.sql
EXPOSE 3306/tcp