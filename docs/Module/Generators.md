[[/Module/Generators]] -- Module generators

# Synopsis
GLM:
~~~
  module generators {
    message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    enable_subsecond_models "<string>";
    deltamode_timestep "<decimal> ns";
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

### `enable_subsecond_models`
~~~
  enable_subsecond_models "<string>";
~~~

Enable deltamode capabilities within the generators module

### `deltamode_timestep`
~~~
  deltamode_timestep "<decimal> ns";
~~~

Desired minimum timestep for deltamode-related simulations

# See also
* [[/Module/Generators/Diesel_dg]]
* [[/Module/Generators/Windturb_dg]]
* [[/Module/Generators/Power_electronics]]
* [[/Module/Generators/Energy_storage]]
* [[/Module/Generators/Inverter]]
* [[/Module/Generators/Dc_dc_converter]]
* [[/Module/Generators/Rectifier]]
* [[/Module/Generators/Microturbine]]
* [[/Module/Generators/Battery]]
* [[/Module/Generators/Solar]]
* [[/Module/Generators/Central_dg_control]]
* [[/Module/Generators/Controller_dg]]

