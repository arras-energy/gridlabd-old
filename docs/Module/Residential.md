[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  module residential {
    message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    default_line_voltage "<string> V";
    default_line_current "<string> A";
    default_outdoor_temperature "<decimal> degF";
    default_humidity "<decimal> %";
    default_solar "<decimal> Btu/sf";
    default_etp_iterations "<integer>";
    ANSI_voltage_check "<string>";
    enable_subsecond_models "<string>";
    deltamode_timestep "<decimal> ns";
    all_house_delta "<string>";
    implicit_enduses "[NONE|DRYER|CLOTHESWASHER|WATERHEATER|EVCHARGER|RANGE|REFRIGERATOR|FREEZER|MICROWAVE|DISHWASHER|OCCUPANCY|PLUGS|LIGHTS]";
    implicit_enduse_source "{EIA2015,RBSA2014_DISCRETE,RBSA2014,ELCAP2010,ELCAP1990}";
    house_low_temperature_warning "<decimal> degF";
    house_high_temperature_warning "<decimal> degF";
    thermostat_control_warning "<decimal>";
    system_dwell_time "<decimal> s";
    aux_cutin_temperature "<decimal> degF";
    paneldump_filename "<string>";
    paneldump_interval "<integer>";
    paneldump_resolution "<decimal>";
  }
~~~

# Description

TODO

## Globals

### `message_flags`
~~~
  message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
~~~

Module message control flags

### `default_line_voltage`
~~~
  default_line_voltage "<string> V";
~~~

Line voltage to use when no circuit is attached

### `default_line_current`
~~~
  default_line_current "<string> A";
~~~

Line current calculated when no circuit is attached

### `default_outdoor_temperature`
~~~
  default_outdoor_temperature "<decimal> degF";
~~~

Outdoor air temperature when no climate data is found

### `default_humidity`
~~~
  default_humidity "<decimal> %";
~~~

Humidity when no climate data is found

### `default_solar`
~~~
  default_solar "<decimal> Btu/sf";
~~~

Solar gains when no climate data is found

### `default_etp_iterations`
~~~
  default_etp_iterations "<integer>";
~~~

Number of iterations ETP solver will run

### `ANSI_voltage_check`
~~~
  ANSI_voltage_check "<string>";
~~~

Enable or disable messages about ANSI voltage limit violations in the house

### `enable_subsecond_models`
~~~
  enable_subsecond_models "<string>";
~~~

Enable deltamode capabilities within the residential module

### `deltamode_timestep`
~~~
  deltamode_timestep "<decimal> ns";
~~~

Desired minimum timestep for deltamode-related simulations

### `all_house_delta`
~~~
  all_house_delta "<string>";
~~~

Modeling convenient - enables all houses in deltamode

### `implicit_enduses`
~~~
  implicit_enduses "[NONE|DRYER|CLOTHESWASHER|WATERHEATER|EVCHARGER|RANGE|REFRIGERATOR|FREEZER|MICROWAVE|DISHWASHER|OCCUPANCY|PLUGS|LIGHTS]";
~~~

List of implicit enduses that are active in houses

### `implicit_enduse_source`
~~~
  implicit_enduse_source "{EIA2015,RBSA2014_DISCRETE,RBSA2014,ELCAP2010,ELCAP1990}";
~~~

TODO

### `house_low_temperature_warning`
~~~
  house_low_temperature_warning "<decimal> degF";
~~~

The low house indoor temperature at which a warning will be generated

### `house_high_temperature_warning`
~~~
  house_high_temperature_warning "<decimal> degF";
~~~

The high house indoor temperature at which a warning will be generated

### `thermostat_control_warning`
~~~
  thermostat_control_warning "<decimal>";
~~~

Boolean to indicate whether a warning is generated when indoor temperature is out of control limits

### `system_dwell_time`
~~~
  system_dwell_time "<decimal> s";
~~~

The heating/cooling system dwell time interval for changing system state

### `aux_cutin_temperature`
~~~
  aux_cutin_temperature "<decimal> degF";
~~~

The outdoor air temperature below which AUX heating is used

### `paneldump_filename`
~~~
  paneldump_filename "<string>";
~~~

The name of the file into which the paneldump is saved

### `paneldump_interval`
~~~
  paneldump_interval "<integer>";
~~~

The interval at which the paneldump is performed (0:disabled; <0:difference; >0:integral)

### `paneldump_resolution`
~~~
  paneldump_resolution "<decimal>";
~~~

The resolution at which the paneldump is performed (differences less than this are ignored)

# See also
* [[/Module/Residential/Residential_enduse]]
* [[/Module/Residential/Appliance]]
* [[/Module/Residential/House]]
* [[/Module/Residential/Waterheater]]
* [[/Module/Residential/Lights]]
* [[/Module/Residential/Refrigerator]]
* [[/Module/Residential/Clotheswasher]]
* [[/Module/Residential/Dishwasher]]
* [[/Module/Residential/Occupantload]]
* [[/Module/Residential/Plugload]]
* [[/Module/Residential/Microwave]]
* [[/Module/Residential/Range]]
* [[/Module/Residential/Freezer]]
* [[/Module/Residential/Dryer]]
* [[/Module/Residential/Evcharger]]
* [[/Module/Residential/ZIPload]]
* [[/Module/Residential/Thermal_storage]]
* [[/Module/Residential/Evcharger_det]]
* [[/Module/Residential/Rbsa]]

