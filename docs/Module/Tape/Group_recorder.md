[[/Module/Tape/Group_recorder]] -- Class group_recorder

# Synopsis

GLM:

~~~
  object group_recorder {
    file "<string>";
    group "<string>";
    interval "<decimal> s";
    flush_interval "<decimal> s";
    strict "<string>";
    print_units "<string>";
    property "<string>";
    limit "<integer>";
    format "<string>";
    complex_part "{ANG_RAD,ANG_DEG,MAG,IMAG,REAL,NONE}";
  }
~~~

# Description

TODO

## Properties

### `file`

~~~
  char256 file;
~~~

Output file name

### `group`

~~~
  char1024 group;
~~~

Group definition string

### `interval`

~~~
  double interval[s];
~~~

Recordering interval (0 'every iteration', -1 'on change')

### `flush_interval`

~~~
  double flush_interval[s];
~~~

File flush interval (0 never, negative on samples)

### `strict`

~~~
  bool strict;
~~~

Causes the group_recorder to stop the simulation should there be a problem opening or writing with the group_recorder

### `print_units`

~~~
  bool print_units;
~~~

Flag to append units to each written value, if applicable

### `property`

~~~
  char256 property;
~~~

Property to record

### `limit`

~~~
  int32 limit;
~~~

The maximum number of lines to write to the file

### `format`

~~~
  bool format;
~~~

Determines whether output timestamp is formatted to be formatted as human-readable (default) or epoch

### `complex_part`

~~~
  enumeration {ANG_RAD, ANG_DEG, MAG, IMAG, REAL, NONE} complex_part;
~~~

The complex part to record if complex properties are gathered

# Example

~~~
  object group_recorder {
    file "";
    group "";
    interval "0.0";
    flush_interval "0.0";
    strict "FALSE";
    print_units "FALSE";
    property "";
    limit "0";
    format "FALSE";
    complex_part "0";
  }
~~~

# See also

* [[/Module/Tape]]

