[[/Module/Tape/Metrics_collector_writer]] -- Class metrics_collector_writer

# Synopsis

GLM:

~~~
  object metrics_collector_writer {
    filename "<string>";
    interval "<decimal> s";
  }
~~~

# Description

TODO

## Properties

### `filename`

~~~
  char256 filename;
~~~

The JSON formatted output file name

### `interval`

~~~
  double interval[s];
~~~

Interval at which the metrics_collector_writer output is stored in JSON format

# Example

~~~
  object metrics_collector_writer {
    filename "";
    interval "0.0";
  }
~~~

# See also

* [[/Module/Tape]]

