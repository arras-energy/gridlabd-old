#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

# Install needed system tools
yum update -y ; yum clean all
yum install systemd -y ; yum clean all
yum groupinstall "Development Tools" -y
yum install cmake -y 
yum install ncurses-devel -y
yum install epel-release -y
yum install libcurl-devel -y
yum install which -y

# upgrade git to version 2.x
yum remove git* -y
yum install https://centos7.iuscommunity.org/ius-release.rpm -y
yum install git2u-all -y

# python3 support needed as of 4.2
yum --disablerepo="*" --enablerepo="epel" install python36 -y
yum install python36 python36-devel python36-pip python36-tkinter -y
[ -f /usr/bin/python3 ] || ln -s /usr/bin/python3.6 /usr/bin/python3
pip3 install --upgrade pip
ln -s /usr/local/bin/pip3 /usr/bin/pip3
pip3 install matplotlib
pip3 install pandas
pip3 install mysql-connector
