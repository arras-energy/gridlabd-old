[[/Module/Residential/Evcharger]] -- Class evcharger

# Synopsis
GLM:
~~~
  object evcharger {
    charger_type "{HIGH,MEDIUM,LOW}";
    vehicle_type "{HYBRID,ELECTRIC}";
    state "{WORK,HOME,UNKNOWN}";
    p_go_home "<decimal> unit/h";
    p_go_work "<decimal> unit/h";
    work_dist "<decimal> mile";
    capacity "<decimal> kWh";
    charge "<decimal> unit";
    charge_at_work "<string>";
    charge_throttle "<decimal> unit";
    charger_efficiency "<decimal> unit";
    power_train_efficiency "<decimal> mile/kWh";
    mileage_classification "<decimal> mile";
    demand_profile "<string>";
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

### `charger_type`
~~~
  enumeration {HIGH, MEDIUM, LOW} charger_type;
~~~

TODO

### `vehicle_type`
~~~
  enumeration {HYBRID, ELECTRIC} vehicle_type;
~~~

TODO

### `state`
~~~
  enumeration {WORK, HOME, UNKNOWN} state;
~~~

TODO

### `p_go_home`
~~~
  double p_go_home[unit/h];
~~~

TODO

### `p_go_work`
~~~
  double p_go_work[unit/h];
~~~

TODO

### `work_dist`
~~~
  double work_dist[mile];
~~~

TODO

### `capacity`
~~~
  double capacity[kWh];
~~~

TODO

### `charge`
~~~
  double charge[unit];
~~~

TODO

### `charge_at_work`
~~~
  bool charge_at_work;
~~~

TODO

### `charge_throttle`
~~~
  double charge_throttle[unit];
~~~

TODO

### `charger_efficiency`
~~~
  double charger_efficiency[unit];
~~~

Efficiency of the charger in terms of energy in to battery stored

### `power_train_efficiency`
~~~
  double power_train_efficiency[mile/kWh];
~~~

Miles per kWh of battery charge

### `mileage_classification`
~~~
  double mileage_classification[mile];
~~~

Miles expected range on battery only

### `demand_profile`
~~~
  char1024 demand_profile;
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
  object evcharger {
    charger_type "0";
    vehicle_type "0";
    state "0";
    p_go_home "0.0";
    p_go_work "0.0";
    work_dist "0.0";
    capacity "0.0";
    charge "0.0";
    charge_at_work "FALSE";
    charge_throttle "0.0";
    charger_efficiency "0.0";
    power_train_efficiency "0.0";
    mileage_classification "0.0";
    demand_profile "";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

