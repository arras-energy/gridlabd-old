[[/Module/Residential/ZIPload]] -- Class ZIPload

# Synopsis
GLM:
~~~
  object ZIPload {
    heat_fraction "<decimal>";
    base_power "<decimal> kW";
    power_pf "<decimal>";
    current_pf "<decimal>";
    impedance_pf "<decimal>";
    is_240 "<string>";
    breaker_val "<decimal> A";
    actual_power "<string> kVA";
    multiplier "<decimal>";
    heatgain_only "<string>";
    demand_response_mode "<string>";
    number_of_devices "<integer>";
    thermostatic_control_range "<integer>";
    number_of_devices_off "<decimal>";
    number_of_devices_on "<decimal>";
    rate_of_cooling "<decimal> K/h";
    rate_of_heating "<decimal> K/h";
    temperature "<integer>";
    phi "<decimal>";
    demand_rate "<decimal> 1/h";
    nominal_power "<decimal>";
    duty_cycle "<decimal> pu";
    recovery_duty_cycle "<decimal> pu";
    period "<decimal> h";
    phase "<decimal> pu";
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

### `heat_fraction`
~~~
  double heat_fraction;
~~~

Fraction of ZIPload that is transferred as heat

### `base_power`
~~~
  double base_power[kW];
~~~

Base real power of the overall load

### `power_pf`
~~~
  double power_pf;
~~~

Power factor for constant power portion

### `current_pf`
~~~
  double current_pf;
~~~

Power factor for constant current portion

### `impedance_pf`
~~~
  double impedance_pf;
~~~

Power factor for constant impedance portion

### `is_240`
~~~
  bool is_240;
~~~

Load is 220/240 V (across both phases)

### `breaker_val`
~~~
  double breaker_val[A];
~~~

Amperage of connected breaker

### `actual_power`
~~~
  complex actual_power[kVA];
~~~

Variable to pull actual load as function of voltage

### `multiplier`
~~~
  double multiplier;
~~~

This variable is used to modify the base power as a function of multiplier x base_power

### `heatgain_only`
~~~
  bool heatgain_only;
~~~

Allows the zipload to generate heat only (no kW), not activated by default

### `demand_response_mode`
~~~
  bool demand_response_mode;
~~~

Activates equilibrium dynamic representation of demand response

### `number_of_devices`
~~~
  int16 number_of_devices;
~~~

Number of devices to model - base power is the total load of all devices

### `thermostatic_control_range`
~~~
  int16 thermostatic_control_range;
~~~

Range of the thermostat's control operation

### `number_of_devices_off`
~~~
  double number_of_devices_off;
~~~

Total number of devices that are off

### `number_of_devices_on`
~~~
  double number_of_devices_on;
~~~

Total number of devices that are on

### `rate_of_cooling`
~~~
  double rate_of_cooling[K/h];
~~~

Rate at which devices cool down

### `rate_of_heating`
~~~
  double rate_of_heating[K/h];
~~~

Rate at which devices heat up

### `temperature`
~~~
  int16 temperature;
~~~

Temperature of the device's controlled media (eg air temp or water temp)

### `phi`
~~~
  double phi;
~~~

Duty cycle of the device

### `demand_rate`
~~~
  double demand_rate[1/h];
~~~

Consumer demand rate that prematurely turns on a device or population

### `nominal_power`
~~~
  double nominal_power;
~~~

The rated amount of power demanded by devices that are on

### `duty_cycle`
~~~
  double duty_cycle[pu];
~~~

Fraction of time in the on state

### `recovery_duty_cycle`
~~~
  double recovery_duty_cycle[pu];
~~~

Fraction of time in the on state, while in recovery interval

### `period`
~~~
  double period[h];
~~~

Time interval to apply duty cycle

### `phase`
~~~
  double phase[pu];
~~~

Initial phase of load; duty will be assumed to occur at beginning of period

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
  object ZIPload {
    heat_fraction "0.0";
    base_power "0.0";
    power_pf "0.0";
    current_pf "0.0";
    impedance_pf "0.0";
    is_240 "FALSE";
    breaker_val "0.0";
    actual_power "0+0i";
    multiplier "0.0";
    heatgain_only "FALSE";
    demand_response_mode "FALSE";
    number_of_devices "0";
    thermostatic_control_range "0";
    number_of_devices_off "0.0";
    number_of_devices_on "0.0";
    rate_of_cooling "0.0";
    rate_of_heating "0.0";
    temperature "0";
    phi "0.0";
    demand_rate "0.0";
    nominal_power "0.0";
    duty_cycle "0.0";
    recovery_duty_cycle "0.0";
    period "0.0";
    phase "0.0";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

