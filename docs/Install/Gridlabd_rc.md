[[/Install/Gridlabd_rc]] - GridLAB-D run control file

# Synopsis

~~~
/usr/local/share/gridlabd/gridlabd.rc
$HOME/.gridlabd/gridlabd.rc
./gridlabd.rc
~~~

# Description

The `gridlabd.rc` file can be used to execute commands that alter the environment in which gridlabd.bin runs.  There are three locations where this file may be deployed. They will be run this order

1. `/usr/local/share/gridlabd/gridlabd.rc`
2. `$HOME/.gridlabd/gridlabd.rc`
3. `./gridlabd.rc`

# Caveat

Some models require the ability to generate and load a dynamic link library at runtime from the local folder.  In such cases it is useful to allow the system
to load library from the local folder.  This is done using the command

~~~
export LD_LIBRARY_PATH=.
~~~

which is placed in the system run control file.  However, this also present a security vulnerability if models can be run using root privileges.  In such cases it is strongly recommended that this line be commented out in the system RC file, and added to the local RC file where the feature is required.
