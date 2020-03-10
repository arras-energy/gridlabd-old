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

Collectors aggregate multiple object properties into a single value. They do not use the parent property but instead use the group property to form a collection of objects over which the aggregate is taken.


## Properties

### `trigger`

~~~
  char32 trigger;
~~~

Trigger which will start the recording.

### `file`

~~~
  char1024 file;
~~~

File name specification

### `limit`

~~~
  int32 limit;
~~~

Size limit of allowed entries in the recording.

### `group`

~~~
  char256 group;
~~~

Defines the group to be recorded and the aggregator

~~~
property aggregator(property)[,...];
~~~

### `flush`

~~~
  int32 flush;
~~~

By default the output buffer is flushed to disk when it is full (the size of the buffer is system specific). This default corresponds to the flush value -1. If flush is set to 0, the buffer is flushed every time a record is written. If flush is set to a value greater than 0, the buffer is flushed whenever the condition clock mod flush == 0 is satisfied.

### `interval`

~~~
  double interval[s];
~~~

Recording interval.

### `property`

~~~
  method property;
~~~

The group property specifies the grouping rule for creating the collection. Groups may be specified using any registered property of the object, such as class, size, parent, id, or rank. The property value is aggregated as a minimum, maximum, count, average, standard deviation, mean, variance (2nd moment), mean bias error (1st moment), or kurtosis (3rd moment). 

If the property is a complex number, the property must be specified in the form

~~~
  property.part
~~~

where `part` is `real`, `imag` (for imaginary), `mag` for magnitude, `ang` for the angle in degrees, or `arg` for angle in radians.

For example, a collector over all water heater objects might aggregate the power property using
~~~
count(power),min(power),max(power),std(power)
~~~
which would print the number of water heaters, the minimum power used by any one water heater, the maximum power used, and the standard deviation of the power used by the set of water heaters.

Aggregating options are `min`, `max`, `count`, `avg`, `std`, `mean`, `var`, `mbe`, `kur`, `sum`, `prod`, `skew`, and `gamma`. (`kur` and `skew` remain unimplemented, and will tell you so if you try to use them).

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

