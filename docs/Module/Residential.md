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

