[[/Command/Pkill]] -- Send signal to a gridlabd job on a processor

# Synopsis

~~~
bash$ gridlabd --pkill [<signal>] <processor>                                       
~~~

# Description

The `--pkill` command line option sends a signal to the job on the specified processor.  If the signal is omitted, the interrupt (SIGINT=2) signal is sent. 

The following signals and actions are supported.

| Signal     | Action    | Remarks 
| ---------- | --------- | -------------------------------
| `-SIGINT`  | Terminate | Interrupt the job
| `-SIGQUIT` | Core dump | Quit the job
| `-SIGTRAP` | Core dump | Trace trap the job
| `-SIGABRT` | Core dump | Abort the job
| `-SIGKILL` | Terminate | Kill the job
| `-SIGTERM` | Terminate | Software termination of job
| `-SIGSTOP` | Stop job  | Stops the job
| `-SIGCONT` | Discarded | Continues the job

Signals can also be given in short form, e.g., `-INT` instead of `-SIGINT` or as a number, e.g., `-2` instead of `-SIGINT`. Note that any signal number is permitted, but only the above signals are supported as a string.

# Example

The following commands interrupt the simulation running on processor 0:

~~~
host$ gridlabd --pkill 0
host$ gridlabd --pkill -INT 0
host$ gridlabd --pkill -SIGINT 0
host$ gridlabd --pkill -2 0
~~~

# See also

* [[/Command/Pcontrol]]
* [[/Command/Plist]]
* [[/Command/Pstatus]]
