[[/Module/Powerflow/Voltdump]] -- Voltage dump object

# Synopsis
GLM
~~~
  object voltdump {
    group "<group-name>";
    runtime "<timestamp>";
    filename|file "<filename>";
    filemode "[a|w]";
    mode "[polar|rect]";
    interval "<double> <unit>";
    maxcount "<integer>";
  }
~~~

	char32 group; // the group ID to output data for (all nodes if empty)
	timestamp runtime; // the time to check voltage data
	char256 filename; // the file to dump the voltage data into
	char256 file; // the file to dump the voltage data into
	int32 runcount; // the number of times the file has been written to
	int32 maxcount; // the maximum number of times the file is written
	enumeration {polar=1, rect=0} mode; // dumps the voltages in either polar or rectangular notation
	char8 filemode; // sets the file write mode
	double interval[s]; // interval at which voltdump runs

# Description

The voltdump object produces a dump of the voltages at the nodes. This can be used to determine the cause of convergence problems.

## Properties

### `group`
~~~
	char32 group; 
~~~
The group ID to output data for (all nodes if empty).

### `runtime`
~~~
	timestamp runtime;
~~~

The time to check voltage data.

### `file|filename`
~~~
	char256 filename;
~~~

The file to dump the voltage data into.

### `runcount`
~~~
  int32 runcount;
~~~

The number of times the file has been written to.

### `maxcount`
~~~
	int32 maxcount;
~~~

The maximum number of times the file is written.

### `mode`
~~~
  enumeration {polar=1, rect=0} mode; 
~~~

Dumps the voltages in either polar or rectangular notation.

### `filemode`
~~~
  char8 filemode;
~~~
Specifies whether the dump is rewritten each time a new timestamp is generated.  Use `w` to the have object write a new dump file each time. Use `a` to append to the new dump to the end of file when a new timestamp is generated.

### `interval`
~~~
  double interval[s];
~~~
Sets the interval at which the dump is executed.

# See also

* [[/Module/Powerflow/Currdump]]
