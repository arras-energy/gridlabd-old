[[/Module/Residential/Dishwasher]] -- Class dishwasher

# Synopsis

GLM:

~~~
  object dishwasher {
    control_power "<decimal> W";
    dishwasher_coil_power_1 "<decimal> W";
    dishwasher_coil_power_2 "<decimal> W";
    dishwasher_coil_power_3 "<decimal> W";
    motor_power "<decimal> W";
    circuit_split "<decimal>";
    queue "<decimal> unit";
    stall_voltage "<decimal> V";
    start_voltage "<decimal> V";
    stall_impedance "<string> Ohm";
    trip_delay "<decimal> s";
    reset_delay "<decimal> s";
    total_power "<decimal> W";
    state "{HEATEDDRY_ONLY,CONTROL_ONLY,COIL_ONLY,MOTOR_COIL_ONLY,MOTOR_ONLY,TRIPPED,STALLED,STOPPED}";
    energy_baseline "<decimal> kWh";
    energy_used "<decimal> kWh";
    control_check1 "<string>";
    control_check2 "<string>";
    control_check3 "<string>";
    control_check4 "<string>";
    control_check5 "<string>";
    control_check6 "<string>";
    control_check7 "<string>";
    control_check8 "<string>";
    control_check9 "<string>";
    control_check10 "<string>";
    control_check11 "<string>";
    control_check12 "<string>";
    control_check_temp "<string>";
    motor_only_check1 "<string>";
    motor_only_check2 "<string>";
    motor_only_check3 "<string>";
    motor_only_check4 "<string>";
    motor_only_check5 "<string>";
    motor_only_check6 "<string>";
    motor_only_check7 "<string>";
    motor_only_check8 "<string>";
    motor_only_check9 "<string>";
    motor_only_temp1 "<string>";
    motor_only_temp2 "<string>";
    motor_coil_only_check1 "<string>";
    motor_coil_only_check2 "<string>";
    heateddry_check1 "<string>";
    heateddry_check2 "<string>";
    coil_only_check1 "<string>";
    coil_only_check2 "<string>";
    coil_only_check3 "<string>";
    Heateddry_option_check "<string>";
    queue_min "<decimal> unit";
    queue_max "<decimal> unit";
    pulse_interval_1 "<decimal> s";
    pulse_interval_2 "<decimal> s";
    pulse_interval_3 "<decimal> s";
    pulse_interval_4 "<decimal> s";
    pulse_interval_5 "<decimal> s";
    pulse_interval_6 "<decimal> s";
    pulse_interval_7 "<decimal> s";
    pulse_interval_8 "<decimal> s";
    pulse_interval_9 "<decimal> s";
    pulse_interval_10 "<decimal> s";
    pulse_interval_11 "<decimal> s";
    pulse_interval_12 "<decimal> s";
    pulse_interval_13 "<decimal> s";
    pulse_interval_14 "<decimal> s";
    pulse_interval_15 "<decimal> s";
    pulse_interval_16 "<decimal> s";
    pulse_interval_17 "<decimal> s";
    pulse_interval_18 "<decimal> s";
    pulse_interval_19 "<decimal> s";
    dishwasher_run_prob "<decimal>";
    energy_needed "<decimal> kWh";
    dishwasher_demand "<decimal> kWh";
    daily_dishwasher_demand "<decimal> kWh";
    actual_dishwasher_demand "<decimal> kWh";
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

### `control_power`

~~~
  double control_power[W];
~~~

TODO

### `dishwasher_coil_power_1`

~~~
  double dishwasher_coil_power_1[W];
~~~

TODO

### `dishwasher_coil_power_2`

~~~
  double dishwasher_coil_power_2[W];
~~~

TODO

### `dishwasher_coil_power_3`

~~~
  double dishwasher_coil_power_3[W];
~~~

TODO

### `motor_power`

~~~
  double motor_power[W];
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
  enumeration {HEATEDDRY_ONLY, CONTROL_ONLY, COIL_ONLY, MOTOR_COIL_ONLY, MOTOR_ONLY, TRIPPED, STALLED, STOPPED} state;
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

### `control_check1`

~~~
  bool control_check1;
~~~

TODO

### `control_check2`

~~~
  bool control_check2;
~~~

TODO

### `control_check3`

~~~
  bool control_check3;
~~~

TODO

### `control_check4`

~~~
  bool control_check4;
~~~

TODO

### `control_check5`

~~~
  bool control_check5;
~~~

TODO

### `control_check6`

~~~
  bool control_check6;
~~~

TODO

### `control_check7`

~~~
  bool control_check7;
~~~

TODO

### `control_check8`

~~~
  bool control_check8;
~~~

TODO

### `control_check9`

~~~
  bool control_check9;
~~~

TODO

### `control_check10`

~~~
  bool control_check10;
~~~

TODO

### `control_check11`

~~~
  bool control_check11;
~~~

TODO

### `control_check12`

~~~
  bool control_check12;
~~~

TODO

### `control_check_temp`

~~~
  bool control_check_temp;
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

### `motor_only_check7`

~~~
  bool motor_only_check7;
~~~

TODO

### `motor_only_check8`

~~~
  bool motor_only_check8;
~~~

TODO

### `motor_only_check9`

~~~
  bool motor_only_check9;
~~~

TODO

### `motor_only_temp1`

~~~
  bool motor_only_temp1;
~~~

TODO

### `motor_only_temp2`

~~~
  bool motor_only_temp2;
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

### `heateddry_check1`

~~~
  bool heateddry_check1;
~~~

TODO

### `heateddry_check2`

~~~
  bool heateddry_check2;
~~~

TODO

### `coil_only_check1`

~~~
  bool coil_only_check1;
~~~

TODO

### `coil_only_check2`

~~~
  bool coil_only_check2;
~~~

TODO

### `coil_only_check3`

~~~
  bool coil_only_check3;
~~~

TODO

### `Heateddry_option_check`

~~~
  bool Heateddry_option_check;
~~~

TODO

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

### `pulse_interval_8`

~~~
  double pulse_interval_8[s];
~~~

TODO

### `pulse_interval_9`

~~~
  double pulse_interval_9[s];
~~~

TODO

### `pulse_interval_10`

~~~
  double pulse_interval_10[s];
~~~

TODO

### `pulse_interval_11`

~~~
  double pulse_interval_11[s];
~~~

TODO

### `pulse_interval_12`

~~~
  double pulse_interval_12[s];
~~~

TODO

### `pulse_interval_13`

~~~
  double pulse_interval_13[s];
~~~

TODO

### `pulse_interval_14`

~~~
  double pulse_interval_14[s];
~~~

TODO

### `pulse_interval_15`

~~~
  double pulse_interval_15[s];
~~~

TODO

### `pulse_interval_16`

~~~
  double pulse_interval_16[s];
~~~

TODO

### `pulse_interval_17`

~~~
  double pulse_interval_17[s];
~~~

TODO

### `pulse_interval_18`

~~~
  double pulse_interval_18[s];
~~~

TODO

### `pulse_interval_19`

~~~
  double pulse_interval_19[s];
~~~

TODO

### `dishwasher_run_prob`

~~~
  double dishwasher_run_prob;
~~~

TODO

### `energy_needed`

~~~
  double energy_needed[kWh];
~~~

TODO

### `dishwasher_demand`

~~~
  double dishwasher_demand[kWh];
~~~

TODO

### `daily_dishwasher_demand`

~~~
  double daily_dishwasher_demand[kWh];
~~~

TODO

### `actual_dishwasher_demand`

~~~
  double actual_dishwasher_demand[kWh];
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
  object dishwasher {
    control_power "0.0";
    dishwasher_coil_power_1 "0.0";
    dishwasher_coil_power_2 "0.0";
    dishwasher_coil_power_3 "0.0";
    motor_power "0.0";
    circuit_split "0.0";
    queue "0.0";
    stall_voltage "0.0";
    start_voltage "0.0";
    stall_impedance "0+0i";
    trip_delay "0.0";
    reset_delay "0.0";
    total_power "0.0";
    state "0";
    energy_baseline "0.0";
    energy_used "0.0";
    control_check1 "FALSE";
    control_check2 "FALSE";
    control_check3 "FALSE";
    control_check4 "FALSE";
    control_check5 "FALSE";
    control_check6 "FALSE";
    control_check7 "FALSE";
    control_check8 "FALSE";
    control_check9 "FALSE";
    control_check10 "FALSE";
    control_check11 "FALSE";
    control_check12 "FALSE";
    control_check_temp "FALSE";
    motor_only_check1 "FALSE";
    motor_only_check2 "FALSE";
    motor_only_check3 "FALSE";
    motor_only_check4 "FALSE";
    motor_only_check5 "FALSE";
    motor_only_check6 "FALSE";
    motor_only_check7 "FALSE";
    motor_only_check8 "FALSE";
    motor_only_check9 "FALSE";
    motor_only_temp1 "FALSE";
    motor_only_temp2 "FALSE";
    motor_coil_only_check1 "FALSE";
    motor_coil_only_check2 "FALSE";
    heateddry_check1 "FALSE";
    heateddry_check2 "FALSE";
    coil_only_check1 "FALSE";
    coil_only_check2 "FALSE";
    coil_only_check3 "FALSE";
    Heateddry_option_check "FALSE";
    queue_min "0.0";
    queue_max "0.0";
    pulse_interval_1 "0.0";
    pulse_interval_2 "0.0";
    pulse_interval_3 "0.0";
    pulse_interval_4 "0.0";
    pulse_interval_5 "0.0";
    pulse_interval_6 "0.0";
    pulse_interval_7 "0.0";
    pulse_interval_8 "0.0";
    pulse_interval_9 "0.0";
    pulse_interval_10 "0.0";
    pulse_interval_11 "0.0";
    pulse_interval_12 "0.0";
    pulse_interval_13 "0.0";
    pulse_interval_14 "0.0";
    pulse_interval_15 "0.0";
    pulse_interval_16 "0.0";
    pulse_interval_17 "0.0";
    pulse_interval_18 "0.0";
    pulse_interval_19 "0.0";
    dishwasher_run_prob "0.0";
    energy_needed "0.0";
    dishwasher_demand "0.0";
    daily_dishwasher_demand "0.0";
    actual_dishwasher_demand "0.0";
    motor_on_off "0.0";
    motor_coil_on_off "0.0";
    is_240 "FALSE";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]

