[[/Module/Residential/Evcharger_det]] -- Class evcharger_det

# Synopsis

GLM:

~~~
  object evcharger_det {
    charge_rate "<decimal> W";
    variation_mean "<decimal> s";
    variation_std_dev "<decimal> s";
    variation_trip_mean "<decimal> mile";
    variation_trip_std_dev "<decimal> mile";
    mileage_classification "<decimal> mile";
    work_charging_available "<string>";
    data_file "<string>";
    vehicle_index "<integer>";
    vehicle_location "{DRIVING_WORK,DRIVING_HOME,WORK,HOME,UNKNOWN}";
    travel_distance "<decimal> mile";
    arrival_at_work "<decimal>";
    duration_at_work "<decimal> s";
    arrival_at_home "<decimal>";
    duration_at_home "<decimal> s";
    battery_capacity "<decimal> kWh";
    battery_SOC "<decimal> %";
    battery_size "<decimal> kWh";
    mileage_efficiency "<decimal> mile/kWh";
    maximum_charge_rate "<decimal> W";
    charging_efficiency "<decimal> unit";
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

### `charge_rate`

~~~
  double charge_rate[W];
~~~

Current demanded charge rate of the vehicle

### `variation_mean`

~~~
  double variation_mean[s];
~~~

Mean of normal variation of schedule variation

### `variation_std_dev`

~~~
  double variation_std_dev[s];
~~~

Standard deviation of normal variation of schedule times

### `variation_trip_mean`

~~~
  double variation_trip_mean[mile];
~~~

Mean of normal variation of trip distance variation

### `variation_trip_std_dev`

~~~
  double variation_trip_std_dev[mile];
~~~

Standard deviation of normal variation of trip distance

### `mileage_classification`

~~~
  double mileage_classification[mile];
~~~

Mileage classification of electric vehicle

### `work_charging_available`

~~~
  bool work_charging_available;
~~~

Charging available when at work

### `data_file`

~~~
  char1024 data_file;
~~~

Path to .CSV file with vehicle travel information

### `vehicle_index`

~~~
  int32 vehicle_index;
~~~

Index of vehicles in file this particular vehicle's data

### `vehicle_location`

~~~
  enumeration {DRIVING_WORK, DRIVING_HOME, WORK, HOME, UNKNOWN} vehicle_location;
~~~

TODO

### `travel_distance`

~~~
  double travel_distance[mile];
~~~

Distance vehicle travels from home to home

### `arrival_at_work`

~~~
  double arrival_at_work;
~~~

Time vehicle arrives at work - HHMM

### `duration_at_work`

~~~
  double duration_at_work[s];
~~~

Duration the vehicle remains at work

### `arrival_at_home`

~~~
  double arrival_at_home;
~~~

Time vehicle arrives at home - HHMM

### `duration_at_home`

~~~
  double duration_at_home[s];
~~~

Duration the vehicle remains at home

### `battery_capacity`

~~~
  double battery_capacity[kWh];
~~~

Current capacity of the battery in kWh

### `battery_SOC`

~~~
  double battery_SOC[%];
~~~

State of charge of battery

### `battery_size`

~~~
  double battery_size[kWh];
~~~

Full capacity of battery

### `mileage_efficiency`

~~~
  double mileage_efficiency[mile/kWh];
~~~

Efficiency of drive train in mile/kWh

### `maximum_charge_rate`

~~~
  double maximum_charge_rate[W];
~~~

Maximum output rate of charger in kW

### `charging_efficiency`

~~~
  double charging_efficiency[unit];
~~~

Efficiency of charger (ratio) when charging

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
  object evcharger_det {
    charge_rate "0.0";
    variation_mean "0.0";
    variation_std_dev "0.0";
    variation_trip_mean "0.0";
    variation_trip_std_dev "0.0";
    mileage_classification "0.0";
    work_charging_available "FALSE";
    data_file "";
    vehicle_index "0";
    vehicle_location "0";
    travel_distance "0.0";
    arrival_at_work "0.0";
    duration_at_work "0.0";
    arrival_at_home "0.0";
    duration_at_home "0.0";
    battery_capacity "0.0";
    battery_SOC "0.0";
    battery_size "0.0";
    mileage_efficiency "0.0";
    maximum_charge_rate "0.0";
    charging_efficiency "0.0";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]

