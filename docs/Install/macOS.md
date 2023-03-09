[[/Install/macOS]] -- Running GridLAB-D on macOS

# Synopsis

~~~
bash% curl -sL http://install.gridlabd.us/install.sh | /bin/bash
~~~

# Caveats

The `gridlabd` executable is installed in `/usr/local/bin` which might not be in the `PATH` by default.  The installation script updates the path in `/etc/bashrc` so you must logout and login again to find `gridlabd` or add it yourself using the command `export PATH=/usr/local/bin:$PATH`. The primary(current) version will link its executable into `/usr/local/bin`, however you can use any other version by calling it's executable directly. 
