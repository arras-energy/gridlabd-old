[[Remote]] -- GridLAB-D remote command interface

# Synopsis
~~~
  localhost% gridlabd --remote hostname[:port] options ...
~~~
where
* `hostname` is the name of the remote machine on which the daemon is running (default is `localhost`)
* `port` is the optional port number to connect to on the remote machine (default is `6266`)
* `options` are the command line options for the remote gridlabd run

# Description

The --[[remote]] command line option is used to send a gridlabd command to a machine running a gridlabd [[daemon]].

The `hostname` is the internet host on which the daemon is running. If `localhost` or `127.0.0.1` is used the local machine is used.

The `port` number is by default `6266`, or the value specified in the configuration, which is by default located at `${GLD_ETC}/gridladb.cnf`.

The remaining `options` are passed to the daemon as the gridlabd command line options.

# Examples

To get the gridlabd version on the remote machine:
~~~
  localhost% gridlabd --remote hostname --version
~~~
To run a simulation model on the remote machine
~~~
  localhost% gridlabd --remote hostname model.glm
~~~
Note that the file `model.glm` must be located on the remote machine.  

# See also

* --[[daemon]] command options