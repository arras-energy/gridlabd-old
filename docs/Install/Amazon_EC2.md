[[/Install/Amazon_Ubuntu_EC2]] -- Running GridLAB-D on Amazon Ubuntu AMI EC2 Instance

# Synopsis

~~~
ec2-user% curl -sL http://install.gridlabd.us/install.sh | sudo /bin/bash
~~~

# Caveats

The `gridlabd` executable is installed in `/usr/local/opt/gridlabd/<version>/bin` which is not in the `PATH` by default.  The installation script updates the loader config and sets the installed version as the primary version. The primary(current) version will link its executable into `/usr/local/bin`, however you can use any other version by calling it's executable directly. 
