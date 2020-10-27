[[/Command/Rusage]] -- Command line option to enable system resource usage data collection

# Synopsis

~~~
bash$ gridlabd --rusage ...
~~~

# Description

The `--rusage` command line option is used to obtain system resource usage data for the simulation run.

The output is generated to the CSV file specified by the [[/Global/Rusage_file]] variable.  Resource usage data is collected every [[/Global/Rusage_rate]] seconds of the simulation global [[/Global/Clock]].

The fields generated are interpreted as follows:

| Field | Description
| ----- | -----------
| timestamp | internal simulation clock value
| utime | total amount of time spent executing in user mode.
| stime | total amount of time spent in the system executing on behalf of the process(es).
| maxrss | maximum resident set size utilized (in bytes).
| ixrss | integral value indicating the amount of memory used by the text segment that was also shared
| idrss | integral value of the amount of unshared memory residing in the data segment of a process
| isrss | integral value of the amount of unshared memory residing in the stack segment of a process (expressed in units of kilobytes ticks-of-execution).
| minflt | number of page faults serviced without any I/O activity; here I/O activity is avoided by reclaiming a page frame from the list of pages awaiting reallocation.
| majflt | number of page faults serviced that required I/O activity.
| nswap | number of times a process was swapped out of main memory.
| inblock | number of times the file system had to perform input.
| oublock | number of times the file system had to perform output.
| msgsnd | number of IPC messages sent.
| msgrcv | number of IPC messages received.
| nsignals | number of signals delivered.
| nvcsw | number of times a context switch resulted due to a process voluntarily giving up the processor before its time slice was completed (usually to await availability of a resource).
| nivcsw | number of times a context switch resulted due to a higher priority process becoming runnable or because the current process exceeded its time slice.

# Example

The following example enables data collection on a simulation.

~~~
bash$ gridlabd --rusage my_model.glm
~~~

# See also

* [[/Global/Clock]]
* [[/Global/Rusage_file]]
* [[/Global/Rusage_rate]]
