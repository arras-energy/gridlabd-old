[[/Module/Powerflow/Restoration]] -- Class restoration

# Synopsis

GLM:

~~~
  object restoration {
    reconfig_attempts "<integer>";
    reconfig_iteration_limit "<integer>";
    source_vertex "<string>";
    faulted_section "<string>";
    feeder_power_limit "<string>";
    feeder_power_links "<string>";
    feeder_vertex_list "<string>";
    microgrid_power_limit "<string>";
    microgrid_power_links "<string>";
    microgrid_vertex_list "<string>";
    lower_voltage_limit "<decimal> pu";
    upper_voltage_limit "<decimal> pu";
    output_filename "<string>";
    generate_all_scenarios "<string>";
  }
~~~

# Description

TODO

## Properties

### `reconfig_attempts`
~~~
  int32 reconfig_attempts;
~~~

Number of reconfigurations/timestep to try before giving up

### `reconfig_iteration_limit`
~~~
  int32 reconfig_iteration_limit;
~~~

Number of iterations to let PF go before flagging this as a bad reconfiguration

### `source_vertex`
~~~
  object source_vertex;
~~~

Source vertex object for reconfiguration

### `faulted_section`
~~~
  object faulted_section;
~~~

Faulted section for reconfiguration

### `feeder_power_limit`
~~~
  char1024 feeder_power_limit;
~~~

Comma-separated power limit (VA) for feeders during reconfiguration

### `feeder_power_links`
~~~
  char1024 feeder_power_links;
~~~

Comma-separated list of link-based objects for monitoring power through

### `feeder_vertex_list`
~~~
  char1024 feeder_vertex_list;
~~~

Comma-separated object list that defines the feeder vertices

### `microgrid_power_limit`
~~~
  char1024 microgrid_power_limit;
~~~

Comma-separated power limit (complex VA) for microgrids during reconfiguration

### `microgrid_power_links`
~~~
  char1024 microgrid_power_links;
~~~

Comma-separated list of link-based objects for monitoring power through

### `microgrid_vertex_list`
~~~
  char1024 microgrid_vertex_list;
~~~

Comma-separated object list that defines the microgrid vertices

### `lower_voltage_limit`
~~~
  double lower_voltage_limit[pu];
~~~

Lower voltage limit for the reconfiguration validity checks - per unit

### `upper_voltage_limit`
~~~
  double upper_voltage_limit[pu];
~~~

Upper voltage limit for the reconfiguration validity checks - per unit

### `output_filename`
~~~
  char1024 output_filename;
~~~

Output text file name to describe final or attempted switching operations

### `generate_all_scenarios`
~~~
  bool generate_all_scenarios;
~~~

Flag to determine if restoration reconfiguration and continues, or explores the full space

# Example

~~~
  object restoration {
    reconfig_attempts "0";
    reconfig_iteration_limit "0";
    feeder_power_limit "";
    feeder_power_links "";
    feeder_vertex_list "";
    microgrid_power_limit "";
    microgrid_power_links "";
    microgrid_vertex_list "";
    lower_voltage_limit "0.0";
    upper_voltage_limit "0.0";
    output_filename "";
    generate_all_scenarios "FALSE";
  }
~~~

# See also
* [[/Module/Powerflow]]

