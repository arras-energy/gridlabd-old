[[/Module/Powerflow/Volt_var_control]] -- Class volt_var_control

# Synopsis

GLM:

~~~
  object volt_var_control {
    control_method "{STANDBY,ACTIVE}";
    capacitor_delay "<decimal> s";
    regulator_delay "<decimal> s";
    desired_pf "<decimal>";
    d_max "<decimal>";
    d_min "<decimal>";
    substation_link "<string>";
    pf_phase "[CBA]";
    regulator_list "<string>";
    capacitor_list "<string>";
    voltage_measurements "<string>";
    minimum_voltages "<string>";
    maximum_voltages "<string>";
    desired_voltages "<string>";
    max_vdrop "<string>";
    high_load_deadband "<string>";
    low_load_deadband "<string>";
    pf_signed "<string>";
  }
~~~

# Description

TODO

## Properties

### `control_method`
~~~
  enumeration {STANDBY, ACTIVE} control_method;
~~~

IVVC activated or in standby

### `capacitor_delay`
~~~
  double capacitor_delay[s];
~~~

Default capacitor time delay - overridden by local defintions

### `regulator_delay`
~~~
  double regulator_delay[s];
~~~

Default regulator time delay - overriden by local definitions

### `desired_pf`
~~~
  double desired_pf;
~~~

Desired power-factor magnitude at the substation transformer or regulator

### `d_max`
~~~
  double d_max;
~~~

Scaling constant for capacitor switching on - typically 0.3 - 0.6

### `d_min`
~~~
  double d_min;
~~~

Scaling constant for capacitor switching off - typically 0.1 - 0.4

### `substation_link`
~~~
  object substation_link;
~~~

Substation link, transformer, or regulator to measure power factor

### `pf_phase`
~~~
  set {C, B, A} pf_phase;
~~~

Phase to include in power factor monitoring

### `regulator_list`
~~~
  char1024 regulator_list;
~~~

List of voltage regulators for the system, separated by commas

### `capacitor_list`
~~~
  char1024 capacitor_list;
~~~

List of controllable capacitors on the system separated by commas

### `voltage_measurements`
~~~
  char1024 voltage_measurements;
~~~

List of voltage measurement devices, separated by commas

### `minimum_voltages`
~~~
  char1024 minimum_voltages;
~~~

Minimum voltages allowed for feeder, separated by commas

### `maximum_voltages`
~~~
  char1024 maximum_voltages;
~~~

Maximum voltages allowed for feeder, separated by commas

### `desired_voltages`
~~~
  char1024 desired_voltages;
~~~

Desired operating voltages for the regulators, separated by commas

### `max_vdrop`
~~~
  char1024 max_vdrop;
~~~

Maximum voltage drop between feeder and end measurements for each regulator, separated by commas

### `high_load_deadband`
~~~
  char1024 high_load_deadband;
~~~

High loading case voltage deadband for each regulator, separated by commas

### `low_load_deadband`
~~~
  char1024 low_load_deadband;
~~~

Low loading case voltage deadband for each regulator, separated by commas

### `pf_signed`
~~~
  bool pf_signed;
~~~

Set to true to consider the sign on the power factor.  Otherwise, it just maintains the deadband of +/-desired_pf

# Example

~~~
  object volt_var_control {
    control_method "0";
    capacitor_delay "0.0";
    regulator_delay "0.0";
    desired_pf "0.0";
    d_max "0.0";
    d_min "0.0";
    pf_phase "0";
    regulator_list "";
    capacitor_list "";
    voltage_measurements "";
    minimum_voltages "";
    maximum_voltages "";
    desired_voltages "";
    max_vdrop "";
    high_load_deadband "";
    low_load_deadband "";
    pf_signed "FALSE";
  }
~~~

# See also
* [[/Module/Powerflow]]

