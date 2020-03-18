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
    version "2";
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

Sets the file write mode. Use w to write a new file each dump. Use a to append each dump to the end of file when a new timestamp is generated.

### `interval`

~~~
  double interval[s];
~~~

Interval at which voltdump runs

### `version`
~~~
  int32 version;
~~~

Specifies the format version to output. The default is version 2, which omits the comment row when a new timestamp is seen, and adds a `datetime` column to the output.

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

