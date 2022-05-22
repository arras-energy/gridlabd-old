# EC2 server build script
#
# See github:/hipas/gridlabd/cloud/servers/version.gridlabd.us/README.md for details

#
# Clone gridlabd
#
yum install httpd git
yum clean metadata
git clone https://source.gridlabd.us/ /var/www/html/gridlabd

#
# Copy WWW files
#
SOURCE=remotes/origin/${BRANCH:-master}:cloud/servers/version.gridlabd.us
git show $SOURCE/access_log.conf > /etc/httpd/conf.d/access_log.conf
git show $SOURCE/index.html > /var/www/html/index.html
git show $SOURCE/robots.txt > /var/www/html/robots.txt
git show $SOURCE/update.sh > /var/www/html/update.sh

#
# Copy CRONTAB files
#
git show $SOURCE/crontab.root > /tmp/crontab.root
crontab -u root /tmp/crontab.root

#
# Install python modules
#
git show $SOURCE/aws_requirements.txt /tmp/requirements.txt
python3 -m pip install -r /tmp/requirements.txt

#
# Restart services
#
service crond restart
service httpd restart
