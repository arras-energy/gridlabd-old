[[/Module/Tape]] -- Module tape

# Synopsis
GLM:
~~~
  object metrics_collector {
    interval "<decimal> s";
  }
~~~

# Description

TODO

## Properties

### `interval`
~~~
  double interval[s];
~~~

Interval at which the metrics_collector output is stored in JSON format

# Example

~~~
  object metrics_collector {
    interval "0.0";
  }
~~~

# See also
* [[/Module/Tape]]

