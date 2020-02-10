[[/Module/Residential/Range]] -- Class range

# Synopsis

GLM:

~~~
  object range {
    oven_volume "<decimal> gal";
    oven_UA "<decimal> Btu/degF/h";
    oven_diameter "<decimal> ft";
    oven_demand "<decimal> gpm";
    heating_element_capacity "<decimal> kW";
    inlet_food_temperature "<decimal> degF";
    heat_mode "{GASHEAT,ELECTRIC}";
    location "{GARAGE,INSIDE}";
    oven_setpoint "<decimal> degF";
    thermostat_deadband "<decimal> degF";
    temperature "<decimal> degF";
    height "<decimal> ft";
    food_density "<decimal>";
    specificheat_food "<decimal>";
    queue_cooktop "<decimal> unit";
    queue_oven "<decimal> unit";
    queue_min "<decimal> unit";
    queue_max "<decimal> unit";
    time_cooktop_operation "<decimal>";
    time_cooktop_setting "<decimal>";
    cooktop_run_prob "<decimal>";
    oven_run_prob "<decimal>";
    cooktop_coil_setting_1 "<decimal> kW";
    cooktop_coil_setting_2 "<decimal> kW";
    cooktop_coil_setting_3 "<decimal> kW";
    total_power_oven "<decimal> kW";
    total_power_cooktop "<decimal> kW";
    total_power_range "<decimal> kW";
    demand_cooktop "<decimal> unit/day";
    demand_oven "<decimal> unit/day";
    stall_voltage "<decimal> V";
    start_voltage "<decimal> V";
    stall_impedance "<string> Ohm";
    trip_delay "<decimal> s";
    reset_delay "<decimal> s";
    time_oven_operation "<decimal> s";
    time_oven_setting "<decimal> s";
    state_cooktop "{CT_TRIPPED,CT_STALLED,STAGE_8_ONLY,STAGE_7_ONLY,STAGE_6_ONLY,CT_STOPPED}";
    cooktop_energy_baseline "<decimal> kWh";
    cooktop_energy_used "<decimal>";
    Toff "<decimal>";
    Ton "<decimal>";
    cooktop_interval_setting_1 "<decimal> s";
    cooktop_interval_setting_2 "<decimal> s";
    cooktop_interval_setting_3 "<decimal> s";
    cooktop_energy_needed "<decimal> kWh";
    heat_needed "<string>";
    oven_check "<string>";
    remainon "<string>";
    cooktop_check "<string>";
    actual_load "<decimal> kW";
    previous_load "<decimal> kW";
    actual_power "<string> kVA";
    is_range_on "<decimal>";
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

### `oven_volume`

~~~
  double oven_volume[gal];
~~~

The volume of the oven

### `oven_UA`

~~~
  double oven_UA[Btu/degF/h];
~~~

The UA of the oven (surface area divided by R-value)

### `oven_diameter`

~~~
  double oven_diameter[ft];
~~~

The diameter of the oven

### `oven_demand`

~~~
  double oven_demand[gpm];
~~~

The hot food take out from the oven

### `heating_element_capacity`

~~~
  double heating_element_capacity[kW];
~~~

The power of the heating element

### `inlet_food_temperature`

~~~
  double inlet_food_temperature[degF];
~~~

The inlet temperature of the food

### `heat_mode`

~~~
  enumeration {GASHEAT, ELECTRIC} heat_mode;
~~~

The energy source for heating the oven

### `location`

~~~
  enumeration {GARAGE, INSIDE} location;
~~~

Whether the range is inside or outside

### `oven_setpoint`

~~~
  double oven_setpoint[degF];
~~~

The temperature around which the oven will heat its contents

### `thermostat_deadband`

~~~
  double thermostat_deadband[degF];
~~~

The degree to heat the food in the oven, when needed

### `temperature`

~~~
  double temperature[degF];
~~~

The outlet temperature of the oven

### `height`

~~~
  double height[ft];
~~~

The height of the oven

### `food_density`

~~~
  double food_density;
~~~

Food density

### `specificheat_food`

~~~
  double specificheat_food;
~~~

TODO

### `queue_cooktop`

~~~
  double queue_cooktop[unit];
~~~

Number of loads accumulated

### `queue_oven`

~~~
  double queue_oven[unit];
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

### `time_cooktop_operation`

~~~
  double time_cooktop_operation;
~~~

TODO

### `time_cooktop_setting`

~~~
  double time_cooktop_setting;
~~~

TODO

### `cooktop_run_prob`

~~~
  double cooktop_run_prob;
~~~

TODO

### `oven_run_prob`

~~~
  double oven_run_prob;
~~~

TODO

### `cooktop_coil_setting_1`

~~~
  double cooktop_coil_setting_1[kW];
~~~

TODO

### `cooktop_coil_setting_2`

~~~
  double cooktop_coil_setting_2[kW];
~~~

TODO

### `cooktop_coil_setting_3`

~~~
  double cooktop_coil_setting_3[kW];
~~~

TODO

### `total_power_oven`

~~~
  double total_power_oven[kW];
~~~

TODO

### `total_power_cooktop`

~~~
  double total_power_cooktop[kW];
~~~

TODO

### `total_power_range`

~~~
  double total_power_range[kW];
~~~

TODO

### `demand_cooktop`

~~~
  double demand_cooktop[unit/day];
~~~

Number of loads accumulating daily

### `demand_oven`

~~~
  double demand_oven[unit/day];
~~~

Number of loads accumulating daily

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

### `time_oven_operation`

~~~
  double time_oven_operation[s];
~~~

TODO

### `time_oven_setting`

~~~
  double time_oven_setting[s];
~~~

TODO

### `state_cooktop`

~~~
  enumeration {CT_TRIPPED, CT_STALLED, STAGE_8_ONLY, STAGE_7_ONLY, STAGE_6_ONLY, CT_STOPPED} state_cooktop;
~~~

TODO

### `cooktop_energy_baseline`

~~~
  double cooktop_energy_baseline[kWh];
~~~

TODO

### `cooktop_energy_used`

~~~
  double cooktop_energy_used;
~~~

TODO

### `Toff`

~~~
  double Toff;
~~~

TODO

### `Ton`

~~~
  double Ton;
~~~

TODO

### `cooktop_interval_setting_1`

~~~
  double cooktop_interval_setting_1[s];
~~~

TODO

### `cooktop_interval_setting_2`

~~~
  double cooktop_interval_setting_2[s];
~~~

TODO

### `cooktop_interval_setting_3`

~~~
  double cooktop_interval_setting_3[s];
~~~

TODO

### `cooktop_energy_needed`

~~~
  double cooktop_energy_needed[kWh];
~~~

TODO

### `heat_needed`

~~~
  bool heat_needed;
~~~

TODO

### `oven_check`

~~~
  bool oven_check;
~~~

TODO

### `remainon`

~~~
  bool remainon;
~~~

TODO

### `cooktop_check`

~~~
  bool cooktop_check;
~~~

TODO

### `actual_load`

~~~
  double actual_load[kW];
~~~

The actual load based on the current voltage across the coils

### `previous_load`

~~~
  double previous_load[kW];
~~~

The actual load based on current voltage stored for use in controllers

### `actual_power`

~~~
  complex actual_power[kVA];
~~~

The actual power based on the current voltage across the coils

### `is_range_on`

~~~
  double is_range_on;
~~~

Simple logic output to determine state of range (1-on, 0-off)

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
  object range {
    oven_volume "0.0";
    oven_UA "0.0";
    oven_diameter "0.0";
    oven_demand "0.0";
    heating_element_capacity "0.0";
    inlet_food_temperature "0.0";
    heat_mode "0";
    location "0";
    oven_setpoint "0.0";
    thermostat_deadband "0.0";
    temperature "0.0";
    height "0.0";
    food_density "0.0";
    specificheat_food "0.0";
    queue_cooktop "0.0";
    queue_oven "0.0";
    queue_min "0.0";
    queue_max "0.0";
    time_cooktop_operation "0.0";
    time_cooktop_setting "0.0";
    cooktop_run_prob "0.0";
    oven_run_prob "0.0";
    cooktop_coil_setting_1 "0.0";
    cooktop_coil_setting_2 "0.0";
    cooktop_coil_setting_3 "0.0";
    total_power_oven "0.0";
    total_power_cooktop "0.0";
    total_power_range "0.0";
    demand_cooktop "0.0";
    demand_oven "0.0";
    stall_voltage "0.0";
    start_voltage "0.0";
    stall_impedance "0+0i";
    trip_delay "0.0";
    reset_delay "0.0";
    time_oven_operation "0.0";
    time_oven_setting "0.0";
    state_cooktop "0";
    cooktop_energy_baseline "0.0";
    cooktop_energy_used "0.0";
    Toff "0.0";
    Ton "0.0";
    cooktop_interval_setting_1 "0.0";
    cooktop_interval_setting_2 "0.0";
    cooktop_interval_setting_3 "0.0";
    cooktop_energy_needed "0.0";
    heat_needed "FALSE";
    oven_check "FALSE";
    remainon "FALSE";
    cooktop_check "FALSE";
    actual_load "0.0";
    previous_load "0.0";
    actual_power "0+0i";
    is_range_on "0.0";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]

