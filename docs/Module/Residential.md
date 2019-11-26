[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  module residential {
    residential::message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    residential::default_line_voltage "<string> V";
    residential::default_line_current "<string> A";
    residential::default_outdoor_temperature "<decimal> degF";
    residential::default_humidity "<decimal> %";
    residential::default_solar "<decimal> Btu/sf";
    residential::default_etp_iterations "<integer>";
    residential::ANSI_voltage_check "<string>";
    residential::enable_subsecond_models "<string>";
    residential::deltamode_timestep "<decimal> ns";
    residential::all_house_delta "<string>";
    residential::implicit_enduses "[NONE|DRYER|CLOTHESWASHER|WATERHEATER|EVCHARGER|RANGE|REFRIGERATOR|FREEZER|MICROWAVE|DISHWASHER|OCCUPANCY|PLUGS|LIGHTS]";
    residential::implicit_enduse_source "{EIA2015,RBSA2014_DISCRETE,RBSA2014,ELCAP2010,ELCAP1990}";
    residential::house_low_temperature_warning "<decimal> degF";
    residential::house_high_temperature_warning "<decimal> degF";
    residential::thermostat_control_warning "<decimal>";
    residential::system_dwell_time "<decimal> s";
    residential::aux_cutin_temperature "<decimal> degF";
    residential::paneldump_filename "<string>";
    residential::paneldump_interval "<integer>";
    residential::paneldump_resolution "<decimal>";
  }
~~~

# Description

TODO

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

