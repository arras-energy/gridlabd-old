[[/Module/Powerflow/Currdump]] -- Current dump object

# Currdump
GLM:
~~~
  object {
    group "";
    runtime "NEVER";
    filename "";
    runcount "0";
    maxcount "0";
    interval "+0 s";
    filemode "w";
    mode "RECT";
  }
~~~

# Description

The currdump object produces a dump of the currents at the nodes. This can be used to determine the cause of convergence problems.

## Properties

### `group`
~~~
	char32 group;
~~~

The group ID to output data for (all links if empty).

### `runtime`
~~~
	timestamp runtime; 
~~~

The time to check current data.

### `filename`
~~~
	char256 filename; 
~~~

The file to dump the current data into.

### `runcount`
~~~
	int32 runcount; 
~~~

The number of times the file has been written to.

### `maxcount`
~~~
	int32 maxcount; 
~~~

The maximum number of times the file is written

### `interval`
~~~
  double interval[s]; 
~~~

Interval at which currdump runs. This allows to set an interval at which the volt/curr dump should be appended with. Note, use the filemode `"a"` to facilitate interval dump.

### `filemode`
~~~
  char8 filemode; 
~~~

Sets the file write mode. Use `w` to write a new file each dump. Use `a` to append each dump to the end of file when a new timestamp is generated.

### `mode`
~~~
  enumeration {POLAR=1, RECT=0} mode;
~~~

Specify whether to dump complex numbers in polar or rectangular form.

# Example

A use case example of each below: 

~~~
  object currdump {
    filemode "a";
    filename "output/curr_dump.csv";
    interval "60";
    maxcount "0";
  }
~~~

# See also

* [[/Module/Powerflow/Voltdump]]
