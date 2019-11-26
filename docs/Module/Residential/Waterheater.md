[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  object waterheater {
    tank_volume "<decimal> gal";
    tank_UA "<decimal> Btu*h/degF";
    tank_diameter "<decimal> ft";
    tank_height "<decimal> ft";
    water_demand "<decimal> gpm";
    heating_element_capacity "<decimal> kW";
    inlet_water_temperature "<decimal> degF";
    waterheater_model "{NONE,FORTRAN,TWONODE,ONEZNODE}";
    heat_mode "{HEAT_PUMP,GASHEAT,ELECTRIC}";
    location "{GARAGE,INSIDE}";
    tank_setpoint "<decimal> degF";
    thermostat_deadband "<decimal> degF";
    temperature "<decimal> degF";
    height "<decimal> ft";
    demand "<string> kVA";
    actual_load "<decimal> kW";
    previous_load "<decimal> kW";
    actual_power "<string> kVA";
    is_waterheater_on "<decimal>";
    gas_fan_power "<decimal> kW";
    gas_standby_power "<decimal> kW";
    heat_pump_coefficient_of_performance "<decimal> Btu/kWh";
    Tcontrol "<decimal> degF";
    current_tank_status "{EMPTY,PARTIAL,FULL}";
    dr_signal "<decimal>";
    COP "<decimal>";
    operating_mode "<decimal>";
    fortran_sim_time "<decimal> s";
    waterheater_power "<decimal> kW";
    load_state "{STABLE,RECOVERING,DEPLETING}";
    actual_voltage "<decimal>";
    nominal_voltage "<decimal>";
    re_override "{OV_OFF,OV_NORMAL,OV_ON}";
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

### `tank_volume`
~~~
  double tank_volume[gal];
~~~

The volume of water in the tank when it is full

### `tank_UA`
~~~
  double tank_UA[Btu*h/degF];
~~~

The UA of the tank (surface area divided by R-value)

### `tank_diameter`
~~~
  double tank_diameter[ft];
~~~

The diameter of the water heater tank

### `tank_height`
~~~
  double tank_height[ft];
~~~

The height of the water heater tank

### `water_demand`
~~~
  double water_demand[gpm];
~~~

The hot water draw from the water heater

### `heating_element_capacity`
~~~
  double heating_element_capacity[kW];
~~~

The power of the heating element

### `inlet_water_temperature`
~~~
  double inlet_water_temperature[degF];
~~~

The inlet temperature of the water tank

### `waterheater_model`
~~~
  enumeration {NONE, FORTRAN, TWONODE, ONEZNODE} waterheater_model;
~~~

The water heater model to use

### `heat_mode`
~~~
  enumeration {HEAT_PUMP, GASHEAT, ELECTRIC} heat_mode;
~~~

The energy source for heating the water heater

### `location`
~~~
  enumeration {GARAGE, INSIDE} location;
~~~

Whether the water heater is inside or outside

### `tank_setpoint`
~~~
  double tank_setpoint[degF];
~~~

The temperature around which the water heater will heat its contents

### `thermostat_deadband`
~~~
  double thermostat_deadband[degF];
~~~

The degree to heat the water tank, when needed

### `temperature`
~~~
  double temperature[degF];
~~~

The outlet temperature of the water tank

### `height`
~~~
  double height[ft];
~~~

The height of the hot water column within the water tank

### `demand`
~~~
  complex demand[kVA];
~~~

The water heater power consumption

### `actual_load`
~~~
  double actual_load[kW];
~~~

The actual load based on the current voltage across the coils

### `previous_load`
~~~
  double previous_load[kW];
~~~

The previous load based on voltage across the coils at the last sync operation

### `actual_power`
~~~
  complex actual_power[kVA];
~~~

The actual power based on the current voltage across the coils

### `is_waterheater_on`
~~~
  double is_waterheater_on;
~~~

Simple logic output to determine state of waterheater (1-on, 0-off)

### `gas_fan_power`
~~~
  double gas_fan_power[kW];
~~~

Load of a running gas waterheater

### `gas_standby_power`
~~~
  double gas_standby_power[kW];
~~~

Load of a gas waterheater in standby

### `heat_pump_coefficient_of_performance`
~~~
  double heat_pump_coefficient_of_performance[Btu/kWh];
~~~

Current COP of the water heater pump - currently calculated internally and not an input

### `Tcontrol`
~~~
  double Tcontrol[degF];
~~~

In heat pump operation, defines the blended temperature used for turning on and off HP - currently calculated internally and not an input

### `current_tank_status`
~~~
  enumeration {EMPTY, PARTIAL, FULL} current_tank_status;
~~~

TODO

### `dr_signal`
~~~
  double dr_signal;
~~~

The on/off signal to send to the fortran waterheater model

### `COP`
~~~
  double COP;
~~~

The cop of the fortran heat pump water heater model.

### `operating_mode`
~~~
  double operating_mode;
~~~

The operating mode the fortran water heater should be using.

### `fortran_sim_time`
~~~
  double fortran_sim_time[s];
~~~

The amount of time the fortran model should simulate.

### `waterheater_power`
~~~
  double waterheater_power[kW];
~~~

The current power draw from the fortran water heater.

### `load_state`
~~~
  enumeration {STABLE, RECOVERING, DEPLETING} load_state;
~~~

TODO

### `actual_voltage`
~~~
  double actual_voltage;
~~~

TODO

### `nominal_voltage`
~~~
  double nominal_voltage;
~~~

TODO

### `re_override`
~~~
  enumeration {OV_OFF, OV_NORMAL, OV_ON} re_override;
~~~

The override setting for the water heater

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
  object waterheater {
    tank_volume "0.0";
    tank_UA "0.0";
    tank_diameter "0.0";
    tank_height "0.0";
    water_demand "0.0";
    heating_element_capacity "0.0";
    inlet_water_temperature "0.0";
    waterheater_model "0";
    heat_mode "0";
    location "0";
    tank_setpoint "0.0";
    thermostat_deadband "0.0";
    temperature "0.0";
    height "0.0";
    demand "0+0i";
    actual_load "0.0";
    previous_load "0.0";
    actual_power "0+0i";
    is_waterheater_on "0.0";
    gas_fan_power "0.0";
    gas_standby_power "0.0";
    heat_pump_coefficient_of_performance "0.0";
    Tcontrol "0.0";
    current_tank_status "0";
    dr_signal "0.0";
    COP "0.0";
    operating_mode "0.0";
    fortran_sim_time "0.0";
    waterheater_power "0.0";
    load_state "0";
    actual_voltage "0.0";
    nominal_voltage "0.0";
    re_override "0";
    override "0";
    power_state "0";
  }
~~~

# See also
* [[/Module/Residential]]

