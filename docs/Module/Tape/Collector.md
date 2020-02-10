[[/Module/Tape/Collector]] -- Class collector

# Synopsis

GLM:

~~~
  object collector {
    trigger "<string>";
    file "<string>";
    limit "<integer>";
    group "<string>";
    flush "<integer>";
    interval "<decimal> s";
    property "<string>";
  }
~~~

# Description

TODO

## Properties

### `trigger`

~~~
  char32 trigger;
~~~

TODO

### `file`

~~~
  char1024 file;
~~~

TODO

### `limit`

~~~
  int32 limit;
~~~

TODO

### `group`

~~~
  char256 group;
~~~

TODO

### `flush`

~~~
  int32 flush;
~~~

TODO

### `interval`

~~~
  double interval[s];
~~~

TODO

### `property`

~~~
  method property;
~~~

TODO

# Example

~~~
  object collector {
    trigger "";
    file "";
    limit "0";
    group "";
    flush "0";
    interval "0.0";
  }
~~~

# See also

* [[/Module/Tape]]

