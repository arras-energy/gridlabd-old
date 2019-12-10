[[/Module/Residential/Refrigerator]] -- Class refrigerator

# Synopsis
GLM:
~~~
  object refrigerator {
    size "<decimal> cf";
    rated_capacity "<decimal> Btu/h";
    temperature "<decimal> degF";
    setpoint "<decimal> degF";
    deadband "<decimal> degF";
    cycle_time "<decimal> s";
    output "<decimal>";
    event_temp "<decimal>";
    UA "<decimal> Btu/degF/h";
    compressor_off_normal_energy "<decimal>";
    compressor_off_normal_power "<decimal> W";
    compressor_on_normal_energy "<decimal>";
    compressor_on_normal_power "<decimal> W";
    defrost_energy "<decimal>";
    defrost_power "<decimal> W";
    icemaking_energy "<decimal>";
    icemaking_power "<decimal> W";
    ice_making_probability "<decimal>";
    FF_Door_Openings "<integer>";
    door_opening_energy "<integer>";
    door_opening_power "<integer>";
    DO_Thershold "<decimal>";
    dr_mode_double "<decimal>";
    energy_needed "<decimal>";
    energy_used "<decimal>";
    refrigerator_power "<decimal>";
    icemaker_running "<string>";
    check_DO "<integer>";
    is_240 "<string>";
    defrostDelayed "<decimal>";
    long_compressor_cycle_due "<string>";
    long_compressor_cycle_time "<decimal>";
    long_compressor_cycle_power "<decimal>";
    long_compressor_cycle_energy "<decimal>";
    long_compressor_cycle_threshold "<decimal>";
    defrost_criterion "{COMPRESSOR_TIME,DOOR_OPENINGS,TIMED}";
    run_defrost "<string>";
    door_opening_criterion "<decimal>";
    compressor_defrost_time "<decimal>";
    delay_defrost_time "<decimal>";
    daily_door_opening "<integer>";
    state "{COMPRESSSOR_ON_NORMAL,COMPRESSSOR_ON_LONG,COMPRESSSOR_OFF_NORMAL,DEFROST}";
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

### `size`
~~~
  double size[cf];
~~~

Volume of the refrigerator

### `rated_capacity`
~~~
  double rated_capacity[Btu/h];
~~~

TODO

### `temperature`
~~~
  double temperature[degF];
~~~

TODO

### `setpoint`
~~~
  double setpoint[degF];
~~~

TODO

### `deadband`
~~~
  double deadband[degF];
~~~

TODO

### `cycle_time`
~~~
  double cycle_time[s];
~~~

TODO

### `output`
~~~
  double output;
~~~

TODO

### `event_temp`
~~~
  double event_temp;
~~~

TODO

### `UA`
~~~
  double UA[Btu/degF/h];
~~~

TODO

### `compressor_off_normal_energy`
~~~
  double compressor_off_normal_energy;
~~~

TODO

### `compressor_off_normal_power`
~~~
  double compressor_off_normal_power[W];
~~~

TODO

### `compressor_on_normal_energy`
~~~
  double compressor_on_normal_energy;
~~~

TODO

### `compressor_on_normal_power`
~~~
  double compressor_on_normal_power[W];
~~~

TODO

### `defrost_energy`
~~~
  double defrost_energy;
~~~

TODO

### `defrost_power`
~~~
  double defrost_power[W];
~~~

TODO

### `icemaking_energy`
~~~
  double icemaking_energy;
~~~

TODO

### `icemaking_power`
~~~
  double icemaking_power[W];
~~~

TODO

### `ice_making_probability`
~~~
  double ice_making_probability;
~~~

TODO

### `FF_Door_Openings`
~~~
  int32 FF_Door_Openings;
~~~

TODO

### `door_opening_energy`
~~~
  int32 door_opening_energy;
~~~

TODO

### `door_opening_power`
~~~
  int32 door_opening_power;
~~~

TODO

### `DO_Thershold`
~~~
  double DO_Thershold;
~~~

TODO

### `dr_mode_double`
~~~
  double dr_mode_double;
~~~

TODO

### `energy_needed`
~~~
  double energy_needed;
~~~

TODO

### `energy_used`
~~~
  double energy_used;
~~~

TODO

### `refrigerator_power`
~~~
  double refrigerator_power;
~~~

TODO

### `icemaker_running`
~~~
  bool icemaker_running;
~~~

TODO

### `check_DO`
~~~
  int32 check_DO;
~~~

TODO

### `is_240`
~~~
  bool is_240;
~~~

TODO

### `defrostDelayed`
~~~
  double defrostDelayed;
~~~

TODO

### `long_compressor_cycle_due`
~~~
  bool long_compressor_cycle_due;
~~~

TODO

### `long_compressor_cycle_time`
~~~
  double long_compressor_cycle_time;
~~~

TODO

### `long_compressor_cycle_power`
~~~
  double long_compressor_cycle_power;
~~~

TODO

### `long_compressor_cycle_energy`
~~~
  double long_compressor_cycle_energy;
~~~

TODO

### `long_compressor_cycle_threshold`
~~~
  double long_compressor_cycle_threshold;
~~~

TODO

### `defrost_criterion`
~~~
  enumeration {COMPRESSOR_TIME, DOOR_OPENINGS, TIMED} defrost_criterion;
~~~

TODO

### `run_defrost`
~~~
  bool run_defrost;
~~~

TODO

### `door_opening_criterion`
~~~
  double door_opening_criterion;
~~~

TODO

### `compressor_defrost_time`
~~~
  double compressor_defrost_time;
~~~

TODO

### `delay_defrost_time`
~~~
  double delay_defrost_time;
~~~

TODO

### `daily_door_opening`
~~~
  int32 daily_door_opening;
~~~

TODO

### `state`
~~~
  enumeration {COMPRESSSOR_ON_NORMAL, COMPRESSSOR_ON_LONG, COMPRESSSOR_OFF_NORMAL, DEFROST} state;
~~~

TODO

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
  object refrigerator {
    size "0.0";
    rated_capacity "0.0";
    temperature "0.0";
    setpoint "0.0";
    deadband "0.0";
    cycle_time "0.0";
    output "0.0";
    event_temp "0.0";
    UA "0.0";
    compressor_off_normal_energy "0.0";
    compressor_off_normal_power "0.0";
    compressor_on_normal_energy "0.0";
    compressor_on_normal_power "0.0";
    defrost_energy "0.0";
    defrost_power "0.0";
    icemaking_energy "0.0";
    icemaking_power "0.0";
    ice_making_probability "0.0";
    FF_Door_Openings "0";
    door_opening_energy "0";
    door_opening_power "0";
    DO_Thershold "0.0";
    dr_mode_double "0.0";
    energy_needed "0.0";
    energy_used "0.0";
    refrigerator_power "0.0";
    icemaker_running "FALSE";
    check_DO "0";
    is_240 "FALSE";
    defrostDelayed "0.0";
    long_compressor_cycle_due "FALSE";
    long_compressor_cycle_time "0.0";
    long_compressor_cycle_power "0.0";
    long_compressor_cycle_energy "0.0";
    long_compressor_cycle_threshold "0.0";
    defrost_criterion "0";
    run_defrost "FALSE";
    door_opening_criterion "0.0";
    compressor_defrost_time "0.0";
    delay_defrost_time "0.0";
    daily_door_opening "0";
    state "0";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

