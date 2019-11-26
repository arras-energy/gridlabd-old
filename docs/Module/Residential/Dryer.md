[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  object dryer {
    motor_power "<decimal> W";
    dryer_coil_power "<decimal> W";
    controls_power "<decimal> W";
    circuit_split "<decimal>";
    queue "<decimal> unit";
    queue_min "<decimal> unit";
    queue_max "<decimal> unit";
    stall_voltage "<decimal> V";
    start_voltage "<decimal> V";
    stall_impedance "<string> Ohm";
    trip_delay "<decimal> s";
    reset_delay "<decimal> s";
    total_power "<decimal> W";
    state "{CONTROL_ONLY,MOTOR_COIL_ONLY,MOTOR_ONLY,TRIPPED,STALLED,STOPPED}";
    energy_baseline "<decimal> kWh";
    energy_used "<decimal> kWh";
    next_t "<decimal>";
    control_check "<string>";
    motor_only_check1 "<string>";
    motor_only_check2 "<string>";
    motor_only_check3 "<string>";
    motor_only_check4 "<string>";
    motor_only_check5 "<string>";
    motor_only_check6 "<string>";
    dryer_on "<string>";
    dryer_ready "<string>";
    dryer_check "<string>";
    motor_coil_only_check1 "<string>";
    motor_coil_only_check2 "<string>";
    motor_coil_only_check3 "<string>";
    motor_coil_only_check4 "<string>";
    motor_coil_only_check5 "<string>";
    motor_coil_only_check6 "<string>";
    dryer_run_prob "<decimal>";
    dryer_turn_on "<decimal>";
    pulse_interval_1 "<decimal> s";
    pulse_interval_2 "<decimal> s";
    pulse_interval_3 "<decimal> s";
    pulse_interval_4 "<decimal> s";
    pulse_interval_5 "<decimal> s";
    pulse_interval_6 "<decimal> s";
    pulse_interval_7 "<decimal> s";
    energy_needed "<decimal> kWh";
    daily_dryer_demand "<decimal> kWh";
    actual_dryer_demand "<decimal> kWh";
    motor_on_off "<decimal>";
    motor_coil_on_off "<decimal>";
    is_240 "<string>";
    shape "<string>";
    load "<string>";
    energy "<string> kVAh";
    power "<string> kVA";
    peak_demand "<string> kVA";
    heatgain "<decimal> Btu/h";
    cumulative_heatgain "<decimal> Btu";
    heatgain_fraction "<decimal> pu";
    current_fraction "<decimal> pu";
    impedance_fraction "<decimal> pu";
    power_fraction "<decimal> pu";
    power_factor "<decimal>";
    constant_power "<string> kVA";
    constant_current "<string> kVA";
    constant_admittance "<string> kVA";
    voltage_factor "<decimal> pu";
    breaker_amps "<decimal> A";
    configuration "[IS220|IS110]";
    override "{OFF,ON,NORMAL}";
    power_state "{UNKNOWN,ON,OFF}";
  }
~~~

# Description

TODO

## Properties

### `motor_power`
~~~
  double motor_power[W];
~~~

TODO

### `dryer_coil_power`
~~~
  double dryer_coil_power[W];
~~~

TODO

### `controls_power`
~~~
  double controls_power[W];
~~~

TODO

### `circuit_split`
~~~
  double circuit_split;
~~~

TODO

### `queue`
~~~
  double queue[unit];
~~~

Number of loads accumulated

### `queue_min`
~~~
  double queue_min[unit];
~~~

TODO

### `queue_max`
~~~
  double queue_max[unit];
~~~

TODO

### `stall_voltage`
~~~
  double stall_voltage[V];
~~~

TODO

### `start_voltage`
~~~
  double start_voltage[V];
~~~

TODO

### `stall_impedance`
~~~
  complex stall_impedance[Ohm];
~~~

TODO

### `trip_delay`
~~~
  double trip_delay[s];
~~~

TODO

### `reset_delay`
~~~
  double reset_delay[s];
~~~

TODO

### `total_power`
~~~
  double total_power[W];
~~~

TODO

### `state`
~~~
  enumeration {CONTROL_ONLY, MOTOR_COIL_ONLY, MOTOR_ONLY, TRIPPED, STALLED, STOPPED} state;
~~~

TODO

### `energy_baseline`
~~~
  double energy_baseline[kWh];
~~~

TODO

### `energy_used`
~~~
  double energy_used[kWh];
~~~

TODO

### `next_t`
~~~
  double next_t;
~~~

TODO

### `control_check`
~~~
  bool control_check;
~~~

TODO

### `motor_only_check1`
~~~
  bool motor_only_check1;
~~~

TODO

### `motor_only_check2`
~~~
  bool motor_only_check2;
~~~

TODO

### `motor_only_check3`
~~~
  bool motor_only_check3;
~~~

TODO

### `motor_only_check4`
~~~
  bool motor_only_check4;
~~~

TODO

### `motor_only_check5`
~~~
  bool motor_only_check5;
~~~

TODO

### `motor_only_check6`
~~~
  bool motor_only_check6;
~~~

TODO

### `dryer_on`
~~~
  bool dryer_on;
~~~

TODO

### `dryer_ready`
~~~
  bool dryer_ready;
~~~

TODO

### `dryer_check`
~~~
  bool dryer_check;
~~~

TODO

### `motor_coil_only_check1`
~~~
  bool motor_coil_only_check1;
~~~

TODO

### `motor_coil_only_check2`
~~~
  bool motor_coil_only_check2;
~~~

TODO

### `motor_coil_only_check3`
~~~
  bool motor_coil_only_check3;
~~~

TODO

### `motor_coil_only_check4`
~~~
  bool motor_coil_only_check4;
~~~

TODO

### `motor_coil_only_check5`
~~~
  bool motor_coil_only_check5;
~~~

TODO

### `motor_coil_only_check6`
~~~
  bool motor_coil_only_check6;
~~~

TODO

### `dryer_run_prob`
~~~
  double dryer_run_prob;
~~~

TODO

### `dryer_turn_on`
~~~
  double dryer_turn_on;
~~~

TODO

### `pulse_interval_1`
~~~
  double pulse_interval_1[s];
~~~

TODO

### `pulse_interval_2`
~~~
  double pulse_interval_2[s];
~~~

TODO

### `pulse_interval_3`
~~~
  double pulse_interval_3[s];
~~~

TODO

### `pulse_interval_4`
~~~
  double pulse_interval_4[s];
~~~

TODO

### `pulse_interval_5`
~~~
  double pulse_interval_5[s];
~~~

TODO

### `pulse_interval_6`
~~~
  double pulse_interval_6[s];
~~~

TODO

### `pulse_interval_7`
~~~
  double pulse_interval_7[s];
~~~

TODO

### `energy_needed`
~~~
  double energy_needed[kWh];
~~~

TODO

### `daily_dryer_demand`
~~~
  double daily_dryer_demand[kWh];
~~~

TODO

### `actual_dryer_demand`
~~~
  double actual_dryer_demand[kWh];
~~~

TODO

### `motor_on_off`
~~~
  double motor_on_off;
~~~

TODO

### `motor_coil_on_off`
~~~
  double motor_coil_on_off;
~~~

TODO

### `is_240`
~~~
  bool is_240;
~~~

Load is 220/240 V (across both phases)

### `shape`
~~~
  loadshape shape;
~~~

TODO

### `load`
~~~
  enduse load;
~~~

The enduse load description

### `energy`
~~~
  complex energy[kVAh];
~~~

The total energy consumed since the last meter reading

### `power`
~~~
  complex power[kVA];
~~~

The total power consumption of the load

### `peak_demand`
~~~
  complex peak_demand[kVA];
~~~

The peak power consumption since the last meter reading

### `heatgain`
~~~
  double heatgain[Btu/h];
~~~

The heat transferred from the enduse to the parent

### `cumulative_heatgain`
~~~
  double cumulative_heatgain[Btu];
~~~

The cumulative heatgain from the enduse to the parent

### `heatgain_fraction`
~~~
  double heatgain_fraction[pu];
~~~

The fraction of the heat that goes to the parent

### `current_fraction`
~~~
  double current_fraction[pu];
~~~

The fraction of total power that is constant current

### `impedance_fraction`
~~~
  double impedance_fraction[pu];
~~~

The fraction of total power that is constant impedance

### `power_fraction`
~~~
  double power_fraction[pu];
~~~

The fraction of the total power that is constant power

### `power_factor`
~~~
  double power_factor;
~~~

The power factor of the load

### `constant_power`
~~~
  complex constant_power[kVA];
~~~

The constant power portion of the total load

### `constant_current`
~~~
  complex constant_current[kVA];
~~~

The constant current portion of the total load

### `constant_admittance`
~~~
  complex constant_admittance[kVA];
~~~

The constant admittance portion of the total load

### `voltage_factor`
~~~
  double voltage_factor[pu];
~~~

The voltage change factor

### `breaker_amps`
~~~
  double breaker_amps[A];
~~~

The rated breaker amperage

### `configuration`
~~~
  set {IS220, IS110} configuration;
~~~

The load configuration options

### `override`
~~~
  enumeration {OFF, ON, NORMAL} override;
~~~

TODO

### `power_state`
~~~
  enumeration {UNKNOWN, ON, OFF} power_state;
~~~

TODO

# Example

~~~
  object dryer {
    motor_power "0.0";
    dryer_coil_power "0.0";
    controls_power "0.0";
    circuit_split "0.0";
    queue "0.0";
    queue_min "0.0";
    queue_max "0.0";
    stall_voltage "0.0";
    start_voltage "0.0";
    stall_impedance "0+0i";
    trip_delay "0.0";
    reset_delay "0.0";
    total_power "0.0";
    state "0";
    energy_baseline "0.0";
    energy_used "0.0";
    next_t "0.0";
    control_check "FALSE";
    motor_only_check1 "FALSE";
    motor_only_check2 "FALSE";
    motor_only_check3 "FALSE";
    motor_only_check4 "FALSE";
    motor_only_check5 "FALSE";
    motor_only_check6 "FALSE";
    dryer_on "FALSE";
    dryer_ready "FALSE";
    dryer_check "FALSE";
    motor_coil_only_check1 "FALSE";
    motor_coil_only_check2 "FALSE";
    motor_coil_only_check3 "FALSE";
    motor_coil_only_check4 "FALSE";
    motor_coil_only_check5 "FALSE";
    motor_coil_only_check6 "FALSE";
    dryer_run_prob "0.0";
    dryer_turn_on "0.0";
    pulse_interval_1 "0.0";
    pulse_interval_2 "0.0";
    pulse_interval_3 "0.0";
    pulse_interval_4 "0.0";
    pulse_interval_5 "0.0";
    pulse_interval_6 "0.0";
    pulse_interval_7 "0.0";
    energy_needed "0.0";
    daily_dryer_demand "0.0";
    actual_dryer_demand "0.0";
    motor_on_off "0.0";
    motor_coil_on_off "0.0";
    is_240 "FALSE";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

