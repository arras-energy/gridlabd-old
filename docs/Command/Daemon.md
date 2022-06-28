[[/Command/Daemon]] -- GridLAB-D daemon control command

# Synopsis

~~~
bash$ gridlabd --daemon <command> [<options> ... ] 
~~~

# Description

The `<command>` is the `daemon` command. The `<options>` are the GridLAB-D `daemon` command options.

The `--daemon` command line option is used to control the GridLAB-D daemon process.  The daemon process listens for incoming TCP connections on port 6266 by default.  Connections can be established using the `--remote` command line option (see [[/Command/Remote]].

The daemon command recognizes the standard process control commands `start`, `stop`, `restart`, and `status`.  

The valid daemon command options are:
* `-f|--configfile filename` is used to specify a configuration file other than the standard one located at `/usr/local/opt/gridlabd/etc/gridlabd.cnf`.
* `-h|--help` is used to get a list of available commands.
* `-j|--jail` is used to isolate the daemon in the work directory (`start` only)
* `-p|--port` is used to override the default port number (usually 6266, `start` only).
* `-w|--wait` is used to force the daemon to wait for it to stop before exiting (`start` only).  This is important when using dockers to keep the instance alive while serving requests.

## Configuration

If the daemon configuration file is found at `/usr/local/opt/gridlabd/etc/gridlabd.cnf` or where specified by the `-f` option, then it is loaded and used to configure the daemon. (Note that this applies also to the `--remote` option.)

The format of the entries in the configuration file must be

~~~
name=value
~~~

where `name` is the name of the parameter and `value` is the value you wish to give it.

Valid parameter names and their defaults are as follows

~~~
# default gridlabd daemon configuration
log=/usr/local/opt/gridlabd/var/gridlabd-log
pid=/usr/local/opt/gridlabd/var/gridlabd-pid
workdir=/
user=
output=
verbose=
error=
warning=
debug=
profile=
progress=
maxbacklog=4
clientmask=0.0.0.0
port=6266
umask=0
keepalive=1
timeout=10
~~~

The names are defined as follows

* `log`: the location of the log file
* `pid`: the location of the pid file
* `workdir`: the default working directory
* `user`: the user name under which the daemon run remote commands (empty is the user that start the daemon)
* `output`: the output stream file for commands (empty sends output to client)
* `verbose`: the verbose stream file for commands (empty sends output to client)
* `error`: the error stream file for commands (empty sends output to client)
* `warning`: the warning stream file for commands (empty sends output to client)
* `debug`: the debug stream file for commands (empty sends output to client)
* `profile`: the profile stream file for commands (empty sends output to client)
* `progress`: the process stream file for commands (empty sends output to client)
* `maxbacklog`: the maximum number of incoming remote commands that can wait 
* `clientmask`: the IP address mask for allowed incoming remote commands
* `listen`: the IP address to listen for connections on; use `127.0.0.1` for localhost only, `0.0.0.0` for any
* `port`: the TCP port on which the daemon listens for incoming remote commands
* `umask`: the file access mask for output during remote commands
* `keepalive`: the frequency with which output is sent to the remote client to ensure the connection stays open
* `timeout=10`: the timeout for read activity on both the ends of the remote connection

## Log

The daemon generates a log to record all activity. By default the log is created in `/usr/local/opt/gridlabd/var/gridlabd-log`, but this can be changed using the configuration file (see the `-f` option).  Log entries are formatted as follows:

~~~
[pid@time] message
~~~

where `pid` is the process id that generated the entry, `time` is the time at which the entry was generated, and `message` is the log entry.

Normally the content of the log file is purged when a new copy of the daemon starts.  If the log file name begins with a `+` character, the old content is preserved and new entries are appended.  The `+` is not included in the log file name when the file is opened by the daemon.

## Process ID

The process of the primary daemon is saved in `/usr/local/opt/gridlabd/var/gridlabd-pid` while the daemon is running.  When the daemon exits, the file is deleted. It is possible that the file might not be deleted, in which case the next instance of the daemon should correct the problem automatically.

# Examples

To start the daemon

~~~
bash$ gridlabd --daemon start
~~~

To stop the daemon

~~~
bash$ gridlabd --daemon stop
~~~

To restart the daemon

~~~
bash$ gridlabd --daemon restart
~~~

To get the daemon status

~~~
bash$ gridlabd --daemon status
~~~

To start the daemon using a local configuration file

~~~
bash$ gridlabd --daemon start -f gridlabd.cnf
~~~

# See also

* [[/Command/Remote]]
