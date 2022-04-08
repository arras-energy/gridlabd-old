[[/Module/Resilience/Impact]] -- Resilience impact measurement

# Synopsis

GLM:

~~~
class impact {
    object node;
    int32 value;
    double duration[h];
}
object impact 
{
    parent METRICS_NAME;
    node NODE_NAME;
    value INITIAL_VALUE;
    duration VIOLATION_THRESHOLD;
}
~~~

# Description

The `impact` object monitors the powerflow node NODE_NAME for violations of current, voltage, power, thermal, or control limits.  If a violation is detected and lasts longer than VIOLATION_THRESHOLD, then the impact metric in the metrics object METRICS_NAME is incremented by one. 

## Properties

### `object node`

The node which is monitored for violations of current, voltage, power, thermal, or control limits.

### `int32 value`

The total number of events observed.

### `double duration[h]`

The minimum number of hours a violation must last to be considered a reportable event.

# See also

* [[/Module/Resilience/Cost]]
* [[/Module/Resilience/Metrics]]
* [[/Module/Resilience/Outage]]
