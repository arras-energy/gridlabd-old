[[/Module/Reliability/Metrics]] -- Class metrics

# Synopsis

GLM:

~~~
  object metrics {
    report_file "<string>";
    customer_group "<string>";
    module_metrics_object "<string>";
    metrics_of_interest "<string>";
    metric_interval "<decimal> s";
    report_interval "<decimal> s";
  }
~~~

# Description

TODO

## Properties

### `report_file`
~~~
  char1024 report_file;
~~~

TODO

### `customer_group`
~~~
  char1024 customer_group;
~~~

TODO

### `module_metrics_object`
~~~
  object module_metrics_object;
~~~

TODO

### `metrics_of_interest`
~~~
  char1024 metrics_of_interest;
~~~

TODO

### `metric_interval`
~~~
  double metric_interval[s];
~~~

TODO

### `report_interval`
~~~
  double report_interval[s];
~~~

TODO

# Example

~~~
  object metrics {
    report_file "";
    customer_group "";
    metrics_of_interest "";
    metric_interval "0.0";
    report_interval "0.0";
  }
~~~

# See also
* [[/Module/Reliability]]

