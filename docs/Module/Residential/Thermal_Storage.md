[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  object thermal_storage {
    total_capacity "<decimal> Btu";
    stored_capacity "<decimal> Btu";
    recharge_power "<decimal> kW";
    discharge_power "<decimal> kW";
    recharge_pf "<decimal>";
    discharge_pf "<decimal>";
    discharge_schedule_type "{EXTERNAL,INTERNAL}";
    recharge_schedule_type "{EXTERNAL,INTERNAL}";
    recharge_time "<decimal>";
    discharge_time "<decimal>";
    discharge_rate "<decimal> Btu/h";
    SOC "<decimal> %";
    k "<decimal> W/m/K";
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

### `total_capacity`
~~~
  double total_capacity[Btu];
~~~

Total storage capacity of unit

### `stored_capacity`
~~~
  double stored_capacity[Btu];
~~~

Amount of capacity that is stored

### `recharge_power`
~~~
  double recharge_power[kW];
~~~

Installed compressor power usage

### `discharge_power`
~~~
  double discharge_power[kW];
~~~

Installed pump power usage

### `recharge_pf`
~~~
  double recharge_pf;
~~~

Installed compressor power factor

### `discharge_pf`
~~~
  double discharge_pf;
~~~

Installed pump power factor

### `discharge_schedule_type`
~~~
  enumeration {EXTERNAL, INTERNAL} discharge_schedule_type;
~~~

Scheduling method for discharging

### `recharge_schedule_type`
~~~
  enumeration {EXTERNAL, INTERNAL} recharge_schedule_type;
~~~

Scheduling method for charging

### `recharge_time`
~~~
  double recharge_time;
~~~

Flag indicating if recharging is available at the current time (1 or 0)

### `discharge_time`
~~~
  double discharge_time;
~~~

Flag indicating if discharging is available at the current time (1 or 0)

### `discharge_rate`
~~~
  double discharge_rate[Btu/h];
~~~

Rating of discharge or cooling

### `SOC`
~~~
  double SOC[%];
~~~

State of charge as percentage of total capacity

### `k`
~~~
  double k[W/m/K];
~~~

Coefficient of thermal conductivity (W/m/K)

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
  object thermal_storage {
    total_capacity "0.0";
    stored_capacity "0.0";
    recharge_power "0.0";
    discharge_power "0.0";
    recharge_pf "0.0";
    discharge_pf "0.0";
    discharge_schedule_type "0";
    recharge_schedule_type "0";
    recharge_time "0.0";
    discharge_time "0.0";
    discharge_rate "0.0";
    SOC "0.0";
    k "0.0";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

