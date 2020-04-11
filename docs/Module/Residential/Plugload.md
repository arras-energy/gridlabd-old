[[/Module/Residential/Plugload]] -- Class plugload

# Synopsis

GLM:

~~~
  object plugload {
    circuit_split "<decimal>";
    demand "<decimal> unit";
    installed_power "<decimal> kW";
    actual_power "<string> kVA";
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

### `circuit_split`

~~~
  double circuit_split;
~~~

TODO

### `demand`

~~~
  double demand[unit];
~~~

TODO

### `installed_power`

~~~
  double installed_power[kW];
~~~

Installed plugs capacity

### `actual_power`

~~~
  complex actual_power[kVA];
~~~

Actual power demand

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
  object plugload {
    circuit_split "0.0";
    demand "0.0";
    installed_power "0.0";
    actual_power "0+0i";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]

