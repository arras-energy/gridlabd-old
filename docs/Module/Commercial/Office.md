[[/Module/Commercial/Office]] -- Class office

# Synopsis

GLM:

~~~
  object office {
    floor_area "<decimal> sf";
    floor_height "<decimal> ft";
    exterior_ua "<decimal> Btu/degF/h";
    interior_ua "<decimal> Btu/degF/h";
    interior_mass "<decimal> Btu/degF";
    glazing "<decimal> sf";
    glazing.north "<decimal> sf";
    glazing.northeast "<decimal> sf";
    glazing.east "<decimal> sf";
    glazing.southeast "<decimal> sf";
    glazing.south "<decimal> sf";
    glazing.southwest "<decimal> sf";
    glazing.west "<decimal> sf";
    glazing.northwest "<decimal> sf";
    glazing.horizontal "<decimal> sf";
    glazing.coefficient "<decimal> pu";
    occupancy "<decimal>";
    occupants "<decimal>";
    schedule "<string>";
    air_temperature "<decimal> degF";
    mass_temperature "<decimal> degF";
    temperature_change "<decimal> degF/h";
    outdoor_temperature "<decimal> degF";
    Qh "<decimal> Btu/h";
    Qs "<decimal> Btu/h";
    Qi "<decimal> Btu/h";
    Qz "<decimal> Btu/h";
    hvac_mode "{OFF,VENT,ECON,COOL,AUX,HEAT}";
    hvac.cooling.balance_temperature "<decimal> degF";
    hvac.cooling.capacity "<decimal> Btu/h";
    hvac.cooling.capacity_perF "<decimal> Btu/degF/h";
    hvac.cooling.design_temperature "<decimal> degF";
    hvac.cooling.efficiency "<decimal> pu";
    hvac.cooling.cop "<decimal> pu";
    hvac.heating.balance_temperature "<decimal> degF";
    hvac.heating.capacity "<decimal> Btu/h";
    hvac.heating.capacity_perF "<decimal> Btu/degF/h";
    hvac.heating.design_temperature "<decimal> degF";
    hvac.heating.efficiency "<decimal> pu";
    hvac.heating.cop "<decimal> pu";
    lights.capacity "<decimal> kW";
    lights.fraction "<decimal> pu";
    plugs.capacity "<decimal> kW";
    plugs.fraction "<decimal> pu";
    demand "<string> kW";
    total_load "<string> kW";
    energy "<string> kWh";
    power_factor "<decimal>";
    power "<string> kW";
    current "<string> A";
    admittance "<string> 1/Ohm";
    hvac.demand "<string> kW";
    hvac.load "<string> kW";
    hvac.energy "<string> kWh";
    hvac.power_factor "<decimal>";
    lights.demand "<string> kW";
    lights.load "<string> kW";
    lights.energy "<string> kWh";
    lights.power_factor "<decimal>";
    lights.heatgain_fraction "<decimal>";
    lights.heatgain "<decimal> kW";
    plugs.demand "<string> kW";
    plugs.load "<string> kW";
    plugs.energy "<string> kWh";
    plugs.power_factor "<decimal>";
    plugs.heatgain_fraction "<decimal>";
    plugs.heatgain "<decimal> kW";
    cooling_setpoint "<decimal> degF";
    heating_setpoint "<decimal> degF";
    thermostat_deadband "<decimal> degF";
    control.ventilation_fraction "<decimal>";
    control.lighting_fraction "<decimal>";
    ACH "<decimal>";
  }
~~~

# Description

TODO

## Properties

### `floor_area`

~~~
  double floor_area[sf];
~~~

TODO

### `floor_height`

~~~
  double floor_height[ft];
~~~

TODO

### `exterior_ua`

~~~
  double exterior_ua[Btu/degF/h];
~~~

TODO

### `interior_ua`

~~~
  double interior_ua[Btu/degF/h];
~~~

TODO

### `interior_mass`

~~~
  double interior_mass[Btu/degF];
~~~

TODO

### `glazing`

~~~
  double glazing[sf];
~~~

TODO

### `glazing.north`

~~~
  double glazing.north[sf];
~~~

TODO

### `glazing.northeast`

~~~
  double glazing.northeast[sf];
~~~

TODO

### `glazing.east`

~~~
  double glazing.east[sf];
~~~

TODO

### `glazing.southeast`

~~~
  double glazing.southeast[sf];
~~~

TODO

### `glazing.south`

~~~
  double glazing.south[sf];
~~~

TODO

### `glazing.southwest`

~~~
  double glazing.southwest[sf];
~~~

TODO

### `glazing.west`

~~~
  double glazing.west[sf];
~~~

TODO

### `glazing.northwest`

~~~
  double glazing.northwest[sf];
~~~

TODO

### `glazing.horizontal`

~~~
  double glazing.horizontal[sf];
~~~

TODO

### `glazing.coefficient`

~~~
  double glazing.coefficient[pu];
~~~

TODO

### `occupancy`

~~~
  double occupancy;
~~~

TODO

### `occupants`

~~~
  double occupants;
~~~

TODO

### `schedule`

~~~
  char256 schedule;
~~~

TODO

### `air_temperature`

~~~
  double air_temperature[degF];
~~~

TODO

### `mass_temperature`

~~~
  double mass_temperature[degF];
~~~

TODO

### `temperature_change`

~~~
  double temperature_change[degF/h];
~~~

TODO

### `outdoor_temperature`

~~~
  double outdoor_temperature[degF];
~~~

TODO

### `Qh`

~~~
  double Qh[Btu/h];
~~~

TODO

### `Qs`

~~~
  double Qs[Btu/h];
~~~

TODO

### `Qi`

~~~
  double Qi[Btu/h];
~~~

TODO

### `Qz`

~~~
  double Qz[Btu/h];
~~~

TODO

### `hvac_mode`

~~~
  enumeration {OFF, VENT, ECON, COOL, AUX, HEAT} hvac_mode;
~~~

TODO

### `hvac.cooling.balance_temperature`

~~~
  double hvac.cooling.balance_temperature[degF];
~~~

TODO

### `hvac.cooling.capacity`

~~~
  double hvac.cooling.capacity[Btu/h];
~~~

TODO

### `hvac.cooling.capacity_perF`

~~~
  double hvac.cooling.capacity_perF[Btu/degF/h];
~~~

TODO

### `hvac.cooling.design_temperature`

~~~
  double hvac.cooling.design_temperature[degF];
~~~

TODO

### `hvac.cooling.efficiency`

~~~
  double hvac.cooling.efficiency[pu];
~~~

TODO

### `hvac.cooling.cop`

~~~
  double hvac.cooling.cop[pu];
~~~

TODO

### `hvac.heating.balance_temperature`

~~~
  double hvac.heating.balance_temperature[degF];
~~~

TODO

### `hvac.heating.capacity`

~~~
  double hvac.heating.capacity[Btu/h];
~~~

TODO

### `hvac.heating.capacity_perF`

~~~
  double hvac.heating.capacity_perF[Btu/degF/h];
~~~

TODO

### `hvac.heating.design_temperature`

~~~
  double hvac.heating.design_temperature[degF];
~~~

TODO

### `hvac.heating.efficiency`

~~~
  double hvac.heating.efficiency[pu];
~~~

TODO

### `hvac.heating.cop`

~~~
  double hvac.heating.cop[pu];
~~~

TODO

### `lights.capacity`

~~~
  double lights.capacity[kW];
~~~

TODO

### `lights.fraction`

~~~
  double lights.fraction[pu];
~~~

TODO

### `plugs.capacity`

~~~
  double plugs.capacity[kW];
~~~

TODO

### `plugs.fraction`

~~~
  double plugs.fraction[pu];
~~~

TODO

### `demand`

~~~
  complex demand[kW];
~~~

TODO

### `total_load`

~~~
  complex total_load[kW];
~~~

TODO

### `energy`

~~~
  complex energy[kWh];
~~~

TODO

### `power_factor`

~~~
  double power_factor;
~~~

TODO

### `power`

~~~
  complex power[kW];
~~~

TODO

### `current`

~~~
  complex current[A];
~~~

TODO

### `admittance`

~~~
  complex admittance[1/Ohm];
~~~

TODO

### `hvac.demand`

~~~
  complex hvac.demand[kW];
~~~

TODO

### `hvac.load`

~~~
  complex hvac.load[kW];
~~~

TODO

### `hvac.energy`

~~~
  complex hvac.energy[kWh];
~~~

TODO

### `hvac.power_factor`

~~~
  double hvac.power_factor;
~~~

TODO

### `lights.demand`

~~~
  complex lights.demand[kW];
~~~

TODO

### `lights.load`

~~~
  complex lights.load[kW];
~~~

TODO

### `lights.energy`

~~~
  complex lights.energy[kWh];
~~~

TODO

### `lights.power_factor`

~~~
  double lights.power_factor;
~~~

TODO

### `lights.heatgain_fraction`

~~~
  double lights.heatgain_fraction;
~~~

TODO

### `lights.heatgain`

~~~
  double lights.heatgain[kW];
~~~

TODO

### `plugs.demand`

~~~
  complex plugs.demand[kW];
~~~

TODO

### `plugs.load`

~~~
  complex plugs.load[kW];
~~~

TODO

### `plugs.energy`

~~~
  complex plugs.energy[kWh];
~~~

TODO

### `plugs.power_factor`

~~~
  double plugs.power_factor;
~~~

TODO

### `plugs.heatgain_fraction`

~~~
  double plugs.heatgain_fraction;
~~~

TODO

### `plugs.heatgain`

~~~
  double plugs.heatgain[kW];
~~~

TODO

### `cooling_setpoint`

~~~
  double cooling_setpoint[degF];
~~~

TODO

### `heating_setpoint`

~~~
  double heating_setpoint[degF];
~~~

TODO

### `thermostat_deadband`

~~~
  double thermostat_deadband[degF];
~~~

TODO

### `control.ventilation_fraction`

~~~
  double control.ventilation_fraction;
~~~

TODO

### `control.lighting_fraction`

~~~
  double control.lighting_fraction;
~~~

TODO

### `ACH`

~~~
  double ACH;
~~~

TODO

# Example

~~~
  object office {
    floor_area "0.0";
    floor_height "0.0";
    exterior_ua "0.0";
    interior_ua "0.0";
    interior_mass "0.0";
    glazing "0.0";
    glazing.north "0.0";
    glazing.northeast "0.0";
    glazing.east "0.0";
    glazing.southeast "0.0";
    glazing.south "0.0";
    glazing.southwest "0.0";
    glazing.west "0.0";
    glazing.northwest "0.0";
    glazing.horizontal "0.0";
    glazing.coefficient "0.0";
    occupancy "0.0";
    occupants "0.0";
    schedule "";
    air_temperature "0.0";
    mass_temperature "0.0";
    temperature_change "0.0";
    outdoor_temperature "0.0";
    Qh "0.0";
    Qs "0.0";
    Qi "0.0";
    Qz "0.0";
    hvac_mode "0";
    hvac.cooling.balance_temperature "0.0";
    hvac.cooling.capacity "0.0";
    hvac.cooling.capacity_perF "0.0";
    hvac.cooling.design_temperature "0.0";
    hvac.cooling.efficiency "0.0";
    hvac.cooling.cop "0.0";
    hvac.heating.balance_temperature "0.0";
    hvac.heating.capacity "0.0";
    hvac.heating.capacity_perF "0.0";
    hvac.heating.design_temperature "0.0";
    hvac.heating.efficiency "0.0";
    hvac.heating.cop "0.0";
    lights.capacity "0.0";
    lights.fraction "0.0";
    plugs.capacity "0.0";
    plugs.fraction "0.0";
    demand "0+0i";
    total_load "0+0i";
    energy "0+0i";
    power_factor "0.0";
    power "0+0i";
    current "0+0i";
    admittance "0+0i";
    hvac.demand "0+0i";
    hvac.load "0+0i";
    hvac.energy "0+0i";
    hvac.power_factor "0.0";
    lights.demand "0+0i";
    lights.load "0+0i";
    lights.energy "0+0i";
    lights.power_factor "0.0";
    lights.heatgain_fraction "0.0";
    lights.heatgain "0.0";
    plugs.demand "0+0i";
    plugs.load "0+0i";
    plugs.energy "0+0i";
    plugs.power_factor "0.0";
    plugs.heatgain_fraction "0.0";
    plugs.heatgain "0.0";
    cooling_setpoint "0.0";
    heating_setpoint "0.0";
    thermostat_deadband "0.0";
    control.ventilation_fraction "0.0";
    control.lighting_fraction "0.0";
    ACH "0.0";
  }
~~~

# See also

* [[/Module/Commercial]]

