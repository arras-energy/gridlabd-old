[[/Module/Powerflow/Voltdump]] -- Class voltdump

# Synopsis
GLM:
~~~
  object voltdump {
    group "<string>";
    runtime "<string>";
    filename "<string>";
    file "<string>";
    runcount "<integer>";
    maxcount "<integer>";
    mode "{polar,rect}";
    filemode "<string>";
    interval "<decimal> s";
  }
~~~

# Description

TODO

## Properties

### `group`
~~~
  char32 group;
~~~

The group ID to output data for (all nodes if empty)

### `runtime`
~~~
  timestamp runtime;
~~~

The time to check voltage data

### `filename`
~~~
  char256 filename;
~~~

The file to dump the voltage data into

### `file`
~~~
  char256 file;
~~~

The file to dump the voltage data into

### `runcount`
~~~
  int32 runcount;
~~~

The number of times the file has been written to

### `maxcount`
~~~
  int32 maxcount;
~~~

The maximum number of times the file is written

### `mode`
~~~
  enumeration {polar, rect} mode;
~~~

Dumps the voltages in either polar or rectangular notation

### `filemode`
~~~
  char8 filemode;
~~~

Sets the file write mode

### `interval`
~~~
  double interval[s];
~~~

Interval at which voltdump runs

# Example

~~~
  object voltdump {
    group "";
    runtime "TS_ZERO";
    filename "";
    file "";
    runcount "0";
    maxcount "0";
    mode "0";
    filemode "";
    interval "0.0";
  }
~~~

# See also
* [[/Module/Powerflow]]

