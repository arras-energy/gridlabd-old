[[/Install/Amazon_EC2]] -- Running GridLAB-D on Amazon EC2

# Synopsis

~~~
ec2-user% curl -sL http://install.gridlabd.us/install.sh | sudo /bin/bash
~~~

# Caveats

The `gridlabd` executable is installed in `/usr/local/opt/gridlabd/bin` which is not in the `PATH` by default.  The installation script updates the path in `/etc/bashrc` so you must logout and login again to find `gridlabd` or add it yourself using the command `export PATH=/usr/local/opt/gridlabd/bin:$PATH`.

The installation script removes the default version of python3 rather than installing python3.9 as an alternative and using `venv`.
