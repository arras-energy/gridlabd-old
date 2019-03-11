echo '
#####################################
# DOCKER BUILD
#   system
#####################################
'

# Install needed system tools
yum update -y ; yum clean all
yum install systemd -y ; yum clean all
yum groupinstall "Development Tools" -y
yum install cmake -y 
yum install ncurses-devel -y
yum install epel-release -y
yum install which -y
yum --disablerepo="*" --enablerepo="epel" install python36 -y
yum install python36-devel -y
ln -s /usr/bin/python3.6 /usr/bin/python3
