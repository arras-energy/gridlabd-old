#!/bin/bash

#yum update
curl http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm > mysql-community-release-el7-5.noarch.rpm
rpm -ivh mysql-community-release-el7-5.noarch.rpm 
yum install mysql-server -y
systemctl start mysqld
