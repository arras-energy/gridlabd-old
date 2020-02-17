[[/Module/Reliability/Eventgen]] -- Class eventgen

# Synopsis

GLM:

~~~
  object eventgen {
    target_group "<string>";
    fault_type "<string>";
    failure_dist "{TRIANGLE,BETA,GAMMA,WEIBULL,RAYLEIGH,EXPONENTIAL,PARETO,BERNOULLI,LOGNORMAL,NORMAL,UNIFORM}";
    restoration_dist "{TRIANGLE,BETA,GAMMA,WEIBULL,RAYLEIGH,EXPONENTIAL,PARETO,BERNOULLI,LOGNORMAL,NORMAL,UNIFORM}";
    failure_dist_param_1 "<decimal>";
    failure_dist_param_2 "<decimal>";
    restoration_dist_param_1 "<decimal>";
    restoration_dist_param_2 "<decimal>";
    manual_outages "<string>";
    max_outage_length "<decimal> s";
    max_simultaneous_faults "<integer>";
    controlled_switch "<string>";
    switch_state "<integer>";
  }
~~~

# Description

TODO

## Properties

### `target_group`

~~~
  char1024 target_group;
~~~

TODO

### `fault_type`

~~~
  char256 fault_type;
~~~

TODO

### `failure_dist`

~~~
  enumeration {TRIANGLE, BETA, GAMMA, WEIBULL, RAYLEIGH, EXPONENTIAL, PARETO, BERNOULLI, LOGNORMAL, NORMAL, UNIFORM} failure_dist;
~~~

TODO

### `restoration_dist`

~~~
  enumeration {TRIANGLE, BETA, GAMMA, WEIBULL, RAYLEIGH, EXPONENTIAL, PARETO, BERNOULLI, LOGNORMAL, NORMAL, UNIFORM} restoration_dist;
~~~

TODO

### `failure_dist_param_1`

~~~
  double failure_dist_param_1;
~~~

TODO

### `failure_dist_param_2`

~~~
  double failure_dist_param_2;
~~~

TODO

### `restoration_dist_param_1`

~~~
  double restoration_dist_param_1;
~~~

TODO

### `restoration_dist_param_2`

~~~
  double restoration_dist_param_2;
~~~

TODO

### `manual_outages`

~~~
  char1024 manual_outages;
~~~

TODO

### `max_outage_length`

~~~
  double max_outage_length[s];
~~~

TODO

### `max_simultaneous_faults`

~~~
  int32 max_simultaneous_faults;
~~~

TODO

### `controlled_switch`

~~~
  char256 controlled_switch;
~~~

Name of a switch to manually fault/un-fault

### `switch_state`

~~~
  int32 switch_state;
~~~

Current state (1=closed, 0=open) for the controlled switch

# Example

~~~
  object eventgen {
    target_group "";
    fault_type "";
    failure_dist "0";
    restoration_dist "0";
    failure_dist_param_1 "0.0";
    failure_dist_param_2 "0.0";
    restoration_dist_param_1 "0.0";
    restoration_dist_param_2 "0.0";
    manual_outages "";
    max_outage_length "0.0";
    max_simultaneous_faults "0";
    controlled_switch "";
    switch_state "0";
  }
~~~

# See also

* [[/Module/Reliability]]

