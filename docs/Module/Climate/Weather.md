[[/Module/Climate/Weather]] -- Class weather

# Synopsis

GLM:

~~~
  object weather {
    temperature "<decimal> degF";
    humidity "<decimal> %";
    solar_dir "<decimal> W/sf";
    solar_direct "<decimal> W/sf";
    solar_diff "<decimal> W/sf";
    solar_diffuse "<decimal> W/sf";
    solar_global "<decimal> W/sf";
    global_horizontal_extra "<decimal> W/sf";
    wind_speed "<decimal> mph";
    wind_dir "<decimal> deg";
    opq_sky_cov "<decimal> pu";
    tot_sky_cov "<decimal> pu";
    rainfall "<decimal> in/h";
    snowdepth "<decimal> in";
    pressure "<decimal> mbar";
    month "<integer>";
    day "<integer>";
    hour "<integer>";
    minute "<integer>";
    second "<integer>";
  }
~~~

# Description

TODO

## Properties

### `temperature`
~~~
  double temperature[degF];
~~~

TODO

### `humidity`
~~~
  double humidity[%];
~~~

TODO

### `solar_dir`
~~~
  double solar_dir[W/sf];
~~~

TODO

### `solar_direct`
~~~
  double solar_direct[W/sf];
~~~

TODO

### `solar_diff`
~~~
  double solar_diff[W/sf];
~~~

TODO

### `solar_diffuse`
~~~
  double solar_diffuse[W/sf];
~~~

TODO

### `solar_global`
~~~
  double solar_global[W/sf];
~~~

TODO

### `global_horizontal_extra`
~~~
  double global_horizontal_extra[W/sf];
~~~

TODO

### `wind_speed`
~~~
  double wind_speed[mph];
~~~

TODO

### `wind_dir`
~~~
  double wind_dir[deg];
~~~

TODO

### `opq_sky_cov`
~~~
  double opq_sky_cov[pu];
~~~

TODO

### `tot_sky_cov`
~~~
  double tot_sky_cov[pu];
~~~

TODO

### `rainfall`
~~~
  double rainfall[in/h];
~~~

TODO

### `snowdepth`
~~~
  double snowdepth[in];
~~~

TODO

### `pressure`
~~~
  double pressure[mbar];
~~~

TODO

### `month`
~~~
  int16 month;
~~~

TODO

### `day`
~~~
  int16 day;
~~~

TODO

### `hour`
~~~
  int16 hour;
~~~

TODO

### `minute`
~~~
  int16 minute;
~~~

TODO

### `second`
~~~
  int16 second;
~~~

TODO

# Example

~~~
  object weather {
    temperature "0.0";
    humidity "0.0";
    solar_dir "0.0";
    solar_direct "0.0";
    solar_diff "0.0";
    solar_diffuse "0.0";
    solar_global "0.0";
    global_horizontal_extra "0.0";
    wind_speed "0.0";
    wind_dir "0.0";
    opq_sky_cov "0.0";
    tot_sky_cov "0.0";
    rainfall "0.0";
    snowdepth "0.0";
    pressure "0.0";
    month "0";
    day "0";
    hour "0";
    minute "0";
    second "0";
  }
~~~

# See also
* [[/Module/Climate]]

