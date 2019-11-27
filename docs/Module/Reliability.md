[[/Module/Reliability]] -- Module reliability

# Synopsis
GLM:
~~~
  module reliability {
    message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    enable_subsecond_models "<string>";
    maximum_event_length "<decimal> s";
    report_event_log "<string>";
    deltamode_timestep "<integer>";
  }
~~~

# Description

TODO

## Globals

### `message_flags`
~~~
  message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
~~~

Module message control flags

### `enable_subsecond_models`
~~~
  enable_subsecond_models "<string>";
~~~

Flag to enable deltamode functionality in the reliability module

### `maximum_event_length`
~~~
  maximum_event_length "<decimal> s";
~~~

Maximum duration of any faulting event

### `report_event_log`
~~~
  report_event_log "<string>";
~~~

Should the metrics object dump a logfile?

### `deltamode_timestep`
~~~
  deltamode_timestep "<integer>";
~~~

Default timestep for reliability deltamode operations

# See also
* [[/Module/Reliability/Metrics]]
* [[/Module/Reliability/Eventgen]]

