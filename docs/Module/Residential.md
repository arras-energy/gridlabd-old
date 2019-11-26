[[/Module/Residential]] -- Module residential

# Synopsis
GLM:
~~~
  module residential {
    message_flags "DEBUG|VERBOSE";
    default_line_voltage "+240+0d V";
    default_line_current "+0+0j A";
    default_outdoor_temperature "+74 degF";
    default_humidity "+75 %";
    default_solar "+0 Btu/sf";
    default_etp_iterations "100";
    ANSI_voltage_check "TRUE";
    enable_subsecond_models "FALSE";
    deltamode_timestep "+1e+07 ns";
    all_house_delta "FALSE";
    implicit_enduses "DRYER|CLOTHESWASHER|WATERHEATER|RANGE|REFRIGERATOR|FREEZER|MICROWAVE|DISHWASHER|PLUGS|LIGHTS";
    implicit_enduse_source "ELCAP1990";
    house_low_temperature_warning "+55 degF";
    house_high_temperature_warning "+95 degF";
    thermostat_control_warning "+4.94066e-324";
    system_dwell_time "+1 s";
    aux_cutin_temperature "+10 degF";
    paneldump_filename "paneldump.csv";
    paneldump_interval "0";
    paneldump_resolution "+0";
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

