[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  object impedance_dump {
    group "<string>";
    filename "<string>";
    runtime "<string>";
    runcount "<integer>";
  }
~~~

# Description

TODO

## Properties

### `group`
~~~
  char32 group;
~~~

The group ID to output data for (all links if empty)

### `filename`
~~~
  char256 filename;
~~~

The file to dump the current data into

### `runtime`
~~~
  timestamp runtime;
~~~

The time to check voltage data

### `runcount`
~~~
  int32 runcount;
~~~

The number of times the file has been written to

# Example

~~~
  object impedance_dump {
    group "";
    filename "";
    runtime "TS_ZERO";
    runcount "0";
  }
~~~

# See also
* [[/Module/Powerflow]]

