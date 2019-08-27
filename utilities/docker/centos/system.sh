echo '
#####################################
# DOCKER BUILD
#   system
#####################################
'

# Install needed system tools
yum update -y
yum groupinstall "Development Tools" -y
yum install cmake -y 
yum install ncurses-devel -y
yum install libcurl-devel -y
yum install which -y
yum clean all

