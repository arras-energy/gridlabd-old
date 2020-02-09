[[/Module/Tape/Violation_recorder]] -- Class violation_recorder

# Synopsis

GLM:

~~~
  object violation_recorder {
    file "<string>";
    summary "<string>";
    virtual_substation "<string>";
    interval "<decimal> s";
    flush_interval "<decimal> s";
    strict "<string>";
    echo "<string>";
    limit "<integer>";
    violation_delay "<integer>";
    xfrmr_thermal_limit_upper "<decimal>";
    xfrmr_thermal_limit_lower "<decimal>";
    line_thermal_limit_upper "<decimal>";
    line_thermal_limit_lower "<decimal>";
    node_instantaneous_voltage_limit_upper "<decimal>";
    node_instantaneous_voltage_limit_lower "<decimal>";
    node_continuous_voltage_limit_upper "<decimal>";
    node_continuous_voltage_limit_lower "<decimal>";
    node_continuous_voltage_interval "<decimal>";
    secondary_dist_voltage_rise_upper_limit "<decimal>";
    secondary_dist_voltage_rise_lower_limit "<decimal>";
    substation_breaker_A_limit "<decimal>";
    substation_breaker_B_limit "<decimal>";
    substation_breaker_C_limit "<decimal>";
    substation_pf_lower_limit "<decimal>";
    inverter_v_chng_per_interval_upper_bound "<decimal>";
    inverter_v_chng_per_interval_lower_bound "<decimal>";
    inverter_v_chng_interval "<decimal>";
    violation_flag "[ALLVIOLATIONS|VIOLATION8|VIOLATION7|VIOLATION6|VIOLATION5|VIOLATION4|VIOLATION3|VIOLATION2|VIOLATION1|VIOLATION0]";
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

### `summary`
~~~
  char256 summary;
~~~

Summary output file name

### `virtual_substation`
~~~
  char256 virtual_substation;
~~~

Name of the substation node to monitor for reverse flow violation

### `interval`
~~~
  double interval[s];
~~~

Recording interval (0 'every iteration', -1 'on change')

### `flush_interval`
~~~
  double flush_interval[s];
~~~

File flush interval (0 never, negative on samples)

### `strict`
~~~
  bool strict;
~~~

Causes the violation_recorder to stop the simulation should there be a problem opening or writing with the violation_recorder

### `echo`
~~~
  bool echo;
~~~

Causes the violation_recorder to echo messages to the screen

### `limit`
~~~
  int32 limit;
~~~

The maximum number of lines to write to the file

### `violation_delay`
~~~
  int32 violation_delay;
~~~

The number of seconds to skip before recording violations

### `xfrmr_thermal_limit_upper`
~~~
  double xfrmr_thermal_limit_upper;
~~~

Xfrmr_thermal_limit_upper

### `xfrmr_thermal_limit_lower`
~~~
  double xfrmr_thermal_limit_lower;
~~~

Xfrmr_thermal_limit_lower

### `line_thermal_limit_upper`
~~~
  double line_thermal_limit_upper;
~~~

Line_thermal_limit_upper

### `line_thermal_limit_lower`
~~~
  double line_thermal_limit_lower;
~~~

Line_thermal_limit_lower

### `node_instantaneous_voltage_limit_upper`
~~~
  double node_instantaneous_voltage_limit_upper;
~~~

Node_instantaneous_voltage_limit_upper

### `node_instantaneous_voltage_limit_lower`
~~~
  double node_instantaneous_voltage_limit_lower;
~~~

Node_instantaneous_voltage_limit_lower

### `node_continuous_voltage_limit_upper`
~~~
  double node_continuous_voltage_limit_upper;
~~~

Node_continuous_voltage_limit_upper

### `node_continuous_voltage_limit_lower`
~~~
  double node_continuous_voltage_limit_lower;
~~~

Node_continuous_voltage_limit_lower

### `node_continuous_voltage_interval`
~~~
  double node_continuous_voltage_interval;
~~~

Node_continuous_voltage_interval

### `secondary_dist_voltage_rise_upper_limit`
~~~
  double secondary_dist_voltage_rise_upper_limit;
~~~

Secondary_dist_voltage_rise_upper_limit

### `secondary_dist_voltage_rise_lower_limit`
~~~
  double secondary_dist_voltage_rise_lower_limit;
~~~

Secondary_dist_voltage_rise_lower_limit

### `substation_breaker_A_limit`
~~~
  double substation_breaker_A_limit;
~~~

Breaker_phase_A_limit

### `substation_breaker_B_limit`
~~~
  double substation_breaker_B_limit;
~~~

Breaker_phase_B_limit

### `substation_breaker_C_limit`
~~~
  double substation_breaker_C_limit;
~~~

Breaker_phase_C_limit

### `substation_pf_lower_limit`
~~~
  double substation_pf_lower_limit;
~~~

Substation_pf_lower_limit

### `inverter_v_chng_per_interval_upper_bound`
~~~
  double inverter_v_chng_per_interval_upper_bound;
~~~

Inverter_v_chng_per_interval_upper_bound

### `inverter_v_chng_per_interval_lower_bound`
~~~
  double inverter_v_chng_per_interval_lower_bound;
~~~

Inverter_v_chng_per_interval_lower_bound

### `inverter_v_chng_interval`
~~~
  double inverter_v_chng_interval;
~~~

Inverter_v_chng_interval

### `violation_flag`
~~~
  set {ALLVIOLATIONS, VIOLATION8, VIOLATION7, VIOLATION6, VIOLATION5, VIOLATION4, VIOLATION3, VIOLATION2, VIOLATION1, VIOLATION0} violation_flag;
~~~

Bit set for determining which violations to check

# Example

~~~
  object violation_recorder {
    file "";
    summary "";
    virtual_substation "";
    interval "0.0";
    flush_interval "0.0";
    strict "FALSE";
    echo "FALSE";
    limit "0";
    violation_delay "0";
    xfrmr_thermal_limit_upper "0.0";
    xfrmr_thermal_limit_lower "0.0";
    line_thermal_limit_upper "0.0";
    line_thermal_limit_lower "0.0";
    node_instantaneous_voltage_limit_upper "0.0";
    node_instantaneous_voltage_limit_lower "0.0";
    node_continuous_voltage_limit_upper "0.0";
    node_continuous_voltage_limit_lower "0.0";
    node_continuous_voltage_interval "0.0";
    secondary_dist_voltage_rise_upper_limit "0.0";
    secondary_dist_voltage_rise_lower_limit "0.0";
    substation_breaker_A_limit "0.0";
    substation_breaker_B_limit "0.0";
    substation_breaker_C_limit "0.0";
    substation_pf_lower_limit "0.0";
    inverter_v_chng_per_interval_upper_bound "0.0";
    inverter_v_chng_per_interval_lower_bound "0.0";
    inverter_v_chng_interval "0.0";
    violation_flag "0";
  }
~~~

# See also
* [[/Module/Tape]]

