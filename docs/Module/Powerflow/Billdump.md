[[/Module/Powerflow/Billdump]] -- Class billdump

# Synopsis

GLM:

~~~
  object billdump {
    group "<string>";
    runtime "<string>";
    filename "<string>";
    runcount "<integer>";
    meter_type "{METER,TRIPLEX_METER}";
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

### `runcount`
~~~
  int32 runcount;
~~~

The number of times the file has been written to

### `meter_type`
~~~
  enumeration {METER, TRIPLEX_METER} meter_type;
~~~

Describes whether to collect from 3-phase or S-phase meters

# Example

~~~
  object billdump {
    group "";
    runtime "TS_ZERO";
    filename "";
    runcount "0";
    meter_type "0";
  }
~~~

# See also
* [[/Module/Powerflow]]

